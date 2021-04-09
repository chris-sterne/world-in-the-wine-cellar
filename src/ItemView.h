/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 2.0 for Linux OS                    *
 * File:    ItemView.h                          *
 * Date:    March 20, 2016                      *
 * Author:  Chris Sterne                        *
 *                                              *
 * ItemView class header.                       *
 *----------------------------------------------*/
 
#ifndef __ITEMVIEW_H__
#define __ITEMVIEW_H__

#include <gtkmm.h>
#include "Map.h"

class CItemView : public Gtk::ScrolledWindow
{
  public:
    // Public methods.
    
    CItemView();
    void Update();
    void SetMap( std::shared_ptr<CMap> aMap );

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
    
    gboolean On_Key_Press( GdkEventKey* key_event );
    void On_Cursor_Changed();
    void Do_Location( CMapLocation aLocation );
    
    void Object_Data_Function( Gtk::CellRenderer* const& aCellRenderer,
                               const Gtk::TreeIter& aTreeIterator );

    void On_Row_Activated( const Gtk::TreeModel::Path& aPath,
                           Gtk::TreeViewColumn* aColumn );
		
    // Private data.

    CObjectColumns iColumnRecord;
		
    // Private data.

    std::shared_ptr<CMap> iMap;                  // Shared Game map.
    std::unique_ptr<Gtk::TreeView> iTreeView;    // Room object list viewer.
    Glib::RefPtr<Gtk::ListStore> iListStore;     // Storage for data entries.
    type_signal_location m_signal_location;      // Item location signal server.
};

#endif // __ITEMVIEW_H__