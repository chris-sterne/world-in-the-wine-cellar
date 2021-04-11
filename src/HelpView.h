// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the HelpView class header.  The HelpView class displays
// information on how to use the application.
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

#ifndef __HELPVIEW_H__
#define __HELPVIEW_H__

#include <gtkmm/scrolledwindow.h>

namespace Enigma
{
	class HelpView : public Gtk::ScrolledWindow
	{
		public:
			// Public methods.

			HelpView();
	};
}

#endif // __HELPVIEW_H__
