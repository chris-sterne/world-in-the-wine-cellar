// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the ControlView class implementation.  The ControlView class
// manages game world controls.
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
#include "ControlView.h"

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::ControlView::ControlView()
{
	set_border_width(10);
	set_halign(Gtk::ALIGN_START);
	set_column_homogeneous(false);

	m_savable = std::unique_ptr<Gtk::CheckButton>(
		new Gtk::CheckButton(_("Able to save map in game.")));

	attach(*m_savable, 0, 0, 1, 1);

	// Connect signal for control changes.

	m_savable_connection = m_savable->signal_toggled()
		.connect(sigc::mem_fun(*this, &Enigma::ControlView::on_changed));
}

//------------------------------------------------------------
// This method is called when the widget is about to be shown.
//------------------------------------------------------------

void Enigma::ControlView::on_map()
{
	// Pass the method to the base class.

	Gtk::Grid::on_map();

	// Update the controls before they are displayed.

	update();
}

//----------------------------------------------
// This method sets the game world to be viewed.
//----------------------------------------------
// world: Game world.
//----------------------------------------------

void Enigma::ControlView::set_world(std::shared_ptr<Enigma::World> world)
{
	m_world = world;
}

//*----------------------------------------------------------*
//* This method is called when any map controls are changed. *
//*----------------------------------------------------------*

void Enigma::ControlView::on_changed()
{
	// Write the world control to the game world.

	m_world->m_savable = m_savable->get_active();
}

//*-------------------------------*
//* This method updates the view. *
//*-------------------------------*

void Enigma::ControlView::update()
{
	// Read the world controls from the game world.  Temporarily block
	// the signal connection to prevent On_Changed() from being called
	// while this occurs.

	m_savable_connection.block();
	m_savable->set_active(m_world->m_savable);
	m_savable_connection.unblock();
}
