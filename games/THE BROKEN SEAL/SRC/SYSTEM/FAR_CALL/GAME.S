@ Far-call stub table heading the party, item and flag code at 0x08077000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global Trade_GetOfferStateFar
	.global Func_08077000
	.thumb_func
Trade_GetOfferStateFar:
Func_08077000:
	overlay_veneer 0x08077331
	.global Owner_GetStateFar
	.thumb_func
Owner_GetStateFar:
	.global Runtime_GetObject
	.thumb_func
Runtime_GetObject:
	overlay_veneer 0x08077395
	.global BattleUnit_Recalculate
	.thumb_func
BattleUnit_Recalculate:
	.global Owner_RecalculateStatsFar
	.thumb_func
Owner_RecalculateStatsFar:
	overlay_veneer 0x08077429
	.global Item_Get
	.thumb_func
Item_Get:
	overlay_veneer 0x08078415
	.global Shop_GetSelectionState
	.thumb_func
Shop_GetSelectionState:
	overlay_veneer 0x080784b1
	.global Inventory_AddItemFar
	.thumb_func
Inventory_AddItemFar:
	overlay_veneer 0x08078589
	.global Func_08077030
	.thumb_func
Func_08077030:
	overlay_veneer 0x08078619
	.global Item_FindSlot
	.thumb_func
Item_FindSlot:
	overlay_veneer 0x08078665
	overlay_veneer 0x08078699
	.global PartyInventory_RemoveFar
	.global Func_08077048
	.thumb_func
PartyInventory_RemoveFar:
Func_08077048:
	overlay_veneer 0x080789dd
	.global Inventory_EquipFar
	.thumb_func
Inventory_EquipFar:
	overlay_veneer 0x08078709
	.global Inventory_RemoveFar
	.thumb_func
Inventory_RemoveFar:
	overlay_veneer 0x080788c5
	.global Inventory_BreakFar
	.thumb_func
Inventory_BreakFar:
	overlay_veneer 0x08078a35
	.global Func_08077068
	.thumb_func
Func_08077068:
	overlay_veneer 0x08078a61
	overlay_veneer 0x08078a8d
	.global Inventory_GetEquippedItemFar
	.global Func_08077078
	.thumb_func
Inventory_GetEquippedItemFar:
Func_08077078:
	overlay_veneer 0x08078871
	.global Ability_GetData
	.thumb_func
Ability_GetData:
	.global BattleAction_Get
	.thumb_func
BattleAction_Get:
	overlay_veneer 0x08078b9d
	.global OwnerAction_AddFar
	.thumb_func
OwnerAction_AddFar:
	overlay_veneer 0x08078e29
	.global Equipment_HasValueFar
	.thumb_func
Equipment_HasValueFar:
	overlay_veneer 0x08078bc1
	overlay_veneer 0x08077d39
	overlay_veneer 0x08078ecd
	overlay_veneer 0x08078ee9
	overlay_veneer 0x08079005
	.global Func_080770b8
	.thumb_func
Func_080770b8:
	overlay_veneer 0x080792c5
	.global GameFlag_IsSet
	.thumb_func
GameFlag_IsSet:
	.global GameFlag_TestFar
	.thumb_func
GameFlag_TestFar:
	overlay_veneer 0x08079339
	.global GameFlag_SetBitFar
	.thumb_func
GameFlag_SetBitFar:
	overlay_veneer 0x08079359
	.global GameFlag_ClearBitFar
	.thumb_func
GameFlag_ClearBitFar:
	overlay_veneer 0x08079375
	overlay_veneer 0x08079391
	.global GameFlag_GetByteFar
	.global Func_080770e0
	.thumb_func
GameFlag_GetByteFar:
Func_080770e0:
	overlay_veneer 0x080793b9
	overlay_veneer 0x080793c9
	overlay_veneer 0x080793d9
	overlay_veneer 0x080793f9
	overlay_veneer 0x08079419
	overlay_veneer 0x08079435
	overlay_veneer 0x08077321
	.global Owner_AdjustFirstValueFar
	.global Func_08077118
	.thumb_func
Owner_AdjustFirstValueFar:
Func_08077118:
	overlay_veneer 0x080783a5
	.global Owner_AdjustSecondValueFar
	.thumb_func
Owner_AdjustSecondValueFar:
	overlay_veneer 0x080783dd
	.global Owner_RecalculateRatiosFar
	.thumb_func
Owner_RecalculateRatiosFar:
	overlay_veneer 0x0807822d
	.global Owner_UpdateRatioPairFar
	.thumb_func
Owner_UpdateRatioPairFar:
	overlay_veneer 0x080782a1
	overlay_veneer 0x08078321
	.global BattleUnit_AssignFar
	.thumb_func
BattleUnit_AssignFar:
	overlay_veneer 0x08079461
	.global Party_CountActiveOwnersFar
	.thumb_func
Party_CountActiveOwnersFar:
	overlay_veneer 0x080795fd
	.global Party_AddActiveOwnerFar
	.thumb_func
Party_AddActiveOwnerFar:
	overlay_veneer 0x0807961d
	.global Party_ListActiveOwnersFar
	.thumb_func
Party_ListActiveOwnersFar:
	overlay_veneer 0x080796c5
	overlay_veneer 0x08079d1d
	.global Party_RemoveActiveOwnerFar
	.thumb_func
Party_RemoveActiveOwnerFar:
	overlay_veneer 0x08079665
	overlay_veneer 0x08079c8d
	.global Battle_HitCheck
	.thumb_func
Battle_HitCheck:
	overlay_veneer 0x08079f11
	.global Battle_CalcAttack
	.thumb_func
Battle_CalcAttack:
	overlay_veneer 0x08079bf9
	.global Battle_CalcPower
	.thumb_func
Battle_CalcPower:
	overlay_veneer 0x08079c31
	.global Battle_CalcRestore
	.thumb_func
Battle_CalcRestore:
	overlay_veneer 0x08079c5d
	.global Owner_GetRecordFar
	.thumb_func
Owner_GetRecordFar:
	overlay_veneer 0x080773d9
	.global BattleRandom16Far
	.thumb_func
BattleRandom16Far:
	overlay_veneer 0x08079bc5
	.global Djinn_AddToOwnerFar
	.thumb_func
Djinn_AddToOwnerFar:
	overlay_veneer 0x0807a1b5
	.global Djinn_ActivateFar
	.thumb_func
Djinn_ActivateFar:
	.global Func_080771b0
	.thumb_func
Func_080771b0:
	overlay_veneer 0x0807a2e5
	.global Func_080771b8
	.thumb_func
Func_080771b8:
	overlay_veneer 0x0807a351
	.global Trade_RemoveOfferFar
	.thumb_func
Trade_RemoveOfferFar:
	overlay_veneer 0x0807a3a9
	.global Trade_AddOfferFar
	.thumb_func
Trade_AddOfferFar:
	overlay_veneer 0x0807a459
	overlay_veneer 0x0807a499
	overlay_veneer 0x0807977d
	.global SummonDefinition_Get
	.thumb_func
SummonDefinition_Get:
	overlay_veneer 0x080797d5
	.global Func_080771e8
	.thumb_func
Func_080771e8:
	overlay_veneer 0x0807a5b1
	.global Func_080771f0
	.thumb_func
Func_080771f0:
	overlay_veneer 0x080792fd
	overlay_veneer 0x0807987d
	overlay_veneer 0x0807a551
	.global Func_08077208
	.thumb_func
Func_08077208:
	overlay_veneer 0x0807a2bd
	.global Trade_CanOfferDjinnFar
	.thumb_func
Trade_CanOfferDjinnFar:
	overlay_veneer 0x0807a1f9
	.global Item_CanOwnerEquip
	.thumb_func
Item_CanOwnerEquip:
	overlay_veneer 0x0807842d
	.global Item_IsCompatibleWithOwnerFar
	.thumb_func
Item_IsCompatibleWithOwnerFar:
	overlay_veneer 0x0807845d
	.global Inventory_FindEquippedFar
	.thumb_func
Inventory_FindEquippedFar:
	overlay_veneer 0x080787dd
	.global Party_AdjustSixDigitCounterAFar
	.thumb_func
Party_AdjustSixDigitCounterAFar:
	overlay_veneer 0x08079701
	.global Item_GetEquipmentGroupFar
	.thumb_func
Item_GetEquipmentGroupFar:
	overlay_veneer 0x08078481
	.global Ability_GetMaximum
	.thumb_func
Ability_GetMaximum:
	.global Func_08077240
	.thumb_func
Func_08077240:
	overlay_veneer 0x08078ad1
	.global Ability_GetAvailability
	.thumb_func
Ability_GetAvailability:
	.global Inventory_CountFar
	.thumb_func
Inventory_CountFar:
	overlay_veneer 0x080784d9
	overlay_veneer 0x08078501
	.global Owner_GetLevelThresholdFar
	.global Func_08077258
	.thumb_func
Owner_GetLevelThresholdFar:
Func_08077258:
	overlay_veneer 0x08079009
	overlay_veneer 0x0807808d
	overlay_veneer 0x08078145
	overlay_veneer 0x08077f41
	overlay_veneer 0x08077f71
	overlay_veneer 0x08078229
	.global Djinn_AddToLeastLoadedOwnerFar
	.global Func_08077288
	.thumb_func
Djinn_AddToLeastLoadedOwnerFar:
Func_08077288:
	overlay_veneer 0x0807a0f5
	.global Party_SumDjinnCountsFar
	.thumb_func
Party_SumDjinnCountsFar:
	overlay_veneer 0x0807a5bd
	.global Party_AdjustSixDigitCounterBFar
	.thumb_func
Party_AdjustSixDigitCounterBFar:
	overlay_veneer 0x08079729
	.global Func_080772a0
	.thumb_func
Func_080772a0:
	overlay_veneer 0x08079755
	.global Inventory_CheckDiscardFar
	.thumb_func
Inventory_CheckDiscardFar:
	overlay_veneer 0x08078981
	.global Func_080772b0
	.thumb_func
Func_080772b0:
	overlay_veneer 0x08078949
	.global BattleFx_IsReviveFar
	.thumb_func
BattleFx_IsReviveFar:
	overlay_veneer 0x08079ef9
	.global Func_080772c0
	.thumb_func
Func_080772c0:
	overlay_veneer 0x08078bf1
	overlay_veneer 0x08077349
	overlay_veneer 0x08079ae9
	.global Inventory_CountItemFar
	.global Func_080772d8
	.thumb_func
Inventory_CountItemFar:
Func_080772d8:
	overlay_veneer 0x08078af9
	overlay_veneer 0x08078b61
	overlay_veneer 0x08078a09
	.global GameFlag_RefreshLureCapFar
	.global Func_080772f0
	.thumb_func
GameFlag_RefreshLureCapFar:
Func_080772f0:
	overlay_veneer 0x08077c11
	overlay_veneer 0x08079cbd
	overlay_veneer 0x08077cb9
	overlay_veneer 0x08078551
	overlay_veneer 0x0807a665
	overlay_veneer 0x0807a7a1
