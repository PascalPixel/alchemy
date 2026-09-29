@ Import veneers after the KORIMA_MAGARI overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — KORIMA_MAGARI overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global IwramSignedDivide
	.thumb_func
IwramSignedDivide:
	overlay_veneer IwramSignedDivideArm
	overlay_veneer WaitFramesFar
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	overlay_veneer Scheduler_RemoveCallbackFar
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	overlay_veneer Vector_AddPolarOffsetFar
	overlay_veneer Runtime_SetIrqHandlerFar
	overlay_veneer Runtime_AllocateBlockFar
	.global Engine_ObjectSetAnimation
	.thumb_func
Engine_ObjectSetAnimation:
	overlay_veneer ObjectDispatch_ApplyArgumentToChildrenFar
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer Object_CreateFar
	overlay_veneer ObjectDispatch_InitFromTable4WithArgumentFar
	overlay_veneer Map_ApplyWorkOriginAndSpanFar
	overlay_veneer Object_SetMoveTargetFar
	overlay_veneer Script_WaitForEventTimeoutFar
	.global Map_GetTerrainHeightFar
	.thumb_func
Map_GetTerrainHeightFar:
	overlay_veneer Func_080091a8
	overlay_veneer Map_CopyMetatileCellsRectFar
	.global Map_CopyCellAttributeRect
	.thumb_func
Map_CopyCellAttributeRect:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Engine_ActorSetSpriteFlags
	.thumb_func
Engine_ActorSetSpriteFlags:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	overlay_veneer GameFlag_TestFar
	overlay_veneer Battle_WaitMode0Far
	overlay_veneer Battle_ResetFar
	overlay_veneer BattleFx_FinishActionFar
	overlay_veneer Object_GetByIdFar
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer ObjectTable_AllocateAndSetObjectSpeedFar
	overlay_veneer Motion_CamBoundsFar
	overlay_veneer BattleFx_CommitObjectPositionAndWaitFar
	.global Field_TryJumpForward
	.thumb_func
Field_TryJumpForward:
	overlay_veneer Field_TryJumpForwardFar
	.global Engine_ColorBufferApplyTarget
	.thumb_func
Engine_ColorBufferApplyTarget:
	overlay_veneer BattleFx_ApplyColorToTargetBufferFar
	.global Engine_ColorBufferApplySource
	.thumb_func
Engine_ColorBufferApplySource:
	overlay_veneer BattleFx_ApplyColorToSourceBufferFar
	overlay_veneer AudioCommand_PlayFar
