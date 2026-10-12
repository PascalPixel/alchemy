#include "object_runtime.h"
#include "objdisp.h"
#include "party_state.h"

void ObjectMotion_CenterOnColumn(u32 object_id, s32 action, s32 z_offset)
{
    struct ObjectRuntime *object;
    s16 current_angle;
    s32 snapped_angle;
    s16 angle_remainder;
    s32 x;
    s32 eight;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        current_angle = *(s16 *)((u8 *)object + 0x0a);
        snapped_angle = current_angle;
        if (current_angle < 0) {
            snapped_angle += 15;
        }
        snapped_angle >>= 4;
        snapped_angle *= 16;
        angle_remainder = current_angle - snapped_angle;
        object->movement_state = 0;
        Object_ResetMotion(object);
        Object_SetMode(object, 2);
        x = object->x;
        eight = 8;
        /* FAKEMATCH: under the ARM9TDMI load schedule the column offset's 8 is built before the x load; the barrier loads x first with neither value changed. */
        __asm__("" : "+r"(eight) : "r"(x));
        Object_SetPosition(object,
            x + ((eight - angle_remainder) << 16),
            object->y, object->z);
        Object_CommitPosition(object);
        ObjectMotion_SetActionVariant(object_id, action);
        Object_SetPosition(object, object->x, object->y,
            object->z + (z_offset << 16));
    }
}

void ObjectMotion_OffsetPositionAndResetMotion(u32 object_id, s32 x_offset, s32 z_offset)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        Object_ResetMotion(object);
        Object_SetPosition(object, object->x + (x_offset << 16),
            object->y, object->z + (z_offset << 16));
    }
}

void ObjectMotion_OffsetPositionAndReset(u32 object_id, s32 x_offset, s32 z_offset)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)Object_OffsetMotionScript);
        Object_SetMode(object, 2);
        Object_SetPosition(object, object->x + (x_offset << 16),
            object->y, object->z + (z_offset << 16));
    }
}

void ObjectMotion_CommitPositionAndActivate(u32 object_id, s32 x_offset, s32 z_offset)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    ObjectMotion_OffsetPositionAndReset(object_id, x_offset, z_offset);
    if (object != NULL) {
        Object_CommitPosition(object);
        Object_SetMode(object, 1);
    }
}

void ObjectMotion_SetSpeedParameters(u32 object_id, s32 speed_limit, s32 acceleration);
void ObjectMotion_MoveToObject(u32 object_id, s32 target_id);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
extern const s32 Data_080f32e0[];

void Func_080d3070(u32 object_id, s32 x_offset, s32 z_offset, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        ObjectMotion_SetSpeedParameters(object_id, 0xcccc, 0x6666);
        ObjectMotion_MoveToObject(object_id, gPartyState.current_owner);
        Object_SetPositionAndResetMotionFar(object, object->x, object->y, object->z - 0x10000);
        object->movement_state = 0;
        Object_SetMode(object, 2);
        ObjectMotion_OffsetPositionAndResetMotion(object_id, x_offset, z_offset);
        ObjectDispatch_InitializeFar((struct DispatchObject *)object, Data_080f32e0);
        object->action = action;
    }
}
