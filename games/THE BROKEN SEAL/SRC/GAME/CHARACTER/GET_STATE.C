/* Characters: return the state record for a party owner (0-7) or a battle owner (0x80-0x85), or NULL. */
#include "TYPES.H"

struct OwnerState {
    u8 bytes[0x14c];
};

struct GameStateOwners {
    u8 unknown_000[0x2c0];
    struct OwnerState owners[8];
};

extern struct GameStateOwners gGameState;
extern struct OwnerState *gBattleOwnerStates;

void *Owner_GetState(u32 owner)
{
    struct OwnerState *states;
    register u32 offset asm("r3"); /* FAKEMATCH: the scaled index in r3 */

    asm volatile("mov r3, lr" ::: "r3"); /* FAKEMATCH: the entry copy of lr */
    states = gGameState.owners;
    if (owner < 8)
        {
        register u8 *r asm("r0"); /* FAKEMATCH: the result in r0 */
        offset = 332;
        offset *= owner;
        r = (u8 *)states + offset;
        return r;
        }
    if (owner - 0x80 < 6 && gBattleOwnerStates != NULL)
        {
        register u8 *r asm("r0"); /* FAKEMATCH: the result in r0 */
        u8 *t;
        offset = 332;
        offset *= owner;
        t = (u8 *)gBattleOwnerStates + offset;
        r = t - 0xa600;
        return r;
        }
    return NULL;
}
