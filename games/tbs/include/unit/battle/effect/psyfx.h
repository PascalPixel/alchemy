#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_PSYFX_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_PSYFX_H

#include "types.h"
#include "effect_slot.h"

struct ArcEffectObject;
struct Effect05Object;
struct EffectSlot;
struct FieldActor;
struct MotionObject;
struct ObjectRuntime;
struct OrbitEffect;
struct SparkObject;
union FieldObject;

void RunSceneTransitionEffect(s32 x, s32 y);
void BattleFx_FinishSceneAndReleaseHeapBlock(void);
void BattleFx_UpdateWaveScene(void);
void BattleFx_ArmBg0HBlankDma(void);
void BattleFx_AdvanceHueCycle(void);
void BattleFx_ComputeHueChannels(s32 hue, s32 *r, s32 *g, s32 *b);
s32 BattleFx_HueChannelRamp(s32 angle, s32 low, s32 high);
s32 Fixed_Remainder(s32 value, s32 divisor);
void BattleFx_SetCallbackWhenTargetUnset(struct ObjectRuntime *obj);
void Ui_FillBank15PaletteGrey(void);
void Ui_SetBank15PaletteAndClearRenderMode(void);
s32 BattleFx_GetCycledTableWord(u32 unused);
void BattleFx_UpdateItemBreakFragment(struct FieldActor *src);
void BattleFx_SteerPushedTarget(s32 target_id);
void BattleFx_UpdateOrbitAndReturn(struct EffectSlot *slot);
void BattleFx_RunItemBreakSequence(void);
void *BattleFx_StartItemBreak(void *object);
void BattleFx_SnapScaleToFull(struct MotionObject *obj);
void UpdateRisingParticleBurst(void *object);
void ObjectDispatch_ApplyValueToKind200Children(s32 value);
void FieldEffect_WatchLeaderDistance(void);
void RunBattleEffect08(void);
void ResetSceneTransitionEffect(void);
void BattleFx_RunBurstParticleMainObject(s32 unused);
void BattleFx_RunBurstParticles(void);
void BattleEffect_SpawnBurstParticleField(void);
void BattleEffect_RunTargetedItemBreak(s32 unused);
struct MotionObject *BattleFx_SpawnItemBreakMode3(s32 x, s32 y, s32 z, s32 angle);
void BattleFx_UpdateRadialBurst(struct EffectSlot *slot);
s32 SpawnHeavyImpactEffect(struct MotionObject *src);
void BattleFx_CallEffect04(s32 unused);
void RunBattleEffect04(void);
void BattleFx_UpdateOrbitingParticleMain(struct FieldActor *particle);
void BattleFx_UpdateOrbitingParticleFade(struct FieldActor *particle);
void BattleFx_UpdateOrbitingParticleLeft(struct FieldActor *particle);
void BattleFx_UpdateOrbitingParticleRight(struct FieldActor *particle);
void BattleFx_StartOrbitingParticles(s32 unused);
void BattleFx_RunOrbitingParticles(void);
void BattleFx_SwayParticle(struct SparkObject *obj);
void BattleFx_UpdateShrinkingOrbitObject(struct FieldActor *obj);
void BattleFx_RunSparkEmitter(struct SparkObject *obj);
void BattleFx_CallEffect03AndStop(s32 unused);
void RunBattleEffect03(void);
void CheckObjectMapTile(void);
void MapEvent_RunTileTriggerSequence(void);
void BattleFx_ScheduleCloakedMapCheck(void);
void BattleFx_RunFlashingCallbackSequence(void);
void BattleFx_SpawnRandomAngleTriplet(struct Effect05Object *obj);
void BattleFx_UpdateDriftingFallObject(struct ObjectRuntime *obj);
void BattleFx_CallEffect05(s32 unused);
void RunBattleEffect05(void);
void BattleFx_SpawnFallingParticles(void);
void BattleFx_SteerLiftedTarget(s32 id);
struct MotionObject *BattleFx_SpawnItemBreakMode1(s32 x, s32 y, s32 z, s32 angle);
void BattleFx_UpdateParticleMotionAndScale(union FieldObject *object);
void BattleFx_SpawnRadialParticleRing(struct MotionObject *origin);
void BattleFx_WanderAroundAnchor(struct OrbitEffect *efx);
void BattleFx_CircleAnchor(struct OrbitEffect *efx);
void BattleFx_ShrinkObjectScaleUntilHalf(struct MotionObject *obj);
void BattleFx_CallEffect14(s32 unused);
void RunBattleEffect14(void);
void BattleFx_UpdateRadialCamera(struct EffectSlot *effect);
void BattleFx_MarkChildAndRunFallbackTransition(s32 unused);
void BattleEffect_RunFallbackObjectTransition(void);
void BattleEffect_SetRandomTableValueOnObject(void *obj);
void BattleEffect_PauseObject(s32 id);
void BattleFx_ResumeObject(s32 id);
s32 BattleFx_FilterObjectIdByFlags(s32 id);
void BattleFx_RunEffect13Hook(s32 unused);
void RunBattleEffect13(void);
void BattleFx_UpdateSpinAngle(struct FieldActor *obj);
void BattleFx_UpdateDescendingObject(struct FieldActor *obj);
void BattleFx_UpdateRadialLaunch(struct EffectSlot *slot);
void BattleFx_RunEffect15(void);
void BattleFx_UpdateDescendingParticlePositiveArc(struct FieldActor *particle);
void BattleFx_UpdateDescendingParticleNegativeArc(struct FieldActor *particle);
void BattleFx_SpawnDescendingArcParticles(struct FieldActor *arc);
void BattleFx_UpdateEffect16State(void);
void BattleFx_UpdatePairedArcSpawner(struct ArcEffectObject *obj);
void FieldEvent_ShowStatusMessage(void);
void RunBattleEffect16(void);

#endif
