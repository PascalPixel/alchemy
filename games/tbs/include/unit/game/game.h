#ifndef ALCHEMY_UNIT_GAME_GAME_H
#define ALCHEMY_UNIT_GAME_GAME_H

#include "types.h"

struct BattleAct;
struct BattleUnit;
struct CharacterDefinition;
struct ClassDefinition;
struct ItemDefinition;
struct LevelUpResult;
struct SummonDefinition;

void Game_Restart(void);
void *Djinn_GetRecoveryTable(s32 arg0);
u32 Party_GetAverageLevel(void);
struct BattleUnit *UnitGet(u32 owner);
const u8 *Enemy_GetDefinition(s32 selector);
void Runtime_CopyBytesDirectional(u8 *first, u8 *second, s32 count,
                                  s32 direction);
void UnitCalc(s32 owner);
void GameFlag_RefreshLureCap(void);
u32 Runtime_GetBuildStampTime(void);
void GameState_InitDefaults(void);
void Party_BeginChildhood(void);
void Party_EndChildhood(void);
void Party_RestoreHpPp(s32 arg0);
void Party_RestorePpOfLitElements(void);
void Owner_RatioNoOp(void);
void UnitRatio(s32 owner_id);
void Unit_SetHp(struct BattleUnit *unit, s32 hp);
void Unit_SetPp(struct BattleUnit *unit, s32 pp);
s32 Unit_AddHp(s32 owner, s32 delta);
s16 Unit_AddPp(s32 owner, s32 delta);
s32 Party_AdjustShopCredit(s32 amount);
s32 Party_AdjustShopGifts(s32 amount);

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
struct ItemDefinition *Inventory_GetEquippedDefinition(struct BattleUnit *inv,
                                                       s32 type);
s32 Inventory_GetEquippedItem(struct BattleUnit *inv, s32 type);
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
struct BattleAct *BattleAction_GetDirect(s32 actionId);
s32 Unit_KnowsAction(s32 owner, s32 value);
s32 Unit_RefreshPsynergy(s32 owner);
s32 Unit_LearnPsynergy(s32 state_index, s32 value);
struct LevelUpResult *Unit_CheckLevelUp(s32 owner, struct LevelUpResult *value);
struct CharacterDefinition *Character_GetDefinition(s32 index);
void Party_InitCharacters(void);
void Owner_LevelNoOp(void);
u32 Unit_GetExperienceForLevel(s32 owner, s32 level);
struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *res);
struct LevelUpResult *Unit_TryLevelUp(s32 owner_id,
                                      struct LevelUpResult *value);
void Unit_LevelUpTo(s32 unit, s32 lv);

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

s32 BattleUnitSet(s32 unitId, s32 enemyId, s32 suffix);
s32 Party_CountActiveOwners(void);
s32 Party_AddActiveOwner(s32 value);
s32 Party_RemoveActiveOwner(s32 value);
s32 Party_ListActiveOwners(s16 *owners);
s32 Party_AdjustCoins(s32 amount);

s32 Summon_ListLearned(u8 *output);
const struct SummonDefinition *SummonDefinition_GetNear(u32 summon_id);
s32 Class_GetSeries(s32 row, s32 column);
s32 Unit_GetElementLevels(s32 record, const u8 *source, s32 output[4]);
s32 Unit_GetElementLevel(s32 owner, s32 index);
s32 Enemy_GetAttackElement(struct BattleUnit *state);
u32 Unit_CalcBaseElements(s32 owner, s16 destination[4][2]);
s32 Class_Determine(s32 character, const u8 *djinn);
struct ClassDefinition *Class_GetDefinition(s32 arg0);
void Unit_UpdateClass(s32 owner_id);

s32 Battle_GetElementMultiplier(s32 input, s32 halve);
s32 BattleRandom(void);
u32 BattleRandomPercent(void);
s32 Battle_CalcAttackDamage(s32 atk, s32 def, s32 add, s32 bonus);
s32 Battle_CalcPsynergyPower(s32 power, s32 bonus, s32 rate);
s32 Battle_CalcRecovery(s32 power, s32 element, s32 rate);
s32 Item_GetEquippedElement(s32 id);
s32 Equipment_GetUnleashRateBonus(struct BattleUnit *owner);
s32 RollWeaponUnleash(struct BattleUnit *owner);
s32 BattleFx_GetBaseSuccessRate(s32 effect_id);
s32 BattleTarget_IsWeakToEffect(const struct BattleUnit *target, s32 effect_id);
s32 Battle_IsReviveEffect(s32 effect_id);
s32 BattleFx_RollSuccess(s32 caster, s32 target, s32 resistance_category,
                         s32 effect_id, s32 success_scale);

const u16 *Djinn_GetDefinition(u32 group, u32 index);
s32 Djinn_AddToLeastLoadedOwner(s32 index, u8 *state);
s32 Djinn_AddToOwner(s32 owner, s32 index, s32 bit);
s32 Djinn_IsReady(s32 owner, s32 index, s32 bit);
u32 Djinn_IsActive(s32 owner, s32 index, s32 bit);
s32 Djinn_Activate(s32 owner, s32 index, s32 bit);
u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit);
s32 DjinnRecovery_Remove(s32 owner, s32 index, s32 bit);
u32 *DjinnRecovery_AddStandby(u32 kind, u32 first, u32 second);
s32 Djinn_Transfer(s32 source, s32 index, s32 bit, s32 target);
s32 DjinnRecovery_CountStandby(u8 *counts);
s32 Djinn_GetDefinitionHeader(u32 group, u32 index);
s32 Party_SumDjinnCounts(s32 index);

void Inventory_AddAndEquip(s32 owner, s32 target);
void InventorySnapshot_Save(void);
void InventorySnapshot_Restore(void);

#endif
