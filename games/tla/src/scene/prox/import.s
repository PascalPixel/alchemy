.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Task_RemoveFar
	.thumb_func
Task_RemoveFar:
	overlay_veneer Func_080000d8
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global ArcTan2
	.thumb_func
ArcTan2:
	overlay_veneer ArcTan2Far
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global HeapSlot_AllocFar
	.thumb_func
HeapSlot_AllocFar:
	overlay_veneer Func_08000140
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
	.global Func_02004384
	.thumb_func
Func_02004384:
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
	.global Func_020043c4
	.thumb_func
Func_020043c4:
	overlay_veneer Actor_SetMode
	.global Actor_SetAnimationSpeed
	.thumb_func
Actor_SetAnimationSpeed:
	overlay_veneer Actor_SetAnimationSpeedFar
	.global Func_020043d4
	.thumb_func
Func_020043d4:
	overlay_veneer Actor_StartScriptFar
	.global Func_020043dc
	.thumb_func
Func_020043dc:
	overlay_veneer Func_080200c0
	.global Func_020043e4
	.thumb_func
Func_020043e4:
	overlay_veneer Actor_Destroy
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_020043f4
	.thumb_func
Func_020043f4:
	overlay_veneer Actor_SetTargetPosition
	.global Func_020043fc
	.thumb_func
Func_020043fc:
	overlay_veneer Actor_CommitPosition
	.global Func_02004404
	.thumb_func
Func_02004404:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_0200440c
	.thumb_func
Func_0200440c:
	overlay_veneer Func_08020198
	.global Func_02004414
	.thumb_func
Func_02004414:
	overlay_veneer Func_080201a0
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02004424
	.thumb_func
Func_02004424:
	overlay_veneer GetMapCellCollisionFar
	.global Func_0200442c
	.thumb_func
Func_0200442c:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02004444
	.thumb_func
Func_02004444:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_0200444c
	.thumb_func
Func_0200444c:
	overlay_veneer Func_080202f8
	.global Func_02004454
	.thumb_func
Func_02004454:
	overlay_veneer Field_CheckSpheresOverlapFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02004464
	.thumb_func
Func_02004464:
	overlay_veneer Event_StartFar
	.global Func_0200446c
	.thumb_func
Func_0200446c:
	overlay_veneer Event_EndFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_SetMovementTypeById
	.thumb_func
Actor_SetMovementTypeById:
	overlay_veneer Actor_SetMovementTypeByIdFar
	.global Actor_RefreshSelectorById
	.thumb_func
Actor_RefreshSelectorById:
	overlay_veneer Actor_RefreshSelectorByIdFar
	.global Actor_Stop
	.thumb_func
Actor_Stop:
	overlay_veneer Actor_StopFar
	.global Actor_SetMovementTypeAndWaitById
	.thumb_func
Actor_SetMovementTypeAndWaitById:
	overlay_veneer Actor_SetMovementTypeAndWaitByIdFar
	.global Actor_ResetAndSetPosition
	.thumb_func
Actor_ResetAndSetPosition:
	overlay_veneer Actor_ResetAndSetPositionFar
	.global Actor_SetPositionAndReset
	.thumb_func
Actor_SetPositionAndReset:
	overlay_veneer Actor_SetPositionAndResetFar
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_020044cc
	.thumb_func
Func_020044cc:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_020044d4
	.thumb_func
Func_020044d4:
	overlay_veneer Actor_SetHorizontalPositionAndAngleWithTerrainFar
	.global Func_020044dc
	.thumb_func
Func_020044dc:
	overlay_veneer Actor_MoveToObjectFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Actor_WaitForAnimationChange
	.thumb_func
Actor_WaitForAnimationChange:
	overlay_veneer Actor_WaitForAnimationChangeFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_0200451c
	.thumb_func
Func_0200451c:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_0200452c
	.thumb_func
Func_0200452c:
	overlay_veneer Func_080c8198
	.global Func_02004534
	.thumb_func
Func_02004534:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Func_0200453c
	.thumb_func
Func_0200453c:
	overlay_veneer Func_080c81a8
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_0200454c
	.thumb_func
Func_0200454c:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_0200455c
	.thumb_func
Func_0200455c:
	overlay_veneer Func_080c8210
	.global Func_02004564
	.thumb_func
Func_02004564:
	overlay_veneer Func_080c8218
	.global Func_0200456c
	.thumb_func
Func_0200456c:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_0200457c
	.thumb_func
Func_0200457c:
	overlay_veneer Func_080c8240
	.global Func_02004584
	.thumb_func
Func_02004584:
	overlay_veneer Func_080c8258
	.global Func_0200458c
	.thumb_func
Func_0200458c:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02004594
	.thumb_func
Func_02004594:
	overlay_veneer Leader_StopMovingFar
	.global Func_0200459c
	.thumb_func
Func_0200459c:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_020045a4
	.thumb_func
Func_020045a4:
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
	.global Actor_SetWideSprite
	.thumb_func
Actor_SetWideSprite:
	overlay_veneer Actor_SetWideSpriteFar
	.global Func_020045cc
	.thumb_func
Func_020045cc:
	overlay_veneer Actor_ReturnHomeFar
	.global ObjectTable_Get
	.thumb_func
ObjectTable_Get:
	overlay_veneer ObjectTable_GetFar
	.global Func_020045dc
	.thumb_func
Func_020045dc:
	overlay_veneer Func_080c8480
	.global Func_020045e4
	.thumb_func
Func_020045e4:
	overlay_veneer Event_ResolveAllPendingActionsFar
	.global Func_020045ec
	.thumb_func
Func_020045ec:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_020045f4
	.thumb_func
Func_020045f4:
	overlay_veneer Func_080c8580
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Func_02004604
	.thumb_func
Func_02004604:
	overlay_veneer Func_080c87c0
	.global Func_0200460c
	.thumb_func
Func_0200460c:
	overlay_veneer Func_080c87d0
	.global Func_02004614
	.thumb_func
Func_02004614:
	overlay_veneer BattleEffect_StepVector3Far
	.global Func_0200461c
	.thumb_func
Func_0200461c:
	overlay_veneer Func_080c8918
	.global Func_02004624
	.thumb_func
Func_02004624:
	overlay_veneer Func_08108008
	.global Func_0200462c
	.thumb_func
Func_0200462c:
	overlay_veneer Func_08108010
	.global Func_02004634
	.thumb_func
Func_02004634:
	overlay_veneer Func_08108018
	.global Func_0200463c
	.thumb_func
Func_0200463c:
	overlay_veneer Sound_PlayFar
