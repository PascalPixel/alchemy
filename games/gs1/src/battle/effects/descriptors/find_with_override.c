#include "types.h"
#include "scene.h"
#include "abi/battle/effects/descriptors/find_with_override.h"

struct State_0808d5a4 {
    u8 padding[0x24a];
    s16 value;
};

extern u8 gCell;
struct EffectDescriptor *Battle_Run(s32, s32);

s32 BattleFx_FindDescriptorWithOverride(s32 arg0)
{
    s32 result = Battle_Run(0, arg0);
    s32 value = ((struct State_0808d5a4 *)&gCell)->value;

    if (value == arg0) {
        s32 next = Battle_Run(7, value);

        if (next != 0) {
            return next;
        }
    }
    return result;
}
