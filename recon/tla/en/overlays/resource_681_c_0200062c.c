/* Near miss (7 instructions differ, order only): the native loads the
 * 0xfffff800 literal before it shifts the first random value, and stores
 * scale_y before it builds 0xfff. */
#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"

extern const u8 Data_020050d0[];

u32 Random16Far(void);
struct ObjectRuntime *Object_GetById(s32 id);
struct ObjectRuntime *Engine_ObjectCreate(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Func_02004dac(struct ObjectRuntime *object, const u8 *script);

void Func_0200062c(struct ObjectRuntime *object)
{
    object->scale_x = object->scale_x + ((Random16Far() << 12) >> 16) - 0x800;
    object->scale_y = object->scale_y + ((Random16Far() << 12) >> 16) - 0x800;
    if (object->scale_x < 0x1000)
        object->scale_x = 0x2000;
    if (object->scale_y < 0x1000)
        object->scale_y = 0x2000;
}
