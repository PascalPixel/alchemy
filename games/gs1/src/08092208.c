#include "object_runtime.h"

void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetMode(struct ObjectRuntime *, s32);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);
void Object_CommitPosition(struct ObjectRuntime *);
void Func_08092b08(u32 object_id, s32 action);

void Func_08092208(u32 object_id, s32 action, s32 z_offset)
{
    struct ObjectRuntime *object;
    s16 angle;
    s32 adjusted;
    s16 remainder;

    object = GetObject(object_id);
    if (object != NULL) {
        angle = *(s16 *)((u8 *)object + 0x0a);
        adjusted = angle;
        if (angle < 0) {
            adjusted += 15;
        }
        adjusted >>= 4;
        adjusted *= 16;
        remainder = angle - adjusted;
        object->movement_state = 0;
        Object_ResetMotion(object);
        Object_SetMode(object, 2);
        Object_SetPosition(object,
            object->x + ((8 - remainder) << 16),
            object->y, object->z);
        Object_CommitPosition(object);
        Func_08092b08(object_id, action);
        Object_SetPosition(object, object->x, object->y,
            object->z + (z_offset << 16));
    }
}
