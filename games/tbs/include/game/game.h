#ifndef GAME_H
#define GAME_H

/// @file
/// The item and summon tables and the game bank's routines.

#include "types.h"
#include "edition.h"
#include "battle_summon.h"
#include "battle_unit.h"
#include "item.h"
#include "character_ids.h"
#include "character.h"
#include "preset_table.h"
#include "farcall.h"

struct ItemDefinition;
struct SummonDefinition;

extern struct ItemDefinition gItem_DefinitionTable[];
extern const struct SummonDefinition gSummon_DefinitionTable[];

struct BattleAct;
struct EnemyDefinition;
struct PresetValues;

extern struct BattleAct gBattleAction_DefinitionTable[];
extern const u8 gCharacter_ElementGroupTable[];
extern u32 gCharacter_LevelExpTable[];
extern s16 gDjinn_ElementSprites[];
extern s32 gOwner_FourColumnValues[];
extern const struct DjinnDefinition gDjinn_DefinitionTable[];
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

#endif
