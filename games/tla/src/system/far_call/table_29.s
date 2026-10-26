.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global EventRuntime_ResolveAllPendingActionsFar
	.thumb_func
EventRuntime_ResolveAllPendingActionsFar:
	overlay_veneer EventRuntime_ResolveAllPendingActions
	.global EventRuntime_ResolvePendingActionsFar
	.thumb_func
EventRuntime_ResolvePendingActionsFar:
	overlay_veneer EventRuntime_ResolvePendingActions
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
	overlay_veneer Ui_FillBank15PaletteGrey
	.global Func_080c8500
	.thumb_func
Func_080c8500:
	overlay_veneer Func_080dcadc
