@ Import veneers after the OVERLAY_663 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_663 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer IwramSignedDivide
	.global Engine_MathModulo
	.thumb_func
Engine_MathModulo:
	overlay_veneer IwramUnsignedRemainder
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
	.global Runtime_BumpAllocate
	.thumb_func
Runtime_BumpAllocate:
	overlay_veneer Runtime_BumpAllocateFar
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_02003af0
	.thumb_func
Func_02003af0:
	overlay_veneer Func_080001a8
	.global Func_02003af8
	.thumb_func
Func_02003af8:
	overlay_veneer Func_080001d8
	.global Func_02003b00
	.thumb_func
Func_02003b00:
	overlay_veneer Func_080001f0
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
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
	.global Func_02003b28
	.thumb_func
Func_02003b28:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_02003b38
	.thumb_func
Func_02003b38:
	overlay_veneer Object_SetCallback
	.global Func_02003b40
	.thumb_func
Func_02003b40:
	overlay_veneer Func_080200c0
	.global Func_02003b48
	.thumb_func
Func_02003b48:
	overlay_veneer Func_080200c8
	.global Func_02003b50
	.thumb_func
Func_02003b50:
	overlay_veneer Func_08020120
	.global Func_02003b58
	.thumb_func
Func_02003b58:
	overlay_veneer Object_SetPosition
	.global Func_02003b60
	.thumb_func
Func_02003b60:
	overlay_veneer Object_CommitPosition
	.global Func_02003b68
	.thumb_func
Func_02003b68:
	overlay_veneer Func_08020178
	.global Func_02003b70
	.thumb_func
Func_02003b70:
	overlay_veneer Func_08020198
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02003b80
	.thumb_func
Func_02003b80:
	overlay_veneer Func_080201e8
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02003b98
	.thumb_func
Func_02003b98:
	overlay_veneer Func_08020228
	.global Func_02003ba0
	.thumb_func
Func_02003ba0:
	overlay_veneer Func_08020230
	.global Func_02003ba8
	.thumb_func
Func_02003ba8:
	overlay_veneer Func_080202f8
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_02003bb8
	.thumb_func
Func_02003bb8:
	overlay_veneer Func_08038348
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02003bc8
	.thumb_func
Func_02003bc8:
	overlay_veneer Func_080c8018
	.global Func_02003bd0
	.thumb_func
Func_02003bd0:
	overlay_veneer Func_080c8020
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
	.global Func_02003bf8
	.thumb_func
Func_02003bf8:
	overlay_veneer Func_080c80f8
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Object_AttachWorkTargetToObject
	.thumb_func
Object_AttachWorkTargetToObject:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Func_02003c20
	.thumb_func
Func_02003c20:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02003c30
	.thumb_func
Func_02003c30:
	overlay_veneer Func_080c8240
	.global Func_02003c38
	.thumb_func
Func_02003c38:
	overlay_veneer Func_080c8248
	.global Func_02003c40
	.thumb_func
Func_02003c40:
	overlay_veneer Func_080c8258
	.global Func_02003c48
	.thumb_func
Func_02003c48:
	overlay_veneer Func_080c8278
	.global Func_02003c50
	.thumb_func
Func_02003c50:
	overlay_veneer Func_080c8378
	.global Func_02003c58
	.thumb_func
Func_02003c58:
	overlay_veneer Func_080c8380
	.global Func_02003c60
	.thumb_func
Func_02003c60:
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
	.global Func_02003c80
	.thumb_func
Func_02003c80:
	overlay_veneer Func_080c8480
	.global Func_02003c88
	.thumb_func
Func_02003c88:
	overlay_veneer Func_080c84e0
	.global Func_02003c90
	.thumb_func
Func_02003c90:
	overlay_veneer Func_080c8518
	.global Func_02003c98
	.thumb_func
Func_02003c98:
	overlay_veneer Func_080c8690
	.global Func_02003ca0
	.thumb_func
Func_02003ca0:
	overlay_veneer Func_080c86f8
	.global Func_02003ca8
	.thumb_func
Func_02003ca8:
	overlay_veneer Func_080c8708
	.global Func_02003cb0
	.thumb_func
Func_02003cb0:
	overlay_veneer Func_080c8718
	.global Func_02003cb8
	.thumb_func
Func_02003cb8:
	overlay_veneer Func_080c8720
	.global Func_02003cc0
	.thumb_func
Func_02003cc0:
	overlay_veneer Func_080c8728
	.global Func_02003cc8
	.thumb_func
Func_02003cc8:
	overlay_veneer Func_080c8778
	.global Func_02003cd0
	.thumb_func
Func_02003cd0:
	overlay_veneer Func_080c88c8
	.global Func_02003cd8
	.thumb_func
Func_02003cd8:
	overlay_veneer Audio_PlayCue
