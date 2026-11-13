#ifndef ALCHEMY_FAR_CALL_SOUND_H
#define ALCHEMY_FAR_CALL_SOUND_H

/// @file
/// The far calls into the sound bank, as the other banks' code calls them.

#include "types.h"

s32 Sound_GetCurrentBgmFar(void);
void Sound_UpdateBgmRampsFar(void);
void Sound_InitializeFar(void);

#endif
