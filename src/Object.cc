// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Object class implementation.  The object class describes
// objects in the game world.
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
#include "Object.h"

//--------------------
// Local declarations.
//--------------------

// Array of strings associated with an object.  These are in the same order
// as the enumerated object IDs, so an object ID can be used as an array
// index.

static const char* id_text_array[(int)Enigma::Object::ID::TOTAL] =
{
  "None",
  "WineCellar",
  "BlockWall",
  "StoneWall",
  "Ladder",
  "LadderTop",
  "Person",
  "Blocker",
  "Mover",
  "Turner",
  "Facer",
  "Surfacer",
  "Mushrooms",
  "CorkScrew",
  "WineBottle",
  "Bread",
  "Cheese",
  "Knife",
  "Apple",
  "WineGlass",
  "PlaceMat",
  "Grapes",
  "Skull",
  "EdgeDrain",
  "AirVent",
  "EasterEgg",
  "Flipper",
  "Edger",
  "Stairs",
  "StairsTop",
  "Sensor",
  "HandHold",
  "Moss0",
  "Moss90",
  "Moss180",
  "Moss270",
  "Moss360",
  "AirBalloon",
  "Plate",
  "Fork",
  "Cake",
  "StonePost",
  "StoneButton",
  "WallEyes",
  "Outdoor",
  "Indoor",
  "SkyObjects",
  "ArchWay",
  "WoodDoor",
  "WoodWall",
  "PullRing",
  "CubeLock",
  "SphereLock",
  "Cube",
  "Sphere",
  "WaterLayer",
  "LightBeam",
  "Egg",
  "Web",
  "Orange",
  "Fish",
  "WaterBottle",
  "Fertilizer",
  "Tree",
  "TreeTop",
  "CatWalk",
  "Teleporter",
  "InnerTube",
  "HydrogenBalloon",
  "Weed",
  "Fern",
  "Vine",
  "WaterLock",
  "AppleLock",
  "OrangeLock",
  "WineLock",
  "PadButton",
  "Fence",
  "EarthWall",
  "Water"
};

// Array of strings associated with a world direction.  These are in the same
// order as the enumerated object directions, so a direction can be used as
// an array index.

static const char* direction_text_array[(int)Enigma::Object::Direction::TOTAL]
{
  "None",
  "North",
  "South",
  "East",
  "West",
  "Above",
  "Below",
  "Center"
};

// Array of strings associated with an item category.  These are in the same
// order as the enumerated item categories, so a category can be used as
// an array index.

static const char* category_text_array[(int)Enigma::Object::Category::TOTAL]
{
  "None",
  "Required Item",
  "Optional Item",
  "Easter Egg Item",
  "Skull Item"
};

//--------------------------------------------------------
// This method returns a text description of the object
// (excludes the world position).
//--------------------------------------------------------
// description: Destination buffer for text.              
//--------------------------------------------------------

void Enigma::Object::get_description(Glib::ustring& description)
{
  Glib::ustring id_text;

  // Select text corresponding to the object's ID.  Ensure the index
  // formed from the ID will not exceed the ID text array.
  
  if ((int)m_id < (int)Enigma::Object::ID::TOTAL)
    id_text = id_text_array[(int)m_id];
  else
    id_text = "???";

  // Select a text string corresponding to the object's type.
  
  Glib::ustring type_text;

  switch(m_type)
  {
    case Enigma::Object::Type::OBJECT:
      type_text = _("Object");
      break;
	
    case Enigma::Object::Type::PLAYER:
      if (m_active)
        type_text = _("Active Player");
      else
        type_text = _("Idle Player");
			
      break;
						
    case Enigma::Object::Type::ITEM:
      if ((int)m_category < (int)Enigma::Object::Category::TOTAL)      
        type_text = category_text_array[(int)m_category];
      else
        type_text = "???";
        
    break;
        
    case Enigma::Object::Type::TELEPORTER:
      type_text = _("Teleporter");
      break;
      
    default:
      break;
  }

  // Built a text string describing teleporter information.  

  Glib::ustring surface_text;
  Glib::ustring rotation_text;
  Glib::ustring east_text;
  Glib::ustring above_text;
  Glib::ustring north_text;
  Glib::ustring teleport_text;

  if (m_type == Enigma::Object::Type::TELEPORTER)
  {
    if (m_surface_arrival == Enigma::Object::Direction::NONE)
      surface_text = _("Player");
    else if ((int)m_surface_arrival < (int)Enigma::Object::Direction::TOTAL)
      surface_text = direction_text_array[(int)m_surface_arrival];
    else
      surface_text = "???";

    if (m_rotation_arrival == Enigma::Object::Direction::NONE)
      rotation_text = _("Player");
    else if ((int)m_rotation_arrival < (int)Enigma::Object::Direction::TOTAL)
      rotation_text = direction_text_array[(int)m_rotation_arrival];
    else
      rotation_text = "???";

		unsigned short position = m_position_arrival.m_east;

    if (position == 65535)
      east_text = _("Player");
    else
      east_text = Glib::ustring::compose("%1", position);

    position = m_position_arrival.m_north;
    
    if (position == 65535)
      north_text = _("Player");
    else
      north_text = Glib::ustring::compose("%1", position);

    position = m_position_arrival.m_above;
    
    if (position == 65535)
      above_text = _("Player");
    else
      above_text = Glib::ustring::compose("%1", position);

    teleport_text = Glib::ustring::compose(
_("\nARRIVAL    SURFACE = %1    ROTATION = %2    EAST = %3    NORTH = %4\
    ABOVE = %5" ),
    surface_text, rotation_text, east_text, north_text, above_text);
  }

  // Select a text string corresponding to the object's surface.

  if ((int)m_surface < (int)Enigma::Object::Direction::TOTAL)
    surface_text = direction_text_array[(int)m_surface];
  else
    surface_text = "???";

  // Select a text string corresponding to the object's rotation.
  
  if (m_id == Enigma::Object::ID::TURNER)
  {
    // Turner are relative rotation offsets rather than absolute
    // directions.
    
    switch((int)m_rotation)
    {
      case 0:
        rotation_text = _("0 degrees");
        break;
        
      case 1:
        rotation_text = _("90 degrees");
        break;
    
      case 2:
        rotation_text = _("180 degrees");
        break;
        
      case 3:
        rotation_text = _("270 degrees");
        break;
    
      default:
        rotation_text = "???";
        break;
    }
  }
  else
  {
    if ((int)m_rotation < (int)Enigma::Object::Direction::TOTAL)
      rotation_text = direction_text_array[(int)m_rotation];
    else
      rotation_text = "???";
  }

  // Built a text string with all object states or state signals.

  Glib::ustring signal_text;
  
  if (m_sense.size())
  {
    if (signal_text.size())
      signal_text.append( "    ");
    else
      signal_text.push_back('\n');

    signal_text.append(_("SENSE = "));
    signal_text.append(m_sense);
  }
   
  if (m_state.size())
  {
    if (signal_text.size())
      signal_text.append("    ");
    else
      signal_text.push_back('\n');
  
    signal_text.append(_("STATE = "));
    signal_text.append(m_state);
  }
  
  if (m_visibility.size())
  {
    if (signal_text.size())
      signal_text.append("    ");
    else
      signal_text.push_back('\n');
      
    signal_text.append(_("VISIBILITY = "));
    signal_text.append(m_visibility);
  }
  
  if (m_presence.size())
  {
    if (signal_text.size())
      signal_text.append("    ");
    else
      signal_text.push_back('\n');

    signal_text.append(_("PRESENCE = "));
    signal_text.append(m_presence);
  } 
  
  // Assemble all text strings into one string.
  
  description =
    Glib::ustring::compose(
_("ID = %1\n\
TYPE = %2    SURFACE = %3    ROTATION = %4\
%5\
%6"),
      id_text,
      type_text,
      surface_text,
      rotation_text,
      teleport_text,
      signal_text);
}
