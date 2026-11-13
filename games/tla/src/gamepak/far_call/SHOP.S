	.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Resource_FarCall008
Resource_FarCall008:
	export Func_08108000, Shop_OpenDebugShop
	export Func_08108008, Shop_Run
	export Func_08108010, Sanctum_Run
	export Func_08108018, Inn_CheckIn
	export Func_08108020, Forge_Run
	export Func_08108028, Inn_QuoteRoomPrice
	export Func_08108030, MenuCursor_Advance
	export Func_08108038, MenuCursor_MoveTowardTarget
	export Func_08108040, MenuCursor_Set
	export Func_08108048, MenuCursor_SetPositionImmediate
	export QuantityBar_FillFar, QuantityBar_Fill
	export Shop_SaveAndTintFieldPaletteFar, Shop_SaveAndTintFieldPalette
	export Shop_RestoreFieldPaletteFar, Shop_RestoreFieldPalette
	export Func_08108068, ItemPicker_RunToDrop
	export Func_08108070, Func_0810b79c
	export Sound_WaitForJingleFar, Sound_WaitForJingle
	export Inn_PlaySleepFar, Inn_PlaySleep
	export VramBlock_LoadResourceFar, VramBlock_LoadResource
	export Func_08108090, Forge_PartyHasMaterial
	export Func_08108098, Forge_SetItem
	export Func_081080a0, Forge_GetItem
