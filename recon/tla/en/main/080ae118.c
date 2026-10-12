/* Draft, differ 2 of 34: for Djinn_AddToOwner the native sets the owner
 * (movs r0, #7) after the other two arguments, right after the flag call
 * returns; this sets it first. Overlay 65F's Func_0200132c and overlay
 * 6AA's Func_02000070 show the same r0-last order after a call. */
#include "types.h"
#include "party_state.h"

void GameFlag_SetBit(s32 flag);
void Djinn_AddToOwner(s32 owner, s32 element, s32 index);
void Djinn_Activate(s32 owner, s32 element, s32 index);
void Func_080ae358(s32 mode);
void UnitCalc(s32 unit);

void Func_080ae118(void)
{
    s32 i;
    s32 count;

    for (i = 10; i < 12; i++) {
        GameFlag_SetBit(i + 67);
        Djinn_AddToOwner(7, 1, i - 1);
        Djinn_Activate(7, 1, i - 1);
    }
    Func_080ae358(0);
    count = Party_CountActiveOwners();
    for (i = 0; i < count; i++)
        UnitCalc(gPartyState.active_owners[i]);
}
