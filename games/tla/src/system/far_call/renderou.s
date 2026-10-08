.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global RenderOutput_CreateFar
	.thumb_func
RenderOutput_CreateFar:
	overlay_veneer RenderOutput_Create
	.global RenderOutput_CreateFromResourceFar
	.thumb_func
RenderOutput_CreateFromResourceFar:
	overlay_veneer RenderOutput_CreateFromResource
	.global SideObject_CreateFar
	.thumb_func
SideObject_CreateFar:
	overlay_veneer CreateSideObject
	.global Ui_LoadCharacterEntryForSlotFar
	.thumb_func
Ui_LoadCharacterEntryForSlotFar:
	overlay_veneer Ui_LoadCharacterEntryForSlot
	.global RenderOutput_SetAnimationFar
	.thumb_func
RenderOutput_SetAnimationFar:
	overlay_veneer RenderOutput_SetAnimation
	.global Func_080380f0
	.thumb_func
Func_080380f0:
	overlay_veneer Func_0804257c
