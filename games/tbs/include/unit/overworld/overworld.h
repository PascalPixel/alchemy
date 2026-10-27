#ifndef ALCHEMY_UNIT_OVERWORLD_OVERWORLD_H
#define ALCHEMY_UNIT_OVERWORLD_OVERWORLD_H

#include "types.h"
#include "field_effect_sprite.h"

struct SpeakerDefinition;
struct Actor;
struct ScreenTransitionState;
struct ActorDouseView;
struct TriggerView;
struct FieldParticle;
struct Vector3;
struct Actor;
struct OrbitEffect;
struct ActorTemplate;
struct Window;
struct Actor;

/// The exit request whose destination is already in gSaveBlock.sceneId and
/// .entrance, so that Scene_TakeExit leaves them as they are. Random battles,
/// Retreat and the script commands that set a destination
/// (Event_SetDestination) raise it, and SceneManager_Run returns it when
/// the whole party falls; Scene_TakeExit also uses it for "no exit found"
/// (sequence_manager.c, scene_manager.c).
#define EXIT_KEEP_DESTINATION 999

/// gSaveBlock.movementMode: how the map loop hands the party leader back to
/// the player (SceneManager_Run).
enum LeaderMovementMode {
  /// Walking: the pad moves the leader, on the world map or a layered map.
  LEADER_MOVEMENT_WALK = 0,
  LEADER_MOVEMENT_CLIMB = 1, ///< on a climbable wall, moved 8 pixels a step
  /// Carried through the air (Suhalla Desert's storms, the world map's
  /// airborne travel): only the configured keys are checked.
  LEADER_MOVEMENT_AIRBORNE = 2
};

/// SequenceManager_Run's mode: where the game starts.
enum GameStart {
  GAME_START_TITLE = 0,      ///< the title screen, with a fresh save block
  GAME_START_DEBUG_VALE = 1, ///< in debug mode, SceneId_ValePlaza
  GAME_START_DEBUG_FILE_SCREEN = 2 ///< in debug mode, the main menu
};

/// A window transition's shape: SCENE_TRANSITION(TRANSITION_WINDOW, mode)'s
/// low byte, which ScreenTransition_UpdateScanlineTable draws.
enum WindowTransitionMode {
  WINDOW_TRANSITION_BARS = 0,   ///< a bar on each line, at random
  WINDOW_TRANSITION_SHAPE = 1,  ///< a shape from gScreenTransition_Shapes
  WINDOW_TRANSITION_NOISE = 2,  ///< ragged lines from the left edge
  WINDOW_TRANSITION_CIRCLE = 3, ///< a circle at the screen's centre
  /// Alternate lines wiping in from the left and the right.
  WINDOW_TRANSITION_INTERLACED_WIPE = 4,
  /// As WINDOW_TRANSITION_LEADER with inside and outside swapped and the
  /// progress run the other way; it flickers.
  WINDOW_TRANSITION_LEADER_INVERSE = 7,
  /// An ellipse around a point (the state's centre) whose outline ripples:
  /// Reveal's.
  WINDOW_TRANSITION_RIPPLE = 8,
  /// An ellipse wider than tall around the party leader, 16 pixels above its
  /// feet.
  WINDOW_TRANSITION_LEADER = 9,
  /// An ellipse a little taller than wide, 8 pixels above the leader's feet.
  WINDOW_TRANSITION_LEADER_LOW = 77
};

/// The encounter table's zones (encounter_zones.c's entries) that code names:
/// the rows no step on a map rolls.
enum EncounterZoneId {
  ENCOUNTER_ZONE_NONE = 0, ///< no battles
  /// The scripted battles of Vale's stormy night (its monsters, and the
  /// travelers who catch Isaac in Vale's plaza), Vault's thieves and the world
  /// map's Djinn tutorial battle.
  ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT = 12,
  /// The scripted battles of Tret Tree (Tret) and Mercury Lighthouse (the
  /// Lizard Man, then Saturos at the aerie).
  ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY = 36,
  /// The scripted battles of Mogall Forest, Altin Peak and Lamakan Desert,
  /// whose last two slots hold the battles with the Ant Lions and the
  /// Manticore.
  ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN = 53,
  /// The monster attacks on the Tolbi-bound ship, the Kraken's last.
  ENCOUNTER_ZONE_TOLBI_SHIP = 62,
  /// The monsters behind the east doors of Crossbone Isle Cave's first four
  /// floors, one slot a floor.
  ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS = 63,
  /// The battles that end the Colosso finals' matches, the slot chosen by
  /// how the match went.
  ENCOUNTER_ZONE_COLOSSO = 72,
  /// The monsters behind the east doors of Crossbone Isle Cave's next five
  /// floors, one slot a floor, then Deadbeard on the Pirate Ship.
  ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS = 84,
  /// The scripted battles: Suhalla Desert, Venus Lighthouse Aerie and the
  /// world map; its first slot is the Aerie's second battle.
  ENCOUNTER_ZONE_SCRIPTED = 98,
  ENCOUNTER_ZONE_MIMIC = 99, ///< mimic chests, by the chest's slot
  /// The Djinn battles, one zone for each element in element order.
  ENCOUNTER_ZONE_VENUS_DJINN = 100,
  ENCOUNTER_ZONE_MERCURY_DJINN = 101,
  ENCOUNTER_ZONE_MARS_DJINN = 102,
  ENCOUNTER_ZONE_JUPITER_DJINN = 103
};

void Scene_TakeExit(s32 exitId);
void Scene_ReturnFromSequence(s32 result);
s32 Scene_GetArea(s32 sceneId);
void SequenceManager_Run(s32 mode);
void Scene_LoadOverlay(s32 unused);
void Scene_Arrive(s32 unused, s32 keep);
void Scene_PlayMusic(void);
void Encounter_SelectMapZones(s32 useRules);
s32 Encounter_ResolveWorldMapZoneAndBackdrop(void *pos);
s32 Encounter_CountStepsAndSelectFormation(s32 zone, s32 steps);
s32 Encounter_RollMapStep(u32 terrain, s32 steps);
s32 Encounter_RollWorldMap(s32 pos, s32 steps);
u16 Encounter_GetFormationAtSlot(s32 zone, s32 slot);
s16 Encounter_GetDjinnFormation(s32 djinni);
void Scene_ChooseMusic(void);
s32 Area_GetNameIndex(s32 sceneId, s32 entrance);
void Scene_SetRestartPoint(void);
s16 Event_GetSpecialValue(void);
void BattleBackground_SelectForMap(void);
void BattleBackground_SelectForEnvironment(s32 behavior);
// Matching builds call Encounter_SelectBattleMusic without a prototype, as in
// overworld.c above its definition: in one file its definition
// would come before Field_RequestDjinnBattle and
// Field_RequestBattleFromEncounterTable, and Field_RequestDjinnBattle changes
// (2 of 52 instructions in tbs-en). Split evidence for
// overworld|field_services.
#ifdef UBFIX
void Encounter_SelectBattleMusic(s32 zone, s32 slot);
#endif
void FieldEffect_ShrinkActorAndDestroySlow(struct Actor *actor);
void FieldEffect_ShrinkActorAndDestroyFast(struct Actor *actor);
void FieldPsynergy_Lift(void);
void FieldPsynergy_Carry(void);

/// One of the map's events (the same 12 bytes as struct Trigger).
struct TriggerView {
  s32 flags; ///< the low 4 bits are the kind (TriggerKind; 7 talks to
             ///< the actor the party stopped)
  u8 value;  ///< the target (an actor id or a metatile behavior)
  u8 unused05;
  s16 condition; ///< as for GameFlag_IsConditionActive
  s32 result;    ///< below 0x10000 a message, else a function to call
};

#define ACTOR_LAST 66 ///< the last actor slot searched

//--------------------------------------------------------------------------
// The map's treasure spots (chests and Psynergy Stones)
//--------------------------------------------------------------------------
#define TREASURE_SPOT_MAX 10

s32 Party_RemapCharacterIdByFlags(s32 character);
s32 Party_RemapCharacterIdIfEnabled(s32 character, s32 childhood);
void Event_SpawnActorTemplates(struct ActorTemplate *entry, s32 slot);
void Actor_ReleaseAtIndex(s32 actorIndex);
void Actor_ResetForLeader(struct ActorTemplate *table);
s32 Actor_FindLastActiveId(void);
void *Actor_GetSlotAddress(u32 actorIndex);
void Scene_AssignViewFlags(struct ActorTemplate *entry);
void Actor_ClearOutOfBounds(void);
void Actor_Respawn(void);
void Actor_ClearAll(void);
void *Actor_Lookup(u32 actorIndex);
void Actor_Snapshot(void);
void Actor_Restore(void);

void Field_ClearRequests(void);
s32 Field_CountRequests(void);
s32 Field_GetCollisionAheadOfLeader(void);
void Field_CheckExitRegions(s32 positionX, s32 positionY, s32 positionZ);
void Field_UpdateWalking(s32 layer, s32 x, s32 y, s32 z);
void Party_AddPp(s32 amount, s32 unused);
void Party_AddHp(s32 amount, s32 percent);
s32 Party_ApplyPoisonDamage(void);
void WorldMap_Freeze(void);
void WorldMap_Unfreeze(void);
s32 SceneManager_Run(s32 entrance);
s32 Actor_GetTriggerCellAheadOfCurrent(void);
void Scene_ApplyEntrance(void);
void PaletteEditor_Run(void);

struct ActorTemplate *Scene_FindActorTemplate(s32 id);
s32 GameFlag_IsConditionActive(s32 condition);
u32 Scene_ExamineTriggerIgnoresCondition(s32 flags, s32 result);
struct TriggerView *Scene_FindTrigger(s32 kind, s32 target);
void Overlay_CallInitialize(void);
s32 Scene_FindActorTalkTrigger(s32 id);
s32 Scene_TalkToActor(s32 id);
s32 Scene_RunRaisedTrigger(s32 value);
s32 Scene_RunStepTriggerOrExit(s32 value);
s32 Field_RunPushCell(s32 behavior);
s32 Field_ExamineCell(s32 tile);
s32 Field_GetPsynergyTargetRange(s32 kind);
s32 Field_FindNearbyTalkTarget(s32 id);
s32 Field_FindNearbyPsynergyTarget(s32 id, s32 kind);
s32 FieldEffect_PlayCueAndStartEmitterOnTarget(s32 actorId, s32 target,
                                               s32 itemId);
void Anim_ApplyChildPalette(struct Actor *actor, s32 palette);
void Field_EndBusyAndReveal(void);
s32 Field_FindItemTrigger(s32 item);
s32 Field_UseItem(s32 packedItem, s32 slot);
void *Scene_FindPsynergyTrigger(s32 eventFlags, s32 group, void *target);
s32 Field_RunPsynergyEndEvents(s32 packed);
s32 Field_UsePsynergy(u32 code);
s32 Scene_HasFieldPsynergyTrigger(s32 group);
u32 Scene_HasPsynergyTrigger(u16 item);

void Actor_SnapToTerrain(void *object);
void Field_PlaceTreasureSpots(void);
s32 TreasureSpot_FindSlot(s32 id);
void TreasureSpot_ReservedNoOp(void);
void TreasureSpot_CloseChest(s32 id);
void TreasureSpot_LaunchMimic(s32 id);
void TreasureSpot_OpenChest(s32 id);
void TreasureSpot_ShowOpened(s32 id);
struct Actor *TreasureSpot_GetActor(s32 id);
void TreasureSpot_Hide(s32 id);
void TreasureSpot_SetActorPosition(s32 id, s32 x, s32 z);

void TreasureSpot_MoveLeaderAway(void);
void ItemDisplay_EmitSparkles(struct Actor *actor);
struct Actor *FieldEffect_StartRandomParticleEmitter(s32 actorId, s32 item);
void Actor_ReleaseIfPresent(void *object);
void ItemDisplay_RaiseAboveLeader(struct Actor *actor);
void ItemDisplay_ShowPicture(void *object, s32 flags);
void ItemDisplay_ShowItem(s32 item, s32 flags);
void FieldEffect_ClearRandomParticles(void);
void Reveal_EmitSparkles(struct Actor *actor);
s32 Field_IsRevealActive(struct Actor *interpreter);
void FieldEffect_SpawnNearbyMarkers(void);

void FieldEffect_StartWindowHBlankDma(void);
void ScreenTransition_UpdateScanlineTable(void);
void ScreenTransition_InitializeBattleEffectState(s32 mode);
void ScreenTransition_SuspendCallbacks(void);
struct ScreenTransitionState *ScreenTransition_AllocateAndClearState(void);
void ScreenTransition_Start(s32 mode, s32 frames);
void ScreenTransition_Finish(s32 mode, s32 frames);
void ScreenTransition_HideBackgrounds(s32 clearBg3, s32 clearBg2, s32 clearBg1);
void ScreenTransition_Update(void);
void ScreenTransition_UpdateFromCentre(void);
void ScreenTransition_UpdateScanline(void);
void ScreenTransition_UpdateFrame(void);
void ScreenTransition_FillTilemapAndSolidTile(s32 color);
void ScreenTransition_InitializeState(s32 value);

void PaletteFade_CalculateSteps(s16 *source, s16 *target, s16 *step,
                                s32 frames);
void PaletteFade_Update(void);
// UB: field_services.c calls PaletteFade_Build with no declaration in
// scope, as a function returning int. With its prototype in scope (as in one
// file with its definition) Weather_StartLightning and
// Weather_StartLightningFlash change, 6 of 67 and 6 of 74 instructions in
// tbs-en, and even a declaration without a prototype moves two instructions
// in tbs-ja: evidence that overworld.c and field_services.c were separate
// files. UBFIX declares it.
#ifdef UBFIX
void PaletteFade_Build();
#endif
void PaletteFade_Initialize(void);
void PaletteFade_Release(void);
void PaletteFade_SetTarget(s32 mode, s32 half);
void PaletteFade_SetSource(s32 mode, s32 half);
void PaletteFade_SetBackdrop(u32 color);
void PaletteFade_StartInterpolation(s32 frames);
s32 PaletteFade_ClampComponent(s32 value);
s32 PaletteFade_ClampShiftedComponent(s32 value);
void FadeOverlay_DrawMarks(void);
void FieldEffect_StartFadeOverlay(void *object);
void FadeOverlay_Suspend(void);
void FadeOverlay_Resume(void);
struct SpeakerDefinition *Speaker_FindDefinition(u32 id);
s32 FieldEffect_GetAnimationValue(s32 id);
s32 Speaker_GetVoice(u32 id);
s32 Speaker_GetIdleFlags(s32 id);

#endif
