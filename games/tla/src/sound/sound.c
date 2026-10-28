#include "types.h"
#include "sound_driver.h"
#include "ram_buffer.h"

u32 umul3232H32(u32 left, u32 right);
extern u8 gSound_PcmPitchCodes[];
extern u32 gSound_PcmFrequencySteps[];

/// Interpolates adjacent key frequencies and scales them by the wave sample
/// frequency.
s32 MidiKeyToFreq(void *wave, u8 key, u8 pitch) {
  u32 keyIndex;
  u32 pitchScale;
  u32 lowerFrequency;
  u32 upperFrequency;

  keyIndex = key;
  pitchScale = (u32)pitch << 24;
  if (keyIndex > 178) {
    keyIndex = 178;
    pitchScale = 255U << 24;
  }

  lowerFrequency = gSound_PcmPitchCodes[keyIndex];
  lowerFrequency =
      gSound_PcmFrequencySteps[lowerFrequency & 15] >> (lowerFrequency >> 4);
  upperFrequency = gSound_PcmPitchCodes[keyIndex + 1];
  upperFrequency =
      gSound_PcmFrequencySteps[upperFrequency & 15] >> (upperFrequency >> 4);

  return umul3232H32(
      *(s32 *)((u8 *)wave + 4),
      lowerFrequency +
          umul3232H32(upperFrequency - lowerFrequency, pitchScale));
}

/// Provides the empty callback retained by the sound driver.
void UnusedDummyFunc(void) {}

// The sound data's symbols supply the player count and maximum track count.
extern char Sound_PlayerCount[];
extern char Sound_MaxLines[];
#define SOUND_PLAYER_COUNT ((u16)(u32)Sound_PlayerCount)
#define SOUND_MAX_LINES ((u32)Sound_MaxLines)

void Sound_Mixer(void);
void SoundInit(struct SoundWork *work);
void m4aSoundMode(u32 mode);
void SoundMain(void);
void MPlayOpen(struct SoundPlayer *player, struct SoundTrack *tracks, u8 count);
void MPlayStart(struct SoundPlayer *player, const struct SongHeader *header);
void m4aMPlayStop(struct SoundPlayer *player);
void MPlayFadeOut(struct SoundPlayer *player, u16 speed);
void MPlayContinue(struct SoundPlayer *player);
extern struct SoundWork gSound_Work;
extern struct CgbNote gSound_CgbNotes[4];
extern u8 gSound_WorkBytes[];
extern const struct PlayerSlot gSound_PlayerSlots[];
extern const struct SongEntry gSongTable[];

void ply_memacc(struct SoundPlayer *player, struct SoundTrack *track);
void ply_lfos(struct SoundPlayer *player, struct SoundTrack *track);
void ply_mod(struct SoundPlayer *player, struct SoundTrack *track);
void ply_endtie(struct SoundPlayer *player, struct SoundTrack *track);
void SampleFreqSet(u32 mode);
void TrackStop(struct SoundPlayer *player, struct SoundTrack *track);
void FadeOutBody(struct SoundPlayer *player);
void TrkVolPitSet(struct SoundPlayer *player, struct SoundTrack *track);
void CgbSound(void);
void CgbOscOff(u8 channel);
s32 MidiKeyToCgbFreq(u8 kind, u8 key, u8 fine);
extern SoundCommand gSound_CommandTable[36];

/// Clears the paused flag while holding the player lock.
void MPlayContinue(struct SoundPlayer *player) {
  if (player->lock == SOUND_LOCK) {
    player->lock++;
    player->status &= ~SOUND_PLAYER_PAUSED;
    player->lock = SOUND_LOCK;
  }
}

/// Starts a fade under the player lock, resuming the player for fade-ins.
static __inline__ void MPlayStartFade(struct SoundPlayer *player, u16 period,
                                      u16 volume) {
  if (player->lock == SOUND_LOCK) {
    player->lock++;
    player->fadeCounter = period;
    player->fadePeriod = period;
    player->fadeVolume = volume;
    if (volume & SOUND_FADE_IN)
      player->status &= ~SOUND_PLAYER_PAUSED;
    player->lock = SOUND_LOCK;
  }
}

/// Begins a fade that stops and clears tracks when its volume reaches zero.
void MPlayFadeOut(struct SoundPlayer *player, u16 speed) {
  MPlayStartFade(player, speed, SOUND_FADE_FULL_VOLUME);
}

/// Copies the RAM mixer, initializes both audio paths and configures every
/// player.
void m4aSoundInit(void) {
  s32 playerIndex;

  // Copy the mixer's executable block to its RAM buffer.
  Bios_CpuSet((void *)((u32)Sound_Mixer & ~1), Ram_SoundMixer, 0x04000100);
  SoundInit(&gSound_Work);
  MPlayExtender(gSound_CgbNotes);
  m4aSoundMode(AUDIO_INITIAL_MODE);
  // Each configured player owns its tracks but shares the work-byte area.
  for (playerIndex = 0; playerIndex < SOUND_PLAYER_COUNT; playerIndex++) {
    struct SoundPlayer *player = gSound_PlayerSlots[playerIndex].player;

    MPlayOpen(player, gSound_PlayerSlots[playerIndex].tracks,
              gSound_PlayerSlots[playerIndex].trackCount);
    player->checkPriority = gSound_PlayerSlots[playerIndex].checkPriority;
    player->workBytes = gSound_WorkBytes;
  }
}

void m4aSoundMain(void) { SoundMain(); }

/// Starts the song on the player selected by its song-table entry.
void m4aSongNumStart(u16 songId) {
  const struct PlayerSlot *slots = gSound_PlayerSlots;
  const struct SongEntry *songs = gSongTable;
  const struct SongEntry *song = &songs[songId];

  MPlayStart(slots[song->slot].player, song->header);
}

/// Starts the song unless its selected player is already playing it.
void m4aSongNumStartOrChange(u16 songId) {
  const struct PlayerSlot *slots = gSound_PlayerSlots;
  const struct SongEntry *songs = gSongTable;
  const struct SongEntry *song = &songs[songId];
  struct SoundPlayer *player = slots[song->slot].player;

  if (player->header != song->header)
    MPlayStart(player, song->header);
  else if ((player->status & 0xFFFF) == 0 ||
           (player->status & SOUND_PLAYER_PAUSED))
    MPlayStart(player, song->header);
}

/// Starts a different or finished song, or resumes the same paused song.
void m4aSongNumStartOrContinue(u16 songId) {
  const struct PlayerSlot *slots = gSound_PlayerSlots;
  const struct SongEntry *songs = gSongTable;
  const struct SongEntry *song = &songs[songId];
  struct SoundPlayer *player = slots[song->slot].player;

  if (player->header != song->header)
    MPlayStart(player, song->header);
  else if ((player->status & 0xFFFF) == 0)
    MPlayStart(player, song->header);
  else if (player->status & SOUND_PLAYER_PAUSED)
    MPlayContinue(player);
}

/// Stops the selected player only when it holds this song.
void m4aSongNumStop(u16 songId) {
  const struct PlayerSlot *slots = gSound_PlayerSlots;
  const struct SongEntry *songs = gSongTable;
  const struct SongEntry *song = &songs[songId];
  struct SoundPlayer *player = slots[song->slot].player;

  if (player->header == song->header)
    m4aMPlayStop(player);
}

/// Resumes the selected player only when it holds this song.
void m4aSongNumContinue(u16 songId) {
  const struct PlayerSlot *slots = gSound_PlayerSlots;
  const struct SongEntry *songs = gSongTable;
  const struct SongEntry *song = &songs[songId];
  struct SoundPlayer *player = slots[song->slot].player;

  if (player->header == song->header)
    MPlayContinue(player);
}

void m4aMPlayAllStop(void) {
  s32 playerIndex;

  for (playerIndex = 0; playerIndex < SOUND_PLAYER_COUNT; playerIndex++)
    m4aMPlayStop(gSound_PlayerSlots[playerIndex].player);
}

void m4aMPlayContinue(struct SoundPlayer *player) { MPlayContinue(player); }

void m4aMPlayAllContinue(void) {
  s32 playerIndex;

  for (playerIndex = 0; playerIndex < SOUND_PLAYER_COUNT; playerIndex++)
    MPlayContinue(gSound_PlayerSlots[playerIndex].player);
}

void m4aMPlayFadeOut(struct SoundPlayer *player, u16 speed) {
  MPlayFadeOut(player, speed);
}

/// Begins a fade that preserves track state for later resumption.
void m4aMPlayFadeOutTemporarily(struct SoundPlayer *player, u16 speed) {
  MPlayStartFade(player, speed, SOUND_FADE_FULL_VOLUME | SOUND_FADE_PAUSE);
}

/// Starts a fade from zero volume and resumes the player.
void m4aMPlayFadeIn(struct SoundPlayer *player, u16 speed) {
  MPlayStartFade(player, speed, SOUND_FADE_IN);
}

/// Initializes active tracks still carrying their reset flag.
void m4aMPlayImmInit(struct SoundPlayer *player) {
  s32 count = player->trackCount;
  struct SoundTrack *track = player->tracks;

  while (count > 0) {
    if (track->flags & SOUND_TRACK_ACTIVE) {
      if (track->flags & SOUND_TRACK_RESET) {
        // Clear the 64-byte settings prefix, retaining the sequence cursor.
        Clear64byte(track);
        track->flags = SOUND_TRACK_ACTIVE;
        track->bendRange = 2;
        track->volumeScale = 0x40;
        track->lfoSpeed = 22;
        track->voice.kind = 1;
      }
    }
    count--;
    track++;
  }
}

/// Resets CGB hardware, installs its command handlers and clears four channel
/// records.
void MPlayExtender(struct CgbNote *notes) {
  struct SoundWork *work;
  u32 lock;

  REG_SOUNDCNT_X = SOUNDCNT_X_ENABLE | SOUNDCNT_X_ALL_TONES_ON;
  REG_SOUNDCNT_L = 0;
  REG_NR12 = 8;
  REG_NR22 = 8;
  REG_NR42 = 8;
  REG_NR14 = 0x80;
  REG_NR24 = 0x80;
  REG_NR44 = 0x80;
  REG_NR30 = 0;
  REG_NR50 = 0x77;

  work = SOUND_WORK;
  lock = work->lock;
  if (lock != SOUND_LOCK)
    return;
  work->lock = lock + 1;

  // Replace generic commands with the CGB-capable handlers.
  gSound_CommandTable[SOUND_COMMAND_WORK_BYTE] = ply_memacc;
  gSound_CommandTable[SOUND_COMMAND_LFO_SPEED] = ply_lfos;
  gSound_CommandTable[SOUND_COMMAND_MODULATION] = ply_mod;
  gSound_CommandTable[SOUND_COMMAND_EXTENDED] = ply_xcmd;
  gSound_CommandTable[SOUND_COMMAND_RELEASE_KEY] = ply_endtie;
  gSound_CommandTable[SOUND_COMMAND_PCM_RATE] = (SoundCommand)SampleFreqSet;
  gSound_CommandTable[SOUND_COMMAND_STOP_TRACK] = TrackStop;
  gSound_CommandTable[SOUND_COMMAND_FADE] = (SoundCommand)FadeOutBody;
  gSound_CommandTable[SOUND_COMMAND_CALCULATE_OUTPUT] = TrkVolPitSet;

  work->cgbNotes = notes;
  work->cgbUpdate = CgbSound;
  work->cgbMute = CgbOscOff;
  work->cgbFrequency = MidiKeyToCgbFreq;
  work->maxLines = SOUND_MAX_LINES;

  // Clear note state, then assign the four hardware routing masks.
  {
    u32 zero = 0;

    Bios_CpuSet((void *)&zero, notes,
                0x05000000 | (sizeof(struct CgbNote) * 4 / 4));
  }
  notes[0].kind = 1;
  notes[0].channelBits = 0x11;
  notes[1].kind = 2;
  notes[1].channelBits = 0x22;
  notes[2].kind = 3;
  notes[2].channelBits = 0x44;
  notes[3].kind = 4;
  notes[3].channelBits = 0x88;

  work->lock = lock;
}
