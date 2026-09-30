@ Import veneers after the OVERLAY_6A4 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6A4 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer IwramSignedDivide
	.global Engine_MathRemainder
	.thumb_func
Engine_MathRemainder:
	overlay_veneer IwramSignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Func_02002958
	.thumb_func
Func_02002958:
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
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	overlay_veneer Func_08000140
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global Runtime_BumpAllocate
	.thumb_func
Runtime_BumpAllocate:
	overlay_veneer Runtime_BumpAllocateFar
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_020029a0
	.thumb_func
Func_020029a0:
	overlay_veneer Func_080001a8
	.global Func_020029a8
	.thumb_func
Func_020029a8:
	overlay_veneer Func_080001b8
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_020029c0
	.thumb_func
Func_020029c0:
	overlay_veneer Func_080001e8
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global Func_020029d0
	.thumb_func
Func_020029d0:
	overlay_veneer Func_080003c8
	.global Func_020029d8
	.thumb_func
Func_020029d8:
	overlay_veneer Func_080003d0
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global Func_020029f0
	.thumb_func
Func_020029f0:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_02002a00
	.thumb_func
Func_02002a00:
	overlay_veneer Object_SetCallback
	.global Func_02002a08
	.thumb_func
Func_02002a08:
	overlay_veneer Func_080200c0
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02002a18
	.thumb_func
Func_02002a18:
	overlay_veneer Func_08020120
	.global Func_02002a20
	.thumb_func
Func_02002a20:
	overlay_veneer Object_ResetMotion
	.global Func_02002a28
	.thumb_func
Func_02002a28:
	overlay_veneer Object_SetPosition
	.global Func_02002a30
	.thumb_func
Func_02002a30:
	overlay_veneer Object_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02002a40
	.thumb_func
Func_02002a40:
	overlay_veneer Func_080201e0
	.global Func_02002a48
	.thumb_func
Func_02002a48:
	overlay_veneer Func_080201e8
	.global Func_02002a50
	.thumb_func
Func_02002a50:
	overlay_veneer Func_080201f0
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_02002a60
	.thumb_func
Func_02002a60:
	overlay_veneer Func_08020228
	.global Func_02002a68
	.thumb_func
Func_02002a68:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02002a78
	.thumb_func
Func_02002a78:
	overlay_veneer Func_08020360
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02002a88
	.thumb_func
Func_02002a88:
	overlay_veneer Func_080c8018
	.global Func_02002a90
	.thumb_func
Func_02002a90:
	overlay_veneer Func_080c8020
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_EnableActionAndResetMotion
	.thumb_func
ObjectMotion_EnableActionAndResetMotion:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global Func_02002ab8
	.thumb_func
Func_02002ab8:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_02002ac8
	.thumb_func
Func_02002ac8:
	overlay_veneer Func_080c8170
	.global Func_02002ad0
	.thumb_func
Func_02002ad0:
	overlay_veneer Func_080c81d0
	.global Func_02002ad8
	.thumb_func
Func_02002ad8:
	overlay_veneer Func_080c8218
	.global Func_02002ae0
	.thumb_func
Func_02002ae0:
	overlay_veneer Func_080c8228
	.global Func_02002ae8
	.thumb_func
Func_02002ae8:
	overlay_veneer Func_080c8238
	.global Func_02002af0
	.thumb_func
Func_02002af0:
	overlay_veneer Func_080c8240
	.global Func_02002af8
	.thumb_func
Func_02002af8:
	overlay_veneer Func_080c8258
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
	.global Func_02002b18
	.thumb_func
Func_02002b18:
	overlay_veneer Func_080c84e0
	.global Func_02002b20
	.thumb_func
Func_02002b20:
	overlay_veneer Func_080c85c0
	.global Func_02002b28
	.thumb_func
Func_02002b28:
	overlay_veneer Func_080c8780
	.global Func_02002b30
	.thumb_func
Func_02002b30:
	overlay_veneer Func_080c87c0
	.global Func_02002b38
	.thumb_func
Func_02002b38:
	overlay_veneer Func_080c87c8
	.global Func_02002b40
	.thumb_func
Func_02002b40:
	overlay_veneer Func_080c87d0
	.global Func_02002b48
	.thumb_func
Func_02002b48:
	overlay_veneer Func_080c87e8
	.global Func_02002b50
	.thumb_func
Func_02002b50:
	overlay_veneer Func_080c8830
	.global Func_02002b58
	.thumb_func
Func_02002b58:
	overlay_veneer Func_080c8848
	.global Func_02002b60
	.thumb_func
Func_02002b60:
	overlay_veneer Func_08108078
	.global Func_02002b68
	.thumb_func
Func_02002b68:
	overlay_veneer Audio_PlayCue
