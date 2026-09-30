#include "AUDIO_ENGINE.H"
#include "TYPES.H"

void CgbNote_UpdatePanEnvelope(struct SoundNote *note);

s32 Math_Div(s32 numerator, s32 denominator);
void Sound_LoadCommandTable(SoundCommand *table);
void MusicTrack_HandleNote(u32 command, struct SoundPlayer *player, struct SoundTrack *track);
void Audio_EmptyCallback(void);
void AudioEngine_SetPcmRate(u32 mode);
void AudioEngine_SuspendDirectSound(void);
void AudioEngine_ResumeDirectSound(void);
extern const u16 Sound_FrameLengths[];
extern SoundCommand Sound_CommandTable[36];

#define SOUND_LOCK 0x68736D53
#define SOUND_WORK (*(struct SoundWork **)0x03007FF0)

void AudioCommand_InvokeSlot35(void *block);
void MusicPlayer_Tick(struct SoundPlayer *player);
void MusicTrack_Stop(struct SoundPlayer *player, struct SoundTrack *track);
void AudioEngine_SetMode(u32 mode);

struct SoundPlayer;

extern const u8 Sound_CgbPitchCodes[];
extern const s16 Sound_CgbFrequencySteps[];
extern const u8 Sound_NoisePitchCodes[];

/* A note on one of the four CGB channels, as the channel update reads it. */
struct CgbNote {
    u8 state;                       /* 0x00 */
    u8 kind;                        /* 0x01 */
    u8 out_right;                   /* 0x02 */
    u8 out_left;                    /* 0x03 */
    u8 attack;                      /* 0x04 */
    u8 decay;                       /* 0x05 */
    u8 sustain;                     /* 0x06 */
    u8 release;                     /* 0x07 */
    u8 key;                         /* 0x08 */
    u8 level;                       /* 0x09 */
    u8 peak;                        /* 0x0a */
    u8 counter;                     /* 0x0b */
    u8 echo_level;                  /* 0x0c */
    u8 echo_length;                 /* 0x0d */
    u8 unk0e[0x0b];
    u8 sustain_level;               /* 0x19 */
    u8 trigger;                     /* 0x1a */
    u8 pan_bits;                    /* 0x1b */
    u8 channel_bits;                /* 0x1c */
    u8 modified;                    /* 0x1d */
    u8 length;                      /* 0x1e */
    u8 sweep;                       /* 0x1f */
    u32 frequency;                  /* 0x20 */
    u32 *wave;                      /* 0x24 */
    u32 *loaded_wave;               /* 0x28 */
    u8 unk2c[0x14];
};

void CgbChannel_Mute(u8 channel);

/* CGB channel 3 output-level codes, indexed by envelope level. */
extern const u8 Sound_Cgb3LevelCodes[];

void AudioEngine_Initialize(struct SoundWork *work)
{
    u32 zero;

    work->lock = 0;
    if (*(u32 *)0x040000C4 & 0x02000000)
        *(u32 *)0x040000C4 = 0x84400004;
    if (*(u32 *)0x040000D0 & 0x02000000)
        *(u32 *)0x040000D0 = 0x84400004;
    *(u16 *)0x040000C6 = 0x400;
    *(u16 *)0x040000D2 = 0x400;
    *(u16 *)0x04000084 = 0x8F;
    *(u16 *)0x04000082 = 0xA90E;
    *(volatile u8 *)0x04000089 = (*(volatile u8 *)0x04000089 & 0x3F) | 0x40;
    *(u32 *)0x040000BC = (u32)work->buffers[0];
    *(u32 *)0x040000C0 = 0x040000A0;
    *(u32 *)0x040000C8 = (u32)work->buffers[1];
    *(u32 *)0x040000CC = 0x040000A4;
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
    u16 *timer;

    mode = (mode & 0xF0000) >> 16;
    work->rate = mode;
    frame = Sound_FrameLengths[mode - 1];
    work->frame_samples = frame;
    work->transfer_period = Math_Div(0x630, frame);
    work->output_rate = Math_Div(0x91D1B * frame + 5000, 10000);
    work->rate_step = (Math_Div(0x1000000, work->output_rate) + 1) >> 1;
    *(u16 *)0x04000102 = 0;
    timer = (u16 *)0x04000100;
    *timer = -Math_Div(0x44940, frame);
    AudioEngine_ResumeDirectSound();
    while (*(volatile u8 *)0x04000006 == 159)
        ;
    while (*(volatile u8 *)0x04000006 != 159)
        ;
    *(u16 *)0x04000102 = 0x80;
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
        *(volatile u8 *)0x04000089 = (*(volatile u8 *)0x04000089 & 0x3F) | value;
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
    struct SoundNote *note;
    s32 i;

    if (work->lock != SOUND_LOCK)
        return;
    work->lock++;

    i = 12;
    note = work->pcm_notes;
    while (i > 0) {
        note->state = 0;
        i--;
        note++;
    }
    note = work->cgb_notes;
    if (note != NULL) {
        for (i = 1; i <= 4; i++, note++) {
            work->cgb_mute(i);
            note->state = 0;
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
    if (*(u32 *)0x040000C4 & 0x02000000)
        *(u32 *)0x040000C4 = 0x84400004;
    if (*(u32 *)0x040000D0 & 0x02000000)
        *(u32 *)0x040000D0 = 0x84400004;
    *(u16 *)0x040000C6 = 0x400;
    *(u16 *)0x040000D2 = 0x400;
    zero = 0;
    Bios_CpuSet(&zero, work->buffers, 0x05000318);
}

void AudioEngine_ResumeDirectSound(void)
{
    struct SoundWork *work = SOUND_WORK;
    u32 lock = work->lock;

    if (lock == SOUND_LOCK)
        return;
    *(u16 *)0x040000C6 = 0xB600;
    *(u16 *)0x040000D2 = 0xB600;
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
    player->status = 0x80000000;
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
        if (player->header == NULL || !(player->tracks[0].flags & 0x40)) {
            if ((player->status & 0xFFFF) == 0 || (player->status & 0x80000000))
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
        track->flags = 0xC0;
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
    player->status |= 0x80000000;
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
                player->status |= 0x80000000;
            else
                player->status = 0x80000000;
            player->fade_period = 0;
            return;
        }
    }

    count = player->track_count;
    track = player->tracks;
    while (count > 0) {
        if (track->flags & 0x80) {
            track->volume_scale = player->fade_volume >> 2;
            track->flags |= 3;
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

    if (track->flags & 1) {
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
    if (track->flags & 4) {
        bend = track->bend * track->bend_range;
        pitch = (track->tune + bend) * 4
            + (track->key_offset_a << 8) + (track->key_offset_b << 8)
            + track->fine;
        if (track->mod_target == 0)
            pitch += track->mod_amount << 4;
        track->pitch_hi = pitch >> 8;
        track->pitch_lo = pitch;
    }
    track->flags &= ~5;
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
        *(u8 *)0x04000063 = 8;
        *(u8 *)0x04000065 = 0x80;
        break;
    case 2:
        *(u8 *)0x04000069 = 8;
        *(u8 *)0x0400006d = 0x80;
        break;
    case 3:
        *(u8 *)0x04000070 = 0;
        break;
    default:
        *(u8 *)0x04000079 = 8;
        *(u8 *)0x0400007d = 0x80;
        break;
    }
}

void CgbNote_UpdatePanEnvelope(struct SoundNote *note)
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

/* Steps the envelope of every sounding CGB channel once per frame and writes
   its registers. Every byte test goes through one mask register, as in the
   ROM, where each test ands with 255 held in a register. */
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

    for (channel = 1, note = (struct CgbNote *)work->cgb_notes; channel <= 4; channel++, note++) {
        if (!(note->state & 0xc7))
            continue;

        switch (channel) {
        case 1:
            reg0 = (volatile u8 *)0x04000060;
            reg1 = (volatile u8 *)0x04000062;
            reg2 = (volatile u8 *)0x04000063;
            reg3 = (volatile u8 *)0x04000064;
            reg4 = (volatile u8 *)0x04000065;
            break;
        case 2:
            reg0 = (volatile u8 *)0x04000061;
            reg1 = (volatile u8 *)0x04000068;
            reg2 = (volatile u8 *)0x04000069;
            reg3 = (volatile u8 *)0x0400006c;
            reg4 = (volatile u8 *)0x0400006d;
            break;
        case 3:
            reg0 = (volatile u8 *)0x04000070;
            reg1 = (volatile u8 *)0x04000072;
            reg2 = (volatile u8 *)0x04000073;
            reg3 = (volatile u8 *)0x04000074;
            reg4 = (volatile u8 *)0x04000075;
            break;
        default:
            reg0 = (volatile u8 *)0x04000071;
            reg1 = (volatile u8 *)0x04000078;
            reg2 = (volatile u8 *)0x04000079;
            reg3 = (volatile u8 *)0x0400007c;
            reg4 = (volatile u8 *)0x0400007d;
            break;
        }

        tick = work->cgb_tick;
        envelope = *reg2;

        if (note->state & 0x80) {
            if (!(note->state & 0x40)) {
                note->state = 3;
                note->modified = 3;
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
                        ((volatile u32 *)0x04000090)[0] = note->wave[0];
                        ((volatile u32 *)0x04000090)[1] = note->wave[1];
                        ((volatile u32 *)0x04000090)[2] = note->wave[2];
                        ((volatile u32 *)0x04000090)[3] = note->wave[3];
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
        } else if (note->state & 0x04) {
            note->echo_length--;
            if ((s8)(note->echo_length & byte_mask) > 0)
                goto write;
        off:
            CgbChannel_Mute(channel);
            note->state = 0;
            goto done;
        } else if ((note->state & 0x40) && (note->state & 0x03)) {
            note->state &= 0xfc;
            note->counter = note->release;
            if ((s8)(note->counter & byte_mask)) {
                note->modified |= 1;
                if (channel != 3)
                    envelope = note->release;
                goto count_down;
            }
            goto echo;
        } else {
        step:
            if (note->counter == 0) {
                if (channel == 3)
                    note->modified |= 1;
                CgbNote_UpdatePanEnvelope(note);
                if ((note->state & 3) == 0) {
                    note->level--;
                    if ((s8)(note->level & byte_mask) <= 0) {
                    echo:
                        note->level = ((note->peak * note->echo_level) + 0xff) >> 8;
                        if (note->level) {
                            note->state |= 4;
                            note->modified |= 1;
                            if (channel != 3)
                                envelope = 8;
                            goto write;
                        }
                        goto off;
                    }
                    note->counter = note->release;
                } else if ((note->state & 3) == 1) {
                sustain:
                    note->level = note->sustain_level;
                    note->counter = 7;
                } else if ((note->state & 3) == 2) {
                    note->level--;
                    if ((s8)(note->level & byte_mask) <= (s8)note->sustain_level) {
                    sustain_or_release:
                        if (note->sustain == 0) {
                            note->state &= 0xfc;
                            goto echo;
                        }
                        note->state--;
                        note->modified |= 1;
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
                        note->modified |= 1;
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
        if (note->modified & 2) {
            if (channel < 4 && (note->kind & 8)) {
                s32 bias = *(volatile u8 *)0x04000089;

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

        if (note->modified & 1) {
            *(volatile u8 *)0x04000081 = (*(volatile u8 *)0x04000081 & ~note->channel_bits) | note->pan_bits;
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
