/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 4.0 for Linux OS                    *
 * File:    DescriptionView.h                   *
 * Date:    September 7, 2016                   *
 * Author:  Chris Sterne                        *
 *                                              *
 * DescriptionView class header.                *
 *----------------------------------------------*/

#ifndef __DESCRIPTIONVIEW_H__
#define __DESCRIPTIONVIEW_H__

#include <gtkmm.h>
#include "Map.h"

class CDescriptionView : public Gtk::ScrolledWindow
{
  public:
    // Public methods.

    CDescriptionView();
    void SetMap( std::shared_ptr<CMap> aMap );
    void On_Changed();
    void Update();

  protected:
    // Overridden base class methods.

    void on_map();

  private:
    // Private data.

		std::shared_ptr<CMap> iMap;                  // Shared Game map.
    std::unique_ptr<Gtk::TextView> iTextView;    // TextView widget.
    sigc::connection iTextBufferConnection;      // Signal connection.
    
};

#endif // __DESCRIPTIONVIEW_H__