#ifndef ALCHEMY_BATTLE_H
#define ALCHEMY_BATTLE_H

/// @file
/// The battle's routines as other files call them: the battle bank's own, by
/// module, then the battle effects, the field routines the battle and its
/// effects borrow, and the far calls into the main image.

#include "fixed_math.h"
#include "types.h"
#include "edition.h"
#include "system/vector3.h"
#include "battle/battle_type.h"
#include "field/sprite.h"

struct ActionDescriptor;
struct Actor;
struct AreaNameRule;
struct BattleActionRecord;
struct BattleEffectParams;
struct BattleObject;
struct BattlePlan;
struct BattlePresentationState;
struct FieldParticle;
struct MapMusicRule;
struct MotionObject;
struct PaletteFade;
struct PendingEvent;
struct Sprite;
struct TriggerEvent;
struct Unit;
struct WorldMapState;

/// A battle unit's sprites' OBJ mode, as BattleHighlight_SetSpriteMode
/// sets it: faded units are semi-transparent, and the battle's blends act on
/// them.
enum BattleSpriteMode { SPRITE_OPAQUE, SPRITE_SEMI_TRANSPARENT };

/// The glows ElementGlow_Play plays about a unit in its element.
enum BattleElementGlow {
  /// The star field gathering on the unit while the background darkens: a
  /// cast.
  GLOW_STAR_FIELD,
  GLOW_STREAKS, ///< the element's streaks: an enemy's cast
  GLOW_PILLAR,  ///< the element's pillar, at a strength: a summon's boost
  GLOW_BURST    ///< the element's burst: a Djinni set
};

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

/// The battle sprites' animations (AnimationObject_SelectAnimation numbers)
/// that the battle code picks by name.
enum BattlePose {
  POSE_ENEMY_HELD = 0, ///< an enemy asleep, stunned or held
  POSE_STANDING = 1,
  POSE_LEAP = 2,       ///< running up to a target and leaping
  POSE_CAST = 3,       ///< a hand raised to cast or use an item
  POSE_PARTY_HELD = 4, ///< asleep, stunned or held; also a downed member
  POSE_DAMAGE = 5,     ///< taking damage
  POSE_ENEMY_DOWN = 5  ///< a downed enemy stays in its damage pose
};

/// BG1's vertical scroll that sets the battle background in the battle view.
#define BATTLE_BACKGROUND_SCROLL_Y 32
/// BG1's map in a battle, a screen block: while the battle background lies
/// flat, lines outside its plane read the padded map there; the command row
/// places its icons' tiles in it, and the effects that take BG1 over (the
/// canvas, Ulysses's rune circles) lay their own maps there.
#define BATTLE_BG1_SCREEN_BLOCK 31
/// BattleCamera_SetView's zoom, in percent, for the battle's usual view.
#define BATTLE_ZOOM_NORMAL 100

/// The battle's multipliers are 256ths: this one keeps an amount as it is,
/// and Battle_GetElementMultiplier returns it for no element difference.
#define BATTLE_MULTIPLIER_ONE 256
/// The element power at which healing is neither stronger nor weaker
/// (Battle_CalculateRecovery): each point above it counts as two points of
/// element difference.
#define HEALING_BASE_POWER 100

// The tables other banks lend the battle, in the order they are declared.
extern const s8 gHalt_FlickerColorModes[];
extern const s8 gBattlePlacement_Spots[];
extern u8 gEncounter_DjinnBattles[];
extern u16 gBattleUnit_WeaponKinds[];
/// The 32x16 spark the swelling arcs over a caster are drawn with
/// (field_effects.s).
extern const u8 gFieldEffect_SparkTiles[];
extern const u16 gInput_DpadAnglesEightWay[];
extern const s32 gItemDisplay_SparkleScript[];
extern const u8 gMindRead_CrescentEndScript[];
extern const u8 gColorBuffer_DarkenOneLevelMap[32];
extern const u8 gColorBuffer_DarkenTwoLevelsMap[32];
extern const u8 gColorBuffer_DarkenFourLevelsMap[32];
/// The map screen's state while it is open.
extern struct WorldMapState gWorldMapState;

// The battle's run and turn: the battle itself, the actions gathered and
// sorted, the turn's end and the spoils.
s32 Battle_Run(s32 enemyGroup);
void BattleIntro_AnnounceEncounter(s32 enemyCount);
void Battle_TurnStartNoOp(void);
s32 Battle_GatherActions(struct BattleActionRecord *actions);
#ifndef OWN_Battle_BuildOpponentActions
s32 Battle_BuildEnemyActions(struct BattleActionRecord *entries);
#endif
#ifndef OWN_Battle_BuildUnitActions
s32 Battle_BuildUnitActions(struct BattleActionRecord *entries);
#endif
s32 LinkBattle_WaitSync(void);
s32 Battle_AppendLinkedActions(struct BattleActionRecord *actions, s32 count);
void BattleActionOrder_Sort(struct BattleActionRecord *actions, s32 count);
s32 Battle_FindUnitPlace(u32 id);
void BattleUnit_ResetGuardLevels(void);
void BattleUnit_ProcessActionEnd(struct BattlePlan *plan);
void Battle_ProcessTurnEnd(void);
void BattleSpoils_Clear(void);
s32 BattleEnemy_RecordDefeat(s32 unitId, s32 earned);
void Battle_AwardSpoils(void);

// The battle's units: who is living, on which side, and where.
s32 BattleUnit_ListLivingIds(s32 sideMask, u16 *unitIds);
s32 BattleUnit_ListLivingWithReserve(s32 sideMask, u16 *unitIds);
s32 BattleParty_PrepareReserveOwners(u16 *owners);
void BattleUnit_RefreshPlacement(void);
s32 BattleUnit_GetEffectHeight(s32 owner);
s32 BattleUnit_GetPartyAttackEffect(s32 kind);
s32 BattleSprite_IsSmall(s32 index);
s32 BattleAbility_ApplyToTarget(struct BattlePlan *plan, s32 slot);

// An action's staging, and the battle as Auto Battle plays it.
s32 BattleStaging_DispatchAction(struct BattleActionRecord *action, s32 delay);
s32 BattleStaging_RunFullScreen(struct BattlePlan *plan, s32 mode);
s32 BattleStaging_RunPsynergyCast(struct BattlePlan *plan, s32 mode);
s32 BattleStaging_RunItem(struct BattlePlan *plan, s32 unused,
                          struct BattleActionRecord *selection);
s32 BattleStaging_RunFlee(struct BattlePlan *plan);
void BattleStaging_BuildTargetList(struct BattlePlan *plan,
                                   struct BattlePresentationState *effect);
s32 AutoBattle_BuildActions(struct BattleActionRecord *entries);
s32 AutoBattle_RunAction(struct BattleActionRecord *action);
s32 AutoBattle_ShowDefend(s16 *);
s32 AutoBattle_RunAttackOrItem(struct BattleActionRecord *action);
s32 AutoBattle_RunPsynergy(struct BattleActionRecord *action);
void BattlePlayback_Run(void);
s32 BattlePlayback_WaitForReady(void);
u32 BattlePlayback_Reset(void);
void BattleMessage_WaitForAdvance(void);

// The battle objects: each unit's sprites on the battlefield, and how they
// move.
#ifndef OWN_BattleObject_Make
void BattleObject_Make(struct BattleObject *battleObject, s32 unitId, s32 x,
                       s32 y);
#endif
#ifndef OWN_BattleActor_SpawnObjectsForList
void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh);
#endif
void BattleObject_SpawnObjectsForList(s16 *list, s32 refresh, s32 spawn);
s32 BattleObject_RemoveFromLists(s32 id);
void BattleObject_ReleaseSpriteSlots(s32 objectId);
void BattleObject_SetColorMode(struct MotionObject *object, s32 value);
void BattleObject_UpdatePose(s32 id);
void BattleObject_PutKeepingPoses(void);
void BattleObject_UpdateSpritePriorities(void);
s32 EnemySprite_LoadIntoSlots(s32 unit);
s32 BattleMotion_GetSlotPalette(s32 id);
void BattleMotion_HopBack(s32 objectId);
void BattleMotion_PlayFall(s32 unitId);
void BattleMotion_SetupEscapeObject(s32 objectId);
void BattleMotion_ReturnObjectToSpot(struct BattleObject *slot);
void BattleMotion_DestroyAllSlotObjects(void);
void BattleMotion_ApproachTargetFar(s32 actorId, s32 targetId, s32 travelFrames,
                                    s32 initialVelocityY);

// The camera, the background and the units' highlight.
void BattleCamera_SetDefaultView(void);
void BattleCamera_SetCommandView(s32 scene);
void BattleCamera_SetViewAtPairMidpoint(s32 first, s32 second, s32 scale);
void BattleBackground_RestorePalette(void);
void ElementGlow_Play(s32 unitId, s32 element, s32 mode, s32 value);
void BattleHighlight_SetModes(u16 *, s32);
void BattleHighlight_SetModesFar(u16 *, s32);

// The battle effects these call by name (battle_effect/).
void BattleEffect_DoNothing(void);
void BattleEffect_RunTargetMotes(struct BattleEffectParams *effect, u32 mode);
void BattleEffect_RunProjectileVolley(struct BattleEffectParams *effect,
                                      s32 mode);
void BattleEffect_RunHealingMotes(struct BattleEffectParams *effect, u32 mode);
void BattleEffect_RunBurstShower(struct BattleEffectParams *effect, u32 mode);
void BattleEffect_RunSparkGroups(struct BattleEffectParams *effect, s32 kind);
void BattleEffect_RunCanvasBurst(struct BattleEffectParams *effect, u32 mode);
void BattleEffect_RunStatReduction(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_UpdateArcEffect(void);

// The field's Psynergy, effects and palette fade, which the battle's effects
// and the field's own share.
void FieldPsynergy_FadeToCastColor(void);
void FieldPsynergy_FadeFromCastColor(void);
void FieldPsynergy_Frost(void);
void FieldPsynergy_Douse(void);
void FieldPsynergy_Cyclone(void);
void FieldPsynergy_WhirlwindOnTarget(s32 unused);
void FieldEffect_DrawScaledActor(struct FieldParticle *object);
void *FieldEffect_SpawnMoveHand(void *object);
void FieldEffect_GrowHandAndWait(struct Actor *actor);
void FieldEffect_BurstHand(void *object);
void FieldParticle_UpdateMotion(struct FieldParticle *effect);
/// Field_AdvanceWithGravity3D for x and y alone: the third word is left as it
/// is, and the velocities follow it.
void Field_AdvanceWithGravity2D(s32 *vector, s32 damping, s32 gravity);
/// Moves a point (three 16.16 coordinates, their velocities after them) by its
/// velocity, then adds gravity to the second velocity and scales each by
/// damping / 64.
void Field_AdvanceWithGravity3D(s32 *vector, s32 damping, s32 gravity);
void Actor_SnapToTerrain(void *actor);
struct Actor *ItemDisplay_ShowItemInFront(s32 actor, s32 item);
void PaletteFade_SetSource(s32 value, s32 mode);
void PaletteFade_Build(u32 mode, u16 *src, u16 *dst, s32 half);
void PaletteFade_StartInterpolation(s32 frames);
void PaletteFade_Update(void);

// The scene's triggers and encounters, as field Psynergy and the battle's
// start read them.
#ifndef OWN_BattleEffect_GetResourceGroup
s32 Scene_GetArea(s32 sceneId);
#endif
struct ActionDescriptor *Scene_FindActorPlacement(s32 id);
s32 Scene_HasPsynergyTrigger(s32 effectId);
s32 Scene_SelectPsynergyTarget(s32 kind);
struct TriggerEvent *Scene_FindPsynergyTrigger(s32 filter, s32 kind,
                                               s32 selector);
s32 Field_MeasurePsynergyReach(s32 kind, s32 selfId, s32 otherId);
u16 Encounter_GetFormationAtSlot(s32 zone, s32 slot);
s32 Encounter_ResolveWorldMapZoneAndBackdrop(void *position);

// The game's rules: units, Djinn, flags and elements.
s32 Battle_GetElementMultiplier(s32 selector, s32 mode);
s32 Enemy_GetAttackElement(void *state);
#ifndef OWN_Unit_CalculateFar
/// Works a unit's stats out again from its base stats, equipment, set Djinn,
/// class and battle stat levels, keeping its HP and PP at the same fraction.
void Unit_CalculateFar(s32 unitId);
#endif
/// Sets the unit's HP, kept between 0 and its maximum, and updates its gauges.
void Unit_SetHpFar(struct Unit *unit, s32 hp);
s32 Party_CountMembersFar(void);
s32 Djinn_SetFar(s32 unitId, s32 element, s32 index);
struct DjinnRecoveryTable *Djinn_GetRecoveryTableFar(s32 owner);
s32 DjinnRecovery_RemoveFar(s32 unitId, s32 element, s32 index);
/// Clears the flag, returning nothing where GameFlag_SetFar returns its byte.
void GameFlag_ClearFar(s32 flag);
void GameFlag_SetFar(s32 flag);

// The main image's actors, sprites and windows.
void Actor_SetPositionAndResetMotionFar(struct Actor *actor, s32 x, s32 y,
                                        s32 z);
/// Sets the timer step of every sprite of an actor (16 is normal speed).
void Actor_SetAnimationSpeedFar(struct Actor *actor, s32 value);
void Actor_ReleaseFar(struct Actor *actor);
void Render_ApplyProjectedPlacementFar(struct Sprite *sprite, s32 *position,
                                       s32 *scale, s32 mode);
s32 SpriteDissolve_RunFar(void *object);
void SpriteDissolve_RunManyFar(void **, s32);
void MessageArgument_ClearAllFar(void);
void MessageArgument_PushFar(s32 value, s32 kind);
void BattleMessage_ShowAndWaitFar(s32 messageId);
void BattleMessage_CloseFar(void);
void PartyStatus_DrawFar(s32 mode);
void PartyStatus_HighlightMembersFar(u16 *);

#endif
