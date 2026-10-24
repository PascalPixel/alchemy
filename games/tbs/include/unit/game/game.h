#ifndef ALCHEMY_UNIT_GAME_GAME_H
#define ALCHEMY_UNIT_GAME_GAME_H

#include "types.h"

struct AbilityDefinition;
struct BattleElementStats;
struct Unit;
struct CharacterDefinition;
struct ClassDefinition;
struct DjinnDefinition;
struct ItemDefinition;
struct LevelUpResult;
struct SummonDefinition;

void Game_Restart(void);
void *Djinn_GetRecoveryTable(s32 enemySide);
u32 Party_GetAverageLevel(void);
struct Unit *Unit_Get(u32 unitId);
const u8 *Enemy_GetDefinition(s32 selector);
void Memory_CopyBytesDirectional(u8 *first, u8 *second, s32 count,
                                 s32 direction);
void Unit_Calculate(s32 unitId);
void GameFlag_RefreshLureCap(void);
u32 BuildStamp_GetTime(void);
void SaveBlock_InitializeDefaults(void);
void Party_StartChildhood(void);
void Party_EndChildhood(void);
void Party_RestoreHpPp(s32 cureConditions);
void Party_RestorePpOfLitElements(void);
void Unit_GaugeNoOp(void);
void Unit_RecalculateGauges(s32 unitId);
void Unit_SetHp(struct Unit *unit, s32 hp);
void Unit_SetPp(struct Unit *unit, s32 pp);
s32 Unit_AddHp(s32 unitId, s32 delta);
s16 Unit_AddPp(s32 unitId, s32 delta);
s32 Party_AdjustShopCredit(s32 amount);
s32 Party_AdjustShopGifts(s32 amount);

struct ItemDefinition *Item_GetDirect(s32 item);
s32 Item_CanUnitEquipDirect(s32 unitId, s32 itemId);
s32 Item_IsCompatibleWithUnit(s32 unit, s32 item);
s32 Item_GetEquipmentGroup(s32 itemId);
s32 Inventory_GetQuantity(s32 unitId, s32 slot);
s32 Inventory_Count(s32 unitId);
s32 PartyInventory_HasSpace(void);
s32 PartyInventory_CountFreeSlots(void);
s32 Inventory_AddItem(s32 unitId, s32 itemId);
s32 PartyInventory_Add(s32 itemId);
s32 Inventory_Find(s32 unitId, s32 itemId);
s32 PartyInventory_FindMember(s32 itemId);
s32 Inventory_Equip(s32 unitId, s32 slot);
s32 Inventory_FindEquipped(s32 unitId, s32 type);
struct ItemDefinition *Inventory_GetEquippedDefinition(struct Unit *unit,
                                                       s32 type);
s32 Inventory_GetEquippedItem(struct Unit *unit, s32 type);
s32 Inventory_Remove(s32 unitId, s32 slot);
s32 Inventory_Discard(s32 unitId, s32 slot);
s32 Inventory_CheckDiscard(s32 unitId, s32 slot);
s32 PartyInventory_Remove(s32 itemId);
s32 PartyInventory_Discard(s32 itemId);
s32 Inventory_Break(s32 unitId, s32 slot);
s32 Inventory_Repair(s32 unitId, s32 slot);
u8 Item_GetTargetMode(s32 itemId);
s32 RareItemStock_Adjust(s32 index, s32 delta);
s32 Item_AdjustCounter(s32 itemId, s32 delta);
s32 Inventory_CountItem(s32 unitId, s32 itemId);
s32 PartyInventory_CountItem(s32 itemId);
struct AbilityDefinition *Ability_GetDirect(s32 actionId);
s32 Unit_KnowsPsynergy(s32 unitId, s32 actionId);
s32 Unit_RefreshPsynergy(s32 unitId);
s32 Unit_LearnPsynergy(s32 stateIndex, s32 value);
struct LevelUpResult *Unit_CheckLevelUp(s32 unitId,
                                        struct LevelUpResult *gains);
struct CharacterDefinition *Character_GetDefinition(s32 index);
void Party_InitializeCharacters(void);
void Unit_LevelNoOp(void);
u32 Unit_GetExperienceForLevel(s32 unitId, s32 level);
struct LevelUpResult *Unit_LevelUp(s32 unitId, struct LevelUpResult *gains);
struct LevelUpResult *Unit_TryLevelUp(s32 unitId, struct LevelUpResult *gains);
void Unit_LevelUpTo(s32 unitId, s32 targetLevel);

s32 GameFlag_Get(s32 flag);
s32 GameFlag_Set(s32 flag);
void GameFlag_Clear(s32 flag);
u32 GameFlag_Toggle(s32 flag);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
u8 GameFlag_IncrementByte(s32 flag);
u8 GameFlag_DecrementByte(s32 flag);
s32 GameFlag_GetNibble(u32 flag);
void GameFlag_SetNibble(s32 flag, s32 value);

s32 BattleUnit_Set(s32 unitId, s32 enemyId, s32 suffix);
s32 Party_CountMembers(void);
s32 Party_AddMember(s32 unitId);
s32 Party_RemoveMember(s32 unitId);
s32 Party_ListMembers(s16 *unitIds);
s32 Party_AdjustCoins(s32 amount);

s32 Summon_ListLearned(u8 *output);
const struct SummonDefinition *SummonDefinition_GetNear(u32 summonId);
s32 Class_GetSeries(s32 primaryElement, s32 secondaryElement);
s32 Unit_GetElementLevels(s32 record, const u8 *source, s32 output[4]);
s32 Unit_GetElementLevel(s32 unitId, s32 element);
s32 Enemy_GetAttackElement(struct Unit *unit);
#ifdef UBFIX
void Unit_CalculateBaseElements(s32 unitId,
                                struct BattleElementStats *destination);
#else
u32 Unit_CalculateBaseElements(s32 unitId, s16 destination[4][2]);
#endif
s32 Class_Determine(s32 character, const u8 *djinn);
struct ClassDefinition *Class_GetDefinition(s32 classId);
void Unit_UpdateClass(s32 unitId);

s32 Battle_GetElementMultiplier(s32 input, s32 halve);
s32 Battle_Random(void);
u32 Battle_RandomPercent(void);
s32 Battle_CalculateAttackDamage(s32 attack, s32 defense, s32 addedPower,
                                 s32 elementDifference);
s32 Battle_CalculatePsynergyPower(s32 power, s32 elementDifference,
                                  s32 powerMultiplier);
s32 Battle_CalculateRecovery(s32 power, s32 elementPower, s32 powerMultiplier);
s32 Item_GetEquippedElement(s32 unitId);
s32 Equipment_GetUnleashRateBonus(struct Unit *unit);
s32 RollWeaponUnleash(struct Unit *unit);
s32 Battle_GetEffectBaseSuccessRate(s32 effectId);
s32 BattleAbility_IsWeakToEffect(const struct Unit *target, s32 effectId);
s32 Battle_IsReviveEffect(s32 effectId);
s32 Battle_RollEffectSuccess(s32 caster, s32 target, s32 resistanceCategory,
                             s32 effectId, s32 successScale);

const struct DjinnDefinition *Djinn_GetDefinition(u32 group, u32 index);
s32 Djinn_GiveToParty(s32 element, s32 djinn);
s32 Djinn_Give(s32 unitId, s32 element, s32 djinnIndex);
s32 Djinn_CanSet(s32 unitId, s32 element, s32 djinnIndex);
u32 Djinn_IsSet(s32 unitId, s32 element, s32 djinnIndex);
s32 Djinn_Set(s32 unitId, s32 element, s32 djinnIndex);
u32 Djinn_PutOnStandby(s32 unitId, s32 element, s32 djinnIndex);
s32 DjinnRecovery_Remove(s32 unitId, s32 element, s32 djinnIndex);
u32 *DjinnRecovery_AddStandby(u32 unitId, u32 element, u32 djinnIndex);
s32 Djinn_Transfer(s32 source, s32 element, s32 djinnIndex, s32 target);
s32 DjinnRecovery_CountStandby(u8 *counts);
s32 Djinn_GetAbilityId(u32 group, u32 index);
s32 Party_SumDjinnCounts(s32 element);

void Inventory_AddAndEquip(s32 unitId, s32 itemId);
void InventorySnapshot_Save(void);
void InventorySnapshot_Restore(void);

#endif
