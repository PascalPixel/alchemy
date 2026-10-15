#ifndef ALCHEMY_UNIT_GAME_GAME_H
#define ALCHEMY_UNIT_GAME_GAME_H

#include "types.h"

struct BtlAct;
struct BtlUnit;
struct CharacterDefinition;
struct ClassDefinition;
struct ItemDefinition;
struct LevelUpResult;
struct SummonDefinition;

void Owner_RefreshAndResetZero(void);
void *Trade_GetOfferState(s32 arg0);
u32 Party_GetAverageLevel(void);
struct BtlUnit *UnitGet(u32 owner);
const u8 *Owner_GetRecord(s32 selector);
void Runtime_CopyBytesDirectional(u8 *first, u8 *second, s32 count, s32 direction);
void UnitCalc(s32 owner);
void GameFlag_RefreshLureCap(void);
u32 Runtime_GetBuildStampTime(void);
void GameState_InitDefaults(void);
void Party_SetFlag32AndRefreshMembers(void);
void Party_ApplyStatePreset(void);
void Owner_RefreshActiveRatios(s32 arg0);
void Owner_RefreshRatiosOnFlag(void);
void Owner_RatioNoOp(void);
void UnitRatio(s32 owner_no);
void Owner_UpdateRatioPair(struct BtlUnit *state, s32 input);
void Owner_UpdateSecondInputAndRatios(struct BtlUnit *unit, s32 pp);
s32 Owner_AdjustFirstValue(s32 owner, s32 delta);
s16 UnitPpAdd(s32 owner, s32 delta);
s32 Party_AdjustSixDigitCounterB(s32 amount);
s32 Party_AdjustCounterCappedAt28(s32 amount);

struct ItemDefinition *Item_GetDirect(s32 item);
s32 Item_CanOwnerEquipDirect(s32 unit, s32 item);
s32 Item_IsCompatibleWithOwner(s32 unit, s32 item);
s32 Item_GetEquipmentGroup(s32 item_id);
s32 Inventory_GetQuantity(s32 owner, s32 slot);
s32 Inventory_Count(s32 owner);
s32 PartyInventory_HasSpace(void);
s32 PartyInventory_CountFreeSlots(void);
s32 Inventory_AddItem(s32 owner_id, s32 item_id);
s32 PartyInventory_Add(s32 item_id);
s32 Inventory_Find(s32 owner, s32 item_id);
s32 PartyInventory_FindOwner(s32 item_id);
s32 Inventory_Equip(s32 owner, s32 slot);
s32 Inventory_FindEquipped(s32 owner, s32 type);
struct ItemDefinition *Inventory_GetEquippedDefinition(struct BtlUnit *inv, s32 type);
s32 Inventory_GetEquippedItem(struct BtlUnit *inv, s32 type);
s32 Inventory_Remove(s32 owner, s32 slot);
s32 Inventory_Discard(s32 owner, s32 slot);
s32 Inventory_CheckDiscard(s32 owner, s32 slot);
s32 PartyInventory_Remove(s32 item_id);
s32 PartyInventory_Discard(s32 item_id);
s32 Inventory_Break(s32 owner, s32 slot);
s32 Inventory_Repair(s32 owner, s32 slot);
u8 Item_GetTargetMode(s32 item_id);
s32 ItemCounter_Adjust(s32 index, s32 delta);
s32 Item_AdjustCounter(s32 item_id, s32 delta);
s32 Inventory_CountItem(s32 owner, s32 item_id);
s32 PartyInventory_CountItem(s32 item_id);
struct BtlAct *BattleAction_GetDirect(s32 action_id);
s32 Equipment_HasValue(s32 owner, s32 value);
s32 Owner_RefreshClassActions(s32 owner);
s32 OwnerAction_Add(s32 state_index, s32 value);
struct LevelUpResult *OwnerAction_CheckLevelThreshold(s32 owner, struct LevelUpResult *value);
struct CharacterDefinition *Character_GetDefinition(s32 index);
void Owner_InitRecords(void);
void Owner_LevelNoOp(void);
u32 Owner_GetLevelThreshold(s32 owner, s32 level);
struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *res);
struct LevelUpResult *Owner_GetValueIfLevelThresholdReached(s32 owner_no, struct LevelUpResult *value);
void Party_AdvanceOwnerCountToTarget(s32 unit, s32 lv);

s32 GameFlag_Test(s32 flag);
s32 GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
u32 GameFlag_Toggle(s32 flag);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
u8 GameFlag_IncrementByte(s32 flag);
u8 GameFlag_DecrementByte(s32 flag);
s32 GameFlag_GetNibble(u32 flag);
void GameFlag_SetNibble(s32 flag, s32 value);

s32 BtlUnitSet(s32 unit_id, s32 enemy_id, s32 suffix);
s32 Party_CountActiveOwners(void);
s32 Party_AddActiveOwner(s32 value);
s32 Party_RemoveActiveOwner(s32 value);
s32 Party_ListActiveOwners(s16 *owners);
s32 Party_AdjustCoins(s32 amount);

s32 Trade_ListFlaggedEntries(u8 *output);
const struct SummonDefinition *SummonDefinition_GetNear(u32 summon_id);
s32 Owner_LookupFourColumnTable(s32 row, s32 column);
s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 output[4]);
s32 Owner_GetResistanceValue(s32 owner, s32 index);
s32 Owner_GetDefaultElement(struct BtlUnit *state);
u32 Owner_CalculateBaseElements(s32 owner, s16 destination[4][2]);
s32 Owner_DetermineClass(s32 character, const u8 *djinn);
struct ClassDefinition *Owner_GetRecordStride84(s32 arg0);
void Owner_RefreshDerivedData(s32 owner_no);

s32 Curve_LookupScaledValue(s32 input, s32 halve);
s32 BtlRnd(void);
u32 BattleRandomPercent(void);
s32 Owner_ScaleAdjustedAmount(s32 atk, s32 def, s32 add, s32 bonus);
s32 Owner_ScaleValueByCurve(s32 pow, s32 bonus, s32 rate);
s32 Owner_ScaleValueByOffsetCurve(s32 pow, s32 elm, s32 rate);
s32 Item_GetEquippedElement(s32 id);
s32 Equipment_GetUnleashRateBonus(struct BtlUnit *owner);
s32 RollWeaponUnleash(struct BtlUnit *owner);
s32 BattleFx_GetBaseSuccessRate(s32 effect_id);
s32 BattleTarget_IsWeakToEffect(const struct BtlUnit *target, s32 effect_id);
s32 BtlEfxRevChk(s32 effect_id);
s32 BattleFx_RollSuccess(s32 caster, s32 target, s32 resistance_category, s32 effect_id, s32 success_scale);

const u16 *Djinn_GetDefinition(u32 group, u32 index);
s32 Djinn_AddToLeastLoadedOwner(s32 index, u8 *state);
s32 Djinn_AddToOwner(s32 owner, s32 index, s32 bit);
s32 Trade_CanOfferDjinn(s32 owner, s32 index, s32 bit);
u32 Djinn_IsActive(s32 owner, s32 index, s32 bit);
s32 Djinn_Activate(s32 owner, s32 index, s32 bit);
u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit);
s32 Trade_RemoveOffer(s32 owner, s32 index, s32 bit);
u32 *Trade_AddOffer(u32 kind, u32 first, u32 second);
s32 Djinn_Transfer(s32 source, s32 index, s32 bit, s32 target);
s32 Trade_CountPendingOffers(u8 *counts);
s32 Djinn_GetDefinitionHeader(u32 group, u32 index);
s32 Party_SumDjinnCounts(s32 index);

void Inventory_AddAndEquip(s32 owner, s32 target);
void InventorySnapshot_Save(void);
void InventorySnapshot_Restore(void);

#endif
