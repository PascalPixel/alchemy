#ifndef ALCHEMY_FAR_CALL_SOUND_H
#define ALCHEMY_FAR_CALL_SOUND_H

/// @file
/// The far calls into the sound bank, as the other banks' code calls them.

#include "types.h"

/// Returns the music Sound_Play last chose.
s32 Sound_GetCurrentBgmFar(void);
/// Brings the music back after a jingle and steps its volume, tempo and pitch
/// ramps; called once a VBlank.
void Sound_UpdateBgmRampsFar(void);
/// Starts the sound driver and resets the music's ramps.
void Sound_InitializeFar(void);

#endif
