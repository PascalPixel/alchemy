.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Event_ResolveAllPendingActionsFar
	.thumb_func
Event_ResolveAllPendingActionsFar:
	overlay_veneer Event_ResolveAllPendingActions
	.global Event_ResolvePendingActionsFar
	.thumb_func
Event_ResolvePendingActionsFar:
	overlay_veneer Event_ResolvePendingActions
	.global Func_080c84e8
	.thumb_func
Func_080c84e8:
	overlay_veneer Func_080dc978
	.global Func_080c84f0
	.thumb_func
Func_080c84f0:
	overlay_veneer BattleEffect_FinishSceneAndReleaseHeapBlock
	.global Func_080c84f8
	.thumb_func
Func_080c84f8:
	overlay_veneer Palette_FillNumber15Grey
	.global Func_080c8500
	.thumb_func
Func_080c8500:
	overlay_veneer Func_080dcadc
