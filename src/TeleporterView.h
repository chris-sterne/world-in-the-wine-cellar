// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the TeleporterView class header.  The TeleporterView class
// displays and allows editing teleporter objects.
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
 
#ifndef __TELEPORTERVIEW_H__
#define __TELEPORTERVIEW_H__

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treeviewcolumn.h>
#include <gtkmm/liststore.h>
#include <gtkmm/cellrenderer.h>
#include <gdkmm/event.h>
#include "Position.h"
#include "Object.h"

namespace Enigma
{
	class World;
	
	class TeleporterView : public Gtk::ScrolledWindow
	{
		public:
			// Public methods.

			TeleporterView();
			void update();
			void set_world(std::shared_ptr<Enigma::World> world);
			bool on_key_press(GdkEventKey* key_event);

			// Map location signal accessor.

			typedef sigc::signal<void, const Enigma::Position&> type_signal_position;
			type_signal_position signal_position();

			// Overridden base class methods.

			void on_map() override;

		private:
			// Private classes.

			class ObjectColumns : public Gtk::TreeModel::ColumnRecord
			{
				public:
					Gtk::TreeModelColumn<std::list<Enigma::Object>::iterator> m_iterator;

					ObjectColumns()
					{ 
						add(m_iterator);
					}
			};

			// Private methods.

			void object_data_function(Gtk::CellRenderer* const& cell_renderer,
			                          const Gtk::TreeIter& tree_iterator);

			void on_cursor_changed();
			void do_position(Enigma::Position position);

			// Private data.

			ObjectColumns m_columnrecord;

			// Private data.

			std::shared_ptr<Enigma::World> m_world;     // Game world.
			std::list<Enigma::Object> m_buffer;         // List of teleporter objects.
			std::unique_ptr<Gtk::TreeView> m_treeview;  // Room object list viewer.
			Glib::RefPtr<Gtk::ListStore> m_liststore;   // Storage for data entries.
			type_signal_position m_signal_position;     // Location signal server.
	};
}

#endif // __TELEPORTERVIEW_H__
