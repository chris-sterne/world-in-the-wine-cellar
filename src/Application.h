// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Application class header.  The Application class serves
// as a foundation on which the application is constructed.
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
 
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <gtkmm/application.h>
#include <gtkmm/label.h>
#include <gtkmm/grid.h>
#include <gtkmm/notebook.h>
#include <gdkmm/event.h>

namespace Enigma
{
	class World;
	class MainWindow;
	class CommandEntry;
	class LevelView;
	class RoomView;
	class TeleporterView;
	class ItemView;
	class PlayerView;
	class ControllerView;
	class ControlView;
	class DescriptionView;
	class HelpView;
	
	class Application : public Gtk::Application
	{
		public:
			// Public methods.

			Application();
			void on_command(const Glib::ustring& command);
			bool on_key_press(GdkEventKey* key_event);

			// Protected methods from base class.

			void on_activate() override;

		private:
			// Private data.

			std::shared_ptr<Enigma::World> m_world;
			std::unique_ptr<Enigma::MainWindow> m_mainwindow;
			std::unique_ptr<Gtk::Grid> m_grid;
			std::unique_ptr<Enigma::CommandEntry> m_commandentry;
			std::unique_ptr<Gtk::Label> m_viewname;
			std::unique_ptr<Gtk::Notebook> m_viewbook;

			// Map viewers.

			std::unique_ptr<Enigma::LevelView> m_levelview;
			std::unique_ptr<Enigma::RoomView> m_roomview;
			std::unique_ptr<Enigma::TeleporterView> m_teleporterview;    
			std::unique_ptr<Enigma::ItemView> m_itemview;
			std::unique_ptr<Enigma::PlayerView> m_playerview;    
			std::unique_ptr<Enigma::ControllerView> m_controllerview;
			std::unique_ptr<Enigma::ControlView> m_controlview;
			std::unique_ptr<Enigma::DescriptionView> m_descriptionview;
			std::unique_ptr<Enigma::HelpView> m_helpview;

			// Map viewer notebook page numbers.

			int m_levelview_number;
			int m_roomView_number;
			int m_teleporterview_number;    
			int m_itemview_number;
			int m_playerview_number;        
			int m_controllerview_number;
			int m_controlview_number;
			int m_descriptionview_number;
			int m_helpview_number;
	};
}

#endif // __APPLICATION_H__
