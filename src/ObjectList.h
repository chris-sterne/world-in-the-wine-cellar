/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 2.0 for Linux OS                    *
 * File:    ObjectList.h                        *
 * Date:    February 10, 2016                   *
 * Author:  Chris Sterne                        *
 *                                              *
 * ObjectList class header.                     *
 *----------------------------------------------*/
 
#ifndef __OBJECTLIST_H__
#define __OBJECTLIST_H__

#include <gtkmm.h>
#include "MapVolume.h"
#include "MapObject.h"

class CObjectList : public std::list<CMapObject>
{
  public:
    // Public methods.

    CObjectList();
    void Clear();
    void Insert( CMapObject& aObject );
    void Insert( std::list<CMapObject>& aBuffer );
    
    void Remove( std::list<std::list<CMapObject>::iterator>& aObjects,
                 std::list<CMapObject>& aBuffer );

    void Erase( std::list<CMapObject>::iterator& aObject );

    void Read( CMapLocation& aLocation,
               std::list<std::list<CMapObject>::iterator>& aBuffer );

    void Read( CMapVolume& aVolume,
               std::list<std::list<CMapObject>::iterator>& aBuffer );

    void Copy( CMapLocation& aVolume, std::list<CMapObject>& aBuffer );
    void Copy( CMapVolume& aVolume, std::list<CMapObject>& aBuffer );

    // Private methods.

    void Seek( const CMapLocation& aLocation );
		
    // Private data.

    std::list<CMapObject>::iterator iIterator;    // Cached list iterator.
};

#endif // __OBJECTLIST_H__