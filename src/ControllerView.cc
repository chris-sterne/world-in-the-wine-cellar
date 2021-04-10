// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the ControllerView class implementation.  The ControllerView
// class displays and allows editing code for logic controllers.
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
#include "ControllerView.h"
#include "World.h"

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::ControllerView::ControllerView()
{
	//add_events(Gdk::KEY_PRESS_MASK);

	set_show_tabs(false);
	set_show_border(false);

	// Prepare a scrolled window containing a list of map controllers.

	m_controller_window
	  = std::unique_ptr<Gtk::ScrolledWindow>( new Gtk::ScrolledWindow );

  m_controller_list = std::unique_ptr<Gtk::TreeView>( new Gtk::TreeView );
  m_controller_window->add( *m_controller_list );


	//m_controller_window = std::make_unique<Gtk::ScrolledWindow>();
	//m_controller_list   = std::make_unique<Gtk::TreeView>();
	//m_controller_window->add( *m_controller_list );

	m_controller_list->set_headers_visible(false);
	m_controller_list->set_hexpand(true);
	m_controller_list->set_vexpand(true);
	m_controller_list->set_can_focus(true);

	//Connect signal for double-click list row activation.

	m_controller_list->signal_row_activated()
		.connect(sigc::mem_fun(*this,
		                       &Enigma::ControllerView::on_list_row_activated));

	// Connect a key press event handler to the controller list, but place it
	// before its default handler.  This allows capturing general key press
	// events.

	m_controller_list->signal_key_press_event()
		.connect(sigc::mem_fun(*this,
		                       &Enigma::ControllerView::On_List_Key_Press),
	                         false);

	Glib::RefPtr<Gtk::TreeSelection> tree_selection =
		m_controller_list->get_selection();

	tree_selection->set_mode(Gtk::SELECTION_BROWSE);

	// Create the TreeView model, but do not attach it to the TreeView.
	// This will be done later after the model has been filled.

	m_liststore = Gtk::ListStore::create(m_column_record);	

	// Create TreeView view.  The column cell has a function added for determining
	// how to display the column data.

	Gtk::TreeViewColumn* column = Gtk::manage(new Gtk::TreeViewColumn);
	m_controller_list->append_column(*column);
	Gtk::CellRendererText* cell = Gtk::manage(new Gtk::CellRendererText);
	Column->pack_start(*cell, true);
	
	Column->set_cell_data_func(*cell, sigc::mem_fun(*this,
		&Enigma::ControllerView::cell_data_function)); 

	// Prepare a scrolled window containing a controller sourcecode editor.

	m_code_window = std::make_unique<Gtk::ScrolledWindow>();
	m_code_editor = std::make_unique<Gtk::TextView>();
	m_code_window->add(*m_code_editor);

	m_code_editor->set_wrap_mode( Gtk::WRAP_WORD );
	m_code_editor->set_editable(true);
	m_code_editor->set_cursor_visible(true);
	m_code_editor->set_hexpand(true);
	m_code_editor->set_vexpand(true);
	m_code_editor->set_can_focus(true);

	// Connect a key press event handler to the code editor, but place it
	// before its default handler.  This allows capturing general key press
	// events.

	m_code_editor->signal_key_press_event()
		.connect(sigc::mem_fun(*this,
	                         &CControllerView::On_Editor_Key_Press),
	                         false);

// Add pages to the notebook.

	m_controller_page_number = append_page(*m_controller_window);
	m_code_page_number       = append_page(*m_code_window);

	// The first displayed page shows the controller list.

	set_current_page(m_controller_page_number);
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

void CControllerView::cell_data_function(Gtk::CellRenderer* const& aCellRenderer,
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
