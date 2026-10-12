.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_080380f8
	.thumb_func
Func_080380f8:
	overlay_veneer Func_0803d3c0
	.global UiWork_FinalizeEntityMatchingLocalizedIdFar
	.thumb_func
UiWork_FinalizeEntityMatchingLocalizedIdFar:
	overlay_veneer Func_0803d450
	.global UiText_GetResourceDimensionsFar
	.thumb_func
UiText_GetResourceDimensionsFar:
	overlay_veneer UiText_GetResourceDimensions
	.global UiText_GetResourceDimensionsAltFar
	.thumb_func
UiText_GetResourceDimensionsAltFar:
	overlay_veneer UiText_GetResourceDimensionsAlt
	.global UiWork_ClearValueNameTablesFar
	.thumb_func
UiWork_ClearValueNameTablesFar:
	overlay_veneer UiWork_ClearValueNameTables
	.global UiText_DrawQuantity
	.thumb_func
UiText_DrawQuantity:
	overlay_veneer UiWork_PushValueSlot
	.global UiWindow_CreateWithLayoutBoundsFar
	.thumb_func
UiWindow_CreateWithLayoutBoundsFar:
	overlay_veneer UiWindow_CreateWithLayoutBounds
	.global PtyStDrawFar
	.thumb_func
PtyStDrawFar:
	overlay_veneer PtyStDraw
	.global UiWork_FinalizeAndReleaseBlock16Far
	.thumb_func
UiWork_FinalizeAndReleaseBlock16Far:
	overlay_veneer UiWork_FinalizeAndReleaseBlock16
	.global UiWork_FinalizePendingCoreFar
	.thumb_func
UiWork_FinalizePendingCoreFar:
	overlay_veneer UiWork_FinalizePendingCore
	.global UiWork_DrainPendingFar
	.thumb_func
UiWork_DrainPendingFar:
	overlay_veneer UiWork_DrainPending
	.global UiGlyph_ResetWorkStateFar
	.thumb_func
UiGlyph_ResetWorkStateFar:
	overlay_veneer Func_0803dc1c
	.global Resource_ClearOwnerListAndCountersFar
	.thumb_func
Resource_ClearOwnerListAndCountersFar:
	overlay_veneer Resource_ClearOwnerListAndCounters
	.global Resource_PushPendingPairFar
	.thumb_func
Resource_PushPendingPairFar:
	overlay_veneer Resource_PushPendingPair
	.global Resource_ScheduleOwnerResetDelayedFar
	.thumb_func
Resource_ScheduleOwnerResetDelayedFar:
	overlay_veneer Func_0803df00
	.global Resource_ScheduleOwnerResetFar
	.thumb_func
Resource_ScheduleOwnerResetFar:
	overlay_veneer Resource_ScheduleOwnerReset
	.global Menu_OpenSelectionWindowFar
	.thumb_func
Menu_OpenSelectionWindowFar:
	overlay_veneer Menu_OpenSelectionWindow
	.global Resource_ResetOwnerEntriesFar
	.thumb_func
Resource_ResetOwnerEntriesFar:
	overlay_veneer Resource_ResetOwnerEntries
	.global Func_08038188
	.thumb_func
Func_08038188:
	overlay_veneer Func_0803e7ac
	.global Func_08038190
	.thumb_func
Func_08038190:
	overlay_veneer Menu_SelectionLoop
	.global Func_08038198
	.thumb_func
Func_08038198:
	overlay_veneer Menu_WaitForSelectionInput
	.global Func_080381a0
	.thumb_func
Func_080381a0:
	overlay_veneer Func_0803f610
	.global Func_080381a8
	.thumb_func
Func_080381a8:
	overlay_veneer Func_0803f614
	.global Func_080381b0
	.thumb_func
Func_080381b0:
	overlay_veneer Func_0803dd98
	.global Func_080381b8
	.thumb_func
Func_080381b8:
	overlay_veneer UiWork_ReservedNoOp
