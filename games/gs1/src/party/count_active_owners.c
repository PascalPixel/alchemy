#include "game_flags.h"
#include "party_state.h"

s32 Party_CountActiveOwners(void)
{
    s32 owner;
    s32 count;

    count = 0;
    owner = 0;
    do {
        if (GameFlag_Test(owner) != 0) {
            count += 1;
        }
        owner += 1;
    } while (owner <= 7);
    return count;
}
