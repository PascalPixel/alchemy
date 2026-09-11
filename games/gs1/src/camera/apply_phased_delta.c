#include "types.h"
#include "scene.h"
#include "abi/camera/apply_phased_delta.h"

struct Actor {
    u8 unknown[54];
    u16 value;
};

extern u32 gIw;

void Camera_ApplyPhasedDelta(void)
{
    u8 *state = (u8 *)gIw;
    struct Actor *actor = *(struct Actor **)((u8 *)&gIw - 108);
    volatile u32 *phase = (u32 *)(state + 0x77B0);

    if (*phase == 1) {
        actor->value += *(s32 *)(state + 0x77AC);
        *phase = 0;
    } else {
        actor->value += *(s32 *)(state + 0x77AC) / 2;
        if (*phase == 2)
            *phase = 0;
        else
            *phase = 2;
    }
}
