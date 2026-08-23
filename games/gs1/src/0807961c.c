#include "game_flags.h"
#include "party_state.h"

s32 Func_0807961c(s32 value) {
    s32 count = Party_CountActiveOwners();
    s32 index;

    GameFlag_Set(value);
    index = 0;
    while (index < count) {
        if (Data_02000240.active_owners[index] == value) {
            return count;
        }
        index++;
    }
    Data_02000240.active_owners[index] = value;
    return count + 1;
}
