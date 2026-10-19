#include "types.h"
#include "audio_engine.h"
#include "ram_buffer.h"

u32 Audio_UmulHigh32(u32 left, u32 right);
extern u8 gSound_PcmPitchCodes[];
extern u32 gSound_PcmFrequencySteps[];

/// Interpolates adjacent key frequencies and scales them by the wave sample
/// frequency.
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

/// Provides the empty callback retained by the sound driver.
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
void MusicPlayer_BeginFadeOut(struct SoundPlayer *player, u16 speed);
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
void MusicTrack_CalcOutput(struct SoundPlayer *player,
                           struct SoundTrack *track);
void Cgb_UpdateChannels(void);
void CgbChannel_Mute(u8 channel);
s32 Cgb_KeyToFrequency(u8 kind, u8 key, u8 fine);
extern SoundCommand gSound_CommandTable[36];

/// Clears the paused flag while holding the player lock.
void Audio_ResumePlayer(struct SoundPlayer *player) {
  if (player->lock == SOUND_LOCK) {
    player->lock++;
    player->status &= ~SOUND_PLAYER_PAUSED;
    player->lock = SOUND_LOCK;
  }
}

/// Starts a fade under the player lock, resuming the player for fade-ins.
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
void MusicPlayer_BeginFadeOut(struct SoundPlayer *player, u16 speed) {
  MusicPlayer_SetFadeParameters(player, speed, SOUND_FADE_FULL_VOLUME);
}

/// Copies the RAM mixer, initializes both audio paths and configures every
/// player.
void Audio_Initialize(void) {
  s32 playerIndex;

  // Copy the mixer's executable block to its RAM buffer.
  Bios_CpuSet((void *)((u32)Sound_Mixer & ~1), Ram_SoundMixer, 0x04000100);
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

/// Runs one mixer frame through the driver entry point.
void AudioEngine_RunMixer(void) { AudioEngine_RunMixerTick(); }

/// Starts the song on the player selected by its song-table entry.
void Audio_PlaySound(u16 songId) {
  const struct PlayerSlot *slots = gSound_PlayerSlots;
  const struct SongEntry *songs = gSound_SongTable;
  const struct SongEntry *song = &songs[songId];

  MusicPlayer_StartSong(slots[song->slot].player, song->header);
}

/// Starts the song unless its selected player is already playing it.
void Audio_PlaySoundIfInactive(u16 songId) {
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

/// Starts a different or finished song, or resumes the same paused song.
void Audio_PlayOrResumeSound(u16 songId) {
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

/// Stops the selected player only when it holds this song.
void Audio_StopSound(u16 songId) {
  const struct PlayerSlot *slots = gSound_PlayerSlots;
  const struct SongEntry *songs = gSound_SongTable;
  const struct SongEntry *song = &songs[songId];
  struct SoundPlayer *player = slots[song->slot].player;

  if (player->header == song->header)
    MusicPlayer_Stop(player);
}

/// Resumes the selected player only when it holds this song.
void Audio_ResumeSound(u16 songId) {
  const struct PlayerSlot *slots = gSound_PlayerSlots;
  const struct SongEntry *songs = gSound_SongTable;
  const struct SongEntry *song = &songs[songId];
  struct SoundPlayer *player = slots[song->slot].player;

  if (player->header == song->header)
    Audio_ResumePlayer(player);
}

/// Stops every configured song player.
void Audio_StopAllPlayers(void) {
  s32 playerIndex;

  for (playerIndex = 0; playerIndex < SOUND_PLAYER_COUNT; playerIndex++)
    MusicPlayer_Stop(gSound_PlayerSlots[playerIndex].player);
}

/// Resumes this player through the shared pause-state operation.
void MusicPlayer_Resume(struct SoundPlayer *player) {
  Audio_ResumePlayer(player);
}

/// Resumes every configured song player.
void Audio_ResumeAllPlayers(void) {
  s32 playerIndex;

  for (playerIndex = 0; playerIndex < SOUND_PLAYER_COUNT; playerIndex++)
    Audio_ResumePlayer(gSound_PlayerSlots[playerIndex].player);
}

/// Begins the standard stop-and-clear fade.
void MusicPlayer_FadeOut(struct SoundPlayer *player, u16 speed) {
  MusicPlayer_BeginFadeOut(player, speed);
}

/// Begins a fade that preserves track state for later resumption.
void MusicPlayer_FadeOutPause(struct SoundPlayer *player, u16 speed) {
  MusicPlayer_SetFadeParameters(player, speed,
                                SOUND_FADE_FULL_VOLUME | SOUND_FADE_PAUSE);
}

/// Starts a fade from zero volume and resumes the player.
void MusicPlayer_FadeIn(struct SoundPlayer *player, u16 speed) {
  MusicPlayer_SetFadeParameters(player, speed, SOUND_FADE_IN);
}

/// Initializes active tracks still carrying their reset flag.
void MusicPlayer_ResetActiveTracks(struct SoundPlayer *player) {
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

/// Resets CGB hardware, installs its command handlers and clears four channel
/// records.
void CgbAudio_Initialize(struct CgbNote *notes) {
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
  gSound_CommandTable[SOUND_COMMAND_WORK_BYTE] = MusicTrack_OperateWorkByte;
  gSound_CommandTable[SOUND_COMMAND_LFO_SPEED] =
      MusicTrack_SetLfoSpeedFromCommand;
  gSound_CommandTable[SOUND_COMMAND_MODULATION] =
      MusicTrack_SetModulationFromCommand;
  gSound_CommandTable[SOUND_COMMAND_EXTENDED] =
      MusicTrack_DispatchStreamCommand;
  gSound_CommandTable[SOUND_COMMAND_RELEASE_KEY] = MusicTrack_ReleaseKey;
  gSound_CommandTable[SOUND_COMMAND_PCM_RATE] =
      (SoundCommand)AudioEngine_SetPcmRate;
  gSound_CommandTable[SOUND_COMMAND_STOP_TRACK] = MusicTrack_Stop;
  gSound_CommandTable[SOUND_COMMAND_FADE] = (SoundCommand)MusicPlayer_StepFade;
  gSound_CommandTable[SOUND_COMMAND_CALCULATE_OUTPUT] = MusicTrack_CalcOutput;

  work->cgbNotes = notes;
  work->cgbUpdate = Cgb_UpdateChannels;
  work->cgbMute = CgbChannel_Mute;
  work->cgbFrequency = Cgb_KeyToFrequency;
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
