// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the MessageBar class implementation.  The MessageBar class
// displays messages while editing or viewing the world.
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
#include "MessageBar.h"

//-------------------------------------------------------------
// This method displays a string description of a map position.
//-------------------------------------------------------------
// position: World position to be displayed.
//-------------------------------------------------------------

void Enigma::MessageBar::set_position(const Enigma::Position& position)
{
	Glib::ustring string =
		Glib::ustring::compose(_("East = %1   North = %2   Above = %3"),
														position.m_east,
    	                     	position.m_north,
     	                      position.m_above);
	set_label(string);
}
