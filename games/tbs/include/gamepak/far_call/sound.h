/// @file
/// Audio mixing, playback commands and music-player updates.

void m4aSoundMainFar(void);
void Sound_InitializeFar(void);
void Sound_PlayFar(s32 id);
void Sound_UpdateBgmRampsFar(void);
void Sound_SetBgmTempoFar(u16 value);
s32 Sound_GetBgmRestoreDelayFar(void);
s32 Sound_GetCurrentBgmFar(void);
void Sound_WaitForBgmRestoreFar(void);
