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
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global Heap_AllocEwram
	.thumb_func
Heap_AllocEwram:
	overlay_veneer Heap_AllocEwramFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global Func_02004aa8
	.thumb_func
Func_02004aa8:
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
	.global Func_02004ac8
	.thumb_func
Func_02004ac8:
	overlay_veneer DebugText_ClearFar
	.global Func_02004ad0
	.thumb_func
Func_02004ad0:
	overlay_veneer Oam_AddSpriteAtDepthFar
	.global Func_02004ad8
	.thumb_func
Func_02004ad8:
	overlay_veneer Window_LoadGraphicsFar
	.global Render_ProjectPoint
	.thumb_func
Render_ProjectPoint:
	overlay_veneer Render_ProjectPointFar
	.global QueueIoWriteDelay2
	.thumb_func
QueueIoWriteDelay2:
	overlay_veneer QueueIoWriteDelay2Far
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
	.global Animation_ApplyChildArgument
	.thumb_func
Animation_ApplyChildArgument:
	overlay_veneer Animation_ApplyChildArgumentFar
	.global Func_02004b18
	.thumb_func
Func_02004b18:
	overlay_veneer Actor_SetMode
	.global Func_02004b20
	.thumb_func
Func_02004b20:
	overlay_veneer Actor_StartScriptFar
	.global Func_02004b28
	.thumb_func
Func_02004b28:
	overlay_veneer Func_080200b0
	.global Func_02004b30
	.thumb_func
Func_02004b30:
	overlay_veneer Func_080200c0
	.global Func_02004b38
	.thumb_func
Func_02004b38:
	overlay_veneer Actor_Destroy
	.global Func_02004b40
	.thumb_func
Func_02004b40:
	overlay_veneer Map_UpdateCurrentTileBlockFar
	.global Func_02004b48
	.thumb_func
Func_02004b48:
	overlay_veneer Actor_ResetMotion
	.global Func_02004b50
	.thumb_func
Func_02004b50:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02004b58
	.thumb_func
Func_02004b58:
	overlay_veneer Actor_CommitPosition
	.global Func_02004b60
	.thumb_func
Func_02004b60:
	overlay_veneer Func_08020200
	.global Func_02004b68
	.thumb_func
Func_02004b68:
	overlay_veneer Func_08020208
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Func_02004b78
	.thumb_func
Func_02004b78:
	overlay_veneer Map_SetScreenShakeFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02004b88
	.thumb_func
Func_02004b88:
	overlay_veneer Func_08020318
	.global Func_02004b90
	.thumb_func
Func_02004b90:
	overlay_veneer Func_08020350
	.global GetWorldMapCollision
	.thumb_func
GetWorldMapCollision:
	overlay_veneer GetWorldMapCollisionFar
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global Func_02004ba8
	.thumb_func
Func_02004ba8:
	overlay_veneer MessageArgument_PushFar
	.global Message_CloseFinishedWindows
	.thumb_func
Message_CloseFinishedWindows:
	overlay_veneer Message_CloseFinishedWindowsFar
	.global Func_02004bb8
	.thumb_func
Func_02004bb8:
	overlay_veneer Func_08038348
	.global Menu_AnimateSelectionToEntry
	.thumb_func
Menu_AnimateSelectionToEntry:
	overlay_veneer Menu_AnimateSelectionToEntryFar
	.global Unit_AddPp
	.thumb_func
Unit_AddPp:
	overlay_veneer Unit_AddPpFar
	.global Party_CountActiveOwners
	.thumb_func
Party_CountActiveOwners:
	overlay_veneer Party_CountActiveOwnersFar
	.global Party_AddActiveOwner
	.thumb_func
Party_AddActiveOwner:
	overlay_veneer Party_AddActiveOwnerFar
	.global Party_RemoveActiveOwner
	.thumb_func
Party_RemoveActiveOwner:
	overlay_veneer Party_RemoveActiveOwnerFar
	.global Djinn_AddToOwner
	.thumb_func
Djinn_AddToOwner:
	overlay_veneer Djinn_AddToOwnerFar
	.global DjinnRecovery_AddStandby
	.thumb_func
DjinnRecovery_AddStandby:
	overlay_veneer DjinnRecovery_AddStandbyFar
	.global Func_02004bf8
	.thumb_func
Func_02004bf8:
	overlay_veneer Func_080ad208
	.global Func_02004c00
	.thumb_func
Func_02004c00:
	overlay_veneer Func_080ad228
	.global Func_02004c08
	.thumb_func
Func_02004c08:
	overlay_veneer Func_080ad310
	.global Func_02004c10
	.thumb_func
Func_02004c10:
	overlay_veneer Func_080ad318
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02004c20
	.thumb_func
Func_02004c20:
	overlay_veneer Event_StartFar
	.global Func_02004c28
	.thumb_func
Func_02004c28:
	overlay_veneer Event_EndFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Func_02004c38
	.thumb_func
Func_02004c38:
	overlay_veneer Func_080c8078
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
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02004c88
	.thumb_func
Func_02004c88:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02004c90
	.thumb_func
Func_02004c90:
	overlay_veneer Actor_SetHorizontalPositionAndAngleWithTerrainFar
	.global Func_02004c98
	.thumb_func
Func_02004c98:
	overlay_veneer Actor_MoveToObjectFar
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
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Actor_TurnToFaceEachOther
	.thumb_func
Actor_TurnToFaceEachOther:
	overlay_veneer Actor_TurnToFaceEachOtherFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02004cd8
	.thumb_func
Func_02004cd8:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02004ce8
	.thumb_func
Func_02004ce8:
	overlay_veneer Func_080c8198
	.global Func_02004cf0
	.thumb_func
Func_02004cf0:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_02004d00
	.thumb_func
Func_02004d00:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Func_02004d08
	.thumb_func
Func_02004d08:
	overlay_veneer Func_080c8210
	.global Func_02004d10
	.thumb_func
Func_02004d10:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_02004d20
	.thumb_func
Func_02004d20:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02004d30
	.thumb_func
Func_02004d30:
	overlay_veneer Func_080c8240
	.global Func_02004d38
	.thumb_func
Func_02004d38:
	overlay_veneer Event_SetDestinationFar
	.global Func_02004d40
	.thumb_func
Func_02004d40:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02004d48
	.thumb_func
Func_02004d48:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Func_02004d50
	.thumb_func
Func_02004d50:
	overlay_veneer Func_080c8288
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Party_SetFields1f2And1f4
	.thumb_func
Party_SetFields1f2And1f4:
	overlay_veneer Party_SetFields1f2And1f4Far
	.global Func_02004d68
	.thumb_func
Func_02004d68:
	overlay_veneer Func_080c82c0
	.global Func_02004d70
	.thumb_func
Func_02004d70:
	overlay_veneer Func_080c8368
	.global Func_02004d78
	.thumb_func
Func_02004d78:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02004d80
	.thumb_func
Func_02004d80:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_02004d88
	.thumb_func
Func_02004d88:
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
	.global Func_02004da8
	.thumb_func
Func_02004da8:
	overlay_veneer Func_080c8430
	.global Func_02004db0
	.thumb_func
Func_02004db0:
	overlay_veneer Func_080c8438
	.global Func_02004db8
	.thumb_func
Func_02004db8:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02004dc0
	.thumb_func
Func_02004dc0:
	overlay_veneer Party_PlaySavedMusicFar
	.global Func_02004dc8
	.thumb_func
Func_02004dc8:
	overlay_veneer Scene_ResolveInteractionResultFar
	.global Func_02004dd0
	.thumb_func
Func_02004dd0:
	overlay_veneer Func_080c8580
	.global Func_02004dd8
	.thumb_func
Func_02004dd8:
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
	.global Func_02004df8
	.thumb_func
Func_02004df8:
	overlay_veneer Func_080c8638
	.global Func_02004e00
	.thumb_func
Func_02004e00:
	overlay_veneer Func_080c8640
	.global Func_02004e08
	.thumb_func
Func_02004e08:
	overlay_veneer Func_080c86f0
	.global Func_02004e10
	.thumb_func
Func_02004e10:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02004e18
	.thumb_func
Func_02004e18:
	overlay_veneer Func_080c8860
	.global Func_02004e20
	.thumb_func
Func_02004e20:
	overlay_veneer SceneEvent_FindActiveRecordFar
	.global Func_02004e28
	.thumb_func
Func_02004e28:
	overlay_veneer DjinnMenu_Open
	.global Func_02004e30
	.thumb_func
Func_02004e30:
	overlay_veneer Sound_PlayFar
