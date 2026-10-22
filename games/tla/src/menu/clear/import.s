.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
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
	.global MemFree
	.thumb_func
MemFree:
	overlay_veneer Sys_FreeFar
	.global Func_020021f8
	.thumb_func
Func_020021f8:
	overlay_veneer Resource_DecodeType01Far
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02002208
	.thumb_func
Func_02002208:
	overlay_veneer Ui_LoadWindowGraphicsFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global Fade_ToBlack
	.thumb_func
Fade_ToBlack:
	overlay_veneer Fade_ToBlackFar
	.global Func_02002220
	.thumb_func
Func_02002220:
	overlay_veneer SerialRuntime_InitializeFar
	.global Func_02002228
	.thumb_func
Func_02002228:
	overlay_veneer SerialRuntime_RemoveIrqHandlersFar
	.global SerialRuntime_BeginReceive
	.thumb_func
SerialRuntime_BeginReceive:
	overlay_veneer Party_CheckFar
	.global Unit_Get
	.thumb_func
Unit_Get:
	overlay_veneer Unit_GetFar
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
	.ifndef TLA_EDITION_JA
	.global UiText_DrawStringInWindow
	.thumb_func
UiText_DrawStringInWindow:
	overlay_veneer UiText_DrawStringInWindowFar
	.endif
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
	overlay_veneer Resource_CopyFixedBlockBFar
	.global Func_02002308
	.thumb_func
Func_02002308:
	overlay_veneer Resource_CopyFixedBlockAFar
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
	overlay_veneer Graphics_ExpandVramTilesByColorTableFar
	.global Func_02002328
	.thumb_func
Func_02002328:
	overlay_veneer Func_08038428
	.global Func_02002330
	.thumb_func
Func_02002330:
	overlay_veneer Func_08038430
	.global Djinn_GetRecoveryTable
	.thumb_func
Djinn_GetRecoveryTable:
	overlay_veneer Djinn_GetRecoveryTableFar
	.global Unit_Calculate
	.thumb_func
Unit_Calculate:
	overlay_veneer Unit_CalculateFar
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
	.ifndef TLA_EDITION_JA
	.global Unit_GetExperienceForLevel
	.thumb_func
Unit_GetExperienceForLevel:
	overlay_veneer Unit_GetExperienceForLevelFar
	.endif
	.global Func_02002370
	.thumb_func
Func_02002370:
	overlay_veneer Func_080ad238
	.global Djinn_AddToLeastLoadedOwner
	.thumb_func
Djinn_AddToLeastLoadedOwner:
	overlay_veneer Djinn_AddToLeastLoadedOwnerFar
	.global Unit_UpdateClass
	.thumb_func
Unit_UpdateClass:
	overlay_veneer Unit_UpdateClassFar
	.global Func_02002388
	.thumb_func
Func_02002388:
	overlay_veneer Runtime_GetBuildStampTimeFar
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
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_020023b8
	.thumb_func
Func_020023b8:
	overlay_veneer Event_SetDestinationFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattlePalette_StopBlendAndReleaseBufferFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattlePalette_StopBlendAndReleaseBufferFar
	.else
	overlay_veneer Event_StartScreenTransitionFar
	.endif
	.endif
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_SetPrimaryBufferValueFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_SetPrimaryBufferValueFar
	.else
	overlay_veneer Event_FinishScreenTransitionFar
	.endif
	.endif
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	.ifdef TLA_EDITION_ES
	overlay_veneer Event_DelayEffectFramesFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Event_DelayEffectFramesFar
	.else
	overlay_veneer Event_WaitTransitionFramesFar
	.endif
	.endif
	.global Func_020023d8
	.thumb_func
Func_020023d8:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleParty_ApplyHealthDeltaFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleParty_ApplyHealthDeltaFar
	.else
	overlay_veneer Func_080c8558
	.endif
	.endif
	.global Func_020023e0
	.thumb_func
Func_020023e0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8930
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8930
	.else
	overlay_veneer Func_080c8940
	.endif
	.endif
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
