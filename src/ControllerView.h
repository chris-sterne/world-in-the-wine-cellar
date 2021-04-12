// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the ControllerView class header.  The ControllerView class
// displays and allows editing code of logic controllers.
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
 
#ifndef __CONTROLLERVIEW_H__
#define __CONTROLLERVIEW_H__

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/textview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/notebook.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treeviewcolumn.h>
#include <gdkmm/event.h>
#include "Controller.h"

namespace Enigma
{
	class World;
	
	class ControllerView : public Gtk::Notebook
	{
		public:
			// Public methods.

			ControllerView();
			void update();
			void reset();
			void do_name(const std::string& string);
			void set_world(std::shared_ptr<Enigma::World> world);
			bool on_list_key_press(GdkEventKey* key_event);
			bool on_editor_key_press(GdkEventKey* key_event);

			void on_list_row_activated(const Gtk::TreeModel::Path& path,
			                           Gtk::TreeViewColumn* column);

			// Controller name signal accessor.

			typedef sigc::signal<void, const std::string&> type_signal_name;
			type_signal_name signal_name();

			// Overridden base class methods.

			void on_map() override;

		private:
			// Private classes.

			class Column : public Gtk::TreeModel::ColumnRecord
			{
				public:
					Gtk::TreeModelColumn<std::list<Enigma::Controller>::iterator>
						m_iterator;

					Column()
					{ 
						add(m_iterator);
					}
			};

			// Private methods.

			void cell_data_function(Gtk::CellRenderer* const& cell_renderer,
			                        const Gtk::TreeIter& tree_iterator);

			// Private data.

			Column m_columnrecord;

			// Private data.

			std::shared_ptr<Enigma::World> m_world;
			std::unique_ptr<Gtk::ScrolledWindow> m_controller_window;
			std::unique_ptr<Gtk::TreeView> m_controller_list;
			Glib::RefPtr<Gtk::ListStore> m_liststore;
			std::unique_ptr<Gtk::ScrolledWindow> m_code_window;
			std::unique_ptr<Gtk::TextView> m_code_editor;
			std::list<Enigma::Controller>::iterator m_selected;
			type_signal_name m_signal_name;
			int m_controller_page_number;
			int m_code_page_number;
	};
}

#endif // __CONTROLLERVIEW_H__
