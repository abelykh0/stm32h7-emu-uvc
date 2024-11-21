#ifndef __VIDEORAM_H__
#define __VIDEORAM_H__

#include "stdint.h"
#include "proxy.h"

namespace Display
{

class VideoRam;

class Pixels
{
	friend VideoRam;

private:
	VideoRam* parent;

public:
	uint8_t array[32 * 8 * 24];
	Proxy<uint8_t, Pixels> operator[](uint16_t index);
	void SetValue(uint16_t index, uint8_t val);
};

class Attributes
{
	friend VideoRam;

private:
	VideoRam* parent;

public:
	uint16_t array[32 * 24];
	void SetValue(uint16_t index, uint16_t val);
	Proxy<uint16_t, Attributes> operator[](uint16_t index);
};

class VideoRam
{
	friend Display::Attributes;

public:
	Display::Pixels Pixels;
	Display::Attributes Attributes;

	VideoRam();
	uint8_t* GetPixelPointer(uint16_t line, uint8_t character);
};

}

#endif
