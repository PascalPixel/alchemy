#include "types.h"

/*
 * Core-drive row 0x080fae58, 1098 bytes: the CGB (legacy Game Boy)
 * sound-channel tick. CLEAN-ROOM REDRAFT, 2026-07-31: written solely
 * from asm/080fae58.s in this repository plus in-repo vocabulary
 * (struct CgbChannel from src/080fadf0.c, SoundInfo offsets from
 * semantic/main/080f9f6c.c). No external reference of any kind was
 * consulted, and the quarantined earlier draft was never opened by
 * this reconstruction.
 *
 * Behaviour, read off the reference body:
 *
 * A 15-step frame counter at SoundInfo+10 counts 14..0; on the tick
 * where it reads 0 each channel's envelope advances one extra step
 * (the .L25/.L29 replay, preserved here as the same goto loop).
 *
 * Four channels, 64-byte records from SoundInfo+28 (the same
 * cgb_channels field 080f9f6c names), each mapped to its register
 * bank: 1 -> NR10/11/12/13/14 (0x04000060..65), 2 -> NR21/22/23/24
 * (0x68/69/6c/6d, sweep slot parked on the unused 0x61), 3 ->
 * NR30/31/32/33/34 (0x70/72/73/74/75), 4 -> NR41/42/43/44
 * (0x78/79/7c/7d, sweep slot parked on 0x71).
 *
 * Per channel with any of status & 0xc7 set:
 *  - status & 0x80, without 0x40: fresh start. Status becomes 3
 *    (attack), both modify bits raised, Func_080fadf0 refreshes pan;
 *    the length/duty/sweep registers are programmed (channel 3
 *    reloads wave RAM at 0x04000090 through a 16-byte copy when the
 *    wave pointer changed, caching it in current_wave), the NRx4
 *    control cache at +26 takes 64/0 (192/0x80 for channel 3) by
 *    length-enable, and the envelope state machine seeds from attack.
 *  - status & 0x80 with 0x40, or a fade that expired: Func_080fada0
 *    releases the hardware channel and status clears to 0.
 *  - status & 0x04: pseudo-echo fade, counting pseudo_echo_length
 *    down to release.
 *  - status & 0x40 with a note still keyed (low two bits set): key
 *    release -- low bits clear, the release rate reloads the
 *    envelope counter.
 *  - otherwise the envelope advances when its counter expires:
 *    attack rises envelope_volume to envelope_goal then falls to
 *    decay; decay sinks to sustain_goal then holds in sustain
 *    (counter 7) or, with sustain disabled, drops into the
 *    pseudo-echo tail ((pseudo_echo_volume * envelope_goal + 255)
 *    >> 8, status |= 4) or dies.
 *
 * Output stage per channel, gated by the modify bits at +29 (bit 2 =
 * pitch, bit 1 = volume, cleared at the end):
 *  - pitch: channels 1-3 with type bit 8 nudge the stored frequency
 *    against the SOUNDBIAS amplitude byte at 0x04000089 ((f+2)&0x7fc
 *    below 64, (f+1)&0x7fe below 128), then NRx3 takes the low
 *    frequency byte (channel 4 preserves bit 3 of NR43), and NRx4
 *    takes (control_cache & 0xc0) + the frequency's second byte.
 *  - volume: the stereo-enable byte at 0x04000081 takes
 *    (old & ~pan_mask) | pan; channel 3 translates envelope_volume
 *    through Data_080fba04 into NR32 and retriggers via NR30=0x80
 *    when the control cache's top bit is set; channels 1/2/4 write
 *    (envelope_volume << 4) + the tracked envelope rate nibble into
 *    NRx2 and retrigger through NRx4 | 0x80 -- channel 1 writing it
 *    twice when NR10's bit 3 reads clear, exactly as the reference
 *    does.
 *
 * Complete owner: asm/080fae58.s in full including its interleaved
 * pools. Span 1098 bytes per the census row.
 */

struct CgbChannel_080fae58 {
    u8 status;              /* 0 */
    u8 type;                /* 1 */
    u8 right_volume;        /* 2 */
    u8 left_volume;         /* 3 */
    u8 attack;              /* 4 */
    u8 decay;               /* 5 */
    u8 sustain;             /* 6 */
    u8 release;             /* 7 */
    u8 key;                 /* 8 */
    u8 envelope_volume;     /* 9 */
    u8 envelope_goal;       /* 10 */
    u8 envelope_counter;    /* 11 */
    u8 pseudo_echo_volume;  /* 12 */
    u8 pseudo_echo_length;  /* 13 */
    u8 unknown_0e[2];       /* 14 */
    u8 gate_time;           /* 16 */
    u8 midi_key;            /* 17 */
    u8 velocity;            /* 18 */
    u8 priority;            /* 19 */
    u8 rhythm_pan;          /* 20 */
    u8 unknown_15[4];       /* 21 */
    s8 sustain_goal;        /* 25 */
    u8 n4_control;          /* 26: NRx4 control cache (src/080fadf0.c's
                             * unknown1a, refined by this body) */
    u8 pan;                 /* 27 */
    u8 pan_mask;            /* 28 */
    u8 modify;              /* 29: bit 1 volume, bit 2 pitch */
    u8 length;              /* 30 */
    u8 sweep;               /* 31 */
    u32 frequency;          /* 32 */
    const u8 *wave;         /* 36 */
    const u8 *current_wave; /* 40 */
    u8 unknown_2c[20];      /* 44 */
};

struct SoundInfo_080fae58 {
    u8 unknown_00[10];
    u8 cgb_frame_counter;   /* 10 */
    u8 unknown_0b[17];      /* 11 */
    struct CgbChannel_080fae58 *cgb_channels; /* 28 */
};

extern struct SoundInfo_080fae58 *Data_03007ff0;
extern const u8 Data_080fba04[];

void Func_080fadf0(struct CgbChannel_080fae58 *channel);
void Func_080fada0(u8 channel);

void Func_080fae58(void)
{
    s32 frame;
    struct SoundInfo_080fae58 *info = Data_03007ff0;
    struct CgbChannel_080fae58 *channel;
    s32 number;

    if (info->cgb_frame_counter != 0) {
        info->cgb_frame_counter -= 1;
    } else {
        info->cgb_frame_counter = 14;
    }

    number = 1;
    channel = info->cgb_channels;
    for (; number <= 4;
         number++,
         channel = (struct CgbChannel_080fae58 *)((u8 *)channel + 64)) {
        volatile u8 *reg_sweep;
        volatile u8 *reg_length;
        volatile u8 *reg_envelope;
        volatile u8 *reg_freq_low;
        volatile u8 *reg_freq_high;
        u32 envelope_rate;
        register u8 status = channel->status;
        u8 initial_status;

        if ((status & 0xc7) == 0) {
            continue;
        }

        switch (number) {
        case 1:
            reg_sweep = (volatile u8 *)0x04000060;
            reg_length = (volatile u8 *)0x04000062;
            reg_envelope = (volatile u8 *)0x04000063;
            reg_freq_low = (volatile u8 *)0x04000064;
            reg_freq_high = (volatile u8 *)0x04000065;
            break;
        case 2:
            reg_sweep = (volatile u8 *)0x04000061;
            reg_length = (volatile u8 *)0x04000068;
            reg_envelope = (volatile u8 *)0x04000069;
            reg_freq_low = (volatile u8 *)0x0400006c;
            reg_freq_high = (volatile u8 *)0x0400006d;
            break;
        case 3:
            reg_sweep = (volatile u8 *)0x04000070;
            reg_length = (volatile u8 *)0x04000072;
            reg_envelope = (volatile u8 *)0x04000073;
            reg_freq_low = (volatile u8 *)0x04000074;
            reg_freq_high = (volatile u8 *)0x04000075;
            break;
        default:
            reg_sweep = (volatile u8 *)0x04000071;
            reg_length = (volatile u8 *)0x04000078;
            reg_envelope = (volatile u8 *)0x04000079;
            reg_freq_low = (volatile u8 *)0x0400007c;
            reg_freq_high = (volatile u8 *)0x0400007d;
            break;
        }

        frame = info->cgb_frame_counter;
        envelope_rate = *reg_envelope;
        initial_status = status;

        if (((u8)initial_status & 0x80) != 0) {
            u32 restarted = (u8)initial_status & 0x40;

            if (restarted == 0) {
                channel->status = 3;
                channel->modify = 3;
                Func_080fadf0(channel);

                switch (number) {
                case 1:
                    *reg_sweep = channel->sweep;
                    /* fall through */
                case 2:
                    /* For the square channels the wave word holds the
                     * duty setting. */
                    *reg_length = (u8)(((u32)channel->wave << 6) +
                                       channel->length);
                    goto seed_envelope_rate;
                case 3:
                    if (channel->wave != channel->current_wave) {
                        const u8 *wave =
                            *(const u8 *volatile *)&channel->wave;
                        volatile u32 *wave_ram =
                            (volatile u32 *)0x04000090;

                        *reg_sweep = 0x40;
                        *wave_ram++ = ((const u32 *)wave)[0];
                        *wave_ram++ = ((const u32 *)wave)[1];
                        *wave_ram++ = ((const u32 *)wave)[2];
                        *wave_ram = ((const u32 *)wave)[3];
                        channel->current_wave = wave;
                    }
                    *reg_sweep = (u8)restarted;
                    *reg_length = channel->length;
                    if (channel->length != 0) {
                        channel->n4_control = 0xc0;
                    } else {
                        /* Spelled -128: the reference materialises the
                         * negative form here, which keeps it from
                         * merging with the +128 status-bit tests (the
                         * merge costs a callee-saved register and a
                         * stack slot in the exact reconstruction). */
                        s32 wave_disabled = -128;

                        *(volatile u8 *)&channel->n4_control = wave_disabled;
                        goto seed_envelope;
                    }
                    goto seed_envelope;
                default:
                    *reg_length = channel->length;
                    *reg_freq_low = (u8)((u32)channel->wave << 3);
                    goto seed_envelope_rate;
                }

            seed_envelope_rate:
                envelope_rate = channel->attack + 8;
                if (channel->length != 0) {
                    channel->n4_control = 64;
                } else {
                    channel->n4_control = 0;
                }

            seed_envelope:
                if ((channel->envelope_counter = channel->attack) == 0) {
                    goto attack_done;
                }
                channel->envelope_volume = 0;
                goto tick;
            }
            /* restart requested while active: release the hardware
             * channel. */
            goto kill;
        }

        if (((u8)initial_status & 0x04) != 0) {
            if ((s8)(channel->pseudo_echo_length -= 1) <= 0) {
                goto kill;
            }
            goto output;
        }
        goto not_killed;

    kill:
        Func_080fada0((u8)number);
        channel->status = 0;
        goto done;

    not_killed:

        if ((status & 0x40) != 0 && (status & 0x03) != 0) {
            s32 inactive_status = 0xfc;

            inactive_status &= status;
            channel->status = inactive_status;
            if ((channel->envelope_counter = channel->release) != 0) {
                channel->modify |= 1;
                if (number != 3) {
                    envelope_rate = channel->release;
                }
                goto tick;
            }
            goto pseudo_echo;
        }

    envelope_step:
        if (channel->envelope_counter != 0) {
            goto tick;
        }
        if (number == 3) {
            channel->modify |= 1;
        }
        Func_080fadf0(channel);
        {
            u32 envelope_state = channel->status & 3;
            u32 next_counter;

            if (envelope_state == 0) {
                if ((s8)(channel->envelope_volume -= 1) > 0) {
                    goto release_counter;
                }

            pseudo_echo:
                channel->envelope_volume =
                    (channel->envelope_goal * channel->pseudo_echo_volume
                     + 255) >> 8;
                if (channel->envelope_volume == 0) {
                    goto kill;
                }
                channel->status |= 4;
                channel->modify |= 1;
                if (number != 3) {
                    envelope_rate = 8;
                }
                goto output;

            release_counter:
                next_counter = channel->release;
                goto set_counter;
            }
            if (envelope_state == 1) {
        sustain:
                channel->envelope_volume = channel->sustain_goal;
                next_counter = 7;
                goto set_counter;
            }
            if (envelope_state == 2) {
                if ((s8)(channel->envelope_volume -= 1) >
                    (s8)channel->sustain_goal) {
                    goto decay_counter;
                }
        decay_done:
                if (channel->sustain == 0) {
                    channel->status &= 0xfc;
                    goto pseudo_echo;
                }
                channel->status -= 1;
                channel->modify |= 1;
                if (number != 3) {
                    envelope_rate = 8;
                }
                goto sustain;

            decay_counter:
                next_counter = channel->decay;
                goto set_counter;
            }
            if ((channel->envelope_volume += 1) < channel->envelope_goal) {
                goto attack_counter;
            }
        attack_done:
            channel->status -= 1;
            if ((channel->envelope_counter = channel->decay) == 0) {
                goto decay_done;
            }
            channel->modify |= 1;
            channel->envelope_volume = channel->envelope_goal;
            if (number != 3) {
                envelope_rate = channel->decay;
            }
            goto tick;

        attack_counter:
            next_counter = channel->attack;
        set_counter:
            channel->envelope_counter = next_counter;
        }

    tick:
        channel->envelope_counter -= 1;
        if (frame == 0) {
            frame -= 1;
            goto envelope_step;
        }

    output:
        if ((channel->modify & 2) != 0) {
            if (number <= 3 && (channel->type & 8) != 0) {
                s32 bias = *(volatile u8 *)0x04000089;

                if (bias <= 63) {
                    channel->frequency =
                        (channel->frequency + 2) & 0x7fc;
                } else if (bias <= 127) {
                    channel->frequency =
                        (channel->frequency + 1) & 0x7fe;
                }
            }
            if (number != 4) {
                *reg_freq_low = (u8)channel->frequency;
            } else {
                s32 current_low = *reg_freq_low;
                current_low &= 8;
                *reg_freq_low = channel->frequency | current_low;
            }
            {
                u32 n4_control = (channel->n4_control & 0xc0)
                    + ((u8 *)&channel->frequency)[1];
                channel->n4_control = n4_control;
                n4_control &= 0xff;
                *reg_freq_high = n4_control;
            }
        }

        if ((channel->modify & 1) != 0) {
            volatile u8 *master = (volatile u8 *)0x04000081;

            *master = (*master & ~channel->pan_mask) | channel->pan;
            if (!(number == 3)) {
                envelope_rate &= 15;
                *reg_envelope =
                    (channel->envelope_volume << 4) + envelope_rate;
                *reg_freq_high = channel->n4_control | 0x80;
                if (number == 1 && (*reg_sweep & 8) == 0) {
                    *reg_freq_high = channel->n4_control | 0x80;
                }
            } else {
                *reg_envelope =
                    Data_080fba04[channel->envelope_volume];
                if ((channel->n4_control & 0x80) != 0) {
                    *reg_sweep = 0x80;
                    *reg_freq_high = channel->n4_control;
                    channel->n4_control &= 0x7f;
                }
            }
        }

    done:
        channel->modify = 0;
    }
}
