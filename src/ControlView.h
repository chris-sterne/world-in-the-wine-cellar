// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the ControlView class header.  The ControlView class manages
// game world controls.
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

#ifndef __CONTROLVIEW_H__
#define __CONTROLVIEW_H__

#include <gtkmm/grid.h>
#include <gtkmm/checkbutton.h>
#include "World.h"

namespace Enigma
{
	class ControlView : public Gtk::Grid
	{
		public:
			// Public methods.

			ControlView();
			void set_world(std::shared_ptr<Enigma::World> world);
			void on_changed();
			void update();

			// Overridden base class methods.

			void on_map() override;

			// Public data.

			std::shared_ptr<Enigma::World> m_world;        // Game world.
			std::unique_ptr<Gtk::CheckButton> m_savable;   // Can save map button.
			sigc::connection m_savable_connection;         // Signal connection.
	};
}

#endif // __CONTROLVIEW_H__
