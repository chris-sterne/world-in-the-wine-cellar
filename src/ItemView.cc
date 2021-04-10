/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 4.0 for Linux OS                    *
 * File:    ItemView.cpp                        *
 * Date:    September 7, 2016                   *
 * Author:  Chris Sterne                        *
 *                                              *
 * This class displays all items in the map.    *
 *----------------------------------------------*/

#include <glibmm/i18n.h>
#include "ItemView.h"

//*----------------------*
//* Default constructor. *
//*----------------------*

CItemView::CItemView()
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
                                    &CItemView::Object_Data_Function )); 

  // Connect a key press event handler to the RoomView, but place it before
  // its default handler.  This allows capturing general key press events.
	
  signal_key_press_event().connect( sigc::mem_fun( *this,
                                    &CItemView::On_Key_Press ),
                                    FALSE );

  //Connect signal for double-click row activation.
	
  iTreeView->signal_row_activated().connect(sigc::mem_fun(*this,
                                    &CItemView::On_Row_Activated) );

  // Add a signal handler for TreeView cursor changes.

  iTreeView->signal_cursor_changed().connect(sigc::mem_fun(*this,
                                     &CItemView::On_Cursor_Changed ));

  return;
}

//*-------------------------------------------------------------*
//* This method is called when the widget is about to be shown. *
//*-------------------------------------------------------------*

void CItemView::on_map()
{
	// Pass the method to the base class.

  Gtk::ScrolledWindow::on_map();

  // Update the item list before it is displayed.

  Update();
  return;
}

//*----------------------------------------------------------------*
//* This method is called when MapObject cells are to be rendered. *
//* Data is read from the model and converted into an appropriate  *
//* content for display.                                           *
//*----------------------------------------------------------------*

void CItemView::Object_Data_Function( Gtk::CellRenderer* const& aCellRenderer,
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

void CItemView::SetMap( std::shared_ptr<CMap> aMap )
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

gboolean CItemView::On_Key_Press( GdkEventKey* key_event )
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

			// Erase the selected item object from the map.
			
			iMap->iItems.Erase( Object );
		}
	}
	else
		Handled = FALSE;
		
	return Handled;
}

//*------------------------------------------------------------*
//* This signal handler is called when the cursor changes row. *
//* The item's location is emitted in a signal.                *
//*------------------------------------------------------------*

void CItemView::On_Cursor_Changed()
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

//*-------------------------------------------------------------*
//* This method is called when a game map list row is activated *
//* (Double-clicked) to go to the item's location.              *
//*-------------------------------------------------------------*

void CItemView::On_Row_Activated( const Gtk::TreeModel::Path& aPath,
                                  Gtk::TreeViewColumn* aColumn )
{
	Gtk::TreeModel::iterator Iterator = iListStore->get_iter( aPath );	
	
  if ( Iterator )
  {
		//Gtk::TreeModel::Row Row = *Iterator;
	}

	return;
}

//*-------------------------------*
//* This method updates the view. *
//*-------------------------------*

void CItemView::Update()
{
  // Detach the model from the TreeView and clear all old entries.
  
  iTreeView->unset_model();
  iListStore->clear();

  // Populate the ListStore with iterators to all items in the map.
	
  Gtk::TreeModel::Row Row;	
  std::list<CMapObject>::iterator Object;

  // Add all required items.
  
  for ( Object = iMap->iItems.begin();
        Object != iMap->iItems.end();
        ++ Object )
  {
    if ( (*Object).iCategory == EnigmaWC::Category::ERequired )
    {
      Row = *( iListStore->append() );
      Row[ iColumnRecord.iIterator ] = Object;
    }
  }
  
  // Add all optional items.
  
  for ( Object = iMap->iItems.begin();
        Object != iMap->iItems.end();
        ++ Object )
  {
    if ( (*Object).iCategory == EnigmaWC::Category::EOptional )
    {
      Row = *( iListStore->append() );
      Row[ iColumnRecord.iIterator ] = Object;
    }
  }
  
  // Add all Easter Egg items.
  
  for ( Object = iMap->iItems.begin();
        Object != iMap->iItems.end();
        ++ Object )
  {
    if ( (*Object).iCategory == EnigmaWC::Category::EEasterEgg )
    {
      Row = *( iListStore->append() );
      Row[ iColumnRecord.iIterator ] = Object;
    }
  }
  
  // Add all skull items.
  
  for ( Object = iMap->iItems.begin();
        Object != iMap->iItems.end();
        ++ Object )
  {
    if ( (*Object).iCategory == EnigmaWC::Category::ESkull )
    {
      Row = *( iListStore->append() );
      Row[ iColumnRecord.iIterator ] = Object;
    }
  }

  // Attach the filled model to the TreeView.

  iTreeView->set_model( iListStore );
  return;
}

//*-----------------------------------------------*
//* This method returns the item location signal. *
//*-----------------------------------------------*

CItemView::type_signal_location CItemView::signal_location()
{
  return m_signal_location;
}

//*------------------------------------------------------------*
//* This method emits a signal containing the item's location. *
//*------------------------------------------------------------*

void CItemView::Do_Location( CMapLocation aLocation)
{	  
  m_signal_location.emit( aLocation );
  return;
}