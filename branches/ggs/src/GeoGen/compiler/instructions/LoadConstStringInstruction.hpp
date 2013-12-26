#pragma once

#include <string>

#include "Instruction.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class LoadConstStringInstruction : public Instruction
			{
			private:
				std::string constString;
			public:				
				LoadConstStringInstruction(std::string const& constString) 
				{
					this->constString = constString;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "LoadConstString " << constString << std::endl; }
			};
		}
	}
}