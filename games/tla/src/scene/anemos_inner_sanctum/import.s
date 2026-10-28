.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
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
	.global MemAlloc
	.thumb_func
MemAlloc:
	overlay_veneer Runtime_BumpAllocateFar
	.global MemFree
	.thumb_func
MemFree:
	overlay_veneer Sys_FreeFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_020032b0
	.thumb_func
Func_020032b0:
	overlay_veneer Oam_AddSpriteAtDepthFar
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
	overlay_veneer ObjectDispatch_InitializeFar
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
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
	overlay_veneer Map_CenterOnFollowedPointFar
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
	overlay_veneer Map_PlayMapEditFar
	.global Func_02003328
	.thumb_func
Func_02003328:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_02003330
	.thumb_func
Func_02003330:
	overlay_veneer DisplayBlend_DisableRunScriptFar
	.global Func_02003338
	.thumb_func
Func_02003338:
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02003348
	.thumb_func
Func_02003348:
	overlay_veneer GetMapCellCollisionFar
	.global Func_02003350
	.thumb_func
Func_02003350:
	overlay_veneer Map_CopyCellsRectFar
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02003368
	.thumb_func
Func_02003368:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02003370
	.thumb_func
Func_02003370:
	overlay_veneer Func_08020230
	.global Func_02003378
	.thumb_func
Func_02003378:
	overlay_veneer SetMapCellCollisionFar
	.global Func_02003380
	.thumb_func
Func_02003380:
	overlay_veneer Func_08020360
	.global Func_02003388
	.thumb_func
Func_02003388:
	overlay_veneer Func_08038210
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02003398
	.thumb_func
Func_02003398:
	overlay_veneer EventRuntime_StartFar
	.global Func_020033a0
	.thumb_func
Func_020033a0:
	overlay_veneer EventRuntime_EndFar
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
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
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
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02003410
	.thumb_func
Func_02003410:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_02003420
	.thumb_func
Func_02003420:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
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
	overlay_veneer Event_SetDestinationFar
	.global Func_02003450
	.thumb_func
Func_02003450:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_02003458
	.thumb_func
Func_02003458:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02003460
	.thumb_func
Func_02003460:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_02003468
	.thumb_func
Func_02003468:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
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
	.global Func_02003488
	.thumb_func
Func_02003488:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
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
	overlay_veneer Sound_PlayFar
