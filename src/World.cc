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

void write_keyvalue_16bit(std::string& buffer,
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

void write_keyvalue_8bit(std::string& buffer,
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

void write_keyvalue_boolean(std::string& buffer,
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

void write_keyvalue_string(std::string& buffer,
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
// afiledata: Keyvalue stream.
// aIndex:    KeyValue index.
// aObject:   Reference to MapObject.
// aMap:      Reference to save map Savable state.
// RETURN:    MapObject filled with keyvalue information.
//---------------------------------------------------------
/*
void ExtractNewObject( const std::string& afiledata,
                       guint& aIndex,
                       CMapObject& aObject,
                       gboolean& aSavable )
{
  // Clear all Object connections, but leave intact the remaining
  // information from a previous Object.  This allows minimizing
  // the number of keyvalue pairs uses for describing all Objects.
  
  aObject.iSense.resize(0);
  aObject.iState.resize(0);
  aObject.iVisibility.resize(0);
  aObject.iPresence.resize(0);

  // Clear the Teleporter arrival orientation so it uses the player's
  // current orientation in the game.  The presence of Teleporter arrival
  // keysvalues will change this default orientation.
  
  aObject.iSurfaceArrival  = EnigmaWC::Direction::ENone;
  aObject.iRotationArrival = EnigmaWC::Direction::ENone;

  aObject.iLocationArrival.iEast  = G_MAXUINT16;  
  aObject.iLocationArrival.iNorth = G_MAXUINT16;
  aObject.iLocationArrival.iAbove = G_MAXUINT16;
  
  // Record information from the header keyvalue, then skip over the header.
  // The calling function ensures the presence of a complete header.

  switch((Enigma::World::Key)afiledata.at( aIndex ) )
  {
    case Enigma::World::Key::EItem:
      aObject.iType = CMapObject::Type::EItem;
      break;
      
    case Enigma::World::Key::EPlayer:
      aObject.iType = CMapObject::Type::EPlayer;
      break;
  
    case Enigma::World::Key::ETeleporter:
      aObject.iType = CMapObject::Type::ETeleporter;
      break;
  
    default:
      aObject.iType = CMapObject::Type::EObject;
      break;
  }

  ++ aIndex;
  aObject.iID = (EnigmaWC::ID)afiledata.at( aIndex );
  ++ aIndex;
  
  // Record Object information from the keyvalue stream until an element
  // header or keyvalue sequence terminator is encountered.
 
  Enigma::World::Key GroupState  = Enigma::World::Key::ENone;
  Enigma::World::Key MemberState = Enigma::World::Key::ENone;
  Enigma::World::Key Key;
  guint8 Value;
  guint16 HighValue;
  gboolean Done = FALSE;
  
  while ((( afiledata.size() - aIndex ) >= 2 ) && !Done )
  {
    Key = (Enigma::World::Key)afiledata.at( aIndex );
    aIndex ++;

    Value = afiledata.at( aIndex );
    aIndex ++;

    switch( key)
    {
      case Enigma::World::Key::ESurface:      
        // A group of Surface keyvalues are arriving.
        
        aObject.iSurface = (EnigmaWC::Direction)Value;
        GroupState  = Key;
        MemberState = Key;
        break;
      
      case Enigma::World::Key::ERotation:
        // A group of Rotation keyvalues are arriving.
        
        aObject.iRotation = (EnigmaWC::Direction)Value;
        GroupState  = Key;
        MemberState = Key;
        break;

      case Enigma::World::Key::EEast:
        // A group of East location keyvalues are arriving.
                
        aObject.iLocation.iEast = (guint16)Value;
        GroupState  = Key;
        MemberState = Key;
        break;

      case Enigma::World::Key::ENorth:
        // A group of North location keyvalues are arriving.
      
        aObject.iLocation.iNorth = (guint16)Value;
        GroupState  = Key;
        MemberState = Key;
        break;
      
      case Enigma::World::Key::EAbove:
        // A group of Above location keyvalues are arriving.

        aObject.iLocation.iAbove = (guint16)Value;
        GroupState  = Key;
        MemberState = Key;
        break;
      
      case Enigma::World::Key::EActive:
        // This keyvalue is used by Item and Player objects.
        
        aObject.iActive = (gboolean)Value;
        break;
        
      case Enigma::World::Key::ECategory:
        // This keyvalue is used by Item objects.
                
        aObject.iCategory = (EnigmaWC::Category)Value;
        break;

      case Enigma::World::Key::EBank:
        // The value of a previous keyvalue is to have a high byte added.
        // Adding only one extra bank is supported, extending the value
        // to a maximum of 16 bits.
                
        HighValue = (guint16)( Value << 8 );

        if ( GroupState == Enigma::World::Key::EEast )
        {
          if ( MemberState == Enigma::World::Key::EEast )
            aObject.iLocation.iEast |= HighValue;
          else if ( MemberState == Enigma::World::Key::EArrival )
            aObject.iLocationArrival.iEast |= HighValue;
        }
        else if ( GroupState == Enigma::World::Key::ENorth )
        {
          if ( MemberState == Enigma::World::Key::ENorth )
            aObject.iLocation.iNorth |= HighValue;
          else if ( MemberState == Enigma::World::Key::EArrival )
            aObject.iLocationArrival.iNorth |= HighValue;
        }
        else if ( GroupState == Enigma::World::Key::EAbove )
        {
          if ( MemberState == Enigma::World::Key::EAbove )
            aObject.iLocation.iAbove |= HighValue;
          else if ( MemberState == Enigma::World::Key::EArrival )
            aObject.iLocationArrival.iAbove |= HighValue;
        }
                   
        break;

      case Enigma::World::Key::EArrival:
        // This is the teleport arrival value member for a group of values.
        // Select the appropriate destination.
        
        if ( GroupState == Enigma::World::Key::ESurface )
          aObject.iSurfaceArrival = (EnigmaWC::Direction)Value;
        else if ( GroupState == Enigma::World::Key::ERotation )
          aObject.iRotationArrival = (EnigmaWC::Direction)Value;
        else if ( GroupState == Enigma::World::Key::EEast )
          aObject.iLocationArrival.iEast = (guint16)Value;
        else if ( GroupState == Enigma::World::Key::ENorth )
          aObject.iLocationArrival.iNorth = (guint16)Value;
        else if ( GroupState == Enigma::World::Key::EAbove )
          aObject.iLocationArrival.iAbove = (guint16)Value;
  
        MemberState = Key;
        break;

      case Enigma::World::Key::ESense:
        // An object state signal name follows this keyvalue pair.  Copy
        // the name to a string buffer.  Value is the name's length.

        aObject.iSense.assign( afiledata, aIndex, value);
        aIndex += Value;
        break;
        
      case Enigma::World::Key::EState:
        // An object state signal name follows this keyvalue pair.  Copy
        // the name to a string buffer.  Value is the name's length.

        aObject.iState.assign( afiledata, aIndex, value);
        aIndex += Value;
        break;
        
      case Enigma::World::Key::EVisibility:
        // An object state signal name follows this keyvalue pair.  Copy
        // the name to a string buffer.  Value is the name's length.

        aObject.iVisibility.assign( afiledata, aIndex, value);
        aIndex += Value;
        break;
        
      case Enigma::World::Key::EPresence:
        // An object state signal name follows this keyvalue pair.  Copy
        // the name to a string buffer.  Value is the name's length.

        aObject.iPresence.assign( afiledata, aIndex, value);
        aIndex += Value;
        break;
      
      case Enigma::World::Key::ESaved:
        // The presence of a Saved key means the game map can be saved
        // while being played.
        
        aSavable = TRUE;
        break;
      
      case Enigma::World::Key::EObject:
      case Enigma::World::Key::ETeleporter:
      case Enigma::World::Key::EPlayer:
      case Enigma::World::Key::EItem:
      case Enigma::World::Key::EDescription:
      case Enigma::World::Key::EController:
      case Enigma::World::Key::EEnd:
        // An element keyvalue or array end keyvalue has been encountered.
        // Move the array index back to this keyvalue before exiting.
			
        aIndex -= 2;
        Done = TRUE;
      break;
       
      default:
        break;
    }
  }
  
  return;
}

//---------------------------------------------------------------*
// This private function extracts a Description from new version *
// game map data.                                                *
//---------------------------------------------------------------*
// afiledata: Keyvalue stream.                                   *
// aIndex:    KeyValue index.                                    *
// aObject:   Reference to text buffer.                          *
// RETURN:    Text buffer filled with description text.          *
//---------------------------------------------------------------*

void ExtractNewDescription( const std::string& afiledata,
                            guint& aIndex,
                            Glib::ustring& aDescription )
{
  // Skip over the header keyvalue.  At present, the language value
  // is ignored.
  
  aIndex += 2;
  
  // Record Description information from the keyvalue array until
  // an element header or keyvalue array terminator is encountered.
 
  Enigma::World::Key BankState = Enigma::World::Key::ENone;
  Enigma::World::Key Key;
  guint8 Value;
  guint16 Length = 0;
  gboolean Done  = FALSE;
  
  while ((( afiledata.size() - aIndex ) >= 2 ) && !Done )
  {
    Key = (Enigma::World::Key)afiledata.at( aIndex );
    aIndex ++;

    Value = afiledata.at( aIndex );
    aIndex ++;

    switch( key)
    {
      case Enigma::World::Key::ELength:      
        Length = (guint16)Value;
        BankState = Key;
        break;
    
      case Enigma::World::Key::EData:
        // The keyvalue is followed by a block of description data.
        
        aDescription = afiledata.substr( aIndex, Length );
        aIndex += Length;
        break;
        
      case Enigma::World::Key::EBank:
        // The value of a previous keyvalue is to have a high byte added.
        // Adding only one extra bank is supported, extending the value
        // to a maximum of 16 bits.

        if ( BankState == Enigma::World::Key::ELength )
          Length |= (guint16)( Value << 8 );
                    
        break;
        
      case Enigma::World::Key::EObject:
      case Enigma::World::Key::ETeleporter:
      case Enigma::World::Key::EPlayer:
      case Enigma::World::Key::EItem:
      case Enigma::World::Key::EDescription:
      case Enigma::World::Key::EController:
      case Enigma::World::Key::EEnd:
        // An element keyvalue or array end keyvalue has been encountered.
        // Move the array index back to this keyvalue before exiting.
			
        aIndex -= 2;
        Done = TRUE;
        break;
        
      default:
        break;
    }
  }
  
  return;
}

//--------------------------------------------------------------*
// This private function extracts a Controller from new version *
// game map data.                                               *
//--------------------------------------------------------------*
// afiledata: Keyvalue stream.                                  *
// aIndex:    KeyValue index.                                   *
// aObject:   Reference to MapController.                       *
// RETURN:    MapController filled with keyvalue information.   *
//--------------------------------------------------------------*

void ExtractNewController( const std::string& afiledata,
                           guint& aIndex,
                           CMapController& aController )
{
  // A controller name follows the new element keyvalue pair.
  // Prepare the controller to receive new information.
 
  ++ aIndex;
  guint8 Value = afiledata.at( aIndex );
  ++ aIndex;
  
  aController.iName = afiledata.substr( aIndex, value);
  aController.iSignalNames.resize(0);
  aController.iRestartCode.resize(0);
  aController.iMainCode.resize(0);
  
  aIndex += Value;
  
  // Record Controller information from the keyvalue array until
  // an element header or keyvalue array terminator is encountered.
 
  Enigma::World::Key BankState = Enigma::World::Key::ENone;
  Enigma::World::Key DataState = Enigma::World::Key::ENone;
  Enigma::World::Key Key;
  guint16 Length = 0;
  gboolean Done  = FALSE;
  
  while ((( afiledata.size() - aIndex ) >= 2 ) && !Done )
  {
    Key = (Enigma::World::Key)afiledata.at( aIndex );
    aIndex ++;

    Value = afiledata.at( aIndex );
    aIndex ++;

    switch( key)
    {
      case Enigma::World::Key::ECode:
      case Enigma::World::Key::ECurrent:
      case Enigma::World::Key::ESaved:      
      case Enigma::World::Key::ERestart:
      case Enigma::World::Key::ESignal:
        // These keys indicates a controller bytecode block or signal
        // name block.  Some of these blocks not be loaded, but the
        // DataState key must still be updated for use by the EData
        // keyvalue later.
      
        DataState = Key;
        break;
    
      case Enigma::World::Key::ELength:      
        Length = (guint16)Value;
        BankState = Key;
        break;
        
      case Enigma::World::Key::EBank:
        // The value of a previous keyvalue is to have a high byte added.
        // Adding only one extra bank is supported, extending the value
        // to a maximum of 16 bits.

        if ( BankState == Enigma::World::Key::ELength )
          Length |= (guint16)( Value << 8 );
                    
        break;
    
      case Enigma::World::Key::EData:
        // The keyvalue is followed by a block of data.  Only current Code,
        // restart Code, and Signal names are loaded.

        if ( DataState == Enigma::World::Key::ECode )
          aController.iMainCode.assign( afiledata, aIndex, Length );
        else if ( DataState == Enigma::World::Key::ERestart )
          aController.iRestartCode.assign( afiledata, aIndex, Length );
        else if ( DataState == Enigma::World::Key::ESignal )
          aController.iSignalNames.assign( afiledata, aIndex, Length );
        
        // Move the array index over the data block.
        
        aIndex += Length;
        break;
    
      case Enigma::World::Key::EObject:
      case Enigma::World::Key::ETeleporter:
      case Enigma::World::Key::EPlayer:
      case Enigma::World::Key::EItem:
      case Enigma::World::Key::EDescription:
      case Enigma::World::Key::EController:
      case Enigma::World::Key::EEnd:
        // An element keyvalue or array end keyvalue has been encountered.
        // Move the array index back to this keyvalue before exiting.
			
        aIndex -= 2;
        Done = TRUE;
        break;
        
      default:
        break;
    }
  }
}
*/
//-------------------------------------------------------------
// This method loads a game world from a file (.ewc extension).
//-------------------------------------------------------------

void Enigma::World::load()
{	
	// Clear old data in map, but retain the filename. 

	//Clear();

	// Load game map file.  Return if unsuccessful.

	/*std::string filedata;

	try
	{
	filedata = Glib::file_get_contents(m_filename);
	}
	catch(Glib::Error error)
	{
	return;
	}

	ExtractWorld( filedata );

	// Choose the appropriate extraction method.

	if ( iFileName.find( ".ewc" ) != Glib::ustring::npos )
	ExtractNewMap( filedata );
	else if ( iFileName.find( ".pdb" ) != Glib::ustring::npos )
	ExtractOldMap( filedata );*/
}
/*
//------------------------------------------------------------------------*
// This method extracts information from new format game maps.  A loaded  *
// group of Saved, Current, and Restart values will have only the Restart *
// values recorded, so all group values will be the same as the Restart   *
// value when saved to a game map file.                                   *
//------------------------------------------------------------------------*
// afiledata: Raw game map file data.                                     *
//------------------------------------------------------------------------*

void CMap::ExtractNewMap( std::string& afiledata )
{ 
  // Confirm that the first line of the game map file has the proper
  // indentification code.

  if ( !afiledata.compare( "ewc\n" ) )
    return;

  // Confirm that the file header has the correct ending.
	
  gboolean EndHeader = FALSE;
  guint Index = 0;
  guint Size  = 0;
  
  do
  {
    Size = find_line( afiledata, Index );
    
    if ( afiledata.compare( Index, Size, "end_header\n" ) == 0 )
    {
      EndHeader = TRUE;
      break;
    }
    else
      Index += Size;
  }
  while ( Size != 0 );

  // Exit if the end of the header was not found.

  if ( !EndHeader )
    return;

  // Skip over the header to the start of the map elements.
  
  Index += Size;

  // Initialize a MapObject to receive keyvalue array information.

  CMapObject Object;
  
  Object.iType     = CMapObject::Type::EObject;
  Object.iID       = EnigmaWC::ID::ENone;
  Object.iSurface  = EnigmaWC::Direction::ENone;
  Object.iRotation = EnigmaWC::Direction::ENone;

  Object.iLocation.iEast  = 0;  
  Object.iLocation.iNorth = 0;
  Object.iLocation.iAbove = 0;

  // Read information from the keyvalue array.
  
  gboolean ValidData = TRUE;
  gboolean Done      = FALSE;
  Enigma::World::Key Key;
  guint8 Value;
  
  while ((( afiledata.size() - Index ) >= 2 ) && !Done )
  {
    // A complete keyvalue pair is available.  Read the keyvalue,
    // but keep the keyvalue array index on the element header. 
  
    Key   = (Enigma::World::Key)afiledata.at( Index );
    Value = (guint8)afiledata.at( Index + 1 );
    
    switch( key)
    {
      case Enigma::World::Key::EObject:
      case Enigma::World::Key::ETeleporter:
      case Enigma::World::Key::EItem:
      case Enigma::World::Key::EPlayer:

        // An Object, Teleporter, Item or Player element header has been
        // encountered.
        
        ExtractNewObject( afiledata, Index, Object, iSavable );
        
        if  (((int)Object.iID < (int)EnigmaWC::ID::TOTAL )
          && ((int)Object.iSurface < (int)EnigmaWC::Direction::TOTAL )
          && ((int)Object.iRotation < (int)EnigmaWC::Direction::TOTAL ))
        {            
          // The MapObject has been filled with valid data.  Add a new
          // object to the appropriate list.
          
          if ( Key == Enigma::World::Key::EItem )
            iItems.push_back( Object );
          else if ( Key == Enigma::World::Key::EPlayer )
            iPlayers.push_back( Object );
          else if ( Key == Enigma::World::Key::ETeleporter )
            iTeleporters.push_back( Object );
          else
            iObjects.push_back( Object );
        }
        else
        {
          // The MapObject has some invalid data.  Force an exit with an
          // invalid data error.
        
          ValidData = FALSE;
          Done      = TRUE;
        }
          
        break;
			
			case Enigma::World::Key::EDescription:
        // A Description element header has been encountered.
        
        ExtractNewDescription( afiledata, Index, iDescription );
        break;
        
      case Enigma::World::Key::EController:
        // A Controller element header has been encountered.
        
        iControllers.emplace_back();
        ExtractNewController( afiledata, Index, iControllers.back() );
        break;
			
			case Enigma::World::Key::EEnd:
			  // The end of the keyvalue array has been found.  Ensure it has
			  // the correct value, and is last in the keyvalue array.
            
        if (( Value != 0 ) || (( afiledata.size() - Index ) != 2 ))
          ValidData = FALSE;
			  
			  Done = TRUE;
			  break;
			
      default:
        // An unrecognized element header has been encountered.  Force
        // an exit with an invalid data error.
        
        ValidData = FALSE;
        Done      = TRUE;
        break;
    }
  }

  // If there was a data error, clear all saved data.  The game map file
  // may be faulty.

  if ( !ValidData )    
    Clear();

  return;
}
*/
//------------------------------------------
// This method saves the game map to a file.
//------------------------------------------

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
/*
  //------------------------------------------*
  // Write KeyValues for all map controllers. *
  //------------------------------------------*
  
  std::list<CMapController>::iterator Controller;
  
  for ( Controller = iControllers.begin();
        Controller != iControllers.end();
        ++ Controller )
	{
		// Add header for a map controller.
		
		write_key_value_string( filedata,
                          Enigma::World::Key::EController,
                          (*Controller).iName );
     
    // Write Main bytecode block.
    
    write_key_value_8bit( filedata,
                        Enigma::World::Key::ECode,
                        0 );
    
    write_key_value_16bit( filedata,
                         Enigma::World::Key::ELength,
                         (*Controller).iMainCode.size() );
    
    write_key_value_8bit( filedata,
                        Enigma::World::Key::EData,
                        0 );
                        
    filedata.append((*Controller).iMainCode );
    
    // Write Current bytecode block (same as Restart bytecode for
    // new game maps).
    
    write_key_value_8bit( filedata,
                        Enigma::World::Key::ECurrent,
                        0 );
    
    write_key_value_16bit( filedata,
                         Enigma::World::Key::ELength,
                         (*Controller).iRestartCode.size() );
    
    write_key_value_8bit( filedata,
                        Enigma::World::Key::EData,
                        0 );
                        
    filedata.append((*Controller).iRestartCode );
        
    // Write Saved bytecode block (same as Restart bytecode for
    // new game maps).
    
    if ( iSavable )
    {
      write_key_value_8bit( filedata,
                          Enigma::World::Key::ESaved,
                          0 );
    
      write_key_value_16bit( filedata,
                           Enigma::World::Key::ELength,
                           (*Controller).iRestartCode.size() );
    
      write_key_value_8bit( filedata,
                          Enigma::World::Key::EData,
                          0 );
                        
      filedata.append((*Controller).iRestartCode );
    }
    
    // Write Restart bytecode block.
    
    write_key_value_8bit( filedata,
                        Enigma::World::Key::ERestart,
                        0 );
    
    write_key_value_16bit( filedata,
                         Enigma::World::Key::ELength,
                         (*Controller).iRestartCode.size() );
    
    write_key_value_8bit( filedata,
                        Enigma::World::Key::EData,
                        0 );
                        
    filedata.append((*Controller).iRestartCode );

    // Add header for a packed array of controller signal names.

    write_key_value_8bit( filedata,
                        Enigma::World::Key::ESignal,
                        0 );

    // Add Length keyvalue.

    write_key_value_16bit( filedata,
                         Enigma::World::Key::ELength,
                         (guint16)(*Controller).iSignalNames.size() );

    // Write Data keyvalue, followed by the packed array of signal names.
  
    write_key_value_8bit( filedata,
                        Enigma::World::Key::EData,
                        0 );

    filedata.append((*Controller).iSignalNames );
  }

  //---------------------------------------------*
  // Write KeyValues for all structural objects. *
  //---------------------------------------------*

	// Set the initial East, North, and Above locations to their maximum,
	// forcing an addition of location keyvalues for the first structural
	// object (the first structural object cannot use the maximum value). 

  guint16 East  = G_MAXUINT16;
  guint16 North = G_MAXUINT16;  	
  guint16 Above = G_MAXUINT16; 

  std::list<CMapObject>::iterator Object;

  for ( Object = iObjects.begin();
        Object != iObjects.end();
        ++ Object )
  {
    // Add the header for a simple object.
	
    write_key_value_8bit( filedata,
                        Enigma::World::Key::EObject,
                        (guint8)(*Object).iID );

    // Add a new East keyvalue if an object with a different location
    // has been encountered.
		
    if ((*Object).iLocation.iEast != East )
    {			
      East = (*Object).iLocation.iEast;

      write_key_value_16bit( filedata,
                           Enigma::World::Key::EEast,
                           East );
    }

    // Add a new North keyvalue if an object with a different location
    // has been encountered.
		
    if ((*Object).iLocation.iNorth != North )
    {
      North = (*Object).iLocation.iNorth;
      
      write_key_value_16bit( filedata,
                           Enigma::World::Key::ENorth,
                           North );
    }

    // Add a new Above keyvalue if an object with a different location
    // has been encountered.
		
    if ((*Object).iLocation.iAbove != Above )
    {
      Above = (*Object).iLocation.iAbove;

      write_key_value_16bit( filedata,
                           Enigma::World::Key::EAbove,
                           Above );
    }

    // Add a surface keyvalue.

    write_key_value_8bit( filedata,
                        Enigma::World::Key::ESurface,
                        (guint8)(*Object).iSurface );

    // Add a rotation keyvalue.

    write_key_value_8bit( filedata,
                        Enigma::World::Key::ERotation,
                        (guint8)(*Object).iRotation );
                        
    // Add object state or signal keyvalues.
        
    write_key_value_string( filedata,
                          Enigma::World::Key::ESense,
                          (*Object).iSense );
    
    write_key_value_string( filedata,
                          Enigma::World::Key::EState,
                          (*Object).iState );
    
    write_key_value_string( filedata,
                          Enigma::World::Key::EVisibility,
                          (*Object).iVisibility );
    
    write_key_value_string( filedata,
                          Enigma::World::Key::EPresence,
                          (*Object).iPresence );
  }

  //---------------------------------------------*
  // Write KeyValues for all teleporter objects. *
  //---------------------------------------------*

  for ( Object = iTeleporters.begin();
        Object != iTeleporters.end();
        ++ Object )
	{
		// Add header for a teleporter object.
		
		write_key_value_8bit( filedata,
                        Enigma::World::Key::ETeleporter,
                        (guint8)(*Object).iID );
  
    // Write departure surface keyvalue, and add arrival surface
    // if it is not to be the player's current surface.
    
    write_key_value_8bit( filedata,
                        Enigma::World::Key::ESurface,
                        (guint8)(*Object).iSurface );
  
    if ((*Object).iSurfaceArrival != EnigmaWC::Direction::ENone )
    {
      write_key_value_8bit( filedata,
                          Enigma::World::Key::EArrival,
                          (guint8)(*Object).iSurfaceArrival );
    }
    
    // Write departure rotation keyvalue, and add arrival rotation
    // if it is not to be the player's current rotation.
    
    write_key_value_8bit( filedata,
                        Enigma::World::Key::ERotation,
                        (guint8)(*Object).iRotation );
  
    if ((*Object).iRotationArrival != EnigmaWC::Direction::ENone )
    {
      write_key_value_8bit( filedata,
                          Enigma::World::Key::EArrival,
                          (guint8)(*Object).iRotationArrival );
    }
    
    // Write departure East location keyvalue, and add arrival East location
    // if it is not to be the player's current East location.
  
    write_key_value_16bit( filedata,
                         Enigma::World::Key::EEast,
                         (*Object).iLocation.iEast );
                         
    if ((*Object).iLocationArrival.iEast != G_MAXUINT16 )
    {
      write_key_value_16bit( filedata,
                           Enigma::World::Key::EArrival,
                           (*Object).iLocationArrival.iEast );
    }
    
    // Write departure North location keyvalue, and add arrival North location
    // if it is not to be the player's current North location.
  
    write_key_value_16bit( filedata,
                         Enigma::World::Key::ENorth,
                         (*Object).iLocation.iNorth );
                         
    if ((*Object).iLocationArrival.iNorth != G_MAXUINT16 )
    {
      write_key_value_16bit( filedata,
                           Enigma::World::Key::EArrival,
                           (*Object).iLocationArrival.iNorth );
    }
    
    // Write departure Above location keyvalue, adding arrival Above location
    // if it is not to be the player's current Above location.
  
    write_key_value_16bit( filedata,
                         Enigma::World::Key::EAbove,
                         (*Object).iLocation.iAbove );

    if ((*Object).iLocationArrival.iAbove != G_MAXUINT16 )
    {           
      write_key_value_16bit( filedata,
                           Enigma::World::Key::EArrival,
                           (*Object).iLocationArrival.iAbove );
    }

      // Add object state or signal keyvalues.
        
    write_key_value_string( filedata,
                          Enigma::World::Key::ESense,
                          (*Object).iSense );
    
    write_key_value_string( filedata,
                          Enigma::World::Key::EState,
                          (*Object).iState );
    
    write_key_value_string( filedata,
                          Enigma::World::Key::EVisibility,
                          (*Object).iVisibility );
    
    write_key_value_string( filedata,
                          Enigma::World::Key::EPresence,
                          (*Object).iPresence );
  }
  
  //-----------------------------------------*
  // Write KeyValues for all player objects. *
  //-----------------------------------------*
  
  for ( Object = iPlayers.begin();
        Object != iPlayers.end();
        ++ Object )
	{
		// Add header for a player object.
		
		write_key_value_8bit( filedata,
                        Enigma::World::Key::EPlayer,
                        (guint8)(*Object).iID );
		
		// Write current, saved, and restart surface keyvalues.
		// All have the same value.
    
    write_key_value_8bit( filedata,
                        Enigma::World::Key::ESurface,
                        (guint8)(*Object).iSurface );

    if ( iSavable )
    {
      write_key_value_8bit( filedata,
                          Enigma::World::Key::ESaved,
                          (guint8)(*Object).iSurface );
    }
                        
    write_key_value_8bit( filedata,
                        Enigma::World::Key::ERestart,
                        (guint8)(*Object).iSurface );

    // Write current, saved, and restart rotation keyvalues.
    // All have the same value.
    
    write_key_value_8bit( filedata,
                        Enigma::World::Key::ERotation,
                        (guint8)(*Object).iRotation );

    if ( iSavable )
    {
      write_key_value_8bit( filedata,
                          Enigma::World::Key::ESaved,
                          (guint8)(*Object).iRotation );
    }
                        
    write_key_value_8bit( filedata,
                        Enigma::World::Key::ERestart,
                        (guint8)(*Object).iRotation );

    // Write current, saved, and restart East location keyvalues.
    // All have the same value.

    write_key_value_16bit( filedata,
                         Enigma::World::Key::EEast,
                         (*Object).iLocation.iEast );

    if ( iSavable )
    {
      write_key_value_16bit( filedata,
                           Enigma::World::Key::ESaved,
                           (*Object).iLocation.iEast );
    } 
                         
    write_key_value_16bit( filedata,
                         Enigma::World::Key::ERestart,
                         (*Object).iLocation.iEast );

    // Write current, saved, and restart North location keyvalues.
    // All have the same value.

    write_key_value_16bit( filedata,
                         Enigma::World::Key::ENorth,
                         (*Object).iLocation.iNorth );

    if ( iSavable )
    {
      write_key_value_16bit( filedata,
                           Enigma::World::Key::ESaved,
                           (*Object).iLocation.iNorth );
    }
                         
    write_key_value_16bit( filedata,
                         Enigma::World::Key::ERestart,
                         (*Object).iLocation.iNorth );


		// Write current, saved, and restart Above location keyvalues.
		// All have the same value.
		
    write_key_value_16bit( filedata,
                         Enigma::World::Key::EAbove,
                         (*Object).iLocation.iAbove );

    if ( iSavable )
    {
      write_key_value_16bit( filedata,
                           Enigma::World::Key::ESaved,
                           (*Object).iLocation.iAbove );
    }
                         
    write_key_value_16bit( filedata,
                         Enigma::World::Key::ERestart,
                         (*Object).iLocation.iAbove );

		// Add current, saved, and restart Active state keyvalues.
    // All have the same value.
        
    write_key_value_boolean( filedata,
                           Enigma::World::Key::EActive,
                           (*Object).iActive );
                           
    if ( iSavable )
    {
      write_key_value_boolean( filedata,
                             Enigma::World::Key::ESaved,
                             (*Object).iActive );
    }
                           
    write_key_value_boolean( filedata,
                           Enigma::World::Key::ERestart,
                           (*Object).iActive );

    // Add object state or signal keyvalues.  Player objects
    // do not have a Sense state or signal.
    
    write_key_value_string( filedata,
                          Enigma::World::Key::EState,
                          (*Object).iState );
    
    write_key_value_string( filedata,
                          Enigma::World::Key::EVisibility,
                          (*Object).iVisibility );
                              
    write_key_value_string( filedata,
                          Enigma::World::Key::EPresence,
                          (*Object).iPresence );
	}

  //---------------------------------------*
  // Write KeyValues for all item objects. *
  //---------------------------------------*

  for ( Object = iItems.begin();
        Object != iItems.end();
        ++ Object )
  {
    // Add header for an item object.

		write_key_value_8bit( filedata,
                        Enigma::World::Key::EItem,
                        (guint8)(*Object).iID );

   // Write surface.

    write_key_value_8bit( filedata,
                        Enigma::World::Key::ESurface,
                        (guint8)(*Object).iSurface );
    
    // Write rotation.

    write_key_value_8bit( filedata,
                        Enigma::World::Key::ERotation,
                        (guint8)(*Object).iRotation );

    // Write location.

    write_key_value_16bit( filedata,
                         Enigma::World::Key::EEast,
                         (*Object).iLocation.iEast );

    write_key_value_16bit( filedata,
                         Enigma::World::Key::ENorth,
                         (*Object).iLocation.iNorth );
		
    write_key_value_16bit( filedata,
                         Enigma::World::Key::EAbove,
                         (*Object).iLocation.iAbove );

    // Write category.

    write_key_value_8bit( filedata,
                        Enigma::World::Key::ECategory,
                        (guint8)(*Object).iCategory );
                        
    // Add current, saved, and restart active state keyvalues.
    // All items begin as active (not yet found).

    write_key_value_boolean( filedata,
                           Enigma::World::Key::EActive,
                           TRUE );
                           
    if ( iSavable )
    {
      write_key_value_boolean( filedata,
                             Enigma::World::Key::ESaved,
                             TRUE );
    }
                           
    write_key_value_boolean( filedata,
                           Enigma::World::Key::ERestart,
                           TRUE );
                           
    // Add object state or signal keyvalues.  Item objects
    // do not have a Sense state or signal.
    
    write_key_value_string( filedata,
                          Enigma::World::Key::EState,
                          (*Object).iState );
    
    write_key_value_string( filedata,
                          Enigma::World::Key::EVisibility,
                          (*Object).iVisibility );

    
    write_key_value_string( filedata,
                          Enigma::World::Key::EPresence,
                          (*Object).iPresence );
  }

  //------------------------------------------*
  // Write KeyValues for the map description. *
  //------------------------------------------*
		
  // Add header for a description.

  write_key_value_8bit( filedata,
                      Enigma::World::Key::EDescription,
                      (guint8)EnigmaWC::Language::EEnglish );

  // Add description length keyvalue.

  write_key_value_16bit( filedata,
                       Enigma::World::Key::ELength,
                       (guint16)iDescription.bytes() );

  // Write data keyvalue, followed by description in UTF-8 format.
  
  write_key_value_8bit( filedata,
                      Enigma::World::Key::EData,
                      0 );

  filedata.append( iDescription );
  
  //----------------------------------*
  // Write KeyValue array terminator. *
  //----------------------------------*

  write_key_value_8bit( filedata,
                      Enigma::World::Key::EEnd,
                      0 );
*/
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
