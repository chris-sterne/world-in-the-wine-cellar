/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 5.1 for Linux OS                    *
 * File:    Application.h                       *
 * Date:    October 4, 2017                     *
 * Author:  Chris Sterne                        *
 *                                              *
 * Application class header.                    *
 *----------------------------------------------*/
 
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <gtkmm.h>
#include "EnigmaWC.h"
#include "MainWindow.h"
#include "LevelView.h"
#include "RoomView.h"
#include "ControllerView.h"
#include "ControlView.h"
#include "TeleporterView.h"
#include "ItemView.h"
#include "PlayerView.h"
#include "DescriptionView.h"
#include "HelpView.h"
#include "CommandEntry.h"
#include "Map.h"

class CApplication : public Gtk::Application
{
  public:
    // Public methods.
		
    CApplication();
    void On_Command( const Glib::ustring& aCommand );
    gboolean On_Key_Press( GdkEventKey* key_event );
    bool On_MainWindow_Delete( GdkEventAny* any_event );
		
  protected:
    // Protected methods from base class.
    
    void on_activate();

  private:
    // Private data.

    std::shared_ptr<CMap> iMap;                   // Game map.
    std::unique_ptr< CMainWindow > iMainWindow;   // Top level window.
    std::unique_ptr<Gtk::Grid> iGrid;							// Container for widgets.
    std::unique_ptr<CCommandEntry> iCommandEntry; // Command line entry widget.
    std::unique_ptr<Gtk::Label> iViewName;        // Map Viewer name label.
    std::unique_ptr< Gtk::Notebook > iViewBook;   // Notebook for viewers.
        
    // Map viewers.
    
    std::unique_ptr<CLevelView> iLevelView;
    std::unique_ptr<CRoomView> iRoomView;
    std::unique_ptr<CTeleporterView> iTeleporterView;    
    std::unique_ptr<CItemView> iItemView;
    std::unique_ptr<CPlayerView> iPlayerView;    
    std::unique_ptr<CControllerView> iControllerView;
    std::unique_ptr<CControlView> iControlView;
    std::unique_ptr<CDescriptionView> iDescriptionView;
    std::unique_ptr<CHelpView> iHelpView;

    // Map viewer notebook page numbers.
    
    int iLevelViewNumber;
    int iRoomViewNumber;
    int iTeleporterViewNumber;    
    int iItemViewNumber;
    int iPlayerViewNumber;        
    int iControllerViewNumber;
    int iControlViewNumber;
    int iDescriptionViewNumber;
    int iHelpViewNumber;
};

#endif // __APPLICATION_H__