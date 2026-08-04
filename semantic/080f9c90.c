#include "types.h"

/*
 * Core-drive row 0x080f9c90, 616 bytes: the per-frame music player tick.
 * Reached both by direct call and by function pointer (the row's own
 * header comment); the `bl .L1` sites inside are calls to the function's
 * OWN tail `bx r3` -- an internal call-via-r3 helper, spelled here as
 * ordinary indirect calls through typed function pointers.
 *
 * Guarded by the Smsh lock: runs only when the player's word at +52
 * holds 0x68736d53 exactly (the same magic pair 080006fc/080f9c44
 * established at 0x03007ff0), increments it while working, restores it
 * on exit -- a run-once-per-frame reentrancy gate.
 *
 * Structure, faithful to the reference:
 *  1. optional pre-callback (player +56, argument +60);
 *  2. if the status word is non-negative: fade tick (Func_080fab7c),
 *     then tempo accumulate (tempo_counter += tempo_increment) and one
 *     command pass per 150 accumulated units:
 *     per active track (flags bit 0x80): walk the channel chain --
 *     live channels (status & 0xc7) count down gate_time and raise
 *     their release bit 0x40 when it expires, dead ones go to
 *     Func_080fa678; a track restart request (flags bit 0x40) mutes via
 *     Func_080fa68c and reinitializes the record; then the command
 *     interpreter runs while wait_01 is zero (running status below
 *     0x80 via lastCommand_07, 0xcf+ to the extended handler at
 *     SoundInfo+56, 0xb1..0xce through the handler table at
 *     SoundInfo+52 -- a handler clearing the track's flags kills the
 *     track -- 0x80..0xb0 load wait_01 from the Data_080fba14 length
 *     table); finally the LFO tick (speed_19/depth_17 gated by
 *     delay_1c, triangle wave from counter_1a, on a changed value
 *     marks the dirty nibble);
 *  3. per 150-unit pass end: clock_0c++, status word becomes the
 *     active-track bit mask, or 0x80000000 when none remain;
 *  4. the output pass (runs every call): per track with flags 0x80 set
 *     AND a dirty nibble, Func_080fac44 recomputes track volume, then
 *     per live channel: pitch-dirty (0x03) runs Func_080f9f3c (the
 *     r4/r5-convention helper, two-argument spelling per
 *     semantic/main/080f9f6c.c's instruction) and marks the channel's
 *     +29 byte with 1; volume-dirty (0x0c) recomputes frequency --
 *     CGB types (type_01 & 7 nonzero) through the function pointer at
 *     SoundInfo+48, PCM through Func_080fa1fc(wave, key, pitch) --
 *     and marks +29 with 2; then the dirty nibble clears (&= 0xf0).
 *
 * THE CHANNEL-FLAGS BYTE (track +0), settled for the census:
 *   0x80 active | 0x40 restart request | low nibble = per-tick dirty
 *   mask: 0x03 pitch-dirty pair, 0x0c volume-dirty pair. The LFO sets
 *   0x0c when its type_18 is 0 (volume target), else 0x03. Cleared by
 *   the &= 0xf0 at the end of the output pass.
 *
 * Struct layouts shared verbatim with semantic/main/080f9f6c.c
 * (Channel/Track) and src/080fab7c.c (player head). One discrepancy
 * flagged rather than resolved: 080f9f6c names SoundInfo+48 pcm_rate,
 * but this body loads +48 into r3 and executes it -- here it is
 * unambiguously a function pointer (the CGB frequency routine, called
 * with (type, key, pitch)). Someone should re-read 080f9f6c's use.
 *
 * Complete owner: asm/080f9c90.s in full including the three-word tail
 * pool (0x080fba14, 0x03007ff0, 0x68736d53). Span 616 bytes, matching
 * the census row exactly.
 */

struct Track_080f9c90;

struct Channel_080f9c90 {
    u8 status;              /* 0: 0xc7 live mask, 0x40 release */
    u8 type;                /* 1: low 3 bits = CGB type, 0 = PCM */
    u8 unknown_02[6];       /* 2 */
    u8 key;                 /* 8 */
    u8 unknown_09[7];       /* 9 */
    u8 gate_time;           /* 16 */
    u8 unknown_11[12];      /* 17 */
    u8 refresh_1d;          /* 29: |=1 volume applied, |=2 pitch applied */
    u8 unknown_1e[2];       /* 30 */
    u32 frequency;          /* 32 */
    void *wave;             /* 36 */
    u8 unknown_28[12];      /* 40 */
    struct Channel_080f9c90 *next; /* 52 */
    u8 unknown_38[8];       /* 56 */
};

struct Track_080f9c90 {
    u8 flags;               /* 0: see header */
    u8 wait;                /* 1 */
    u8 unknown_02[5];       /* 2 */
    u8 last_command;        /* 7: running status */
    s8 key_shift;           /* 8 */
    u8 pitch;               /* 9 */
    u8 unknown_0a[5];       /* 10 */
    u8 unknown_0f;          /* 15: restart writes 2 */
    u8 unknown_10[3];       /* 16 */
    u8 unknown_13;          /* 19: restart writes 64 */
    u8 unknown_14[2];       /* 20 */
    u8 mod_calculated;      /* 22 */
    u8 mod_depth;           /* 23 */
    u8 mod_type;            /* 24: 0 = volume target, else pitch */
    u8 lfo_speed;           /* 25: restart writes 22 */
    u8 lfo_counter;         /* 26 */
    u8 unknown_1b;          /* 27 */
    u8 lfo_delay_counter;   /* 28 */
    u8 unknown_1d[3];       /* 29 */
    struct Channel_080f9c90 *chain; /* 32 */
    u8 voice_type;          /* 36: restart writes 1 */
    u8 unknown_25[27];      /* 37 */
    u8 *command;            /* 64 */
    u8 unknown_44[12];      /* 68 */
};

struct Player_080f9c90 {
    void *song_header;      /* 0 */
    u32 status;             /* 4: sign bit = idle, else track bit mask */
    u8 track_count;         /* 8 */
    u8 priority;            /* 9 */
    u8 command;             /* 10 */
    u8 config;              /* 11 */
    u32 clock;              /* 12 */
    u8 unknown_10[16];      /* 16 */
    u16 tempo_increment;    /* 32 */
    u16 tempo_counter;      /* 34 */
    u8 unknown_24[8];       /* 36 */
    struct Track_080f9c90 *tracks; /* 44 */
    u8 unknown_30[4];       /* 48 */
    u32 lock;               /* 52: 0x68736d53 idle, +1 while running */
    void (*callback)(u32);  /* 56 */
    u32 callback_argument;  /* 60 */
};

struct SoundInfo_080f9c90 {
    u8 unknown_00[48];
    /* 48: CGB frequency routine, (type, key, pitch) -> frequency.
     * 080f9f6c calls this offset pcm_rate; this body executes it. */
    u32 (*cgb_frequency)(u32 type, u32 key, u8 pitch);
    void (**command_table)(struct Player_080f9c90 *,
                           struct Track_080f9c90 *); /* 52 */
    void (*extended_command)(u32 command,
                             struct Player_080f9c90 *,
                             struct Track_080f9c90 *); /* 56 */
};

extern struct SoundInfo_080f9c90 *Data_03007ff0;
extern const u8 Data_080fba14[];

void Func_080fab7c(struct Player_080f9c90 *player);
void Func_080fa678(struct Channel_080f9c90 *channel);
void Func_080fa68c(struct Track_080f9c90 *track);
void Func_080fac44(struct Player_080f9c90 *player,
                   struct Track_080f9c90 *track);
void Func_080f9f3c(struct Channel_080f9c90 *channel,
                   struct Track_080f9c90 *track);
u32 Func_080fa1fc(void *wave, u32 key, u8 pitch);

void Func_080f9c90(struct Player_080f9c90 *player)
{
    struct SoundInfo_080f9c90 *sound_info;
    struct Track_080f9c90 *track;
    struct Channel_080f9c90 *channel;
    u32 active_mask;
    u32 track_bit;
    s32 remaining;

    if (player->lock != 0x68736d53) {
        return;
    }
    player->lock = 0x68736d53 + 1;

    if (player->callback != 0) {
        player->callback(player->callback_argument);
    }

    if ((s32)player->status >= 0) {
        sound_info = Data_03007ff0;
        Func_080fab7c(player);
        if ((s32)player->status >= 0) {
            player->tempo_counter += player->tempo_increment;
            while (player->tempo_counter >= 150) {
                remaining = player->track_count;
                track = player->tracks;
                track_bit = 1;
                active_mask = 0;

                for (; remaining > 0;
                     remaining--, track++, track_bit <<= 1) {
                    if ((track->flags & 0x80) == 0) {
                        continue;
                    }
                    active_mask |= track_bit;

                    for (channel = track->chain; channel != 0;
                         channel = channel->next) {
                        if ((channel->status & 0xc7) != 0) {
                            if (channel->gate_time != 0) {
                                channel->gate_time -= 1;
                                if (channel->gate_time == 0) {
                                    channel->status |= 0x40;
                                }
                            }
                        } else {
                            Func_080fa678(channel);
                        }
                    }

                    if ((track->flags & 0x40) != 0) {
                        Func_080fa68c(track);
                        track->flags = 0x80;
                        track->unknown_0f = 2;
                        track->unknown_13 = 64;
                        track->lfo_speed = 22;
                        track->voice_type = 1;
                    }

                    for (;;) {
                        if (track->wait == 0) {
                            u8 *cursor = track->command;
                            u32 command = cursor[0];

                            if (command < 0x80) {
                                command = track->last_command;
                            } else {
                                track->command = cursor + 1;
                                if (command >= 0xbd) {
                                    track->last_command = command;
                                }
                            }
                            if (command >= 0xcf) {
                                sound_info->extended_command(
                                    command - 0xcf, player, track);
                                continue;
                            }
                            if (command > 0xb0) {
                                player->command = command - 0xb1;
                                sound_info->command_table[
                                    command - 0xb1](player, track);
                                if (track->flags == 0) {
                                    break;
                                }
                                continue;
                            }
                            track->wait = Data_080fba14[command - 0x80];
                            continue;
                        }

                        track->wait -= 1;
                        if (track->lfo_speed != 0 &&
                            track->mod_depth != 0) {
                            if (track->lfo_delay_counter != 0) {
                                track->lfo_delay_counter -= 1;
                            } else {
                                s32 counter;
                                s32 value;

                                track->lfo_counter += track->lfo_speed;
                                counter = track->lfo_counter;
                                if ((s8)(counter - 64) < 0) {
                                    value = (s8)counter;
                                } else {
                                    value = 128 - counter;
                                }
                                value = (track->mod_depth * value) >> 6;
                                if ((u8)(value ^ track->mod_calculated)
                                    != 0) {
                                    track->mod_calculated = value;
                                    track->flags |=
                                        track->mod_type == 0 ? 0x0c
                                                             : 0x03;
                                }
                            }
                        }
                        break;
                    }
                }

                player->clock += 1;
                if (active_mask == 0) {
                    player->status = 0x80000000;
                    goto unlock;
                }
                player->status = active_mask;
                player->tempo_counter -= 150;
            }

            remaining = player->track_count;
            track = player->tracks;
            for (; remaining > 0; remaining--, track++) {
                if ((track->flags & 0x80) == 0 ||
                    (track->flags & 0x0f) == 0) {
                    continue;
                }
                Func_080fac44(player, track);
                for (channel = track->chain; channel != 0;
                     channel = channel->next) {
                    u32 type;

                    if ((channel->status & 0xc7) == 0) {
                        Func_080fa678(channel);
                        continue;
                    }
                    type = channel->type & 7;
                    if ((track->flags & 0x03) != 0) {
                        Func_080f9f3c(channel, track);
                        if (type != 0) {
                            channel->refresh_1d |= 1;
                        }
                    }
                    if ((track->flags & 0x0c) != 0) {
                        s32 key = channel->key + track->key_shift;

                        if (key < 0) {
                            key = 0;
                        }
                        if (type != 0) {
                            channel->frequency =
                                sound_info->cgb_frequency(
                                    type, key, track->pitch);
                            channel->refresh_1d |= 2;
                        } else {
                            channel->frequency = Func_080fa1fc(
                                channel->wave, key, track->pitch);
                        }
                    }
                }
                track->flags &= 0xf0;
            }
        }
    }

unlock:
    player->lock = 0x68736d53;
}
