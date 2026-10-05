#include "PROTO.H"
#include "EVENTWRK.H"
#include "OBJECT_RUNTIME.H"
#include "OBJDISP.H"
#include "SYSTEM.H"

#define ObjectDispatch_ApplyValueToChildrenFar ((void (*)(struct ObjectRuntime*,s32))ObjectDispatch_ApplyValueToChildrenFar)

void ObjectMotion_SetActionVariant(u32, s32);
void ObjectMotion_SetHorizontalPositionWithTerrain(u32, s32, s32);
void Audio_PlayCue(s32);

void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);
void Object_CommitPosition(struct ObjectRuntime *);
void Object_SetMode(struct ObjectRuntime *, s32);

struct ObjectRuntime *Object_GetById(u32 object_id);
void ObjectMotion_SetSpeedParameters(u32 object_id, s32 speed_limit, s32 acceleration);
void ObjectMotion_EnableActionAndSetCallback(u32 object_id, s32 action);
void ObjectMotion_EnableActionAndResetMotion(u32 object_id);
void Object_LinkObjectAndSetCallback(u32 object_id, u32 linked_object_id);
void Object_RefreshSelectorById(u32 object_id);
void Object_SetActionCallbackAndRefreshById(u32 object_id, s32 action);
void ObjectMotion_ResetAndSetPositionInMode2(u32 object_id, s32 x, s32 z);
void ObjectMotion_SetPositionAndReset(u32 object_id, s32 x, s32 z);
void ObjectMotion_OffsetPositionAndResetMotion(u32 object_id, s32 x_offset, s32 z_offset);
void Motion_LaunchFromFocusedObject(u32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ObjectMotion_CommitCurrentPositionAndActivate(u32 object_id);
void ObjectMotion_SetHorizontalPositionWithTerrain(u32 object_id, s32 x, s32 z);
void Object_SetModeById(u32 object_id, s32 action);
void ObjectMotion_WaitForAnimationChange(u32 object_id);
void Motion_SetModeAndWaitAnimation(u32 object_id, s32 action);
void ObjectMotion_Launch(u32 object_id, s32 speed, s32 event_id);

void Object_SetActionById(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL)
        ObjectDispatch_ApplyValueToChildrenFar(object, action);
}
