/*------------------------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker               *
 * Version: 5.1 for Linux OS                                  *
 * File:    PDBMap.h                                          *
 * Date:    October 4, 2017                                   *
 * Author:  Chris Sterne                                      *
 *                                                            *
 * This header contains definitions for old version game maps *
 * in Palm OS PDB database format.                            *
 *------------------------------------------------------------*/

#ifndef PDBMAP_H
#define PDBMAP_H

//*-------------------------------------------------------------------------*
//* Miscellaneous data types introduced for porting Mapper from PC version. *
//*-------------------------------------------------------------------------*

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef bool BOOL;
typedef unsigned int DWORD;

//*---------------------------------------------------*
//* Definitions for the PalmOS PDB database map file. *
//*---------------------------------------------------*

#define V3DatabaseName			"EnigmaWCDB"
#define DatabaseNamePrefix	"EnigmaWC_"
#define V3DatabaseVersion		1
#define DatabaseVersion			3

#define PDB_HeaderSize				(32 + (2*2) + (4*9))
#define PDB_RecordHeaderSize	(4 + 2)
#define PDB_RecordEntrySize		( 4 + 1 + 3 )
#define PDB_PlaceHolderBytes  2

#define	dmHdrAttrBundle	0x0800	// PalmOS database flag.
#define	dmHdrAttrBackup	0x0008	// PalmOS database flag.

#define CAT_Description 0x01		// Category of a map description record.
#define CAT_Control			0x02		// Category of a game control record
#define CAT_Players			0x03		// Category of a game players record.
#define CAT_Items				0x04		// Category of a map items record.
#define CAT_Devices			0x05		// Category of a map Devices record.
#define CAT_Level 			0x06		// Category of a map level record.

struct PDBMapObject
{
  BYTE ID;
  BYTE Data;
};

#define PDBControl_NoSave			0		// PDBMapControl flag bit numbers;
#define PDBControl_Unlocked		1
#define PDBControl_V4Style		2

struct PDBMapControl
{
	WORD Flags;
};

#define PDBItem_Optional	0				// PDBItemEntry Bit numbers.
#define PDBItem_Found			1
#define PDBItem_Used			2

struct PDBItemEntry
{
	BYTE ID;
	BYTE Flags;
	BYTE DefaultFlags;
	BYTE SavedFlags;
};

#define PDBMaximumPlayers	4

#define PDBPlayer_Active				0		// Active player flag bits.
#define PDBPlayer_DefaultActive	1
#define PDBPlayer_SavedActive		2

struct PDBPlayerEntry
{
	BYTE ID;
	BYTE Flags;

	struct PDBPlayerState
	{
		BYTE Level;
		BYTE Row;
		BYTE Column;	
		BYTE Compass;
	} State;

	struct PDBPlayerState Default;
	struct PDBPlayerState Saved;
};

#define PDBDeviceNameLength		4							// Device name length.

#define PDBDevice_BUFFER  0
#define PDBDevice_NOT			1									// Device types.
#define PDBDevice_AND			2
#define PDBDevice_OR			3
#define PDBDevice_TOGGLE	4
#define PDBDevice_LATCH		5
#define PDBDevice_SHIFT		6
#define PDBDevice_NOR			7

#define PDBDevice_TypeMask			0x07				// Device type.

#define PDBDevice_DataPrestore	3						// Control bit numbers.
#define PDBDevice_ClockState		4	
#define PDBDevice_State					5
#define PDBDevice_DefaultState	6
#define PDBDevice_SavedState		7

#define PDBDevice_SectionMask		0x7F				// Isolate device section.

#define PDBDevice_PlayerState		7						// Section bit numbers.

#define PDBDevice_Player	254
#define PDBDevice_Unused	255

struct PDBDeviceEntry
{
	BYTE Section;
	BYTE Control;
	char Name[ PDBDeviceNameLength ];
	
	union
	{
		struct GeneralInputs
		{
			BYTE In1;
			BYTE In2;
			BYTE In3;
			BYTE In4;
		} General;
		
		struct BUFFERInputs
		{
			BYTE In;
			BYTE Unused1;
			BYTE Unused2;
			BYTE Unused3;
		} BUFFER;
		
		struct NOTInputs
		{
			BYTE In;
			BYTE Unused1;
			BYTE Unused2;
			BYTE Unused3;
		} NOT;
		
		struct ANDInputs
		{
			BYTE In1;
			BYTE In2;
			BYTE In3;
			BYTE In4;
		} AND;

		struct ORInputs
		{
			BYTE In1;
			BYTE In2;
			BYTE In3;
			BYTE In4;
		} OR;

		struct TOGGLEInputs
		{
			BYTE Clock;
			BYTE Set;
			BYTE Clear;
			BYTE Unused;
		} TOGGLE;
		
		struct LATCHInputs
		{
			BYTE Clock;
			BYTE Set;
			BYTE Clear;
			BYTE Data;
		} LATCH;

		struct SHIFTInputs
		{
			BYTE Clock;
			BYTE Set;
			BYTE Clear;
			BYTE Data;
		} SHIFT;

		struct NORInputs
		{
			BYTE In1;
			BYTE In2;
			BYTE In3;
			BYTE In4;
		} NOR;

	} Inputs;
};

namespace PDB
{
  enum class ObjectClass
  {
  	fmt=0,
  	obj
  };

  enum class ID
  {
  	// Visible object IDs.
  	
  	dor = 0,					// WoodDoor
  	bkw,							// BlockWall
  	snc,							// StoneCeiling
  	snf,							// StoneFloor
	  cellar,						// WineCellar
  	ring,							// PullRing
	  culk,							// CubeLock
  	splk,							// SphereLock
  	egdn,							// EdgeDrain
  	rlic,							// RightLichen
  	llic,							// LeftLichen
  	eyes,							// WallEyes
  	cweb,							// CeilingWeb
  	fweb,							// FloorWeb
	  lbm,							// LightBeam
  	bar,							// HandBar
	  lad,							// Ladder
  	tre,							// Tree
  	cat,							// CatWalk
  	vent,							// AirVent
  	post,							// WoodPost
	  watl,					    // WaterLayer
  	corkscrew,				// CorkScrew item
	  mush,							// Mushrooms
  	winebottle,				// WineBottle item
  	waterbottle,			// WaterBottle item
  	bread,						// Bread item
	  cheese,						// Cheese item
	  knife,						// Knife item
	  apple,						// Apple item
	  orange,						// Orange item
	  fish,							// Fish item
	  wineglass,				// Wineglass item
  	placemat,					// Placemat item
  	grapes,						// Grapes item
  	balloon,					// Balloon item
  	plate,						// Plate item
  	fork,							// Fork item
  	cake,							// Cake item
  	boiledegg,				// BoiledEgg item
  	easteregg,				// EasterEgg item
  	cube,		    			// Cube item
  	sphere,		    		// Sphere item
  	innertube,				// InnerTube item.	
  	skull,						// Skull item
  	str,							// Stairs
  	arch,							// ArchWay
  	snw,							// StoneWall
  	wdw,							// WoodWall
  	wdc,							// WoodCeiling
   	wdf,							// WoodFloor
  	weed,							// WaterWeed
  	play,							// Player
  	walk,							// WaterLock
  	aplk,							// AppleLock
  	orlk,							// OrangeLock
  	wilk,							// WineLock
  	fertilizer,				// Fertilizer
  	fern,							// Fern
  	vin,							// Vine
  	fsw,							// FloorSwitch
  	tel,							// Teleporter
  	out,							// Outdoor
  	in,								// Indoor
  	fenc,							// Fence
  	horizontaledge,		// HorizontalEdge
  	verticaledge,			// VerticalEdge
  	erf,							// EarthFloor
	  hyballoon,        // HydrogenBalloon
  
    // Invisible Object IDs.
 
  	pres = 255-21,		// PressureSensor object
  	ltel,							// Level Teleporter object.
  	rtel,							// Row Teleporter object.
  	ctel,							// Column Teleporter object.
  	watr,							// Water object.
  	v3play,						// V3 game map Player object.
  	stp,							// StairsTop object.
	  lnd,							// LadderEnd object.
  	ttp,							// TreeTop object
  	mots,							// MotionSensor object.
  	mov,							// Mover object.	
  	face,							// Facer object.
  	blkr,							// Blocker object.
  	trn,							// Turner object.
  	dim,							// Dimmer object.
  	dev,							// Device object.
  	item,							// Item object.
  	col,							// Map column.
  	row,							// Map row
  	lvl,							// Map level.
  	end,							// End map level.
  	bgn								// Begin map level.
  };

  enum class MapPosition
  {
  	n = 0,      // North.
  	s,          // South.
  	e,          // East.
  	w,          // West.
  	a,          // Above.
  	b,          // Below.
  	c           // Center.
  };
};

#endif  // __PDBMAP_H__
