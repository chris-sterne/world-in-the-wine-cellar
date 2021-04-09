/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 4.0 for Linux OS                    *
 * File:    ControllerView.cpp                  *
 * Date:    September 7, 2016                   *
 * Author:  Chris Sterne                        *
 *                                              *
 * This class displays a list of controllers.   *
 *----------------------------------------------*/

#include <glibmm/i18n.h>
#include "ControllerView.h"

//*----------------------*
//* Default constructor. *
//*----------------------*

CControllerView::CControllerView()
{
  //add_events( Gdk::KEY_PRESS_MASK );
	
  set_show_tabs( FALSE );
  set_show_border( FALSE );
	
	// Prepare a scrolled window containing a list of map controllers.
	
	iControllerWindow
	  = std::unique_ptr<Gtk::ScrolledWindow>( new Gtk::ScrolledWindow );

  iControllerList = std::unique_ptr<Gtk::TreeView>( new Gtk::TreeView );
  iControllerWindow->add( *iControllerList );
     
  iControllerList->set_headers_visible( FALSE );
  iControllerList->set_hexpand(TRUE);
  iControllerList->set_vexpand(TRUE);
  iControllerList->set_can_focus(TRUE);
  
  //Connect signal for double-click list row activation.
	
  iControllerList->signal_row_activated().connect(
                   sigc::mem_fun( *this,
                   &CControllerView::On_List_Row_Activated) );
    
  // Connect a key press event handler to the controller list, but place it
  // before its default handler.  This allows capturing general key press
  // events.
	
  iControllerList->signal_key_press_event().connect(
                   sigc::mem_fun( *this,
                   &CControllerView::On_List_Key_Press ),
                   FALSE );
 
  Glib::RefPtr<Gtk::TreeSelection> TreeSelection
    = iControllerList->get_selection();

  TreeSelection->set_mode(Gtk::SELECTION_BROWSE);
	
  // Create the TreeView model, but do not attach it to the TreeView.
  // This will be done later after the model has been filled.
	
  iListStore = Gtk::ListStore::create( iColumnRecord );	

  // Create TreeView view.  The column cell has a function added for determining
  // how to display the column data.
	
  Gtk::TreeViewColumn* Column = Gtk::manage( new Gtk::TreeViewColumn );
  iControllerList->append_column( *Column );
  Gtk::CellRendererText* Cell = Gtk::manage( new Gtk::CellRendererText );
  Column->pack_start( *Cell, TRUE );
  Column->set_cell_data_func( *Cell,
                              sigc::mem_fun( *this,
                              &CControllerView::Data_Function )); 

  // Prepare a scrolled window containing a controller sourcecode editor.

  iCodeWindow
	  = std::unique_ptr<Gtk::ScrolledWindow>( new Gtk::ScrolledWindow );

  iCodeEditor = std::unique_ptr<Gtk::TextView>( new Gtk::TextView );
  iCodeWindow->add( *iCodeEditor );
	
	iCodeEditor->set_wrap_mode( Gtk::WRAP_WORD );
  iCodeEditor->set_editable(TRUE);
  iCodeEditor->set_cursor_visible(TRUE);
  iCodeEditor->set_hexpand(TRUE);
  iCodeEditor->set_vexpand(TRUE);
  iCodeEditor->set_can_focus(TRUE);

  // Connect a key press event handler to the code editor, but place it
  // before its default handler.  This allows capturing general key press
  // events.
	
  iCodeEditor->signal_key_press_event().connect(
               sigc::mem_fun( *this,
               &CControllerView::On_Editor_Key_Press ),
               FALSE );

  // Add pages to the notebook.
  
  iControllerPageNumber = append_page( *iControllerWindow );
  iCodePageNumber       = append_page( *iCodeWindow );

  // The first displayed page shows the controller list.

  set_current_page( iControllerPageNumber );
  return;
}

//*-------------------------------------------------------------*
//* This method is called when the widget is about to be shown. *
//*-------------------------------------------------------------*

void CControllerView::on_map()
{
  // Pass the method to the base class.
  
  Gtk::Notebook::on_map();

  // Update the current page.

  Update();
  return;
}

//*---------------------------------------------------------------*
//* This method is called when ListView cells are to be rendered. *
//* Data is read from the model and converted into an appropriate *
//* content for display.                                          *
//*---------------------------------------------------------------*

void CControllerView::Data_Function( Gtk::CellRenderer* const& aCellRenderer,
                                     const Gtk::TreeIter& aTreeIterator )
{	
  // Exit if there is no iterator.

  if ( !aTreeIterator )
    return;
	
  Gtk::TreeModel::Row Row = *aTreeIterator;
  Gtk::CellRendererText* CellRenderer = (Gtk::CellRendererText*)( aCellRenderer );

  // Set the text.

  std::list<CMapController>::iterator Controller;
  Controller = Row[ iColumnRecord.iIterator ];

  CellRenderer->property_text() = (*Controller).iName;
  return;
}

//*---------------------------------------------*
//* This method sets the game map to be viewed. *
//*---------------------------------------------*
//* aMap: Game map.                             *
//*---------------------------------------------*

void CControllerView::SetMap( std::shared_ptr<CMap> aMap )
{
  iMap      = aMap;
  iSelected = iMap->iControllers.end();
  return;
}

//*-------------------------------------------------------------*
//* Method to handle key press events from the controller list. *
//*-------------------------------------------------------------*
//* key_event: Pointer to GdkEventKey.                          *
//* RETURN:    TRUE if key press was handled.                   *
//*-------------------------------------------------------------*

gboolean CControllerView::On_List_Key_Press( GdkEventKey* key_event )
{
  // Exit immediately if the event is not due to a key press.
  // The event is allowed to propagate to other handlers.
		
  if ( key_event->type != GDK_KEY_PRESS )
    return FALSE;

  gboolean Handled = TRUE;	
  int KeyValue     = key_event->keyval;

  if ( KeyValue == GDK_KEY_Delete )
  {		
    Glib::RefPtr<Gtk::TreeSelection> TreeSelection
      = iControllerList->get_selection();
    
    Gtk::TreeModel::iterator TreeIterator
      = TreeSelection->get_selected();

    if ( TreeIterator )
    {
      // An iterator for a selected item object is available.

      Gtk::TreeModel::Row Row = *TreeIterator;
      std::list<CMapController>::iterator Controller;
      Controller = Row[ iColumnRecord.iIterator ];

      // Display a dialog confirming the deletion.

      Gtk::Dialog Dialog(_("Controllers") );
      
     	Dialog.add_button(_("Cancel"), Gtk::RESPONSE_CANCEL );
	    Dialog.add_button(_("Ok"), Gtk::RESPONSE_OK );
      
      // Add a message to the content label.
      
      Gtk::Label* Message = Gtk::manage( new Gtk::Label );
      Message->set_label(_("Delete map controller?") );

      // Add extra dialog widgets to the content area.
	
	    Gtk::Box* Content = Dialog.get_content_area();
      Content->add( *Message );
      Content->set_border_width( 5 );
      Message->show();

      if ( Dialog.run() == Gtk::RESPONSE_OK )
      {
        // Erase the selected entry from the ListStore before erasing the
        // controller from the map.  This ensures the iterator in the ListStore
        // entry remains valid while the entry is being erased.  Erasing the
        // ListStore entry will result in an update to the TreeView.
      
        iListStore->erase( Row );

			  // Erase the selected controller from the map.
			
			  iMap->iControllers.erase( Controller );
      }
    }
  }
  else
    Handled = FALSE;
		
  return Handled;
}

//*---------------------------------------------------------*
//* Method to handle key press events from the code editor. *
//*---------------------------------------------------------*
//* key_event: Pointer to GdkEventKey.                      *
//* RETURN:    TRUE if key press was handled.               *
//*---------------------------------------------------------*

gboolean CControllerView::On_Editor_Key_Press( GdkEventKey* key_event )
{
  // Exit immediately if the event is not due to a key press.
  // The event is allowed to propagate to other handlers.
		
  if ( key_event->type != GDK_KEY_PRESS )
    return FALSE;

  gboolean Handled = TRUE;	
  int KeyValue     = key_event->keyval;

  if ( KeyValue == GDK_KEY_Escape )
  { 
    Glib::RefPtr<Gtk::TextBuffer> Buffer = iCodeEditor->get_buffer();
    Glib::ustring SourceCode;
    
    if ( Buffer->get_modified() )
    {
      // The sourcecode was modified.  Compile the sourcecode and then
      // uncompile the bytecode to show the changes.
    
      SourceCode = Buffer->get_text();
      (*iSelected).Compile( SourceCode );
      
      SourceCode.clear();
      (*iSelected).UnCompile( SourceCode );
      Buffer->set_text( SourceCode );
      Buffer->set_modified( FALSE );
      Buffer->place_cursor( Buffer->begin() );
    }
    else
    {
      // The sourcecode was not modified.  Return to the controller list.

      set_current_page( iControllerPageNumber );     
      iSelected = iMap->iControllers.end();   
      Do_Name( "" );
    }
  }
  else
    Handled = FALSE;
		
  return Handled;
}

//*---------------------------------------------------------------*
//* This method is called when a controller list row is activated *
//* (Double-clicked) to display the code editor.                  *
//*---------------------------------------------------------------*

void CControllerView::On_List_Row_Activated( const Gtk::TreeModel::Path& aPath,
                                             Gtk::TreeViewColumn* aColumn )
{
  Gtk::TreeModel::iterator Iterator = iListStore->get_iter( aPath );	
	
  if ( Iterator )
  {
    Gtk::TreeModel::Row Row = *Iterator;
    
    // Save iterator to selected controller. 
   
    iSelected = Row[ iColumnRecord.iIterator ];

    // Uncompile the controller's bytecode for the editor.

    Glib::ustring SourceCode;
    (*iSelected).UnCompile( SourceCode );
    
    Glib::RefPtr<Gtk::TextBuffer> Buffer = iCodeEditor->get_buffer();
    Buffer->set_text( SourceCode );
    Buffer->set_modified( FALSE );
    Buffer->place_cursor( Buffer->begin() );
    
    // Displayed the sourcecode editor.

    set_current_page( iCodePageNumber );
    
    // Update the controller name in the title bar.
    
    Do_Name( (*iSelected).iName );
  }

  return;
}

//*-------------------------------*
//* This method updates the view. *
//*-------------------------------*

void CControllerView::Update()
{
  if ( get_current_page() == iControllerPageNumber )
  {
    // Detach the model from the TreeView and clear all old entries.
  
    iControllerList->unset_model();
    iListStore->clear();

    // Populate the ListStore with iterators to the map controllers.
	
    Gtk::TreeModel::Row Row;	
    std::list<CMapController>::iterator Controller;

    for ( Controller = iMap->iControllers.begin();
          Controller != iMap->iControllers.end();
          ++ Controller )
    {
      Row = *( iListStore->append() );
      Row[ iColumnRecord.iIterator ] = Controller;
    }

    // Attach the filled model to the TreeView.

    iControllerList->set_model( iListStore );  

    // Clear selected controller and name in title bar.

    iSelected = iMap->iControllers.end();
    Do_Name("");
  }
  else if ( get_current_page() == iCodePageNumber )
  {
    // Update selected controller name in title bar.
    
    if ( iSelected != iMap->iControllers.end() )
      Do_Name( (*iSelected).iName );
  }
    
  return;
}

//*-----------------------------------------------------*
//* This method resets the view to the controller list. *
//*-----------------------------------------------------*

void CControllerView::Reset()
{
  // Displayed the controller list.

  set_current_page( iControllerPageNumber );
  return;
}

//*----------------------------------------------------------------------*
//* This method emits a signal with the name of the selected controller. *
//*----------------------------------------------------------------------*

void CControllerView::Do_Name( const std::string& aString )
{
  // Send controller name as a string in a signal.

  m_signal_name.emit( aString );
  return;
}

//*--------------------------------------------------------*
//* This method returns the controller name signal server. *
//*--------------------------------------------------------*

CControllerView::type_signal_name CControllerView::signal_name()
{
  return m_signal_name;
}