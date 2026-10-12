#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT_H

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
void BattleFx_SelectBattleCue(s32 first, s32 second);
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
s32 BattleFx_FindMatchingEvent(s32 requested_flags, s32 group, void *result);
s32 BattleFx_ExecutePackedAbilityEffect(s32 packed);
s32 BattleCommand_ExecuteSelectedAction(u32 code);
s32 BattleFx_HasMatchingEvent5(s32 group);
u32 BattleFx_HasTrigger(u16 item);
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

struct ActionDescriptor;

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

static __inline__ s32 Vitals_Ratio(s32 value, s32 max)
{
	s32 ratio = (value << 14) / max;
	s32 clamped;

	if (ratio > 0x4000)
		clamped = 0x4000;
	else if (ratio < 0)
		clamped = 0;
	else
		clamped = ratio;
	return clamped;
}

//--------------------------------------------------------------------------
// マップのマーカー物体
//--------------------------------------------------------------------------
#define MARKER_MAX 10

#endif
