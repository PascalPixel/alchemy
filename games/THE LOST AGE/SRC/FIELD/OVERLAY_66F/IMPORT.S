@ Import veneers after the OVERLAY_66F overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_66F overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
	.global Func_0200102c
	.thumb_func
Func_0200102c:
	overlay_veneer Func_080003c8
	.global Func_02001034
	.thumb_func
Func_02001034:
	overlay_veneer Func_080003d0
	.global Func_0200103c
	.thumb_func
Func_0200103c:
	overlay_veneer Object_SetMode
	.global Func_02001044
	.thumb_func
Func_02001044:
	overlay_veneer Object_SetCallback
	.global Func_0200104c
	.thumb_func
Func_0200104c:
	overlay_veneer Object_ResetMotion
	.global Func_02001054
	.thumb_func
Func_02001054:
	overlay_veneer Object_SetPosition
	.global Func_0200105c
	.thumb_func
Func_0200105c:
	overlay_veneer Object_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_0200106c
	.thumb_func
Func_0200106c:
	overlay_veneer Func_080201e8
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02001084
	.thumb_func
Func_02001084:
	overlay_veneer Func_080c8018
	.global Func_0200108c
	.thumb_func
Func_0200108c:
	overlay_veneer Func_080c8020
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
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
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_020010d4
	.thumb_func
Func_020010d4:
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
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Func_020010fc
	.thumb_func
Func_020010fc:
	overlay_veneer Func_080c8158
	.global Func_02001104
	.thumb_func
Func_02001104:
	overlay_veneer Func_080c8180
	.global Func_0200110c
	.thumb_func
Func_0200110c:
	overlay_veneer Func_080c81a0
	.global Func_02001114
	.thumb_func
Func_02001114:
	overlay_veneer Func_080c8200
	.global Func_0200111c
	.thumb_func
Func_0200111c:
	overlay_veneer Func_080c8210
	.global Func_02001124
	.thumb_func
Func_02001124:
	overlay_veneer Func_080c8218
	.global Func_0200112c
	.thumb_func
Func_0200112c:
	overlay_veneer Func_080c8238
	.global Func_02001134
	.thumb_func
Func_02001134:
	overlay_veneer Func_080c8240
	.global Func_0200113c
	.thumb_func
Func_0200113c:
	overlay_veneer Func_080c8268
	.global Func_02001144
	.thumb_func
Func_02001144:
	overlay_veneer Func_080c8288
	.global Func_0200114c
	.thumb_func
Func_0200114c:
	overlay_veneer Func_080c82e0
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_02001164
	.thumb_func
Func_02001164:
	overlay_veneer Func_080c84e0
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02001174
	.thumb_func
Func_02001174:
	overlay_veneer Func_080c86a8
	.global Func_0200117c
	.thumb_func
Func_0200117c:
	overlay_veneer Func_080c86e8
	.global Func_02001184
	.thumb_func
Func_02001184:
	overlay_veneer Func_080c8778
	.global Func_0200118c
	.thumb_func
Func_0200118c:
	overlay_veneer Func_080c8860
	.global ObjectMotion_ResetTargetsAndVelocity
	.thumb_func
ObjectMotion_ResetTargetsAndVelocity:
	overlay_veneer ObjectMotion_ResetTargetsAndVelocityFar
	.global Func_0200119c
	.thumb_func
Func_0200119c:
	overlay_veneer Audio_PlayCue
