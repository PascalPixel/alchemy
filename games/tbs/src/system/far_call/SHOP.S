@ Shops, inns and the cursors used by their screens.
@ Ordered 8-byte jumps into this bank. Named exports are callable symbols;
@ bare veneers preserve entries that have no exported name.
#include "stub.inc"
	.global FarCall_ShopTable
FarCall_ShopTable:
	export Shop_OpenDebugShopFar, Shop_OpenDebugShop
	export Shop_RunFar, Shop_Run
	export Shop_ConfirmActFar, Shop_ConfirmAct
	export Inn_CheckInFar, Inn_CheckIn
	export ShopCursor_AdvanceFar, ShopCursor_Advance
	export ShopCursor_MoveTowardTargetFar, ShopCursor_MoveTowardTarget
	export Shop_SetCursorFar, Shop_SetCursor
	export ShopCursor_SetPositionImmediateFar, ShopCursor_SetPositionImmediate
	export Shop_FillSelectorFar, Shop_FillSelector
	export Shop_SaveAndTintFieldPaletteFar, Shop_SaveAndTintFieldPalette
	export Shop_RestoreFieldPaletteFar, Shop_RestoreFieldPalette
	export Shop_PickUnitItemFar, Shop_PickUnitItem
	export AudioCommand_WaitForStateByteClearFar, AudioCommand_WaitForStateByteClear
	export Inn_PlaySleepFar, Inn_PlaySleep
