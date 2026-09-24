#include "AUDIO_ENGINE.H"

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
void CgbNote_UpdatePanEnvelope(struct CgbNote *note);

/* CGB channel 3 output-level codes, indexed by envelope level. */
extern const u8 Sound_Cgb3LevelCodes[];

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
