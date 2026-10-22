#include "types.h"
#include "audio_engine.h"
#include "ram_buffer.h"
#include "../../include/system/io_reg.h"

u32 Audio_UmulHigh32(u32 left, u32 right);
extern u8 gSound_PcmPitchCodes[];
extern u32 gSound_PcmFrequencySteps[];

/// Interpolates adjacent key frequencies and scales them by the wave sample
/// frequency. Called by the song player for each PCM note. Returns the wave's
/// frequency word (offset 4) times 2^((key - 60) / 12) / 1024, with `pitch`
/// (0 to 255) moving it in a straight line towards the next key; keys above
/// 178 play key 178 at pitch 255.
s32 Pcm_KeyToFrequency(void *wave, u8 key, u8 pitch) {
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
  lowerFrequency = gSound_PcmPitchCodes[keyIndex];
  lowerFrequency =
      gSound_PcmFrequencySteps[lowerFrequency & 15] >> (lowerFrequency >> 4);
  upperFrequency = gSound_PcmPitchCodes[keyIndex + 1];
  upperFrequency =
      gSound_PcmFrequencySteps[upperFrequency & 15] >> (upperFrequency >> 4);

  return Audio_UmulHigh32(
      *(s32 *)((u8 *)wave + 4),
      lowerFrequency +
          Audio_UmulHigh32(upperFrequency - lowerFrequency, pitchScale));
}

/// Does nothing; nothing in ☀️ refers to it. The driver's empty callback is
/// Audio_EmptyCallback (empty_callback.c).
void Audio_NoopStub(void) {}

// The sound data's symbols supply the player count and maximum track count.
extern char Sound_PlayerCount[];
extern char Sound_MaxLines[];
#define SOUND_PLAYER_COUNT ((u16)(u32)Sound_PlayerCount)
#define SOUND_MAX_LINES ((u32)Sound_MaxLines)

void Sound_Mixer(void);
void AudioEngine_Initialize(struct SoundWork *work);
void AudioEngine_SetMode(u32 mode);
void AudioEngine_RunMixerTick(void);
void MusicPlayer_Initialize(struct SoundPlayer *player,
                            struct SoundTrack *tracks, u8 count);
void MusicPlayer_StartSong(struct SoundPlayer *player,
                           const struct SequenceHeader *header);
void MusicPlayer_Stop(struct SoundPlayer *player);
void MusicPlayer_StartFadeOut(struct SoundPlayer *player, u16 speed);
void Audio_ResumePlayer(struct SoundPlayer *player);
extern struct SoundWork gSound_Work;
extern struct CgbNote gSound_CgbNotes[4];
extern u8 gSound_WorkBytes[];
extern const struct PlayerSlot gSound_PlayerSlots[];
extern const struct SongEntry gSound_SongTable[];

void MusicTrack_OperateWorkByte(struct SoundPlayer *player,
                                struct SoundTrack *track);
void MusicTrack_SetLfoSpeedFromCommand(struct SoundPlayer *player,
                                       struct SoundTrack *track);
void MusicTrack_SetModulationFromCommand(struct SoundPlayer *player,
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

/// Clears the player's paused flag while holding the player lock. Called by
/// the resume and play-or-resume commands.
void Audio_ResumePlayer(struct SoundPlayer *player) {
  if (player->lock == SOUND_LOCK) {
    player->lock++;
    player->status &= ~SOUND_PLAYER_PAUSED;
    player->lock = SOUND_LOCK;
  }
}

/// Starts a fade under the player lock, resuming the player for fade-ins:
/// MusicPlayer_StepFade moves the level 16 of 0x100 every `period` frames, so
/// a whole fade takes 16 * period frames.
static __inline__ void MusicPlayer_SetFadeParameters(struct SoundPlayer *player,
                                                     u16 period, u16 volume) {
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
/// Called by MusicPlayer_FadeOut.
void MusicPlayer_StartFadeOut(struct SoundPlayer *player, u16 speed) {
  MusicPlayer_SetFadeParameters(player, speed, SOUND_FADE_FULL_VOLUME);
}

/// The bytes of mixer code Audio_Initialize copies to IWRAM.
#define SOUND_MIXER_CODE_SIZE 0x400

/// Copies the RAM mixer to IWRAM, initializes the PCM and CGB paths and
/// configures every player from gSound_PlayerSlots. Called by
/// Audio_InitializeRuntimeDefaults at power-on.
void Audio_Initialize(void) {
  s32 playerIndex;

  // Sound_Mixer is a Thumb symbol: its address has bit 0 set
  Bios_CpuSet((void *)((u32)Sound_Mixer & ~1), Ram_SoundMixer,
              CPU_SET_32BIT | (SOUND_MIXER_CODE_SIZE / 4));
  AudioEngine_Initialize(&gSound_Work);
  CgbAudio_Initialize(gSound_CgbNotes);
  AudioEngine_SetMode(AUDIO_INITIAL_MODE);
  // Each configured player owns its tracks but shares the work-byte area.
  for (playerIndex = 0; playerIndex < SOUND_PLAYER_COUNT; playerIndex++) {
    struct SoundPlayer *player = gSound_PlayerSlots[playerIndex].player;

    MusicPlayer_Initialize(player, gSound_PlayerSlots[playerIndex].tracks,
                           gSound_PlayerSlots[playerIndex].trackCount);
    player->checkPriority = gSound_PlayerSlots[playerIndex].checkPriority;
    player->workBytes = gSound_WorkBytes;
  }
}

/// Runs one frame of the sound driver (AudioEngine_RunMixerTick,
/// MusicPlayer2000's SoundMain). Exported first in the sound far-call table
/// as AudioEngine_RunMixerFar; nothing in ☀️ calls it by that name.
void AudioEngine_RunMixer(void) { AudioEngine_RunMixerTick(); }

/// Starts the song on the player selected by its song-table entry. Called by
/// AudioCommand_Play for music and fanfares; it starts sound effects (cues
/// from 100) itself, so that those naming player 7 can take a free player
/// from 7 down to 4.
void Audio_PlaySound(u16 songId) {
  const struct PlayerSlot *slots = gSound_PlayerSlots;
  const struct SongEntry *songs = gSound_SongTable;
  const struct SongEntry *song = &songs[songId];

  MusicPlayer_StartSong(slots[song->slot].player, song->header);
}

/// Unused: starts the song unless its selected player is already playing it.
/// Nothing in ☀️ calls it.
UNUSED void Audio_PlaySoundIfInactive(u16 songId) {
  const struct PlayerSlot *slots = gSound_PlayerSlots;
  const struct SongEntry *songs = gSound_SongTable;
  const struct SongEntry *song = &songs[songId];
  struct SoundPlayer *player = slots[song->slot].player;

  if (player->header != song->header)
    MusicPlayer_StartSong(player, song->header);
  else if ((player->status & 0xFFFF) == 0 ||
           (player->status & SOUND_PLAYER_PAUSED))
    MusicPlayer_StartSong(player, song->header);
}

/// Unused: starts a different or finished song, or resumes the same paused
/// song. Nothing in ☀️ calls it.
UNUSED void Audio_PlayOrResumeSound(u16 songId) {
  const struct PlayerSlot *slots = gSound_PlayerSlots;
  const struct SongEntry *songs = gSound_SongTable;
  const struct SongEntry *song = &songs[songId];
  struct SoundPlayer *player = slots[song->slot].player;

  if (player->header != song->header)
    MusicPlayer_StartSong(player, song->header);
  else if ((player->status & 0xFFFF) == 0)
    MusicPlayer_StartSong(player, song->header);
  else if (player->status & SOUND_PLAYER_PAUSED)
    Audio_ResumePlayer(player);
}

/// Unused: stops the selected player only when it holds this song. Nothing in ☀️
/// calls it.
UNUSED void Audio_StopSound(u16 songId) {
  const struct PlayerSlot *slots = gSound_PlayerSlots;
  const struct SongEntry *songs = gSound_SongTable;
  const struct SongEntry *song = &songs[songId];
  struct SoundPlayer *player = slots[song->slot].player;

  if (player->header == song->header)
    MusicPlayer_Stop(player);
}

/// Unused: resumes the selected player only when it holds this song. Nothing in
/// ☀️ calls it.
UNUSED void Audio_ResumeSound(u16 songId) {
  const struct PlayerSlot *slots = gSound_PlayerSlots;
  const struct SongEntry *songs = gSound_SongTable;
  const struct SongEntry *song = &songs[songId];
  struct SoundPlayer *player = slots[song->slot].player;

  if (player->header == song->header)
    Audio_ResumePlayer(player);
}

/// Unused: stops every player. Only AudioCommand_StopAllPlayers calls it, and
/// only a bare far-call veneer, which no ☀️ code names, leads there.
UNUSED void Audio_StopAllPlayers(void) {
  s32 playerIndex;

  for (playerIndex = 0; playerIndex < SOUND_PLAYER_COUNT; playerIndex++)
    MusicPlayer_Stop(gSound_PlayerSlots[playerIndex].player);
}

/// Unused: resumes `player`. Nothing in ☀️ calls it.
UNUSED void MusicPlayer_Resume(struct SoundPlayer *player) {
  Audio_ResumePlayer(player);
}

/// Unused: resumes every player. Only AudioCommand_ResumeAllPlayers calls it,
/// and only a bare far-call veneer, which no ☀️ code names, leads there.
UNUSED void Audio_ResumeAllPlayers(void) {
  s32 playerIndex;

  for (playerIndex = 0; playerIndex < SOUND_PLAYER_COUNT; playerIndex++)
    Audio_ResumePlayer(gSound_PlayerSlots[playerIndex].player);
}

/// Begins the standard stop-and-clear fade. Called by AudioCommand_Play's fade
/// cues.
void MusicPlayer_FadeOut(struct SoundPlayer *player, u16 speed) {
  MusicPlayer_StartFadeOut(player, speed);
}

/// Unused: begins a fade that pauses the player at zero volume, keeping its
/// tracks' state for a later resume. Nothing in ☀️ calls it.
UNUSED void MusicPlayer_FadeOutPause(struct SoundPlayer *player, u16 speed) {
  MusicPlayer_SetFadeParameters(player, speed,
                                SOUND_FADE_FULL_VOLUME | SOUND_FADE_PAUSE);
}

/// Unused: starts a fade from zero volume and resumes the player. Nothing in ☀️
/// calls it.
UNUSED void MusicPlayer_FadeIn(struct SoundPlayer *player, u16 speed) {
  MusicPlayer_SetFadeParameters(player, speed, SOUND_FADE_IN);
}

/// Unused: initializes active tracks still carrying their reset flag: clears
/// their settings and gives them the default bend range, volume and LFO speed.
/// Nothing in ☀️ calls it.
UNUSED void MusicPlayer_ResetActiveTracks(struct SoundPlayer *player) {
  s32 count = player->trackCount;
  struct SoundTrack *track = player->tracks;

  while (count > 0) {
    if (track->flags & SOUND_TRACK_ACTIVE) {
      if (track->flags & SOUND_TRACK_RESET) {
        // Clear the 64-byte settings prefix, retaining the sequence cursor.
        AudioCommand_InvokeSlot35(track);
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

/// Resets the CGB sound hardware, installs the CGB-aware command handlers and
/// callbacks in the sound work, and clears the four channel records `notes`.
/// Called by Audio_Initialize. The hardware reset happens even when the
/// driver's lock is held; the rest is then skipped.
void CgbAudio_Initialize(struct CgbNote *notes) {
  struct SoundWork *work;
  u32 lock;

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

  work = SOUND_WORK;
  lock = work->lock;
  if (lock != SOUND_LOCK)
    return;
  work->lock = lock + 1;

  // Replace generic commands with the CGB-capable handlers.
  gSound_CommandTable[SOUND_COMMAND_WORK_BYTE] = MusicTrack_OperateWorkByte;
  gSound_CommandTable[SOUND_COMMAND_LFO_SPEED] =
      MusicTrack_SetLfoSpeedFromCommand;
  gSound_CommandTable[SOUND_COMMAND_MODULATION] =
      MusicTrack_SetModulationFromCommand;
  gSound_CommandTable[SOUND_COMMAND_EXTENDED] =
      MusicTrack_DispatchStreamCommand;
  gSound_CommandTable[SOUND_COMMAND_RELEASE_KEY] = MusicTrack_ReleaseKey;
  gSound_CommandTable[SOUND_COMMAND_PCM_RATE] =
      SOUND_COMMAND_CAST(SoundCommand, AudioEngine_SetPcmRate);
  gSound_CommandTable[SOUND_COMMAND_STOP_TRACK] = MusicTrack_Stop;
  gSound_CommandTable[SOUND_COMMAND_FADE] =
      SOUND_COMMAND_CAST(SoundCommand, MusicPlayer_StepFade);
  gSound_CommandTable[SOUND_COMMAND_CALCULATE_OUTPUT] =
      MusicTrack_CalculateOutput;

  work->cgbNotes = notes;
  work->cgbUpdate = Cgb_UpdateChannels;
  work->cgbMute = CgbChannel_Mute;
  work->cgbFrequency = Cgb_KeyToFrequency;
  work->maxLines = SOUND_MAX_LINES;

  // Clear note state, then assign the four hardware routing masks: channel
  // n's NR51 bits, n - 1 for the right speaker and n + 3 for the left.
  {
    u32 zero = 0;

    Bios_CpuSet((void *)&zero, notes,
                CPU_SET_SRC_FIXED | CPU_SET_32BIT |
                    (sizeof(struct CgbNote) * 4 / 4));
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
