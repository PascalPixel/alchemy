#include "audio_engine.h"
#include "types.h"
#include "../../../common/src/sound/../../include/system/io_reg.h"

void AudioCommand_InvokeSlot34(struct SoundNote *note)
{
    void (*func)(struct SoundNote *) = (void (*)(struct SoundNote *))*(&Sound_CommandTable[34]);

    func(note);
}

void AudioCommand_InvokeSlot35(void *block)
{
    void (*func)(void *) = (void (*)(void *))*(&Sound_CommandTable[35]);

    func(block);
}

s32 __divsi3(s32 numerator, s32 denominator);
void Sound_LoadCommandTable(SoundCommand *table);
void MusicTrack_HandleNote(u32 command, struct SoundPlayer *player, struct SoundTrack *track);
void Audio_EmptyCallback(void);
void AudioEngine_SetPcmRate(u32 mode);
void AudioEngine_SuspendDirectSound(void);
void AudioEngine_ResumeDirectSound(void);
extern const u16 Sound_FrameLengths[];
extern SoundCommand Sound_CommandTable[36];

void MusicPlayer_Tick(struct SoundPlayer *player);
void MusicTrack_Stop(struct SoundPlayer *player, struct SoundTrack *track);
void AudioEngine_SetMode(u32 mode);

struct SoundPlayer;

extern const u8 Sound_CgbPitchCodes[];
extern const s16 Sound_CgbFrequencySteps[];
extern const u8 Sound_NoisePitchCodes[];

void CgbChannel_Mute(u8 channel);

extern const u8 Sound_Cgb3LevelCodes[];

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
    work->pcm_note_count = 8;
    work->volume = 15;
    work->note_on = MusicTrack_HandleNote;
    work->cgb_update = Audio_EmptyCallback;
    work->cgb_mute = (void (*)(u8))Audio_EmptyCallback;
    work->cgb_frequency = (s32 (*)(u8, u8, u8))Audio_EmptyCallback;
    work->unk3c = Audio_EmptyCallback;
    Sound_LoadCommandTable(Sound_CommandTable);
    work->commands = Sound_CommandTable;
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
    frame = Sound_FrameLengths[mode - 1];
    work->frame_samples = frame;
    work->transfer_period = __divsi3(0x630, frame);
    work->output_rate = __divsi3(0x91D1B * frame + 5000, 10000);
    work->rate_step = (__divsi3(0x1000000, work->output_rate) + 1) >> 1;
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
        work->pcm_note_count = value >> 8;
        value = 12;
        note = work->pcm_notes;
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
    channel = work->pcm_notes;
    while (i > 0) {
        ((struct SoundNote *)channel)->state = 0;
        i--;
        channel = (void *)((s32)channel + sizeof(struct SoundNote));
    }
    channel = work->cgb_notes;
    if (channel != NULL) {
        i = 1;
        while (i <= 4) {
            work->cgb_mute(i);
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
    work->transfer_countdown = 0;
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
    player->track_count = count;
    player->status = SOUND_PLAYER_PAUSED;
    while (count != 0) {
        tracks->flags = 0;
        count--;
        tracks++;
    }
    if (work->tick != NULL) {
        player->next_tick = work->tick;
        player->next_tick_player = work->tick_player;
        work->tick = NULL;
    }
    work->tick_player = player;
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

    if (player->check_priority != 0) {
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
    player->tempo_step = 150;
    player->tempo_scale = 256;
    player->tempo_count = 0;
    player->fade_period = 0;

    i = 0;
    track = player->tracks;
    while (i < header->track_count && i < player->track_count) {
        MusicTrack_Stop(player, track);
        track->flags = SOUND_TRACK_ACTIVE | SOUND_TRACK_RESET;
        track->notes = NULL;
        track->cursor = header->tracks[i];
        i++;
        track++;
    }
    while (i < player->track_count) {
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
    i = player->track_count;
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

    if (player->fade_period == 0)
        return;
    if (--player->fade_counter != 0)
        return;
    player->fade_counter = player->fade_period;

    if (player->fade_volume & 2) {
        if ((player->fade_volume += 16) >= 256) {
            player->fade_volume = 256;
            player->fade_period = 0;
        }
    } else {
        if ((s16)(player->fade_volume -= 16) <= 0) {
            count = player->track_count;
            track = player->tracks;
            while (count > 0) {
                MusicTrack_Stop(player, track);
                if (!(player->fade_volume & 1))
                    track->flags = 0;
                count--;
                track++;
            }
            if (player->fade_volume & 1)
                player->status |= SOUND_PLAYER_PAUSED;
            else
                player->status = SOUND_PLAYER_PAUSED;
            player->fade_period = 0;
            return;
        }
    }

    count = player->track_count;
    track = player->tracks;
    while (count > 0) {
        if (track->flags & SOUND_TRACK_ACTIVE) {
            track->volume_scale = player->fade_volume >> 2;
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
        volume = (u32)(track->volume * track->volume_scale) >> 5;
        if (track->mod_target == 1)
            volume = ((track->mod_amount + 128) * volume) >> 7;
        pan = track->pan * 2 + track->pan_offset;
        if (track->mod_target == 2)
            pan += track->mod_amount;
        if (pan < -128)
            pan = -128;
        else if (pan > 127)
            pan = 127;
        track->out_volume_a = ((pan + 128) * volume) >> 8;
        track->out_volume_b = ((127 - pan) * volume) >> 8;
    }
    if (track->flags & SOUND_TRACK_PITCH_DIRTY) {
        bend = track->bend * track->bend_range;
        pitch = (track->tune + bend) * 4
            + (track->key_offset_a << 8) + (track->key_offset_b << 8)
            + track->fine;
        if (track->mod_target == 0)
            pitch += track->mod_amount << 4;
        track->pitch_hi = pitch >> 8;
        track->pitch_lo = pitch;
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
        return Sound_NoisePitchCodes[key];
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
    low = Sound_CgbPitchCodes[key];
    low = Sound_CgbFrequencySteps[low & 15] >> (low >> 4);
    high = Sound_CgbPitchCodes[key + 1];
    high = Sound_CgbFrequencySteps[high & 15] >> (high >> 4);
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
    u32 right = note->out_right;
    u32 left = note->out_left;

    if ((u8)right >= (u8)left) {
        if ((u8)right / 2 >= (u8)left) {
            note->pan_bits = 0x0F;
            goto one_side;
        }
    } else if ((u8)left / 2 >= (u8)right) {
        note->pan_bits = 0xF0;
        goto one_side;
    }
    note->pan_bits = 0xFF;
    note->peak = (u32)(note->out_right + note->out_left) >> 4;
    goto scale;
one_side:
    note->peak = (u32)(note->out_right + note->out_left) >> 4;
    if (note->peak > 15)
        note->peak = 15;
scale:
    note->sustain_level = (note->peak * note->sustain + 15) >> 4;
    note->pan_bits &= note->channel_bits;
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
    u32 byte_mask = 0xff;

    if (work->cgb_tick)
        work->cgb_tick--;
    else
        work->cgb_tick = 14;

    for (channel = 1, note = work->cgb_notes; channel <= 4; channel++, note++) {
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

        tick = work->cgb_tick;
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
                    if (note->wave != note->loaded_wave) {
                        *reg0 = 0x40;
                        REG_WAVE_RAM[0] = note->wave[0];
                        REG_WAVE_RAM[1] = note->wave[1];
                        REG_WAVE_RAM[2] = note->wave[2];
                        REG_WAVE_RAM[3] = note->wave[3];
                        note->loaded_wave = note->wave;
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
                if ((s8)(note->counter & byte_mask)) {
                    note->level = 0;
                    goto count_down;
                }
                goto decay;
            }
            goto off;
        } else if (note->state & CGB_NOTE_ECHO) {
            note->echo_length--;
            if ((s8)(note->echo_length & byte_mask) > 0)
                goto write;
        off:
            CgbChannel_Mute(channel);
            note->state = 0;
            goto done;
        } else if ((note->state & CGB_NOTE_STOP) && (note->state & CGB_NOTE_PHASE)) {
            note->state &= ~CGB_NOTE_PHASE;
            note->counter = note->release;
            if ((s8)(note->counter & byte_mask)) {
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
                    if ((s8)(note->level & byte_mask) <= 0) {
                    echo:
                        note->level = ((note->peak * note->echo_level) + 0xff) >> 8;
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
                    note->level = note->sustain_level;
                    note->counter = 7;
                } else if ((note->state & CGB_NOTE_PHASE) == CGB_PHASE_DECAY) {
                    note->level--;
                    if ((s8)(note->level & byte_mask) <= (s8)note->sustain_level) {
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
                    if ((u8)(note->level & byte_mask) >= note->peak) {
                    decay:
                        note->state--;
                        note->counter = note->decay;
                        if ((s8)(note->counter & byte_mask) == 0)
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
            *reg4 = (s8)(note->trigger & byte_mask);
        }

        if (note->modified & CGB_NOTE_VOLUME_DIRTY) {
            REG_NR51 = (REG_NR51 & ~note->channel_bits) | note->pan_bits;
            if (channel == 3) {
                *reg2 = Sound_Cgb3LevelCodes[note->level];
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
        player->tempo_scale = scale;
        player->tempo_step = (player->tempo * player->tempo_scale) >> 8;
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
    i = player->track_count;
    track = player->tracks;
    bit = 1;
    while (i > 0) {
        if (mask & bit) {
            if (track->flags & SOUND_TRACK_ACTIVE) {
                track->volume_scale = volume >> 2;
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
    i = player->track_count;
    track = player->tracks;
    bit = 1;
    while (i > 0) {
        if (mask & bit) {
            if (track->flags & SOUND_TRACK_ACTIVE) {
                track->key_offset_b = pitch >> 8;
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
    i = player->track_count;
    track = player->tracks;
    bit = 1;
    while (i > 0) {
        if (mask & bit) {
            if (track->flags & SOUND_TRACK_ACTIVE) {
                track->pan_offset = pan;
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
    track->lfo_phase = 0;
    track->mod_amount = 0;
    if (track->mod_target == 0)
        track->flags |= SOUND_TRACK_UPDATE_PITCH;
    else
        track->flags |= SOUND_TRACK_UPDATE_VOLUME;
}

void MusicPlayer_SetModulationDepth(struct SoundPlayer *player, s32 mask_arg, s32 value_arg)
{
    u16 mask = mask_arg;
    u8 value = value_arg;
    s32 count;
    struct SoundTrack *track;
    u32 bit;

    if (player->lock != SOUND_LOCK)
        return;

    player->lock++;
    count = player->track_count;
    track = player->tracks;
    bit = 1;
    while (count > 0) {
        if (mask & bit) {
            if (track->flags & SOUND_TRACK_ACTIVE) {
                track->mod_depth = value;
                if (!track->mod_depth)
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
    count = player->track_count;
    track = player->tracks;
    bit = 1;
    while (count > 0) {
        if (mask & bit) {
            if (track->flags & SOUND_TRACK_ACTIVE) {
                track->lfo_speed = speed;
                if (!track->lfo_speed)
                    MusicTrack_ClearModulation(track);
            }
        }
        count--;
        track++;
        bit <<= 1;
    }
    player->lock = SOUND_LOCK;
}
