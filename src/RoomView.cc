// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the RoomView class implementation.  The RoomView class displays
// and allows editing objects in a world room.
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
#include "RoomView.h"
#include "World.h"

//*----------------------*
//* Default constructor. *
//*----------------------*

CRoomView::CRoomView()
{
	add_events( Gdk::KEY_PRESS_MASK );
	
	iTreeView = std::unique_ptr<Gtk::TreeView>( new Gtk::TreeView );
	add( *iTreeView );
	iTreeView->set_headers_visible( FALSE );
  iTreeView->set_hexpand(TRUE);
  iTreeView->set_vexpand(TRUE);
	iTreeView->set_can_focus(TRUE);

	Glib::RefPtr<Gtk::TreeSelection> TreeSelection = iTreeView->get_selection();
	TreeSelection->set_mode(Gtk::SELECTION_BROWSE);

	// Create the TreeView model, but do not attach it to the TreeView.
	// This will be done later after the model has been filled.
	
	iListStore = Gtk::ListStore::create( iColumnRecord );

	// Create TreeView view.  The column cell has a function added for determining
	// how to display the column data.
	
  Gtk::TreeViewColumn* ObjectColumn = Gtk::manage( new Gtk::TreeViewColumn );
	iTreeView->append_column( *ObjectColumn );
	Gtk::CellRendererText* ObjectCell = Gtk::manage( new Gtk::CellRendererText );
  ObjectColumn->pack_start( *ObjectCell, TRUE );
	ObjectColumn->set_cell_data_func( *ObjectCell,
	                                   sigc::mem_fun( *this,
	                                   &CRoomView::Object_Data_Function )); 

	// Connect a key press event handler to the RoomView, but place it before
	// its default handler.  This allows capturing general key press events.
	
	signal_key_press_event().connect( sigc::mem_fun( *this,
	                         &CRoomView::On_Key_Press ),
	                         FALSE );

	return;
}

//*-------------------------------------------------------------*
//* This method is called when the widget is about to be shown. *
//*-------------------------------------------------------------*

void CRoomView::on_map()
{
	// Pass the method to the base class.

	Gtk::ScrolledWindow::on_map();
	
	// Update the list contents.

	Update();
	return;
}

//*----------------------------------------------------------------*
//* This method is called when MapObject cells are to be rendered. *
//* Data is read from the model and converted into an appropriate  *
//* content for display.                                           *
//*----------------------------------------------------------------*

void CRoomView::Object_Data_Function( Gtk::CellRenderer* const& aCellRenderer,
                         				          const Gtk::TreeIter& aTreeIterator )
{	
	// Exit if there is no iterator.

	if ( !aTreeIterator )
		return;
	
	Gtk::TreeModel::Row Row = *aTreeIterator;
	Gtk::CellRendererText* CellRenderer = (Gtk::CellRendererText*)( aCellRenderer );

	std::list<CMapObject>::iterator Object;
	Object = Row[ iColumnRecord.iIterator ];

	Glib::ustring Description;
	(*Object).GetDescription( Description );
	
	CellRenderer->property_text() = Description;
	return;
}

//*---------------------------------------------*
//* This method sets the game map to be viewed. *
//*---------------------------------------------*
//* aMap: Game map.                             *
//*---------------------------------------------*

void CRoomView::SetMap( std::shared_ptr<CMap> aMap )
{
	iMap = aMap;
	return;
}

//*------------------------------------------------*
//* This method sets the map location of the room. *
//*------------------------------------------------*
//* aLocation: Map location of room.               *
//*------------------------------------------------*

void CRoomView::SetLocation( const CMapLocation& aLocation )
{
  // Update the room location.

  iLocation = aLocation;
  return;
}

//*------------------------------------------------------*
//* Method to handle key press events from the RoomView. *
//*------------------------------------------------------*
//* key_event: Pointer to GdkEventKey.                   *
//* RETURN:    TRUE if key press was handled.            *
//*------------------------------------------------------*

gboolean CRoomView::On_Key_Press( GdkEventKey* key_event )
{
  // Exit immediately if the event is not due to a key press.
  // The event is allowed to propagate to other handlers.

  if ( key_event->type != GDK_KEY_PRESS )
    return FALSE;

  gboolean Handled = TRUE;	
  int KeyValue     = key_event->keyval;

  if ( KeyValue == GDK_KEY_Delete )
  {		
    Glib::RefPtr<Gtk::TreeSelection> TreeSelection = iTreeView->get_selection();
    Gtk::TreeModel::iterator TreeIterator = TreeSelection->get_selected();

    if ( TreeIterator )
    {
      // An iterator for a selected object entry is available.

      Gtk::TreeModel::Row Row = *TreeIterator;
      std::list<CMapObject>::iterator Object;
      Object = Row[ iColumnRecord.iIterator ];

      // Erase the selected entry from the ListStore before erasing the object
      // from the map.  This ensures the iterator in the ListStore entry remains
      // valid while the entry is being erased.  Erasing the ListStore entry
      // will result in an update to the TreeView.
      
      iListStore->erase( Row );

      // Erase the selected object from the correct list in the map.
			
      if ( (*Object).iType == CMapObject::Type::EItem )
        iMap->iItems.Erase( Object );
      else if ( (*Object).iType == CMapObject::Type::EPlayer )
        iMap->iPlayers.Erase( Object );
      else if ( (*Object).iType == CMapObject::Type::ETeleporter )
        iMap->iTeleporters.Erase( Object );
      else
        iMap->iObjects.Erase( Object );
    }
  }
  else
    Handled = FALSE;

  return Handled;
}

//*-------------------------------*
//* This method updates the view. *
//*-------------------------------*

void CRoomView::Update()
{
  // Detach the model from the TreeView and clear all old entries.

  iTreeView->unset_model();
  iListStore->clear();

  // Read iterators to MapObjects in the room from all lists.

  std::list<std::list<CMapObject>::iterator> Buffer;
  
  iMap->iObjects.Read( iLocation, Buffer );
  iMap->iTeleporters.Read( iLocation, Buffer );
  iMap->iItems.Read( iLocation, Buffer );
  iMap->iPlayers.Read( iLocation, Buffer );

  // Populate the ListStore.

  std::list<std::list<CMapObject>::iterator>::iterator Object;
  Gtk::TreeModel::Row Row;
	
  for ( Object = Buffer.begin();
        Object != Buffer.end();
        ++ Object )
  {
    Row = *( iListStore->append() );
    Row[ iColumnRecord.iIterator ] = *Object;
  }

  // Attach the filled model to the TreeView.

  iTreeView->set_model( iListStore );
  return;
}
