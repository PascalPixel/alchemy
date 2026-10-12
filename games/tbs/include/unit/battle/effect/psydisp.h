#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_PSYDISP_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_PSYDISP_H

#include "types.h"
#include "effect_slot.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/marker.h"
#include "unit/battle/effect/sparkle.h"
#include "unit/battle/effect/evactor.h"
#include "unit/battle/effect/evcamera.h"
#include "unit/battle/effect/djinnev.h"

struct EffectVector;
struct FieldActor;
struct ObjectRuntime;

void Field_DispatchTypeHandler(s32 kind);
void BattleFx_Run(void);
void BattleFx_DispatchRequestKind(void);
void BattleFx_ClearChildValueOnMismatch(void);
void FieldEvent_RunTypeHandler(void);
s32 BattleFx_RunEventAction(void *event, s32 param, s32 target);
void ObjectGroup_ApplyRandomChildValues(void *owner);
void Motion_SetTargetPositionFromMagnitudeAngle(struct ObjectRuntime *object, s32 magnitude, s32 angle);
s32 ResourceTable_CountFreeBlocks(void);
void *Object_ReplaceResourceEntry(void *src, void *alt);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void ObjectGroup_SetActionForOthers(struct ObjectRuntime *except, s32 state, s32 value);
void BattleFx_UpdateScaledArcObjectA(struct FieldActor *obj);
void BattleFx_UpdateScaledArcObjectB(struct FieldActor *obj);
void BattleFx_SpawnScaledArcObjects(struct FieldActor *linked);
void BattleFx_FlickerObjectAndTick(s32 obj);
void BattleFx_CycleObjectValueByCounter(s32 obj);
void BattleFx_UpdateAllEffectSlots(void);
void BattleFx_LoadActionEffectResources(s32 act_no, s32 mode);
void BattleFx_SetupObjectPair(s32 user_id, s32 tgt_id);
void EffectRuntime_StopCurrentObject(void);
void BattleEffect_CleanupSceneObjects(void);
void EventObject_Initialize(void);
void BattleEffect_InitializeSharedScene(void);
void BattleFx_PrepareBufferInterpolation(void);
void Camera_WorldToScreen(struct EffectVector *position);

#endif
