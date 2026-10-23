#ifndef ALCHEMY_UNIT_OVERWORLD_OVERWORLD_H
#define ALCHEMY_UNIT_OVERWORLD_OVERWORLD_H

#include "types.h"
#include "field_effect_sprite.h"

struct ArcEffectObject;
struct SpeakerDefinition;
struct DescendingOrbitObject;
struct DispatchObject;
struct ScreenTransitionState;
struct Effect05Object;
struct EffectDescriptor;
struct FieldEffectSprite;
struct EffectVector;
struct EffectObject;
struct FacingObject;
struct ObjectEvent;
struct MotionObject;
struct Object;
struct OrbitEffect;
struct ObjectEventTemplate;
struct ObjectScriptContext;
struct SparkObject;
struct Window;
union FieldObject;

/// gGameState.movementMode: how the map loop hands the party leader back to
/// the player (Map_Run).
enum LeaderMovementMode {
  /// Walking: the pad moves the leader, on the world map or a layered map.
  LEADER_MOVEMENT_WALK = 0,
  LEADER_MOVEMENT_CLIMB = 1, ///< on a climbable wall, moved 8 pixels a step
  /// Carried through the air (Suhalla Desert's storms, the world map's
  /// airborne travel): only the configured keys are checked.
  LEADER_MOVEMENT_AIRBORNE = 2
};

/// Game_Run's mode: where the game starts.
enum GameRunMode {
  GAME_RUN_TITLE = 0,          ///< the title screen, with a fresh game state
  GAME_RUN_DEBUG_VALE = 1,     ///< in debug mode, MapId_ValeGate
  GAME_RUN_DEBUG_MAIN_MENU = 2 ///< in debug mode, the main menu
};

/// A window transition's shape: MAP_TRANSITION(TRANSITION_WINDOW, mode)'s
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

/// The encounter table's zones (encounter.tsv's rows) that code names: the
/// rows no step on a map rolls.
enum EncounterZoneId {
  ENCOUNTER_ZONE_NONE = 0, ///< no battles
  /// The scripted battles of Vale's stormy night (its monsters, and the
  /// travelers who catch Isaac at Vale Gate), Vault's thieves and the world
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

void Map_TakeWarp(s32 warpId);
void Map_ReturnFromRun(s32 result);
s32 Map_GetMapSec(s32 mapId);
void Game_Run(s32 mode);
void Map_LoadOverlay(s32 unused);
void Map_ResetFlagsOnEnter(s32 unused, s32 keep);
void Map_PlayMusic(void);
void Encounter_SelectMapZones(s32 useRules);
s32 Encounter_ResolveWorldMapZoneAndBackdrop(void *pos);
s32 Encounter_CountStepsAndSelectFormation(s32 zone, s32 steps);
s32 Encounter_RollLayoutStep(u32 terrain, s32 steps);
s32 Encounter_RollWorldMap(s32 pos, s32 steps);
u16 Encounter_GetFormationAtSlot(s32 zone, s32 slot);
s16 Encounter_GetDjinnFormation(s32 djinni);
void Map_ChooseMusic(void);
s32 MapSec_GetNameIndex(s32 mapId, s32 warpDest);
void Map_SetDefeatReturnPoint(void);
s16 FieldScript_GetSpecialValue(void);
void BattleBackground_SelectForMap(void);
void BattleBackground_SelectForEnvironment(s32 behavior);
// Matching builds call Encounter_SelectBattleMusic without a prototype, as in
// overworld.c above its definition: in one file its definition
// would come before Field_RequestDjinnBattle and
// Field_RequestBattleFromEncounterTable, and Field_RequestDjinnBattle changes
// (2 of 52 instructions in tbs-en). Split evidence for object|psynergy_effects.
#ifdef UBFIX
void Encounter_SelectBattleMusic(s32 zone, s32 slot);
#endif
void FieldEffect_ShrinkObjectAndDestroySlow(union FieldObject *fieldObject);
void FieldEffect_ShrinkObjectAndDestroyFast(union FieldObject *fieldObject);
void FieldEffect_RunLift(void);
void FieldEffect_RunCarry(void);

/// One of the map's events (the same 12 bytes as struct MapEvent).
struct EffectDescriptor {
  s32 flags; ///< the low 4 bits are the kind (MapEventKind; 7 talks to
             ///< the object event the party stopped)
  u8 value;  ///< the target (a local id or a metatile behavior)
  u8 unused05;
  s16 condition; ///< as for GameFlag_IsConditionActive
  s32 result;    ///< below 0x10000 a message, else a function to call
};

#define OBJECT_LAST 66 ///< the last object event slot searched

//--------------------------------------------------------------------------
// The map's treasure spots (chests and Psynergy Stones)
//--------------------------------------------------------------------------
#define TREASURE_SPOT_MAX 10

s32 Party_RemapCharacterIdByFlags(s32 character);
s32 Party_RemapCharacterIdIfEnabled(s32 character, s32 childhood);
void FieldScript_SpawnObjectEventTemplates(struct ObjectEventTemplate *entry,
                                           s32 slot);
void ObjectEvent_DestroyAtIndex(s32 objectEventIndex);
void ObjectEvent_ResetForObject(struct ObjectEventTemplate *table);
s32 ObjectEvent_FindLastActiveId(void);
void *ObjectEvent_GetSlotAddress(u32 objectEventIndex);
void Map_AssignViewFlags(struct ObjectEventTemplate *entry);
void ObjectEvent_ClearOutOfBounds(void);
void ObjectEvent_Respawn(void);
void ObjectEvent_ClearAll(void);
void *ObjectEvent_GetByLocalId(u32 objectEventIndex);
void ObjectEvent_Snapshot(void);
void ObjectEvent_Restore(void);

void Field_ClearRequests(void);
s32 Field_CountRequests(void);
s32 Field_GetCollisionAheadOfLeader(void);
void Field_CheckWarpRegions(s32 positionX, s32 positionY, s32 positionZ);
void Field_ProcessStep(s32 layer, s32 x, s32 y, s32 z);
void Party_AddPp(s32 amount, s32 unused);
void Party_AddHp(s32 amount, s32 percent);
s32 Party_ApplyPoisonDamage(void);
void WorldMap_StartScript(void);
void WorldMap_EndScript(void);
s32 Map_Run(s32 warpDest);
s32 ObjectEvent_GetTriggerTileAheadOfCurrent(void);
void Map_ApplyWarpDest(void);
void PaletteEditor_Run(void);

struct ObjectEventTemplate *Map_FindObjectEventTemplate(s32 id);
s32 GameFlag_IsConditionActive(s32 condition);
u32 Map_BgEventIgnoresCondition(s32 flags, s32 result);
struct EffectDescriptor *Map_FindEvent(s32 kind, s32 target);
void Runtime_CallWorkBlockEntry(void);
s32 Map_FindObjectEventTalkEvent(s32 id);
s32 Map_TalkToObjectEvent(s32 id);
s32 Map_RunRaisedEvent(s32 value);
s32 Map_RunCoordEventOrWarp(s32 value);
s32 Field_RunPushCellEvent(s32 collisionCode);
s32 Field_SearchCell(s32 tile);
s32 Field_GetPsynergyTargetRange(s32 kind);
s32 Field_FindNearbyTalkTarget(s32 id);
s32 Field_FindNearbyPsynergyTarget(s32 id, s32 kind);
s32 FieldEffect_PlayCueAndStartEmitterOnTarget(s32 localId, s32 target,
                                               s32 itemId);
void Animation_ApplyChildPalette(struct MotionObject *object, s32 palette);
void Field_EndBusyAndReveal(void);
s32 Field_FindItemEvent(s32 item);
s32 Field_UseItem(s32 packedItem, s32 slot);
void *Map_FindPsynergyEvent(s32 eventFlags, s32 group, void *target);
s32 Field_RunPsynergyEvents(s32 packed);
s32 Field_UsePsynergy(u32 code);
s32 FieldEffect_HasMatchingEvent5(s32 group);
u32 Map_HasPsynergyEvent(u16 item);

void ObjectMotion_SnapToTerrain(void *object);
void Field_PlaceTreasureSpots(void);
s32 TreasureSpot_FindSlot(s32 id);
void TreasureSpot_ReservedNoOp(void);
void TreasureSpot_CloseChest(s32 id);
void TreasureSpot_LaunchMimic(s32 id);
void TreasureSpot_OpenChest(s32 id);
void TreasureSpot_ShowOpened(s32 id);
struct Object *TreasureSpot_GetObjectEvent(s32 id);
void TreasureSpot_Hide(s32 id);
void TreasureSpot_SetObjectEventPosition(s32 id, s32 x, s32 z);

void TreasureSpot_MoveLeaderAway(void);
void ItemDisplay_EmitSparkles(union FieldObject *fieldObject);
struct EffectObject *FieldEffect_StartRandomParticleEmitter(s32 localId,
                                                            s32 item);
void Object_DestroyIfPresent(void *object);
void ItemDisplay_RaiseAboveLeader(struct Object *object);
void ItemDisplay_ShowPicture(void *object, s32 flags);
void FieldEffect_StartEffectObject22(s32 item, s32 flags);
void FieldEffect_ClearRandomParticles(void);
void Reveal_EmitSparkles(union FieldObject *fieldObject);
s32 Field_IsRevealActive(struct ObjectScriptContext *interpreter);
void FieldEffect_SpawnNearbyMarkers(void);

void FieldEffect_StartWindowHBlankDma(void);
void ScreenTransition_UpdateScanlineTable(void);
void ScreenTransition_InitializeBattleEffectState(s32 mode);
void ScreenTransition_SuspendCallbacks(void);
struct ScreenTransitionState *ScreenTransition_AllocateAndClearState(void);
void ScreenTransition_Start(s32 mode, s32 frames);
void ScreenTransition_Finish(s32 mode, s32 frames);
void DisplayState_ClearFlags(s32 clearBg3, s32 clearBg2, s32 clearBg1);
void ScreenTransition_Update(void);
void ScreenTransition_UpdateFromCentre(void);
void ScreenTransition_UpdateScanline(void);
void ScreenTransition_UpdateFrame(void);
void ScreenTransition_FillTilemapAndSolidTile(s32 color);
void ScreenTransition_InitializeState(s32 value);

void PaletteFade_CalculateSteps(s16 *source, s16 *target, s16 *step,
                                s32 frames);
void PaletteFade_Update(void);
// UB: psynergy_effects.c calls PaletteFade_Build with no declaration in
// scope, as a function returning int. With its prototype in scope (as in one
// file with its definition) Weather_StartLightning and
// Weather_StartLightningFlash change, 6 of 67 and 6 of 74 instructions in
// tbs-en, and even a declaration without a prototype moves two instructions
// in tbs-ja: evidence that overworld.c and psynergy_effects.c were separate
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
