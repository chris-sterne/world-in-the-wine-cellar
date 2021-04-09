/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 5.0 for Linux OS                    *
 * File:    TeleporterView.h                    *
 * Date:    September 13, 2015                  *
 * Author:  Chris Sterne                        *
 *                                              *
 * TeleporterView class header.                 *
 *----------------------------------------------*/
 
#ifndef __TELEPORTERVIEW_H__
#define __TELEPORTERVIEW_H__

#include <gtkmm.h>
#include "Map.h"

class CTeleporterView : public Gtk::ScrolledWindow
{
  public:
    // Public methods.
    
    CTeleporterView();
		void Update();
		void SetMap( std::shared_ptr<CMap> aMap );
		gboolean On_Key_Press( GdkEventKey* key_event );

    // Map location signal accessor.
		
    typedef sigc::signal<void, const CMapLocation&> type_signal_location;
    type_signal_location signal_location();

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
		
		void On_Cursor_Changed();
    void Do_Location( CMapLocation aLocation );
		
		// Private data.

		CObjectColumns iColumnRecord;
		
		// Private data.

		std::shared_ptr<CMap> iMap;                // Shared Game map.
		std::list<CMapObject> iBuffer;             // List of teleporter objects.
		std::unique_ptr<Gtk::TreeView> iTreeView;  // Room object list viewer.
		Glib::RefPtr<Gtk::ListStore> iListStore;   // Storage for data entries.
		type_signal_location m_signal_location;    // Location signal server.
};

#endif // __TELEPORTERVIEW_H__