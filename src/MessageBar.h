/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 2.0 for Linux OS                    *
 * File:    MessageBar.h                        *
 * Date:    March 16, 2016                      *
 * Author:  Chris Sterne                        *
 *                                              *
 * MessageBar class header.                     *
 *----------------------------------------------*/
 
#ifndef __MESSAGEBAR_H__
#define __MESSAGEBAR_H__

#include <gtkmm.h>
#include "MapLocation.h"

class CMessageBar : public Gtk::Label
{
  public:
		// Public methods.

		void SetLocation( const CMapLocation& aLocation );
};

#endif // __MESSAGEBAR_H__