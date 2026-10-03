@ Far-call stub table at 0x080380f8: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
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
	overlay_veneer Func_0803cca8
	.global UiText_DrawQuantity
	.thumb_func
UiText_DrawQuantity:
	overlay_veneer UiWork_PushValueSlot
	.global Func_08038128
	.thumb_func
Func_08038128:
	overlay_veneer Func_08042690
	.global UiWindow_DrawPartyStatusContentsFar
	.thumb_func
UiWindow_DrawPartyStatusContentsFar:
	overlay_veneer Func_0804297c
	.global Func_08038138
	.thumb_func
Func_08038138:
	overlay_veneer Func_08042dac
	.global UiWork_FinalizePendingCoreFar
	.thumb_func
UiWork_FinalizePendingCoreFar:
	overlay_veneer Func_0803ce1c
	.global Func_08038148
	.thumb_func
Func_08038148:
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
	.global Func_08038180
	.thumb_func
Func_08038180:
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
