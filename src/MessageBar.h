// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the MessageBar class header.  The MessageBar class displays
// messages while editing or viewing the world.
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
 
#ifndef __MESSAGEBAR_H__
#define __MESSAGEBAR_H__

#include <gtkmm/label.h>
#include "Position.h"

namespace Enigma
{
	class MessageBar : public Gtk::Label
	{
		public:
			// Public methods.

			void set_position(const Enigma::Position& position);
	};
}

#endif // __MESSAGEBAR_H__
