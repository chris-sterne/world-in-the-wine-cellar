/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 5.1 for Linux OS                    *
 * File:    ImageTiles.h                        *
 * Date:    September 8, 2017                   *
 * Author:  Chris Sterne                        *
 *                                              *
 * ImageTiles class header.                     *
 *----------------------------------------------*/
 
#ifndef __IMAGETILES_H__
#define __IMAGETILES_H__

#include <gtkmm.h>
#include "MapObject.h"
#include "MapVolume.h"

class CImageTiles
{
  public:
		// Public methods.

		CImageTiles();
		int GetTileSize() const;
		
    gboolean DrawMapObject( const Cairo::RefPtr<Cairo::Context>& aCC, 
                            Gtk::Allocation aAllocation,
                            guint16 aColumn,
                            guint16 aRow,
                            CMapObject& aObject );

    void DrawArrival( const Cairo::RefPtr<Cairo::Context>& aCC,
                      Gtk::Allocation aAllocation,                      
                      guint16 aColumn,
                      guint16 aRow );

    void DrawGeneric( const Cairo::RefPtr<Cairo::Context>& aCC,
                      Gtk::Allocation aAllocation,                      
                      guint16 aColumn,
                      guint16 aRow );

    void DrawCursor( const Cairo::RefPtr<Cairo::Context>& aCC,
                     Gtk::Allocation aAllocation,
                     guint16 aColumn,
                     guint16 aRow );

    void DrawMarker( const Cairo::RefPtr<Cairo::Context>& aCC,
                     Gtk::Allocation aAllocation,
                     guint16 aColumn,
                     guint16 aRow );

		// Public data.

		Cairo::RefPtr<Cairo::ImageSurface> iCursorImage;
    Cairo::RefPtr<Cairo::ImageSurface> iMarkerImage;
    Cairo::RefPtr<Cairo::ImageSurface> iGenericImage;
		Cairo::RefPtr<Cairo::ImageSurface> iWallImage;
		Cairo::RefPtr<Cairo::ImageSurface> iCeilingImage;
		Cairo::RefPtr<Cairo::ImageSurface> iFloorImage;
		Cairo::RefPtr<Cairo::ImageSurface> iLadderImage;
		Cairo::RefPtr<Cairo::ImageSurface> iLadderEndImage;
    Cairo::RefPtr<Cairo::ImageSurface> iLadderEndHorizontalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iLadderEndCeilingImage;
    Cairo::RefPtr<Cairo::ImageSurface> iLadderEndFloorImage;
    Cairo::RefPtr<Cairo::ImageSurface> iLadderHorizontalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iLadderCeilingImage;
    Cairo::RefPtr<Cairo::ImageSurface> iLadderFloorImage;
		Cairo::RefPtr<Cairo::ImageSurface> iPlayerActiveImage;
		Cairo::RefPtr<Cairo::ImageSurface> iPlayerIdleImage;
    Cairo::RefPtr<Cairo::ImageSurface> iBlockerImage;
    Cairo::RefPtr<Cairo::ImageSurface> iBlockerHorizontalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iMoverImage;
    Cairo::RefPtr<Cairo::ImageSurface> iMoverAboveImage;
    Cairo::RefPtr<Cairo::ImageSurface> iMoverBelowImage;
    Cairo::RefPtr<Cairo::ImageSurface> iTurnerImage;
    Cairo::RefPtr<Cairo::ImageSurface> iItemImage;
    Cairo::RefPtr<Cairo::ImageSurface> iSurfacerImage;
    Cairo::RefPtr<Cairo::ImageSurface> iCornerSurfacerImage;
    Cairo::RefPtr<Cairo::ImageSurface> iFlipperImage;
    Cairo::RefPtr<Cairo::ImageSurface> iStairsImage;
    Cairo::RefPtr<Cairo::ImageSurface> iStairsTopImage;
    Cairo::RefPtr<Cairo::ImageSurface> iHandHoldImage;
    Cairo::RefPtr<Cairo::ImageSurface> iHandHoldHorizontalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iOutdoorImage;
    Cairo::RefPtr<Cairo::ImageSurface> iOutdoorVerticalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iOutdoorCeilingImage;
    Cairo::RefPtr<Cairo::ImageSurface> iOutdoorFloorImage;
    Cairo::RefPtr<Cairo::ImageSurface> iIndoorImage;
    Cairo::RefPtr<Cairo::ImageSurface> iIndoorVerticalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iIndoorCeilingImage;
    Cairo::RefPtr<Cairo::ImageSurface> iIndoorFloorImage;
    Cairo::RefPtr<Cairo::ImageSurface> iArchWayVerticalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iArchWayCeilingImage;
    Cairo::RefPtr<Cairo::ImageSurface> iArchWayFloorImage;
    Cairo::RefPtr<Cairo::ImageSurface> iWoodDoorVerticalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iWoodDoorCeilingImage;
    Cairo::RefPtr<Cairo::ImageSurface> iWoodDoorFloorImage;
    Cairo::RefPtr<Cairo::ImageSurface> iWoodWallVerticalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iWoodWallCeilingImage;
    Cairo::RefPtr<Cairo::ImageSurface> iWoodWallFloorImage;
    Cairo::RefPtr<Cairo::ImageSurface> iPullRingVerticalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iPullRingHorizontalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iLockVerticalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iLockHorizontalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iWaterLayerVerticalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iWaterLayerBelowImage;
    Cairo::RefPtr<Cairo::ImageSurface> iWaterLayerAboveImage;
    Cairo::RefPtr<Cairo::ImageSurface> iLightBeamHorizontalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iLightBeamVerticalImage;   
    Cairo::RefPtr<Cairo::ImageSurface> iTreeImage;
    Cairo::RefPtr<Cairo::ImageSurface> iTreeHorizontalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iTreeCeilingImage;
    Cairo::RefPtr<Cairo::ImageSurface> iTreeFloorImage;
		Cairo::RefPtr<Cairo::ImageSurface> iTreeTopImage;
    Cairo::RefPtr<Cairo::ImageSurface> iTreeTopHorizontalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iTreeTopCeilingImage;
    Cairo::RefPtr<Cairo::ImageSurface> iTreeTopFloorImage; 
    Cairo::RefPtr<Cairo::ImageSurface> iCatWalkCeilingImage;
    Cairo::RefPtr<Cairo::ImageSurface> iCatWalkFloorImage;
    Cairo::RefPtr<Cairo::ImageSurface> iCatWalkHorizontalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iCatWalkVerticalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iTeleporterDepartureImage;
    Cairo::RefPtr<Cairo::ImageSurface> iTeleporterArrivalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iFernFloorImage;
    Cairo::RefPtr<Cairo::ImageSurface> iWaterImage;
    Cairo::RefPtr<Cairo::ImageSurface> iEarthWallVerticalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iEarthWallCeilingImage;
    Cairo::RefPtr<Cairo::ImageSurface> iEarthWallFloorImage;
    Cairo::RefPtr<Cairo::ImageSurface> iPadButtonVerticalImage;
    Cairo::RefPtr<Cairo::ImageSurface> iPadButtonCeilingImage;
    Cairo::RefPtr<Cairo::ImageSurface> iPadButtonFloorImage;   
};

#endif // __IMAGETILES_H__