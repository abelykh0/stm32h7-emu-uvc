#ifndef _SPECTRUMSCREEN_H
#define _SPECTRUMSCREEN_H

#include "VideoSettings.h"
#include "Screen.h"

namespace Display
{

class SpectrumScreen : public Screen
{
public:
	SpectrumScreen(VideoSettings* settings);
	SpectrumScreen(VideoSettings* settings, uint16_t startLine, uint16_t height);

	void ShowScreenshot(const uint8_t* screenshot);
	uint16_t FromSpectrumColor(uint8_t sinclairColor);
	uint8_t ToSpectrumColor(uint16_t color);

	void Clear() override;
};

}

#endif
