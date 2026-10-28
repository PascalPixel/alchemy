#ifndef GAME_H
#define GAME_H

/// @file
/// GAME

#include "types.h"
#include "edition.h"
#include "battle_summon.h"
#include "battle_unit.h"
#include "item.h"
#include "character_ids.h"
#include "battle_type.h"

struct ItemDefinition;
struct SummonDefinition;

extern struct ItemDefinition gItem_DefinitionTable[];
extern const struct SummonDefinition gSummon_DefinitionTable[];
s32 Djinn_IsReady(s32 owner, s32 index, s32 bit);

struct ActionDefinition;
struct BattleElementStats;

extern const u16 gBattle_Effect90Classes[47];
extern const u16 gBattle_Effect91Classes[29];
extern struct ActionDefinition gBattleAction_DefinitionTable[];
extern const u16 gDjinn_Definitions[][6];
extern const u8 gEnemy_DefinitionTable[];
extern u8 gGameFlagBytes[];
void DjinnRecovery_CountDownTwentyFar(void);
s32 Djinn_AddToOwnerFar(s32, s32, s32);
void Event_ClearInvalidPackedValuesFar(s32);
s32 Inventory_AddItemFar(s32, s32);
s32 Inventory_RemoveFar(s32 owner, s32 slot);
void Unit_CalculateBaseElements(s32 owner_id,
                                struct BattleElementStats *elements);
s32 Class_Determine(s32 classId, u32 *djinn, s32 item);
s32 Unit_GetElementLevels(s32 record, const u8 *source, s32 *output);
void Unit_RefreshPsynergy(s32);
void Owner_UpdateClassActions(s32 owner_id);
void *Resource_FarCall005(s32);
void Message_DecodeRawUnboundedFar(s32, u16 *);

#endif
