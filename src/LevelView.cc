// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the LevelView class header.  The LevelView class is an editor
// for levels and rooms of a game world.
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

//-------------------
// Local declarations
//-------------------

#define BACKGROUND_R 0                 // Background colour.
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

//----------------------------------------------------
// Method to get preferred width of PlayerView widget.
//----------------------------------------------------

void Enigma::LevelView::get_preferred_width_vfunc(int& minimum_width,
                                                  int& natural_width) const
{
  minimum_width = m_imagetiles.get_tile_size() * 3;
  natural_width = minimum_width;
}

//*------------------------------------------------------*
//* Method to get preferred height of PlayerView widget. *
//*------------------------------------------------------*

void CLevelView::get_preferred_height_vfunc(int& minimum_height,
                                            int& natural_height) const
{
  minimum_height = m_imagetiles.get_tile_size() * 3;
  natural_height = minimum_height;
}

//*---------------------------------------------------------------*
//* This private function adjusts a number range in order to keep *
//* a particular number within the range, and is used to keep     *
//* the cursor visible if it leaves the view range.               *
//*---------------------------------------------------------------*
//* number: Number to be kept within range.                       *
//* lower:  Lower limit of range.                                 *
//* upper:  Upper limit of range.                                 *
//*---------------------------------------------------------------*

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

			lower = 0;
			upper = difference;
		}
	}
	else if (number >= upper )
	{
		// The number falls on or above the upper limit.
		// Shift the range to center the number within it.

		unsigned short difference      = upper - lower;
		unsigned short half_difference = difference / 2;

		if ((65535 - number) >= half_difference)
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

			upper = 65535;
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
		allocation.get_width() / m_imagetiles.get_tile_size();
	
	unsigned short height =
		allocation.get_height() / m_imagetiles.get_tile_size();

	// Set initial view row and column ranges with correct lengths.

	m_view.m_WSB.m_east  = 0;
	m_view.m_ENA.m_east  = width;	
	m_view.m_WSB.m_north = 0;
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
	
	do_location();
}

//----------------------------------
// Method to draw PlayerView widget.
//----------------------------------
// context: Cairo context.
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
	
  for ( room.m_north = m_view.m_WSB.m_north;
        room.m_north <= m_view.m_ENA.m_north;
        ++ room.m_north )
  {
    for ( room.m_east = m_view.m_WSB.m_east;
          room.m_east <= m_view.m_ENA.m_east;
          ++ room.m_east )
    {
      Row    = room.m_north - m_view.m_WSB.m_north;
      Column = room.m_east - m_view.m_WSB.m_east;
			
      // Read all MapObjects in the room.

      buffer.resize(0);
      m_world->m_objects.Read(room, buffer);
      m_world->m_items.Read(room, buffer);
      m_world->m_players.Read(room, buffer);
      m_world->m_teleporters.Read(room, buffer);
      
      for (object = buffer.begin(); object != buffer.end(); ++ object)
      {
        // Draw all environment objects first.
			
        if (((*(*Object)).iID == Enigma::Object::ID::EOutdoor )
          || ((*(*Object)).iID == Enigma::Object::ID::EIndoor ))
        {
          if  (( m_filter == Enigma::Object::ID::ENone )
            || ( m_filter == (*(*Object)).iID ))
          {
            Drawn = m_imagetiles.DrawMapObject( aCC,
                                               Allocation,
                                               Column,
                                               Row,
                                               *(*Object) );					
            if ( !Drawn )
              m_imagetiles.DrawGeneric( aCC, Allocation, Column, Row );
          }
            
          // Erase environment object so it will not drawn again.
	
          Object = buffer.erase( Object );
        }
      }
      
      for ( Object = buffer.begin(); Object != buffer.end(); ++ Object )
      {
        // Draw all floor objects first.
			
        if ((((*(*object)).iID == Enigma::Object::ID::EBlockWall )
          || ((*(*object)).iID == Enigma::Object::ID::EStoneWall ))
          && ((*(*object)).iSurface == EnigmaWC::Direction::EBelow ))
          
        {
          if  (( m_filter == Enigma::Object::ID::ENone )
            || ( m_filter == (*(*Object)).iID ))
          {
            Drawn = m_imagetiles.DrawMapObject( aCC,
                                               Allocation,
                                               Column,
                                               Row,
                                               *(*Object) );					
            if (!drawn)
              m_imagetiles.DrawGeneric( aCC, Allocation, Column, Row );
          }
            
          // Erase floor object so it will not drawn again.
	
          object = buffer.erase( Object );
        }
      }

      // Draw all objects above the floor.

      for (object = buffer.begin(); object != buffer.end(); ++ object)
      {
        if  (( m_filter == Enigma::Object::ID::ENone )
          || ( m_filter == (*(*Object)).iID ))
        {
          drawn = m_imagetiles.DrawMapObject( aCC,
                                             Allocation,
                                             Column,
                                             Row,
                                             *(*Object ));                       
          if ( !Drawn )
            m_imagetiles.DrawGeneric( aCC, Allocation, Column, Row );
        }
      }
      
      // Draw a marker if the current room location falls within the marked area
      // and the marked area is greater than one room in volume.
  
      if ((( m_mark.m_ENA.m_above != m_mark.m_WSB.m_above )
        || ( m_mark.m_ENA.m_north != m_mark.m_WSB.m_north )
        || ( m_mark.m_ENA.m_east != m_mark.m_WSB.m_east ))
        && ( room.m_above >= m_mark.m_WSB.m_above )
        && ( room.m_above <= m_mark.m_ENA.m_above )
        && ( room.m_north >= m_mark.m_WSB.m_north )
        && ( room.m_north <= m_mark.m_ENA.m_north )
        && ( room.m_east >= m_mark.m_WSB.m_east )
        && ( room.m_east <= m_mark.m_ENA.m_east ))
      {
        m_imagetiles.draw_marker(context, allocation, column, row);
      }
    }
  }

  // Draw all teleporter arrival marks that fall within the view. 

  std::list<Enigma::object>::iterator teleporter;

  unsigned short east_arrival;
  unsigned short north_arrival;
  unsigned short above_arrival;
    
  for (teleporter = m_world->m_teleporters.begin();
       teleporter != m_world->m_teleporters.end();
       ++ teleporter )
  {
    east_arrival = (*teleporter).m_Position_arrival.m_east;
    north_arrival = (*teleporter).m_Position_arrival.m_north;
    above_arrival = (*teleporter).m_Position_arrival.m_above;
    
    // If an arrival location is set to its maximum value, the arrival
    // location is the player's (and teleporter's) current location.
    
    if (east_arrival == 65535)
      east_arrival = (*teleporter).m_position.m_east;
      
    if (north_arrival == 65535)
      north_arrival = (*teleporter).m_position.m_north;
      
    if (above_arrival == 65535)
      above_arrival = (*teleporter).m_position.m_above;
    
    if  ((east_arrival >= m_view.m_WSB.m_east)
      && (east_arrival <= m_view.m_ENA.m_east)
      && (north_arrival >= m_view.m_WSB.m_north)
      && (north_arrival <= m_view.m_ENA.m_north)
      && (above_arrival == room.m_above))
    {
      Column = east_arrival - m_view.m_WSB.m_east;
      Row    = north_arrival - m_view.m_WSB.m_north;

      m_imagetiles.draw_arrival(context, allocation, column, row);
    }
  }

  // Draw the cursor last.
	
  column = m_cursor.m_east - m_view.m_WSB.m_east;
  row    = m_cursor.m_north - m_view.m_WSB.m_north;

  m_imagetiles.draw_cursor(context, allocation, column, row);
  
  // Return TRUE to indicate all drawing has been done.

  return true;
}

//*-------------------------------------------*
//* Method to handle key press events.        *
//*-------------------------------------------*
//* key_event: Pointer to GdkEventKey.        *
//* RETURN:    TRUE if key press was handled. *
//*-------------------------------------------*

bool CLevelView::on_key_press_event( GdkEventKey* key_event )
{
  gboolean Handled = FALSE;

  if ( key_event->type == GDK_KEY_PRESS )
  {
    int KeyValue = key_event->keyval;
    int KeyState = key_event->state;
    
    if ( KeyState & GDK_CONTROL_MASK )
    {
      switch ( KeyValue )
      {
        case GDK_KEY_x:
          Cut();
          Handled = TRUE;
          break;
          
        case GDK_KEY_c:
          Copy();
          Handled = TRUE;
          break;
          
        case GDK_KEY_v:
          Paste();
          Handled = TRUE;
          break;
          
        default:
          break;
      }
    }
    else
    {
      switch ( KeyValue )
      {
        case GDK_KEY_Up:
          if ( m_cursor.m_north < G_MAXUINT16 )
          {
            ++ m_cursor.m_north;
            Do_Location();

            // Make sure the cursor falls within the visible rows.
				
            AlignRange( m_cursor.m_north, m_view.m_WSB.m_north, m_view.m_ENA.m_north );
                        			    
  			    // The cursor has now been moved.  Use its new location
  			    // to update the marked volume.
  			    
  			    if ( KeyState & GDK_SHIFT_MASK )
              Mark();            
            
            // Update the view to show the changes.
            
            Update();
          }

			    
			    Handled = TRUE;
			    break;
			    
        case GDK_KEY_Down:
          if ( m_cursor.m_north > 0 )
          {           
            -- m_cursor.m_north;
            Do_Location();

            // Make sure the cursor falls within the visible rows.
				
            AlignRange( m_cursor.m_north, m_view.m_WSB.m_north, m_view.m_ENA.m_north );
            
            // The cursor has now been moved.  Use its new location
  			    // to update the marked volume.
            
            if ( KeyState & GDK_SHIFT_MASK )
              Mark();
            
            // Update the view to show the changes.
            
            Update();
          }
          
          Handled = TRUE;
          break;
          
        case GDK_KEY_Left:  
          if ( m_cursor.m_east > 0 )
          {
            m_cursor.m_east --;
            Do_Location();

            // Make sure the cursor falls within the visible columns.
				
            AlignRange( m_cursor.m_east, m_view.m_WSB.m_east, m_view.m_ENA.m_east );
            
            // The cursor has now been moved.  Use its new location
  			    // to update the marked volume.
            
            if ( KeyState & GDK_SHIFT_MASK )
              Mark();
            
            // Update the view to show the changes.
            
            Update();
          }
           
          Handled = TRUE;
          break;
          
        case GDK_KEY_Right:
          if ( m_cursor.m_east < G_MAXUINT16 )
          {           
            m_cursor.m_east ++;
            Do_Location();

            // Make sure the cursor falls within the visible columns.
				
            AlignRange( m_cursor.m_east, m_view.m_WSB.m_east, m_view.m_ENA.m_east );
            
            // The cursor has now been moved.  Use its new location
  			    // to update the marked volume.
            
            if ( KeyState & GDK_SHIFT_MASK )
              Mark();
            
            // Update the view to show the changes.
            
            Update();
          }
          
          Handled = TRUE;
          break;
          
        case GDK_KEY_Page_Down:
          if ( m_cursor.m_above > 0 )
          {          
            m_cursor.m_above --;
            Do_Location();

            // Make sure the cursor falls within the visible level.
				
            m_view.m_WSB.m_above = m_cursor.m_above;
            m_view.m_ENA.m_above = m_cursor.m_above;
            
            // The cursor has now been moved.  Use its new location
  			    // to update the marked volume.
            
            if ( KeyState & GDK_SHIFT_MASK )
              Mark();
            
            // Update the view to show the changes.
            
            Update();
          }
          
          Handled = TRUE;
          break;
          
        case GDK_KEY_Page_Up:
          if ( m_cursor.m_above < G_MAXUINT16 )
          {          
            m_cursor.m_above ++;
            Do_Location();

            // Make sure the cursor falls within the visible level.
				
            m_view.m_WSB.m_above = m_cursor.m_above;
            m_view.m_ENA.m_above = m_cursor.m_above;
            
            // The cursor has now been moved.  Use its new location
  			    // to update the marked volume.
            
            if ( KeyState & GDK_SHIFT_MASK )
              Mark();
            
            // Update the view to show the changes.
            
            Update();
          }
          
          Handled = TRUE;
          break;
        
        case GDK_KEY_Home:
	        // Set the map view to convenient location.
	         
	        Home();
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

          iMarkOrigin = m_cursor;
          mark();
          break;

        default:
          break;
      }  
    }
	}

	return Handled;
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

//*------------------------------------------------------------------*
//* This method changes the map view so it shows objects of the map. *
//*------------------------------------------------------------------*

void CLevelView::Home()
{ 
  // Search for an active player.
  
  std::list<CMapObject>::iterator Object;
  
  for ( Object = iMap->iPlayers.begin();
        Object != iMap->iPlayers.end();
        ++ Object )
  {
    if ( (*Object).iActive )
    {
       // An active player was found.  Set the cursor to its location.
    
      m_cursor = (*Object).m_position;
      break;
    }
  }
  
  if ( Object == iMap->iPlayers.end() )
  {
    // There are no active players present, so try the first player.
    
    Object = iMap->iPlayers.begin();
    
    if ( Object != iMap->iPlayers.end() )
      m_cursor = (*Object).m_position;
    else   
    {
      // There are no players present, so try the first item.
    
      Object = iMap->iItems.begin();
    
      if ( Object != iMap->iItems.end() )
        m_cursor = (*Object).m_position;
      else
      {
        // There are no items present, so try the first teleporter.
        
        Object = iMap->iTeleporters.begin();
        
        if ( Object != iMap->iTeleporters.end() )
          m_cursor = (*Object).m_position;
        else
        {
          // There are no teleporters present, so try the first structural
          // object.
      
          Object = iMap->iObjects.begin();
      
          if ( Object != iMap->iObjects.end() )
            m_cursor = (*Object).m_position;
          else
          {
            // There are no objects in the map.  Set the cursor to a location
            // of zero.
        
            m_cursor.m_above = 0;
            m_cursor.m_north = 0;
	          m_cursor.m_east  = 0;
	        }
        }
      }
    }
  }

  // Report changes to the cursor location.
  
  Do_Location();

  // Adjust the visible view volume so the cursor is visible.

  AlignRange( m_cursor.m_north, m_view.m_WSB.m_north, m_view.m_ENA.m_north );
  AlignRange( m_cursor.m_east, m_view.m_WSB.m_east, m_view.m_ENA.m_east );
  
  // Update the view to show the changes.

  Update();
}

//*----------------------------------------------------------------------*
//* This method expands the marked map volume using the cursor location. *
//*----------------------------------------------------------------------*

void CLevelView::Mark()
{
  // Update the marked volume so it contain the cursor volume
  // and the proper relationship between the volume corners.

  if ( m_cursor.m_above >= iMarkOrigin.m_above )
  {
    m_mark.m_WSB.m_above = iMarkOrigin.m_above;
    m_mark.m_ENA.m_above = m_cursor.m_above;
  }
  else if ( m_cursor.m_above < iMarkOrigin.m_above )
  {
    m_mark.m_WSB.m_above = m_cursor.m_above;
    m_mark.m_ENA.m_above = iMarkOrigin.m_above;
  }

  if ( m_cursor.m_north >= iMarkOrigin.m_north )
  {
    m_mark.m_WSB.m_north = iMarkOrigin.m_north;
    m_mark.m_ENA.m_north = m_cursor.m_north;
  }
  else if ( m_cursor.m_north < iMarkOrigin.m_north )
  {
    m_mark.m_WSB.m_north = m_cursor.m_north;
    m_mark.m_ENA.m_north = iMarkOrigin.m_north;
  } 
     
  if ( m_cursor.m_east >= iMarkOrigin.m_east )
  {
    m_mark.m_WSB.m_east = iMarkOrigin.m_east;
    m_mark.m_ENA.m_east = m_cursor.m_east;
  }
  else if ( m_cursor.m_east < iMarkOrigin.m_east )
  {
    m_mark.m_WSB.m_east = m_cursor.m_east;
    m_mark.m_ENA.m_east = iMarkOrigin.m_east;
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
  
  if  (( m_mark.m_ENA.m_above == m_mark.m_WSB.m_above )
    && ( m_mark.m_ENA.m_north == m_mark.m_WSB.m_north )
    && ( m_mark.m_ENA.m_east == m_mark.m_WSB.m_east ))
  {
    m_mark.m_ENA = m_cursor;
    m_mark.m_WSB = m_cursor;
  }

  // Move all marked volume objects to a buffer.

  std::list<std::list<CMapObject>::iterator> Marked;
  std::list<CMapObject> buffer;

  iMap->iObjects.Read( iMark, Marked );
  iMap->iObjects.Remove( Marked, buffer );

  // Move all marked volume items to a buffer.
  
  Marked.clear();

  iMap->iItems.Read( iMark, Marked );
  iMap->iItems.Remove( Marked, buffer );

  // Move all marked volume players to the buffer.
  
  Marked.clear();
  
  iMap->iPlayers.Read( iMark, Marked );
  iMap->iPlayers.Remove( Marked, buffer );

  // Move all marked volume teleporters to the buffer.
  
  Marked.clear();
  
  iMap->iTeleporters.Read( iMark, Marked );
  iMap->iTeleporters.Remove( Marked, buffer );

  // Unmark the marked volume.
  
  m_mark.m_ENA = m_cursor;
  m_mark.m_WSB = m_cursor;
  
  // Update the view to show the changes.

  Update();
  return;
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
  
  for (object = m_edit_buffer.begin(); object != m_edit_buffer.end(); ++ object)
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

  std::list<CMapObject>::iterator object;
  
  for (object = m_edit_buffer.begin(); object != m_edit_buffer.end(); ++ object)
  {
    (*Object).m_position.m_above =
      (*Object).m_position.m_above - m_mark.m_WSB.m_above;
    
    (*Object).m_position.m_north =
      (*Object).m_position.m_north - m_mark.m_WSB.m_north;
    
    (*Object).m_position.m_east =
      (*Object).m_position.m_east - m_mark.m_WSB.m_east;
  }

  // A marked volume will remain marked to show the volume that was copied.
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
  
  for (object = m_edit_buffer.begin(); object != m_edit_buffer.end(); ++ object )
  {
    new_object = (*object);
   
    // Prevent a wrap-around of the map location if pasting near
    // a map boundary.
 
    if  ((new_object.m_position.m_above <= (65535 - m_cursor.m_above))
      && (new_object.m_position.m_north <= (65535 - m_cursor.m_north)) 
      && (new_object.m_position.m_east <= (65535 - m_cursor.m_east)))
    { 
      new_object.m_position.m_above  += m_cursor.m_above;
      new_object.m_position.m_north    += m_cursor.m_north;
      new_object.m_position.m_east += m_cursor.m_east;
  
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
// This method returns the map location signal.
//---------------------------------------------

Enigma::LevelView::type_signal_location Enigma::LevelView::signal_location()
{
  return m_signal_location;
}

//-----------------------------------------------------------
// This method emits a signal containing the cursor location.
//-----------------------------------------------------------

void Enigma::LevelView::do_location()
{	  
  m_signal_location.emit(m_cursor);
}
