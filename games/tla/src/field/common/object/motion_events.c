#include "eventwrk.h"
#include "object_runtime.h"
#include "objdisp.h"

void Motion_SetModeAndWaitAnimation(u32 object_id, s32 action)
{
    Object_SetModeById(object_id, action);
    ObjectMotion_WaitForAnimationChange(object_id);
}

void ObjectMotion_Launch(u32 object_id, s32 speed, s32 event_id)
{
    struct ObjectRuntime *object;
    u8 *object_flags;
    s32 updated_flags;
    s32 vertical_velocity;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        object_flags = &object->motion_flags;
        updated_flags = *object_flags | 2;
        vertical_velocity = speed << 16;
        *object_flags = updated_flags;
        object->velocity_y = vertical_velocity;
        if (speed > 5) {
            Audio_PlayCue(0x99);
        } else {
            Audio_PlayCue(0x98);
        }
        EventRuntime_Wait(event_id);
    }
}

void ObjectMotion_SetVariantCallback(u32 object_id, s32 variant)
{
    struct ObjectRuntime *object;
    const u8 *scripts;

    object = ObjectTable_Get(object_id);
    if (object != NULL && variant > 0) {
        if (variant > 3)
            variant = 3;
        scripts = Object_VariantMotionScripts;
        ObjectDispatch_InitializeFar((struct DispatchObject *)object,
            (u32)(scripts + ((3 - variant) << 7)));
    }
}

void Motion_SetVarCbAndRefresh(u32 object_id, s32 variant)
{
    ObjectMotion_SetVariantCallback(object_id, variant);
    Object_RefreshSelectorById(object_id);
}

void Object_DriftAndDamp(struct ObjectRuntime *object)
{
    s32 drift_x;
    s32 drift_z;
    s32 pos;

    drift_x = object->speed_limit;
    pos = object->x;
    drift_z = object->acceleration;
    object->x = pos + drift_x;
    object->z += drift_z;
    object->y += 0x400;
    object->speed_limit = drift_x - drift_x / 18;
    object->acceleration = drift_z - drift_z / 16;
}
