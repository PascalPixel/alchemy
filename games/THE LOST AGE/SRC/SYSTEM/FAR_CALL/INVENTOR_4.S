@ Far-call stub table at 0x080ad248: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Djinn_AddToLeastLoadedOwnerFar
	.thumb_func
Djinn_AddToLeastLoadedOwnerFar:
	overlay_veneer Djinn_AddToLeastLoadedOwner
	.global Party_SumDjinnCountsFar
	.thumb_func
Party_SumDjinnCountsFar:
	overlay_veneer Party_SumDjinnCounts
	.global Party_AdjustSixDigitCounterBFar
	.thumb_func
Party_AdjustSixDigitCounterBFar:
	overlay_veneer Party_AdjustSixDigitCounterB
	.global Party_AdjustCounterCappedAt28Far
	.thumb_func
Party_AdjustCounterCappedAt28Far:
	overlay_veneer Party_AdjustCounterCappedAt28
	.global Func_080ad268
	.thumb_func
Func_080ad268:
	overlay_veneer Func_080af244
	.global Func_080ad270
	.thumb_func
Func_080ad270:
	overlay_veneer Func_080af1fc
	.global Inventory_DiscardFar
	.thumb_func
Inventory_DiscardFar:
	overlay_veneer Inventory_Discard
	.global BattleFx_IsReviveFar
	.thumb_func
BattleFx_IsReviveFar:
	overlay_veneer Func_080b0740
	.global Owner_RefreshClassActionsFar
	.thumb_func
Owner_RefreshClassActionsFar:
	overlay_veneer Owner_RefreshClassActions
	.global Func_080ad290
	.thumb_func
Func_080ad290:
	overlay_veneer Party_GetAverageLevel
	.global Owner_RefreshDerivedDataFar
	.thumb_func
Owner_RefreshDerivedDataFar:
	overlay_veneer Owner_RefreshDerivedData
	.global Inventory_CountItemFar
	.thumb_func
Inventory_CountItemFar:
	overlay_veneer Inventory_CountItem
	.global PartyInventory_CountItemFar
	.thumb_func
PartyInventory_CountItemFar:
	overlay_veneer PartyInventory_CountItem
	.global Func_080ad2b0
	.thumb_func
Func_080ad2b0:
	overlay_veneer PartyInventory_Discard
