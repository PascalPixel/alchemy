#include "types.h"
#include "ram_buffer.h"

struct EffectSceneState {
    u8 unknown_000[0xd31];
    u8 value_d31;
    u8 value_d32;
    u8 value_d33;
};

struct EffectSceneWork {
    u8 unknown_00[0x20];
    u8 failed;
};

s32 Func_080d9a74(s32 value, s32 *first, s32 *second);
void Func_080e09c0(void);

void Func_080e0978(s32 value)
{
    struct EffectSceneState *state = Ram_HeapSlots->event_work;
    struct EffectSceneWork *work = Ram_HeapSlots->effect_work;
    s32 second;
    s32 first;

    if (Func_080d9a74(value, &first, &second)) {
        work->failed = 1;
    } else {
        state->value_d32 = value;
        /* FAKEMATCH: the ARM9 scheduling would load the first result before storing the value. */
        __asm__("");
        state->value_d33 = first;
        /* FAKEMATCH: and would step the offset back before loading the second result. */
        __asm__("");
        state->value_d31 = second;
    }
    Func_080e09c0();
}
