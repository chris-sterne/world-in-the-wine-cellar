/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 5.1 for Linux OS                    *
 * File:    ImageTiles.cpp                      *
 * Date:    September 7, 2017                   *
 * Author:  Chris Sterne                        *
 *                                              *
 * ImageTiles class.                            *
 *----------------------------------------------*
 * This class handles map image tile drawing.   *
 *----------------------------------------------*/

#include "ImageTiles.h"

//*--------------------*
//* Local declarations *
//*--------------------*

#define KTileSize 60         // Pixel size of image tile.
               
//*----------------------*
//* Default constructor. *
//*----------------------*

CImageTiles::CImageTiles()
{ 
  iCursorImage =
    Cairo::ImageSurface::create_from_png( "./Images/Cursor.png" );

  iMarkerImage =
    Cairo::ImageSurface::create_from_png( "./Images/Marker.png" );

  iGenericImage =
    Cairo::ImageSurface::create_from_png( "./Images/Generic.png" );

  iWallImage = 
    Cairo::ImageSurface::create_from_png( "./Images/Wall.png" );

  iCeilingImage =
    Cairo::ImageSurface::create_from_png( "./Images/Ceiling.png" );

  iFloorImage =
    Cairo::ImageSurface::create_from_png( "./Images/Floor.png" );

  iLadderImage =
    Cairo::ImageSurface::create_from_png( "./Images/Ladder.png" );

  iLadderEndImage =
    Cairo::ImageSurface::create_from_png( "./Images/LadderEnd.png" );

  iLadderEndHorizontalImage =
    Cairo::ImageSurface::create_from_png( "./Images/LadderEndHorizontal.png" );
 
  iLadderEndCeilingImage =
    Cairo::ImageSurface::create_from_png( "./Images/LadderEndCeiling.png" );
 
  iLadderEndFloorImage =
    Cairo::ImageSurface::create_from_png( "./Images/LadderEndFloor.png" );
 
  iLadderHorizontalImage =
    Cairo::ImageSurface::create_from_png( "./Images/LadderHorizontal.png" );

  iLadderCeilingImage =
    Cairo::ImageSurface::create_from_png( "./Images/LadderCeiling.png" );

  iLadderFloorImage =
    Cairo::ImageSurface::create_from_png( "./Images/LadderFloor.png" );
  
  iPlayerActiveImage =
    Cairo::ImageSurface::create_from_png( "./Images/PlayerActive.png" );

  iPlayerIdleImage =
    Cairo::ImageSurface::create_from_png( "./Images/PlayerIdle.png" );

  iBlockerImage =
    Cairo::ImageSurface::create_from_png( "./Images/Blocker.png" );

  iBlockerHorizontalImage =
    Cairo::ImageSurface::create_from_png( "./Images/BlockerHorizontal.png" );

  iMoverImage =
    Cairo::ImageSurface::create_from_png( "./Images/Mover.png" );

  iMoverBelowImage =
    Cairo::ImageSurface::create_from_png( "./Images/MoverBelow.png" );

  iMoverAboveImage =
    Cairo::ImageSurface::create_from_png( "./Images/MoverAbove.png" );

  iTurnerImage =
    Cairo::ImageSurface::create_from_png( "./Images/Turner.png" );

  iItemImage =
    Cairo::ImageSurface::create_from_png( "./Images/Item.png" );
  
  iFlipperImage =
    Cairo::ImageSurface::create_from_png( "./Images/Flipper.png" );

  iSurfacerImage =
    Cairo::ImageSurface::create_from_png( "./Images/Surfacer.png" );

  iCornerSurfacerImage =
    Cairo::ImageSurface::create_from_png( "./Images/CornerSurfacer.png" );

  iStairsImage =
    Cairo::ImageSurface::create_from_png( "./Images/Stairs.png" );

  iStairsTopImage =
    Cairo::ImageSurface::create_from_png( "./Images/StairsTop.png" );

  iHandHoldImage =
    Cairo::ImageSurface::create_from_png( "./Images/HandHold.png" );

  iHandHoldHorizontalImage =
    Cairo::ImageSurface::create_from_png( "./Images/HandHoldHorizontal.png" );

  iOutdoorImage =
    Cairo::ImageSurface::create_from_png( "./Images/Outdoor.png" );

  iOutdoorVerticalImage =
    Cairo::ImageSurface::create_from_png( "./Images/OutdoorVertical.png" );
    
  iOutdoorCeilingImage =
    Cairo::ImageSurface::create_from_png( "./Images/OutdoorCeiling.png" );
    
  iOutdoorFloorImage =
    Cairo::ImageSurface::create_from_png( "./Images/OutdoorFloor.png" );

  iIndoorImage =
    Cairo::ImageSurface::create_from_png( "./Images/Indoor.png" );

  iIndoorVerticalImage =
    Cairo::ImageSurface::create_from_png( "./Images/IndoorVertical.png" );
    
  iIndoorCeilingImage =
    Cairo::ImageSurface::create_from_png( "./Images/IndoorCeiling.png" );
    
  iIndoorFloorImage =
    Cairo::ImageSurface::create_from_png( "./Images/IndoorFloor.png" );

  iArchWayVerticalImage =
    Cairo::ImageSurface::create_from_png( "./Images/ArchWayVertical.png" );

  iArchWayCeilingImage =
    Cairo::ImageSurface::create_from_png( "./Images/ArchWayCeiling.png" );

  iArchWayFloorImage =
    Cairo::ImageSurface::create_from_png( "./Images/ArchWayFloor.png" );

  iWoodDoorVerticalImage =
    Cairo::ImageSurface::create_from_png( "./Images/WoodDoorVertical.png" );

  iWoodDoorCeilingImage =
    Cairo::ImageSurface::create_from_png( "./Images/WoodDoorCeiling.png" );

  iWoodDoorFloorImage =
    Cairo::ImageSurface::create_from_png( "./Images/WoodDoorFloor.png" );

  iWoodWallVerticalImage =
    Cairo::ImageSurface::create_from_png( "./Images/WoodWallVertical.png" );

  iWoodWallCeilingImage =
    Cairo::ImageSurface::create_from_png( "./Images/WoodWallCeiling.png" );

  iWoodWallFloorImage =
    Cairo::ImageSurface::create_from_png( "./Images/WoodWallFloor.png" );

  iPullRingHorizontalImage =
    Cairo::ImageSurface::create_from_png( "./Images/PullRingHorizontal.png" );

  iPullRingVerticalImage =
    Cairo::ImageSurface::create_from_png( "./Images/PullRingVertical.png" );

  iLockHorizontalImage =
    Cairo::ImageSurface::create_from_png( "./Images/LockHorizontal.png" );

  iLockVerticalImage =
    Cairo::ImageSurface::create_from_png( "./Images/LockVertical.png" );

  iWaterLayerVerticalImage =
    Cairo::ImageSurface::create_from_png( "./Images/WaterLayerVertical.png" );

  iWaterLayerBelowImage =
    Cairo::ImageSurface::create_from_png( "./Images/WaterLayerBelow.png" );

  iWaterLayerAboveImage =
    Cairo::ImageSurface::create_from_png( "./Images/WaterLayerAbove.png" );

  iLightBeamHorizontalImage =
    Cairo::ImageSurface::create_from_png( "./Images/LightBeamHorizontal.png" );

  iLightBeamVerticalImage =
    Cairo::ImageSurface::create_from_png( "./Images/LightBeamVertical.png" );

  iTreeImage =
    Cairo::ImageSurface::create_from_png( "./Images/Tree.png" );
    
  iTreeHorizontalImage =
    Cairo::ImageSurface::create_from_png( "./Images/TreeHorizontal.png" );

  iTreeCeilingImage =
    Cairo::ImageSurface::create_from_png( "./Images/TreeCeiling.png" );

  iTreeFloorImage =
    Cairo::ImageSurface::create_from_png( "./Images/TreeFloor.png" );

  iTreeTopImage =
    Cairo::ImageSurface::create_from_png( "./Images/TreeTop.png" );

  iTreeTopHorizontalImage =
    Cairo::ImageSurface::create_from_png( "./Images/TreeTopHorizontal.png" );
 
  iTreeTopCeilingImage =
    Cairo::ImageSurface::create_from_png( "./Images/TreeTopCeiling.png" );
 
  iTreeTopFloorImage =
    Cairo::ImageSurface::create_from_png( "./Images/TreeTopFloor.png" );

  iCatWalkCeilingImage =
    Cairo::ImageSurface::create_from_png( "./Images/CatWalkCeiling.png" );
    
  iCatWalkFloorImage =
    Cairo::ImageSurface::create_from_png( "./Images/CatWalkFloor.png" );
    
  iCatWalkHorizontalImage =
    Cairo::ImageSurface::create_from_png( "./Images/CatWalkHorizontal.png" );
    
  iCatWalkVerticalImage =
    Cairo::ImageSurface::create_from_png( "./Images/CatWalkVertical.png" );

  iTeleporterDepartureImage =
    Cairo::ImageSurface::create_from_png( "./Images/TeleporterDeparture.png" );
 
  iTeleporterArrivalImage =
    Cairo::ImageSurface::create_from_png( "./Images/TeleporterArrival.png" ); 
 
  iFernFloorImage =
    Cairo::ImageSurface::create_from_png( "./Images/FernFloor.png" );  
 
  iWaterImage =
    Cairo::ImageSurface::create_from_png( "./Images/Water.png" );
    
  iEarthWallVerticalImage =
    Cairo::ImageSurface::create_from_png( "./Images/EarthWallVertical.png" );

  iEarthWallCeilingImage =
    Cairo::ImageSurface::create_from_png( "./Images/EarthWallCeiling.png" );

  iEarthWallFloorImage =
    Cairo::ImageSurface::create_from_png( "./Images/EarthWallFloor.png" );
 
  iPadButtonCeilingImage =
    Cairo::ImageSurface::create_from_png( "./Images/PadButtonCeiling.png" );
 
  iPadButtonFloorImage =
    Cairo::ImageSurface::create_from_png( "./Images/PadButtonFloor.png" );
 
  iPadButtonVerticalImage =
    Cairo::ImageSurface::create_from_png( "./Images/PadButtonVertical.png" );
 
  return;
}

//*------------------------------------------------*
//* This method returns the image tile pixel size. *
//*------------------------------------------------*
//* RETURN: Tile size.                             *                             
//*------------------------------------------------*

int CImageTiles::GetTileSize() const
{
	return KTileSize;
}

//*-------------------------------------------------------------------*
//* This private function draws an image tile.                        *
//*-------------------------------------------------------------------*
//* aCC:    Cairo drawing context.                                    *
//* aX:     X screen pixel coordinate of tile.                        *
//* aY:     Y screen pixel coordinate of tile.                        *
//* aR:     Rotation amount (0, 90, 180, 270 degrees).                *
//* aImage: Image surface to draw.                                    *
//*-------------------------------------------------------------------*
//* RETURN:   TRUE to draw tile.                                      *
//*-------------------------------------------------------------------*

void DrawTile( const Cairo::RefPtr<Cairo::Context>& aCC,
               double aX,
               double aY,
               double aR,
               Cairo::RefPtr<Cairo::ImageSurface> aImage )
{
	if ( aR == 90 )
	{
		aX += KTileSize;
	}
	else if ( aR == 180 )
	{
		aX += KTileSize;
		aY += KTileSize;
	}	
	else if ( aR == 270 )
	{
		aY += KTileSize;
	}
	else
	{
		aR = 0;
	}

	aCC->save();
	aCC->translate( aX, aY );
	aCC->rotate_degrees( aR );
	
	aCC->set_source( aImage, 0, 0 );
	aCC->paint();
	aCC->restore();
	
	return;
}

//*---------------------------------------------------------*
//* This method draws a cursor tile.  The lower-left corner *
//* of the view is the origin (Row = 0, Column = 0).        *  
//*---------------------------------------------------------*
//* aCC:         Cairo context for drawing.                 *
//* aAllocation: Pixel boundary of view.                    *
//* aColumn:     Cursor column number.                      *
//* aRow:        Cursor row number.                         *
//*---------------------------------------------------------*

void CImageTiles::DrawCursor( const Cairo::RefPtr<Cairo::Context>& aCC,
                             Gtk::Allocation aAllocation,
                             guint16 aColumn,
                             guint16 aRow )
{
  double X = aColumn * KTileSize;
  double Y = aAllocation.get_height()
           - KTileSize
           - (aRow * KTileSize);

  aCC->set_source( iCursorImage, X, Y );
  aCC->paint();
	
	return;
}

//*---------------------------------------------------------*
//* This method draws a marker tile.  The lower-left corner *
//* of the view is the origin (Row = 0, Column = 0).        *
//*---------------------------------------------------------*
//* aCC:         Cairo context for drawing.                 *
//* aAllocation: Pixel boundary of view.                    *
//* aColumn:     Cursor column number.                      *
//* aRow:        Cursor row number.                         *
//*---------------------------------------------------------*

void CImageTiles::DrawMarker( const Cairo::RefPtr<Cairo::Context>& aCC,
                             Gtk::Allocation aAllocation,
                             guint16 aColumn,
                             guint16 aRow )
{
	double X = aColumn * KTileSize;
  double Y = aAllocation.get_height()
           - KTileSize
           - (aRow * KTileSize);

	aCC->set_source( iMarkerImage, X, Y );
	aCC->paint();
	
	return;
}

//*---------------------------------------------------------------*
//* This method draws a tile for a generic object.  This would be *   
//* used for objects that have no unique image, but require some  *
//* image to indicate their presence.                             *
//*---------------------------------------------------------------*
//* aCC:         Cairo context for drawing.                       *
//* aAllocation: Pixel boundary of view.                          *
//* aColumn:     Cursor column number.                            *
//* aRow:        Cursor row number.                               *
//*---------------------------------------------------------------*

void CImageTiles::DrawGeneric( const Cairo::RefPtr<Cairo::Context>& aCC,
                              Gtk::Allocation aAllocation,
                              guint16 aColumn,
                              guint16 aRow )
{
	double X = aColumn * KTileSize;
  double Y = aAllocation.get_height()
           - KTileSize
           - (aRow * KTileSize);

	aCC->set_source( iGenericImage, X, Y );
	aCC->paint();
	
	return;
}

//*-----------------------------------------------------------------*
//* This method draws a tile marking a teleporter arrival location. *
//*-----------------------------------------------------------------*
//* aCC:         Cairo context for drawing.                         *
//* aAllocation: Pixel boundary of view.                            *
//* aColumn:     Cursor column number.                              *
//* aRow:        Cursor row number.                                 *
//*-----------------------------------------------------------------*

void CImageTiles::DrawArrival( const Cairo::RefPtr<Cairo::Context>& aCC,
                               Gtk::Allocation aAllocation,
                               guint16 aColumn,
                               guint16 aRow )
{
	double X = aColumn * KTileSize;
  double Y = aAllocation.get_height()
           - KTileSize
           - (aRow * KTileSize);

	aCC->set_source( iTeleporterArrivalImage, X, Y );
	aCC->paint();
	
	return;
}

//*-------------------------------------------------------------*
//* This method draws a map object tile.  The lower-left corner *
//* of the view is the origin (Row = 0, Column = 0).            *
//*-------------------------------------------------------------*
//* aCC:         Cairo context for drawing.                     *
//* aAllocation: Pixel boundary of view.                        *
//* aColumn:     Visible column of map level.                   *
//* aRow:        Visible row of map level.                      *
//* aObject:     Map object to draw.                            *
//* RETURN:      TRUE if an image was drawn.                    *
//*-------------------------------------------------------------*

gboolean CImageTiles::DrawMapObject( const Cairo::RefPtr<Cairo::Context>& aCC,
                                     Gtk::Allocation aAllocation,
                                     guint16 aColumn,
                                     guint16 aRow,
                                     CMapObject& aObject)
{
  gboolean Drawn = TRUE;
  
  double X = aColumn * KTileSize;
  double Y = aAllocation.get_height()
           - KTileSize
           - (aRow * KTileSize);

  if ( aObject.iType == CMapObject::Type::EItem )
  {
    // Draw a general image for all items.
    
		DrawTile( aCC, X, Y, 0, iItemImage );
  }
  else if ( aObject.iType == CMapObject::Type::ETeleporter )
  {
    // Draw a general image for all teleporters.
    
		DrawTile( aCC, X, Y, 0, iTeleporterDepartureImage );
  }
	else if ( aObject.iID == EnigmaWC::ID::EStairs )
	{
    switch( aObject.iRotation )
		{
			case EnigmaWC::Direction::ENorth:
				DrawTile( aCC, X, Y, 0, iStairsImage ); 
				break;

			case EnigmaWC::Direction::ESouth:
				DrawTile( aCC, X, Y, 180, iStairsImage );
				break;

			case EnigmaWC::Direction::EEast:
				DrawTile( aCC, X, Y, 90, iStairsImage );
				break;

			case EnigmaWC::Direction::EWest:
				DrawTile( aCC, X, Y, 270, iStairsImage );
				break;

			default:
        Drawn = FALSE;
				break;
		}
	}
	else if ( aObject.iID == EnigmaWC::ID::EStairsTop )
	{
    switch( aObject.iRotation )
		{
			case EnigmaWC::Direction::ENorth:
				DrawTile( aCC, X, Y, 0, iStairsTopImage ); 
				break;

			case EnigmaWC::Direction::ESouth:
				DrawTile( aCC, X, Y, 180, iStairsTopImage );
				break;

			case EnigmaWC::Direction::EEast:
				DrawTile( aCC, X, Y, 90, iStairsTopImage );
				break;

			case EnigmaWC::Direction::EWest:
				DrawTile( aCC, X, Y, 270, iStairsTopImage );
				break;

			default:
        Drawn = FALSE;
				break;
		}
	}
	else if ( aObject.iID == EnigmaWC::ID::EArchWay )
	{
		switch( aObject.iSurface )
		{
			case EnigmaWC::Direction::ENorth:
				DrawTile( aCC, X, Y, 0, iArchWayVerticalImage ); 
				break;

			case EnigmaWC::Direction::ESouth:
				DrawTile( aCC, X, Y, 180, iArchWayVerticalImage );
				break;

			case EnigmaWC::Direction::EEast:
				DrawTile( aCC, X, Y, 90, iArchWayVerticalImage );
				break;

			case EnigmaWC::Direction::EWest:
				DrawTile( aCC, X, Y, 270, iArchWayVerticalImage );
				break;

			case EnigmaWC::Direction::EAbove:
				DrawTile( aCC, X, Y, 270, iArchWayCeilingImage );
				break;
				
			case EnigmaWC::Direction::EBelow:
				DrawTile( aCC, X, Y, 270, iArchWayFloorImage );
				break;

			default:
        Drawn = FALSE;
				break;
		}
	}
	else if ( aObject.iID == EnigmaWC::ID::EWoodDoor )
	{
		switch( aObject.iSurface )
		{
			case EnigmaWC::Direction::ENorth:
				DrawTile( aCC, X, Y, 0, iWoodDoorVerticalImage ); 
				break;

			case EnigmaWC::Direction::ESouth:
				DrawTile( aCC, X, Y, 180, iWoodDoorVerticalImage );
				break;

			case EnigmaWC::Direction::EEast:
				DrawTile( aCC, X, Y, 90, iWoodDoorVerticalImage );
				break;

			case EnigmaWC::Direction::EWest:
				DrawTile( aCC, X, Y, 270, iWoodDoorVerticalImage );
				break;

			case EnigmaWC::Direction::EAbove:
				DrawTile( aCC, X, Y, 270, iWoodDoorCeilingImage );
				break;
				
			case EnigmaWC::Direction::EBelow:
				DrawTile( aCC, X, Y, 270, iWoodDoorFloorImage );
				break;

			default:
        Drawn = FALSE;
				break;
		}
	}
  else if ( aObject.iID == EnigmaWC::ID::EPullRing )
	{
		switch( aObject.iSurface )
		{
			case EnigmaWC::Direction::ENorth:
				DrawTile( aCC, X, Y, 0, iPullRingVerticalImage ); 
				break;

			case EnigmaWC::Direction::ESouth:
				DrawTile( aCC, X, Y, 180, iPullRingVerticalImage );
				break;

			case EnigmaWC::Direction::EEast:
				DrawTile( aCC, X, Y, 90, iPullRingVerticalImage );
				break;

			case EnigmaWC::Direction::EWest:
				DrawTile( aCC, X, Y, 270, iPullRingVerticalImage );
				break;

			case EnigmaWC::Direction::EAbove:
			case EnigmaWC::Direction::EBelow:
				DrawTile( aCC, X, Y, 0, iPullRingHorizontalImage );	
        break;

			default:
        Drawn = FALSE;
				break;
		}
	}
  else if (( aObject.iID == EnigmaWC::ID::ECubeLock )
        || ( aObject.iID == EnigmaWC::ID::ESphereLock )
        || ( aObject.iID == EnigmaWC::ID::EWaterLock )
        || ( aObject.iID == EnigmaWC::ID::EAppleLock )
        || ( aObject.iID == EnigmaWC::ID::EOrangeLock )
        || ( aObject.iID == EnigmaWC::ID::EWineLock ))
	{
		switch( aObject.iSurface )
		{
			case EnigmaWC::Direction::ENorth:
				DrawTile( aCC, X, Y, 0, iLockVerticalImage ); 
				break;

			case EnigmaWC::Direction::ESouth:
				DrawTile( aCC, X, Y, 180, iLockVerticalImage );
				break;

			case EnigmaWC::Direction::EEast:
				DrawTile( aCC, X, Y, 90, iLockVerticalImage );
				break;

			case EnigmaWC::Direction::EWest:
				DrawTile( aCC, X, Y, 270, iLockVerticalImage );
				break;

			case EnigmaWC::Direction::EAbove:
			case EnigmaWC::Direction::EBelow:
				DrawTile( aCC, X, Y, 0, iLockHorizontalImage );	
        break;

			default:
        Drawn = FALSE;
				break;
		}
	}
	else if (( aObject.iID == EnigmaWC::ID::EWaterLayer ))
	{
		switch( aObject.iSurface )
		{
		  case EnigmaWC::Direction::ENorth:
				DrawTile( aCC, X, Y, 0, iWaterLayerVerticalImage ); 
				break;

			case EnigmaWC::Direction::ESouth:
				DrawTile( aCC, X, Y, 180, iWaterLayerVerticalImage );
				break;

			case EnigmaWC::Direction::EEast:
				DrawTile( aCC, X, Y, 90, iWaterLayerVerticalImage );
				break;

			case EnigmaWC::Direction::EWest:
				DrawTile( aCC, X, Y, 270, iWaterLayerVerticalImage );
				break;
		
			case EnigmaWC::Direction::EBelow:
				DrawTile( aCC, X, Y, 0, iWaterLayerBelowImage );	
        break;
        
      case EnigmaWC::Direction::EAbove:
				DrawTile( aCC, X, Y, 0, iWaterLayerAboveImage );	
        break;

			default:
        Drawn = FALSE;
				break;
		}
	}
	else if ( aObject.iID == EnigmaWC::ID::EWater )
    DrawTile( aCC, X, Y, 0, iWaterImage );
	else if ( aObject.iID == EnigmaWC::ID::ELightBeam )
	{
		switch( aObject.iSurface )
		{
			case EnigmaWC::Direction::ENorth:
				DrawTile( aCC, X, Y, 0, iLightBeamHorizontalImage ); 
				break;

			case EnigmaWC::Direction::ESouth:
				DrawTile( aCC, X, Y, 180, iLightBeamHorizontalImage );
				break;

			case EnigmaWC::Direction::EEast:
				DrawTile( aCC, X, Y, 90, iLightBeamHorizontalImage );
				break;

			case EnigmaWC::Direction::EWest:
				DrawTile( aCC, X, Y, 270, iLightBeamHorizontalImage );
				break;

			case EnigmaWC::Direction::EAbove:
			case EnigmaWC::Direction::EBelow:
				DrawTile( aCC, X, Y, 0, iLightBeamVerticalImage );	
        break;

			default:
        Drawn = FALSE;
				break;
		}
	}
	else if ( aObject.iID == EnigmaWC::ID::EWoodWall )
	{
		switch( aObject.iSurface )
		{
			case EnigmaWC::Direction::ENorth:
				DrawTile( aCC, X, Y, 0, iWoodWallVerticalImage ); 
				break;

			case EnigmaWC::Direction::ESouth:
				DrawTile( aCC, X, Y, 180, iWoodWallVerticalImage );
				break;

			case EnigmaWC::Direction::EEast:
				DrawTile( aCC, X, Y, 90, iWoodWallVerticalImage );
				break;

			case EnigmaWC::Direction::EWest:
				DrawTile( aCC, X, Y, 270, iWoodWallVerticalImage );
				break;

			case EnigmaWC::Direction::EAbove:
				DrawTile( aCC, X, Y, 0, iWoodWallCeilingImage );
				break;
				
			case EnigmaWC::Direction::EBelow:
				DrawTile( aCC, X, Y, 0, iWoodWallFloorImage );
				break;

			default:
        Drawn = FALSE;
				break;
		}
	}
	else if ( aObject.iID == EnigmaWC::ID::EPadButton )
	{
		switch( aObject.iSurface )
		{
			case EnigmaWC::Direction::ENorth:
				DrawTile( aCC, X, Y, 0, iPadButtonVerticalImage ); 
				break;

			case EnigmaWC::Direction::ESouth:
				DrawTile( aCC, X, Y, 180, iPadButtonVerticalImage );
				break;

			case EnigmaWC::Direction::EEast:
				DrawTile( aCC, X, Y, 90, iPadButtonVerticalImage );
				break;

			case EnigmaWC::Direction::EWest:
				DrawTile( aCC, X, Y, 270, iPadButtonVerticalImage );
				break;

			case EnigmaWC::Direction::EAbove:
				DrawTile( aCC, X, Y, 0, iPadButtonCeilingImage );
				break;
				
			case EnigmaWC::Direction::EBelow:
				DrawTile( aCC, X, Y, 0, iPadButtonFloorImage );
				break;

			default:
        Drawn = FALSE;
				break;
		}
	}
	else if ( aObject.iID == EnigmaWC::ID::EEarthWall )
	{
		switch( aObject.iSurface )
		{
			case EnigmaWC::Direction::ENorth:
				DrawTile( aCC, X, Y, 0, iEarthWallVerticalImage ); 
				break;

			case EnigmaWC::Direction::ESouth:
				DrawTile( aCC, X, Y, 180, iEarthWallVerticalImage );
				break;

			case EnigmaWC::Direction::EEast:
				DrawTile( aCC, X, Y, 90, iEarthWallVerticalImage );
				break;

			case EnigmaWC::Direction::EWest:
				DrawTile( aCC, X, Y, 270, iEarthWallVerticalImage );
				break;

			case EnigmaWC::Direction::EAbove:
				DrawTile( aCC, X, Y, 0, iEarthWallCeilingImage );
				break;
				
			case EnigmaWC::Direction::EBelow:
				DrawTile( aCC, X, Y, 0, iEarthWallFloorImage );
				break;

			default:
        Drawn = FALSE;
				break;
		}
	}
	else if  (( aObject.iID == EnigmaWC::ID::EBlockWall )
	       || ( aObject.iID == EnigmaWC::ID::EStoneWall ))      
	{
		switch( aObject.iSurface )
		{
			case EnigmaWC::Direction::ENorth:
				DrawTile( aCC, X, Y, 0, iWallImage ); 
				break;

			case EnigmaWC::Direction::ESouth:
				DrawTile( aCC, X, Y, 180, iWallImage );
				break;

			case EnigmaWC::Direction::EEast:
				DrawTile( aCC, X, Y, 90, iWallImage );
				break;

			case EnigmaWC::Direction::EWest:
				DrawTile( aCC, X, Y, 270, iWallImage );
				break;

			case EnigmaWC::Direction::EAbove:
				DrawTile( aCC, X, Y, 0, iCeilingImage );
				break;
				
			case EnigmaWC::Direction::EBelow:
				DrawTile( aCC, X, Y, 0, iFloorImage );
				break;

			default:
        Drawn = FALSE;
				break;
		}
	}	
  else if ( aObject.iID == EnigmaWC::ID::EBlocker )
	{
		switch( aObject.iSurface )
		{
			case EnigmaWC::Direction::ENorth:
				DrawTile( aCC, X, Y, 0, iBlockerImage ); 
				break;

			case EnigmaWC::Direction::ESouth:
				DrawTile( aCC, X, Y, 180, iBlockerImage );
				break;

			case EnigmaWC::Direction::EEast:
				DrawTile( aCC, X, Y, 90, iBlockerImage );
				break;

			case EnigmaWC::Direction::EWest:
				DrawTile( aCC, X, Y, 270, iBlockerImage );
				break;

      case EnigmaWC::Direction::EAbove:
			case EnigmaWC::Direction::EBelow:
				DrawTile( aCC, X, Y, 0, iBlockerHorizontalImage );
				break;

			default:
        Drawn = FALSE;
				break;
		}
	}
	else if ( aObject.iID == EnigmaWC::ID::EFlipper )
  {
    // Draw an image for a Flipper.
    
		switch( aObject.iSurface )
    {
      case EnigmaWC::Direction::ENorth:
        DrawTile( aCC, X, Y, 0, iSurfacerImage ); 
        break;

      case EnigmaWC::Direction::ESouth:
        DrawTile( aCC, X, Y, 180, iSurfacerImage );
        break;

      case EnigmaWC::Direction::EEast:
        DrawTile( aCC, X, Y, 90, iSurfacerImage );
        break;

      case EnigmaWC::Direction::EWest:
        DrawTile( aCC, X, Y, 270, iSurfacerImage );
        break;

      case EnigmaWC::Direction::EAbove:
      case EnigmaWC::Direction::EBelow:
        DrawTile( aCC, X, Y, 0, iFlipperImage ); 
        break;
          
      default:
        Drawn = FALSE;
        break;
    }
  }
	else if (( aObject.iID == EnigmaWC::ID::ESurfacer )
	      || ( aObject.iID == EnigmaWC::ID::EEdger ))
	{
		// Draw images for Surfacers and Edgers.
				
		if  (( aObject.iSurface == EnigmaWC::Direction::EAbove )
		  || ( aObject.iSurface == EnigmaWC::Direction::EBelow ))
		{
      switch( aObject.iRotation )
      {
        case EnigmaWC::Direction::ENorth:
          DrawTile( aCC, X, Y, 0, iSurfacerImage ); 
          break;

        case EnigmaWC::Direction::ESouth:
          DrawTile( aCC, X, Y, 180, iSurfacerImage );
          break;

        case EnigmaWC::Direction::EEast:
          DrawTile( aCC, X, Y, 90, iSurfacerImage );
          break;

        case EnigmaWC::Direction::EWest:
          DrawTile( aCC, X, Y, 270, iSurfacerImage );
          break;
          
        default:
          Drawn = FALSE;
          break;
      }
    }
    else
		{
		  switch ( aObject.iSurface )
      {
        case EnigmaWC::Direction::ENorth:
          if ( aObject.iRotation == EnigmaWC::Direction::EWest )
            DrawTile( aCC, X, Y, 0, iCornerSurfacerImage );
          else if ( aObject.iRotation == EnigmaWC::Direction::EEast )
            DrawTile( aCC, X, Y, 90, iCornerSurfacerImage );
          else
            DrawTile( aCC, X, Y, 0, iSurfacerImage ); 
          break;

        case EnigmaWC::Direction::ESouth:
          if ( aObject.iRotation == EnigmaWC::Direction::EWest )
            DrawTile( aCC, X, Y, 270, iCornerSurfacerImage );
          else if ( aObject.iRotation == EnigmaWC::Direction::EEast )
            DrawTile( aCC, X, Y, 180, iCornerSurfacerImage );
          else
            DrawTile( aCC, X, Y, 180, iSurfacerImage );
          break;

        case EnigmaWC::Direction::EEast:
          if ( aObject.iRotation == EnigmaWC::Direction::ENorth )
            DrawTile( aCC, X, Y, 90, iCornerSurfacerImage );
          else if ( aObject.iRotation == EnigmaWC::Direction::ESouth )
            DrawTile( aCC, X, Y, 180, iCornerSurfacerImage );
          else
            DrawTile( aCC, X, Y, 90, iSurfacerImage );
          break;

        case EnigmaWC::Direction::EWest:
          if ( aObject.iRotation == EnigmaWC::Direction::ENorth )
            DrawTile( aCC, X, Y, 0, iCornerSurfacerImage );
          else if ( aObject.iRotation == EnigmaWC::Direction::ESouth )
            DrawTile( aCC, X, Y, 270, iCornerSurfacerImage );
          else
            DrawTile( aCC, X, Y, 270, iSurfacerImage );
          break;
          
        default:
          Drawn = FALSE;
          break;
      }
		}
	}
  else if ( aObject.iID == EnigmaWC::ID::EMover )
	{
		switch( aObject.iRotation )
		{
			case EnigmaWC::Direction::ENorth:
				DrawTile( aCC, X, Y, 0, iMoverImage ); 
				break;

			case EnigmaWC::Direction::ESouth:
				DrawTile( aCC, X, Y, 180, iMoverImage );
				break;

			case EnigmaWC::Direction::EEast:
				DrawTile( aCC, X, Y, 90, iMoverImage );
				break;

			case EnigmaWC::Direction::EWest:
				DrawTile( aCC, X, Y, 270, iMoverImage );
				break;

      case EnigmaWC::Direction::EAbove:
				DrawTile( aCC, X, Y, 0, iMoverAboveImage );
        break;
        
			case EnigmaWC::Direction::EBelow:
				DrawTile( aCC, X, Y, 0, iMoverBelowImage );
				break;

			default:
        Drawn = FALSE;
				break;
		}
  }
  else if ( aObject.iID == EnigmaWC::ID::ETurner )
	{
    // Draw a turner.
    
	  DrawTile( aCC, X, Y, 0, iTurnerImage ); 
  }
	else if ( aObject.iID == EnigmaWC::ID::ELadder )
	{
    if  (( aObject.iRotation == EnigmaWC::Direction::EAbove )
      || ( aObject.iRotation == EnigmaWC::Direction::EBelow ))	  
    {
      // Draw an Above/Below complete ladder.

      switch( aObject.iSurface )
		  {
  			case EnigmaWC::Direction::ENorth:
  				DrawTile( aCC, X, Y, 0, iLadderImage );
  				break;

  			case EnigmaWC::Direction::ESouth:
  				DrawTile( aCC, X, Y, 180, iLadderImage );
  				break;

  			case EnigmaWC::Direction::EEast:
  				DrawTile( aCC, X, Y, 90, iLadderImage );
  				break;

  			case EnigmaWC::Direction::EWest:
	  			DrawTile( aCC, X, Y, 270, iLadderImage );
  				break;
  				
        default:
          Drawn = FALSE;
          break;
  		  }
    }
    else if (( aObject.iRotation == EnigmaWC::Direction::EEast )
      || ( aObject.iRotation == EnigmaWC::Direction::EWest ))
    {
      // Draw an East/West complete ladder.
    
      switch( aObject.iSurface )
		  {  
        case EnigmaWC::Direction::EAbove:
          DrawTile( aCC, X, Y, 0, iLadderCeilingImage );
          break;

        case EnigmaWC::Direction::EBelow:
          DrawTile( aCC, X, Y, 0, iLadderFloorImage );
          break;

        case EnigmaWC::Direction::ENorth:
          DrawTile( aCC, X, Y, 0, iLadderHorizontalImage );
          break;

        case EnigmaWC::Direction::ESouth:
          DrawTile( aCC, X, Y, 180, iLadderHorizontalImage );
          break;

        default:
          Drawn = FALSE;
          break;
      }
    }
    else if (( aObject.iRotation == EnigmaWC::Direction::ENorth )
      || ( aObject.iRotation == EnigmaWC::Direction::ESouth ))
    {
      // Draw a North/South complete ladder.
    
      switch( aObject.iSurface )
		  {  
        case EnigmaWC::Direction::EAbove:
          DrawTile( aCC, X, Y, 90, iLadderFloorImage );
          break;

        case EnigmaWC::Direction::EBelow:
          DrawTile( aCC, X, Y, 90, iLadderCeilingImage );
          break;

        case EnigmaWC::Direction::EEast:
          DrawTile( aCC, X, Y, 90, iLadderHorizontalImage );
          break;

        case EnigmaWC::Direction::EWest:
          DrawTile( aCC, X, Y, 270, iLadderHorizontalImage );
          break;

        default:
          Drawn = FALSE;
          break;
      }
    }  
	}
	else if ( aObject.iID == EnigmaWC::ID::ELadderEnd )
	{
    if  (( aObject.iRotation == EnigmaWC::Direction::EAbove )
      || ( aObject.iRotation == EnigmaWC::Direction::EBelow ))
    {
      // Draw an Above/Below ladder top.

      switch( aObject.iSurface )
      {
          case EnigmaWC::Direction::ENorth:
          DrawTile( aCC, X, Y, 0, iLadderEndImage ); 
          break;

        case EnigmaWC::Direction::ESouth:
          DrawTile( aCC, X, Y, 180, iLadderEndImage );
          break;

        case EnigmaWC::Direction::EEast:
          DrawTile( aCC, X, Y, 90, iLadderEndImage );
          break;

        case EnigmaWC::Direction::EWest:
          DrawTile( aCC, X, Y, 270, iLadderEndImage );
          break;
          
        default:
          Drawn = FALSE;
          break;
      }
    }
    else if ( aObject.iRotation == EnigmaWC::Direction::EWest )
    {
      // Draw a West ladder top.
      
      switch( aObject.iSurface )
      {
        case EnigmaWC::Direction::ENorth:
          DrawTile( aCC, X, Y, 0, iLadderEndHorizontalImage ); 
          break;

        case EnigmaWC::Direction::ESouth:
          DrawTile( aCC, X, Y, 270, iLadderEndHorizontalImage );
          break;

        case EnigmaWC::Direction::EAbove:
	  			DrawTile( aCC, X, Y, 0, iLadderEndCeilingImage );
	  			break;
	  			
        case EnigmaWC::Direction::EBelow:
	  			DrawTile( aCC, X, Y, 0, iLadderEndFloorImage );
	  			break;
      
        default:
          Drawn = FALSE;
          break;
      }
    }
    else if ( aObject.iRotation == EnigmaWC::Direction::EEast )
    {
      // Draw an East ladder top.
      
      switch( aObject.iSurface )
      {
        case EnigmaWC::Direction::ENorth:
          DrawTile( aCC, X, Y, 90, iLadderEndHorizontalImage ); 
          break;

        case EnigmaWC::Direction::ESouth:
          DrawTile( aCC, X, Y, 180, iLadderEndHorizontalImage );
          break;

        case EnigmaWC::Direction::EAbove:
	  			DrawTile( aCC, X, Y, 180, iLadderEndCeilingImage );
	  			break;
	  			
        case EnigmaWC::Direction::EBelow:
	  			DrawTile( aCC, X, Y, 180, iLadderEndFloorImage );
	  			break;
      
        default:
          Drawn = FALSE;
          break;
      }
    }
    else if ( aObject.iRotation == EnigmaWC::Direction::ENorth )
    {
      // Draw a North ladder top.
      
      switch( aObject.iSurface )
      {
        case EnigmaWC::Direction::EWest:
          DrawTile( aCC, X, Y, 0, iLadderEndHorizontalImage ); 
          break;

        case EnigmaWC::Direction::EEast:
          DrawTile( aCC, X, Y, 90, iLadderEndHorizontalImage );
          break;

        case EnigmaWC::Direction::EAbove:
	  			DrawTile( aCC, X, Y, 90, iLadderEndFloorImage );
	  			break;
	  			
        case EnigmaWC::Direction::EBelow:
	  			DrawTile( aCC, X, Y, 90, iLadderEndCeilingImage );
	  			break;
      
        default:
          Drawn = FALSE;
          break;
      }
    }
    else if ( aObject.iRotation == EnigmaWC::Direction::ESouth )
    {
      // Draw a South ladder top.
      
      switch( aObject.iSurface )
      {
        case EnigmaWC::Direction::EWest:
          DrawTile( aCC, X, Y, 270, iLadderEndHorizontalImage ); 
          break;

        case EnigmaWC::Direction::EEast:
          DrawTile( aCC, X, Y, 180, iLadderEndHorizontalImage );
          break;

        case EnigmaWC::Direction::EAbove:
	  			DrawTile( aCC, X, Y, 270, iLadderEndFloorImage );
	  			break;
	  			
        case EnigmaWC::Direction::EBelow:
	  			DrawTile( aCC, X, Y, 270, iLadderEndCeilingImage );
	  			break;
      
        default:
          Drawn = FALSE;
          break;
      }
    }
    else
      Drawn = FALSE;
	}
	else if ( aObject.iID == EnigmaWC::ID::ETree )
	{
    if  (( aObject.iSurface == EnigmaWC::Direction::EAbove )
      || ( aObject.iSurface == EnigmaWC::Direction::EBelow ))	  
    {
      // Draw an tree growing on the Above/Below surface.

      switch( aObject.iRotation )
		  {
  			case EnigmaWC::Direction::ENorth:
  				DrawTile( aCC, X, Y, 0, iTreeImage );
  				break;

  			case EnigmaWC::Direction::ESouth:
  				DrawTile( aCC, X, Y, 180, iTreeImage );
  				break;

  			case EnigmaWC::Direction::EEast:
  				DrawTile( aCC, X, Y, 90, iTreeImage );
  				break;

  			case EnigmaWC::Direction::EWest:
	  			DrawTile( aCC, X, Y, 270, iTreeImage );
  				break;
  				
        default:
          Drawn = FALSE;
          break;
  		  }
    }
    else if (( aObject.iSurface == EnigmaWC::Direction::EEast )
      || ( aObject.iSurface == EnigmaWC::Direction::EWest ))
    {
      // Draw a tree growing on the East/West surface.
    
      switch( aObject.iRotation )
		  {  
        case EnigmaWC::Direction::EAbove:
          DrawTile( aCC, X, Y, 0, iTreeCeilingImage );
          break;

        case EnigmaWC::Direction::EBelow:
          DrawTile( aCC, X, Y, 0, iTreeFloorImage );
          break;

        case EnigmaWC::Direction::ENorth:
          DrawTile( aCC, X, Y, 0, iTreeHorizontalImage );
          break;

        case EnigmaWC::Direction::ESouth:
          DrawTile( aCC, X, Y, 180, iTreeHorizontalImage );
          break;

        default:
          Drawn = FALSE;
          break;
      }
    }
    else if (( aObject.iSurface == EnigmaWC::Direction::ENorth )
      || ( aObject.iSurface == EnigmaWC::Direction::ESouth ))
    {
      // Draw a tree growing on the North/South surface.
    
      switch( aObject.iRotation )
		  {  
        case EnigmaWC::Direction::EAbove:
          DrawTile( aCC, X, Y, 90, iTreeFloorImage );
          break;

        case EnigmaWC::Direction::EBelow:
          DrawTile( aCC, X, Y, 90, iTreeCeilingImage );
          break;

        case EnigmaWC::Direction::EEast:
          DrawTile( aCC, X, Y, 90, iTreeHorizontalImage );
          break;

        case EnigmaWC::Direction::EWest:
          DrawTile( aCC, X, Y, 270, iTreeHorizontalImage );
          break;

        default:
          Drawn = FALSE;
          break;
      }
    }  
	}
	else if ( aObject.iID == EnigmaWC::ID::ETreeTop )
	{
    if  (( aObject.iSurface == EnigmaWC::Direction::EAbove )
      || ( aObject.iSurface == EnigmaWC::Direction::EBelow ))
    {
      // Draw a tree top on the Above/Below surface.

      switch( aObject.iRotation )
      {
          case EnigmaWC::Direction::ENorth:
          DrawTile( aCC, X, Y, 0, iTreeTopImage ); 
          break;

        case EnigmaWC::Direction::ESouth:
          DrawTile( aCC, X, Y, 180, iTreeTopImage );
          break;

        case EnigmaWC::Direction::EEast:
          DrawTile( aCC, X, Y, 90, iTreeTopImage );
          break;

        case EnigmaWC::Direction::EWest:
          DrawTile( aCC, X, Y, 270, iTreeTopImage );
          break;
          
        default:
          Drawn = FALSE;
          break;
      }
    }
    else if ( aObject.iSurface == EnigmaWC::Direction::EWest )
    {
      // Draw a tree top on the West surface.
      
      switch( aObject.iRotation )
      {
        case EnigmaWC::Direction::ENorth:
          DrawTile( aCC, X, Y, 0, iTreeTopHorizontalImage ); 
          break;

        case EnigmaWC::Direction::ESouth:
          DrawTile( aCC, X, Y, 270, iTreeTopHorizontalImage );
          break;

        case EnigmaWC::Direction::EAbove:
	  			DrawTile( aCC, X, Y, 0, iTreeTopCeilingImage );
	  			break;
	  			
        case EnigmaWC::Direction::EBelow:
	  			DrawTile( aCC, X, Y, 0, iTreeTopFloorImage );
	  			break;
      
        default:
          Drawn = FALSE;
          break;
      }
    }
    else if ( aObject.iSurface == EnigmaWC::Direction::EEast )
    {
      // Draw a tree top on the East surface.
      
      switch( aObject.iRotation )
      {
        case EnigmaWC::Direction::ENorth:
          DrawTile( aCC, X, Y, 90, iTreeTopHorizontalImage ); 
          break;

        case EnigmaWC::Direction::ESouth:
          DrawTile( aCC, X, Y, 180, iTreeTopHorizontalImage );
          break;

        case EnigmaWC::Direction::EAbove:
	  			DrawTile( aCC, X, Y, 180, iTreeTopCeilingImage );
	  			break;
	  			
        case EnigmaWC::Direction::EBelow:
	  			DrawTile( aCC, X, Y, 180, iTreeTopFloorImage );
	  			break;
      
        default:
          Drawn = FALSE;
          break;
      }
    }
    else if ( aObject.iSurface == EnigmaWC::Direction::ENorth )
    {
      // Draw a tree top on the North surface.
      
      switch( aObject.iRotation )
      {
        case EnigmaWC::Direction::EWest:
          DrawTile( aCC, X, Y, 0, iTreeTopHorizontalImage ); 
          break;

        case EnigmaWC::Direction::EEast:
          DrawTile( aCC, X, Y, 90, iTreeTopHorizontalImage );
          break;

        case EnigmaWC::Direction::EAbove:
	  			DrawTile( aCC, X, Y, 90, iTreeTopFloorImage );
	  			break;
	  			
        case EnigmaWC::Direction::EBelow:
	  			DrawTile( aCC, X, Y, 90, iTreeTopCeilingImage );
	  			break;
      
        default:
          Drawn = FALSE;
          break;
      }
    }
    else if ( aObject.iSurface == EnigmaWC::Direction::ESouth )
    {
      // Draw a tree top on the South surface.
      
      switch( aObject.iRotation )
      {
        case EnigmaWC::Direction::EWest:
          DrawTile( aCC, X, Y, 270, iTreeTopHorizontalImage ); 
          break;

        case EnigmaWC::Direction::EEast:
          DrawTile( aCC, X, Y, 180, iTreeTopHorizontalImage );
          break;

        case EnigmaWC::Direction::EAbove:
	  			DrawTile( aCC, X, Y, 270, iTreeTopFloorImage );
	  			break;
	  			
        case EnigmaWC::Direction::EBelow:
	  			DrawTile( aCC, X, Y, 270, iTreeTopCeilingImage );
	  			break;
      
        default:
          Drawn = FALSE;
          break;
      }
    }
    else
      Drawn = FALSE;
	}
	else if ( aObject.iID == EnigmaWC::ID::ECatWalk )
	{
    if  (( aObject.iRotation == EnigmaWC::Direction::EAbove )
      || ( aObject.iRotation == EnigmaWC::Direction::EBelow ))	  
    {
      // Draw an Above/Below catwalk. 

      switch( aObject.iSurface )
		  {
  			case EnigmaWC::Direction::ENorth:
  				DrawTile( aCC, X, Y, 0, iCatWalkVerticalImage );
  				break;

  			case EnigmaWC::Direction::ESouth:
  				DrawTile( aCC, X, Y, 180, iCatWalkVerticalImage );
  				break;

  			case EnigmaWC::Direction::EEast:
  				DrawTile( aCC, X, Y, 90, iCatWalkVerticalImage );
  				break;

  			case EnigmaWC::Direction::EWest:
	  			DrawTile( aCC, X, Y, 270, iCatWalkVerticalImage );
  				break;
  				
        default:
          Drawn = FALSE;
          break;
  		  }
    }
    else if (( aObject.iRotation == EnigmaWC::Direction::EEast )
      || ( aObject.iRotation == EnigmaWC::Direction::EWest ))
    {
      // Draw an East/West catwalk.
    
      switch( aObject.iSurface )
		  {  
        case EnigmaWC::Direction::EAbove:
          DrawTile( aCC, X, Y, 0, iCatWalkCeilingImage );
          break;

        case EnigmaWC::Direction::EBelow:
          DrawTile( aCC, X, Y, 0, iCatWalkFloorImage );
          break;

        case EnigmaWC::Direction::ENorth:
          DrawTile( aCC, X, Y, 0, iCatWalkHorizontalImage );
          break;

        case EnigmaWC::Direction::ESouth:
          DrawTile( aCC, X, Y, 180, iCatWalkHorizontalImage );
          break;

        default:
          Drawn = FALSE;
          break;
      }
    }
    else if (( aObject.iRotation == EnigmaWC::Direction::ENorth )
      || ( aObject.iRotation == EnigmaWC::Direction::ESouth ))
    {
      // Draw a North/South catwalk.
    
      switch( aObject.iSurface )
		  {  
        case EnigmaWC::Direction::EAbove:
          DrawTile( aCC, X, Y, 90, iCatWalkFloorImage );
          break;

        case EnigmaWC::Direction::EBelow:
          DrawTile( aCC, X, Y, 90, iCatWalkCeilingImage );
          break;

        case EnigmaWC::Direction::EEast:
          DrawTile( aCC, X, Y, 90, iCatWalkHorizontalImage );
          break;

        case EnigmaWC::Direction::EWest:
          DrawTile( aCC, X, Y, 270, iCatWalkHorizontalImage );
          break;

        default:
          Drawn = FALSE;
          break;
      }
    }  
	}
	else if ( aObject.iID == EnigmaWC::ID::EFern )
	{
	  if ( aObject.iRotation == EnigmaWC::Direction::EBelow )
	  {
	    switch( aObject.iSurface )
		  {  
        case EnigmaWC::Direction::ENorth:
          DrawTile( aCC, X, Y, 0, iFernFloorImage );
          break;

        case EnigmaWC::Direction::EEast:
          DrawTile( aCC, X, Y, 90, iFernFloorImage );
          break;

        case EnigmaWC::Direction::ESouth:
          DrawTile( aCC, X, Y, 180, iFernFloorImage );
          break;

        case EnigmaWC::Direction::EWest:
          DrawTile( aCC, X, Y, 270, iFernFloorImage );
          break;

        default:
          Drawn = FALSE;
          break;
      }
    }
    else
      Drawn = FALSE;
	}
	else if ( aObject.iID == EnigmaWC::ID::EHandHold )
	{
    if (((( aObject.iSurface == EnigmaWC::Direction::EAbove )
	    || ( aObject.iSurface == EnigmaWC::Direction::EBelow ))  
	    && ( aObject.iRotation == EnigmaWC::Direction::ENorth ))
	    ||
      ((( aObject.iRotation == EnigmaWC::Direction::EAbove )
	    || ( aObject.iRotation == EnigmaWC::Direction::EBelow ))
	    && ( aObject.iSurface == EnigmaWC::Direction::ENorth )))
	  {
	    DrawTile( aCC, X, Y, 0, iHandHoldImage ); 
	  }
    else if (((( aObject.iSurface == EnigmaWC::Direction::EAbove )
	    || ( aObject.iSurface == EnigmaWC::Direction::EBelow ))  
	    && ( aObject.iRotation == EnigmaWC::Direction::EEast ))
	    ||
      ((( aObject.iRotation == EnigmaWC::Direction::EAbove )
	    || ( aObject.iRotation == EnigmaWC::Direction::EBelow ))
	    && ( aObject.iSurface == EnigmaWC::Direction::EEast )))
	  {
	    DrawTile( aCC, X, Y, 90, iHandHoldImage ); 
	  }
	  else if (((( aObject.iSurface == EnigmaWC::Direction::EAbove )
	    || ( aObject.iSurface == EnigmaWC::Direction::EBelow ))  
	    && ( aObject.iRotation == EnigmaWC::Direction::ESouth ))
	    ||
      ((( aObject.iRotation == EnigmaWC::Direction::EAbove )
	    || ( aObject.iRotation == EnigmaWC::Direction::EBelow ))
	    && ( aObject.iSurface == EnigmaWC::Direction::ESouth )))
	  {
	    DrawTile( aCC, X, Y, 180, iHandHoldImage ); 
	  }
	  else if (((( aObject.iSurface == EnigmaWC::Direction::EAbove )
	    || ( aObject.iSurface == EnigmaWC::Direction::EBelow ))  
	    && ( aObject.iRotation == EnigmaWC::Direction::EWest ))
	    ||
      ((( aObject.iRotation == EnigmaWC::Direction::EAbove )
	    || ( aObject.iRotation == EnigmaWC::Direction::EBelow ))
	    && ( aObject.iSurface == EnigmaWC::Direction::EWest )))
	  {
	    DrawTile( aCC, X, Y, 270, iHandHoldImage ); 
	  }
	  else if ((( aObject.iSurface == EnigmaWC::Direction::ENorth )
	    && ( aObject.iRotation == EnigmaWC::Direction::EWest ))
	    || (( aObject.iSurface == EnigmaWC::Direction::EWest )
	    && ( aObject.iRotation == EnigmaWC::Direction::ENorth )))
	  {
	    DrawTile( aCC, X, Y, 0, iHandHoldHorizontalImage );
	  }
    else if ((( aObject.iSurface == EnigmaWC::Direction::EEast )
	    && ( aObject.iRotation == EnigmaWC::Direction::ENorth ))
	    || (( aObject.iSurface == EnigmaWC::Direction::ENorth )
	    && ( aObject.iRotation == EnigmaWC::Direction::EEast )))
	  {
	    DrawTile( aCC, X, Y, 90, iHandHoldHorizontalImage );
	  }
    else if ((( aObject.iSurface == EnigmaWC::Direction::ESouth )
	    && ( aObject.iRotation == EnigmaWC::Direction::EEast ))
	    || (( aObject.iSurface == EnigmaWC::Direction::EEast )
	    && ( aObject.iRotation == EnigmaWC::Direction::ESouth )))
	  {
	    DrawTile( aCC, X, Y, 180, iHandHoldHorizontalImage );
	  }
    else if ((( aObject.iSurface == EnigmaWC::Direction::EWest )
	    && ( aObject.iRotation == EnigmaWC::Direction::ESouth ))
	    || (( aObject.iSurface == EnigmaWC::Direction::ESouth )
	    && ( aObject.iRotation == EnigmaWC::Direction::EWest )))
	  {
	    DrawTile( aCC, X, Y, 270, iHandHoldHorizontalImage );
	  }	  
    else
      Drawn = FALSE;
	}
  else if ( aObject.iID == EnigmaWC::ID::EOutdoor )      
  {
    switch( aObject.iSurface )
    {
      case EnigmaWC::Direction::ENorth:
        DrawTile( aCC, X, Y, 0, iOutdoorVerticalImage ); 
        break;

      case EnigmaWC::Direction::ESouth:
        DrawTile( aCC, X, Y, 180, iOutdoorVerticalImage );
        break;

      case EnigmaWC::Direction::EEast:
        DrawTile( aCC, X, Y, 90, iOutdoorVerticalImage );
        break;

      case EnigmaWC::Direction::EWest:
        DrawTile( aCC, X, Y, 270, iOutdoorVerticalImage );
        break;

      case EnigmaWC::Direction::EAbove:
        DrawTile( aCC, X, Y, 0, iOutdoorCeilingImage );
        break;
				
      case EnigmaWC::Direction::EBelow:
        DrawTile( aCC, X, Y, 0, iOutdoorFloorImage );
        break;

      case EnigmaWC::Direction::ECenter:
        DrawTile( aCC, X, Y, 0, iOutdoorImage );
        break;

      default:
        Drawn = FALSE;
        break;
    }
  }
  else if ( aObject.iID == EnigmaWC::ID::EIndoor )      
  {
    switch( aObject.iSurface )
    {
      case EnigmaWC::Direction::ENorth:
        DrawTile( aCC, X, Y, 0, iIndoorVerticalImage ); 
        break;

      case EnigmaWC::Direction::ESouth:
        DrawTile( aCC, X, Y, 180, iIndoorVerticalImage );
        break;

      case EnigmaWC::Direction::EEast:
        DrawTile( aCC, X, Y, 90, iIndoorVerticalImage );
        break;

      case EnigmaWC::Direction::EWest:
        DrawTile( aCC, X, Y, 270, iIndoorVerticalImage );
        break;

      case EnigmaWC::Direction::EAbove:
        DrawTile( aCC, X, Y, 0, iIndoorCeilingImage );
        break;
				
      case EnigmaWC::Direction::EBelow:
        DrawTile( aCC, X, Y, 0, iIndoorFloorImage );
        break;

      case EnigmaWC::Direction::ECenter:
        DrawTile( aCC, X, Y, 0, iIndoorImage );
        break;

      default:
        Drawn = FALSE;
        break;
    }
  }
  else if ( aObject.iID == EnigmaWC::ID::EPerson )
	{
		if ( aObject.iActive )
			DrawTile( aCC, X, Y, 0, iPlayerActiveImage );
		else
			DrawTile( aCC, X, Y, 0, iPlayerIdleImage );
	}
	else
	  Drawn = false;
	
	return Drawn;
}

