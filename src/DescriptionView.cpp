/*-----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker  *
 * Version: 4.0 for Linux OS                     *
 * File:    DescriptionView.cpp                  *
 * Date:    September 7, 2016                    *
 * Author:  Chris Sterne                         *
 *                                               *
 * DescriptionView class.                        *
 *-----------------------------------------------*
 * This class displays a map description editor. *
 *-----------------------------------------------*/

#include <glibmm/i18n.h>
#include "DescriptionView.h"

//*--------------------------*
//* C++ default constructor. *
//*--------------------------*

CDescriptionView::CDescriptionView()
{
	// Prepare the TextView widget.

  iTextView = std::unique_ptr<Gtk::TextView>( new Gtk::TextView );
	add( *iTextView );
	
	iTextView->set_wrap_mode( Gtk::WRAP_WORD );
  iTextView->set_editable(TRUE);
  iTextView->set_cursor_visible(TRUE);
  iTextView->set_hexpand(TRUE);
  iTextView->set_vexpand(TRUE);

  // Connect signal for text buffer changes.
  
  iTextBufferConnection =  
    iTextView->get_buffer()->signal_changed().connect( sigc::mem_fun( *this,
                                             &CDescriptionView::On_Changed ));

	return;
}

//*-------------------------------------------------------------*
//* This method is called when the widget is about to be shown. *
//*-------------------------------------------------------------*

void CDescriptionView::on_map()
{
  // Pass the method to the base class.

  Gtk::ScrolledWindow::on_map();
  
  // Update the TextView buffer.
  
  Update();
  return;
}

//*---------------------------------------------*
//* This method sets the game map to be viewed. *
//*---------------------------------------------*
//* aMap: Game map.                             *
//*---------------------------------------------*

void CDescriptionView::SetMap( std::shared_ptr<CMap> aMap )
{                                          
  iMap = aMap;
  return;
}

//*------------------------------------------------------------------*
//* This method is called when the text buffer contents are changed. *
//*------------------------------------------------------------------*
//* aMap: Game map.                                                  *
//*------------------------------------------------------------------*

void CDescriptionView::On_Changed()
{
  // Write the description to the game map.

  iMap->iDescription = iTextView->get_buffer()->get_text();
  return;
}

//*-------------------------------*
//* This method updates the view. *
//*-------------------------------*

void CDescriptionView::Update()
{
	// Read the description from the game map.  Block the TextBuffer signal
	// to prevent On_Changed() from being called while this occurs.

  iTextBufferConnection.block();
  iTextView->get_buffer()->set_text( iMap->iDescription );
  iTextBufferConnection.unblock();

  return;
}