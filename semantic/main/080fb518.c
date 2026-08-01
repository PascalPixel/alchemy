/*
 * Correctness fix, veneer audit (mars, 2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320.  A `bl` into that range
 * is an indirect call through the named register.
 *
 * The regularity that makes these readable, and it is CHECKED per site rather
 * than assumed: the callee value always appeared in the draft's argument list
 * at exactly the position matching the veneer's register index.  The compiler
 * loaded the callee into rN and the draft read rN as argument N.  So a
 * __call_via_r0 site has ZERO real arguments, r1 has one, r2 has two.  Each
 * site below was confirmed by checking that the register and the argument
 * position agree.
 *
 * The value at 0x02004004 is the callee, reached through __call_via_r2, so
 * the call takes TWO arguments. Confirmed at 0x080fb658: r0 and r1 are set
 * from r4 and r6 immediately before the branch, and r2 holds the load.
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
        ((Callee_02004004)*(s32 *)0x02004004)(player, track);
    } else {
        track->command += 4;
    }
}
