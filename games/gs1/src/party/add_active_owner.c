#include "scene.h"
#include "game_flags.h"
#include "party_state.h"

s32 Party_AddActiveOwner(s32 value)
{
    s32 count = Party_CountActiveOwners();
    s32 index;

    GameFlag_Set(value);
    index = 0;
    while (index < count) {
        if (gCell.active_owners[index] == value) {
            return count;
        }
        index++;
    }
    gCell.active_owners[index] = value;
    return count + 1;
}
