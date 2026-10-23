#include "types.h"
#include "audio_engine.h"
#include "ram_buffer.h"
#include "../../include/system/io_reg.h"

u32 umul3232H32(u32 left, u32 right);
extern u8 gScaleTable[];
extern u32 gFreqTable[];

/// Interpolates adjacent key frequencies and scales them by the wave sample
/// frequency. Called by ply_note for each PCM note. Returns the wave's
/// frequency word (offset 4) times 2^((key - 60) / 12) / 1024, with `pitch`
/// (0 to 255) moving it in a straight line towards the next key; keys above
/// 178 play key 178 at pitch 255.
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

  // A key's code: the low four bits pick the semitone's step (2^31 times
  // 2^(n / 12)), the high four bits the octaves to shift it down, 9 for key 60.
  lowerFrequency = gScaleTable[keyIndex];
  lowerFrequency = gFreqTable[lowerFrequency & 15] >> (lowerFrequency >> 4);
  upperFrequency = gScaleTable[keyIndex + 1];
  upperFrequency = gFreqTable[upperFrequency & 15] >> (upperFrequency >> 4);

  return umul3232H32(
      *(s32 *)((u8 *)wave + 4),
      lowerFrequency +
          umul3232H32(upperFrequency - lowerFrequency, pitchScale));
}

/// Does nothing; nothing in ☀️ refers to it. The driver's empty callback is
/// DummyFunc (empty_callback.c).
void UnusedDummyFunc(void) {}

// The music player table supplies the music player count and the mixer's line
// limit.
extern char gNumMusicPlayers[];
extern char gMaxLines[];
#define NUM_MUSIC_PLAYERS ((u16)(u32)gNumMusicPlayers)
#define MAX_LINES ((u32)gMaxLines)

void SoundMainRAM_Rom(void);
void SoundInit(struct SoundInfo *work);
void m4aSoundMode(u32 mode);
void SoundMain(void);
void MPlayOpen(struct MusicPlayerInfo *player, struct MusicPlayerTrack *tracks,
               u8 count);
void MPlayStart(struct MusicPlayerInfo *player,
                const struct SongHeader *header);
void m4aMPlayStop(struct MusicPlayerInfo *player);
void MPlayFadeOut(struct MusicPlayerInfo *player, u16 speed);
void MPlayContinue(struct MusicPlayerInfo *player);
extern struct SoundInfo gSoundInfo;
extern struct CgbChannel gCgbChans[4];
extern u8 gMPlayMemAccArea[];
extern const struct MusicPlayer gMPlayTable[];
extern const struct Song gSongTable[];

void ply_memacc(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_lfos(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_mod(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void ply_endtie(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void SampleFreqSet(u32 mode);
void TrackStop(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void FadeOutBody(struct MusicPlayerInfo *player);
void TrkVolPitSet(struct MusicPlayerInfo *player,
                  struct MusicPlayerTrack *track);
void CgbSound(void);
void CgbOscOff(u8 channel);
s32 MidiKeyToCgbFreq(u8 kind, u8 key, u8 fine);
extern MPlayFunc gMPlayJumpTable[36];

/// Clears the music player's paused flag, raising its ident while it works.
/// Called by the resume and play-or-resume commands.
void MPlayContinue(struct MusicPlayerInfo *player) {
  if (player->ident == ID_NUMBER) {
    player->ident++;
    player->status &= ~MUSICPLAYER_STATUS_PAUSE;
    player->ident = ID_NUMBER;
  }
}

/// Starts a fade with the music player's ident raised, resuming the music
/// player for fade-ins: FadeOutBody moves the level 16 of 0x100 every `period`
/// frames, so a whole fade takes 16 * period frames. pret writes this body out
/// in each of MPlayFadeOut, m4aMPlayFadeOutTemporarily and m4aMPlayFadeIn; the
/// name is ours.
static __inline__ void MPlayStartFade(struct MusicPlayerInfo *player,
                                      u16 period, u16 volume) {
  if (player->ident == ID_NUMBER) {
    player->ident++;
    player->fadeCounter = period;
    player->fadePeriod = period;
    player->fadeVolume = volume;
    if (volume & FADE_IN)
      player->status &= ~MUSICPLAYER_STATUS_PAUSE;
    player->ident = ID_NUMBER;
  }
}

/// Begins a fade that stops and clears tracks when its volume reaches zero.
/// Called by m4aMPlayFadeOut.
void MPlayFadeOut(struct MusicPlayerInfo *player, u16 speed) {
  MPlayStartFade(player, speed, SOUND_FADE_FULL_VOLUME);
}

/// The bytes of mixer code m4aSoundInit copies to IWRAM.
#define SOUND_MIXER_CODE_SIZE 0x400

/// Copies the RAM mixer to IWRAM, initializes the PCM and CGB paths and
/// configures every music player from gMPlayTable. Called by Sound_Initialize
/// at power-on. Unlike pret's games, it opens no extra music players for
/// cries.
void m4aSoundInit(void) {
  s32 playerIndex;

  // SoundMainRAM_Rom is a Thumb symbol: its address has bit 0 set
  Bios_CpuSet((void *)((u32)SoundMainRAM_Rom & ~1), Ram_SoundMixer,
              CPU_SET_32BIT | (SOUND_MIXER_CODE_SIZE / 4));
  SoundInit(&gSoundInfo);
  MPlayExtender(gCgbChans);
  m4aSoundMode(AUDIO_INITIAL_MODE);
  // Each music player owns its tracks but shares the memacc area.
  for (playerIndex = 0; playerIndex < NUM_MUSIC_PLAYERS; playerIndex++) {
    struct MusicPlayerInfo *player = gMPlayTable[playerIndex].info;

    MPlayOpen(player, gMPlayTable[playerIndex].track,
              gMPlayTable[playerIndex].numTracks);
    player->checkPriority = gMPlayTable[playerIndex].checkPriority;
    player->workBytes = gMPlayMemAccArea;
  }
}

/// Runs one frame of the sound driver through SoundMain. Exported first in the
/// sound far-call table as m4aSoundMainFar; nothing in ☀️ calls it by that
/// name, and the VBlank handler mixes through Mixer_Enter instead.
void m4aSoundMain(void) { SoundMain(); }

/// Starts the song on the music player its song-table entry names. Called by
/// Sound_PlaySong for music and fanfares; it starts sound effects (songs
/// from 100) itself, so that those naming player 7 can take a free player
/// from 7 down to 4.
void m4aSongNumStart(u16 songId) {
  const struct MusicPlayer *mplayTable = gMPlayTable;
  const struct Song *songTable = gSongTable;
  const struct Song *song = &songTable[songId];

  MPlayStart(mplayTable[song->ms].info, song->header);
}

/// Unused: starts the song unless its music player is already playing it.
/// Nothing in ☀️ calls it.
UNUSED void m4aSongNumStartOrChange(u16 songId) {
  const struct MusicPlayer *mplayTable = gMPlayTable;
  const struct Song *songTable = gSongTable;
  const struct Song *song = &songTable[songId];
  struct MusicPlayerInfo *player = mplayTable[song->ms].info;

  if (player->header != song->header)
    MPlayStart(player, song->header);
  else if ((player->status & MUSICPLAYER_STATUS_TRACK) == 0 ||
           (player->status & MUSICPLAYER_STATUS_PAUSE))
    MPlayStart(player, song->header);
}

/// Unused: starts a different or finished song, or resumes the same paused
/// song. Nothing in ☀️ calls it.
UNUSED void m4aSongNumStartOrContinue(u16 songId) {
  const struct MusicPlayer *mplayTable = gMPlayTable;
  const struct Song *songTable = gSongTable;
  const struct Song *song = &songTable[songId];
  struct MusicPlayerInfo *player = mplayTable[song->ms].info;

  if (player->header != song->header)
    MPlayStart(player, song->header);
  else if ((player->status & MUSICPLAYER_STATUS_TRACK) == 0)
    MPlayStart(player, song->header);
  else if (player->status & MUSICPLAYER_STATUS_PAUSE)
    MPlayContinue(player);
}

/// Unused: stops the song's music player only when it holds this song. Nothing
/// in ☀️ calls it.
UNUSED void m4aSongNumStop(u16 songId) {
  const struct MusicPlayer *mplayTable = gMPlayTable;
  const struct Song *songTable = gSongTable;
  const struct Song *song = &songTable[songId];
  struct MusicPlayerInfo *player = mplayTable[song->ms].info;

  if (player->header == song->header)
    m4aMPlayStop(player);
}

/// Unused: resumes the song's music player only when it holds this song.
/// Nothing in ☀️ calls it.
UNUSED void m4aSongNumContinue(u16 songId) {
  const struct MusicPlayer *mplayTable = gMPlayTable;
  const struct Song *songTable = gSongTable;
  const struct Song *song = &songTable[songId];
  struct MusicPlayerInfo *player = mplayTable[song->ms].info;

  if (player->header == song->header)
    MPlayContinue(player);
}

/// Unused: stops every music player. Only Sound_StopAllMusicPlayers calls it,
/// and only a bare far-call veneer, which no ☀️ code names, leads there.
UNUSED void m4aMPlayAllStop(void) {
  s32 playerIndex;

  for (playerIndex = 0; playerIndex < NUM_MUSIC_PLAYERS; playerIndex++)
    m4aMPlayStop(gMPlayTable[playerIndex].info);
}

/// Unused: resumes `player`. Nothing in ☀️ calls it.
UNUSED void m4aMPlayContinue(struct MusicPlayerInfo *player) {
  MPlayContinue(player);
}

/// Unused: resumes every music player. Only Sound_ResumeAllMusicPlayers calls
/// it, and only a bare far-call veneer, which no ☀️ code names, leads there.
UNUSED void m4aMPlayAllContinue(void) {
  s32 playerIndex;

  for (playerIndex = 0; playerIndex < NUM_MUSIC_PLAYERS; playerIndex++)
    MPlayContinue(gMPlayTable[playerIndex].info);
}

/// Begins the standard stop-and-clear fade. Called by Sound_PlaySong's fade
/// controls.
void m4aMPlayFadeOut(struct MusicPlayerInfo *player, u16 speed) {
  MPlayFadeOut(player, speed);
}

/// Unused: begins a fade that pauses the music player at zero volume, keeping
/// its tracks' state for a later resume. Nothing in ☀️ calls it.
UNUSED void m4aMPlayFadeOutTemporarily(struct MusicPlayerInfo *player,
                                       u16 speed) {
  MPlayStartFade(player, speed, SOUND_FADE_FULL_VOLUME | TEMPORARY_FADE);
}

/// Unused: starts a fade from zero volume and resumes the music player. Nothing
/// in ☀️ calls it.
UNUSED void m4aMPlayFadeIn(struct MusicPlayerInfo *player, u16 speed) {
  MPlayStartFade(player, speed, FADE_IN);
}

/// Unused: initializes active tracks still carrying their reset flag: clears
/// their settings and gives them the default bend range, volume and LFO speed.
/// Nothing in ☀️ calls it.
UNUSED void m4aMPlayImmInit(struct MusicPlayerInfo *player) {
  s32 count = player->trackCount;
  struct MusicPlayerTrack *track = player->tracks;

  while (count > 0) {
    if (track->flags & MPT_FLG_EXIST) {
      if (track->flags & MPT_FLG_START) {
        // Clear the 64-byte settings prefix, retaining the sequence cursor.
        Clear64byte(track);
        track->flags = MPT_FLG_EXIST;
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

/// Resets the CGB sound hardware, installs the CGB-aware command handlers and
/// callbacks in the sound work, and clears the four channels' `notes`.
/// Called by m4aSoundInit. The hardware reset happens even when the driver's
/// ident is raised; the rest is then skipped.
void MPlayExtender(struct CgbChannel *notes) {
  struct SoundInfo *work;
  u32 ident;

  REG_SOUNDCNT_X = SOUNDCNT_X_ENABLE | SOUNDCNT_X_ALL_TONES_ON;
  REG_SOUNDCNT_L = 0;
  // The square and noise channels silent with a rising envelope, restarted;
  // the wave channel off; the master volume full on both sides
  REG_NR12 = 8;
  REG_NR22 = 8;
  REG_NR42 = 8;
  REG_NR14 = 0x80;
  REG_NR24 = 0x80;
  REG_NR44 = 0x80;
  REG_NR30 = 0;
  REG_NR50 = 0x77;

  work = SOUND_INFO_PTR;
  ident = work->ident;
  if (ident != ID_NUMBER)
    return;
  work->ident = ident + 1;

  // Replace generic commands with the CGB-capable handlers.
  gMPlayJumpTable[MPLAY_JUMP_MEMACC] = ply_memacc;
  gMPlayJumpTable[MPLAY_JUMP_LFOS] = ply_lfos;
  gMPlayJumpTable[MPLAY_JUMP_MOD] = ply_mod;
  gMPlayJumpTable[MPLAY_JUMP_XCMD] = ply_xcmd;
  gMPlayJumpTable[MPLAY_JUMP_EOT] = ply_endtie;
  gMPlayJumpTable[MPLAY_JUMP_SAMPLE_FREQ_SET] =
      MPLAY_FUNC_CAST(MPlayFunc, SampleFreqSet);
  gMPlayJumpTable[MPLAY_JUMP_TRACK_STOP] = TrackStop;
  gMPlayJumpTable[MPLAY_JUMP_FADE_OUT_BODY] =
      MPLAY_FUNC_CAST(MPlayFunc, FadeOutBody);
  gMPlayJumpTable[MPLAY_JUMP_TRK_VOL_PIT_SET] = TrkVolPitSet;

  work->cgbNotes = notes;
  work->cgbUpdate = CgbSound;
  work->cgbMute = CgbOscOff;
  work->cgbFrequency = MidiKeyToCgbFreq;
  work->lineLimit = MAX_LINES;

  // Clear note state, then assign the four hardware routing masks: channel
  // n's NR51 bits, n - 1 for the right speaker and n + 3 for the left.
  {
    u32 zero = 0;

    Bios_CpuSet((void *)&zero, notes,
                CPU_SET_SRC_FIXED | CPU_SET_32BIT |
                    (sizeof(struct CgbChannel) * 4 / 4));
  }
  notes[0].kind = 1;
  notes[0].channelBits = 0x11;
  notes[1].kind = 2;
  notes[1].channelBits = 0x22;
  notes[2].kind = 3;
  notes[2].channelBits = 0x44;
  notes[3].kind = 4;
  notes[3].channelBits = 0x88;

  work->ident = ident;
}
