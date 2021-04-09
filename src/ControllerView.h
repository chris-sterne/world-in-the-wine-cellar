/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 4.0 for Linux OS                    *
 * File:    ControllerView.h                    *
 * Date:    September 7, 2016                   *
 * Author:  Chris Sterne                        *
 *                                              *
 * ControllerView class header.                 *
 *----------------------------------------------*/
 
#ifndef __CONTROLLERVIEW_H__
#define __CONTROLLERVIEW_H__

#include <gtkmm.h>
#include "Map.h"

class CControllerView : public Gtk::Notebook
{
  public:
    // Public methods.
    
    CControllerView();
    void Update();
    void Reset();
    void Do_Name( const std::string& aString );
    void SetMap( std::shared_ptr<CMap> aMap );
    gboolean On_List_Key_Press( GdkEventKey* key_event );
    gboolean On_Editor_Key_Press( GdkEventKey* key_event );

    void On_List_Row_Activated( const Gtk::TreeModel::Path& aPath,
                                Gtk::TreeViewColumn* aColumn );

    // Controller name signal accessor.
		
    typedef sigc::signal<void, const std::string&> type_signal_name;
    type_signal_name signal_name();

  protected:
    // Overridden base class methods.

    void on_map();

  private:
    // Private classes.
		
    class CColumn : public Gtk::TreeModel::ColumnRecord
    {
      public:
        Gtk::TreeModelColumn<std::list<CMapController>::iterator> iIterator;
				
        CColumn()
        { 
          add( iIterator );
        }
    };

    // Private methods.

    void Data_Function( Gtk::CellRenderer* const& aCellRenderer,
                        const Gtk::TreeIter& aTreeIterator );
		
    // Private data.

    CColumn iColumnRecord;
		
    // Private data.

    std::shared_ptr<CMap> iMap;                             // Shared Game map.
    std::unique_ptr<Gtk::ScrolledWindow> iControllerWindow; // Controller window.
    std::unique_ptr<Gtk::TreeView> iControllerList;         // Controller list.
    Glib::RefPtr<Gtk::ListStore> iListStore;                // Storage for data entries.
    std::unique_ptr<Gtk::ScrolledWindow> iCodeWindow;       // Code window.
    std::unique_ptr<Gtk::TextView> iCodeEditor;             // Code editor.
    std::list<CMapController>::iterator iSelected;          // Iterator to selected controller.
    type_signal_name m_signal_name;                         // Name signal server.
    int iControllerPageNumber;                              // Page number of list.
    int iCodePageNumber;                                    // Page number of editor.
};

#endif // __CONTROLLERVIEW_H__