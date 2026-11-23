#ifndef ALCHEMY_FAR_CALL_GAME_H
#define ALCHEMY_FAR_CALL_GAME_H

/// @file
/// The far calls into the game bank, as the other banks' code calls them.

#include "types.h"

struct Unit;
struct EnemyDefinition;
/// Levels character unitId up one level at a time until targetLevel, then
/// recomputes its stats.
void Unit_LevelUpToFar(s32 unitId, s32 targetLevel);
/// Teaches character owner ability `ability` for good (or makes a class action
/// permanent by clearing its marks), then rebuilds its list.
void Unit_LearnPsynergyFar(s32 owner, s32 ability);
/// Equips the item in `slot`, clearing the equipped bit of the item of the same
/// type it replaces.
s32 Inventory_EquipFar(s32 unitId, s32 slot);
/// Returns the id of the unit's equipped item of `type`, or 0.
s32 Inventory_GetEquippedItemFar(struct Unit *unit, s32 type);
/// Returns 1 when the unit holds the Djinni and it can be set.
s32 Djinn_CanSetFar(s32 unitId, s32 element, s32 djinnIndex);
/// Returns the slot of the equipped item of `type`, or -1.
s32 Inventory_FindEquippedFar(s32 unitId, s32 type);
/// Returns 1 when the Djinni is set on the unit.
s32 Djinn_IsSetFar(s32 unitId, s32 element, s32 djinnIndex);
/// Puts a unit's Djinni at the end of its side's list on standby.
u32 *DjinnRecovery_AddStandbyFar(u32 unitId, u32 element, u32 djinnIndex);
/// Returns the element the unit attacks with (ELEMENT_NONE for none).
s32 Item_GetEquippedElementFar(s32 unitId);
/// Returns the unleash chance, in percent, that a unit's equipment adds
/// (EQUIP_EFFECT_ADD_UNLEASH_CHANCE).
s32 Equipment_GetUnleashRateBonusFar(struct Unit *unit);
/// Gives the item to the first party member with room, and returns who took it,
/// or -1.
s32 PartyInventory_AddFar(s32 itemId);
/// Returns the definition of enemy enemyId.
const struct EnemyDefinition *Enemy_GetDefinitionFar(s32 enemyId);
/// Writes the summons the party can call to output, in menu order, ending with
/// SUMMON_END, and returns how many.
s32 Summon_ListLearnedFar(u8 *output);
/// Takes a set Djinni off the unit and works out its class again.
void Djinn_PutOnStandbyFar(s32 unitId, s32 element, s32 djinnIndex);
/// Returns the average level of the party members, rounded down (0 with no
/// party members).
s32 Party_GetAverageLevelFar(void);
/// Adds delta to how many of a rare item the shops may sell, and returns the
/// new count.
s32 Item_AdjustCounterFar(s32 itemId, s32 delta);
/// Counts matching nonstackable items, or the quantity in the first matching
/// stack.
s32 Inventory_CountItemFar(s32 unitId, s32 itemId);
/// Removes one item as Inventory_Remove does, and puts a rare item back into
/// circulation (its counter goes up by one) for the shops to sell.
void Inventory_DiscardFar(s32 unitId, s32 slot);
/// Works out a character's class, Psynergy and base elements again.
void Unit_RefreshPsynergyFar(s32 unitId);
/// Counts `itemId` across the party.
s32 PartyInventory_CountItemFar(s32 itemId);
/// Removes one `itemId` from whoever carries it first.
void PartyInventory_RemoveFar(s32 itemId);
/// Gives a newly found Djinni to the party member holding the fewest Djinn who
/// has room, on standby.
s32 Djinn_GiveToPartyFar(s32 element, s32 djinn);
/// Returns how strongly a unit is attuned to `element`, its whole element level
/// (a tenth of what Unit_GetElementLevels counts), and 0 for `ELEMENT_NONE`.
s32 Unit_GetElementLevelFar(s32 unitId, s32 element);
/// Returns 1 for an item that is not a weapon or armor, and for those whether
/// the character can equip it.
s32 Item_IsCompatibleWithUnitFar(s32 unit, s32 item);
/// Returns the first slot holding `itemId` (any quantity, equipped or not), or
/// -1.
s32 Inventory_FindFar(s32 unitId, s32 itemId);
/// Adds `amount` to the shop credit purchases build up and Game Tickets spend,
/// kept between 0 and COIN_MAX, and returns the new credit.
s32 Party_AdjustShopCreditFar(s32 amount);
/// Clears the broken mark of the item in `slot`.
void Inventory_RepairFar(s32 unitId, s32 slot);
/// Adds `amount` to the count of Game Tickets given, which stops at
/// SHOP_GIFTS_MAX.
void Party_AdjustShopGiftsFar(s32 amount);
/// Returns an item's equipment group from its type.
s32 Item_GetEquipmentGroupFar(s32 item);
/// Counts filled inventory slots up to the first empty slot.
s32 Inventory_CountFar(s32 unitId);

/// Encodes the build stamp's time, which a new game takes as its start stamp
/// and the file list compares saves with. Declared with no prototype, as in
/// The Broken Seal: the save passes it a 0 it does not read, and the file list
/// passes nothing.
u32 BuildStamp_GetTimeFar();
/// Starts a new game's state.
void MainRoutineWork_InitializeDefaultsFar(void);
/// Returns the ability the Djinni performs when it is unleashed.
s32 Djinn_GetAbilityIdFar(s32 element, s32 index);
/// Returns how many of the item in `slot` the unit carries (0 for an empty
/// slot).
s32 Inventory_GetQuantityFar(s32 unitId, s32 slot);
/// Returns the first party member who knows Psynergy `action`, or -1.
s32 Party_FindOwnerWithActionFar(s32 action);
/// Returns 0 when the item in `slot` may be dropped, or else why not.
s32 Inventory_CheckDiscardFar(s32 unitId, s32 slot);
/// Returns whether the item in `slot` may be taken off.
s32 Inventory_CheckUnequipFar(s32 unitId, s32 slot);
/// Sets the unit's PP, clamped to 0..ppMax, and recomputes its HP and PP
/// gauges.
void Unit_SetPpFar(struct Unit *unit, s32 pp);
/// Lets the party call summon summonId from now on.
void Summon_LearnFar(s32 summonId);

#endif
