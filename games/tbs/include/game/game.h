#ifndef ALCHEMY_GAME_H
#define ALCHEMY_GAME_H

/* @file */
/* The item and summon tables and the game module's functions. */

#include "types.h"
#include "edition.h"
#include "battle/battle_sprites.h"
#include "battle/battle_unit.h"
#include "game/item.h"
#include "game/character_ids.h"
#include "game/character.h"
#include "game/enemy_element_preset.h"
#include "stubs.h"

struct ABILITY_DEFINITION;
struct BATTLE_ELEMENT_STATS;
struct CHARACTER_DEFINITION;
struct CLASS_DEFINITION;
struct DJINN_DEFINITION;
struct DJINN_RECOVERY_TABLE;
struct ENEMY_DEFINITION;
struct ENEMY_ELEMENT_PRESET;
struct ITEM_DEFINITION;
struct LEVEL_UP_RESULT;
struct SUMMON_DEFINITION;
struct UNIT;

/* The game module's functions in the order of its stub table */
/* (src/game/stubs.s). */
struct DJINN_RECOVERY_TABLE *gmDjinnGetRecoveryTable(s32 enemySide);
UNIT *gmUnitGet(u32 unitId);
void gmUnitCalc(s32 unitId);
const ITEM_DEFINITION *gmItemGetDirect(s32 item);
s32 gmInventoryGetQuantity(s32 unitId, s32 slot);
s32 gmInventoryAddItem(s32 unitId, s32 itemId);
s32 gmPartyInventoryAdd(s32 itemId);
s32 gmInventoryFind(s32 unitId, s32 itemId);
s32 gmPartyInventoryFindMember(s32 itemId);
s32 gmPartyInventoryRemove(s32 itemId);
s32 gmInventoryEquip(s32 unitId, s32 slot);
s32 gmInventoryRemove(s32 unitId, s32 slot);
s32 gmInventoryBreak(s32 unitId, s32 slot);
s32 gmInventoryRepair(s32 unitId, s32 slot);
u8 gmItemGetTargetMode(s32 itemId);
s32 gmInventoryGetEquippedItem(UNIT *unit, s32 type);
ABILITY_DEFINITION *gmAbilityGetDirect(s32 abilityId);
s32 gmUnitLearnPsynergy(s32 unitId, s32 abilityId);
s32 gmUnitKnowsPsynergy(s32 unitId, s32 abilityId);
void gmMainRoutineStateInitDefaults(void);
LEVEL_UP_RESULT *gmUnitCheckLevelUp(s32 unitId, LEVEL_UP_RESULT *gains);
void gmPartyInitCharacters(void);
void gmUnitLevelNoOp(void);
LEVEL_UP_RESULT *gmUnitTryLevelUp(s32 unitId, LEVEL_UP_RESULT *gains);
s32 gmFlagGet(s32 flag);
s32 gmFlagSet(s32 flag);
void gmFlagClear(s32 flag);
u32 gmFlagToggle(s32 flag);
s32 gmFlagGetByte(s32 flag);
void gmFlagSetByte(s32 flag, s32 value);
u8 gmFlagIncrementByte(s32 flag);
u8 gmFlagDecrementByte(s32 flag);
s32 gmFlagGetNibble(u32 flag);
void gmFlagSetNibble(s32 flag, s32 value);
void gmRestartQuest(void);
/* Adds delta to unit unitId's HP, kept from 0 to its maximum, and returns */
/* the new HP. */
s32 gmUnitAddHp(s32 unitId, s32 delta);
/* Adds delta to unit unitId's PP, kept from 0 to its maximum, and returns */
/* the new PP. */
s16 gmUnitAddPp(s32 unitId, s32 delta);
void gmUnitRecalculateGauges(s32 unitId);
void gmUnitSetHp(UNIT *unit, s32 value);
void gmUnitSetPp(UNIT *unit, s32 value);
s32 gmBattleUnitSet(s32 unitId, s32 enemyId, s32 suffix);
s32 gmPartyCountMembers(void);
s32 gmPartyAddMember(s32 unitId);
s32 gmPartyListMembers(s16 *unitIds);
s32 gmEquipmentRollUnleash(UNIT *unit);
s32 gmPartyRemoveMember(s32 unitId);
s32 gmUnitGetAttackElement(s32 unitId);
s32 gmBattleRollEffectSuccess(s32 caster, s32 target, s32 element, s32 effectId,
							  s32 successScale);
s32 gmBattleCalcAttackDamage(s32 attack, s32 defense, s32 addedPower,
							 s32 elementDifference);
s32 gmBattleCalcPsynergyPower(s32 power, s32 elementDifference,
							  s32 powerMultiplier);
s32 gmBattleCalcRecovery(s32 power, s32 elementPower, s32 powerMultiplier);
const ENEMY_DEFINITION *gmEnemyGetDefinition(s32 enemyId);
s32 gmBattleRandom(void);
s32 gmDjinnGive(s32 unitId, s32 element, s32 djinnIndex);
s32 gmDjinnSet(s32 unitId, s32 element, s32 djinnIndex);
u32 gmDjinnPutOnStandby(s32 unitId, s32 element, s32 djinnIndex);
s32 gmDjinnRecoveryRemove(s32 unitId, s32 element, s32 djinnIndex);
u32 *gmDjinnRecoveryAddStandby(u32 unitId, u32 element, u32 djinnIndex);
s32 gmDjinnTransfer(s32 source, s32 element, s32 djinnIndex, s32 target);
s32 gmSummonListLearned(u8 *output);
const SUMMON_DEFINITION *gmSummonDefinitionGetNear(u32 summonId);
s32 gmDjinnGetAbilityId(u32 element, u32 djinni);
void gmUnitLevelUpTo(s32 unitId, s32 targetLevel);
s32 gmUnitGetElementLevel(s32 unitId, s32 element);
s32 gmDjinnRecoveryCountStandby(u8 *counts);
u32 gmDjinnIsSet(s32 unitId, s32 element, s32 djinnIndex);
s32 gmDjinnCanSet(s32 unitId, s32 element, s32 djinnIndex);
s32 gmItemCanUnitEquipDirect(s32 unitId, s32 itemId);
s32 gmItemIsCompatibleWithUnit(s32 unitId, s32 itemId);
s32 gmInventoryFindEquipped(s32 unitId, s32 type);
s32 gmPartyAdjustCoins(s32 amount);
s32 gmItemGetEquipmentGroup(s32 itemId);
s32 gmItemAdjustRareItemStock(s32 itemId, s32 delta);
s32 gmInventoryCount(s32 unitId);
s32 gmPartyInventoryHasSpace(void);
u32 gmUnitGetExperienceForLevel(s32 unitId, s32 level);
void gmPartyRestoreHpPp(s32 cureConditions);
void gmPartyRestorePpInLighthouse(void);
void gmPartyStartChildhood(void);
void gmPartyEndChildhood(void);
void gmUnitGaugeNoOp(void);
s32 gmDjinnGiveToParty(s32 element, s32 djinn);
s32 gmPartySumDjinnCounts(s32 element);
s32 gmPartyAdjustShopCredit(s32 amount);
s32 gmPartyAdjustShopGifts(s32 amount);
s32 gmInventoryCheckDiscard(s32 unitId, s32 slot);
s32 gmInventoryDiscard(s32 unitId, s32 slot);
s32 gmBattleIsReviveEffect(s32 effectId);
s32 gmUnitRefreshPsynergy(s32 unitId);
u32 gmPartyGetAverageLevel(void);
void gmUnitUpdateClass(s32 unitId);
s32 gmInventoryCountItem(s32 unitId, s32 itemId);
s32 gmPartyInventoryCountItem(s32 itemId);
s32 gmPartyInventoryDiscard(s32 itemId);
void gmFlagRefreshLureCap(void);
s32 gmEquipmentGetUnleashRateBonus(UNIT *unit);
u32 gmBuildStampGetTime(void);
s32 gmPartyInventoryCountFreeSlots(void);
void gmInventorySnapshotSave(void);
void gmInventorySnapshotRestore(void);

/* The 324 items by item id (item_table.c), read through gmItemGetDirect. */
extern const ITEM_DEFINITION Item_DefinitionTable[];
/* The 16 summons (summon_table.c) by summon id, the numbers Summon_OrderList */
/* lists; read through gmSummonDefinitionGet. */
extern const SUMMON_DEFINITION Summon_DefinitionTable[];

/* The 519 abilities by ability id (ability_table.c). */
extern const ABILITY_DEFINITION AbilityDefinitions[];
/* The lighthouses inside which a character's PP is refilled at an aerie */
/* (gmPartyRestorePpInLighthouse). */
typedef enum LIGHTHOUSE_GROUP {
	LIGHTHOUSE_GROUP_VENUS_MARS,	 /* Isaac, Garet, Felix and Jenna */
	LIGHTHOUSE_GROUP_MERCURY_JUPITER /* Ivan, Mia, Sheba and Piers */
} LIGHTHOUSE_GROUP;
/* Each character's LIGHTHOUSE_GROUP. */
extern const u8 Character_LighthouseGroupTable[];
/* The experience each character needs for levels 1 to 99, 99 words a */
/* character (gmUnitGetExperienceForLevel). */
extern const u32 Character_LevelExpTable[];
/* The Djinni sprite of each element, SPRITE_VENUS_DJINNI to */
/* SPRITE_JUPITER_DJINNI, whose picture mnDjinnShowJoinedMessage shows as the */
/* portrait. */
extern s16 Djinn_ElementSprites[];
/* The elemental power and resistance of one element level, in */
/* Element_PowerResistByLevel, which gmUnitCalcBaseElements reads by whole */
/* level, 0 to 15. */
typedef struct ELEMENT_LEVEL_BONUS {
	u16 power;		/* base power, before the tenths of a level */
	u16 resistance; /* base resistance, likewise */
} ELEMENT_LEVEL_BONUS;
/* The power and resistance by element level (element_table.c): a table */
/* nothing writes, defined writable in .rodata as Font_MarkGlyphCodes is. */
extern ELEMENT_LEVEL_BONUS Element_PowerResistByLevel[16];
/* The class series by a character's strongest and second element, a 4 by 4 */
/* table read as [strongest * 4 + second] (gmClassGetSeries). */
extern const s32 Class_SeriesByElements[];
/* The 80 Djinn, 20 of each element in element order (djinn_table.c). */
extern const DJINN_DEFINITION Djinn_DefinitionTable[];
/* Pairs of an element difference (power less resistance) and what it adds to */
/* the multiplier in 256ths (256 = 1x), from 200 (+256) down to -200 (-256); */
/* gmBattleGetElementMultiplier interpolates and adds 256. */
extern const s16 Element_PowerResistCurve[5][2];
/* The enemies' rows (enemy_table.c), from ENEMY_ID_FIRST to the last Djinni, */
/* then a closing row named 0xff. */
#define ENEMY_DEFINITION_COUNT 165
extern const ENEMY_DEFINITION Enemy_Definitions[ENEMY_DEFINITION_COUNT];
/* The enemies' element presets (element_table.c): attack element, levels and */
/* base power and resistance. */
extern const ENEMY_ELEMENT_PRESET Enemy_ElementPresetTable[];
/* The game flags, 4096 bits in 512 bytes, saved with the game: flag n is bit */
/* n & 7 of byte GAME_FLAG_BYTE(n). */
extern u8 GameFlagBytes[];
/* The party's equipment put aside while the Colosso takes it away: a */
/* sentinel, four 15-slot inventories, the two shortcuts and the party order. */
extern u16 InventorySnapshotWork[];
/* How many rare items the shops keep a stock of, and the most of each. */
enum { RARE_ITEM_STOCK_COUNT = 128, RARE_ITEM_STOCK_MAX = 99 };
/* How many of each rare item the shops' Artifacts offer, 0 to */
/* RARE_ITEM_STOCK_MAX, by stock number (Item_RareItemStockIndex; */
/* gmRareItemStockAdjust). */
extern u8 RareItemStock[RARE_ITEM_STOCK_COUNT];
/* The 64-byte save stamp, in the first half of the save buffer, that the */
/* Battle Arena writes into the current file alone. */
extern u8 BattleArenaSaveStamp[];
/* Each item's place in RareItemStock plus one, or 0 for an item the shops */
/* keep no stock of (gmItemAdjustRareItemStock). */
extern const u8 Item_RareItemStockIndex[];
/* Each party member's sprite, by character; Piers, who has none, keeps his */
/* character number. */
extern s16 Character_SpriteTable[];
/* Each party member's sprite during the prologue (FLAG_CHILDHOOD): */
/* SPRITE_ISAAC_CHILD, SPRITE_GARET_CHILD, SPRITE_FELIX_CHILD and */
/* SPRITE_JENNA_CHILD in place of their own. */
extern s16 Character_ChildSpriteTable[];
/* The summon ids in menu order (gmSummonListLearned). */
extern const u8 Summon_OrderList[16];
/* Ends a list of summon ids (gmSummonListLearned); the battle summon list also
 */
/* writes it over each summon it has moved to the front. */
#define SUMMON_END 32

void gmMemoryCopyBytesDirectional(u8 *first, u8 *second, s32 count,
								  s32 direction);

const ITEM_DEFINITION *gmInventoryGetEquippedDefinition(UNIT *unit, s32 type);
s32 gmRareItemStockAdjust(s32 index, s32 delta);
const CHARACTER_DEFINITION *gmCharacterGetDefinition(s32 characterId);
LEVEL_UP_RESULT *gmUnitLevelUp(s32 unitId, LEVEL_UP_RESULT *gains);

s32 gmClassGetSeries(s32 primaryElement, s32 secondaryElement);
s32 gmUnitGetElementLevels(s32 unitId, const u8 *djinnHeld, s32 output[4]);
s32 gmEnemyGetAttackElement(UNIT *unit);
UNREAD_RESULT
gmUnitCalcBaseElements(s32 unitId, BATTLE_ELEMENT_STATS *destination);
s32 gmClassDetermine(s32 character, const u8 *djinn);
CLASS_DEFINITION *gmClassGetDefinition(s32 classId);

s32 gmBattleGetElementMultiplier(s32 difference, s32 halve);
u32 gmBattleRandomPercent(void);
/* The element power at which gmBattleCalcRecovery's healing is neither */
/* stronger nor weaker: each point above it counts as two points of element */
/* difference. */
#define HEALING_BASE_POWER 100
/* The battle's multipliers are 256ths: this one keeps an amount as it is. */
#define BATTLE_MULTIPLIER_ONE 256
s32 gmBattleGetEffectBaseSuccessRate(s32 effectId);
s32 gmBattleAbilityIsWeakToEffect(const UNIT *target, s32 effectId);

const DJINN_DEFINITION *gmDjinnGetDefinition(u32 element, u32 djinni);

void gmInventoryAddAndEquip(s32 unitId, s32 itemId);

#endif
