@ Import veneers after the OVERLAY_64A overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_64A overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __modsi3
	.thumb_func
__modsi3:
	overlay_veneer IwramSignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	overlay_veneer Func_080000d8
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	overlay_veneer Runtime_BumpAllocateAlternatePoolFar
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_020021f8
	.thumb_func
Func_020021f8:
	overlay_veneer Func_080001a8
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02002208
	.thumb_func
Func_02002208:
	overlay_veneer Func_080001f0
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global Blend_SetDarkenTarget16
	.thumb_func
Blend_SetDarkenTarget16:
	overlay_veneer Blend_SetDarkenTarget16Far
	.global Func_02002220
	.thumb_func
Func_02002220:
	overlay_veneer Func_08000300
	.global Func_02002228
	.thumb_func
Func_02002228:
	overlay_veneer Func_08000370
	.global SerialRuntime_BeginTransferB
	.thumb_func
SerialRuntime_BeginTransferB:
	overlay_veneer Party_CheckFar
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer Owner_GetStateFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
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
	.global Func_020022b0
	.thumb_func
Func_020022b0:
	overlay_veneer Func_080381f8
	.global Func_020022b8
	.thumb_func
Func_020022b8:
	overlay_veneer Func_08038210
	.global RenderOutput_RedrawSavedRect
	.thumb_func
RenderOutput_RedrawSavedRect:
	overlay_veneer RenderOutput_RedrawSavedRectFar
	.global UiWindow_SetTilemapEntry
	.thumb_func
UiWindow_SetTilemapEntry:
	overlay_veneer UiWindow_SetTilemapEntryFar
	.global Func_020022d0
	.thumb_func
Func_020022d0:
	overlay_veneer Func_080382c0
	.global Func_020022d8
	.thumb_func
Func_020022d8:
	overlay_veneer Func_080382c8
	.global Func_020022e0
	.thumb_func
Func_020022e0:
	overlay_veneer Func_080382d0
	.global Func_020022e8
	.thumb_func
Func_020022e8:
	overlay_veneer Func_080382d8
	.global Func_020022f0
	.thumb_func
Func_020022f0:
	overlay_veneer Func_080382f0
	.global Func_020022f8
	.thumb_func
Func_020022f8:
	overlay_veneer Func_080382f8
	.global Func_02002300
	.thumb_func
Func_02002300:
	overlay_veneer Func_08038300
	.global Func_02002308
	.thumb_func
Func_02002308:
	overlay_veneer Func_08038308
	.global Func_02002310
	.thumb_func
Func_02002310:
	overlay_veneer Func_08038348
	.global Menu_RunConfirmSelection
	.thumb_func
Menu_RunConfirmSelection:
	overlay_veneer Menu_RunConfirmSelectionFar
	.global Func_02002320
	.thumb_func
Func_02002320:
	overlay_veneer Func_080383b8
	.global Func_02002328
	.thumb_func
Func_02002328:
	overlay_veneer Func_08038428
	.global Func_02002330
	.thumb_func
Func_02002330:
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
	.global Func_02002350
	.thumb_func
Func_02002350:
	overlay_veneer Func_080ad090
	.global Party_AddActiveOwner
	.thumb_func
Party_AddActiveOwner:
	overlay_veneer Party_AddActiveOwnerFar
	.global Party_RemoveActiveOwner
	.thumb_func
Party_RemoveActiveOwner:
	overlay_veneer Party_RemoveActiveOwnerFar
	.global Owner_GetLevelThreshold
	.thumb_func
Owner_GetLevelThreshold:
	overlay_veneer Owner_GetLevelThresholdFar
	.global Func_02002370
	.thumb_func
Func_02002370:
	overlay_veneer Func_080ad238
	.global Djinn_AddToLeastLoadedOwner
	.thumb_func
Djinn_AddToLeastLoadedOwner:
	overlay_veneer Djinn_AddToLeastLoadedOwnerFar
	.global Owner_RefreshDerivedData
	.thumb_func
Owner_RefreshDerivedData:
	overlay_veneer Owner_RefreshDerivedDataFar
	.global Func_02002388
	.thumb_func
Func_02002388:
	overlay_veneer Func_080ad2c8
	.global Func_02002390
	.thumb_func
Func_02002390:
	overlay_veneer Func_080ad2e0
	.global Func_02002398
	.thumb_func
Func_02002398:
	overlay_veneer Func_080ad320
	.global Func_020023a0
	.thumb_func
Func_020023a0:
	overlay_veneer Func_080ad330
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_020023b8
	.thumb_func
Func_020023b8:
	overlay_veneer Event_SetPairWork1c0Far
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
	.ifdef TLA_EDITION_ES
	overlay_veneer Event_SetStatus1c6Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Event_SetStatus1c6Far
	.else
	overlay_veneer Event_WaitValue1c8FramesFar
	.endif
	.endif
	.global Func_020023d8
	.thumb_func
Func_020023d8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8548
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8548
	.else
	overlay_veneer Func_080c8558
	.endif
	.endif
	.global Func_020023e0
	.thumb_func
Func_020023e0:
	overlay_veneer Func_080c8940
	.global Func_020023e8
	.thumb_func
Func_020023e8:
	overlay_veneer Func_080f8010
	.global Func_020023f0
	.thumb_func
Func_020023f0:
	overlay_veneer Func_08108030
	.global Func_020023f8
	.thumb_func
Func_020023f8:
	overlay_veneer Func_08108040
	.global Func_02002400
	.thumb_func
Func_02002400:
	overlay_veneer Func_08108048
	.global Func_02002408
	.thumb_func
Func_02002408:
	overlay_veneer Audio_PlayCue
