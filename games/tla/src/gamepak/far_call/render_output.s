.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global WindowSprite_CreateFar
	.thumb_func
WindowSprite_CreateFar:
	overlay_veneer WindowSprite_Create
	.global WindowSprite_CreateFromResourceFar
	.thumb_func
WindowSprite_CreateFromResourceFar:
	overlay_veneer WindowSprite_CreateFromResource
	.global SideObject_CreateFar
	.thumb_func
SideObject_CreateFar:
	overlay_veneer CreateSideObject
	.global Portrait_ReloadFar
	.thumb_func
Portrait_ReloadFar:
	overlay_veneer Portrait_Reload
	.global WindowSprite_SetMotionFar
	.thumb_func
WindowSprite_SetMotionFar:
	overlay_veneer WindowSprite_SetMotion
	.global Func_080380f0
	.thumb_func
Func_080380f0:
	overlay_veneer Func_0804257c
