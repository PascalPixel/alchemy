.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global GameFlag_RefreshLureCapFar
	.thumb_func
GameFlag_RefreshLureCapFar:
	overlay_veneer GameFlag_RefreshLureCap
	.global Equipment_GetUnleashRateBonusFar
	.thumb_func
Equipment_GetUnleashRateBonusFar:
	overlay_veneer Equipment_GetUnleashRateBonus
	.global Runtime_GetBuildStampTimeFar
	.thumb_func
Runtime_GetBuildStampTimeFar:
	overlay_veneer Runtime_GetBuildStampTime
	.global PartyInventory_CountFreeSlotsFar
	.thumb_func
PartyInventory_CountFreeSlotsFar:
	overlay_veneer PartyInventory_CountFreeSlots
	.global Func_080ad2d8
	.thumb_func
Func_080ad2d8:
	overlay_veneer Func_080b1088
	.global Func_080ad2e0
	.thumb_func
Func_080ad2e0:
	overlay_veneer Func_080b11f8
