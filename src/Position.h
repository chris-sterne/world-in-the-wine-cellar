// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file contains the Position class header.  The Position class describes
// a position in the game world. 
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
 
#ifndef __POSITION_H__
#define __POSITION_H__

namespace Enigma
{
	class Position
	{
		public:
			// Public declarations.
			
			static const unsigned short MAXIMUM = 65535;  // Maximum position value.
			static const unsigned short MINIMUM = 0;      // Minimum position value.
			
			// Public data.

			unsigned short m_above;
			unsigned short m_north;
			unsigned short m_east;
	};
}

#endif // __POSITION_H__
