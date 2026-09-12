#include "types.h"

struct OwnerLevelState {
    u8 unknown[0x128];
    u8 type;
    u8 enabled;
};

void *Owner_GetState(s32);
extern u32 Data_0807a830[];

void Owner_LevelNoOp(void)
{
}

u32 Owner_GetLevelThreshold(s32 owner, s32 level)
{
    struct OwnerLevelState *state = (struct OwnerLevelState *)Owner_GetState(owner);

    if (state->enabled != 0) {
        if (level <= 0) {
            return 0;
        }
        if (level <= 99 && state->type <= 7) {
            return Data_0807a830[state->type * 99 + level - 1];
        }
    }
    return (u32)-1;
}
