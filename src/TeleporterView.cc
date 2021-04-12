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
#include "World.h"

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::TeleporterView::TeleporterView()
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

	m_liststore = Gtk::ListStore::create( iColumnRecord );

	// Create TreeView view.  The column cell has a function added for
	// determining how to display the column data.

	Gtk::TreeViewColumn* column = Gtk::manage(new Gtk::TreeViewColumn);
	m_treeview->append_column(*column);
	Gtk::CellRendererText* cell = Gtk::manage(new Gtk::CellRendererText);
	ObjectColumn->pack_start(cell, true);
	
	ObjectColumn->set_cell_data_func(*cell,
		sigc::mem_fun(*this, &Enigma::PlayerView::object_data_function)); 

	// Connect a key press event handler to the PlayerView, but place it before
	// its default handler.  This allows capturing general key press events.

	signal_key_press_event()
		.connect(sigc::mem_fun(*this,
		                       &Enigma::TeleporterView::on_key_press),
		                       false);

	// Add a signal handler for TreeView cursor changes.

	m_treeview->signal_cursor_changed()
		.connect(sigc::mem_fun(*this,
		                       &Enigma::TeleporterView::on_cursor_changed));
}

//------------------------------------------------------------
// This method is called when the widget is about to be shown.
//------------------------------------------------------------

void Enigma::TeleporterView::on_map()
{
	// Pass the method to the base class.

	Gtk::ScrolledWindow::on_map();

	// Update the teleporter list before it is displayed.

	update();
}

//--------------------------------------------------------------
// This method is called when object cells are to be rendered.
// Data is read from the model and converted into an appropriate
// content for display.
//--------------------------------------------------------------

void Enigma::PlayerView::object_data_function(
	Gtk::CellRenderer* const& cell_renderer,
	const Gtk::TreeIter& tree_iterator)
{	
	// Exit if there is no iterator.

	if (!tree_iterator)
		return;
	
	Gtk::TreeModel::Row row = *tree_iterator;
	Gtk::CellRendererText* renderer = (Gtk::CellRendererText*)(cell_renderer);

	// Get the text to be rendered from the object.

	std::list<CMapObject>::iterator object;
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

void Enigma::TeleporterView::set_world(std::shared_ptr<Enigma::World> world)
{
	m_world = world;
}

//--------------------------------------------------------------
// This method handles key press events from the TeleporterView.
//--------------------------------------------------------------
// key_event: Pointer to GdkEventKey.
// RETURN:    TRUE if key press was handled.
//--------------------------------------------------------------

bool Enigma::TeleporterView::on_key_press(GdkEventKey* key_event)
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
			// An iterator for a selected player object is available.

			Gtk::TreeModel::Row row = *iterator;
			std::list<Enigma::Object>::iterator object;
			object = row[m_columnrecord.m_iterator];

			// Erase the selected entry from the ListStore before erasing the object
			// from the map.  This ensures the iterator in the ListStore entry
			// remains valid while the entry is being erased.  Erasing the ListStore
			// entry will result in an update to the TreeView.

			m_liststore->erase(row);

			// Erase the selected player from the world.

			m_world->m_players.erase(object);
		}
	}
	else
		handled = false;

	return handled;
}

//-----------------------------------------------------------
// This signal handler is called when the cursor changes row.
//-----------------------------------------------------------

void Enigma::PlayerView::on_cursor_changed()
{
	// Get an iterator to the highlighted (selected) entry.

	Glib::RefPtr<Gtk::TreeSelection> selection = m_treeview->get_selection(); 
	Gtk::TreeModel::iterator iterator = selection->get_selected();

	if (iterator)
	{
		Gtk::TreeModel::Row row = *iterator;
		std::list<CMapObject>::iterator object;
		object = Row[m_columnrecord.m_iterator];

		// Emit the teleporter's position in a signal.

		do_position((*object).m_position);
	}
}

//------------------------------
// This method updates the view.
//------------------------------

void Enigma::TeleporterView::update()
{
	// Detach the model from the TreeView and clear all old entries.

	m_treeview->unset_model();
	m_liststore->clear();

	// Populate the ListStore with iterators to all teleporters in the world.

	Gtk::TreeModel::Row row;	
	std::list<CMapObject>::iterator object;

	for (object = m_world->m_players.begin();
	     object != m_world->m_players.end();
	     ++ object)
	{
		row = *(m_liststore->append());
		row[m_columnrecord.m_iterator] = object;
	}

	// Attach the filled model to the TreeView.

	m_treeview->set_model(m_liststore);
}

//-----------------------------------------------------------
// This method returns the teleporter position signal server.
//-----------------------------------------------------------

Enigma::TeleporterView::type_signal_position
	Enigma::TeleporterView::signal_position()
{
	return m_signal_position;
}

//---------------------------------------------------------------
// This method emits a signal containing the teleporter position.
//---------------------------------------------------------------

void Enigma::TeleporterView::do_position(Enigma::Position position)
{	  
	m_signal_position.emit(position);
