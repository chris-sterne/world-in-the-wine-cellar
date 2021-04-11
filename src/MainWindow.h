// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the MainWindow class header.  The MainWindow class is the
// main window of the application.
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
 
#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <gtkmm/applicationwindow.h>

namespace Enigma
{
	class MainWindow : public Gtk::ApplicationWindow
	{
		public:
			// Public methods.

			MainWindow();
			void set_title_message(const Glib::ustring message);		
	};
}

#endif // __MAINWINDOW_H__
