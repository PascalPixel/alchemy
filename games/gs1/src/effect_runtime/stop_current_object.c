#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 WaitFrames(s32);
s32 Func_08009240(void *, s32);

struct State_08097174 {
    u8 unknown_00[0x10];
    u8 *object;
};

void EffectRuntime_StopCurrentObject(void) {
    struct State_08097174 *state = *(void **)ADDR_03001F30;
    u8 *object = state->object;

    FIELD_AT_OFFSET(object, s32 *, 0x6C) = 0;
    Func_08009240(object, 0);
    WaitFrames(1);
}
