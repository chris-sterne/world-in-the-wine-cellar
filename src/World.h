// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file contains the World class header.  The World class describes
// a game world. 
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

#ifndef __WORLD_H__
#define __WORLD_H__

#include "ObjectList.h"
#include "Controller.h"

namespace Enigma
{
	class World
	{
		public:
		  // Public declarations.
		  
		  enum class Key       // World file key definitions.
			{
				NONE = 0,
				END,               // Introduces the end of keyvalue array.		
				OBJECT,            // Introduces a simple object.
				PLAYER,            // Introduces a player object.
				ITEM,              // Introduces an item object.
				DESCRIPTION,       // Introduces a description of the map.
				SURFACE,           // Surface on which object is located.
				ROTATION,          // Rotation of object on surface.
				ABOVE,             // Above location of object.
				NORTH,             // North location of object.
				EAST,              // East location of object.
				BANK,              // Adds high byte onto previous key's value.
				CATEGORY,          // Category of item.
				OWNER,             // Owner of item.
				ACTIVE,            // Active player or item.
				LENGTH,            // Length of a data section.
				DATA,              // Indicates the start of data section.
				SAVED,             // Saved value.
				RESTART,           // Restart value.
				CURRENT,           // Current value.    
				CONTROLLER,        // Introduces a map controller.
				SIGNAL,            // Map controller signal name block.
				CODE,              // Map controller bytecode block.
				SENSE,             // Sensor state or signal.
				STATE,             // Functional state or signal.
				VISIBILITY,        // Visibility state or signal.
				PRESENCE,          // Presence state or signal.
				OUTDOOR,           // Player's environment.
				SELECTED,          // Selected state of item.
				USED,              // Used state of item.
				TELEPORTER,        // Introduces a teleporter object.
				ARRIVAL,           // Teleporter arrival value.
				TOTAL
			};
			
			// Public methods.

			World();
			void clear();
			void load();
			void save();

			// Public data.
		
			Glib::ustring m_filename;            // World filename.
			Enigma::ObjectList m_objects;        // Sorted object list.
			Enigma::ObjectList m_players;        // Sorted player list.
			Enigma::ObjectList m_items;          // Sorted item list.
			Enigma::ObjectList m_teleporters;    // Sorted teleporter list.
			Glib::ustring m_description;         // Description of game world.
			bool m_savable;                      // TRUE if game can be saved.

			// List of logic controllers.

			std::list<Enigma::Controller> m_controllers;
	};
}

#endif // __WORLD_H__
