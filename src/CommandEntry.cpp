/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 4.0 for Linux OS                    *
 * File:    CommandEntry.cpp                    *
 * Date:    September 7, 2016                   *
 * Author:  Chris Sterne                        *
 *                                              *
 * CommandEntry class.                          *
 *----------------------------------------------*
 * This class is for entering a command line.   *
 *----------------------------------------------*/

#include <iostream>
#include <glibmm/i18n.h>
#include "CommandEntry.h"

//*----------------------*
//* Default constructor. *
//*----------------------*

CCommandEntry::CCommandEntry()
{ 
	Gtk::Label* Label = Gtk::manage( new Gtk::Label );
	attach( *Label, 0, 0, 1, 1 );
	Label->set_label( _("Command: ") );
	Label->set_halign( Gtk::ALIGN_START );
	
	iEntry = std::unique_ptr<Gtk::Entry>( new Gtk::Entry );
	attach( *iEntry, 1, 0, 1, 1 );
	iEntry->set_hexpand( TRUE );
	iEntry->set_activates_default( TRUE );

	iEntry->signal_activate().connect( sigc::mem_fun( *this,
	    		                           &CCommandEntry::On_Enter ));
	return;
}

//*---------------------------------------------------------------*
//* This method is called when the enter key is pressed to accept *
//* the entered command line.  The command line is then emitted   *
//* in a signal.                                                  *
//*---------------------------------------------------------------*

void CCommandEntry::On_Enter()
{
	// Move the focus away from the command entry widget.
	
	get_toplevel()->child_focus( Gtk::DIR_TAB_FORWARD );
	
	// Emit the command string in a signal.
	
	Glib::ustring Command = iEntry->get_text();
	m_signal_command.emit( Command );
	return;
}

//*----------------------------------------------------*
//* This method returns the command line enter server. *
//*----------------------------------------------------*

CCommandEntry::type_signal_command CCommandEntry::signal_command()
{
  return m_signal_command;
}