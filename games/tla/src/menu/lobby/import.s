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
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Task_RemoveFar
	.thumb_func
Task_RemoveFar:
	overlay_veneer Func_080000d8
	.global HeapSlot_AllocEwram
	.thumb_func
HeapSlot_AllocEwram:
	overlay_veneer HeapSlot_AllocEwramFar
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
	.global Heap_AllocEwram
	.thumb_func
Heap_AllocEwram:
	overlay_veneer Heap_AllocEwramFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global Func_02001914
	.thumb_func
Func_02001914:
	overlay_veneer Serial_InitializeFar
	.global Func_0200191c
	.thumb_func
Func_0200191c:
	overlay_veneer Serial_RemoveIrqHandlersFar
	.global Sound_LoadPresetParameters
	.thumb_func
Sound_LoadPresetParameters:
	overlay_veneer Sound_LoadPresetParametersFar
	.global Func_0200192c
	.thumb_func
Func_0200192c:
	overlay_veneer Func_08000380
	.global Serial_StartReceive
	.thumb_func
Serial_StartReceive:
	overlay_veneer Party_CheckFar
	.global Func_0200193c
	.thumb_func
Func_0200193c:
	overlay_veneer Serial_GetActiveTransfersFar
	.global Func_02001944
	.thumb_func
Func_02001944:
	overlay_veneer Task_SetFlagsFar
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
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global Func_0200197c
	.thumb_func
Func_0200197c:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02001984
	.thumb_func
Func_02001984:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_0200198c
	.thumb_func
Func_0200198c:
	overlay_veneer Func_08020198
	.global Func_02001994
	.thumb_func
Func_02001994:
	overlay_veneer Func_080201a0
	.global Func_0200199c
	.thumb_func
Func_0200199c:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Window_Create
	.thumb_func
Window_Create:
	overlay_veneer Window_CreateFar
	.global Window_Close
	.thumb_func
Window_Close:
	overlay_veneer Window_CloseFar
	.global Message_DecodeRawUnbounded
	.thumb_func
Message_DecodeRawUnbounded:
	overlay_veneer Message_DecodeRawUnboundedFar
	.global Message_OpenWindow
	.thumb_func
Message_OpenWindow:
	overlay_veneer Message_OpenWindowFar
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global WindowSprite_PrepareForRedraw
	.thumb_func
WindowSprite_PrepareForRedraw:
	overlay_veneer WindowSprite_PrepareForRedrawFar
	.global Text_DrawNumber
	.thumb_func
Text_DrawNumber:
	overlay_veneer Text_DrawNumberFar
	.global MessageArgument_ClearAll
	.thumb_func
MessageArgument_ClearAll:
	overlay_veneer MessageArgument_ClearAllFar
	.global Func_020019e4
	.thumb_func
Func_020019e4:
	overlay_veneer MessageArgument_PushFar
	.global Func_020019ec
	.thumb_func
Func_020019ec:
	overlay_veneer Func_080382e0
	.global Djinn_GetRecoveryTable
	.thumb_func
Djinn_GetRecoveryTable:
	overlay_veneer Djinn_GetRecoveryTableFar
	.global Party_CountActiveOwners
	.thumb_func
Party_CountActiveOwners:
	overlay_veneer Party_CountActiveOwnersFar
	.global Func_02001a04
	.thumb_func
Func_02001a04:
	overlay_veneer Func_080ad208
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02001a14
	.thumb_func
Func_02001a14:
	overlay_veneer Event_StartFar
	.global Func_02001a1c
	.thumb_func
Func_02001a1c:
	overlay_veneer Event_EndFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_ResetAndSetPositionInMode2
	.thumb_func
Actor_ResetAndSetPositionInMode2:
	overlay_veneer Actor_ResetAndSetPositionInMode2Far
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Func_02001a64
	.thumb_func
Func_02001a64:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02001a74
	.thumb_func
Func_02001a74:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Func_02001a7c
	.thumb_func
Func_02001a7c:
	overlay_veneer Func_080c8210
	.global Func_02001a84
	.thumb_func
Func_02001a84:
	overlay_veneer Event_SetDestinationFar
	.global Func_02001a8c
	.thumb_func
Func_02001a8c:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Party_SetFields1f2And1f4
	.thumb_func
Party_SetFields1f2And1f4:
	overlay_veneer Party_SetFields1f2And1f4Far
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02001ab4
	.thumb_func
Func_02001ab4:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02001abc
	.thumb_func
Func_02001abc:
	overlay_veneer Func_08108010
	.global DjinnRecovery_CountDownTwenty
	.thumb_func
DjinnRecovery_CountDownTwenty:
	overlay_veneer DjinnRecovery_CountDownTwentyFar
	.global DebugParty_LoadPreset
	.thumb_func
DebugParty_LoadPreset:
	overlay_veneer DebugParty_LoadPresetFar
	.global Func_02001ad4
	.thumb_func
Func_02001ad4:
	overlay_veneer Sound_PlayFar
	.global Func_02001adc
	.thumb_func
Func_02001adc:
	overlay_veneer SoundFlag_TestFar
