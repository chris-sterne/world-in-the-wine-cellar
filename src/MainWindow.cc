/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 2.0 for Linux OS                    *
 * File:    MainWindow.cpp                      *
 * Date:    February 10, 2016                   *
 * Author:  Chris Sterne                        *
 *                                              *
 * MainWindow class.                            *
 *----------------------------------------------*
 * This class is the main application window.   *
 *----------------------------------------------*/

#include <glibmm/i18n.h>
#include "MainWindow.h"

//*---------------------*
//* Local declarations. *
//*---------------------*

static const char* KTitle = _("Enigma in the Wine Cellar Mapper");

//*----------------------*
//* Default constructor. *
//*----------------------*

CMainWindow::CMainWindow() : Gtk::ApplicationWindow()
{ 
	set_title( KTitle );
  set_default_size( 160*6, 160*4 );
	return;
}

//*-------------------------------------------------*
//* This method adds a message to the window title. *
//*-------------------------------------------------*

void CMainWindow::SetTitleMessage( const Glib::ustring aMessage )
{	
	if ( aMessage.size() )
	{
	  Glib::ustring Title = KTitle;
	  
	  Title.append( " - " );
	  Title.append( aMessage );
	  set_title( Title );
	}
	else
	  set_title( KTitle );

	return;
}