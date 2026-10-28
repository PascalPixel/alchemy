#include "sound_driver.h"
#include "ram_buffer.h"

void Sound_Mixer(void);
void SoundInit(struct SoundWork *work);
void MPlayExtender(struct CgbNote *notes);
void m4aSoundMode(u32 mode);
void SoundMain(void);
void MPlayOpen(struct SoundPlayer *player,
                            struct SoundTrack *tracks, u8 count);
void MPlayStart(struct SoundPlayer *player,
                           const struct SongHeader *header);
void m4aMPlayStop(struct SoundPlayer *player);
void MPlayFadeOut(struct SoundPlayer *player, u16 speed);
void MPlayContinue(struct SoundPlayer *player);
void Clear64byte(void *block);
extern struct SoundWork gSound_Work;
extern struct CgbNote gSound_CgbNotes[4];
extern u8 gSound_WorkBytes[];
extern const struct PlayerSlot gSound_PlayerSlots[];
extern const struct SongEntry gSongTable[];
extern u8 Sound_PlayerCount;

void ply_memacc(struct SoundPlayer *player,
                                struct SoundTrack *track);
void ply_lfos(struct SoundPlayer *player,
                                       struct SoundTrack *track);
void ply_mod(struct SoundPlayer *player,
                                         struct SoundTrack *track);
void ply_xcmd(struct SoundPlayer *player,
                                      struct SoundTrack *track);
void ply_endtie(struct SoundPlayer *player,
                           struct SoundTrack *track);
void SampleFreqSet(u32 mode);
void TrackStop(struct SoundPlayer *player, struct SoundTrack *track);
void FadeOutBody(struct SoundPlayer *player);
void TrkVolPitSet(struct SoundPlayer *player,
                           struct SoundTrack *track);
void CgbSound(void);
void CgbOscOff(u8 channel);
s32 MidiKeyToCgbFreq(u8 kind, u8 key, u8 fine);
extern SoundCommand gSound_CommandTable[36];
extern u8 Sound_MaxLines;

void m4aSongNumStart(u16 id) {
  const struct PlayerSlot *slots = gSound_PlayerSlots;
  const struct SongEntry *songs = gSongTable;
  const struct SongEntry *song = &songs[id];

  MPlayStart(slots[song->slot].player, song->header);
}
