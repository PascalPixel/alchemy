// Func_020015e8 and Func_02001630 (the listing at 02009630), differ 4 each:
// the listing forms &object->unknown_66 after the store to object->action;
// the draft forms it before the sprite load.
#include "../../../../games/tla/src/field/overlay_67b/local.h"

void Func_020015e8(struct ObjectRuntime *object)
{
    struct FieldSprite *sprite = object->animation;
    s16 *amplitude;

    object->action += 0x2000;
    amplitude = (s16 *)&object->unknown_66;
    *amplitude += 32;
    if (*amplitude > 0x200)
        *amplitude = 0x200;
    sprite->rotation = Iwram_MulQ16(Math_Sine(object->action), *amplitude);
}

void Func_02001630(struct ObjectRuntime *object)
{
    struct FieldSprite *sprite = object->animation;
    s16 *amplitude;

    object->action += 0x2000;
    amplitude = (s16 *)&object->unknown_66;
    *amplitude -= 16;
    if (*amplitude < 0)
        *amplitude = 0;
    sprite->rotation = Iwram_MulQ16(Math_Sine(object->action), *amplitude);
}
