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
	.global Heap_Alloc
	.thumb_func
Heap_Alloc:
	overlay_veneer Heap_AllocFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global Func_02005420
	.thumb_func
Func_02005420:
	overlay_veneer Resource_DecodeType01Far
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer Resource_ResetEntryFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02005440
	.thumb_func
Func_02005440:
	overlay_veneer Oam_AddSpriteAtDepthFar
	.global Unit_Get
	.thumb_func
Unit_Get:
	overlay_veneer Unit_GetFar
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
	.global ResourceMetadata_ClearRecord
	.thumb_func
ResourceMetadata_ClearRecord:
	overlay_veneer ResourceMetadata_ClearRecordFar
	.global Func_02005480
	.thumb_func
Func_02005480:
	overlay_veneer Actor_SetMode
	.global Actor_SetAnimationSpeed
	.thumb_func
Actor_SetAnimationSpeed:
	overlay_veneer Actor_SetAnimationSpeedFar
	.global Func_02005490
	.thumb_func
Func_02005490:
	overlay_veneer Actor_StartScriptFar
	.global Func_02005498
	.thumb_func
Func_02005498:
	overlay_veneer Func_080200c0
	.global Func_020054a0
	.thumb_func
Func_020054a0:
	overlay_veneer Actor_Destroy
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_020054b0
	.thumb_func
Func_020054b0:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_020054b8
	.thumb_func
Func_020054b8:
	overlay_veneer Actor_SetTargetPosition
	.global Func_020054c0
	.thumb_func
Func_020054c0:
	overlay_veneer Actor_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_020054d0
	.thumb_func
Func_020054d0:
	overlay_veneer GetMapCellCollisionFar
	.global Func_020054d8
	.thumb_func
Func_020054d8:
	overlay_veneer Func_080201d8
	.global Func_020054e0
	.thumb_func
Func_020054e0:
	overlay_veneer Map_CopyCellsRectFar
	.global Func_020054e8
	.thumb_func
Func_020054e8:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_020054f0
	.thumb_func
Func_020054f0:
	overlay_veneer Func_080201f0
	.global Func_020054f8
	.thumb_func
Func_020054f8:
	overlay_veneer Func_08020210
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02005510
	.thumb_func
Func_02005510:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02005518
	.thumb_func
Func_02005518:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02005528
	.thumb_func
Func_02005528:
	overlay_veneer Actor_SetSpritePriorityDirectFar
	.global Func_02005530
	.thumb_func
Func_02005530:
	overlay_veneer Func_080202f8
	.global Func_02005538
	.thumb_func
Func_02005538:
	overlay_veneer Field_CheckSpheresOverlapFar
	.global Func_02005540
	.thumb_func
Func_02005540:
	overlay_veneer Func_08020360
	.global Func_02005548
	.thumb_func
Func_02005548:
	overlay_veneer Func_08020390
	.global Party_CountActiveOwners
	.thumb_func
Party_CountActiveOwners:
	overlay_veneer Party_CountActiveOwnersFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02005560
	.thumb_func
Func_02005560:
	overlay_veneer Event_StartFar
	.global Func_02005568
	.thumb_func
Func_02005568:
	overlay_veneer Event_EndFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_SetPositionAndCommit
	.thumb_func
Actor_SetPositionAndCommit:
	overlay_veneer Actor_SetPositionAndCommitFar
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02005598
	.thumb_func
Func_02005598:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Func_020055a8
	.thumb_func
Func_020055a8:
	overlay_veneer Actor_JumpDownLedgeFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_020055b8
	.thumb_func
Func_020055b8:
	overlay_veneer Func_080c8208
	.global Func_020055c0
	.thumb_func
Func_020055c0:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_020055d0
	.thumb_func
Func_020055d0:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_020055e0
	.thumb_func
Func_020055e0:
	overlay_veneer Func_080c8240
	.global Func_020055e8
	.thumb_func
Func_020055e8:
	overlay_veneer Func_080c8258
	.global Func_020055f0
	.thumb_func
Func_020055f0:
	overlay_veneer Event_SetExitRequestFar
	.global Func_020055f8
	.thumb_func
Func_020055f8:
	overlay_veneer Func_080c82c0
	.global Func_02005600
	.thumb_func
Func_02005600:
	overlay_veneer Func_080c82e0
	.global Func_02005608
	.thumb_func
Func_02005608:
	overlay_veneer Leader_StopMovingFar
	.global Func_02005610
	.thumb_func
Func_02005610:
	overlay_veneer ScreenTransition_StartFar
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
	.global ObjectTable_Get
	.thumb_func
ObjectTable_Get:
	overlay_veneer ObjectTable_GetFar
	.global Func_02005638
	.thumb_func
Func_02005638:
	overlay_veneer Func_080c8480
	.global Func_02005640
	.thumb_func
Func_02005640:
	overlay_veneer Event_ResolveAllPendingActionsFar
	.global Func_02005648
	.thumb_func
Func_02005648:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02005650
	.thumb_func
Func_02005650:
	overlay_veneer BattleParty_ApplyHealthDeltaFar
	.global Func_02005658
	.thumb_func
Func_02005658:
	overlay_veneer Camera_WorldToScreenFar
	.global Func_02005660
	.thumb_func
Func_02005660:
	overlay_veneer ObjectTable_SnapshotFar
	.global Func_02005668
	.thumb_func
Func_02005668:
	overlay_veneer Event_NoOpFar
	.global Func_02005670
	.thumb_func
Func_02005670:
	overlay_veneer Func_080c86d0
	.global Func_02005678
	.thumb_func
Func_02005678:
	overlay_veneer Func_080c86d8
	.global Func_02005680
	.thumb_func
Func_02005680:
	overlay_veneer Func_080c8738
	.global Func_02005688
	.thumb_func
Func_02005688:
	overlay_veneer Func_080c8748
	.global Func_02005690
	.thumb_func
Func_02005690:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02005698
	.thumb_func
Func_02005698:
	overlay_veneer Func_080c87e8
	.global Func_020056a0
	.thumb_func
Func_020056a0:
	overlay_veneer Func_080c8830
	.global Func_020056a8
	.thumb_func
Func_020056a8:
	overlay_veneer Func_080c8848
	.global Func_020056b0
	.thumb_func
Func_020056b0:
	overlay_veneer Func_080c8880
	.global Func_020056b8
	.thumb_func
Func_020056b8:
	overlay_veneer Func_080c8888
	.global Func_020056c0
	.thumb_func
Func_020056c0:
	overlay_veneer Sound_PlayFar
