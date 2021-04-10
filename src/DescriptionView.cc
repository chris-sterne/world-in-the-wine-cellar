// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the DescriptionView class implementation.  The DescriptionView
// class displays and allows editing a description of the world.
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
