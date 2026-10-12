/* Near miss: differ 2 of 39 (one reordered row): the native reads the timer
 * between the two halves of the multiply by five; the draft reads it after.
 * Tried: timer + random + 40 written out. */
#include "../../../../games/tla/src/field/overlay_6ae/ov6ae.h"

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
        *timer += (((u32)Random16Far() * 80) >> 16) + 40;
        object->scale_x = 0x12000;
        object->scale_y = 0x11000;
    }
}
