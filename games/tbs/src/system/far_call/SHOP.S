@ Shops, inns and the cursors used by their screens.
@ Ordered 8-byte jumps into this bank. Named exports are callable symbols;
@ bare veneers preserve entries that have no exported name.
#include "stub.inc"
	.global FarCall_ShopTable
FarCall_ShopTable:
	export Shop_OpenDebugShopFar, Shop_OpenDebugShop
	export Shop_RunFar, Shop_Run
	export Sanctum_RunFar, Sanctum_Run
	export Inn_CheckInFar, Inn_CheckIn
	export MenuCursor_AdvanceFar, MenuCursor_Advance
	export MenuCursor_MoveTowardTargetFar, MenuCursor_MoveTowardTarget
	export MenuCursor_SetFar, MenuCursor_Set
	export MenuCursor_SetPositionImmediateFar, MenuCursor_SetPositionImmediate
	export QuantityBar_FillFar, QuantityBar_Fill
	export Shop_SaveAndTintFieldPaletteFar, Shop_SaveAndTintFieldPalette
	export Shop_RestoreFieldPaletteFar, Shop_RestoreFieldPalette
	export ItemPicker_RunFar, ItemPicker_Run
	export AudioCommand_WaitForStateByteClearFar, AudioCommand_WaitForStateByteClear
	export Inn_PlaySleepFar, Inn_PlaySleep
