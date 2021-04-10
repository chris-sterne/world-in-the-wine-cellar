/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 5.1 for Linux OS                    *
 * File:    HelpView.cpp                        *
 * Date:    October 8, 2017                     *
 * Author:  Chris Sterne                        *
 *                                              *
 * HelpView class.                              *
 *----------------------------------------------*
 * This class displays program instructions.    *
 *----------------------------------------------*/

#include <glibmm/i18n.h>
#include "HelpView.h"

//*-----------------------------*
//* Local declarations and data *
//*-----------------------------*

const char* KHelpText =_("\
MAP MAKER for ENIGMA IN THE WINE CELLAR\n\
Version 5.1 for Linux OS\n\
By: Chris Sterne\n\
Date: September, 2017\n\
\n\
FUNCTION KEYS\n\
Toggle between Level viewer and other viewers.\n\
F1:  Room contents viewer.\n\
F2:  Map teleporters viewer.\n\
F3:  Map items viewer.\n\
F4:  Map players viewer.\n\
F5:  Map controllers viewer.\n\
F6:  Map control editor.\n\
F7:  Map description editor.\n\
F8:  Help.\n\
\n\
LEVEL VIEWER KEYS\n\
Cursor Left/Right: Move cursor West/East.\n\
Cursor Up/Down: Move cursor North/South.\n\
Page Up:  Move Above.\n\
Page Down: Move Below.\n\
Shift with Cursor Left/Right/Up/Down, Page Up/Down: Mark volume.\n\
Shift or Escape: Unmark volume.\n\
Control X: Cut volume.\n\
Control C: Copy volume.\n\
Control V: Paste volume.\n\
\n\
CONTROLLER VIEWER KEYS\n\
Delete: Delete selected controller (requires confirmation)\n\
\n\
CODE EDITOR KEYS\n\
Esc: Compile code and return to Map Controller list.\n\
\n\
ROOM, TELEPORTERS, ITEMS, PLAYERS VIEWER KEYS\n\
Cursor Up/Down: Select object.\n\
Delete: Delete selected object.\n\
\n\
COMMAND LINE\n\
Insert Map Object: o id='code' typ='type' surf='surface' rot='rotation' \
sens='signal' stat='signal' visi='signal' pres='signal' \
stel='teleport surface' rtel='teleport rotation' etel='teleport east' \
ntel='teleport north' atel='teleport above'\n\
Filter Level Viewer object: f 'code'\n\
Add Map Controller: c 'name'\n\
Load game map: l 'filename'\n\
Save game map: s 'filename'\n\
Reload game map: l\n\
Resave game map: s\n\
Begin new named game map: n 'filename'\n\
Begin new unnamed game map: n\n\
Quit: q\n\
\n\
SIGNAL FORMAT\n\
Connection: 'controller_name:signal_name'\n\
True value: 'T'\n\
False value: 'F'\n\
\n\
OBJECT ID CODES\n\
AirBalloon: airballoon\n\
AirVent: vent\n\
Apple: apple\n\
AppleLock: aplk\n\
ArchWay: arch\n\
Blocker: blkr\n\
BlockWall: bkw\n\
Bread: bread\n\
Cake: cake\n\
CatWalk: cat\n\
Cheese: cheese\n\
Corkscrew: corkscrew\n\
Cube: cube\n\
CubeLock: culk\n\
EarthWall: erw\n\
EasterEgg: easteregg\n\
EdgeDrain: egdn\n\
Edger: eger\n\
Egg: egg\n\
Facer: face\n\
Fence: fenc\n\
Fern: frn\n\
Fertilizer: fertilizer\n\
Fish: fish\n\
Flipper: flip\n\
Fork: fork\n\
Grapes: grapes\n\
HandHold: hold\n\
HydrogenBalloon: hballoon\n\
Indoor: in\n\
InnerTube: innertube\n\
Knife: knife\n\
Ladder: lad\n\
LadderEnd: lnd\n\
LightBeam: lbm\n\
Moss0: m0\n\
Moss90: m90\n\
Moss180: m180\n\
Moss270: m270\n\
Moss360: m360\n\
Mover: mov\n\
Mushrooms: mush\n\
Orange: orange\n\
OrangeLock: orlk\n\
Outdoor: out\n\
PadButton: pab\n\
Person: person\n\
PlaceMat: placemat\n\
Plate: plate\n\
PullRing: ring\n\
Sensor: sens\n\
Skull: skull\n\
Sphere: sphere\n\
SphereLock: splk\n\
Stairs: str\n\
StairsTop: stp\n\
StoneButton: snb\n\
StonePost: snp\n\
StoneWall: snw\n\
Surfacer: surf\n\
Teleporter: tel\n\
Tree: tre\n\
TreeTop: ttp\n\
Turner: turn\n\
Vine: vin\n\
WallEyes: eyes\n\
Water: wat\n\
WaterBottle: waterbottle\n\
WaterLock: walk\n\
WaterLayer: watl\n\
Web: web\n\
Weed: wed\n\
WineBottle: winebottle\n\
WineCellar: cellar\n\
WineGlass: wineglass\n\
WineLock: wilk\n\
WoodDoor: dor\n\
WoodWall: wdw");

//*--------------------------*
//* C++ default constructor. *
//*--------------------------*

CHelpView::CHelpView()
{
  // Prepare the TextView widget.

  Gtk::TextView* TextView = Gtk::manage( new Gtk::TextView );
  add( *TextView );
	
  TextView->set_wrap_mode( Gtk::WRAP_WORD );
  TextView->set_editable(FALSE);
  TextView->set_cursor_visible(FALSE);
  TextView->set_hexpand(TRUE);
  TextView->set_vexpand(TRUE);

  // Add help text to the TextView built-in buffer.
	
 	Glib::RefPtr<Gtk::TextBuffer> Buffer = TextView->get_buffer();
  Buffer->set_text( KHelpText );
  return;
}
