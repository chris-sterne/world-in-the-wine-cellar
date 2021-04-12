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

	m_controller_window = std::make_unique<Gtk::ScrolledWindow>();
  m_controller_list = std::make_unique<Gtk::TreeView>();
  m_controller_window->add(*m_controller_list);

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
		                       &Enigma::ControllerView::on_list_key_press),
	                         false);

	Glib::RefPtr<Gtk::TreeSelection> selection =
		m_controller_list->get_selection();

	selection->set_mode(Gtk::SELECTION_BROWSE);

	// Create the TreeView model, but do not attach it to the TreeView.
	// This will be done later after the model has been filled.

	m_liststore = Gtk::ListStore::create(m_columnrecord);	

	// Create TreeView view.  The column cell has a function added
	// for determining how to display the column data.

	Gtk::TreeViewColumn* column = Gtk::manage(new Gtk::TreeViewColumn);
	m_controller_list->append_column(*column);
	Gtk::CellRendererText* cell = Gtk::manage(new Gtk::CellRendererText);
	column->pack_start(*cell, true);
	
	column->set_cell_data_func(*cell, sigc::mem_fun(*this,
		&Enigma::ControllerView::cell_data_function)); 

	// Prepare a scrolled window containing a controller sourcecode editor.

	m_code_window = std::make_unique<Gtk::ScrolledWindow>();
	m_code_editor = std::make_unique<Gtk::TextView>();
	m_code_window->add(*m_code_editor);

	m_code_editor->set_wrap_mode(Gtk::WRAP_WORD);
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
	                         &Enigma::ControllerView::on_editor_key_press),
	                         false);

	// Add pages to the viewbook.

	m_controller_page_number = append_page(*m_controller_window);
	m_code_page_number       = append_page(*m_code_window);

	// The first displayed page shows the controller list.

	set_current_page(m_controller_page_number);
}

//------------------------------------------------------------
// This method is called when the widget is about to be shown.
//------------------------------------------------------------

void Enigma::ControllerView::on_map()
{
	// Pass the method to the base class.

	Gtk::Notebook::on_map();

	// Update the current page.

	update();
}

//--------------------------------------------------------------
// This method is called when ListView cells are to be rendered.
// Data is read from the model and converted into an appropriate
// content for display.
//--------------------------------------------------------------

void Enigma::ControllerView::cell_data_function(
  Gtk::CellRenderer* const& cell_renderer,
  const Gtk::TreeIter& tree_iterator)
{	
	// Exit if there is no iterator.

	if (!tree_iterator)
		return;

	Gtk::TreeModel::Row row = *tree_iterator;
	Gtk::CellRendererText* renderer = (Gtk::CellRendererText*)(cell_renderer);

	// Get the controller name to be rendered.

	std::list<Enigma::Controller>::iterator controller;
	controller = row[m_columnrecord.m_iterator];
	
	// Render the controller name.
	
	renderer->property_text() = (*controller).m_name;
}

//----------------------------------------------
// This method sets the game world to be viewed.
//----------------------------------------------
// world: Game world.
//----------------------------------------------

void Enigma::ControllerView::set_world(std::shared_ptr<Enigma::World> world)
{
	m_world    = world;
	m_selected = m_world->m_controllers.end();
}

//---------------------------------------------------------------
// This method handles key press events from the controller list.
//---------------------------------------------------------------
// key_event: Pointer to GdkEventKey.
// RETURN:    TRUE if key press was handled.
//---------------------------------------------------------------

bool Enigma::ControllerView::on_list_key_press(GdkEventKey* key_event)
{
	// Exit immediately if the event is not due to a key press.
	// The event is allowed to propagate to other handlers.

	if (key_event->type != GDK_KEY_PRESS)
		return false;

	bool handled  = true;	
	int key_value = key_event->keyval;

	if (key_value == GDK_KEY_Delete)
	{		
		// The selected controller is to be deleted.
		
		Glib::RefPtr<Gtk::TreeSelection> selection =
			m_controller_list->get_selection();

		Gtk::TreeModel::iterator iterator = selection->get_selected();

		if (iterator)
		{
			// An iterator for a selected controller is available.
			// Get the controller to be deleted.

			Gtk::TreeModel::Row row = *iterator;
			std::list<Enigma::Controller>::iterator controller;
			controller = row[m_columnrecord.m_iterator];

			// Display a dialog confirming the deletion.

			Gtk::Dialog dialog(_("Controllers"));

			dialog.add_button(_("Cancel"), Gtk::RESPONSE_CANCEL);
			dialog.add_button(_("Ok"), Gtk::RESPONSE_OK);

			// Add a confirmation message to the content label.

			Gtk::Label* message = Gtk::manage(new Gtk::Label);
			message->set_label(_("Delete map controller?"));

			// Add extra dialog widgets to the content area.

			Gtk::Box* content = dialog.get_content_area();
			content->add(*message);
			content->set_border_width(5);
			
			// Show the dialog.
			
			message->show();

			if (dialog.run() == Gtk::RESPONSE_OK)
			{
				// Erase the selected entry from the ListStore before erasing the
				// controller from the world.  This ensures the iterator in the
				// ListStore entry remains valid while the entry is being erased.
				// Erasing the ListStore entry will result in an update to
				// the TreeView.

				m_liststore->erase(row);

				// Erase the selected controller from the world.

				m_world->m_controllers.erase(controller);
			}
		}
	}
	else
		handled = false;

	return handled;
}

//--------------------------------------------------------
// Method to handle key press events from the code editor.
//--------------------------------------------------------
// key_event: Pointer to GdkEventKey.
// RETURN:    TRUE if key press was handled.
//--------------------------------------------------------

bool Enigma::ControllerView::on_editor_key_press(GdkEventKey* key_event)
{
	// Exit immediately if the event is not due to a key press.
	// The event is allowed to propagate to other handlers.

	if (key_event->type != GDK_KEY_PRESS)
		return false;

	bool handled  = true;	
	int key_value = key_event->keyval;

	if (key_value == GDK_KEY_Escape)
	{ 
		Glib::RefPtr<Gtk::TextBuffer> buffer = m_code_editor->get_buffer();
		Glib::ustring sourcecode;

		if (buffer->get_modified())
		{
			// The sourcecode was modified.  Compile the sourcecode and then
			// uncompile the bytecode to show the changes.

			sourcecode = buffer->get_text();
			(*m_selected).compile(sourcecode);

			sourcecode.clear();
			(*m_selected).uncompile(sourcecode);
			buffer->set_text(sourcecode);
			buffer->set_modified(false);
			buffer->place_cursor(buffer->begin());
		}
		else
		{
			// The sourcecode was not modified.  Return to the controller list.

			set_current_page(m_controller_page_number);     
			m_selected = m_world->m_controllers.end();   
			do_name( "" );
		}
	}
	else
		handled = false;

	return handled;
}

//--------------------------------------------------------------
// This method is called when a controller list row is activated
// (Double-clicked) to display the code editor.
//--------------------------------------------------------------

void Enigma::ControllerView::on_list_row_activated(
	const Gtk::TreeModel::Path& path,
	Gtk::TreeViewColumn* column)
{
	Gtk::TreeModel::iterator iterator = m_liststore->get_iter(path);	

	if (iterator)
	{
		Gtk::TreeModel::Row row = *iterator;

		// Save iterator to selected controller. 

		m_selected = row[m_columnrecord.m_iterator];

		// Uncompile the controller's bytecode for the editor.

		Glib::ustring sourcecode;
		(*m_selected).uncompile(sourcecode);

		Glib::RefPtr<Gtk::TextBuffer> buffer = m_code_editor->get_buffer();
		buffer->set_text(sourcecode);
		buffer->set_modified(true);
		buffer->place_cursor(buffer->begin());

		// Displayed the sourcecode editor.

		set_current_page(m_code_page_number);

		// Update the controller name in the title bar.

		do_name((*m_selected).m_name);
	}
}

//------------------------------
// This method updates the view.
//------------------------------

void Enigma::ControllerView::update()
{
	if (get_current_page() == m_controller_page_number)
	{
		// Detach the model from the TreeView and clear all old entries.

		m_controller_list->unset_model();
		m_liststore->clear();

		// Populate the ListStore with iterators to the map controllers.

		Gtk::TreeModel::Row row;	
		std::list<Enigma::Controller>::iterator controller;

		for (controller = m_world->m_controllers.begin();
		     controller != m_world->m_controllers.end();
		     ++ controller )
		{
			row = *(m_liststore->append());
			row[m_columnrecord.m_iterator] = controller;
		}

		// Attach the filled model to the TreeView.

		m_controller_list->set_model(m_liststore);  

		// Clear selected controller and name in title bar.

		m_selected = m_world->m_controllers.end();
		do_name("");
	}
	else if (get_current_page() == m_code_page_number)
	{
		// Update selected controller name in title bar.

		if (m_selected != m_world->m_controllers.end())
		do_name((*m_selected).m_name);
	}
}

//----------------------------------------------------
// This method resets the view to the controller list.
//----------------------------------------------------

void Enigma::ControllerView::reset()
{
	// Displayed the controller list.

	set_current_page(m_controller_page_number);
}

//---------------------------------------------------------------------
// This method emits a signal with the name of the selected controller.
//---------------------------------------------------------------------

void Enigma::ControllerView::do_name(const std::string& string)
{
	// Send controller name as a string in a signal.

	m_signal_name.emit(string);
}

//-------------------------------------------------------
// This method returns the controller name signal server.
//-------------------------------------------------------

Enigma::ControllerView::type_signal_name Enigma::ControllerView::signal_name()
{
	return m_signal_name;
}
