#include "types.h"
#include "scene.h"
#include "audio_engine.h"

/* audio/music/track/clear_modulation.c */
struct MusicTrackChannelState {
    u8 flags;
    u8 unknown01[21];
    u8 pending22;
    u8 unknown17;
    u8 mode24;
    u8 unknown19;
    u8 pending26;
};

void MusicTrack_ClearModulation(struct MusicTrackChannelState *channel)
{
    channel->pending26 = 0;
    channel->pending22 = 0;
    if (channel->mode24 == 0)
        channel->flags |= 12;
    else
        channel->flags |= 3;
}

/* audio/music/player/set_modulation_depth.c */
void MusicTrack_ClearModulation(u8 *entry);

void MusicPlayer_SetModulationDepth(u8 *player, s32 mask_arg, s32 value_arg)
{
    register s32 mask = (u16)mask_arg;
    register s32 value = (u8)value_arg;
    register s32 count;
    register u8 *entry;
    register u32 bit;

    if (*(u32 *)(player + 0x34) != 0x68736d53)
        return;

    *(u32 *)(player + 0x34) += 1;
    count = player[8];
    entry = *(u8 **)(player + 0x2c);
    bit = 1;
    if (count > 0) {
        register s32 check = value;
        do {
            if ((mask & bit) && (entry[0] & 0x80)) {
                entry[0x17] = value;
                if (check == 0)
                    MusicTrack_ClearModulation(entry);
            }
            count--;
            entry += 0x50;
            bit <<= 1;
        } while (count > 0);
    }
    *(u32 *)(player + 0x34) = 0x68736d53;
}

/* audio/music/player/set_lfo_speed.c */

void MusicPlayer_SetLfoSpeed(u8 *object, u32 selected, u32 value)
{
    u16 selected_bits = selected;
    u8 stored_value = value;

    if (*(u32 *)(object + 52) == 0x68736D53) {
        s32 count;
        u8 *entry;
        u32 mask;

        *(u32 *)(object + 52) = *(u32 *)(object + 52) + 1;
        count = object[8];
        entry = *(u8 **)(object + 44);
        mask = 1;

        if (count > 0) {
            u8 test_value = stored_value;

            do {
                if ((selected_bits & mask) != 0 && (entry[0] & 0x80) != 0) {
                    entry[25] = stored_value;
                    if (test_value == 0) {
                        MusicTrack_ClearModulation(entry);
                    }
                }
                count--;
                entry += 80;
                mask <<= 1;
            } while (count > 0);
        }

        *(u32 *)(object + 52) = 0x68736D53;
    }
}

/* audio/misc/execute_memory_access_command.c */
/*
 * Execute an M4A memory-access command.
 *
 * Clean-room reconstruction from games/gs1/asm/080fb518.s and the current repository's
 * veneer map.  The callee at 0x02004004 is reached through __call_via_r2, so
 * r0 and r1 are the two real arguments.
 */

typedef void (*MemoryAccessHandler)(void *player, void *track);

void MusicPlayer_ExecuteMemoryAccessCommand(
    struct MusicPlayerState *player,
    struct MusicTrackState *track)
{
    u32 operation;
    u32 address;
    u32 value;

    operation = *(u8 *)track->command;
    track->command++;
    address = (u32)player->memory_area + *(u8 *)track->command;
    track->command++;
    value = *(u8 *)track->command;
    track->command++;

    switch (operation) {
    case 0:
        *(u8 *)address = value;
        return;
    case 1:
        *(u8 *)address += value;
        return;
    case 2:
        *(u8 *)address -= value;
        return;
    case 3:
        *(u8 *)address = player->memory_area[value];
        return;
    case 4:
        *(u8 *)address += player->memory_area[value];
        return;
    case 5:
        *(u8 *)address -= player->memory_area[value];
        return;
    case 6:
        if (*(u8 *)address == value)
            goto call_handler;
        goto advance;
    case 7:
        if (*(u8 *)address != value)
            goto call_handler;
        goto advance;
    case 8:
        if (*(u8 *)address > value)
            goto call_handler;
        goto advance;
    case 9:
        if (*(u8 *)address >= value)
            goto call_handler;
        goto advance;
    case 10:
        if (*(u8 *)address <= value)
            goto call_handler;
        goto advance;
    case 11:
        if (*(u8 *)address < value)
            goto call_handler;
        goto advance;
    case 12:
        if (*(u8 *)address == player->memory_area[value])
            goto call_handler;
        goto advance;
    case 13:
        if (*(u8 *)address != player->memory_area[value])
            goto call_handler;
        goto advance;
    case 14:
        if (*(u8 *)address > player->memory_area[value])
            goto call_handler;
        goto advance;
    case 15:
        if (*(u8 *)address >= player->memory_area[value])
            goto call_handler;
        goto advance;
    case 16:
        if (*(u8 *)address <= player->memory_area[value])
            goto call_handler;
        goto advance;
    case 17:
        if (*(u8 *)address < player->memory_area[value])
            goto call_handler;
        goto advance;
    default:
        goto done;
    }

call_handler:
    ((MemoryAccessHandler)*(s32 *)0x02004004)(player, track);
    goto done;
advance:
    track->command += 4;
done:
    return;
}

/* audio/music/track/dispatch_stream_command.c */
/* Handler per stream command byte, indexed by the byte itself. */
extern s32 gRom[];

/*
 * Take the next byte of the track's command stream, advance the read
 * pointer past it, and hand the matching handler to the runtime caller.
 */
void MusicTrack_DispatchStreamCommand(s32 player, s32 track)
{
    s32 command;

    command = *(u8 *)(*(s32 *)(track + 64));
    *(s32 *)(track + 64) += 1;
    Audio_Run(player, track, gRom[command]);
}
