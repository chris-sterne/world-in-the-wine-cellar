// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the LevelView class header.  The LevelView class displays
// levels of a game world, and allows editing the room contents on the level.
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
#include "LevelView.h"
#include "World.h"

//-------------------
// Local declarations
//-------------------

#define BACKGROUND_R 0      // Background colour.
#define BACKGROUND_G 0
#define BACKGROUND_B 0

//-------------------------
// This is the constructor.
//-------------------------

Enigma::LevelView::LevelView()
{	
  set_has_window(true);
  set_hexpand(true);
  set_vexpand(true);
  add_events(Gdk::KEY_PRESS_MASK);
  set_can_focus(true);

  m_cursor.m_east  = 0;
  m_cursor.m_north = 0;
  m_cursor.m_above = 0;
 
  m_view.m_WSB = m_cursor;
  m_view.m_ENA = m_cursor;

  m_mark.m_WSB = m_cursor;
  m_mark.m_ENA = m_cursor;
  
  m_filter = Enigma::Object::ID::NONE;
}

//---------------------------------------------------
// Method to get preferred width of LevelView widget.
//---------------------------------------------------

void Enigma::LevelView::get_preferred_width_vfunc(
	int& minimum_width,
	int& natural_width) const
{
  minimum_width = m_tiles.get_tile_size() * 3;
  natural_width = minimum_width;
}

//-----------------------------------------------------------
// This method gets the preferred height of LevelView widget.
//-----------------------------------------------------------

void Enigma::LevelView::get_preferred_height_vfunc(
	int& minimum_height,
	int& natural_height) const
{
  minimum_height = m_tiles.get_tile_size() * 3;
  natural_height = minimum_height;
}

//--------------------------------------------------------------
// This private function adjusts a number range in order to keep
// a particular number within the range, and is used to keep    
// the cursor visible if it leaves the view range.              
//--------------------------------------------------------------
// number: Number to be kept within range.
// lower:  Lower limit of range.
// upper:  Upper limit of range.
//--------------------------------------------------------------

void align_range(unsigned short number,
                 unsigned short& lower,
                 unsigned short& upper)
{	
	if (lower > upper)
	{
		// The range limits are not valid, so exchange them.

		unsigned short temporary = lower;
		lower = upper;
		upper = temporary;
	}
	
	if (number <= lower)
	{
		// The number falls on or below the lower limit.
		// Shift the range to center the number within it.

		unsigned short difference      = upper - lower;
		unsigned short half_difference = difference / 2;

		if (number >= half_difference)
		{
			// The range length is long enough to be centered around the number.
			// Set the lower limit to the half-way point.
		
			lower = number - half_difference;
			upper = lower + difference;
		}
		else
		{
			// The range length is too short to be centered around number.
			// Set the lower limit to its minimum value (zero).

			lower = Enigma::Position::MINIMUM;
			upper = difference;
		}
	}
	else if (number >= upper )
	{
		// The number falls on or above the upper limit.
		// Shift the range to center the number within it.

		unsigned short difference      = upper - lower;
		unsigned short half_difference = difference / 2;

		if ((Enigma::Position::MAXIMUM - number) >= half_difference)
		{
			// The upper limit can be increased to center the number within the range,
			// and also not exceed the maximum range value.

			upper = number + half_difference;
			lower = upper - difference;
		}
		else
		{
			// The range is too wide to center the number within the range.
			// Set the upper range limit to its maximum value.

			upper = Enigma::Position::MAXIMUM;
			lower = upper - difference;
		}
	}
}

//----------------------------------------------------------------
// This method is called when the widget has been allocate a size.
//----------------------------------------------------------------
// allocation: Widget space allocation data.
//----------------------------------------------------------------

void Enigma::LevelView::on_size_allocate(Gtk::Allocation& allocation)
{	
	Gtk::Widget::on_size_allocate(allocation);
	
	unsigned short width  =
		allocation.get_width() / m_tiles.get_tile_size();
	
	unsigned short height =
		allocation.get_height() / m_tiles.get_tile_size();

	// Set initial view row and column ranges with correct lengths.

	m_view.m_WSB.m_east  = Enigma::Position::MINIMUM;
	m_view.m_ENA.m_east  = width;	
	m_view.m_WSB.m_north = Enigma::Position::MINIMUM;
	m_view.m_ENA.m_north = height;

	// Align the view row and column ranges around the cursor.
	
	align_range(m_cursor.m_east, m_view.m_WSB.m_east, m_view.m_ENA.m_east);
	align_range(m_cursor.m_north, m_view.m_WSB.m_north, m_view.m_ENA.m_north);
}

//*-------------------------------------------------------------*
//* This method is called when the widget is about to be shown. *
//*-------------------------------------------------------------*

void Enigma::LevelView::on_map()
{
	// Pass the method to the base class.

	Gtk::DrawingArea::on_map();
	
	// Update cursor location information.
	
	do_position();
}

//----------------------------------
// Method to draw PlayerView widget.
//----------------------------------
// context: Cairo drawing context.
//----------------------------------

bool Enigma::LevelView::on_draw(const Cairo::RefPtr<Cairo::Context>& context)
{	
	// Draw the background colour.

	context->set_source_rgb(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B);
	context->paint();

	Gtk::Allocation allocation = get_allocation();
	Enigma::Position room;
	std::list<std::list<Enigma::Object>::iterator> buffer;
	std::list<std::list<Enigma::Object>::iterator>::iterator object;
	bool drawn;

	unsigned short row;
	unsigned short column; 

	room.m_above = m_cursor.m_above;

	for (room.m_north = m_view.m_WSB.m_north;
	     room.m_north <= m_view.m_ENA.m_north;
	     ++ room.m_north )
	{
		for (room.m_east = m_view.m_WSB.m_east;
		     room.m_east <= m_view.m_ENA.m_east;
		     ++ room.m_east)
		{
			row    = room.m_north - m_view.m_WSB.m_north;
			column = room.m_east - m_view.m_WSB.m_east;

			// Read all objects in the room.

			buffer.resize(0);
			m_world->m_objects.read(room, buffer);
			m_world->m_items.read(room, buffer);
			m_world->m_players.read(room, buffer);
			m_world->m_teleporters.read(room, buffer);

			// Draw all environment objects first.

			for (object = buffer.begin();
			     object != buffer.end();
			     ++ object)
			{
				if  (((*(*object)).m_id == Enigma::Object::ID::OUTDOOR)
					|| ((*(*object)).m_id == Enigma::Object::ID::INDOOR))
				{
					if  ((m_filter == Enigma::Object::ID::NONE)
						|| (m_filter == (*(*object)).m_id))
					{
						drawn = m_tiles.draw_object(context,
						                            allocation,
						                            column,
						                            row,
						                            *(*object));					
						if (!drawn)
							m_tiles.draw_generic(context, allocation, column, row);
					}

					// Erase environment object so it will not drawn again.

					object = buffer.erase(object);
				}
			}

			// Draw all floor objects first.

			for (object = buffer.begin();
			     object != buffer.end();
			     ++ object)
			{
				if ((((*(*object)).m_id == Enigma::Object::ID::BLOCKWALL)
					|| ((*(*object)).m_id == Enigma::Object::ID::STONEWALL)
					|| ((*(*object)).m_id == Enigma::Object::ID::WOODWALL)
					|| ((*(*object)).m_id == Enigma::Object::ID::EARTHWALL))
					&& ((*(*object)).m_surface == Enigma::Object::Direction::BELOW))
				{
					if  ((m_filter == Enigma::Object::ID::NONE)
					  || (m_filter == (*(*object)).m_id))
					{
						drawn = m_tiles.draw_object(context,
						                            allocation,
						                            column,
						                            row,
						                            *(*object));
						if (!drawn)
							m_tiles.draw_generic(context, allocation, column, row);
					}

					// Erase floor object so it will not drawn again.

					object = buffer.erase(object);
				}
			}

			// Draw all objects above the floor.

			for (object = buffer.begin(); object != buffer.end(); ++ object)
			{
				if  ((m_filter == Enigma::Object::ID::NONE)
				  || ( m_filter == (*(*object)).m_id))
				{
					drawn = m_tiles.draw_object(context,
					                            allocation,
					                            column,
					                            row,
					                            *(*object));                       
					
					if (!drawn)
						m_tiles.draw_generic(context, allocation, column, row);
				}
			}

			// Draw a marker if the current room location falls within the marked
			// area and the marked area is greater than one room in volume.

			if (((m_mark.m_ENA.m_above != m_mark.m_WSB.m_above)
			  || (m_mark.m_ENA.m_north != m_mark.m_WSB.m_north)
			  || (m_mark.m_ENA.m_east != m_mark.m_WSB.m_east))
			  && (room.m_above >= m_mark.m_WSB.m_above)
			  && (room.m_above <= m_mark.m_ENA.m_above)
			  && (room.m_north >= m_mark.m_WSB.m_north)
			  && (room.m_north <= m_mark.m_ENA.m_north)
			  && (room.m_east >= m_mark.m_WSB.m_east)
			  && (room.m_east <= m_mark.m_ENA.m_east))
			{
				m_tiles.draw_marker(context, allocation, column, row);
			}
		}
	}

	// Draw all teleporter arrival marks that fall within the view. 

	std::list<Enigma::Object>::iterator teleporter;

	unsigned short east_arrival;
	unsigned short north_arrival;
	unsigned short above_arrival;

	for (teleporter = m_world->m_teleporters.begin();
	     teleporter != m_world->m_teleporters.end();
	     ++ teleporter )
	{
		east_arrival  = (*teleporter).m_position_arrival.m_east;
		north_arrival = (*teleporter).m_position_arrival.m_north;
		above_arrival = (*teleporter).m_position_arrival.m_above;

		// If an arrival location is set to its maximum value, the arrival
		// location is the player's (and teleporter's) current location.

		if (east_arrival == Enigma::Position::MAXIMUM)
			east_arrival = (*teleporter).m_position.m_east;

		if (north_arrival == Enigma::Position::MAXIMUM)
			north_arrival = (*teleporter).m_position.m_north;

		if (above_arrival == Enigma::Position::MAXIMUM)
			above_arrival = (*teleporter).m_position.m_above;

		if  ((east_arrival >= m_view.m_WSB.m_east)
		  && (east_arrival <= m_view.m_ENA.m_east)
		  && (north_arrival >= m_view.m_WSB.m_north)
		  && (north_arrival <= m_view.m_ENA.m_north)
		  && (above_arrival == room.m_above))
		{
			column = east_arrival - m_view.m_WSB.m_east;
			row    = north_arrival - m_view.m_WSB.m_north;

			m_tiles.draw_arrival(context, allocation, column, row);
		}
	}

	// Draw the cursor last.

	column = m_cursor.m_east - m_view.m_WSB.m_east;
	row    = m_cursor.m_north - m_view.m_WSB.m_north;

	m_tiles.draw_cursor(context, allocation, column, row);

	// Return TRUE to indicate all drawing has been done.

	return true;
}

//------------------------------------------
// Method to handle key press events.
//------------------------------------------
// key_event: Pointer to GdkEventKey.
// RETURN:    TRUE if key press was handled.
//------------------------------------------

bool Enigma::LevelView::on_key_press_event(GdkEventKey* key_event)
{
	bool handled = false;

	if (key_event->type == GDK_KEY_PRESS)
	{
		int key_value = key_event->keyval;
		int key_state = key_event->state;

		if (key_state & GDK_CONTROL_MASK )
		{
			switch (key_value)
			{
				case GDK_KEY_x:
					cut();
					handled = true;
					break;

				case GDK_KEY_c:
					copy();
					handled = true;
					break;

				case GDK_KEY_v:
					paste();
					handled = true;
					break;

				default:
					break;
			}
		}
		else
		{
			switch (key_value)
			{
				case GDK_KEY_Up:
					if (m_cursor.m_north < Enigma::Position::MAXIMUM)
					{
						++ m_cursor.m_north;
						do_position();

						// Make sure the cursor falls within the visible rows.

						align_range(m_cursor.m_north,
							          m_view.m_WSB.m_north,
							          m_view.m_ENA.m_north);

						// The cursor has now been moved.  Use its new location
						// to update the marked volume.

						if (key_state & GDK_SHIFT_MASK)
							mark();            

						// Update the view to show the changes.

						update();
					}

					handled = true;
					break;

				case GDK_KEY_Down:
					if (m_cursor.m_north > Enigma::Position::MINIMUM)
					{           
						-- m_cursor.m_north;
						do_position();

						// Make sure the cursor falls within the visible rows.

						align_range(m_cursor.m_north,
							          m_view.m_WSB.m_north,
							          m_view.m_ENA.m_north);

						// The cursor has now been moved.  Use its new location
						// to update the marked volume.

						if (key_state & GDK_SHIFT_MASK)
							mark();

						// Update the view to show the changes.

						update();
					}

					handled = true;
					break;

				case GDK_KEY_Left:  
					if (m_cursor.m_east > Enigma::Position::MINIMUM)
					{
						m_cursor.m_east --;
						do_position();

						// Make sure the cursor falls within the visible columns.

						align_range(m_cursor.m_east,
							          m_view.m_WSB.m_east,
							          m_view.m_ENA.m_east);

						// The cursor has now been moved.  Use its new location
						// to update the marked volume.

						if (key_state & GDK_SHIFT_MASK)
							mark();

						// Update the view to show the changes.

						update();
					}

					handled = true;
					break;

				case GDK_KEY_Right:
					if (m_cursor.m_east < Enigma::Position::MAXIMUM)
					{           
						m_cursor.m_east ++;
						do_position();

						// Make sure the cursor falls within the visible columns.

						align_range(m_cursor.m_east,
							          m_view.m_WSB.m_east,
							          m_view.m_ENA.m_east);

						// The cursor has now been moved.  Use its new location
						// to update the marked volume.

						if (key_state & GDK_SHIFT_MASK)
							mark();

						// Update the view to show the changes.

						update();
					}

					handled = true;
					break;

				case GDK_KEY_Page_Down:
					if (m_cursor.m_above > Enigma::Position::MINIMUM)
					{          
						m_cursor.m_above --;
						do_position();

						// Make sure the cursor falls within the visible level.

						m_view.m_WSB.m_above = m_cursor.m_above;
						m_view.m_ENA.m_above = m_cursor.m_above;

						// The cursor has now been moved.  Use its new location
						// to update the marked volume.

						if (key_state & GDK_SHIFT_MASK)
							mark();

						// Update the view to show the changes.

						update();
					}

					handled = true;
					break;

				case GDK_KEY_Page_Up:
					if (m_cursor.m_above < Enigma::Position::MAXIMUM)
					{          
						m_cursor.m_above ++;
						do_position();

						// Make sure the cursor falls within the visible level.

						m_view.m_WSB.m_above = m_cursor.m_above;
						m_view.m_ENA.m_above = m_cursor.m_above;

						// The cursor has now been moved.  Use its new location
						// to update the marked volume.

						if (key_state & GDK_SHIFT_MASK)
							mark();

						// Update the view to show the changes.

						update();
					}

					handled = true;
					break;

				case GDK_KEY_Home:
					// Set the map view to convenient location.

					home();
					break;

				case GDK_KEY_Delete:
					// Erase the marked volume.

					erase();  
					break;

				case GDK_KEY_Escape:
				case GDK_KEY_Shift_L:
				case GDK_KEY_Shift_R:
					// Begin volume marking, or clear the current marking.  Since this
					// must occur only once when starting to mark a volume,  the SHIFT
					// key must not repeat while being pressed.

					m_mark_origin = m_cursor;
					mark();
					break;

				default:
					break;
			}  
		}
	}

	return handled;
}

//----------------------------------------------
// This method sets the game world to be viewed.
//----------------------------------------------
// world: Game world.                         
//----------------------------------------------

void Enigma::LevelView::set_world(std::shared_ptr<Enigma::World> world)
{
	m_world = world;
}

//-------------------------------------------------------------
// This method sets an object view filter.
//-------------------------------------------------------------
// filter: ID of object to filter, or NONE to show all objects.
//-------------------------------------------------------------

void Enigma::LevelView::set_filter(Enigma::Object::ID filter)
{
	m_filter = filter;
	
	// Update the view to show the result of the object filter.

  update();
}

//--------------------------------------------------------
// This method returns a reference to the cursor location.
//--------------------------------------------------------

Enigma::Position& Enigma::LevelView::get_cursor()
{
	return m_cursor;
}

//-----------------------------------------------------------------
// This method changes the map view so it shows objects of the map.
//-----------------------------------------------------------------

void Enigma::LevelView::home()
{ 
	// Search for an active player.

	std::list<Enigma::Object>::iterator object;

	for (object = m_world->m_players.begin();
	     object != m_world->m_players.end();
	     ++ object)
	{
		if ((*object).m_active )
		{
			// An active player was found.  Set the cursor to its location.

			m_cursor = (*object).m_position;
			break;
		}
	}

	if (object == m_world->m_players.end())
	{
		// There are no active players present, so try the first player.

		object = m_world->m_players.begin();

		if ( object != m_world->m_players.end())
			m_cursor = (*object).m_position;
		else   
		{
			// There are no players present, so try the first item.

			object = m_world->m_items.begin();

			if (object != m_world->m_items.end())
				m_cursor = (*object).m_position;
			else
			{
				// There are no items present, so try the first teleporter.

				object = m_world->m_teleporters.begin();

				if ( object != m_world->m_teleporters.end())
					m_cursor = (*object).m_position;
				else
				{
					// There are no teleporters present, so try the first structural
					// object.

					object = m_world->m_objects.begin();

					if (object != m_world->m_objects.end())
						m_cursor = (*object).m_position;
					else
					{
						// There are no objects in the map.  Set the cursor to a position
						// of zero (0, 0, 0).

						m_cursor.m_above = Enigma::Position::MINIMUM;
						m_cursor.m_north = Enigma::Position::MINIMUM;
						m_cursor.m_east  = Enigma::Position::MINIMUM;
					}
				}
			}
		}
	}

	// Report changes to the cursor location.

	do_position();

	// Adjust the visible view volume so the cursor is visible.

	align_range(m_cursor.m_north, m_view.m_WSB.m_north, m_view.m_ENA.m_north);
	align_range(m_cursor.m_east, m_view.m_WSB.m_east, m_view.m_ENA.m_east);

	// Update the view to show the changes.

	update();
}

//---------------------------------------------------------------------
// This method expands the marked map volume using the cursor position.
//---------------------------------------------------------------------

void Enigma::LevelView::mark()
{
	// Update the marked volume so it contain the cursor volume
	// and the proper relationship between the volume corners.

	if (m_cursor.m_above >= m_mark_origin.m_above)
	{
		m_mark.m_WSB.m_above = m_mark_origin.m_above;
		m_mark.m_ENA.m_above = m_cursor.m_above;
	}
	else if (m_cursor.m_above < m_mark_origin.m_above)
	{
		m_mark.m_WSB.m_above = m_cursor.m_above;
		m_mark.m_ENA.m_above = m_mark_origin.m_above;
	}

	if (m_cursor.m_north >= m_mark_origin.m_north)
	{
		m_mark.m_WSB.m_north = m_mark_origin.m_north;
		m_mark.m_ENA.m_north = m_cursor.m_north;
	}
	else if (m_cursor.m_north < m_mark_origin.m_north)
	{
		m_mark.m_WSB.m_north = m_cursor.m_north;
		m_mark.m_ENA.m_north = m_mark_origin.m_north;
	} 

	if (m_cursor.m_east >= m_mark_origin.m_east)
	{
		m_mark.m_WSB.m_east = m_mark_origin.m_east;
		m_mark.m_ENA.m_east = m_cursor.m_east;
	}
	else if (m_cursor.m_east < m_mark_origin.m_east)
	{
		m_mark.m_WSB.m_east = m_cursor.m_east;
		m_mark.m_ENA.m_east = m_mark_origin.m_east;
	}

	// Update the view to show the changes.

	update();
}

//*-----------------------------------------*
//* This method erases all marked objects. *
//*-----------------------------------------*

void Enigma::LevelView::erase()
{
	// Select the map volume to be erased.  If the marked volume has a size
	// of only one room, set it to the cursor volume instead.

	if  ((m_mark.m_ENA.m_above == m_mark.m_WSB.m_above)
	  && (m_mark.m_ENA.m_north == m_mark.m_WSB.m_north)
	  && (m_mark.m_ENA.m_east == m_mark.m_WSB.m_east))
	{
		m_mark.m_ENA = m_cursor;
		m_mark.m_WSB = m_cursor;
	}

	// Move all marked volume objects to a buffer.

	std::list<std::list<Enigma::Object>::iterator> marked;
	std::list<Enigma::Object> buffer;

	m_world->m_objects.read(m_mark, marked);
	m_world->m_objects.remove(marked, buffer);

	// Move all marked volume items to a buffer.

	marked.clear();

	m_world->m_items.read(m_mark, marked);
	m_world->m_items.remove(marked, buffer);

	// Move all marked volume players to the buffer.

	marked.clear();

	m_world->m_players.read(m_mark, marked);
	m_world->m_players.remove(marked, buffer);

	// Move all marked volume teleporters to the buffer.

	marked.clear();

	m_world->m_teleporters.read(m_mark, marked);
	m_world->m_teleporters.remove(marked, buffer);

	// Unmark the marked volume.

	m_mark.m_ENA = m_cursor;
	m_mark.m_WSB = m_cursor;

	// Update the view to show the changes.

	update();
}

//-----------------------------------------------------------
// This method moves all marked objects to an editing buffer.
//-----------------------------------------------------------

void Enigma::LevelView::cut()
{  
	// Clear any old objects in the editing buffer.

	m_edit_buffer.clear();

	// Select the map volume to be cut.  If the marked volume has a size
	// of only one room, set it to the cursor volume instead.

	if  ((m_mark.m_ENA.m_above == m_mark.m_WSB.m_above)
	  && (m_mark.m_ENA.m_north == m_mark.m_WSB.m_north)
	  && (m_mark.m_ENA.m_east == m_mark.m_WSB.m_east))
	{
		m_mark.m_ENA = m_cursor;
		m_mark.m_WSB = m_cursor;
	}

	// Move all marked volume objects to the editing buffer.

	std::list<std::list<Enigma::Object>::iterator> marked;

	m_world->m_objects.read(m_mark, marked);
	m_world->m_objects.remove(marked, m_edit_buffer);

	// Move all marked volume items to the editing buffer.

	marked.clear();

	m_world->m_items.read(m_mark, marked);
	m_world->m_items.remove(marked, m_edit_buffer);

	// Move all marked volume players to the edit buffer.

	marked.clear();

	m_world->m_players.read(m_mark, marked);
	m_world->m_players.remove(marked, m_edit_buffer);

	// Move all marked volume teleporters to the edit buffer.

	marked.clear();

	m_world->m_teleporters.read(m_mark, marked);
	m_world->m_teleporters.remove(marked, m_edit_buffer);

	// Change the map position of all objects to be an offset from a map
	// position of (0, 0, 0), corresponding to the West-South-Below corner
	// of the marked volume.

	std::list<Enigma::Object>::iterator object;

	for (object = m_edit_buffer.begin();
	     object != m_edit_buffer.end();
	     ++ object)
	{
		(*object).m_position.m_above =
			(*object).m_position.m_above - m_mark.m_WSB.m_above;

		(*object).m_position.m_north =
			(*object).m_position.m_north - m_mark.m_WSB.m_north;

		(*object).m_position.m_east =
			(*object).m_position.m_east - m_mark.m_WSB.m_east;
	}

	// Unmark the marked volume.

	m_mark.m_ENA = m_cursor;
	m_mark.m_WSB = m_cursor;

	// Update the view to show the changes.

	update();
}

//-----------------------------------------------------------
// This method copies all marked objects to a editing buffer.
//-----------------------------------------------------------

void Enigma::LevelView::copy()
{
	// Clear any old objects in the editing buffer.

	m_edit_buffer.clear();

	// Select the map volume to be cut.  If the marked volume has a size
	// of only one room, use the cursor volume instead.

	if  ((m_mark.m_ENA.m_above == m_mark.m_WSB.m_above)
	  && (m_mark.m_ENA.m_north == m_mark.m_WSB.m_north)
	  && (m_mark.m_ENA.m_east == m_mark.m_WSB.m_east))
	{
		m_mark.m_ENA = m_cursor;
		m_mark.m_WSB = m_cursor;
	}

	// Get a copy of all marked objects from all object lists.

	m_world->m_objects.copy(m_mark, m_edit_buffer);
	m_world->m_items.copy(m_mark, m_edit_buffer);
	m_world->m_players.copy(m_mark, m_edit_buffer);
	m_world->m_teleporters.copy(m_mark, m_edit_buffer);

	// Change the map position of all objects to be an offset from a map
	// location of (0, 0, 0), corresponding to the upper-top-left corner
	// of the marked volume.

	std::list<Enigma::Object>::iterator object;

	for (object = m_edit_buffer.begin();
	     object != m_edit_buffer.end();
	     ++ object)
	{
		(*object).m_position.m_above =
			(*object).m_position.m_above - m_mark.m_WSB.m_above;

		(*object).m_position.m_north =
			(*object).m_position.m_north - m_mark.m_WSB.m_north;

		(*object).m_position.m_east =
			(*object).m_position.m_east - m_mark.m_WSB.m_east;
	}

	// The marked volume will remain marked.
}

//----------------------------------------------------------------
// This method inserts a copy of all object in the editing buffer,
// changing their world position to be relative to the cursor.      
//----------------------------------------------------------------

void Enigma::LevelView::paste()
{ 
	// Set the world position of all new objects being inserted relative to the
	// world position of the cursor.  The position of the original objects in
	// the editing buffer must not be modified, since they are an offset from 
	// position of (0, 0, 0).

	Enigma::Object new_object;
	std::list<Enigma::Object>::iterator object;

	for (object = m_edit_buffer.begin();
	     object != m_edit_buffer.end();
	     ++ object )
	{
		new_object = (*object);

		// Prevent a wrap-around of the map position if pasting near
		// position limits.  Objects outside the limit will be lost.

		if  ((new_object.m_position.m_above <=
			     (Enigma::Position::MAXIMUM - m_cursor.m_above))
		  && (new_object.m_position.m_north <=
		       (Enigma::Position::MAXIMUM - m_cursor.m_north)) 
		  && (new_object.m_position.m_east <=
		       (Enigma::Position::MAXIMUM - m_cursor.m_east)))
		{ 
			new_object.m_position.m_above += m_cursor.m_above;
			new_object.m_position.m_north += m_cursor.m_north;
			new_object.m_position.m_east  += m_cursor.m_east;

			// Add the new object to the appropriate list in the map.

			switch (new_object.m_type)
			{
				case Enigma::Object::Type::OBJECT:
					m_world->m_objects.insert(new_object);
					break;

				case Enigma::Object::Type::ITEM:
					m_world->m_items.insert(new_object);
					break;

				case Enigma::Object::Type::PLAYER:
					m_world->m_players.insert(new_object);
					break;

				case Enigma::Object::Type::TELEPORTER:
					m_world->m_teleporters.insert(new_object);
					break;

				default:
					break;
			}
		}
	}

	// Update the view to show the changes.

	update();
}

//------------------------------
// This method updates the view.
//------------------------------

void Enigma::LevelView::update()
{
	queue_draw();
}

//---------------------------------------------
// This method returns the map position signal.
//---------------------------------------------

Enigma::LevelView::type_signal_position Enigma::LevelView::signal_position()
{
	return m_signal_position;
}

//-----------------------------------------------------------
// This method emits a signal containing the cursor position.
//-----------------------------------------------------------

void Enigma::LevelView::do_position()
{	  
	m_signal_position.emit(m_cursor);
}
