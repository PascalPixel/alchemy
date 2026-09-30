#include "TYPES.H"
#include "SCENE.H"
#include "OWNER_STATE.H"
#include "PARTY_STATE.H"
#include "RUNTIME_INTERFACES.H"

s32 GameState_InitDefaults();
s32 Game_ResetForNewGameFar(s32);

/* trade/get_offer_state.c */
void *Owner_GetState();

/* party/get_average_level.c */
u32 Math_Div(s32, s32);

struct OwnerState {
    u8 bytes[0x14c];
};

struct GameStateOwners {
    u8 unknown_000[0x2c0];
    struct OwnerState owners[8];
};

extern struct OwnerState *gBattleOwnerStates;
extern const u8 Data_08080ec8[];

/* owner/refresh_and_reset_zero.c */
void Owner_RefreshAndResetZero(void)
{
    GameState_InitDefaults();
    Game_ResetForNewGameFar(0);
}

s32 Trade_GetOfferState(s32 arg0)
{
    if (arg0 != 0) {
        return Owner_GetState(0x83);
    }
    return (s32)&gGameState.unknown_000[0xc];
}

u32 Party_GetAverageLevel(void)
{
    s32 count;
    s32 total;
    s32 i;

    total = 0;
    count = Party_CountActiveOwners();
    if (count == 0) {
        return 0;
    }
    for (i = 0; i < count; i++) {
        total += ((u8 *)Owner_GetState(
            gGameState.active_owners[i]))[15];
    }
    total = Math_Div(total, count);
    return total;
}

void *Owner_GetState(u32 owner)
{
    struct OwnerState *states;
    register u32 offset asm("r3"); /* FAKEMATCH: the scaled index in r3 */

    asm volatile("mov r3, lr" ::: "r3"); /* FAKEMATCH: the entry copy of lr */
    states = (*(struct GameStateOwners *)&gGameState).owners;
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

const u8 *Owner_GetRecord(s32 selector)
{
    u32 record_index;

    record_index = selector - 8;
    if (record_index > 0xF9U) {
        record_index = 0;
    }
    return Data_08080ec8 + record_index * 0x54;
}
