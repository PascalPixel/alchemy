#include "mars_aerie.h"

void Func_02000038(struct ObjectRuntime *object)
{
    u8 *timer = &object->unknown_5c[6];

    if (*timer != 0) {
        (*timer)--;
    } else {
        *timer = (((u32)Random16Far() * 80) >> 16) + 60;
        object->angle = Random16Far();
    }
}

void Func_02000068(struct ObjectRuntime *object)
{
    if ((gFrameCount >> 1) & 1)
        Object_SetPartAttribute(object, 15);
    else
        Object_SetPartAttribute(object, 0);
}

// Shrinks the object toward its normal size, and when its timer runs out
// restarts it at 40 to 119 frames and swells it again.
void Func_0200008c(struct ObjectRuntime *object)
{
    u8 *timer;
    s32 scale = object->scale_x;

    /* FAKEMATCH: a one-pass block loads the scale before the bound is built (one reordered row). */
    do {
    } while (0);
    if (scale > 0x10000) {
        object->scale_x = scale - 0x800;
        object->scale_y -= 0x400;
    }
    timer = &object->unknown_5c[6];
    if (*timer != 0) {
        (*timer)--;
    } else {
        u32 r = Random16Far();
        /* FAKEMATCH: the random number times 4 apart from times 80, and the timer read in a one-pass block, read the timer between the multiply's two halves; as one expression it is read after (differ 2). */
        u32 r4 = r << 2;
        u8 t;

        do {
            t = *timer;
        } while (0);
        *timer = t + (((r4 + r) * 16) >> 16) + 40;
        object->scale_x = 0x12000;
        object->scale_y = 0x11000;
    }
}
