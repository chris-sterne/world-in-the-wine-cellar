// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the LevelView class header.  The LevelView class displays
// levels of a game world, and allows editing the room contents on the level.
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


#ifndef __LEVELVIEW_H__
#define __LEVELVIEW_H__

#include <gtkmm/drawingarea.h>
#include "Volume.h"
#include "Tiles.h"

namespace Enigma
{
	class World;
	
	class LevelView : public Gtk::DrawingArea
	{
		public:
			// Public methods.

			LevelView();
			void home();
			void update();
			void set_world(std::shared_ptr<Enigma::World> world);
			void set_filter(Enigma::Object::ID m_filter);
			Enigma::Position& get_cursor();

			// Map position signal accessor.

			typedef sigc::signal<void, const Enigma::Position&> type_signal_position;
			type_signal_position signal_position();

			// Overridden base class methods.

			void on_size_allocate(Gtk::Allocation& allocation) override;
			void on_map() override;
			bool on_draw(const Cairo::RefPtr<Cairo::Context>& context) override;
			bool on_key_press_event(GdkEventKey* key_event) override;
			
			void get_preferred_width_vfunc(int& minimum_width,
			                               int& natural_width) const override;
			
			void get_preferred_height_vfunc(int& minimum_height,
			                                int& natural_height) const override;

		private:
			// Private methods.

			void do_location();
			void mark();
			void erase();
			void cut();
			void copy();
			void paste();

			// Private data.

			std::shared_ptr<Enigma::World> m_world;    // Game world being viewed.
			Enigma::Tiles m_tiles;                     // Image tile drawing object.
			Enigma::Volume m_view;                     // Visible area of world.
			Enigma::Volume m_mark;                     // Marked volume of world.
			Enigma::Position m_mark_origin;            // Origin of marked volume.
			Enigma::Position m_cursor;                 // Position of cursor.
			type_signal_position m_signal_position;    // Position signal server.
			std::list<Enigma::Object> m_edit_buffer;   // Objects editing buffer.
			Enigma::Object::ID m_filter;               // Object viewing filter.
	};
}

#endif // __LEVELVIEW_H__
