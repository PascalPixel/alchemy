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
	.global Decompress_TaggedLz
	.thumb_func
Decompress_TaggedLz:
	overlay_veneer Decompress_TaggedLzFar
	.global VramCache_FreeEntry
	.thumb_func
VramCache_FreeEntry:
	overlay_veneer VramCache_FreeEntryFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global VramCache_FindFreeEntry
	.thumb_func
VramCache_FindFreeEntry:
	overlay_veneer VramCache_FindFreeEntryFar
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
	.global IoWriteQueue_Store16
	.thumb_func
IoWriteQueue_Store16:
	overlay_veneer IoWriteQueue_Store16Far
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
	overlay_veneer GameFlag_SetFar
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearFar
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
	overlay_veneer Actor_CreateFar
	.global Func_02004b38
	.thumb_func
Func_02004b38:
	overlay_veneer Actor_Destroy
	.global WorldMap_UpdateCurrentTileBlock
	.thumb_func
WorldMap_UpdateCurrentTileBlock:
	overlay_veneer WorldMap_UpdateCurrentTileBlockFar
	.global Func_02004b48
	.thumb_func
Func_02004b48:
	overlay_veneer Actor_ResetMotion
	.global Actor_SetMoveTarget
	.thumb_func
Actor_SetMoveTarget:
	overlay_veneer Actor_SetTargetPosition
	.global Actor_WaitForMove
	.thumb_func
Actor_WaitForMove:
	overlay_veneer Actor_CommitPosition
	.global WorldMap_SetWindowCellTile
	.thumb_func
WorldMap_SetWindowCellTile:
	overlay_veneer WorldMap_SetWindowCellTileFar
	.global WorldMap_SetWindowCellTileAndDraw
	.thumb_func
WorldMap_SetWindowCellTileAndDraw:
	overlay_veneer WorldMap_SetWindowCellTileAndDrawFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global MapGrid_SetScreenShake
	.thumb_func
MapGrid_SetScreenShake:
	overlay_veneer MapGrid_SetScreenShakeFar
	.global Actor_SetSpriteColorMode
	.thumb_func
Actor_SetSpriteColorMode:
	overlay_veneer Actor_SetSpriteColorModeFar
	.global Func_02004b88
	.thumb_func
Func_02004b88:
	overlay_veneer WorldMap_IsWalkableFar
	.global Func_02004b90
	.thumb_func
Func_02004b90:
	overlay_veneer Func_08020350
	.global WorldMap_GetCollision
	.thumb_func
WorldMap_GetCollision:
	overlay_veneer WorldMap_GetCollisionFar
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
	.global Actor_SetDestinationById
	.thumb_func
Actor_SetDestinationById:
	overlay_veneer Actor_SetDestinationByIdFar
	.global Actor_MoveToAndWaitById
	.thumb_func
Actor_MoveToAndWaitById:
	overlay_veneer Actor_MoveToAndWaitByIdFar
	.global Actor_WalkToById
	.thumb_func
Actor_WalkToById:
	overlay_veneer Actor_WalkToByIdFar
	.global Actor_WalkToAndWaitById
	.thumb_func
Actor_WalkToAndWaitById:
	overlay_veneer Actor_WalkToAndWaitByIdFar
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
	.global Actor_FaceActorById
	.thumb_func
Actor_FaceActorById:
	overlay_veneer Actor_FaceActorByIdFar
	.global Actor_TurnToFaceEachOther
	.thumb_func
Actor_TurnToFaceEachOther:
	overlay_veneer Actor_TurnToFaceEachOtherFar
	.global Actor_SetSpriteColor
	.thumb_func
Actor_SetSpriteColor:
	overlay_veneer Actor_SetSpriteColorFar
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
	.global Actor_FaceDirectionById
	.thumb_func
Actor_FaceDirectionById:
	overlay_veneer Actor_FaceDirectionByIdFar
	.global Actor_ShowEmoteById
	.thumb_func
Actor_ShowEmoteById:
	overlay_veneer Actor_ShowEmoteByIdFar
	.global FieldEffect_SetAttached
	.thumb_func
FieldEffect_SetAttached:
	overlay_veneer FieldEffect_SetAttachedFar
	.global FieldCamera_FollowActor
	.thumb_func
FieldCamera_FollowActor:
	overlay_veneer FieldCamera_FollowActorFar
	.global Camera_SetSpeedParameters
	.thumb_func
Camera_SetSpeedParameters:
	overlay_veneer Camera_SetSpeedParametersFar
	.global FieldCamera_MoveWithinMapBounds
	.thumb_func
FieldCamera_MoveWithinMapBounds:
	overlay_veneer FieldCamera_MoveWithinMapBoundsFar
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
	.global Event_SetAfterBattleDestination
	.thumb_func
Event_SetAfterBattleDestination:
	overlay_veneer Event_SetAfterBattleDestinationFar
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
	overlay_veneer WorldMap_FreezeFar
	.global Func_02004db0
	.thumb_func
Func_02004db0:
	overlay_veneer Func_080c8438
	.global Event_ResolvePendingActions
	.thumb_func
Event_ResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Scene_PlayMusic
	.thumb_func
Scene_PlayMusic:
	overlay_veneer Scene_PlayMusicFar
	.global Scene_ChooseMusic
	.thumb_func
Scene_ChooseMusic:
	overlay_veneer Scene_ChooseMusicFar
	.global Djinn_JoinParty
	.thumb_func
Djinn_JoinParty:
	overlay_veneer Djinn_JoinPartyFar
	.global Actor_LaunchFromLeader
	.thumb_func
Actor_LaunchFromLeader:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global Actor_WalkByById
	.thumb_func
Actor_WalkByById:
	overlay_veneer Actor_WalkByByIdFar
	.global Actor_WalkByAndWaitById
	.thumb_func
Actor_WalkByAndWaitById:
	overlay_veneer Actor_WalkByAndWaitByIdFar
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
	overlay_veneer Event_WaitForZoomTransitionFar
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
