/* Draft, differ 2 of 56: the listing loads the phase (ldr r0, [r5, #48]) before it builds 0xc0000; this draft builds the constant first. Tried: the angle or the phase in a local, the radius split in two statements, the operands swapped. */
#include "../../../../games/tla/src/field/overlay_696/ov696.h"
#include "iwram_call.h"

struct Ov696Orbit {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x1c];
    s32 phase;
    u8 unknown_34[0x10];
    s32 center_x;
    s32 rise;
    s32 center_z;
};

s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);

void Func_02000488(struct Ov696Orbit *orbit)
{
    s32 radius = (orbit->y >> 2) + 0xc0000;

    orbit->x = orbit->center_x + Iwram_MulQ16(Math_Cosine((orbit->phase & 255) << 11), radius) * 2;
    orbit->z = orbit->center_z + Iwram_MulQ16(Math_Sine((orbit->phase & 255) << 11), radius);
    orbit->y += orbit->rise;
    orbit->phase++;
    Object_SetPartAttribute((struct Ov696Object *)orbit, (((u32)gFrameCount & 2) >> 1) * 9);
}
