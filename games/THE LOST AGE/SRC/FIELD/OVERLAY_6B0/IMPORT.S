@ Import veneers after the OVERLAY_6B0 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6B0 overlay import veneers
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
	.global ArcTan2
	.thumb_func
ArcTan2:
	overlay_veneer ArcTan2Far
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
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_020032b0
	.thumb_func
Func_020032b0:
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
	.global Func_020032e0
	.thumb_func
Func_020032e0:
	overlay_veneer Object_SetMode
	.global Func_020032e8
	.thumb_func
Func_020032e8:
	overlay_veneer Object_SetCallback
	.global Func_020032f0
	.thumb_func
Func_020032f0:
	overlay_veneer Func_080200c0
	.global Func_020032f8
	.thumb_func
Func_020032f8:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02003308
	.thumb_func
Func_02003308:
	overlay_veneer Func_08020120
	.global Func_02003310
	.thumb_func
Func_02003310:
	overlay_veneer Object_SetPosition
	.global Func_02003318
	.thumb_func
Func_02003318:
	overlay_veneer Object_CommitPosition
	.global Func_02003320
	.thumb_func
Func_02003320:
	overlay_veneer Func_08020170
	.global Func_02003328
	.thumb_func
Func_02003328:
	overlay_veneer Func_08020178
	.global Func_02003330
	.thumb_func
Func_02003330:
	overlay_veneer Func_080201a8
	.global Func_02003338
	.thumb_func
Func_02003338:
	overlay_veneer Func_080201b0
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02003348
	.thumb_func
Func_02003348:
	overlay_veneer Func_080201c8
	.global Func_02003350
	.thumb_func
Func_02003350:
	overlay_veneer Func_080201e0
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02003368
	.thumb_func
Func_02003368:
	overlay_veneer Func_08020228
	.global Func_02003370
	.thumb_func
Func_02003370:
	overlay_veneer Func_08020230
	.global Func_02003378
	.thumb_func
Func_02003378:
	overlay_veneer Func_080202c0
	.global Func_02003380
	.thumb_func
Func_02003380:
	overlay_veneer Func_08020360
	.global Func_02003388
	.thumb_func
Func_02003388:
	overlay_veneer Func_08038210
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02003398
	.thumb_func
Func_02003398:
	overlay_veneer Func_080c8018
	.global Func_020033a0
	.thumb_func
Func_020033a0:
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
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_020033e0
	.thumb_func
Func_020033e0:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02003400
	.thumb_func
Func_02003400:
	overlay_veneer Func_080c81d8
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02003410
	.thumb_func
Func_02003410:
	overlay_veneer Func_080c8218
	.global Object_AttachWorkTargetToObject
	.thumb_func
Object_AttachWorkTargetToObject:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Func_02003420
	.thumb_func
Func_02003420:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02003430
	.thumb_func
Func_02003430:
	overlay_veneer Func_080c8240
	.global Func_02003438
	.thumb_func
Func_02003438:
	overlay_veneer Func_080c8248
	.global Func_02003440
	.thumb_func
Func_02003440:
	overlay_veneer Func_080c8258
	.global Func_02003448
	.thumb_func
Func_02003448:
	overlay_veneer Event_SetPairWork1c0Far
	.global Func_02003450
	.thumb_func
Func_02003450:
	overlay_veneer Func_080c8278
	.global Func_02003458
	.thumb_func
Func_02003458:
	overlay_veneer Func_080c8378
	.global Func_02003460
	.thumb_func
Func_02003460:
	overlay_veneer Func_080c8380
	.global Func_02003468
	.thumb_func
Func_02003468:
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
	.global Func_02003488
	.thumb_func
Func_02003488:
	overlay_veneer Func_080c84e0
	.global Func_02003490
	.thumb_func
Func_02003490:
	overlay_veneer Func_080c8718
	.global Func_02003498
	.thumb_func
Func_02003498:
	overlay_veneer Func_080c8720
	.global Func_020034a0
	.thumb_func
Func_020034a0:
	overlay_veneer Func_080c8728
	.global Func_020034a8
	.thumb_func
Func_020034a8:
	overlay_veneer Func_080c88d8
	.global Func_020034b0
	.thumb_func
Func_020034b0:
	overlay_veneer Func_080c88e8
	.global Func_020034b8
	.thumb_func
Func_020034b8:
	overlay_veneer Audio_PlayCue
