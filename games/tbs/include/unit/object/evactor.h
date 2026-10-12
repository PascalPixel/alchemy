#ifndef ALCHEMY_UNIT_OBJECT_EVACTOR_H
#define ALCHEMY_UNIT_OBJECT_EVACTOR_H

#include "types.h"
#include "effect_slot.h"
#include "unit/object/object.h"
#include "unit/object/marker.h"
#include "unit/object/sparkle.h"

struct DispatchObject;
struct FacingObject;
struct FieldActor;
struct ObjectRuntime;

struct FieldActor *Object_GetById(s32 object_id);
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
void ObjectMotion_CenterOnColumn(u32 object_id, s32 action, s32 z_offset);
void ObjectMotion_OffsetPositionAndResetMotion(u32 object_id, s32 x_offset, s32 z_offset);
void ObjectMotion_OffsetPositionAndReset(u32 object_id, s32 x_offset, s32 z_offset);
void ObjectMotion_CommitPositionAndActivate(u32 object_id, s32 x_offset, s32 z_offset);
void Motion_LaunchFromFocusedObject(u32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ObjectMotion_CommitCurrentPositionAndActivate(u32 object_id);
void ObjectMotion_SetHorizontalPositionWithTerrain(u32 object_id, s32 x, s32 z);
void ObjectMotion_SetPositionWithTerrain(u32 object_id, s32 x, s32 y, s32 z);
void Object_SetModeById(u32 object_id, s32 action);
void Object_SetActionById(u32 object_id, s32 action);
void ObjectMotion_WaitForAnimationChange(u32 object_id);
void Motion_SetModeAndWaitAnimation(u32 object_id, s32 action);
void ObjectMotion_NoOp(void);
void ObjectMotion_Launch(u32 object_id, s32 speed, s32 event_id);
void ObjectMotion_SetVariantCallback(u32 object_id, s32 variant);
void Motion_SetVarCbAndRefresh(u32 object_id, s32 variant);
void BattleFx_UpdateParticleLinearMotion(struct FieldActor *particle);
void BattleFx_SpawnBurstParticle(struct FieldActor *source, s32 optional);
void BattleFx_RunRisingObjectSequence(s32 sequence_arg, s32 mode_or_frame, s32 optional_action);
void ObjectMotion_SetAngleToward(s32 first_id, s32 second_id, s32 wait);
void Object_LinkPair(s32 first_id, s32 second_id, s32 wait);
void FacingObject_TurnPairToFaceEachOther(struct FacingObject *raw_first, struct FacingObject *raw_second);
void ObjectTable_DestroyById(s32 index);
void ObjectTable_DestroyNoOp(void);
void ObjectGroup_ConfigureChildValue(s32 object_id, s32 value);
void ObjectGroup_ApplyIndexedChildValue(struct DispatchObject *object);
void ObjectGroup_SetChildValue(struct FieldActor *actor, s32 value);
void Object_SetTargetAndCallback(u32 object_id, s32 target_id, const void *callback);
s32 ObjectMotion_StepAngle(struct ObjectRuntime *object);
void Object_ResetTargetAndSetMode1(u32 object_id);
void ObjectMotion_ArmCallback(s32 object_id, s32 angle, s32 wait);
void ObjectMotion_SetActionVariant(s32 object_id, s32 priority);
void ObjectVisual_CopyAttributes(u32 target_id, u32 source_id);
void ObjectVisual_ReservedNoOp(void);

#endif
