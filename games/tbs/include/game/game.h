#ifndef ALCHEMY_GAME_H
#define ALCHEMY_GAME_H

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

/* The game module's functions in the order of its stub table
   (src/game/stubs.s). */
struct DJINN_RECOVERY_TABLE *gmDjinnGetRecoveryTable(Sint32 enemySide);
UNIT						*gmUnitGet(Uint32 unitId);
void						 gmUnitCalc(Sint32 unitId);
const ITEM_DEFINITION		*gmItemGetDirect(Sint32 item);
Sint32						 gmInventoryGetQuantity(Sint32 unitId, Sint32 slot);
Sint32						 gmInventoryAddItem(Sint32 unitId, Sint32 itemId);
Sint32						 gmPartyInventoryAdd(Sint32 itemId);
Sint32						 gmInventoryFind(Sint32 unitId, Sint32 itemId);
Sint32						 gmPartyInventoryFindMember(Sint32 itemId);
Sint32						 gmPartyInventoryRemove(Sint32 itemId);
Sint32						 gmInventoryEquip(Sint32 unitId, Sint32 slot);
Sint32						 gmInventoryRemove(Sint32 unitId, Sint32 slot);
Sint32						 gmInventoryBreak(Sint32 unitId, Sint32 slot);
Sint32						 gmInventoryRepair(Sint32 unitId, Sint32 slot);
Uint8						 gmItemGetTargetMode(Sint32 itemId);
Sint32				gmInventoryGetEquippedItem(UNIT *unit, Sint32 type);
ABILITY_DEFINITION *gmAbilityGetDirect(Sint32 abilityId);
Sint32				gmUnitLearnPsynergy(Sint32 unitId, Sint32 abilityId);
Bool				gmUnitKnowsPsynergy(Sint32 unitId, Sint32 abilityId);
void				gmMainRoutineStateInitDefaults(void);
LEVEL_UP_RESULT	   *gmUnitCheckLevelUp(Sint32 unitId, LEVEL_UP_RESULT *gains);
void				gmPartyInitCharacters(void);
void				gmUnitLevelNoOperation(void);
LEVEL_UP_RESULT	   *gmUnitTryLevelUp(Sint32 unitId, LEVEL_UP_RESULT *gains);
Bool				gmFlagGet(Sint32 flg);
Sint32				gmFlagSet(Sint32 flg);
void				gmFlagClear(Sint32 flg);
Uint32				gmFlagToggle(Sint32 flg);
Sint32				gmFlagGetByte(Sint32 flg);
void				gmFlagSetByte(Sint32 flg, Sint32 value);
Uint8				gmFlagIncrementByte(Sint32 flg);
Uint8				gmFlagDecrementByte(Sint32 flg);
Sint32				gmFlagGetNibble(Uint32 flg);
void				gmFlagSetNibble(Sint32 flg, Sint32 value);
void				gmRestartQuest(void);
/* Adds delta to unit unitId's HP, kept from 0 to its maximum, and returns the
   new HP. */
Sint32 gmUnitAddHp(Sint32 unitId, Sint32 delta);
/* Adds delta to unit unitId's PP, kept from 0 to its maximum, and returns the
   new PP. */
Sint16 gmUnitAddPp(Sint32 unitId, Sint32 delta);
void   gmUnitRecalculateGauges(Sint32 unitId);
void   gmUnitSetHp(UNIT *unit, Sint32 value);
void   gmUnitSetPp(UNIT *unit, Sint32 value);
Bool   gmBattleUnitSet(Sint32 unitId, Sint32 enemyId, Sint32 suffix);
Sint32 gmPartyCountMembers(void);
Sint32 gmPartyAddMember(Sint32 unitId);
Sint32 gmPartyListMembers(Sint16 *unitIds);
Sint32 gmEquipmentRollUnleash(UNIT *unit);
Sint32 gmPartyRemoveMember(Sint32 unitId);
Sint32 gmUnitGetAttackElement(Sint32 unitId);
Sint32 gmBattleRollEffectSuccess(Sint32 caster, Sint32 tgt, Sint32 element,
	Sint32 effectId, Sint32 successScale);
Sint32 gmBattleCalcAttackDamage(
	Sint32 attack, Sint32 defense, Sint32 addedPower, Sint32 elementDifference);
Sint32 gmBattleCalcPsynergyPower(
	Sint32 power, Sint32 elementDifference, Sint32 powerMultiplier);
Sint32 gmBattleCalcRecovery(
	Sint32 power, Sint32 elementPower, Sint32 powerMultiplier);
const ENEMY_DEFINITION *gmEnemyGetDefinition(Sint32 enemyId);
Sint32					gmBattleRandom(void);
Sint32	gmDjinnGive(Sint32 unitId, Sint32 element, Sint32 djinnIdx);
Sint32	gmDjinnSet(Sint32 unitId, Sint32 element, Sint32 djinnIdx);
Uint32	gmDjinnPutOnStandby(Sint32 unitId, Sint32 element, Sint32 djinnIdx);
Sint32	gmDjinnRecoveryRemove(Sint32 unitId, Sint32 element, Sint32 djinnIdx);
Uint32 *gmDjinnRecoveryAddStandby(
	Uint32 unitId, Uint32 element, Uint32 djinnIdx);
Sint32 gmDjinnTransfer(Sint32 src, Sint32 element, Sint32 djinnIdx, Sint32 tgt);
Sint32 gmSummonListLearned(Uint8 *output);
const SUMMON_DEFINITION *gmSummonDefinitionGetNear(Uint32 summonId);
Sint32					 gmDjinnGetAbilityId(Uint32 element, Uint32 djinni);
void					 gmUnitLevelUpTo(Sint32 unitId, Sint32 tgtLevel);
Sint32					 gmUnitGetElementLevel(Sint32 unitId, Sint32 element);
Sint32					 gmDjinnRecoveryCountStandby(Uint8 *cnts);
Uint32 gmDjinnIsSet(Sint32 unitId, Sint32 element, Sint32 djinnIdx);
Bool   gmDjinnCanSet(Sint32 unitId, Sint32 element, Sint32 djinnIdx);
Sint32 gmItemCanUnitEquipDirect(Sint32 unitId, Sint32 itemId);
Sint32 gmItemIsCompatibleWithUnit(Sint32 unitId, Sint32 itemId);
Sint32 gmInventoryFindEquipped(Sint32 unitId, Sint32 type);
Sint32 gmPartyAdjustCoins(Sint32 amount);
Sint32 gmItemGetEquipmentGroup(Sint32 itemId);
Sint32 gmItemAdjustRareItemStock(Sint32 itemId, Sint32 delta);
Sint32 gmInventoryCount(Sint32 unitId);
Bool   gmPartyInventoryHasSpace(void);
Uint32 gmUnitGetExperienceForLevel(Sint32 unitId, Sint32 level);
void   gmPartyRestoreHpPp(Sint32 cureConditions);
void   gmPartyRestorePpInLighthouse(void);
void   gmPartyStartChildhood(void);
void   gmPartyEndChildhood(void);
void   gmUnitGaugeNoOperation(void);
Sint32 gmDjinnGiveToParty(Sint32 element, Sint32 djinn);
Sint32 gmPartySumDjinnCounts(Sint32 element);
Sint32 gmPartyAdjustShopCredit(Sint32 amount);
Sint32 gmPartyAdjustShopGifts(Sint32 amount);
Sint32 gmInventoryCheckDiscard(Sint32 unitId, Sint32 slot);
Sint32 gmInventoryDiscard(Sint32 unitId, Sint32 slot);
Bool   gmBattleIsReviveEffect(Sint32 effectId);
Sint32 gmUnitRefreshPsynergy(Sint32 unitId);
Uint32 gmPartyGetAverageLevel(void);
void   gmUnitUpdateClass(Sint32 unitId);
Sint32 gmInventoryCountItem(Sint32 unitId, Sint32 itemId);
Sint32 gmPartyInventoryCountItem(Sint32 itemId);
Sint32 gmPartyInventoryDiscard(Sint32 itemId);
void   gmFlagRefreshLureCap(void);
Sint32 gmEquipmentGetUnleashRateBonus(UNIT *unit);
Uint32 gmBuildStampGetTime(void);
Sint32 gmPartyInventoryCountFreeSlots(void);
void   gmInventorySnapshotSave(void);
void   gmInventorySnapshotRestore(void);

/* The 324 items by item id (item_table.c), read through gmItemGetDirect. */
extern const ITEM_DEFINITION ItemDefinitionTable[];
/* The 16 summons (summon_table.c) by summon id, the numbers SummonOrderList
   lists; read through gmSummonDefinitionGetNearStub. */
extern const SUMMON_DEFINITION SummonDefinitionTable[];

/* The 519 abilities by ability id (ability_table.c). */
extern const ABILITY_DEFINITION AbilityDefinitions[];

/* The lighthouses inside which a character's PP is refilled at an aerie
   (gmPartyRestorePpInLighthouse). */
typedef enum LIGHTHOUSE_GROUP {
	LIGHTHOUSE_GROUP_VENUS_MARS,	 /* Isaac, Garet, Felix and Jenna */
	LIGHTHOUSE_GROUP_MERCURY_JUPITER /* Ivan, Mia, Sheba and Piers */
} LIGHTHOUSE_GROUP;

/* Each character's LIGHTHOUSE_GROUP. */
extern const Uint8 CharacterLighthouseGroupTable[];
/* The experience each character needs for levels 1 to 99, 99 words a character
   (gmUnitGetExperienceForLevel). */
extern const Uint32 CharacterLevelExpTable[];
/* The Djinni sprite of each element, SPRITE_VENUS_DJINNI to
   SPRITE_JUPITER_DJINNI, whose picture mnDjinnShowJoinedMessage shows as the
   portrait. */
extern Sint16 DjinnElementSprites[];

/* The elemental power and resistance of one element level, in
   ElementPowerResistByLevel, which gmUnitCalcBaseElements reads by whole
   level, 0 to 15. */
typedef struct ELEMENT_LEVEL_BONUS {
	Uint16 power;	   /* base power, before the tenths of a level */
	Uint16 resistance; /* base resistance, likewise */
} ELEMENT_LEVEL_BONUS;

/* The power and resistance by element level (element_table.c): a table nothing
   writes, defined writable in .rodata as FontMarkGlyphCodes is. */
extern ELEMENT_LEVEL_BONUS ElementPowerResistByLevel[16];
/* The class series by a character's strongest and second element, a 4 by 4
   table read as [strongest * 4 + second] (gmClassGetSeries). */
extern const Sint32 ClassSeriesByElements[];
/* The 80 Djinn, 20 of each element in element order (djinn_table.c). */
extern const DJINN_DEFINITION DjinnDefinitionTable[];
/* Pairs of an element difference (power less resistance) and what it adds to
   the multiplier in 256ths (256 = 1x), from 200 (+256) down to -200 (-256);
   gmBattleGetElementMultiplier interpolates and adds 256. */
extern const Sint16 ElementPowerResistCurve[5][2];
/* The enemies' rows (enemy_table.c), from ENEMY_ID_FIRST to the last Djinni,
   then a closing row named 0xff. */
#define ENEMY_DEFINITION_COUNT 165
extern const ENEMY_DEFINITION EnemyDefinitions[ENEMY_DEFINITION_COUNT];
/* The enemies' element presets (element_table.c): attack element, levels and
   base power and resistance. */
extern const ENEMY_ELEMENT_PRESET EnemyElementPresetTable[];
/* The game flags, 4096 bits in 512 bytes, saved with the game: flag n is bit n
   & 7 of byte GAME_FLAG_BYTE(n). */
extern Uint8 GameFlagBytes[];
/* The party's equipment put aside while the Colosso takes it away: a sentinel,
   four 15-slot inventories, the two shortcuts and the party order. */
extern Uint16 InventorySnapshotWork[];

/* How many rare items the shops keep a stock of, and the most of each. */
enum { RARE_ITEM_STOCK_COUNT = 128, RARE_ITEM_STOCK_MAX = 99 };

/* How many of each rare item the shops' Artifacts offer, 0 to
   RARE_ITEM_STOCK_MAX, by stock number (ItemRareItemStockIndex;
   gmRareItemStockAdjust). */
extern Uint8 RareItemStock[RARE_ITEM_STOCK_COUNT];
/* The 64-byte save stamp, in the first half of the save buffer, that the Battle
   Arena writes into the current file alone. */
extern Uint8 BattleArenaSaveStamp[];
/* Each item's place in RareItemStock plus one, or 0 for an item the shops keep
   no stock of (gmItemAdjustRareItemStock). */
extern const Uint8 ItemRareItemStockIndex[];
/* Each party member's sprite, by character; Piers, who has none, keeps his
   character number. */
extern Sint16 CharacterSpriteTable[];
/* Each party member's sprite during the prologue (FLAG_CHILDHOOD):
   SPRITE_ISAAC_CHILD, SPRITE_GARET_CHILD, SPRITE_FELIX_CHILD and
   SPRITE_JENNA_CHILD in place of their own. */
extern Sint16 CharacterChildSpriteTable[];
/* The summon ids in menu order (gmSummonListLearned). */
extern const Uint8 SummonOrderList[16];
/* Ends a list of summon ids (gmSummonListLearned); the battle summon list also
   writes it over each summon it has moved to the front. */
#define SUMMON_END 32

void gmMemoryCopyBytesDirectional(
	Uint8 *first, Uint8 *second, Sint32 cnt, Sint32 direction);

const ITEM_DEFINITION *gmInventoryGetEquippedDefinition(
	UNIT *unit, Sint32 type);
Sint32						gmRareItemStockAdjust(Sint32 idx, Sint32 delta);
const CHARACTER_DEFINITION *gmCharacterGetDefinition(Sint32 characterId);
LEVEL_UP_RESULT *gmUnitLevelUp(Sint32 unitId, LEVEL_UP_RESULT *gains);

Sint32 gmClassGetSeries(Sint32 primaryElement, Sint32 secondaryElement);
Sint32 gmUnitGetElementLevels(
	Sint32 unitId, const Uint8 *djinnHeld, Sint32 output[4]);
Sint32 gmEnemyGetAttackElement(UNIT *unit);
UNREAD_RESULT
gmUnitCalcBaseElements(Sint32 unitId, BATTLE_ELEMENT_STATS *dst);
Sint32			  gmClassDetermine(Sint32 character, const Uint8 *djinn);
CLASS_DEFINITION *gmClassGetDefinition(Sint32 classId);

Sint32 gmBattleGetElementMultiplier(Sint32 difference, Sint32 halve);
Uint32 gmBattleRandomPercent(void);
/* The element power at which gmBattleCalcRecovery's healing is neither stronger
   nor weaker: each point above it counts as two points of element
   difference. */
#define HEALING_BASE_POWER	  100
/* The battle's multipliers are 256ths: this one keeps an amount as it is. */
#define BATTLE_MULTIPLIER_ONE 256
Sint32 gmBattleGetEffectBaseSuccessRate(Sint32 effectId);
Bool   gmBattleAbilityIsWeakToEffect(const UNIT *tgt, Sint32 effectId);

const DJINN_DEFINITION *gmDjinnGetDefinition(Uint32 element, Uint32 djinni);

void gmInventoryAddAndEquip(Sint32 unitId, Sint32 itemId);

#endif
