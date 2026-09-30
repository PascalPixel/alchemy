@ Import veneers after the OVERLAY_6B5 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6B5 overlay import veneers
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
	.global Func_020018e4
	.thumb_func
Func_020018e4:
	overlay_veneer Func_080000d0
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	overlay_veneer Func_080000d8
	.global Runtime_AllocateBlock
	.thumb_func
Runtime_AllocateBlock:
	overlay_veneer Runtime_AllocateBlockFar
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global Func_02001904
	.thumb_func
Func_02001904:
	overlay_veneer Func_08000170
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_02001914
	.thumb_func
Func_02001914:
	overlay_veneer Func_08000300
	.global Func_0200191c
	.thumb_func
Func_0200191c:
	overlay_veneer Func_08000370
	.global Func_02001924
	.thumb_func
Func_02001924:
	overlay_veneer Func_08000378
	.global Func_0200192c
	.thumb_func
Func_0200192c:
	overlay_veneer Func_08000380
	.global Func_02001934
	.thumb_func
Func_02001934:
	overlay_veneer Func_08000388
	.global Func_0200193c
	.thumb_func
Func_0200193c:
	overlay_veneer Func_080003a8
	.global Func_02001944
	.thumb_func
Func_02001944:
	overlay_veneer Func_080003b8
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer Owner_GetStateFar
	.global Func_02001954
	.thumb_func
Func_02001954:
	overlay_veneer Func_080003c8
	.global Func_0200195c
	.thumb_func
Func_0200195c:
	overlay_veneer Func_080003d0
	.global Func_02001964
	.thumb_func
Func_02001964:
	overlay_veneer Func_080003d8
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
	overlay_veneer Func_08020120
	.global Func_02001984
	.thumb_func
Func_02001984:
	overlay_veneer Func_08020178
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
	overlay_veneer Func_080201e8
	.global UiWindow_Create
	.thumb_func
UiWindow_Create:
	overlay_veneer UiWindow_CreateFar
	.global UiWork_Finalize
	.thumb_func
UiWork_Finalize:
	overlay_veneer UiWork_FinalizeFar
	.global Ui_AdjustValueWithoutLimit
	.thumb_func
Ui_AdjustValueWithoutLimit:
	overlay_veneer Ui_AdjustValueWithoutLimitFar
	.global UiText_OpenMessageWindow
	.thumb_func
UiText_OpenMessageWindow:
	overlay_veneer UiText_OpenMessageWindowFar
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global RenderOutput_PrepareForRedraw
	.thumb_func
RenderOutput_PrepareForRedraw:
	overlay_veneer RenderOutput_PrepareForRedrawFar
	.global UiText_DrawNumber
	.thumb_func
UiText_DrawNumber:
	overlay_veneer UiText_DrawNumberFar
	.global UiWork_ClearValueNameTables
	.thumb_func
UiWork_ClearValueNameTables:
	overlay_veneer UiWork_ClearValueNameTablesFar
	.global Func_020019e4
	.thumb_func
Func_020019e4:
	overlay_veneer UiText_DrawQuantity
	.global Func_020019ec
	.thumb_func
Func_020019ec:
	overlay_veneer Func_080382e0
	.global Trade_GetOfferState
	.thumb_func
Trade_GetOfferState:
	overlay_veneer Trade_GetOfferStateFar
	.global Party_CountActiveOwners
	.thumb_func
Party_CountActiveOwners:
	overlay_veneer Party_CountActiveOwnersFar
	.global Func_02001a04
	.thumb_func
Func_02001a04:
	overlay_veneer Func_080ad208
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02001a14
	.thumb_func
Func_02001a14:
	overlay_veneer Func_080c8018
	.global Func_02001a1c
	.thumb_func
Func_02001a1c:
	overlay_veneer Func_080c8020
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Func_02001a5c
	.thumb_func
Func_02001a5c:
	overlay_veneer Func_080c8158
	.global Func_02001a64
	.thumb_func
Func_02001a64:
	overlay_veneer Func_080c8180
	.global Func_02001a6c
	.thumb_func
Func_02001a6c:
	overlay_veneer Func_080c8188
	.global Func_02001a74
	.thumb_func
Func_02001a74:
	overlay_veneer Func_080c81a0
	.global Func_02001a7c
	.thumb_func
Func_02001a7c:
	overlay_veneer Func_080c8210
	.global Func_02001a84
	.thumb_func
Func_02001a84:
	overlay_veneer Func_080c8268
	.global Func_02001a8c
	.thumb_func
Func_02001a8c:
	overlay_veneer Func_080c8280
	.global Func_02001a94
	.thumb_func
Func_02001a94:
	overlay_veneer Func_080c8290
	.global Func_02001a9c
	.thumb_func
Func_02001a9c:
	overlay_veneer Func_080c8298
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_02001ab4
	.thumb_func
Func_02001ab4:
	overlay_veneer Func_080c84e0
	.global Func_02001abc
	.thumb_func
Func_02001abc:
	overlay_veneer Func_08108010
	.global BattlePlacement_UpdateTimedEntriesTwentyTimes
	.thumb_func
BattlePlacement_UpdateTimedEntriesTwentyTimes:
	overlay_veneer BattlePlacement_UpdateTimedEntriesTwentyTimesFar
	.global DebugParty_LoadPreset
	.thumb_func
DebugParty_LoadPreset:
	overlay_veneer DebugParty_LoadPresetFar
	.global Func_02001ad4
	.thumb_func
Func_02001ad4:
	overlay_veneer Audio_PlayCue
	.global Func_02001adc
	.thumb_func
Func_02001adc:
	overlay_veneer Func_081c0090
