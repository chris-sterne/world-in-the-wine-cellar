// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Tiles class header.  The Tiles class draws object image
// tiles in a game world level viewer.
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
 
#ifndef __TILES_H__
#define __TILES_H__

#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <gtkmm/widget.h>
#include "Object.h"
#include "Volume.h"

namespace Enigma
{
	class Tiles
	{
		public:
			// Public methods.

			Tiles();
			int get_tile_size() const;

			bool draw_object(const Cairo::RefPtr<Cairo::Context>& context,
			                 Gtk::Allocation allocation,
			                 unsigned short column,
			                 unsigned short row,
			                 Enigma::Object& object);

			void draw_arrival(const Cairo::RefPtr<Cairo::Context>& context,
			                  Gtk::Allocation allocation,                      
			                  unsigned short column,
			                  unsigned short row );

			void draw_generic(const Cairo::RefPtr<Cairo::Context>& context,
			                  Gtk::Allocation allocation,                      
			                  unsigned short column,
			                  unsigned short row);

			void draw_cursor(const Cairo::RefPtr<Cairo::Context>& context,
			                 Gtk::Allocation allocation,
			                 unsigned short column,
			                 unsigned short row);

			void draw_marker(const Cairo::RefPtr<Cairo::Context>& context,
			                 Gtk::Allocation allocation,
			                 unsigned short column,
			                 unsigned short row);

		// Public data.

		Cairo::RefPtr<Cairo::ImageSurface> m_cursor;
		Cairo::RefPtr<Cairo::ImageSurface> m_marker;
		Cairo::RefPtr<Cairo::ImageSurface> m_generic;
		Cairo::RefPtr<Cairo::ImageSurface> m_wall;
		Cairo::RefPtr<Cairo::ImageSurface> m_ceiling;
		Cairo::RefPtr<Cairo::ImageSurface> m_floor;
		Cairo::RefPtr<Cairo::ImageSurface> m_ladder;
		Cairo::RefPtr<Cairo::ImageSurface> m_ladderend;
		Cairo::RefPtr<Cairo::ImageSurface> m_ladderend_horizontal;
		Cairo::RefPtr<Cairo::ImageSurface> m_ladderend_ceiling;
		Cairo::RefPtr<Cairo::ImageSurface> m_ladderend_floor;
		Cairo::RefPtr<Cairo::ImageSurface> m_ladder_horizontal;
		Cairo::RefPtr<Cairo::ImageSurface> m_ladder_ceiling;
		Cairo::RefPtr<Cairo::ImageSurface> m_ladder_floor;
		Cairo::RefPtr<Cairo::ImageSurface> m_player_active;
		Cairo::RefPtr<Cairo::ImageSurface> m_player_idle;
		Cairo::RefPtr<Cairo::ImageSurface> m_blocker;
		Cairo::RefPtr<Cairo::ImageSurface> m_blocker_horizontal;
		Cairo::RefPtr<Cairo::ImageSurface> m_mover;
		Cairo::RefPtr<Cairo::ImageSurface> m_mover_above;
		Cairo::RefPtr<Cairo::ImageSurface> m_mover_below;
		Cairo::RefPtr<Cairo::ImageSurface> m_turner;
		Cairo::RefPtr<Cairo::ImageSurface> m_item;
		Cairo::RefPtr<Cairo::ImageSurface> m_surfacer;
		Cairo::RefPtr<Cairo::ImageSurface> m_corner_surfacer;
		Cairo::RefPtr<Cairo::ImageSurface> m_flipper;
		Cairo::RefPtr<Cairo::ImageSurface> m_stairs;
		Cairo::RefPtr<Cairo::ImageSurface> m_stairstop;
		Cairo::RefPtr<Cairo::ImageSurface> m_handhold;
		Cairo::RefPtr<Cairo::ImageSurface> m_handhold_horizontal;
		Cairo::RefPtr<Cairo::ImageSurface> m_outdoor;
		Cairo::RefPtr<Cairo::ImageSurface> m_outdoor_vertical;
		Cairo::RefPtr<Cairo::ImageSurface> m_outdoor_ceiling;
		Cairo::RefPtr<Cairo::ImageSurface> m_outdoor_floor;
		Cairo::RefPtr<Cairo::ImageSurface> m_indoor;
		Cairo::RefPtr<Cairo::ImageSurface> m_indoor_vertical;
		Cairo::RefPtr<Cairo::ImageSurface> m_indoor_ceiling;
		Cairo::RefPtr<Cairo::ImageSurface> m_indoor_floor;
		Cairo::RefPtr<Cairo::ImageSurface> m_archway_vertical;
		Cairo::RefPtr<Cairo::ImageSurface> m_archway_ceiling;
		Cairo::RefPtr<Cairo::ImageSurface> m_archway_floor;
		Cairo::RefPtr<Cairo::ImageSurface> m_woodDoor_vertical;
		Cairo::RefPtr<Cairo::ImageSurface> m_wooddoor_ceiling;
		Cairo::RefPtr<Cairo::ImageSurface> m_Wooddoor_floor;
		Cairo::RefPtr<Cairo::ImageSurface> m_woodwall_vertical;
		Cairo::RefPtr<Cairo::ImageSurface> m_woodwall_ceiling;
		Cairo::RefPtr<Cairo::ImageSurface> m_woodwall_floor;
		Cairo::RefPtr<Cairo::ImageSurface> m_pullring_vertical;
		Cairo::RefPtr<Cairo::ImageSurface> m_pullring_horizontal;
		Cairo::RefPtr<Cairo::ImageSurface> m_lock_vertical;
		Cairo::RefPtr<Cairo::ImageSurface> m_lock_horizontal;
		Cairo::RefPtr<Cairo::ImageSurface> m_waterlayer_vertical;
		Cairo::RefPtr<Cairo::ImageSurface> m_waterlayer_below;
		Cairo::RefPtr<Cairo::ImageSurface> m_waterlayer_above;
		Cairo::RefPtr<Cairo::ImageSurface> m_lightbeam_horizontal;
		Cairo::RefPtr<Cairo::ImageSurface> m_lightbeam_vertical;   
		Cairo::RefPtr<Cairo::ImageSurface> m_tree;
		Cairo::RefPtr<Cairo::ImageSurface> m_tree_horizontal;
		Cairo::RefPtr<Cairo::ImageSurface> m_tree_ceiling;
		Cairo::RefPtr<Cairo::ImageSurface> m_tree_floor;
		Cairo::RefPtr<Cairo::ImageSurface> m_treetop;
		Cairo::RefPtr<Cairo::ImageSurface> m_treetop_horizontal;
		Cairo::RefPtr<Cairo::ImageSurface> m_treetop_ceiling;
		Cairo::RefPtr<Cairo::ImageSurface> m_treetop_floor; 
		Cairo::RefPtr<Cairo::ImageSurface> m_catwalkCeiling;
		Cairo::RefPtr<Cairo::ImageSurface> m_catwalkFloor;
		Cairo::RefPtr<Cairo::ImageSurface> m_catwalk_horizontal;
		Cairo::RefPtr<Cairo::ImageSurface> m_catwalk_vertical;
		Cairo::RefPtr<Cairo::ImageSurface> m_teleporter_departure;
		Cairo::RefPtr<Cairo::ImageSurface> m_teleporter_arrival;
		Cairo::RefPtr<Cairo::ImageSurface> m_fern_floor;
		Cairo::RefPtr<Cairo::ImageSurface> m_water;
		Cairo::RefPtr<Cairo::ImageSurface> m_earthwall_vertical;
		Cairo::RefPtr<Cairo::ImageSurface> m_earthwall_ceiling;
		Cairo::RefPtr<Cairo::ImageSurface> m_earthwall_floor;
		Cairo::RefPtr<Cairo::ImageSurface> m_padbutton_vertical;
		Cairo::RefPtr<Cairo::ImageSurface> m_padbutton_ceiling;
		Cairo::RefPtr<Cairo::ImageSurface> m_padbutton_floor;   
	};

#endif // __IMAGETILES_H__
