.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __umodsi3
	.thumb_func
__umodsi3:
	overlay_veneer IwramUnsignedRemainder
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
	.global Heap_AllocEwram
	.thumb_func
Heap_AllocEwram:
	overlay_veneer Heap_AllocEwramFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global Resource_DecodeType01
	.thumb_func
Resource_DecodeType01:
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
	.global DebugText_Clear
	.thumb_func
DebugText_Clear:
	overlay_veneer DebugText_ClearFar
	.global Func_02004ad0
	.thumb_func
Func_02004ad0:
	overlay_veneer Oam_AddSpriteAtDepthFar
	.global Window_LoadGraphics
	.thumb_func
Window_LoadGraphics:
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
	.global Actor_StartScript
	.thumb_func
Actor_StartScript:
	overlay_veneer Actor_StartScriptFar
	.global Func_02004b28
	.thumb_func
Func_02004b28:
	overlay_veneer Func_080200b0
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Func_02004b38
	.thumb_func
Func_02004b38:
	overlay_veneer Actor_Destroy
	.global Map_UpdateCurrentTileBlock
	.thumb_func
Map_UpdateCurrentTileBlock:
	overlay_veneer Map_UpdateCurrentTileBlockFar
	.global Func_02004b48
	.thumb_func
Func_02004b48:
	overlay_veneer Actor_ResetMotion
	.global Actor_SetMoveTarget
	.thumb_func
Actor_SetMoveTarget:
	overlay_veneer Actor_SetTargetPosition
	.global Script_WaitForEventTimeout
	.thumb_func
Script_WaitForEventTimeout:
	overlay_veneer Actor_CommitPosition
	.global Map_SetWindowCellTile
	.thumb_func
Map_SetWindowCellTile:
	overlay_veneer Map_SetWindowCellTileFar
	.global WorldMap_SetWindowCellTileAndDraw
	.thumb_func
WorldMap_SetWindowCellTileAndDraw:
	overlay_veneer WorldMap_SetWindowCellTileAndDrawFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Map_SetScreenShake
	.thumb_func
Map_SetScreenShake:
	overlay_veneer Map_SetScreenShakeFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02004b88
	.thumb_func
Func_02004b88:
	overlay_veneer WorldMap_IsWalkableFar
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
	.global MessageArgument_Push
	.thumb_func
MessageArgument_Push:
	overlay_veneer MessageArgument_PushFar
	.global Message_CloseFinishedWindows
	.thumb_func
Message_CloseFinishedWindows:
	overlay_veneer Message_CloseFinishedWindowsFar
	.global WindowColor_Apply
	.thumb_func
WindowColor_Apply:
	overlay_veneer WindowColor_ApplyFar
	.global Menu_AnimateSelectionToEntry
	.thumb_func
Menu_AnimateSelectionToEntry:
	overlay_veneer Menu_AnimateSelectionToEntryFar
	.global Unit_AddPp
	.thumb_func
Unit_AddPp:
	overlay_veneer Unit_AddPpFar
	.global Party_CountMembers
	.thumb_func
Party_CountMembers:
	overlay_veneer Party_CountMembersFar
	.global Party_AddMember
	.thumb_func
Party_AddMember:
	overlay_veneer Party_AddMemberFar
	.global Party_RemoveMember
	.thumb_func
Party_RemoveMember:
	overlay_veneer Party_RemoveMemberFar
	.global Djinn_Give
	.thumb_func
Djinn_Give:
	overlay_veneer Djinn_GiveFar
	.global DjinnRecovery_AddStandby
	.thumb_func
DjinnRecovery_AddStandby:
	overlay_veneer DjinnRecovery_AddStandbyFar
	.global Func_02004bf8
	.thumb_func
Func_02004bf8:
	overlay_veneer Party_RestoreHpPpFar
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
	.global Event_Wait
	.thumb_func
Event_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Event_Start
	.thumb_func
Event_Start:
	overlay_veneer Event_StartFar
	.global Event_End
	.thumb_func
Event_End:
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
	.global Actor_SetHorizontalPositionWithTerrain
	.thumb_func
Actor_SetHorizontalPositionWithTerrain:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetHorizontalPositionAndAngleWithTerrain
	.thumb_func
Actor_SetHorizontalPositionAndAngleWithTerrain:
	overlay_veneer Actor_SetHorizontalPositionAndAngleWithTerrainFar
	.global Actor_MoveToObject
	.thumb_func
Actor_MoveToObject:
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
	.global Event_SetMessageId
	.thumb_func
Event_SetMessageId:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Event_ShowMessageThenWait
	.thumb_func
Event_ShowMessageThenWait:
	overlay_veneer Event_ShowMessageThenWaitFar
	.global Event_ShowMessageAndWait
	.thumb_func
Event_ShowMessageAndWait:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Actor_ArmCallbackAndRefresh
	.thumb_func
Actor_ArmCallbackAndRefresh:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Actor_ShowEmoteById
	.thumb_func
Actor_ShowEmoteById:
	overlay_veneer Actor_ShowEmoteByIdFar
	.global FieldEffect_SetAttached
	.thumb_func
FieldEffect_SetAttached:
	overlay_veneer FieldEffect_SetAttachedFar
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_SetSpeedParameters
	.thumb_func
Camera_SetSpeedParameters:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02004d30
	.thumb_func
Func_02004d30:
	overlay_veneer FieldCamera_WaitForArrivalFar
	.global Event_SetDestination
	.thumb_func
Event_SetDestination:
	overlay_veneer Event_SetDestinationFar
	.global Import_EventSetExitRequest
	.thumb_func
Import_EventSetExitRequest:
	overlay_veneer Event_SetExitRequestFar
	.global Field_RequestBattleFromEncounterTable
	.thumb_func
Field_RequestBattleFromEncounterTable:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Field_RequestDjinnBattle
	.thumb_func
Field_RequestDjinnBattle:
	overlay_veneer Field_RequestDjinnBattleFar
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
	overlay_veneer Field_FindNearbyTalkTargetFar
	.global Func_02004d70
	.thumb_func
Func_02004d70:
	overlay_veneer Scene_TalkToActorFar
	.global BattleEffect_ApplyColorToTargetBuffer
	.thumb_func
BattleEffect_ApplyColorToTargetBuffer:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global BattleEffect_ApplyColorToSourceBuffer
	.thumb_func
BattleEffect_ApplyColorToSourceBuffer:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global PaletteFade_StartInterpolation
	.thumb_func
PaletteFade_StartInterpolation:
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
	.global Event_ResolvePendingActions
	.thumb_func
Event_ResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Party_PlaySavedMusic
	.thumb_func
Party_PlaySavedMusic:
	overlay_veneer Party_PlaySavedMusicFar
	.global Scene_ResolveInteractionResult
	.thumb_func
Scene_ResolveInteractionResult:
	overlay_veneer Scene_ResolveInteractionResultFar
	.global Djinn_JoinParty
	.thumb_func
Djinn_JoinParty:
	overlay_veneer Djinn_JoinPartyFar
	.global Actor_LaunchFromLeader
	.thumb_func
Actor_LaunchFromLeader:
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
	.global WorldMap_StartZoom
	.thumb_func
WorldMap_StartZoom:
	overlay_veneer WorldMap_StartZoomFar
	.global Func_02004e00
	.thumb_func
Func_02004e00:
	overlay_veneer Func_080c8640
	.global Func_02004e08
	.thumb_func
Func_02004e08:
	overlay_veneer FlagEntries_GetTableFar
	.global Party_GetLeaderActor
	.thumb_func
Party_GetLeaderActor:
	overlay_veneer Party_GetLeaderActorFar
	.global Actor_ShowItemIconById
	.thumb_func
Actor_ShowItemIconById:
	overlay_veneer Actor_ShowItemIconByIdFar
	.global SceneEvent_FindActiveRecord
	.thumb_func
SceneEvent_FindActiveRecord:
	overlay_veneer SceneEvent_FindActiveRecordFar
	.global Func_02004e28
	.thumb_func
Func_02004e28:
	overlay_veneer DjinnMenu_RunFar
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
