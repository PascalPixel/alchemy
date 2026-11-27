#ifndef ALCHEMY_GAME_STUBS_H
#define ALCHEMY_GAME_STUBS_H

// Party members, items, Djinn and game-state operations.

struct DJINN_RECOVERY_TABLE *gmDjinnGetRecoveryTableStub(Sint32 enemySide);
struct UNIT                 *gmUnitGetStub(Uint32 unitId);
// Works a unit's stats out again from its base stats, equipment, set Djinn, class and battle stat
// levels, keeping its HP and PP at the same fraction.
void                         gmUnitCalculateStub(Sint32 unitId);
struct ITEM_DEFINITION      *gmItemGetDirectStub(Sint32 item);
// Returns how many of the item in `slot` the unit carries, 0 for an empty slot
// (gmInventoryGetQuantity); the sell screen reads a stack's size with it.
Sint32                       gmInventoryGetQuantityStub(Sint32 unitId, Sint32 slot);
Sint32                       gmInventoryAddItemStub(Sint32 unitId, Sint32 itemId);
Sint32                       gmPartyInventoryAddStub(Sint32 itemId);
Sint32                       gmInventoryFindStub(Sint32 unitId, Sint32 itemId);
Sint32                       gmPartyInventoryFindMemberStub(Sint32 itemId);
Sint32                       gmPartyInventoryRemoveStub(Sint32 itemId);
Sint32                       gmInventoryEquipStub(Sint32 unitId, Sint32 slot);
Sint32                       gmInventoryRemoveStub(Sint32 unitId, Sint32 slot);
Sint32                       gmInventoryBreakStub(Sint32 unitId, Sint32 slot);
Sint32                       gmInventoryRepairStub(Sint32 unitId, Sint32 slot);
Sint32                       gmInventoryGetEquippedItemStub(struct UNIT *unit, Sint32 type);
// Returns the definition of an ability by its low 14 bits, the first for an id past the table
// (gmAbilityGetDirect); every module reads abilities so.
struct ABILITY_DEFINITION   *gmAbilityGetDirectStub(Sint32 abilityId);
Sint32                       gmUnitLearnPsynergyStub();
Sint32                       gmUnitKnowsPsynergyStub(Sint32 unitId, Sint32 abilityId);
void                         gmMainRoutineStateInitDefaultsStub(void);
struct LEVEL_UP_RESULT      *gmUnitTryLevelUpStub(Sint32 unitId, struct LEVEL_UP_RESULT *gains);
Sint32                       gmFlagGetStub(Sint32 flag);
Sint32                       gmFlagSetStub(Sint32 flag);
// Clears the flag, returning nothing where gmFlagSetStub returns its byte.
void                         gmFlagClearStub(Sint32 flag);
Sint32                       gmFlagGetByteStub(Sint32 flag);
void                         gmFlagSetByteStub(Sint32 flag, Sint32 value);
// Adds `delta` to the unit's HP, kept between 0 and its maximum, updates its gauges and returns the
// new HP.
Sint32                       gmUnitAddHpStub(Sint32 unitId, Sint32 delta);
// Adds `delta` to the unit's PP, kept between 0 and its maximum, updates its gauges and returns the
// new PP.
Sint16                       gmUnitAddPpStub(Sint32 unitId, Sint32 delta);
void                         gmUnitRecalculateGaugesStub(Sint32 unitId);
// Sets the unit's HP, kept between 0 and its maximum, and updates its gauges.
void                         gmUnitSetHpStub(struct UNIT *unit, Sint32 hp);
Sint32                       gmBattleUnitSetStub(Sint32 unitId, Sint32 enemyId, Sint32 suffix);
Sint32                       gmPartyCountMembersStub(void);
Sint32                       gmPartyAddMemberStub(Sint32 unitId);
Sint32                       gmPartyListMembersStub(Sint16 *unitIds);
Sint32                       gmEquipmentRollUnleashStub(struct UNIT *unit);
Sint32                       gmPartyRemoveMemberStub(Sint32 unitId);
Sint32                       gmUnitGetAttackElementStub(Sint32 unitId);
Sint32                       gmBattleRollEffectSuccessStub(
						  Sint32 caster, Sint32 target, Sint32 element, Sint32 effectId, Sint32 successScale);
Sint32 gmBattleCalculateAttackDamageStub(Sint32 attack,
                                         Sint32 defense,
                                         Sint32 addedPower,
                                         Sint32 elementDifference);
Sint32
gmBattleCalculatePsynergyPowerStub(Sint32 power, Sint32 elementDifference, Sint32 powerMultiplier);
Sint32 gmBattleCalculateRecoveryStub(Sint32 power, Sint32 elementPower, Sint32 powerMultiplier);
const struct ENEMY_DEFINITION *gmEnemyGetDefinitionStub(Sint32 enemyId);
Sint32                         gmBattleRandomStub(void);
Sint32                         gmDjinnGiveStub(Sint32 unitId, Sint32 element, Sint32 djinnIndex);
Sint32                         gmDjinnSetStub(Sint32 unitId, Sint32 element, Sint32 djinnIndex);
Uint32  gmDjinnPutOnStandbyStub(Sint32 unitId, Sint32 element, Sint32 djinnIndex);
Sint32  gmDjinnRecoveryRemoveStub(Sint32 unitId, Sint32 element, Sint32 djinnIndex);
Uint32 *gmDjinnRecoveryAddStandbyStub(Uint32 unitId, Uint32 element, Uint32 djinnIndex);
Sint32  gmDjinnTransferStub(Sint32 src, Sint32 element, Sint32 djinnIndex, Sint32 target);
Sint32  gmSummonListLearnedStub(Uint8 *output);
// Returns a summon's definition, or NULL past the sixteenth (gmSummonDefinitionGetNear).
const struct SUMMON_DEFINITION *gmSummonDefinitionGetNearStub(Uint32 summonId);
Sint32                          gmDjinnGetAbilityIdStub(Uint32 element, Uint32 djinni);
void                            gmUnitLevelUpToStub(Sint32 unitId, Sint32 targetLevel);
Sint32                          gmUnitGetElementLevelStub(Sint32 unitId, Sint32 element);
Uint32                          gmDjinnIsSetStub(Sint32 unitId, Sint32 element, Sint32 djinnIndex);
Sint32                          gmDjinnCanSetStub(Sint32 unitId, Sint32 element, Sint32 djinnIndex);
// Returns whether the character's class may equip the item, 0 for any unit past the eight
// characters (gmItemCanUnitEquipDirect).
Sint32                          gmItemCanUnitEquipDirectStub(Sint32 unitId, Sint32 itemId);
Sint32                          gmItemIsCompatibleWithUnitStub(Sint32 unitId, Sint32 itemId);
Sint32                          gmInventoryFindEquippedStub(Sint32 unitId, Sint32 type);
Sint32                          gmPartyAdjustCoinsStub(Sint32 amount);
Sint32                          gmItemGetEquipmentGroupStub(Sint32 itemId);
Sint32                          gmItemAdjustRareItemStockStub(Sint32 itemId, Sint32 delta);
Sint32                          gmInventoryCountStub(Sint32 unitId);
Sint32                          gmPartyInventoryHasSpaceStub(void);
Uint32                          gmUnitGetExperienceForLevelStub(Sint32 unitId, Sint32 level);
void                            gmPartyRestoreHpPpStub(Sint32 cureConditions);
void                            gmPartyRestorePpInLighthouseStub(void);
void                            gmPartyStartChildhoodStub(void);
void                            gmPartyEndChildhoodStub(void);
Sint32                          gmDjinnGiveToPartyStub(Sint32 element, Sint32 djinn);
Sint32                          gmPartySumDjinnCountsStub(Sint32 element);
// Adds `amount` to the shop credit purchases build up and Game Tickets spend, kept between 0 and
// COIN_MAX, and returns the new credit.
Sint32                          gmPartyAdjustShopCreditStub(Sint32 amount);
// Adds `amount` to the count of Game Tickets given, which stops at SHOP_GIFTS_MAX and picks the
// next ticket's credit cost, and returns the new count.
Sint32                          gmPartyAdjustShopGiftsStub(Sint32 amount);
Sint32                          gmInventoryCheckDiscardStub(Sint32 unitId, Sint32 slot);
Sint32                          gmInventoryDiscardStub(Sint32 unitId, Sint32 slot);
Sint32                          gmBattleIsReviveEffectStub(Sint32 effectId);
Sint32                          gmUnitRefreshPsynergyStub(Sint32 unitId);
Uint32                          gmPartyGetAverageLevelStub(void);
Sint32                          gmInventoryCountItemStub(Sint32 unitId, Sint32 itemId);
Sint32                          gmPartyInventoryCountItemStub(Sint32 itemId);
Sint32                          gmPartyInventoryDiscardStub(Sint32 itemId);
void                            gmFlagRefreshLureCapStub(void);
Sint32                          gmEquipmentGetUnleashRateBonusStub(struct UNIT *unit);
Uint32                          gmBuildStampGetTimeStub();
Sint32                          gmPartyInventoryCountFreeSlotsStub(void);
void                            gmInventorySnapshotSaveStub(void);
void                            gmInventorySnapshotRestoreStub(void);

#endif
