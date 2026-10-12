#include "local.h"

void Func_02001670(struct ObjectRuntime *object)
{
    struct FieldSprite *sprite = object->animation;

    object->action += 0x100;
    sprite->rotation = Math_Sine(object->action) / 64 + 0x600;
}
