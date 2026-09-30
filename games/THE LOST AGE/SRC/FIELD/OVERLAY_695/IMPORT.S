@ Import veneers after the OVERLAY_695 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_695 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Func_02001048
	.thumb_func
Func_02001048:
	overlay_veneer Func_080003c8
	.global Func_02001050
	.thumb_func
Func_02001050:
	overlay_veneer Func_080003d0
	.global Func_02001058
	.thumb_func
Func_02001058:
	overlay_veneer Func_080003d8
	.global Func_02001060
	.thumb_func
Func_02001060:
	overlay_veneer Func_080201e8
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global UiWork_FinalizePendingCore
	.thumb_func
UiWork_FinalizePendingCore:
	overlay_veneer UiWork_FinalizePendingCoreFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02001080
	.thumb_func
Func_02001080:
	overlay_veneer Func_080c8018
	.global Func_02001088
	.thumb_func
Func_02001088:
	overlay_veneer Func_080c8020
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Func_02001098
	.thumb_func
Func_02001098:
	overlay_veneer Func_080c8078
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
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_020010e8
	.thumb_func
Func_020010e8:
	overlay_veneer Func_080c80f8
	.global Func_020010f0
	.thumb_func
Func_020010f0:
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
	.global Func_02001120
	.thumb_func
Func_02001120:
	overlay_veneer Func_080c8158
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Func_02001130
	.thumb_func
Func_02001130:
	overlay_veneer Func_080c8170
	.global Func_02001138
	.thumb_func
Func_02001138:
	overlay_veneer Func_080c8180
	.global Func_02001140
	.thumb_func
Func_02001140:
	overlay_veneer Func_080c8188
	.global Func_02001148
	.thumb_func
Func_02001148:
	overlay_veneer Func_080c8198
	.global Func_02001150
	.thumb_func
Func_02001150:
	overlay_veneer Func_080c81d0
	.global Func_02001158
	.thumb_func
Func_02001158:
	overlay_veneer Func_080c81d8
	.global Func_02001160
	.thumb_func
Func_02001160:
	overlay_veneer Func_080c8210
	.global Func_02001168
	.thumb_func
Func_02001168:
	overlay_veneer Func_080c8218
	.global Func_02001170
	.thumb_func
Func_02001170:
	overlay_veneer Func_080c8230
	.global Func_02001178
	.thumb_func
Func_02001178:
	overlay_veneer Func_080c8238
	.global Func_02001180
	.thumb_func
Func_02001180:
	overlay_veneer Func_080c8240
	.global Func_02001188
	.thumb_func
Func_02001188:
	overlay_veneer Func_080c8248
	.global Func_02001190
	.thumb_func
Func_02001190:
	overlay_veneer Func_080c8268
	.global Func_02001198
	.thumb_func
Func_02001198:
	overlay_veneer Func_080c8278
	.global Func_020011a0
	.thumb_func
Func_020011a0:
	overlay_veneer Func_080c8280
	.global Func_020011a8
	.thumb_func
Func_020011a8:
	overlay_veneer Func_080c8288
	.global Func_020011b0
	.thumb_func
Func_020011b0:
	overlay_veneer Func_080c8290
	.global Func_020011b8
	.thumb_func
Func_020011b8:
	overlay_veneer Func_080c8298
	.global Func_020011c0
	.thumb_func
Func_020011c0:
	overlay_veneer Func_080c82e0
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
	.global Func_020011e0
	.thumb_func
Func_020011e0:
	overlay_veneer Func_080c84e0
	.global Func_020011e8
	.thumb_func
Func_020011e8:
	overlay_veneer Func_080c8558
	.global Func_020011f0
	.thumb_func
Func_020011f0:
	overlay_veneer Func_080c85e8
	.global ObjectMotion_OffsetPositionAndReset
	.thumb_func
ObjectMotion_OffsetPositionAndReset:
	overlay_veneer ObjectMotion_OffsetPositionAndResetFar
	.global Func_02001200
	.thumb_func
Func_02001200:
	overlay_veneer Func_080c8860
	.global Func_02001208
	.thumb_func
Func_02001208:
	overlay_veneer Func_080c8940
	.global Func_02001210
	.thumb_func
Func_02001210:
	overlay_veneer Audio_PlayCue
