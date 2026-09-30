#include "TYPES.H"
#include "SCENE.H"
#include "OWNER_STATE.H"
#include "PARTY_STATE.H"
s32 GameState_InitDefaults();
s32 Game_ResetForNewGameFar(s32);

/* owner/refresh_and_reset_zero.c */

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
