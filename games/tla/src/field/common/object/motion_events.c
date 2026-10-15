#include "event_work.h"
#include "object_runtime.h"
#include "objdisp.h"

void Motion_SetModeAndWaitAnimation(u32 objectId, s32 action)
{
    Object_SetModeById(objectId, action);
    ObjectMotion_WaitForAnimationChange(objectId);
}

void ObjectMotion_Launch(u32 objectId, s32 speed, s32 eventId)
{
    struct ObjectRuntime *object;
    u8                   *objectFlags;
    s32                   updatedFlags;
    s32                   verticalVelocity;

    object = ObjectTable_Get(objectId);
    if (object != NULL)
    {
        objectFlags = &object->motionFlags;
        updatedFlags = *objectFlags | 2;
        verticalVelocity = speed << 16;
        *objectFlags = updatedFlags;
        object->velocityY = verticalVelocity;
        if (speed > 5)
        {
            Audio_PlayCue(0x99);
        }
        else
        {
            Audio_PlayCue(0x98);
        }
        EventRuntime_Wait(eventId);
    }
}

void ObjectMotion_SetVariantCallback(u32 objectId, s32 variant)
{
    struct ObjectRuntime *object;
    const u8             *scripts;

    object = ObjectTable_Get(objectId);
    if (object != NULL && variant > 0)
    {
        if (variant > 3)
            variant = 3;
        scripts = gObject_VariantMotionScripts;
        ObjectDispatch_InitializeFar(
            (struct DispatchObject *)object, (u32)(scripts + ((3 - variant) << 7)));
    }
}

void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant)
{
    ObjectMotion_SetVariantCallback(objectId, variant);
    Object_RefreshSelectorById(objectId);
}

void Object_DriftAndDamp(struct ObjectRuntime *object)
{
    s32 driftX;
    s32 driftZ;
    s32 pos;

    driftX = object->speedLimit;
    pos = object->x;
    driftZ = object->acceleration;
    object->x = pos + driftX;
    object->z += driftZ;
    object->y += 0x400;
    object->speedLimit = driftX - driftX / 18;
    object->acceleration = driftZ - driftZ / 16;
}
