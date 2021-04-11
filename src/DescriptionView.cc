// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the DescriptionView class implementation.  The DescriptionView
// class displays and allows editing a description of the world.
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
#include "DescriptionView.h"

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::DescriptionView::DescriptionView()
{
// Prepare the TextView widget.

m_textview = std::make_unique<Gtk::TextView>();
add(*m_textview);

m_textview->set_wrap_mode( Gtk::WRAP_WORD );
m_textview->set_editable(true);
m_textview->set_cursor_visible(true);
m_textview->set_hexpand(true);
m_textview->set_vexpand(true);

// Connect signal for text buffer changes.

m_textbuffer_connection = m_textview->get_buffer()->signal_changed()
	.connect(sigc::mem_fun(*this, &Enigma::DescriptionView::on_changed));
}

//------------------------------------------------------------
// This method is called when the widget is about to be shown.
//------------------------------------------------------------

void Enigma::DescriptionView::on_map()
{
	// Pass the method to the base class.

	Gtk::ScrolledWindow::on_map();

	// Update the TextView buffer.

	update();
}

//----------------------------------------------
// This method sets the game world to be viewed.
//----------------------------------------------
// world: Game world.
//----------------------------------------------

void Enigma::DescriptionView::set_world(std::shared_ptr<Enigma::World> world)
{                                          
	m_world = world;
}

//-----------------------------------------------------------------
// This method is called when the text buffer contents are changed.
//-----------------------------------------------------------------
// world: Game world.
//-----------------------------------------------------------------

void Enigma::DescriptionView::on_changed()
{
	// Write the description to the game map.

	m_world->m_description = m_textview->get_buffer()->get_text();
}

//------------------------------
// This method updates the view.
//------------------------------

void Enigma::DescriptionView::update()
{
	// Read the description from the game map.  Block the TextBuffer signal
	// to prevent on_changed() from being called while this occurs.

	m_textbuffer_connection.block();
	m_textview->get_buffer()->set_text(m_world->m_description);
	m_textbuffer_connection.unblock();
}
