/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 2.0 for Linux OS                    *
 * File:    RoomView.h                          *
 * Date:    December 28, 2015                   *
 * Author:  Chris Sterne                        *
 *                                              *
 * RoomView class header.                       *
 *----------------------------------------------*/
 
#ifndef __ROOMVIEW_H__
#define __ROOMVIEW_H__

#include <gtkmm.h>
#include "Map.h"

class CRoomView : public Gtk::ScrolledWindow
{
  public:
    // Public methods.
    
    CRoomView();
		void Update();
		void SetMap( std::shared_ptr<CMap> aMap );
		void SetLocation( const CMapLocation& aLocation );
		gboolean On_Key_Press( GdkEventKey* key_event );

	protected:
		// Overridden base class methods.

		void on_map();
		
	private:
		// Private classes.
		
		class CObjectColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:
        Gtk::TreeModelColumn<std::list<CMapObject>::iterator> iIterator;
        
        CObjectColumns()
				{
					add( iIterator );
				}
		};

		// Private methods.

		void Object_Data_Function( Gtk::CellRenderer* const& aCellRenderer,
															 const Gtk::TreeIter& aTreeIterator );
		
		// Private data.

		CObjectColumns iColumnRecord;
		
		// Private data.

		std::shared_ptr<CMap> iMap;                  // Shared Game map.
		CMapLocation iLocation;                      // Location of room.
		std::unique_ptr<Gtk::TreeView> iTreeView;    // Room object list viewer.
		Glib::RefPtr<Gtk::ListStore> iListStore;     // Storage for data entries.
};

#endif // __ROOMVIEW_H__