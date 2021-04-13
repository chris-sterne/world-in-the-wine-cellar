// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file contains the World class implementation.  The World class
// describes a game world. 
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along
// with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <glibmm/i18n.h>
#include "World.h"

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::World::World()
{	
  m_filename.clear();
  clear();
}

//---------------------------------------------------------------------
// This method clears data in the game world, but retains the filename.
//---------------------------------------------------------------------

void Enigma::World::clear()
{	
  m_controllers.clear();
  m_objects.clear();
  m_players.clear();
  m_items.clear();
  m_teleporters.clear();
  m_description.clear();
  
  // Initialize instance variables.
  
  m_savable = false;
}

//------------------------------------------------------------
// This private function writes a KeyValue with a 16-bit value
// to a buffer.
//------------------------------------------------------------
// buffer: Destination buffer.
// key:    KeyValue key.
// value:  16-bit value.
//------------------------------------------------------------

void write_key_value_16bit(std::string& buffer,
                           Enigma::World::Key key,
                           guint16 value)
{
  buffer.push_back((guchar)key);
  buffer.push_back((guchar)value);

  // Write a bank keyvalue if the level exceeds that for a byte.
			
  if (value > G_MAXUINT8)
  {
    buffer.push_back((guchar)Enigma::World::Key::BANK);
    buffer.push_back((guchar)(value >> 8));
  }
}

//------------------------------------------------------------
// This private function writes a KeyValue with an 8-bit value
// to a buffer.
//------------------------------------------------------------
// buffer: Destination buffer.
// key:    KeyValue key.
// value:  8-bit value.
//------------------------------------------------------------

void write_key_value_8bit(std::string& buffer,
                          Enigma::World::Key key,
                          guint8 value)
{
	buffer.push_back((guchar)key);
	buffer.push_back((guchar)value);
}

//-------------------------------------------------------------
// This private function writes a KeyValue with a boolean value
// to a buffer.    
//-------------------------------------------------------------
// buffer: Destination buffer.
// key:    KeyValue key.
// value:  boolean value.
//-------------------------------------------------------------

void write_key_value_boolean(std::string& buffer,
                             Enigma::World::Key key,
                             gboolean value)
{
	buffer.push_back((guchar)key);

	if ( value)
		buffer.push_back((guchar)1);
	else
		buffer.push_back((guchar)0);
}

//------------------------------------------------------------
// This private function writes a KeyValue followed by a short
// string (255 character maximum) to a buffer.
//------------------------------------------------------------
// buffer: Destination buffer.
// key:    KeyValue key.
// string: Reference to string to be written.
//------------------------------------------------------------

void write_key_value_string(std::string& buffer,
                            Enigma::World::Key key,
                            std::string& string)
{
	// Write KeyValue if the string is not empty.

	if (string.size())
	{
		buffer.push_back((guchar)key);
		buffer.push_back((guint8)string.size());                          
		buffer.append(string);
	}
}

//-------------------------------------------------------------------
// This private function returns the number of bytes in a string line
// of a line ending with the newline '\n' or \x00 characters.          
//-------------------------------------------------------------------
// source: Source string to be searched.
// offset: Initial byte offset into the string. 
// RETURN: Line length, or zero if no line is available.             
//-------------------------------------------------------------------

guint find_line(std::string& source, guint offset)
{
	// Count the number of characters before the line terminator.

	guint index = offset;

	while ((index < source.size())
	    && (source.at(index) != '\n')
	    && (source.at(index) != '\x00'))
	{
		++ index;
	}

	// Include the line terminator in the final count, unless the end
	// of the source string has been reached.

	if (index >= source.size())
		return (index - offset);
	else
		return (1 + index - offset);
}

//---------------------------------------------------------
// This private function extracts an object from file data.
//---------------------------------------------------------
// filedata: Keyvalue stream.
// index:    KeyValue index.
// object:   Reference to Mapobject.
// savable:  Reference to save Savable state.
// RETURN:   MapObject filled with keyvalue information.
//---------------------------------------------------------

void extract_object(const std::string& filedata,
                    guint& index,
                    Enigma::Object& object,
                    bool& savable)
{
	// Clear all Object connections, but leave intact the remaining
	// information from a previous object.  This allows minimizing
	// the number of keyvalue pairs uses for describing all Objects.

	object.m_sense.resize(0);
	object.m_state.resize(0);
	object.m_visibility.resize(0);
	object.m_presence.resize(0);

	// Clear the Teleporter arrival orientation so it uses the player's
	// current orientation in the game.  The presence of Teleporter arrival
	// keysvalues will change this default orientation.

	object.m_surface_arrival  = Enigma::Object::Direction::NONE;
	object.m_rotation_arrival = Enigma::Object::Direction::NONE;

	object.m_position_arrival.m_east  = Enigma::Position::MAXIMUM;  
	object.m_position_arrival.m_north = Enigma::Position::MAXIMUM;
	object.m_position_arrival.m_above = Enigma::Position::MAXIMUM;

	// Record information from the header keyvalue, then skip over the header.
	// The calling function ensures the presence of a complete header.

	switch ((Enigma::World::Key)filedata.at(index))
	{
		case Enigma::World::Key::ITEM:
			object.m_type = Enigma::Object::Type::ITEM;
			break;

		case Enigma::World::Key::PLAYER:
			object.m_type = Enigma::Object::Type::PLAYER;
			break;

		case Enigma::World::Key::TELEPORTER:
			object.m_type = Enigma::Object::Type::TELEPORTER;
			break;

		default:
			object.m_type = Enigma::Object::Type::OBJECT;
			break;
	}

	++ index;
	object.m_id = (Enigma::Object::ID)filedata.at(index);
	++ index;

	// Record Object information from the keyvalue stream until an element
	// header or keyvalue sequence terminator is encountered.

	Enigma::World::Key group_state  = Enigma::World::Key::NONE;
	Enigma::World::Key member_state = Enigma::World::Key::NONE;
	Enigma::World::Key key;
	guint8 value;
	guint16 high_value;
	bool done = false;

	while (((filedata.size() - index) >= 2) && !done)
	{
		key = (Enigma::World::Key)filedata.at(index);
		index ++;

		value = filedata.at(index);
		index ++;

		switch(key)
		{
			case Enigma::World::Key::SURFACE:      
				// A group of Surface keyvalues are arriving.

				object.m_surface = (Enigma::Object::Direction)value;
				group_state  = key;
				member_state = key;
				break;

			case Enigma::World::Key::ROTATION:
				// A group of Rotation keyvalues are arriving.

				object.m_rotation = (Enigma::Object::Direction)value;
				group_state  = key;
				member_state = key;
				break;

			case Enigma::World::Key::EAST:
				// A group of East location keyvalues are arriving.

				object.m_position.m_east = (guint16)value;
				group_state  = key;
				member_state = key;
				break;

			case Enigma::World::Key::NORTH:
				// A group of North location keyvalues are arriving.

				object.m_position.m_north = (guint16)value;
				group_state  = key;
				member_state = key;
				break;

			case Enigma::World::Key::ABOVE:
				// A group of Above location keyvalues are arriving.

				object.m_position.m_above = (guint16)value;
				group_state  = key;
				member_state = key;
				break;

			case Enigma::World::Key::ACTIVE:
				// This keyvalue is used by Item and Player objects.

				object.m_active = (bool)value;
				break;

			case Enigma::World::Key::CATEGORY:
				// This keyvalue is used by Item objects.

				object.m_category = (Enigma::Object::Category)value;
				break;

			case Enigma::World::Key::BANK:
				// The value of a previous keyvalue is to have a high byte added.
				// Adding only one extra bank is supported, extending the value
				// to a maximum of 16 bits.

				high_value = (guint16)(value << 8);

				if (group_state == Enigma::World::Key::EAST)
				{
					if (member_state == Enigma::World::Key::EAST)
						object.m_position.m_east |= high_value;
					else if ( member_state == Enigma::World::Key::ARRIVAL)
						object.m_position_arrival.m_east |= high_value;
				}
				else if (group_state == Enigma::World::Key::NORTH)
				{
					if (member_state == Enigma::World::Key::NORTH)
						object.m_position.m_north |= high_value;
					else if (member_state == Enigma::World::Key::ARRIVAL)
						object.m_position_arrival.m_north |= high_value;
				}
				else if (group_state == Enigma::World::Key::ABOVE)
				{
					if (member_state == Enigma::World::Key::ABOVE)
						object.m_position.m_above |= high_value;
					else if (member_state == Enigma::World::Key::ARRIVAL)
						object.m_position_arrival.m_above |= high_value;
				}

				break;

			case Enigma::World::Key::ARRIVAL:
				// This is the teleport arrival value member for a group of values.
				// Select the appropriate destination.

				if (group_state == Enigma::World::Key::SURFACE)
					object.m_surface_arrival = (Enigma::Object::Direction)value;
				else if (group_state == Enigma::World::Key::ROTATION)
					object.m_rotation_arrival = (Enigma::Object::Direction)value;
				else if (group_state == Enigma::World::Key::EAST)
					object.m_position_arrival.m_east = (guint16)value;
				else if (group_state == Enigma::World::Key::NORTH)
					object.m_position_arrival.m_north = (guint16)value;
				else if (group_state == Enigma::World::Key::ABOVE)
					object.m_position_arrival.m_above = (guint16)value;

				member_state = key;
				break;

			case Enigma::World::Key::SENSE:
				// An object state signal name follows this keyvalue pair.  Copy
				// the name to a string buffer.  Value is the name's length.

				object.m_sense.assign(filedata, index, value);
				index += value;
				break;

			case Enigma::World::Key::STATE:
				// An object state signal name follows this keyvalue pair.  Copy
				// the name to a string buffer.  Value is the name's length.

				object.m_state.assign(filedata, index, value);
				index += value;
				break;

			case Enigma::World::Key::VISIBILITY:
				// An object state signal name follows this keyvalue pair.  Copy
				// the name to a string buffer.  Value is the name's length.

				object.m_visibility.assign(filedata, index, value);
				index += value;
				break;

			case Enigma::World::Key::PRESENCE:
				// An object state signal name follows this keyvalue pair.  Copy
				// the name to a string buffer.  Value is the name's length.

				object.m_presence.assign(filedata, index, value);
				index += value;
				break;

			case Enigma::World::Key::SAVED:
				// The presence of a Saved key means the game map can be saved
				// while being played.

				savable = true;
				break;

			case Enigma::World::Key::OBJECT:
			case Enigma::World::Key::TELEPORTER:
			case Enigma::World::Key::PLAYER:
			case Enigma::World::Key::ITEM:
			case Enigma::World::Key::DESCRIPTION:
			case Enigma::World::Key::CONTROLLER:
			case Enigma::World::Key::END:
				// An element keyvalue or array end keyvalue has been encountered.
				// Move the array index back to this keyvalue before exiting.

				index -= 2;
				done = true;
				break;

			default:
				break;
		}
	}
}

//-------------------------------------------------------------
// This private function extracts a Description from file data.
//-------------------------------------------------------------
// filedata: Keyvalue stream.
// index:    KeyValue index.
// object:   Reference to text buffer.
// RETURN:   Text buffer filled with description text.
//-------------------------------------------------------------

void extract_description(const std::string& filedata,
                         guint& index,
                         Glib::ustring& description)
{
	// Skip over the header keyvalue.  At present, the language value
	// is ignored.

	index += 2;

	// Record Description information from the keyvalue array until
	// an element header or keyvalue array terminator is encountered.

	Enigma::World::Key bank_state = Enigma::World::Key::NONE;
	Enigma::World::Key key;
	guint8 value;
	guint16 length = 0;
	bool done      = false;

	while (((filedata.size() - index) >= 2) && !done)
	{
		key = (Enigma::World::Key)filedata.at(index);
		index ++;

		value = filedata.at(index);
		index ++;

		switch( key)
		{
			case Enigma::World::Key::LENGTH:      
				length = (guint16)value;
				bank_state = key;
				break;

			case Enigma::World::Key::DATA:
				// The keyvalue is followed by a block of description data.

				description = filedata.substr(index, length);
				index += length;
				break;

			case Enigma::World::Key::BANK:
				// The value of a previous keyvalue is to have a high byte added.
				// Adding only one extra bank is supported, extending the value
				// to a maximum of 16 bits.

				if (bank_state == Enigma::World::Key::LENGTH)
				length |= (guint16)(value << 8);

				break;

			case Enigma::World::Key::OBJECT:
			case Enigma::World::Key::TELEPORTER:
			case Enigma::World::Key::PLAYER:
			case Enigma::World::Key::ITEM:
			case Enigma::World::Key::DESCRIPTION:
			case Enigma::World::Key::CONTROLLER:
			case Enigma::World::Key::END:
				// An element keyvalue or array end keyvalue has been encountered.
				// Move the array index back to this keyvalue before exiting.

				index -= 2;
				done = true;
				break;

			default:
				break;
		}
	}
}

//------------------------------------------------------------
// This private function extracts a Controller from file data.
//------------------------------------------------------------
// filedata: Keyvalue stream.
// index:    KeyValue index.
// object:   Reference to Controller.
// RETURN:   Controller filled with keyvalue information.
//------------------------------------------------------------

void extract_controller(const std::string& filedata,
                        guint& index,
                        Enigma::Controller& controller)
{
	// A controller name follows the new element keyvalue pair.
	// Prepare the controller to receive new information.

	++ index;
	guint8 value = filedata.at(index);
	++ index;

	controller.m_name = filedata.substr( index, value);
	controller.m_signal_names.resize(0);
	controller.m_restart_code.resize(0);
	controller.m_main_code.resize(0);

	index += value;

	// Record Controller information from the keyvalue array until
	// an element header or keyvalue array terminator is encountered.

	Enigma::World::Key bank_state = Enigma::World::Key::NONE;
	Enigma::World::Key data_state = Enigma::World::Key::NONE;
	Enigma::World::Key key;
	guint16 length = 0;
	bool done = false;

	while (((filedata.size() - index) >= 2) && !done)
	{
		key = (Enigma::World::Key)filedata.at(index);
		index ++;

		value = filedata.at(index);
		index ++;

		switch(key)
		{
			case Enigma::World::Key::CODE:
			case Enigma::World::Key::CURRENT:
			case Enigma::World::Key::SAVED:      
			case Enigma::World::Key::RESTART:
			case Enigma::World::Key::SIGNAL:
				// These keys indicates a controller bytecode block or signal
				// name block.  Some of these blocks may not be loaded, but
				// the data_state key must still be updated for use by the
				// DATA keyvalue later.

				data_state = key;
				break;

			case Enigma::World::Key::LENGTH:      
				length = (guint16)value;
				bank_state = key;
				break;

			case Enigma::World::Key::BANK:
				// The value of a previous keyvalue is to have a high byte added.
				// Adding only one extra bank is supported, extending the value
				// to a maximum of 16 bits.

				if (bank_state == Enigma::World::Key::LENGTH)
					length |= (guint16)(value << 8);

				break;

			case Enigma::World::Key::DATA:
				// The keyvalue is followed by a block of data.  Only current Code,
				// restart Code, and Signal names are loaded.

				if (data_state == Enigma::World::Key::CODE)
					controller.m_main_code.assign(filedata, index, length);
				else if (data_state == Enigma::World::Key::RESTART)
					controller.m_restart_code.assign(filedata, index, length);
				else if (data_state == Enigma::World::Key::SIGNAL)
					controller.m_signal_names.assign(filedata, index, length);

				// Move the array index over the data block.

				index += length;
				break;

			case Enigma::World::Key::OBJECT:
			case Enigma::World::Key::TELEPORTER:
			case Enigma::World::Key::PLAYER:
			case Enigma::World::Key::ITEM:
			case Enigma::World::Key::DESCRIPTION:
			case Enigma::World::Key::CONTROLLER:
			case Enigma::World::Key::END:
				// An element keyvalue or array end keyvalue has been encountered.
				// Move the array index back to this keyvalue before exiting.

				index -= 2;
				done = true;
				break;

			default:
				break;
		}
	}
}

//-------------------------------------------------------------
// This method loads a game world from a file (.ewc extension).
//-------------------------------------------------------------

void Enigma::World::load()
{	
	// Clear old data in map, but retain the filename. 

	clear();

	// Load game world file.  Return if unsuccessful.

	std::string filedata;

	try
	{
		filedata = Glib::file_get_contents(m_filename);
	}
	catch(Glib::Error error)
	{
		return;
	}

	// Confirm that the first line of the game map file has the proper
	// indentification code.

	if (!filedata.compare("ewc\n"))
		return;

	// Confirm that the file header has the correct ending.

	bool end_header = false;
	guint index = 0;
	guint size  = 0;

	do
	{
	size = find_line(filedata, index);

	if (filedata.compare( index, size, "end_header\n") == 0)
	{
		end_header = true;
		break;
	}
	else
		index += size;
	}
	while (size != 0);

	// Exit if the end of the header was not found.

	if (!end_header)
		return;

	// Skip over the header to the start of the map elements.

	index += size;

	// Initialize an Object to receive keyvalue array information.

	Enigma::Object object;

	object.m_type     = Enigma::Object::Type::OBJECT;
	object.m_id       = Enigma::Object::ID::NONE;
	object.m_surface  = Enigma::Object::Direction::NONE;
	object.m_rotation = Enigma::Object::Direction::NONE;

	object.m_position.m_east  = Enigma::Position::MINIMUM;  
	object.m_position.m_north = Enigma::Position::MINIMUM;
	object.m_position.m_above = Enigma::Position::MINIMUM;

	// Read information from the keyvalue array.

	bool valid_data = true;
	bool done       = false;
	Enigma::World::Key key;
	guint8 value;

	while (((filedata.size() - index) >= 2) && !done)
	{
		// A complete keyvalue pair is available.  Read the keyvalue,
		// but keep the keyvalue array index on the element header. 

		key   = (Enigma::World::Key)filedata.at(index);
		value = (guint8)filedata.at( index + 1);

		switch(key)
		{
			case Enigma::World::Key::OBJECT:
			case Enigma::World::Key::TELEPORTER:
			case Enigma::World::Key::ITEM:
			case Enigma::World::Key::PLAYER:
				// An Object, Teleporter, Item or Player element header has been
				// encountered.

				extract_object(filedata, index, object, m_savable);

				if  (((int)object.m_id < (int)Enigma::Object::ID::TOTAL)
					&& ((int)object.m_surface < (int)Enigma::Object::Direction::TOTAL)
					&& ((int)object.m_rotation < (int)Enigma::Object::Direction::TOTAL))
				{            
					// The MapObject has been filled with valid data.  Add a new
					// object to the appropriate list.

					if (key == Enigma::World::Key::ITEM)
						m_items.push_back(object);
					else if (key == Enigma::World::Key::PLAYER)
						m_players.push_back(object);
					else if (key == Enigma::World::Key::TELEPORTER)
						m_teleporters.push_back(object);
					else
						m_objects.push_back(object);
				}
				else
				{
					// The MapObject has some invalid data.  Force an exit with an
					// invalid data error.

					valid_data = false;
					done       = true;
				}

				break;

			case Enigma::World::Key::DESCRIPTION:
				// A Description element header has been encountered.

				extract_description(filedata, index, m_description);
				break;

			case Enigma::World::Key::CONTROLLER:
				// A Controller element header has been encountered.

				m_controllers.emplace_back();
				extract_controller(filedata, index, m_controllers.back());
				break;

			case Enigma::World::Key::END:
				// The end of the keyvalue array has been found.  Ensure it has
				// the correct value, and is last in the keyvalue array.

				if ((value != 0) || ((filedata.size() - index) != 2))
				valid_data = false;

				done = true;
				break;

			default:
				// An unrecognized element header has been encountered.  Force
				// an exit with an invalid data error.

				valid_data = false;
				done       = true;
				break;
		}
	}

	// If there was a data error, clear all saved data.  The game world file
	// may be faulty.

	if (!valid_data)    
		clear();
}

//--------------------------------------------
// This method saves the game world to a file.
//--------------------------------------------

void Enigma::World::save()
{	
	//-------------------------
	// Write a game map header.
	//-------------------------

	std::string filedata =
	"ewc\n\
format binary_byte 1.0\n\
comment Enigma in the Wine Cellar 1.0 game world\n\
comment Created by World in the Wine Cellar 1.0\n";

	filedata.append("element object ");
	filedata.append(std::to_string(m_objects.size()));
	filedata.push_back('\n');

	filedata.append("element teleporter ");
	filedata.append(std::to_string(m_teleporters.size()));
	filedata.push_back('\n');

	filedata.append("element player ");
	filedata.append(std::to_string(m_players.size()));
	filedata.push_back('\n');

	filedata.append("element item ");
	filedata.append(std::to_string(m_items.size()));
	filedata.push_back('\n');

	filedata.append("element description 1");
	filedata.push_back('\n');

	filedata.append("element controller ");
	filedata.append(std::to_string(m_controllers.size()));
	filedata.push_back('\n');

	filedata.append("end_header\n");

	//-------------------------------------
	// Write KeyValues for all Controllers.
	//-------------------------------------

	std::list<Enigma::Controller>::iterator controller;

	for (controller = m_controllers.begin();
	     controller != m_controllers.end();
	     ++ controller)
	{
		// Add header for a map controller.

		write_key_value_string(filedata,
			                     Enigma::World::Key::CONTROLLER,
			                     (*controller).m_name);

		// Write Main bytecode block.

		write_key_value_8bit(filedata,
			                   Enigma::World::Key::CODE,
			                   0);

		write_key_value_16bit(filedata,
			                    Enigma::World::Key::LENGTH,
			                    (*controller).m_main_code.size());

		write_key_value_8bit(filedata,
			                   Enigma::World::Key::DATA,
			                   0);

		filedata.append((*controller).m_main_code);

		// Write Current bytecode block (same as Restart bytecode for
		// new game maps).

		write_key_value_8bit(filedata,
			                   Enigma::World::Key::CURRENT,
			                   0);

		write_key_value_16bit(filedata,
			                    Enigma::World::Key::LENGTH,
			                    (*controller).m_restart_code.size());

		write_key_value_8bit(filedata,
			                   Enigma::World::Key::DATA,
			                   0);

		filedata.append((*controller).m_restart_code);

		// Write Saved bytecode block (same as Restart bytecode for
		// new game maps).

		if (m_savable)
		{
			write_key_value_8bit(filedata,
				                   Enigma::World::Key::SAVED,
				                   0);

			write_key_value_16bit(filedata,
				                    Enigma::World::Key::LENGTH,
				                   (*controller).m_restart_code.size());

			write_key_value_8bit(filedata,
				                   Enigma::World::Key::DATA,
				                   0);

			filedata.append((*controller).m_restart_code);
		}

		// Write Restart bytecode block.

		write_key_value_8bit(filedata,
		                     Enigma::World::Key::RESTART,
		                     0);

		write_key_value_16bit(filedata,
		                      Enigma::World::Key::LENGTH,
		                      (*controller).m_restart_code.size());

		write_key_value_8bit(filedata,
		                     Enigma::World::Key::DATA,
		                     0);

		filedata.append((*controller).m_restart_code);

		// Add header for a packed array of controller signal names.

		write_key_value_8bit(filedata,
			                   Enigma::World::Key::SIGNAL,
			                   0);

		// Add Length keyvalue.

		write_key_value_16bit(filedata,
			                    Enigma::World::Key::LENGTH,
			                    (guint16)(*controller).m_signal_names.size());

		// Write Data keyvalue, followed by the packed array of signal names.

		write_key_value_8bit(filedata,
			                   Enigma::World::Key::DATA,
			                   0);

		filedata.append((*controller).m_signal_names);
	}

	//--------------------------------------------
	// Write KeyValues for all structural objects.
	//--------------------------------------------

	// Set the initial East, North, and Above positions to their maximum,
	// forcing an addition of position keyvalues for the first structural
	// object (the first structural object cannot use the maximum value). 

	guint16 east  = Enigma::Position::MAXIMUM;
	guint16 north = Enigma::Position::MAXIMUM;  	
	guint16 above = Enigma::Position::MAXIMUM; 

	std::list<Enigma::Object>::iterator object;

	for (object = m_objects.begin();
	     object != m_objects.end();
	     ++ object)
	{
		// Add the header for a simple object.

		write_key_value_8bit(filedata,
		                     Enigma::World::Key::OBJECT,
		                     (guint8)(*object).m_id);

		// Add a new East keyvalue if an object with a different position
		// has been encountered.

		if ((*object).m_position.m_east != east)
		{			
			east = (*object).m_position.m_east;

			write_key_value_16bit(filedata,
			                      Enigma::World::Key::EAST,
			                      east);
		}

		// Add a new North keyvalue if an object with a different position
		// has been encountered.

		if ((*object).m_position.m_north != north)
		{
			north = (*object).m_position.m_north;

			write_key_value_16bit(filedata,
			                      Enigma::World::Key::NORTH,
			                      north);
		}

		// Add a new Above keyvalue if an object with a different position
		// has been encountered.

		if ((*object).m_position.m_above != above)
		{
			above = (*object).m_position.m_above;

			write_key_value_16bit(filedata,
			                      Enigma::World::Key::ABOVE,
			                      above);
		}

		// Add a surface keyvalue.

		write_key_value_8bit(filedata,
		                     Enigma::World::Key::SURFACE,
		                     (guint8)(*object).m_surface);

		// Add a rotation keyvalue.

		write_key_value_8bit(filedata,
		                     Enigma::World::Key::ROTATION,
		                     (guint8)(*object).m_rotation);

		// Add object state or signal keyvalues.

		write_key_value_string(filedata,
		                       Enigma::World::Key::SENSE,
		                       (*object).m_sense);

		write_key_value_string(filedata,
		                       Enigma::World::Key::STATE,
		                       (*object).m_state);

		write_key_value_string(filedata,
		                       Enigma::World::Key::VISIBILITY,
		                       (*object).m_visibility);

		write_key_value_string(filedata,
		                       Enigma::World::Key::PRESENCE,
		                       (*object).m_presence);
	}

	//---------------------------------------------*
	// Write KeyValues for all teleporter objects. *
	//---------------------------------------------*

	for (object = m_teleporters.begin();
	     object != m_teleporters.end();
	     ++ object)
	{
		// Add header for a teleporter object.

		write_key_value_8bit(filedata,
			                   Enigma::World::Key::TELEPORTER,
			                   (guint8)(*object).m_id);

		// Write departure surface keyvalue, and add arrival surface
		// if it is not to be the player's current surface.

		write_key_value_8bit(filedata,
			                   Enigma::World::Key::SURFACE,
			                   (guint8)(*object).m_surface);

		if ((*object).m_surface_arrival != Enigma::Object::Direction::NONE)
		{
			write_key_value_8bit(filedata,
				                   Enigma::World::Key::ARRIVAL,
				                   (guint8)(*object).m_surface_arrival);
		}

		// Write departure rotation keyvalue, and add arrival rotation
		// if it is not to be the player's current rotation.

		write_key_value_8bit(filedata,
			                   Enigma::World::Key::ROTATION,
			                   (guint8)(*object).m_rotation);

		if ((*object).m_rotation_arrival != Enigma::Object::Direction::NONE)
		{
			write_key_value_8bit(filedata,
				                   Enigma::World::Key::ARRIVAL,
				                   (guint8)(*object).m_rotation_arrival);
		}

		// Write departure East location keyvalue, and add arrival East location
		// if it is not to be the player's current East location.

		write_key_value_16bit(filedata,
			                    Enigma::World::Key::EAST,
			                    (*object).m_position.m_east);

		if ((*object).m_position_arrival.m_east != Enigma::Position::MAXIMUM)
		{
			write_key_value_16bit(filedata,
				                    Enigma::World::Key::ARRIVAL,
				                    (*object).m_position_arrival.m_east);
		}

		// Write departure North location keyvalue, and add arrival North location
		// if it is not to be the player's current North location.

		write_key_value_16bit(filedata,
			                    Enigma::World::Key::NORTH,
			                    (*object).m_position.m_north);

		if ((*object).m_position_arrival.m_north != Enigma::Position::MAXIMUM)
		{
			write_key_value_16bit(filedata,
					                  Enigma::World::Key::ARRIVAL,
					                  (*object).m_position_arrival.m_north);
		}

		// Write departure Above location keyvalue, adding arrival Above location
		// if it is not to be the player's current Above location.

		write_key_value_16bit(filedata,
			                    Enigma::World::Key::ABOVE,
			                    (*object).m_position.m_above);

		if ((*object).m_position_arrival.m_above != Enigma::Position::MAXIMUM)
		{           
			write_key_value_16bit(filedata,
				                    Enigma::World::Key::ARRIVAL,
				                    (*object).m_position_arrival.m_above);
		}

			// Add object state or signal keyvalues.

			write_key_value_string(filedata,
				                     Enigma::World::Key::SENSE,
				                     (*object).m_sense);

			write_key_value_string(filedata,
				                     Enigma::World::Key::STATE,
				                     (*object).m_state);

			write_key_value_string(filedata,
				                     Enigma::World::Key::VISIBILITY,
				                     (*object).m_visibility);

			write_key_value_string(filedata,
				                     Enigma::World::Key::PRESENCE,
				                     (*object).m_presence);
	}

	//-----------------------------------------*
	// Write KeyValues for all player objects. *
	//-----------------------------------------*

	for (object = m_players.begin();
	     object != m_players.end();
	     ++ object)
	{
		// Add header for a player object.

		write_key_value_8bit(filedata,
		                     Enigma::World::Key::PLAYER,
		                     (guint8)(*object).m_id);

		// Write current, saved, and restart surface keyvalues.
		// All have the same value.

		write_key_value_8bit(filedata,
		                     Enigma::World::Key::SURFACE,
		                     (guint8)(*object).m_surface);

		if (m_savable)
		{
			write_key_value_8bit(filedata,
			                     Enigma::World::Key::SAVED,
			                     (guint8)(*object).m_surface);
		}

		write_key_value_8bit(filedata,
		                     Enigma::World::Key::RESTART,
		                     (guint8)(*object).m_surface);

		// Write current, saved, and restart rotation keyvalues.
		// All have the same value.

		write_key_value_8bit(filedata,
		                     Enigma::World::Key::ROTATION,
		                     (guint8)(*object).m_rotation);

		if (m_savable)
		{
			write_key_value_8bit(filedata,
			                     Enigma::World::Key::SAVED,
			                     (guint8)(*object).m_rotation);
		}

		write_key_value_8bit(filedata,
		                     Enigma::World::Key::RESTART,
		                     (guint8)(*object).m_rotation);

		// Write current, saved, and restart East location keyvalues.
		// All have the same value.

		write_key_value_16bit(filedata,
		                      Enigma::World::Key::EAST,
		                      (*object).m_position.m_east);

		if (m_savable)
		{
			write_key_value_16bit(filedata,
			                      Enigma::World::Key::SAVED,
			                      (*object).m_position.m_east);
		} 

		write_key_value_16bit(filedata,
		                      Enigma::World::Key::RESTART,
		                      (*object).m_position.m_east);

		// Write current, saved, and restart North location keyvalues.
		// All have the same value.

		write_key_value_16bit(filedata,
		                      Enigma::World::Key::NORTH,
		                      (*object).m_position.m_north);

		if (m_savable)
		{
			write_key_value_16bit(filedata,
			                      Enigma::World::Key::SAVED,
			                      (*object).m_position.m_north);
		}

		write_key_value_16bit(filedata,
		                      Enigma::World::Key::RESTART,
		                      (*object).m_position.m_north);

		// Write current, saved, and restart Above location keyvalues.
		// All have the same value.

		write_key_value_16bit(filedata,
		                      Enigma::World::Key::ABOVE,
		                      (*object).m_position.m_above);

		if (m_savable)
		{
			write_key_value_16bit(filedata,
			                      Enigma::World::Key::SAVED,
			                      (*object).m_position.m_above);
		}

		write_key_value_16bit(filedata,
		                      Enigma::World::Key::RESTART,
		                      (*object).m_position.m_above);

		// Add current, saved, and restart Active state keyvalues.
		// All have the same value.

		write_key_value_boolean(filedata,
			                      Enigma::World::Key::ACTIVE,
			                      (*object).m_active);

		if (m_savable)
		{
			write_key_value_boolean(filedata,
				                      Enigma::World::Key::SAVED,
				                      (*object).m_active);
		}

		write_key_value_boolean(filedata,
		                        Enigma::World::Key::RESTART,
		                        (*object).m_active);

		// Add object state or signal keyvalues.  Player objects
		// do not have a Sense state or signal.

		write_key_value_string(filedata,
		                       Enigma::World::Key::STATE,
		                       (*object).m_state);

		write_key_value_string(filedata,
		                       Enigma::World::Key::VISIBILITY,
		                       (*object).m_visibility);

		write_key_value_string(filedata,
		                       Enigma::World::Key::PRESENCE,
		                       (*object).m_presence);
	}

	//---------------------------------------*
	// Write KeyValues for all item objects. *
	//---------------------------------------*

	for (object = m_items.begin();
	     object != m_items.end();
	     ++ object)
	{
		// Add header for an item object.

		write_key_value_8bit(filedata,
		                     Enigma::World::Key::ITEM,
		                     (guint8)(*object).m_id);

		// Write surface.

		write_key_value_8bit(filedata,
		                      Enigma::World::Key::SURFACE,
		                      (guint8)(*object).m_surface);

		// Write rotation.

		write_key_value_8bit(filedata,
		                     Enigma::World::Key::ROTATION,
		                     (guint8)(*object).m_rotation);

		// Write location.

		write_key_value_16bit(filedata,
		                      Enigma::World::Key::EAST,
		                      (*object).m_position.m_east);

		write_key_value_16bit(filedata,
		                      Enigma::World::Key::NORTH,
		                      (*object).m_position.m_north);

		write_key_value_16bit(filedata,
		                      Enigma::World::Key::ABOVE,
		                      (*object).m_position.m_above);

		// Write category.

		write_key_value_8bit(filedata,
		                     Enigma::World::Key::CATEGORY,
		                     (guint8)(*object).m_category);

		// Add current, saved, and restart active state keyvalues.
		// All items begin as active (not yet found).

		write_key_value_boolean(filedata,
		                        Enigma::World::Key::ACTIVE,
		                        true);

		if (m_savable)
		{
			write_key_value_boolean(filedata,
			                        Enigma::World::Key::SAVED,
			                        true);
		}

		write_key_value_boolean(filedata,
		                        Enigma::World::Key::RESTART,
		                        true);

		// Add object state or signal keyvalues.  Item objects
		// do not have a Sense state or signal.

		write_key_value_string(filedata,
		                       Enigma::World::Key::STATE,
		                       (*object).m_state);

		write_key_value_string(filedata,
		                       Enigma::World::Key::VISIBILITY,
		                       (*object).m_visibility);

		write_key_value_string(filedata,
		                       Enigma::World::Key::PRESENCE,
		                       (*object).m_presence);
	}

	//-----------------------------------------
	// Write KeyValues for the map description.
	//-----------------------------------------

	// Add header for a description.

	write_key_value_8bit(filedata,
	                     Enigma::World::Key::DESCRIPTION,
	                     (guint8)Enigma::World::Language::ENGLISH);

	// Add description length keyvalue.

	write_key_value_16bit(filedata,
	                      Enigma::World::Key::LENGTH,
	                      (guint16)m_description.bytes());

	// Write data keyvalue, followed by description in UTF-8 format.

	write_key_value_8bit(filedata,
	                     Enigma::World::Key::DATA,
	                     0);

	filedata.append(m_description);

	//---------------------------------
	// Write KeyValue array terminator.
	//---------------------------------

	write_key_value_8bit(filedata,
	                     Enigma::World::Key::END,
	                     0);

	// Write the file data to the file.

	try
	{
		Glib::file_set_contents(m_filename,
		filedata.data(),
		filedata.size());
	}
	catch(Glib::Error error)
	{
		return;
	}
}
