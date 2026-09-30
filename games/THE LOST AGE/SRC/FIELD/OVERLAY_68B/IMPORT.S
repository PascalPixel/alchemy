@ Import veneers after the OVERLAY_68B overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_68B overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathRemainder
	.thumb_func
Engine_MathRemainder:
	overlay_veneer IwramSignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Func_02001b54
	.thumb_func
Func_02001b54:
	overlay_veneer Func_080000d0
	.global Func_02001b5c
	.thumb_func
Func_02001b5c:
	overlay_veneer Func_08000100
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Func_02001b6c
	.thumb_func
Func_02001b6c:
	overlay_veneer Func_080003c8
	.global Func_02001b74
	.thumb_func
Func_02001b74:
	overlay_veneer Func_080003d0
	.global Func_02001b7c
	.thumb_func
Func_02001b7c:
	overlay_veneer Object_SetMode
	.global Func_02001b84
	.thumb_func
Func_02001b84:
	overlay_veneer Func_08020170
	.global Func_02001b8c
	.thumb_func
Func_02001b8c:
	overlay_veneer Func_080201f0
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetIndexAndInitObjects
	.thumb_func
Animation_SetIndexAndInitObjects:
	overlay_veneer Animation_SetIndexAndInitObjectsFar
	.global Func_02001ba4
	.thumb_func
Func_02001ba4:
	overlay_veneer Func_08020290
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_02001bb4
	.thumb_func
Func_02001bb4:
	overlay_veneer Func_080ad208
	.global Func_02001bbc
	.thumb_func
Func_02001bbc:
	overlay_veneer Func_080ad2e8
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02001bcc
	.thumb_func
Func_02001bcc:
	overlay_veneer Func_080c8018
	.global Func_02001bd4
	.thumb_func
Func_02001bd4:
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
	.global ObjectMotion_EnableActionAndSetCallback
	.thumb_func
ObjectMotion_EnableActionAndSetCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global Object_RefreshSelectorById
	.thumb_func
Object_RefreshSelectorById:
	overlay_veneer Object_RefreshSelectorByIdFar
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
	.global Func_02001c2c
	.thumb_func
Func_02001c2c:
	overlay_veneer Func_080c80f8
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
	.global Func_02001c5c
	.thumb_func
Func_02001c5c:
	overlay_veneer Func_080c8158
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Func_02001c6c
	.thumb_func
Func_02001c6c:
	overlay_veneer Func_080c8180
	.global Func_02001c74
	.thumb_func
Func_02001c74:
	overlay_veneer Func_080c8188
	.global Func_02001c7c
	.thumb_func
Func_02001c7c:
	overlay_veneer Func_080c8198
	.global Func_02001c84
	.thumb_func
Func_02001c84:
	overlay_veneer Func_080c81a0
	.global Func_02001c8c
	.thumb_func
Func_02001c8c:
	overlay_veneer Func_080c81d0
	.global Func_02001c94
	.thumb_func
Func_02001c94:
	overlay_veneer Func_080c81d8
	.global Func_02001c9c
	.thumb_func
Func_02001c9c:
	overlay_veneer Func_080c8200
	.global Func_02001ca4
	.thumb_func
Func_02001ca4:
	overlay_veneer Func_080c8210
	.global Func_02001cac
	.thumb_func
Func_02001cac:
	overlay_veneer Func_080c8218
	.global Func_02001cb4
	.thumb_func
Func_02001cb4:
	overlay_veneer Func_080c8228
	.global Func_02001cbc
	.thumb_func
Func_02001cbc:
	overlay_veneer Func_080c8238
	.global Func_02001cc4
	.thumb_func
Func_02001cc4:
	overlay_veneer Func_080c8240
	.global Func_02001ccc
	.thumb_func
Func_02001ccc:
	overlay_veneer Func_080c8268
	.global Func_02001cd4
	.thumb_func
Func_02001cd4:
	overlay_veneer Func_080c8278
	.global Event_ClearStatus1c6
	.thumb_func
Event_ClearStatus1c6:
	overlay_veneer Event_ClearStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_02001cec
	.thumb_func
Func_02001cec:
	overlay_veneer Func_080c8420
	.global Func_02001cf4
	.thumb_func
Func_02001cf4:
	overlay_veneer Func_080c8428
	.global Func_02001cfc
	.thumb_func
Func_02001cfc:
	overlay_veneer Func_080c8480
	.global Func_02001d04
	.thumb_func
Func_02001d04:
	overlay_veneer Func_080c84e0
	.global Func_02001d0c
	.thumb_func
Func_02001d0c:
	overlay_veneer Func_080c85e8
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02001d24
	.thumb_func
Func_02001d24:
	overlay_veneer Func_080c86a8
	.global Func_02001d2c
	.thumb_func
Func_02001d2c:
	overlay_veneer Func_080c86b8
	.global Func_02001d34
	.thumb_func
Func_02001d34:
	overlay_veneer Func_080c86c0
	.global Func_02001d3c
	.thumb_func
Func_02001d3c:
	overlay_veneer Func_080c86e8
	.global Func_02001d44
	.thumb_func
Func_02001d44:
	overlay_veneer Func_080c8760
	.global Func_02001d4c
	.thumb_func
Func_02001d4c:
	overlay_veneer Func_080c8780
	.global Func_02001d54
	.thumb_func
Func_02001d54:
	overlay_veneer Func_080c87e8
	.global Func_02001d5c
	.thumb_func
Func_02001d5c:
	overlay_veneer Func_080c8838
	.global Func_02001d64
	.thumb_func
Func_02001d64:
	overlay_veneer Func_080c8848
	.global Func_02001d6c
	.thumb_func
Func_02001d6c:
	overlay_veneer Func_080c8878
	.global Func_02001d74
	.thumb_func
Func_02001d74:
	overlay_veneer Func_080c88e0
	.global Func_02001d7c
	.thumb_func
Func_02001d7c:
	overlay_veneer Audio_PlayCue
