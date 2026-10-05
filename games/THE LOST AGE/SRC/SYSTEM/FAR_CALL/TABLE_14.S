.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
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
	.global Func_08038110
	.thumb_func
Func_08038110:
	overlay_veneer Func_0803b8cc
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
	.global UiWindow_DrawPartyStatusContentsFar
	.thumb_func
UiWindow_DrawPartyStatusContentsFar:
	overlay_veneer UiWindow_DrawPartyStatusContents
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
	.global Func_08038150
	.thumb_func
Func_08038150:
	overlay_veneer Func_0803dc1c
	.global Func_08038158
	.thumb_func
Func_08038158:
	overlay_veneer Func_0803dd24
	.global Func_08038160
	.thumb_func
Func_08038160:
	overlay_veneer Func_0803dd68
	.global Func_08038168
	.thumb_func
Func_08038168:
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
	overlay_veneer Func_0803e918
	.global Func_08038198
	.thumb_func
Func_08038198:
	overlay_veneer Func_0803e998
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
	overlay_veneer Func_0803dea8
