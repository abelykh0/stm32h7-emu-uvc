#include "stm32h7xx_hal.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_video_if.h"
#include "rtc.h"

#include "startup.h"
#include "emulator/emulator.h"
#include "emulator/z80main.h"
#include "emulator/SDCard.h"
#include "emulator/z80snapshot.h"
#include "canvas.h"

//#include "emulator/ps2Keyboard.h"

uint8_t ROM[16384];

extern JPEG_HandleTypeDef hjpeg;
extern USBD_HandleTypeDef hUsbDeviceFS;
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

extern "C" void USB_DEVICE_Init(void);

//using namespace Display2;
//static Screen2 screen;

extern "C" void initialize()
{
}

extern "C" void setup()
{
	USB_DEVICE_Init();

	JPEG_ConfTypeDef config;
	config.ImageWidth = UVC_WIDTH;
	config.ImageHeight = UVC_HEIGHT;
	config.ColorSpace = JPEG_YCBCR_COLORSPACE;
	config.ChromaSubsampling = JPEG_444_SUBSAMPLING;
	config.ImageQuality = 90;
	HAL_JPEG_ConfigEncoding(&hjpeg, &config);

	//screen.SetAttribute(0x3F2A);
	//screen.Clear();
/*
    // Frame
	screen.SetAttribute(0x2A01);
    screen.PrintAt(0, 0, "\xC9"); // ╔
    screen.PrintAt(TEXT_COLUMNS - 1, 0, "\xBB"); // ╗
    screen.PrintAt(0, TEXT_ROWS - 1, "\xC8"); // ╚
    screen.PrintAt(TEXT_COLUMNS - 1, TEXT_ROWS - 1, "\xBC"); // ╝
    for (uint16_t i = 1; i < TEXT_COLUMNS - 1; i++)
    {
        screen.PrintAt(i, 0, "\x0CD"); // ═
        screen.PrintAt(i, TEXT_ROWS - 1, "\x0CD"); // ═
    }
    for (uint16_t i = 1; i < TEXT_ROWS - 1; i++)
    {
        screen.PrintAt(0, i, "\x0BA"); // ║
        screen.PrintAt(TEXT_COLUMNS - 1, i, "\x0BA"); // ║
    }

    // 64 colors
	char buf[20];
    for (int i = 0; i < 64; i++)
    {
    	screen.SetAttribute((i << 8) | 0x01);
    	screen.PrintAt(3 + (i % 5) * 7, 3 + (i / 5) * 2, "\xDF\xDF\xDF\xDF\xDF\xDF"); // ▀▀▀▀▀▀

    	screen.SetAttribute(0x2A01);
    	sprintf(buf, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(i));
    	screen.PrintAt(3 + (i % 5) * 7, 2 + (i / 5) * 2, buf);
    }

	screen.SetAttribute(0x3F01);


	FRESULT fr = mount();
	if (fr == FR_OK)
	{
		FIL file;
		TCHAR* scrFileName = (TCHAR*)u"opense.rom";
		fr = f_open(&file, scrFileName, FA_READ | FA_OPEN_EXISTING);
		if (fr == FR_OK)
		{
			UINT bytesRead;
			int remainingBytes = sizeof(ROM);
			uint8_t* buffer = ROM;

			do
			{
				UINT bytesToRead = remainingBytes < _MIN_SS ? remainingBytes : _MIN_SS;
				FRESULT readResult = f_read(&file, buffer, bytesToRead, &bytesRead);
				if (readResult != FR_OK || bytesRead != bytesToRead)
				{
					//return false;
				}

				remainingBytes -= bytesRead;
				buffer += bytesRead;
			} while (remainingBytes > 0);
			f_close(&file);
		}
		unmount();
	}


	startVideo();

	// Initialize PS2 Keyboard
	//Ps2_Initialize();

	// Initialize Spectrum emulator
	zx_setup(&MainScreen);

	//loadSnapshot((const TCHAR*)u"bubble-bobble.z80");

	fr = mount();
	if (fr == FR_OK)
	{
		FIL file;
		TCHAR* scrFileName = (TCHAR*)u"bubble-bobble.scr";
		fr = f_open(&file, scrFileName, FA_READ | FA_OPEN_EXISTING);
		if (fr == FR_OK)
		{
			zx::LoadScreenshot(&file, _buffer16K_1);
			f_close(&file);
		}
		unmount();
	}

    for (uint16_t offset = 0; offset < 32 * 24; offset++)
    {
		uint16_t colors = _spectrumScreen->Settings.Attributes[offset];
		//colors = 0x3F03;

		uint8_t charX = offset % 32;
		uint8_t charY = offset / 32;
		uint8_t backColor = colors & 0x3F;
		uint8_t foreColor = (colors >> 8) & 0x3F;
		for (uint8_t i = 0; i < 8; i++)
		{
			uint8_t pixels = *_spectrumScreen->GetPixelPointer(charY * 8 + i, charX);
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
				SetPixel(48 + charX * 8 + j, 8 + charY * 8 + i, color);
			}
		}
    }
*/
}

extern "C" void loop()
{
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
	HAL_Delay(200);

    //zx_loop();
	/*
	char showTime[20];


	RTC_DateTypeDef dateStruct;
	RTC_TimeTypeDef timeStruct;
	HAL_RTC_GetTime(&hrtc, &timeStruct, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &dateStruct, RTC_FORMAT_BIN); // important
	sprintf(showTime, " %.2d:%.2d:%.2d ", timeStruct.Hours, timeStruct.Minutes, timeStruct.Seconds);
    screen.PrintAlignCenter(0, showTime);
    */
	//HAL_Delay(200);
    //zx_loop();
}

extern "C" void USB_DEVICE_Init()
{
	  if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
	  {
	    Error_Handler();
	  }

	  // defaults are 128, 64, 128 (320)
	  HAL_PCDEx_SetRxFiFo(&hpcd_USB_OTG_FS,    64);  // 48
	  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 0, 16);  // 16
	  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 1, 240); // 256

	  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_VIDEO) != USBD_OK)
	  {
	    Error_Handler();
	  }
	  if (USBD_VIDEO_RegisterInterface(&hUsbDeviceFS, &USBD_VIDEO_fops_FS) != USBD_OK)
	  {
	    Error_Handler();
	  }
	  if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
	  {
	    Error_Handler();
	  }
}
