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

#include "Tiles.h"

//--------------------
// Local declarations.
//--------------------

#define TILESIZE 60         // Pixel size of image tile.
               
//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::Tiles::Tiles()
{ 	
	// Load image tiles from files.
	
	m_cursor =
		Cairo::ImageSurface::create_from_png("./images/Cursor.png");

	m_marker =
		Cairo::ImageSurface::create_from_png("./images/Marker.png");

	m_generic =
		Cairo::ImageSurface::create_from_png("./images/Generic.png");

	m_wall = 
		Cairo::ImageSurface::create_from_png("./images/Wall.png");

	m_ceiling =
		Cairo::ImageSurface::create_from_png("./images/Ceiling.png");

	m_floor =
		Cairo::ImageSurface::create_from_png("./images/Floor.png");

	m_ladder =
		Cairo::ImageSurface::create_from_png("./images/Ladder.png");

	m_ladderend =
		Cairo::ImageSurface::create_from_png("./images/LadderEnd.png");

	m_ladderend_horizontal =
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

	m_mover_below =
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
		Cairo::ImageSurface::create_from_png("./images/IndoorCeiling.png");

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

	m_tree =
		Cairo::ImageSurface::create_from_png("./images/Tree.png");

	m_tree_horizontal =
		Cairo::ImageSurface::create_from_png("./images/TreeHorizontal.png");

	m_tree_ceiling =
		Cairo::ImageSurface::create_from_png("./images/TreeCeiling.png");

	m_tree_floor =
		Cairo::ImageSurface::create_from_png("./images/TreeFloor.png");

	m_treetop =
		Cairo::ImageSurface::create_from_png("./images/TreeTop.png");

	m_treetop_horizontal =
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

	m_fern_floor =
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

	m_padbutton_floor =
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

void Enigma::Tiles::draw_cursor(const Cairo::RefPtr<Cairo::Context>& context,
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
	double x = column * TILESIZE;
	double y = allocation.get_height() - TILESIZE - (row * TILESIZE);

	context->set_source(m_generic, x, y);
	context->paint();
}

//----------------------------------------------------------------
// This method draws a tile marking a teleporter arrival location.
//----------------------------------------------------------------
// context:     Cairo context for drawing.
// aAllocation: Pixel boundary of view.
// aColumn:     Cursor column number.
// aRow:        Cursor row number.
//----------------------------------------------------------------

void Enigma::Tiles::draw_arrival(const Cairo::RefPtr<Cairo::Context>& context,
                                 Gtk::Allocation allocation,
                                 guint16 column,
                                 guint16 row)
{
	double x = column * TILESIZE;
	double y = allocation.get_height() - TILESIZE - (row * TILESIZE);

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
		// Draw a generic image for an item.

		draw_tile(context, x, y, 0, m_item);
	}
	else if (object.m_type == Enigma::Object::Type::TELEPORTER)
	{
		// Draw a general image for all teleporters.

		draw_tile(context, x, y, 0, m_teleporter_departure);
	}
	else if (object.m_id == Enigma::Object::ID::STAIRS)
	{
		// Draw stairs image.
		
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
			// Draw StairsTop image.
			
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
			// Draw ArchWay image.
			
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
		// Draw WoodDoor image.
		
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
		// Draw PullRing image.
		
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
		// Draw generic lock image.
		
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
	else if ((object.m_id == Enigma::Object::ID::WATERLAYER))
	{
		// Draw WaterLayer image.
		
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
	else if (object.m_id == Enigma::Object::ID::WATER)
	{
		// Draw Water image.
		
		draw_tile(context, x, y, 0, m_water);
	}
	else if (object.m_id == Enigma::Object::ID::LIGHTBEAM)
	{
		// Draw LightBeam image.
		
		switch(object.m_surface)
		{
			case Enigma::Object::Direction::NORTH:
				draw_tile(context, x, y, 0, m_lightbeam_horizontal); 
				break;

			case Enigma::Object::Direction::SOUTH:
				draw_tile(context, x, y, 180, m_lightbeam_horizontal);
				break;

			case Enigma::Object::Direction::EAST:
				draw_tile(context, x, y, 90, m_lightbeam_horizontal);
				break;

			case Enigma::Object::Direction::WEST:
				draw_tile(context, x, y, 270, m_lightbeam_horizontal);
				break;

			case Enigma::Object::Direction::ABOVE:
			case Enigma::Object::Direction::BELOW:
				draw_tile(context, x, y, 0, m_lightbeam_vertical);	
				break;

			default:
				drawn = false;
				break;
		}
	}
	else if (object.m_id == Enigma::Object::ID::WOODWALL)
	{
		// Draw WoodWall image.
		
		switch(object.m_surface)
		{
			case Enigma::Object::Direction::NORTH:
				draw_tile(context, x, y, 0, m_woodwall_vertical); 
				break;

			case Enigma::Object::Direction::SOUTH:
				draw_tile(context, x, y, 180, m_woodwall_vertical);
				break;

			case Enigma::Object::Direction::EAST:
				draw_tile(context, x, y, 90, m_woodwall_vertical);
				break;

			case Enigma::Object::Direction::WEST:
				draw_tile(context, x, y, 270, m_woodwall_vertical);
				break;

			case Enigma::Object::Direction::ABOVE:
				draw_tile(context, x, y, 0, m_woodwall_ceiling);
				break;

			case Enigma::Object::Direction::BELOW:
				draw_tile(context, x, y, 0, m_woodwall_floor);
				break;

			default:
				drawn = false;
				break;
		}
	}
	else if (object.m_id == Enigma::Object::ID::PADBUTTON)
	{
		// Draw PadButton image.
		
		switch(object.m_surface)
		{
			case Enigma::Object::Direction::NORTH:
				draw_tile(context, x, y, 0, m_padbutton_vertical); 
				break;

			case Enigma::Object::Direction::SOUTH:
				draw_tile(context, x, y, 180, m_padbutton_vertical);
				break;

			case Enigma::Object::Direction::EAST:
				draw_tile(context, x, y, 90, m_padbutton_vertical);
				break;

			case Enigma::Object::Direction::WEST:
				draw_tile(context, x, y, 270, m_padbutton_vertical);
				break;

			case Enigma::Object::Direction::ABOVE:
				draw_tile(context, x, y, 0, m_padbutton_ceiling);
				break;

			case Enigma::Object::Direction::BELOW:
				draw_tile(context, x, y, 0, m_padbutton_floor);
				break;

			default:
				drawn = false;
				break;
		}
	}
	else if (object.m_id == Enigma::Object::ID::EARTHWALL)
	{
		// Draw EarthWall image.
		
		switch(object.m_surface)
		{
			case Enigma::Object::Direction::NORTH:
				draw_tile(context, x, y, 0, m_earthwall_vertical); 
				break;

			case Enigma::Object::Direction::SOUTH:
				draw_tile(context, x, y, 180, m_earthwall_vertical);
				break;

			case Enigma::Object::Direction::EAST:
				draw_tile(context, x, y, 90, m_earthwall_vertical);
				break;

			case Enigma::Object::Direction::WEST:
				draw_tile(context, x, y, 270, m_earthwall_vertical);
				break;

			case Enigma::Object::Direction::ABOVE:
				draw_tile(context, x, y, 0, m_earthwall_ceiling);
				break;

			case Enigma::Object::Direction::BELOW:
				draw_tile(context, x, y, 0, m_earthwall_floor);
				break;

			default:
				drawn = false;
				break;
		}
	}
	else if ((object.m_id == Enigma::Object::ID::BLOCKWALL)
	      || (object.m_id == Enigma::Object::ID::STONEWALL))      
	{
		// Draw BlockWall or StoneWall image.
		
		switch(object.m_surface)
		{
			case Enigma::Object::Direction::NORTH:
				draw_tile(context, x, y, 0, m_wall); 
				break;

			case Enigma::Object::Direction::SOUTH:
				draw_tile(context, x, y, 180, m_wall);
				break;

			case Enigma::Object::Direction::EAST:
				draw_tile(context, x, y, 90, m_wall);
				break;

			case Enigma::Object::Direction::WEST:
				draw_tile(context, x, y, 270, m_wall);
				break;

			case Enigma::Object::Direction::ABOVE:
				draw_tile(context, x, y, 0, m_ceiling);
				break;

			case Enigma::Object::Direction::BELOW:
				draw_tile(context, x, y, 0, m_floor);
				break;

			default:
				drawn = false;
				break;
		}
	}	
	else if (object.m_id == Enigma::Object::ID::BLOCKER)
	{
		// Draw Blocker image.
	
		switch(object.m_surface)
		{
			case Enigma::Object::Direction::NORTH:
				draw_tile(context, x, y, 0, m_blocker); 
				break;

			case Enigma::Object::Direction::SOUTH:
				draw_tile(context, x, y, 180, m_blocker);
				break;

			case Enigma::Object::Direction::EAST:
				draw_tile(context, x, y, 90, m_blocker);
				break;

			case Enigma::Object::Direction::WEST:
				draw_tile(context, x, y, 270, m_blocker);
				break;

			case Enigma::Object::Direction::ABOVE:
			case Enigma::Object::Direction::BELOW:
				draw_tile(context, x, y, 0, m_blocker_horizontal);
				break;

			default:
				drawn = false;
				break;
		}
	}
	else if (object.m_id == Enigma::Object::ID::FLIPPER)
	{
		// Draw Flipper image.

		switch(object.m_surface)
		{
			case Enigma::Object::Direction::NORTH:
				draw_tile(context, x, y, 0, m_surfacer); 
				break;

			case Enigma::Object::Direction::SOUTH:
				draw_tile(context, x, y, 180, m_surfacer);
				break;

			case Enigma::Object::Direction::EAST:
				draw_tile(context, x, y, 90, m_surfacer);
				break;

			case Enigma::Object::Direction::WEST:
				draw_tile(context, x, y, 270, m_surfacer);
				break;

			case Enigma::Object::Direction::ABOVE:
			case Enigma::Object::Direction::BELOW:
				draw_tile(context, x, y, 0, m_flipper); 
				break;

			default:
				drawn = false;
				break;
		}
	}
	else if ((object.m_id == Enigma::Object::ID::SURFACER)
	      || (object.m_id == Enigma::Object::ID::EDGER))
	{
		// Draw Surfacer or Edger image on a horizontal surface.

		if  ((object.m_surface == Enigma::Object::Direction::ABOVE)
		  || (object.m_surface == Enigma::Object::Direction::BELOW))
		{
			switch( object.m_rotation )
			{
				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 0, m_surfacer); 
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 180, m_surfacer);
					break;

				case Enigma::Object::Direction::EAST:
					draw_tile(context, x, y, 90, m_surfacer);
					break;

				case Enigma::Object::Direction::WEST:
					draw_tile(context, x, y, 270, m_surfacer);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else
		{
			// Draw Surfacer or Edger image on a vertical surface.
			
			switch (object.m_surface)
			{
				case Enigma::Object::Direction::NORTH:
					if (object.m_rotation == Enigma::Object::Direction::WEST)
						draw_tile(context, x, y, 0, m_surfacer_corner);
					else if (object.m_rotation == Enigma::Object::Direction::EAST)
						draw_tile(context, x, y, 90, m_surfacer_corner);
					else
						draw_tile(context, x, y, 0, m_surfacer); 
					break;

				case Enigma::Object::Direction::SOUTH:
					if ( object.m_rotation == Enigma::Object::Direction::WEST )
						draw_tile(context, x, y, 270, m_surfacer_corner);
					else if ( object.m_rotation == Enigma::Object::Direction::EAST )
						draw_tile(context, x, y, 180, m_surfacer_corner);
					else
						draw_tile(context, x, y, 180, m_surfacer);
					break;

				case Enigma::Object::Direction::EAST:
					if ( object.m_rotation == Enigma::Object::Direction::NORTH )
						draw_tile(context, x, y, 90, m_surfacer_corner);
					else if ( object.m_rotation == Enigma::Object::Direction::SOUTH )
						draw_tile(context, x, y, 180, m_surfacer_corner);
					else
						draw_tile(context, x, y, 90, m_surfacer);
					break;

				case Enigma::Object::Direction::WEST:
					if ( object.m_rotation == Enigma::Object::Direction::NORTH )
						draw_tile(context, x, y, 0, m_surfacer_corner);
					else if ( object.m_rotation == Enigma::Object::Direction::SOUTH )
						draw_tile(context, x, y, 270, m_surfacer_corner);
					else
						draw_tile(context, x, y, 270, m_surfacer);
					break;

				default:
					drawn = false;
					break;
			}
		}
	}
	else if (object.m_id == Enigma::Object::ID::MOVER)
	{
		// Draw Mover image.
		
		switch (object.m_rotation)
		{
			case Enigma::Object::Direction::NORTH:
				draw_tile(context, x, y, 0, m_mover); 
				break;

			case Enigma::Object::Direction::SOUTH:
				draw_tile(context, x, y, 180, m_mover);
				break;

			case Enigma::Object::Direction::EAST:
				draw_tile(context, x, y, 90, m_mover);
				break;

			case Enigma::Object::Direction::WEST:
				draw_tile(context, x, y, 270, m_mover);
				break;

			case Enigma::Object::Direction::ABOVE:
				draw_tile(context, x, y, 0, m_mover_above);
				break;

			case Enigma::Object::Direction::BELOW:
				draw_tile(context, x, y, 0, m_mover_below);
				break;

			default:
				drawn = false;
				break;
		}
	}
	else if (object.m_id == Enigma::Object::ID::TURNER)
	{
		// Draw Turner image.

		draw_tile(context, x, y, 0, m_turner); 
	}
	else if (object.m_id == Enigma::Object::ID::LADDER)
	{
		if  ((object.m_rotation == Enigma::Object::Direction::ABOVE)
		  || (object.m_rotation == Enigma::Object::Direction::BELOW))	  
		{
			// Draw horizontal surface Ladder image.

			switch (object.m_surface)
			{
				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 0, m_ladder);
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 180, m_ladder);
					break;

				case Enigma::Object::Direction::EAST:
					draw_tile(context, x, y, 90, m_ladder);
					break;

				case Enigma::Object::Direction::WEST:
					draw_tile(context, x, y, 270, m_ladder);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else if ((object.m_rotation == Enigma::Object::Direction::EAST)
		      || (object.m_rotation == Enigma::Object::Direction::WEST))
		{
			// Draw vertical surface East/West Ladder image.

			switch (object.m_surface)
			{  
				case Enigma::Object::Direction::ABOVE:
					draw_tile(context, x, y, 0, m_ladder_ceiling);
					break;

				case Enigma::Object::Direction::BELOW:
					draw_tile(context, x, y, 0, m_ladder_floor);
					break;

				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 0, m_ladder_horizontal);
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 180, m_ladder_horizontal);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else if ((object.m_rotation == Enigma::Object::Direction::NORTH)
		      || (object.m_rotation == Enigma::Object::Direction::SOUTH))
		{
			// Draw vertical surface North/South Ladder image.

			switch (object.m_surface)
			{  
				case Enigma::Object::Direction::ABOVE:
					draw_tile(context, x, y, 90, m_ladder_floor);
					break;

				case Enigma::Object::Direction::BELOW:
					draw_tile(context, x, y, 90, m_ladder_ceiling);
					break;

				case Enigma::Object::Direction::EAST:
					draw_tile(context, x, y, 90, m_ladder_horizontal);
					break;

				case Enigma::Object::Direction::WEST:
					draw_tile(context, x, y, 270, m_ladder_horizontal);
					break;

				default:
					drawn = false;
					break;
			}
		}  
	}
	else if ( object.m_id == Enigma::Object::ID::LADDEREND)
	{
		if  ((object.m_rotation == Enigma::Object::Direction::ABOVE)
		  || (object.m_rotation == Enigma::Object::Direction::BELOW))
		{
			// Draw a vertical LadderTop image.

			switch (object.m_surface)
			{
				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 0, m_ladderend); 
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 180, m_ladderend);
					break;

				case Enigma::Object::Direction::EAST:
					draw_tile(context, x, y, 90, m_ladderend);
					break;

				case Enigma::Object::Direction::WEST:
					draw_tile(context, x, y, 270, m_ladderend);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else if (object.m_rotation == Enigma::Object::Direction::WEST)
		{
			// Draw West rotation LadderTop image.

			switch (object.m_surface)
			{
				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 0, m_ladderend_horizontal); 
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 270, m_ladderend_horizontal);
					break;

				case Enigma::Object::Direction::ABOVE:
					draw_tile(context, x, y, 0, m_ladderend_ceiling);
					break;

				case Enigma::Object::Direction::BELOW:
					draw_tile(context, x, y, 0, m_ladderend_floor);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else if (object.m_rotation == Enigma::Object::Direction::EAST)
		{
			// Draw East rotation LadderTop image.

			switch (object.m_surface)
			{
				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 90, m_ladderend_horizontal); 
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 180, m_ladderend_horizontal);
					break;

				case Enigma::Object::Direction::ABOVE:
					draw_tile(context, x, y, 180, m_ladderend_ceiling);
					break;

				case Enigma::Object::Direction::BELOW:
					draw_tile(context, x, y, 180, m_ladderend_floor);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else if (object.m_rotation == Enigma::Object::Direction::NORTH)
		{
			// Draw a North rotation LadderTop image.

			switch (object.m_surface)
			{
				case Enigma::Object::Direction::WEST:
					draw_tile(context, x, y, 0, m_ladderend_horizontal); 
					break;

				case Enigma::Object::Direction::EAST:
					draw_tile(context, x, y, 90, m_ladderend_horizontal );
					break;

				case Enigma::Object::Direction::ABOVE:
					draw_tile(context, x, y, 90, m_ladderend_floor);
					break;

				case Enigma::Object::Direction::BELOW:
					draw_tile(context, x, y, 90, m_ladderend_ceiling);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else if (object.m_rotation == Enigma::Object::Direction::SOUTH)
		{
			// Draw a South rotation LadderTop image.

			switch (object.m_surface)
			{
				case Enigma::Object::Direction::WEST:
					draw_tile(context, x, y, 270, m_ladderend_horizontal); 
					break;

				case Enigma::Object::Direction::EAST:
					draw_tile(context, x, y, 180, m_ladderend_horizontal);
					break;

				case Enigma::Object::Direction::ABOVE:
					draw_tile(context, x, y, 270, m_ladderend_floor);
					break;

				case Enigma::Object::Direction::BELOW:
					draw_tile(context, x, y, 270, m_ladderend_ceiling);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else
			drawn = false;
	}
	else if (object.m_id == Enigma::Object::ID::TREE)
	{
		if  ((object.m_surface == Enigma::Object::Direction::ABOVE)
		  || (object.m_surface == Enigma::Object::Direction::BELOW))	  
		{
			// Draw image of Tree growing on Above/Below surface.

			switch (object.m_rotation)
			{
				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 0, m_tree);
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 180, m_tree);
					break;

				case Enigma::Object::Direction::EAST:
					draw_tile(context, x, y, 90, m_tree);
					break;

				case Enigma::Object::Direction::WEST:
					draw_tile(context, x, y, 270, m_tree);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else if ((object.m_surface == Enigma::Object::Direction::EAST)
		      || (object.m_surface == Enigma::Object::Direction::WEST))
		{
			// Draw image of Tree growing on the East/West surface.

			switch (object.m_rotation)
			{  
				case Enigma::Object::Direction::ABOVE:
					draw_tile(context, x, y, 0, m_tree_ceiling);
					break;

				case Enigma::Object::Direction::BELOW:
					draw_tile(context, x, y, 0, m_tree_floor);
					break;

				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 0, m_tree_horizontal);
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 180, m_tree_horizontal);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else if ((object.m_surface == Enigma::Object::Direction::NORTH)
		      || (object.m_surface == Enigma::Object::Direction::SOUTH))
		{
			// Draw image of Tree growing on the North/South surface.

			switch (object.m_rotation)
			{  
				case Enigma::Object::Direction::ABOVE:
					draw_tile(context, x, y, 90, m_tree_floor);
					break;

				case Enigma::Object::Direction::BELOW:
					draw_tile(context, x, y, 90, m_tree_ceiling);
					break;

				case Enigma::Object::Direction::EAST:
					draw_tile(context, x, y, 90, m_tree_horizontal);
					break;

				case Enigma::Object::Direction::WEST:
					draw_tile(context, x, y, 270, m_tree_horizontal);
					break;

				default:
					drawn = false;
					break;
			}
		}  
	}
	else if (object.m_id == Enigma::Object::ID::TREETOP)
	{
		if  ((object.m_surface == Enigma::Object::Direction::ABOVE)
		  || (object.m_surface == Enigma::Object::Direction::BELOW))
		{
			// Draw a tree top on the Above/Below surface.

			switch (object.m_rotation)
			{
				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 0, m_treetop); 
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 180, m_treetop);
					break;

				case Enigma::Object::Direction::EAST:
					draw_tile(context, x, y, 90, m_treetop);
					break;

				case Enigma::Object::Direction::WEST:
					draw_tile(context, x, y, 270, m_treetop);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else if (object.m_surface == Enigma::Object::Direction::WEST)
		{
			// Draw image of TreeTop on West surface.

			switch (object.m_rotation)
			{
				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 0, m_treetop_horizontal); 
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 270, m_treetop_horizontal);
					break;

				case Enigma::Object::Direction::ABOVE:
					draw_tile(context, x, y, 0, m_treetop_ceiling);
					break;

				case Enigma::Object::Direction::BELOW:
					draw_tile(context, x, y, 0, m_treetop_floor);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else if ( object.m_surface == Enigma::Object::Direction::EAST )
		{
			// Draw image of TreeTop on East surface.

			switch (object.m_rotation)
			{
				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 90, m_treetop_horizontal); 
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 180, m_treetop_horizontal);
					break;

				case Enigma::Object::Direction::ABOVE:
					draw_tile(context, x, y, 180, m_treetop_ceiling);
					break;

				case Enigma::Object::Direction::BELOW:
					draw_tile(context, x, y, 180, m_treetop_floor);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else if ( object.m_surface == Enigma::Object::Direction::NORTH )
		{
			// Draw a tree top on the North surface.

			switch (object.m_rotation)
			{
				case Enigma::Object::Direction::WEST:
					draw_tile(context, x, y, 0, m_treetop_horizontal); 
					break;

				case Enigma::Object::Direction::EAST:
					draw_tile(context, x, y, 90, m_treetop_horizontal);
					break;

				case Enigma::Object::Direction::ABOVE:
					draw_tile(context, x, y, 90, m_treetop_floor);
					break;

				case Enigma::Object::Direction::BELOW:
					draw_tile(context, x, y, 90, m_treetop_ceiling);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else if ( object.m_surface == Enigma::Object::Direction::SOUTH )
		{
			// Draw image of TreeTop on South surface.

			switch (object.m_rotation)
			{
				case Enigma::Object::Direction::WEST:
					draw_tile(context, x, y, 270, m_treetop_horizontal); 
					break;

				case Enigma::Object::Direction::EAST:
					draw_tile(context, x, y, 180, m_treetop_horizontal);
					break;

				case Enigma::Object::Direction::ABOVE:
					draw_tile(context, x, y, 270, m_treetop_floor);
					break;

				case Enigma::Object::Direction::BELOW:
					draw_tile(context, x, y, 270, m_treetop_ceiling);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else
			drawn = false;
	}
	else if (object.m_id == Enigma::Object::ID::CATWALK)
	{
		if  ((object.m_rotation == Enigma::Object::Direction::ABOVE)
		  || (object.m_rotation == Enigma::Object::Direction::BELOW))	  
		{
			// Draw image of Above/Below CatWalk. 

			switch (object.m_surface)
			{
				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 0, m_catwalk_vertical);
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 180, m_catwalk_vertical);
					break;

				case Enigma::Object::Direction::EAST:
					draw_tile(context, x, y, 90, m_catwalk_vertical);
					break;

				case Enigma::Object::Direction::WEST:
					draw_tile(context, x, y, 270, m_catwalk_vertical);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else if ((object.m_rotation == Enigma::Object::Direction::EAST)
		      || (object.m_rotation == Enigma::Object::Direction::WEST))
		{
			// Draw image of East/West catwalk.

			switch (object.m_surface)
			{  
				case Enigma::Object::Direction::ABOVE:
					draw_tile(context, x, y, 0, m_catwalk_ceiling);
					break;

				case Enigma::Object::Direction::BELOW:
					draw_tile(context, x, y, 0, m_catwalk_floor);
					break;

				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 0, m_catwalk_horizontal);
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 180, m_catwalk_horizontal);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else if ((object.m_rotation == Enigma::Object::Direction::NORTH)
		      || (object.m_rotation == Enigma::Object::Direction::SOUTH))
		{
			// Draw image of North/South catwalk.

			switch (object.m_surface)
			{  
				case Enigma::Object::Direction::ABOVE:
				draw_tile(context, x, y, 90, m_catwalk_floor);
				break;

				case Enigma::Object::Direction::BELOW:
				draw_tile(context, x, y, 90, m_catwalk_ceiling);
				break;

				case Enigma::Object::Direction::EAST:
				draw_tile(context, x, y, 90, m_catwalk_horizontal);
				break;

				case Enigma::Object::Direction::WEST:
				draw_tile(context, x, y, 270, m_catwalk_horizontal);
				break;

				default:
				drawn = false;
				break;
			}
		}  
	}
	else if (object.m_id == Enigma::Object::ID::FERN)
	{
		// Draw image of Fern on the Below surface growing out from
		// a vertical surface.
		
		if (object.m_rotation == Enigma::Object::Direction::BELOW)
		{
			switch (object.m_surface)
			{  
				case Enigma::Object::Direction::NORTH:
					draw_tile(context, x, y, 0, m_fern_floor);
					break;

				case Enigma::Object::Direction::EAST:
					draw_tile(context, x, y, 90, m_fern_floor);
					break;

				case Enigma::Object::Direction::SOUTH:
					draw_tile(context, x, y, 180, m_fern_floor);
					break;

				case Enigma::Object::Direction::WEST:
					draw_tile(context, x, y, 270, m_fern_floor);
					break;

				default:
					drawn = false;
					break;
			}
		}
		else
			drawn = false;
	}
	else if (object.m_id == Enigma::Object::ID::HANDHOLD)
	{
		if ((((object.m_surface == Enigma::Object::Direction::ABOVE)
		   || (object.m_surface == Enigma::Object::Direction::BELOW))  
		   && (object.m_rotation == Enigma::Object::Direction::NORTH))
		   ||
		    (((object.m_rotation == Enigma::Object::Direction::ABOVE)
		   || (object.m_rotation == Enigma::Object::Direction::BELOW))
		   && (object.m_surface == Enigma::Object::Direction::NORTH)))
		{
			draw_tile(context, x, y, 0, m_handhold); 
		}
		else if ((((object.m_surface == Enigma::Object::Direction::ABOVE)
		  || (object.m_surface == Enigma::Object::Direction::BELOW))  
		  && (object.m_rotation == Enigma::Object::Direction::EAST))
		  ||
		  (((object.m_rotation == Enigma::Object::Direction::ABOVE)
		  || (object.m_rotation == Enigma::Object::Direction::BELOW))
		  && (object.m_surface == Enigma::Object::Direction::EAST)))
		{
			draw_tile(context, x, y, 90, m_handhold); 
		}
		else if ((((object.m_surface == Enigma::Object::Direction::ABOVE)
		  || (object.m_surface == Enigma::Object::Direction::BELOW))  
		  && (object.m_rotation == Enigma::Object::Direction::SOUTH))
		  ||
		  (((object.m_rotation == Enigma::Object::Direction::ABOVE)
		  || (object.m_rotation == Enigma::Object::Direction::BELOW))
		  && (object.m_surface == Enigma::Object::Direction::SOUTH)))
		{
			draw_tile(context, x, y, 180, m_handhold); 
		}
		else if ((((object.m_surface == Enigma::Object::Direction::ABOVE)
		  || (object.m_surface == Enigma::Object::Direction::BELOW))  
		  && (object.m_rotation == Enigma::Object::Direction::WEST))
		  ||
		  (((object.m_rotation == Enigma::Object::Direction::ABOVE)
		  || (object.m_rotation == Enigma::Object::Direction::BELOW))
		  && (object.m_surface == Enigma::Object::Direction::WEST)))
		{
			draw_tile(context, x, y, 270, m_handhold); 
		}
		else if (((object.m_surface == Enigma::Object::Direction::NORTH)
		  && (object.m_rotation == Enigma::Object::Direction::WEST))
		  || ((object.m_surface == Enigma::Object::Direction::WEST)
		  && (object.m_rotation == Enigma::Object::Direction::NORTH)))
		{
			draw_tile(context, x, y, 0, m_handhold_horizontal);
		}
		else if (((object.m_surface == Enigma::Object::Direction::EAST)
		  && (object.m_rotation == Enigma::Object::Direction::NORTH))
		  || ((object.m_surface == Enigma::Object::Direction::NORTH)
		  && (object.m_rotation == Enigma::Object::Direction::EAST)))
		{
			draw_tile(context, x, y, 90, m_handhold_horizontal);
		}
		else if (((object.m_surface == Enigma::Object::Direction::SOUTH)
		  && (object.m_rotation == Enigma::Object::Direction::EAST))
		  || ((object.m_surface == Enigma::Object::Direction::EAST)
		  && (object.m_rotation == Enigma::Object::Direction::SOUTH)))
		{
			draw_tile(context, x, y, 180, m_handhold_horizontal);
		}
		else if (((object.m_surface == Enigma::Object::Direction::WEST)
		  && (object.m_rotation == Enigma::Object::Direction::SOUTH))
		  || ((object.m_surface == Enigma::Object::Direction::SOUTH)
		  && (object.m_rotation == Enigma::Object::Direction::WEST)))
		{
			draw_tile(context, x, y, 270, m_handhold_horizontal);
		}	  
		else
			drawn = false;
	}
	else if (object.m_id == Enigma::Object::ID::OUTDOOR)      
	{
		// Draw an Outdoor environment image.
		
		switch (object.m_surface)
		{
			case Enigma::Object::Direction::NORTH:
				draw_tile(context, x, y, 0, m_outdoor_vertical); 
				break;

			case Enigma::Object::Direction::SOUTH:
				draw_tile(context, x, y, 180, m_outdoor_vertical );
				break;

			case Enigma::Object::Direction::EAST:
				draw_tile(context, x, y, 90, m_outdoor_vertical );
				break;

			case Enigma::Object::Direction::WEST:
				draw_tile(context, x, y, 270, m_outdoor_vertical );
				break;

			case Enigma::Object::Direction::ABOVE:
				draw_tile(context, x, y, 0, m_outdoor_ceiling);
				break;

			case Enigma::Object::Direction::BELOW:
				draw_tile(context, x, y, 0, m_outdoor_floor);
				break;

			case Enigma::Object::Direction::CENTER:
				draw_tile(context, x, y, 0, m_outdoor);
				break;

			default:
				drawn = false;
				break;
		}
	}
	else if (object.m_id == Enigma::Object::ID::INDOOR)      
	{
		// Draw an Indoor environment image.
		
		switch (object.m_surface)
		{
			case Enigma::Object::Direction::NORTH:
				draw_tile(context, x, y, 0, m_indoor_vertical); 
				break;

			case Enigma::Object::Direction::SOUTH:
				draw_tile(context, x, y, 180, m_indoor_vertical);
				break;

			case Enigma::Object::Direction::EAST:
				draw_tile(context, x, y, 90, m_indoor_vertical);
				break;

			case Enigma::Object::Direction::WEST:
				draw_tile(context, x, y, 270, m_indoor_vertical);
				break;

			case Enigma::Object::Direction::ABOVE:
				draw_tile(context, x, y, 0, m_indoor_ceiling);
				break;

			case Enigma::Object::Direction::BELOW:
				draw_tile(context, x, y, 0, m_indoor_floor);
				break;

			case Enigma::Object::Direction::CENTER:
				draw_tile(context, x, y, 0, m_indoor);
				break;

			default:
				drawn = false;
				break;
		}
	}
	else if (object.m_id == Enigma::Object::ID::PERSON)
	{
		// Draw a Person image associated with a player.
		
		if (object.m_active)
			draw_tile(context, x, y, 0, m_player_active);
		else
			draw_tile(context, x, y, 0, m_player_idle);
	}
	else
		drawn = false;

	return drawn;
}

