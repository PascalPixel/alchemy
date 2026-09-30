#include "TYPES.H"
#include "SCENE.H"
#include "OWNER_STATE.H"
#include "PARTY_STATE.H"
s32 GameState_InitDefaults();
s32 Game_ResetForNewGameFar(s32);

/* owner/refresh_and_reset_zero.c */

s32 Trade_GetOfferState(s32 arg0)
{
    if (arg0 != 0) {
        return Owner_GetState(0x83);
    }
    return (s32)&gGameState.unknown_000[0xc];
}
