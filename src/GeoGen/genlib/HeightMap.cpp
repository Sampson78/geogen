#include "HeightMap.hpp"
#include "../ApiUsageException.hpp"

using namespace geogen;
using namespace genlib;

#define FOR_EACH_IN_RECT(x, y, rect) \
	for (Coordinate y = 0; y < (Coordinate)rect.GetSize().GetWidth(); y++) \
		for (Coordinate x = 0; x < (Coordinate)rect.GetSize().GetWidth(); x++) \

HeightMap::HeightMap(Rectangle rectangle, Height height)
:rectangle(rectangle)
{
	this->heightData = new Height[rectangle.GetSize().GetTotalLength()];
	
	FOR_EACH_IN_RECT(x, y, rectangle)
	{
		(*this)(x, y) = height;
	}
}

HeightMap::HeightMap(HeightMap const& other)
{
	this->rectangle = other.rectangle;
	
	this->heightData = new Height[this->rectangle.GetSize().GetTotalLength()];
	memcpy(this->heightData, other.heightData, sizeof(Height) * this->rectangle.GetSize().GetTotalLength());
}

HeightMap::HeightMap(HeightMap const& other, Rectangle cutoutRect)
{
	this->rectangle = cutoutRect;
	this->heightData = new Height[cutoutRect.GetSize().GetTotalLength()];

	// Fill all pixels with 0, because the cuout rect might have only partially overlapped with the original rect.
	FOR_EACH_IN_RECT(x, y, cutoutRect)
	{
		(*this)(x, y) = 0;
	}

	Rectangle intersection = Rectangle::Intersect(other.rectangle, cutoutRect);

	
	Point offsetSource = intersection.GetPosition() - other.rectangle.GetPosition();
	Point offsetDestination = intersection.GetPosition() - cutoutRect.GetPosition();
	FOR_EACH_IN_RECT(x, y, intersection)
	{
		(*this)(x + offsetDestination.GetX(), y + offsetDestination.GetY()) = other(x + offsetSource.GetX(), y + offsetSource.GetY());
	}
	
	memcpy(this->heightData, other.heightData, sizeof(Height)* this->rectangle.GetSize().GetTotalLength());
}

HeightMap& HeightMap::operator=(HeightMap& other)
{
	this->rectangle = other.rectangle;

	delete [] this->heightData;

	this->heightData = new Height[this->rectangle.GetSize().GetTotalLength()];
	memcpy(this->heightData, other.heightData, sizeof(Height) * this->rectangle.GetSize().GetTotalLength());

	return *this;
}

HeightMap::~HeightMap()
{
	delete[] this->heightData;
}

void HeightMap::AddMap(HeightMap* addend)
{
	if (!this->rectangle.Contains(addend->GetRectangle()))
	{
		throw ApiUsageException("The second addend must be contained by the first addend.");
	}

	Rectangle operationRect = Rectangle::Intersect(this->rectangle, addend->rectangle) - (addend->rectangle.GetPosition() - this->rectangle.GetPosition());

	FOR_EACH_IN_RECT(x, y, operationRect) 
	{
		(*this)(x, y) += (*addend)(x, y);
	}
}

void HeightMap::RadialGradient(Point point, Size1D radius, Height fromHeight, Height toHeight)
{
	FOR_EACH_IN_RECT(x, y, this->rectangle)
	{
		long distance = point.GetDistanceTo(this->rectangle.GetPosition() + Point(x, y));

		if (distance > radius)
		{
			(*this)(x, y) = toHeight;
		}
		else
		{
			(*this)(x, y) = fromHeight + (Height)(((long long)toHeight - (long long)fromHeight) * (long long)distance / (long long)radius);
		}
	}
}

void HeightMap::Blur(Size1D radius)
{
	this->Blur(radius, ORIENTATION_HORIZONTAL);
	this->Blur(radius, ORIENTATION_VERTICAL);
}

void HeightMap::Blur(Size1D radius, Orientation direction)
{
	// Allocate the new array.
	Height* new_data = new Height[this->rectangle.GetSize().GetTotalLength()];

	if (direction == ORIENTATION_HORIZONTAL) {
		for (Coordinate y = 0; y < (Coordinate)this->GetHeight(); y++) {
			// Prefill the window with value of the left edge + n leftmost values (where n is kernel size).
			Size1D window_size = radius * 2 + 1;
			long long window_value = (*this)(0, y) * radius;

			for (long long x = 0; x < radius; x++) {
				window_value += (*this)(x, y);
			}

			/* In every step shift the window one tile to the right  (= subtract its leftmost cell and add
			value of rightmost + 1). i represents position of the central cell of the window. */
			for (Coordinate x = 0; x < this->GetWidth(); x++) {
				// If the window is approaching right border, use the rightmost value as fill.
				if (x < radius) {
					window_value += (*this)(x + radius, y) - (*this)(0, y);
				}
				else if (x + radius < this->GetWidth()) {
					window_value += (*this)(x + radius, y) - (*this)(x - radius, y);
				}
				else {
					window_value += (*this)(this->GetWidth() - 1, y) - (*this)(x - radius, y);
				}

				// Set the value of current tile to arithmetic average of window tiles.
				new_data[x + this->GetWidth() * y] = window_value / window_size;
			}
		}
	}
	else { 
		for (Coordinate x = 0; x < this->GetHeight(); x++) {
			// Prefill the window with value of the left edge + n topmost values (where n is radius).
			Size1D window_size = radius * 2 + 1;
			long long window_value = (*this)(x, 0) * radius;

			for (Size1D y = 0; y < radius; y++) {
				window_value += (*this)(x, y);
			}

			/* In every step shift the window one tile to the bottom  (= subtract its topmost cell and add
			value of bottommost + 1). i represents position of the central cell of the window. */
			for (Coordinate y = 0; y < this->GetHeight(); y++) {
				// If the window is approaching right border, use the rightmost value as fill.
				if (y < radius) {
					window_value += (*this)(x, y + radius) - (*this)(x, 0);
				}
				else if (y + radius < this->GetHeight()) {
					window_value += (*this)(x, y + radius) - (*this)(x, y - radius);
				}
				else {
					window_value += (*this)(x, this->GetHeight() - 1) - (*this)(x, y - radius);
				}

				/* Set the value of current tile to arithmetic average of window tiles. */
				new_data[x + this->GetWidth() * y] = window_value / window_size;
			}
		}
	}

	/* Relink and delete the original array data */
	delete[] this->heightData;
	this->heightData = new_data;
}