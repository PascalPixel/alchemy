#ifndef ALCHEMY_UNIT_OBJECT_OBJECT_H
#define ALCHEMY_UNIT_OBJECT_OBJECT_H

#include "types.h"
#include "effect_slot.h"

struct ActionDescriptor;
struct ArcEffectObject;
struct BattleEffectEntry;
struct DescendingOrbitObject;
struct DispatchObject;
struct DisplayTransitionState;
struct Effect05Object;
struct EffectDescriptor;
struct EffectSlot;
struct EffectVector;
struct EfxObj;
struct FacingObject;
struct FieldActor;
struct MotionObject;
struct ObjectRuntime;
struct OrbitEffect;
struct ScenePlacement;
struct SceneRegionEntry;
struct SparkObject;
struct UiWindow;
union FieldObject;

void MapGroupTable_SelectEntry(s32 kind);
void Party_SetReturnPoint(s32 reason);
s32 BattleFx_GetResourceGroup(s32 scene);
void Game_ResetForNewGame(s32 mode);
void BattleFx_LoadResourceGroup(s32 unused);
void Scene_ResetFlagsOnEnter(s32 unused, s32 keep);
void Audio_PlayCueFromEventWork(void);
void BattleFx_SelectLocationRule(s32 use_rules);
s32 BattleFx_LookupResult(void *pos);
s32 Encounter_SelectEnemyGroup(s32 zone, s32 steps);
s32 EffectRuntime_LookupByTableEntry(u32 terrain, s32 steps);
s32 BattleFx_ApplyLookupResult(s32 pos, s32 steps);
u16 BattleFx_GetWeightedResult(s32 zone, s32 slot);
s16 BattleFx_GetPhaseResult(s32 no);
void Scene_ResolveInteractionResult(void);
s32 Scene_GetAreaNameIndex(s32 scene, s32 entrance);
void Party_ResolveTablePair(void);
s16 Event_GetSpecialValue(void);
void BattleFx_SetSpecialFromTable(void);
void BattleFx_SelectResultPointer(s32 arg0);
/* BattleFx_SelectBattleCue is called before its definition with no prototype in scope. */
void BattleFx_ShrinkObjectAndDestroySlow(void *obj);
void BattleFx_ShrinkObjectAndDestroyFast(void *obj);
void RunBattleEffect07(void);
void RunBattleEffect11(void);

struct ActionDescriptor
{
	u16 sprite;
	s16 condition;
	s32 behavior;
	s32 x;
	s32 y;
	s32 z;
	u16 facing;
	u8	talk_facing;
	u8	flags;
};

// シーンのイベント表の 1 件（struct SceneEvent と同じ 12 バイト）
struct EffectDescriptor
{
	s32 flags; // 下位 4bit が種類（SceneEventKind、7 は止めた相手と話す）
	u8	value; // 相手（オブジェクト番号など）
	u8	unused_05;
	s16 condition; // GameFlag_IsConditionActive の条件
	s32 result;	   // 0x10000 未満はメッセージ、それ以上は呼ぶ関数
};

#define OBJ_LAST 66 // 調べるオブジェクト番号の最後

//--------------------------------------------------------------------------
// マップのマーカー物体
//--------------------------------------------------------------------------
#define MARKER_MAX 10

#define MODE_WORLD_MAP 3 // ワールドマップ

s32 Party_RemapCharacterIdByFlags(s32 arg0);
s32 Party_RemapCharacterIdIfEnabled(s32 arg0, s32 arg1);
void Event_SpawnObjectTable(struct ScenePlacement *entry, s32 slot);
void ObjectTable_DestroyAtIndex(s32 index);
void ObjectTable_ResetForObject(struct ScenePlacement *table);
s32 ObjectTable_FindLastActiveId(void);
void *ObjectTable_GetSlotAddress(u32 index);
void Scene_AssignViewFlags(struct SceneRegionEntry *entry);
void BattleEffect_ClearOutOfBoundsObjects(void);
void BattleEffect_ClearAllObjects(void);
void ObjectTable_ClearBattleSlots(void);
void *ObjectTable_Get(u32 index);
void ObjectTable_Snapshot(void);
void ObjectTable_Restore(void);

void BattleFx_ResetCounters(void);
s32 BattleFx_SumCounters(void);
s32 GetFocusedObjectCollision(void);
void UpdateMapRegionAtPosition(s32 position_x, s32 position_y, s32 position_z);
void Field_ProcessStep(s32 layer, s32 x, s32 y, s32 z);
void BattleParty_ApplyDrain(s32 amount, s32 unused);
void BattleParty_ApplyHealthDelta(s32 amount, s32 scaled);
s32 BattleParty_ApplyStatusDamage(void);
void Battle_SetObjectFlag5bWhenMode3(void);
void Battle_ClearObjectFlag5bWhenMode3(void);
s32 Field_RunScene(s32 entrance);
s32 Object_GetTriggerTileAheadOfCurrent(void);
void BattleMap_ApplyEntranceView(void);
void Debug_RunPaletteEditor(void);

struct ActionDescriptor *BattleAction_FindDescriptor(s32 id);
s32 GameFlag_IsConditionActive(s32 condition);
u32 BattleFx_CheckDescriptorKind3Result(s32 flags, s32 result);
struct EffectDescriptor *BattleFx_FindDescriptor(s32 kind, s32 value);
void Runtime_CallWorkBlockEntry(void);
s32 BattleFx_FindDescriptorWithOverride(s32 id);
s32 BattleFx_RunDescriptorAction(s32 id);
s32 BattleFx_RunKind6DescriptorAction(s32 value);
s32 BattleAction_RunDescriptor(s32 value);
s32 Battle_DispatchInputEvent(s32 event);
s32 Field_RunTileAction(s32 tile);
s32 BattleFx_MapKeyThroughTable(s32 mode);
s32 BattleEffect_SelectNearbyObject(s32 id);
s32 BattleEffect_SelectNearbyTargetObject(s32 id, s32 mode);
s32 BattleFx_PlayCueAndStartEmitterOnTarget(s32 effect, s32 target, s32 mode);
void Animation_ApplyChildPalette(struct MotionObject *obj, s32 palette);
void Battle_ResetEffectCounter(void);
s32 Event_FindFacingTrigger(s32 source);
s32 BattleCommand_ExecuteSelectedItem(s32 arg, s32 slot);
void *BattleFx_FindMatchingEvent(s32 requested_flags, s32 group, void *result);
s32 BattleFx_ExecutePackedAbilityEffect(s32 packed);
s32 BattleCommand_ExecuteSelectedAction(u32 code);
s32 BattleFx_HasMatchingEvent5(s32 group);
u32 BattleFx_HasTrigger(u16 item);

void ObjectMotion_SnapToTerrain(void *object);
void Battle_PlaceMapMarkers(void);
s32 EffectRuntime_FindSlotByObject(s32 id);
void EffectRuntime_ReservedNoOp(void);
void EffectRuntime_SetMode5AndPlayCue(s32 id);
void EffectRuntime_SetMode7AndLaunch(s32 id);
void EffectRuntime_SetMode4AndPlayCue(s32 id);
void EffectRuntime_SetMode2(s32 id);
struct ObjectRuntime *EffectRuntime_GetCurrentObject(s32 id);
void EffectRuntime_ClearCurrentFlags(s32 id);
void EffectRuntime_SetCurrentPosition(s32 id, s32 x, s32 y);

void BattleFx_EmitRandomParticle(void);
void BattleFx_EmitRandomParticleFromEmitter(struct ObjectRuntime *emitter);
struct EfxObj *BattleFx_StartRandomParticleEmitter(s32 actor, s32 item);
void Object_DestroyIfPresent(void *object);
void EffectRuntime_PrepareRisingObject(struct ObjectRuntime *object);
void EffectRuntime_RunRisingObjectSequence(void *object, s32 flags);
void BattleFx_StartEffectObject22(s32 value, s32 flags);
void BattleFx_ClearRandomParticles(void);
void BattleFx_SpawnRandomParticleAtPosition(const struct FieldActor *source);
u32 EffectRuntime_IsActive(void);
void FieldEffect_SpawnNearbyMarkers(void);

void BattleFx_StartWindowHBlankDma(void);
void DisplayTransition_UpdateScanlineTable(void);
void DisplayTransition_InitializeBattleEffectState(s32 mode);
void BattleFx_EnableTwoCallbacks(void);
struct DisplayTransitionState *DisplayTransition_AllocateAndClearState(void);
void DisplayTransition_Start(s32 mode, s32 frames);
void DisplayTransition_Finish(s32 mode, s32 frames);
void DisplayState_ClearFlags(s32 clear_0800, s32 clear_0400, s32 clear_0200);
void DisplayTransition_Update(void);
void DisplayTransition_UpdateFromCentre(void);
void DisplayTransition_UpdateScanline(void);
void DisplayTransition_UpdateFrame(void);
void DisplayTransition_FillTilemapAndSolidTile(s32 color);
void DisplayTransition_InitializeState(s32 value);

void BattleFx_InterpolateBuffers(s16 *source, s16 *target, s16 *step, s32 frames);
void BattlePalette_UpdateBlend(void);
/* BattleFx_BuildBuffer is called before its definition with no prototype in scope. */
void BattleEffect_InitializeBuffers(void);
void Runtime_ScheduleCallbackAndReleaseBlock32B(void);
void BattleFx_ApplyColorToTargetBuffer(s32 mode, s32 half);
void BattleFx_ApplyColorToSourceBuffer(s32 mode, s32 half);
void BattleFx_SetPrimaryBufferValue(u32 col);
void BattleFx_StartBufferInterpolation(s32 frames);
s32 BattleFx_ClampRgb555Channel(s32 value);
s32 BattleFx_ClampRgb555Component(s32 value);
void Object_EffectSpawnCallback(void);
void BattleFx_StartFadeOverlay(void *object);
void Object_EnableEffectSpawnCallback(void);
void Object_DisableEffectSpawnCallback(void);
struct BattleEffectEntry *BattleFx_FindDefinition(u32 id);
s32 BattleFx_GetAnimationValue(s32 id);
s32 BattleFx_GetResourceId(u32 id);
s32 BattleFx_GetFlags(s32 id);

#endif
