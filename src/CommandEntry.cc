// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the CommandEntry class implementation.  The CommandEntry class
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

#include <iostream>
#include <glibmm/i18n.h>
#include "CommandEntry.h"

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::CommandEntry::CommandEntry()
{ 
	Gtk::Label* label = Gtk::manage(new Gtk::Label);
	attach(*label, 0, 0, 1, 1);
	label->set_label(_("Command: "));
	label->set_halign(Gtk::ALIGN_START);

	m_entry = std::make_unique<Gtk::Entry>();
	attach(*m_entry, 1, 0, 1, 1);
	m_entry->set_hexpand(true);
	m_entry->set_activates_default(true);

	m_entry->signal_activate()
		.connect(sigc::mem_fun(*this, &Enigma::CommandEntry::on_enter));
}

//--------------------------------------------------------------------------
// This method is called when the enter key is pressed to accept the entered
// command line.  The command line is then emitted in a signal.
//--------------------------------------------------------------------------

void Enigma::CommandEntry::on_enter()
{
	// Move the focus away from the command entry widget.
	
	get_toplevel()->child_focus(Gtk::DIR_TAB_FORWARD);
	
	// Emit the command string in a signal.
	
	Glib::ustring command = m_entry->get_text();
	m_signal_command.emit(command);
}

//---------------------------------------------------
// This method returns the command line enter server.
//---------------------------------------------------

Enigma::CommandEntry::type_signal_command
	Enigma::CommandEntry::signal_command()
{
  return m_signal_command;
}
