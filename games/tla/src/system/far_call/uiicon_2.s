.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Icon_PrepareActorFar
	.thumb_func
Icon_PrepareActorFar:
	overlay_veneer Icon_PrepareActor
	.global DjinnMenu_Open
	.thumb_func
DjinnMenu_Open:
	overlay_veneer Func_08100e7c
	.global ItemMenu_DrawItemDetailsFar
	.thumb_func
ItemMenu_DrawItemDetailsFar:
	overlay_veneer ItemMenu_DrawItemDetails
	.global Func_080f8040
	.thumb_func
Func_080f8040:
	overlay_veneer Func_080fb8b8
	.global Palette_CopyObjectBankToBackground14Far
	.thumb_func
Palette_CopyObjectBankToBackground14Far:
	overlay_veneer Palette_CopyObjectBankToBackground14
	.global Menu_UpdateEntryObjectTransformsFar
	.thumb_func
Menu_UpdateEntryObjectTransformsFar:
	overlay_veneer Menu_UpdateEntryObjectTransforms
