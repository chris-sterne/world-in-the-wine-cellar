// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the PlayerView class implementation.  The PlayerView
// class displays and allows editing players in the world.
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
#include "PlayerView.h"

//*----------------------*
//* Default constructor. *
//*----------------------*

CPlayerView::CPlayerView()
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
	                                   &CPlayerView::Object_Data_Function )); 

	// Connect a key press event handler to the RoomView, but place it before
	// its default handler.  This allows capturing general key press events.
	
	signal_key_press_event().connect( sigc::mem_fun( *this,
	                         &CPlayerView::On_Key_Press ),
	                         FALSE );

	// Add a signal handler for TreeView cursor changes.

  iTreeView->signal_cursor_changed().connect(sigc::mem_fun(*this,
                                     &CPlayerView::On_Cursor_Changed ));
	
	return;
}

//*-------------------------------------------------------------*
//* This method is called when the widget is about to be shown. *
//*-------------------------------------------------------------*

void CPlayerView::on_map()
{
  // Pass the method to the base class.

  Gtk::ScrolledWindow::on_map();

  // Update the player list before it is displayed.
  
  Update();
  return;
}

//*----------------------------------------------------------------*
//* This method is called when MapObject cells are to be rendered. *
//* Data is read from the model and converted into an appropriate  *
//* content for display.                                           *
//*----------------------------------------------------------------*

void CPlayerView::Object_Data_Function( Gtk::CellRenderer* const& aCellRenderer,
                         				          const Gtk::TreeIter& aTreeIterator )
{	
	// Exit if there is no iterator.

	if ( !aTreeIterator )
		return;
	
	Gtk::TreeModel::Row Row = *aTreeIterator;
	Gtk::CellRendererText* CellRenderer = (Gtk::CellRendererText*)( aCellRenderer );

	// Set the text.

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

void CPlayerView::SetMap( std::shared_ptr<CMap> aMap )
{
	iMap = aMap;
	return;
}

//*------------------------------------------------------*
//* Method to handle key press events from the ItemView. *
//*------------------------------------------------------*
//* key_event: Pointer to GdkEventKey.                   *
//* RETURN:    TRUE if key press was handled.            *
//*------------------------------------------------------*

gboolean CPlayerView::On_Key_Press( GdkEventKey* key_event )
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
			// An iterator for a selected item object is available.

			Gtk::TreeModel::Row Row = *TreeIterator;
			std::list<CMapObject>::iterator Object;
			Object = Row[ iColumnRecord.iIterator ];

      // Erase the selected entry from the ListStore before erasing the object
      // from the map.  This ensures the iterator in the ListStore entry remains
      // valid while the entry is being erased.  Erasing the ListStore entry
      // will result in an update to the TreeView.
      
      iListStore->erase( Row );

			// Erase the selected player object from the map.
			
			iMap->iPlayers.Erase( Object );
		}
	}
	else
		Handled = FALSE;
		
	return Handled;
}

//*------------------------------------------------------------*
//* This signal handler is called when the cursor changes row. *
//* The player's location is emitted in a signal.              *
//*------------------------------------------------------------*

void CPlayerView::On_Cursor_Changed()
{
	// Get an iterator to the highlighted (selected) entry.

	Glib::RefPtr< Gtk::TreeSelection > Selection = iTreeView->get_selection(); 
	Gtk::TreeModel::iterator Iterator = Selection->get_selected();

	if ( Iterator )
  {
		Gtk::TreeModel::Row Row = *Iterator;
    std::list<CMapObject>::iterator Object;
    Object = Row[ iColumnRecord.iIterator ];
		
		Do_Location( (*Object).iLocation );
	}
	
	return;
}

//------------------------------
// This method updates the view.
//------------------------------

void Enigma::PlayerView::update()
{
  // Detach the model from the TreeView and clear all old entries.
  
  m_treeview->unset_model();
  m_liststore->clear();

  // Populate the ListStore with iterators to all players in the map.
	
  Gtk::TreeModel::Row row;	
  std::list<CMapObject>::iterator object;

  for (object = m_world->m_players.begin();
       object != m_world->m_players.end();
       ++ object)
  {
    Row = *(m_liststore->append());
    Row[m_columnrecord.m_iterator] = object;
  }

  // Attach the filled model to the TreeView.

  m_treeview->set_model(m_liststore);
}

//----------------------------------------------
// This method returns the item location signal.
//----------------------------------------------

PlayerView::type_signal_position Enigma::PlayerView::signal_position()
{
  return m_signal_position;
}

//-----------------------------------------------------------
// This method emits a signal containing the item's location.
//-----------------------------------------------------------

void Enigma::PlayerView::do_location(Enigma::Position position)
{	  
  m_signal_position.emit(position);
}
