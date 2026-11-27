#ifndef ALCHEMY_GAME_FUNCTIONS_H
#define ALCHEMY_GAME_FUNCTIONS_H

/* @file */
/* The game module's functions, declared once each in the order of its */
/* stub table, without the inline game.h brings in with */
/* battle/battle_unit.h. It is for the units that must not see that inline: */
/* the scenes Apojii Islands, Naribwe and Jupiter Lighthouse's Erinyes Tunic */
/* chest room, which have it through system_call.h. game.h includes it for */
/* everyone else. */

#include "types.h"
#include "fixed_math.h"

struct ABILITY_DEFINITION;
struct DJINN_RECOVERY_TABLE;
struct ENEMY_DEFINITION;
struct ITEM_DEFINITION;
struct LEVEL_UP_RESULT;
struct SUMMON_DEFINITION;
struct UNIT;

/* The game module's functions in the order of its stub table */
/* (src/game/stubs.s). */
/* Returns the party's Djinn recovery bank, the standby Djinn and their turns */
/* to recover, or for the enemy side a battle unit read as one. */
struct DJINN_RECOVERY_TABLE *gmDjinnGetRecoveryTable(s32 enemySide);
/* Works out a unit's stats from its base stats, equipment, set Djinn, class */
/* and battle stat levels; its HP and PP keep their share of a new maximum. */
void gmUnitCalc(s32 unitId);
/* Returns an item's definition; an inventory entry's other bits are ignored. */
struct ITEM_DEFINITION *gmItemGetDirect(s32 item);
/* Returns how many of the item in `slot` the unit carries (0 for an empty */
/* slot). The entry stores one less than the count. */
s32 gmInventoryGetQuantity(s32 unitId, s32 slot);
/* Gives a unit one item, joining its first matching stack (up to */
/* INVENTORY_STACK_MAX) or taking the first free slot. A full matching stack */
/* fails immediately, even if another slot is free. */
s32 gmInventoryAddItem(s32 unitId, s32 itemId);
/* Gives the item to the first party member with room. Returns who took it, or
 */
/* -1. */
s32 gmPartyInventoryAdd(s32 itemId);
/* Returns the first slot holding `itemId` (any quantity, equipped or not), or
 */
/* -1. */
s32 gmInventoryFind(s32 unitId, s32 itemId);
/* Returns who carries `itemId`, asking the leader first, or -1. */
s32 gmPartyInventoryFindMember(s32 itemId);
/* Removes one `itemId` from whoever carries it first; always returns 0. */
s32 gmPartyInventoryRemove(s32 itemId);
/* Equips the item in `slot`, clearing the equipped bit of the item of the same
 */
/* type it replaces. Psynergy items may coexist. Recomputes the unit's Psynergy
 */
/* and stats. Returns 0, -1 when the unit cannot equip it, or -2 when the item
 */
/* it would replace is cursed on. */
s32 gmInventoryEquip(s32 unitId, s32 slot);
/* Takes one item out of `slot`, closing up the inventory when it was the last.
 */
/* The Psynergy list is not rebuilt, so a removed item's Psynergy stays listed
 */
/* until something rebuilds it. */
s32 gmInventoryRemove(s32 unitId, s32 slot);
/* Marks the item in `slot` broken; returns -1 for an empty slot. */
s32 gmInventoryBreak(s32 unitId, s32 slot);
/* Clears the broken mark of the item in `slot`; returns -1 for an empty slot.
 */
s32 gmInventoryRepair(s32 unitId, s32 slot);
/* Returns how the action an item performs when used picks its targets. */
/* Nothing calls it or its stub. */
u8 gmItemGetTargetMode(s32 itemId);
/* Returns the id of the unit's equipped item of `type`, or 0. */
s32 gmInventoryGetEquippedItem(struct UNIT *unit, s32 type);
/* Returns the definition of ability `abilityId` (its low 14 bits); an id */
/* past the table gives the first. Every module reads an ability through its */
/* stub call, btAbilityGet. */
struct ABILITY_DEFINITION *gmAbilityGetDirect(s32 abilityId);
/* Teaches character `owner` ability `ability` for good (or makes a class */
/* action permanent by clearing its marks), then rebuilds its list. Returns the
 */
/* action's place in the list, or -1 when the list is full. */
s32 gmUnitLearnPsynergy(s32 owner, s32 ability);
/* Returns 1 when `abilityId` is in the unit's Psynergy list. Called by the */
/* events that check whether a party member knows a Psynergy */
/* (fdPartyCheckShortcut). */
s32 gmUnitKnowsPsynergy(s32 unitId, s32 abilityId);
/* Starts a new game's state: clears MainRoutineStateWork, the scene state and
 */
/* the snapshots, the flags and the characters by DMA, builds the characters, */
/* sets the defaults (the button layout, Felix and Jenna in the party, their */
/* first Psynergy, NEW_GAME_COINS, the message settings and the play time */
/* clock) and fills the two tables after the rare item stock. */
void gmMainRoutineStateInitDefaults(void);
/* Levels the character up once if it has the experience, and returns gains, or
 */
/* NULL. */
struct LEVEL_UP_RESULT *gmUnitCheckLevelUp(s32 unitId,
										   struct LEVEL_UP_RESULT *gains);
/* Sets up all eight characters for a new game: names from the message table, */
/* starting equipment equipped, class and elements worked out, and levels */
/* raised to their starting level. */
void gmPartyInitCharacters(void);
/* Unused: reserved entry; keeps its place in the stub table. */
void gmUnitLevelNoOp(void);
/* Levels the character up once if its experience reaches the next level, and */
/* returns gains, or NULL. */
struct LEVEL_UP_RESULT *gmUnitTryLevelUp(s32 unitId,
										 struct LEVEL_UP_RESULT *gains);
/* Starts a new game: clears the main routine's state and restarts at the */
/* opening. */
void gmRestartQuest(void);
/* Adds delta to a unit's HP, kept between 0 and its maximum, updates its */
/* gauges and returns the new HP. */
s32 gmUnitAddHp(s32 unitId, s32 delta);
/* Adds delta to unit unitId's PP, clamped to 0 and its maximum, recomputes its
 */
/* gauges and returns the new PP. */
s16 gmUnitAddPp(s32 unitId, s32 delta);
/* Recomputes the HP and PP gauges of `unitId` from its current HP and PP, so */
/* that gmUnitCalc keeps the same fractions. */
void gmUnitRecalculateGauges(s32 unitId);
/* Sets the unit's HP, clamped to 0 and its maximum, and recomputes its HP and
 */
/* PP gauges. */
void gmUnitSetHp(struct UNIT *unit, s32 value);
/* Sets the unit's PP, clamped to 0 and its maximum, and recomputes its HP and
 */
/* PP gauges. */
void gmUnitSetPp(struct UNIT *unit, s32 value);
/* Fills a battle unit with an enemy: its stats, its name with its suffix, its
 */
/* items, rewards and elements. A Djinni fought is put on the party's side */
/* (this folder's README.md, "The unit"). Returns 0 for an id out of range. */
s32 gmBattleUnitSet(s32 unitId, s32 enemyId, s32 suffix);
/* Returns the number of party members, counted from its FLAG_IN_PARTY flags. */
s32 gmPartyCountMembers(void);
/* Adds a character to the end of the party unless it is there already, and */
/* returns the party size. */
s32 gmPartyAddMember(s32 unitId);
/* Writes the party members to `unitIds`, ended by BATTLE_UNIT_LIST_END, and */
/* returns how many there are. */
s32 gmPartyListMembers(s16 *unitIds);
/* Rolls whether a character's weapon unleashes, 5 percent a point of its */
/* chance. Returns the unleash's ability, or ABILITY_ATTACK for a plain attack.
 */
s32 gmEquipmentRollUnleash(struct UNIT *owner);
/* Takes character `unitId` out of the party: clears its game flag and closes */
/* the gap it leaves in MainRoutineStateWork.party. Returns the party size. */
s32 gmPartyRemoveMember(s32 unitId);
/* Returns the element the unit attacks with (ELEMENT_NONE for none): an */
/* enemy's comes from its kind, a character's from its equipped weapon. */
s32 gmUnitGetAttackElement(s32 unitId);
/* Rolls whether effect `effectId` takes hold on `target`: it fails at once */
/* when there is nothing for it to do. The chance is the base rate, plus three
 */
/* times the caster's lead over the target in `element`'s level less half */
/* the target's luck (so every two points of luck take off three), plus */
/* WEAKNESS_BONUS against a weakness, times `successScale` percent. A base rate
 */
/* of 0 or less counts as its magnitude, still scaled but skipping the levels,
 */
/* luck and weakness; 0 still succeeds when gmBattleRandomPercent gives 0, */
/* about 1 time in 100. Luff, Gasp and Fire Puppet's Paralyze get three tries.
 */
/* Raiden's Wrath and the Trident take hold only on a dragon or a sea creature;
 */
/* a target with LUCK_IMMUNITY Luck or more shrugs off poison, haunting, */
/* instant death, the curse, Leave One HP, Gasp and Gale, and a character Gale
 */
/* too. */
s32 gmBattleRollEffectSuccess(s32 caster, s32 target, s32 element, s32 effectId,
							  s32 successScale);
/* Scales max(attack - defense, 0) plus twice the added power by half the */
/* element multiplier, then halves it. */
s32 gmBattleCalcAttackDamage(s32 attack, s32 defense, s32 addedPower,
							 s32 elementDifference);
/* Returns Psynergy's power times the element multiplier and powerMultiplier. */
s32 gmBattleCalcPsynergyPower(s32 power, s32 elementDifference,
							  s32 powerMultiplier);
/* Returns the amount healed: power times powerMultiplier and the multiplier */
/* for the healer's element power above HEALING_BASE_POWER. */
s32 gmBattleCalcRecovery(s32 power, s32 elementPower, s32 powerMultiplier);
/* Returns the row of enemy_table.c for `enemyId` (see ENEMY_ROW). */
const struct ENEMY_DEFINITION *gmEnemyGetDefinition(s32 enemyId);
/* The battle's random number generator, the same as syRandom with its own */
/* seed: returns bits 8 to 23 of the new seed. */
u32 gmBattleRandom(void);
/* Gives a Djinni to a unit. Returns -1 when the unit holds */
/* DJINN_HELD_PER_ELEMENT_MAX of the element or has it already. */
s32 gmDjinnGive(s32 unitId, s32 element, s32 djinnIndex);
/* Sets a ready Djinni on the unit, counts it and works out the unit's class */
/* again. Returns 1 when it was set. */
s32 gmDjinnSet(s32 unitId, s32 element, s32 djinnIndex);
/* Takes a set Djinni off the unit and works out its class again. Returns 1 */
/* when it was set. */
u32 gmDjinnPutOnStandby(s32 unitId, s32 element, s32 djinnIndex);
/* Takes the Djinni off its side's standby and recovery list, closing the gap.
 */
/* Returns 1 when it was there. */
s32 gmDjinnRecoveryRemove(s32 unitId, s32 element, s32 djinnIndex);
/* Puts a unit's Djinni at the end of its side's list on standby. Nothing */
/* checks the list's capacity. */
u32 *gmDjinnRecoveryAddStandby(u32 unitId, u32 element, u32 djinnIndex);
/* Moves a Djinni from one unit to another, set or on standby as it was. */
/* Returns -1 when it cannot. */
s32 gmDjinnTransfer(s32 source, s32 element, s32 djinnIndex, s32 target);
/* Writes the summons the party can call to `output`, in menu order, ending */
/* with SUMMON_END, and returns how many. */
s32 gmSummonListLearned(u8 *output);
/* Returns summon `summonId`'s definition, or NULL past the last. */
const struct SUMMON_DEFINITION *gmSummonDefinitionGetNear(u32 summonId);
/* Returns the ability the Djinni performs when it is unleashed. */
u16 gmDjinnGetAbilityId(u32 element, u32 djinn);
/* Levels character `unitId` up one level at a time until `targetLevel`, then */
/* recomputes its stats. */
void gmUnitLevelUpTo(s32 unitId, s32 targetLevel);
/* Returns how strongly a unit is attuned to `element`, its whole element level
 */
/* (a tenth of what gmUnitGetElementLevels counts), and 0 for `ELEMENT_NONE`. */
s32 gmUnitGetElementLevel(s32 unitId, s32 element);
/* Counts the party's Djinn on standby, and by element into `counts` when it */
/* is given. */
s32 gmDjinnRecoveryCountStandby(u8 *counts);
/* Returns 1 when the Djinni is set on the unit. */
u32 gmDjinnIsSet(s32 unitId, s32 element, s32 djinnIndex);
/* Returns 1 when the unit holds the Djinni and it can be set: not set, not */
/* recovering and not held by a summon. */
s32 gmDjinnCanSet(s32 unitId, s32 element, s32 djinnIndex);
/* Returns whether the character can equip the item: the bit of its equipMask */
/* for who the unit is, and 0 for anyone past PIERS. */
s32 gmItemCanUnitEquipDirect(s32 unitId, s32 itemId);
/* Returns 1 for an item that is not a weapon or armor, and for those whether */
/* the character can equip it. */
s32 gmItemIsCompatibleWithUnit(s32 unit, s32 item);
/* Returns the slot of the equipped item of `type`, or -1. */
s32 gmInventoryFindEquipped(s32 unitId, s32 type);
/* Adds `amount` to the party's coins, kept between 0 and COIN_MAX, and */
/* returns the new total. */
s32 gmPartyAdjustCoins(s32 amount);
/* Returns an item's equipment group from its type: weapons, tridents and the */
/* class-changing items count as weapons, armor, shields, helms, boots and */
/* shirts as armor, and anything else as neither. */
s32 gmItemGetEquipmentGroup(s32 itemId);
/* Adds `delta` to how many of a rare item the shops may sell, and returns the
 */
/* new count; ordinary items have none. */
s32 gmItemAdjustRareItemStock(s32 itemId, s32 delta);
/* Counts filled inventory slots up to the first empty slot; the inventory is */
/* kept closed up, so this is how many items the unit carries. */
s32 gmInventoryCount(s32 unitId);
/* Returns 1 when the leader or any party member has a free inventory slot. */
s32 gmPartyInventoryHasSpace(void);
/* Returns the experience a character needs for `level`: 0 below level 1, and */
/* -1 for an enemy or a level past UNIT_LEVEL_MAX. */
u32 gmUnitGetExperienceForLevel(s32 unitId, s32 level);
/* Restores every party member's HP and PP; with `cureConditions` 1 it also */
/* cures poison and haunting. */
void gmPartyRestoreHpPp(s32 cureConditions);
/* Restores the PP of each party member whose element group's lighthouse */
/* the party is inside: Venus or Mars for Isaac, Garet, Felix and Jenna, */
/* Mercury or Jupiter for the others. Nothing calls it or its stub. */
void gmPartyRestorePpInLighthouse(void);
/* Makes Jenna the leader. */
void gmPartySetLeaderJenna(void);
/* Makes Felix the leader, with Jenna and Sheba out of the party. */
void gmPartySetLeaderFelix(void);
/* Gives Piers his two Mercury Djinn, Spring and Shade (Mercury 9 and 10), */
/* set, then evens out the party's Djinn and recalculates its members. */
/* Kibombo calls it when Piers joins, and the world map */
/* (WORLD_MAP_PIERS_DJINN). */
void gmPartyGivePiersDjinn(void);
/* Does nothing; nothing calls gmUnitReservedNoOpStub. */
void gmUnitReservedNoOp(void);
/* The data transfer from Golden Sun ("Transfer data from Golden Sun", */
/* MsgTransferDataFromGoldenSun): copies a The Broken Seal save's Djinn, six of
 */
/* its flags, its coins, its options (when it came by link rather than by */
/* password) and its four units into this game's state. */
void gmDataTransferApply(u8 *save, s32 byPassword);
/* Isaac's party joins: the transferred Djinn flags move to their places, */
/* Isaac, Garet, Ivan and Mia join with their Djinn evened out and restored, */
/* a missing Djinni of each element is picked, and the transferred items and */
/* coins are given. Contigo calls it. */
void gmPartyAddIsaacsParty(void);
/* Gives a newly found Djinni to the party member holding the fewest Djinn who
 */
/* has room, on standby. Returns who got it, -1 if found before, -2 when nobody
 */
/* has room. */
s32 gmDjinnGiveToParty(s32 element, s32 djinn);
/* Returns how many Djinn of `element` the party holds, or of all elements for
 */
/* -1. */
s32 gmPartySumDjinnCounts(s32 element);
/* Adds `amount` to the shop credit, kept between 0 and COIN_MAX, and returns */
/* the new credit. */
s32 gmPartyAdjustShopCredit(s32 amount);
/* Adds `amount` to the Game Tickets given, kept between 0 and SHOP_GIFTS_MAX,
 */
/* and returns the new count. */
s32 gmPartyAdjustShopGifts(s32 amount);
/* Returns 0 when the item in `slot` may be dropped, or else why not, as */
/* INVENTORY_REFUSAL bits: the item is indispensable, it is equipped and cannot
 */
/* be taken off, or the slot is empty. */
s32 gmInventoryCheckDiscard(s32 unitId, s32 slot);
/* Returns 0 when the item in `slot` may be taken off, or else why not: it is */
/* equipped and cannot be (INVENTORY_REFUSE_CANNOT_UNEQUIP), or the slot is */
/* empty. */
s32 gmInventoryCheckUnequip(s32 unitId, s32 slot);
/* Removes one item as gmInventoryRemove does, and puts a rare item back into */
/* circulation (its counter goes up by one) for the shops to sell. */
s32 gmInventoryDiscard(s32 unitId, s32 slot);
/* Returns 1 for the revive effects, which only work on a downed unit. */
s32 gmBattleIsReviveEffect(s32 effectId);
/* Works out a character's class, Psynergy and base elements again. */
void gmUnitRefreshPsynergy(s32 unitId);
/* Returns the average level of the party members, rounded down (0 with no */
/* party members). */
u32 gmPartyGetAverageLevel(void);
/* Works out a character's class again after its Djinn or its class-changing */
/* item change, then its Psynergy list and its base elements. */
void gmUnitUpdateClass(s32 unitId);
/* Counts matching nonstackable items, or the quantity in the first matching */
/* stack. */
s32 gmInventoryCountItem(s32 unitId, s32 itemId);
/* Counts `itemId` across the party. */
s32 gmPartyInventoryCountItem(s32 itemId);
/* Discards one `itemId` from whoever carries it first, as gmInventoryDiscard */
/* does; always returns 0. */
s32 gmPartyInventoryDiscard(s32 itemId);
/* Sets FLAG_LURE_CAP when any party member has the Lure Cap (the one item with
 */
/* EQUIP_EFFECT_LURE) equipped, and clears it otherwise. */
void gmFlagRefreshLureCap(void);
/* Returns the unleash chance, in percent, that a unit's equipment adds */
/* (EQUIP_EFFECT_ADD_UNLEASH_CHANCE). */
s32 gmEquipmentGetUnleashRateBonus(struct UNIT *unit);
/* Encodes the build stamp's time, which a new game takes as its start stamp */
/* and the file list compares saves with. */
u32 gmBuildStampGetTime(void);
/* Returns the free inventory slots of the whole party. */
s32 gmPartyInventoryCountFreeSlots(void);
/* Saves the inventories of Felix's party (FELIX to PIERS), the shortcuts and */
/* the party order as the leader and Moapa set off on the Trial Road */
/* (scShamanVillageTakeLeftPath and its twin), then drops every weapon, armor,
 */
/* shield and helm not equipped (and, from patch 1, every item that raises a */
/* stat), sets FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT and restores everyone's HP and */
/* PP. As The Broken Seal's Colosso snapshot; nothing is saved again while a */
/* snapshot is held. */
void gmInventorySnapshotSave(void);
/* Gives back the inventories, shortcuts and party order */
/* gmInventorySnapshotSave put aside, empties the snapshot and clears */
/* FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT: at the end of the Trial Road, and from the
 */
/* clear scene. */
void gmInventorySnapshotRestore(void);
/* Returns the first party member who knows Psynergy `action`, or -1. */
s32 gmPartyFindOwnerWithAction(s32 action);
/* Returns the first party member with room for `itemId`, found by giving it */
/* and taking it back, or -1. */
s32 gmPartyInventoryFindRoom(s32 itemId);
/* Lets the party call summon `summonId` from now on, setting its bit in the */
/* party's summon mask. A summon tablet and Lemuria's spring call it. */
void gmSummonLearn(s32 summonId);
/* Counts the Djinn recovery down, then sets every Djinni each party member */
/* holds and can set, taking it off the recovery list. */
void gmDjinnSetAllForParty(void);
/* gmInventoryClearSlot through its stub call: empties an inventory slot and */
/* returns 2. */
s32 gmInventoryClearSlot(s32 unitId, s32 slot);
/* Takes `count` PP from the party, one at a time from the fullest member. */
void gmPartyDrainPp(s32 count);
/* Returns the party's PP over its maximum PP, in 16.16; 0 with no members. */
FIXED gmPartyGetPpRatio(void);
/* Stands in for transferred data when the player transfers none: unless the */
/* transferred Djinn are already set, sets the party aside, makes Isaac's */
/* party the party, gives it Flint and Fizz, then hands out */
/* DEFAULT_TRANSFER_DJINN random Djinn of The Broken Seal's still unowned, */
/* keeping the elements' counts within one, each with its chance in */
/* DataTransfer_DjinnChances; then moves the Djinn flags to the transferred */
/* ones and puts the party back. */
void gmDataTransferGiveDefaultDjinn(void);
/* Returns how many Djinn of `element` the unit holds, or of every element for
 */
/* -1. */
s32 gmOwnerSumDjinnCounts(s32 unitId, s32 element);
/* Carries Clear Data over to a new quest: the options, coins and each unit's */
/* base stats, level and experience. */
void gmClearDataApply(u8 *save);

#endif
