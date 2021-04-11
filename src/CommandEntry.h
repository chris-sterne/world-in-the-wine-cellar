// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the CommandEntry class header.  The CommandEntry class
// provides a line to enter an editing command.
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
 
#ifndef __COMMANDENTRY_H__
#define __COMMANDENTRY_H__

#include <gtkmm/grid.h>
#include <gtkmm/entry.h>

namespace Enigma
{
	class CommandEntry : public Gtk::Grid
	{
		public:
			// Public methods.

			CommandEntry();
			void on_enter();

			// Command enter signal accessor.

			typedef sigc::signal<void, const Glib::ustring&> type_signal_command;
			type_signal_command signal_command();

		private:
			// Private data.

			type_signal_command m_signal_command;    // Command enter signal server.
			std::unique_ptr<Gtk::Entry> m_entry;     // Command entry widget.
	};
}

#endif // __MAPSTORE_H__
