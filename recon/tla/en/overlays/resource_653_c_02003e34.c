/* Near miss, differ 12 of 20, registers only: the native keeps the frame
 * count in r6 and the sprite in r7; the draft swaps them. Locals copying
 * the parameters and a while form were tried. */
#include "../../../../games/tla/src/field/overlay_653/local.h"

void Func_02003e34(struct FieldSprite *sprite, u32 frames, s32 step)
{
    u32 i;

    for (i = 0; i < frames; i++) {
        sprite->rotation += step;
        Battle_WaitMode0(1);
    }
}

