@ Shops, inns and the cursors used by their screens.
@ C2: aligned 8-byte far-call stubs: export and overlay_veneer emit ldr r4, bx r4 and a target word.
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
	export Sound_WaitForFanfareFar, Sound_WaitForFanfare
	export Inn_PlaySleepFar, Inn_PlaySleep
