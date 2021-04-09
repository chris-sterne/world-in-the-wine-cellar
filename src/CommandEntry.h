/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 2.0 for Linux OS                    *
 * File:    CommandEntry.h                      *
 * Date:    December 27, 2015                   *
 * Author:  Chris Sterne                        *
 *                                              *
 * MapStore class header.                       *
 *----------------------------------------------*/
 
#ifndef __COMMANDENTRY_H__
#define __COMMANDENTRY_H__

#include <gtkmm.h>

class CCommandEntry : public Gtk::Grid
{
  public:
		// Public methods.

		CCommandEntry();
		void On_Enter();

		// Command enter signal accessor.
		
	  typedef sigc::signal<void, const Glib::ustring&> type_signal_command;
		type_signal_command signal_command();

	private:
		// Private data.

		type_signal_command m_signal_command;    // Command enter signal server.
		std::unique_ptr<Gtk::Entry> iEntry;      // Command entry widget.
};

#endif // __MAPSTORE_H__