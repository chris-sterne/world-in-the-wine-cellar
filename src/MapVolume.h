/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 3.0 for Linux OS                    *
 * File:    MapVolume.h                         *
 * Date:    April 7, 2016                       *
 * Author:  Chris Sterne                        *
 *                                              *
 * MapVolume class header.                      *
 *----------------------------------------------*/
 
#ifndef __MAPVOLUME_H__
#define __MAPVOLUME_H__

#include <gtkmm.h>
#include "MapLocation.h"

class CMapVolume
{
  public:
		// Public data.

		CMapLocation iWSB;     // West-South-Below (lowest) corner of volume.
		CMapLocation iENA;     // East-North-Above (highest) corner of volume.
};

#endif // __MAPVOLUME_H__