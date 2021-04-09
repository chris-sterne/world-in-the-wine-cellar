/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 4.0 for Linux OS                    *
 * File:    PlayerView.h                        *
 * Date:    September 8, 2015                   *
 * Author:  Chris Sterne                        *
 *                                              *
 * PlayerView class header.                     *
 *----------------------------------------------*/
 
#ifndef __PLAYERVIEW_H__
#define __PLAYERVIEW_H__

#include <gtkmm.h>
#include "Map.h"

class CPlayerView : public Gtk::ScrolledWindow
{
  public:
    // Public methods.
    
    CPlayerView();
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
		std::list<CMapObject> iBuffer;             // List of all player objects.
		std::unique_ptr<Gtk::TreeView> iTreeView;  // Room object list viewer.
		Glib::RefPtr<Gtk::ListStore> iListStore;   // Storage for data entries.
		type_signal_location m_signal_location;    // Player location signal server.
};

#endif // __ITEMVIEW_H__