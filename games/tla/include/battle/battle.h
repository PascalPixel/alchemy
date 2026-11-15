#ifndef BATTLE_H
#define BATTLE_H

/// @file
/// BATTLE

#include "types.h"
#include "edition.h"
#include "system/vector3.h"

struct BattleActionRecord;
struct BattleEffectParams;
struct BattlePlan;
struct Unit;
struct Actor;
struct MotionObject;
struct FieldParticle;
struct Actor;
struct SceneAreaNameRule;

extern u16 gBattleEffect_CyclePatternWords[];
extern s8 gBattleEffect_RandomChildValues[];
extern const s8 gBattlePlacement_StepPairs[];
extern u8 gEncounter_AreaEntryTable[];
extern u16 gBattleUnit_WeaponKinds[];
s32 BattleAbility_ApplyToTarget(struct BattlePlan *plan, s32 slot);
struct BattleObject;
#ifndef OWN_BattleObject_Make
void BattleObject_Make(struct BattleObject *actor, s32 unitId, s32 x, s32 y);
#endif
#ifndef OWN_BattleActor_SpawnObjectsForList
void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh);
#endif
void FieldPsynergy_FadeToCastColor(void);
void BattlePlayback_Run(void);
void FieldEffect_DrawScaledActor(struct FieldParticle *object);
#ifndef OWN_BattleEffect_GetResourceGroup
s32 Scene_GetArea(s32 sceneId);
#endif
void FieldPsynergy_FadeFromCastColor(void);
void BattleEffect_RunTargetMotes(struct BattleEffectParams *effect, u32 mode);
void BattleEffect_DoNothing(void);
void FieldPsynergy_Frost(void);
void BattleEffect_RunProjectileVolley(struct BattleEffectParams *effect,
                                      s32 mode);
void BattleEffect_RunHealingMotes(struct BattleEffectParams *effect, u32 mode);
void *FieldEffect_SpawnMoveHand(void *object);
s32 BattleMotion_GetSlotPalette(s32 id);
void BattleMotion_HopBack(s32 objectId);
void BattleMotion_PlayFall(s32 unitId);
void BattleMotion_SetupEscapeObject(s32 objectId);
#ifndef OWN_Battle_BuildOpponentActions
s32 Battle_BuildEnemyActions(struct BattleActionRecord *entries);
#endif
#ifndef OWN_Battle_BuildUnitActions
s32 Battle_BuildUnitActions(struct BattleActionRecord *entries);
#endif
s32 LinkBattle_WaitSync(void);
s32 Battle_AppendLinkedActions(struct BattleActionRecord *actions, s32 count);
void BattleUnit_ResetGuardLevels(void);
void BattleCamera_SetDefaultView(void);
void FieldParticle_UpdateMotion(struct FieldParticle *effect);
void FieldPsynergy_Douse(void);
s32 BattleSprite_IsSmall(s32 index);
/// BattleHighlight_SetModes's modes: the units listed take one blend and
/// every other unit the other (mode bit 0), and the battle steps the blend
/// each frame (BattleBackground_StepHighlightBlend).
enum BattleHighlight {
  HIGHLIGHT_OFF = 0,      ///< every unit opaque, the blend stopped
  HIGHLIGHT_SELECTED = 1, ///< the units listed lighten by the ramp
  /// The units listed, semi-transparent, pulse by the ramp: the reserve member
  /// a party command offers.
  HIGHLIGHT_PULSE = 2,
  HIGHLIGHT_FADE_OUT = 4, ///< the units listed fade out over 32 frames
  HIGHLIGHT_FADE_IN = 6   ///< the units listed fade in over 32 frames
};

void BattleHighlight_SetModesFar(u16 *, s32);
void SpriteDissolve_RunManyFar(void **, s32);
void Actor_SetPositionAndResetMotionFar(struct Actor *object, s32 x, s32 y,
                                        s32 z);
/// Sets the timer step of every sprite of an actor (16 is normal speed).
void Actor_SetAnimationSpeedFar(struct Actor *object, s32 value);
/// Sets the unit's HP, kept between 0 and its maximum, and updates its gauges.
void Unit_SetHpFar(struct Unit *, s32);
void MessageArgument_ClearAllFar(void);
void BattleMessage_CloseFar(void);
void UpdateRisingParticleBurst(void *object);

struct Sprite;
struct BattleActionRecord;
struct PaletteFade;
struct BattleObject;
struct Actor;
struct ParticleChild;
struct PendingEvent;
struct ScaledSprite;
struct SceneCueRule;
struct WorldMapWork;

/// A spawned object as a particle emitter sees it: only its sprite.
struct ParticleEffectObject {
  u8 padding[80];              ///< not named through this view
  struct ParticleChild *child; ///< the sprite, MotionObject's records
};
/// A point in the field, in 16.16 pixels.
struct ParticlePosition {
  s32 x;
  s32 y;
  s32 z;
};
extern const u8 gBattleEffect_ArcTiles[];
extern u16 gBattleEffect_EightWayHeadingWords[];
extern const u8 gBattleEffect_ParticleScript[];
extern const u8 gBattleEffect_UntargetedObjectScript[];
extern const u8 gColorBuffer_FadeStep1Map[32];
extern const u8 gColorBuffer_FadeStep2Map[32];
extern const u8 gColorBuffer_FadeStep3Map[32];
extern struct WorldMapWork gWorldMapWork;
s32 AnimationObject_SelectAnimationFar(struct Sprite *, s32);
struct ActionDescriptor;
struct ActionDescriptor *Scene_FindActorPlacement(s32 id);
s32 BattleObject_RemoveFromLists(s32);
void PaletteFade_SetSource(s32 value, s32 mode);
void PaletteFade_Build(u32 mode, u16 *src, u16 *dst, s32 half);
void FieldPsynergy_WhirlwindOnTarget(void);
u16 Encounter_GetFormationAtSlot(s32 arg0, s32 arg1);
s32 Scene_HasPsynergyTrigger(s32 effectId);
void BattleEffect_IntegrateVector2(s32 *vector, s32 damping, s32 gravity);
void BattleEffect_IntegrateVector3(s32 *vector, s32 damping, s32 gravity);
s32 Encounter_ResolveWorldMapZoneAndBackdrop(void *);
void BattleEffect_RunBurstShower(struct BattleEffectParams *effect,
                                       u32 mode);
void BattleEffect_RunSparkGroups(struct BattleEffectParams *effect, s32 kind);
void FieldEffect_SnapScaleToFull(struct Actor *object);
void PaletteFade_StartInterpolation(s32 frames);
struct Actor *BattleEffect_StartRandomParticleEmitter(s32 actor, s32 item);
void BattleEffect_UpdateArcEffect(void);
void PartyStatus_HighlightMembersFar(u16 *);
void BattleMotion_ApproachTargetFar(s32 actorId, s32 targetId,
                                    s32 travelDivisor, s32 initialVelocityY);
void BattleMotion_DestroyAllSlotObjects(void);
void BattleObject_ReleaseSpriteSlots(s32 objectId);
void BattleMotion_ReturnObjectToSpot(struct BattleObject *slot);
void BattleObject_SetColorMode(struct MotionObject *object, s32 value);
void PaletteFade_Update(void);
s32 BattleParty_PrepareReserveOwners(u16 *owners);
void BattleHighlight_SetModes(u16 *, s32);
void BattleMessage_WaitForAdvance(void);
void BattleActionOrder_Sort(struct BattleActionRecord *actions, s32 count);
void BattleCamera_SetCommandView(s32 scene);
s32 Battle_GetElementMultiplier(s32 selector, s32 mode);
s32 Djinn_SetFar(s32 unitId, s32 element, s32 index);
s32 Map_SetFlagAndApplyAttributeEditsFar(s32, s32, s32);
s32 Field_MeasurePsynergyReach(s32 effect, s32 from, s32 to);
s32 Scene_SelectPsynergyTarget(s32 kind);
struct TriggerEvent;
struct TriggerEvent *Scene_FindPsynergyTrigger(s32 filter, s32 kind,
                                               s32 selector);
void FieldPsynergy_Cyclone(void);
void BattleObject_SpawnObjectsForList(s16 *list, s32 refresh, s32 spawn);
void BattleEffect_RunCanvasBurst(struct BattleEffectParams *effect, u32 mode);
/// Clears the flag, returning nothing where GameFlag_SetFar returns its byte.
void GameFlag_ClearFar(s32 flag);
void GameFlag_SetFar(s32 flag);
s32 SpriteDissolve_RunFar(void *object);
void Actor_ReleaseFar(struct Actor *object);
void Actor_SnapToTerrain(void *object);
s32 Enemy_GetAttackElement(void *state);
#ifndef OWN_Unit_CalculateFar
/// Works a unit's stats out again from its base stats, equipment, set Djinn,
/// class and battle stat levels, keeping its HP and PP at the same fraction.
void Unit_CalculateFar(s32);
#endif
s32 Party_CountMembersFar(void);
void Render_ApplyProjectedPlacementFar(struct ScaledSprite *sprite,
                                       s32 *position, s32 *scale, s32 mode);
void BattleEffect_RunStatReduction(struct BattleEffectParams *effect, s32 mode);
struct DjinnRecoveryTable *Djinn_GetRecoveryTableFar(s32 owner);
s32 DjinnRecovery_RemoveFar(s32 unitId, s32 element, s32 index);
void MessageArgument_PushFar(s32, s32);
void BattleMessage_ShowAndWaitFar(s32 messageId);
void PartyStatus_DrawFar(s32 mode);

/// The battle sprites' animations (AnimationObject_SelectAnimation numbers)
/// that the battle code picks by name.
enum BattlePose {
  POSE_ENEMY_HELD = 0, ///< an enemy asleep, stunned or held
  POSE_STANDING = 1,
  POSE_PARTY_HELD = 4, ///< asleep, stunned or held; also a downed member
  POSE_DAMAGE = 5,     ///< taking damage
  POSE_ENEMY_DOWN = 5  ///< a downed enemy stays in its damage pose
};

#endif
