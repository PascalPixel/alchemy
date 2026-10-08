#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct FxGroup {
    u8 unknown_00[32];
    struct ObjectRuntime *objects[8];
    u16 values[8];
    s32 count;
};

union FxSprite {
    u16 halves[10];
    u32 words[5];
};

void Func_080e8d80(void)
{
    struct FxGroup *group = (struct FxGroup *)Ram_HeapSlots->battle_fx_work[0];
    s32 i;

    for (i = 0; i < group->count; i++)
        ((union FxSprite *)group->objects[i]->animation)->halves[9] = group->values[i];
}
