#ifndef __SDCARD_H__
#define __SDCARD_H__

#include "fatfs.h"

FRESULT mount();
void unmount();
void loadSnapshot(const TCHAR* fileName);

bool loadSnapshotSetup();
bool loadSnapshotLoop();

bool saveSnapshotSetup();
bool saveSnapshotLoop();

#endif /* __SDCARD_H__ */
