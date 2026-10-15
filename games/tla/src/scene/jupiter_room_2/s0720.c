#include "types.h"
#include "iwram_call.h"
#include "object_runtime.h"
#include "field_sprite.h"

s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_020021b4(struct ObjectRuntime *object, s32 a);

// A particle's step: it circles at its angle (action) around x 214 and
// rises with its phase (unknown_66), drawn in front while the sine is
// negative and behind otherwise.
void Func_02000720(struct ObjectRuntime *object)
{
    s16 angle = object->action;
    s16 height = object->unknown_66;
    struct FieldSprite *sprite;
    s32 sine;
    s32 y;

    object->x = Iwram_MulQ16(Math_Cosine(angle), object->speed_limit) + (214 << 16);
    object->y = (height * 3 << 16) + 0x80000;
    object->z = 170 << 18;
    if ((s16)object->unknown_66 == 0)
        Func_020021b4(object, 7);
    /* FAKEMATCH: the one-pass block loads the height before the limit is built, as the native does (one reordered row without it). */
    do {
        y = object->y;
    } while (0);
    if (y > 170 << 17)
        object->unknown_66 = 0xffff;
    sine = Math_Sine(angle);
    sprite = object->animation;
    if (sine < 0)
        sprite->priority = 1;
    else
        sprite->priority = 2;
    object->action += object->acceleration;
    object->unknown_66++;
}
