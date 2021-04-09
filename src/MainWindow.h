/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 2.0 for Linux OS                    *
 * File:    MainWindow.h                        *
 * Date:    February 10, 2016                   *
 * Author:  Chris Sterne                        *
 *                                              *
 * MainWindow class header.                     *
 *----------------------------------------------*/
 
#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <gtkmm.h>
#include "MainWindow.h"

class CMainWindow : public Gtk::ApplicationWindow
{
  public:
    // Public methods.
    
    CMainWindow();
    void SetTitleMessage( const Glib::ustring aMessage );		
};

#endif // __MAINWINDOW_H__