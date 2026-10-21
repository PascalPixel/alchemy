/// @file
/// Party members, items, Djinn and game-state operations.
/// Far-call interfaces; their ordered jump stubs live in
/// games/tbs/src/system/far_call/GAME.S.

void *Djinn_GetRecoveryTableFar(s32 arg0);
#ifndef OWN_UnitGetFar
struct BattleUnit *UnitGetFar(u32 owner);
#endif
void UnitCalcFar(s32 owner);
struct ItemDefinition *Item_GetDirectFar(s32 item);
/// Far call to Inventory_GetQuantity.
s32 Shop_GetSelectionState(s32 owner, s32 slot);
s32 Inventory_AddItemFar(s32 owner_id, s32 item_id);
s32 PartyInventory_AddFar(s32 item_id);
s32 Inventory_FindFar(s32 owner, s32 item_id);
s32 PartyInventory_FindOwnerFar(s32 item_id);
s32 PartyInventory_RemoveFar(s32 item_id);
s32 Inventory_EquipFar(s32 owner, s32 slot);
s32 Inventory_RemoveFar(s32 owner, s32 slot);
s32 Inventory_BreakFar(s32 owner, s32 slot);
s32 Inventory_RepairFar(s32 owner, s32 slot);
s32 Inventory_GetEquippedItemFar(struct BattleUnit *inv, s32 type);
/// Far call to BattleAction_GetDirect.
struct BattleAct *BattleActGet(s32 actionId);
s32 Unit_LearnPsynergyFar();
s32 Unit_KnowsActionFar(s32 owner, s32 value);
void GameState_InitDefaultsFar(void);
struct LevelUpResult *Unit_TryLevelUpFar(s32 owner_id,
                                         struct LevelUpResult *value);
s32 GameFlag_TestFar(s32 flag);
s32 GameFlag_SetBitFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
s32 GameFlag_GetByteFar(s32 flag);
void GameFlag_SetByteFar(s32 flag, s32 value);
s32 Unit_AddHpFar(s32 owner, s32 delta);
s16 Unit_AddPpFar(s32 owner, s32 delta);
void UnitRatioFar(s32 owner_id);
void Unit_SetHpFar(struct BattleUnit *unit, s32 hp);
s32 BattleUnitSetFar(s32 unitId, s32 enemyId, s32 suffix);
s32 Party_CountActiveOwnersFar(void);
s32 Party_AddActiveOwnerFar(s32 value);
s32 Party_ListActiveOwnersFar(s16 *owners);
s32 RollWeaponUnleashFar(struct BattleUnit *owner);
s32 Party_RemoveActiveOwnerFar(s32 value);
s32 Item_GetEquippedElementFar(s32 id);
/// Far call to Battle_RollEffectSuccess.
s32 BattleHitCheck(s32 caster, s32 target, s32 resistance_category,
                   s32 effect_id, s32 success_scale);
/// Far call to Battle_CalcAttackDamage.
s32 BattleAtkCalc(s32 atk, s32 def, s32 add, s32 bonus);
/// Far call to Battle_CalcPsynergyPower.
s32 BattlePowerCalc(s32 power, s32 bonus, s32 rate);
/// Far call to Battle_CalcRecovery.
s32 BattleRecoveryCalc(s32 power, s32 element, s32 rate);
const u8 *Enemy_GetDefinitionFar(s32 selector);
s32 BattleRandomFar(void);
s32 Djinn_AddToOwnerFar(s32 owner, s32 index, s32 bit);
s32 Djinn_ActivateFar(s32 owner, s32 index, s32 bit);
u32 Djinn_DeactivateFar(s32 owner, s32 index, s32 bit);
s32 DjinnRecovery_RemoveFar(s32 owner, s32 index, s32 bit);
u32 *DjinnRecovery_AddStandbyFar(u32 kind, u32 first, u32 second);
s32 Djinn_TransferFar(s32 source, s32 index, s32 bit, s32 target);
s32 Summon_ListLearnedFar(u8 *output);
/// Far call to SummonDefinition_GetNear.
const struct SummonDefinition *SummonDefinition_Get(u32 summon_id);
s32 Djinn_GetAbilityIdFar(u32 group, u32 index);
void Unit_LevelUpToFar(s32 unit, s32 lv);
s32 Unit_GetElementLevelFar(s32 owner, s32 index);
u32 Djinn_IsActiveFar(s32 owner, s32 index, s32 bit);
s32 Djinn_IsReadyFar(s32 owner, s32 index, s32 bit);
/// Far call to Item_CanOwnerEquipDirect.
s32 Item_CanOwnerEquip(s32 unit, s32 item);
s32 Item_IsCompatibleWithOwnerFar(s32 unit, s32 item);
s32 Inventory_FindEquippedFar(s32 owner, s32 type);
s32 Party_AdjustCoinsFar(s32 amount);
s32 Item_GetEquipmentGroupFar(s32 item_id);
s32 Item_AdjustCounterFar(s32 item_id, s32 delta);
s32 Inventory_CountFar(s32 owner);
s32 PartyInventory_HasSpaceFar(void);
u32 Unit_GetExperienceForLevelFar(s32 owner, s32 level);
void Party_RestoreHpPpFar(s32 arg0);
void Party_RestorePpOfLitElementsFar(void);
void Party_BeginChildhoodFar(void);
void Party_EndChildhoodFar(void);
s32 Djinn_AddToLeastLoadedOwnerFar(s32 element, s32 djinn);
s32 Party_SumDjinnCountsFar(s32 index);
s32 Party_AdjustShopCreditFar(s32 amount);
s32 Party_AdjustShopGiftsFar(s32 amount);
s32 Inventory_CheckDiscardFar(s32 owner, s32 slot);
s32 Inventory_DiscardFar(s32 owner, s32 slot);
s32 Battle_IsReviveEffectFar(s32 effect_id);
s32 Unit_RefreshPsynergyFar(s32 owner);
u32 Party_GetAverageLevelFar(void);
s32 Inventory_CountItemFar(s32 owner, s32 item_id);
s32 PartyInventory_CountItemFar(s32 item_id);
s32 PartyInventory_DiscardFar(s32 item_id);
void GameFlag_RefreshLureCapFar(void);
s32 Equipment_GetUnleashRateBonusFar(struct BattleUnit *owner);
u32 Runtime_GetBuildStampTimeFar();
s32 PartyInventory_CountFreeSlotsFar(void);
void InventorySnapshot_SaveFar(void);
void InventorySnapshot_RestoreFar(void);
