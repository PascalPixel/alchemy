.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __umodsi3
	.thumb_func
__umodsi3:
	overlay_veneer IwramUnsignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
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
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
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
	.global Func_0200479c
	.thumb_func
Func_0200479c:
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
	.global Func_020047bc
	.thumb_func
Func_020047bc:
	overlay_veneer DebugText_ClearFar
	.global AffineMatrix_BuildForEffect
	.thumb_func
AffineMatrix_BuildForEffect:
	overlay_veneer AffineMatrix_BuildForEffectFar
	.global Func_020047cc
	.thumb_func
Func_020047cc:
	overlay_veneer Oam_AddSpriteAtDepthFar
	.global Func_020047d4
	.thumb_func
Func_020047d4:
	overlay_veneer Window_LoadGraphicsFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
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
	.global Import_ActorSetMode
	.thumb_func
Import_ActorSetMode:
	overlay_veneer Actor_SetMode
	.global Import_ActorSetScript
	.thumb_func
Import_ActorSetScript:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Func_0200481c
	.thumb_func
Func_0200481c:
	overlay_veneer Actor_Destroy
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_0200482c
	.thumb_func
Func_0200482c:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02004834
	.thumb_func
Func_02004834:
	overlay_veneer Actor_SetTargetPosition
	.global Func_0200483c
	.thumb_func
Func_0200483c:
	overlay_veneer Actor_CommitPosition
	.global Func_02004844
	.thumb_func
Func_02004844:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_0200484c
	.thumb_func
Func_0200484c:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02004864
	.thumb_func
Func_02004864:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_0200486c
	.thumb_func
Func_0200486c:
	overlay_veneer Func_08020230
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global Func_0200487c
	.thumb_func
Func_0200487c:
	overlay_veneer MessageArgument_PushFar
	.global Func_02004884
	.thumb_func
Func_02004884:
	overlay_veneer Func_08038248
	.global Func_0200488c
	.thumb_func
Func_0200488c:
	overlay_veneer Func_08038348
	.global Func_02004894
	.thumb_func
Func_02004894:
	overlay_veneer Item_Get
	.global PartyInventory_FindOwner
	.thumb_func
PartyInventory_FindOwner:
	overlay_veneer PartyInventory_FindOwnerFar
	.global Party_ListActiveOwners
	.thumb_func
Party_ListActiveOwners:
	overlay_veneer Party_ListActiveOwnersFar
	.global Func_020048ac
	.thumb_func
Func_020048ac:
	overlay_veneer Func_080ad208
	.global Func_020048b4
	.thumb_func
Func_020048b4:
	overlay_veneer Func_080ad2d8
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_020048c4
	.thumb_func
Func_020048c4:
	overlay_veneer Event_StartFar
	.global Func_020048cc
	.thumb_func
Func_020048cc:
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
	.global Actor_SetPositionAndCommit
	.thumb_func
Actor_SetPositionAndCommit:
	overlay_veneer Actor_SetPositionAndCommitFar
	.global Actor_ResetAndSetPositionInMode2
	.thumb_func
Actor_ResetAndSetPositionInMode2:
	overlay_veneer Actor_ResetAndSetPositionInMode2Far
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
	.global Func_02004934
	.thumb_func
Func_02004934:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_0200493c
	.thumb_func
Func_0200493c:
	overlay_veneer Actor_SetHorizontalPositionAndAngleWithTerrainFar
	.global Func_02004944
	.thumb_func
Func_02004944:
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
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_0200498c
	.thumb_func
Func_0200498c:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_0200499c
	.thumb_func
Func_0200499c:
	overlay_veneer Func_080c8198
	.global Func_020049a4
	.thumb_func
Func_020049a4:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Func_020049ac
	.thumb_func
Func_020049ac:
	overlay_veneer Func_080c81a8
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_020049bc
	.thumb_func
Func_020049bc:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_020049cc
	.thumb_func
Func_020049cc:
	overlay_veneer Func_080c8210
	.global Func_020049d4
	.thumb_func
Func_020049d4:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_020049e4
	.thumb_func
Func_020049e4:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_020049f4
	.thumb_func
Func_020049f4:
	overlay_veneer Func_080c8240
	.global Func_020049fc
	.thumb_func
Func_020049fc:
	overlay_veneer Func_080c8258
	.global Func_02004a04
	.thumb_func
Func_02004a04:
	overlay_veneer Event_SetExitRequestFar
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
	.global Func_02004a24
	.thumb_func
Func_02004a24:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02004a2c
	.thumb_func
Func_02004a2c:
	overlay_veneer Party_PlaySavedMusicFar
	.global Func_02004a34
	.thumb_func
Func_02004a34:
	overlay_veneer Func_080c8580
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Func_02004a44
	.thumb_func
Func_02004a44:
	overlay_veneer Func_080c86a8
	.global Func_02004a4c
	.thumb_func
Func_02004a4c:
	overlay_veneer Func_080c86e8
	.global Func_02004a54
	.thumb_func
Func_02004a54:
	overlay_veneer BattleEffect_StartArcEffectFar
	.global Func_02004a5c
	.thumb_func
Func_02004a5c:
	overlay_veneer BattleEffect_EndArcEffectFar
	.global Func_02004a64
	.thumb_func
Func_02004a64:
	overlay_veneer BattleEffect_WaitForArcGroupsFar
	.global Func_02004a6c
	.thumb_func
Func_02004a6c:
	overlay_veneer BattleEffect_SetObjectUpdateModeFar
	.global Func_02004a74
	.thumb_func
Func_02004a74:
	overlay_veneer Func_08108008
	.global Func_02004a7c
	.thumb_func
Func_02004a7c:
	overlay_veneer Func_08108010
	.global Func_02004a84
	.thumb_func
Func_02004a84:
	overlay_veneer Func_08108018
	.global Func_02004a8c
	.thumb_func
Func_02004a8c:
	overlay_veneer Func_08108028
	.global Func_02004a94
	.thumb_func
Func_02004a94:
	overlay_veneer Inn_PlaySleepFar
	.global Import_SoundPlay
	.thumb_func
Import_SoundPlay:
	overlay_veneer Sound_PlayFar
