// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Application class implementation.  The Application class
// serves as a foundation on which the application is constructed.
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

#include <glibmm/i18n.h>
#include "Application.h"
#include "MessageBar.h"

//-------------------------
// This is the constructor.
//-------------------------

Enigma::Application::Application() : Gtk::Application(G_APPLICATION_ID)
{
}

//---------------------------------------------
// Method called when application is activated.
//---------------------------------------------

void Enigma::Application::on_activate()
{	
	// Create a MainWindow.  Since only a reference to this window is passed
	// to the base class, ownership is retained by the derived class,
	// so the derived class will handle the window's destruction.

	m_window = std::make_unique<Enigma::MainWindow>();
	add_window(*m_window);

	// The Giomm 2.4 headers do not define a virtual on_shutdown() method that
	// can be overridden, so attach a signal handler to the shutdown signal.
	
	signal_shutdown().connect(sigc::mem_fun(this, &Application::do_shutdown));

	// Connect a key press event handler to the window, but place it before
	// its default handler.  This allows capturing general key press events.
	
	m_window->signal_key_press_event()
		.connect(sigc::mem_fun(*this, &Enigma::Application::on_key_press), false);
	
	// Create window contents.

	m_grid = std::make_unique<Gtk::Grid>();
	m_window->add(*m_grid);

	m_command = std::make_unique<Enigma::CommandEntry>();
	m_grid->attach(*m_command, 0, 0, 2, 1 );
	m_command->signal_command()
		.connect( sigc::mem_fun(*this, &Enigma::Application::on_command));

	m_view = std::make_unique<Gtk::Label>();
	m_grid->attach( *m_view, 0, 1, 1, 1 );
	m_view->set_halign(Gtk::ALIGN_START);
	m_view->set_hexpand(false);

	Enigma::MessageBar* message = Gtk::manage(new Enigma::MessageBar);
	m_grid->attach(*message, 1, 1, 1, 1);
	MessageBar->set_padding(0, 3);
	MessageBar->set_hexpand(true);

	m_notebook = std::make_unique<Gtk::Notebook>();
	m_grid->attach(*m_notebook, 0, 2, 2, 1);
	m_notebook->set_show_tabs(false);
	m_notebook->set_show_border(false);

	// Add viewer pages to the view notebook.
	
	m_levelview       = std::make_unique<Enigma::LevelView>();
	m_levelviewNumber = m_notebook->append_page(*m_levelview);

	m_roomview       = std::make_unique<Enigma::RoomView>();
	m_roomviewNumber = m_notebook->append_page(*m_roomview);

	m_teleporterview        = std::make_unique<Enigma::TeleporterView>();
	m_teleporterview_number = m_notebook->append_page(*m_teleporterview);

	m_itemview        = std::make_unique<Enigma::ItemView>();
	m_itemview_number = m_notebook->append_page(*m_itemview );

	m_playerview        = std::make_unique<Enigma::PlayerView>();
	m_playerview_number = m_notebook->append_page(*m_playerview);

  m_controllerview        = std::make_unique<Enigma::ControllerView>();
  m_controllerview_number = m_notebook->append_page(*m_controllerview);

	m_controlview        = std::make_unique<Enigma::ControlView>();
	m_controlview_number = m_notebook->append_page(*m_controlview);
	
	m_descriptionview       = std::make_unique<Enigma::DescriptionView>();
	m_descriptionview_number = m_notebook->append_page(*m_descriptionview);

	m_helpview        = std::make_unique<Enigma::HelpView>();
	m_helpview_number = m_notebook->append_page(*m_helpview);

  // Create a game map and provided it to the viewers.

  iMap = std::shared_ptr<CMap>( new CMap );
 
  m_levelview->SetMap( iMap );
  m_roomview->SetMap( iMap );
  m_teleporterview->SetMap( iMap );  
  m_itemview->SetMap( iMap );
  m_playerview->SetMap( iMap );
  m_controllerview->SetMap( iMap );
  m_controlview->SetMap( iMap );
  m_descriptionview->SetMap( iMap );

  // Connect the LevelView widget to the MessageBar widget so it will
  // display the current map location.
	
  m_levelview->signal_location()
  	.connect(sigc::mem_fun(*MessageBar, &CMessageBar::SetLocation));

  // Connect the LevelView widget to the RoomView widget to update
  // its current map location.
	
  m_levelview->signal_location()
  	.connect( sigc::mem_fun(*m_roomview, &CRoomView::SetLocation));
	
  // Connect the TeleporterView widget to the MessageBar widget so it will
  // display the teleporter's map location.
	
  m_teleporterview->signal_location()
  	.connect( sigc::mem_fun(*MessageBar, &CMessageBar::SetLocation));

  // Connect the ItemView widget to the MessageBar widget so it will
  // display the item's map location.
	
  m_itemview->signal_location()
  	.connect( sigc::mem_fun(*MessageBar, &CMessageBar::SetLocation));
	
  // Connect the PlayerView widget to the MessageBar widget so it will
  // display the player's map location.
	
  m_playerview->signal_location()
  	.connect( sigc::mem_fun(*MessageBar, &CMessageBar::SetLocation));
	
  // Connect the ControllerView widget to the MessageBar widget so it will
  // display the current controller name.
	
  m_controllerview->signal_name()
  	.connect( sigc::mem_fun( *MessageBar, &CMessageBar::set_label));
                                          	
	// Open the main window showing the help viewer.

	m_window->maximize();
	m_window->show_all();
	m_view->set_label( _("[ Help ]"));
	m_notebook->set_current_page(m_helpviewNumber);
}

//------------------------------------------------------
// This method is called when a command line is entered.
//------------------------------------------------------
// command: Command line.
//------------------------------------------------------

void Enigma::Application::on_command const Glib::ustring& command)
{
  std::vector<Glib::ustring> Arguments;
  std::vector<Glib::ustring> Parts;

  // Split the command line into parts separated by one or more white spaces.

  arguments = Glib::Regex::split_simple("\\s+", command);
  int total = arguments.size();
  int part_total;

  if (total > 0)
  {
    if (arguments.at(0).compare(_("o")) == 0)
    {
      // Create a new MapObject and assign it default values.

      Enigma::Object object;

      object.m_type     = Enigma::World::Type::OBJECT;
      object.m_id       = Enigma::Object::ID::BLOCKWALL;
      object.m_surface  = Enigma::World::Direction::BELOW;
      object.m_rotation = Enigma::World::Direction::NORTH;
      object.m_location = m_levelview->GetCursor();
      object.m_active   = false;
      object.m_category = World::Category::OPTIONAL;
      
      object.m_location_arrival.m_East  = G_MAXUINT16;
      object.m_location_arrival.m_Above = G_MAXUINT16;
      object.m_location_arrival.m_North = G_MAXUINT16;
      
      object.m_surface_arrival  = Enigma::World::Direction::NONE;
      object.m_rotation_arrival = Enigma::World::Direction::NONE;

      // Examine all arguments for a new MapObject.

      bool valid = true;
			
			for (int index = 1; index < total; ++ index)
			{
				parts      = Glib::Regex::split_simple("=", arguments.at(index));
				part_total = parts.size();
        
        if (part_total == 2)
        {
          // The command argument has two parts separated by an "=" sign.

          if (parts.at(0).compare(_("id")) == 0)
          {
            // Choose object ID.

            if (parts.at(1).compare(_("cellar")) == 0)
              object.m_id = Enigma::Object::ID::WINECELLAR;
            else if (parts.at(1).compare(_("bkw")) == 0)
              object.m_id = Enigma::Object::ID::BLOCKWALL;
            else if (parts.at(1).compare(_("snw")) == 0)
              object.m_id = Enigma::Object::ID::STONEWALL;
            else if (parts.at(1).compare(_("lad")) == 0)
              object.m_id = Enigma::Object::ID::LADDER;
            else if (parts.at(1).compare(_("lnd")) == 0)
              object.m_id = Enigma::Object::ID::LADDEREND;
            else if (parts.at(1).compare(_("person")) == 0)
              object.m_id = Enigma::Object::ID::PERSON;
            else if (parts.at(1).compare(_("blkr")) == 0)
              object.m_id = Enigma::Object::ID::BLOCKER;
            else if (parts.at(1).compare(_("mov")) == 0)
              object.m_id = Enigma::Object::ID::MOVER;
            else if (parts.at(1).compare(_("turn")) == 0)
              object.m_id = Enigma::Object::ID::TURNER;
            else if (parts.at(1).compare(_("face")) == 0)
              object.m_id = Enigma::Object::ID::FACER;
            else if (parts.at(1).compare(_("surf")) == 0)
              object.m_id = Enigma::Object::ID::SURFACER;	
            else if (parts.at(1).compare(_("mush")) == 0)
              object.m_id = Enigma::Object::ID::MUSHROOMS;
            else if (parts.at(1).compare(_("corkscrew")) == 0)
              object.m_id = Enigma::Object::ID::CORKSCREW;
            else if (parts.at(1).compare(_("winebottle")) == 0)
              object.m_id = Enigma::Object::ID::WINEBOTTLE;
            else if (parts.at(1).compare(_("bread")) == 0)
              object.m_id = Enigma::Object::ID::BREAD;
            else if (parts.at(1).compare(_("cheese")) == 0)
              object.m_id = Enigma::Object::ID::CHEESE;
            else if (parts.at(1).compare(_("knife")) == 0)
              object.m_id = Enigma::Object::ID::KNIFE;
            else if (parts.at(1).compare(_("apple")) == 0)
              object.m_id = Enigma::Object::ID::APPLE;
            else if (parts.at(1).compare(_("wineglass")) == 0)
              object.m_id = Enigma::Object::ID::WINEGLASS;
            else if (parts.at(1).compare(_("placemat")) == 0)
              object.m_id = Enigma::Object::ID::PLACEMAT;
            else if (parts.at(1).compare(_("grapes")) == 0)
              object.m_id = Enigma::Object::ID::GRAPES;
            else if (parts.at(1).compare(_("skull")) == 0)
              object.m_id = Enigma::Object::ID::SKULL;
            else if (parts.at(1).compare(_("egdn")) == 0)
              object.m_id = Enigma::Object::ID::EDGEDRAIN;
            else if (parts.at(1).compare(_("vent")) == 0)
              object.m_id = Enigma::Object::ID::AIRVENT;
            else if (parts.at(1).compare(_("easteregg")) == 0)
              object.m_id = Enigma::Object::ID::EASTEREGG;
            else if (parts.at(1).compare(_("flip")) == 0)
              object.m_id = Enigma::Object::ID::FLIPPER;
            else if (parts.at(1).compare(_("eger")) == 0)
              object.m_id = Enigma::Object::ID::EDGER;
            else if (parts.at(1).compare(_("str")) == 0)
              object.m_id = Enigma::Object::ID::STAIRS;
            else if (parts.at(1).compare(_("stp")) == 0)
              object.m_id = Enigma::Object::ID::STAIRSTOP;
            else if (parts.at(1).compare(_("sens")) == 0)
              object.m_id = Enigma::Object::ID::SENSOR;
            else if (parts.at(1).compare(_("hold")) == 0)
              object.m_id = Enigma::Object::ID::HANDHOLD;
            else if (parts.at(1).compare(_("moss0")) == 0)
              object.m_id = Enigma::Object::ID::MOSS0;
            else if (parts.at(1).compare(_("moss90")) == 0)
              object.m_id = Enigma::Object::ID::MOSS90;
            else if (parts.at(1).compare(_("moss180")) == 0)
              object.m_id = Enigma::Object::ID::MOSS180;
            else if (parts.at(1).compare(_("moss270")) == 0)
              object.m_id = Enigma::Object::ID::MOSS270;
            else if (parts.at(1).compare(_("moss360")) == 0)
              object.m_id = Enigma::Object::ID::MOSS360;
            else if (parts.at(1).compare(_("aballoon")) == 0)
              object.m_id = Enigma::Object::ID::AIRBALLOON;
            else if (parts.at(1).compare(_("plate")) == 0)
              object.m_id = Enigma::Object::ID::PLATE;
            else if (parts.at(1).compare(_("fork")) == 0)
              object.m_id = Enigma::Object::ID::FORK;
            else if (parts.at(1).compare(_("cake")) == 0)
              object.m_id = Enigma::Object::ID::CAKE;
            else if (parts.at(1).compare(_("snp")) == 0)
              object.m_id = Enigma::Object::ID::STONEPOST;
            else if (parts.at(1).compare(_("snb")) == 0)
              object.m_id = Enigma::Object::ID::STONEBUTTON;
            else if (parts.at(1).compare(_("eyes")) == 0)
              object.m_id = Enigma::Object::ID::WALLEYES;
            else if (parts.at(1).compare(_("out")) == 0)
              object.m_id = Enigma::Object::ID::OUTDOOR;
            else if (parts.at(1).compare(_("in")) == 0)
              object.m_id = Enigma::Object::ID::INDOOR;
            else if (parts.at(1).compare(_("arch")) == 0)
              object.m_id = Enigma::Object::ID::ARCHWAY;
            else if (parts.at(1).compare(_("dor")) == 0)
              object.m_id = Enigma::Object::ID::WOODDOOR;
            else if (parts.at(1).compare(_("wdw")) == 0)
              object.m_id = Enigma::Object::ID::WOODWALL;
            else if (parts.at(1).compare(_("ring")) == 0)
              object.m_id = Enigma::Object::ID::PULLRING;
            else if (parts.at(1).compare(_("culk")) == 0)
              object.m_id = Enigma::Object::ID::CUBELOCK;
            else if (parts.at(1).compare(_("splk")) == 0)
              object.m_id = Enigma::Object::ID::SPHERELOCK;
            else if (parts.at(1).compare(_("cube")) == 0)
              object.m_id = Enigma::Object::ID::CUBE;
            else if (parts.at(1).compare(_("sphere")) == 0)
              object.m_id = Enigma::Object::ID::SPHERE;
            else if (parts.at(1).compare(_("watl")) == 0)
              object.m_id = Enigma::Object::ID::WATERLAYER;
            else if (parts.at(1).compare(_("lbm")) == 0)
              object.m_id = Enigma::Object::ID::LIGHTBEAM;
            else if (parts.at(1).compare(_("egg")) == 0)
              object.m_id = Enigma::Object::ID::EGG;
            else if (parts.at(1).compare(_("web")) == 0)
              object.m_id = Enigma::Object::ID::WEB;
            else if (parts.at(1).compare(_("orange")) == 0)
              object.m_id = Enigma::Object::ID::ORANGE;
            else if (parts.at(1).compare(_("fish")) == 0)
              object.m_id = Enigma::Object::ID::FISH;
            else if (parts.at(1).compare(_("waterbottle")) == 0)
              object.m_id = Enigma::Object::ID::WATERBOTTLE;
            else if (parts.at(1).compare(_("fertilizer")) == 0)
              object.m_id = Enigma::Object::ID::FERTILIZER;
            else if (parts.at(1).compare(_("tre")) == 0)
              object.m_id = Enigma::Object::ID::TREE;
            else if (parts.at(1).compare(_("ttp")) == 0)
              object.m_id = Enigma::Object::ID::TREETOP;
            else if (parts.at(1).compare(_("cat")) == 0)
              object.m_id = Enigma::Object::ID::CATWALK;
            else if (parts.at(1).compare(_("tel")) == 0)
              object.m_id = Enigma::Object::ID::TELEPORTER;             
            else if (parts.at(1).compare(_("innertube")) == 0)
              object.m_id = Enigma::Object::ID::INNERTUBE;
            else if (parts.at(1).compare(_("hballoon")) == 0)
              object.m_id = Enigma::Object::ID::HYDROGENBALLOON;
            else if (parts.at(1).compare(_("wed")) == 0)
              object.m_id = Enigma::Object::ID::WEED;
            else if (parts.at(1).compare(_("frn")) == 0)
              object.m_id = Enigma::Object::ID::FERN;
            else if (parts.at(1).compare(_("vin")) == 0)
              object.m_id = Enigma::Object::ID::VINE;
            else if (parts.at(1).compare(_("walk")) == 0)
              object.m_id = Enigma::Object::ID::WATERLOCK;
            else if (parts.at(1).compare(_("aplk")) == 0)
              object.m_id = Enigma::Object::ID::APPLELOCK;
            else if (parts.at(1).compare(_("orlk")) == 0)
              object.m_id = Enigma::Object::ID::ORANGELOCK;
            else if (parts.at(1).compare(_("wilk")) == 0)
              object.m_id = Enigma::Object::ID::WINELOCK;
            else if (parts.at(1).compare(_("pab")) == 0)
              object.m_id = Enigma::Object::ID::PADBUTTON;
            else if (parts.at(1).compare(_("fenc")) == 0)
              object.m_id = Enigma::Object::ID::FENCE;
            else if (parts.at(1).compare(_("erw")) == 0)
              object.m_id = Enigma::Object::ID::EARTHWALL;
            else if (parts.at(1).compare(_("watr")) == 0)
              object.m_id = Enigma::Object::ID::WATER;
            else
              valid = false;
          }
          else if (parts.at(0).compare(_("typ")) == 0)
          {
            // Choose object type.

            if (parts.at(1).compare(_("reqi")) == 0)
            {
              object.m_type = Enigma::Object::Type::ITEM;
              object.m_category = Enigma::Object::Category::REQUIRED;
            }
            else if (parts.at(1).compare(_("opti")) == 0)
            {
              object.m_type = Enigma::Object::Type::ITEM;
              object.m_category = Enigma::Object::Category::OPTIONAL;
            }
            else if (parts.at(1).compare(_("eggi")) == 0)
            {
              object.m_type = Enigma::Object::Type::ITEM;
              object.m_category = Enigma::Object::Category::EASTEREGG;
            }
            else if (parts.at(1).compare(_("skli")) == 0)
            {
              object.m_type = Enigma::Object::Type::ITEM;
              object.m_category = Enigma::Object::Category::SKULL;
            }
            else if (parts.at(1).compare(_("actp")) == 0)
            {
              object.m_type     = CMapObject::Type::PLAYER;
              MapObject.iActive = true;
            }
            else if (parts.at(1).compare(_("idlp")) == 0)
            {
              object.m_type     = CMapObject::Type::PLAYER;
              MapObject.iActive = false;
            }
            else
              Valid = false;
          }
					else if (parts.at( 0).compare(_("surf")) == 0)
					{
						// Choose object surface.
						
						if (parts.at(1).compare(_("n")) == 0)
							object.m_surface = Enigma::Object::Direction::NORTH;
						else if (parts.at(1).compare(_("s")) == 0)
							object.m_surface = Enigma::Object::Direction::SOUTH;
						else if (parts.at(1).compare(_("e")) == 0)
							object.m_surface = Enigma::Object::Direction::EAST;
						else if (parts.at(1).compare(_("w")) == 0)
							object.m_surface = Enigma::Object::Direction::WEST;
						else if (parts.at(1).compare(_("a")) == 0)
							object.m_surface = Enigma::Object::Direction::ABOVE;
						else if (parts.at(1).compare(_("b")) == 0)
							object.m_surface = Enigma::Object::Direction::BELOW;
						else if (parts.at(1).compare(_("c")) == 0)
							object.m_surface = Enigma::Object::Direction::CENTER;
						else
						  Valid = false;
					}
          else if (parts.at(0).compare(_("rot")) == 0)
          {
            // Choose object rotation on surface.  Facers use directions
            // (e.g. "n" = North), while Turners used relative quarter-turns
            // to the right (e.g. "2" = two turns to the right).
					
            if (parts.at(1).compare(_("n")) == 0)
              object.m_rotation = Enigma::Object::Direction::NORTH;
            else if (parts.at(1).compare(_("s")) == 0)
              object.m_rotation = Enigma::Object::Direction::SOUTH;
            else if (parts.at(1).compare(_("e")) == 0)
              object.m_rotation = Enigma::Object::Direction::EAST;
            else if (parts.at(1).compare(_("w")) == 0)
              object.m_rotation = Enigma::Object::Direction::WEST;
            else if (parts.at(1).compare(_("a")) == 0)
              object.m_rotation = Enigma::Object::Direction::ABOVE;
            else if (parts.at(1).compare(_("b")) == 0)
              object.m_rotation = Enigma::Object::Direction::BELOW;
            else if (parts.at(1).compare(_("c")) == 0)
              object.m_rotation = Enigma::Object::Direction::ECenter;
            else if (parts.at(1).compare(_("0")) == 0)
              object.m_rotation = (Enigma::Object::Direction)0;
            else if (parts.at(1).compare(_("90")) == 0)
              object.m_rotation = (Enigma::Object::Direction)1;
            else if (parts.at(1).compare(_("180")) == 0)
              object.m_rotation = (Enigma::Object::Direction)2;
            else if (parts.at(1).compare(_("270")) == 0)
              object.m_rotation = (Enigma::Object::Direction)3;
            else if (parts.at(1).compare(_("-90")) == 0)
              object.m_rotation = (Enigma::Object::Direction)3;
            else
              Valid = false;
          }
					else if (parts.at( 0).compare(_("stel")) == 0)
          {
            // Choose Teleporter object arrival surface.

            if (parts.at(1).compare(_("n")) == 0)
              object.m_surface_arrival = Enigma::Object::Direction::NORTH;
            else if (parts.at(1).compare(_("s")) == 0)
              object.m_surface_arrival = Enigma::Object::Direction::SOUTH;
            else if (parts.at(1).compare(_("e")) == 0)
              object.m_surface_arrival = Enigma::Object::Direction::EAST;
            else if (parts.at(1).compare(_("w")) == 0)
              object.m_surface_arrival = Enigma::Object::Direction::WEST;
            else if (parts.at(1).compare(_("a")) == 0)
              object.m_surface_arrival = Enigma::Object::Direction::ABOVE;
            else if (parts.at(1).compare(_("b")) == 0)
              object.m_surface_arrival = Enigma::Object::Direction::BELOW;
            else if (parts.at(1).compare(_("p")) == 0)
              object.m_surface_arrival = Enigma::Object::Direction::NONE;
            else
              Valid = false;

            object.m_type = Enigma::Object::Type::TELEPORTER;
          }
          else if (parts.at( 0).compare(_("rtel")) == 0)
          {
            // Choose Teleporter object arrival rotation on surface.

            if (parts.at(1).compare(_("n")) == 0)
              object.m_rotationArrival = Enigma::Object::Direction::NORTH;
            else if (parts.at(1).compare(_("s")) == 0)
              object.m_rotationArrival = Enigma::Object::Direction::SOUTH;
            else if (parts.at(1).compare(_("e")) == 0)
              object.m_rotationArrival = Enigma::Object::Direction::EAST;
            else if (parts.at(1).compare(_("w")) == 0)
              object.m_rotationArrival = Enigma::Object::Direction::WEST;
            else if (parts.at(1).compare(_("a")) == 0)
              object.m_rotationArrival = Enigma::Object::Direction::ABOVE;
            else if (parts.at(1).compare(_("b")) == 0)
              object.m_rotationArrival = Enigma::Object::Direction::BELOW;
            else if (parts.at(1).compare(_("p")) == 0)
              object.m_surfaceArrival = Enigma::Object::Direction::NONE;
            else
              Valid = false;
              
            object.m_type = Enigma::Object::Type::TELEPORTER;
          }
          else if (parts.at( 0).compare(_("etel")) == 0)
          {
            // Set teleporter object arrival East coordinate.
            
            object.m_location_arrival.m_east = std::stoi(parts.at(1));
            object.m_type = Enigma::Object::Type::TELEPORTER;
          }
          else if (parts.at( 0).compare(_("ntel")) == 0)
          {
            // Set teleporter object arrival North coordinate.
            
            object.m_location_arrival.m_north = std::stoi(parts.at(1));
            object.m_type = Enigma::Object::Type::TELEPORTER;
          }
          else if (parts.at( 0).compare(_("atel")) == 0)
          {
            // Set teleporter object arrival Above coordinate.
            
            object.m_location_arrival.m_above = std::stoi(parts.at(1));
            object.m_type = Enigma::Object::Type::TELEPORTER;
          }
          else if (parts.at( 0).compare(_("sens")) == 0)
          {
            // Add object sense state or signal.
            
            object.iSense = parts.at(1);
          }
          else if (parts.at( 0).compare(_("stat")) == 0)
          {
            // Add object functional state or signal.
            
            object.iState = parts.at(1);
          }
          else if (parts.at( 0).compare(_("visi")) == 0)
          {
            // Add object visibility state or signal.
            
            object.iVisibility = parts.at(1);
          }
          else if (parts.at( 0).compare(_("pres")) == 0)
          {
            // Add object presence state or signal.
            
            object.iPresence = parts.at(1);
          }
        }
      }

      if ( Valid )
      {
        // Add the object to the correct list in the game map.
			
        if (object.m_type == Enigma::Object::Type::ITEM)
          m_world->m_items.Insert( MapObject );
        else if (object.m_type == Enigma::Object::Type::PLAYER)
          m_world->m_players.Insert( MapObject );
        else if (object.m_type == Enigma::Object::Type::TELEPORTER)
          m_world->m_teleporters.Insert(object);
        else
          m_world->m_objects.Insert(object);

        // Update the appropriate view.
			
        if (m_notebook->get_current_page() == m_levelview_number)
          m_levelview->update();
        else if (m_notebook->get_current_page() == m_roomview_number)
          m_roomview->update();
        else if (m_notebook->get_current_page() == m_itemview_number)
          m_itemview->update();
      }
    }
    else if (arguments.at(0).compare(_("c")) == 0)
    {
      // Add a new controller to the map.
      
      if (total == 2)
      {
        Enigma::Controller controller;
        
        controller.m_name = arguments.at(1);
        m_world->m_controllers.push_back(controller);
        
        // Update the Controller view if showing.
        
        if (m_notebook->get_current_page() == m_controllerview_number)
          m_controllerview->update();
      }
    }
    else if (arguments.at(0).compare(_("l")) == 0)
    {
      if (total == 1)
      {
        // No filename is provided, so load using the world's internal
        // filename.
        
        m_world->load();
      }
      else if ( Total == 2 )
      {
        // A filename is provided, so update the world's internal name
        // to be used for loading.
      
        m_world->m_filename = arguments.at(1);
        m_world->load();
      
        // Report the game world filename in the window title.

        m_window->set_title_message(m_world->m_filename);
      }
      
      // Reset the ControllerView.
      
      m_controllerview->reset();
      
      // Switch to the LevelView.
      
      m_view->set_label(_("[ Map Level ]"));
      m_notebook->set_current_page( m_levelview_number);
      m_levelview->Home();
    }
    else if (arguments.at(0).compare(_("s")) == 0)
    {
      if (total == 1)
      {
        // No filename is provided, so save using the world's internal
        // filename.
        
        m_world->save();
      }
      else if (total == 2)
      {
        // A filename is provided, so update the world's internal name
        // to be used for saving.
      
        m_world->m_filename = arguments.at(1);
        m_world->save();
      
        // Report the game world filename in the window title.
      
        m_window->set_title_message(m_world->m_filename);
      }
    }
    else if (arguments.at(0).compare(_("n")) == 0)
    {    
      if (total == 1)
      {
        // No filename is provided, so clear the map's internal name.
        
        m_world->m_filename = "";
      }
      else if (total == 2)
      {
        // A filename is provided, so update the world's internal name
        // to be used for saving.
        
        m_world->m_filename = arguments.at(1);
      }
     
      m_world->clear();
      
      // Reset the ControllerView.
      
      m_controllerview->reset();
      
      // Report the game world filename in the window title.
      
      m_window->set_title_message(m_world->m_filename); 
      
      // Switch to the LevelView view.  This will show an empty map with
      // the cursor located at 0, 0, 0.
      
      m_view->set_label(_("[ Map Level ]"));
      m_notebook->set_current_page(m_levelview_number);
      m_levelview->home();
    }
    else if (arguments.at(0).compare(_("f")) == 0)
    {
      // View only filter certain objects in the LevelView.
      
      Enigma::Object::ID Filter = Enigma::Object::ID::NONE;      
      
      if ( Total == 2 )
      {
        if (arguments.at(1).compare(_("cellar")) == 0)
          Filter = Enigma::Object::ID::WINECELLAR;
        else if (arguments.at(1).compare(_("bkw")) == 0)
          Filter = Enigma::Object::ID::BLOCKWALL;
        else if (arguments.at(1).compare(_("snw")) == 0)
          Filter = Enigma::Object::ID::STONEWALL;
        else if (arguments.at(1).compare(_("lad")) == 0)
          Filter = Enigma::Object::ID::LADDER;
        else if (arguments.at(1).compare(_("lnd")) == 0)
          Filter = Enigma::Object::ID::LADDEREND;
        else if (arguments.at(1).compare(_("person")) == 0)
          Filter = Enigma::Object::ID::PERSON;
        else if (arguments.at(1).compare(_("blkr")) == 0)
          Filter = Enigma::Object::ID::BLOCKER;
        else if (arguments.at(1).compare(_("mov")) == 0)
          Filter = Enigma::Object::ID::MOVER;
        else if (arguments.at(1).compare(_("turn")) == 0)
          Filter = Enigma::Object::ID::TURNER;
        else if (arguments.at(1).compare(_("face")) == 0)
          Filter = Enigma::Object::ID::FACER;
        else if (arguments.at(1).compare(_("surf")) == 0)
          Filter = Enigma::Object::ID::SURFACER;	
        else if (arguments.at(1).compare(_("mush")) == 0)
          Filter = Enigma::Object::ID::MUSHROOMS;
        else if (arguments.at(1).compare(_("corkscrew")) == 0)
          Filter = Enigma::Object::ID::CORKSCREW;
        else if (arguments.at(1).compare(_("winebottle")) == 0)
          Filter = Enigma::Object::ID::WINEBOTTLE;
        else if (arguments.at(1).compare(_("bread")) == 0)
          Filter = Enigma::Object::ID::BREAD;
        else if (arguments.at(1).compare(_("cheese")) == 0)
          Filter = Enigma::Object::ID::CHEESE;
        else if (arguments.at(1).compare(_("knife")) == 0)
          Filter = Enigma::Object::ID::KNIFE;
        else if (arguments.at(1).compare(_("apple")) == 0)
          Filter = Enigma::Object::ID::APPLE;
        else if (arguments.at(1).compare(_("wineglass")) == 0)
          Filter = Enigma::Object::ID::WINEGLASS;
        else if (arguments.at(1).compare(_("placemat")) == 0)
          Filter = Enigma::Object::ID::PLACEMAT;
        else if (arguments.at(1).compare(_("grapes")) == 0)
          Filter = Enigma::Object::ID::GRAPES;
        else if (arguments.at(1).compare(_("skull")) == 0)
          Filter = Enigma::Object::ID::SKULL;
        else if (arguments.at(1).compare(_("egdn")) == 0)
          Filter = Enigma::Object::ID::EDGEDRAIN;
        else if (arguments.at(1).compare(_("vent")) == 0)
          Filter = Enigma::Object::ID::AIRVENT;
        else if (arguments.at(1).compare(_("easteregg")) == 0)
          Filter = Enigma::Object::ID::EASTEREGG;
        else if (arguments.at(1).compare(_("flip")) == 0)
          Filter = Enigma::Object::ID::FLIPPER;
        else if (arguments.at(1).compare(_("eger")) == 0)
          Filter = Enigma::Object::ID::EDGER;
        else if (arguments.at(1).compare(_("str")) == 0)
          Filter = Enigma::Object::ID::STAIRS;
        else if (arguments.at(1).compare(_("stp")) == 0)
          Filter = Enigma::Object::ID::STAIRSTOP;
        else if (arguments.at(1).compare(_("sens")) == 0)
          Filter = Enigma::Object::ID::SENSOR;
        else if (arguments.at(1).compare(_("hold")) == 0)
          Filter = Enigma::Object::ID::HANDHOLD;
        else if (arguments.at(1).compare(_("m0")) == 0)
          Filter = Enigma::Object::ID::MOSS0;
        else if (arguments.at(1).compare(_("m90")) == 0)
          Filter = Enigma::Object::ID::MOSS90;
        else if (arguments.at(1).compare(_("m180")) == 0)
          Filter = Enigma::Object::ID::MOSS180;
        else if (arguments.at(1).compare(_("m270")) == 0)
          Filter = Enigma::Object::ID::MOSS270;
        else if (arguments.at(1).compare(_("m360")) == 0)
          Filter = Enigma::Object::ID::MOSS360;
        else if (arguments.at(1).compare(_("aballoon")) == 0)
          Filter = Enigma::Object::ID::AIRBALLOON;
        else if (arguments.at(1).compare(_("plate")) == 0)
          Filter = Enigma::Object::ID::PLATE;
        else if (arguments.at(1).compare(_("fork")) == 0)
          Filter = Enigma::Object::ID::FORK;
        else if (arguments.at(1).compare(_("cake")) == 0)
          Filter = Enigma::Object::ID::CAKE;
        else if (arguments.at(1).compare(_("snp")) == 0)
          Filter = Enigma::Object::ID::STONEPOST;
        else if (arguments.at(1).compare(_("snb")) == 0)
          Filter = Enigma::Object::ID::STONEBUTTON;
        else if (arguments.at(1).compare(_("eyes")) == 0)
          Filter = Enigma::Object::ID::WALLEYES;
        else if (arguments.at(1).compare(_("out")) == 0)
          Filter = Enigma::Object::ID::OUTDOOR;
        else if (arguments.at(1).compare(_("in")) == 0)
          Filter = Enigma::Object::ID::INDOOR;
        else if (arguments.at(1).compare(_("arch")) == 0)
          Filter = Enigma::Object::ID::ARCHWAY;
        else if (arguments.at(1).compare(_("dor")) == 0)
          Filter = Enigma::Object::ID::WOODDOOR;
        else if (arguments.at(1).compare(_("wdw")) == 0)
          Filter = Enigma::Object::ID::WOODWALL;
        else if (arguments.at(1).compare(_("ring")) == 0)
          Filter = Enigma::Object::ID::PULLRING;
        else if (arguments.at(1).compare(_("culk")) == 0)
          Filter = Enigma::Object::ID::CUBELOCK;
        else if (arguments.at(1).compare(_("splk")) == 0)
          Filter = Enigma::Object::ID::SPHEREKEY;
        else if (arguments.at(1).compare(_("cube")) == 0)
          Filter = Enigma::Object::ID::CUBE;
        else if (arguments.at(1).compare(_("sphere")) == 0)
          Filter = Enigma::Object::ID::SPHERE;
        else if (arguments.at(1).compare(_("watl")) == 0)
          Filter = Enigma::Object::ID::WATERLAYER;
        else if (arguments.at(1).compare(_("lbm")) == 0)
          Filter = Enigma::Object::ID::LIGHTBEAM;
        else if (arguments.at(1).compare(_("egg")) == 0)
          Filter = Enigma::Object::ID::EGG;
        else if (arguments.at(1).compare(_("web")) == 0)
          Filter = Enigma::Object::ID::WEB;
        else if (arguments.at(1).compare(_("orange")) == 0)
          Filter = Enigma::Object::ID::ORANGE;
        else if (arguments.at(1).compare(_("fish")) == 0)
          Filter = Enigma::Object::ID::FISH;
        else if (arguments.at(1).compare(_("waterbottle")) == 0)
          Filter = Enigma::Object::ID::WATERBOTTLE;
        else if (arguments.at(1).compare(_("fertilizer")) == 0)
          Filter = Enigma::Object::ID::FERTILIZER;
        else if (arguments.at(1).compare(_("tre")) == 0)
          Filter = Enigma::Object::ID::TREE;
        else if (arguments.at(1).compare(_("ttp")) == 0)
          Filter = Enigma::Object::ID::TREETOP;
        else if (arguments.at(1).compare(_("cat")) == 0)
          Filter = Enigma::Object::ID::CATWALK;
        else if (arguments.at(1).compare(_("tel")) == 0)
          Filter = Enigma::Object::ID::TELEPORTER;
        else if (arguments.at(1).compare(_("innertube")) == 0)
          Filter = Enigma::Object::ID::INNERTUBE;
        else if (arguments.at(1).compare(_("hballoon")) == 0)
          Filter = Enigma::Object::ID::HYDROGENBALLOON;
        else if (arguments.at(1).compare(_("wed")) == 0)
          Filter = Enigma::Object::ID::WEED;
        else if (arguments.at(1).compare(_("frn")) == 0)
          Filter = Enigma::Object::ID::FERN;
        else if (arguments.at(1).compare(_("vin")) == 0)
          Filter = Enigma::Object::ID::VINE;
        else if (arguments.at(1).compare(_("walk")) == 0)
          Filter = Enigma::Object::ID::WATERLOCK;
        else if (arguments.at(1).compare(_("aplk")) == 0)
          Filter = Enigma::Object::ID::APPLELOCK;
        else if (arguments.at(1).compare(_("orlk")) == 0)
          Filter = Enigma::Object::ID::ORANGELOCK;
        else if (arguments.at(1).compare(_("wilk")) == 0)
          Filter = Enigma::Object::ID::WINELOCK;
        else if (arguments.at(1).compare(_("pab")) == 0)
          Filter = Enigma::Object::ID::PADBUTTON;
        else if (arguments.at(1).compare(_("fenc")) == 0)
          Filter = Enigma::Object::ID::FENCE;
        else if (arguments.at(1).compare(_("erw")) == 0)
          Filter = Enigma::Object::ID::EARTHWALL;
        else if (arguments.at(1).compare(_("watr")) == 0)
          Filter = Enigma::Object::ID::WATER;          
      }

      m_levelview->set_filter(filter);          
    }
    else if (arguments.at(0).compare(_("q")) == 0)
      quit();
  }
}

//-------------------------------------------------
// Method called when application is shutting down.
//-------------------------------------------------

void Enigma::Application::do_shutdown()
{
}

//-------------------------------------------------------
// Method to handle key press events from the MainWindow.
//-------------------------------------------------------
// key_event: Pointer to GdkEventKey.
// RETURN:    TRUE if key press was handled.
//-------------------------------------------------------

bool Enigma::Application::on_key_press(GdkEventKey* key_event)
{
  // Exit immediately if the event is not due to a key press.
  // The event is allowed to propagate to other handlers.
	
  if (key_event->type != GDK_KEY_PRESS)
    return false;

  bool handled = true;	
  int keyvalue = key_event->keyval;

  if (keyvalue == GDK_KEY_F1)
  {
    if (m_notebook->get_current_page() == m_roomviewNumber)
    {
      m_view->set_label(_("[ Map Level ]"));
      m_notebook->set_current_page( m_levelviewNumber);
    }    
    else
    {
      m_view->set_label(_("[ Map Room Contents ]"));
      m_notebook->set_current_page( m_roomviewNumber);
    }
  }
  else if (keyvalue == GDK_KEY_F2)
  {
    if (m_notebook->get_current_page() == m_teleporterviewNumber)
    {
      m_view->set_label(_("[ Map Level ]"));
      m_notebook->set_current_page( m_levelviewNumber);
    }
    else
    {
      m_view->set_label(_("[ Map Teleporters ]"));
      m_notebook->set_current_page( m_teleporterviewNumber);
    }
  }  
  else if (KeyValue == GDK_KEY_F3)
  {
    if (m_notebook->get_current_page() == m_itemviewNumber)
    {
      m_view->set_label(_("[ Map Level ]"));
      m_notebook->set_current_page( m_levelviewNumber );
    }
    else
    {
      m_view->set_label(_("[ Map Items ]"));
      m_notebook->set_current_page( m_itemviewNumber);
    }
  }
  else if (keyvalue == GDK_KEY_F4)
  {
    if (m_notebook->get_current_page() == m_playerviewNumber)
    {
      m_view->set_label(_("[ Map Level ]"));
      m_notebook->set_current_page(m_levelviewNumber);
    }
    else
    {
      m_view->set_label(_("[ Map Players ]"));
      m_notebook->set_current_page(m_playerviewNumber);
    }
  }
  else if (keyvalue == GDK_KEY_F5)
  {
    if (m_notebook->get_current_page() == m_controllerviewNumber)
    {
      m_view->set_label( _("[ Map Level ]"));
      m_notebook->set_current_page(m_levelviewNumber);
    }
    else
    {
      m_view->set_label(_("[ Map Controllers ]"));
      m_notebook->set_current_page(m_controllerviewNumber);
    }
  }
  else if (keyvalue == GDK_KEY_F6)
  {
    if ( m_notebook->get_current_page() == m_controlviewNumber )
    {
      m_view->set_label(_("[ Map Level ]"));
      m_notebook->set_current_page(m_levelview_number);
    }
    else
    {
      m_view->set_label(_("[ Map Control ]"));
      m_notebook->set_current_page(m_controlview_number);
    }
  }
  else if (keyvalue == GDK_KEY_F7)
  {
    if (m_notebook->get_current_page() == m_descriptionviewNumber)
    {
      m_view->set_label(_("[ Map Level ]"));
      m_notebook->set_current_page(m_levelviewNumber);
    }
    else
    {
      m_view->set_label(_("[ Map Description ]"));
      m_notebook->set_current_page(m_descriptionview_number);
    }
  }
  else if (keyvalue == GDK_KEY_F8)
  {
    if (m_notebook->get_current_page() == m_helpview_number)
    {
      m_view->set_label(_("[ Map Level ]"));
      m_notebook->set_current_page(m_levelview_number);
    }
    else
    {
      m_view->set_label(_("[ Help ]"));
      m_notebook->set_current_page(m_helpview_number);
    }
  }
  else
    handled = false;
		
	return handled;
}
