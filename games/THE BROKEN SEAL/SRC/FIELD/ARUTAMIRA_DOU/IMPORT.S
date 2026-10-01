@ Import veneers after the ARUTAMIRA_DOU overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — ARUTAMIRA_DOU overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __modsi3
	.thumb_func
__modsi3:
	overlay_veneer IwramSignedRemainder
	.global Math_DivideUnsigned
	.thumb_func
Math_DivideUnsigned:
	overlay_veneer IwramUnsignedDivide
	.global WaitFrames
	.thumb_func
WaitFrames:
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer WaitFramesFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Scheduler_RemoveCallback
	.thumb_func
Scheduler_RemoveCallback:
	overlay_veneer Scheduler_RemoveCallbackFar
	.global Engine_RandomNext
	.thumb_func
Engine_RandomNext:
	overlay_veneer Random16Far
	.global Vector_AddPolarOffset
	.thumb_func
Vector_AddPolarOffset:
	overlay_veneer Vector_AddPolarOffsetFar
	.global Object_SetMode
	.thumb_func
Object_SetMode:
	overlay_veneer ObjectDispatch_ApplyArgumentToChildrenFar
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer Object_CreateFar
	.global Object_SetPosition
	.thumb_func
Object_SetPosition:
	overlay_veneer Object_SetMoveTargetFar
	.global Object_CommitPosition
	.thumb_func
Object_CommitPosition:
	overlay_veneer Script_WaitForEventTimeoutFar
	.global Map_CopyCellAttributeRect
	.thumb_func
Map_CopyCellAttributeRect:
	.global Engine_MapCopyCellAttributes
	.thumb_func
Engine_MapCopyCellAttributes:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Engine_MapCopyCellsTo
	.thumb_func
Engine_MapCopyCellsTo:
	overlay_veneer Func_080091c8
	.global Object_CheckMovementCollision
	.thumb_func
Object_CheckMovementCollision:
	overlay_veneer Func_080091d8
	.global Engine_ActorSetSpriteFlags
	.thumb_func
Engine_ActorSetSpriteFlags:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Engine_WorkSetValuesIfNonNegative
	.thumb_func
Engine_WorkSetValuesIfNonNegative:
	overlay_veneer Runtime_SetWorkTripleIfNonNegativeFar
	.global Engine_MapWaitWorkValuesBelow256
	.thumb_func
Engine_MapWaitWorkValuesBelow256:
	overlay_veneer Map_WaitWorkValuesBelow256Far
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global ObjectGroup_SetChildValueUnlessFifteen
	.thumb_func
ObjectGroup_SetChildValueUnlessFifteen:
	overlay_veneer ObjectGroup_SetChildValueUnlessFifteenFar
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer GameFlag_TestFar
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer GameFlag_SetBitFar
	.global Engine_GameFlagClear
	.thumb_func
Engine_GameFlagClear:
	overlay_veneer GameFlag_ClearBitFar
	.global Engine_PartyRemoveItem
	.thumb_func
Engine_PartyRemoveItem:
	overlay_veneer PartyInventory_DiscardFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	.global Engine_EventWait
	.thumb_func
Engine_EventWait:
	overlay_veneer Battle_WaitMode0Far
	.global Engine_EventBegin
	.thumb_func
Engine_EventBegin:
	overlay_veneer Battle_ResetFar
	.global Engine_EventEnd
	.thumb_func
Engine_EventEnd:
	overlay_veneer BattleFx_FinishActionFar
	.global Engine_EventChooseYesNo
	.thumb_func
Engine_EventChooseYesNo:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global Engine_ActorEnableActionCallback
	.thumb_func
Engine_ActorEnableActionCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global Engine_ActorStartAction
	.thumb_func
Engine_ActorStartAction:
	overlay_veneer Object_RefreshSelectorByIdFar
	.global Engine_ActorStop
	.thumb_func
Engine_ActorStop:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global Engine_ActorSetDestination
	.thumb_func
Engine_ActorSetDestination:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global Engine_ObjectMotionSetPositionAndCommit
	.thumb_func
Engine_ObjectMotionSetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global Engine_ActorWalkToAndWait
	.thumb_func
Engine_ActorWalkToAndWait:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	.global Engine_ActorWaitForMove
	.thumb_func
Engine_ActorWaitForMove:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Engine_ActorSetPosition
	.thumb_func
Engine_ActorSetPosition:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	.global Engine_ActorSetAnimation
	.thumb_func
Engine_ActorSetAnimation:
	overlay_veneer Object_SetModeByIdFar
	.global Engine_ActorSetAnimationAndWait
	.thumb_func
Engine_ActorSetAnimationAndWait:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Engine_ActorJump
	.thumb_func
Engine_ActorJump:
	overlay_veneer ObjectMotion_LaunchFar
	.global Engine_ActorStartRepeatedMotion
	.thumb_func
Engine_ActorStartRepeatedMotion:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Engine_ActorRunRepeatedMotion
	.thumb_func
Engine_ActorRunRepeatedMotion:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Engine_ActorFaceActor
	.thumb_func
Engine_ActorFaceActor:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Engine_ActorFaceEachOther
	.thumb_func
Engine_ActorFaceEachOther:
	overlay_veneer Object_LinkPairFar
	.global Engine_ActorSetChildValue
	.thumb_func
Engine_ActorSetChildValue:
	overlay_veneer ObjectGroup_ConfigureChildValueFar
	.global Engine_EventSetMessage
	.thumb_func
Engine_EventSetMessage:
	overlay_veneer Event_SetValue1d8Far
	.global Engine_EventOpenMessage
	.thumb_func
Engine_EventOpenMessage:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Engine_EventShowMessage
	.thumb_func
Engine_EventShowMessage:
	overlay_veneer BattleEv_RunWaitFar
	.global Engine_ActorFaceDirection
	.thumb_func
Engine_ActorFaceDirection:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Engine_ActorShowEmote
	.thumb_func
Engine_ActorShowEmote:
	overlay_veneer BattleFx_SpawnLinkedFar
	.global Engine_ActorSetAttachedEffect
	.thumb_func
Engine_ActorSetAttachedEffect:
	overlay_veneer BattleFx_ConfigureLinkedObjectFar
	.global Engine_CameraFollowActor
	.thumb_func
Engine_CameraFollowActor:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Engine_CameraMoveTo
	.thumb_func
Engine_CameraMoveTo:
	overlay_veneer Motion_CamBoundsFar
	.global Engine_CameraWaitForMove
	.thumb_func
Engine_CameraWaitForMove:
	overlay_veneer BattleFx_CommitObjectPositionAndWaitFar
	.global Engine_CameraMoveToActor
	.thumb_func
Engine_CameraMoveToActor:
	overlay_veneer Object_PlaceCurrentWithinCameraBoundsFar
	.global BattleFx_SetPhaseRequest
	.thumb_func
BattleFx_SetPhaseRequest:
	overlay_veneer BattleFx_SetPhaseRequestFar
	.global DisplayTransition_InitializeBattleEffectState
	.thumb_func
DisplayTransition_InitializeBattleEffectState:
	overlay_veneer DisplayTransition_InitializeBattleEffectStateFar
	.global Engine_PsynergyPlayEffect
	.thumb_func
Engine_PsynergyPlayEffect:
	overlay_veneer Field_DispatchTypeHandlerFar
	.global Engine_PsynergyBegin
	.thumb_func
Engine_PsynergyBegin:
	overlay_veneer BattleFx_LoadActionEffectResourcesFar
	.global Engine_PsynergySetTarget
	.thumb_func
Engine_PsynergySetTarget:
	overlay_veneer BattleFx_SetupObjectPairFar
	.global Engine_PsynergyLowerHands
	.thumb_func
Engine_PsynergyLowerHands:
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.global Engine_PsynergyRaiseHands
	.thumb_func
Engine_PsynergyRaiseHands:
	overlay_veneer EventObject_InitializeFar
	.global Battle_ResetEffectCounter
	.thumb_func
Battle_ResetEffectCounter:
	overlay_veneer Battle_ResetEffectCounterFar
	.global Audio_PlayCueFromEventWork
	.thumb_func
Audio_PlayCueFromEventWork:
	overlay_veneer Audio_PlayCueFromEventWorkFar
	.global BattleFx_HasReachedTarget
	.thumb_func
BattleFx_HasReachedTarget:
	overlay_veneer BattleFx_HasReachedTargetFar
	.global EffectSlot_SetObjectMode
	.thumb_func
EffectSlot_SetObjectMode:
	overlay_veneer EffectSlot_SetObjectModeFar
	.global EffectSlot_SetCallback
	.thumb_func
EffectSlot_SetCallback:
	overlay_veneer EffectSlot_SetCallbackFar
	.global EffectSlot_Initialize
	.thumb_func
EffectSlot_Initialize:
	overlay_veneer EffectSlot_InitializeFar
	.global BattleFx_ClearOwnedSlot
	.thumb_func
BattleFx_ClearOwnedSlot:
	overlay_veneer BattleFx_ClearOwnedSlotFar
	.global Camera_WorldToScreen
	.thumb_func
Camera_WorldToScreen:
	overlay_veneer Camera_WorldToScreenFar
	.global BattleFx_InitializeSlots
	.thumb_func
BattleFx_InitializeSlots:
	overlay_veneer BattleFx_InitializeSlotsFar
	.global BattleFx_ClearActiveSlotsAndScheduleUpdates
	.thumb_func
BattleFx_ClearActiveSlotsAndScheduleUpdates:
	overlay_veneer BattleFx_ClearActiveSlotsAndScheduleUpdatesFar
	.global Motion_LaunchFromFocusedObject
	.thumb_func
Motion_LaunchFromFocusedObject:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global Engine_ActorWalkBy
	.thumb_func
Engine_ActorWalkBy:
	overlay_veneer ObjectMotion_OffsetPositionAndResetFar
	.global Engine_ActorWalkByAndWait
	.thumb_func
Engine_ActorWalkByAndWait:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Engine_ActorFollow
	.thumb_func
Engine_ActorFollow:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global BattleFx_PlayQueuedSound
	.thumb_func
BattleFx_PlayQueuedSound:
	overlay_veneer BattleFx_PlayQueuedSoundFar
	.global Shop_RestoreSceneTiles
	.thumb_func
Shop_RestoreSceneTiles:
	overlay_veneer Unnamed_080b0840Far
	.global Shop_InitEffect
	.thumb_func
Shop_InitEffect:
	overlay_veneer Shop_InitEffectFar
	.global Audio_PlayCue
	.thumb_func
Audio_PlayCue:
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer AudioCommand_PlayFar
