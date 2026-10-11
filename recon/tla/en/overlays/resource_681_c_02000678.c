/* Near miss (9 instructions differ, order only): the native loads the
 * source x before it builds 0x80000, and stores both scales before it
 * loads the sprite. */
#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"

extern const u8 Data_020050d0[];

u32 Random16Far(void);
struct ObjectRuntime *Object_GetById(s32 id);
struct ObjectRuntime *Engine_ObjectCreate(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Func_02004dac(struct ObjectRuntime *object, const u8 *script);

void Func_0200062c(struct ObjectRuntime *object);

s32 Func_02000678(void)
{
    struct ObjectRuntime *source = Object_GetById(9);
    struct ObjectRuntime *object = Engine_ObjectCreate(14, source->x + 0x80000, source->y - 0x20000, source->z);
    struct FieldSprite *sprite;

    object->motion_flags = 0;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02004dac(object, Data_020050d0);
    object->scale_x = 0x6666;
    object->scale_y = 0x6666;
    sprite = object->animation;
    sprite->priority = 1;
    object->bounce = 0;
    object->unknown_4c = 0;
    object->update = (void (*)(void))Func_0200062c;
    return 0;
}
