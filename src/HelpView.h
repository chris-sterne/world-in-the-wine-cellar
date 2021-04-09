/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 2.0 for Linux OS                    *
 * File:    HelpView.h                          *
 * Date:    December 27, 2015                   *
 * Author:  Chris Sterne                        *
 *                                              *
 * HelpView class header.                       *
 *----------------------------------------------*/

#ifndef __HELPVIEW_H__
#define __HELPVIEW_H__

#include <gtkmm.h>

class CHelpView : public Gtk::ScrolledWindow
{
  public:
    // Public methods.

    CHelpView();
};

#endif // __HELPVIEW_H__