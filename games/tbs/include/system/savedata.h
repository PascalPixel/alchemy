#ifndef ALCHEMY_SAVEDATA_H
#define ALCHEMY_SAVEDATA_H

/// @file
/// The save sectors' signature and header template, and the link test's
/// strings (system/savedata.c).

#include "types.h"

extern const u8 gSave_Signature[];
extern const u8 gSave_HeaderTemplate[8];

extern const u32 gLink_TestWord;
extern const u8 gLink_TestDataA[];
extern const u8 gLink_TestDataB[];

#endif
