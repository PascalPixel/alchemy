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
	.global Task_Remove
	.thumb_func
Task_Remove:
	overlay_veneer Task_RemoveFar
	.global Random16
	.thumb_func
Random16:
	overlay_veneer Random16Far
	.global Trig_Sin
	.thumb_func
Trig_Sin:
	overlay_veneer Trig_SinFar
	.global Trig_Cos
	.thumb_func
Trig_Cos:
	overlay_veneer Trig_CosFar
	.global Heap_Alloc
	.thumb_func
Heap_Alloc:
	overlay_veneer Heap_AllocFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Heap_ReleaseFar
	.global Decompress_TaggedLz
	.thumb_func
Decompress_TaggedLz:
	overlay_veneer Decompress_TaggedLzFar
	.global DebugText_Clear
	.thumb_func
DebugText_Clear:
	overlay_veneer DebugText_ClearFar
	.global Window_LoadGraphics
	.thumb_func
Window_LoadGraphics:
	overlay_veneer Window_LoadGraphicsFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
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
	.global Actor_SetAnimationSpeed
	.thumb_func
Actor_SetAnimationSpeed:
	overlay_veneer Actor_SetAnimationSpeedFar
	.global Actor_StartScript
	.thumb_func
Actor_StartScript:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Actor_CreateFar
	.global Actor_Release
	.thumb_func
Actor_Release:
	overlay_veneer Actor_ReleaseFar
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
	.global Map_ClearLayerEntryFlag
	.thumb_func
Map_ClearLayerEntryFlag:
	overlay_veneer Map_ClearLayerEntryFlagFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer MapGrid_GetTerrainHeightFar
	.global Map_CopyCellAttributeRect
	.thumb_func
Map_CopyCellAttributeRect:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Actor_SetSpriteFlags
	.thumb_func
Actor_SetSpriteFlags:
	overlay_veneer Actor_SetSpriteFlagsFar
	.global Actor_SetBlendMode
	.thumb_func
Actor_SetBlendMode:
	overlay_veneer Actor_SetBlendModeFar
	.global MapGrid_SetScreenShake
	.thumb_func
MapGrid_SetScreenShake:
	overlay_veneer MapGrid_SetScreenShakeFar
	.global MapGrid_WaitForShakeToSettle
	.thumb_func
MapGrid_WaitForShakeToSettle:
	overlay_veneer MapGrid_WaitForShakeToSettleFar
	.global Actor_HasArrived
	.thumb_func
Actor_HasArrived:
	overlay_veneer Actor_HasArrivedFar
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global WindowColor_Apply
	.thumb_func
WindowColor_Apply:
	overlay_veneer WindowColor_ApplyFar
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
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_WalkToById
	.thumb_func
Actor_WalkToById:
	overlay_veneer Actor_WalkToByIdFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Actor_SetHorizontalPositionWithTerrain
	.thumb_func
Actor_SetHorizontalPositionWithTerrain:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetSpriteColor
	.thumb_func
Actor_SetSpriteColor:
	overlay_veneer Actor_SetSpriteColorFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
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
	.global FieldCamera_WaitForArrival
	.thumb_func
FieldCamera_WaitForArrival:
	overlay_veneer FieldCamera_WaitForArrivalFar
	.global FieldCamera_MoveToActorById
	.thumb_func
FieldCamera_MoveToActorById:
	overlay_veneer FieldCamera_MoveToActorByIdFar
	.global FieldCamera_GetActor
	.thumb_func
FieldCamera_GetActor:
	overlay_veneer FieldCamera_GetActorFar
	.global Import_EventSetExitRequest
	.thumb_func
Import_EventSetExitRequest:
	overlay_veneer Event_SetExitRequestFar
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
	.global FieldEffect_StartBlankOverlay
	.thumb_func
FieldEffect_StartBlankOverlay:
	overlay_veneer FieldEffect_StartBlankOverlayFar
	.global Event_ResolvePendingActions
	.thumb_func
Event_ResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Sprite_ShareVramCacheEntry
	.thumb_func
Sprite_ShareVramCacheEntry:
	overlay_veneer Sprite_ShareVramCacheEntryFar
	.global Event_NoOp
	.thumb_func
Event_NoOp:
	overlay_veneer Event_NoOpFar
	.global FlagBlock_PlaceAll
	.thumb_func
FlagBlock_PlaceAll:
	overlay_veneer FlagBlock_PlaceAllFar
	.global FlagBlock_ApplyFlags
	.thumb_func
FlagBlock_ApplyFlags:
	overlay_veneer FlagBlock_ApplyFlagsFar
	.global Actor_PlaceBlocks
	.thumb_func
Actor_PlaceBlocks:
	overlay_veneer Actor_PlaceBlocksFar
	.global Actor_LiftTargetBlock
	.thumb_func
Actor_LiftTargetBlock:
	overlay_veneer Actor_LiftTargetBlockFar
	.global Actor_PlaceMovedBlock
	.thumb_func
Actor_PlaceMovedBlock:
	overlay_veneer Actor_PlaceMovedBlockFar
	.global Party_GetLeaderActor
	.thumb_func
Party_GetLeaderActor:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02003cd0
	.thumb_func
Func_02003cd0:
	overlay_veneer Event_PlayCueNearFocusFar
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
