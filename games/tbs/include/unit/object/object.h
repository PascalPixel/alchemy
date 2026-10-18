#ifndef ALCHEMY_UNIT_OBJECT_OBJECT_H
#define ALCHEMY_UNIT_OBJECT_OBJECT_H

#include "types.h"
#include "effect_slot.h"

struct ArcEffectObject;
struct BattleEffectEntry;
struct DescendingOrbitObject;
struct DispatchObject;
struct DisplayTransitionState;
struct Effect05Object;
struct EffectDescriptor;
struct EffectSlot;
struct EffectVector;
struct EffectObj;
struct FacingObject;
struct FieldActor;
struct MotionObject;
struct ObjectRuntime;
struct OrbitEffect;
struct ScenePlacement;
struct ScriptInterpreter;
struct SparkObject;
struct UiWindow;
union FieldObject;

void Scene_TakeExit(s32 exitKind);
void Scene_ReturnFromBattle(s32 result);
s32 BattleFx_GetResourceGroup(s32 scene);
void Game_ResetForNewGame(s32 mode);
void Scene_LoadOverlay(s32 unused);
void Scene_ResetFlagsOnEnter(s32 unused, s32 keep);
void Audio_PlayCueFromEventWork(void);
void Encounter_SelectSceneZones(s32 useRules);
s32 Encounter_ResolveWorldMapZoneAndBackdrop(void *pos);
s32 Encounter_SelectEnemyGroup(s32 zone, s32 steps);
s32 Encounter_RollTerrain(u32 terrain, s32 steps);
s32 Encounter_RollWorldMap(s32 pos, s32 steps);
u16 Encounter_GetEnemyGroupAtSlot(s32 zone, s32 slot);
s16 Encounter_GetDjinnEnemyGroup(s32 djinni);
void Scene_ResolveInteractionResult(void);
s32 Scene_GetAreaNameIndex(s32 scene, s32 entrance);
void Scene_SetDefeatReturnPoint(void);
s16 Event_GetSpecialValue(void);
void Battle_SelectSceneBackdrop(void);
void Battle_SelectTerrainBackdrop(s32 behavior);
// Matching builds call BattleFx_SelectBattleCue without a prototype, as in
// object.c above its definition: in one file its definition
// would come before Field_RequestDjinnBattle and
// Field_RequestBattleFromEncounterTable, and Field_RequestDjinnBattle changes
// (2 of 52 instructions in tbs-en). Split evidence for object|psynergy_effects.
#ifdef UBFIX
void BattleFx_SelectBattleCue(s32 zone, s32 slot);
#endif
void BattleFx_ShrinkObjectAndDestroySlow(union FieldObject *object);
void BattleFx_ShrinkObjectAndDestroyFast(union FieldObject *object);
void RunBattleEffect07(void);
void RunBattleEffect11(void);

// One of the scene's events (the same 12 bytes as struct SceneEvent).
struct EffectDescriptor {
  s32 flags; // the low 4 bits are the kind (SceneEventKind; 7 talks to
             // the actor the party stopped)
  u8 value;  // the target (an actor or a tile code)
  u8 unused05;
  s16 condition; // as for GameFlag_IsConditionActive
  s32 result;    // below 0x10000 a message, else a function to call
};

#define OBJ_LAST 66 // the last actor slot searched

//--------------------------------------------------------------------------
// The map's marker actors (chests and Psynergy stones)
//--------------------------------------------------------------------------
#define MARKER_MAX 10

#define MODE_WORLD_MAP 3 // the world map

s32 Party_RemapCharacterIdByFlags(s32 character);
s32 Party_RemapCharacterIdIfEnabled(s32 character, s32 childhood);
void Event_SpawnObjectTable(struct ScenePlacement *entry, s32 slot);
void ObjectTable_DestroyAtIndex(s32 actorIndex);
void ObjectTable_ResetForObject(struct ScenePlacement *table);
s32 ObjectTable_FindLastActiveId(void);
void *ObjectTable_GetSlotAddress(u32 actorIndex);
void Scene_AssignViewFlags(struct ScenePlacement *entry);
void BattleEffect_ClearOutOfBoundsObjects(void);
void BattleEffect_ClearAllObjects(void);
void ObjectTable_ClearAll(void);
void *ObjectTable_Get(u32 actorIndex);
void ObjectTable_Snapshot(void);
void ObjectTable_Restore(void);

void Field_ClearRequests(void);
s32 Field_CountRequests(void);
s32 Field_GetCollisionAheadOfLeader(void);
void Field_CheckExitRegions(s32 positionX, s32 positionY, s32 positionZ);
void Field_ProcessStep(s32 layer, s32 x, s32 y, s32 z);
void BattleParty_ApplyDrain(s32 amount, s32 unused);
void BattleParty_ApplyHealthDelta(s32 amount, s32 percent);
s32 BattleParty_ApplyStatusDamage(void);
void Battle_SetObjectFlag5bWhenMode3(void);
void Battle_ClearObjectFlag5bWhenMode3(void);
s32 Field_RunScene(s32 entrance);
s32 Object_GetTriggerTileAheadOfCurrent(void);
void Scene_ApplyEntrance(void);
void Debug_RunPaletteEditor(void);

struct ScenePlacement *Scene_FindActorPlacement(s32 id);
s32 GameFlag_IsConditionActive(s32 condition);
u32 Event_SearchIgnoresCondition(s32 flags, s32 result);
struct EffectDescriptor *Scene_FindEvent(s32 kind, s32 target);
void Runtime_CallWorkBlockEntry(void);
s32 Scene_FindActorTalkEvent(s32 id);
s32 Scene_TalkToActor(s32 id);
s32 Scene_RunRaisedEvent(s32 value);
s32 Scene_RunTouchOrExit(s32 value);
s32 Field_RunPushTileEvent(s32 collisionCode);
s32 Field_SearchTile(s32 tile);
s32 Field_GetPsynergyTargetRange(s32 kind);
s32 Field_FindNearbyTalkTarget(s32 id);
s32 Field_FindNearbyPsynergyTarget(s32 id, s32 kind);
s32 BattleFx_PlayCueAndStartEmitterOnTarget(s32 actor, s32 target, s32 itemId);
void Animation_ApplyChildPalette(struct MotionObject *object, s32 palette);
void Battle_ResetEffectCounter(void);
s32 Field_FindItemEvent(s32 item);
s32 Field_UseItem(s32 packedItem, s32 slot);
void *BattleFx_FindMatchingEvent(s32 eventFlags, s32 group, void *target);
s32 Field_RunPsynergyEvents(s32 packed);
s32 Field_UsePsynergy(u32 code);
s32 BattleFx_HasMatchingEvent5(s32 group);
u32 BattleFx_HasTrigger(u16 item);

void ObjectMotion_SnapToTerrain(void *object);
void Field_PlaceMapMarkers(void);
s32 Marker_FindSlot(s32 id);
void EffectRuntime_ReservedNoOp(void);
void Marker_CloseChest(s32 id);
void Marker_LaunchMimic(s32 id);
void Marker_OpenChest(s32 id);
void Marker_ShowOpened(s32 id);
struct ObjectRuntime *Marker_GetActor(s32 id);
void Marker_Hide(s32 id);
void Marker_SetActorPosition(s32 id, s32 x, s32 z);

void Marker_MoveLeaderAway(void);
void ItemDisplay_EmitSparkles(union FieldObject *fieldObject);
struct EffectObj *BattleFx_StartRandomParticleEmitter(s32 actor, s32 item);
void Object_DestroyIfPresent(void *object);
void ItemDisplay_RaiseAboveLeader(struct ObjectRuntime *object);
void EffectRuntime_RunRisingObjectSequence(void *object, s32 flags);
void BattleFx_StartEffectObject22(s32 item, s32 flags);
void BattleFx_ClearRandomParticles(void);
void RevealMarker_EmitSparkles(union FieldObject *fieldObject);
s32 Field_IsRevealActive(struct ScriptInterpreter *interpreter);
void FieldEffect_SpawnNearbyMarkers(void);

void BattleFx_StartWindowHBlankDma(void);
void DisplayTransition_UpdateScanlineTable(void);
void DisplayTransition_InitializeBattleEffectState(s32 mode);
void DisplayTransition_SuspendCallbacks(void);
struct DisplayTransitionState *DisplayTransition_AllocateAndClearState(void);
void DisplayTransition_Start(s32 mode, s32 frames);
void DisplayTransition_Finish(s32 mode, s32 frames);
void DisplayState_ClearFlags(s32 clearBg3, s32 clearBg2, s32 clearBg1);
void DisplayTransition_Update(void);
void DisplayTransition_UpdateFromCentre(void);
void DisplayTransition_UpdateScanline(void);
void DisplayTransition_UpdateFrame(void);
void DisplayTransition_FillTilemapAndSolidTile(s32 color);
void DisplayTransition_InitializeState(s32 value);

void BattleFx_InterpolateBuffers(s16 *source, s16 *target, s16 *step,
                                 s32 frames);
void BattlePalette_UpdateBlend(void);
// BattleFx_BuildBuffer is called from psynergy_effects.c with no prototype in
// scope: in one file its definition would come before
// FieldFx_StartLightning and BattleFx_StartBufferBlend, and they change
// (6 of 67 and 6 of 74 instructions in tbs-en). Split evidence for
// object|psynergy_effects.
void BattleEffect_InitializeBuffers(void);
void BattlePalette_StopBlendAndReleaseBuffer(void);
void BattleFx_ApplyColorToTargetBuffer(s32 mode, s32 half);
void BattleFx_ApplyColorToSourceBuffer(s32 mode, s32 half);
void BattleFx_SetPrimaryBufferValue(u32 color);
void BattleFx_StartBufferInterpolation(s32 frames);
s32 BattleFx_ClampRgb555Channel(s32 value);
s32 BattleFx_ClampRgb555Component(s32 value);
void FadeOverlay_DrawMarks(void);
void BattleFx_StartFadeOverlay(void *object);
void FadeOverlay_Suspend(void);
void FadeOverlay_Resume(void);
struct BattleEffectEntry *BattleFx_FindDefinition(u32 id);
s32 BattleFx_GetAnimationValue(s32 id);
s32 BattleFx_GetResourceId(u32 id);
s32 BattleFx_GetFlags(s32 id);

#endif
