@ Import veneers after the OVERLAY_675 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_675 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Func_020011e4
	.thumb_func
Func_020011e4:
	overlay_veneer Func_080000d0
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	overlay_veneer Func_080000d8
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
	.global Func_020011fc
	.thumb_func
Func_020011fc:
	overlay_veneer Func_080003c8
	.global Func_02001204
	.thumb_func
Func_02001204:
	overlay_veneer Func_080003d0
	.global Func_0200120c
	.thumb_func
Func_0200120c:
	overlay_veneer Func_080003d8
	.global Func_02001214
	.thumb_func
Func_02001214:
	overlay_veneer Object_SetMode
	.global Func_0200121c
	.thumb_func
Func_0200121c:
	overlay_veneer Object_SetPosition
	.global Func_02001224
	.thumb_func
Func_02001224:
	overlay_veneer Object_CommitPosition
	.global Func_0200122c
	.thumb_func
Func_0200122c:
	overlay_veneer Func_08020170
	.global Func_02001234
	.thumb_func
Func_02001234:
	overlay_veneer Func_080201e8
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_02001244
	.thumb_func
Func_02001244:
	overlay_veneer Func_08038210
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02001254
	.thumb_func
Func_02001254:
	overlay_veneer Func_080c8018
	.global Func_0200125c
	.thumb_func
Func_0200125c:
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
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global Func_0200129c
	.thumb_func
Func_0200129c:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Func_020012bc
	.thumb_func
Func_020012bc:
	overlay_veneer Func_080c8168
	.global Func_020012c4
	.thumb_func
Func_020012c4:
	overlay_veneer Func_080c8200
	.global Func_020012cc
	.thumb_func
Func_020012cc:
	overlay_veneer Func_080c8278
	.global Func_020012d4
	.thumb_func
Func_020012d4:
	overlay_veneer Func_080c8280
	.global Func_020012dc
	.thumb_func
Func_020012dc:
	overlay_veneer Func_080c8288
	.global Func_020012e4
	.thumb_func
Func_020012e4:
	overlay_veneer Func_080c8290
	.global Func_020012ec
	.thumb_func
Func_020012ec:
	overlay_veneer Func_080c8378
	.global Func_020012f4
	.thumb_func
Func_020012f4:
	overlay_veneer Func_080c8380
	.global Func_020012fc
	.thumb_func
Func_020012fc:
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
	.global Func_0200131c
	.thumb_func
Func_0200131c:
	overlay_veneer Func_080c8480
	.global Func_02001324
	.thumb_func
Func_02001324:
	overlay_veneer Func_080c84e0
	.global Func_0200132c
	.thumb_func
Func_0200132c:
	overlay_veneer Func_080c88d8
	.global Func_02001334
	.thumb_func
Func_02001334:
	overlay_veneer Func_080c88e8
	.global Func_0200133c
	.thumb_func
Func_0200133c:
	overlay_veneer Func_080c8968
	.global Func_02001344
	.thumb_func
Func_02001344:
	overlay_veneer Audio_PlayCue
