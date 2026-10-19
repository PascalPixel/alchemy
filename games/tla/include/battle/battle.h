#ifndef BATTLE_H
#define BATTLE_H

/// @file
/// BATTLE

#include "types.h"
#include "edition.h"
#include "vector3.h"

struct BattleActionRecord;
struct BattleEffectArgument;
struct BattlePlan;
struct BattleUnit;
struct DispatchObject;
struct EffectSlot;
struct ObjectRuntime;
struct SceneAreaNameRule;

extern u16 gBattleFx_CyclePatternWords[];
extern s8 gBattleFx_RandomChildValues[];
extern const s8 gBattlePlacement_StepPairs[];
extern u8 gEncounter_AreaEntryTable[];
extern u8 gPlayerObjectId[];
extern u16 gResource_SlotAssignments[];
s32 BattleTargetExec(struct BattlePlan *plan, s32 slot);
struct BattleObj;
#ifndef OWN_BattlePresentation_SpawnActorObject
void BattleObjMake(struct BattleObj *actor, s32 unit, s32 x, s32 y);
#endif
#ifndef OWN_BattleActor_SpawnObjectsForList
void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh);
#endif
void BattleEffect_InitializeSharedScene(void);
void BattleEvent_Playback(void);
void BattleFx_DrawScaledObject(struct EffectSlot *object);
#ifndef OWN_BattleFx_GetResourceGroup
s8 BattleFx_GetResourceGroup(s32 effect_index);
#endif
void BattleFx_PrepareBufferInterpolation(void);
void BattleFx_RunTargetMotes(struct BattleEffectArgument *effect, u32 mode);
void BattleFx_DoNothing(void);
void FieldFx_RunFrost(void);
void BattleFx_RunProjectileVolley(struct BattleEffectArgument *effect,
                                  s32 mode);
void BattleFx_RunHealingMotes(struct BattleEffectArgument *effect, u32 mode);
void *BattleFx_StartItemBreak(void *object);
s32 BattleMotion_GetSlotPalette(s32 id);
void BattleMotion_ResetObjectAtScaledAnchor(s32 objectId);
void BattleMotion_SetMode5AndActivateSlot(s32 unitId);
void BattleMotion_SetupEscapeObject(s32 objectId);
#ifndef OWN_BattlePres_BuildOpponentEntries
s32 BattlePres_BuildOpponentEntries(struct BattleActionRecord *entries);
#endif
#ifndef OWN_BattlePres_BuildUnitEntries
s32 BattlePres_BuildUnitEntries(struct BattleActionRecord *entries);
#endif
s32 BattlePres_WaitSync(void);
s32 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions,
                                           s32 count);
void BattleUnit_ResetGuardLevels(void);
void Camera_InitDefaultTransform(void);
void EffectSlot_UpdateMotion(struct EffectSlot *effect);
void FieldFx_RunDouse(void);
s32 BattleClass_IsSmall(s32 index);
void BattlePres_SetActorModesFar(u16 *, s32);
void Map_RenderAllAnimatedTileFramesFar(void **, s32);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x,
                                         s32 y, s32 z);
void ObjectDispatch_SetAnimationSpeedFar(struct DispatchObject *object,
                                         s32 value);
void Unit_SetHpFar(struct BattleUnit *, s32);
void UiWork_ClearValueNameTablesFar(void);
void UiWork_FinalizeSharedSlotFar(void);
void UpdateRisingParticleBurst(void *object);

struct AnimationObject;
struct BattleActionRecord;
struct BattleEffectBuffers;
struct BattleObj;
struct DispatchObject;
struct ParticleChild;
struct PendingEvent;
struct PlacementTable;
struct ScaledSprite;
struct SceneCueRule;
struct WorldMapWork;

/// A spawned object as a particle emitter sees it: only its sprite.
struct ParticleEffectObject {
  u8 padding[80];              ///< not named through this view
  struct ParticleChild *child; ///< the sprite, MotionObject's records
};
struct ParticlePosition {
  s32 x;
  s32 y;
  s32 z;
};
extern const u8 gBattleFx_ArcTiles[];
extern u16 gBattleFx_EightWayHeadingWords[];
extern const u8 gBattleFx_ParticleScript[];
extern const u8 gBattleFx_UntargetedObjectScript[];
extern const u8 gColorBuffer_FadeStep1Map[32];
extern const u8 gColorBuffer_FadeStep2Map[32];
extern const u8 gColorBuffer_FadeStep3Map[32];
extern u16 gEncounter_EnemyGroupTable[];
extern struct WorldMapWork gWorldMapWork;
extern const struct SceneCueRule gScene_CueRuleTable[];
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *, s32);
s16 *Scene_FindActorPlacement(s16 action);
s32 BattleActor_RemoveFromLists(s32);
void BattleFx_ApplyColorToSourceBuffer(s32 value, s32 mode);
void BattleFx_BuildBuffer(u32 mode, u16 *src, u16 *dst, s32 half);
void FieldFx_RunWhirlwindOnTarget(void);
u16 Encounter_GetEnemyGroupAtSlot(s32 arg0, s32 arg1);
s32 BattleFx_HasMatchingEvent5(s32 effectId);
void BattleFx_IntegrateVector2(s32 *vector, s32 damping, s32 gravity);
void BattleFx_IntegrateVector3(s32 *vector, s32 damping, s32 gravity);
s32 Encounter_ResolveWorldMapZoneAndBackdrop(void *);
s32 BattleFx_RunProjectileVolleyB(void *effect, s32 mode);
s32 BattleFx_RunSparkGroups(void *, s32);
void BattleFx_SnapScaleToFull(void *object);
void BattleFx_StartBufferInterpolation(s32 frames);
s32 BattleFx_StartRandomParticleEmitter(s32, s32);
void BattleFx_UpdateArcEffect(void);
void BattleLayout_HighlightPartyPanelsFar(u16 *);
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
void BattleMotion_DestroyAllSlotObjects(void);
void BattleMotion_ReleaseObjectSlotByValue(s32 objectId);
void BattleMotion_SetObjectPosition(struct BattleObj *slot);
void BattleMotion_SetRecordChildValues(void *, s32);
void BattlePalette_UpdateBlend(void);
s32 BattleParty_PrepareReserveOwners(u16 *owners);
void BattlePres_SetActorModes(u16 *, s32);
void BattlePresentation_WaitForAdvance(void);
void BattleQueue_SortByPriority(struct BattleActionRecord *actions, s32 count);
void Camera_ConfigureScene(s32 scene);
s32 Battle_GetElementMultiplier(s32 selector, s32 mode);
s32 Djinn_ActivateFar(s32 unitId, s32 element, s32 index);
s32 Func_08020190(s32, s32, s32);
s32 Func_080cdac0(s32 effect, s32 from, s32 to);
s32 Func_080ce31c(s32 kind);
struct PendingEvent *Func_080ce458(u32 filter, s32 kind, s32 selector);
void Func_080e3074(void);
void Func_0811b75c(s16 *list, s32 refresh, s32 spawn);
void Func_0818f620(void *effect, s32 mode);
void GameFlag_ClearBitFar(s32 flag);
void GameFlag_SetBitFar(s32 flag);
s32 Map_RenderAnimatedTileFramesForObjectFar(void *object);
#ifndef OWN_Object_Destroy
void Object_Destroy(struct DispatchObject *work);
#endif
void ObjectMotion_SnapToTerrain(void *object);
s32 Enemy_GetAttackElement(void *state);
#ifndef OWN_Owner_RecalculateStatsFar
void UnitCalcFar(s32);
#endif
s32 Party_CountActiveOwnersFar(void);
void Render_ApplyProjectedPlacementFar(struct ScaledSprite *sprite,
                                       s32 *position, s32 *scale, s32 mode);
void SummonFx_RunCanvasEffect(void *action, s32 mode);
struct PlacementTable *Djinn_GetRecoveryTableFar(s32 owner);
s32 DjinnRecovery_RemoveFar(s32 unitId, s32 element, s32 index);
void UiText_DrawQuantity(s32, s32);
void UiText_ShowMessageAndWaitCoreFar(s32 messageId);
void PartyStatusDrawFar(s32 mode);

/// The battle sprites' animations (AnimationObjects_SelectAnimation numbers)
/// that the battle code picks by name.
enum BattlePose {
  POSE_ENEMY_HELD = 0, ///< an enemy asleep, stunned or held
  POSE_STANDING = 1,
  POSE_PARTY_HELD = 4, ///< asleep, stunned or held; also a downed member
  POSE_DAMAGE = 5,     ///< taking damage
  POSE_ENEMY_DOWN = 5  ///< a downed enemy stays in its damage pose
};

#endif
