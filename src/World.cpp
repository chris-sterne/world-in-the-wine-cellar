// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file contains the World class implementation.  The World class
// describes a game world. 
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
#include "World.h"

//*-----------------------------------------------------*
//* Default constructor.  All game map data is cleared. *
//*-----------------------------------------------------*

Enigma::World::World()
{	
  m_filename.clear();
  clear();
}

//*-------------------------------------------------------------*
//* This private function writes a KeyValue with a 16-bit value *
//* to a buffer.  This function is used for new version maps.   *                                                *
//*-------------------------------------------------------------*
//* aBuffer: Destination buffer.                                *
//* aKey:    KeyValue key.                                      *
//* aValue:  16-bit value.                                      *
//*-------------------------------------------------------------*

void WriteKeyValue_16Bit( std::string& aBuffer,
                          EnigmaWC::Key aKey,
                          guint16 aValue )
{
  aBuffer.push_back( (guchar)aKey );
  aBuffer.push_back( (guchar)aValue );

  // Write a bank keyvalue if the level exceeds that for a byte.
			
  if ( aValue > G_MAXUINT8 )
  {
    aBuffer.push_back( (guchar)EnigmaWC::Key::EBank );
    aBuffer.push_back( (guchar)(aValue >> 8) );
  }
  
  return;
}

//*-------------------------------------------------------------*
//* This private function writes a KeyValue with an 8-bit value *
//* to a buffer.  This function is used for new version maps.   *
//*-------------------------------------------------------------*
//* aBuffer: Destination buffer.                                *
//* aKey:    KeyValue key.                                      *
//* aValue:  8-bit value.                                       *
//*-------------------------------------------------------------*

void WriteKeyValue_8Bit( std::string& aBuffer,
                         EnigmaWC::Key aKey,
                         guint8 aValue )
{
  aBuffer.push_back( (guchar)aKey );
  aBuffer.push_back( (guchar)aValue );  
  return;
}

//*--------------------------------------------------------------*
//* This private function writes a KeyValue with a boolean value *
//* to a buffer.  This function is used for new version maps.    *
//*--------------------------------------------------------------*
//* aBuffer: Destination buffer.                                 *
//* aKey:    KeyValue key.                                       *
//* aValue:  boolean value.                                      *
//*--------------------------------------------------------------*

void WriteKeyValue_Boolean( std::string& aBuffer,
                            EnigmaWC::Key aKey,
                            gboolean aValue )
{
  aBuffer.push_back( (guchar)aKey );
  
  if ( aValue )
    aBuffer.push_back( (guchar)1 );
  else
    aBuffer.push_back( (guchar)0 );
    
  return;
}

//*-------------------------------------------------------------*
//* This private function writes a KeyValue followed by a short *
//* string (255 character maximum) to a buffer.  This function  *
//* is used for new version maps.                               *
//*-------------------------------------------------------------*
//* aBuffer: Destination buffer.                                *
//* aKey:    KeyValue key.                                      *
//* aString: Reference to string to be written.                 *
//*-------------------------------------------------------------*

void WriteKeyValue_String( std::string& aBuffer,
                           EnigmaWC::Key aKey,
                           std::string& aString )
{
  // Write KeyValue if the string is not empty.
  
  if ( aString.size() )
  {
    aBuffer.push_back( (guchar)aKey );
    aBuffer.push_back( (guint8)aString.size() );                          
    aBuffer.append( aString );
  }
    
  return;
}

//*--------------------------------------------------------------------*
//* This private function returns the number of bytes in a string line *
//* of a line ending with the newline '\n' or \x00 characters.         *  
//*--------------------------------------------------------------------*
//* RETURN: Line length, or zero if no line is available.              *
//*--------------------------------------------------------------------*

guint FindLine( std::string& aSource, guint aIndex )
{
  // Count the number of characters before the line terminator.
  
  guint Index = aIndex;
  
  while (( Index < aSource.size() )
    && ( aSource.at( Index ) != '\n' )
    && ( aSource.at( Index ) != '\x00' ))
  {
    ++ Index;
  }

  // Include the line terminator in the final count, unless the end  of the
  // source string has been reached.

  if ( Index >= aSource.size() )
    return ( Index - aIndex );
  else
    return ( 1 + Index - aIndex );
}

//*-------------------------------------------------------------*
//* This private function extracts a MapObject from new version *
//* game map data.                                              *
//*-------------------------------------------------------------*
//* aFileData: Keyvalue stream.                                 *
//* aIndex:    KeyValue index.                                  *
//* aObject:   Reference to MapObject.                          *
//* aMap:      Reference to save map Savable state.             *
//* RETURN:    MapObject filled with keyvalue information.      *
//*-------------------------------------------------------------*

void ExtractNewObject( const std::string& aFileData,
                       guint& aIndex,
                       CMapObject& aObject,
                       gboolean& aSavable )
{
  // Clear all Object connections, but leave intact the remaining
  // information from a previous Object.  This allows minimizing
  // the number of keyvalue pairs uses for describing all Objects.
  
  aObject.iSense.resize(0);
  aObject.iState.resize(0);
  aObject.iVisibility.resize(0);
  aObject.iPresence.resize(0);

  // Clear the Teleporter arrival orientation so it uses the player's
  // current orientation in the game.  The presence of Teleporter arrival
  // keysvalues will change this default orientation.
  
  aObject.iSurfaceArrival  = EnigmaWC::Direction::ENone;
  aObject.iRotationArrival = EnigmaWC::Direction::ENone;

  aObject.iLocationArrival.iEast  = G_MAXUINT16;  
  aObject.iLocationArrival.iNorth = G_MAXUINT16;
  aObject.iLocationArrival.iAbove = G_MAXUINT16;
  
  // Record information from the header keyvalue, then skip over the header.
  // The calling function ensures the presence of a complete header.

  switch( (EnigmaWC::Key)aFileData.at( aIndex ) )
  {
    case EnigmaWC::Key::EItem:
      aObject.iType = CMapObject::Type::EItem;
      break;
      
    case EnigmaWC::Key::EPlayer:
      aObject.iType = CMapObject::Type::EPlayer;
      break;
  
    case EnigmaWC::Key::ETeleporter:
      aObject.iType = CMapObject::Type::ETeleporter;
      break;
  
    default:
      aObject.iType = CMapObject::Type::EObject;
      break;
  }

  ++ aIndex;
  aObject.iID = (EnigmaWC::ID)aFileData.at( aIndex );
  ++ aIndex;
  
  // Record Object information from the keyvalue stream until an element
  // header or keyvalue sequence terminator is encountered.
 
  EnigmaWC::Key GroupState  = EnigmaWC::Key::ENone;
  EnigmaWC::Key MemberState = EnigmaWC::Key::ENone;
  EnigmaWC::Key Key;
  guint8 Value;
  guint16 HighValue;
  gboolean Done = FALSE;
  
  while ((( aFileData.size() - aIndex ) >= 2 ) && !Done )
  {
    Key = (EnigmaWC::Key)aFileData.at( aIndex );
    aIndex ++;

    Value = aFileData.at( aIndex );
    aIndex ++;

    switch( Key )
    {
      case EnigmaWC::Key::ESurface:      
        // A group of Surface keyvalues are arriving.
        
        aObject.iSurface = (EnigmaWC::Direction)Value;
        GroupState  = Key;
        MemberState = Key;
        break;
      
      case EnigmaWC::Key::ERotation:
        // A group of Rotation keyvalues are arriving.
        
        aObject.iRotation = (EnigmaWC::Direction)Value;
        GroupState  = Key;
        MemberState = Key;
        break;

      case EnigmaWC::Key::EEast:
        // A group of East location keyvalues are arriving.
                
        aObject.iLocation.iEast = (guint16)Value;
        GroupState  = Key;
        MemberState = Key;
        break;

      case EnigmaWC::Key::ENorth:
        // A group of North location keyvalues are arriving.
      
        aObject.iLocation.iNorth = (guint16)Value;
        GroupState  = Key;
        MemberState = Key;
        break;
      
      case EnigmaWC::Key::EAbove:
        // A group of Above location keyvalues are arriving.

        aObject.iLocation.iAbove = (guint16)Value;
        GroupState  = Key;
        MemberState = Key;
        break;
      
      case EnigmaWC::Key::EActive:
        // This keyvalue is used by Item and Player objects.
        
        aObject.iActive = (gboolean)Value;
        break;
        
      case EnigmaWC::Key::ECategory:
        // This keyvalue is used by Item objects.
                
        aObject.iCategory = (EnigmaWC::Category)Value;
        break;

      case EnigmaWC::Key::EBank:
        // The value of a previous keyvalue is to have a high byte added.
        // Adding only one extra bank is supported, extending the value
        // to a maximum of 16 bits.
                
        HighValue = (guint16)( Value << 8 );

        if ( GroupState == EnigmaWC::Key::EEast )
        {
          if ( MemberState == EnigmaWC::Key::EEast )
            aObject.iLocation.iEast |= HighValue;
          else if ( MemberState == EnigmaWC::Key::EArrival )
            aObject.iLocationArrival.iEast |= HighValue;
        }
        else if ( GroupState == EnigmaWC::Key::ENorth )
        {
          if ( MemberState == EnigmaWC::Key::ENorth )
            aObject.iLocation.iNorth |= HighValue;
          else if ( MemberState == EnigmaWC::Key::EArrival )
            aObject.iLocationArrival.iNorth |= HighValue;
        }
        else if ( GroupState == EnigmaWC::Key::EAbove )
        {
          if ( MemberState == EnigmaWC::Key::EAbove )
            aObject.iLocation.iAbove |= HighValue;
          else if ( MemberState == EnigmaWC::Key::EArrival )
            aObject.iLocationArrival.iAbove |= HighValue;
        }
                   
        break;

      case EnigmaWC::Key::EArrival:
        // This is the teleport arrival value member for a group of values.
        // Select the appropriate destination.
        
        if ( GroupState == EnigmaWC::Key::ESurface )
          aObject.iSurfaceArrival = (EnigmaWC::Direction)Value;
        else if ( GroupState == EnigmaWC::Key::ERotation )
          aObject.iRotationArrival = (EnigmaWC::Direction)Value;
        else if ( GroupState == EnigmaWC::Key::EEast )
          aObject.iLocationArrival.iEast = (guint16)Value;
        else if ( GroupState == EnigmaWC::Key::ENorth )
          aObject.iLocationArrival.iNorth = (guint16)Value;
        else if ( GroupState == EnigmaWC::Key::EAbove )
          aObject.iLocationArrival.iAbove = (guint16)Value;
  
        MemberState = Key;
        break;

      case EnigmaWC::Key::ESense:
        // An object state signal name follows this keyvalue pair.  Copy
        // the name to a string buffer.  Value is the name's length.

        aObject.iSense.assign( aFileData, aIndex, Value );
        aIndex += Value;
        break;
        
      case EnigmaWC::Key::EState:
        // An object state signal name follows this keyvalue pair.  Copy
        // the name to a string buffer.  Value is the name's length.

        aObject.iState.assign( aFileData, aIndex, Value );
        aIndex += Value;
        break;
        
      case EnigmaWC::Key::EVisibility:
        // An object state signal name follows this keyvalue pair.  Copy
        // the name to a string buffer.  Value is the name's length.

        aObject.iVisibility.assign( aFileData, aIndex, Value );
        aIndex += Value;
        break;
        
      case EnigmaWC::Key::EPresence:
        // An object state signal name follows this keyvalue pair.  Copy
        // the name to a string buffer.  Value is the name's length.

        aObject.iPresence.assign( aFileData, aIndex, Value );
        aIndex += Value;
        break;
      
      case EnigmaWC::Key::ESaved:
        // The presence of a Saved key means the game map can be saved
        // while being played.
        
        aSavable = TRUE;
        break;
      
      case EnigmaWC::Key::EObject:
      case EnigmaWC::Key::ETeleporter:
      case EnigmaWC::Key::EPlayer:
      case EnigmaWC::Key::EItem:
      case EnigmaWC::Key::EDescription:
      case EnigmaWC::Key::EController:
      case EnigmaWC::Key::EEnd:
        // An element keyvalue or array end keyvalue has been encountered.
        // Move the array index back to this keyvalue before exiting.
			
        aIndex -= 2;
        Done = TRUE;
      break;
       
      default:
        break;
    }
  }
  
  return;
}

//*---------------------------------------------------------------*
//* This private function extracts a Description from new version *
//* game map data.                                                *
//*---------------------------------------------------------------*
//* aFileData: Keyvalue stream.                                   *
//* aIndex:    KeyValue index.                                    *
//* aObject:   Reference to text buffer.                          *
//* RETURN:    Text buffer filled with description text.          *
//*---------------------------------------------------------------*

void ExtractNewDescription( const std::string& aFileData,
                            guint& aIndex,
                            Glib::ustring& aDescription )
{
  // Skip over the header keyvalue.  At present, the language value
  // is ignored.
  
  aIndex += 2;
  
  // Record Description information from the keyvalue array until
  // an element header or keyvalue array terminator is encountered.
 
  EnigmaWC::Key BankState = EnigmaWC::Key::ENone;
  EnigmaWC::Key Key;
  guint8 Value;
  guint16 Length = 0;
  gboolean Done  = FALSE;
  
  while ((( aFileData.size() - aIndex ) >= 2 ) && !Done )
  {
    Key = (EnigmaWC::Key)aFileData.at( aIndex );
    aIndex ++;

    Value = aFileData.at( aIndex );
    aIndex ++;

    switch( Key )
    {
      case EnigmaWC::Key::ELength:      
        Length = (guint16)Value;
        BankState = Key;
        break;
    
      case EnigmaWC::Key::EData:
        // The keyvalue is followed by a block of description data.
        
        aDescription = aFileData.substr( aIndex, Length );
        aIndex += Length;
        break;
        
      case EnigmaWC::Key::EBank:
        // The value of a previous keyvalue is to have a high byte added.
        // Adding only one extra bank is supported, extending the value
        // to a maximum of 16 bits.

        if ( BankState == EnigmaWC::Key::ELength )
          Length |= (guint16)( Value << 8 );
                    
        break;
        
      case EnigmaWC::Key::EObject:
      case EnigmaWC::Key::ETeleporter:
      case EnigmaWC::Key::EPlayer:
      case EnigmaWC::Key::EItem:
      case EnigmaWC::Key::EDescription:
      case EnigmaWC::Key::EController:
      case EnigmaWC::Key::EEnd:
        // An element keyvalue or array end keyvalue has been encountered.
        // Move the array index back to this keyvalue before exiting.
			
        aIndex -= 2;
        Done = TRUE;
        break;
        
      default:
        break;
    }
  }
  
  return;
}

//*--------------------------------------------------------------*
//* This private function extracts a Controller from new version *
//* game map data.                                               *
//*--------------------------------------------------------------*
//* aFileData: Keyvalue stream.                                  *
//* aIndex:    KeyValue index.                                   *
//* aObject:   Reference to MapController.                       *
//* RETURN:    MapController filled with keyvalue information.   *
//*--------------------------------------------------------------*

void ExtractNewController( const std::string& aFileData,
                           guint& aIndex,
                           CMapController& aController )
{
  // A controller name follows the new element keyvalue pair.
  // Prepare the controller to receive new information.
 
  ++ aIndex;
  guint8 Value = aFileData.at( aIndex );
  ++ aIndex;
  
  aController.iName = aFileData.substr( aIndex, Value );
  aController.iSignalNames.resize(0);
  aController.iRestartCode.resize(0);
  aController.iMainCode.resize(0);
  
  aIndex += Value;
  
  // Record Controller information from the keyvalue array until
  // an element header or keyvalue array terminator is encountered.
 
  EnigmaWC::Key BankState = EnigmaWC::Key::ENone;
  EnigmaWC::Key DataState = EnigmaWC::Key::ENone;
  EnigmaWC::Key Key;
  guint16 Length = 0;
  gboolean Done  = FALSE;
  
  while ((( aFileData.size() - aIndex ) >= 2 ) && !Done )
  {
    Key = (EnigmaWC::Key)aFileData.at( aIndex );
    aIndex ++;

    Value = aFileData.at( aIndex );
    aIndex ++;

    switch( Key )
    {
      case EnigmaWC::Key::ECode:
      case EnigmaWC::Key::ECurrent:
      case EnigmaWC::Key::ESaved:      
      case EnigmaWC::Key::ERestart:
      case EnigmaWC::Key::ESignal:
        // These keys indicates a controller bytecode block or signal
        // name block.  Some of these blocks not be loaded, but the
        // DataState key must still be updated for use by the EData
        // keyvalue later.
      
        DataState = Key;
        break;
    
      case EnigmaWC::Key::ELength:      
        Length = (guint16)Value;
        BankState = Key;
        break;
        
      case EnigmaWC::Key::EBank:
        // The value of a previous keyvalue is to have a high byte added.
        // Adding only one extra bank is supported, extending the value
        // to a maximum of 16 bits.

        if ( BankState == EnigmaWC::Key::ELength )
          Length |= (guint16)( Value << 8 );
                    
        break;
    
      case EnigmaWC::Key::EData:
        // The keyvalue is followed by a block of data.  Only current Code,
        // restart Code, and Signal names are loaded.

        if ( DataState == EnigmaWC::Key::ECode )
          aController.iMainCode.assign( aFileData, aIndex, Length );
        else if ( DataState == EnigmaWC::Key::ERestart )
          aController.iRestartCode.assign( aFileData, aIndex, Length );
        else if ( DataState == EnigmaWC::Key::ESignal )
          aController.iSignalNames.assign( aFileData, aIndex, Length );
        
        // Move the array index over the data block.
        
        aIndex += Length;
        break;
    
      case EnigmaWC::Key::EObject:
      case EnigmaWC::Key::ETeleporter:
      case EnigmaWC::Key::EPlayer:
      case EnigmaWC::Key::EItem:
      case EnigmaWC::Key::EDescription:
      case EnigmaWC::Key::EController:
      case EnigmaWC::Key::EEnd:
        // An element keyvalue or array end keyvalue has been encountered.
        // Move the array index back to this keyvalue before exiting.
			
        aIndex -= 2;
        Done = TRUE;
        break;
        
      default:
        break;
    }
  }
}

//*--------------------------------------------------------------------*
//* This method clears data in the game map, but retains the filename. *
//*--------------------------------------------------------------------*

void CMap::Clear()
{	
  iControllers.clear();
  iObjects.Clear();
  iPlayers.Clear();
  iItems.Clear();
  iTeleporters.Clear();
  iDescription.clear();
  
  // Initialize instance variables.
  
  iSavable = FALSE;
  return;
}

//*--------------------------------------------------------------------*
//* This method loads a game map from a file.  The filename extension  *
//* is used to determinine if the game map is an older version in Palm *
//* PDB format (.pdb) or a new version (.ewc).                         *
//*--------------------------------------------------------------------*

void CMap::Load()
{	
  // Clear old data in map, but retain the filename. 

  Clear();

  // Load game map file.  Return if unsuccessful.

  std::string FileData;
  
  try
  {
    FileData = Glib::file_get_contents( iFileName );
  }
    catch( Glib::Error error )
  {
    return;
  }

  // Choose the appropriate extraction method.

  if ( iFileName.find( ".ewc" ) != Glib::ustring::npos )
    ExtractNewMap( FileData );
  else if ( iFileName.find( ".pdb" ) != Glib::ustring::npos )
    ExtractOldMap( FileData );

  return;
}

//*------------------------------------------------------------------------*
//* This method extracts information from new format game maps.  A loaded  *
//* group of Saved, Current, and Restart values will have only the Restart *
//* values recorded, so all group values will be the same as the Restart   *
//* value when saved to a game map file.                                   *
//*------------------------------------------------------------------------*
//* aFileData: Raw game map file data.                                     *
//*------------------------------------------------------------------------*

void CMap::ExtractNewMap( std::string& aFileData )
{ 
  // Confirm that the first line of the game map file has the proper
  // indentification code.

  if ( !aFileData.compare( "ewc\n" ) )
    return;

  // Confirm that the file header has the correct ending.
	
  gboolean EndHeader = FALSE;
  guint Index = 0;
  guint Size  = 0;
  
  do
  {
    Size = FindLine( aFileData, Index );
    
    if ( aFileData.compare( Index, Size, "end_header\n" ) == 0 )
    {
      EndHeader = TRUE;
      break;
    }
    else
      Index += Size;
  }
  while ( Size != 0 );

  // Exit if the end of the header was not found.

  if ( !EndHeader )
    return;

  // Skip over the header to the start of the map elements.
  
  Index += Size;

  // Initialize a MapObject to receive keyvalue array information.

  CMapObject Object;
  
  Object.iType     = CMapObject::Type::EObject;
  Object.iID       = EnigmaWC::ID::ENone;
  Object.iSurface  = EnigmaWC::Direction::ENone;
  Object.iRotation = EnigmaWC::Direction::ENone;

  Object.iLocation.iEast  = 0;  
  Object.iLocation.iNorth = 0;
  Object.iLocation.iAbove = 0;

  // Read information from the keyvalue array.
  
  gboolean ValidData = TRUE;
  gboolean Done      = FALSE;
  EnigmaWC::Key Key;
  guint8 Value;
  
  while ((( aFileData.size() - Index ) >= 2 ) && !Done )
  {
    // A complete keyvalue pair is available.  Read the keyvalue,
    // but keep the keyvalue array index on the element header. 
  
    Key   = (EnigmaWC::Key)aFileData.at( Index );
    Value = (guint8)aFileData.at( Index + 1 );
    
    switch( Key )
    {
      case EnigmaWC::Key::EObject:
      case EnigmaWC::Key::ETeleporter:
      case EnigmaWC::Key::EItem:
      case EnigmaWC::Key::EPlayer:

        // An Object, Teleporter, Item or Player element header has been
        // encountered.
        
        ExtractNewObject( aFileData, Index, Object, iSavable );
        
        if  (( (int)Object.iID < (int)EnigmaWC::ID::TOTAL )
          && ( (int)Object.iSurface < (int)EnigmaWC::Direction::TOTAL )
          && ( (int)Object.iRotation < (int)EnigmaWC::Direction::TOTAL ))
        {            
          // The MapObject has been filled with valid data.  Add a new
          // object to the appropriate list.
          
          if ( Key == EnigmaWC::Key::EItem )
            iItems.push_back( Object );
          else if ( Key == EnigmaWC::Key::EPlayer )
            iPlayers.push_back( Object );
          else if ( Key == EnigmaWC::Key::ETeleporter )
            iTeleporters.push_back( Object );
          else
            iObjects.push_back( Object );
        }
        else
        {
          // The MapObject has some invalid data.  Force an exit with an
          // invalid data error.
        
          ValidData = FALSE;
          Done      = TRUE;
        }
          
        break;
			
			case EnigmaWC::Key::EDescription:
        // A Description element header has been encountered.
        
        ExtractNewDescription( aFileData, Index, iDescription );
        break;
        
      case EnigmaWC::Key::EController:
        // A Controller element header has been encountered.
        
        iControllers.emplace_back();
        ExtractNewController( aFileData, Index, iControllers.back() );
        break;
			
			case EnigmaWC::Key::EEnd:
			  // The end of the keyvalue array has been found.  Ensure it has
			  // the correct value, and is last in the keyvalue array.
            
        if (( Value != 0 ) || (( aFileData.size() - Index ) != 2 ))
          ValidData = FALSE;
			  
			  Done = TRUE;
			  break;
			
      default:
        // An unrecognized element header has been encountered.  Force
        // an exit with an invalid data error.
        
        ValidData = FALSE;
        Done      = TRUE;
        break;
    }
  }

  // If there was a data error, clear all saved data.  The game map file
  // may be faulty.

  if ( !ValidData )    
    Clear();

  return;
}

//------------------------------------------
// This method saves the game map to a file.
//------------------------------------------

void Enigma::World::save()
{	
  //-------------------------
  // Write a game map header.
  //-------------------------

	std::string FileData =
"ewc\n\
format binary_byte 1.0\n\
comment Enigma in the Wine Cellar 5.0 game map, created by MapperWC 5.0\n";

	FileData.append( "element object " );
	FileData.append( std::to_string( iObjects.size() ));
	FileData.push_back( '\n' );

	FileData.append( "element teleporter " );
	FileData.append( std::to_string( iTeleporters.size() ));
	FileData.push_back( '\n' );

	FileData.append( "element player " );
	FileData.append( std::to_string( iPlayers.size() ));
	FileData.push_back( '\n' );

	FileData.append( "element item " );
	FileData.append( std::to_string( iItems.size() ));
	FileData.push_back( '\n' );

  FileData.append( "element description 1" );
	FileData.push_back( '\n' );

	FileData.append( "element controller " );
	FileData.append( std::to_string( iControllers.size() ));
	FileData.push_back( '\n' );

	FileData.append( "end_header\n" );

  //*------------------------------------------*
  //* Write KeyValues for all map controllers. *
  //*------------------------------------------*
  
  std::list<CMapController>::iterator Controller;
  
  for ( Controller = iControllers.begin();
        Controller != iControllers.end();
        ++ Controller )
	{
		// Add header for a map controller.
		
		WriteKeyValue_String( FileData,
                          EnigmaWC::Key::EController,
                          (*Controller).iName );
     
    // Write Main bytecode block.
    
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ECode,
                        0 );
    
    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::ELength,
                         (*Controller).iMainCode.size() );
    
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::EData,
                        0 );
                        
    FileData.append( (*Controller).iMainCode );
    
    // Write Current bytecode block (same as Restart bytecode for
    // new game maps).
    
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ECurrent,
                        0 );
    
    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::ELength,
                         (*Controller).iRestartCode.size() );
    
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::EData,
                        0 );
                        
    FileData.append( (*Controller).iRestartCode );
        
    // Write Saved bytecode block (same as Restart bytecode for
    // new game maps).
    
    if ( iSavable )
    {
      WriteKeyValue_8Bit( FileData,
                          EnigmaWC::Key::ESaved,
                          0 );
    
      WriteKeyValue_16Bit( FileData,
                           EnigmaWC::Key::ELength,
                           (*Controller).iRestartCode.size() );
    
      WriteKeyValue_8Bit( FileData,
                          EnigmaWC::Key::EData,
                          0 );
                        
      FileData.append( (*Controller).iRestartCode );
    }
    
    // Write Restart bytecode block.
    
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ERestart,
                        0 );
    
    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::ELength,
                         (*Controller).iRestartCode.size() );
    
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::EData,
                        0 );
                        
    FileData.append( (*Controller).iRestartCode );

    // Add header for a packed array of controller signal names.

    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ESignal,
                        0 );

    // Add Length keyvalue.

    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::ELength,
                         (guint16)(*Controller).iSignalNames.size() );

    // Write Data keyvalue, followed by the packed array of signal names.
  
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::EData,
                        0 );

    FileData.append( (*Controller).iSignalNames );
  }

  //*---------------------------------------------*
  //* Write KeyValues for all structural objects. *
  //*---------------------------------------------*

	// Set the initial East, North, and Above locations to their maximum,
	// forcing an addition of location keyvalues for the first structural
	// object (the first structural object cannot use the maximum value). 

  guint16 East  = G_MAXUINT16;
  guint16 North = G_MAXUINT16;  	
  guint16 Above = G_MAXUINT16; 

  std::list<CMapObject>::iterator Object;

  for ( Object = iObjects.begin();
        Object != iObjects.end();
        ++ Object )
  {
    // Add the header for a simple object.
	
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::EObject,
                        (guint8)(*Object).iID );

    // Add a new East keyvalue if an object with a different location
    // has been encountered.
		
    if ( (*Object).iLocation.iEast != East )
    {			
      East = (*Object).iLocation.iEast;

      WriteKeyValue_16Bit( FileData,
                           EnigmaWC::Key::EEast,
                           East );
    }

    // Add a new North keyvalue if an object with a different location
    // has been encountered.
		
    if ( (*Object).iLocation.iNorth != North )
    {
      North = (*Object).iLocation.iNorth;
      
      WriteKeyValue_16Bit( FileData,
                           EnigmaWC::Key::ENorth,
                           North );
    }

    // Add a new Above keyvalue if an object with a different location
    // has been encountered.
		
    if ( (*Object).iLocation.iAbove != Above )
    {
      Above = (*Object).iLocation.iAbove;

      WriteKeyValue_16Bit( FileData,
                           EnigmaWC::Key::EAbove,
                           Above );
    }

    // Add a surface keyvalue.

    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ESurface,
                        (guint8)(*Object).iSurface );

    // Add a rotation keyvalue.

    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ERotation,
                        (guint8)(*Object).iRotation );
                        
    // Add object state or signal keyvalues.
        
    WriteKeyValue_String( FileData,
                          EnigmaWC::Key::ESense,
                          (*Object).iSense );
    
    WriteKeyValue_String( FileData,
                          EnigmaWC::Key::EState,
                          (*Object).iState );
    
    WriteKeyValue_String( FileData,
                          EnigmaWC::Key::EVisibility,
                          (*Object).iVisibility );
    
    WriteKeyValue_String( FileData,
                          EnigmaWC::Key::EPresence,
                          (*Object).iPresence );
  }

  //*---------------------------------------------*
  //* Write KeyValues for all teleporter objects. *
  //*---------------------------------------------*

  for ( Object = iTeleporters.begin();
        Object != iTeleporters.end();
        ++ Object )
	{
		// Add header for a teleporter object.
		
		WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ETeleporter,
                        (guint8)(*Object).iID );
  
    // Write departure surface keyvalue, and add arrival surface
    // if it is not to be the player's current surface.
    
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ESurface,
                        (guint8)(*Object).iSurface );
  
    if ( (*Object).iSurfaceArrival != EnigmaWC::Direction::ENone )
    {
      WriteKeyValue_8Bit( FileData,
                          EnigmaWC::Key::EArrival,
                          (guint8)(*Object).iSurfaceArrival );
    }
    
    // Write departure rotation keyvalue, and add arrival rotation
    // if it is not to be the player's current rotation.
    
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ERotation,
                        (guint8)(*Object).iRotation );
  
    if ( (*Object).iRotationArrival != EnigmaWC::Direction::ENone )
    {
      WriteKeyValue_8Bit( FileData,
                          EnigmaWC::Key::EArrival,
                          (guint8)(*Object).iRotationArrival );
    }
    
    // Write departure East location keyvalue, and add arrival East location
    // if it is not to be the player's current East location.
  
    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::EEast,
                         (*Object).iLocation.iEast );
                         
    if ( (*Object).iLocationArrival.iEast != G_MAXUINT16 )
    {
      WriteKeyValue_16Bit( FileData,
                           EnigmaWC::Key::EArrival,
                           (*Object).iLocationArrival.iEast );
    }
    
    // Write departure North location keyvalue, and add arrival North location
    // if it is not to be the player's current North location.
  
    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::ENorth,
                         (*Object).iLocation.iNorth );
                         
    if ( (*Object).iLocationArrival.iNorth != G_MAXUINT16 )
    {
      WriteKeyValue_16Bit( FileData,
                           EnigmaWC::Key::EArrival,
                           (*Object).iLocationArrival.iNorth );
    }
    
    // Write departure Above location keyvalue, adding arrival Above location
    // if it is not to be the player's current Above location.
  
    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::EAbove,
                         (*Object).iLocation.iAbove );

    if ( (*Object).iLocationArrival.iAbove != G_MAXUINT16 )
    {           
      WriteKeyValue_16Bit( FileData,
                           EnigmaWC::Key::EArrival,
                           (*Object).iLocationArrival.iAbove );
    }

      // Add object state or signal keyvalues.
        
    WriteKeyValue_String( FileData,
                          EnigmaWC::Key::ESense,
                          (*Object).iSense );
    
    WriteKeyValue_String( FileData,
                          EnigmaWC::Key::EState,
                          (*Object).iState );
    
    WriteKeyValue_String( FileData,
                          EnigmaWC::Key::EVisibility,
                          (*Object).iVisibility );
    
    WriteKeyValue_String( FileData,
                          EnigmaWC::Key::EPresence,
                          (*Object).iPresence );
  }
  
  //*-----------------------------------------*
  //* Write KeyValues for all player objects. *
  //*-----------------------------------------*
  
  for ( Object = iPlayers.begin();
        Object != iPlayers.end();
        ++ Object )
	{
		// Add header for a player object.
		
		WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::EPlayer,
                        (guint8)(*Object).iID );
		
		// Write current, saved, and restart surface keyvalues.
		// All have the same value.
    
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ESurface,
                        (guint8)(*Object).iSurface );

    if ( iSavable )
    {
      WriteKeyValue_8Bit( FileData,
                          EnigmaWC::Key::ESaved,
                          (guint8)(*Object).iSurface );
    }
                        
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ERestart,
                        (guint8)(*Object).iSurface );

    // Write current, saved, and restart rotation keyvalues.
    // All have the same value.
    
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ERotation,
                        (guint8)(*Object).iRotation );

    if ( iSavable )
    {
      WriteKeyValue_8Bit( FileData,
                          EnigmaWC::Key::ESaved,
                          (guint8)(*Object).iRotation );
    }
                        
    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ERestart,
                        (guint8)(*Object).iRotation );

    // Write current, saved, and restart East location keyvalues.
    // All have the same value.

    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::EEast,
                         (*Object).iLocation.iEast );

    if ( iSavable )
    {
      WriteKeyValue_16Bit( FileData,
                           EnigmaWC::Key::ESaved,
                           (*Object).iLocation.iEast );
    } 
                         
    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::ERestart,
                         (*Object).iLocation.iEast );

    // Write current, saved, and restart North location keyvalues.
    // All have the same value.

    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::ENorth,
                         (*Object).iLocation.iNorth );

    if ( iSavable )
    {
      WriteKeyValue_16Bit( FileData,
                           EnigmaWC::Key::ESaved,
                           (*Object).iLocation.iNorth );
    }
                         
    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::ERestart,
                         (*Object).iLocation.iNorth );


		// Write current, saved, and restart Above location keyvalues.
		// All have the same value.
		
    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::EAbove,
                         (*Object).iLocation.iAbove );

    if ( iSavable )
    {
      WriteKeyValue_16Bit( FileData,
                           EnigmaWC::Key::ESaved,
                           (*Object).iLocation.iAbove );
    }
                         
    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::ERestart,
                         (*Object).iLocation.iAbove );

		// Add current, saved, and restart Active state keyvalues.
    // All have the same value.
        
    WriteKeyValue_Boolean( FileData,
                           EnigmaWC::Key::EActive,
                           (*Object).iActive );
                           
    if ( iSavable )
    {
      WriteKeyValue_Boolean( FileData,
                             EnigmaWC::Key::ESaved,
                             (*Object).iActive );
    }
                           
    WriteKeyValue_Boolean( FileData,
                           EnigmaWC::Key::ERestart,
                           (*Object).iActive );

    // Add object state or signal keyvalues.  Player objects
    // do not have a Sense state or signal.
    
    WriteKeyValue_String( FileData,
                          EnigmaWC::Key::EState,
                          (*Object).iState );
    
    WriteKeyValue_String( FileData,
                          EnigmaWC::Key::EVisibility,
                          (*Object).iVisibility );
                              
    WriteKeyValue_String( FileData,
                          EnigmaWC::Key::EPresence,
                          (*Object).iPresence );
	}

  //*---------------------------------------*
  //* Write KeyValues for all item objects. *
  //*---------------------------------------*

  for ( Object = iItems.begin();
        Object != iItems.end();
        ++ Object )
  {
    // Add header for an item object.

		WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::EItem,
                        (guint8)(*Object).iID );

   // Write surface.

    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ESurface,
                        (guint8)(*Object).iSurface );
    
    // Write rotation.

    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ERotation,
                        (guint8)(*Object).iRotation );

    // Write location.

    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::EEast,
                         (*Object).iLocation.iEast );

    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::ENorth,
                         (*Object).iLocation.iNorth );
		
    WriteKeyValue_16Bit( FileData,
                         EnigmaWC::Key::EAbove,
                         (*Object).iLocation.iAbove );

    // Write category.

    WriteKeyValue_8Bit( FileData,
                        EnigmaWC::Key::ECategory,
                        (guint8)(*Object).iCategory );
                        
    // Add current, saved, and restart active state keyvalues.
    // All items begin as active (not yet found).

    WriteKeyValue_Boolean( FileData,
                           EnigmaWC::Key::EActive,
                           TRUE );
                           
    if ( iSavable )
    {
      WriteKeyValue_Boolean( FileData,
                             EnigmaWC::Key::ESaved,
                             TRUE );
    }
                           
    WriteKeyValue_Boolean( FileData,
                           EnigmaWC::Key::ERestart,
                           TRUE );
                           
    // Add object state or signal keyvalues.  Item objects
    // do not have a Sense state or signal.
    
    WriteKeyValue_String( FileData,
                          EnigmaWC::Key::EState,
                          (*Object).iState );
    
    WriteKeyValue_String( FileData,
                          EnigmaWC::Key::EVisibility,
                          (*Object).iVisibility );

    
    WriteKeyValue_String( FileData,
                          EnigmaWC::Key::EPresence,
                          (*Object).iPresence );
  }

  //*------------------------------------------*
  //* Write KeyValues for the map description. *
  //*------------------------------------------*
		
  // Add header for a description.

  WriteKeyValue_8Bit( FileData,
                      EnigmaWC::Key::EDescription,
                      (guint8)EnigmaWC::Language::EEnglish );

  // Add description length keyvalue.

  WriteKeyValue_16Bit( FileData,
                       EnigmaWC::Key::ELength,
                       (guint16)iDescription.bytes() );

  // Write data keyvalue, followed by description in UTF-8 format.
  
  WriteKeyValue_8Bit( FileData,
                      EnigmaWC::Key::EData,
                      0 );

  FileData.append( iDescription );
  
  //*----------------------------------*
  //* Write KeyValue array terminator. *
  //*----------------------------------*

  WriteKeyValue_8Bit( FileData,
                      EnigmaWC::Key::EEnd,
                      0 );

  // Write the file data to the file.

  try
  {
    Glib::file_set_contents( iFileName,
                             FileData.data(),
                             FileData.size() );
  }
    catch( Glib::Error error )
  {
    return;
  }

  return;
}
