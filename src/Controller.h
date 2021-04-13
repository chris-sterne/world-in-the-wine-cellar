// "World in the Wine Cellar" world creator for "Enigma in the Wine Cellar".
// Copyright (C) 2021 Chris Sterne <chris_sterne@hotmail.com>
//
// This file is the Controller class header.  The Controller class manages
// code for a logic controller.
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
 
#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <glibmm/ustring.h>

namespace Enigma
{
	class Controller
	{
		public:
			// Public declarations.
		
			enum class OpCode   // Controller object opcodes.
			{
				NONE = 0,          // No operation.
				AND,               // Logical AND.
				OR,                // Logical OR.
				NOT,               // Logical NOT.
				XOR,               // Logical XOR.
				SIGNAL,            // Signal index.
				STORE,             // Store signal value.
				CONDITIONAL,       // Conditional block.
				JUMP,              // Relative jump.
				FALSE0,             // False (0) value.
				TRUE1,              // True (1) value.
				RANDOM,            // Random (True/False) value.
				TOTAL
			};
  
		
			// Public methods.

			Controller();
			Controller(const std::string& name);
			void compile(const Glib::ustring& sourcecode);
			void uncompile(Glib::ustring& sourcecode);

			// Public data.

			std::string m_name;            // Controller name.
			std::string m_signal_names;    // Packed array of signal names.
			std::string m_restart_code;    // Compiled restart bytecode.
			std::string m_main_code;       // Compiled main bytecode.

		private:
			// Private methods.

			void uncompile(std::string& bytecode, Glib::ustring& sourcecode);
	};
}

#endif // __CONTROLLER_H__
