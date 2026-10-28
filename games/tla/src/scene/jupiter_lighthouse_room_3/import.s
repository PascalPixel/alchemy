.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
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
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
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
	.global Heap_Alloc
	.thumb_func
Heap_Alloc:
	overlay_veneer Heap_AllocFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global Func_020022a0
	.thumb_func
Func_020022a0:
	overlay_veneer Resource_DecodeType01Far
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_020022b8
	.thumb_func
Func_020022b8:
	overlay_veneer Oam_AddSpriteAtDepthFar
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
	.global Func_020022e0
	.thumb_func
Func_020022e0:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_SetAnimationSpeed
	.thumb_func
ObjectDispatch_SetAnimationSpeed:
	overlay_veneer ObjectDispatch_SetAnimationSpeedFar
	.global Func_020022f0
	.thumb_func
Func_020022f0:
	overlay_veneer ObjectDispatch_InitializeFar
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
	overlay_veneer Map_CenterOnFollowedPointFar
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
	overlay_veneer Map_CopyCellsRectFar
	.global Func_02002330
	.thumb_func
Func_02002330:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02002338
	.thumb_func
Func_02002338:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_02002348
	.thumb_func
Func_02002348:
	overlay_veneer Map_SetScreenShakeFar
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
	overlay_veneer ObjectDispatch_SetSpritePriorityFar
	.global Func_02002368
	.thumb_func
Func_02002368:
	overlay_veneer Func_08020360
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02002378
	.thumb_func
Func_02002378:
	overlay_veneer Event_StartFar
	.global Func_02002380
	.thumb_func
Func_02002380:
	overlay_veneer Event_EndFar
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
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
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
	.global Func_020023d0
	.thumb_func
Func_020023d0:
	overlay_veneer Func_080c8208
	.global Func_020023d8
	.thumb_func
Func_020023d8:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_020023f0
	.thumb_func
Func_020023f0:
	overlay_veneer Func_080c8258
	.global Func_020023f8
	.thumb_func
Func_020023f8:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02002400
	.thumb_func
Func_02002400:
	overlay_veneer Func_080c82e0
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	overlay_veneer Event_FinishScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02002420
	.thumb_func
Func_02002420:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02002428
	.thumb_func
Func_02002428:
	overlay_veneer Camera_WorldToScreenFar
	.global Func_02002430
	.thumb_func
Func_02002430:
	overlay_veneer Event_NoOpFar
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
	overlay_veneer Sound_PlayFar
