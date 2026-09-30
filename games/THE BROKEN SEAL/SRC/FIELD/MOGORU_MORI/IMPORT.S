@ Import veneers after the MOGORU_MORI overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — MOGORU_MORI overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global IwramSignedDivideEntry
	.thumb_func
IwramSignedDivideEntry:
	overlay_veneer IwramSignedDivide
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Engine_RandomNext
	.thumb_func
Engine_RandomNext:
	overlay_veneer Random16Far
	.global Engine_MathSin
	.thumb_func
Engine_MathSin:
	overlay_veneer Trig_SinFar
	.global Engine_MathCos
	.thumb_func
Engine_MathCos:
	overlay_veneer Trig_CosFar
	.global AllocateEffectTransfer
	.thumb_func
AllocateEffectTransfer:
	overlay_veneer Runtime_AllocateHeapBlockFar
	.global Engine_HeapRelease
	.thumb_func
Engine_HeapRelease:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global Engine_VramLoad
	.thumb_func
Engine_VramLoad:
	overlay_veneer VramBlock_LoadCachedFar
	.global Object_SetMode
	.thumb_func
Object_SetMode:
	.global Object_SetMode
	.thumb_func
Object_SetMode:
	overlay_veneer ObjectDispatch_ApplyArgumentToChildrenFar
	.global Engine_ObjectSetScript
	.thumb_func
Engine_ObjectSetScript:
	overlay_veneer ObjectDispatch_InitializeFar
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
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Object_CheckMovementCollision
	.thumb_func
Object_CheckMovementCollision:
	overlay_veneer Func_080091d8
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
	.global Engine_MapWaitWorkValuesBelow256
	.thumb_func
Engine_MapWaitWorkValuesBelow256:
	overlay_veneer Map_WaitWorkValuesBelow256Far
	.global Engine_MessageShowCentered
	.thumb_func
Engine_MessageShowCentered:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Engine_ItemLoadIcon
	.thumb_func
Engine_ItemLoadIcon:
	overlay_veneer ItemIcon_LoadTilesFar
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer GameFlag_TestFar
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer GameFlag_SetBitFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Engine_EventBegin
	.thumb_func
Engine_EventBegin:
	overlay_veneer Battle_ResetFar
	.global Engine_EventEnd
	.thumb_func
Engine_EventEnd:
	overlay_veneer BattleFx_FinishActionFar
	.global Engine_PartyGiveItem
	.thumb_func
Engine_PartyGiveItem:
	overlay_veneer PartyInventory_GiveItemFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global Engine_ActorEnableActionCallback
	.thumb_func
Engine_ActorEnableActionCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global Engine_ActorMoveToAndWait
	.thumb_func
Engine_ActorMoveToAndWait:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global Engine_ActorWalkTo
	.thumb_func
Engine_ActorWalkTo:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Engine_ActorSetPosition
	.thumb_func
Engine_ActorSetPosition:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
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
	.global ObjectGroup_SetChildValue
	.thumb_func
ObjectGroup_SetChildValue:
	overlay_veneer ObjectGroup_SetChildValueFar
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
	.global Engine_CameraMoveToActor
	.thumb_func
Engine_CameraMoveToActor:
	overlay_veneer Object_PlaceCurrentWithinCameraBoundsFar
	.global BattleFx_SetWeightedResult
	.thumb_func
BattleFx_SetWeightedResult:
	overlay_veneer BattleFx_SetWeightedResultFar
	.global BattleFx_SetPhaseRequest
	.thumb_func
BattleFx_SetPhaseRequest:
	overlay_veneer BattleFx_SetPhaseRequestFar
	.global Party_SetFields1ceAnd1d0
	.thumb_func
Party_SetFields1ceAnd1d0:
	overlay_veneer Party_SetFields1ceAnd1d0Far
	.global Engine_EventOpenScreen
	.thumb_func
Engine_EventOpenScreen:
	overlay_veneer Event_SetStatus1c6Far
	.global Engine_EventWaitForScreen
	.thumb_func
Engine_EventWaitForScreen:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Engine_ItemShowFound
	.thumb_func
Engine_ItemShowFound:
	overlay_veneer BattleFx_StartEffectObject22Far
	.global BattleFx_PlayQueuedSound
	.thumb_func
BattleFx_PlayQueuedSound:
	overlay_veneer BattleFx_PlayQueuedSoundFar
	.global Audio_PlayCue
	.thumb_func
Audio_PlayCue:
	overlay_veneer AudioCommand_PlayFar
