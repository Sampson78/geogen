#include "RendererObjectTable.hpp"
#include "../InternalErrorException.hpp"
#include "RendererObject.hpp"

using namespace geogen;
using namespace renderer;

RendererObjectTable::~RendererObjectTable()
{
	for (iterator it = this->table.begin(); it != this->table.end(); it++)
	{
		delete *it;
	}
}

RendererObject* RendererObjectTable::GetObject(unsigned slot)
{
	if (slot >= this->GetSize())
	{
		throw InternalErrorException("Invalid renderer object slot number.");
	}

	return this->table[slot];
}

void RendererObjectTable::SetObject(unsigned slot, RendererObject* object)
{
	if (slot >= this->GetSize())
	{
		throw InternalErrorException("Invalid renderer object slot number.");
	}

	this->table[slot] = object;
}

void RendererObjectTable::ReleaseObject(unsigned slot)
{
	if (slot >= this->GetSize())
	{
		throw InternalErrorException("Invalid renderer object slot number.");
	}

	delete this->table[slot];
	this->table[slot] = NULL;
}