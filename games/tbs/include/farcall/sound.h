/// @file
/// Audio mixing, playback commands and music-player updates.

/// Runs one frame of the sound driver. Nothing in ☀️ calls it by this name.
void AudioEngine_RunMixerFar(void);
/// Starts the sound driver and clears the music ramps and cue records, at
/// power-on.
void Audio_InitializeRuntimeDefaultsFar(void);
#ifndef OWN_AudioCommand_PlayFar
/// Plays a cue: a fade, a sound effect, a fanfare or music. The whole game
/// plays its sounds this way.
void AudioCommand_PlayFar(s32 id);
#endif
/// Steps the music's volume and tempo ramps once a VBlank and brings the music
/// back after a fanfare.
void MusicPlayer_StepVolumeAndPitchTowardTargetsFar(void);
/// Sets the music's tempo scale until the next song or ramp, 0x100 being its
/// own tempo.
void MusicCommand_SetPitchAndUpdateFrequencyFar(u16 value);
/// Returns nonzero while a fanfare holds the music; Colosso and the message
/// windows poll it.
s32 AudioCommand_GetStateByteFar(void);
/// Returns the music cue last chosen, 0xff before any; the pause menu saves it
/// to restart the music.
s32 AudioCommand_GetSecondaryStateByteFar(void);
/// Waits a frame at a time for the music to come back after a fanfare, at most
/// 300 frames.
void AudioCommand_WaitForCompletionFar(void);
