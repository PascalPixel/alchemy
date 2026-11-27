#ifndef ALCHEMY_GAME_STUBS_H
#define ALCHEMY_GAME_STUBS_H

/* @file */
/* Party members, items, Djinn and game-state operations. */

struct DJINN_RECOVERY_TABLE *gmDjinnGetRecoveryTableStub(s32 enemySide);
struct UNIT *gmUnitGetStub(u32 unitId);
/* Works a unit's stats out again from its base stats, equipment, set Djinn, */
/* class and battle stat levels, keeping its HP and PP at the same fraction. */
void gmUnitCalcStub(s32 unitId);
struct ITEM_DEFINITION *gmItemGetDirectStub(s32 item);
/* Returns how many of the item in `slot` the unit carries, 0 for an empty */
/* slot (gmInventoryGetQuantity); the sell screen reads a stack's size with it.
 */
s32 gmInventoryGetQuantityStub(s32 unitId, s32 slot);
s32 gmInventoryAddItemStub(s32 unitId, s32 itemId);
s32 gmPartyInventoryAddStub(s32 itemId);
s32 gmInventoryFindStub(s32 unitId, s32 itemId);
s32 gmPartyInventoryFindMemberStub(s32 itemId);
s32 gmPartyInventoryRemoveStub(s32 itemId);
s32 gmInventoryEquipStub(s32 unitId, s32 slot);
s32 gmInventoryRemoveStub(s32 unitId, s32 slot);
s32 gmInventoryBreakStub(s32 unitId, s32 slot);
s32 gmInventoryRepairStub(s32 unitId, s32 slot);
s32 gmInventoryGetEquippedItemStub(struct UNIT *unit, s32 type);
/* Returns the definition of an ability by its low 14 bits, the first for an id
 */
/* past the table (gmAbilityGetDirect); every module reads abilities so. */
struct ABILITY_DEFINITION *mnAbilityGet(s32 abilityId);
s32 gmUnitLearnPsynergyStub();
s32 gmUnitKnowsPsynergyStub(s32 unitId, s32 abilityId);
void gmMainRoutineStateInitDefaultsStub(void);
struct LEVEL_UP_RESULT *gmUnitTryLevelUpStub(s32 unitId,
											 struct LEVEL_UP_RESULT *gains);
s32 gmFlagGetStub(s32 flag);
s32 gmFlagSetStub(s32 flag);
/* Clears the flag, returning nothing where gmFlagSetStub returns its */
/* byte. */
void gmFlagClearStub(s32 flag);
s32 gmFlagGetByteStub(s32 flag);
void gmFlagSetByteStub(s32 flag, s32 value);
/* Adds `delta` to the unit's HP, kept between 0 and its maximum, updates its */
/* gauges and returns the new HP. */
s32 gmUnitAddHpStub(s32 unitId, s32 delta);
/* Adds `delta` to the unit's PP, kept between 0 and its maximum, updates its */
/* gauges and returns the new PP. */
s16 gmUnitAddPpStub(s32 unitId, s32 delta);
void gmUnitRecalculateGaugesStub(s32 unitId);
/* Sets the unit's HP, kept between 0 and its maximum, and updates its gauges.
 */
void gmUnitSetHpStub(struct UNIT *unit, s32 hp);
s32 btUnitSetStub(s32 unitId, s32 enemyId, s32 suffix);
s32 gmPartyCountMembersStub(void);
s32 gmPartyAddMemberStub(s32 unitId);
s32 gmPartyListMembersStub(s16 *unitIds);
s32 gmEquipmentRollUnleashStub(struct UNIT *unit);
s32 gmPartyRemoveMemberStub(s32 unitId);
s32 gmUnitGetAttackElementStub(s32 unitId);
s32 btRollEffectSuccessStub(s32 caster, s32 target, s32 element, s32 effectId,
							s32 successScale);
s32 btCalcAttackDamageStub(s32 attack, s32 defense, s32 addedPower,
						   s32 elementDifference);
s32 btCalcPsynergyPowerStub(s32 power, s32 elementDifference,
							s32 powerMultiplier);
s32 btCalcRecoveryStub(s32 power, s32 elementPower, s32 powerMultiplier);
const struct ENEMY_DEFINITION *btEnemyGetDefinitionStub(s32 enemyId);
s32 btRandomStub(void);
s32 gmDjinnGiveStub(s32 unitId, s32 element, s32 djinnIndex);
s32 gmDjinnSetStub(s32 unitId, s32 element, s32 djinnIndex);
u32 gmDjinnPutOnStandbyStub(s32 unitId, s32 element, s32 djinnIndex);
s32 btDjinnRecoveryRemoveStub(s32 unitId, s32 element, s32 djinnIndex);
u32 *btDjinnRecoveryAddStandbyStub(u32 unitId, u32 element, u32 djinnIndex);
s32 gmDjinnTransferStub(s32 source, s32 element, s32 djinnIndex, s32 target);
s32 gmSummonListLearnedStub(u8 *output);
/* Returns a summon's definition, or NULL past the sixteenth */
/* (gmSummonDefinitionGetNear). */
const struct SUMMON_DEFINITION *gmSummonDefinitionGet(u32 summonId);
s32 gmDjinnGetAbilityIdStub(u32 element, u32 djinni);
void gmUnitLevelUpToStub(s32 unitId, s32 targetLevel);
s32 gmUnitGetElementLevelStub(s32 unitId, s32 element);
u32 gmDjinnIsSetStub(s32 unitId, s32 element, s32 djinnIndex);
s32 gmDjinnCanSetStub(s32 unitId, s32 element, s32 djinnIndex);
/* Returns whether the character's class may equip the item, 0 for any unit */
/* past the eight characters (gmItemCanUnitEquipDirect). */
s32 gmItemCanUnitEquip(s32 unitId, s32 itemId);
s32 gmItemIsCompatibleWithUnitStub(s32 unitId, s32 itemId);
s32 gmInventoryFindEquippedStub(s32 unitId, s32 type);
s32 gmPartyAdjustCoinsStub(s32 amount);
s32 gmItemGetEquipmentGroupStub(s32 itemId);
s32 gmItemAdjustRareItemStockStub(s32 itemId, s32 delta);
s32 gmInventoryCountStub(s32 unitId);
s32 gmPartyInventoryHasSpaceStub(void);
u32 gmUnitGetExperienceForLevelStub(s32 unitId, s32 level);
void gmPartyRestoreHpPpStub(s32 cureConditions);
void gmPartyRestorePpInLighthouseStub(void);
void gmPartyStartChildhoodStub(void);
void gmPartyEndChildhoodStub(void);
s32 gmDjinnGiveToPartyStub(s32 element, s32 djinn);
s32 gmPartySumDjinnCountsStub(s32 element);
/* Adds `amount` to the shop credit purchases build up and Game Tickets spend,
 */
/* kept between 0 and COIN_MAX, and returns the new credit. */
s32 gmPartyAdjustShopCreditStub(s32 amount);
/* Adds `amount` to the count of Game Tickets given, which stops at */
/* SHOP_GIFTS_MAX and picks the next ticket's credit cost, and returns the new
 */
/* count. */
s32 gmPartyAdjustShopGiftsStub(s32 amount);
s32 gmInventoryCheckDiscardStub(s32 unitId, s32 slot);
s32 gmInventoryDiscardStub(s32 unitId, s32 slot);
s32 btIsReviveEffectStub(s32 effectId);
s32 gmUnitRefreshPsynergyStub(s32 unitId);
u32 gmPartyGetAverageLevelStub(void);
s32 gmInventoryCountItemStub(s32 unitId, s32 itemId);
s32 gmPartyInventoryCountItemStub(s32 itemId);
s32 gmPartyInventoryDiscardStub(s32 itemId);
void gmFlagRefreshLureCapStub(void);
s32 gmEquipmentGetUnleashRateBonusStub(struct UNIT *unit);
u32 gmBuildStampGetTimeStub();
s32 gmPartyInventoryCountFreeSlotsStub(void);
void gmInventorySnapshotSaveStub(void);
void gmInventorySnapshotRestoreStub(void);

#endif
