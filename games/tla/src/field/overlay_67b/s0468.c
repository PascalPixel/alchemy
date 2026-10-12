#include "local.h"

void Func_02000468(struct ObjectRuntime *object)
{
    struct FieldSprite *sprite;
    s32 angle;

    /* FAKEMATCH: the one-pass block loads the angle and the sprite before the quarter turn is built (differ 2 without it). */
    do {
        angle = object->angle;
    } while (0);
    do {
        sprite = object->animation;
    } while (0);
    sprite->rotation = angle + 0x4000;
}
