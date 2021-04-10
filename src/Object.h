// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Object class implementation.  The object class describes
// objects in the game world.
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
 
#ifndef __OBJECT_H__
#define __OBJECT_H__

//#include "Position.h"

namespace Enigma
{
	class Object
	{
		/*public:
			enum class Type         // Object type.
			{
				OBJECT = 0,
				ITEM,
				PLAYER,
				TELEPORTER
			};
		
			enum class ID           // Object ID values.
			{
				NONE = 0,
				WINECELLAR,
				BLOCKWALL,
				STONEWALL,
				LADDER,
				LADDEREND,
				PERSON,
				BLOCKER,
				MOVER,
				TURNER,
				FACER,
				SURFACER,
				MUSHROOMS,
				CORKSCREW,
				WINEBOTTLE,
				BREAD,
				CHEESE,
				KNIFE,
				APPLE,	
				WINEGLASS,
				PLACEMAT,
				GRAPES,
				SKULL,
				EDGEDRAIN,
				AIRVENT,
				EASTEREGG,
				FLIPPER,
				EDGER,
				STAIRS,
				STAIRSTOP,
				SENSOR,
				HANDHOLD,
				MOSS0,
				MOSS90,
				MOSS180,
				MOSS270,
				MOSS360,
				AIRBALLOON,
				PLATE,
				FORK,
				CAKE,
				STONEPOST,
				STONEBUTTON,
				WALLEYES,
				OUTDOOR,
				INDOOR,
				SKYOBJECTS,
				ARCHWAY,
				WOODDOOR,
				WOODWALL,
				PULLRING,
				CUBELOCK,
				SPHERELOCK,
				CUBE,
				SPHERE,
				WATERLAYER,
				LIGHBEAM,
				EGG,
				WEB,
				ORANGE,
				FISH,
				WATERBOTTLE,
				FERTILIZER,
				TREE,
				TREETOP,
				CATWALK,
				TELEPORTER,
				INNERTUBE,
				HYDROGENBALLOON,
				WEED,
				FERN,
				VINE,
				WATERLOCK,
				APPLELOCK,
				ORANGELOCK,
				WINELOCK,
				PADBUTTON,
				FENCE,
				EARTHWALL,
				WATER,
				TOTAL
			};

			enum class Direction    // Directions in the world.
			{
				NONE = 0,
				NORTH,
				SOUTH,
				EAST,
				WEST,
				ABOVE,
				BELOW,
				CENTER,
				TOTAL
			};
			
			enum class Category   // Item object categories.
			{
				NONE = 0,
				REQUIRED,           // Item is required.
				OPTIONAL,           // Item is optional.
				EASTEREGG,          // Item is a secret bonus.
				SKULL,              // Item indicates player is trapped.
				TOTAL
			};

			// Public methods.

			void get_description(Glib::ustring& description);

			// Public data.

			Enigma::Object::Type m_type;             // Type of object.
			Enigma::Object::ID m_id;                 // Object ID.
			Enigma::Position m_position;             // Position in the world.
			Enigma::Object::Direction m_surface;     // Surface containing object.
			Enigma::Object::Direction m_rotation;    // Rotation of object on surface.

			// Player-specific data.

			bool m_active;                           // TRUE if active.
			
			// Item-specific data. 
			
			Enigma::Object::Category m_category;     // Category.
			
			// Teleporter-specific data.
			
			Enigma::Object::Direction m_surface_arrival;   // Arrival surface.
			Enigma::Object::Direction m_rotation_arrival;  // Arrival rotation.
			Enigma::Position m_position_arrival;           // Arrival position.

			// Connection signal names.

			std::string m_sense;          // Sense input.
			std::string m_state;          // Active/idle visual state.
			std::string m_visibility;     // TRUE if visible.
			std::string m_presence;       // TRUE if present and functional.*/
	};
}

#endif // __OBJECT_H__
