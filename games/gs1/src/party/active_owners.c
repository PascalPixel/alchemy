#include "game_flags.h"
#include "party_state.h"

s32 GameFlag_SetBit(s32);

s32 Party_CountActiveOwners(void)
{
    s32 owner;
    s32 count;

    count = 0;
    owner = 0;
    do {
        if (GameFlag_Test(owner) != 0)
            count++;
        owner++;
    } while (owner <= 7);
    return count;
}

s32 Party_AddActiveOwner(s32 value)
{
    s32 count = Party_CountActiveOwners();
    s32 index;

    GameFlag_SetBit(value);
    index = 0;
    while (index < count) {
        if (Data_02000240.active_owners[index] == value)
            return count;
        index++;
    }
    Data_02000240.active_owners[index] = value;
    return count + 1;
}
