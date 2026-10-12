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

#define MODE_WORLD_MAP 3 // ワールドマップ

#endif
