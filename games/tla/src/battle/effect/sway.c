#include "types.h"
#include "iwram_call.h"

struct SwayObject {
    u8 unknown_00[8];
    s32 y;
    u8 unknown_0c[44];
    s32 base_y;
    u8 unknown_3c[40];
    s16 phase;
};

s32 Trig_Sin(s32 angle);

void Func_080de818(struct SwayObject *object)
{
    s16 *phase = &object->phase;
    s32 amplitude = 0x40000;

    object->y = object->base_y + Iwram_MulQ16(amplitude, Trig_Sin(*phase << 9));
    (*phase)++;
    *phase = (*phase + 128) % 128;
}
