/* NONMATCHING: 16 halfwords. The reference opens with a dead "mov r3, lr" and
 * loads the 0x02000500 base before the first test; the single-return form
 * kept its register order closer than early returns (27 halfwords).
 * 2026-09-29 slice 4: the owner records are gGameState's array at +0x2c0
 * (0x02000500 is gGameState + 0x2c0), and with the base taken from it once
 * and early returns the draft scores 210 (5 register-only, 1 operand, 1
 * reordered, 1 deleted) against the single-return form's 480; alchemy
 * permute (3 jobs, 10 minutes) found nothing below either. Remaining: each
 * multiply writes the owner's register instead of the 0x14c constant's, the
 * remote sum adds in the other order, and the dead mov r3, lr, which only a
 * return-address read whose use is deleted late (or an asm) leaves behind;
 * __builtin_return_address with no use is deleted outright. 0x03001f28
 * needs a label in the IWRAM symbols.
 */
#include "TYPES.H"

struct OwnerState {
    u8 bytes[0x14c];
};

struct GameStateOwners {
    u8 unknown_000[0x2c0];
    struct OwnerState owners[8];
};

extern struct GameStateOwners gGameState;
extern struct OwnerState *Data_03001f28;

void *Owner_GetState(u32 owner)
{
    struct OwnerState *states = gGameState.owners;

    if (owner < 8)
        return &states[owner];
    if (owner - 0x80 < 6 && Data_03001f28 != NULL)
        return &Data_03001f28[owner - 0x80];
    return NULL;
}
