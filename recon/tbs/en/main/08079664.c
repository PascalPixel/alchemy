/* 2026-09-24: 8 differing halfwords, register swap only: the reference keeps
   value in r5 and count in r6; this C (like the matching Party_AddActiveOwner)
   gives value r6. Local order, register hints and loop spellings do not move it. */
#include "GAME_FLAGS.H"
#include "PARTY_STATE.H"

void GameFlag_ClearBit(s32);

s32 Party_RemoveActiveOwner(s32 value)
{
    s32 count = Party_CountActiveOwners();
    s32 index;

    GameFlag_ClearBit(value);
    for (index = 0; index < count; index++) {
        if (PARTY_STATE.active_owners[index] == value)
            break;
    }
    for (; index < count - 1; index++)
        PARTY_STATE.active_owners[index] = PARTY_STATE.active_owners[index + 1];
    return Party_CountActiveOwners();
}
