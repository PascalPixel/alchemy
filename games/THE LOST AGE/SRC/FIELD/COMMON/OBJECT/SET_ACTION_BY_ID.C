#include "EVENTWRK.H"
#include "OBJECT_RUNTIME.H"
#include "OBJDISP.H"
#include "SYSTEM.H"

void ObjectMotion_SetActionVariant(u32, s32);
void ObjectMotion_SetHorizontalPositionWithTerrain(u32, s32, s32);
void ObjectDispatch_ApplyValueToChildrenFar(struct ObjectRuntime *, s32);
s32 Map_GetTerrainHeightFar(u8, s32, s32);
void Audio_PlayCue(s32);

void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);
void Object_CommitPosition(struct ObjectRuntime *);
void Object_SetMode(struct ObjectRuntime *, s32);
void ObjectDispatch_WaitForValue16Far(struct ObjectRuntime *);
void ObjectMotion_SetActionCallback(struct ObjectRuntime *, s32);

struct ObjectRuntime *Object_GetById(u32 object_id);
void ObjectMotion_SetSpeedParameters(u32 object_id, s32 speed_limit, s32 acceleration);
void ObjectMotion_EnableActionAndSetCallback(u32 object_id, s32 action);
void ObjectMotion_EnableActionAndResetMotion(u32 object_id);
void Object_LinkObjectAndSetCallback(u32 object_id, u32 linked_object_id);
void Object_RefreshSelectorById(u32 object_id);
void Object_SetActionCallbackAndRefreshById(u32 object_id, s32 action);
void ObjectMotion_ResetAndSetPosition(u32 object_id, s32 x, s32 z);
void ObjectMotion_SetPositionAndCommit(u32 object_id, s32 x, s32 z);
void ObjectMotion_ResetAndSetPositionInMode2(u32 object_id, s32 x, s32 z);
void ObjectMotion_SetPositionAndReset(u32 object_id, s32 x, s32 z);
void ObjectMotion_SnapHeadingAndOffset(u32 object_id, s32 action, s32 z_offset);
void ObjectMotion_OffsetPositionAndResetMotion(u32 object_id, s32 x_offset, s32 z_offset);
void ObjectMotion_OffsetPositionAndReset(u32 object_id, s32 x_offset, s32 z_offset);
void ObjectMotion_CommitPositionAndActivate(u32 object_id, s32 x_offset, s32 z_offset);
void Motion_LaunchFromFocusedObject(u32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ObjectMotion_CommitCurrentPositionAndActivate(u32 object_id);
void ObjectMotion_SetHorizontalPositionWithTerrain(u32 object_id, s32 x, s32 z);
void ObjectMotion_SetPositionWithTerrain(u32 object_id, s32 x, s32 y, s32 z);
void Object_SetModeById(u32 object_id, s32 action);
void ObjectMotion_WaitForAnimationChange(u32 object_id);
void Motion_SetModeAndWaitAnimation(u32 object_id, s32 action);
void ObjectMotion_NoOp(void);
void ObjectMotion_Launch(u32 object_id, s32 speed, s32 event_id);
void ObjectMotion_SetVariantCallback(u32 object_id, s32 variant);
void Motion_SetVarCbAndRefresh(u32 object_id, s32 variant);

void Object_SetActionById(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL)
        ObjectDispatch_ApplyValueToChildrenFar(object, action);
}
