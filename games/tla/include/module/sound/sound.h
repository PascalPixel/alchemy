#ifndef ALCHEMY_MODULE_SOUND_SOUND_H
#define ALCHEMY_MODULE_SOUND_SOUND_H

/// @file
/// What games/tla/src/sound/ declares for the files that use it, written once.

#include "types.h"

struct SoundTestCues {
  s32 cue[3];
};

void Sound_WaitForBgmRestore(void);

extern u8 gData_02003a90[];

#endif
