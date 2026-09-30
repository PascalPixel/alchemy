@ Import veneers after the OVERLAY_69C overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_69C overlay import veneers
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
	.global Func_02002268
	.thumb_func
Func_02002268:
	overlay_veneer Func_080000d0
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
	.global Func_020022a0
	.thumb_func
Func_020022a0:
	overlay_veneer Func_080001a8
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_020022b8
	.thumb_func
Func_020022b8:
	overlay_veneer Func_080001e8
	.global Func_020022c0
	.thumb_func
Func_020022c0:
	overlay_veneer Func_080003c8
	.global Func_020022c8
	.thumb_func
Func_020022c8:
	overlay_veneer Func_080003d0
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global Func_020022e0
	.thumb_func
Func_020022e0:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_020022f0
	.thumb_func
Func_020022f0:
	overlay_veneer Object_SetCallback
	.global Func_020022f8
	.thumb_func
Func_020022f8:
	overlay_veneer Func_080200c0
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02002308
	.thumb_func
Func_02002308:
	overlay_veneer Func_08020120
	.global Func_02002310
	.thumb_func
Func_02002310:
	overlay_veneer Object_SetPosition
	.global Func_02002318
	.thumb_func
Func_02002318:
	overlay_veneer Object_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02002328
	.thumb_func
Func_02002328:
	overlay_veneer Func_080201e0
	.global Func_02002330
	.thumb_func
Func_02002330:
	overlay_veneer Func_080201e8
	.global Func_02002338
	.thumb_func
Func_02002338:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_02002348
	.thumb_func
Func_02002348:
	overlay_veneer Func_08020228
	.global Func_02002350
	.thumb_func
Func_02002350:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02002360
	.thumb_func
Func_02002360:
	overlay_veneer Func_08020290
	.global Func_02002368
	.thumb_func
Func_02002368:
	overlay_veneer Func_08020360
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02002378
	.thumb_func
Func_02002378:
	overlay_veneer Func_080c8018
	.global Func_02002380
	.thumb_func
Func_02002380:
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
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_020023b0
	.thumb_func
Func_020023b0:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_020023c0
	.thumb_func
Func_020023c0:
	overlay_veneer Func_080c8170
	.global Func_020023c8
	.thumb_func
Func_020023c8:
	overlay_veneer Func_080c8200
	.global Func_020023d0
	.thumb_func
Func_020023d0:
	overlay_veneer Func_080c8208
	.global Func_020023d8
	.thumb_func
Func_020023d8:
	overlay_veneer Func_080c8218
	.global Func_020023e0
	.thumb_func
Func_020023e0:
	overlay_veneer Func_080c8228
	.global Func_020023e8
	.thumb_func
Func_020023e8:
	overlay_veneer Func_080c8238
	.global Func_020023f0
	.thumb_func
Func_020023f0:
	overlay_veneer Func_080c8258
	.global Func_020023f8
	.thumb_func
Func_020023f8:
	overlay_veneer Func_080c8278
	.global Func_02002400
	.thumb_func
Func_02002400:
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
	.global Func_02002420
	.thumb_func
Func_02002420:
	overlay_veneer Func_080c84e0
	.global Func_02002428
	.thumb_func
Func_02002428:
	overlay_veneer Func_080c85c0
	.global Func_02002430
	.thumb_func
Func_02002430:
	overlay_veneer Func_080c8690
	.global Func_02002438
	.thumb_func
Func_02002438:
	overlay_veneer Func_080c8738
	.global Func_02002440
	.thumb_func
Func_02002440:
	overlay_veneer Func_080c8780
	.global Func_02002448
	.thumb_func
Func_02002448:
	overlay_veneer Func_080c87e8
	.global Func_02002450
	.thumb_func
Func_02002450:
	overlay_veneer Func_080c8830
	.global Func_02002458
	.thumb_func
Func_02002458:
	overlay_veneer Func_080c8840
	.global Func_02002460
	.thumb_func
Func_02002460:
	overlay_veneer Func_080c8848
	.global Func_02002468
	.thumb_func
Func_02002468:
	overlay_veneer Func_080c8880
	.global Func_02002470
	.thumb_func
Func_02002470:
	overlay_veneer Audio_PlayCue
