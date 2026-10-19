/// @file
/// Audio mixing, playback commands and music-player updates.
/// Far-call interfaces; their ordered jump stubs live in
/// games/tbs/src/system/far_call/SOUND.S.

/// Far call to AudioEngine_RunMixer.
void AudioEngine_RunMixerFar(void);
/// Far call to Audio_InitializeRuntimeDefaults.
void Audio_InitializeRuntimeDefaultsFar(void);
#ifndef OWN_AudioCommand_PlayFar
/// Far call to AudioCommand_Play.
void AudioCommand_PlayFar(s32 id);
#endif
/// Far call to MusicPlayer_StepVolumeAndPitchTowardTargets.
void MusicPlayer_StepVolumeAndPitchTowardTargetsFar(void);
/// Far call to MusicCommand_SetPitchAndUpdateFrequency.
void MusicCommand_SetPitchAndUpdateFrequencyFar(u16 value);
/// Far call to AudioCommand_GetStateByte.
s32 AudioCommand_GetStateByteFar(void);
/// Far call to AudioCommand_GetSecondaryStateByte.
s32 AudioCommand_GetSecondaryStateByteFar(void);
/// Far call to AudioCommand_WaitForCompletion.
void AudioCommand_WaitForCompletionFar(void);
