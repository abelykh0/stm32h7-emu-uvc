#ifndef _VIDEOSETTINGS_H
#define _VIDEOSETTINGS_H

#include <stdint.h>

namespace Display
{

class VideoSettings
{
  public:
	uint8_t Scale;

	// text resolution
	uint8_t TextColumns;
	uint8_t TextRows;

	// video memory
	uint8_t* Pixels;
	uint16_t* Attributes; // colors
	uint8_t* BorderColor;
};

}

#endif