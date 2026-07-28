#include "m7_interfaces.h"

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);

void Func_080a1c6c(struct Object080a1c **slot, s32 index,
    s32 origin_x, s32 origin_y,
    s32 phase) {
    struct Object080a1c *object;
    s32 wave;
    s32 stable_phase;
    s32 wrapped_index;

    wrapped_index = index;
    if (wrapped_index > 0xF) {
        wrapped_index = 0;
    }
    stable_phase = *(volatile s32 *)&phase;
    object = *(struct Object080a1c * volatile *)slot;
    wave = Func_080022ec(wrapped_index, stable_phase);
    object->y = (s16)((wave * 0x10) + origin_y);
    object->x =
        (s16)((Func_080022fc(wrapped_index, phase) * 0x18) + origin_x);
    Func_080a17c4(object);
}
