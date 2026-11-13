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
#include "character.h"
#include "battle_type.h"

struct ItemDefinition;
struct SummonDefinition;

/// An ability's definition, a row of gBattleAction_DefinitionTable: its
/// targeting, kind, element, effect, icon, range, PP cost and power, as The
/// Broken Seal's first twelve bytes.
struct AbilityDefinition {
  u8 targetMode; ///< whom it can be aimed at
  /// bits: 7 usable in battle | 6 usable in the field | 3..0 ability kind.
  u8 flags;
  u8 element;              ///< enum Element
  u8 effect;               ///< enum AbilityEffect
  u8 icon;                 ///< the icon the menus draw
  u8 reservedAfterIcon[3]; ///< 0 in every row
  u8 range;  ///< the aimed unit and range - 1 either side, or 255 for all
  u8 ppCost; ///< PP the ability costs
  u16 power; ///< what the ability's kind applies
};

STATIC_ASSERT(sizeof(struct AbilityDefinition) == 12);

/// The elemental power and resistance of one element level, in Data_080c6644.
struct ElementLevelBonus {
  u16 power; ///< base power, before the tenths of a level
  u16 res;   ///< base resistance, likewise
};

extern const struct ItemDefinition gItem_DefinitionTable[];
extern const struct SummonDefinition gSummon_DefinitionTable[];
s32 Djinn_CanSet(s32 owner, s32 index, s32 bit);

struct BattleElementStats;

extern const u16 gBattle_Effect90Classes[47];
extern const u16 gBattle_Effect91Classes[29];
extern const struct AbilityDefinition gBattleAction_DefinitionTable[];
extern const struct DjinnDefinition gDjinn_Definitions[];
extern const struct EnemyDefinition gEnemy_DefinitionTable[];
extern u8 gGameFlagBytes[];
void DjinnRecovery_CountDownTwentyFar(void);
s32 Djinn_GiveFar(s32, s32, s32);
void Event_ClearInvalidPackedValuesFar(void);
s32 Inventory_AddItemFar(s32, s32);
s32 Inventory_RemoveFar(s32 owner, s32 slot);
void Unit_CalculateBaseElements(s32 owner_id,
                                struct BattleElementStats *elements);
s32 Class_Determine(s32 classId, u32 *djinn, s32 item);
s32 Unit_GetElementLevels(s32 record, const u8 *source, s32 *output);
void Unit_RefreshPsynergy(s32);
s32 Owner_UpdateClassActions(s32 owner_id);
void *Resource_FarCall005(s32);
void Message_DecodeRawUnboundedFar(s32, u16 *);

extern const s16 gElement_PowerResistCurve[5][2];

extern const u8 gCharacter_ElementGroupTable[];

extern const s16 gDjinn_ElementSprites[];
extern const s16 gCharacter_SpriteTable[];

#endif
