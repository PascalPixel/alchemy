#include "types.h"

/*
 * Core-drive row 0x080fae58, 1098 bytes: the CGB (legacy Game Boy)
 * sound-channel tick. CLEAN-ROOM REDRAFT, 2026-07-31: written solely
 * from asm/080fae58.s in this repository plus in-repo vocabulary
 * (struct CgbChannel from src/080fadf0.c, SoundInfo offsets from
 * semantic/main/080f9f6c.c). No external reference of any kind was
 * consulted, and the quarantined earlier draft was never opened by
 * this lane.
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
    const u32 *wave;        /* 36 */
    const u32 *current_wave;/* 40 */
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
        volatile s32 frame;
        u32 envelope_rate;
        u8 status = channel->status;

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

        if ((status & 0x80) != 0) {
            u32 restarted = status & 0x40;

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
                        volatile u32 *wave_ram =
                            (volatile u32 *)0x04000090;
                        const u32 *wave = channel->wave;

                        *reg_sweep = 0x40;
                        wave_ram[0] = wave[0];
                        wave_ram[1] = wave[1];
                        wave_ram[2] = wave[2];
                        wave_ram[3] = wave[3];
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
                         * stack slot in the exact lane). */
                        s32 wave_disabled = -128;

                        channel->n4_control = wave_disabled;
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
                channel->n4_control =
                    channel->length != 0 ? 64 : 0;

            seed_envelope:
                channel->envelope_counter = channel->attack;
                if (channel->attack == 0) {
                    goto attack_done;
                }
                channel->envelope_volume = 0;
                goto tick;
            }
            /* restart requested while active: release the hardware
             * channel. */
            goto kill;
        }

        if ((status & 0x04) != 0) {
            channel->pseudo_echo_length -= 1;
            if ((s8)channel->pseudo_echo_length <= 0) {
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
            channel->status = status & 0xfc;
            channel->envelope_counter = channel->release;
            if (channel->release != 0) {
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
        switch (channel->status & 3) {
        case 0:
            channel->envelope_volume -= 1;
            if ((s8)channel->envelope_volume > 0) {
                channel->envelope_counter = channel->release;
                goto tick;
            }
            goto pseudo_echo;
        case 1:
        sustain:
            channel->envelope_volume = channel->sustain_goal;
            channel->envelope_counter = 7;
            goto tick;
        case 2:
            channel->envelope_volume -= 1;
            if ((s8)channel->envelope_volume >
                (s8)channel->sustain_goal) {
                channel->envelope_counter = channel->decay;
                goto tick;
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
        default:
            channel->envelope_volume += 1;
            if (channel->envelope_volume < channel->envelope_goal) {
                channel->envelope_counter = channel->attack;
                goto tick;
            }
        attack_done:
            channel->status -= 1;
            channel->envelope_counter = channel->decay;
            if (channel->decay == 0) {
                goto decay_done;
            }
            channel->modify |= 1;
            channel->envelope_volume = channel->envelope_goal;
            if (number != 3) {
                envelope_rate = channel->decay;
            }
            goto tick;
        }

    pseudo_echo:
        channel->envelope_volume =
            (channel->pseudo_echo_volume * channel->envelope_goal
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

    tick:
        channel->envelope_counter -= 1;
        if (frame == 0) {
            frame = -1;
            goto envelope_step;
        }

    output:
        if ((channel->modify & 2) != 0) {
            if (number <= 3 && (channel->type & 8) != 0) {
                u8 bias = *(volatile u8 *)0x04000089;

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
                *reg_freq_low =
                    (u8)channel->frequency | (*reg_freq_low & 8);
            }
            channel->n4_control = (channel->n4_control & 0xc0)
                + (u8)(channel->frequency >> 8);
            *reg_freq_high = channel->n4_control;
        }

        if ((channel->modify & 1) != 0) {
            volatile u8 *master = (volatile u8 *)0x04000081;

            *master = (*master & ~channel->pan_mask) | channel->pan;
            if (number == 3) {
                *reg_envelope =
                    Data_080fba04[channel->envelope_volume];
                if ((channel->n4_control & 0x80) != 0) {
                    *reg_sweep = 0x80;
                    *reg_freq_high = channel->n4_control;
                    channel->n4_control &= 0x7f;
                }
            } else {
                envelope_rate &= 15;
                *reg_envelope =
                    (channel->envelope_volume << 4) + envelope_rate;
                *reg_freq_high = channel->n4_control | 0x80;
                if (number == 1 && (*reg_sweep & 8) == 0) {
                    *reg_freq_high = channel->n4_control | 0x80;
                }
            }
        }

    done:
        channel->modify = 0;
    }
}
