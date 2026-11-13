#ifndef GAME_H
#define GAME_H

/// @file
/// The item and summon tables and the game bank's routines.

#include "types.h"
#include "edition.h"
#include "battle/battle_sprites.h"
#include "battle/battle_unit.h"
#include "game/item.h"
#include "game/character_ids.h"
#include "game/character.h"
#include "game/enemy_element_preset.h"
#include "gamepak/far_call.h"

struct ItemDefinition;
struct SummonDefinition;

/// The 324 items by item id (item_table.c), read through Item_GetDirect
/// (Item_GetDirect in the overlays).
extern const struct ItemDefinition gItem_DefinitionTable[];
/// The 16 summons (summon_table.c) by summon id, the numbers gSummon_OrderList
/// lists; read through SummonDefinition_Get.
extern const struct SummonDefinition gSummon_DefinitionTable[];

struct AbilityDefinition;
struct EnemyDefinition;
struct EnemyElementPreset;

/// The 519 abilities by ability id (ability_table.c).
extern const struct AbilityDefinition gAbilityDefinitions[];
/// Each character's lighthouse group: 0 for Isaac, Garet, Felix and Jenna,
/// whose PP a lit Venus or Mars Lighthouse restores; 1 for the others, by
/// Mercury or Jupiter (Party_RestorePpOfLitElements).
extern const u8 gCharacter_ElementGroupTable[];
/// The experience each character needs for levels 1 to 99, 99 words a
/// character (Unit_GetExperienceForLevel).
extern const u32 gCharacter_LevelExpTable[];
/// The Djinni sprite of each element, SPRITE_VENUS_DJINNI to
/// SPRITE_JUPITER_DJINNI, whose picture Djinn_ShowJoinedMessage shows as the
/// portrait.
extern s16 gDjinn_ElementSprites[];
/// The elemental power and resistance of one element level, in
/// gElement_PowerResistByLevel, which Unit_CalculateBaseElements reads by whole
/// level, 0 to 15.
struct ElementLevelBonus {
  u16 power; ///< base power, before the tenths of a level
  u16 res;   ///< base resistance, likewise
};
/// The power and resistance by element level (element_table.c): a table
/// nothing writes, defined writable in .rodata as gFont_MarkGlyphCodes is.
extern struct ElementLevelBonus gElement_PowerResistByLevel[16];
/// The class series by a character's strongest and second element, a 4 by 4
/// table read as [strongest * 4 + second] (Class_GetSeries).
extern const s32 gClass_SeriesByElements[];
/// The 80 Djinn, 20 of each element in element order (djinn_table.c).
extern const struct DjinnDefinition gDjinn_DefinitionTable[];
/// Pairs of an element difference (power less resistance) and what it adds to
/// the multiplier in 256ths (256 = 1x), from 200 (+256) down to -200 (-256);
/// Battle_GetElementMultiplier interpolates and adds 256.
extern const s16 gElement_PowerResistCurve[5][2];
/// The 165 enemies (enemy_table.c).
extern const struct EnemyDefinition gEnemy_Definitions[];
/// The enemies' element presets (element_table.c): attack element, levels and
/// base power and resistance.
extern const struct EnemyElementPreset gEnemy_ElementPresetTable[];
/// The game flags, 4096 bits in 512 bytes, saved with the game: flag n is bit
/// n & 7 of byte GAME_FLAG_BYTE(n).
extern u8 gGameFlagBytes[];
/// The party's equipment put aside while the Colosso takes it away: a
/// sentinel, four 15-slot inventories, the two shortcuts and the party order.
extern u16 gInventorySnapshot[];
/// How many of each rare item the shops may still sell, 0 to 99, by counter
/// number (gItem_ArtifactSlotTable; RareItemStock_Adjust).
extern u8 gRareItemStock[128];
/// The 64-byte save stamp, in the first half of the save buffer, that the
/// Battle Arena writes into the current file alone.
extern u8 gBattleArenaRecord[];
/// Each item's rare item counter number plus one, or 0 for an ordinary item
/// (Item_AdjustCounter).
extern const u8 gItem_ArtifactSlotTable[];
/// Each party member's sprite, by character: Felix's is 35.
extern s16 gCharacter_SpriteTable[];
/// Each party member's sprite during the prologue (FLAG_CHILDHOOD):
/// SPRITE_ISAAC_CHILD, SPRITE_GARET_CHILD, SPRITE_FELIX_CHILD and
/// SPRITE_JENNA_CHILD in place of their own.
extern s16 gCharacter_ChildSpriteTable[];
/// The summon ids in menu order (Summon_ListLearned).
extern const u8 gSummon_OrderList[16];
/// Ends a list of summon ids (Summon_ListLearned).
#define SUMMON_END 32

#include "types.h"

struct AbilityDefinition;
struct BattleElementStats;
struct Unit;
struct CharacterDefinition;
struct ClassDefinition;
struct DjinnDefinition;
struct DjinnRecoveryTable;
struct EnemyDefinition;
struct ItemDefinition;
struct LevelUpResult;
struct SummonDefinition;

void Game_RestartQuest(void);
struct DjinnRecoveryTable *Djinn_GetRecoveryTable(s32 enemySide);
u32 Party_GetAverageLevel(void);
struct Unit *Unit_Get(u32 unitId);
const struct EnemyDefinition *Enemy_GetDefinition(s32 selector);
void Memory_CopyBytesDirectional(u8 *first, u8 *second, s32 count,
                                 s32 direction);
void Unit_Calculate(s32 unitId);
void GameFlag_RefreshLureCap(void);
u32 BuildStamp_GetTime(void);
void MainRoutineWork_InitializeDefaults(void);
void Party_StartChildhood(void);
void Party_EndChildhood(void);
void Party_RestoreHpPp(s32 cureConditions);
void Party_RestorePpOfLitElements(void);
void Unit_GaugeNoOp(void);
void Unit_RecalculateGauges(s32 unitId);
void Unit_SetHp(struct Unit *unit, s32 hp);
void Unit_SetPp(struct Unit *unit, s32 pp);
s32 Unit_AddHp(s32 unitId, s32 delta);
s16 Unit_AddPp(s32 unitId, s32 delta);
s32 Party_AdjustShopCredit(s32 amount);
s32 Party_AdjustShopGifts(s32 amount);

struct ItemDefinition *Item_GetDirect(s32 item);
s32 Item_CanUnitEquipDirect(s32 unitId, s32 itemId);
s32 Item_IsCompatibleWithUnit(s32 unit, s32 item);
s32 Item_GetEquipmentGroup(s32 itemId);
s32 Inventory_GetQuantity(s32 unitId, s32 slot);
s32 Inventory_Count(s32 unitId);
s32 PartyInventory_HasSpace(void);
s32 PartyInventory_CountFreeSlots(void);
s32 Inventory_AddItem(s32 unitId, s32 itemId);
s32 PartyInventory_Add(s32 itemId);
s32 Inventory_Find(s32 unitId, s32 itemId);
s32 PartyInventory_FindMember(s32 itemId);
s32 Inventory_Equip(s32 unitId, s32 slot);
s32 Inventory_FindEquipped(s32 unitId, s32 type);
struct ItemDefinition *Inventory_GetEquippedDefinition(struct Unit *unit,
                                                       s32 type);
s32 Inventory_GetEquippedItem(struct Unit *unit, s32 type);
s32 Inventory_Remove(s32 unitId, s32 slot);
s32 Inventory_Discard(s32 unitId, s32 slot);
s32 Inventory_CheckDiscard(s32 unitId, s32 slot);
s32 PartyInventory_Remove(s32 itemId);
s32 PartyInventory_Discard(s32 itemId);
s32 Inventory_Break(s32 unitId, s32 slot);
s32 Inventory_Repair(s32 unitId, s32 slot);
u8 Item_GetTargetMode(s32 itemId);
s32 RareItemStock_Adjust(s32 index, s32 delta);
s32 Item_AdjustCounter(s32 itemId, s32 delta);
s32 Inventory_CountItem(s32 unitId, s32 itemId);
s32 PartyInventory_CountItem(s32 itemId);
struct AbilityDefinition *Ability_GetDirect(s32 abilityId);
s32 Unit_KnowsPsynergy(s32 unitId, s32 abilityId);
s32 Unit_RefreshPsynergy(s32 unitId);
s32 Unit_LearnPsynergy(s32 stateIndex, s32 value);
struct LevelUpResult *Unit_CheckLevelUp(s32 unitId,
                                        struct LevelUpResult *gains);
struct CharacterDefinition *Character_GetDefinition(s32 index);
void Party_InitializeCharacters(void);
void Unit_LevelNoOp(void);
u32 Unit_GetExperienceForLevel(s32 unitId, s32 level);
struct LevelUpResult *Unit_LevelUp(s32 unitId, struct LevelUpResult *gains);
struct LevelUpResult *Unit_TryLevelUp(s32 unitId, struct LevelUpResult *gains);
void Unit_LevelUpTo(s32 unitId, s32 targetLevel);

s32 GameFlag_Get(s32 flag);
s32 GameFlag_Set(s32 flag);
void GameFlag_Clear(s32 flag);
u32 GameFlag_Toggle(s32 flag);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
u8 GameFlag_IncrementByte(s32 flag);
u8 GameFlag_DecrementByte(s32 flag);
s32 GameFlag_GetNibble(u32 flag);
void GameFlag_SetNibble(s32 flag, s32 value);

s32 BattleUnit_Set(s32 unitId, s32 enemyId, s32 suffix);
s32 Party_CountMembers(void);
s32 Party_AddMember(s32 unitId);
s32 Party_RemoveMember(s32 unitId);
s32 Party_ListMembers(s16 *unitIds);
s32 Party_AdjustCoins(s32 amount);

s32 Summon_ListLearned(u8 *output);
const struct SummonDefinition *SummonDefinition_GetNear(u32 summonId);
s32 Class_GetSeries(s32 primaryElement, s32 secondaryElement);
s32 Unit_GetElementLevels(s32 record, const u8 *source, s32 output[4]);
s32 Unit_GetElementLevel(s32 unitId, s32 element);
s32 Enemy_GetAttackElement(struct Unit *unit);
UNREAD_RESULT
Unit_CalculateBaseElements(s32 unitId, struct BattleElementStats *destination);
s32 Class_Determine(s32 character, const u8 *djinn);
struct ClassDefinition *Class_GetDefinition(s32 classId);
void Unit_UpdateClass(s32 unitId);

s32 Battle_GetElementMultiplier(s32 input, s32 halve);
s32 Battle_Random(void);
u32 Battle_RandomPercent(void);
s32 Battle_CalculateAttackDamage(s32 attack, s32 defense, s32 addedPower,
                                 s32 elementDifference);
s32 Battle_CalculatePsynergyPower(s32 power, s32 elementDifference,
                                  s32 powerMultiplier);
s32 Battle_CalculateRecovery(s32 power, s32 elementPower, s32 powerMultiplier);
s32 Item_GetEquippedElement(s32 unitId);
s32 Equipment_GetUnleashRateBonus(struct Unit *unit);
s32 RollWeaponUnleash(struct Unit *unit);
s32 Battle_GetEffectBaseSuccessRate(s32 effectId);
s32 BattleAbility_IsWeakToEffect(const struct Unit *target, s32 effectId);
s32 Battle_IsReviveEffect(s32 effectId);
s32 Battle_RollEffectSuccess(s32 caster, s32 target, s32 resistanceCategory,
                             s32 effectId, s32 successScale);

const struct DjinnDefinition *Djinn_GetDefinition(u32 group, u32 index);
s32 Djinn_GiveToParty(s32 element, s32 djinn);
s32 Djinn_Give(s32 unitId, s32 element, s32 djinnIndex);
s32 Djinn_CanSet(s32 unitId, s32 element, s32 djinnIndex);
u32 Djinn_IsSet(s32 unitId, s32 element, s32 djinnIndex);
s32 Djinn_Set(s32 unitId, s32 element, s32 djinnIndex);
u32 Djinn_PutOnStandby(s32 unitId, s32 element, s32 djinnIndex);
s32 DjinnRecovery_Remove(s32 unitId, s32 element, s32 djinnIndex);
u32 *DjinnRecovery_AddStandby(u32 unitId, u32 element, u32 djinnIndex);
s32 Djinn_Transfer(s32 source, s32 element, s32 djinnIndex, s32 target);
s32 DjinnRecovery_CountStandby(u8 *counts);
s32 Djinn_GetAbilityId(u32 group, u32 index);
s32 Party_SumDjinnCounts(s32 element);

void Inventory_AddAndEquip(s32 unitId, s32 itemId);
void InventorySnapshot_Save(void);
void InventorySnapshot_Restore(void);

#endif
