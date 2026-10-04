#include "EVENTWRK.H"
#include "OBJECT_RUNTIME.H"
#include "OBJDISP.H"

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
        object_flags = &object->flags;
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

/* Start one of the three variant motion scripts (128 bytes each, the
   strongest first) on an object. */
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
