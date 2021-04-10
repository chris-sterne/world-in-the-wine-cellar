// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file contains the Volume class header.  The Volume class describes
// a volume of positions in a game world. 
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
 
#ifndef __VOLUME_H__
#define __VOLUME_H__

#include <gtkmm.h>
#include "Position.h"

namespace Enigma
{
	class Volume
	{
		public:
			// Public data.

			Enigma::Position m_WSB;  // West-South-Below (lowest) corner of volume.
			Enigma::Position m_ENA;  // East-North-Above (highest) corner of volume.
	};
}

#endif // __VOLUME_H__
