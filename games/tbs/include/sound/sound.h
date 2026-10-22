#ifndef SOUND_H
#define SOUND_H

/// @file
/// The sound tables (CGB codes, frequency steps, frame lengths) and the sound
/// routines.

#include "types.h"
#include "edition.h"
#include "audio_engine_types.h"

struct SoundPlayer;
struct SoundTrack;
struct SoundWork;

extern const u8 gSound_Cgb3LevelCodes[];
extern const s16 gSound_CgbFrequencySteps[];
extern const u8 gSound_CgbPitchCodes[];
extern const u16 gSound_FrameLengths[];
extern const u8 gSound_NoisePitchCodes[];
extern u32 gSound_PcmFrequencySteps[];
extern u8 gSound_PcmPitchCodes[];
/// The sound engine's work, which Audio_Initialize sets up through
/// AudioEngine_Initialize.
extern struct SoundWork gSound_Work;
/// The 16 work bytes every music player shares (SoundPlayer.workBytes), so
/// one song can test what another has set (MusicTrack_OperateWorkByte).
extern u8 gSound_WorkBytes[];
void Audio_ResumeAllPlayers(void);
void Audio_ResumePlayer(struct SoundPlayer *player);
void Audio_StopAllPlayers(void);
u32 Audio_UmulHigh32(u32 left, u32 right);
void AudioEngine_Initialize(struct SoundWork *work);
void AudioEngine_ResumeDirectSound(void);
void AudioEngine_RunMixerTick(void);
void AudioEngine_SetMode(u32 mode);
void AudioEngine_SetPcmRate(u32 mode);
void AudioEngine_SuspendDirectSound(void);
s32 Cgb_KeyToFrequency(u8 kind, u8 key, u8 fine);
void Cgb_UpdateChannels(void);
void CgbChannel_Mute(u8 channel);
void MusicPlayer_StartFadeOut(struct SoundPlayer *player, u16 speed);
void MusicPlayer_Initialize(struct SoundPlayer *player,
                            struct SoundTrack *tracks, u8 count);
void MusicPlayer_StepFade(struct SoundPlayer *player);
void MusicPlayer_Stop(struct SoundPlayer *player);
void MusicPlayer_Tick(struct SoundPlayer *player);
void MusicTrack_CalculateOutput(struct SoundPlayer *player,
                                struct SoundTrack *track);
void MusicTrack_HandleNote(u32 command, struct SoundPlayer *player,
                           struct SoundTrack *track);
void MusicTrack_OperateWorkByte(struct SoundPlayer *player,
                                struct SoundTrack *track);
void MusicTrack_ReleaseKey(struct SoundPlayer *player,
                           struct SoundTrack *track);
void MusicTrack_SetLfoSpeedFromCommand(struct SoundPlayer *player,
                                       struct SoundTrack *track);
void MusicTrack_SetModulationFromCommand(struct SoundPlayer *player,
                                         struct SoundTrack *track);
void MusicTrack_Stop(struct SoundPlayer *player, struct SoundTrack *track);
void Sound_Mixer(void);

/// The build stamp System_CopyBuildStamp copies to IWRAM at power-on: twelve
/// digits and a zero word, at the bottom of the frame-wait stack.
extern s32 gBuildStamp[];
/// Nonzero once AUDIO_CUE_FADE_BGM has started the music's fade; further fade
/// cues are ignored until the next music clears it.
extern u8 gMusicFadeRequested;
/// Cleared by the audio test, AudioTest_RunParameterController (reached only
/// through a sound far-call veneer), which restarts a 20-frame timer when it
/// finds it set; no other code names it (see the XXX there).
extern s32 gSoundClipFlag;
/// An EWRAM byte Audio_InitializeRuntimeDefaults clears and nothing else
/// names.
extern u8 gEwram_Data1;
void Audio_Initialize(void);
void Audio_PlaySound(u16 id);
void Sound_TickDmaRestartTimer(void);

#endif
