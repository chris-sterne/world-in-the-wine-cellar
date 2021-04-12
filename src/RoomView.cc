// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the RoomView class implementation.  The RoomView class displays
// room objects in a list for editing.
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

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::RoomView::RoomView()
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

	// Create TreeView view, and add a column cell function to control
	// how the column data is displayed in the TreeView.
	
  Gtk::TreeViewColumn* column = Gtk::manage(new Gtk::TreeViewColumn);
	m_treeview->append_column(*column);
	
	Gtk::CellRendererText* cell = Gtk::manage(new Gtk::CellRendererText);
  column->pack_start(*cell, true);
	
	column->set_cell_data_func(*cell,
		sigc::mem_fun(*this, &Enigma::RoomView::object_data_function)); 

	// Connect a key press event handler to the RoomView, but place it before
	// its default handler.  This allows capturing general key press events.
	
	signal_key_press_event()
		.connect(sigc::mem_fun(*this, &Enigma::RoomView::on_key_press), false);
}

//------------------------------------------------------------
// This method is called when the widget is about to be shown.
//------------------------------------------------------------

void Enigma::RoomView::on_map()
{
	// Pass the method to the base class.

	Gtk::ScrolledWindow::on_map();
	
	// Update the list contents.

	update();
}

//--------------------------------------------------------------
// This method is called when object cells are to be rendered.  
// Data is read from the model and converted into an appropriate
// content for display.
//--------------------------------------------------------------

void Enigma::RoomView::object_data_function(
	Gtk::CellRenderer* const& cell_renderer,
	const Gtk::TreeIter& tree_iterator)
{	
	// Exit if there is no iterator.

	if (!tree_iterator)
		return;

	Gtk::TreeModel::Row row = *tree_iterator;
	Gtk::CellRendererText* renderer = (Gtk::CellRendererText*)(cell_renderer);

	std::list<Enigma::Object>::iterator object;
	object = row[m_columnrecord.m_iterator];

	Glib::ustring description;
	(*object).get_description(description);

	// Render the object description.
	
	renderer->property_text() = description;
}

//--------------------------------------------
// This method sets the game map to be viewed.
//--------------------------------------------
// world: Game world.
//--------------------------------------------

void Enigma::RoomView::set_world(std::shared_ptr<Enigma::World> world)
{
	m_world = world;
}

//-------------------------------------------------
// This method sets the world position of the room.
//-------------------------------------------------
// position: World position of room.
//-------------------------------------------------

void Enigma::RoomView::set_position(const Enigma::Position& position)
{
  // Update the room position.

  m_position = position;
}

//-----------------------------------------------------
// Method to handle key press events from the RoomView.
//-----------------------------------------------------
// key_event: Pointer to GdkEventKey.
// RETURN:    TRUE if key press was handled.
//-----------------------------------------------------

bool Enigma::RoomView::on_key_press(GdkEventKey* key_event)
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
      // An iterator for a selected object entry is available.

      Gtk::TreeModel::Row row = *iterator;
      std::list<Enigma::Object>::iterator object;
      object = row[m_columnrecord.m_iterator];

      // Erase the selected entry from the ListStore before erasing the object
      // from the world.  This ensures the iterator in the ListStore entry
      // remains valid while the entry is being erased.  Erasing the ListStore
      // entry will result in an update to the TreeView.
      
      m_liststore->erase(row);

      // Erase the selected object from the correct list in the map.
			
      if ((*object).m_type == Enigma::Object::Type::ITEM)
        m_world->m_items.erase(object);
      else if ((*object).m_type == Enigma::Object::Type::PLAYER)
        m_world->m_players.erase(object);
      else if ((*object).m_type == Enigma::Object::Type::TELEPORTER)
        m_world->m_teleporters.erase(object);
      else
        m_world->m_objects.erase(object);
    }
  }
  else
    handled = false;

  return handled;
}

//------------------------------
// This method updates the view.
//------------------------------

void Enigma::RoomView::update()
{
  // Detach the model from the TreeView and clear all old entries.

  m_treeview->unset_model();
  m_liststore->clear();

  // Read iterators from all lists to world objects in the room.

  std::list<std::list<Enigma::Object>::iterator> buffer;
  
  m_world->m_objects.read(m_position, buffer);
  m_world->m_teleporters.read(m_position, buffer);
  m_world->m_items.read(m_position, buffer);
  m_world->m_players.read(m_position, buffer);

  // Populate the ListStore.

  std::list<std::list<Enigma::Object>::iterator>::iterator object;
  Gtk::TreeModel::Row row;
	
  for (object = buffer.begin();
       object != buffer.end();
       ++ object)
  {
    row = *(m_liststore->append());
    row[m_columnrecord.m_iterator] = *object;
  }

  // Attach the filled model to the TreeView.

  m_treeview->set_model(m_liststore);
}
