#include "GAME_FLAGS.H"
#include "PARTY_STATE.H"

s32 GameFlag_SetBit(s32);
void GameFlag_ClearBit(s32);

s32 Party_AddActiveOwner(s32 value)
{
    s32 count = Party_CountActiveOwners();
    s32 index;

    GameFlag_SetBit(value);
    index = 0;
    while (index < count) {
        if (gGameState.active_owners[index] == value)
            return count;
        index++;
    }
    gGameState.active_owners[index] = value;
    return count + 1;
}
