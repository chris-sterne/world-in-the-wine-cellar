// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Tiles class implementation.  The Tiles class draws object
// image tiles in a game world level viewer.
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

#include "ImageTiles.h"

//--------------------
// Local declarations.
//--------------------

#define TILESIZE 60         // Pixel size of image tile.
               
//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::ImageTiles::ImageTiles()
{ 
	// Load image tiles from files.
	
	m_cursor =
		Cairo::ImageSurface::create_from_png("./images/Cursor.png");

	m_marker =
		Cairo::ImageSurface::create_from_png("./images/Marker.png");

	m_generic =
		Cairo::ImageSurface::create_from_png("./images/Generic.png");

	m_Wall = 
		Cairo::ImageSurface::create_from_png("./images/Wall.png");

	m_ceiling =
		Cairo::ImageSurface::create_from_png("./images/Ceiling.png");

	m_Floor =
		Cairo::ImageSurface::create_from_png("./images/Floor.png");

	m_ladder =
		Cairo::ImageSurface::create_from_png("./images/Ladder.png");

	m_ladderend =
		Cairo::ImageSurface::create_from_png("./images/LadderEnd.png");

	m_ladderEnd_horizontal =
		Cairo::ImageSurface::create_from_png("./images/LadderEndHorizontal.png");

	m_ladderend_ceiling =
		Cairo::ImageSurface::create_from_png("./images/LadderEndCeiling.png");

	m_ladderend_floor =
		Cairo::ImageSurface::create_from_png("./images/LadderEndFloor.png");

	m_ladder_horizontal =
		Cairo::ImageSurface::create_from_png("./images/LadderHorizontal.png");

	m_ladder_ceiling =
		Cairo::ImageSurface::create_from_png("./images/LadderCeiling.png");

	m_ladder_floor =
		Cairo::ImageSurface::create_from_png("./images/LadderFloor.png");

	m_player_active =
		Cairo::ImageSurface::create_from_png("./images/PlayerActive.png");

	m_player_idle =
		Cairo::ImageSurface::create_from_png("./images/PlayerIdle.png");

	m_blocker =
		Cairo::ImageSurface::create_from_png("./images/Blocker.png");

	m_blocker_horizontal =
		Cairo::ImageSurface::create_from_png("./images/BlockerHorizontal.png");

	m_mover =
		Cairo::ImageSurface::create_from_png("./images/Mover.png");

	m_Mover_Below =
		Cairo::ImageSurface::create_from_png("./images/MoverBelow.png");

	m_mover_above =
		Cairo::ImageSurface::create_from_png("./images/MoverAbove.png");

	m_turner =
		Cairo::ImageSurface::create_from_png("./images/Turner.png");

	m_item =
		Cairo::ImageSurface::create_from_png("./images/Item.png");

	m_flipper =
		Cairo::ImageSurface::create_from_png("./images/Flipper.png");

	m_surfacer =
		Cairo::ImageSurface::create_from_png("./images/Surfacer.png");

	m_surfacer_corner =
		Cairo::ImageSurface::create_from_png("./images/SurfacerCorner.png");

	m_stairs =
		Cairo::ImageSurface::create_from_png("./images/Stairs.png");

	m_stairstop =
		Cairo::ImageSurface::create_from_png("./images/StairsTop.png");

	m_handhold =
		Cairo::ImageSurface::create_from_png("./images/HandHold.png");

	m_handhold_horizontal =
		Cairo::ImageSurface::create_from_png("./images/HandHoldHorizontal.png");

	m_outdoor =
		Cairo::ImageSurface::create_from_png("./images/Outdoor.png");

	m_outdoor_vertical =
		Cairo::ImageSurface::create_from_png("./images/OutdoorVertical.png");

	m_outdoor_ceiling =
		Cairo::ImageSurface::create_from_png("./images/OutdoorCeiling.png");

	m_outdoor_floor =
		Cairo::ImageSurface::create_from_png("./images/OutdoorFloor.png");

	m_indoor =
		Cairo::ImageSurface::create_from_png("./images/Indoor.png");

	m_indoor_vertical =
		Cairo::ImageSurface::create_from_png("./images/IndoorVertical.png");

	m_indoor_ceiling =
		Cairo::ImageSurface::create_from_png("./images/IndoorCeiling.png" ;

	m_indoor_floor =
		Cairo::ImageSurface::create_from_png("./images/IndoorFloor.png");

	m_archway_vertical =
		Cairo::ImageSurface::create_from_png("./images/ArchWayVertical.png");

	m_archway_ceiling =
		Cairo::ImageSurface::create_from_png("./images/ArchWayCeiling.png");

	m_archway_floor =
		Cairo::ImageSurface::create_from_png("./images/ArchWayFloor.png");

	m_wooddoor_vertical =
		Cairo::ImageSurface::create_from_png("./images/WoodDoorVertical.png");

	m_wooddoor_ceiling =
		Cairo::ImageSurface::create_from_png("./images/WoodDoorCeiling.png");

	m_wooddoor_floor =
		Cairo::ImageSurface::create_from_png("./images/WoodDoorFloor.png");

	m_woodwall_vertical =
		Cairo::ImageSurface::create_from_png("./images/WoodWallVertical.png");

	m_woodwall_ceiling =
		Cairo::ImageSurface::create_from_png("./images/WoodWallCeiling.png");

	m_woodwall_floor =
		Cairo::ImageSurface::create_from_png("./images/WoodWallFloor.png");

	m_pullring_horizontal =
		Cairo::ImageSurface::create_from_png("./images/PullRingHorizontal.png");

	m_pullring_vertical =
		Cairo::ImageSurface::create_from_png("./images/PullRingVertical.png");

	m_lock_horizontal =
		Cairo::ImageSurface::create_from_png("./images/LockHorizontal.png");

	m_lock_vertical =
		Cairo::ImageSurface::create_from_png("./images/LockVertical.png");

	m_waterlayer_vertical =
		Cairo::ImageSurface::create_from_png("./images/WaterLayerVertical.png");

	m_waterlayer_below =
		Cairo::ImageSurface::create_from_png("./images/WaterLayerBelow.png");

	m_waterlayer_above =
		Cairo::ImageSurface::create_from_png("./images/WaterLayerAbove.png");

	m_lightbeam_horizontal =
		Cairo::ImageSurface::create_from_png("./images/LightBeamHorizontal.png");

	m_lightbeam_vertical =
		Cairo::ImageSurface::create_from_png("./images/LightBeamVertical.png");

	m_Tree =
		Cairo::ImageSurface::create_from_png("./images/Tree.png");

	m_tree_vorizontal =
		Cairo::ImageSurface::create_from_png("./images/TreeHorizontal.png");

	m_tree_ceiling =
		Cairo::ImageSurface::create_from_png("./images/TreeCeiling.png");

	m_tree_floor =
		Cairo::ImageSurface::create_from_png("./images/TreeFloor.png");

	m_treetop =
		Cairo::ImageSurface::create_from_png("./images/TreeTop.png");

	m_treetop_gorizontal =
		Cairo::ImageSurface::create_from_png("./images/TreeTopHorizontal.png");

	m_treetop_ceiling =
		Cairo::ImageSurface::create_from_png("./images/TreeTopCeiling.png");

	m_treetop_floor =
		Cairo::ImageSurface::create_from_png("./images/TreeTopFloor.png");

	m_catwalk_ceiling =
		Cairo::ImageSurface::create_from_png("./images/CatWalkCeiling.png");

	m_catwalk_floor =
		Cairo::ImageSurface::create_from_png("./images/CatWalkFloor.png");

	m_catwalk_horizontal =
		Cairo::ImageSurface::create_from_png("./images/CatWalkHorizontal.png");

	m_catwalk_vertical =
		Cairo::ImageSurface::create_from_png("./images/CatWalkVertical.png");

	m_teleporter_departure =
		Cairo::ImageSurface::create_from_png("./images/TeleporterDeparture.png");

	m_teleporter_arrival =
		Cairo::ImageSurface::create_from_png("./images/TeleporterArrival.png"); 

	m_fernfloor =
		Cairo::ImageSurface::create_from_png("./images/FernFloor.png");  

	m_water =
		Cairo::ImageSurface::create_from_png("./images/Water.png");

	m_earthwall_vertical =
		Cairo::ImageSurface::create_from_png("./images/EarthWallVertical.png");

	m_earthwall_ceiling =
		Cairo::ImageSurface::create_from_png("./images/EarthWallCeiling.png");

	m_earthwall_floor =
		Cairo::ImageSurface::create_from_png("./images/EarthWallFloor.png");

	m_padbutton_ceiling =
		Cairo::ImageSurface::create_from_png("./images/PadButtonCeiling.png");

	m_PadButton_floor =
		Cairo::ImageSurface::create_from_png("./images/PadButtonFloor.png");

	m_padbutton_vertical =
		Cairo::ImageSurface::create_from_png("./images/PadButtonVertical.png");
}

//-----------------------------------------------
// This method returns the image tile pixel size.
//-----------------------------------------------
// RETURN: Tile size.                            
//-----------------------------------------------

int Enigma::Tiles::get_tile_size() const
{
	return TILESIZE;
}

//---------------------------------------------------
// This private function draws an image tile.
//---------------------------------------------------
// context: Cairo drawing context.
// x:       X screen pixel coordinate of tile.
// y:       Y screen pixel coordinate of tile.
// r:       Rotation amount (0, 90, 180, 270 degrees).
// image:   Image surface to draw.
//----------------------------------------------------

void draw_tile(const Cairo::RefPtr<Cairo::Context>& context,
              double x,
              double y,
              double r,
              Cairo::RefPtr<Cairo::ImageSurface> image)
{
	if (r == 90)
	{
		x += TILESIZE;
	}
	else if (r == 180)
	{
		x += TILESIZE;
		y += TILESIZE;
	}	
	else if (r == 270)
	{
		y += TILESIZE;
	}
	else
	{
		r = 0;
	}

	context->save();
	context->translate(x, y);
	context->rotate_degrees(r);

	context->set_source(image, 0, 0);
	context->paint();
	context->restore();
}

//--------------------------------------------------------
// This method draws a cursor tile.  The lower-left corner
// of the view is the origin (Row = 0, Column = 0).         
//--------------------------------------------------------
// context:    Cairo context for drawing.
// allocation: Pixel boundary of view.
// column:     Cursor column number.
// row:        Cursor row number.
//--------------------------------------------------------

void Enigma::Tiles::DrawCursor(const Cairo::RefPtr<Cairo::Context>& context,
                               Gtk::Allocation allocation,
                               guint16 column,
                               guint16 row)
{
	double x = column * TILESIZE;
	double y = allocation.get_height() - TILESIZE - (row * TILESIZE);

	context->set_source(m_cursor, x, y);
	context->paint();
}

//--------------------------------------------------------
// This method draws a marker tile.  The lower-left corner
// of the view is the origin (Row = 0, Column = 0).
//--------------------------------------------------------
// context:    Cairo context for drawing.
// allocation: Pixel boundary of view.
// column:     Cursor column number.
// row:        Cursor row number.
//--------------------------------------------------------

void Enigma::Tiles::draw_marker(const Cairo::RefPtr<Cairo::Context>& context,
                                Gtk::Allocation allocation,
                                guint16 column,
                                guint16 row)
{
	double x = column * TILESIZE;
	double y = allocation.get_height() - TILESIZE - (row * TILESIZE);

	context->set_source(m_marker, x, y);
	context->paint();
}

//--------------------------------------------------------------
// This method draws a tile for a generic object.  This would be   
// used for objects that have no unique image, but require some
// image to indicate their presence.
//--------------------------------------------------------------
// context:    Cairo context for drawing.
// allocation: Pixel boundary of view.
// column:     Cursor column number.
// row:        Cursor row number.
//--------------------------------------------------------------

void Enigma::Tiles::draw_generic(const Cairo::RefPtr<Cairo::Context>& context,
                                 Gtk::Allocation allocation,
                                 guint16 column,
                                 guint16 row)
{
	double X = column * TILESIZE;
	double Y = allocation.get_height() - TILESIZE - (row * TILESIZE);

	context->set_source(m_Generic, x, y);
	context->paint();
}

//*-----------------------------------------------------------------*
//* This method draws a tile marking a teleporter arrival location. *
//*-----------------------------------------------------------------*
//* context:         Cairo context for drawing.                         *
//* aAllocation: Pixel boundary of view.                            *
//* aColumn:     Cursor column number.                              *
//* aRow:        Cursor row number.                                 *
//*-----------------------------------------------------------------*

void Enigma::Tiles::draw_arrival(const Cairo::RefPtr<Cairo::Context>& context,
                                 Gtk::Allocation allocation,
                                 guint16 column,
                                 guint16 row)
{
	double X = column * TILESIZE;
	double Y = allocation.get_height() - TILESIZE - (row * TILESIZE);

	context->set_source(m_teleporter_arrival, x, y);
	context->paint();
}

//------------------------------------------------------------
// This method draws a map object tile.  The lower-left corner
// of the view is the origin (Row = 0, Column = 0).
//------------------------------------------------------------
// context:    Cairo context for drawing.
// allocation: Pixel boundary of view.
// column:     Visible column of map level.
// row:        Visible row of map level.
// object:     Map object to draw.
// RETURN:     TRUE if an image was drawn.
//------------------------------------------------------------

bool Enigma::Tiles::draw_object(const Cairo::RefPtr<Cairo::Context>& context,
                                Gtk::Allocation allocation,
                                guint16 column,
                                guint16 row,
                                Enigma::Object& object)
{
	bool drawn = true;
	double x   = column * TILESIZE;
	double y   = allocation.get_height() - TILESIZE - (row * TILESIZE);

	if (object.m_type == Enigma::Object::Type::ITEM)
	{
		// Draw a general image for all items.

		draw_tile(context, x, y, 0, m_item);
	}
	else if (object.m_type == Enigma::Object::Type::TELEPORTER)
	{
		// Draw a general image for all teleporters.

		draw_tile(context, x, y, 0, m_teleporter_departure);
	}
	else if (object.m_id == Enigma::Object::ID::STAIRS)
	{
		// Draw stairs images.
		
		switch (object.m_rotation)
		{
			case Enigma::Object::Direction::NORTH:
				draw_tile(context, x, y, 0, m_stairs); 
				break;

			case Enigma::Object::Direction::SOUTH:
				draw_tile(context, x, y, 180, m_stairs);
				break;

			case Enigma::Object::Direction::EAST:
				draw_tile(context, x, y, 90, m_stairs);
				break;

			case Enigma::Object::Direction::WEST:
				draw_tile(context, x, y, 270, m_stairs);
				break;

			default:
				drawn = false;
				break;
			}
		}
		else if (object.m_id == Enigma::Object::ID::STAIRSTOP)
		{
			switch (object.m_rotation)
			{
				// Draw StairsTop images.
				
				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 0, m_stairstop); 
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 180, m_stairstop);
					break;

				case Enigma::Object::Direction::EAST:
					draw_tile(context, x, y, 90, m_stairstop);
					break;

				case Enigma::Object::Direction::WEST:
					draw_tile(context, x, y, 270, m_stairstop);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else if (object.m_id == Enigma::Object::ID::ARCHWAY)
		{
			switch (object.m_surface)
			{
				// Draw ArchWay images.
				
				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 0, m_archway_vertical); 
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 180, m_archway_vertical);
					break;

				case Enigma::Object::Direction::EAST:
					draw_tile(context, x, y, 90, m_archway_vertical);
					break;

				case Enigma::Object::Direction::WEST:
					draw_tile(context, x, y, 270, m_archway_vertical);
					break;

				case Enigma::Object::Direction::ABOVE:
					draw_tile(context, x, y, 270, m_archway_ceiling);
					break;

				case Enigma::Object::Direction::BELOW:
					draw_tile(context, x, y, 270, m_archway_floor);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else if (object.m_id == Enigma::Object::ID::WOODDOOR)
		{
			// Draw WoodDoor images.
			
			switch (object.m_surface)
			{
				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 0, m_wooddoor_vertical); 
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 180, m_wooddoor_vertical);
					break;

				case Enigma::Object::Direction::EAST:
					draw_tile(context, x, y, 90, m_wooddoor_vertical);
					break;

				case Enigma::Object::Direction::WEST:
					draw_tile(context, x, y, 270, m_wooddoor_vertical);
					break;

				case Enigma::Object::Direction::ABOVE:
					draw_tile(context, x, y, 270, m_wooddoor_ceiling);
					break;

				case Enigma::Object::Direction::BELOW:
					draw_tile(context, x, y, 270, m_wooddoor_floor);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else if (object.m_id == Enigma::Object::ID::PULLRING)
		{
			// Draw PullRing images.
			
			switch (object.m_surface)
			{
				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 0, m_pullring_vertical); 
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 180, m_pullring_vertical);
					break;

				case Enigma::Object::Direction::EAST:
					draw_tile(context, x, y, 90, m_pullring_vertical);
					break;

				case Enigma::Object::Direction::WEST:
					draw_tile(context, x, y, 270, m_pullring_vertical);
					break;

				case Enigma::Object::Direction::ABOVE:
				case Enigma::Object::Direction::BELOW:
					draw_tile(context, x, y, 0, m_pullring_horizontal);	
					break;

				default:
					drawn = false;
					break;
			}
		}
		else if ((object.m_id == Enigma::Object::ID::CUBELOCK)
		      || (object.m_id == Enigma::Object::ID::SPHERELOCK)
		      || (object.m_id == Enigma::Object::ID::WATERLOCK)
		      || (object.m_id == Enigma::Object::ID::APPLELOCK)
		      || (object.m_id == Enigma::Object::ID::ORANGELOCK)
		      || (object.m_id == Enigma::Object::ID::WINELOCK))
		{
			// Draw generic lock images.
			
			switch (object.m_surface)
			{
				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 0, m_lock_vertical); 
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 180, m_lock_vertical);
					break;

				case Enigma::Object::Direction::EAST:
					draw_tile(context, x, y, 90, m_lock_vertical);
					break;

				case Enigma::Object::Direction::WEST:
					draw_tile(context, x, y, 270, m_lock_vertical);
					break;

				case Enigma::Object::Direction::ABOVE:
				case Enigma::Object::Direction::BELOW:
					draw_tile(context, x, y, 0, m_lock_horizontal);	
					break;

				default:
					drawn = false;
					break;
			}
		}
		else if ((object.m_id == Enigma::Object::ID::EWaterLayer))
		{
			switch(object.m_surface)
			{
				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 0, m_waterlayer_vertical); 
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 180, m_waterlayer_vertical);
					break;

				case Enigma::Object::Direction::EAST:
					draw_tile(context, x, y, 90, m_waterlayer_vertical);
					break;

				case Enigma::Object::Direction::WEST:
					draw_tile(context, x, y, 270, m_waterlayer_vertical);
					break;

				case Enigma::Object::Direction::BELOW:
					draw_tile(context, x, y, 0, m_waterlayer_below);	
					break;

				case Enigma::Object::Direction::ABOVE:
					draw_tile(context, x, y, 0, m_waterlayer_above);	
					break;

				default:
					drawn = false;
					break;
			}
		}
else if ( object.m_id == Enigma::Object::ID::EWater )
draw_tile(context, x, y, 0, iWaterImage );
else if ( object.m_id == Enigma::Object::ID::ELightBeam )
{
switch( aObject.iSurface )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iLightBeamHorizontalImage ); 
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iLightBeamHorizontalImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iLightBeamHorizontalImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iLightBeamHorizontalImage );
break;

case Enigma::Object::Direction::ABOVE:
case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 0, iLightBeamVerticalImage );	
break;

default:
Drawn = FALSE;
break;
}
}
else if ( object.m_id == Enigma::Object::ID::EWoodWall )
{
switch( aObject.iSurface )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iWoodWallVerticalImage ); 
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iWoodWallVerticalImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iWoodWallVerticalImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iWoodWallVerticalImage );
break;

case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 0, iWoodWallCeilingImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 0, iWoodWallFloorImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if ( object.m_id == Enigma::Object::ID::EPadButton )
{
switch( aObject.iSurface )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iPadButtonVerticalImage ); 
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iPadButtonVerticalImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iPadButtonVerticalImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iPadButtonVerticalImage );
break;

case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 0, iPadButtonCeilingImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 0, iPadButtonFloorImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if ( object.m_id == Enigma::Object::ID::EEarthWall )
{
switch( aObject.iSurface )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iEarthWallVerticalImage ); 
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iEarthWallVerticalImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iEarthWallVerticalImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iEarthWallVerticalImage );
break;

case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 0, iEarthWallCeilingImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 0, iEarthWallFloorImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if  (( object.m_id == Enigma::Object::ID::EBlockWall )
|| ( object.m_id == Enigma::Object::ID::EStoneWall ))      
{
switch( aObject.iSurface )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iWallImage ); 
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iWallImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iWallImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iWallImage );
break;

case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 0, iCeilingImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 0, iFloorImage );
break;

default:
Drawn = FALSE;
break;
}
}	
else if ( object.m_id == Enigma::Object::ID::EBlocker )
{
switch( aObject.iSurface )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iBlockerImage ); 
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iBlockerImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iBlockerImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iBlockerImage );
break;

case Enigma::Object::Direction::ABOVE:
case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 0, iBlockerHorizontalImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if ( object.m_id == Enigma::Object::ID::EFlipper )
{
// Draw an image for a Flipper.

switch( aObject.iSurface )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iSurfacerImage ); 
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iSurfacerImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iSurfacerImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iSurfacerImage );
break;

case Enigma::Object::Direction::ABOVE:
case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 0, iFlipperImage ); 
break;

default:
Drawn = FALSE;
break;
}
}
else if (( object.m_id == Enigma::Object::ID::ESurfacer )
|| ( object.m_id == Enigma::Object::ID::EEdger ))
{
// Draw images for Surfacers and Edgers.

if  (( aObject.iSurface == Enigma::Object::Direction::ABOVE )
|| ( aObject.iSurface == Enigma::Object::Direction::BELOW ))
{
switch( object.m_rotation )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iSurfacerImage ); 
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iSurfacerImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iSurfacerImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iSurfacerImage );
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
case Enigma::Object::Direction::NORTH:
if ( object.m_rotation == Enigma::Object::Direction::WEST )
draw_tile(context, x, y, 0, iCornerSurfacerImage );
else if ( object.m_rotation == Enigma::Object::Direction::EAST )
draw_tile(context, x, y, 90, iCornerSurfacerImage );
else
draw_tile(context, x, y, 0, iSurfacerImage ); 
break;

case Enigma::Object::Direction::SOUTH:
if ( object.m_rotation == Enigma::Object::Direction::WEST )
draw_tile(context, x, y, 270, iCornerSurfacerImage );
else if ( object.m_rotation == Enigma::Object::Direction::EAST )
draw_tile(context, x, y, 180, iCornerSurfacerImage );
else
draw_tile(context, x, y, 180, iSurfacerImage );
break;

case Enigma::Object::Direction::EAST:
if ( object.m_rotation == Enigma::Object::Direction::NORTH )
draw_tile(context, x, y, 90, iCornerSurfacerImage );
else if ( object.m_rotation == Enigma::Object::Direction::SOUTH )
draw_tile(context, x, y, 180, iCornerSurfacerImage );
else
draw_tile(context, x, y, 90, iSurfacerImage );
break;

case Enigma::Object::Direction::WEST:
if ( object.m_rotation == Enigma::Object::Direction::NORTH )
draw_tile(context, x, y, 0, iCornerSurfacerImage );
else if ( object.m_rotation == Enigma::Object::Direction::SOUTH )
draw_tile(context, x, y, 270, iCornerSurfacerImage );
else
draw_tile(context, x, y, 270, iSurfacerImage );
break;

default:
Drawn = FALSE;
break;
}
}
}
else if ( object.m_id == Enigma::Object::ID::EMover )
{
switch( object.m_rotation )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iMoverImage ); 
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iMoverImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iMoverImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iMoverImage );
break;

case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 0, iMoverAboveImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 0, iMoverBelowImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if ( object.m_id == Enigma::Object::ID::ETurner )
{
// Draw a turner.

draw_tile(context, x, y, 0, iTurnerImage ); 
}
else if ( object.m_id == Enigma::Object::ID::ELadder )
{
if  (( object.m_rotation == Enigma::Object::Direction::ABOVE )
|| ( object.m_rotation == Enigma::Object::Direction::BELOW ))	  
{
// Draw an Above/Below complete ladder.

switch( aObject.iSurface )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iLadderImage );
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iLadderImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iLadderImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iLadderImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if (( object.m_rotation == Enigma::Object::Direction::EAST )
|| ( object.m_rotation == Enigma::Object::Direction::WEST ))
{
// Draw an East/West complete ladder.

switch( aObject.iSurface )
{  
case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 0, iLadderCeilingImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 0, iLadderFloorImage );
break;

case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iLadderHorizontalImage );
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iLadderHorizontalImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if (( object.m_rotation == Enigma::Object::Direction::NORTH )
|| ( object.m_rotation == Enigma::Object::Direction::SOUTH ))
{
// Draw a North/South complete ladder.

switch( aObject.iSurface )
{  
case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 90, iLadderFloorImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 90, iLadderCeilingImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iLadderHorizontalImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iLadderHorizontalImage );
break;

default:
Drawn = FALSE;
break;
}
}  
}
else if ( object.m_id == Enigma::Object::ID::ELadderEnd )
{
if  (( object.m_rotation == Enigma::Object::Direction::ABOVE )
|| ( object.m_rotation == Enigma::Object::Direction::BELOW ))
{
// Draw an Above/Below ladder top.

switch( aObject.iSurface )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iLadderEndImage ); 
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iLadderEndImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iLadderEndImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iLadderEndImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if ( object.m_rotation == Enigma::Object::Direction::WEST )
{
// Draw a West ladder top.

switch( aObject.iSurface )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iLadderEndHorizontalImage ); 
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 270, iLadderEndHorizontalImage );
break;

case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 0, iLadderEndCeilingImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 0, iLadderEndFloorImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if ( object.m_rotation == Enigma::Object::Direction::EAST )
{
// Draw an East ladder top.

switch( aObject.iSurface )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 90, iLadderEndHorizontalImage ); 
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iLadderEndHorizontalImage );
break;

case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 180, iLadderEndCeilingImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 180, iLadderEndFloorImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if ( object.m_rotation == Enigma::Object::Direction::NORTH )
{
// Draw a North ladder top.

switch( aObject.iSurface )
{
case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 0, iLadderEndHorizontalImage ); 
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iLadderEndHorizontalImage );
break;

case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 90, iLadderEndFloorImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 90, iLadderEndCeilingImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if ( object.m_rotation == Enigma::Object::Direction::SOUTH )
{
// Draw a South ladder top.

switch( aObject.iSurface )
{
case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iLadderEndHorizontalImage ); 
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 180, iLadderEndHorizontalImage );
break;

case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 270, iLadderEndFloorImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 270, iLadderEndCeilingImage );
break;

default:
Drawn = FALSE;
break;
}
}
else
Drawn = FALSE;
}
else if ( object.m_id == Enigma::Object::ID::ETree )
{
if  (( aObject.iSurface == Enigma::Object::Direction::ABOVE )
|| ( aObject.iSurface == Enigma::Object::Direction::BELOW ))	  
{
// Draw an tree growing on the Above/Below surface.

switch( object.m_rotation )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iTreeImage );
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iTreeImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iTreeImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iTreeImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if (( aObject.iSurface == Enigma::Object::Direction::EAST )
|| ( aObject.iSurface == Enigma::Object::Direction::WEST ))
{
// Draw a tree growing on the East/West surface.

switch( object.m_rotation )
{  
case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 0, iTreeCeilingImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 0, iTreeFloorImage );
break;

case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iTreeHorizontalImage );
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iTreeHorizontalImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if (( aObject.iSurface == Enigma::Object::Direction::NORTH )
|| ( aObject.iSurface == Enigma::Object::Direction::SOUTH ))
{
// Draw a tree growing on the North/South surface.

switch( object.m_rotation )
{  
case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 90, iTreeFloorImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 90, iTreeCeilingImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iTreeHorizontalImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iTreeHorizontalImage );
break;

default:
Drawn = FALSE;
break;
}
}  
}
else if ( object.m_id == Enigma::Object::ID::ETreeTop )
{
if  (( aObject.iSurface == Enigma::Object::Direction::ABOVE )
|| ( aObject.iSurface == Enigma::Object::Direction::BELOW ))
{
// Draw a tree top on the Above/Below surface.

switch( object.m_rotation )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iTreeTopImage ); 
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iTreeTopImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iTreeTopImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iTreeTopImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if ( aObject.iSurface == Enigma::Object::Direction::WEST )
{
// Draw a tree top on the West surface.

switch( object.m_rotation )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iTreeTopHorizontalImage ); 
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 270, iTreeTopHorizontalImage );
break;

case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 0, iTreeTopCeilingImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 0, iTreeTopFloorImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if ( aObject.iSurface == Enigma::Object::Direction::EAST )
{
// Draw a tree top on the East surface.

switch( object.m_rotation )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 90, iTreeTopHorizontalImage ); 
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iTreeTopHorizontalImage );
break;

case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 180, iTreeTopCeilingImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 180, iTreeTopFloorImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if ( aObject.iSurface == Enigma::Object::Direction::NORTH )
{
// Draw a tree top on the North surface.

switch( object.m_rotation )
{
case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 0, iTreeTopHorizontalImage ); 
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iTreeTopHorizontalImage );
break;

case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 90, iTreeTopFloorImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 90, iTreeTopCeilingImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if ( aObject.iSurface == Enigma::Object::Direction::SOUTH )
{
// Draw a tree top on the South surface.

switch( object.m_rotation )
{
case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iTreeTopHorizontalImage ); 
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 180, iTreeTopHorizontalImage );
break;

case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 270, iTreeTopFloorImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 270, iTreeTopCeilingImage );
break;

default:
Drawn = FALSE;
break;
}
}
else
Drawn = FALSE;
}
else if ( object.m_id == Enigma::Object::ID::ECatWalk )
{
if  (( object.m_rotation == Enigma::Object::Direction::ABOVE )
|| ( object.m_rotation == Enigma::Object::Direction::BELOW ))	  
{
// Draw an Above/Below catwalk. 

switch( aObject.iSurface )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iCatWalkVerticalImage );
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iCatWalkVerticalImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iCatWalkVerticalImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iCatWalkVerticalImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if (( object.m_rotation == Enigma::Object::Direction::EAST )
|| ( object.m_rotation == Enigma::Object::Direction::WEST ))
{
// Draw an East/West catwalk.

switch( aObject.iSurface )
{  
case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 0, iCatWalkCeilingImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 0, iCatWalkFloorImage );
break;

case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iCatWalkHorizontalImage );
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iCatWalkHorizontalImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if (( object.m_rotation == Enigma::Object::Direction::NORTH )
|| ( object.m_rotation == Enigma::Object::Direction::SOUTH ))
{
// Draw a North/South catwalk.

switch( aObject.iSurface )
{  
case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 90, iCatWalkFloorImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 90, iCatWalkCeilingImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iCatWalkHorizontalImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iCatWalkHorizontalImage );
break;

default:
Drawn = FALSE;
break;
}
}  
}
else if ( object.m_id == Enigma::Object::ID::EFern )
{
if ( object.m_rotation == Enigma::Object::Direction::BELOW )
{
switch( aObject.iSurface )
{  
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iFernFloorImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iFernFloorImage );
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iFernFloorImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iFernFloorImage );
break;

default:
Drawn = FALSE;
break;
}
}
else
Drawn = FALSE;
}
else if ( object.m_id == Enigma::Object::ID::EHandHold )
{
if (((( aObject.iSurface == Enigma::Object::Direction::ABOVE )
|| ( aObject.iSurface == Enigma::Object::Direction::BELOW ))  
&& ( object.m_rotation == Enigma::Object::Direction::NORTH ))
||
((( object.m_rotation == Enigma::Object::Direction::ABOVE )
|| ( object.m_rotation == Enigma::Object::Direction::BELOW ))
&& ( aObject.iSurface == Enigma::Object::Direction::NORTH )))
{
draw_tile(context, x, y, 0, iHandHoldImage ); 
}
else if (((( aObject.iSurface == Enigma::Object::Direction::ABOVE )
|| ( aObject.iSurface == Enigma::Object::Direction::BELOW ))  
&& ( object.m_rotation == Enigma::Object::Direction::EAST ))
||
((( object.m_rotation == Enigma::Object::Direction::ABOVE )
|| ( object.m_rotation == Enigma::Object::Direction::BELOW ))
&& ( aObject.iSurface == Enigma::Object::Direction::EAST )))
{
draw_tile(context, x, y, 90, iHandHoldImage ); 
}
else if (((( aObject.iSurface == Enigma::Object::Direction::ABOVE )
|| ( aObject.iSurface == Enigma::Object::Direction::BELOW ))  
&& ( object.m_rotation == Enigma::Object::Direction::SOUTH ))
||
((( object.m_rotation == Enigma::Object::Direction::ABOVE )
|| ( object.m_rotation == Enigma::Object::Direction::BELOW ))
&& ( aObject.iSurface == Enigma::Object::Direction::SOUTH )))
{
draw_tile(context, x, y, 180, iHandHoldImage ); 
}
else if (((( aObject.iSurface == Enigma::Object::Direction::ABOVE )
|| ( aObject.iSurface == Enigma::Object::Direction::BELOW ))  
&& ( object.m_rotation == Enigma::Object::Direction::WEST ))
||
((( object.m_rotation == Enigma::Object::Direction::ABOVE )
|| ( object.m_rotation == Enigma::Object::Direction::BELOW ))
&& ( aObject.iSurface == Enigma::Object::Direction::WEST )))
{
draw_tile(context, x, y, 270, iHandHoldImage ); 
}
else if ((( aObject.iSurface == Enigma::Object::Direction::NORTH )
&& ( object.m_rotation == Enigma::Object::Direction::WEST ))
|| (( aObject.iSurface == Enigma::Object::Direction::WEST )
&& ( object.m_rotation == Enigma::Object::Direction::NORTH )))
{
draw_tile(context, x, y, 0, iHandHoldHorizontalImage );
}
else if ((( aObject.iSurface == Enigma::Object::Direction::EAST )
&& ( object.m_rotation == Enigma::Object::Direction::NORTH ))
|| (( aObject.iSurface == Enigma::Object::Direction::NORTH )
&& ( object.m_rotation == Enigma::Object::Direction::EAST )))
{
draw_tile(context, x, y, 90, iHandHoldHorizontalImage );
}
else if ((( aObject.iSurface == Enigma::Object::Direction::SOUTH )
&& ( object.m_rotation == Enigma::Object::Direction::EAST ))
|| (( aObject.iSurface == Enigma::Object::Direction::EAST )
&& ( object.m_rotation == Enigma::Object::Direction::SOUTH )))
{
draw_tile(context, x, y, 180, iHandHoldHorizontalImage );
}
else if ((( aObject.iSurface == Enigma::Object::Direction::WEST )
&& ( object.m_rotation == Enigma::Object::Direction::SOUTH ))
|| (( aObject.iSurface == Enigma::Object::Direction::SOUTH )
&& ( object.m_rotation == Enigma::Object::Direction::WEST )))
{
draw_tile(context, x, y, 270, iHandHoldHorizontalImage );
}	  
else
Drawn = FALSE;
}
else if ( object.m_id == Enigma::Object::ID::EOutdoor )      
{
switch( aObject.iSurface )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iOutdoorVerticalImage ); 
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iOutdoorVerticalImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iOutdoorVerticalImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iOutdoorVerticalImage );
break;

case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 0, iOutdoorCeilingImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 0, iOutdoorFloorImage );
break;

case Enigma::Object::Direction::ECenter:
draw_tile(context, x, y, 0, iOutdoorImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if ( object.m_id == Enigma::Object::ID::EIndoor )      
{
switch( aObject.iSurface )
{
case Enigma::Object::Direction::NORTH:
draw_tile(context, x, y, 0, iIndoorVerticalImage ); 
break;

case Enigma::Object::Direction::SOUTH:
draw_tile(context, x, y, 180, iIndoorVerticalImage );
break;

case Enigma::Object::Direction::EAST:
draw_tile(context, x, y, 90, iIndoorVerticalImage );
break;

case Enigma::Object::Direction::WEST:
draw_tile(context, x, y, 270, iIndoorVerticalImage );
break;

case Enigma::Object::Direction::ABOVE:
draw_tile(context, x, y, 0, iIndoorCeilingImage );
break;

case Enigma::Object::Direction::BELOW:
draw_tile(context, x, y, 0, iIndoorFloorImage );
break;

case Enigma::Object::Direction::ECenter:
draw_tile(context, x, y, 0, iIndoorImage );
break;

default:
Drawn = FALSE;
break;
}
}
else if ( object.m_id == Enigma::Object::ID::EPerson )
{
if ( aObject.iActive )
draw_tile(context, x, y, 0, iPlayerActiveImage );
else
draw_tile(context, x, y, 0, iPlayerIdleImage );
}
else
Drawn = false;

return Drawn;
}

