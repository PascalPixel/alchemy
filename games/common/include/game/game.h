#ifndef GAME_H
#define GAME_H

/* GAME */

#include "types.h"
#include "edition.h"
#include "btlsmn.h"
#include "battle_unit.h"
#include "item.h"
#include "chara.h"
#if !GAME_TLA
#include "character.h"
#include "preset_table.h"
#endif
#if GAME_TLA
#include "btltype.h"
#endif
#if !GAME_TLA
#include "farcall.h"
#endif

struct ItemDefinition;
struct SummonDefinition;

extern struct ItemDefinition		 Item_DefinitionTable[];
extern const struct SummonDefinition Summon_DefinitionTable[];
#if GAME_TLA
s32 Trade_CanOfferDjinn(s32 owner, s32 index, s32 bit);
#endif

#if !GAME_TLA
struct BtlAct;
struct EnemyDefinition;
struct PresetValues;

extern struct BtlAct				BattleAction_DefinitionTable[];
extern const u8						Character_ElementGroupTable[];
extern u32							Character_LevelExpTable[];
extern s16							Djinn_ElementSprites[];
extern s32							Owner_FourColumnValues[];
extern const u16					Djinn_DefinitionTable[];
extern const s16					Element_PowerResistCurve[5][2];
extern const struct EnemyDefinition Enemy_Definitions[];
extern struct PresetValues			Enemy_ElementPresetTable[];
extern u8							GameFlagBytes[];
extern u16							gInventorySnapshot[];
extern u8							gItemCounters[128];
extern u8							gSaveStamp[];
extern u8							Item_ArtifactSlotTable[];
extern s16							Character_SpriteTable[];
extern s16							Character_ChildSpriteTable[];
extern u8							Summon_OrderList[16];
s32									Audio_Check(void);
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

extern const u16			   Battle_Effect90Classes[47];
extern const u16			   Battle_Effect91Classes[29];
extern struct ActionDefinition BattleAction_DefinitionTable[];
extern const u16			   Djinn_Definitions[][6];
extern const u8				   Enemy_DefinitionTable[];
extern u8					   GameFlagBytes[];
void						   BattlePlacement_UpdateTimedEntriesTwentyTimesFar(void);
s32							   Djinn_AddToOwnerFar(s32, s32, s32);
void						   Event_ClearInvalidPackedValuesFar(s32);
s32							   Inventory_AddItemFar(s32, s32);
s32							   Inventory_RemoveFar(s32 owner, s32 slot);
void  Owner_CalculateBaseElements(s32 owner_no, struct BattleElementStats *elements);
s32	  Owner_DetermineClass(s32 class_id, u32 *djinn, s32 item);
s32	  Owner_GetDigitValues(s32 record, const u8 *source, s32 *output);
void  Owner_RefreshClassActions(s32);
void  Owner_UpdateClassActions(s32 owner_no);
void *Resource_FarCall005(s32);
void  Ui_AdjustValueWithoutLimitFar(s32, u16 *);
#endif

#endif
