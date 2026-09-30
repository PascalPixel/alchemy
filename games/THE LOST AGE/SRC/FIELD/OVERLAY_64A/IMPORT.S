@ Import veneers after the OVERLAY_64A overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_64A overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer IwramSignedDivide
	.global Engine_MathRemainder
	.thumb_func
Engine_MathRemainder:
	overlay_veneer IwramSignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	overlay_veneer Func_080000d0
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	overlay_veneer Func_080000d8
	overlay_veneer Func_08000170
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	overlay_veneer Func_080001a8
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	overlay_veneer Func_080001f0
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	overlay_veneer Func_080002a8
	overlay_veneer Func_08000300
	overlay_veneer Func_08000370
	overlay_veneer Func_08000388
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer Owner_GetStateFar
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_080003d8
	.global UiWindow_Create
	.thumb_func
UiWindow_Create:
	overlay_veneer UiWindow_CreateFar
	.global UiWork_Finalize
	.thumb_func
UiWork_Finalize:
	overlay_veneer UiWork_FinalizeFar
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global RenderOutput_PrepareForRedraw
	.thumb_func
RenderOutput_PrepareForRedraw:
	overlay_veneer RenderOutput_PrepareForRedrawFar
	.global UiWindow_DrawDividerLine
	.thumb_func
UiWindow_DrawDividerLine:
	overlay_veneer UiWindow_DrawDividerLineFar
	.global UiText_DrawResource
	.thumb_func
UiText_DrawResource:
	overlay_veneer UiText_DrawResourceFar
	.global UiText_DrawCharacterAtOffset
	.thumb_func
UiText_DrawCharacterAtOffset:
	overlay_veneer UiText_DrawCharacterAtOffsetFar
	.global UiText_DrawStringInWindow
	.thumb_func
UiText_DrawStringInWindow:
	overlay_veneer UiText_DrawStringInWindowFar
	.global RenderOutput_Create
	.thumb_func
RenderOutput_Create:
	overlay_veneer RenderOutput_CreateFar
	.global UiWork_FinalizePendingCore
	.thumb_func
UiWork_FinalizePendingCore:
	overlay_veneer UiWork_FinalizePendingCoreFar
	.global Menu_SelectSaveSlotAction
	.thumb_func
Menu_SelectSaveSlotAction:
	overlay_veneer Menu_SelectSaveSlotActionFar
	overlay_veneer Func_080381f8
	overlay_veneer Func_08038210
	.global RenderOutput_RedrawSavedRect
	.thumb_func
RenderOutput_RedrawSavedRect:
	overlay_veneer RenderOutput_RedrawSavedRectFar
	.global UiWindow_SetTilemapEntry
	.thumb_func
UiWindow_SetTilemapEntry:
	overlay_veneer UiWindow_SetTilemapEntryFar
	overlay_veneer Func_080382c0
	overlay_veneer Func_080382c8
	overlay_veneer Func_080382d0
	overlay_veneer Func_080382d8
	overlay_veneer Func_080382f0
	overlay_veneer Func_080382f8
	overlay_veneer Func_08038300
	overlay_veneer Func_08038308
	overlay_veneer Func_08038348
	.global Menu_RunConfirmSelection
	.thumb_func
Menu_RunConfirmSelection:
	overlay_veneer Menu_RunConfirmSelectionFar
	overlay_veneer Func_080383b8
	overlay_veneer Func_08038428
	overlay_veneer Func_08038430
	.global Trade_GetOfferState
	.thumb_func
Trade_GetOfferState:
	overlay_veneer Trade_GetOfferStateFar
	.global Owner_RecalculateStats
	.thumb_func
Owner_RecalculateStats:
	overlay_veneer Owner_RecalculateStatsFar
	.global PartyInventory_Add
	.thumb_func
PartyInventory_Add:
	overlay_veneer PartyInventory_AddFar
	overlay_veneer Func_080ad090
	.global Party_AddActiveOwner
	.thumb_func
Party_AddActiveOwner:
	overlay_veneer Party_AddActiveOwnerFar
	.global Party_RemoveActiveOwner
	.thumb_func
Party_RemoveActiveOwner:
	overlay_veneer Party_RemoveActiveOwnerFar
	overlay_veneer Func_080ad200
	overlay_veneer Func_080ad238
	.global Djinn_AddToLeastLoadedOwner
	.thumb_func
Djinn_AddToLeastLoadedOwner:
	overlay_veneer Djinn_AddToLeastLoadedOwnerFar
	overlay_veneer Func_080ad298
	overlay_veneer Func_080ad2c8
	overlay_veneer Func_080ad2e0
	overlay_veneer Func_080ad320
	overlay_veneer Func_080ad330
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	overlay_veneer Func_080c8268
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_ClearStatus1c6
	.thumb_func
Event_ClearStatus1c6:
	overlay_veneer Event_ClearStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	overlay_veneer Func_080c8558
	overlay_veneer Func_080c8940
	overlay_veneer Func_080f8010
	overlay_veneer Func_08108030
	overlay_veneer Func_08108040
	overlay_veneer Func_08108048
	overlay_veneer Audio_PlayCue
