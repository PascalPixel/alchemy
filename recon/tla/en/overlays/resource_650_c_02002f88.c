/* Near miss, differ 6: three loads are scheduled one instruction away from
 * the listing (the pooled zero, object->x and the sprite pointer). */
#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);

struct ActorSprite {
    u8 unknown_00[18];
    u16 rotation;
};

void Func_02002f88(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    ObjectDispatch_SetSingleChildFlags(Object_GetById(id), 0);
    object->angle = 0x4000;
    object->motion_flags = 0;
    object->x += 0xc0000;
    object->y = 0x100000;
    object->collision_flags = 0;
    ((struct ActorSprite *)object->animation)->rotation = 0xc000;
}
