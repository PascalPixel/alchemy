#include "owner_state.h"
s32 Inventory_AddItem(s32 owner, s32 item);

void Owner_RefreshClassActions(s32);
s32 Owner_GetValueIfLevelThresholdReached(s32, s32);

u32 Owner_GetLevelThreshold(s32 owner, s32 level)
{
    struct OwnerLevelState *state = (struct OwnerLevelState *)UnitGet(owner);

    if (state->enabled != 0) {
        if (level <= 0) {
            return 0;
        }
        if (level <= 99 && state->type <= 7) {
            return Character_LevelExpTable[state->type * 99 + level - 1];
        }
    }
    return (u32)-1;
}
