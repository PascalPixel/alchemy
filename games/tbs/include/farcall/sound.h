/// @file
/// Audio mixing, playback commands and music-player updates.
/// Far-call interfaces; their ordered jump stubs live in
/// games/tbs/src/system/far_call/SOUND.S.

void AudioEngine_RunMixerFar(void);
void Audio_InitializeRuntimeDefaultsFar(void);
#ifndef OWN_AudioCommand_PlayFar
void AudioCommand_PlayFar(s32 id);
#endif
void MusicPlayer_StepVolumeAndPitchTowardTargetsFar(void);
void MusicCommand_SetPitchAndUpdateFrequencyFar(u16 value);
s32 AudioCommand_GetStateByteFar(void);
s32 AudioCommand_GetSecondaryStateByteFar(void);
void AudioCommand_WaitForCompletionFar(void);
