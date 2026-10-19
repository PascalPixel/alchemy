#include "audio_engine.h"
#include "types.h"
#include "../../include/system/io_reg.h"

/// Detaches a note through the command table entry installed by the driver.
void AudioCommand_InvokeSlot34(struct SoundNote *note) {
  // Taking the entry address folds its slot into a literal address. Direct
  // indexing adds an address-adjustment instruction with the library compiler.
  void (*detachNote)(struct SoundNote *) =
      (void (*)(struct SoundNote *)) *
      (&gSound_CommandTable[SOUND_COMMAND_DETACH_NOTE]);

  detachNote(note);
}

/// Clears a 64-byte player or track prefix through the driver command table.
void AudioCommand_InvokeSlot35(void *block) {
  void (*clearWork)(void *) =
      (void (*)(void *)) * (&gSound_CommandTable[SOUND_COMMAND_CLEAR_WORK]);

  clearWork(block);
}

s32 __divsi3(s32 numerator, s32 denominator);
void Sound_LoadCommandTable(SoundCommand *table);
void MusicTrack_HandleNote(u32 command, struct SoundPlayer *player,
                           struct SoundTrack *track);
void Audio_EmptyCallback(void);
void AudioEngine_SetPcmRate(u32 mode);
void AudioEngine_SuspendDirectSound(void);
void AudioEngine_ResumeDirectSound(void);
extern const u16 gSound_FrameLengths[];
extern SoundCommand gSound_CommandTable[36];

void MusicPlayer_Tick(struct SoundPlayer *player);
void MusicTrack_Stop(struct SoundPlayer *player, struct SoundTrack *track);
void AudioEngine_SetMode(u32 mode);

struct SoundPlayer;

extern const u8 gSound_CgbPitchCodes[];
extern const s16 gSound_CgbFrequencySteps[];
extern const u8 gSound_NoisePitchCodes[];

void CgbChannel_Mute(u8 channel);

extern const u8 gSound_Cgb3LevelCodes[];

/// Resets the sound hardware, clears mixer state and installs the default PCM
/// driver.
void AudioEngine_Initialize(struct SoundWork *work) {
  u32 zero;

  // Stop repeat transfers before configuring the stereo FIFO outputs.
  work->lock = 0;
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
  // Clear the complete work block, then install the PCM driver entries.
  SOUND_WORK = work;
  zero = 0;
  Bios_CpuSet(&zero, work, CPU_SET_SRC_FIXED | CPU_SET_32BIT | 0x3EC);
  work->pcmNoteCount = 8;
  work->volume = 15;
  work->noteOn = MusicTrack_HandleNote;
  work->cgbUpdate = Audio_EmptyCallback;
  work->cgbMute = (void (*)(u8))Audio_EmptyCallback;
  work->cgbFrequency = (s32 (*)(u8, u8, u8))Audio_EmptyCallback;
  work->unk3c = Audio_EmptyCallback;
  Sound_LoadCommandTable(gSound_CommandTable);
  work->commands = gSound_CommandTable;
  AudioEngine_SetPcmRate(0x40000);
  work->lock = SOUND_LOCK;
}

/// Sets the PCM frame length and Timer 0 reload, starting playback at scanline
/// 159.
void AudioEngine_SetPcmRate(u32 mode) {
  struct SoundWork *work = SOUND_WORK;
  u32 frameSamples;
  volatile u16 *timer;

  // Convert the selected frame length into mixer and timer increments.
  mode = (mode & 0xF0000) >> 16;
  work->rate = mode;
  frameSamples = gSound_FrameLengths[mode - 1];
  work->frameSamples = frameSamples;
  work->transferPeriod = __divsi3(0x630, frameSamples);
  work->outputRate = __divsi3(0x91D1B * frameSamples + 5000, 10000);
  work->rateStep = (__divsi3(0x1000000, work->outputRate) + 1) >> 1;
  // Arm Timer 0 at scanline 159 after restarting the FIFO transfers.
  REG_TM0CNT_H = 0;
  timer = &REG_TM0CNT_L;
  *timer = -__divsi3(0x44940, frameSamples);
  AudioEngine_ResumeDirectSound();
  while (*(volatile u8 *)&REG_VCOUNT == 159)
    ;
  while (*(volatile u8 *)&REG_VCOUNT != 159)
    ;
  REG_TM0CNT_H = 0x80;
}

/// Applies the supplied reverb, channel-count, volume, bias and sample-rate
/// fields.
void AudioEngine_SetMode(u32 mode) {
  struct SoundWork *work = SOUND_WORK;
  u32 modeField;
  struct SoundNote *note;

  if (work->lock != SOUND_LOCK)
    return;
  work->lock++;

  // A zero field leaves the corresponding setting unchanged.
  modeField = mode & 0xFF;
  if (modeField != 0)
    work->reverb = modeField & 0x7F;
  modeField = mode & 0xF00;
  if (modeField != 0) {
    work->pcmNoteCount = modeField >> 8;
    modeField = SOUND_PCM_NOTE_CAPACITY;
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
  modeField = mode & 0xB00000;
  if (modeField != 0) {
    modeField = (modeField & 0x300000) >> 14;
    REG_SOUNDBIAS_H = (REG_SOUNDBIAS_H & 0x3F) | modeField;
  }
  modeField = mode & 0xF0000;
  if (modeField != 0) {
    AudioEngine_SuspendDirectSound();
    AudioEngine_SetPcmRate(modeField);
  }
  work->lock = SOUND_LOCK;
}

/// Silences every PCM and CGB note while holding the sound work lock.
void AudioEngine_StopAllChannels(void) {
  // Splitting the counter or typed iterators changes the native EN registers.
  struct SoundWork *work = SOUND_WORK;
  s32 channelCounter;
  void *channel;

  if (work->lock != SOUND_LOCK)
    return;
  work->lock++;

  // Clear every PCM slot, including slots beyond the active note count.
  channelCounter = SOUND_PCM_NOTE_CAPACITY;
  channel = work->pcmNotes;
  while (channelCounter > 0) {
    ((struct SoundNote *)channel)->state = 0;
    channelCounter--;
    channel = (void *)((s32)channel + sizeof(struct SoundNote));
  }
  // Reuse the counter as the hardware channel number for the CGB path.
  channel = work->cgbNotes;
  if (channel != NULL) {
    channelCounter = 1;
    while (channelCounter <= SOUND_CGB_CHANNEL_COUNT) {
      work->cgbMute(channelCounter);
      ((struct CgbNote *)channel)->state = 0;
      channelCounter++;
      channel = (void *)((s32)channel + sizeof(struct CgbNote));
    }
  }
  work->lock = SOUND_LOCK;
}

/// Stops both FIFO transfers and clears their sample buffers.
void AudioEngine_SuspendDirectSound(void) {
  struct SoundWork *work = SOUND_WORK;
  u32 zero;

  if (work->lock - SOUND_LOCK > 1)
    return;
  work->lock += 10;
  if (REG_DMA1CNT & (DMA_REPEAT << 16))
    REG_DMA1CNT = (DMA_ENABLE | DMA_32BIT | DMA_DEST_FIXED) << 16 | 4;
  if (REG_DMA2CNT & (DMA_REPEAT << 16))
    REG_DMA2CNT = (DMA_ENABLE | DMA_32BIT | DMA_DEST_FIXED) << 16 | 4;
  REG_DMA1CNT_H = DMA_32BIT;
  REG_DMA2CNT_H = DMA_32BIT;
  zero = 0;
  Bios_CpuSet(&zero, work->buffers, CPU_SET_SRC_FIXED | CPU_SET_32BIT | 0x318);
}

/// Restarts both FIFO transfers and releases the suspended-work lock.
void AudioEngine_ResumeDirectSound(void) {
  struct SoundWork *work = SOUND_WORK;
  u32 lock = work->lock;

  if (lock == SOUND_LOCK)
    return;
  REG_DMA1CNT_H = DMA_ENABLE | DMA_START_SPECIAL | DMA_REPEAT | DMA_32BIT;
  REG_DMA2CNT_H = DMA_ENABLE | DMA_START_SPECIAL | DMA_REPEAT | DMA_32BIT;
  work->transferCountdown = 0;
  work->lock = lock - 10;
}

/// Deactivates up to sixteen tracks and links the player into the mixer tick
/// chain.
void MusicPlayer_Initialize(struct SoundPlayer *player,
                            struct SoundTrack *tracks, u8 trackCount) {
  struct SoundWork *work;

  if (trackCount == 0)
    return;
  if (trackCount > SOUND_PLAYER_TRACK_LIMIT)
    trackCount = SOUND_PLAYER_TRACK_LIMIT;
  work = SOUND_WORK;
  if (work->lock != SOUND_LOCK)
    return;
  work->lock++;

  // Clear the player record and deactivate the supplied track slots.
  AudioCommand_InvokeSlot35(player);
  player->tracks = tracks;
  player->trackCount = trackCount;
  player->status = SOUND_PLAYER_PAUSED;
  while (trackCount != 0) {
    tracks->flags = 0;
    trackCount--;
    tracks++;
  }
  // Prepend this player to the existing mixer tick chain.
  if (work->tick != NULL) {
    player->nextTick = work->tick;
    player->nextTickPlayer = work->tickPlayer;
    work->tick = NULL;
  }
  work->tickPlayer = player;
  work->tick = MusicPlayer_Tick;
  work->lock = SOUND_LOCK;
  player->lock = SOUND_LOCK;
}

/// Starts a song when its priority permits, resetting its tracks and tempo.
void MusicPlayer_StartSong(struct SoundPlayer *player,
                           const struct SequenceHeader *header) {
  s32 trackIndex;
  struct SoundTrack *track;

  if (player->lock != SOUND_LOCK)
    return;

  // Preserve a higher-priority song unless the player is idle.
  if (player->checkPriority != 0) {
    if (player->header == NULL ||
        !(player->tracks[0].flags & SOUND_TRACK_RESET)) {
      if ((player->status & 0xFFFF) == 0 ||
          (player->status & SOUND_PLAYER_PAUSED))
        goto start;
    }
    if (player->priority > header->priority)
      return;
  }
start:
  player->lock++;
  // Reset song timing and fade state before assigning track streams.
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
    MusicTrack_Stop(player, track);
    track->flags = SOUND_TRACK_ACTIVE | SOUND_TRACK_RESET;
    track->notes = NULL;
    track->cursor = header->tracks[trackIndex];
    trackIndex++;
    track++;
  }
  // Stop leftover slots that the new song does not use.
  while (trackIndex < player->trackCount) {
    MusicTrack_Stop(player, track);
    track->flags = 0;
    trackIndex++;
    track++;
  }
  if (header->mode & 0x80)
    AudioEngine_SetMode(header->mode);
  player->lock = SOUND_LOCK;
}

/// Pauses this player and stops all of its tracks.
void MusicPlayer_Stop(struct SoundPlayer *player) {
  s32 tracksRemaining;
  struct SoundTrack *track;

  if (player->lock != SOUND_LOCK)
    return;
  player->lock++;
  player->status |= SOUND_PLAYER_PAUSED;
  tracksRemaining = player->trackCount;
  track = player->tracks;
  while (tracksRemaining > 0) {
    MusicTrack_Stop(player, track);
    tracksRemaining--;
    track++;
  }
  player->lock = SOUND_LOCK;
}

/// Advances a due fade step, updating active track volumes or stopping the
/// player.
void MusicPlayer_StepFade(struct SoundPlayer *player) {
  s32 tracksRemaining;
  struct SoundTrack *track;

  if (player->fadePeriod == 0)
    return;
  if (--player->fadeCounter != 0)
    return;
  player->fadeCounter = player->fadePeriod;

  // Advance the volume while retaining its low-bit fade flags.
  if (player->fadeVolume & SOUND_FADE_IN) {
    if ((player->fadeVolume += SOUND_FADE_STEP) >= SOUND_FADE_FULL_VOLUME) {
      player->fadeVolume = SOUND_FADE_FULL_VOLUME;
      player->fadePeriod = 0;
    }
  } else {
    if ((s16)(player->fadeVolume -= SOUND_FADE_STEP) <= 0) {
      tracksRemaining = player->trackCount;
      track = player->tracks;
      while (tracksRemaining > 0) {
        MusicTrack_Stop(player, track);
        if (!(player->fadeVolume & SOUND_FADE_PAUSE))
          track->flags = 0;
        tracksRemaining--;
        track++;
      }
      if (player->fadeVolume & SOUND_FADE_PAUSE)
        player->status |= SOUND_PLAYER_PAUSED;
      else
        player->status = SOUND_PLAYER_PAUSED;
      player->fadePeriod = 0;
      return;
    }
  }

  // Propagate the new level to each active track.
  tracksRemaining = player->trackCount;
  track = player->tracks;
  while (tracksRemaining > 0) {
    if (track->flags & SOUND_TRACK_ACTIVE) {
      track->volumeScale = player->fadeVolume >> 2;
      track->flags |= SOUND_TRACK_UPDATE_VOLUME;
    }
    tracksRemaining--;
    track++;
  }
}

/// Recalculates dirty stereo volumes and pitch from track settings and
/// modulation.
void MusicTrack_CalcOutput(struct SoundPlayer *player,
                           struct SoundTrack *track) {
  u32 volume;
  s32 pan;
  s32 pitch;
  s32 bend;

  // Mix the volume modulation and clamped stereo pan.
  if (track->flags & SOUND_TRACK_VOLUME_DIRTY) {
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
  // Signed semitone offsets are scaled by multiplication, including negatives.
  if (track->flags & SOUND_TRACK_PITCH_DIRTY) {
    bend = track->bend * track->bendRange;
    pitch = (track->tune + bend) * 4 + (track->keyOffsetA * 256) +
            (track->keyOffsetB * 256) + track->fine;
    if (track->modTarget == SOUND_MODULATION_PITCH)
      pitch += track->modAmount * 16;
    track->pitchHi = pitch >> 8;
    track->pitchLo = pitch;
  }
  track->flags &= ~(SOUND_TRACK_VOLUME_DIRTY | SOUND_TRACK_PITCH_DIRTY);
}

/// Converts a key and fractional pitch to a CGB tone or noise-register value.
s32 Cgb_KeyToFrequency(u8 kind, u8 key, u8 fine) {
  s32 lowerFrequency;
  s32 upperFrequency;

  if (kind == 4) {
    if (key <= 20)
      key = 0;
    else {
      key -= 21;
      if (key > 59)
        key = 59;
    }
    return gSound_NoisePitchCodes[key];
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
  lowerFrequency = gSound_CgbPitchCodes[key];
  lowerFrequency =
      gSound_CgbFrequencySteps[lowerFrequency & 15] >> (lowerFrequency >> 4);
  upperFrequency = gSound_CgbPitchCodes[key + 1];
  upperFrequency =
      gSound_CgbFrequencySteps[upperFrequency & 15] >> (upperFrequency >> 4);
  return lowerFrequency + ((fine * (upperFrequency - lowerFrequency)) >> 8) +
         2048;
}

/// Disables one CGB channel using its envelope or wave-control register.
void CgbChannel_Mute(u8 channel) {
  switch (channel) {
  case 1:
    REG_NR12 = 8;
    REG_NR14 = 0x80;
    break;
  case 2:
    REG_NR22 = 8;
    REG_NR24 = 0x80;
    break;
  case 3:
    REG_NR30 = 0;
    break;
  default:
    REG_NR42 = 8;
    REG_NR44 = 0x80;
    break;
  }
}

/// Chooses stereo routing and derives peak and sustain levels for a CGB note.
/// Sharing the peak calculation and removing these jumps shortens EN by four
/// bytes.
void CgbNote_UpdatePanEnvelope(struct CgbNote *note) {
  u32 right = note->outRight;
  u32 left = note->outLeft;

  if ((u8)right >= (u8)left) {
    if ((u8)right / 2 >= (u8)left) {
      note->panBits = 0x0F;
      goto one_side;
    }
  } else if ((u8)left / 2 >= (u8)right) {
    note->panBits = 0xF0;
    goto one_side;
  }
  note->panBits = 0xFF;
  note->peak = (u32)(note->outRight + note->outLeft) >> 4;
  goto scale;
one_side:
  note->peak = (u32)(note->outRight + note->outLeft) >> 4;
  if (note->peak > 15)
    note->peak = 15;
scale:
  note->sustainLevel = (note->peak * note->sustain + 15) >> 4;
  note->panBits &= note->channelBits;
}

/// Steps CGB note envelopes and writes changed pitch, volume and trigger
/// registers.
void Cgb_UpdateChannels(void) {
  s32 channel;
  s32 extraEnvelopeTick;
  struct SoundWork *work = SOUND_WORK;
  struct CgbNote *note;
  volatile u8 *sweepOrWaveControl;
  volatile u8 *lengthDutyRegister;
  volatile u8 *volumeEnvelopeRegister;
  volatile u8 *frequencyOrNoiseRegister;
  volatile u8 *triggerRegister;
  s32 envelopeControl;
  u32 byteMask = 0xff;

  // Periodically compensate with a second software envelope step.
  if (work->cgbTick)
    work->cgbTick--;
  else
    work->cgbTick = 14;

  for (channel = 1, note = work->cgbNotes; channel <= SOUND_CGB_CHANNEL_COUNT;
       channel++, note++) {
    if (!(note->state & CGB_NOTE_ON))
      continue;

    // Select this channel's sweep, length, envelope and frequency registers.
    switch (channel) {
    case 1:
      sweepOrWaveControl = &REG_NR10;
      lengthDutyRegister = &REG_NR11;
      volumeEnvelopeRegister = &REG_NR12;
      frequencyOrNoiseRegister = &REG_NR13;
      triggerRegister = &REG_NR14;
      break;
    case 2:
      sweepOrWaveControl = &REG_NR10 + 1;
      lengthDutyRegister = &REG_NR21;
      volumeEnvelopeRegister = &REG_NR22;
      frequencyOrNoiseRegister = &REG_NR23;
      triggerRegister = &REG_NR24;
      break;
    case 3:
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
    if (note->state & CGB_NOTE_START) {
      if (!(note->state & CGB_NOTE_STOP)) {
        note->state = CGB_PHASE_ATTACK;
        note->modified = CGB_NOTE_VOLUME_DIRTY | CGB_NOTE_PITCH_DIRTY;
        CgbNote_UpdatePanEnvelope(note);
        switch (channel) {
        case 1:
          *sweepOrWaveControl = note->sweep;
          // Fall through.
        case 2:
          *lengthDutyRegister = ((u32)note->wave << 6) + note->length;
          goto init_envelope;
        case 3:
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
          if (note->length)
            note->trigger = 0xc0;
          else
            note->trigger = 0x80;
          break;
        default:
          *lengthDutyRegister = note->length;
          *frequencyOrNoiseRegister = (u32)note->wave << 3;
        init_envelope:
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
    } else if (note->state & CGB_NOTE_ECHO) {
      note->echoLength--;
      if ((s8)(note->echoLength & byteMask) > 0)
        goto write;
    off:
      CgbChannel_Mute(channel);
      note->state = 0;
      goto done;
    } else if ((note->state & CGB_NOTE_STOP) &&
               (note->state & CGB_NOTE_PHASE)) {
      note->state &= ~CGB_NOTE_PHASE;
      note->counter = note->release;
      if ((s8)(note->counter & byteMask)) {
        note->modified |= CGB_NOTE_VOLUME_DIRTY;
        if (channel != 3)
          envelopeControl = note->release;
        goto count_down;
      }
      goto echo;
    } else {
    // Attack, decay, sustain and release share this countdown path.
    step:
      if (note->counter == 0) {
        if (channel == 3)
          note->modified |= CGB_NOTE_VOLUME_DIRTY;
        CgbNote_UpdatePanEnvelope(note);
        if ((note->state & CGB_NOTE_PHASE) == CGB_PHASE_RELEASE) {
          note->level--;
          if ((s8)(note->level & byteMask) <= 0) {
          echo:
            note->level = ((note->peak * note->echoLevel) + 0xff) >> 8;
            if (note->level) {
              note->state |= CGB_NOTE_ECHO;
              note->modified |= CGB_NOTE_VOLUME_DIRTY;
              if (channel != 3)
                envelopeControl = 8;
              goto write;
            }
            goto off;
          }
          note->counter = note->release;
        } else if ((note->state & CGB_NOTE_PHASE) == CGB_PHASE_SUSTAIN) {
        sustain:
          note->level = note->sustainLevel;
          note->counter = 7;
        } else if ((note->state & CGB_NOTE_PHASE) == CGB_PHASE_DECAY) {
          note->level--;
          if ((s8)(note->level & byteMask) <= (s8)note->sustainLevel) {
          sustain_or_release:
            if (note->sustain == 0) {
              note->state &= ~CGB_NOTE_PHASE;
              goto echo;
            }
            note->state--;
            note->modified |= CGB_NOTE_VOLUME_DIRTY;
            if (channel != 3)
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
            note->modified |= CGB_NOTE_VOLUME_DIRTY;
            note->level = note->peak;
            if (channel != 3)
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
    if (note->modified & CGB_NOTE_PITCH_DIRTY) {
      if (channel < 4 && (note->kind & 8)) {
        s32 bias = REG_SOUNDBIAS_H;

        if (bias < 0x40)
          note->frequency = (note->frequency + 2) & 0x7fc;
        else if (bias < 0x80)
          note->frequency = (note->frequency + 1) & 0x7fe;
      }
      if (channel != 4)
        *frequencyOrNoiseRegister = note->frequency;
      else
        *frequencyOrNoiseRegister =
            (*frequencyOrNoiseRegister & 8) | note->frequency;
      note->trigger = (note->trigger & 0xc0) + *((u8 *)&note->frequency + 1);
      *triggerRegister = (s8)(note->trigger & byteMask);
    }

    if (note->modified & CGB_NOTE_VOLUME_DIRTY) {
      REG_NR51 = (REG_NR51 & ~note->channelBits) | note->panBits;
      if (channel == 3) {
        *volumeEnvelopeRegister = gSound_Cgb3LevelCodes[note->level];
        if (note->trigger & 0x80) {
          *sweepOrWaveControl = 0x80;
          *triggerRegister = note->trigger;
          note->trigger &= 0x7f;
        }
      } else {
        envelopeControl &= 0xf;
        *volumeEnvelopeRegister = (note->level << 4) + envelopeControl;
        *triggerRegister = note->trigger | 0x80;
        if (channel == 1 && !(*sweepOrWaveControl & 8))
          *triggerRegister = note->trigger | 0x80;
      }
    }

  done:
    note->modified = 0;
  }
}

/// Sets the tempo multiplier and updates the number of ticks advanced per
/// frame.
void MusicPlayer_SetPitchAndUpdateFrequency(struct SoundPlayer *player,
                                            u16 scale) {
  if (player->lock == SOUND_LOCK) {
    player->lock++;
    player->tempoScale = scale;
    player->tempoStep = (player->tempo * player->tempoScale) >> 8;
    player->lock = SOUND_LOCK;
  }
}

/// Sets the volume scale of active tracks selected by the bit mask.
void MusicPlayer_SetVolume(struct SoundPlayer *player, u16 mask, u16 volume) {
  s32 tracksRemaining;
  struct SoundTrack *track;
  u32 trackBit;

  if (player->lock != SOUND_LOCK)
    return;
  player->lock++;
  tracksRemaining = player->trackCount;
  track = player->tracks;
  trackBit = 1;
  while (tracksRemaining > 0) {
    if (mask & trackBit) {
      if (track->flags & SOUND_TRACK_ACTIVE) {
        track->volumeScale = volume >> 2;
        track->flags |= SOUND_TRACK_UPDATE_VOLUME;
      }
    }
    tracksRemaining--;
    track++;
    trackBit <<= 1;
  }
  player->lock = SOUND_LOCK;
}

/// Sets the semitone and fractional pitch offsets of selected active tracks.
void MusicPlayer_SetPitch(struct SoundPlayer *player, u16 mask, s16 pitch) {
  s32 tracksRemaining;
  struct SoundTrack *track;
  u32 trackBit;

  if (player->lock != SOUND_LOCK)
    return;
  player->lock++;
  tracksRemaining = player->trackCount;
  track = player->tracks;
  trackBit = 1;
  while (tracksRemaining > 0) {
    if (mask & trackBit) {
      if (track->flags & SOUND_TRACK_ACTIVE) {
        track->keyOffsetB = pitch >> 8;
        track->fine = pitch;
        track->flags |= SOUND_TRACK_UPDATE_PITCH;
      }
    }
    tracksRemaining--;
    track++;
    trackBit <<= 1;
  }
  player->lock = SOUND_LOCK;
}

/// Sets the stereo pan offset of selected active tracks.
void MusicPlayer_SetPan(struct SoundPlayer *player, u16 mask, s8 pan) {
  s32 tracksRemaining;
  struct SoundTrack *track;
  u32 trackBit;

  if (player->lock != SOUND_LOCK)
    return;
  player->lock++;
  tracksRemaining = player->trackCount;
  track = player->tracks;
  trackBit = 1;
  while (tracksRemaining > 0) {
    if (mask & trackBit) {
      if (track->flags & SOUND_TRACK_ACTIVE) {
        track->panOffset = pan;
        track->flags |= SOUND_TRACK_UPDATE_VOLUME;
      }
    }
    tracksRemaining--;
    track++;
    trackBit <<= 1;
  }
  player->lock = SOUND_LOCK;
}

/// Resets the LFO phase and amount, marking the affected output for
/// recalculation.
void MusicTrack_ClearModulation(struct SoundTrack *track) {
  track->lfoPhase = 0;
  track->modAmount = 0;
  if (track->modTarget == SOUND_MODULATION_PITCH)
    track->flags |= SOUND_TRACK_UPDATE_PITCH;
  else
    track->flags |= SOUND_TRACK_UPDATE_VOLUME;
}

/// Sets modulation depth on selected active tracks, resetting zero-depth
/// modulation.
void MusicPlayer_SetModulationDepth(struct SoundPlayer *player,
                                    s32 selectedTracks, s32 depthArgument) {
  u16 mask = selectedTracks;
  u8 depth = depthArgument;
  s32 tracksRemaining;
  struct SoundTrack *track;
  u32 trackBit;

  if (player->lock != SOUND_LOCK)
    return;

  player->lock++;
  tracksRemaining = player->trackCount;
  track = player->tracks;
  trackBit = 1;
  while (tracksRemaining > 0) {
    if (mask & trackBit) {
      if (track->flags & SOUND_TRACK_ACTIVE) {
        track->modDepth = depth;
        if (!track->modDepth)
          MusicTrack_ClearModulation(track);
      }
    }
    tracksRemaining--;
    track++;
    trackBit <<= 1;
  }
  player->lock = SOUND_LOCK;
}

/// Sets LFO speed on selected active tracks, resetting modulation when stopped.
void MusicPlayer_SetLfoSpeed(struct SoundPlayer *player, u32 selectedTracks,
                             u32 speedArgument) {
  u16 mask = selectedTracks;
  u8 speed = speedArgument;
  s32 tracksRemaining;
  struct SoundTrack *track;
  u32 trackBit;

  if (player->lock != SOUND_LOCK)
    return;

  player->lock++;
  tracksRemaining = player->trackCount;
  track = player->tracks;
  trackBit = 1;
  while (tracksRemaining > 0) {
    if (mask & trackBit) {
      if (track->flags & SOUND_TRACK_ACTIVE) {
        track->lfoSpeed = speed;
        if (!track->lfoSpeed)
          MusicTrack_ClearModulation(track);
      }
    }
    tracksRemaining--;
    track++;
    trackBit <<= 1;
  }
  player->lock = SOUND_LOCK;
}

/// Executes a work-byte operation and consumes a branch address for
/// comparisons.
void MusicTrack_OperateWorkByte(struct SoundPlayer *player,
                                struct SoundTrack *track) {
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
  case SOUND_WORK_SET:
    *workByte = operand;
    return;
  case SOUND_WORK_ADD:
    *workByte += operand;
    return;
  case SOUND_WORK_SUBTRACT:
    *workByte -= operand;
    return;
  case SOUND_WORK_COPY:
    *workByte = player->workBytes[operand];
    return;
  case SOUND_WORK_ADD_BYTE:
    *workByte += player->workBytes[operand];
    return;
  case SOUND_WORK_SUBTRACT_BYTE:
    *workByte -= player->workBytes[operand];
    return;
  case SOUND_WORK_EQUAL:
    if (*workByte == operand)
      goto jump;
    goto skip;
  case SOUND_WORK_NOT_EQUAL:
    if (*workByte != operand)
      goto jump;
    goto skip;
  case SOUND_WORK_GREATER:
    if (*workByte > operand)
      goto jump;
    goto skip;
  case SOUND_WORK_GREATER_EQUAL:
    if (*workByte >= operand)
      goto jump;
    goto skip;
  case SOUND_WORK_LESS_EQUAL:
    if (*workByte <= operand)
      goto jump;
    goto skip;
  case SOUND_WORK_LESS:
    if (*workByte < operand)
      goto jump;
    goto skip;
  case SOUND_WORK_EQUAL_BYTE:
    if (*workByte == player->workBytes[operand])
      goto jump;
    goto skip;
  case SOUND_WORK_NOT_EQUAL_BYTE:
    if (*workByte != player->workBytes[operand])
      goto jump;
    goto skip;
  case SOUND_WORK_GREATER_BYTE:
    if (*workByte > player->workBytes[operand])
      goto jump;
    goto skip;
  case SOUND_WORK_GREATER_EQUAL_BYTE:
    if (*workByte >= player->workBytes[operand])
      goto jump;
    goto skip;
  case SOUND_WORK_LESS_EQUAL_BYTE:
    if (*workByte <= player->workBytes[operand])
      goto jump;
    goto skip;
  case SOUND_WORK_LESS_BYTE:
    if (*workByte < player->workBytes[operand])
      goto jump;
    goto skip;
  default:
    return;
  }

jump:
  (*(&gSound_CommandTable[SOUND_COMMAND_GOTO]))(player, track);
  return;
skip:
  track->cursor += 4;
}

/// Reads an extended-command index and invokes its handler.
void MusicTrack_DispatchStreamCommand(struct SoundPlayer *player,
                                      struct SoundTrack *track) {
  s32 command;

  command = *track->cursor;
  track->cursor++;
  gSound_ExtendedCommandTable[command](player, track);
}

/// A word put together from its four bytes, lowest first.
union PackedWord {
  u32 value;
  struct {
    u32 byte0 : 8;
    u32 byte1 : 8;
    u32 byte2 : 8;
    u32 byte3 : 8;
  } part;
};

/// Finishes a track through the base command table entry.
void Runtime_CallWorkEntryWithArgs(struct SoundPlayer *player,
                                   struct SoundTrack *track) {
  SoundCommand finishSequence = *(&gSound_CommandTable[SOUND_COMMAND_FINISH]);

  finishSequence(player, track);
}

/// Reads an unaligned little-endian voice target address from the sequence.
void MusicTrack_ReadVoiceTarget(struct SoundPlayer *player,
                                struct SoundTrack *track) {
  const u8 *source = track->cursor;
  union PackedWord targetAddress;

  targetAddress.part.byte0 = source[0];
  targetAddress.part.byte1 = source[1];
  targetAddress.part.byte2 = source[2];
  targetAddress.part.byte3 = source[3];
  track->voice.target = (const void *)targetAddress.value;
  track->cursor = source + 4;
}

/// Reads the voice kind from the sequence.
void MusicTrack_SetVoiceKind(struct SoundPlayer *player,
                             struct SoundTrack *track) {
  track->voice.kind = *track->cursor;
  track->cursor++;
}

/// Reads the voice attack rate from the sequence.
void MusicTrack_SetVoiceAttack(struct SoundPlayer *player,
                               struct SoundTrack *track) {
  track->voice.attack = *track->cursor;
  track->cursor++;
}

/// Reads the voice decay rate from the sequence.
void MusicTrack_SetVoiceDecay(struct SoundPlayer *player,
                              struct SoundTrack *track) {
  track->voice.decay = *track->cursor;
  track->cursor++;
}

/// Reads the voice sustain level from the sequence.
void MusicTrack_SetVoiceSustain(struct SoundPlayer *player,
                                struct SoundTrack *track) {
  track->voice.sustain = *track->cursor;
  track->cursor++;
}

/// Reads the voice release rate from the sequence.
void MusicTrack_SetVoiceRelease(struct SoundPlayer *player,
                                struct SoundTrack *track) {
  track->voice.release = *track->cursor;
  track->cursor++;
}

/// Reads the note tail volume from the sequence.
void MusicTrack_SetTailLevel(struct SoundPlayer *player,
                             struct SoundTrack *track) {
  track->tailLevel = *track->cursor;
  track->cursor++;
}

/// Reads the note tail duration from the sequence.
void MusicTrack_SetTailLength(struct SoundPlayer *player,
                              struct SoundTrack *track) {
  track->tailLength = *track->cursor;
  track->cursor++;
}

/// Reads the hardware note length from the sequence.
void MusicTrack_SetVoiceLength(struct SoundPlayer *player,
                               struct SoundTrack *track) {
  track->voice.length = *track->cursor;
  track->cursor++;
}

/// Reads the voice pan or CGB sweep setting from the sequence.
void MusicTrack_SetVoicePanOrSweep(struct SoundPlayer *player,
                                   struct SoundTrack *track) {
  track->voice.panOrSweep = *track->cursor;
  track->cursor++;
}
