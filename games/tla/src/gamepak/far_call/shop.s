	.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Resource_FarCall008
Resource_FarCall008:
	export Shop_OpenDebugShopFar, Shop_OpenDebugShop
	export Shop_RunFar, Shop_Run
	export Sanctum_RunFar, Sanctum_Run
	export Inn_CheckInFar, Inn_CheckIn
	export Forge_RunFar, Forge_Run
	export Inn_QuoteRoomPriceFar, Inn_QuoteRoomPrice
	export MenuCursor_AdvanceFar, MenuCursor_Advance
	export MenuCursor_MoveTowardTargetFar, MenuCursor_MoveTowardTarget
	export MenuCursor_SetFar, MenuCursor_Set
	export MenuCursor_SetPositionImmediateFar, MenuCursor_SetPositionImmediate
	export QuantityBar_FillFar, QuantityBar_Fill
	export Shop_SaveAndTintFieldPaletteFar, Shop_SaveAndTintFieldPalette
	export Shop_RestoreFieldPaletteFar, Shop_RestoreFieldPalette
	export ItemPicker_RunToDropFar, ItemPicker_RunToDrop
	export ItemPicker_RunMode1Far, ItemPicker_RunMode1
	export Sound_WaitForJingleFar, Sound_WaitForJingle
	export Inn_PlaySleepFar, Inn_PlaySleep
	export VramBlock_LoadResourceFar, VramBlock_LoadResource
	export Forge_PartyHasMaterialFar, Forge_PartyHasMaterial
	export Forge_SetItemFar, Forge_SetItem
	export Forge_GetItemFar, Forge_GetItem
