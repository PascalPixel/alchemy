	.include "games/common/include/game/ed_asm.h"
.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer WaitFramesFar
	.global Engine_TaskAddCallback
	.thumb_func
Engine_TaskAddCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Engine_TaskRemoveCallback
	.thumb_func
Engine_TaskRemoveCallback:
	overlay_veneer Scheduler_RemoveCallbackFar
	.global Engine_RandomNext
	.thumb_func
Engine_RandomNext:
	overlay_veneer Random16Far
	.if EDITION_INTERNATIONAL
	.global Vector_AddPolarOffset
	.thumb_func
Vector_AddPolarOffset:
	overlay_veneer Vector_AddPolarOffsetFar
	.endif
	.global Runtime_SetIrqHandler
	.thumb_func
Runtime_SetIrqHandler:
	overlay_veneer Runtime_SetIrqHandlerFar
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
	.global Engine_ObjectDispatchRelease
	.thumb_func
Engine_ObjectDispatchRelease:
	overlay_veneer ObjectDispatch_ReleaseFar
	.global Engine_MapRedraw
	.thumb_func
Engine_MapRedraw:
	overlay_veneer Map_ApplyWorkOriginAndSpanFar
	.global Engine_ObjectSetPosition
	.thumb_func
Engine_ObjectSetPosition:
	overlay_veneer Object_SetMoveTargetFar
	.global Object_CommitPosition
	.thumb_func
Object_CommitPosition:
	overlay_veneer Script_WaitForEventTimeoutFar
	.global Engine_MapAnimateCells
	.thumb_func
Engine_MapAnimateCells:
	overlay_veneer Map_PlayMetatileCopySequenceFar
	.global Engine_MapCopyCellsTo
	.thumb_func
Engine_MapCopyCellsTo:
	overlay_veneer Map_CopyMetatileIndicesRectFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global GetMapCellCollision
	.thumb_func
GetMapCellCollision:
	overlay_veneer GetMapCellCollisionFar
	.global Engine_MapCopyCells
	.thumb_func
Engine_MapCopyCells:
	overlay_veneer Map_CopyMetatileCellsRectFar
	.global Engine_MapCopyCellAttributes
	.thumb_func
Engine_MapCopyCellAttributes:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Object_CheckMovementCollision
	.thumb_func
Object_CheckMovementCollision:
	overlay_veneer Map_CheckObjectStepFar
	.global Engine_ActorSetSpriteFlags
	.thumb_func
Engine_ActorSetSpriteFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Engine_ObjectSetBlendMode
	.thumb_func
Engine_ObjectSetBlendMode:
	overlay_veneer ObjectDispatch_SetChildBlendModeFar
	.global Engine_MessageShowCentered
	.thumb_func
Engine_MessageShowCentered:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
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
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global Engine_ActorEnableActionCallback
	.thumb_func
Engine_ActorEnableActionCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global Engine_ActorMoveToAndWait
	.thumb_func
Engine_ActorMoveToAndWait:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global Engine_ActorSetDestinationOffset
	.thumb_func
Engine_ActorSetDestinationOffset:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
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
	.global ObjectMotion_WaitForAnimationChange
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	overlay_veneer ObjectMotion_WaitForAnimationChangeFar
	.global Engine_ActorSetChildValue
	.thumb_func
Engine_ActorSetChildValue:
	overlay_veneer ObjectGroup_ConfigureChildValueFar
	.global ObjectGroup_SetChildValue
	.thumb_func
ObjectGroup_SetChildValue:
	overlay_veneer ObjectGroup_SetChildValueFar
	.global Engine_ActorSetSpritePriority
	.thumb_func
Engine_ActorSetSpritePriority:
	overlay_veneer ObjectMotion_SetActionVariantFar
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
	.global FieldEffect_UpdateGridPlacement
	.thumb_func
FieldEffect_UpdateGridPlacement:
	overlay_veneer FieldEffect_UpdateGridPlacementFar
	.global battle_owner_69
	.thumb_func
battle_owner_69:
	overlay_veneer battle_owner_69Far
	.global BattleFx_StartFadeOverlay
	.thumb_func
BattleFx_StartFadeOverlay:
	overlay_veneer BattleFx_StartFadeOverlayFar
	.global BattleFx_PlayQueuedSound
	.thumb_func
BattleFx_PlayQueuedSound:
	overlay_veneer BattleFx_PlayQueuedSoundFar
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer AudioCommand_PlayFar
