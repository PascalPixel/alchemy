#include "types.h"
#include "item.h"

struct State_0807842c {
    u8 padding[296];
    volatile u8 shift;
};

struct State_0807842c *Func_08077394(s32);

s32 Func_0807842c(s32 owner, s32 index)
{
    struct State_0807842c *state = Func_08077394(owner);
    struct ItemDefinition *item = Func_08078414(index);
    u32 amount = state->shift;
    s32 result = item->properties;

    if (amount > 7) {
        result = 0;
    } else {
        result >>= state->shift;
        result &= 1;
    }
    return result;
}
