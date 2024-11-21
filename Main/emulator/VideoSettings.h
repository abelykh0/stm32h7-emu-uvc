#ifndef _VIDEOSETTINGS_H
#define _VIDEOSETTINGS_H

#include "ClassProperties.h"
#include "videoRam.h"

namespace Display
{

class VideoSettings : public VideoRam
{
private:

public:
	// text resolution
	uint8_t TextColumns = 32;
	uint8_t TextRows = 24;

	uint8_t BorderColor;
};

}

#endif
