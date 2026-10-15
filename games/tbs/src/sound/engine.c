#include "audio_engine.h"
#include "types.h"
#include "../../../common/src/sound/../../include/system/io_reg.h"

void AudioCommand_InvokeSlot34(struct SoundNote *note)
{
    void (*func)(struct SoundNote *) = (void (*)(struct SoundNote *))*(&gSound_CommandTable[34]);

    func(note);
}

void AudioCommand_InvokeSlot35(void *block)
{
    void (*func)(void *) = (void (*)(void *))*(&gSound_CommandTable[35]);

    func(block);
}

s32 __divsi3(s32 numerator, s32 denominator);
void Sound_LoadCommandTable(SoundCommand *table);
void MusicTrack_HandleNote(u32 command, struct SoundPlayer *player, struct SoundTrack *track);
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

void AudioEngine_Initialize(struct SoundWork *work)
{
    u32 zero;

    work->lock = 0;
    if (REG_DMA1CNT & 0x02000000)
        REG_DMA1CNT = 0x84400004;
    if (REG_DMA2CNT & 0x02000000)
        REG_DMA2CNT = 0x84400004;
    REG_DMA1CNT_H = 0x400;
    REG_DMA2CNT_H = 0x400;
    REG_SOUNDCNT_X = 0x8F;
    REG_SOUNDCNT_H = 0xA90E;
    REG_SOUNDBIAS_H = (REG_SOUNDBIAS_H & 0x3F) | 0x40;
    REG_DMA1SAD = (u32)work->buffers[0];
    REG_DMA1DAD = (u32)&REG_FIFO_A;
    REG_DMA2SAD = (u32)work->buffers[1];
    REG_DMA2DAD = (u32)&REG_FIFO_B;
    SOUND_WORK = work;
    zero = 0;
    Bios_CpuSet(&zero, work, 0x050003EC);
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

void AudioEngine_SetPcmRate(u32 mode)
{
    struct SoundWork *work = SOUND_WORK;
    u32 frame;
    volatile u16 *timer;

    mode = (mode & 0xF0000) >> 16;
    work->rate = mode;
    frame = gSound_FrameLengths[mode - 1];
    work->frameSamples = frame;
    work->transferPeriod = __divsi3(0x630, frame);
    work->outputRate = __divsi3(0x91D1B * frame + 5000, 10000);
    work->rateStep = (__divsi3(0x1000000, work->outputRate) + 1) >> 1;
    REG_TM0CNT_H = 0;
    timer = &REG_TM0CNT_L;
    *timer = -__divsi3(0x44940, frame);
    AudioEngine_ResumeDirectSound();
    while (*(volatile u8 *)0x04000006 == 159)
        ;
    while (*(volatile u8 *)0x04000006 != 159)
        ;
    REG_TM0CNT_H = 0x80;
}

void AudioEngine_SetMode(u32 mode)
{
    struct SoundWork *work = SOUND_WORK;
    u32 value;
    struct SoundNote *note;

    if (work->lock != SOUND_LOCK)
        return;
    work->lock++;

    value = mode & 0xFF;
    if (value != 0)
        work->reverb = value & 0x7F;
    value = mode & 0xF00;
    if (value != 0) {
        work->pcmNoteCount = value >> 8;
        value = 12;
        note = work->pcmNotes;
        do {
            note->state = 0;
            value--;
            note++;
        } while (value != 0);
    }
    value = mode & 0xF000;
    if (value != 0)
        work->volume = value >> 12;
    value = mode & 0xB00000;
    if (value != 0) {
        value = (value & 0x300000) >> 14;
        REG_SOUNDBIAS_H = (REG_SOUNDBIAS_H & 0x3F) | value;
    }
    value = mode & 0xF0000;
    if (value != 0) {
        AudioEngine_SuspendDirectSound();
        AudioEngine_SetPcmRate(value);
    }
    work->lock = SOUND_LOCK;
}

void AudioEngine_StopAllChannels(void)
{
    struct SoundWork *work = SOUND_WORK;
    s32 i;
    void *channel;

    if (work->lock != SOUND_LOCK)
        return;
    work->lock++;

    i = 12;
    channel = work->pcmNotes;
    while (i > 0) {
        ((struct SoundNote *)channel)->state = 0;
        i--;
        channel = (void *)((s32)channel + sizeof(struct SoundNote));
    }
    channel = work->cgbNotes;
    if (channel != NULL) {
        i = 1;
        while (i <= 4) {
            work->cgbMute(i);
            ((struct CgbNote *)channel)->state = 0;
            i++;
            channel = (void *)((s32)channel + sizeof(struct CgbNote));
        }
    }
    work->lock = SOUND_LOCK;
}

void AudioEngine_SuspendDirectSound(void)
{
    struct SoundWork *work = SOUND_WORK;
    u32 zero;

    if (work->lock - SOUND_LOCK > 1)
        return;
    work->lock += 10;
    if (REG_DMA1CNT & 0x02000000)
        REG_DMA1CNT = 0x84400004;
    if (REG_DMA2CNT & 0x02000000)
        REG_DMA2CNT = 0x84400004;
    REG_DMA1CNT_H = 0x400;
    REG_DMA2CNT_H = 0x400;
    zero = 0;
    Bios_CpuSet(&zero, work->buffers, 0x05000318);
}

void AudioEngine_ResumeDirectSound(void)
{
    struct SoundWork *work = SOUND_WORK;
    u32 lock = work->lock;

    if (lock == SOUND_LOCK)
        return;
    REG_DMA1CNT_H = 0xB600;
    REG_DMA2CNT_H = 0xB600;
    work->transferCountdown = 0;
    work->lock = lock - 10;
}

void MusicPlayer_Initialize(struct SoundPlayer *player, struct SoundTrack *tracks, u8 count)
{
    struct SoundWork *work;

    if (count == 0)
        return;
    if (count > 16)
        count = 16;
    work = SOUND_WORK;
    if (work->lock != SOUND_LOCK)
        return;
    work->lock++;

    AudioCommand_InvokeSlot35(player);
    player->tracks = tracks;
    player->trackCount = count;
    player->status = SOUND_PLAYER_PAUSED;
    while (count != 0) {
        tracks->flags = 0;
        count--;
        tracks++;
    }
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

void MusicPlayer_StartSong(struct SoundPlayer *player, const struct SequenceHeader *header)
{
    s32 i;
    struct SoundTrack *track;

    if (player->lock != SOUND_LOCK)
        return;

    if (player->checkPriority != 0) {
        if (player->header == NULL || !(player->tracks[0].flags & SOUND_TRACK_RESET)) {
            if ((player->status & 0xFFFF) == 0 || (player->status & SOUND_PLAYER_PAUSED))
                goto start;
        }
        if (player->priority > header->priority)
            return;
    }
start:
    player->lock++;
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

    i = 0;
    track = player->tracks;
    while (i < header->trackCount && i < player->trackCount) {
        MusicTrack_Stop(player, track);
        track->flags = SOUND_TRACK_ACTIVE | SOUND_TRACK_RESET;
        track->notes = NULL;
        track->cursor = header->tracks[i];
        i++;
        track++;
    }
    while (i < player->trackCount) {
        MusicTrack_Stop(player, track);
        track->flags = 0;
        i++;
        track++;
    }
    if (header->mode & 0x80)
        AudioEngine_SetMode(header->mode);
    player->lock = SOUND_LOCK;
}

void MusicPlayer_Stop(struct SoundPlayer *player)
{
    s32 i;
    struct SoundTrack *track;

    if (player->lock != SOUND_LOCK)
        return;
    player->lock++;
    player->status |= SOUND_PLAYER_PAUSED;
    i = player->trackCount;
    track = player->tracks;
    while (i > 0) {
        MusicTrack_Stop(player, track);
        i--;
        track++;
    }
    player->lock = SOUND_LOCK;
}

void MusicPlayer_StepFade(struct SoundPlayer *player)
{
    s32 count;
    struct SoundTrack *track;

    if (player->fadePeriod == 0)
        return;
    if (--player->fadeCounter != 0)
        return;
    player->fadeCounter = player->fadePeriod;

    if (player->fadeVolume & 2) {
        if ((player->fadeVolume += 16) >= 256) {
            player->fadeVolume = 256;
            player->fadePeriod = 0;
        }
    } else {
        if ((s16)(player->fadeVolume -= 16) <= 0) {
            count = player->trackCount;
            track = player->tracks;
            while (count > 0) {
                MusicTrack_Stop(player, track);
                if (!(player->fadeVolume & 1))
                    track->flags = 0;
                count--;
                track++;
            }
            if (player->fadeVolume & 1)
                player->status |= SOUND_PLAYER_PAUSED;
            else
                player->status = SOUND_PLAYER_PAUSED;
            player->fadePeriod = 0;
            return;
        }
    }

    count = player->trackCount;
    track = player->tracks;
    while (count > 0) {
        if (track->flags & SOUND_TRACK_ACTIVE) {
            track->volumeScale = player->fadeVolume >> 2;
            track->flags |= SOUND_TRACK_UPDATE_VOLUME;
        }
        count--;
        track++;
    }
}

void MusicTrack_CalcOutput(struct SoundPlayer *player, struct SoundTrack *track)
{
    u32 volume;
    s32 pan;
    s32 pitch;
    s32 bend;

    if (track->flags & SOUND_TRACK_VOLUME_DIRTY) {
        volume = (u32)(track->volume * track->volumeScale) >> 5;
        if (track->modTarget == 1)
            volume = ((track->modAmount + 128) * volume) >> 7;
        pan = track->pan * 2 + track->panOffset;
        if (track->modTarget == 2)
            pan += track->modAmount;
        if (pan < -128)
            pan = -128;
        else if (pan > 127)
            pan = 127;
        track->outVolumeA = ((pan + 128) * volume) >> 8;
        track->outVolumeB = ((127 - pan) * volume) >> 8;
    }
    if (track->flags & SOUND_TRACK_PITCH_DIRTY) {
        bend = track->bend * track->bendRange;
        pitch = (track->tune + bend) * 4
            + (track->keyOffsetA << 8) + (track->keyOffsetB << 8)
            + track->fine;
        if (track->modTarget == 0)
            pitch += track->modAmount << 4;
        track->pitchHi = pitch >> 8;
        track->pitchLo = pitch;
    }
    track->flags &= ~(SOUND_TRACK_VOLUME_DIRTY | SOUND_TRACK_PITCH_DIRTY);
}

s32 Cgb_KeyToFrequency(u8 kind, u8 key, u8 fine)
{
    s32 low;
    s32 high;

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
    low = gSound_CgbPitchCodes[key];
    low = gSound_CgbFrequencySteps[low & 15] >> (low >> 4);
    high = gSound_CgbPitchCodes[key + 1];
    high = gSound_CgbFrequencySteps[high & 15] >> (high >> 4);
    return low + ((fine * (high - low)) >> 8) + 2048;
}

void CgbChannel_Mute(u8 channel)
{
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

void CgbNote_UpdatePanEnvelope(struct CgbNote *note)
{
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

void Cgb_UpdateChannels(void)
{
    s32 channel;
    s32 tick;
    struct SoundWork *work = SOUND_WORK;
    struct CgbNote *note;
    volatile u8 *reg0;
    volatile u8 *reg1;
    volatile u8 *reg2;
    volatile u8 *reg3;
    volatile u8 *reg4;
    s32 envelope;
    u32 byteMask = 0xff;

    if (work->cgbTick)
        work->cgbTick--;
    else
        work->cgbTick = 14;

    for (channel = 1, note = work->cgbNotes; channel <= 4; channel++, note++) {
        if (!(note->state & CGB_NOTE_ON))
            continue;

        switch (channel) {
        case 1:
            reg0 = &REG_NR10;
            reg1 = &REG_NR11;
            reg2 = &REG_NR12;
            reg3 = &REG_NR13;
            reg4 = &REG_NR14;
            break;
        case 2:
            reg0 = &REG_NR10 + 1;
            reg1 = &REG_NR21;
            reg2 = &REG_NR22;
            reg3 = &REG_NR23;
            reg4 = &REG_NR24;
            break;
        case 3:
            reg0 = &REG_NR30;
            reg1 = &REG_NR31;
            reg2 = &REG_NR32;
            reg3 = &REG_NR33;
            reg4 = &REG_NR34;
            break;
        default:
            reg0 = &REG_NR30 + 1;
            reg1 = &REG_NR41;
            reg2 = &REG_NR42;
            reg3 = &REG_NR43;
            reg4 = &REG_NR44;
            break;
        }

        tick = work->cgbTick;
        envelope = *reg2;

        if (note->state & CGB_NOTE_START) {
            if (!(note->state & CGB_NOTE_STOP)) {
                note->state = CGB_PHASE_ATTACK;
                note->modified = CGB_NOTE_VOLUME_DIRTY | CGB_NOTE_PITCH_DIRTY;
                CgbNote_UpdatePanEnvelope(note);
                switch (channel) {
                case 1:
                    *reg0 = note->sweep;
                case 2:
                    *reg1 = ((u32)note->wave << 6) + note->length;
                    goto init_envelope;
                case 3:
                    if (note->wave != note->loadedWave) {
                        *reg0 = 0x40;
                        REG_WAVE_RAM[0] = note->wave[0];
                        REG_WAVE_RAM[1] = note->wave[1];
                        REG_WAVE_RAM[2] = note->wave[2];
                        REG_WAVE_RAM[3] = note->wave[3];
                        note->loadedWave = note->wave;
                    }
                    *reg0 = 0;
                    *reg1 = note->length;
                    if (note->length)
                        note->trigger = 0xc0;
                    else
                        note->trigger = 0x80;
                    break;
                default:
                    *reg1 = note->length;
                    *reg3 = (u32)note->wave << 3;
                init_envelope:
                    envelope = note->attack + 8;
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
        } else if (note->state & CGB_NOTE_ECHO) {
            note->echoLength--;
            if ((s8)(note->echoLength & byteMask) > 0)
                goto write;
        off:
            CgbChannel_Mute(channel);
            note->state = 0;
            goto done;
        } else if ((note->state & CGB_NOTE_STOP) && (note->state & CGB_NOTE_PHASE)) {
            note->state &= ~CGB_NOTE_PHASE;
            note->counter = note->release;
            if ((s8)(note->counter & byteMask)) {
                note->modified |= CGB_NOTE_VOLUME_DIRTY;
                if (channel != 3)
                    envelope = note->release;
                goto count_down;
            }
            goto echo;
        } else {
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
                                envelope = 8;
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
                            envelope = 8;
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
                            envelope = note->decay;
                    } else {
                        note->counter = note->attack;
                    }
                }
            }
        count_down:
            note->counter--;
            if (tick == 0) {
                tick--;
                goto step;
            }
        }

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
                *reg3 = note->frequency;
            else
                *reg3 = (*reg3 & 8) | note->frequency;
            note->trigger = (note->trigger & 0xc0) + *((u8 *)&note->frequency + 1);
            *reg4 = (s8)(note->trigger & byteMask);
        }

        if (note->modified & CGB_NOTE_VOLUME_DIRTY) {
            REG_NR51 = (REG_NR51 & ~note->channelBits) | note->panBits;
            if (channel == 3) {
                *reg2 = gSound_Cgb3LevelCodes[note->level];
                if (note->trigger & 0x80) {
                    *reg0 = 0x80;
                    *reg4 = note->trigger;
                    note->trigger &= 0x7f;
                }
            } else {
                envelope &= 0xf;
                *reg2 = (note->level << 4) + envelope;
                *reg4 = note->trigger | 0x80;
                if (channel == 1 && !(*reg0 & 8))
                    *reg4 = note->trigger | 0x80;
            }
        }

    done:
        note->modified = 0;
    }
}

void MusicPlayer_SetPitchAndUpdateFrequency(struct SoundPlayer *player, u16 scale)
{
    if (player->lock == SOUND_LOCK) {
        player->lock++;
        player->tempoScale = scale;
        player->tempoStep = (player->tempo * player->tempoScale) >> 8;
        player->lock = SOUND_LOCK;
    }
}

void MusicPlayer_SetVolume(struct SoundPlayer *player, u16 mask, u16 volume)
{
    s32 i;
    struct SoundTrack *track;
    u32 bit;

    if (player->lock != SOUND_LOCK)
        return;
    player->lock++;
    i = player->trackCount;
    track = player->tracks;
    bit = 1;
    while (i > 0) {
        if (mask & bit) {
            if (track->flags & SOUND_TRACK_ACTIVE) {
                track->volumeScale = volume >> 2;
                track->flags |= SOUND_TRACK_UPDATE_VOLUME;
            }
        }
        i--;
        track++;
        bit <<= 1;
    }
    player->lock = SOUND_LOCK;
}

void MusicPlayer_SetPitch(struct SoundPlayer *player, u16 mask, s16 pitch)
{
    s32 i;
    struct SoundTrack *track;
    u32 bit;

    if (player->lock != SOUND_LOCK)
        return;
    player->lock++;
    i = player->trackCount;
    track = player->tracks;
    bit = 1;
    while (i > 0) {
        if (mask & bit) {
            if (track->flags & SOUND_TRACK_ACTIVE) {
                track->keyOffsetB = pitch >> 8;
                track->fine = pitch;
                track->flags |= SOUND_TRACK_UPDATE_PITCH;
            }
        }
        i--;
        track++;
        bit <<= 1;
    }
    player->lock = SOUND_LOCK;
}

void MusicPlayer_SetPan(struct SoundPlayer *player, u16 mask, s8 pan)
{
    s32 i;
    struct SoundTrack *track;
    u32 bit;

    if (player->lock != SOUND_LOCK)
        return;
    player->lock++;
    i = player->trackCount;
    track = player->tracks;
    bit = 1;
    while (i > 0) {
        if (mask & bit) {
            if (track->flags & SOUND_TRACK_ACTIVE) {
                track->panOffset = pan;
                track->flags |= SOUND_TRACK_UPDATE_VOLUME;
            }
        }
        i--;
        track++;
        bit <<= 1;
    }
    player->lock = SOUND_LOCK;
}

void MusicTrack_ClearModulation(struct SoundTrack *track)
{
    track->lfoPhase = 0;
    track->modAmount = 0;
    if (track->modTarget == 0)
        track->flags |= SOUND_TRACK_UPDATE_PITCH;
    else
        track->flags |= SOUND_TRACK_UPDATE_VOLUME;
}

void MusicPlayer_SetModulationDepth(struct SoundPlayer *player, s32 maskArg, s32 valueArg)
{
    u16 mask = maskArg;
    u8 value = valueArg;
    s32 count;
    struct SoundTrack *track;
    u32 bit;

    if (player->lock != SOUND_LOCK)
        return;

    player->lock++;
    count = player->trackCount;
    track = player->tracks;
    bit = 1;
    while (count > 0) {
        if (mask & bit) {
            if (track->flags & SOUND_TRACK_ACTIVE) {
                track->modDepth = value;
                if (!track->modDepth)
                    MusicTrack_ClearModulation(track);
            }
        }
        count--;
        track++;
        bit <<= 1;
    }
    player->lock = SOUND_LOCK;
}

void MusicPlayer_SetLfoSpeed(struct SoundPlayer *player, u32 selected, u32 value)
{
    u16 mask = selected;
    u8 speed = value;
    s32 count;
    struct SoundTrack *track;
    u32 bit;

    if (player->lock != SOUND_LOCK)
        return;

    player->lock++;
    count = player->trackCount;
    track = player->tracks;
    bit = 1;
    while (count > 0) {
        if (mask & bit) {
            if (track->flags & SOUND_TRACK_ACTIVE) {
                track->lfoSpeed = speed;
                if (!track->lfoSpeed)
                    MusicTrack_ClearModulation(track);
            }
        }
        count--;
        track++;
        bit <<= 1;
    }
    player->lock = SOUND_LOCK;
}

void MusicTrack_OperateWorkByte(struct SoundPlayer *player, struct SoundTrack *track)
{
    u32 op;
    u8 *byte;
    u8 value;

    op = *track->cursor;
    track->cursor++;
    byte = player->workBytes + *track->cursor;
    track->cursor++;
    value = *track->cursor;
    track->cursor++;

    switch (op) {
    case SOUND_WORK_SET:
        *byte = value;
        return;
    case SOUND_WORK_ADD:
        *byte += value;
        return;
    case SOUND_WORK_SUBTRACT:
        *byte -= value;
        return;
    case SOUND_WORK_COPY:
        *byte = player->workBytes[value];
        return;
    case SOUND_WORK_ADD_BYTE:
        *byte += player->workBytes[value];
        return;
    case SOUND_WORK_SUBTRACT_BYTE:
        *byte -= player->workBytes[value];
        return;
    case SOUND_WORK_EQUAL:
        if (*byte == value)
            goto jump;
        goto skip;
    case SOUND_WORK_NOT_EQUAL:
        if (*byte != value)
            goto jump;
        goto skip;
    case SOUND_WORK_GREATER:
        if (*byte > value)
            goto jump;
        goto skip;
    case SOUND_WORK_GREATER_EQUAL:
        if (*byte >= value)
            goto jump;
        goto skip;
    case SOUND_WORK_LESS_EQUAL:
        if (*byte <= value)
            goto jump;
        goto skip;
    case SOUND_WORK_LESS:
        if (*byte < value)
            goto jump;
        goto skip;
    case SOUND_WORK_EQUAL_BYTE:
        if (*byte == player->workBytes[value])
            goto jump;
        goto skip;
    case SOUND_WORK_NOT_EQUAL_BYTE:
        if (*byte != player->workBytes[value])
            goto jump;
        goto skip;
    case SOUND_WORK_GREATER_BYTE:
        if (*byte > player->workBytes[value])
            goto jump;
        goto skip;
    case SOUND_WORK_GREATER_EQUAL_BYTE:
        if (*byte >= player->workBytes[value])
            goto jump;
        goto skip;
    case SOUND_WORK_LESS_EQUAL_BYTE:
        if (*byte <= player->workBytes[value])
            goto jump;
        goto skip;
    case SOUND_WORK_LESS_BYTE:
        if (*byte < player->workBytes[value])
            goto jump;
        goto skip;
    default:
        return;
    }

jump:
    (*(&gSound_CommandTable[1]))(player, track);
    return;
skip:
    track->cursor += 4;
}

void MusicTrack_DispatchStreamCommand(struct SoundPlayer *player, struct SoundTrack *track)
{
    s32 command;

    command = *track->cursor;
    track->cursor++;
    gSound_ExtendedCommandTable[command](player, track);
}

union PackedWord {
    u32 value;
    struct {
        u32 byte0:8;
        u32 byte1:8;
        u32 byte2:8;
        u32 byte3:8;
    } part;
};

void Runtime_CallWorkEntryWithArgs(struct SoundPlayer *player, struct SoundTrack *track)
{
    SoundCommand func = *(&gSound_CommandTable[0]);

    func(player, track);
}

void MusicTrack_ReadVoiceTarget(struct SoundPlayer *player, struct SoundTrack *track)
{
    const u8 *pos = track->cursor;
    union PackedWord word;

    word.part.byte0 = pos[0];
    word.part.byte1 = pos[1];
    word.part.byte2 = pos[2];
    word.part.byte3 = pos[3];
    track->voice.target = (const void *)word.value;
    track->cursor = pos + 4;
}

void MusicTrack_SetVoiceKind(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.kind = *track->cursor;
    track->cursor++;
}

void MusicTrack_SetVoiceAttack(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.attack = *track->cursor;
    track->cursor++;
}

void MusicTrack_SetVoiceDecay(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.decay = *track->cursor;
    track->cursor++;
}

void MusicTrack_SetVoiceSustain(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.sustain = *track->cursor;
    track->cursor++;
}

void MusicTrack_SetVoiceRelease(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.release = *track->cursor;
    track->cursor++;
}

void MusicTrack_SetTailLevel(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->tailLevel = *track->cursor;
    track->cursor++;
}

void MusicTrack_SetTailLength(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->tailLength = *track->cursor;
    track->cursor++;
}

void MusicTrack_SetVoiceLength(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.length = *track->cursor;
    track->cursor++;
}

void MusicTrack_SetVoicePanOrSweep(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.panOrSweep = *track->cursor;
    track->cursor++;
}
