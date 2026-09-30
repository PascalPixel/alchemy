@ Import veneers after the OVERLAY_654 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_654 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer IwramSignedDivide
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
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
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
	.global Func_02003968
	.thumb_func
Func_02003968:
	overlay_veneer Object_SetMode
	.global Func_02003970
	.thumb_func
Func_02003970:
	overlay_veneer Object_SetCallback
	.global Func_02003978
	.thumb_func
Func_02003978:
	overlay_veneer Func_080200c0
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02003988
	.thumb_func
Func_02003988:
	overlay_veneer Func_08020120
	.global Func_02003990
	.thumb_func
Func_02003990:
	overlay_veneer Object_SetPosition
	.global Func_02003998
	.thumb_func
Func_02003998:
	overlay_veneer Func_08020178
	.global Func_020039a0
	.thumb_func
Func_020039a0:
	overlay_veneer Func_080201b0
	.global Func_020039a8
	.thumb_func
Func_020039a8:
	overlay_veneer Func_080201e8
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_020039b8
	.thumb_func
Func_020039b8:
	overlay_veneer Func_08020228
	.global Func_020039c0
	.thumb_func
Func_020039c0:
	overlay_veneer Func_08020230
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_020039d8
	.thumb_func
Func_020039d8:
	overlay_veneer Func_080c8018
	.global Func_020039e0
	.thumb_func
Func_020039e0:
	overlay_veneer Func_080c8020
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
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02003a30
	.thumb_func
Func_02003a30:
	overlay_veneer Func_080c80f8
	.global Func_02003a38
	.thumb_func
Func_02003a38:
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
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02003a70
	.thumb_func
Func_02003a70:
	overlay_veneer Func_080c8180
	.global Func_02003a78
	.thumb_func
Func_02003a78:
	overlay_veneer Func_080c8198
	.global Func_02003a80
	.thumb_func
Func_02003a80:
	overlay_veneer Func_080c81a0
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02003a90
	.thumb_func
Func_02003a90:
	overlay_veneer Func_080c81d8
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02003aa0
	.thumb_func
Func_02003aa0:
	overlay_veneer Func_080c8208
	.global Func_02003aa8
	.thumb_func
Func_02003aa8:
	overlay_veneer Func_080c8210
	.global Func_02003ab0
	.thumb_func
Func_02003ab0:
	overlay_veneer Func_080c8218
	.global Func_02003ab8
	.thumb_func
Func_02003ab8:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02003ac8
	.thumb_func
Func_02003ac8:
	overlay_veneer Func_080c8240
	.global Func_02003ad0
	.thumb_func
Func_02003ad0:
	overlay_veneer Func_080c8258
	.global Func_02003ad8
	.thumb_func
Func_02003ad8:
	overlay_veneer Func_080c8278
	.global Func_02003ae0
	.thumb_func
Func_02003ae0:
	overlay_veneer Func_080c8288
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
	.global Func_02003b00
	.thumb_func
Func_02003b00:
	overlay_veneer Func_080c8480
	.global Func_02003b08
	.thumb_func
Func_02003b08:
	overlay_veneer Func_080c84e0
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02003b18
	.thumb_func
Func_02003b18:
	overlay_veneer Func_080c86a8
	.global Func_02003b20
	.thumb_func
Func_02003b20:
	overlay_veneer Func_080c86e8
	.global Func_02003b28
	.thumb_func
Func_02003b28:
	overlay_veneer Func_080c8710
	.global Func_02003b30
	.thumb_func
Func_02003b30:
	overlay_veneer Func_080c8718
	.global Func_02003b38
	.thumb_func
Func_02003b38:
	overlay_veneer Func_080c8720
	.global Func_02003b40
	.thumb_func
Func_02003b40:
	overlay_veneer Func_080c8728
	.global Func_02003b48
	.thumb_func
Func_02003b48:
	overlay_veneer Audio_PlayCue
