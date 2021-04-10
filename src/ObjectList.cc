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

//*----------------------*
//* Default constructor. *
//*----------------------*

CObjectList::CObjectList()
{ 
	// Initialize the list iterator.

	iIterator = end();
	return;
}

//*------------------------------*
//* This method clears the list. *
//*------------------------------*

void CObjectList::Clear()
{
	clear();
	iIterator = end();
	return;
}

//*------------------------------------------------------------------*
//* This method moves the list iterator just past an object whose    *
//* location (East, North, Above) matches the one provided.          *
//* If none exist, the iterator will point past the end of the list. *
//* Using this method before inserting objects into the list will    *
//* cause them to be sorted in an ascending order based on location. *
//*------------------------------------------------------------------*
//* aLocation: Map location to seek.                                 *
//*------------------------------------------------------------------*

void CObjectList::Seek( const CMapLocation& aLocation )
{
	// Move the list iterator if East, North, or Above are too high. 
	
	while (( iIterator != begin() )
	  && 
			(( iIterator == end() )
		||
		  ( (*iIterator).iLocation.iAbove > aLocation.iAbove )
		||
			(( (*iIterator).iLocation.iAbove == aLocation.iAbove )
		&& ( (*iIterator).iLocation.iNorth > aLocation.iNorth ))
		|| 
			(( (*iIterator).iLocation.iAbove == aLocation.iAbove )
		&& ( (*iIterator).iLocation.iNorth == aLocation.iNorth )
		&& ( (*iIterator).iLocation.iEast > aLocation.iEast ))))
		{
			iIterator --;
		}

	// Move the list iterator if East, North, or Above are too low
	// or equal.
	
	while (( iIterator != end() )
		&& (( (*iIterator).iLocation.iAbove < aLocation.iAbove )
		||
			(( (*iIterator).iLocation.iAbove == aLocation.iAbove )
		&& ( (*iIterator).iLocation.iNorth < aLocation.iNorth ))
		|| 
			(( (*iIterator).iLocation.iAbove == aLocation.iAbove )
		&& ( (*iIterator).iLocation.iNorth == aLocation.iNorth )
		&& ( (*iIterator).iLocation.iEast <= aLocation.iEast ))))
		{
			iIterator ++;
		}
	
	return;
}

//*-------------------------------------------------------*
//* This method removes a list of MapObjects.             *
//*-------------------------------------------------------*
//* aObjects: List of iterators to objects to be removed. *
//* aBuffer:  Buffer to receive removed objects.          *
//*-------------------------------------------------------*

void CObjectList::Remove( std::list<std::list<CMapObject>::iterator>& aObjects,
                          std::list<CMapObject>& aBuffer )
{
  std::list<std::list<CMapObject>::iterator>::iterator Object;

  for ( Object = aObjects.begin(); Object != aObjects.end(); ++ Object )
  {
    // If the cached iterator points to an object about to be removed,
    // move the iterator to the next object.  This keeps the cached
    // iterator valid.
  
    if ( *Object != end() )
    {
      if ( iIterator == *Object )
        ++ iIterator;

      // Move the MapObject to the end of the buffer.

      aBuffer.splice( aBuffer.end(), *this, *Object );
    }
  }

	return;
}

//*---------------------------------------------*
//* This method erases a MapObject in the list. *
//*---------------------------------------------*
//* aObjects: Iterator to object to be erased.  *
//*---------------------------------------------*

void CObjectList::Erase( std::list<CMapObject>::iterator& aObject )
{
  if ( aObject != end() )
    iIterator = erase( aObject );

  return;
}

//*--------------------------------------------------------------------*
//* This method sorts a MapObject by location and inserts a copy of it *
//* into the list.                                                     * 
//*--------------------------------------------------------------------*
//* aObject: MapObject to be inserted.                                 *
//*--------------------------------------------------------------------*

void CObjectList::Insert( CMapObject& aObject )
{ 
	Seek( aObject.iLocation );
	
	// The list iterator has been moved to the insertion point.
	// Insert a copy of the object just before the insertion point.
	
	iIterator = insert( iIterator, aObject );
	return;
}

//*------------------------------------------------------------------*
//* This method sorts and inserts copies of MapObjects from a buffer *
//* into the list.                                                   * 
//*------------------------------------------------------------------*
//* aBuffer: List of MapObject to be inserted.                       *
//*------------------------------------------------------------------*

void CObjectList::Insert( std::list<CMapObject>& aBuffer )
{ 
	std::list<CMapObject>::iterator Object;

	for ( Object = aBuffer.begin(); Object != aBuffer.end(); ++ Object )
	{
		Insert( (*Object) );
	}

	return;
}

//*-----------------------------------------------------------------*
//* This method copies MapObject iterators within a map volume into *
//* a buffer.  The MapObjects in the buffer will most likely be     *
//* unsorted.                                                       *
//*-----------------------------------------------------------------*
//* aVolume: Map volume to be copied.                               *
//* aBuffer: Buffer to receive copies of MapObject iterators.       *
//*-----------------------------------------------------------------*

void CObjectList::Read( CMapVolume& aVolume,
                        std::list<std::list<CMapObject>::iterator>& aBuffer )
{
	CMapLocation Location;
	
	for ( Location.iAbove = aVolume.iWSB.iAbove;
	     Location.iAbove <= aVolume.iENA.iAbove;
	     ++ Location.iAbove )
	{
		for ( Location.iNorth = aVolume.iWSB.iNorth;
					Location.iNorth <= aVolume.iENA.iNorth;
					++ Location.iNorth )
		{
			for ( Location.iEast = aVolume.iWSB.iEast;
	  			  Location.iEast <= aVolume.iENA.iEast;
			      ++ Location.iEast )
			{
				Read( Location, aBuffer );
			}
		}
	}
	     
	return;
}

//*---------------------------------------------------------------------------*
//* This method copies MapObject iterators from a map location into a buffer. *
//*---------------------------------------------------------------------------*
//* aLocation: Map volume to be copied.                                       *
//* aBuffer:   Buffer to receive copies of MapObject iterators.               *
//*---------------------------------------------------------------------------*

void CObjectList::Read( CMapLocation& aLocation,
                        std::list<std::list<CMapObject>::iterator>& aBuffer )
{
	Seek( aLocation );

	while ( iIterator != begin() )
	{
		// Since the list iterator following a seek will be on the object
		// just past a desired object (or past the end of the list),
		// move the iterator back.

		iIterator --;

		if  (((*iIterator).iLocation.iAbove == aLocation.iAbove )
			&& ((*iIterator).iLocation.iNorth == aLocation.iNorth )
			&& ((*iIterator).iLocation.iEast == aLocation.iEast ))
		{
			// An object is present with the correct location.
			// Copy this object into the provided buffer.
			
			aBuffer.push_back( iIterator );
		}
		else
			break;
	}
	
	return;
}

//*-----------------------------------------------------------------*
//* This method copies MapObject from a map location into a buffer. *
//*-----------------------------------------------------------------*
//* aLocation: Map volume to be copied.                             *
//* aBuffer:   Buffer to receive copies of MapObject.               *
//*-----------------------------------------------------------------*

void CObjectList::Copy( CMapLocation& aLocation,
                        std::list<CMapObject>& aBuffer )
{
	Seek( aLocation );

	while ( iIterator != begin() )
	{
		// Since the list iterator following a seek will be on the object
		// just past a desired object (or past the end of the list),
		// move the iterator back.

		iIterator --;

		if  (((*iIterator).iLocation.iAbove == aLocation.iAbove )
			&& ((*iIterator).iLocation.iNorth == aLocation.iNorth )
			&& ((*iIterator).iLocation.iEast == aLocation.iEast ))
		{
			// An object is present with the correct location.
			// Copy this object into the provided buffer.
			
			aBuffer.push_back( *iIterator );
		}
		else
			break;
	}
	
	return;
}

//*----------------------------------------------------------*
//* This method copies MapObjects within a map volume into   *
//* a buffer.  The MapObjects in the buffer will most likely *
//* be unsorted.                                             *
//*----------------------------------------------------------*
//* aVolume: Map volume to be copied.                        *
//* aBuffer: Buffer to receive copies of MapObjects.         *
//*----------------------------------------------------------*

void CObjectList::Copy( CMapVolume& aVolume,
                        std::list<CMapObject>& aBuffer )
{
	CMapLocation Location;
	
	for ( Location.iAbove = aVolume.iWSB.iAbove;
	     Location.iAbove <= aVolume.iENA.iAbove;
	     ++ Location.iAbove )
	{
		for ( Location.iNorth = aVolume.iWSB.iNorth;
					Location.iNorth <= aVolume.iENA.iNorth;
					++ Location.iNorth )
		{
			for ( Location.iEast = aVolume.iWSB.iEast;
	  			  Location.iEast <= aVolume.iENA.iEast;
			      ++ Location.iEast )
			{
	      Copy( Location, aBuffer );
			}
		}
	}
	     
	return;
}
