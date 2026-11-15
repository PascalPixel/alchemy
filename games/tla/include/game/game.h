#ifndef GAME_H
#define GAME_H

/// @file
/// GAME

#include "types.h"
#include "edition.h"
#include "battle/battle_summon.h"
#include "battle/battle_unit.h"
#include "item.h"
#include "character_ids.h"
#include "character.h"
#include "battle/battle_type.h"

struct ItemDefinition;
struct SummonDefinition;

/// A field Psynergy, as the field kind: an ability's fieldPsynergy, named for
/// the abilities that have it (the English release's names in
/// gAbilityDefinitions). The field effect keeps it as its animation.
/// The Broken Seal has 1 to 16; The Lost Age adds 17 to 30.
enum FieldPsynergy {
  FIELD_PSYNERGY_MOVE = 1,
  FIELD_PSYNERGY_MIND_READ = 2,
  FIELD_PSYNERGY_PLY = 3,
  FIELD_PSYNERGY_FORCE = 4,
  FIELD_PSYNERGY_DOUSE = 5,
  FIELD_PSYNERGY_FROST = 6,
  FIELD_PSYNERGY_LIFT = 7,
  FIELD_PSYNERGY_REVEAL = 8,
  FIELD_PSYNERGY_HALT = 9,
  FIELD_PSYNERGY_CLOAK = 10,
  FIELD_PSYNERGY_CARRY = 11,
  FIELD_PSYNERGY_GROWTH = 12,
  FIELD_PSYNERGY_CATCH = 13,
  /// Whirlwind and Gale.
  FIELD_PSYNERGY_WHIRLWIND = 14,
  FIELD_PSYNERGY_RETREAT = 15,
  FIELD_PSYNERGY_AVOID = 16,
  FIELD_PSYNERGY_LASH = 17,
  FIELD_PSYNERGY_POUND = 18,
  FIELD_PSYNERGY_TREMOR = 19,
  FIELD_PSYNERGY_SCOOP = 20,
  FIELD_PSYNERGY_BURST = 21,
  FIELD_PSYNERGY_PARCH = 22,
  FIELD_PSYNERGY_SAND = 23,
  FIELD_PSYNERGY_HOVER = 24,
  FIELD_PSYNERGY_CYCLONE = 25,
  FIELD_PSYNERGY_BLAZE = 26,
  FIELD_PSYNERGY_GRIND = 27,
  /// The two abilities whose names are question marks.
  FIELD_PSYNERGY_RING = 28,
  FIELD_PSYNERGY_ARROW = 29,
  FIELD_PSYNERGY_TELEPORT = 30
};

/// A step of the current field Psynergy (FieldPsynergy_RunHandler).
enum FieldPsynergyHandler {
  /// Casts it with no target (FieldPsynergy_Run).
  FIELD_PSYNERGY_HANDLER_UNTARGETED = 0,
  /// Casts it on its target, secondActorId (FieldPsynergy_Dispatch).
  FIELD_PSYNERGY_HANDLER_ON_TARGET = 1,
  FIELD_PSYNERGY_HANDLER_FINISH = 2, ///< finishes it
  FIELD_PSYNERGY_HANDLER_EXTRA = 3   ///< runs its kind's extra handler
};

/// An ability's definition, a row of gAbilityDefinitions: its
/// targeting, kind, element, effect, icon, range, PP cost and power, as The
/// Broken Seal's first twelve bytes.
struct AbilityDefinition {
  u8 targetMode; ///< whom it can be aimed at
  /// bits: 7 usable in battle | 6 usable in the field | 3..0 ability kind.
  u8 flags;
  u8 element;       ///< enum Element
  u8 effect;        ///< enum AbilityEffect
  u16 icon;         ///< the icon the menus draw
  u8 fieldPsynergy; ///< enum FieldPsynergy, or 0 for none
  u8 unk07;         ///< 0 in every row
  u8 range;  ///< the aimed unit and range - 1 either side, or 255 for all
  u8 ppCost; ///< PP the ability costs
  u16 power; ///< what the ability's kind applies
};

STATIC_ASSERT(sizeof(struct AbilityDefinition) == 12);

/// The elemental power and resistance of one element level, in
/// gElement_PowerResistByLevel.
struct ElementLevelBonus {
  u16 power; ///< base power, before the tenths of a level
  u16 res;   ///< base resistance, likewise
};

extern const struct ItemDefinition gItem_DefinitionTable[];
extern const struct SummonDefinition gSummon_DefinitionTable[];
s32 Djinn_CanSet(s32 owner, s32 index, s32 bit);

struct BattleElementStats;

/// How many enemies gEnemy_DragonIds and gEnemy_SeaCreatureIds list.
#define DRAGON_ENEMY_COUNT 47
#define SEA_CREATURE_ENEMY_COUNT 29
extern const u16 gEnemy_DragonIds[DRAGON_ENEMY_COUNT];
extern const u16 gEnemy_SeaCreatureIds[SEA_CREATURE_ENEMY_COUNT];
extern const struct AbilityDefinition gAbilityDefinitions[];
extern const struct DjinnDefinition gDjinn_DefinitionTable[];
extern const struct EnemyDefinition gEnemy_Definitions[];
/// The game flags, 4096 bits in 512 bytes, saved with the game: flag n is bit
/// n & 7 of byte GAME_FLAG_BYTE(n).
extern u8 gGameFlagBytes[];
void DjinnRecovery_CountDownTwentyFar(void);
/// Gives a Djinni to a unit.
s32 Djinn_GiveFar(s32, s32, s32);
void Party_ClearUnusableShortcutsFar(void);
/// Gives a unit one item, joining its first matching stack (up to 30) or taking
/// the first free slot.
s32 Inventory_AddItemFar(s32, s32);
/// Takes one item out of slot, closing up the inventory when it was the last.
s32 Inventory_RemoveFar(s32 owner, s32 slot);
void Unit_CalculateBaseElements(s32 owner_id,
                                struct BattleElementStats *elements);
s32 Class_Determine(s32 classId, u32 *djinn, s32 item);
s32 Unit_GetElementLevels(s32 record, const u8 *source, s32 *output);
void Unit_RefreshPsynergy(s32);
s32 Owner_UpdateClassActions(s32 owner_id);
/// Expands message into buffer without an upper length bound.
void Message_DecodeRawUnboundedFar(s32, u16 *);

/// How many summons there are (summon_table.c), and the number that ends a
/// list of them (Summon_ListLearned).
#define SUMMON_COUNT 29
/// Ends the summon lists, and marks a summon already moved to the front.
#define SUMMON_END 32
/// The summons in the order the Summon command lists them (summon_table.c).
extern const u8 gSummon_OrderList[SUMMON_COUNT];
/// The experience each character needs for each level (experience_table.c).
extern const u32 gCharacter_LevelExpTable[];

extern const s16 gElement_PowerResistCurve[5][2];
/// The power and resistance by element level (element_table.c): a table
/// nothing writes, defined writable in .rodata as The Broken Seal's is.
extern struct ElementLevelBonus gElement_PowerResistByLevel[16];
extern const struct EnemyElementPreset gEnemy_ElementPresetTable[];
extern const struct CharacterDefinition gCharacter_DefinitionTable[];
const struct CharacterDefinition *Character_GetDefinition(s32 character);
extern const s32 gCharacter_StartingEquipUnitIds[];
extern const u16 gItem_ArtifactSlotTable[512];

extern const u8 gCharacter_ElementGroupTable[];

/// The Djinni sprite of each element, SPRITE_VENUS_DJINNI to
/// SPRITE_JUPITER_DJINNI, whose picture Djinn_ShowJoinedMessage shows as the
/// portrait.
extern const s16 gDjinn_ElementSprites[];
/// Each party member's sprite, by character; each is the character's own id.
extern const s16 gCharacter_SpriteTable[];

#endif
