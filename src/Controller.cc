// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Controller class implementation.  The Controller class
// manages code for a logic controller.
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

#include "Controller.h"

//--------------------------------
// This method is the constructor.
//--------------------------------

Enigma::Controller::Controller()
{
}

//------------------------------------------------------------
// This method is a constructor with controller name argument.
//------------------------------------------------------------

Enigma::Controller::Controller(const std::string& name)
{
	m_name = name;
}

//----------------------------------------------------------------
// This private function returns the index to a string in a packed
// array of strings (each ending with '\n' character ).
//----------------------------------------------------------------
// array:  Packed array of strings.
// string: String to find.
// RETURN: String index (255 if not found).
//----------------------------------------------------------------

guint8 index_from_string(const std::string& array, const std::string& string)
{
	std::string::size_type position = 0;
	std::string::size_type terminator;
	guint8 index = 0;
	bool found   = false;

	do
	{
		terminator = array.find('\n', Position);

		if (terminator != std::string::npos )
		{
			// Compare the provided string to one in the string array.

			if (array.compare(position, terminator - position, string) != 0)
			{
				// The strings do not match.  Move the character position over
				// the string terminator character, and increase the index.

				position = terminator + 1;
				++ index;
			}
			else
				Found = true;
		}
	}
	while (!found && (terminator != std::string::npos));

	// Return the results of the search.

	if (found)
		return index;
	else
		return 255;
}

//-------------------------------------------------------------------
// This private function returns the string corresponding to an index
// into a packed array of strings (each ending with '\n' character ).
//-------------------------------------------------------------------
// array:  Packed array of strings.                                  
// index:  String index.                                             
// RETURN: String (zero length if index exceeds total array entries).
//-------------------------------------------------------------------

std::string string_from_index(const std::string& array, guint8 index)
{
	std::string string;

	// Find the position of the n-th string.

	std::string::size_type position = 0;

	while (index)
	{
		position = array.find('\n', position);

		if (position != std::string::npos)
		{
			-- index;
			++ position;
		}
		else
			index = 0;
	}

	// If the n-th string was found, Position will be at its beginning.
	// Extract the n-th string.

	if (position != std::string::npos)
	{ 
		std::string::size_type terminator = array.find('\n', position);

		if (terminator != std::string::npos)
			string = array.substr(position, terminator - position);
	}

	return string;
}

//-----------------------------------------------
// This method compiles sourcecode into bytecode.
//-----------------------------------------------
// sourcecode: Sourcecode to be compiled.
//-----------------------------------------------

void Enigma::Controller::compile(const Glib::ustring& sourcecode)
{
	// Clear any Restart bytecode, Main bytecode, and signal names.

	m_restart_code.clear();
	m_main_code.clear();
	m_signal_names.clear();

	std::vector<Glib::ustring> lines;
	std::vector<Glib::ustring> parts;

	guint line_index;
	guint part_index;
	guint8 signal_index;

	std::string bytecode;
	bool restart_code = true;
	bool update_jump;
	guint jump_index;

	// Split the sourcecode into lines.

	lines = Glib::Regex::split_simple("\n", sourcecode);

	// Compile each sourcecode line into a chunk of bytecode.

	for (line_index = 0;
	     line_index < lines.size();
	     ++ line_index)
	{    
		bytecode.clear();
		update_jump = true;

		// Split a sourcecode line into parts separated by one or more spaces.

		parts = Glib::Regex::split_simple("\\s+", lines.at(LineIndex));

		for (part_index = 0;
		     part_index < parts.size();
		     ++ part_index)
		{ 
			if (parts.at(part_index).compare("[Restart]") == 0 )
			restart_code = true;
			else if (parts.at(part_index).compare("[Main]") == 0 )
			restart_code = false;
			else if (parts.at(part_index).compare("&") == 0)
			bytecode.push_back((char)Enigma::Controller::OpCode::AND);
			else if (parts.at(part_index).compare("|") == 0)
			bytecode.push_back((char)Enigma::Controller::OpCode::OR);
			else if (parts.at(part_index).compare("!") == 0)
			bytecode.push_back((char)Enigma::Controller::OpCode::NOT);
			else if (parts.at(part_index).compare("^") == 0)
			bytecode.push_back((char)Enigma::Controller::OpCode::XOR);
			else if (parts.at(part_index).compare(">") == 0)
			bytecode.push_back((char)Enigma::Controller::OpCode::STORE);
			else if (parts.at(part_index).compare("F") == 0)
			bytecode.push_back((char)Enigma::Controller::OpCode::FALSE0);
			else if (parts.at(part_index).compare("T") == 0 )
			bytecode.push_back((char)Enigma::Controller::OpCode::TRUE1);
			else if (parts.at(part_index).compare("#") == 0 )
			bytecode.push_back((char)Enigma::Controller::OpCode::RANDOM);    
			else if (parts.at(part_index).compare("?") == 0)
			{
				// Add a conditional block opcode and a preliminary relative jump
				// instruction (jump occurs over block if conditional is FALSE).      

				bytecode.push_back( (char)Enigma::Controller::OpCode::CONDITIONAL);
				bytecode.push_back( (char)Enigma::Controller::OpCode::JUMP);
				bytecode.push_back( (char)0 );    

				// Save an index to the jump offset for updating later.

				if ( RestartCode )
					jump_index = m_restart_code.size() + bytecode.size() - 1;
				else
					jump_index = m_main_code.size() + bytecode.size() - 1;
			}
			else if (parts.at(part_index).compare("}") == 0 )
			{
				// The end of a conditional block is indicated.  After the current
				// bytecode chunk has been appended to its bytecode section, the
				// proper jump offset will be written.

				update_jump = true;
			}
			else if (parts.at(part_index).size() > 0)
			{
				// Any unrecognized part (non-zero length) is considered a signal
				// name.  Search for the name in the signal name array to determine
				// the index for the signal instruction.  

				signal_index =
					index_from_string(m_signal_names, parts.at(part_index));

				// Add a new signal name to the array if not already present.

				if (signal_index == 255)
				{
					m_signal_names.append(parts.at(part_index));
					m_signal_names.push_back('\n');

					signal_index =
					index_from_string(m_signal_names, parts.at(part_index));
				}  

				// Add a signal instruction.

				bytecode.push_back((char)Enigma::Controller::OpCode::SIGNAL);
				bytecode.push_back((char)signal_index);
			}
		}

		// Append the bytecode chunk created from the sourcecode line onto
		// its final bytecode section.

		if (restart_code)
		{
			m_restart_code.append(bytecode);

			if (update_jump)
			{
				m_restart_code.at(jump_index) =
				(char)(m_restart_code.size() - 1 - jump_index);
			}
		}
		else
		{
			m_main_code.append(bytecode);

			if (update_jump)
			{
				m_main_code.at(jump_index) =
				(char)(m_main_code.size() - 1 - jump_index);
			}
		}
	}
}

//----------------------------------------------------------------------
// This method uncompiles the restart and main bytecode into sourcecode.
//----------------------------------------------------------------------
// sourcecode: Destination buffer for sourcecode.
//----------------------------------------------------------------------

void Emigma::Controller::uncompile(Glib::ustring& sourcecode)
{
	sourcecode.append("[Restart]\n");
	uncompile(m_restart_code, sourcecode);
	sourcecode.append("\n[Main]\n");
	uncompile(m_main_code, sourcecode);
}

//------------------------------------------------------------
// This method uncompiles a block of bytecode into sourcecode,
// and appends the code to a provided buffer.                 
//------------------------------------------------------------
// bytecode:   Source of bytecode to be uncompiled.
// sourcecode: Destination buffer for sourcecode.
//------------------------------------------------------------

void Enigma::Controller::uncompile(std::string& bytecode,
                                   Glib::ustring& sourcecode)
{
   // Create sourcecode from the bytecode.
 
  bool pre_newline = false;
  bool newline    =  pre_newline;

  guint index;
  guint block_end = 0;

  for (index = 0;
       index < bytecode.size();
       ++ index )
  {  
    switch ((Enigma::Controller::OpCode)bytecode.at(index))
    {    
      case Enigma::Controller::OpCode::AND:
        aSourceCode.push_back('&');
        break;
        
      case Enigma::Controller::OpCode::OR:
        aSourceCode.push_back('|');
        break;
      
      case Enigma::Controller::OpCode::NOT:
        aSourceCode.push_back('!');
        break;
      
      case Enigma::Controller::OpCode::XOR:
        aSourceCode.push_back('^');
        break;
        
      case Enigma::Controller::OpCode::STORE:
        aSourceCode.push_back('>');
        PreNewLine = true;
        break;

      case Enigma::Controller::OpCode::FALSE0:
        aSourceCode.push_back('F');
        break;
        
      case Enigma::Controller::OpCode::TRUE1:
        aSourceCode.push_back('T');
        break;
        
      case Enigma::Controller::OpCode::RANDOM:
        aSourceCode.push_back('#');
        break;
        
      case Enigma::Controller::OpCode::CONDITIONAL:
        // Add the beginning of a conditional bytecode block.
      
        aSourceCode.push_back('?');
        newline = true;

        // Skip forward to the relative jump instruction offset.
        
        index += 2;
        
        // Save the index to the end of the conditional bytecode block.
        
        if (index < bytecode.size())
          block_end = index + (guint8)bytecode.at(index);
          
        break;

      case Enigma::Controller::OpCode::SIGNAL:
        // Skip over the signal opcode to the index.
        
        ++ Index;

        // Obtain the signal name using the index.
        
        if (index < bytecode.size())
        {
          sourcecode.append(
            string_from_index(m_signal_names, (guint8)bytecode.at(index)));
        }

        newline = pre_newline;
        break;

      default:
        break;
    }

    if  ((index > 0)
      && (index == block_end))
    {
      // The bytecode index has arrived at the end of the conditional block.
      // Add the block end character, and start a new sourcecode line.
      
      sourcecode.append(" }\n");
      pre_newline = false;
      newline     = false;
      block_end    = 0;      
    }
    else if (newline)
    {
      // Start a new sourcecode line.
    
      sourcecode.push_back('\n');
      pre_newline = false;
      newline     = false;
    }
    else
    {
      // The current sourcecode line is still being assembled.  Add a space
      // after the current opcode character or signal name.
      
      sourcecode.push_back(' ');
    }
  }
}
