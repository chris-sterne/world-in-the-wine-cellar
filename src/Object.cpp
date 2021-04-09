/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 5.1 for Linux OS                    *
 * File:    MapObject.cpp                       *
 * Date:    October 8, 2017                     *
 * Author:  Chris Sterne                        *
 *                                              *
 * MapObject class.                             *
 *----------------------------------------------*/

#include <glibmm/i18n.h>
#include "MapObject.h"

static const char* KIDText[ (int)EnigmaWC::ID::TOTAL ] =
{
  "None",             // ENone
  "WineCellar",	      // EWineCellar
  "BlockWall",        // EBlockWall
  "StoneWall",        // EStoneWall
  "Ladder",           // ELadder
  "LadderTop",        // ELadderTop
  "Person",           // EPerson
  "Blocker",          // EBlocker
  "Mover",            // EMover
  "Turner",           // ETurner
  "Facer",            // EFacer
  "Surfacer",         // ESurfacer
  "Mushrooms",        // EMushrooms
  "CorkScrew",        // ECorkScrew
  "WineBottle",       // EWineBottle
  "Bread",            // EBread
  "Cheese",           // ECheese
  "Knife",            // EKnife
  "Apple",            // EApple	
  "WineGlass",        // EWineGlass
  "PlaceMat",         // EPlaceMat
  "Grapes",           // EGrapes
  "Skull",            // ESkull
  "EdgeDrain",        // EEdgeDrain
  "AirVent",          // EAirVent
  "EasterEgg",        // EEasterEgg
  "Flipper",          // EFlipper
  "Edger",            // EEdger
  "Stairs",           // EStairs
  "StairsTop",        // EStairsTop
  "Sensor",           // ESensor
  "HandHold",         // EHandHold
  "Moss0",            // EMoss0
  "Moss90",           // EMoss90
  "Moss180",          // EMoss180
  "Moss270",          // EMoss270
  "Moss360",          // EMoss360
  "AirBalloon",       // EAirBalloon
  "Plate",            // EPlate
  "Fork",             // EFork
  "Cake",             // ECake
  "StonePost",        // EStonePost
  "StoneButton",      // EStoneButton
  "WallEyes",         // EWallEyes
  "Outdoor",          // EOutdoor
  "Indoor",           // EIndoor
  "SkyObjects",       // ESkyObjects
  "ArchWay",          // EArchWay
  "WoodDoor",         // EWoodDoor
  "WoodWall",         // EWoodWall
  "PullRing",         // EPullRing
  "CubeLock",         // ECubeLock
  "SphereLock",       // ESphereLock
  "Cube",             // ECube
  "Sphere",           // ESphere
  "WaterLayer",       // EWaterLayer
  "LightBeam",        // ELightBeam
  "Egg",              // EEgg
  "Web",              // EWeb
  "Orange",           // EOrange
  "Fish",             // EFish
  "WaterBottle",      // EWaterbottle
  "Fertilizer",       // EFertilizer
  "Tree",             // ETree
  "TreeTop",          // ETreeTop
  "CatWalk",          // ECatWalk
  "Teleporter",       // ETeleporter
  "InnerTube",        // EInnerTube
  "HydrogenBalloon",  // EHydrogenBalloon
  "Weed",             // EWeed
  "Fern",             // EFern
  "Vine",             // EVine
  "WaterLock",        // EWaterLock
  "AppleLock",        // EAppleLock
  "OrangeLock",       // EOrangeLock
  "WineLock",         // EWineLock
  "PadButton",        // EPadButton,
  "Fence",            // EFence
  "EarthWall",        // EEarthWall
  "Water"             // EWater
};

static const char* KDirectionText[ (int)EnigmaWC::Direction::TOTAL ]
{
  "ENone",     // ENone
  "North",     // ENorth
  "South",     // ESouth
  "East",      // EEast
  "West",      // EWest
  "Above",     // EAbove
  "Below",     // EBelow
  "Center"     // ECenter
};

static const char* KCategoryText[ (int)EnigmaWC::Category::TOTAL ]
{
  "None",             // ENone
  "Required Item",    // ERequired
  "Optional Item",    // EOptional
  "Easter Egg Item",  // EEasterEgg
  "Skull Item"        // ESkull
};

//*---------------------------------------------------------*
//* This method returns a text description of the MapObject *
//* (excludes the map location).                            *
//*---------------------------------------------------------*
//* aDescription: Destination buffer for text.              *
//*---------------------------------------------------------*

void CMapObject::GetDescription( Glib::ustring& aDescription )
{
  Glib::ustring IDText;

  // Select a text string corresponding to the object's ID.  Ensure an index
  // formed from the ID will not exceed the ID text array.
  
  if ( (int)iID < (int)EnigmaWC::ID::TOTAL )
    IDText = KIDText[ (int)iID ];
  else
    IDText = "???";

  // Select a text string corresponding to the object's type.
  
  Glib::ustring TypeText;

  switch( iType )
  {
    case CMapObject::Type::EObject:
      TypeText = _("Object");
      break;
	
    case CMapObject::Type::EPlayer:
      if ( iActive )
        TypeText = _("Active Player");
      else
        TypeText = _("Idle Player");
			
      break;
						
    case CMapObject::Type::EItem:
      if ( (int)iCategory < (int)EnigmaWC::Category::TOTAL )      
        TypeText = KCategoryText[ (int)iCategory ];
      else
        TypeText = "???";
        
    break;
        
    case CMapObject::Type::ETeleporter:
      TypeText = _("Teleporter");
      break;
      
    default:
      break;
  }

  // Built a text string describing teleporter information.  

  Glib::ustring SurfaceText;
  Glib::ustring RotationText;
  Glib::ustring EastText;
  Glib::ustring AboveText;
  Glib::ustring NorthText;
  Glib::ustring TeleportText;

  if ( iType == CMapObject::Type::ETeleporter )
  {
    if ( iSurfaceArrival == EnigmaWC::Direction::ENone )
      SurfaceText = _("Player");
    else if ( (int)iSurfaceArrival < (int)EnigmaWC::Direction::TOTAL )
      SurfaceText = KDirectionText[ (int)iSurfaceArrival ];
    else
      SurfaceText = "???";

    if ( iRotationArrival == EnigmaWC::Direction::ENone )
      RotationText = _("Player");
    else if ( (int)iRotationArrival < (int)EnigmaWC::Direction::TOTAL )
      RotationText = KDirectionText[ (int)iRotationArrival ];
    else
      RotationText = "???";

    if ( iLocationArrival.iEast == G_MAXUINT16 )
      EastText = _("Player");
    else
      EastText = Glib::ustring::compose( "%1", iLocationArrival.iEast );

    if ( iLocationArrival.iNorth == G_MAXUINT16 )
      NorthText = _("Player");
    else
      NorthText = Glib::ustring::compose( "%1", iLocationArrival.iNorth );

    if ( iLocationArrival.iAbove == G_MAXUINT16 )
      AboveText = _("Player");
    else
      AboveText = Glib::ustring::compose( "%1", iLocationArrival.iAbove );

    TeleportText = Glib::ustring::compose(
_("\nARRIVAL    SURFACE = %1    ROTATION = %2    EAST = %3    NORTH = %4\
    ABOVE = %5" ),
    SurfaceText, RotationText, EastText, NorthText, AboveText );
  }

  // Select a text string corresponding to the object's surface.

  if ( (int)iSurface < (int)EnigmaWC::Direction::TOTAL )
    SurfaceText = KDirectionText[ (int)iSurface ];
  else
    SurfaceText = "???";

  // Select a text string corresponding to the object's rotation.
  
  if ( iID == EnigmaWC::ID::ETurner )
  {
    // Turner are relative rotation offsets rather than compass
    // directions.
    
    switch( (int)iRotation )
    {
      case 0:
        RotationText = _("0 degrees");
        break;
        
      case 1:
        RotationText = _("90 degrees");
        break;
    
      case 2:
        RotationText = _("180 degrees");
        break;
        
      case 3:
        RotationText = _("270 degrees");
        break;
    
      default:
        RotationText = "???";
        break;
    }
  }
  else
  {
    if ( (int)iRotation < (int)EnigmaWC::Direction::TOTAL )
      RotationText = KDirectionText[ (int)iRotation ];
    else
      RotationText = "???";
  }

  // Built a text string with all object states or state signals.

  Glib::ustring SignalText;
  
  if ( iSense.size() )
  {
    if ( SignalText.size() )
      SignalText.append( "    " );
    else
      SignalText.push_back( '\n' );

    SignalText.append(_("SENSE = ") );
    SignalText.append( iSense );
  }
   
  if ( iState.size() )
  {
    if ( SignalText.size() )
      SignalText.append( "    " );
    else
      SignalText.push_back( '\n' );
  
    SignalText.append(_("STATE = ") );
    SignalText.append( iState );
  }
  
  if ( iVisibility.size() )
  {
    if ( SignalText.size() )
      SignalText.append( "    " );
    else
      SignalText.push_back( '\n' );
      
    SignalText.append(_("VISIBILITY = ") );
    SignalText.append( iVisibility );
  }
  
  if ( iPresence.size() )
  {
    if ( SignalText.size() )
      SignalText.append( "    " );
    else
      SignalText.push_back( '\n' );

    SignalText.append(_("PRESENCE = ") );
    SignalText.append( iPresence );
  } 
  
  // Assemble all text strings into one string.
  
  aDescription =
    Glib::ustring::compose(
_("ID = %1\n\
TYPE = %2    SURFACE = %3    ROTATION = %4\
%5\
%6"),
      IDText,
      TypeText,
      SurfaceText,
      RotationText,
      TeleportText,
      SignalText );

  return;
}