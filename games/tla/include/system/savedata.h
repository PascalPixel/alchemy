#ifndef ALCHEMY_SAVEDATA_H
#define ALCHEMY_SAVEDATA_H

/// @file
/// The save signature and sector header template, and the link test strings
/// nothing reads (system/savedata.c).

#include "types.h"

extern const u8 gSave_Signature[];
extern const u8 gSave_HeaderTemplate[8];

extern const u32 gLink_TestWord;
extern const u8 gLink_TestDataA[];
extern const u8 gLink_TestDataB[];

#endif
