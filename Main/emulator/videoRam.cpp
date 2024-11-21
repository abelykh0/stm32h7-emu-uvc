#include "videoram.h"
#include "canvas.h"

using namespace Display;

VideoRam::VideoRam()
{
	this->Pixels.parent = this;
	this->Attributes.parent = this;
}

uint8_t* VideoRam::GetPixelPointer(uint16_t line, uint8_t character)
{
	// ZX Sinclair addressing
	// 00-00-00-Y7-Y6-Y2-Y1-Y0 Y5-Y4-Y3-x4-x3-x2-x1-x0
	//          12 11 10  9  8  7  6  5  4  3  2  1  0

	uint32_t y012 = ((line & 0B00000111) << 8);
	uint32_t y345 = ((line & 0B00111000) << 2);
	uint32_t y67 =  ((line & 0B11000000) << 5);
	return &this->Pixels.array[y012 | y345 | y67] + (character & 0B00011111);
}

Proxy<uint8_t, Pixels> Pixels::operator[](uint16_t index)
{
	return Proxy<uint8_t, Pixels>(this, index);
}

Proxy<uint16_t, Attributes> Attributes::operator[](uint16_t index)
{
	return Proxy<uint16_t, Attributes>(this, index);
}

void Pixels::SetValue(uint16_t offset, uint8_t val)
{
	this->array[offset] = val;

	// ZX Sinclair addressing
	// 00-00-00-Y7-Y6-Y2-Y1-Y0 Y5-Y4-Y3-x4-x3-x2-x1-x0
	//          12 11 10  9  8  7  6  5  4  3  2  1  0

	uint8_t charX = offset & 0x1F; // x4-x3-x2-x1-x0
	uint8_t charY = ((offset >> 5) & 0x07) | ((offset >> 8) & 0x18); // Y7-Y6-Y5-Y4-Y3
	uint8_t lineOffset = (offset >> 8) & 0x07; // Y2-Y1-Y0

	uint16_t colors = this->parent->Attributes.array[charY * 32 + charX];
	uint8_t backColor = colors & 0x3F;
	uint8_t foreColor = (colors >> 8) & 0x3F;
	uint8_t pixels = val;

	for (uint8_t j = 0; j < 8; j++)
	{
		uint8_t color;
		if (pixels << j & 0x80)
		{
			color = foreColor;
		}
		else
		{
			color = backColor;
		}

		SetPixel(48 + charX * 8 + j, 8 + charY * 8 + lineOffset, color);
	}
}


void Attributes::SetValue(uint16_t offset, uint16_t val)
{
	this->array[offset] = val;

	uint16_t colors = val;
	uint8_t charX = offset % 32;
	uint8_t charY = offset / 32;
	uint8_t backColor = colors & 0x3F;
	uint8_t foreColor = (colors >> 8) & 0x3F;

	uint8_t* pixels = this->parent->GetPixelPointer(charY * 8, charX);
	for (uint8_t i = 0; i < 8; i++)
	{
		for (uint8_t j = 0; j < 8; j++)
		{
			uint8_t color;
			if (*pixels << j & 0x80)
			{
				color = foreColor;
			}
			else
			{
				color = backColor;
			}
			SetPixel(48 + charX * 8 + j, 8 + charY * 8 + i, color);
		}

		pixels += 0x0100;
	}
}
