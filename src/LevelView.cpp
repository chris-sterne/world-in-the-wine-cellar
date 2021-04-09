/*-----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker  *
 * Version: 5.0 for Linux OS                     *
 * File:    LevelView.cpp                        *
 * Date:    September 15, 2016                   *
 * Author:  Chris Sterne                         *
 *                                               *
 * LevelView class header.                       *
 *-----------------------------------------------*
 * This class displays the levels of a game map. *
 *-----------------------------------------------*/

#include <glibmm/i18n.h>
#include "LevelView.h"

//*--------------------*
//* Local declarations *
//*--------------------*

#define KBackgroundR 0                 // Background colour.
#define KBackgroundG 0
#define KBackgroundB 0

//*--------------------------*
//* C++ default constructor. *
//*--------------------------*

CLevelView::CLevelView()
{	
  set_has_window(TRUE);
  set_hexpand(TRUE);
  set_vexpand(TRUE);
  add_events( Gdk::KEY_PRESS_MASK );
  set_can_focus( TRUE );

  iCursor.iEast  = 0;
  iCursor.iNorth = 0;
  iCursor.iAbove = 0;
 
  iView.iWSB = iCursor;
  iView.iENA = iCursor;

  iMark.iWSB = iCursor;
  iMark.iENA = iCursor;
  
  iFilter = EnigmaWC::ID::ENone;
  return;
}

//*-------------------------*
//* C++ default destructor. *
//*-------------------------*

CLevelView::~CLevelView()
{
	return;
}

//*-----------------------------------------------------*
//* Method to get preferred width of PlayerView widget. *
//*-----------------------------------------------------*

void CLevelView::get_preferred_width_vfunc(int& minimum_width, int& natural_width) const
{
  minimum_width = iImageTiles.GetTileSize() * 3;
  natural_width = minimum_width;
  return;
}

//*------------------------------------------------------*
//* Method to get preferred height of PlayerView widget. *
//*------------------------------------------------------*

void CLevelView::get_preferred_height_vfunc(int& minimum_height, int& natural_height) const
{
  minimum_height = iImageTiles.GetTileSize() * 3;
  natural_height = minimum_height;
  return;
}

//*---------------------------------------------------------------*
//* This private function adjusts a number range in order to keep *
//* a particular number within the range, and is used to keep     *
//* the cursor visible if it leaves the view range.               *
//*---------------------------------------------------------------*
//* aNumber: Number to be kept within range.                      *
//* aLower:  Lower limit of range.                                *
//* aUpper:  Upper limit of range.                                *
//*---------------------------------------------------------------*

void AlignRange( guint16 aNumber, guint16& aLower, guint16& aUpper )
{	
	if ( aLower > aUpper )
	{
		// The range limits are not valid, so exchange them.

		guint16 Temporary = aLower;
		aLower = aUpper;
		aUpper = Temporary;
	}
	
	if ( aNumber <= aLower )
	{
		// The number falls on or below the lower limit.
		// Shift the range to center the number within it.

		guint16 Difference     = aUpper - aLower;
		guint16 HalfDifference = Difference / 2;

		if ( aNumber >= HalfDifference )
		{
			// The range length is long enough to be centered around the number.
			// Set the lower limit to the half-way point.
		
			aLower = aNumber - HalfDifference;
			aUpper = aLower + Difference;
		}
		else
		{
			// The range length is too short to be centered around number.
			// Set the lower limit to its minimum value (zero).

			aLower = 0;
			aUpper = Difference;
		}
	}
	else if ( aNumber >= aUpper )
	{
		// The number falls on or above the upper limit.
		// Shift the range to center the number within it.

		guint16 Difference     = aUpper - aLower;
		guint16 HalfDifference = Difference / 2;

		if (( G_MAXUINT16 - aNumber ) >= HalfDifference )
		{
			// The upper limit can be increased to center the number within the range,
			// and also not exceed the maximum range value.

			aUpper = aNumber + HalfDifference;
			aLower = aUpper - Difference;
		}
		else
		{
			// The range is too wide to center the number within the range.
			// Set the upper range limit to its maximum value.

			aUpper = G_MAXUINT16;
			aLower = aUpper - Difference;
		}
	}
	
	return;
}

//*-----------------------------------------------------------------*
//* This method is called when the widget has been allocate a size. *
//*-----------------------------------------------------------------*

void CLevelView::on_size_allocate( Gtk::Allocation& allocation )
{	
	Gtk::Widget::on_size_allocate( allocation );
	
	guint16 Width  = allocation.get_width() / iImageTiles.GetTileSize();
	guint16 Height = allocation.get_height() / iImageTiles.GetTileSize();

	// Set initial view row and column ranges with correct lengths.

	iView.iWSB.iEast  = 0;
	iView.iENA.iEast  = Width;	
	iView.iWSB.iNorth = 0;
	iView.iENA.iNorth = Height;

	// Align the view row and column ranges around the cursor.
	
	AlignRange( iCursor.iEast, iView.iWSB.iEast, iView.iENA.iEast );
	AlignRange( iCursor.iNorth, iView.iWSB.iNorth, iView.iENA.iNorth );
	return;
}

//*-------------------------------------------------------------*
//* This method is called when the widget is about to be shown. *
//*-------------------------------------------------------------*

void CLevelView::on_map()
{
	// Pass the method to the base class.

	Gtk::DrawingArea::on_map();
	
	// Update cursor location information.
	
	Do_Location();
	return;
}

//*-----------------------------------*
//* Method to draw PlayerView widget. *
//*-----------------------------------*

bool CLevelView::on_draw( const Cairo::RefPtr<Cairo::Context>& aCC )
{	
  // Draw the background colour.

  aCC->set_source_rgb( KBackgroundR, KBackgroundG, KBackgroundB );
  aCC->paint();

  Gtk::Allocation Allocation = get_allocation();
  CMapLocation Room;
  std::list<std::list<CMapObject>::iterator> Buffer;
  std::list<std::list<CMapObject>::iterator>::iterator Object;
  gboolean Drawn;
  
  guint16 Row;
  guint16 Column; 
  
  Room.iAbove = iCursor.iAbove;
	
  for ( Room.iNorth = iView.iWSB.iNorth;
        Room.iNorth <= iView.iENA.iNorth;
        ++ Room.iNorth )
  {
    for ( Room.iEast = iView.iWSB.iEast;
          Room.iEast <= iView.iENA.iEast;
          ++ Room.iEast )
    {
      Row    = Room.iNorth - iView.iWSB.iNorth;
      Column = Room.iEast - iView.iWSB.iEast;
			
      // Read all MapObjects in the room.

      Buffer.resize(0);
      iMap->iObjects.Read( Room, Buffer );
      iMap->iItems.Read( Room, Buffer );
      iMap->iPlayers.Read( Room, Buffer );
      iMap->iTeleporters.Read( Room, Buffer );
      
      for ( Object = Buffer.begin(); Object != Buffer.end(); ++ Object )
      {
        // Draw all environment objects first.
			
        if (((*(*Object)).iID == EnigmaWC::ID::EOutdoor )
          || ((*(*Object)).iID == EnigmaWC::ID::EIndoor ))
        {
          if  (( iFilter == EnigmaWC::ID::ENone )
            || ( iFilter == (*(*Object)).iID ))
          {
            Drawn = iImageTiles.DrawMapObject( aCC,
                                               Allocation,
                                               Column,
                                               Row,
                                               *(*Object) );					
            if ( !Drawn )
              iImageTiles.DrawGeneric( aCC, Allocation, Column, Row );
          }
            
          // Erase environment object so it will not drawn again.
	
          Object = Buffer.erase( Object );
        }
      }
      
      for ( Object = Buffer.begin(); Object != Buffer.end(); ++ Object )
      {
        // Draw all floor objects first.
			
        if ((((*(*Object)).iID == EnigmaWC::ID::EBlockWall )
          || ((*(*Object)).iID == EnigmaWC::ID::EStoneWall ))
          && ((*(*Object)).iSurface == EnigmaWC::Direction::EBelow ))
          
        {
          if  (( iFilter == EnigmaWC::ID::ENone )
            || ( iFilter == (*(*Object)).iID ))
          {
            Drawn = iImageTiles.DrawMapObject( aCC,
                                               Allocation,
                                               Column,
                                               Row,
                                               *(*Object) );					
            if ( !Drawn )
              iImageTiles.DrawGeneric( aCC, Allocation, Column, Row );
          }
            
          // Erase floor object so it will not drawn again.
	
          Object = Buffer.erase( Object );
        }
      }

      // Draw all objects above the floor.

      for ( Object = Buffer.begin(); Object != Buffer.end(); ++ Object )
      {
        if  (( iFilter == EnigmaWC::ID::ENone )
          || ( iFilter == (*(*Object)).iID ))
        {
          Drawn = iImageTiles.DrawMapObject( aCC,
                                             Allocation,
                                             Column,
                                             Row,
                                             *(*Object ));                       
          if ( !Drawn )
            iImageTiles.DrawGeneric( aCC, Allocation, Column, Row );
        }
      }
      
      // Draw a marker if the current room location falls within the marked area
      // and the marked area is greater than one room in volume.
  
      if ((( iMark.iENA.iAbove != iMark.iWSB.iAbove )
        || ( iMark.iENA.iNorth != iMark.iWSB.iNorth )
        || ( iMark.iENA.iEast != iMark.iWSB.iEast ))
        && ( Room.iAbove >= iMark.iWSB.iAbove )
        && ( Room.iAbove <= iMark.iENA.iAbove )
        && ( Room.iNorth >= iMark.iWSB.iNorth )
        && ( Room.iNorth <= iMark.iENA.iNorth )
        && ( Room.iEast >= iMark.iWSB.iEast )
        && ( Room.iEast <= iMark.iENA.iEast ))
      {
        iImageTiles.DrawMarker( aCC, Allocation, Column, Row );
      }
    }
  }

  // Draw all teleporter arrival marks that fall within the view. 

  std::list<CMapObject>::iterator Teleporter;

  guint16 EastArrival;
  guint16 NorthArrival;
  guint16 AboveArrival;
    
  for ( Teleporter = iMap->iTeleporters.begin();
        Teleporter != iMap->iTeleporters.end();
        ++ Teleporter )
  {
    EastArrival = (*Teleporter).iLocationArrival.iEast;
    NorthArrival = (*Teleporter).iLocationArrival.iNorth;
    AboveArrival = (*Teleporter).iLocationArrival.iAbove;
    
    // If an arrival location is set to its maximum value, the arrival
    // location is the player's (and teleporter's) current location.
    
    if ( EastArrival == G_MAXUINT16 )
      EastArrival = (*Teleporter).iLocation.iEast;
      
    if ( NorthArrival == G_MAXUINT16 )
      NorthArrival = (*Teleporter).iLocation.iNorth;
      
    if ( AboveArrival == G_MAXUINT16 )
      AboveArrival = (*Teleporter).iLocation.iAbove;
    
    if  (( EastArrival >= iView.iWSB.iEast )
      && ( EastArrival <= iView.iENA.iEast )
      && ( NorthArrival >= iView.iWSB.iNorth )
      && ( NorthArrival <= iView.iENA.iNorth )
      && ( AboveArrival == Room.iAbove ))
    {
      Column = EastArrival - iView.iWSB.iEast;
      Row    = NorthArrival - iView.iWSB.iNorth;

      iImageTiles.DrawArrival( aCC, Allocation, Column, Row );
    }
  }

  // Draw the cursor last.
	
  Column = iCursor.iEast - iView.iWSB.iEast;
  Row    = iCursor.iNorth - iView.iWSB.iNorth;

  iImageTiles.DrawCursor( aCC, Allocation, Column, Row );
  
  // Return TRUE to indicate all drawing has been done.

  return TRUE;
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
          if ( iCursor.iNorth < G_MAXUINT16 )
          {
            ++ iCursor.iNorth;
            Do_Location();

            // Make sure the cursor falls within the visible rows.
				
            AlignRange( iCursor.iNorth, iView.iWSB.iNorth, iView.iENA.iNorth );
                        			    
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
          if ( iCursor.iNorth > 0 )
          {           
            -- iCursor.iNorth;
            Do_Location();

            // Make sure the cursor falls within the visible rows.
				
            AlignRange( iCursor.iNorth, iView.iWSB.iNorth, iView.iENA.iNorth );
            
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
          if ( iCursor.iEast > 0 )
          {
            iCursor.iEast --;
            Do_Location();

            // Make sure the cursor falls within the visible columns.
				
            AlignRange( iCursor.iEast, iView.iWSB.iEast, iView.iENA.iEast );
            
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
          if ( iCursor.iEast < G_MAXUINT16 )
          {           
            iCursor.iEast ++;
            Do_Location();

            // Make sure the cursor falls within the visible columns.
				
            AlignRange( iCursor.iEast, iView.iWSB.iEast, iView.iENA.iEast );
            
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
          if ( iCursor.iAbove > 0 )
          {          
            iCursor.iAbove --;
            Do_Location();

            // Make sure the cursor falls within the visible level.
				
            iView.iWSB.iAbove = iCursor.iAbove;
            iView.iENA.iAbove = iCursor.iAbove;
            
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
          if ( iCursor.iAbove < G_MAXUINT16 )
          {          
            iCursor.iAbove ++;
            Do_Location();

            // Make sure the cursor falls within the visible level.
				
            iView.iWSB.iAbove = iCursor.iAbove;
            iView.iENA.iAbove = iCursor.iAbove;
            
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
	        // Delete the marked volume.
	        
          Delete();  
          break;
        
        case GDK_KEY_Escape:
        case GDK_KEY_Shift_L:
        case GDK_KEY_Shift_R:
          // Begin volume marking, or clear the current marking.  Since this
          // must occur only once when starting to mark a volume,  the SHIFT
          // key must not repeat while being pressed.

          iMarkOrigin = iCursor;
          Mark();
          break;

        default:
          break;
      }  
    }
	}

	return Handled;
}

//*---------------------------------------------*
//* This method sets the game map to be viewed. *
//*---------------------------------------------*
//* aMap: Game map.                             *
//*---------------------------------------------*

void CLevelView::SetMap( std::shared_ptr<CMap> aMap )
{
	iMap = aMap;
	return;
}

//*----------------------------------------------------------------*
//* This method sets an object view filter.                        *
//*----------------------------------------------------------------*
//* aFilter: ID of object to filter, or ENone to show all objects. *
//*----------------------------------------------------------------*

void CLevelView::SetFilter( EnigmaWC::ID aFilter )
{
	iFilter = aFilter;
	
	// Update the view to show the result of the object filter.

  Update();
	return;
}

//*---------------------------------------------------------*
//* This method returns a reference to the cursor location. *
//*---------------------------------------------------------*

CMapLocation& CLevelView::GetCursor()
{
	return iCursor;
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
    
      iCursor = (*Object).iLocation;
      break;
    }
  }
  
  if ( Object == iMap->iPlayers.end() )
  {
    // There are no active players present, so try the first player.
    
    Object = iMap->iPlayers.begin();
    
    if ( Object != iMap->iPlayers.end() )
      iCursor = (*Object).iLocation;
    else   
    {
      // There are no players present, so try the first item.
    
      Object = iMap->iItems.begin();
    
      if ( Object != iMap->iItems.end() )
        iCursor = (*Object).iLocation;
      else
      {
        // There are no items present, so try the first teleporter.
        
        Object = iMap->iTeleporters.begin();
        
        if ( Object != iMap->iTeleporters.end() )
          iCursor = (*Object).iLocation;
        else
        {
          // There are no teleporters present, so try the first structural
          // object.
      
          Object = iMap->iObjects.begin();
      
          if ( Object != iMap->iObjects.end() )
            iCursor = (*Object).iLocation;
          else
          {
            // There are no objects in the map.  Set the cursor to a location
            // of zero.
        
            iCursor.iAbove = 0;
            iCursor.iNorth = 0;
	          iCursor.iEast  = 0;
	        }
        }
      }
    }
  }

  // Report changes to the cursor location.
  
  Do_Location();

  // Adjust the visible view volume so the cursor is visible.

  AlignRange( iCursor.iNorth, iView.iWSB.iNorth, iView.iENA.iNorth );
  AlignRange( iCursor.iEast, iView.iWSB.iEast, iView.iENA.iEast );
  
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

  if ( iCursor.iAbove >= iMarkOrigin.iAbove )
  {
    iMark.iWSB.iAbove = iMarkOrigin.iAbove;
    iMark.iENA.iAbove = iCursor.iAbove;
  }
  else if ( iCursor.iAbove < iMarkOrigin.iAbove )
  {
    iMark.iWSB.iAbove = iCursor.iAbove;
    iMark.iENA.iAbove = iMarkOrigin.iAbove;
  }

  if ( iCursor.iNorth >= iMarkOrigin.iNorth )
  {
    iMark.iWSB.iNorth = iMarkOrigin.iNorth;
    iMark.iENA.iNorth = iCursor.iNorth;
  }
  else if ( iCursor.iNorth < iMarkOrigin.iNorth )
  {
    iMark.iWSB.iNorth = iCursor.iNorth;
    iMark.iENA.iNorth = iMarkOrigin.iNorth;
  } 
     
  if ( iCursor.iEast >= iMarkOrigin.iEast )
  {
    iMark.iWSB.iEast = iMarkOrigin.iEast;
    iMark.iENA.iEast = iCursor.iEast;
  }
  else if ( iCursor.iEast < iMarkOrigin.iEast )
  {
    iMark.iWSB.iEast = iCursor.iEast;
    iMark.iENA.iEast = iMarkOrigin.iEast;
  }
  
  // Update the view to show the changes.

  Update();
	return;
}

//*-----------------------------------------*
//* This method deletes all marked objects. *
//*-----------------------------------------*

void CLevelView::Delete()
{
  // Select the map volume to be deleted.  If the marked volume has a size
  // of only one room, set it to the cursor volume instead.
  
  if  (( iMark.iENA.iAbove == iMark.iWSB.iAbove )
    && ( iMark.iENA.iNorth == iMark.iWSB.iNorth )
    && ( iMark.iENA.iEast == iMark.iWSB.iEast ))
  {
    iMark.iENA = iCursor;
    iMark.iWSB = iCursor;
  }

  // Move all marked volume objects to a buffer.

  std::list<std::list<CMapObject>::iterator> Marked;
  std::list<CMapObject> Buffer;

  iMap->iObjects.Read( iMark, Marked );
  iMap->iObjects.Remove( Marked, Buffer );

  // Move all marked volume items to a buffer.
  
  Marked.clear();

  iMap->iItems.Read( iMark, Marked );
  iMap->iItems.Remove( Marked, Buffer );

  // Move all marked volume players to the buffer.
  
  Marked.clear();
  
  iMap->iPlayers.Read( iMark, Marked );
  iMap->iPlayers.Remove( Marked, Buffer );

  // Move all marked volume teleporters to the buffer.
  
  Marked.clear();
  
  iMap->iTeleporters.Read( iMark, Marked );
  iMap->iTeleporters.Remove( Marked, Buffer );

  // Unmark the marked volume.
  
  iMark.iENA = iCursor;
  iMark.iWSB = iCursor;
  
  // Update the view to show the changes.

  Update();
  return;
}

//*---------------------------------------------------*
//* This method moves all marked objects to a buffer. *
//*---------------------------------------------------*

void CLevelView::Cut()
{  
  // Clear any old objects in the editing buffer.
  
  iEditBuffer.clear();
  
  // Select the map volume to be cut.  If the marked volume has a size
  // of only one room, set it to the cursor volume instead.
  
  if  (( iMark.iENA.iAbove == iMark.iWSB.iAbove )
    && ( iMark.iENA.iNorth == iMark.iWSB.iNorth )
    && ( iMark.iENA.iEast == iMark.iWSB.iEast ))
  {
    iMark.iENA = iCursor;
    iMark.iWSB = iCursor;
  }

  // Move all marked volume objects to the editing buffer.

  std::list<std::list<CMapObject>::iterator> Marked;

  iMap->iObjects.Read( iMark, Marked );
  iMap->iObjects.Remove( Marked, iEditBuffer );

  // Move all marked volume items to the editing buffer.
  
  Marked.clear();

  iMap->iItems.Read( iMark, Marked );
  iMap->iItems.Remove( Marked, iEditBuffer );

  // Move all marked volume players to the edit buffer.
  
  Marked.clear();
  
  iMap->iPlayers.Read( iMark, Marked );
  iMap->iPlayers.Remove( Marked, iEditBuffer );

  // Move all marked volume teleporters to the edit buffer.
  
  Marked.clear();
  
  iMap->iTeleporters.Read( iMark, Marked );
  iMap->iTeleporters.Remove( Marked, iEditBuffer );

  // Change the map location of all objects to be an offset from a map
  // location of (0, 0, 0), corresponding to the West-South-Below corner
  // of the marked volume.

  std::list<CMapObject>::iterator Object;
  
  for ( Object = iEditBuffer.begin(); Object != iEditBuffer.end(); ++ Object )
  {
    (*Object).iLocation.iAbove =
      (*Object).iLocation.iAbove - iMark.iWSB.iAbove;
    
    (*Object).iLocation.iNorth =
      (*Object).iLocation.iNorth - iMark.iWSB.iNorth;
    
    (*Object).iLocation.iEast =
      (*Object).iLocation.iEast - iMark.iWSB.iEast;
  }
  
  // Unmark the marked volume.
  
  iMark.iENA = iCursor;
  iMark.iWSB = iCursor;
  
  // Update the view to show the changes.

  Update();
	return;
}

//*----------------------------------------------------*
//* This method copies all marked objects to a buffer. *
//*----------------------------------------------------*

void CLevelView::Copy()
{
  // Clear any old objects in the editing buffer.
  
  iEditBuffer.clear();

  // Select the map volume to be cut.  If the marked volume has a size
  // of only one room, use the cursor volume instead.
  
  if  (( iMark.iENA.iAbove == iMark.iWSB.iAbove )
    && ( iMark.iENA.iNorth == iMark.iWSB.iNorth )
    && ( iMark.iENA.iEast == iMark.iWSB.iEast ))
  {
    iMark.iENA = iCursor;
    iMark.iWSB = iCursor;
  }

  // Get a copy of all marked objects from all object lists.

  iMap->iObjects.Copy( iMark, iEditBuffer );
  iMap->iItems.Copy( iMark, iEditBuffer );
  iMap->iPlayers.Copy( iMark, iEditBuffer );
  iMap->iTeleporters.Copy( iMark, iEditBuffer );
  
  // Change the map location of all objects to be an offset from a map
  // location of (0, 0, 0), corresponding to the upper-top-left corner
  // of the marked volume.

  std::list<CMapObject>::iterator Object;
  
  for ( Object = iEditBuffer.begin(); Object != iEditBuffer.end(); ++ Object )
  {
    (*Object).iLocation.iAbove =
      (*Object).iLocation.iAbove - iMark.iWSB.iAbove;
    
    (*Object).iLocation.iNorth =
      (*Object).iLocation.iNorth - iMark.iWSB.iNorth;
    
    (*Object).iLocation.iEast =
      (*Object).iLocation.iEast - iMark.iWSB.iEast;
  }

  // A marked volume will remain marked to show the volume that was copied.

	return;
}

//*-----------------------------------------------------------------*
//* This method inserts a copy of all object in the editing buffer, *
//* changing their map location to be relative to the cursor.       *
//*-----------------------------------------------------------------*

void CLevelView::Paste()
{ 
  // Set the map location of all new objects being inserted relative to the
  // map location of the cursor.  The location of the original objects in
  // the editing buffer must not be modified, since they are an offset from 
  // location of (0, 0, 0).

  CMapObject NewObject;
  std::list<CMapObject>::iterator Object;
  
  for ( Object = iEditBuffer.begin(); Object != iEditBuffer.end(); ++ Object )
  {
    NewObject = (*Object);
   
    // Prevent a wrap-around of the map location if pasting near
    // a map boundary.
 
    if  (( NewObject.iLocation.iAbove <= ( G_MAXUINT16 - iCursor.iAbove ))
      && ( NewObject.iLocation.iNorth <= ( G_MAXUINT16 - iCursor.iNorth )) 
      && ( NewObject.iLocation.iEast <= ( G_MAXUINT16 - iCursor.iEast )))
    { 
      NewObject.iLocation.iAbove  += iCursor.iAbove;
      NewObject.iLocation.iNorth    += iCursor.iNorth;
      NewObject.iLocation.iEast += iCursor.iEast;
  
      // Add the new object to the appropriate list in the map.
  
      switch ( NewObject.iType )
      {
        case CMapObject::Type::EObject:
          iMap->iObjects.Insert( NewObject );
          break;
        
        case CMapObject::Type::EItem:
          iMap->iItems.Insert( NewObject );
          break;
        
        case CMapObject::Type::EPlayer:
          iMap->iPlayers.Insert( NewObject );
          break;
        
        case CMapObject::Type::ETeleporter:
          iMap->iTeleporters.Insert( NewObject );
          break;
        
        default:
          break;
      }
    }
  }

  // Update the view to show the changes.

  Update();
	return;
}

//*-------------------------------*
//* This method updates the view. *
//*-------------------------------*

void CLevelView::Update()
{
	queue_draw();
	return;
}

//*----------------------------------------------*
//* This method returns the map location signal. *
//*----------------------------------------------*

CLevelView::type_signal_location CLevelView::signal_location()
{
  return m_signal_location;
}

//*------------------------------------------------------------*
//* This method emits a signal containing the cursor location. *
//*------------------------------------------------------------*

void CLevelView::Do_Location()
{	  
  m_signal_location.emit( iCursor );
  return;
}