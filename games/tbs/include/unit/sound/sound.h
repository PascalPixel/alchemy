#ifndef ALCHEMY_UNIT_SOUND_SOUND_H
#define ALCHEMY_UNIT_SOUND_SOUND_H

#include "types.h"

void AudioCommand_Play(s32 id);
void MusicPlayer_StepVolumeAndPitchTowardTargets(void);
void AudioTest_RunParameterController(void);
void Audio_InitializeRuntimeDefaults(void);
void MusicCommand_SetPitchAndUpdateFrequency(u16 value);
void MusicCommand_SetPitch(u16 pitch);
void Audio_SetWorkPairB(u16 primary, u16 secondary);
void MusicCommand_SetVolume(s16 volume);
void Audio_SetMusicVolumeTargetAndStep(u16 primary, u16 secondary);
s32 AudioCommand_GetStateByte(void);
void AudioCommand_StopAllPlayers(void);
void AudioCommand_ResumeAllPlayers(void);
void AudioCommand_UpdateToggleMask(u32 command);
s32 AudioCommand_GetSecondaryStateByte(void);
void AudioCommand_WaitForCompletion(void);
s32 AudioCommand_GetWidth(s32 command);

#endif
