/* Pin-free trial (tla-en): listing 28 instructions, draft 28, common 7, differ 42.
 * Earlier register-allocation notes describe the prior pinned form.
 */
#include "owner_state.h"
#include "types.h"

struct OwnerState {
    u8 bytes[0x14c];
};

struct GameStateOwners {
    u8 unknown_000[0x2c0];
    struct OwnerState owners[8];
};

extern struct GameStateOwners gGameState;
extern struct OwnerState *gBattleOwnerStates;

void *UnitGet(s32 owner)
{
    struct OwnerState *states;
    u32 offset; /* Earlier pinned trial: the scaled index in r3 */

    asm volatile("mov r3, lr" ::: "r3"); /* FAKEMATCH: the entry copy of lr */
    states = gGameState.owners;
    if ((u32)owner < 8)
        {
        u8 *r; /* Earlier pinned trial: the result in r0 */
        offset = 332;
        offset *= owner;
        r = (u8 *)states + offset;
        return r;
        }
    if ((u32)owner - 0x80 < 6 && gBattleOwnerStates != NULL)
        {
        u8 *r; /* Earlier pinned trial: the result in r0 */
        u8 *t;
        offset = 332;
        offset *= owner;
        t = (u8 *)gBattleOwnerStates + offset;
        r = t - 0xa600;
        return r;
        }
    return NULL;
}
