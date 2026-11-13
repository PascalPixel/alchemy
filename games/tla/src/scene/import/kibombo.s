.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __udivsi3
	.thumb_func
__udivsi3:
	overlay_veneer IwramUnsignedDivide
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
	overlay_veneer Trig_SinFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Trig_CosFar
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
	.global VramCache_FreeEntry
	.thumb_func
VramCache_FreeEntry:
	overlay_veneer VramCache_FreeEntryFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
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
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Map_CenterOnFollowedPoint
	.thumb_func
Map_CenterOnFollowedPoint:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Actor_SetMoveTarget
	.thumb_func
Actor_SetMoveTarget:
	overlay_veneer Actor_SetMoveTargetFar
	.global Actor_WaitForMove
	.thumb_func
Actor_WaitForMove:
	overlay_veneer Actor_WaitForMoveFar
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
	.global Actor_SetBlendMode
	.thumb_func
Actor_SetBlendMode:
	overlay_veneer Actor_SetBlendModeFar
	.global Icon_DrawItem
	.thumb_func
Icon_DrawItem:
	overlay_veneer Icon_DrawItemFar
	.global Func_02005a8c
	.thumb_func
Func_02005a8c:
	overlay_veneer Func_080ad228
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
	.global Event_PrepareObjectAndApplyValue
	.thumb_func
Event_PrepareObjectAndApplyValue:
	overlay_veneer Event_PrepareObjectAndApplyValueFar
	.global Party_RemoveMemberRestored
	.thumb_func
Party_RemoveMemberRestored:
	overlay_veneer Party_RemoveMemberRestoredFar
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
	.global Actor_SetDestinationById
	.thumb_func
Actor_SetDestinationById:
	overlay_veneer Actor_SetDestinationByIdFar
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
	.global Actor_FaceActorById
	.thumb_func
Actor_FaceActorById:
	overlay_veneer Actor_FaceActorByIdFar
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
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02005b8c
	.thumb_func
Func_02005b8c:
	overlay_veneer Func_080c8208
	.global Actor_ShowEmoteById
	.thumb_func
Actor_ShowEmoteById:
	overlay_veneer Actor_ShowEmoteByIdFar
	.global FieldEffect_SetAttached
	.thumb_func
FieldEffect_SetAttached:
	overlay_veneer FieldEffect_SetAttachedFar
	.global Camera_SetSpeedParameters
	.thumb_func
Camera_SetSpeedParameters:
	overlay_veneer Camera_SetSpeedParametersFar
	.global FieldCamera_MoveWithinMapBounds
	.thumb_func
FieldCamera_MoveWithinMapBounds:
	overlay_veneer FieldCamera_MoveWithinMapBoundsFar
	.global Import_FieldCameraWaitForArrival
	.thumb_func
Import_FieldCameraWaitForArrival:
	overlay_veneer FieldCamera_WaitForArrivalFar
	.global FieldCamera_GetActor
	.thumb_func
FieldCamera_GetActor:
	overlay_veneer FieldCamera_GetActorFar
	.global Import_EventSetExitRequest
	.thumb_func
Import_EventSetExitRequest:
	overlay_veneer Event_SetExitRequestFar
	.global Field_TryJumpForward
	.thumb_func
Field_TryJumpForward:
	overlay_veneer Field_TryJumpForwardFar
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
	.global Func_02005c04
	.thumb_func
Func_02005c04:
	overlay_veneer Func_080c8480
	.global FieldPsynergy_RunHandler
	.thumb_func
FieldPsynergy_RunHandler:
	overlay_veneer FieldPsynergy_RunHandlerFar
	.global BattleEffect_LoadActionEffectResources
	.thumb_func
BattleEffect_LoadActionEffectResources:
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.global Import_BattleEffectSetupObjectPair
	.thumb_func
Import_BattleEffectSetupObjectPair:
	overlay_veneer FieldPsynergy_SetupActorPairFar
	.global BattleEffect_StopMainActor
	.thumb_func
BattleEffect_StopMainActor:
	overlay_veneer BattleEffect_StopMainActorFar
	.global BattleEffect_CleanupSceneObjects
	.thumb_func
BattleEffect_CleanupSceneObjects:
	overlay_veneer BattleEffect_CleanupSceneObjectsFar
	.global EventObject_Initialize
	.thumb_func
EventObject_Initialize:
	overlay_veneer EventObject_InitializeFar
	.global Event_ResolvePendingActions
	.thumb_func
Event_ResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Sprite_ShareVramCacheEntry
	.thumb_func
Sprite_ShareVramCacheEntry:
	overlay_veneer Sprite_ShareVramCacheEntryFar
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Func_02005c54
	.thumb_func
Func_02005c54:
	overlay_veneer Func_080c86a8
	.global Func_02005c5c
	.thumb_func
Func_02005c5c:
	overlay_veneer Func_080c86e8
	.global Func_02005c64
	.thumb_func
Func_02005c64:
	overlay_veneer Func_080c86f8
	.global Func_02005c6c
	.thumb_func
Func_02005c6c:
	overlay_veneer Func_080c8708
	.global Func_02005c74
	.thumb_func
Func_02005c74:
	overlay_veneer Func_080c87b0
	.global Func_02005c7c
	.thumb_func
Func_02005c7c:
	overlay_veneer Func_080c8930
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
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
