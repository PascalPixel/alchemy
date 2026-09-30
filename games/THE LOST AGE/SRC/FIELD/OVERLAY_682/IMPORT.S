@ Import veneers after the OVERLAY_682 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_682 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Func_020029bc
	.thumb_func
Func_020029bc:
	overlay_veneer Func_080000d0
	.global Func_020029c4
	.thumb_func
Func_020029c4:
	overlay_veneer Func_080003c8
	.global Func_020029cc
	.thumb_func
Func_020029cc:
	overlay_veneer Func_080003d0
	.global Func_020029d4
	.thumb_func
Func_020029d4:
	overlay_veneer Func_080003d8
	.global Func_020029dc
	.thumb_func
Func_020029dc:
	overlay_veneer Object_SetMode
	.global Func_020029e4
	.thumb_func
Func_020029e4:
	overlay_veneer Object_SetCallback
	.global Func_020029ec
	.thumb_func
Func_020029ec:
	overlay_veneer Func_080200c0
	.global Func_020029f4
	.thumb_func
Func_020029f4:
	overlay_veneer Func_08020198
	.global Func_020029fc
	.thumb_func
Func_020029fc:
	overlay_veneer Func_080201a0
	.global Func_02002a04
	.thumb_func
Func_02002a04:
	overlay_veneer Func_080201e8
	.global Func_02002a0c
	.thumb_func
Func_02002a0c:
	overlay_veneer Func_080201f0
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global UiText_OpenMessageWindow
	.thumb_func
UiText_OpenMessageWindow:
	overlay_veneer UiText_OpenMessageWindowFar
	.global UiWork_IsComplete
	.thumb_func
UiWork_IsComplete:
	overlay_veneer UiWork_IsCompleteFar
	.global Func_02002a2c
	.thumb_func
Func_02002a2c:
	overlay_veneer Func_080380f8
	.global UiWork_FinalizeEntityMatchingLocalizedId
	.thumb_func
UiWork_FinalizeEntityMatchingLocalizedId:
	overlay_veneer UiWork_FinalizeEntityMatchingLocalizedIdFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02002a44
	.thumb_func
Func_02002a44:
	overlay_veneer Func_080c8018
	.global Func_02002a4c
	.thumb_func
Func_02002a4c:
	overlay_veneer Func_080c8020
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Func_02002a5c
	.thumb_func
Func_02002a5c:
	overlay_veneer Func_080c8078
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_SnapHeadingAndOffset
	.thumb_func
ObjectMotion_SnapHeadingAndOffset:
	overlay_veneer ObjectMotion_SnapHeadingAndOffsetFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02002a9c
	.thumb_func
Func_02002a9c:
	overlay_veneer Func_080c80f8
	.global Func_02002aa4
	.thumb_func
Func_02002aa4:
	overlay_veneer Func_080c8100
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Func_02002adc
	.thumb_func
Func_02002adc:
	overlay_veneer Func_080c8180
	.global Func_02002ae4
	.thumb_func
Func_02002ae4:
	overlay_veneer Func_080c8188
	.global Func_02002aec
	.thumb_func
Func_02002aec:
	overlay_veneer Func_080c8198
	.global Func_02002af4
	.thumb_func
Func_02002af4:
	overlay_veneer Func_080c81a0
	.global Func_02002afc
	.thumb_func
Func_02002afc:
	overlay_veneer Func_080c81d0
	.global Func_02002b04
	.thumb_func
Func_02002b04:
	overlay_veneer Func_080c81d8
	.global Func_02002b0c
	.thumb_func
Func_02002b0c:
	overlay_veneer Func_080c81f0
	.global Func_02002b14
	.thumb_func
Func_02002b14:
	overlay_veneer Func_080c8200
	.global Func_02002b1c
	.thumb_func
Func_02002b1c:
	overlay_veneer Func_080c8210
	.global Func_02002b24
	.thumb_func
Func_02002b24:
	overlay_veneer Func_080c8218
	.global Func_02002b2c
	.thumb_func
Func_02002b2c:
	overlay_veneer Func_080c8228
	.global Func_02002b34
	.thumb_func
Func_02002b34:
	overlay_veneer Func_080c8238
	.global Func_02002b3c
	.thumb_func
Func_02002b3c:
	overlay_veneer Func_080c8240
	.global Func_02002b44
	.thumb_func
Func_02002b44:
	overlay_veneer Func_080c8278
	.global Event_ClearStatus1c6
	.thumb_func
Event_ClearStatus1c6:
	overlay_veneer Event_ClearStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_02002b5c
	.thumb_func
Func_02002b5c:
	overlay_veneer Func_080c8408
	.global Func_02002b64
	.thumb_func
Func_02002b64:
	overlay_veneer Func_080c84e0
	.global BattleFx_GetResourceId
	.thumb_func
BattleFx_GetResourceId:
	overlay_veneer BattleFx_GetResourceIdFar
	.global Func_02002b74
	.thumb_func
Func_02002b74:
	overlay_veneer Func_080c85e8
	.global ObjectMotion_OffsetPositionAndReset
	.thumb_func
ObjectMotion_OffsetPositionAndReset:
	overlay_veneer ObjectMotion_OffsetPositionAndResetFar
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global ObjectTable_ReadActiveValue
	.thumb_func
ObjectTable_ReadActiveValue:
	overlay_veneer ObjectTable_ReadActiveValueFar
	.global Object_SetActionCallback
	.thumb_func
Object_SetActionCallback:
	overlay_veneer Object_SetActionCallbackFar
	.global Func_02002ba4
	.thumb_func
Func_02002ba4:
	overlay_veneer Func_080c86d0
	.global Func_02002bac
	.thumb_func
Func_02002bac:
	overlay_veneer Func_080c86d8
	.global Func_02002bb4
	.thumb_func
Func_02002bb4:
	overlay_veneer Func_080c8778
	.global Func_02002bbc
	.thumb_func
Func_02002bbc:
	overlay_veneer Func_080c8860
	.global Func_02002bc4
	.thumb_func
Func_02002bc4:
	overlay_veneer Audio_PlayCue
