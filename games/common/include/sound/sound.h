#ifndef SOUND_H
#define SOUND_H

/* SOUND */

#include "types.h"
#include "edition.h"
#include "audio_engine_types.h"

struct SoundPlayer;
struct SoundTrack;
struct SoundWork;

extern const u8 Sound_Cgb3LevelCodes[];
extern const s16 Sound_CgbFrequencySteps[];
extern const u8 Sound_CgbPitchCodes[];
extern const u16 Sound_FrameLengths[];
extern const u8 Sound_NoisePitchCodes[];
extern u32 Sound_PcmFrequencySteps[];
extern u8 Sound_PcmPitchCodes[];
extern struct SoundWork Sound_Work;
extern u8 Sound_WorkBytes[];
void Audio_EmptyCallback(void);
void Audio_ResumeAllPlayers(void);
void Audio_ResumePlayer(struct SoundPlayer *player);
void Audio_StopAllPlayers(void);
s32 Audio_UmulHigh32(s32, s32);
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
void MusicPlayer_Initialize(struct SoundPlayer *player, struct SoundTrack *tracks, u8 count);
void MusicPlayer_StepFade(struct SoundPlayer *player);
void MusicPlayer_Stop(struct SoundPlayer *player);
void MusicPlayer_Tick(struct SoundPlayer *player);
void MusicTrack_CalcOutput(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_HandleNote(u32 command, struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_OperateWorkByte(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_ReleaseKey(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_SetLfoSpeedFromCommand(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_SetModulationFromCommand(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_Stop(struct SoundPlayer *player, struct SoundTrack *track);
void Sound_Mixer(void);

#if !GAME_TLA
#ifndef OWN_Data_03007804
extern s32 Data_03007804;
#endif
extern u8 gMusicFadeRequested;
extern s32 gSoundClipFlag;
extern u8 RomBytes_02003004;
void Audio_Initialize(void);
void Audio_PlaySound(u16 id);
void Sound_TickDmaRestartTimer(void);
#endif

#if GAME_TLA
extern u16 Data_02005830;
extern u8 gSongFlags[];
s32 Audio_Check(void);
void AudioCommand_WaitForStateByteClear(void);
#endif

#endif
