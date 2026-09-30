@ Import veneers after the OVERLAY_6A0 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6A0 overlay import veneers
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
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
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
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global Runtime_BumpAllocate
	.thumb_func
Runtime_BumpAllocate:
	overlay_veneer Runtime_BumpAllocateFar
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_02002a8c
	.thumb_func
Func_02002a8c:
	overlay_veneer Func_080001a8
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02002aa4
	.thumb_func
Func_02002aa4:
	overlay_veneer Func_080001e8
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
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global Func_02002ad4
	.thumb_func
Func_02002ad4:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_02002ae4
	.thumb_func
Func_02002ae4:
	overlay_veneer Object_SetCallback
	.global Func_02002aec
	.thumb_func
Func_02002aec:
	overlay_veneer Func_080200c0
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02002afc
	.thumb_func
Func_02002afc:
	overlay_veneer Func_08020120
	.global Func_02002b04
	.thumb_func
Func_02002b04:
	overlay_veneer Object_SetPosition
	.global Func_02002b0c
	.thumb_func
Func_02002b0c:
	overlay_veneer Object_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02002b1c
	.thumb_func
Func_02002b1c:
	overlay_veneer Func_080201e0
	.global Func_02002b24
	.thumb_func
Func_02002b24:
	overlay_veneer Func_080201e8
	.global Func_02002b2c
	.thumb_func
Func_02002b2c:
	overlay_veneer Func_080201f0
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_02002b3c
	.thumb_func
Func_02002b3c:
	overlay_veneer Func_08020228
	.global Func_02002b44
	.thumb_func
Func_02002b44:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02002b54
	.thumb_func
Func_02002b54:
	overlay_veneer Func_08020360
	.global Func_02002b5c
	.thumb_func
Func_02002b5c:
	overlay_veneer Func_08020368
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02002b6c
	.thumb_func
Func_02002b6c:
	overlay_veneer Func_080c8018
	.global Func_02002b74
	.thumb_func
Func_02002b74:
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
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global Func_02002ba4
	.thumb_func
Func_02002ba4:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02002bbc
	.thumb_func
Func_02002bbc:
	overlay_veneer Func_080c8218
	.global Object_AttachWorkTargetToObject
	.thumb_func
Object_AttachWorkTargetToObject:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02002bd4
	.thumb_func
Func_02002bd4:
	overlay_veneer Func_080c8240
	.global Func_02002bdc
	.thumb_func
Func_02002bdc:
	overlay_veneer Func_080c8258
	.global Func_02002be4
	.thumb_func
Func_02002be4:
	overlay_veneer Func_080c8288
	.global Func_02002bec
	.thumb_func
Func_02002bec:
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
	.global Func_02002c0c
	.thumb_func
Func_02002c0c:
	overlay_veneer Func_080c84e0
	.global Func_02002c14
	.thumb_func
Func_02002c14:
	overlay_veneer Func_080c85c0
	.global Func_02002c1c
	.thumb_func
Func_02002c1c:
	overlay_veneer Func_080c8710
	.global Func_02002c24
	.thumb_func
Func_02002c24:
	overlay_veneer Func_080c8748
	.global Func_02002c2c
	.thumb_func
Func_02002c2c:
	overlay_veneer Func_080c87e8
	.global Func_02002c34
	.thumb_func
Func_02002c34:
	overlay_veneer Func_080c8830
	.global Func_02002c3c
	.thumb_func
Func_02002c3c:
	overlay_veneer Func_080c8840
	.global Func_02002c44
	.thumb_func
Func_02002c44:
	overlay_veneer Func_080c8848
	.global Func_02002c4c
	.thumb_func
Func_02002c4c:
	overlay_veneer Func_080c8850
	.global Func_02002c54
	.thumb_func
Func_02002c54:
	overlay_veneer Func_080c8858
	.global Func_02002c5c
	.thumb_func
Func_02002c5c:
	overlay_veneer Audio_PlayCue
