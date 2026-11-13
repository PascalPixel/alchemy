.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_080380f8
	.thumb_func
Func_080380f8:
	overlay_veneer Portrait_OpenWindow
	.global Window_CloseMatchingIdFar
	.thumb_func
Window_CloseMatchingIdFar:
	overlay_veneer Portrait_CloseWindow
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
	overlay_veneer SelectionMenu_Create
	.global SelectionMenu_ClearFar
	.thumb_func
SelectionMenu_ClearFar:
	overlay_veneer SelectionMenu_Clear
	.global SelectionMenu_AddEntryFar
	.thumb_func
SelectionMenu_AddEntryFar:
	overlay_veneer SelectionMenu_AddEntry
	.global Resource_ScheduleOwnerResetDelayedFar
	.thumb_func
Resource_ScheduleOwnerResetDelayedFar:
	overlay_veneer SelectionMenu_StartDrawing
	.global SelectionMenu_StopDrawingFar
	.thumb_func
SelectionMenu_StopDrawingFar:
	overlay_veneer SelectionMenu_StopDrawing
	.global SelectionMenu_OpenNameWindowFar
	.thumb_func
SelectionMenu_OpenNameWindowFar:
	overlay_veneer SelectionMenu_OpenNameWindow
	.global SelectionMenu_CloseFar
	.thumb_func
SelectionMenu_CloseFar:
	overlay_veneer SelectionMenu_Close
	.global Func_08038188
	.thumb_func
Func_08038188:
	overlay_veneer Menu_SetupSelectionBothSides
	.global Func_08038190
	.thumb_func
Func_08038190:
	overlay_veneer SelectionMenu_Run
	.global Func_08038198
	.thumb_func
Func_08038198:
	overlay_veneer SelectionMenu_RunWithSounds
	.global Func_080381a0
	.thumb_func
Func_080381a0:
	overlay_veneer MenuArrow_NoOpCallbackBank
	.global Func_080381a8
	.thumb_func
Func_080381a8:
	overlay_veneer MenuArrow_NoOpCallback1
	.global Func_080381b0
	.thumb_func
Func_080381b0:
	overlay_veneer SelectionMenu_BuildEntries
	.global Func_080381b8
	.thumb_func
Func_080381b8:
	overlay_veneer SelectionMenu_ReservedNoOp
