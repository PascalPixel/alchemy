#ifndef ALCHEMY_UNIT_SOUND_SOUND_H
#define ALCHEMY_UNIT_SOUND_SOUND_H

#include "types.h"

void Sound_PlaySong(s32 id);
void Sound_UpdateBgmRamps(void);
void SoundTest_Run(void);
void Sound_Initialize(void);
void Sound_SetBgmTempo(u16 tempoScale);
void Sound_SetBgmPitch(u16 pitch);
void Sound_SetBgmTempoPitchTarget(u16 targetScale, u16 step);
void Sound_SetBgmVolume(s16 volume);
void Sound_SetBgmVolumeTarget(u16 targetVolume, u16 step);
s32 Sound_GetBgmRestoreDelay(void);
void Sound_StopAllMusicPlayers(void);
void Sound_ResumeAllMusicPlayers(void);
void Sound_UpdateCommandMask(u32 command);
s32 Sound_GetCurrentBgm(void);
void Sound_WaitForSong(void);
s32 Sound_GetReverbCode(s32 command);

#endif
