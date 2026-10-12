#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT14_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT14_H

#include "types.h"
#include "effect_slot.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object3.h"
#include "unit/battle/effect/object4.h"
#include "unit/battle/effect/object5.h"
#include "unit/battle/effect/object6.h"
#include "unit/battle/effect/object7.h"
#include "unit/battle/effect/object8.h"
#include "unit/battle/effect/object9.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"

struct DescendingOrbitObject;
struct EffectSlot;
struct FieldActor;
struct MotionObject;
struct ObjectRuntime;

void ObjectMotion_MoveHalfwayTowardTarget(struct ObjectRuntime *obj);
void EventActor_RunHopAndLeapSequence(s32 id);
void BattleFx_RunPageEffectForSlot(s32 slot, s32 elm, void *djinn);
void FieldObject_PlaceSceneActors(void);
/* Djinn_ResolvePendingEvent is called before its definition with no prototype in scope. */
void BattleFx_UpdateAllSlots(void);
void BattleFx_InitializeSlots(void);
void BattleFx_ClearActiveSlotsAndScheduleUpdates(void);
void BattleFx_AdvanceSpinAngle(struct MotionObject *object);
void BattleFx_RunAngledApproachPhases(struct EffectSlot *actor);
void BattleFx_FinishHeavyImpact(s32 jin_id);
void BattleFx_SetObjectAlternatingWords(u8 *object);
void BattleFx_AdvanceAngleWithRamp(void *obj);
void BattleEffect_UpdatePhasedRadialParticle(struct EffectSlot *effect);
void BattleEffect_RunPhasedRadialParticleSequence(s32 arg);
void BattleFx_UpdateDescendingOrbitObject(struct DescendingOrbitObject *arg);
void BattleFx_UpdateRadialSpread(struct EffectSlot *effect);
void BattleFx_RunVenusDjinnCapture(s32 arg);
void BattleFx_UpdateRandomTargetParticle(struct EffectSlot *effect);
void BattleFx_HalveDistanceToTarget(struct FieldActor *object);
void BattleFx_RunMercuryDjinnCapture(s32 id);

#endif
