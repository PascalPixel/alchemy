	.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Resource_FarCall008
Resource_FarCall008:
	export Func_08108000, Debug_ApplyPresetItemsAndFlags
	export Func_08108008, Func_081082c4
	export Func_08108010, Shop_ConfirmAct
	export Func_08108018, Inn_CheckIn
	export Func_08108020, Func_0810b5c4
	export Func_08108028, Func_0810b3ec
	export Func_08108030, ShopCursor_Advance
	export Func_08108038, MenuCursor_MoveTowardTarget
	export Func_08108040, Shop_SetCursor
	export Func_08108048, MenuCursor_SetPositionImmediate
	export Shop_FillSelectorFar, Shop_FillSelector
	export Shop_RestoreSceneTilesFar, Shop_RestoreSceneTiles
	export Shop_InitializeEffectFar, Shop_InitializeEffect
	export Func_08108068, Func_0810b7a8
	export Func_08108070, Func_0810b79c
	export Sound_WaitForJingleFar, Sound_WaitForJingle
	export Inn_PlaySleepFar, Inn_PlaySleep
	export VramBlock_LoadResourceFar, VramBlock_LoadResource
	export Func_08108090, Forge_PartyHasMaterial
	export Func_08108098, Forge_SetItem
	export Func_081080a0, Forge_GetItem
