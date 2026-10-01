@ Import veneers after the OVERLAY_69B overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_69B overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __modsi3
	.thumb_func
__modsi3:
	overlay_veneer IwramSignedRemainder
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
	.global Func_02002174
	.thumb_func
Func_02002174:
	overlay_veneer Func_080001a8
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_0200218c
	.thumb_func
Func_0200218c:
	overlay_veneer Func_080001e8
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global Func_020021b4
	.thumb_func
Func_020021b4:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_020021c4
	.thumb_func
Func_020021c4:
	overlay_veneer Object_SetCallback
	.global Func_020021cc
	.thumb_func
Func_020021cc:
	overlay_veneer Func_080200c0
	.global Func_020021d4
	.thumb_func
Func_020021d4:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_020021e4
	.thumb_func
Func_020021e4:
	overlay_veneer Func_08020120
	.global Func_020021ec
	.thumb_func
Func_020021ec:
	overlay_veneer Object_SetPosition
	.global Func_020021f4
	.thumb_func
Func_020021f4:
	overlay_veneer Object_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02002204
	.thumb_func
Func_02002204:
	overlay_veneer Func_080201e0
	.global Func_0200220c
	.thumb_func
Func_0200220c:
	overlay_veneer Func_080201e8
	.global Func_02002214
	.thumb_func
Func_02002214:
	overlay_veneer Func_080201f0
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_02002224
	.thumb_func
Func_02002224:
	overlay_veneer Func_08020228
	.global Func_0200222c
	.thumb_func
Func_0200222c:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_0200223c
	.thumb_func
Func_0200223c:
	overlay_veneer Func_08020360
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_0200224c
	.thumb_func
Func_0200224c:
	overlay_veneer Func_080c8018
	.global Func_02002254
	.thumb_func
Func_02002254:
	overlay_veneer Func_080c8020
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global Func_0200227c
	.thumb_func
Func_0200227c:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_0200229c
	.thumb_func
Func_0200229c:
	overlay_veneer Func_080c8218
	.global Object_AttachWorkTargetToObject
	.thumb_func
Object_AttachWorkTargetToObject:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_020022b4
	.thumb_func
Func_020022b4:
	overlay_veneer Func_080c8258
	.global Func_020022bc
	.thumb_func
Func_020022bc:
	overlay_veneer Func_080c8278
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
	.global Func_020022dc
	.thumb_func
Func_020022dc:
	overlay_veneer Func_080c84e0
	.global Func_020022e4
	.thumb_func
Func_020022e4:
	overlay_veneer Func_080c85c0
	.global Object_Spawn
	.thumb_func
Object_Spawn:
	overlay_veneer Object_SpawnFar
	.global Func_020022f4
	.thumb_func
Func_020022f4:
	overlay_veneer Func_080c87e8
	.global Func_020022fc
	.thumb_func
Func_020022fc:
	overlay_veneer Func_080c8830
	.global Func_02002304
	.thumb_func
Func_02002304:
	overlay_veneer Func_080c8840
	.global Func_0200230c
	.thumb_func
Func_0200230c:
	overlay_veneer Func_080c8848
	.global Func_02002314
	.thumb_func
Func_02002314:
	overlay_veneer Func_080c88a0
	.global Func_0200231c
	.thumb_func
Func_0200231c:
	overlay_veneer Func_080c88b0
	.global Func_02002324
	.thumb_func
Func_02002324:
	overlay_veneer Func_080c88b8
	.global Func_0200232c
	.thumb_func
Func_0200232c:
	overlay_veneer Audio_PlayCue
