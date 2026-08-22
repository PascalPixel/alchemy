/*
 * Execute an M4A MEMACC command.
 *
 * Clean-room reconstruction from games/gs1/asm/080fb518.s and the current repository's
 * veneer map.  The callee at 0x02004004 is reached through __call_via_r2, so
 * r0 and r1 are the two real arguments.
 */
#include "types.h"

typedef void (*Callee_02004004)(void *player, void *track);

struct MusicPlayerState_080fb518 {
    u8 unknown_00[0x18];
    u8 *memory_area;
};

struct MusicTrackState_080fb518 {
    u8 unknown_00[0x40];
    u8 *command;
};

void Func_080fb518(
    struct MusicPlayerState_080fb518 *player,
    struct MusicTrackState_080fb518 *track)
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
    ((Callee_02004004)*(s32 *)0x02004004)(player, track);
    goto done;
advance:
    track->command += 4;
done:
    return;
}
