// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the MainWindow class implementation.  The MainWindow class
// is the main window of the application.
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
#include "MainWindow.h"

//*---------------------*
//* Local declarations. *
//*---------------------*

static const char* window_title = _("World in the Wine Cellar");

//---------------------
// Default constructor.
//---------------------

Enigma::MainWindow::MainWindow() : Gtk::ApplicationWindow()
{ 
	set_title(window_title);
  set_default_size(160*6, 160*4);
}

//---------------------------------------------------------
// This method appends a short message to the window title.
// eg. "World in the Wine Celler - This is a message."
//---------------------------------------------------------
// message: Message to be added.
//---------------------------------------------------------

void Enigma::MainWindow::set_title_message(const Glib::ustring message)
{	
	if (message.size())
	{
	  Glib::ustring title = window_title;
	  
	  title.append(" - ");
	  title.append(message);
	  set_title(title);
	}
	else
	  set_title(window_title);
}
