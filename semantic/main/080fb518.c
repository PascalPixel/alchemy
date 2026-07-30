#include "types.h"

struct MusicPlayerState_080fb518 {
    u8 unknown_00[0x18];
    u8 *memory_area;
};

struct MusicTrackState_080fb518 {
    u8 unknown_00[0x40];
    u8 *command;
};

void Func_080072ec(
    struct MusicPlayerState_080fb518 *player,
    struct MusicTrackState_080fb518 *track,
    s32 handler);

/*
 * Execute an M4A MEMACC command.  The first operand selects an assignment or
 * comparison, the second addresses the player's sixteen-byte scratch area,
 * and the third is either an immediate value or another scratch-area index.
 */
void Func_080fb518(
    struct MusicPlayerState_080fb518 *player,
    struct MusicTrackState_080fb518 *track)
{
    u32 operation;
    u8 *address;
    u8 value;
    s32 condition;

    operation = *track->command++;
    address = player->memory_area + *track->command++;
    value = *track->command++;

    switch (operation) {
    case 0:
        *address = value;
        return;
    case 1:
        *address += value;
        return;
    case 2:
        *address -= value;
        return;
    case 3:
        *address = player->memory_area[value];
        return;
    case 4:
        *address += player->memory_area[value];
        return;
    case 5:
        *address -= player->memory_area[value];
        return;
    case 6:
        condition = *address == value;
        break;
    case 7:
        condition = *address != value;
        break;
    case 8:
        condition = *address > value;
        break;
    case 9:
        condition = *address >= value;
        break;
    case 10:
        condition = *address <= value;
        break;
    case 11:
        condition = *address < value;
        break;
    case 12:
        condition = *address == player->memory_area[value];
        break;
    case 13:
        condition = *address != player->memory_area[value];
        break;
    case 14:
        condition = *address > player->memory_area[value];
        break;
    case 15:
        condition = *address >= player->memory_area[value];
        break;
    case 16:
        condition = *address <= player->memory_area[value];
        break;
    case 17:
        condition = *address < player->memory_area[value];
        break;
    default:
        return;
    }

    if (condition) {
        Func_080072ec(player, track, *(s32 *)0x02004004);
    } else {
        track->command += 4;
    }
}
