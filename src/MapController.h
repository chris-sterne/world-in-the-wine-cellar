/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 2.0 for Linux OS                    *
 * File:    MapController.h                     *
 * Date:    June 23, 2016                       *
 * Author:  Chris Sterne                        *
 *                                              *
 * MapController class header.                  *
 *----------------------------------------------*/
 
#ifndef __MAPCONTROLLER_H__
#define __MAPCONTROLLER_H__

#include <gtkmm.h>
#include "MapController.h"

class CMapController
{
  public:
    // Public methods.

    CMapController();
    CMapController( const std::string& aName );
    void Compile( const Glib::ustring& aSourceCode );
    void UnCompile( Glib::ustring& aSourceCode );
		
		// Public data.
	  
    std::string iName;           // Controller name.
    std::string iSignalNames;    // Packed array of signal names.
    std::string iRestartCode;    // Compiled restart bytecode.
    std::string iMainCode;       // Compiled main bytecode.

  private:
    // Private methods.
  
    void UnCompile( std::string& aByteCode, Glib::ustring& aSourceCode );
};

#endif // __MAPCONTROLLER_H__