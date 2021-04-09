/*----------------------------------------------*
 * Program: Enigma in the Wine Cellar Map Maker *
 * Version: 5.0 for Linux OS                    *
 * File:    MapController.cpp                   *
 * Date:    November 20, 2016                   *
 * Author:  Chris Sterne                        *
 *                                              *
 * MapController class.                         *
 *----------------------------------------------*/

#include "EnigmaWC.h"
#include "MapController.h"

//*----------------------*
//* Default constructor. *
//*----------------------*

CMapController::CMapController()
{
  return;
}

//*-------------------------------------------*
//* Default constructor with controller name. *
//*-------------------------------------------*

CMapController::CMapController( const std::string& aName )
{
  iName = aName;
  return;
}

//*--------------------------------------------------------------------*
//* This private function returns the index to a string in a packed    *
//* array of strings (each ending with '\n' character ).               *
//*--------------------------------------------------------------------*
//* aArray:  Packed array of strings.                                  *
//* aString: String to find.                                           *
//* RETURN:  String index (G_MAXUINT8 if not found).                   *
//*--------------------------------------------------------------------*

guint8 IndexFromString( const std::string& aArray, const std::string& aString )
{
  std::string::size_type Position = 0;
  std::string::size_type Terminator;
  guint8 Index   = 0;
  gboolean Found = FALSE;

  do
  {
    Terminator = aArray.find( '\n',  Position );
    
    if ( Terminator != std::string::npos )
    {
      // Compare the provided string to one in the string array.
    
      if ( aArray.compare( Position, Terminator - Position, aString ) != 0 )
      {
        // The strings do not match.  Move the character position over
        // the string terminator character, and increase the index.
    
        Position = Terminator + 1;
        ++ Index;
      }
      else
        Found = TRUE;
    }
  }
  while ( !Found && ( Terminator != std::string::npos ));

  // Return the results of the search.

  if ( Found )
    return Index;
  else
    return G_MAXUINT8;
}

//*--------------------------------------------------------------------*
//* This private function returns the string corresponding to an index *
//* into a packed array of strings (each ending with '\n' character ). *
//*--------------------------------------------------------------------*
//* aArray: Packed array of strings.                                   *
//* aIndex: String index.                                              *
//* RETURN: String (zero length if index exceeds total array entries). *
//*--------------------------------------------------------------------*

std::string StringFromIndex( const std::string& aArray, guint8 aIndex )
{
  std::string String;

  // Find the position of the n-th string.
  
  std::string::size_type Position = 0;

  while ( aIndex )
  {
    Position = aArray.find( '\n',  Position );
  
    if ( Position != std::string::npos )
    {
      -- aIndex;
      ++ Position;
    }
    else
      aIndex = 0;
  }

  // If the n-th string was found, Position will be at its beginning.
  // Extract the n-th string.
  
  if ( Position != std::string::npos )
  { 
    std::string::size_type Terminator = aArray.find( '\n',  Position );
    
    if ( Terminator != std::string::npos )
      String = aArray.substr( Position, Terminator - Position );
  }

  return String;
}

//*------------------------------------------------*
//* This method compiles sourcecode into bytecode. *
//*------------------------------------------------*
//* aSourceCode: Sourcecode to be compiled.        *
//*------------------------------------------------*

void CMapController::Compile( const Glib::ustring& aSourceCode )
{
  // Clear any Restart bytecode, Main bytecode, and signal names.

  iRestartCode.clear();
  iMainCode.clear();
  iSignalNames.clear();
    
  std::vector<Glib::ustring> Lines;
  std::vector<Glib::ustring> Parts;
  
  guint LineIndex;
  guint PartIndex;
  guint8 SignalIndex;
 
  std::string ByteCode;
  gboolean RestartCode = TRUE;
  gboolean UpdateJump;
  guint JumpIndex;

  // Split the sourcecode into lines.
  
  Lines = Glib::Regex::split_simple( "\n", aSourceCode );

  // Compile each sourcecode line into a chunk of bytecode.
	
  for ( LineIndex = 0;
        LineIndex < Lines.size();
        ++ LineIndex )
  {    
    ByteCode.clear();
    UpdateJump = FALSE;
    
    // Split a sourcecode line into parts separated by one or more white spaces.
  
    Parts = Glib::Regex::split_simple( "\\s+", Lines.at( LineIndex ) );
    
    for ( PartIndex = 0;
          PartIndex < Parts.size();
          ++ PartIndex )
    { 
      if ( Parts.at( PartIndex ).compare( "[Restart]" ) == 0 )
        RestartCode = TRUE;
      else if ( Parts.at( PartIndex ).compare( "[Main]" ) == 0 )
        RestartCode = FALSE;
      else if ( Parts.at( PartIndex ).compare( "&" ) == 0 )
        ByteCode.push_back( (char)EnigmaWC::OpCode::EAnd );
      else if ( Parts.at( PartIndex ).compare( "|" ) == 0 )
        ByteCode.push_back( (char)EnigmaWC::OpCode::EOr );
      else if ( Parts.at( PartIndex ).compare( "!" ) == 0 )
        ByteCode.push_back( (char)EnigmaWC::OpCode::ENot );
      else if ( Parts.at( PartIndex ).compare( "^" ) == 0 )
        ByteCode.push_back( (char)EnigmaWC::OpCode::EXor );
      else if ( Parts.at( PartIndex ).compare( ">" ) == 0 )
        ByteCode.push_back( (char)EnigmaWC::OpCode::EStore );
      else if ( Parts.at( PartIndex ).compare( "F" ) == 0 )
        ByteCode.push_back( (char)EnigmaWC::OpCode::EFalse );
      else if ( Parts.at( PartIndex ).compare( "T" ) == 0 )
        ByteCode.push_back( (char)EnigmaWC::OpCode::ETrue );
      else if ( Parts.at( PartIndex ).compare( "#" ) == 0 )
        ByteCode.push_back( (char)EnigmaWC::OpCode::ERandom );    
      else if ( Parts.at( PartIndex ).compare( "?" ) == 0 )
      {
        // Add a conditional block opcode and a preliminary relative jump
        // instruction (jump occurs over block if conditional is FALSE).      
        
        ByteCode.push_back( (char)EnigmaWC::OpCode::EConditional );
        ByteCode.push_back( (char)EnigmaWC::OpCode::EJump );
        ByteCode.push_back( (char)0 );    
        
        // Save an index to the jump offset for updating later.
        
        if ( RestartCode )
          JumpIndex = iRestartCode.size() + ByteCode.size() - 1;
        else
          JumpIndex = iMainCode.size() + ByteCode.size() - 1;
      }
      else if ( Parts.at( PartIndex ).compare( "}" ) == 0 )
      {
        // The end of a conditional block is indicated.  After the current
        // bytecode chunk has been appended to its bytecode section, the proper
        // jump offset will be written.

        UpdateJump = TRUE;
      }
      else if ( Parts.at( PartIndex ).size() > 0 )
      {
        // Any unrecognized part (non-zero length) is considered a signal name
        // Search for the name in the signal name array to determine the index
        // for the signal instruction.  
        
        SignalIndex = IndexFromString( iSignalNames, Parts.at( PartIndex ) );
        
        // Add a new signal name to the array if not already present.
        
        if ( SignalIndex == G_MAXUINT8 )
        {
          iSignalNames.append( Parts.at( PartIndex ) );
          iSignalNames.push_back( '\n' );

          SignalIndex = IndexFromString( iSignalNames, Parts.at( PartIndex ) );
        }  
          
        // Add a signal instruction.
         
        ByteCode.push_back( (char)EnigmaWC::OpCode::ESignal );
        ByteCode.push_back( (char)SignalIndex );
      }
    }

    // Append the bytecode chunk created from the sourcecode line onto its final
    // bytecode section.
      
    if ( RestartCode )
    {
      iRestartCode.append( ByteCode );
    
      if ( UpdateJump )
      {
        iRestartCode.at( JumpIndex ) =
          (char)( iRestartCode.size() - 1 - JumpIndex );
      }
    }
    else
    {
      iMainCode.append( ByteCode );
      
      if ( UpdateJump )
      {
        iMainCode.at( JumpIndex ) =
          (char)( iMainCode.size() - 1 - JumpIndex );
      }
    }
  }

  return;
}

//*-----------------------------------------------------------------------*
//* This method uncompiles the restart and main bytecode into sourcecode. *
//*-----------------------------------------------------------------------*
//* aSourceCode: Destination buffer for sourcecode.                       *
//*-----------------------------------------------------------------------*

void CMapController::UnCompile( Glib::ustring& aSourceCode )
{
  aSourceCode.append( "[Restart]\n" );
  UnCompile( iRestartCode, aSourceCode );
  aSourceCode.append( "\n[Main]\n" );
  UnCompile( iMainCode, aSourceCode );
  return;
}

//*-------------------------------------------------------------*
//* This method uncompiles a block of bytecode into sourcecode, *
//* and appends the code to a provided buffer.                  *
//*-------------------------------------------------------------*
//* aByteCode:   Source of bytecode to be uncompiled.           *
//* aSourceCode: Destination buffer for sourcecode.             *
//*-------------------------------------------------------------*

void CMapController::UnCompile( std::string& aByteCode, Glib::ustring& aSourceCode )
{
   // Create sourcecode from the bytecode.
 
  gboolean PreNewLine = FALSE;
  gboolean NewLine    = PreNewLine;

  guint Index;
  guint BlockEnd = 0;

  for ( Index = 0;
        Index < aByteCode.size();
        ++ Index )
  {  
    switch ( (EnigmaWC::OpCode)aByteCode.at(Index) )
    {    
      case EnigmaWC::OpCode::EAnd:
        aSourceCode.push_back( '&' );
        break;
        
      case EnigmaWC::OpCode::EOr:
        aSourceCode.push_back( '|' );
        break;
      
      case EnigmaWC::OpCode::ENot:
        aSourceCode.push_back( '!' );
        break;
      
      case EnigmaWC::OpCode::EXor:
        aSourceCode.push_back( '^' );
        break;
        
      case EnigmaWC::OpCode::EStore:
        aSourceCode.push_back( '>' );
        PreNewLine = TRUE;
        break;

      case EnigmaWC::OpCode::EFalse:
        aSourceCode.push_back( 'F' );
        break;
        
      case EnigmaWC::OpCode::ETrue:
        aSourceCode.push_back( 'T' );
        break;
        
      case EnigmaWC::OpCode::ERandom:
        aSourceCode.push_back( '#' );
        break;
        
      case EnigmaWC::OpCode::EConditional:
        // Add the beginning of a conditional bytecode block.
      
        aSourceCode.push_back( '?' );
        NewLine = TRUE;

        // Skip forward to the relative jump instruction offset.
        
        Index += 2;
        
        // Save the index to the end of the conditional bytecode block.
        
        if ( Index < aByteCode.size() )
          BlockEnd = Index + (guint8)aByteCode.at(Index);
          
        break;

      case EnigmaWC::OpCode::ESignal:
        // Skip over the signal opcode to the index.
        
        ++ Index;

        // Obtain the signal name using the index.
        
        if ( Index < aByteCode.size() )
        {
          aSourceCode.append(
            StringFromIndex( iSignalNames, (guint8)aByteCode.at(Index) ));
        }

        NewLine = PreNewLine;
        break;

      default:
        break;
    }

    if  (( Index > 0 )
      && ( Index == BlockEnd ))
    {
      // The bytecode index has arrived at the end of the conditional block.
      // Add the block end character, and start a new sourcecode line.
      
      aSourceCode.append( " }\n" );
      PreNewLine = FALSE;
      NewLine    = FALSE;
      BlockEnd   = 0;      
    }
    else if ( NewLine )
    {
      // Start a new sourcecode line.
    
      aSourceCode.push_back( '\n' );
      PreNewLine = FALSE;
      NewLine    = FALSE;
    }
    else
    {
      // The current sourcecode line is still being assembled.  Add a space
      // after the current opcode character or signal name.
      
      aSourceCode.push_back( ' ' );
    }
  }
  
  return;
}