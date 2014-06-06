#include "Dimension.hpp"
#include "InternalErrorException.hpp"

std::string geogen::DimensionToString(Dimension dimensions)
{
	switch (dimensions)
	{
	case DIMENSION_WIDTH: return "Width"; 
	case DIMENSION_HEIGHT: return "Height";
	default:
		throw InternalErrorException("Invalid Dimension.");
	}
}