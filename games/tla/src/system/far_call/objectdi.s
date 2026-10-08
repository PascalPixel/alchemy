.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Animation_SetWorkEntryFar
	.thumb_func
Animation_SetWorkEntryFar:
	overlay_veneer Animation_SetWorkEntry
	.global ObjectSystem_InitializeFar
	.thumb_func
ObjectSystem_InitializeFar:
	overlay_veneer Func_080230e0
	.global Object_SetMode
	.thumb_func
Object_SetMode:
	overlay_veneer ObjectDispatch_ApplyArgumentToChildren
	.global ObjectDispatch_ApplyValueToChildrenFar
	.thumb_func
ObjectDispatch_ApplyValueToChildrenFar:
	overlay_veneer ObjectDispatch_ApplyValueToChildren
	.global ObjectDispatch_ApplyPairToChildrenFar
	.thumb_func
ObjectDispatch_ApplyPairToChildrenFar:
	overlay_veneer ObjectDispatch_ApplyPairToChildren
