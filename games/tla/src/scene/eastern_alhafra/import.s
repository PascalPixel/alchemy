.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
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
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
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
	.global Func_020049d4
	.thumb_func
Func_020049d4:
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
	.global Func_02004a0c
	.thumb_func
Func_02004a0c:
	overlay_veneer Actor_SetMode
	.global Func_02004a14
	.thumb_func
Func_02004a14:
	overlay_veneer Actor_StartScriptFar
	.global Func_02004a1c
	.thumb_func
Func_02004a1c:
	overlay_veneer Func_080200c0
	.global Func_02004a24
	.thumb_func
Func_02004a24:
	overlay_veneer Actor_Destroy
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_02004a34
	.thumb_func
Func_02004a34:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02004a3c
	.thumb_func
Func_02004a3c:
	overlay_veneer Actor_CommitPosition
	.global Func_02004a44
	.thumb_func
Func_02004a44:
	overlay_veneer Map_PlayMapEditFar
	.global Func_02004a4c
	.thumb_func
Func_02004a4c:
	overlay_veneer Map_CopyCellsRectFar
	.global Func_02004a54
	.thumb_func
Func_02004a54:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02004a5c
	.thumb_func
Func_02004a5c:
	overlay_veneer Func_080201f0
	.global Func_02004a64
	.thumb_func
Func_02004a64:
	overlay_veneer Func_08020210
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Func_02004a74
	.thumb_func
Func_02004a74:
	overlay_veneer Actor_SetSpritePriorityDirectFar
	.global Func_02004a7c
	.thumb_func
Func_02004a7c:
	overlay_veneer Func_080202f8
	.global Func_02004a84
	.thumb_func
Func_02004a84:
	overlay_veneer Party_FindOwnerWithActionFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02004a94
	.thumb_func
Func_02004a94:
	overlay_veneer Event_StartFar
	.global Func_02004a9c
	.thumb_func
Func_02004a9c:
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
	.global Actor_ResetAndSetPosition
	.thumb_func
Actor_ResetAndSetPosition:
	overlay_veneer Actor_ResetAndSetPositionFar
	.global Actor_ResetAndSetPositionInMode2
	.thumb_func
Actor_ResetAndSetPositionInMode2:
	overlay_veneer Actor_ResetAndSetPositionInMode2Far
	.global Actor_SetPositionAndReset
	.thumb_func
Actor_SetPositionAndReset:
	overlay_veneer Actor_SetPositionAndResetFar
	.global Actor_CenterOnColumn
	.thumb_func
Actor_CenterOnColumn:
	overlay_veneer Actor_CenterOnColumnFar
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02004b04
	.thumb_func
Func_02004b04:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Func_02004b3c
	.thumb_func
Func_02004b3c:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02004b4c
	.thumb_func
Func_02004b4c:
	overlay_veneer Func_080c8198
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_02004b5c
	.thumb_func
Func_02004b5c:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02004b6c
	.thumb_func
Func_02004b6c:
	overlay_veneer Func_080c8210
	.global Func_02004b74
	.thumb_func
Func_02004b74:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_02004b84
	.thumb_func
Func_02004b84:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02004b94
	.thumb_func
Func_02004b94:
	overlay_veneer Func_080c8240
	.global Func_02004b9c
	.thumb_func
Func_02004b9c:
	overlay_veneer Func_080c8250
	.global Func_02004ba4
	.thumb_func
Func_02004ba4:
	overlay_veneer Event_SetDestinationFar
	.global Func_02004bac
	.thumb_func
Func_02004bac:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02004bb4
	.thumb_func
Func_02004bb4:
	overlay_veneer Func_080c82c0
	.global Func_02004bbc
	.thumb_func
Func_02004bbc:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02004bc4
	.thumb_func
Func_02004bc4:
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
	.global Func_02004be4
	.thumb_func
Func_02004be4:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02004bec
	.thumb_func
Func_02004bec:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global Actor_OffsetPositionAndReset
	.thumb_func
Actor_OffsetPositionAndReset:
	overlay_veneer Actor_OffsetPositionAndResetFar
	.global Actor_CommitPositionAndActivate
	.thumb_func
Actor_CommitPositionAndActivate:
	overlay_veneer Actor_CommitPositionAndActivateFar
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Func_02004c0c
	.thumb_func
Func_02004c0c:
	overlay_veneer Event_NoOpFar
	.global Func_02004c14
	.thumb_func
Func_02004c14:
	overlay_veneer Func_080c86a8
	.global Func_02004c1c
	.thumb_func
Func_02004c1c:
	overlay_veneer Func_080c86d0
	.global Func_02004c24
	.thumb_func
Func_02004c24:
	overlay_veneer Func_080c86d8
	.global Func_02004c2c
	.thumb_func
Func_02004c2c:
	overlay_veneer Func_080c86e8
	.global Func_02004c34
	.thumb_func
Func_02004c34:
	overlay_veneer Func_080c8710
	.global Func_02004c3c
	.thumb_func
Func_02004c3c:
	overlay_veneer Func_080c8738
	.global Func_02004c44
	.thumb_func
Func_02004c44:
	overlay_veneer Func_080c8740
	.global Func_02004c4c
	.thumb_func
Func_02004c4c:
	overlay_veneer Func_080c8748
	.global Func_02004c54
	.thumb_func
Func_02004c54:
	overlay_veneer Func_080c87a8
	.global Func_02004c5c
	.thumb_func
Func_02004c5c:
	overlay_veneer Func_080c87b8
	.global Func_02004c64
	.thumb_func
Func_02004c64:
	overlay_veneer Func_080c87c0
	.global Func_02004c6c
	.thumb_func
Func_02004c6c:
	overlay_veneer Func_080c87d0
	.global Func_02004c74
	.thumb_func
Func_02004c74:
	overlay_veneer BattleEffect_StepVector3Far
	.global Func_02004c7c
	.thumb_func
Func_02004c7c:
	overlay_veneer Func_080c8840
	.global Func_02004c84
	.thumb_func
Func_02004c84:
	overlay_veneer Func_080c8848
	.global Func_02004c8c
	.thumb_func
Func_02004c8c:
	overlay_veneer Func_080c8868
	.global Func_02004c94
	.thumb_func
Func_02004c94:
	overlay_veneer Func_080c8870
	.global Func_02004c9c
	.thumb_func
Func_02004c9c:
	overlay_veneer Sound_PlayFar
