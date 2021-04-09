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

#include <gtkmm.h>
#include "ObjectList.h"
#include "Controller.h"

namespace Enigma
{
	class World : public sigc::trackable
	{
		public:
			// Public methods.

			World();
			void clear();
			void load();
			void save();

			// Public data.
		
			Glib::ustring m_filename;                 // World filename.
			Enigma::ObjectList m_objects;             // Sorted object list.
			Enigma::ObjectList m_players;             // Sorted player list.
			Enigma::ObjectList m_items;               // Sorted item list.
			Enigma::ObjectList m_teleporters;         // Sorted teleporter list.
			Glib::ustring m_description;              // Description of game world.
			bool m_savable;                           // TRUE if game can be saved.

			// List of logic controllers.

			std::list<Enigma::Controller> m_controllers;

		private:
			// Private methods.

			void extract_world(std::string& filedata);
	};
}

#endif // __WORLD_H__
