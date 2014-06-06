#pragma once

#include <string>
#include <map>

#include "VariableTableItem.hpp"

namespace geogen
{
	namespace runtime
	{
		class ManagedObject;
		class MemoryManager;

		class VariableTable
		{
		private:
			MemoryManager* memoryManager;
			std::map<std::string, VariableTableItem> table;

			// Non-copyable
			VariableTable(VariableTable const&) {}
			VariableTable& operator=(VariableTable const&) {};
		public:
			VariableTable(MemoryManager* memoryManager) : memoryManager(memoryManager) {}
			~VariableTable();

			typedef std::map<std::string, VariableTableItem const>::const_iterator const_iterator;
			typedef std::map<std::string, VariableTableItem>::iterator iterator;

			inline MemoryManager* GetMemoryManager() { return this->memoryManager; };
			inline MemoryManager const* GetMemoryManager() const { return this->memoryManager; };

			VariableTableItem const* GetVariable(std::string const& variableName) const;
			VariableTableItem* GetVariable(std::string const& variableName);
			//bool SetVariable(std::string const& variableName, ManagedObject* value);
			bool IsVariableDeclared(std::string const& symbolName) const;
			bool DeclareVariable(std::string const& symbolName, ManagedObject* value, bool isConst);

			inline const_iterator Begin() const { return *(const_iterator*)(&this->table.begin()); }
			inline const_iterator End() const { return *(const_iterator*)(&this->table.end()); }

			inline iterator Begin() { return this->table.begin(); }
			inline iterator End() { return this->table.end(); }			
		};
	}
}