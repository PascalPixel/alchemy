#ifndef GAME_H
#define GAME_H

/* GAME */

#include "types.h"
#include "edition.h"
#include "battle_summon.h"
#include "battle_unit.h"
#include "item.h"
#include "character_ids.h"
#if !GAME_TLA
#include "character.h"
#include "preset_table.h"
#endif
#if GAME_TLA
#include "battle_type.h"
#endif
#if !GAME_TLA
#include "farcall.h"
#endif

struct ItemDefinition;
struct SummonDefinition;

extern struct ItemDefinition gItem_DefinitionTable[];
extern const struct SummonDefinition gSummon_DefinitionTable[];
#if GAME_TLA
s32 Djinn_IsReady(s32 owner, s32 index, s32 bit);
#endif

#if !GAME_TLA
struct BattleAct;
struct EnemyDefinition;
struct PresetValues;

extern struct BattleAct gBattleAction_DefinitionTable[];
extern const u8 gCharacter_ElementGroupTable[];
extern u32 gCharacter_LevelExpTable[];
extern s16 gDjinn_ElementSprites[];
extern s32 gOwner_FourColumnValues[];
extern const u16 gDjinn_DefinitionTable[];
extern const s16 gElement_PowerResistCurve[5][2];
extern const struct EnemyDefinition gEnemy_Definitions[];
extern struct PresetValues gEnemy_ElementPresetTable[];
extern u8 gGameFlagBytes[];
extern u16 gInventorySnapshot[];
extern u8 gItemCounters[128];
extern u8 gSaveStamp[];
extern u8 gItem_ArtifactSlotTable[];
extern s16 gCharacter_SpriteTable[];
extern s16 gCharacter_ChildSpriteTable[];
extern u8 gSummon_OrderList[16];
s32 Audio_Check(void);
#ifndef OWN_BattleEffect_ApplyToTargets
#endif
#ifndef OWN_Party_GetAverageLevel
#endif
#ifndef OWN_Party_LookupCharacterValueByFlag32
#endif
#endif

#if GAME_TLA
struct ActionDefinition;
struct BattleElementStats;

extern const u16 gBattle_Effect90Classes[47];
extern const u16 gBattle_Effect91Classes[29];
extern struct ActionDefinition gBattleAction_DefinitionTable[];
extern const u16 gDjinn_Definitions[][6];
extern const u8 gEnemy_DefinitionTable[];
extern u8 gGameFlagBytes[];
void BattlePlacement_UpdateTimedEntriesTwentyTimesFar(void);
s32 Djinn_AddToOwnerFar(s32, s32, s32);
void Event_ClearInvalidPackedValuesFar(s32);
s32 Inventory_AddItemFar(s32, s32);
s32 Inventory_RemoveFar(s32 owner, s32 slot);
void Unit_CalcBaseElements(s32 owner_id, struct BattleElementStats *elements);
s32 Class_Determine(s32 classId, u32 *djinn, s32 item);
s32 Unit_GetElementLevels(s32 record, const u8 *source, s32 *output);
void Unit_RefreshPsynergy(s32);
void Owner_UpdateClassActions(s32 owner_id);
void *Resource_FarCall005(s32);
void Ui_AdjustValueWithoutLimitFar(s32, u16 *);
#endif

#endif
