#include "audio_engine.h"
#include "types.h"
#include "../../include/system/io_reg.h"

/// Detaches a note through the jump table entry installed by the driver
/// (RealClearChain). Called by MPlayMain and ply_note.
void ClearChain(struct SoundChannel *note) {
  // Taking the entry address folds its slot into a literal address. Direct
  // indexing adds an address-adjustment instruction with the library compiler.
  void (*detachNote)(struct SoundChannel *) =
      MPLAY_FUNC_CAST(void (*)(struct SoundChannel *),
                      *(&gMPlayJumpTable[MPLAY_JUMP_CLEAR_CHAIN]));

  detachNote(note);
}

/// Clears a 64-byte music player or track prefix through the driver's jump
/// table (SoundMainBTM). Called by MPlayOpen and m4aMPlayImmInit.
void Clear64byte(void *block) {
  void (*clearWork)(void *) = MPLAY_FUNC_CAST(
      void (*)(void *), *(&gMPlayJumpTable[MPLAY_JUMP_CLEAR_64BYTE]));

  clearWork(block);
}

s32 __divsi3(s32 numerator, s32 denominator);
void MPlayJumpTableCopy(MPlayFunc *table);
void ply_note(u32 command, struct MusicPlayerInfo *player,
              struct MusicPlayerTrack *track);
void DummyFunc(void);
void SampleFreqSet(u32 mode);
void m4aSoundVSyncOff(void);
void m4aSoundVSyncOn(void);
extern const u16 gPcmSamplesPerVBlankTable[];
extern MPlayFunc gMPlayJumpTable[36];

void MPlayMain(struct MusicPlayerInfo *player);
void TrackStop(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track);
void m4aSoundMode(u32 mode);

struct MusicPlayerInfo;

extern const u8 gCgbScaleTable[];
extern const s16 gCgbFreqTable[];
extern const u8 gNoiseTable[];

void CgbOscOff(u8 channel);

extern const u8 gCgb3Vol[];

/// The PCM buffer's size in samples. It holds transferPeriod whole frames
/// (0x630 / frameSamples, rounded down: 4 of 352 at the game's rate), after
/// which m4aSoundVSync starts the DMAs again from its start.
#define SOUND_PCM_BUFFER_SIZE 0x630
/// The display's frame rate in ten-thousandths of a hertz: 59.7275 frames a
/// second.
#define SOUND_FRAME_RATE_E4 597275
/// CPU cycles in a frame: 228 lines of 1232 cycles.
#define SOUND_CYCLES_PER_FRAME (228 * 1232)

/// Resets the sound hardware, clears the whole sound work block and installs
/// the default PCM driver: stores `work` in SOUND_INFO_PTR, points the FIFO
/// DMAs at its two buffers, fills in the driver callbacks and command table,
/// sets the default rate and finally sets the work's ident to ID_NUMBER.
/// Called by m4aSoundInit.
void SoundInit(struct SoundInfo *work) {
  u32 zero;

  // Stop repeat transfers before configuring the stereo FIFO outputs.
  work->ident = 0;
  if (REG_DMA1CNT & (DMA_REPEAT << 16))
    REG_DMA1CNT = (DMA_ENABLE | DMA_32BIT | DMA_DEST_FIXED) << 16 | 4;
  if (REG_DMA2CNT & (DMA_REPEAT << 16))
    REG_DMA2CNT = (DMA_ENABLE | DMA_32BIT | DMA_DEST_FIXED) << 16 | 4;
  REG_DMA1CNT_H = DMA_32BIT;
  REG_DMA2CNT_H = DMA_32BIT;
  REG_SOUNDCNT_X = SOUNDCNT_X_ENABLE | SOUNDCNT_X_ALL_TONES_ON;
  REG_SOUNDCNT_H = SOUND_B_FIFO_RESET | SOUND_B_TIMER_0 | SOUND_B_LEFT_OUTPUT |
                   SOUND_A_FIFO_RESET | SOUND_A_TIMER_0 | SOUND_A_RIGHT_OUTPUT |
                   SOUND_ALL_MIX_FULL;
  REG_SOUNDBIAS_H = (REG_SOUNDBIAS_H & 0x3F) | 0x40;
  REG_DMA1SAD = (u32)work->buffers[0];
  REG_DMA1DAD = (u32)&REG_FIFO_A;
  REG_DMA2SAD = (u32)work->buffers[1];
  REG_DMA2DAD = (u32)&REG_FIFO_B;
  // Clear the complete work block (0x3ec words, sizeof(struct SoundInfo)),
  // then install the PCM driver entries.
  SOUND_INFO_PTR = work;
  zero = 0;
  Bios_CpuSet(&zero, work, CPU_SET_SRC_FIXED | CPU_SET_32BIT | 0x3EC);
  work->pcmNoteCount = 8;
  work->volume = 15;
  work->noteOn = ply_note;
  work->cgbUpdate = DummyFunc;
  work->cgbMute = (void (*)(u8))DummyFunc;
  work->cgbFrequency = (s32 (*)(u8, u8, u8))DummyFunc;
  work->externalVolumePitch = DummyFunc;
  MPlayJumpTableCopy(gMPlayJumpTable);
  work->commands = gMPlayJumpTable;
  // Frame length 4 (224 samples, 13379 a second) until m4aSoundInit sets
  // the game's mode.
  SampleFreqSet(0x40000);
  work->ident = ID_NUMBER;
}

/// Sets the PCM frame length (`mode` bits 16 to 19 pick a
/// gPcmSamplesPerVBlankTable entry) and the rates derived from it, and Timer
/// 0's reload, starting playback at scanline 159. Called by SoundInit and
/// m4aSoundMode, and by songs through the command table.
void SampleFreqSet(u32 mode) {
  struct SoundInfo *work = SOUND_INFO_PTR;
  u32 frameSamples;
  volatile u16 *timer;

  // Convert the selected frame length into mixer and timer increments.
  mode = (mode & 0xF0000) >> 16;
  work->rate = mode;
  frameSamples = gPcmSamplesPerVBlankTable[mode - 1];
  work->frameSamples = frameSamples;
  // Frames the DMA plays before the buffer wraps, and the sample rate in hertz
  // (rounded), and half its reciprocal in 24-bit fixed point
  work->transferPeriod = __divsi3(SOUND_PCM_BUFFER_SIZE, frameSamples);
  work->outputRate = __divsi3(SOUND_FRAME_RATE_E4 * frameSamples + 5000, 10000);
  work->rateStep = (__divsi3(0x1000000, work->outputRate) + 1) >> 1;
  // Timer 0 counts the CPU cycles of one sample: it overflows, and the FIFOs
  // take their next sample, frameSamples times a frame. It starts at scanline
  // 159 after the FIFO transfers restart.
  REG_TM0CNT_H = 0;
  timer = &REG_TM0CNT_L;
  *timer = -__divsi3(SOUND_CYCLES_PER_FRAME, frameSamples);
  m4aSoundVSyncOn();
  // Wait out a line 159 already begun, then for the next one to begin.
  while (*(volatile u8 *)&REG_VCOUNT == 159)
    ;
  while (*(volatile u8 *)&REG_VCOUNT != 159)
    ;
  REG_TM0CNT_H = TIMER_ENABLE;
}

/// Applies the reverb, channel-count, volume, bias and sample-rate fields of
/// `mode` to the sound work, with its ident raised. Called by m4aSoundInit with
/// AUDIO_INITIAL_MODE, and by MPlayStart with a song's mode byte
/// when its bit 7 is set, which can change only the reverb: the songs' 0xb2
/// sets it to 50 and 0x80 to 0, and a song with bit 7 clear leaves it.
void m4aSoundMode(u32 mode) {
  struct SoundInfo *work = SOUND_INFO_PTR;
  u32 modeField;
  struct SoundChannel *note;

  if (work->ident != ID_NUMBER)
    return;
  work->ident++;

  // A zero field leaves the corresponding setting unchanged.
  modeField = mode & 0xFF;
  if (modeField != 0)
    work->reverb = modeField & 0x7F;
  modeField = mode & 0xF00;
  if (modeField != 0) {
    work->pcmNoteCount = modeField >> 8;
    modeField = MAX_DIRECTSOUND_CHANNELS;
    note = work->pcmNotes;
    do {
      note->state = 0;
      modeField--;
      note++;
    } while (modeField != 0);
  }
  modeField = mode & 0xF000;
  if (modeField != 0)
    work->volume = modeField >> 12;
  // Bits 21..20 are the sound bias's resolution, into REG_SOUNDBIAS_H bits
  // 7..6 (SOUNDBIAS bits 15..14); bit 23 lets a mode ask for resolution 0.
  modeField = mode & 0xB00000;
  if (modeField != 0) {
    modeField = (modeField & 0x300000) >> 14;
    REG_SOUNDBIAS_H = (REG_SOUNDBIAS_H & 0x3F) | modeField;
  }
  modeField = mode & 0xF0000;
  if (modeField != 0) {
    m4aSoundVSyncOff();
    SampleFreqSet(modeField);
  }
  work->ident = ID_NUMBER;
}

/// Unused: silences every PCM and CGB channel with the sound work's ident
/// raised. Nothing in ☀️ calls it.
UNUSED void SoundClear(void) {
  // Splitting the counter or typed iterators changes the native EN registers.
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
/// work's ident by 10 until m4aSoundVSyncOn. Called by m4aSoundMode around a
/// rate change.
void m4aSoundVSyncOff(void) {
  struct SoundInfo *work = SOUND_INFO_PTR;
  u32 zero;

  // Only a free driver, or one raised once (m4aSoundMode's), may be
  // suspended; a suspended or uninitialized driver is left as it is.
  if (work->ident - ID_NUMBER > 1)
    return;
  work->ident += 10;
  if (REG_DMA1CNT & (DMA_REPEAT << 16))
    REG_DMA1CNT = (DMA_ENABLE | DMA_32BIT | DMA_DEST_FIXED) << 16 | 4;
  if (REG_DMA2CNT & (DMA_REPEAT << 16))
    REG_DMA2CNT = (DMA_ENABLE | DMA_32BIT | DMA_DEST_FIXED) << 16 | 4;
  REG_DMA1CNT_H = DMA_32BIT;
  REG_DMA2CNT_H = DMA_32BIT;
  // 0x318 words: both 0x630-byte buffers.
  zero = 0;
  Bios_CpuSet(&zero, work->buffers, CPU_SET_SRC_FIXED | CPU_SET_32BIT | 0x318);
}

/// Restarts both FIFO transfers and takes the 10 m4aSoundVSyncOff added off
/// the work's ident. Called by SampleFreqSet.
void m4aSoundVSyncOn(void) {
  struct SoundInfo *work = SOUND_INFO_PTR;
  u32 ident = work->ident;

  if (ident == ID_NUMBER)
    return;
  REG_DMA1CNT_H = DMA_ENABLE | DMA_START_SPECIAL | DMA_REPEAT | DMA_32BIT;
  REG_DMA2CNT_H = DMA_ENABLE | DMA_START_SPECIAL | DMA_REPEAT | DMA_32BIT;
  work->transferCountdown = 0;
  work->ident = ident - 10;
}

/// Deactivates up to sixteen tracks and links the music player into the mixer
/// tick chain. Called by m4aSoundInit for each row of gMPlayTable; does nothing
/// for no tracks or while the driver's ident is raised, and leaves the music
/// player paused with its ident free (ID_NUMBER).
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

  // Clear the music player's state and deactivate its tracks.
  Clear64byte(player);
  player->tracks = tracks;
  player->trackCount = trackCount;
  player->status = MUSICPLAYER_STATUS_PAUSE;
  while (trackCount != 0) {
    tracks->flags = 0;
    trackCount--;
    tracks++;
  }
  // Prepend this music player to the existing mixer tick chain.
  if (work->tick != NULL) {
    player->nextTick = work->tick;
    player->nextTickPlayer = work->tickPlayer;
    work->tick = NULL;
  }
  work->tickPlayer = player;
  work->tick = MPlayMain;
  work->ident = ID_NUMBER;
  player->ident = ID_NUMBER;
}

/// Starts a song when its priority permits, resetting its tracks and tempo.
/// The song takes the first of the music player's tracks, as many as both
/// have; the other tracks are stopped. Called by Sound_PlaySong and the
/// m4aSongNumStart family; does nothing while the music player's ident is
/// raised.
void MPlayStart(struct MusicPlayerInfo *player,
                const struct SongHeader *header) {
  s32 trackIndex;
  struct MusicPlayerTrack *track;

  if (player->ident != ID_NUMBER)
    return;

  // Preserve a higher-priority song unless the music player is idle.
  // checkPriority is 0 for every music player (sound/music_player_table.s), so
  // in ☀️ a new song always replaces the one playing.
  if (player->checkPriority != 0 &&
      ((player->header != NULL && (player->tracks[0].flags & MPT_FLG_START)) ||
       ((player->status & MUSICPLAYER_STATUS_TRACK) != 0 &&
        !(player->status & MUSICPLAYER_STATUS_PAUSE))) &&
      player->priority > header->priority)
    return;

  player->ident++;
  // Reset song timing and fade state before assigning track streams: tempo
  // 150 runs a tick a frame until the song's tempo command.
  player->status = 0;
  player->header = header;
  player->voices = header->voices;
  player->priority = header->priority;
  player->ticks = 0;
  player->tempo = 150;
  player->tempoStep = 150;
  player->tempoScale = 256;
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
  if (header->mode & 0x80)
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
/// fade out stops (or pauses) the music player. Called by MPlayMain each tick
/// through the jump table.
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

/// Recalculates dirty stereo volumes and pitch from track settings and
/// modulation. Called by MPlayMain through the jump table.
void TrkVolPitSet(struct MusicPlayerInfo *player,
                  struct MusicPlayerTrack *track) {
  u32 volume;
  s32 pan;
  s32 pitch;
  s32 bend;

  // Mix the volume modulation and clamped stereo pan: the song's volume
  // times volumeScale (0 to 64) / 32, then shared between the speakers by
  // pan, -128 (left) to 127 (right).
  if (track->flags & MPT_FLG_VOLSET) {
    volume = (u32)(track->volume * track->volumeScale) >> 5;
    if (track->modTarget == SOUND_MODULATION_VOLUME)
      volume = ((track->modAmount + 128) * volume) >> 7;
    pan = track->pan * 2 + track->panOffset;
    if (track->modTarget == SOUND_MODULATION_PAN)
      pan += track->modAmount;
    if (pan < -128)
      pan = -128;
    else if (pan > 127)
      pan = 127;
    track->outVolumeA = ((pan + 128) * volume) >> 8;
    track->outVolumeB = ((127 - pan) * volume) >> 8;
  }
  // The pitch in 1/256 semitone: tune and bend * bendRange are in 1/64
  // semitone, the key offsets in semitones and the LFO in 1/16. The signed
  // offsets are multiplied rather than shifted.
  if (track->flags & MPT_FLG_PITSET) {
    bend = track->bend * track->bendRange;
    pitch = (track->tune + bend) * 4 + (track->keyOffsetA * 256) +
            (track->keyOffsetB * 256) + track->fine;
    if (track->modTarget == SOUND_MODULATION_PITCH)
      pitch += track->modAmount * 16;
    track->pitchHi = pitch >> 8;
    track->pitchLo = pitch;
  }
  track->flags &= ~(MPT_FLG_VOLSET | MPT_FLG_PITSET);
}

/// Converts a key and fractional pitch to a CGB tone or noise-register value.
/// Installed as the work's cgbFrequency callback by MPlayExtender. For
/// the noise channel (CGB_CHANNEL_NOISE) it returns an NR43 value for keys 21
/// to 80 (others play the nearer end) and ignores fine. For a tone channel it
/// returns the 11-bit frequency value, 2048 minus the period, for keys 36 to
/// 166, with fine (0 to 255) moving it towards the next key; keys below 36
/// play key 36 and keys above 166 play key 166 at fine 255: key 36 gives 44,
/// a period of 2004.
s32 MidiKeyToCgbFreq(u8 kind, u8 key, u8 fine) {
  s32 lowerFrequency;
  s32 upperFrequency;

  if (kind == CGB_CHANNEL_NOISE) {
    if (key <= 20)
      key = 0;
    else {
      key -= 21;
      if (key > 59)
        key = 59;
    }
    return gNoiseTable[key];
  }

  if (key <= 35) {
    fine = 0;
    key = 0;
  } else {
    key -= 36;
    if (key > 130) {
      key = 130;
      fine = 255;
    }
  }
  // A key's code: the low four bits pick the semitone's negative period in
  // gCgbFreqTable, the high four bits the octaves to halve it.
  lowerFrequency = gCgbScaleTable[key];
  lowerFrequency = gCgbFreqTable[lowerFrequency & 15] >> (lowerFrequency >> 4);
  upperFrequency = gCgbScaleTable[key + 1];
  upperFrequency = gCgbFreqTable[upperFrequency & 15] >> (upperFrequency >> 4);
  return lowerFrequency + ((fine * (upperFrequency - lowerFrequency)) >> 8) +
         2048;
}

/// Disables one CGB channel using its envelope or wave-control register.
/// Called by CgbSound, and installed as the work's cgbMute callback.
void CgbOscOff(u8 channel) {
  switch (channel) {
  case CGB_CHANNEL_SQUARE_1:
    REG_NR12 = 8;
    REG_NR14 = 0x80;
    break;
  case CGB_CHANNEL_SQUARE_2:
    REG_NR22 = 8;
    REG_NR24 = 0x80;
    break;
  case CGB_CHANNEL_WAVE:
    REG_NR30 = 0;
    break;
  default:
    REG_NR42 = 8;
    REG_NR44 = 0x80;
    break;
  }
}

/// Routes a CGB note at least twice as loud on one side to that side only
/// (NR51 bits 0x0f right, 0xf0 left), returning whether it did. Compiled into
/// CgbModVol, which routes any other note to both.
static __inline__ s32 CgbPan(struct CgbChannel *note) {
  u32 right = note->outRight;
  u32 left = note->outLeft;

  if ((u8)right >= (u8)left) {
    if ((u8)right / 2 >= (u8)left) {
      note->panBits = 0x0F;
      return 1;
    }
  } else if ((u8)left / 2 >= (u8)right) {
    note->panBits = 0xF0;
    return 1;
  }
  return 0;
}

/// Chooses stereo routing and derives peak and sustain levels for a CGB note,
/// writing them into `note`: peak is the two levels' sum / 16, at most 15 on
/// one side; sustainLevel is peak * sustain / 16, rounded up. Called by
/// CgbSound when a note starts and at each envelope step.
void CgbModVol(struct CgbChannel *note) {
  // The peak is calculated in each branch, the form the bytes show: with one
  // shared calculation tbs-en is four bytes shorter.
  if (!CgbPan(note)) {
    note->panBits = 0xFF;
    note->peak = (u32)(note->outRight + note->outLeft) >> 4;
  } else {
    note->peak = (u32)(note->outRight + note->outLeft) >> 4;
    if (note->peak > 15)
      note->peak = 15;
  }
  note->sustainLevel = (note->peak * note->sustain + 15) >> 4;
  note->panBits &= note->channelBits;
}

/// Steps CGB note envelopes and writes changed pitch, volume and trigger
/// registers, once a frame. Installed as the work's cgbUpdate callback by
/// MPlayExtender; the mixer calls it.
void CgbSound(void) {
  s32 channel;
  s32 extraEnvelopeTick;
  struct SoundInfo *work = SOUND_INFO_PTR;
  struct CgbChannel *note;
  volatile u8 *sweepOrWaveControl;
  volatile u8 *lengthDutyRegister;
  volatile u8 *volumeEnvelopeRegister;
  volatile u8 *frequencyOrNoiseRegister;
  volatile u8 *triggerRegister;
  s32 envelopeControl;
  u32 byteMask = 0xff;

  // Periodically compensate with a second software envelope step: once in
  // every 15 frames, 16 steps in 15 frames, 63.7 a second. XXX: likely to keep
  // near the hardware envelope's 64 steps a second, because the frame alone
  // gives 59.7.
  if (work->cgbTick)
    work->cgbTick--;
  else
    work->cgbTick = 14;

  for (channel = CGB_CHANNEL_SQUARE_1, note = work->cgbNotes;
       channel <= SOUND_CGB_CHANNEL_COUNT; channel++, note++) {
    if (!(note->state & SOUND_CHANNEL_SF_ON))
      continue;

    // Select this channel's sweep, length, envelope and frequency registers.
    switch (channel) {
    case CGB_CHANNEL_SQUARE_1:
      sweepOrWaveControl = &REG_NR10;
      lengthDutyRegister = &REG_NR11;
      volumeEnvelopeRegister = &REG_NR12;
      frequencyOrNoiseRegister = &REG_NR13;
      triggerRegister = &REG_NR14;
      break;
    case CGB_CHANNEL_SQUARE_2:
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

    extraEnvelopeTick = work->cgbTick;
    envelopeControl = *volumeEnvelopeRegister;

    // A new note initializes its waveform and enters the attack phase.
    if (note->state & SOUND_CHANNEL_SF_START) {
      if (!(note->state & SOUND_CHANNEL_SF_STOP)) {
        note->state = SOUND_CHANNEL_SF_ENV_ATTACK;
        note->modified = CGB_CHANNEL_MO_VOL | CGB_CHANNEL_MO_PIT;
        CgbModVol(note);
        switch (channel) {
        case CGB_CHANNEL_SQUARE_1:
          *sweepOrWaveControl = note->sweep;
          // Fall through.
        case CGB_CHANNEL_SQUARE_2:
          // NRx1: the duty, which a square voice keeps in wave, in bits 7..6
          // over the length.
          *lengthDutyRegister = ((u32)note->wave << 6) + note->length;
          // A labelled jump, the form this match needs.
          goto init_envelope;
        case CGB_CHANNEL_WAVE:
          // A new wave: NR30 bit 6 selects the bank the CPU writes, then 0
          // plays it
          if (note->wave != note->loadedWave) {
            *sweepOrWaveControl = 0x40;
            REG_WAVE_RAM[0] = note->wave[0];
            REG_WAVE_RAM[1] = note->wave[1];
            REG_WAVE_RAM[2] = note->wave[2];
            REG_WAVE_RAM[3] = note->wave[3];
            note->loadedWave = note->wave;
          }
          *sweepOrWaveControl = 0;
          *lengthDutyRegister = note->length;
          // The trigger byte: 0x80 restarts the channel, 0x40 lets its length
          // counter stop it
          if (note->length)
            note->trigger = 0xc0;
          else
            note->trigger = 0x80;
          break;
        default:
          *lengthDutyRegister = note->length;
          *frequencyOrNoiseRegister = (u32)note->wave << 3;
        init_envelope:
          // Envelope bit 3 counts the volume up, at the attack's step time
          envelopeControl = note->attack + 8;
          if (note->length)
            note->trigger = 0x40;
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
      // Tail notes expire; stopped notes enter release before their tail.
    } else if (note->state & SOUND_CHANNEL_SF_IEC) {
      note->echoLength--;
      if ((s8)(note->echoLength & byteMask) > 0)
        goto write;
    off:
      CgbOscOff(channel);
      note->state = 0;
      goto done;
    } else if ((note->state & SOUND_CHANNEL_SF_STOP) &&
               (note->state & SOUND_CHANNEL_SF_ENV)) {
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
            note->level = ((note->peak * note->echoLevel) + 0xff) >> 8;
            if (note->level) {
              note->state |= SOUND_CHANNEL_SF_IEC;
              note->modified |= CGB_CHANNEL_MO_VOL;
              if (channel != CGB_CHANNEL_WAVE)
                envelopeControl = 8;
              goto write;
            }
            goto off;
          }
          note->counter = note->release;
        } else if ((note->state & SOUND_CHANNEL_SF_ENV) ==
                   SOUND_CHANNEL_SF_ENV_SUSTAIN) {
        sustain:
          note->level = note->sustainLevel;
          note->counter = 7;
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
              envelopeControl = 8;
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
      if (extraEnvelopeTick == 0) {
        extraEnvelopeTick--;
        goto step;
      }
    }

  // Only changed pitch and volume settings reach the hardware.
  write:
    if (note->modified & CGB_CHANNEL_MO_PIT) {
      if (channel < CGB_CHANNEL_NOISE && (note->kind & 8)) {
        s32 bias = REG_SOUNDBIAS_H;

        // The sound bias's resolution (REG_SOUNDBIAS_H bits 7..6): at 0 or 1
        // the frequency is rounded to a multiple of 4 or 2

        if (bias < 0x40)
          note->frequency = (note->frequency + 2) & 0x7fc;
        else if (bias < 0x80)
          note->frequency = (note->frequency + 1) & 0x7fe;
      }
      if (channel != CGB_CHANNEL_NOISE)
        *frequencyOrNoiseRegister = note->frequency;
      else
        *frequencyOrNoiseRegister =
            (*frequencyOrNoiseRegister & 8) | note->frequency;
      // The frequency's top three bits go in the trigger register's low bits
      note->trigger = (note->trigger & 0xc0) + *((u8 *)&note->frequency + 1);
      *triggerRegister = (s8)(note->trigger & byteMask);
    }

    if (note->modified & CGB_CHANNEL_MO_VOL) {
      // NR51 routes each channel to the left and right speakers
      REG_NR51 = (REG_NR51 & ~note->channelBits) | note->panBits;
      if (channel == CGB_CHANNEL_WAVE) {
        // CGB_CHANNEL_WAVE has no envelope: its level maps to an NR32 volume
        // code
        *volumeEnvelopeRegister = gCgb3Vol[note->level];
        if (note->trigger & 0x80) {
          *sweepOrWaveControl = 0x80;
          *triggerRegister = note->trigger;
          note->trigger &= 0x7f;
        }
      } else {
        // NRx2: the starting volume in the high nibble, the envelope below
        envelopeControl &= 0xf;
        *volumeEnvelopeRegister = (note->level << 4) + envelopeControl;
        *triggerRegister = note->trigger | 0x80;
        // XXX: CGB_CHANNEL_SQUARE_1 is restarted a second time unless NR10 bit
        // 3 is set (a falling sweep, or 8 alone, no sweep); why is not known.
        // How the hardware starts a rising sweep, tested on a console, would
        // say.
        if (channel == CGB_CHANNEL_SQUARE_1 && !(*sweepOrWaveControl & 8))
          *triggerRegister = note->trigger | 0x80;
      }
    }

  done:
    note->modified = 0;
  }
}

/// Sets the tempo multiplier (0x100 the song's own tempo) and updates the
/// number of ticks advanced per frame. Called by the music layer's tempo and
/// pitch ramp and Sound_SetBgmTempo (sound.c); does nothing while the music
/// player's ident is raised.
void m4aMPlayTempoControl(struct MusicPlayerInfo *player, u16 scale) {
  if (player->ident == ID_NUMBER) {
    player->ident++;
    player->tempoScale = scale;
    player->tempoStep = (player->tempo * player->tempoScale) >> 8;
    player->ident = ID_NUMBER;
  }
}

/// Sets the volume scale of active tracks selected by the bit mask and marks
/// their output for recalculation. `volume` runs 0 to 0x100 (full) and is
/// kept as volumeScale, a quarter of it. Called by the music layer's volume
/// ramps and commands (sound.c).
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

/// Sets the semitone and fractional pitch offsets of selected active tracks
/// and marks their pitch for recalculation. `pitch` is in 1/256 semitone: its
/// high byte becomes keyOffsetB and its low byte fine. Called by the music
/// layer's pitch ramps and commands (sound.c).
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

/// Unused: sets the stereo pan offset of selected active tracks. Nothing in ☀️
/// calls it.
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
/// modulation. Nothing in ☀️ calls it; songs set the depth with their own
/// command (ply_mod).
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
/// Nothing in ☀️ calls it; songs set the speed with their own command
/// (ply_lfos).
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
/// Song byte 0xB9 (MPLAY_JUMP_MEMACC): an operation (MemAccOp), the index of
/// a byte in the music player's workBytes and an operand, which the MEMACC_MEM_
/// operations read as the index of a second byte. A comparison that holds
/// jumps like GOTO to the address that follows; one that fails skips it. Every
/// music player shares gMPlayMemAccArea (m4aSoundInit), so one song can test
/// what another has set.
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
  (*(&gMPlayJumpTable[MPLAY_JUMP_GOTO]))(player, track);
  return;
skip:
  track->cursor += 4;
}

/// Reads an extended-command index and invokes its handler. Song byte 0xCD
/// (MPLAY_JUMP_XCMD); the index is not checked against the 12 entries
/// of gXcmdTable.
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
    u32 byte0 : 8;
    u32 byte1 : 8;
    u32 byte2 : 8;
    u32 byte3 : 8;
  } part;
};

/// Finishes a track through the jump table's ply_fine entry. Extended
/// commands 0 and 3: they end the track as the finish command (0xB1) does.
void ply_xxx(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track) {
  MPlayFunc finishSequence = *(&gMPlayJumpTable[MPLAY_JUMP_FINE]);

  finishSequence(player, track);
}

/// Reads an unaligned little-endian voice target address from the sequence
/// (extended command 1).
void ply_xwave(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track) {
  const u8 *source = track->cursor;
  union WordBytes targetAddress;

  targetAddress.part.byte0 = source[0];
  targetAddress.part.byte1 = source[1];
  targetAddress.part.byte2 = source[2];
  targetAddress.part.byte3 = source[3];
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
  track->tailLevel = *track->cursor;
  track->cursor++;
}

/// Reads each new note's echo length from the sequence (extended command 9,
/// MPlayDef's xIECL).
void ply_xiecl(struct MusicPlayerInfo *player, struct MusicPlayerTrack *track) {
  track->tailLength = *track->cursor;
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
