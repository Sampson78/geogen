#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace runtime 
	{
		namespace instructions
		{
			class CallGlobalInstruction : public Instruction
			{
			private:
				String functionName;
				int argumentCount;
			public:				
				CallGlobalInstruction(CodeLocation location, String functionName, int argumentCount) : Instruction(location)
				{
					this->functionName = functionName;
					this->argumentCount = argumentCount;
				}

				virtual void Serialize(IOStream& stream) const { stream << "CallGlobal " << functionName << " " << argumentCount; }

				virtual String GetInstructionName() const { return GG_STR("CallGlobal"); };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;

				virtual Instruction* Clone() const { return new CallGlobalInstruction(this->GetLocation(), this->functionName, this->argumentCount); };
			};
		}
	}
}