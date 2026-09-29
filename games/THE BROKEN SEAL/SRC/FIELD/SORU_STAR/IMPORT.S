@ Import veneers after the SORU_STAR overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — SORU_STAR overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global IwramSignedDivideEntry
	.thumb_func
IwramSignedDivideEntry:
	overlay_veneer IwramSignedDivide
	overlay_veneer IwramUnsignedDivide
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer WaitFramesFar
	.global Engine_TaskAddCallback
	.thumb_func
Engine_TaskAddCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Engine_RandomNext
	.thumb_func
Engine_RandomNext:
	overlay_veneer Random16Far
	.global CalculateAngleFromCoordinateDelta
	.thumb_func
CalculateAngleFromCoordinateDelta:
	overlay_veneer ArcTan2Far
	overlay_veneer Trig_SinFar
	overlay_veneer Trig_CosFar
	overlay_veneer Runtime_AllocateHeapBlockFar
	.global Engine_AllocateBlock
	.thumb_func
Engine_AllocateBlock:
	overlay_veneer Runtime_AllocateBlockFar
	overlay_veneer Runtime_ReleaseHeapBlockFar
	overlay_veneer VramBlock_LoadCachedFar
	.global Engine_ObjectSetAnimation
	.thumb_func
Engine_ObjectSetAnimation:
	overlay_veneer ObjectDispatch_ApplyArgumentToChildrenFar
	.global Engine_ObjectSetScript
	.thumb_func
Engine_ObjectSetScript:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer Object_CreateFar
	overlay_veneer ObjectDispatch_ReleaseFar
	.global Engine_MapRedraw
	.thumb_func
Engine_MapRedraw:
	overlay_veneer Map_ApplyWorkOriginAndSpanFar
	.global Object_SetMoveTarget
	.thumb_func
Object_SetMoveTarget:
	overlay_veneer Object_SetMoveTargetFar
	.global Engine_MapAnimateCells
	.thumb_func
Engine_MapAnimateCells:
	overlay_veneer Map_PlayMetatileCopySequenceFar
	.global Engine_MapCopyCellsTo
	.thumb_func
Engine_MapCopyCellsTo:
	overlay_veneer Map_CopyMetatileIndicesRectFar
	.global Engine_MapCopyCellAttributes
	.thumb_func
Engine_MapCopyCellAttributes:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Engine_ActorSetSpriteFlags
	.thumb_func
Engine_ActorSetSpriteFlags:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Engine_ObjectSetBlendMode
	.thumb_func
Engine_ObjectSetBlendMode:
	overlay_veneer Animation_SetStateField5Bits2To3Far
	.global Engine_WorkSetValuesIfNonNegative
	.thumb_func
Engine_WorkSetValuesIfNonNegative:
	overlay_veneer Runtime_SetWorkTripleIfNonNegativeFar
	.global Engine_MapRenderWaitForValues
	.thumb_func
Engine_MapRenderWaitForValues:
	overlay_veneer Map_WaitWorkValuesBelow256Far
	.global Engine_MessageShowCentered
	.thumb_func
Engine_MessageShowCentered:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global UiWork_PushValueSlotFar
	.thumb_func
UiWork_PushValueSlotFar:
	overlay_veneer Func_08015120
	.global UiText_ShowCenteredMessage
	.thumb_func
UiText_ShowCenteredMessage:
	overlay_veneer UiText_ShowCenteredMessageFar
	overlay_veneer ItemIcon_LoadTilesFar
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer Owner_GetStateFar
	overlay_veneer Inventory_AddItemFar
	.global PartyInventory_Add
	.thumb_func
PartyInventory_Add:
	overlay_veneer PartyInventory_AddFar
	overlay_veneer Inventory_FindFar
	overlay_veneer PartyInventory_FindOwnerFar
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
	.global Inventory_Count
	.thumb_func
Inventory_Count:
	overlay_veneer Inventory_CountFar
	.global Inventory_Discard
	.thumb_func
Inventory_Discard:
	overlay_veneer Inventory_DiscardFar
	.global PartyInventory_Discard
	.thumb_func
PartyInventory_Discard:
	overlay_veneer PartyInventory_DiscardFar
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
	.global Party_RemoveOwnerRestored
	.thumb_func
Party_RemoveOwnerRestored:
	overlay_veneer Party_RemoveOwnerRestoredFar
	.global Engine_EventChooseYesNo
	.thumb_func
Engine_EventChooseYesNo:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	overlay_veneer Object_GetByIdFar
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global Engine_ActorEnableActionCallback
	.thumb_func
Engine_ActorEnableActionCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global Engine_ActorStop
	.thumb_func
Engine_ActorStop:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global Engine_ActorSetDestination
	.thumb_func
Engine_ActorSetDestination:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global Engine_ActorMoveToAndWait
	.thumb_func
Engine_ActorMoveToAndWait:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global Engine_ActorWalkTo
	.thumb_func
Engine_ActorWalkTo:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global Engine_ActorWalkToAndWait
	.thumb_func
Engine_ActorWalkToAndWait:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global Engine_ActorWaitForMove
	.thumb_func
Engine_ActorWaitForMove:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Engine_ActorSetPosition
	.thumb_func
Engine_ActorSetPosition:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
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
	.global Engine_ObjectSetPalette
	.thumb_func
Engine_ObjectSetPalette:
	overlay_veneer ObjectGroup_SetChildValueFar
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
	.global BattlePres_RunActionThenWaitIfModeZero
	.thumb_func
BattlePres_RunActionThenWaitIfModeZero:
	overlay_veneer BattlePres_RunActionThenWaitIfModeZeroFar
	.global Engine_EventAskYesNo
	.thumb_func
Engine_EventAskYesNo:
	overlay_veneer BattleEventRuntime_ProcessActionFar
	.global Engine_ActorFaceDirection
	.thumb_func
Engine_ActorFaceDirection:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Engine_ActorSetSpritePriority
	.thumb_func
Engine_ActorSetSpritePriority:
	overlay_veneer ObjectMotion_SetActionVariantFar
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
	.global Engine_CameraSetSpeed
	.thumb_func
Engine_CameraSetSpeed:
	overlay_veneer ObjectTable_AllocateAndSetObjectSpeedFar
	.global Engine_CameraMoveTo
	.thumb_func
Engine_CameraMoveTo:
	overlay_veneer Motion_CamBoundsFar
	.global Engine_CameraWaitForMove
	.thumb_func
Engine_CameraWaitForMove:
	overlay_veneer BattleFx_CommitObjectPositionAndWaitFar
	.global Engine_EventRequestExit
	.thumb_func
Engine_EventRequestExit:
	overlay_veneer Event_SetValue170Far
	.global Engine_LeaderCheckAhead
	.thumb_func
Engine_LeaderCheckAhead:
	overlay_veneer Field_TryJumpForwardFar
	.global Engine_ColorBufferApplyTarget
	.thumb_func
Engine_ColorBufferApplyTarget:
	overlay_veneer BattleFx_ApplyColorToTargetBufferFar
	.global Engine_ColorBufferApplySource
	.thumb_func
Engine_ColorBufferApplySource:
	overlay_veneer BattleFx_ApplyColorToSourceBufferFar
	.global Engine_ColorBufferInterpolate
	.thumb_func
Engine_ColorBufferInterpolate:
	overlay_veneer BattleFx_StartBufferInterpolationFar
	.global Engine_EventOpenScreen
	.thumb_func
Engine_EventOpenScreen:
	overlay_veneer Event_SetStatus1c6Far
	.global Engine_EventCloseScreen
	.thumb_func
Engine_EventCloseScreen:
	overlay_veneer Event_ClearStatus1c6Far
	.global Engine_EventWaitForScreen
	.thumb_func
Engine_EventWaitForScreen:
	overlay_veneer Event_WaitValue1c8FramesFar
	overlay_veneer EffectRuntime_RunRisingObjectSequenceFar
	.global Engine_ItemShowFound
	.thumb_func
Engine_ItemShowFound:
	overlay_veneer BattleFx_StartEffectObject22Far
	.global Shop_PickUnitItem
	.thumb_func
Shop_PickUnitItem:
	overlay_veneer Shop_PickUnitItemFar
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer AudioCommand_PlayFar
