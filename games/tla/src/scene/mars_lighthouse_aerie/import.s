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
	.global Func_0200556c
	.thumb_func
Func_0200556c:
	overlay_veneer Actor_StartScriptFar
	.global Func_02005574
	.thumb_func
Func_02005574:
	overlay_veneer Func_080200c0
	.global Func_0200557c
	.thumb_func
Func_0200557c:
	overlay_veneer Actor_Destroy
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_0200558c
	.thumb_func
Func_0200558c:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02005594
	.thumb_func
Func_02005594:
	overlay_veneer Actor_SetTargetPosition
	.global Func_0200559c
	.thumb_func
Func_0200559c:
	overlay_veneer Actor_CommitPosition
	.global Func_020055a4
	.thumb_func
Func_020055a4:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_020055ac
	.thumb_func
Func_020055ac:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_020055c4
	.thumb_func
Func_020055c4:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_020055cc
	.thumb_func
Func_020055cc:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_020055dc
	.thumb_func
Func_020055dc:
	overlay_veneer Func_08020390
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global Func_020055ec
	.thumb_func
Func_020055ec:
	overlay_veneer Func_08038248
	.global PartyInventory_Remove
	.thumb_func
PartyInventory_Remove:
	overlay_veneer PartyInventory_RemoveFar
	.global Func_020055fc
	.thumb_func
Func_020055fc:
	overlay_veneer Func_080ad208
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_0200560c
	.thumb_func
Func_0200560c:
	overlay_veneer Event_StartFar
	.global Func_02005614
	.thumb_func
Func_02005614:
	overlay_veneer Event_EndFar
	.global Event_PrepareObjectAndApplyValue
	.thumb_func
Event_PrepareObjectAndApplyValue:
	overlay_veneer Event_PrepareObjectAndApplyValueFar
	.global Party_RemoveOwnerRestored
	.thumb_func
Party_RemoveOwnerRestored:
	overlay_veneer Party_RemoveOwnerRestoredFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Func_0200563c
	.thumb_func
Func_0200563c:
	overlay_veneer ObjectTable_DestroyByIdFar
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
	.global Actor_SetPositionAndCommit
	.thumb_func
Actor_SetPositionAndCommit:
	overlay_veneer Actor_SetPositionAndCommitFar
	.global Actor_SetPositionAndReset
	.thumb_func
Actor_SetPositionAndReset:
	overlay_veneer Actor_SetPositionAndResetFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_0200567c
	.thumb_func
Func_0200567c:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02005684
	.thumb_func
Func_02005684:
	overlay_veneer Actor_SetHorizontalPositionAndAngleWithTerrainFar
	.global Func_0200568c
	.thumb_func
Func_0200568c:
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
	.global Func_020056cc
	.thumb_func
Func_020056cc:
	overlay_veneer Actor_ConfigureSpriteColorFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_020056dc
	.thumb_func
Func_020056dc:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_020056ec
	.thumb_func
Func_020056ec:
	overlay_veneer Func_080c8198
	.global Func_020056f4
	.thumb_func
Func_020056f4:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_02005704
	.thumb_func
Func_02005704:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02005714
	.thumb_func
Func_02005714:
	overlay_veneer Func_080c8210
	.global Func_0200571c
	.thumb_func
Func_0200571c:
	overlay_veneer Func_080c8218
	.global Func_02005724
	.thumb_func
Func_02005724:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02005734
	.thumb_func
Func_02005734:
	overlay_veneer Func_080c8240
	.global Func_0200573c
	.thumb_func
Func_0200573c:
	overlay_veneer Func_080c8258
	.global Func_02005744
	.thumb_func
Func_02005744:
	overlay_veneer Event_SetExitRequestFar
	.global Func_0200574c
	.thumb_func
Func_0200574c:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Func_0200575c
	.thumb_func
Func_0200575c:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02005764
	.thumb_func
Func_02005764:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_0200576c
	.thumb_func
Func_0200576c:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02005784
	.thumb_func
Func_02005784:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_0200578c
	.thumb_func
Func_0200578c:
	overlay_veneer Func_080c84e8
	.global Func_02005794
	.thumb_func
Func_02005794:
	overlay_veneer Func_080c84f0
	.global Func_0200579c
	.thumb_func
Func_0200579c:
	overlay_veneer Camera_WorldToScreenFar
	.global Func_020057a4
	.thumb_func
Func_020057a4:
	overlay_veneer Func_080c87c0
	.global Func_020057ac
	.thumb_func
Func_020057ac:
	overlay_veneer Func_080c87c8
	.global Func_020057b4
	.thumb_func
Func_020057b4:
	overlay_veneer Func_080c87e8
	.global Func_020057bc
	.thumb_func
Func_020057bc:
	overlay_veneer Func_080c87f0
	.global Func_020057c4
	.thumb_func
Func_020057c4:
	overlay_veneer Func_080c88d8
	.global Func_020057cc
	.thumb_func
Func_020057cc:
	overlay_veneer Func_080c88e8
	.global Func_020057d4
	.thumb_func
Func_020057d4:
	overlay_veneer Func_080c8918
	.global Func_020057dc
	.thumb_func
Func_020057dc:
	overlay_veneer Sound_PlayFar
