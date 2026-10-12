#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT12_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT12_H

#include "types.h"
#include "effect_slot.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object11.h"

struct FieldActor;
struct ObjectRuntime;

void Object_AttachWorkTargetToObject(s32 id, s32 flag);
void ObjectTable_AllocateAndSetObjectSpeed(s32 first, s32 second);
void Motion_CamBounds(s32 requested_x, s32 requested_y, s32 requested_z, s32 use_setter);
void Object_PlaceCurrentWithinCameraBounds(s32 arg0, s32 arg1);
void BattleFx_CommitObjectPositionAndWait(void);
struct FieldActor *Battle_GetWorkObject1e0(void);
void BattleFx_LinkObjectToTarget(struct ObjectRuntime *target, s32 keep_current_position);
void Map_SetViewBounds(s32 min_x, s32 min_y, s32 max_x, s32 max_y);
s32 BattleFx_StepRatioTransition(void);
void BattleFx_ScheduleRatioTransition(s32 zoom, s32 frames);
void Event_WaitForZoomTransition(void);
s32 BattleFx_CopyLinkedObjectPosition(void *self);
void BattleFx_SpawnLinked(s32 id, s32 mode, s32 wait);
void BattleFx_ConfigureLinkedObject(s32 id, s32 flags);
s32 Object_ClearChildAndCollisionFlags(void *obj);
s32 ObjectMotion_MoveTowardTarget(struct ObjectRuntime *object);
s32 ObjectMotion_TurnTowardLinkedTarget(struct ObjectRuntime *object);
void ObjectMotion_SetActionCallback(struct ObjectRuntime *object, s32 kind);
struct ObjectRuntime *Object_FindNearestFacingTarget(struct ObjectRuntime *self, s32 id);
s32 Field_TryJumpForward(void);
s32 FieldEffect_UpdateGridPlacement(void);
s32 battle_owner_69(void);
s32 Object_GetScreenPosition(s32 object_id, s32 *position);
void ObjectEffect_ReservedNoOp941DC(void);
s32 Scene_FadeColorFromWhite(s32 unused);
void ObjectEffect_PrepareContextEffect(s32 value);
void ObjectEffect_BeginContextEffect26(void);
void ObjectEffect_BeginContextEffect25(void);
void ObjectEffect_EndContextEffect(s32 arg0);
s32 ObjectEffect_RunPendingFlagEvent(void);

#endif
