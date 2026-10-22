#ifndef ALCHEMY_UNIT_GAME_GAME_H
#define ALCHEMY_UNIT_GAME_GAME_H

#include "types.h"

struct BattleAct;
struct BattleElementStats;
struct BattleUnit;
struct CharacterDefinition;
struct ClassDefinition;
struct DjinnDefinition;
struct ItemDefinition;
struct LevelUpResult;
struct SummonDefinition;

void Game_Restart(void);
void *Djinn_GetRecoveryTable(s32 enemySide);
u32 Party_GetAverageLevel(void);
struct BattleUnit *Unit_Get(u32 owner);
const u8 *Enemy_GetDefinition(s32 selector);
void Runtime_CopyBytesDirectional(u8 *first, u8 *second, s32 count,
                                  s32 direction);
void Unit_Calculate(s32 owner);
void GameFlag_RefreshLureCap(void);
u32 Runtime_GetBuildStampTime(void);
void GameState_InitializeDefaults(void);
void Party_StartChildhood(void);
void Party_EndChildhood(void);
void Party_RestoreHpPp(s32 cureConditions);
void Party_RestorePpOfLitElements(void);
void Unit_RatioNoOp(void);
void Unit_UpdateRatios(s32 owner);
void Unit_SetHp(struct BattleUnit *unit, s32 hp);
void Unit_SetPp(struct BattleUnit *unit, s32 pp);
s32 Unit_AddHp(s32 owner, s32 delta);
s16 Unit_AddPp(s32 owner, s32 delta);
s32 Party_AdjustShopCredit(s32 amount);
s32 Party_AdjustShopGifts(s32 amount);

struct ItemDefinition *Item_GetDirect(s32 item);
s32 Item_CanOwnerEquipDirect(s32 owner, s32 item);
s32 Item_IsCompatibleWithOwner(s32 unit, s32 item);
s32 Item_GetEquipmentGroup(s32 itemId);
s32 Inventory_GetQuantity(s32 owner, s32 slot);
s32 Inventory_Count(s32 owner);
s32 PartyInventory_HasSpace(void);
s32 PartyInventory_CountFreeSlots(void);
s32 Inventory_AddItem(s32 owner, s32 itemId);
s32 PartyInventory_Add(s32 itemId);
s32 Inventory_Find(s32 owner, s32 itemId);
s32 PartyInventory_FindOwner(s32 itemId);
s32 Inventory_Equip(s32 owner, s32 slot);
s32 Inventory_FindEquipped(s32 owner, s32 type);
struct ItemDefinition *Inventory_GetEquippedDefinition(struct BattleUnit *unit,
                                                       s32 type);
s32 Inventory_GetEquippedItem(struct BattleUnit *unit, s32 type);
s32 Inventory_Remove(s32 owner, s32 slot);
s32 Inventory_Discard(s32 owner, s32 slot);
s32 Inventory_CheckDiscard(s32 owner, s32 slot);
s32 PartyInventory_Remove(s32 itemId);
s32 PartyInventory_Discard(s32 itemId);
s32 Inventory_Break(s32 owner, s32 slot);
s32 Inventory_Repair(s32 owner, s32 slot);
u8 Item_GetTargetMode(s32 itemId);
s32 ItemCounter_Adjust(s32 index, s32 delta);
s32 Item_AdjustCounter(s32 itemId, s32 delta);
s32 Inventory_CountItem(s32 owner, s32 itemId);
s32 PartyInventory_CountItem(s32 itemId);
struct BattleAct *BattleAction_GetDirect(s32 actionId);
s32 Unit_KnowsAction(s32 owner, s32 actionId);
s32 Unit_RefreshPsynergy(s32 owner);
s32 Unit_LearnPsynergy(s32 stateIndex, s32 value);
struct LevelUpResult *Unit_CheckLevelUp(s32 owner, struct LevelUpResult *gains);
struct CharacterDefinition *Character_GetDefinition(s32 index);
void Party_InitializeCharacters(void);
void Unit_LevelNoOp(void);
u32 Unit_GetExperienceForLevel(s32 owner, s32 level);
struct LevelUpResult *Unit_LevelUp(s32 owner, struct LevelUpResult *gains);
struct LevelUpResult *Unit_TryLevelUp(s32 owner, struct LevelUpResult *gains);
void Unit_LevelUpTo(s32 owner, s32 targetLevel);

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

s32 BattleUnit_Set(s32 unitId, s32 enemyId, s32 suffix);
s32 Party_CountActiveOwners(void);
s32 Party_AddActiveOwner(s32 owner);
s32 Party_RemoveActiveOwner(s32 owner);
s32 Party_ListActiveOwners(s16 *owners);
s32 Party_AdjustCoins(s32 amount);

s32 Summon_ListLearned(u8 *output);
const struct SummonDefinition *SummonDefinition_GetNear(u32 summonId);
s32 Class_GetSeries(s32 primaryElement, s32 secondaryElement);
s32 Unit_GetElementLevels(s32 record, const u8 *source, s32 output[4]);
s32 Unit_GetElementLevel(s32 owner, s32 element);
s32 Enemy_GetAttackElement(struct BattleUnit *unit);
#ifdef UBFIX
void Unit_CalculateBaseElements(s32 owner,
                                struct BattleElementStats *destination);
#else
u32 Unit_CalculateBaseElements(s32 owner, s16 destination[4][2]);
#endif
s32 Class_Determine(s32 character, const u8 *djinn);
struct ClassDefinition *Class_GetDefinition(s32 classId);
void Unit_UpdateClass(s32 ownerId);

s32 Battle_GetElementMultiplier(s32 input, s32 halve);
s32 Battle_Random(void);
u32 Battle_RandomPercent(void);
s32 Battle_CalculateAttackDamage(s32 attack, s32 defense, s32 addedPower,
                                 s32 elementDifference);
s32 Battle_CalculatePsynergyPower(s32 power, s32 elementDifference,
                                  s32 powerMultiplier);
s32 Battle_CalculateRecovery(s32 power, s32 elementPower, s32 powerMultiplier);
s32 Item_GetEquippedElement(s32 owner);
s32 Equipment_GetUnleashRateBonus(struct BattleUnit *unit);
s32 RollWeaponUnleash(struct BattleUnit *owner);
s32 Battle_GetEffectBaseSuccessRate(s32 effectId);
s32 BattleTarget_IsWeakToEffect(const struct BattleUnit *target, s32 effectId);
s32 Battle_IsReviveEffect(s32 effectId);
s32 Battle_RollEffectSuccess(s32 caster, s32 target, s32 resistanceCategory,
                             s32 effectId, s32 successScale);

const struct DjinnDefinition *Djinn_GetDefinition(u32 group, u32 index);
s32 Djinn_AddToLeastLoadedOwner(s32 element, s32 djinn);
s32 Djinn_AddToOwner(s32 owner, s32 element, s32 djinnIndex);
s32 Djinn_IsReady(s32 owner, s32 element, s32 djinnIndex);
u32 Djinn_IsActive(s32 owner, s32 element, s32 djinnIndex);
s32 Djinn_Activate(s32 owner, s32 element, s32 djinnIndex);
u32 Djinn_Deactivate(s32 owner, s32 element, s32 djinnIndex);
s32 DjinnRecovery_Remove(s32 owner, s32 element, s32 djinnIndex);
u32 *DjinnRecovery_AddStandby(u32 owner, u32 element, u32 djinnIndex);
s32 Djinn_Transfer(s32 source, s32 element, s32 djinnIndex, s32 target);
s32 DjinnRecovery_CountStandby(u8 *counts);
s32 Djinn_GetAbilityId(u32 group, u32 index);
s32 Party_SumDjinnCounts(s32 element);

void Inventory_AddAndEquip(s32 owner, s32 itemId);
void InventorySnapshot_Save(void);
void InventorySnapshot_Restore(void);

#endif
