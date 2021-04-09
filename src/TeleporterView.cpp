/*--------------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker     *
 * Version: 5.0 for Linux OS                        *
 * File:    TeleporterView.cpp                      *
 * Date:    September 13, 2016                      *
 * Author:  Chris Sterne                            *
 *                                                  *
 * This class displays all teleporters in the map.  *
 *--------------------------------------------------*/

#include <glibmm/i18n.h>
#include "TeleporterView.h"

//*----------------------*
//* Default constructor. *
//*----------------------*

CTeleporterView::CTeleporterView()
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
	                                   &CTeleporterView::Object_Data_Function )); 

	// Connect a key press event handler to the RoomView, but place it before
	// its default handler.  This allows capturing general key press events.
	
	signal_key_press_event().connect( sigc::mem_fun( *this,
	                         &CTeleporterView::On_Key_Press ),
	                         FALSE );

	// Add a signal handler for TreeView cursor changes.

  iTreeView->signal_cursor_changed().connect(sigc::mem_fun(*this,
                                     &CTeleporterView::On_Cursor_Changed ));
	
	return;
}

//*-------------------------------------------------------------*
//* This method is called when the widget is about to be shown. *
//*-------------------------------------------------------------*

void CTeleporterView::on_map()
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

void CTeleporterView::Object_Data_Function( Gtk::CellRenderer* const& aCellRenderer,
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

void CTeleporterView::SetMap( std::shared_ptr<CMap> aMap )
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

gboolean CTeleporterView::On_Key_Press( GdkEventKey* key_event )
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
			
			iMap->iTeleporters.Erase( Object );
		}
	}
	else
		Handled = FALSE;
		
	return Handled;
}

//*------------------------------------------------------------*
//* This signal handler is called when the cursor changes row. *
//* The teleporter's location is emitted in a signal.          *
//*------------------------------------------------------------*

void CTeleporterView::On_Cursor_Changed()
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

//*-------------------------------*
//* This method updates the view. *
//*-------------------------------*

void CTeleporterView::Update()
{
  // Detach the model from the TreeView and clear all old entries.
  
  iTreeView->unset_model();
  iListStore->clear();

  // Populate the ListStore with iterators to all teleporters in the map.
	
  Gtk::TreeModel::Row Row;	
  std::list<CMapObject>::iterator Object;

  for ( Object = iMap->iTeleporters.begin();
        Object != iMap->iTeleporters.end();
        ++ Object )
  {
    Row = *( iListStore->append() );
    Row[ iColumnRecord.iIterator ] = Object;
  }

  // Attach the filled model to the TreeView.

  iTreeView->set_model( iListStore );
	return;
}

//*-----------------------------------------------------*
//* This method returns the teleporter location signal. *
//*-----------------------------------------------------*

CTeleporterView::type_signal_location CTeleporterView::signal_location()
{
  return m_signal_location;
}

//*------------------------------------------------------------------*
//* This method emits a signal containing the teleporter's location. *
//*------------------------------------------------------------------*

void CTeleporterView::Do_Location( CMapLocation aLocation)
{	  
  m_signal_location.emit( aLocation );
  return;
}
