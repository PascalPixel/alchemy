#ifndef ALCHEMY_FAR_CALL_SOUND_H
#define ALCHEMY_FAR_CALL_SOUND_H

/// @file
/// The sound bank's far calls: playing sounds and music, and the driver's
/// update each frame.

void m4aSoundMainFar(void);
void Sound_InitializeFar(void);
void Sound_PlayFar(s32 id);
void Sound_UpdateBgmRampsFar(void);
void Sound_SetBgmTempoFar(u16 tempoScale);
s32 Sound_GetBgmRestoreDelayFar(void);
s32 Sound_GetCurrentBgmFar(void);
void Sound_WaitForBgmRestoreFar(void);

#endif
