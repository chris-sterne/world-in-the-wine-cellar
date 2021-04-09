/*----------------------------------------*
 * Program: Enigma in the Wine Cellar     *
 * Version: 4.0 for Linux OS              *
 * File:    ControlView.cpp               *
 * Date:    September 7, 2016             *
 * Author:  Chris Sterne                  *
 *                                        *
 * ControlView class.                     *
 *----------------------------------------*
 * This class displays game map controls. *
 *----------------------------------------*/

#include <glibmm/i18n.h>
#include "ControlView.h"

//*--------------------------*
//* C++ default constructor. *
//*--------------------------*

CControlView::CControlView()
{
  set_border_width( 10 );
  set_halign( Gtk::ALIGN_START );
  set_column_homogeneous( FALSE );

  iSavable = std::unique_ptr<Gtk::CheckButton>(
             new Gtk::CheckButton(_("Able to save map in game.") ));

  attach( *iSavable, 0, 0, 1, 1 );
  
  // Connect signal for control changes.
  
  iSavableConnection = 
    iSavable->signal_toggled().connect( sigc::mem_fun( *this,
                                        &CControlView::On_Changed ));
  
  return;
}

//*-------------------------------------------------------------*
//* This method is called when the widget is about to be shown. *
//*-------------------------------------------------------------*

void CControlView::on_map()
{
	// Pass the method to the base class.

	Gtk::Grid::on_map();
	
	// Update the controls before they are displayed.
	
	Update();
	return;
}

//*---------------------------------------------*
//* This method sets the game map to be viewed. *
//*---------------------------------------------*
//* aMap: Game map.                             *
//*---------------------------------------------*

void CControlView::SetMap( std::shared_ptr<CMap> aMap )
{
  iMap = aMap;
  return;
}

//*----------------------------------------------------------*
//* This method is called when any map controls are changed. *
//*----------------------------------------------------------*

void CControlView::On_Changed()
{
	// Write the map control to the game map.
	
	iMap->iSavable = iSavable->get_active();
	return;
}

//*-------------------------------*
//* This method updates the view. *
//*-------------------------------*

void CControlView::Update()
{
  // Read the map controls from the game map.  Temporarily block the signal
  // connection to prevent On_Changed() from being called while this occurs.

	iSavableConnection.block();
	iSavable->set_active( iMap->iSavable );
	iSavableConnection.unblock();
  return;
}