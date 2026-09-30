@ Import veneers after the OVERLAY_6AD overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6AD overlay import veneers
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
	.global Func_020053e0
	.thumb_func
Func_020053e0:
	overlay_veneer Func_080000d0
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Func_020053f0
	.thumb_func
Func_020053f0:
	overlay_veneer Func_08000100
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
	.global Func_02005420
	.thumb_func
Func_02005420:
	overlay_veneer Func_080001a8
	.global Func_02005428
	.thumb_func
Func_02005428:
	overlay_veneer Func_080001b8
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02005440
	.thumb_func
Func_02005440:
	overlay_veneer Func_080001e8
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer Owner_GetStateFar
	.global Func_02005450
	.thumb_func
Func_02005450:
	overlay_veneer Func_080003c8
	.global Func_02005458
	.thumb_func
Func_02005458:
	overlay_veneer Func_080003d0
	.global Func_02005460
	.thumb_func
Func_02005460:
	overlay_veneer Func_080003d8
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global Func_02005478
	.thumb_func
Func_02005478:
	overlay_veneer Func_08020070
	.global Func_02005480
	.thumb_func
Func_02005480:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_02005490
	.thumb_func
Func_02005490:
	overlay_veneer Object_SetCallback
	.global Func_02005498
	.thumb_func
Func_02005498:
	overlay_veneer Func_080200c0
	.global Func_020054a0
	.thumb_func
Func_020054a0:
	overlay_veneer Func_080200c8
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_020054b0
	.thumb_func
Func_020054b0:
	overlay_veneer Func_08020120
	.global Func_020054b8
	.thumb_func
Func_020054b8:
	overlay_veneer Object_SetPosition
	.global Func_020054c0
	.thumb_func
Func_020054c0:
	overlay_veneer Object_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_020054d0
	.thumb_func
Func_020054d0:
	overlay_veneer Func_080201c8
	.global Func_020054d8
	.thumb_func
Func_020054d8:
	overlay_veneer Func_080201d8
	.global Func_020054e0
	.thumb_func
Func_020054e0:
	overlay_veneer Func_080201e0
	.global Func_020054e8
	.thumb_func
Func_020054e8:
	overlay_veneer Func_080201e8
	.global Func_020054f0
	.thumb_func
Func_020054f0:
	overlay_veneer Func_080201f0
	.global Func_020054f8
	.thumb_func
Func_020054f8:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02005510
	.thumb_func
Func_02005510:
	overlay_veneer Func_08020228
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
	overlay_veneer Func_08020290
	.global Func_02005530
	.thumb_func
Func_02005530:
	overlay_veneer Func_080202f8
	.global Func_02005538
	.thumb_func
Func_02005538:
	overlay_veneer Func_08020348
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
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02005560
	.thumb_func
Func_02005560:
	overlay_veneer Func_080c8018
	.global Func_02005568
	.thumb_func
Func_02005568:
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
	.global Func_02005598
	.thumb_func
Func_02005598:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_020055a8
	.thumb_func
Func_020055a8:
	overlay_veneer Func_080c8150
	.global Func_020055b0
	.thumb_func
Func_020055b0:
	overlay_veneer Func_080c8170
	.global Func_020055b8
	.thumb_func
Func_020055b8:
	overlay_veneer Func_080c8208
	.global Func_020055c0
	.thumb_func
Func_020055c0:
	overlay_veneer Func_080c8218
	.global Func_020055c8
	.thumb_func
Func_020055c8:
	overlay_veneer Func_080c8228
	.global Func_020055d0
	.thumb_func
Func_020055d0:
	overlay_veneer Func_080c8230
	.global Func_020055d8
	.thumb_func
Func_020055d8:
	overlay_veneer Func_080c8238
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
	overlay_veneer Func_080c8278
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
	overlay_veneer Func_080c82f8
	.global Func_02005610
	.thumb_func
Func_02005610:
	overlay_veneer Func_080c8350
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
	overlay_veneer Func_080c84d8
	.global Func_02005648
	.thumb_func
Func_02005648:
	overlay_veneer Func_080c84e0
	.global Func_02005650
	.thumb_func
Func_02005650:
	overlay_veneer Func_080c8548
	.global Func_02005658
	.thumb_func
Func_02005658:
	overlay_veneer Func_080c85c0
	.global Func_02005660
	.thumb_func
Func_02005660:
	overlay_veneer Func_080c8628
	.global Func_02005668
	.thumb_func
Func_02005668:
	overlay_veneer Func_080c8690
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
	overlay_veneer Func_080c8778
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
	overlay_veneer Audio_PlayCue
