@ Import veneers after the OVERLAY_657 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_657 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Func_02001ea4
	.thumb_func
Func_02001ea4:
	overlay_veneer Func_080000d0
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	overlay_veneer Func_080000d8
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	overlay_veneer Func_08000140
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Func_02001ee4
	.thumb_func
Func_02001ee4:
	overlay_veneer Func_080003c8
	.global Func_02001eec
	.thumb_func
Func_02001eec:
	overlay_veneer Func_080003d0
	.global Func_02001ef4
	.thumb_func
Func_02001ef4:
	overlay_veneer Func_080003d8
	.global Func_02001efc
	.thumb_func
Func_02001efc:
	overlay_veneer Object_SetMode
	.global Func_02001f04
	.thumb_func
Func_02001f04:
	overlay_veneer Object_SetCallback
	.global Func_02001f0c
	.thumb_func
Func_02001f0c:
	overlay_veneer Func_080200c0
	.global Func_02001f14
	.thumb_func
Func_02001f14:
	overlay_veneer Func_080200c8
	.global Func_02001f1c
	.thumb_func
Func_02001f1c:
	overlay_veneer Func_08020120
	.global Func_02001f24
	.thumb_func
Func_02001f24:
	overlay_veneer Object_SetPosition
	.global Func_02001f2c
	.thumb_func
Func_02001f2c:
	overlay_veneer Func_08020178
	.global Func_02001f34
	.thumb_func
Func_02001f34:
	overlay_veneer Func_08020198
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_02001f4c
	.thumb_func
Func_02001f4c:
	overlay_veneer Func_08038248
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02001f5c
	.thumb_func
Func_02001f5c:
	overlay_veneer Func_080c8018
	.global Func_02001f64
	.thumb_func
Func_02001f64:
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
	.global ObjectMotion_EnableActionAndResetMotion
	.thumb_func
ObjectMotion_EnableActionAndResetMotion:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global Object_SetActionCallbackAndRefreshById
	.thumb_func
Object_SetActionCallbackAndRefreshById:
	overlay_veneer Object_SetActionCallbackAndRefreshByIdFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_SnapHeadingAndOffset
	.thumb_func
ObjectMotion_SnapHeadingAndOffset:
	overlay_veneer ObjectMotion_SnapHeadingAndOffsetFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02001fc4
	.thumb_func
Func_02001fc4:
	overlay_veneer Func_080c80f8
	.global Func_02001fcc
	.thumb_func
Func_02001fcc:
	overlay_veneer Func_080c8100
	.global Func_02001fd4
	.thumb_func
Func_02001fd4:
	overlay_veneer Func_080c8110
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_WaitForAnimationChange
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	overlay_veneer ObjectMotion_WaitForAnimationChangeFar
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
	.global Func_0200200c
	.thumb_func
Func_0200200c:
	overlay_veneer Func_080c8158
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Func_0200201c
	.thumb_func
Func_0200201c:
	overlay_veneer Func_080c8180
	.global Func_02002024
	.thumb_func
Func_02002024:
	overlay_veneer Func_080c8188
	.global Func_0200202c
	.thumb_func
Func_0200202c:
	overlay_veneer Func_080c8198
	.global Func_02002034
	.thumb_func
Func_02002034:
	overlay_veneer Func_080c81a0
	.global Func_0200203c
	.thumb_func
Func_0200203c:
	overlay_veneer Func_080c81d0
	.global Func_02002044
	.thumb_func
Func_02002044:
	overlay_veneer Func_080c81d8
	.global Func_0200204c
	.thumb_func
Func_0200204c:
	overlay_veneer Func_080c8210
	.global Func_02002054
	.thumb_func
Func_02002054:
	overlay_veneer Func_080c8218
	.global Func_0200205c
	.thumb_func
Func_0200205c:
	overlay_veneer Func_080c8230
	.global Func_02002064
	.thumb_func
Func_02002064:
	overlay_veneer Func_080c8238
	.global Func_0200206c
	.thumb_func
Func_0200206c:
	overlay_veneer Func_080c8240
	.global Func_02002074
	.thumb_func
Func_02002074:
	overlay_veneer Func_080c8258
	.global Func_0200207c
	.thumb_func
Func_0200207c:
	overlay_veneer Func_080c8278
	.global Func_02002084
	.thumb_func
Func_02002084:
	overlay_veneer Func_080c8378
	.global Func_0200208c
	.thumb_func
Func_0200208c:
	overlay_veneer Func_080c8390
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
	.global Func_020020ac
	.thumb_func
Func_020020ac:
	overlay_veneer Func_080c84e0
	.global Func_020020b4
	.thumb_func
Func_020020b4:
	overlay_veneer Func_080c85e8
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Func_020020c4
	.thumb_func
Func_020020c4:
	overlay_veneer Func_080c86f8
	.global Func_020020cc
	.thumb_func
Func_020020cc:
	overlay_veneer Func_080c8708
	.global Func_020020d4
	.thumb_func
Func_020020d4:
	overlay_veneer Audio_PlayCue
