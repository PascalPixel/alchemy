.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Unit_AddHpFar
	.thumb_func
Unit_AddHpFar:
	overlay_veneer Unit_AddHp
	.global Unit_AddPpFar
	.thumb_func
Unit_AddPpFar:
	overlay_veneer Unit_AddPp
	.global Unit_UpdateRatiosFar
	.thumb_func
Unit_UpdateRatiosFar:
	overlay_veneer Unit_UpdateRatios
	.global Unit_SetHpFar
	.thumb_func
Unit_SetHpFar:
	overlay_veneer Unit_SetHp
	.global Unit_SetPpFar
	.thumb_func
Unit_SetPpFar:
	overlay_veneer Unit_SetPp
	.global BattleUnit_SetFar
	.thumb_func
BattleUnit_SetFar:
	overlay_veneer Func_080afbec
	.global Party_CountActiveOwnersFar
	.thumb_func
Party_CountActiveOwnersFar:
	overlay_veneer Party_CountActiveOwners
	.global Party_AddActiveOwnerFar
	.thumb_func
Party_AddActiveOwnerFar:
	overlay_veneer Party_AddActiveOwner
	.global Party_ListActiveOwnersFar
	.thumb_func
Party_ListActiveOwnersFar:
	overlay_veneer Party_ListActiveOwners
	.global RollWeaponUnleashFar
	.thumb_func
RollWeaponUnleashFar:
	overlay_veneer RollWeaponUnleash
	.global Party_RemoveActiveOwnerFar
	.thumb_func
Party_RemoveActiveOwnerFar:
	overlay_veneer Party_RemoveActiveOwner
	.global Item_GetEquippedElementFar
	.thumb_func
Item_GetEquippedElementFar:
	overlay_veneer Item_GetEquippedElement
	.global BattleEffect_RollSuccessFar
	.thumb_func
BattleEffect_RollSuccessFar:
	overlay_veneer BattleEffect_RollSuccess
	.global Battle_CalculateAttackDamageFar
	.thumb_func
Battle_CalculateAttackDamageFar:
	overlay_veneer Battle_CalculateAttackDamage
	.global Battle_CalculatePsynergyPowerFar
	.thumb_func
Battle_CalculatePsynergyPowerFar:
	overlay_veneer Battle_CalculatePsynergyPower
	.global Battle_CalculateRecoveryFar
	.thumb_func
Battle_CalculateRecoveryFar:
	overlay_veneer Battle_CalculateRecovery
	.global Enemy_GetDefinitionFar
	.thumb_func
Enemy_GetDefinitionFar:
	overlay_veneer Enemy_GetDefinition
	.global Battle_RandomFar
	.thumb_func
Battle_RandomFar:
	overlay_veneer Battle_Random
	.global Djinn_AddToOwnerFar
	.thumb_func
Djinn_AddToOwnerFar:
	overlay_veneer Djinn_AddToOwner
	.global Djinn_ActivateFar
	.thumb_func
Djinn_ActivateFar:
	overlay_veneer Djinn_Activate
	.global Djinn_DeactivateFar
	.thumb_func
Djinn_DeactivateFar:
	overlay_veneer Djinn_Deactivate
	.global DjinnRecovery_RemoveFar
	.thumb_func
DjinnRecovery_RemoveFar:
	overlay_veneer DjinnRecovery_Remove
	.global DjinnRecovery_AddStandbyFar
	.thumb_func
DjinnRecovery_AddStandbyFar:
	overlay_veneer DjinnRecovery_AddStandby
	.global Djinn_TransferFar
	.thumb_func
Djinn_TransferFar:
	overlay_veneer Djinn_Transfer
	.global Summon_ListLearnedFar
	.thumb_func
Summon_ListLearnedFar:
	overlay_veneer Summon_ListLearned
	.global SummonDefinition_Get
	.thumb_func
SummonDefinition_Get:
	overlay_veneer SummonDefinition_GetNear
	.global Djinn_GetDefinitionHeaderFar
	.thumb_func
Djinn_GetDefinitionHeaderFar:
	overlay_veneer Djinn_GetDefinitionHeader
	.global Unit_LevelUpToFar
	.thumb_func
Unit_LevelUpToFar:
	overlay_veneer Unit_LevelUpTo
	.global Unit_GetElementLevelFar
	.thumb_func
Unit_GetElementLevelFar:
	overlay_veneer Unit_GetElementLevel
	.global DjinnRecovery_CountStandbyFar
	.thumb_func
DjinnRecovery_CountStandbyFar:
	overlay_veneer DjinnRecovery_CountStandby
