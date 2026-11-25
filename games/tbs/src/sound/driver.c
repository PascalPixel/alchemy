/// @file
/// The sound driver's core: its start-up and sound mode, stopping and fading
/// the music players, the four CGB channels, the track controls (tempo,
/// volume, pitch, pan and modulation) and the songs' extended commands.

#include "fixed_math.h"
#include "types.h"
#include "sound/sound.h"
#include "sound/sound_driver.h"
#include "system/io_reg.h"

/// Detaches a note through the jump table entry installed by the driver
/// (RealClearChain).
void ClearChain(struct SoundChannel *note) {
  /* FAKEMATCH(dead): the entry read through its address. */
  MPLAY_FUNC_CAST(void (*)(struct SoundChannel *),
                  *(&gMPlayJumpTable[MPLAY_JUMP_CLEAR_CHAIN]))(note);
}

/// Clears a 64-byte music player or track prefix through the driver's jump
/// table (SoundMainBTM). Called by MPlayOpen and m4aMPlayImmInit.
void Clear64byte(void *block) {
  /* FAKEMATCH(dead): the entry read through its address. */
  MPLAY_FUNC_CAST(void (*)(void *),
                  *(&gMPlayJumpTable[MPLAY_JUMP_CLEAR_64BYTE]))(block);
}

/// The VBlank rate in ten-thousandths of a hertz (SOUND_RATE_E4_UNIT): 59.7275
/// VBlanks a second.
#define SOUND_VBLANK_RATE_E4 597275
#define SOUND_RATE_E4_UNIT 10000
/// CPU cycles from one VBlank to the next: 228 lines of 1232 cycles.
#define SOUND_CYCLES_PER_VBLANK (228 * 1232)
/// 1.0 in the 24-bit fixed point SampleFreqSet takes the rate's reciprocal in.
#define SOUND_RATE_STEP_ONE (1 << 24)

/// The PCM notes and volume SoundInit starts with, until m4aSoundInit sets the
/// game's mode: 8 notes at full volume.
enum { SOUND_INITIAL_PCM_NOTE_COUNT = 8, SOUND_PCM_VOLUME_MAX = 15 };

/// What m4aSoundVSyncOff adds to the work's ident while the FIFO transfers are
/// stopped, so that nothing else takes the driver until m4aSoundVSyncOn.
enum { SOUND_IDENT_VSYNC_OFF = 10 };

/// A sound FIFO DMA's control word while it feeds its FIFO at each request.
#define SOUND_DMA_REPEATING (DMA_REPEAT << 16)
/// Rearms a sound FIFO's DMA for four words at once, not repeated, which
/// leaves it stopped.
#define SOUND_DMA_LAST_TRANSFER                                                \
  ((DMA_ENABLE | DMA_32BIT | DMA_DEST_FIXED) << 16 | 4)

/// Resets the sound hardware, clears the whole sound work block and installs
/// the default PCM driver: stores `work` in SOUND_INFO_PTR, points the FIFO
/// DMAs at its two buffers, fills in the driver callbacks and command table,
/// sets the default rate and finally sets the work's ident to ID_NUMBER.
void SoundInit(struct SoundInfo *work) {
  u32 zero;

  work->ident = 0;
  if (REG_DMA1CNT & SOUND_DMA_REPEATING)
    REG_DMA1CNT = SOUND_DMA_LAST_TRANSFER;
  if (REG_DMA2CNT & SOUND_DMA_REPEATING)
    REG_DMA2CNT = SOUND_DMA_LAST_TRANSFER;
  REG_DMA1CNT_H = DMA_32BIT;
  REG_DMA2CNT_H = DMA_32BIT;
  REG_SOUNDCNT_X = SOUNDCNT_X_ENABLE | SOUNDCNT_X_ALL_TONES_ON;
  REG_SOUNDCNT_H = SOUND_B_FIFO_RESET | SOUND_B_TIMER_0 | SOUND_B_LEFT_OUTPUT |
                   SOUND_A_FIFO_RESET | SOUND_A_TIMER_0 | SOUND_A_RIGHT_OUTPUT |
                   SOUND_ALL_MIX_FULL;
  REG_SOUNDBIAS_H =
      (REG_SOUNDBIAS_H & SOUNDBIAS_H_LEVEL) | SOUNDBIAS_H_RESOLUTION_8BIT;
  REG_DMA1SAD = (u32)work->buffers[0];
  REG_DMA1DAD = (u32)&REG_FIFO_A;
  REG_DMA2SAD = (u32)work->buffers[1];
  REG_DMA2DAD = (u32)&REG_FIFO_B;
  SOUND_INFO_PTR = work;
  zero = 0;
  Bios_CpuSet(&zero, work,
              CPU_SET_SRC_FIXED | CPU_SET_32BIT | sizeof(struct SoundInfo) / 4);
  work->pcmNoteCount = SOUND_INITIAL_PCM_NOTE_COUNT;
  work->volume = SOUND_PCM_VOLUME_MAX;
  work->noteOn = ply_note;
  work->cgbUpdate = DummyFunc;
  work->cgbMute = (void (*)(u8))DummyFunc;
  work->cgbFrequency = (s32 (*)(u8, u8, u8))DummyFunc;
  work->externalVolumePitch = DummyFunc;
  MPlayJumpTableCopy(gMPlayJumpTable);
  work->commands = gMPlayJumpTable;
  // Rate 4 (224 samples a VBlank, 13379 a second) until m4aSoundInit sets
  // the game's mode.
  SampleFreqSet(SOUND_MODE_FREQ_INITIAL);
  work->ident = ID_NUMBER;
}

/// Sets the PCM rate in samples a VBlank (`mode` bits 16 to 19 pick a
/// gPcmSamplesPerVBlankTable entry) and the rates derived from it, and Timer
/// 0's reload, starting it as the last line is drawn.
void SampleFreqSet(u32 mode) {
  struct SoundInfo *work = SOUND_INFO_PTR;
  u32 pcmSamplesPerVBlank;
  volatile u16 *timer;

  mode = (mode & SOUND_MODE_FREQ) >> SOUND_MODE_FREQ_SHIFT;
  work->rate = mode;
  pcmSamplesPerVBlank = gPcmSamplesPerVBlankTable[mode - 1];
  work->pcmSamplesPerVBlank = pcmSamplesPerVBlank;
  // VBlanks the DMA plays before the buffer wraps, and the sample rate in hertz
  // (rounded), and half its reciprocal in 24-bit fixed point
  work->transferPeriod = __divsi3(SOUND_PCM_BUFFER_SIZE, pcmSamplesPerVBlank);
  work->outputRate = __divsi3(SOUND_VBLANK_RATE_E4 * pcmSamplesPerVBlank +
                                  SOUND_RATE_E4_UNIT / 2,
                              SOUND_RATE_E4_UNIT);
  work->rateStep = (__divsi3(SOUND_RATE_STEP_ONE, work->outputRate) + 1) >> 1;
  // Timer 0 counts the CPU cycles of one sample: it overflows, and the FIFOs
  // take their next sample, pcmSamplesPerVBlank times a VBlank.
  REG_TM0CNT_H = 0;
  timer = &REG_TM0CNT_L;
  *timer = -__divsi3(SOUND_CYCLES_PER_VBLANK, pcmSamplesPerVBlank);
  m4aSoundVSyncOn();
  // Wait out the last line drawn if it has begun, then for it to begin anew.
  while (*(volatile u8 *)&REG_VCOUNT == DISPLAY_HEIGHT - 1)
    ;
  while (*(volatile u8 *)&REG_VCOUNT != DISPLAY_HEIGHT - 1)
    ;
  REG_TM0CNT_H = TIMER_ENABLE;
}

/// Applies the reverb, channel-count, volume, bias and sample-rate fields of
/// `mode` to the sound work, with its ident raised.
void m4aSoundMode(u32 mode) {
  struct SoundInfo *work = SOUND_INFO_PTR;
  u32 modeField;
  struct SoundChannel *note;

  if (work->ident != ID_NUMBER)
    return;
  work->ident++;

  // A zero field leaves the corresponding setting unchanged.
  modeField = mode & (SOUND_MODE_REVERB_SET | SOUND_MODE_REVERB_VAL);
  if (modeField != 0)
    work->reverb = modeField & SOUND_MODE_REVERB_VAL;
  modeField = mode & SOUND_MODE_MAXCHN;
  if (modeField != 0) {
    work->pcmNoteCount = modeField >> SOUND_MODE_MAXCHN_SHIFT;
    modeField = MAX_DIRECTSOUND_CHANNELS;
    note = work->pcmNotes;
    do {
      note->state = 0;
      modeField--;
      note++;
    } while (modeField != 0);
  }
  modeField = mode & SOUND_MODE_MASVOL;
  if (modeField != 0)
    work->volume = modeField >> SOUND_MODE_MASVOL_SHIFT;
  modeField = mode & SOUND_MODE_DA_BIT;
  if (modeField != 0) {
    modeField = (modeField & SOUND_MODE_DA_BIT_RESOLUTION) >>
                (SOUND_MODE_DA_BIT_SHIFT - SOUNDBIAS_H_RESOLUTION_SHIFT);
    REG_SOUNDBIAS_H = (REG_SOUNDBIAS_H & SOUNDBIAS_H_LEVEL) | modeField;
  }
  modeField = mode & SOUND_MODE_FREQ;
  if (modeField != 0) {
    m4aSoundVSyncOff();
    SampleFreqSet(modeField);
  }
  work->ident = ID_NUMBER;
}

/// Unused: silences every PCM and CGB channel with the sound work's ident
/// raised.
UNUSED void SoundClear(void) {
  struct SoundInfo *work = SOUND_INFO_PTR;
  s32 channelCounter;
  void *channel;

  if (work->ident != ID_NUMBER)
    return;
  work->ident++;

  // Clear every PCM slot, including slots beyond the active note count.
  channelCounter = MAX_DIRECTSOUND_CHANNELS;
  channel = work->pcmNotes;
  while (channelCounter > 0) {
    ((struct SoundChannel *)channel)->state = 0;
    channelCounter--;
    channel = (void *)((s32)channel + sizeof(struct SoundChannel));
  }
  // Reuse the counter as the hardware channel number for the CGB path.
  channel = work->cgbNotes;
  if (channel != NULL) {
    channelCounter = 1;
    while (channelCounter <= SOUND_CGB_CHANNEL_COUNT) {
      work->cgbMute(channelCounter);
      ((struct CgbChannel *)channel)->state = 0;
      channelCounter++;
      channel = (void *)((s32)channel + sizeof(struct CgbChannel));
    }
  }
  work->ident = ID_NUMBER;
}

/// Stops both FIFO transfers and clears their sample buffers, raising the
/// work's ident by SOUND_IDENT_VSYNC_OFF until m4aSoundVSyncOn.
void m4aSoundVSyncOff(void) {
  struct SoundInfo *work = SOUND_INFO_PTR;
  u32 zero;

  // Only a free driver, or one raised once (m4aSoundMode's), may be
  // suspended; a suspended or uninitialized driver is left as it is.
  if (work->ident - ID_NUMBER > 1)
    return;
  work->ident += SOUND_IDENT_VSYNC_OFF;
  if (REG_DMA1CNT & SOUND_DMA_REPEATING)
    REG_DMA1CNT = SOUND_DMA_LAST_TRANSFER;
  if (REG_DMA2CNT & SOUND_DMA_REPEATING)
    REG_DMA2CNT = SOUND_DMA_LAST_TRANSFER;
  REG_DMA1CNT_H = DMA_32BIT;
  REG_DMA2CNT_H = DMA_32BIT;
  zero = 0;
  Bios_CpuSet(&zero, work->buffers,
              CPU_SET_SRC_FIXED | CPU_SET_32BIT | sizeof(work->buffers) / 4);
}

/// Restarts both FIFO transfers and takes SOUND_IDENT_VSYNC_OFF back off the
/// work's ident.
void m4aSoundVSyncOn(void) {
  struct SoundInfo *work = SOUND_INFO_PTR;
  u32 ident = work->ident;

  if (ident == ID_NUMBER)
    return;
  REG_DMA1CNT_H = DMA_ENABLE | DMA_START_SPECIAL | DMA_REPEAT | DMA_32BIT;
  REG_DMA2CNT_H = DMA_ENABLE | DMA_START_SPECIAL | DMA_REPEAT | DMA_32BIT;
  work->transferCountdown = 0;
  work->ident = ident - SOUND_IDENT_VSYNC_OFF;
}

/// Deactivates up to sixteen tracks and links the music player into the
/// MPlayMain chain.
void MPlayOpen(struct MusicPlayerInfo *player, struct MusicPlayerTrack *tracks,
               u8 trackCount) {
  struct SoundInfo *work;

  if (trackCount == 0)
    return;
  if (trackCount > MAX_MUSICPLAYER_TRACKS)
    trackCount = MAX_MUSICPLAYER_TRACKS;
  work = SOUND_INFO_PTR;
  if (work->ident != ID_NUMBER)
    return;
  work->ident++;

  Clear64byte(player);
  player->tracks = tracks;
  player->trackCount = trackCount;
  player->status = MUSICPLAYER_STATUS_PAUSE;
  while (trackCount != 0) {
    tracks->flags = 0;
    trackCount--;
    tracks++;
  }
  // Prepend this music player to the existing MPlayMain chain.
  if (work->MPlayMainHead != NULL) {
    player->MPlayMainNext = work->MPlayMainHead;
    player->musicPlayerNext = work->musicPlayerHead;
    work->MPlayMainHead = NULL;
  }
  work->musicPlayerHead = player;
  work->MPlayMainHead = MPlayMain;
  work->ident = ID_NUMBER;
  player->ident = ID_NUMBER;
}

/// Starts a song when its priority permits, resetting its tracks and tempo.
void MPlayStart(struct MusicPlayerInfo *player,
                const struct SongHeader *header) {
  s32 trackIndex;
  struct MusicPlayerTrack *track;

  if (player->ident != ID_NUMBER)
    return;

  // Preserve a higher-priority song unless the music player is idle.
  // checkPriority is 0 for every music player (sound/music_player_table.s),
  // so in this game a new song always replaces the one playing.
  if (player->checkPriority != 0 &&
      ((player->header != NULL && (player->tracks[0].flags & MPT_FLG_START)) ||
       ((player->status & MUSICPLAYER_STATUS_TRACK) != 0 &&
        !(player->status & MUSICPLAYER_STATUS_PAUSE))) &&
      player->priority > header->priority)
    return;

  player->ident++;
  // Reset song timing and fade state before assigning track streams: a tick a
  // VBlank until the song's tempo command.
  player->status = 0;
  player->header = header;
  player->voices = header->voices;
  player->priority = header->priority;
  player->clock = 0;
  player->tempo = SOUND_TEMPO_PER_TICK;
  player->tempoStep = SOUND_TEMPO_PER_TICK;
  player->tempoScale = SOUND_TEMPO_SCALE_NORMAL;
  player->tempoCount = 0;
  player->fadePeriod = 0;

  trackIndex = 0;
  track = player->tracks;
  while (trackIndex < header->trackCount && trackIndex < player->trackCount) {
    TrackStop(player, track);
    track->flags = MPT_FLG_EXIST | MPT_FLG_START;
    track->notes = NULL;
    track->cursor = header->tracks[trackIndex];
    trackIndex++;
    track++;
  }
  // Stop leftover tracks that the new song does not use.
  while (trackIndex < player->trackCount) {
    TrackStop(player, track);
    track->flags = 0;
    trackIndex++;
    track++;
  }
  if (header->mode & SOUND_MODE_REVERB_SET)
    m4aSoundMode(header->mode);
  player->ident = ID_NUMBER;
}

/// Pauses this music player and stops all of its tracks, with its ident
/// raised. Called by m4aSongNumStop and m4aMPlayAllStop.
void m4aMPlayStop(struct MusicPlayerInfo *player) {
  s32 tracksRemaining;
  struct MusicPlayerTrack *track;

  if (player->ident != ID_NUMBER)
    return;
  player->ident++;
  player->status |= MUSICPLAYER_STATUS_PAUSE;
  tracksRemaining = player->trackCount;
  track = player->tracks;
  while (tracksRemaining > 0) {
    TrackStop(player, track);
    tracksRemaining--;
    track++;
  }
  player->ident = ID_NUMBER;
}

/// Advances a due fade step, updating active track volumes, or at the end of a
/// fade out stops (or pauses) the music player.
void FadeOutBody(struct MusicPlayerInfo *player) {
  s32 tracksRemaining;
  struct MusicPlayerTrack *track;

  if (player->fadePeriod == 0)
    return;
  if (--player->fadeCounter != 0)
    return;
  player->fadeCounter = player->fadePeriod;

  // Advance the volume while retaining its low-bit fade flags.
  if (player->fadeVolume & FADE_IN) {
    if ((player->fadeVolume += SOUND_FADE_STEP) >= SOUND_FADE_FULL_VOLUME) {
      player->fadeVolume = SOUND_FADE_FULL_VOLUME;
      player->fadePeriod = 0;
    }
  } else {
    if ((s16)(player->fadeVolume -= SOUND_FADE_STEP) <= 0) {
      tracksRemaining = player->trackCount;
      track = player->tracks;
      while (tracksRemaining > 0) {
        TrackStop(player, track);
        if (!(player->fadeVolume & TEMPORARY_FADE))
          track->flags = 0;
        tracksRemaining--;
        track++;
      }
      if (player->fadeVolume & TEMPORARY_FADE)
        player->status |= MUSICPLAYER_STATUS_PAUSE;
      else
        player->status = MUSICPLAYER_STATUS_PAUSE;
      player->fadePeriod = 0;
      return;
    }
  }

  // Propagate the new level to each active track.
  tracksRemaining = player->trackCount;
  track = player->tracks;
  while (tracksRemaining > 0) {
    if (track->flags & MPT_FLG_EXIST) {
      track->volumeScale = player->fadeVolume >> 2;
      track->flags |= MPT_FLG_VOLCHG;
    }
    tracksRemaining--;
    track++;
  }
}

/// A track's stereo pan, full left to full right.
enum { SOUND_PAN_LEFT = -128, SOUND_PAN_RIGHT = 127 };
/// 1.0 in the factor (SOUND_MODULATION_ONE + modAmount) / SOUND_MODULATION_ONE
/// by which volume modulation scales a track's volume.
enum { SOUND_MODULATION_ONE = 128 };
/// A semitone in a track's pitch, kept in 1/256 semitone, and in its tune and
/// bend, in 1/64, and its pitch modulation, in 1/16.
enum {
  SOUND_PITCH_SEMITONE = 256,
  SOUND_TUNE_SEMITONE = 64,
  SOUND_MODULATION_SEMITONE = 16
};

/// Works out a track's volume on each speaker and its pitch again, each only
/// when its flags ask, from the song's settings, the player's controls and the
/// vibrato.
void TrkVolPitSet(struct MusicPlayerInfo *player,
                  struct MusicPlayerTrack *track) {
  u32 volume;
  s32 pan;
  s32 pitch;
  s32 bend;

  // Mix the volume modulation and clamped stereo pan: the song's volume
  // times volumeScale (0 to 64) / 32, then shared between the speakers by
  // pan.
  if (track->flags & MPT_FLG_VOLSET) {
    volume = (u32)(track->volume * track->volumeScale) >> 5;
    if (track->modTarget == SOUND_MODULATION_VOLUME)
      volume = ((track->modAmount + SOUND_MODULATION_ONE) * volume) >> 7;
    pan = track->pan * 2 + track->panOffset;
    if (track->modTarget == SOUND_MODULATION_PAN)
      pan += track->modAmount;
    if (pan < SOUND_PAN_LEFT)
      pan = SOUND_PAN_LEFT;
    else if (pan > SOUND_PAN_RIGHT)
      pan = SOUND_PAN_RIGHT;
    track->outVolumeA = ((pan - SOUND_PAN_LEFT) * volume) >> 8;
    track->outVolumeB = ((SOUND_PAN_RIGHT - pan) * volume) >> 8;
  }
  // The key offsets are in semitones, fine already in 1/256.
  if (track->flags & MPT_FLG_PITSET) {
    bend = track->bend * track->bendRange;
    pitch =
        (track->tune + bend) * (SOUND_PITCH_SEMITONE / SOUND_TUNE_SEMITONE) +
        (track->keyOffsetA * SOUND_PITCH_SEMITONE) +
        (track->keyOffsetB * SOUND_PITCH_SEMITONE) + track->fine;
    if (track->modTarget == SOUND_MODULATION_PITCH)
      pitch +=
          track->modAmount * (SOUND_PITCH_SEMITONE / SOUND_MODULATION_SEMITONE);
    track->pitchHi = pitch >> 8;
    track->pitchLo = pitch;
  }
  track->flags &= ~(MPT_FLG_VOLSET | MPT_FLG_PITSET);
}

/// The keys the CGB channels play: the noise channel's from its lowest, one
/// gNoiseTable entry each, and the tone channels' from theirs, one
/// gCgbScaleTable entry each. A key below a channel's lowest plays it.
enum {
  CGB_NOISE_LOWEST_KEY = 21,
  CGB_NOISE_KEY_COUNT = 60,
  CGB_TONE_LOWEST_KEY = 36,
  CGB_TONE_KEY_COUNT = 132
};
/// The tone channels' frequency values run below it: a value is it less the
/// tone's period.
#define CGB_FREQUENCY_RANGE 2048
/// A gCgbScaleTable code: its semitone's period in gCgbFreqTable in the low
/// four bits, and above them the octaves to halve it.
#define CGB_SCALE_SEMITONE 0x0F
#define CGB_SCALE_OCTAVE_SHIFT 4

/// Converts a key and fractional pitch to a CGB tone or noise-register value.
/// Installed as the work's cgbFrequency callback by MPlayExtender.
s32 MidiKeyToCgbFreq(u8 kind, u8 key, u8 fine) {
  s32 lowerFrequency;
  s32 upperFrequency;

  if (kind == CGB_CHANNEL_NOISE) {
    if (key < CGB_NOISE_LOWEST_KEY)
      key = 0;
    else {
      key -= CGB_NOISE_LOWEST_KEY;
      if (key > CGB_NOISE_KEY_COUNT - 1)
        key = CGB_NOISE_KEY_COUNT - 1;
    }
    return gNoiseTable[key];
  }

  // The highest key plays as the one below it at the finest pitch under the
  // next, so that a key above it is always in the table.
  if (key < CGB_TONE_LOWEST_KEY) {
    fine = 0;
    key = 0;
  } else {
    key -= CGB_TONE_LOWEST_KEY;
    if (key > CGB_TONE_KEY_COUNT - 2) {
      key = CGB_TONE_KEY_COUNT - 2;
      fine = SOUND_PITCH_SEMITONE - 1;
    }
  }
  lowerFrequency = gCgbScaleTable[key];
  lowerFrequency = gCgbFreqTable[lowerFrequency & CGB_SCALE_SEMITONE] >>
                   (lowerFrequency >> CGB_SCALE_OCTAVE_SHIFT);
  upperFrequency = gCgbScaleTable[key + 1];
  upperFrequency = gCgbFreqTable[upperFrequency & CGB_SCALE_SEMITONE] >>
                   (upperFrequency >> CGB_SCALE_OCTAVE_SHIFT);
  return lowerFrequency + ((fine * (upperFrequency - lowerFrequency)) >> 8) +
         CGB_FREQUENCY_RANGE;
}

/// Silences one CGB channel: restarted at volume 0, or for CGB_CHANNEL_WAVE,
/// stopped.
void CgbOscOff(u8 channel) {
  switch (channel) {
  case CGB_CHANNEL_SWEEP_SQUARE:
    REG_NR12 = NRX2_ENVELOPE_UP;
    REG_NR14 = NRX4_RESTART;
    break;
  case CGB_CHANNEL_SQUARE:
    REG_NR22 = NRX2_ENVELOPE_UP;
    REG_NR24 = NRX4_RESTART;
    break;
  case CGB_CHANNEL_WAVE:
    REG_NR30 = 0;
    break;
  default:
    REG_NR42 = NRX2_ENVELOPE_UP;
    REG_NR44 = NRX4_RESTART;
    break;
  }
}

/// Routes a CGB note at least twice as loud on one side to that side only,
/// returning whether it did.
static __inline__ s32 CgbPan(struct CgbChannel *note) {
  u32 right = note->outRight;
  u32 left = note->outLeft;

  if ((u8)right >= (u8)left) {
    if ((u8)right / 2 >= (u8)left) {
      note->panBits = NR51_RIGHT_ALL;
      return 1;
    }
  } else if ((u8)left / 2 >= (u8)right) {
    note->panBits = NR51_LEFT_ALL;
    return 1;
  }
  return 0;
}

/// The loudest level a CGB envelope reaches.
enum { CGB_LEVEL_MAX = 15 };
/// 1.0 in a note's echo level, the share of its peak held after release.
enum { SOUND_ECHO_LEVEL_ONE = 256 };

/// Chooses stereo routing and derives peak and sustain levels for a CGB note,
/// writing them into `note`: peak is the two levels' sum / 16, at most
/// CGB_LEVEL_MAX on one side; sustainLevel is peak * sustain / 16, rounded up.
void CgbModVol(struct CgbChannel *note) {
  if (!CgbPan(note)) {
    note->panBits = NR51_RIGHT_ALL | NR51_LEFT_ALL;
    note->peak = (u32)(note->outRight + note->outLeft) >> 4;
  } else {
    note->peak = (u32)(note->outRight + note->outLeft) >> 4;
    if (note->peak > CGB_LEVEL_MAX)
      note->peak = CGB_LEVEL_MAX;
  }
  note->sustainLevel = (note->peak * note->sustain + CGB_LEVEL_MAX) >> 4;
  note->panBits &= note->channelBits;
}

/// The VBlanks in which the CGB envelopes take one step more than VBlanks, 16
/// in 15: 63.7 steps a second, close to the 64 a second the hardware steps its
/// own envelopes, where one step a VBlank would give 59.7.
enum { CGB_EXTRA_STEP_PERIOD = 15 };

/// The envelope steps a sustained CGB note waits each time before its level is
/// set to its sustain level again.
enum { CGB_SUSTAIN_STEPS = 7 };

/// Steps CGB note envelopes and writes changed pitch, volume and trigger
/// registers, once a VBlank.
void CgbSound(void) {
  s32 channel;
  s32 prevC15;
  struct SoundInfo *work = SOUND_INFO_PTR;
  struct CgbChannel *note;
  volatile u8 *sweepOrWaveControl;
  volatile u8 *lengthDutyRegister;
  volatile u8 *volumeEnvelopeRegister;
  volatile u8 *frequencyOrNoiseRegister;
  volatile u8 *triggerRegister;
  s32 envelopeControl;
  /* FAKEMATCH(temporary): byteMask, 0xff, cuts each byte field it is anded
   * with to the byte it already is. */
  u32 byteMask = 0xff;

  if (work->c15)
    work->c15--;
  else
    work->c15 = CGB_EXTRA_STEP_PERIOD - 1;

  for (channel = CGB_CHANNEL_SWEEP_SQUARE, note = work->cgbNotes;
       channel <= SOUND_CGB_CHANNEL_COUNT; channel++, note++) {
    if (!(note->state & SOUND_CHANNEL_SF_ON))
      continue;

    // Select this channel's sweep, length, envelope and frequency registers.
    switch (channel) {
    case CGB_CHANNEL_SWEEP_SQUARE:
      sweepOrWaveControl = &REG_NR10;
      lengthDutyRegister = &REG_NR11;
      volumeEnvelopeRegister = &REG_NR12;
      frequencyOrNoiseRegister = &REG_NR13;
      triggerRegister = &REG_NR14;
      break;
    case CGB_CHANNEL_SQUARE:
      sweepOrWaveControl = &REG_NR10 + 1;
      lengthDutyRegister = &REG_NR21;
      volumeEnvelopeRegister = &REG_NR22;
      frequencyOrNoiseRegister = &REG_NR23;
      triggerRegister = &REG_NR24;
      break;
    case CGB_CHANNEL_WAVE:
      sweepOrWaveControl = &REG_NR30;
      lengthDutyRegister = &REG_NR31;
      volumeEnvelopeRegister = &REG_NR32;
      frequencyOrNoiseRegister = &REG_NR33;
      triggerRegister = &REG_NR34;
      break;
    default:
      sweepOrWaveControl = &REG_NR30 + 1;
      lengthDutyRegister = &REG_NR41;
      volumeEnvelopeRegister = &REG_NR42;
      frequencyOrNoiseRegister = &REG_NR43;
      triggerRegister = &REG_NR44;
      break;
    }

    prevC15 = work->c15;
    envelopeControl = *volumeEnvelopeRegister;

    // A new note initializes its waveform and enters the attack phase.
    if (note->state & SOUND_CHANNEL_SF_START) {
      if (!(note->state & SOUND_CHANNEL_SF_STOP)) {
        note->state = SOUND_CHANNEL_SF_ENV_ATTACK;
        note->modified = CGB_CHANNEL_MO_VOL | CGB_CHANNEL_MO_PIT;
        CgbModVol(note);
        switch (channel) {
        case CGB_CHANNEL_SWEEP_SQUARE:
          *sweepOrWaveControl = note->sweep;
          // Fall through.
        case CGB_CHANNEL_SQUARE:
          // NRx1: the duty, which a square voice keeps in wave, in bits 7..6
          // over the length.
          *lengthDutyRegister = ((u32)note->wave << 6) + note->length;
          goto init_envelope;
        case CGB_CHANNEL_WAVE:
          // A new wave is written into the first bank while the second is
          // selected, then the first selected.
          if (note->wave != note->loadedWave) {
            *sweepOrWaveControl = NR30_SECOND_BANK;
            REG_WAVE_RAM[0] = note->wave[0];
            REG_WAVE_RAM[1] = note->wave[1];
            REG_WAVE_RAM[2] = note->wave[2];
            REG_WAVE_RAM[3] = note->wave[3];
            note->loadedWave = note->wave;
          }
          *sweepOrWaveControl = 0;
          *lengthDutyRegister = note->length;
          if (note->length)
            note->trigger = NRX4_RESTART | NRX4_LENGTH_STOP;
          else
            note->trigger = NRX4_RESTART;
          break;
        default:
          *lengthDutyRegister = note->length;
          *frequencyOrNoiseRegister = (u32)note->wave << 3;
        init_envelope:
          // The hardware envelope counts the volume up, at the attack's pace.
          envelopeControl = note->attack + NRX2_ENVELOPE_UP;
          if (note->length)
            note->trigger = NRX4_LENGTH_STOP;
          else
            note->trigger = 0;
          break;
        }
        note->counter = note->attack;
        if ((s8)(note->counter & byteMask)) {
          note->level = 0;
          goto count_down;
        }
        goto decay;
      }
      goto off;
    } else if (note->state & SOUND_CHANNEL_SF_IEC) {
      // A released note's echo holds until its length runs out.
      note->echoLength--;
      if ((s8)(note->echoLength & byteMask) > 0)
        goto write;
    off:
      CgbOscOff(channel);
      note->state = 0;
      goto done;
    } else if ((note->state & SOUND_CHANNEL_SF_STOP) &&
               (note->state & SOUND_CHANNEL_SF_ENV)) {
      // A note let go enters its release, then its echo.
      note->state &= ~SOUND_CHANNEL_SF_ENV;
      note->counter = note->release;
      if ((s8)(note->counter & byteMask)) {
        note->modified |= CGB_CHANNEL_MO_VOL;
        if (channel != CGB_CHANNEL_WAVE)
          envelopeControl = note->release;
        goto count_down;
      }
      goto echo;
    } else {
    // Attack, decay, sustain and release share this countdown path.
    step:
      if (note->counter == 0) {
        if (channel == CGB_CHANNEL_WAVE)
          note->modified |= CGB_CHANNEL_MO_VOL;
        CgbModVol(note);
        if ((note->state & SOUND_CHANNEL_SF_ENV) ==
            SOUND_CHANNEL_SF_ENV_RELEASE) {
          note->level--;
          if ((s8)(note->level & byteMask) <= 0) {
          echo:
            note->level =
                ((note->peak * note->echoLevel) + SOUND_ECHO_LEVEL_ONE - 1) >>
                8;
            if (note->level) {
              note->state |= SOUND_CHANNEL_SF_IEC;
              note->modified |= CGB_CHANNEL_MO_VOL;
              if (channel != CGB_CHANNEL_WAVE)
                envelopeControl = NRX2_ENVELOPE_UP;
              goto write;
            }
            goto off;
          }
          note->counter = note->release;
        } else if ((note->state & SOUND_CHANNEL_SF_ENV) ==
                   SOUND_CHANNEL_SF_ENV_SUSTAIN) {
        sustain:
          note->level = note->sustainLevel;
          note->counter = CGB_SUSTAIN_STEPS;
        } else if ((note->state & SOUND_CHANNEL_SF_ENV) ==
                   SOUND_CHANNEL_SF_ENV_DECAY) {
          note->level--;
          if ((s8)(note->level & byteMask) <= (s8)note->sustainLevel) {
          sustain_or_release:
            if (note->sustain == 0) {
              note->state &= ~SOUND_CHANNEL_SF_ENV;
              goto echo;
            }
            note->state--;
            note->modified |= CGB_CHANNEL_MO_VOL;
            if (channel != CGB_CHANNEL_WAVE)
              envelopeControl = NRX2_ENVELOPE_UP;
            goto sustain;
          }
          note->counter = note->decay;
        } else {
          note->level++;
          if ((u8)(note->level & byteMask) >= note->peak) {
          decay:
            note->state--;
            note->counter = note->decay;
            if ((s8)(note->counter & byteMask) == 0)
              goto sustain_or_release;
            note->modified |= CGB_CHANNEL_MO_VOL;
            note->level = note->peak;
            if (channel != CGB_CHANNEL_WAVE)
              envelopeControl = note->decay;
          } else {
            note->counter = note->attack;
          }
        }
      }
    count_down:
      note->counter--;
      if (prevC15 == 0) {
        prevC15--;
        goto step;
      }
    }

  // Only changed pitch and volume settings reach the hardware.
  write:
    if (note->modified & CGB_CHANNEL_MO_PIT) {
      if (channel < CGB_CHANNEL_NOISE && (note->kind & TONEDATA_TYPE_FIX)) {
        s32 bias = REG_SOUNDBIAS_H;

        // At the sound bias's two finest resolutions the frequency is rounded
        // to a multiple of 4 or 2.
        if (bias < SOUNDBIAS_H_RESOLUTION_8BIT)
          note->frequency = (note->frequency + 2) & (CGB_FREQUENCY_RANGE - 4);
        else if (bias < SOUNDBIAS_H_RESOLUTION_7BIT)
          note->frequency = (note->frequency + 1) & (CGB_FREQUENCY_RANGE - 2);
      }
      if (channel != CGB_CHANNEL_NOISE)
        *frequencyOrNoiseRegister = note->frequency;
      else
        *frequencyOrNoiseRegister =
            (*frequencyOrNoiseRegister & NR43_WIDTH_7BIT) | note->frequency;
      // The frequency's top three bits go in the trigger register's low bits.
      note->trigger = (note->trigger & (NRX4_RESTART | NRX4_LENGTH_STOP)) +
                      *((u8 *)&note->frequency + 1);
      *triggerRegister = (s8)(note->trigger & byteMask);
    }

    if (note->modified & CGB_CHANNEL_MO_VOL) {
      // NR51 routes each channel to the left and right speakers.
      REG_NR51 = (REG_NR51 & ~note->channelBits) | note->panBits;
      if (channel == CGB_CHANNEL_WAVE) {
        // CGB_CHANNEL_WAVE has no envelope: its level maps to an NR32 volume
        // code.
        *volumeEnvelopeRegister = gCgb3Vol[note->level];
        if (note->trigger & NRX4_RESTART) {
          *sweepOrWaveControl = NR30_PLAY;
          *triggerRegister = note->trigger;
          note->trigger &= ~NRX4_RESTART;
        }
      } else {
        // NRx2: the starting volume in the high nibble, the envelope below.
        envelopeControl &= NRX2_ENVELOPE;
        *volumeEnvelopeRegister = (note->level << 4) + envelopeControl;
        *triggerRegister = note->trigger | NRX4_RESTART;
        // CGB_CHANNEL_SWEEP_SQUARE is restarted a second time unless its sweep
        // falls (8 alone is no sweep), as in pret's m4a.c.
        if (channel == CGB_CHANNEL_SWEEP_SQUARE &&
            !(*sweepOrWaveControl & NR10_SWEEP_DOWN))
          *triggerRegister = note->trigger | NRX4_RESTART;
      }
    }

  done:
    note->modified = 0;
  }
}

/// Sets the tempo multiplier (SOUND_TEMPO_SCALE_NORMAL the song's own tempo)
/// and updates the number of ticks advanced per VBlank.
void m4aMPlayTempoControl(struct MusicPlayerInfo *player, u16 scale) {
  if (player->ident == ID_NUMBER) {
    player->ident++;
    player->tempoScale = scale;
    player->tempoStep = (player->tempo * player->tempoScale) >> 8;
    player->ident = ID_NUMBER;
  }
}

/// Sets the volume scale of active tracks selected by the bit mask and marks
/// their output for recalculation. `volume` runs 0 to 256 (full) and is kept as
/// volumeScale, a quarter of it.
void m4aMPlayVolumeControl(struct MusicPlayerInfo *player, u16 mask,
                           u16 volume) {
  s32 tracksRemaining;
  struct MusicPlayerTrack *track;
  u32 trackBit;

  if (player->ident != ID_NUMBER)
    return;
  player->ident++;
  tracksRemaining = player->trackCount;
  track = player->tracks;
  trackBit = 1;
  while (tracksRemaining > 0) {
    if (mask & trackBit) {
      if (track->flags & MPT_FLG_EXIST) {
        track->volumeScale = volume >> 2;
        track->flags |= MPT_FLG_VOLCHG;
      }
    }
    tracksRemaining--;
    track++;
    trackBit <<= 1;
  }
  player->ident = ID_NUMBER;
}

/// Sets the semitone and fractional pitch offsets of selected active tracks and
/// marks their pitch for recalculation. `pitch` is in 1/256 semitone: its high
/// byte becomes keyOffsetB and its low byte fine.
void m4aMPlayPitchControl(struct MusicPlayerInfo *player, u16 mask, s16 pitch) {
  s32 tracksRemaining;
  struct MusicPlayerTrack *track;
  u32 trackBit;

  if (player->ident != ID_NUMBER)
    return;
  player->ident++;
  tracksRemaining = player->trackCount;
  track = player->tracks;
  trackBit = 1;
  while (tracksRemaining > 0) {
    if (mask & trackBit) {
      if (track->flags & MPT_FLG_EXIST) {
        track->keyOffsetB = pitch >> 8;
        track->fine = pitch;
        track->flags |= MPT_FLG_PITCHG;
      }
    }
    tracksRemaining--;
    track++;
    trackBit <<= 1;
  }
  player->ident = ID_NUMBER;
}

/// Unused: sets the stereo pan offset of selected active tracks.
UNUSED void m4aMPlayPanpotControl(struct MusicPlayerInfo *player, u16 mask,
                                  s8 pan) {
  s32 tracksRemaining;
  struct MusicPlayerTrack *track;
  u32 trackBit;

  if (player->ident != ID_NUMBER)
    return;
  player->ident++;
  tracksRemaining = player->trackCount;
  track = player->tracks;
  trackBit = 1;
  while (tracksRemaining > 0) {
    if (mask & trackBit) {
      if (track->flags & MPT_FLG_EXIST) {
        track->panOffset = pan;
        track->flags |= MPT_FLG_VOLCHG;
      }
    }
    tracksRemaining--;
    track++;
    trackBit <<= 1;
  }
  player->ident = ID_NUMBER;
}

/// Resets the LFO phase and amount, marking the affected output for
/// recalculation. Called by m4aMPlayModDepthSet and
/// m4aMPlayLFOSpeedSet when they set 0.
void ClearModM(struct MusicPlayerTrack *track) {
  track->lfoPhase = 0;
  track->modAmount = 0;
  if (track->modTarget == SOUND_MODULATION_PITCH)
    track->flags |= MPT_FLG_PITCHG;
  else
    track->flags |= MPT_FLG_VOLCHG;
}

/// Sets modulation depth on selected active tracks, resetting zero-depth
/// modulation.
void m4aMPlayModDepthSet(struct MusicPlayerInfo *player, s32 selectedTracks,
                         s32 depthArgument) {
  u16 mask = selectedTracks;
  u8 depth = depthArgument;
  s32 tracksRemaining;
  struct MusicPlayerTrack *track;
  u32 trackBit;

  if (player->ident != ID_NUMBER)
    return;

  player->ident++;
  tracksRemaining = player->trackCount;
  track = player->tracks;
  trackBit = 1;
  while (tracksRemaining > 0) {
    if (mask & trackBit) {
      if (track->flags & MPT_FLG_EXIST) {
        track->modDepth = depth;
        if (!track->modDepth)
          ClearModM(track);
      }
    }
    tracksRemaining--;
    track++;
    trackBit <<= 1;
  }
  player->ident = ID_NUMBER;
}

/// Sets LFO speed on selected active tracks, resetting modulation when stopped.
void m4aMPlayLFOSpeedSet(struct MusicPlayerInfo *player, u32 selectedTracks,
                         u32 speedArgument) {
  u16 mask = selectedTracks;
  u8 speed = speedArgument;
  s32 tracksRemaining;
  struct MusicPlayerTrack *track;
  u32 trackBit;

  if (player->ident != ID_NUMBER)
    return;

  player->ident++;
  tracksRemaining = player->trackCount;
  track = player->tracks;
  trackBit = 1;
  while (tracksRemaining > 0) {
    if (mask & trackBit) {
      if (track->flags & MPT_FLG_EXIST) {
        track->lfoSpeed = speed;
        if (!track->lfoSpeed)
          ClearModM(track);
      }
    }
    tracksRemaining--;
    track++;
    trackBit <<= 1;
  }
  player->ident = ID_NUMBER;
}

/// Executes a memacc operation and consumes a branch address for comparisons.
void ply_memacc(struct MusicPlayerInfo *player,
                struct MusicPlayerTrack *track) {
  u32 operation;
  u8 *workByte;
  u8 operand;

  operation = *track->cursor;
  track->cursor++;
  workByte = player->workBytes + *track->cursor;
  track->cursor++;
  operand = *track->cursor;
  track->cursor++;

  switch (operation) {
  case MEMACC_SET:
    *workByte = operand;
    return;
  case MEMACC_ADD:
    *workByte += operand;
    return;
  case MEMACC_SUB:
    *workByte -= operand;
    return;
  case MEMACC_MEM_SET:
    *workByte = player->workBytes[operand];
    return;
  case MEMACC_MEM_ADD:
    *workByte += player->workBytes[operand];
    return;
  case MEMACC_MEM_SUB:
    *workByte -= player->workBytes[operand];
    return;
  case MEMACC_BEQ:
    if (*workByte == operand)
      goto jump;
    goto skip;
  case MEMACC_BNE:
    if (*workByte != operand)
      goto jump;
    goto skip;
  case MEMACC_BHI:
    if (*workByte > operand)
      goto jump;
    goto skip;
  case MEMACC_BHS:
    if (*workByte >= operand)
      goto jump;
    goto skip;
  case MEMACC_BLS:
    if (*workByte <= operand)
      goto jump;
    goto skip;
  case MEMACC_BLO:
    if (*workByte < operand)
      goto jump;
    goto skip;
  case MEMACC_MEM_BEQ:
    if (*workByte == player->workBytes[operand])
      goto jump;
    goto skip;
  case MEMACC_MEM_BNE:
    if (*workByte != player->workBytes[operand])
      goto jump;
    goto skip;
  case MEMACC_MEM_BHI:
    if (*workByte > player->workBytes[operand])
      goto jump;
    goto skip;
  case MEMACC_MEM_BHS:
    if (*workByte >= player->workBytes[operand])
      goto jump;
    goto skip;
  case MEMACC_MEM_BLS:
    if (*workByte <= player->workBytes[operand])
      goto jump;
    goto skip;
  case MEMACC_MEM_BLO:
    if (*workByte < player->workBytes[operand])
      goto jump;
    goto skip;
  default:
    return;
  }

jump:
  /* FAKEMATCH(dead): the entry read through its address. */
  (*(&gMPlayJumpTable[MPLAY_JUMP_GOTO]))(player, track);
  return;
skip:
  track->cursor += 4;
}

/// Reads an extended-command index and invokes its handler.
void ply_xcmd(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track) {
  s32 command;

  command = *track->cursor;
  track->cursor++;
  gXcmdTable[command](player, track);
}

/// A word put together from its four bytes, lowest first.
union WordBytes {
  u32 value;
  struct {
    u32 lowest : 8;
    u32 low : 8;
    u32 high : 8;
    u32 highest : 8;
  } part;
};

/// Finishes a track through the jump table's ply_fine entry. Extended
/// commands 0 and 3: they end the track as the finish command (0xB1) does.
void ply_xxx(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track) {
  MPlayFunc finishSequence = gMPlayJumpTable[MPLAY_JUMP_FINE];

  finishSequence(player, track);
}

/// Reads an unaligned little-endian voice target address from the sequence
/// (extended command 1).
void ply_xwave(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track) {
  const u8 *source = track->cursor;
  union WordBytes targetAddress;

  targetAddress.part.lowest = source[0];
  targetAddress.part.low = source[1];
  targetAddress.part.high = source[2];
  targetAddress.part.highest = source[3];
  track->voice.target = (const void *)targetAddress.value;
  track->cursor = source + 4;
}

/// Reads the voice kind from the sequence (extended command 2).
void ply_xtype(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track) {
  track->voice.kind = *track->cursor;
  track->cursor++;
}

/// Reads the voice attack rate from the sequence (extended command 4).
void ply_xatta(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track) {
  track->voice.attack = *track->cursor;
  track->cursor++;
}

/// Reads the voice decay rate from the sequence (extended command 5).
void ply_xdeca(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track) {
  track->voice.decay = *track->cursor;
  track->cursor++;
}

/// Reads the voice sustain level from the sequence (extended command 6).
void ply_xsust(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track) {
  track->voice.sustain = *track->cursor;
  track->cursor++;
}

/// Reads the voice release rate from the sequence (extended command 7).
void ply_xrele(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track) {
  track->voice.release = *track->cursor;
  track->cursor++;
}

/// Reads each new note's echo level from the sequence (extended command 8,
/// MPlayDef's xIECV).
void ply_xiecv(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track) {
  track->echoLevel = *track->cursor;
  track->cursor++;
}

/// Reads each new note's echo length from the sequence (extended command 9,
/// MPlayDef's xIECL).
void ply_xiecl(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track) {
  track->echoLength = *track->cursor;
  track->cursor++;
}

/// Reads the hardware note length from the sequence (extended command 10).
void ply_xleng(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track) {
  track->voice.length = *track->cursor;
  track->cursor++;
}

/// Reads the voice pan or CGB sweep setting from the sequence (extended
/// command 11).
void ply_xswee(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track) {
  track->voice.panOrSweep = *track->cursor;
  track->cursor++;
}
