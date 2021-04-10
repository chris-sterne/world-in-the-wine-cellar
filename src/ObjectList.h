// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the ObjectList class header.  The ObjectList class is
// a sorted list of map objects.  A cached iterator is used to improve
// searches relative to the last accessed position.
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
 
#ifndef __OBJECTLIST_H__
#define __OBJECTLIST_H__

//#include <list>
//#include "Volume.h"
//#include "Object.h"

namespace Enigma
{
	class ObjectList : public std::list<Enigma::Object>
	{
		/*public:
			// Public methods.

			ObjectList();
			void clear() override;
			void insert(Enigma::Object& object);
			void insert(std::list<Enigma::Object>& buffer);

			void remove(std::list<std::list<Enigma::Object>::iterator>& objects,
			            std::list<Enigma::Object>& buffer);

			void erase(std::list<Enigma::Object>::iterator& object);

			void read(Enigma::Position& location,
			          std::list<std::list<Enigma::Object>::iterator>& buffer);

			void read(Enigma::Volume& volume,
			          std::list<std::list<Enigma::Object>::iterator>& buffer);

			void copy(Enigma::Position& volume, std::list<Enigma::Object>& buffer );
			void copy(Enigma::Volume& volume, std::list<Enigma::Object>& buffer );

		private:
			// Private methods.

			void seek(const Enigma::Position& position);

			// Private data.

			std::list<Enigma::Object>::iterator m_iterator;    // Cached list iterator.*/
	};
}

#endif // __OBJECTLIST_H__
