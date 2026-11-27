#ifndef ALCHEMY_GAME_STUBS_H
#define ALCHEMY_GAME_STUBS_H

// The stub calls into the game module, as the other modules' code calls them.

#include "types.h"

struct UNIT;
struct ENEMY_DEFINITION;
// Levels character unitId up one level at a time until targetLevel, then recomputes its stats.
void    gmUnitLevelUpToStub(Sint32 unitId, Sint32 targetLevel);
// Teaches character owner ability `ability` for good (or makes a class action permanent by clearing
// its marks), then rebuilds its list.
void    gmUnitLearnPsynergyStub(Sint32 owner, Sint32 ability);
// Equips the item in `slot`, clearing the equipped bit of the item of the same type it replaces.
Sint32  gmInventoryEquipStub(Sint32 unitId, Sint32 slot);
// Returns the id of the unit's equipped item of `type`, or 0.
Sint32  gmInventoryGetEquippedItemStub(struct UNIT *unit, Sint32 type);
// Returns 1 when the unit holds the Djinni and it can be set.
Sint32  gmDjinnCanSetStub(Sint32 unitId, Sint32 element, Sint32 djinnIndex);
// Returns the slot of the equipped item of `type`, or -1.
Sint32  gmInventoryFindEquippedStub(Sint32 unitId, Sint32 type);
// Returns 1 when the Djinni is set on the unit.
Sint32  gmDjinnIsSetStub(Sint32 unitId, Sint32 element, Sint32 djinnIndex);
// Puts a unit's Djinni at the end of its side's list on standby.
Uint32 *gmDjinnRecoveryAddStandbyStub(Uint32 unitId, Uint32 element, Uint32 djinnIndex);
// Returns the element the unit attacks with (ELEMENT_NONE for none).
Sint32  gmUnitGetAttackElementStub(Sint32 unitId);
// Returns the unleash chance, in percent, that a unit's equipment adds
// (EQUIP_EFFECT_ADD_UNLEASH_CHANCE).
Sint32  gmEquipmentGetUnleashRateBonusStub(struct UNIT *unit);
// Gives the item to the first party member with room, and returns who took it, or -1.
Sint32  gmPartyInventoryAddStub(Sint32 itemId);
// Returns the definition of enemy enemyId.
const struct ENEMY_DEFINITION *gmEnemyGetDefinitionStub(Sint32 enemyId);
// Writes the summons the party can call to output, in menu order, ending with SUMMON_END, and
// returns how many.
Sint32                         gmSummonListLearnedStub(Uint8 *output);
// Takes a set Djinni off the unit and works out its class again.
void   gmDjinnPutOnStandbyStub(Sint32 unitId, Sint32 element, Sint32 djinnIndex);
// Returns the average level of the party members, rounded down (0 with no party members).
Sint32 gmPartyGetAverageLevelStub(void);
// Adds delta to how many of a rare item the shops may sell, and returns the new count.
Sint32 gmItemAdjustRareItemStockStub(Sint32 itemId, Sint32 delta);
// Counts matching nonstackable items, or the quantity in the first matching stack.
Sint32 gmInventoryCountItemStub(Sint32 unitId, Sint32 itemId);
// Removes one item as gmInventoryRemove does, and puts a rare item back into circulation (its
// counter goes up by one) for the shops to sell.
void   gmInventoryDiscardStub(Sint32 unitId, Sint32 slot);
// Works out a character's class, Psynergy and base elements again.
void   gmUnitRefreshPsynergyStub(Sint32 unitId);
// Counts `itemId` across the party.
Sint32 gmPartyInventoryCountItemStub(Sint32 itemId);
// Removes one `itemId` from whoever carries it first.
void   gmPartyInventoryRemoveStub(Sint32 itemId);
// Gives a newly found Djinni to the party member holding the fewest Djinn who has room, on standby.
Sint32 gmDjinnGiveToPartyStub(Sint32 element, Sint32 djinn);
// Returns how strongly a unit is attuned to `element`, its whole element level (a tenth of what
// gmUnitGetElementLevels counts), and 0 for `ELEMENT_NONE`.
Sint32 gmUnitGetElementLevelStub(Sint32 unitId, Sint32 element);
// Returns 1 for an item that is not a weapon or armor, and for those whether the character can
// equip it.
Sint32 gmItemIsCompatibleWithUnitStub(Sint32 unit, Sint32 item);
// Returns the first slot holding `itemId` (any quantity, equipped or not), or -1.
Sint32 gmInventoryFindStub(Sint32 unitId, Sint32 itemId);
// Adds `amount` to the shop credit purchases build up and Game Tickets spend, kept between 0 and
// COIN_MAX, and returns the new credit.
Sint32 gmPartyAdjustShopCreditStub(Sint32 amount);
// Clears the broken mark of the item in `slot`.
void   gmInventoryRepairStub(Sint32 unitId, Sint32 slot);
// Adds `amount` to the count of Game Tickets given, which stops at SHOP_GIFTS_MAX.
void   gmPartyAdjustShopGiftsStub(Sint32 amount);
// Returns an item's equipment group from its type.
Sint32 gmItemGetEquipmentGroupStub(Sint32 item);
// Counts filled inventory slots up to the first empty slot.
Sint32 gmInventoryCountStub(Sint32 unitId);

// Encodes the build stamp's time, which a new game takes as its start stamp and the file list
// compares saves with. Declared with no prototype, as in The Broken Seal: the save passes it a 0 it
// does not read, and the file list passes nothing.
Uint32 gmBuildStampGetTimeStub();
// Starts a new game's state.
void   gmMainRoutineStateInitDefaultsStub(void);
// Returns the ability the Djinni performs when it is unleashed.
Sint32 gmDjinnGetAbilityIdStub(Sint32 element, Sint32 index);
// Returns how many of the item in `slot` the unit carries (0 for an empty slot).
Sint32 gmInventoryGetQuantityStub(Sint32 unitId, Sint32 slot);
// Returns the first party member who knows Psynergy `action`, or -1.
Sint32 gmPartyFindOwnerWithActionStub(Sint32 action);
// Returns 0 when the item in `slot` may be dropped, or else why not.
Sint32 gmInventoryCheckDiscardStub(Sint32 unitId, Sint32 slot);
// Returns whether the item in `slot` may be taken off.
Sint32 gmInventoryCheckUnequipStub(Sint32 unitId, Sint32 slot);
// Sets the unit's PP, clamped to 0..ppMax, and recomputes its HP and PP gauges.
void   gmUnitSetPpStub(struct UNIT *unit, Sint32 pp);
// Lets the party call summon summonId from now on.
void   gmSummonLearnStub(Sint32 summonId);
// Returns the party's PP over its maximum PP, in 16.16; 0 with no members.
Sint32 gmPartyGetPpRatioStub(void);

#endif
