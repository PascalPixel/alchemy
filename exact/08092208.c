#include "object_runtime.h"

void Func_08009140(struct ObjectRuntime *);
void Func_08009080(struct ObjectRuntime *, s32);
void Func_08009150(struct ObjectRuntime *, s32, s32, s32);
void Func_08009158(struct ObjectRuntime *);
void Func_08092b08(u32 object_id, s32 action);

void Func_08092208(u32 object_id, s32 action, s32 z_offset)
{
    struct ObjectRuntime *object;
    s16 angle;
    s32 adjusted;
    s16 remainder;

    object = Func_0808ba1c(object_id);
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
        Func_08009140(object);
        Func_08009080(object, 2);
        Func_08009150(object,
            object->x + ((8 - remainder) << 16),
            object->y, object->z);
        Func_08009158(object);
        Func_08092b08(object_id, action);
        Func_08009150(object, object->x, object->y,
            object->z + (z_offset << 16));
    }
}
