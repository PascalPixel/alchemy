#include "types.h"
#include "scene.h"
#include "abi/owner/get_level_threshold.h"

struct State_08079008 {
    u8 padding[0x128];
    u8 type;
    u8 enabled;
};

void *Owner_GetState(s32);
extern u32 gRom[];

u32 Owner_GetLevelThreshold(s32 owner, s32 level)
{
    struct State_08079008 *state = (struct State_08079008 *)Owner_GetState(owner);

    if (state->enabled != 0) {
        if (level <= 0) {
            return 0;
        }
        if (level <= 99 && state->type <= 7) {
            return gRom[state->type * 99 + level - 1];
        }
    }
    return (u32)-1;
}
