// differ 2: the listing loads the angle and the sprite before it starts
// building 0x4000 (ldrh, ldr, movs, lsls); the draft starts the constant
// between the two loads. Tried: no sprite local, angle in an s32 local.
#include "../../../../games/tla/src/field/overlay_67b/local.h"

void Func_02000468(struct ObjectRuntime *object)
{
    struct FieldSprite *sprite = object->animation;

    sprite->rotation = object->angle + 0x4000;
}
