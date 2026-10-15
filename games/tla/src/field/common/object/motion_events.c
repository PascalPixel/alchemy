#include "event_work.h"
#include "object_runtime.h"
#include "objdisp.h"

void Motion_SetModeAndWaitAnimation(u32 objectId, s32 action)
{
    Object_SetModeById(objectId, action);
    ObjectMotion_WaitForAnimationChange(objectId);
}

void ObjectMotion_Launch(u32 objectId, s32 speed, s32 event_id)
{
    struct ObjectRuntime *object;
    u8 *object_flags;
    s32 updated_flags;
    s32 vertical_velocity;

    object = ObjectTable_Get(objectId);
    if (object != NULL) {
        object_flags = &object->motionFlags;
        updated_flags = *object_flags | 2;
        vertical_velocity = speed << 16;
        *object_flags = updated_flags;
        object->velocityY = vertical_velocity;
        if (speed > 5) {
            Audio_PlayCue(0x99);
        } else {
            Audio_PlayCue(0x98);
        }
        EventRuntime_Wait(event_id);
    }
}

void ObjectMotion_SetVariantCallback(u32 objectId, s32 variant)
{
    struct ObjectRuntime *object;
    const u8 *scripts;

    object = ObjectTable_Get(objectId);
    if (object != NULL && variant > 0) {
        if (variant > 3)
            variant = 3;
        scripts = Object_VariantMotionScripts;
        ObjectDispatch_InitializeFar((struct DispatchObject *)object,
            (u32)(scripts + ((3 - variant) << 7)));
    }
}

void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant)
{
    ObjectMotion_SetVariantCallback(objectId, variant);
    Object_RefreshSelectorById(objectId);
}

void Object_DriftAndDamp(struct ObjectRuntime *object)
{
    s32 drift_x;
    s32 drift_z;
    s32 pos;

    drift_x = object->speedLimit;
    pos = object->x;
    drift_z = object->acceleration;
    object->x = pos + drift_x;
    object->z += drift_z;
    object->y += 0x400;
    object->speedLimit = drift_x - drift_x / 18;
    object->acceleration = drift_z - drift_z / 16;
}
