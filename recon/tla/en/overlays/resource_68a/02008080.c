/* Near miss, differ 2 of 9: the native loads the sprite before it builds
   the zero (ldr r2, [r0, #80]; movs r3, #0); this draft builds the zero
   first. A struct field, a chained assignment and a zero local compile the
   same; only an empty asm after the load matches, and no device is spent on
   it. */
#include "../../../../../games/tla/src/field/overlay_68a/scene.h"

void Func_02000080(s32 id)
{
    struct FieldActor *object = Object_GetById(id);
    struct FieldSprite *sprite = object->sprite;

    WORD_AT(object, 108) = 0;
    object->timer = 0;
    sprite->rotation = 0;
}
