// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the DescriptionView class header.  The DescriptionView class
// displays and allows editing a description of the world.
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

#ifndef __DESCRIPTIONVIEW_H__
#define __DESCRIPTIONVIEW_H__

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/textview.h>

namespace Enigma
{
	class World;

		class DescriptionView : public Gtk::ScrolledWindow
		{
			public:
				// Public methods.

				DescriptionView();
				void set_world(std::shared_ptr<Enigma::World> world);
				void on_changed();
				void update();

				// Overridden base class methods.

				void on_map() override;

			private:
				// Private data.

				std::shared_ptr<Enigma::World> m_world;     // Game world.
				std::unique_ptr<Gtk::TextView> m_textview;  // TextView widget.
				sigc::connection m_textbuffer_connection;   // Signal connection.
	};
}

#endif // __DESCRIPTIONVIEW_H__
