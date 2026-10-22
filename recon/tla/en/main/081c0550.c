#include "audio_engine.h"
#include "ram_buffer.h"

void Sound_Mixer(void);
void AudioEngine_Initialize(struct SoundWork *work);
void CgbAudio_Initialize(struct CgbNote *notes);
void AudioEngine_SetMode(u32 mode);
void AudioEngine_RunMixerTick(void);
void MusicPlayer_Initialize(struct SoundPlayer *player,
                            struct SoundTrack *tracks, u8 count);
void MusicPlayer_StartSong(struct SoundPlayer *player,
                           const struct SequenceHeader *header);
void MusicPlayer_Stop(struct SoundPlayer *player);
void MusicPlayer_StartFadeOut(struct SoundPlayer *player, u16 speed);
void Audio_ResumePlayer(struct SoundPlayer *player);
void AudioCommand_InvokeSlot35(void *block);
extern struct SoundWork gSound_Work;
extern struct CgbNote gSound_CgbNotes[4];
extern u8 gSound_WorkBytes[];
extern const struct PlayerSlot gSound_PlayerSlots[];
extern const struct SongEntry gSound_SongTable[];
extern u8 Sound_PlayerCount;

void MusicTrack_OperateWorkByte(struct SoundPlayer *player,
                                struct SoundTrack *track);
void MusicTrack_SetLfoSpeedFromCommand(struct SoundPlayer *player,
                                       struct SoundTrack *track);
void MusicTrack_SetModulationFromCommand(struct SoundPlayer *player,
                                         struct SoundTrack *track);
void MusicTrack_DispatchStreamCommand(struct SoundPlayer *player,
                                      struct SoundTrack *track);
void MusicTrack_ReleaseKey(struct SoundPlayer *player,
                           struct SoundTrack *track);
void AudioEngine_SetPcmRate(u32 mode);
void MusicTrack_Stop(struct SoundPlayer *player, struct SoundTrack *track);
void MusicPlayer_StepFade(struct SoundPlayer *player);
void MusicTrack_CalculateOutput(struct SoundPlayer *player,
                           struct SoundTrack *track);
void Cgb_UpdateChannels(void);
void CgbChannel_Mute(u8 channel);
s32 Cgb_KeyToFrequency(u8 kind, u8 key, u8 fine);
extern SoundCommand gSound_CommandTable[36];
extern u8 Sound_MaxLines;

void Audio_PlaySound(u16 id) {
  const struct PlayerSlot *slots = gSound_PlayerSlots;
  const struct SongEntry *songs = gSound_SongTable;
  const struct SongEntry *song = &songs[id];

  MusicPlayer_StartSong(slots[song->slot].player, song->header);
}
