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
	.global Func_02005164
	.thumb_func
Func_02005164:
	overlay_veneer Actor_SetMode
	.global Actor_StartScript
	.thumb_func
Actor_StartScript:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Actor_CreateFar
	.global Map_PlayMapEdit
	.thumb_func
Map_PlayMapEdit:
	overlay_veneer Map_PlayMapEditFar
	.global Map_CopyCellPictureIndicesRect
	.thumb_func
Map_CopyCellPictureIndicesRect:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Map_CopyCellAttributeRect
	.thumb_func
Map_CopyCellAttributeRect:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global MapGrid_CopyCellPictureCellsRect
	.thumb_func
MapGrid_CopyCellPictureCellsRect:
	overlay_veneer MapGrid_CopyCellPictureCellsRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
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
	.global Actor_ResetAndSetPosition
	.thumb_func
Actor_ResetAndSetPosition:
	overlay_veneer Actor_ResetAndSetPositionFar
	.global Actor_WalkToAndWaitById
	.thumb_func
Actor_WalkToAndWaitById:
	overlay_veneer Actor_WalkToAndWaitByIdFar
	.global Actor_CenterOnColumn
	.thumb_func
Actor_CenterOnColumn:
	overlay_veneer Actor_CenterOnColumnFar
	.global Actor_SetDestinationOffsetById
	.thumb_func
Actor_SetDestinationOffsetById:
	overlay_veneer Actor_SetDestinationOffsetByIdFar
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
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
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
	.global Actor_FaceActorById
	.thumb_func
Actor_FaceActorById:
	overlay_veneer Actor_FaceActorByIdFar
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
	.global Battle_ShowPairedUnitWorkAndWait
	.thumb_func
Battle_ShowPairedUnitWorkAndWait:
	overlay_veneer Battle_ShowPairedUnitWorkAndWaitFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
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
	.global Func_020052b4
	.thumb_func
Func_020052b4:
	overlay_veneer FieldCamera_WaitForArrivalFar
	.global Func_020052bc
	.thumb_func
Func_020052bc:
	overlay_veneer FieldCamera_MoveToActorByIdFar
	.global Import_EventSetExitRequest
	.thumb_func
Import_EventSetExitRequest:
	overlay_veneer Event_SetExitRequestFar
	.global BattleEffect_ApplyColorToTargetBuffer
	.thumb_func
BattleEffect_ApplyColorToTargetBuffer:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
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
	.global Func_020052f4
	.thumb_func
Func_020052f4:
	overlay_veneer Func_080c8480
	.global FieldPsynergy_RunHandler
	.thumb_func
FieldPsynergy_RunHandler:
	overlay_veneer Field_DispatchTypeHandlerFar
	.global BattleEffect_LoadActionEffectResources
	.thumb_func
BattleEffect_LoadActionEffectResources:
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.global FieldPsynergy_SetupActorPair
	.thumb_func
FieldPsynergy_SetupActorPair:
	overlay_veneer BattleEffect_SetupObjectPairFar
	.global BattleEffect_StopMainActor
	.thumb_func
BattleEffect_StopMainActor:
	overlay_veneer BattleEffect_StopMainActorFar
	.global BattleEffect_CleanupSceneObjects
	.thumb_func
BattleEffect_CleanupSceneObjects:
	overlay_veneer BattleEffect_CleanupSceneObjectsFar
	.global Event_ResolvePendingActions
	.thumb_func
Event_ResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
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
	.global Actor_WalkByAndWaitById
	.thumb_func
Actor_WalkByAndWaitById:
	overlay_veneer Actor_WalkByAndWaitByIdFar
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Func_02005354
	.thumb_func
Func_02005354:
	overlay_veneer Func_080c86c0
	.global Func_0200535c
	.thumb_func
Func_0200535c:
	overlay_veneer Func_080c86d0
	.global Func_02005364
	.thumb_func
Func_02005364:
	overlay_veneer Func_080c86d8
	.global Func_0200536c
	.thumb_func
Func_0200536c:
	overlay_veneer Func_080c8718
	.global Func_02005374
	.thumb_func
Func_02005374:
	overlay_veneer Func_080c8720
	.global Func_0200537c
	.thumb_func
Func_0200537c:
	overlay_veneer Func_080c8728
	.global Party_GetLeaderActor
	.thumb_func
Party_GetLeaderActor:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_0200538c
	.thumb_func
Func_0200538c:
	overlay_veneer Func_080c8968
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
