// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the ObjectList class implementation.  The ObjectList class
// is a sorted list of map objects.  A cached iterator is used to improve
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

#include "ObjectList.h"

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::ObjectList::ObjectList()
{ 
	// Initialize the cached list iterator.

	m_iterator = end();
}

//-----------------------------
// This method clears the list.
//-----------------------------

void Enigma::ObjectList::clear()
{
	// Call parent class clear().

	std::list<Enigma::Object>::clear();
	m_iterator = end();
}

//-----------------------------------------------------------------
// This method moves the list iterator just past an object whose
// position (East, North, Above) matches the one provided.
// If none exist, the iterator will point past the end of the list.
// Using this method before inserting objects into the list will
// cause them to be sorted in an ascending order based on location.
//-----------------------------------------------------------------
// position: World position to seek.
//-----------------------------------------------------------------

void Enigma::ObjectList::seek(const Enigma::Position& position)
{
	// Move the list iterator if East, North, or Above are too high. 

	while ((m_iterator != begin())
		&& 
		  ((m_iterator == end())
		||
		  ((*m_iterator).m_position.m_above > position.m_above)
		||
		  (((*m_iterator).m_position.m_above == position.m_above)
		&& ((*m_iterator).m_position.m_north > position.m_north))
		|| 
		  (((*m_iterator).m_position.m_above == position.m_above)
		&& ((*m_iterator).m_position.m_north == position.m_north)
		&& ((*m_iterator).m_position.m_east > position.m_east))))
		{
			m_iterator --;
		}

	// Move the list iterator if East, North, or Above are too low
	// or equal.

	while ((m_iterator != end())
		&& (((*m_iterator).m_position.m_above < position.m_above)
		||
		  (((*m_iterator).m_position.m_above == position.m_above)
		&& ((*m_iterator).m_position.m_north < position.m_north))
		|| 
		  (( (*m_iterator).m_position.m_above == position.m_above)
		&& ((*m_iterator).m_position.m_north == position.m_north)
		&& ((*m_iterator).m_position.m_east <= position.m_east))))
		{
			m_iterator ++;
		}
}

//-----------------------------------------------------
// This method removes a list of objects.
//-----------------------------------------------------
// objects: List of iterators to objects to be removed.
// buffer:  Buffer to receive removed objects.         
//-----------------------------------------------------

void Enigma::ObjectList::remove(
	std::list<std::list<Enigma::Object>::iterator>& objects,
  std::list<Enigma::Object>& buffer)
{
  std::list<std::list<Enigma::Object>::iterator>::iterator object;

  for (object = objects.begin();
       object != objects.end();
       ++ object)
  {
    // If the cached iterator points to an object about to be removed,
    // move the iterator to the next object.  This keeps the cached
    // iterator valid.
  
    if (*object != end())
    {
      if (m_iterator == *object)
        ++ m_iterator;

      // Move the object to the end of the buffer.

      buffer.splice(buffer.end(), *this, *object);
    }
  }
}

//------------------------------------------
// This method erases an object in the list.
//------------------------------------------
// object: Iterator to object to be erased.
//------------------------------------------

void Enigma::ObjectList::erase(std::list<Enigma::Object>::iterator& object)
{
  // If the list is not empty, call the parent erase() method
  // to erase the object. 
  
  if (object != end())
    m_iterator = std::list<Enigma::Object>::erase(object);
}

//-----------------------------------------------------------------
// This method sorts an object by position and inserts a copy of it
// into the list.
//-----------------------------------------------------------------
// object: Object to be inserted.
//-----------------------------------------------------------------

void Enigma::ObjectList::insert(Enigma::Object& object)
{ 
	seek(object.m_position);
	
	// The list iterator has been moved to the insertion point.  Call
	// the parent insert() method to insert a copy of the object just
	// before the insertion point.
	
	m_iterator = std::list<Enigma::Object>::insert(m_iterator, object);
}

//--------------------------------------------------------------
// This method sorts and inserts copies of objects from a buffer
// into the list.
//--------------------------------------------------------------
// buffer: List of world objects to be inserted.
//--------------------------------------------------------------

void Enigma::ObjectList::insert(std::list<Enigma::Object>& buffer)
{ 
	std::list<Enigma::Object>::iterator object;

	for (object = buffer.begin();
	     object != buffer.end();
	     ++ object)
	{
		insert((*object));
	}
}

//-------------------------------------------------------------------
// This method copies object iterators within a world volume into
// a buffer.  The objects in the buffer will most likely be unsorted.
//-------------------------------------------------------------------
// volume: World volume to be copied.
// buffer: Buffer to receive copies of object iterators.
//-------------------------------------------------------------------

void Enigma::ObjectList::read(
	Enigma::Volume& volume,
  std::list<std::list<Enigma::Object>::iterator>& buffer)
{
	Enigma::Position position;

	for (position.m_above = volume.m_WSB.m_above;
	     position.m_above <= volume.m_ENA.m_above;
	     ++ position.m_above )
	{
		for (position.m_north = volume.m_WSB.m_north;
		     position.m_north <= volume.m_ENA.m_north;
		     ++ position.m_north )
		{
			for (position.m_east = volume.m_WSB.m_east;
			     position.m_east <= volume.m_ENA.m_east;
			     ++ position.m_east )
			{
				read(position, buffer);
			}
		}
	}
}

//---------------------------------------------------------------------------
// This method copies object iterators within a world position into a buffer.
//---------------------------------------------------------------------------
// position: World position to be copied.
// buffer:   Buffer to receive copies of object iterators.
//---------------------------------------------------------------------------

void Enigma::ObjectList::read(
	Enigma::Position& position,
	std::list<std::list<Enigma::Object>::iterator>& buffer)
{
	seek(position);

	while (m_iterator != begin())
	{
		// Since the list iterator following a seek will be on the object
		// just past a desired object (or past the end of the list),
		// move the iterator back.

		m_iterator --;

		if  (((*m_iterator).m_position.m_above == position.m_above)
		  && ((*m_iterator).m_position.m_north == position.m_north)
		  && ((*m_iterator).m_position.m_east == position.m_east))
		{
			// An object is present with the correct location.
			// Copy this object into the provided buffer.

			buffer.push_back(m_iterator);
		}
		else
			break;
	}
}

//----------------------------------------------------------------
// This method copies objects from a world position into a buffer.
//----------------------------------------------------------------
// position: World position to be copied.
// buffer:   Buffer to receive copies of objects.
//----------------------------------------------------------------

void Enigma::ObjectList::copy(Enigma::Position& position,
                              std::list<Enigma::Object>& buffer)
{
	seek(position);

	while (m_iterator != begin())
	{
		// Since the list iterator following a seek will be on the object
		// just past a desired object (or past the end of the list),
		// move the iterator back.

		m_iterator --;

		if  (((*m_iterator).m_position.m_above == position.m_above)
		  && ((*m_iterator).m_position.m_north == position.m_north)
		  && ((*m_iterator).m_position.m_east == position.m_east))
		{
			// An object is present with the correct location.
			// Copy this object into the provided buffer.

			buffer.push_back(*m_iterator);
		}
		else
			break;
	}
}

//----------------------------------------------------------------
// This method copies objects within a world volume into a buffer.
//  The objects in the buffer will most likely be unsorted.
//----------------------------------------------------------------
// volume: World volume to be copied.
// buffer: Buffer to receive copies of objects.
//----------------------------------------------------------------

void Enigma::ObjectList::copy(Enigma::Volume& volume,
                             std::list<Enigma::Object>& buffer)
{
	Enigma::Position position;

	for (position.m_above = volume.m_WSB.m_above;
	     position.m_above <= volume.m_ENA.m_above;
	     ++ position.m_above)
	{
		for (position.m_north = volume.m_WSB.m_north;
		     position.m_north <= volume.m_ENA.m_north;
		     ++ position.m_north)
		{
			for (position.m_east = volume.m_WSB.m_east;
			     position.m_east <= volume.m_ENA.m_east;
			     ++ position.m_east)
			{
				copy(position, buffer);
			}
		}
	}
}
