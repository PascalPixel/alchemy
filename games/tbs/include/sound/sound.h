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
extern struct SoundWork gSound_Work;
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
void MusicPlayer_BeginFadeOut(struct SoundPlayer *player, u16 speed);
void MusicPlayer_Initialize(struct SoundPlayer *player,
                            struct SoundTrack *tracks, u8 count);
void MusicPlayer_StepFade(struct SoundPlayer *player);
void MusicPlayer_Stop(struct SoundPlayer *player);
void MusicPlayer_Tick(struct SoundPlayer *player);
void MusicTrack_CalcOutput(struct SoundPlayer *player,
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

#ifndef OWN_IwramHi_Data1
extern s32 gIwramHi_Data1;
#endif
extern u8 gMusicFadeRequested;
extern s32 gSoundClipFlag;
extern u8 gEwram_Data1;
void Audio_Initialize(void);
void Audio_PlaySound(u16 id);
void Sound_TickDmaRestartTimer(void);

#endif
