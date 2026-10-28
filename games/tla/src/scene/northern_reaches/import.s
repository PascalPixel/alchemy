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
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global HeapSlot_AllocFar
	.thumb_func
HeapSlot_AllocFar:
	overlay_veneer Func_08000140
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
	.global Heap_Alloc
	.thumb_func
Heap_Alloc:
	overlay_veneer Heap_AllocFar
	.global Heap_AllocEwram
	.thumb_func
Heap_AllocEwram:
	overlay_veneer Heap_AllocEwramFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global Func_020009f0
	.thumb_func
Func_020009f0:
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
	.global Func_02000a10
	.thumb_func
Func_02000a10:
	overlay_veneer Oam_AddSpriteAtDepthFar
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
	.global Func_02000a30
	.thumb_func
Func_02000a30:
	overlay_veneer Actor_CommitPosition
	.global Func_02000a38
	.thumb_func
Func_02000a38:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02000a40
	.thumb_func
Func_02000a40:
	overlay_veneer Func_080201f0
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Func_02000a50
	.thumb_func
Func_02000a50:
	overlay_veneer Map_SetScreenShakeFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02000a68
	.thumb_func
Func_02000a68:
	overlay_veneer Event_StartFar
	.global Func_02000a70
	.thumb_func
Func_02000a70:
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
	.global Actor_SetPositionAndCommit
	.thumb_func
Actor_SetPositionAndCommit:
	overlay_veneer Actor_SetPositionAndCommitFar
	.global Actor_ResetAndSetPositionInMode2
	.thumb_func
Actor_ResetAndSetPositionInMode2:
	overlay_veneer Actor_ResetAndSetPositionInMode2Far
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02000ab0
	.thumb_func
Func_02000ab0:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Func_02000ac0
	.thumb_func
Func_02000ac0:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02000ad0
	.thumb_func
Func_02000ad0:
	overlay_veneer Func_080c8198
	.global Func_02000ad8
	.thumb_func
Func_02000ad8:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02000af0
	.thumb_func
Func_02000af0:
	overlay_veneer Func_080c8240
	.global Func_02000af8
	.thumb_func
Func_02000af8:
	overlay_veneer Event_SetDestinationFar
	.global Func_02000b00
	.thumb_func
Func_02000b00:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02000b08
	.thumb_func
Func_02000b08:
	overlay_veneer Func_080c8330
	.global Func_02000b10
	.thumb_func
Func_02000b10:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02000b18
	.thumb_func
Func_02000b18:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02000b30
	.thumb_func
Func_02000b30:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Actor_OffsetPositionAndReset
	.thumb_func
Actor_OffsetPositionAndReset:
	overlay_veneer Actor_OffsetPositionAndResetFar
	.global Func_02000b40
	.thumb_func
Func_02000b40:
	overlay_veneer Func_080c87c0
	.global Func_02000b48
	.thumb_func
Func_02000b48:
	overlay_veneer Func_080c87d0
	.global Func_02000b50
	.thumb_func
Func_02000b50:
	overlay_veneer BattleEffect_StepVector3Far
	.global Func_02000b58
	.thumb_func
Func_02000b58:
	overlay_veneer Sound_PlayFar
