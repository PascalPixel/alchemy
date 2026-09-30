@ Import veneers after the OVERLAY_656 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_656 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Func_02002bb8
	.thumb_func
Func_02002bb8:
	overlay_veneer Func_080000d0
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	overlay_veneer Func_080000d8
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global Func_02002bd8
	.thumb_func
Func_02002bd8:
	overlay_veneer Func_080003c8
	.global Func_02002be0
	.thumb_func
Func_02002be0:
	overlay_veneer Func_080003d0
	.global Func_02002be8
	.thumb_func
Func_02002be8:
	overlay_veneer Func_080003d8
	.global Func_02002bf0
	.thumb_func
Func_02002bf0:
	overlay_veneer Object_SetMode
	.global Func_02002bf8
	.thumb_func
Func_02002bf8:
	overlay_veneer Object_SetCallback
	.global Func_02002c00
	.thumb_func
Func_02002c00:
	overlay_veneer Func_080200c0
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02002c10
	.thumb_func
Func_02002c10:
	overlay_veneer Func_08020120
	.global Func_02002c18
	.thumb_func
Func_02002c18:
	overlay_veneer Object_SetPosition
	.global Func_02002c20
	.thumb_func
Func_02002c20:
	overlay_veneer Object_CommitPosition
	.global Func_02002c28
	.thumb_func
Func_02002c28:
	overlay_veneer Func_08020170
	.global Func_02002c30
	.thumb_func
Func_02002c30:
	overlay_veneer Func_08020178
	.global Func_02002c38
	.thumb_func
Func_02002c38:
	overlay_veneer Func_080201e8
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_02002c48
	.thumb_func
Func_02002c48:
	overlay_veneer Func_08020228
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global PartyInventory_Remove
	.thumb_func
PartyInventory_Remove:
	overlay_veneer PartyInventory_RemoveFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02002c68
	.thumb_func
Func_02002c68:
	overlay_veneer Func_080c8018
	.global Func_02002c70
	.thumb_func
Func_02002c70:
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
	.global Func_02002cd0
	.thumb_func
Func_02002cd0:
	overlay_veneer Func_080c80f8
	.global Func_02002cd8
	.thumb_func
Func_02002cd8:
	overlay_veneer Func_080c8110
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
	.global Func_02002d08
	.thumb_func
Func_02002d08:
	overlay_veneer Func_080c8158
	.global Func_02002d10
	.thumb_func
Func_02002d10:
	overlay_veneer Func_080c8170
	.global Func_02002d18
	.thumb_func
Func_02002d18:
	overlay_veneer Func_080c8180
	.global Func_02002d20
	.thumb_func
Func_02002d20:
	overlay_veneer Func_080c81a0
	.global Func_02002d28
	.thumb_func
Func_02002d28:
	overlay_veneer Func_080c81d0
	.global Func_02002d30
	.thumb_func
Func_02002d30:
	overlay_veneer Func_080c81d8
	.global Func_02002d38
	.thumb_func
Func_02002d38:
	overlay_veneer Func_080c8200
	.global Func_02002d40
	.thumb_func
Func_02002d40:
	overlay_veneer Func_080c8210
	.global Func_02002d48
	.thumb_func
Func_02002d48:
	overlay_veneer Func_080c8218
	.global Func_02002d50
	.thumb_func
Func_02002d50:
	overlay_veneer Func_080c8228
	.global Func_02002d58
	.thumb_func
Func_02002d58:
	overlay_veneer Func_080c8230
	.global Func_02002d60
	.thumb_func
Func_02002d60:
	overlay_veneer Func_080c8238
	.global Func_02002d68
	.thumb_func
Func_02002d68:
	overlay_veneer Func_080c8260
	.global Func_02002d70
	.thumb_func
Func_02002d70:
	overlay_veneer Func_080c8278
	.global Func_02002d78
	.thumb_func
Func_02002d78:
	overlay_veneer Func_080c8280
	.global Func_02002d80
	.thumb_func
Func_02002d80:
	overlay_veneer Func_080c8290
	.global Func_02002d88
	.thumb_func
Func_02002d88:
	overlay_veneer Func_080c8378
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
	.global Func_02002da8
	.thumb_func
Func_02002da8:
	overlay_veneer Func_080c84e0
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02002db8
	.thumb_func
Func_02002db8:
	overlay_veneer Func_080c86f8
	.global Func_02002dc0
	.thumb_func
Func_02002dc0:
	overlay_veneer Func_080c8708
	.global Func_02002dc8
	.thumb_func
Func_02002dc8:
	overlay_veneer Func_080c8718
	.global Func_02002dd0
	.thumb_func
Func_02002dd0:
	overlay_veneer Func_080c8720
	.global Func_02002dd8
	.thumb_func
Func_02002dd8:
	overlay_veneer Func_080c8728
	.global Func_02002de0
	.thumb_func
Func_02002de0:
	overlay_veneer Audio_PlayCue
