/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 4.0 for Linux OS                    *
 * File:    ControlView.h                       *
 * Date:    September 7, 2015                   *
 * Author:  Chris Sterne                        *
 *                                              *
 * ControlView class header.                    *
 *----------------------------------------------*/

#ifndef __CONTROLVIEW_H__
#define __CONTROLVIEW_H__

#include <gtkmm.h>
#include "Map.h"

class CControlView : public Gtk::Grid
{
  public:
    // Public methods.

    CControlView();
    void SetMap( std::shared_ptr<CMap> aMap );
    void On_Changed();
    void Update();
    
  protected:
    // Overridden base class methods.

    void on_map();
    
    // Public data.

    std::shared_ptr<CMap> iMap;                     // Shared Game map.
    std::unique_ptr< Gtk::CheckButton > iSavable;   // Can save map button.
    sigc::connection iSavableConnection;            // Signal connection.
};

#endif // __CONTROLVIEW_H__