/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 3.0 for Linux OS                    *
 * File:    LevelView.h                         *
 * Date:    July 4, 2016                        *
 * Author:  Chris Sterne                        *
 *                                              *
 * LevelView class header.                      *
 *----------------------------------------------*/


#ifndef __LEVELVIEW_H__
#define __LEVELVIEW_H__

#include <gtkmm.h>
#include "MapVolume.h"
#include "ImageTiles.h"
#include "Map.h"

class CLevelView : public Gtk::DrawingArea
{
  public:
    // Public methods.

    CLevelView();
    ~CLevelView();
    void Home();
		void Update();
		void SetMap( std::shared_ptr<CMap> aMap );
		void SetFilter( EnigmaWC::ID aFilter );
		CMapLocation& GetCursor();

		// Map location signal accessor.
		
	  typedef sigc::signal<void, const CMapLocation&> type_signal_location;
		type_signal_location signal_location();
		
  protected:
		// Overridden base class methods.
		
		void on_size_allocate( Gtk::Allocation& allocation );
		void on_map();
    bool on_draw( const Cairo::RefPtr<Cairo::Context>& aCC );
		bool on_key_press_event( GdkEventKey* key_event );
		virtual void get_preferred_width_vfunc(int& minimum_width, int& natural_width) const;
		virtual void get_preferred_height_vfunc(int& minimum_height, int& natural_height) const;
		
  private:
    // Private methods.
    
    void Do_Location();
    void Mark();
    void Delete();
    void Cut();
    void Copy();
    void Paste();
  
		// Private data.

		std::shared_ptr<CMap> iMap;                // Shared Game map.
		CImageTiles iImageTiles;                   // Image tile drawing object.
		CMapVolume iView;                          // Visible volume (one level) of map.
		CMapVolume iMark;                          // Marked volume of map.
		CMapLocation iMarkOrigin;                  // Origin of marked volume.
		CMapLocation iCursor;                      // Map location of cursor.
		type_signal_location m_signal_location;    // Map location signal server.
    std::list<CMapObject> iEditBuffer;         // Objects editing buffer.
    EnigmaWC::ID iFilter;                      // Object viewing filter.
};

#endif // __LEVELVIEW_H__