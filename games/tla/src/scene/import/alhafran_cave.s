.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Vector_AddPolarOffset
	.thumb_func
Vector_AddPolarOffset:
	overlay_veneer Vector_AddPolarOffsetFar
	.global GameFlag_Get
	.thumb_func
GameFlag_Get:
	overlay_veneer GameFlag_GetFar
	.global GameFlag_Set
	.thumb_func
GameFlag_Set:
	overlay_veneer GameFlag_SetFar
	.global GameFlag_Clear
	.thumb_func
GameFlag_Clear:
	overlay_veneer GameFlag_ClearFar
	.global Actor_SelectAnimation
	.thumb_func
Actor_SelectAnimation:
	overlay_veneer Actor_SelectAnimationFar
	.global Actor_SetMoveTarget
	.thumb_func
Actor_SetMoveTarget:
	overlay_veneer Actor_SetMoveTargetFar
	.global Map_CopyCellAttributeRect
	.thumb_func
Map_CopyCellAttributeRect:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global MapGrid_CopyCellPictureCellsRect
	.thumb_func
MapGrid_CopyCellPictureCellsRect:
	overlay_veneer MapGrid_CopyCellPictureCellsRectFar
	.global Actor_SetSpriteFlags
	.thumb_func
Actor_SetSpriteFlags:
	overlay_veneer Actor_SetSpriteFlagsFar
	.global Actor_SetSpriteColorMode
	.thumb_func
Actor_SetSpriteColorMode:
	overlay_veneer Actor_SetSpriteColorModeFar
	.global Func_02001420
	.thumb_func
Func_02001420:
	overlay_veneer MapGrid_SetCellHeightFar
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
	.global Actor_RefreshSelectorById
	.thumb_func
Actor_RefreshSelectorById:
	overlay_veneer Actor_RefreshSelectorByIdFar
	.global Actor_Stop
	.thumb_func
Actor_Stop:
	overlay_veneer Actor_StopFar
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
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
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
	.global FieldCamera_MoveWithinMapBounds
	.thumb_func
FieldCamera_MoveWithinMapBounds:
	overlay_veneer FieldCamera_MoveWithinMapBoundsFar
	.global FieldCamera_WaitForArrival
	.thumb_func
FieldCamera_WaitForArrival:
	overlay_veneer FieldCamera_WaitForArrivalFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02001520
	.thumb_func
Func_02001520:
	overlay_veneer FieldEffect_StartBlankOverlayFar
	.global Event_ResolvePendingActions
	.thumb_func
Event_ResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
	.global FieldEffect_SpawnMoveHand
	.thumb_func
FieldEffect_SpawnMoveHand:
	overlay_veneer FieldEffect_SpawnMoveHandFar
	.global FieldEffect_SnapScaleToFull
	.thumb_func
FieldEffect_SnapScaleToFull:
	overlay_veneer FieldEffect_SnapScaleToFullFar
	.global UpdateRisingParticleBurst
	.thumb_func
UpdateRisingParticleBurst:
	overlay_veneer UpdateRisingParticleBurstFar
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
	.global Actor_SetActionCallback
	.thumb_func
Actor_SetActionCallback:
	overlay_veneer Actor_SetActionCallbackFar
	.global Func_02001568
	.thumb_func
Func_02001568:
	overlay_veneer Rope_InitializeFar
	.global Func_02001570
	.thumb_func
Func_02001570:
	overlay_veneer Rope_PlaceFar
	.global Func_02001578
	.thumb_func
Func_02001578:
	overlay_veneer FlagBlock_PlaceAllMarkingEdgesFar
	.global Func_02001580
	.thumb_func
Func_02001580:
	overlay_veneer FlagBlock_ApplyFlagsFar
	.global Func_02001588
	.thumb_func
Func_02001588:
	overlay_veneer Actor_GetCellIndexFar
	.global Party_GetLeaderActor
	.thumb_func
Party_GetLeaderActor:
	overlay_veneer Party_GetLeaderActorFar
	.global Actor_SetSpriteRotationById
	.thumb_func
Actor_SetSpriteRotationById:
	overlay_veneer Actor_SetSpriteRotationByIdFar
	.global Field_StartColorTransition
	.thumb_func
Field_StartColorTransition:
	overlay_veneer Field_StartColorTransitionFar
	.global Field_StartPaletteTransition
	.thumb_func
Field_StartPaletteTransition:
	overlay_veneer Field_StartPaletteTransitionFar
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
	.global MapGrid_SetCellBehavior
	.thumb_func
MapGrid_SetCellBehavior:
	overlay_veneer MapGrid_SetCellBehaviorFar
	.global Func_020015d8
	.thumb_func
Func_020015d8:
	overlay_veneer Actor_TakeLeaderLayerFar
	.global BattleEffect_FillOverlayTiles
	.thumb_func
BattleEffect_FillOverlayTiles:
	overlay_veneer BattleEffect_FillOverlayTilesFar
	.global Func_020015e8
	.thumb_func
Func_020015e8:
	overlay_veneer EffectSprite_InitFromSlot94Far
	.global BattleEffect_DrawSpriteAtWorldPosition
	.thumb_func
BattleEffect_DrawSpriteAtWorldPosition:
	overlay_veneer BattleEffect_DrawSpriteAtWorldPositionFar
	.global Actor_ApplyRandomChildValues
	.thumb_func
Actor_ApplyRandomChildValues:
	overlay_veneer Actor_ApplyRandomChildValuesFar
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
