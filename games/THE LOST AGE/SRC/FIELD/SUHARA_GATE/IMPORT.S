@ Import veneers after the SUHARA_GATE overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — SUHARA_GATE overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Func_02000960
	.thumb_func
Func_02000960:
	overlay_veneer Func_080003c8
	.global Func_02000968
	.thumb_func
Func_02000968:
	overlay_veneer Func_080003d0
	.global Func_02000970
	.thumb_func
Func_02000970:
	overlay_veneer Func_08020178
	.global Func_02000978
	.thumb_func
Func_02000978:
	overlay_veneer Func_080201e8
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02000988
	.thumb_func
Func_02000988:
	overlay_veneer Func_080c8018
	.global Func_02000990
	.thumb_func
Func_02000990:
	overlay_veneer Func_080c8020
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
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
	.global ObjectMotion_SnapHeadingAndOffset
	.thumb_func
ObjectMotion_SnapHeadingAndOffset:
	overlay_veneer ObjectMotion_SnapHeadingAndOffsetFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_020009d0
	.thumb_func
Func_020009d0:
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
	.global Func_020009f8
	.thumb_func
Func_020009f8:
	overlay_veneer Func_080c8158
	.global Func_02000a00
	.thumb_func
Func_02000a00:
	overlay_veneer Func_080c8180
	.global Func_02000a08
	.thumb_func
Func_02000a08:
	overlay_veneer Func_080c8198
	.global Func_02000a10
	.thumb_func
Func_02000a10:
	overlay_veneer Func_080c81d0
	.global Func_02000a18
	.thumb_func
Func_02000a18:
	overlay_veneer Func_080c8210
	.global Func_02000a20
	.thumb_func
Func_02000a20:
	overlay_veneer Func_080c8228
	.global Func_02000a28
	.thumb_func
Func_02000a28:
	overlay_veneer Func_080c8238
	.global Func_02000a30
	.thumb_func
Func_02000a30:
	overlay_veneer Func_080c8240
	.global Func_02000a38
	.thumb_func
Func_02000a38:
	overlay_veneer Func_080c8278
	.global Func_02000a40
	.thumb_func
Func_02000a40:
	overlay_veneer Func_080c8280
	.global Func_02000a48
	.thumb_func
Func_02000a48:
	overlay_veneer Func_080c8288
	.global Func_02000a50
	.thumb_func
Func_02000a50:
	overlay_veneer Func_080c8290
	.global Func_02000a58
	.thumb_func
Func_02000a58:
	overlay_veneer Func_080c8298
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_02000a70
	.thumb_func
Func_02000a70:
	overlay_veneer Func_080c84e0
	.global Func_02000a78
	.thumb_func
Func_02000a78:
	overlay_veneer Func_080c85e8
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Object_SetActionCallback
	.thumb_func
Object_SetActionCallback:
	overlay_veneer Object_SetActionCallbackFar
	.global Func_02000a90
	.thumb_func
Func_02000a90:
	overlay_veneer Func_080c87b0
	.global Func_02000a98
	.thumb_func
Func_02000a98:
	overlay_veneer Func_080c87b8
	.global Func_02000aa0
	.thumb_func
Func_02000aa0:
	overlay_veneer Audio_PlayCue
