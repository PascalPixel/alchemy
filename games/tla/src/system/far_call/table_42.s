.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global VramBlock_LoadResourceFar
	.thumb_func
VramBlock_LoadResourceFar:
	overlay_veneer VramBlock_LoadResource
	.global Func_08108090
	.thumb_func
Func_08108090:
	overlay_veneer Forge_PartyHasMaterial
	.global Func_08108098
	.thumb_func
Func_08108098:
	overlay_veneer Forge_SetItem
	.global Func_081080a0
	.thumb_func
Func_081080a0:
	overlay_veneer Forge_GetItem
