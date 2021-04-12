// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the ItemView class implementation.  The ItemView class
// displays and allows editing item objects.
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
#include "ItemView.h"
#include "World.h"

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::ItemView::ItemView()
{
	add_events(Gdk::KEY_PRESS_MASK);

	m_treeview = std::make_unique<Gtk::TreeView>();
	add(*m_treeview);
	m_treeview->set_headers_visible(false);
	m_treeview->set_hexpand(true);
	m_treeview->set_vexpand(true);
	m_treeview->set_can_focus(true);

	Glib::RefPtr<Gtk::TreeSelection> selection = m_treeview->get_selection();
	selection->set_mode(Gtk::SELECTION_BROWSE);

	// Create the TreeView model, but do not attach it to the TreeView.
	// This will be done later after the model has been filled.

	m_liststore = Gtk::ListStore::create(m_columnrecord);	

	// Create TreeView view.  The column cell has a function added for determining
	// how to display the column data.

	Gtk::TreeViewColumn* column = Gtk::manage(new Gtk::TreeViewColumn);
	m_treeview->append_column(*column);
	Gtk::CellRendererText* cell = Gtk::manage(new Gtk::CellRendererText);
	column->pack_start(*cell, true);

	column->set_cell_data_func(*cell,
		sigc::mem_fun(*this, &Enigma::ItemView::object_data_function));

	// Connect a key press event handler to the RoomView, but place it before
	// its default handler.  This allows capturing general key press events.

	signal_key_press_event()
		.connect(sigc::mem_fun(*this, &Enigma::ItemView::on_key_press), false);

	//Connect signal for double-click row activation.

	m_treeview->signal_row_activated()
		.connect(sigc::mem_fun(*this, &Enigma::ItemView::on_row_activated));

	// Add a signal handler for TreeView cursor changes.

	m_treeview->signal_cursor_changed()
		.connect(sigc::mem_fun(*this, &Enigma::ItemView::on_cursor_changed ));
}

//------------------------------------------------------------
// This method is called when the widget is about to be shown.
//------------------------------------------------------------

void Enigma::ItemView::on_map()
{
	// Pass the method to the base class.

	Gtk::ScrolledWindow::on_map();

	// Update the item list before it is displayed.

	update();

}

//---------------------------------------------------------------
// This method is called when Mapobject cells are to be rendered.
// Data is read from the model and converted into an appropriate 
// content for display.
//---------------------------------------------------------------

void Enigma::ItemView::object_data_function(
	Gtk::CellRenderer* const& cell_renderer,
	const Gtk::TreeIter& tree_iterator)
{	
		// Exit if there is no iterator.

	if (!tree_iterator)
		return;
	
	Gtk::TreeModel::Row row = *tree_iterator;
	Gtk::CellRendererText* renderer = (Gtk::CellRendererText*)(cell_renderer);

	// Get the text to be rendered from the object.

	std::list<Enigma::Object>::iterator object;
	object = row[m_columnrecord.m_iterator];

	Glib::ustring description;
	(*object).get_description(description);

	// Render object description text.
	
	renderer->property_text() = description;
}

//----------------------------------------------
// This method sets the game world to be viewed.
//----------------------------------------------
// world: Game world.
//----------------------------------------------

void Enigma::ItemView::set_world(std::shared_ptr<Enigma::World> world)
{
	m_world = world;
}

//----------------------------------------------------------
// This method handles key press events from the PlayerView.
//----------------------------------------------------------
// key_event: Pointer to GdkEventKey.
// RETURN:    TRUE if key press was handled.
//----------------------------------------------------------

bool Enigma::ItemView::on_key_press(GdkEventKey* key_event)
{
	// Exit immediately if the event is not due to a key press.
	// The event is allowed to propagate to other handlers.

	if (key_event->type != GDK_KEY_PRESS)
	return false;

	bool handled  = true;	
	int key_value = key_event->keyval;

	if (key_value == GDK_KEY_Delete)
	{		
		Glib::RefPtr<Gtk::TreeSelection> selection = m_treeview->get_selection();
		Gtk::TreeModel::iterator iterator = selection->get_selected();

		if (iterator)
		{
			// An iterator for a selected item object is available.

			Gtk::TreeModel::Row row = *iterator;
			std::list<Enigma::Object>::iterator object;
			object = row[m_columnrecord.m_iterator];

			// Erase the selected entry from the ListStore before erasing the object
			// from the map.  This ensures the iterator in the ListStore entry
			// remains valid while the entry is being erased.  Erasing the ListStore
			// entry will result in an update to the TreeView.

			m_liststore->erase(row);

			// Erase the selected item from the world.

			m_world->m_items.erase(object);
		}
	}
	else
		handled = false;

	return handled;
}

//-----------------------------------------------------------
// This signal handler is called when the cursor changes row.
//-----------------------------------------------------------

void Enigma::ItemView::on_cursor_changed()
{
	// Get an iterator to the highlighted (selected) entry.

	Glib::RefPtr<Gtk::TreeSelection> selection = m_treeview->get_selection(); 
	Gtk::TreeModel::iterator iterator = selection->get_selected();

	if (iterator)
	{
		Gtk::TreeModel::Row row = *iterator;
		std::list<Enigma::Object>::iterator object;
		object = row[m_columnrecord.m_iterator];

		// Emit the item's position in a signal.
		
		do_position((*object).m_position);
	}
}

//------------------------------------------------------------
// This method is called when a game map list row is activated
// (Double-clicked) to go to the item's location.
//------------------------------------------------------------

void Enigma::ItemView::on_row_activated(const Gtk::TreeModel::Path& path,
                                        Gtk::TreeViewColumn* column )
{
	// NOTE: Not yet implemented.

	/*Gtk::TreeModel::iterator iterator = m_liststore->get_iter(path);	

	if (iterator)
	{
		Gtk::TreeModel::Row row = *iterator;
	}*/
}

//------------------------------
// This method updates the view.
//------------------------------

void Enigma::ItemView::update()
{
	// Detach the model from the TreeView and clear all old entries.

	m_treeview->unset_model();
	m_liststore->clear();

	// Populate the ListStore with iterators to all items in the map.

	Gtk::TreeModel::Row row;	
	std::list<Enigma::Object>::iterator object;

	// Add all required items.

	for (object = m_world->m_items.begin();
	     object != m_world->m_items.end();
	     ++ object )
	{
		if ((*object).m_category == Enigma::Object::Category::REQUIRED)
		{
			row = *(m_liststore->append());
			row[m_columnrecord.m_iterator] = object;
		}
	}

	// Add all optional items.

	for (object = m_world->m_items.begin();
	     object != m_world->m_items.end();
	     ++ object)
	{
		if ((*object).m_category == Enigma::Object::Category::OPTIONAL)
		{
			row = *(m_liststore->append());
			row[m_columnrecord.m_iterator] = object;
		}
	}

	// Add all Easter Egg items.

	for (object = m_world->m_items.begin();
	     object != m_world->m_items.end();
	     ++ object)
	{
		if ((*object).m_category == Enigma::Object::Category::EASTEREGG)
		{
			row = *(m_liststore->append());
			row[m_columnrecord.m_iterator] = object;
		}
	}

	// Add all Skull items.

	for (object = m_world->m_items.begin();
	     object != m_world->m_items.end();
	     ++ object)
	{
		if ((*object).m_category == Enigma::Object::Category::SKULL)
		{
			row = *(m_liststore->append());
			row[ m_columnrecord.m_iterator] = object;
		}
	}

	// Attach the filled model to the TreeView.

	m_treeview->set_model(m_liststore);
}

//-----------------------------------------------------
// This method returns the item position signal server.
//-----------------------------------------------------

Enigma::ItemView::type_signal_position Enigma::ItemView::signal_position()
{
	return m_signal_position;
}

//---------------------------------------------------------
// This method emits a signal containing the item position.
//---------------------------------------------------------

void Enigma::ItemView::do_position(Enigma::Position position)
{	  
	m_signal_position.emit(position);
}
