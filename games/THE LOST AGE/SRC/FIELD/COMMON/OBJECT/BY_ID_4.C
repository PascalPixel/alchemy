#include "OBJECT_RUNTIME.H"
#include "FIELD_EVENT.H"

/* Centres an object on its 16-pixel column, plays an action, then moves
   it by z_offset; offset 0x0a is the whole-pixel half of x. */
void ObjectMotion_CenterOnColumn(u32 object_id, s32 action, s32 z_offset)
{
    struct ObjectRuntime *object;
    s16 pixel_x;
    s32 cell_x;
    s16 offset;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        pixel_x = *(s16 *)((u8 *)object + 0x0a);
        cell_x = pixel_x;
        if (pixel_x < 0) {
            cell_x += 15;
        }
        cell_x >>= 4;
        cell_x *= 16;
        offset = pixel_x - cell_x;
        object->movement_state = 0;
        Object_ResetMotion(object);
        Object_SetMode(object, 2);
        Object_SetPosition(object,
            object->x + ((8 - offset) << 16),
            object->y, object->z);
        Object_CommitPosition(object);
        ObjectMotion_SetActionVariant(object_id, action);
        Object_SetPosition(object, object->x, object->y,
            object->z + (z_offset << 16));
    }
}
