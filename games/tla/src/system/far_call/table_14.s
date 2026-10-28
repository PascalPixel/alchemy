.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_080380f8
	.thumb_func
Func_080380f8:
	overlay_veneer Func_0803d3c0
	.global Window_CloseMatchingIdFar
	.thumb_func
Window_CloseMatchingIdFar:
	overlay_veneer Func_0803d450
	.global Text_GetMessageSizeFar
	.thumb_func
Text_GetMessageSizeFar:
	overlay_veneer Text_GetMessageSize
	.global Text_GetMessageSizeAltFar
	.thumb_func
Text_GetMessageSizeAltFar:
	overlay_veneer Text_GetMessageSizeAlt
	.global MessageArgument_ClearAllFar
	.thumb_func
MessageArgument_ClearAllFar:
	overlay_veneer MessageArgument_ClearAll
	.global MessageArgument_PushFar
	.thumb_func
MessageArgument_PushFar:
	overlay_veneer MessageArgument_Push
	.global PartyStatus_CreateWindowFar
	.thumb_func
PartyStatus_CreateWindowFar:
	overlay_veneer PartyStatus_CreateWindow
	.global PartyStatus_DrawFar
	.thumb_func
PartyStatus_DrawFar:
	overlay_veneer PartyStatus_Draw
	.global PartyStatus_CloseFar
	.thumb_func
PartyStatus_CloseFar:
	overlay_veneer PartyStatus_Close
	.global Message_CloseFinishedWindowsFar
	.thumb_func
Message_CloseFinishedWindowsFar:
	overlay_veneer Message_CloseFinishedWindows
	.global Window_CloseAllFar
	.thumb_func
Window_CloseAllFar:
	overlay_veneer Window_CloseAll
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
	overlay_veneer SelectionScreen_BuildEntries
	.global Func_080381b8
	.thumb_func
Func_080381b8:
	overlay_veneer Window_ReservedNoOp
