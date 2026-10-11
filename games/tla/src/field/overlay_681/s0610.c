#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"

struct ObjectRuntime *Object_GetById(s32 id);

s32 Func_02000610(void)
{
    struct FieldSprite *sprite = Object_GetById(9)->animation;

    sprite->rotation -= 0x300;
    return 0;
}
