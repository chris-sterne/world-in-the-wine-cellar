// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the RoomView class header.  The RoomView class displays
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
 
#ifndef __ROOMVIEW_H__
#define __ROOMVIEW_H__

#include <gdkmm/event.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/cellrenderer.h>
#include "Object.h"

namespace Enigma
{
	class World;
	
	class RoomView : public Gtk::ScrolledWindow
	{
		public:
			// Public methods.

			RoomView();
			void update();
			void set_world(std::shared_ptr<Enigma::World> world);
			void set_position(const Enigma::Position& position);
			bool on_key_press(GdkEventKey* key_event);

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

			// Private data.

			ObjectColumns m_columnrecord;

			// Private data.

			std::shared_ptr<Enigma::World> m_world;      // Game world.
			Enigma::Position m_position;                 // Position of room.
			std::unique_ptr<Gtk::TreeView> m_treeview;   // Room object list viewer.
			Glib::RefPtr<Gtk::ListStore> m_liststore;    // Storage for data entries.
	};
}

#endif // __ROOMVIEW_H__
