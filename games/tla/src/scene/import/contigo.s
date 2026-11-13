.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __modsi3
	.thumb_func
__modsi3:
	overlay_veneer IwramSignedRemainder
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
	.global ArcTan2
	.thumb_func
ArcTan2:
	overlay_veneer ArcTan2Far
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Trig_SinFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Trig_CosFar
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
	.global Actor_SelectAnimation
	.thumb_func
Actor_SelectAnimation:
	overlay_veneer Actor_SelectAnimationFar
	.global Actor_StartScript
	.thumb_func
Actor_StartScript:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Actor_CreateFar
	.global Import_ActorDestroy
	.thumb_func
Import_ActorDestroy:
	overlay_veneer Actor_ReleaseFar
	.global Map_CenterOnFollowedPoint
	.thumb_func
Map_CenterOnFollowedPoint:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global MapGrid_CopyCellPictureIndicesRect
	.thumb_func
MapGrid_CopyCellPictureIndicesRect:
	overlay_veneer MapGrid_CopyCellPictureIndicesRectFar
	.global Map_CopyCellAttributeRect
	.thumb_func
Map_CopyCellAttributeRect:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSpriteFlagsFar
	.global MapGrid_SetScreenShake
	.thumb_func
MapGrid_SetScreenShake:
	overlay_veneer MapGrid_SetScreenShakeFar
	.global Actor_SetSpriteColorMode
	.thumb_func
Actor_SetSpriteColorMode:
	overlay_veneer Actor_SetSpriteColorModeFar
	.global Actor_SetSpritePriorityDirect
	.thumb_func
Actor_SetSpritePriorityDirect:
	overlay_veneer Actor_SetSpritePriorityDirectFar
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global MessageArgument_Push
	.thumb_func
MessageArgument_Push:
	overlay_veneer MessageArgument_PushFar
	.global Func_02004044
	.thumb_func
Func_02004044:
	overlay_veneer Func_080ad240
	.global Party_GetAverageLevel
	.thumb_func
Party_GetAverageLevel:
	overlay_veneer Party_GetAverageLevelFar
	.global PartyInventory_CountItem
	.thumb_func
PartyInventory_CountItem:
	overlay_veneer PartyInventory_CountItemFar
	.global PartyInventory_CountFreeSlots
	.thumb_func
PartyInventory_CountFreeSlots:
	overlay_veneer PartyInventory_CountFreeSlotsFar
	.global Event_Wait
	.thumb_func
Event_Wait:
	overlay_veneer Event_WaitFar
	.global Event_Start
	.thumb_func
Event_Start:
	overlay_veneer Event_StartFar
	.global Event_End
	.thumb_func
Event_End:
	overlay_veneer Event_EndFar
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
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
	.global Actor_MoveToAndWaitById
	.thumb_func
Actor_MoveToAndWaitById:
	overlay_veneer Actor_MoveToAndWaitByIdFar
	.global Actor_WalkToAndWaitById
	.thumb_func
Actor_WalkToAndWaitById:
	overlay_veneer Actor_WalkToAndWaitByIdFar
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
	.global Event_AskYesNoAndShowResponse
	.thumb_func
Event_AskYesNoAndShowResponse:
	overlay_veneer Event_AskYesNoAndShowResponseFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Actor_FaceDirectionById
	.thumb_func
Actor_FaceDirectionById:
	overlay_veneer Actor_FaceDirectionByIdFar
	.global Event_ShowTwoMessagesAndWait
	.thumb_func
Event_ShowTwoMessagesAndWait:
	overlay_veneer Event_ShowTwoMessagesAndWaitFar
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
	.global Func_0200419c
	.thumb_func
Func_0200419c:
	overlay_veneer FieldCamera_WaitForArrivalFar
	.global MapGrid_SetCameraBounds
	.thumb_func
MapGrid_SetCameraBounds:
	overlay_veneer MapGrid_SetCameraBoundsFar
	.global Event_SetDestination
	.thumb_func
Event_SetDestination:
	overlay_veneer Event_SetDestinationFar
	.global Import_EventSetExitRequest
	.thumb_func
Import_EventSetExitRequest:
	overlay_veneer Event_SetExitRequestFar
	.global Event_SetAfterBattleDestination
	.thumb_func
Event_SetAfterBattleDestination:
	overlay_veneer Event_SetAfterBattleDestinationFar
	.global Event_LeaveFor
	.thumb_func
Event_LeaveFor:
	overlay_veneer Event_LeaveForFar
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
	overlay_veneer PaletteFade_StartInterpolationFar
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
	.global ItemDisplay_ShowItem
	.thumb_func
ItemDisplay_ShowItem:
	overlay_veneer ItemDisplay_ShowItemFar
	.global Event_ResolvePendingActions
	.thumb_func
Event_ResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Scene_PlayMusic
	.thumb_func
Scene_PlayMusic:
	overlay_veneer Scene_PlayMusicFar
	.global Djinn_JoinParty
	.thumb_func
Djinn_JoinParty:
	overlay_veneer Djinn_JoinPartyFar
	.global Camera_WorldToScreen
	.thumb_func
Camera_WorldToScreen:
	overlay_veneer Camera_WorldToScreenFar
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global ObjectTable_Snapshot
	.thumb_func
ObjectTable_Snapshot:
	overlay_veneer ObjectTable_SnapshotFar
	.global Actor_Spawn
	.thumb_func
Actor_Spawn:
	overlay_veneer Actor_SpawnFar
	.global Func_0200423c
	.thumb_func
Func_0200423c:
	overlay_veneer Func_080c8760
	.global Func_02004244
	.thumb_func
Func_02004244:
	overlay_veneer Func_080c87c0
	.global Func_0200424c
	.thumb_func
Func_0200424c:
	overlay_veneer Func_080c87c8
	.global BattleEffect_GetObjectPriority
	.thumb_func
BattleEffect_GetObjectPriority:
	overlay_veneer BattleEffect_GetObjectPriorityFar
	.global BattleEffect_GetObjectDepth
	.thumb_func
BattleEffect_GetObjectDepth:
	overlay_veneer BattleEffect_GetObjectDepthFar
	.global BattleEffect_StartArcEffect
	.thumb_func
BattleEffect_StartArcEffect:
	overlay_veneer BattleEffect_StartArcEffectFar
	.global BattleEffect_EndArcEffect
	.thumb_func
BattleEffect_EndArcEffect:
	overlay_veneer BattleEffect_EndArcEffectFar
	.global BattleEffect_WaitForArcGroups
	.thumb_func
BattleEffect_WaitForArcGroups:
	overlay_veneer BattleEffect_WaitForArcGroupsFar
	.global BattleEffect_SetObjectUpdateMode
	.thumb_func
BattleEffect_SetObjectUpdateMode:
	overlay_veneer BattleEffect_SetObjectUpdateModeFar
	.global Func_02004284
	.thumb_func
Func_02004284:
	overlay_veneer Func_080c88d8
	.global Func_0200428c
	.thumb_func
Func_0200428c:
	overlay_veneer Func_080c88e8
	.global Shop_Run
	.thumb_func
Shop_Run:
	overlay_veneer Shop_RunFar
	.global Sanctum_Run
	.thumb_func
Sanctum_Run:
	overlay_veneer Sanctum_RunFar
	.global Inn_CheckIn
	.thumb_func
Inn_CheckIn:
	overlay_veneer Inn_CheckInFar
	.global Sound_WaitForJingle
	.thumb_func
Sound_WaitForJingle:
	overlay_veneer Sound_WaitForJingleFar
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
