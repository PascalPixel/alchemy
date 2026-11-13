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
	.global HeapSlot_Alloc
	.thumb_func
HeapSlot_Alloc:
	overlay_veneer HeapSlot_AllocFar
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
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
	.global Import_OamAddSpriteAtDepthClamped
	.thumb_func
Import_OamAddSpriteAtDepthClamped:
	overlay_veneer Oam_AddSpriteAtDepthClampedFar
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
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global Import_ActorSelectAnimation
	.thumb_func
Import_ActorSelectAnimation:
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
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Map_CenterOnFollowedPoint
	.thumb_func
Map_CenterOnFollowedPoint:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Import_ActorStartIdle
	.thumb_func
Import_ActorStartIdle:
	overlay_veneer Actor_StartIdleFar
	.global Actor_SetMoveTarget
	.thumb_func
Actor_SetMoveTarget:
	overlay_veneer Actor_SetMoveTargetFar
	.global Actor_WaitForMove
	.thumb_func
Actor_WaitForMove:
	overlay_veneer Actor_WaitForMoveFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer MapGrid_GetTerrainHeightFar
	.global Map_CopyCellsRect
	.thumb_func
Map_CopyCellsRect:
	overlay_veneer Map_CopyCellsRectFar
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
	.global MapGrid_SetScreenShake
	.thumb_func
MapGrid_SetScreenShake:
	overlay_veneer MapGrid_SetScreenShakeFar
	.global MapGrid_WaitForShakeToSettle
	.thumb_func
MapGrid_WaitForShakeToSettle:
	overlay_veneer MapGrid_WaitForShakeToSettleFar
	.global Actor_SetSpriteColorMode
	.thumb_func
Actor_SetSpriteColorMode:
	overlay_veneer Actor_SetSpriteColorModeFar
	.global MapGrid_SetCellHeight
	.thumb_func
MapGrid_SetCellHeight:
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
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_Stop
	.thumb_func
Actor_Stop:
	overlay_veneer Actor_StopFar
	.global Actor_MoveToAndWaitById
	.thumb_func
Actor_MoveToAndWaitById:
	overlay_veneer Actor_MoveToAndWaitByIdFar
	.global Actor_SetHorizontalPositionWithTerrain
	.thumb_func
Actor_SetHorizontalPositionWithTerrain:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_SetSpriteColor
	.thumb_func
Actor_SetSpriteColor:
	overlay_veneer Actor_SetSpriteColorFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
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
	.global Import_FieldCameraWaitForArrival
	.thumb_func
Import_FieldCameraWaitForArrival:
	overlay_veneer FieldCamera_WaitForArrivalFar
	.global FieldCamera_GetActor
	.thumb_func
FieldCamera_GetActor:
	overlay_veneer FieldCamera_GetActorFar
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
	.global Event_ResolvePendingActions
	.thumb_func
Event_ResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Camera_WorldToScreen
	.thumb_func
Camera_WorldToScreen:
	overlay_veneer Camera_WorldToScreenFar
	.global Func_02002b28
	.thumb_func
Func_02002b28:
	overlay_veneer HangingVine_PlaceFar
	.global Func_02002b30
	.thumb_func
Func_02002b30:
	overlay_veneer EffectSprite_InitFar
	.global Func_02002b38
	.thumb_func
Func_02002b38:
	overlay_veneer EffectSprite_DrawFar
	.global EffectSprite_DrawAtWorldPosition
	.thumb_func
EffectSprite_DrawAtWorldPosition:
	overlay_veneer EffectSprite_DrawAtWorldPositionFar
	.global BattleEffect_GetObjectPriority
	.thumb_func
BattleEffect_GetObjectPriority:
	overlay_veneer BattleEffect_GetObjectPriorityFar
	.global MapGrid_GetCellBehavior
	.thumb_func
MapGrid_GetCellBehavior:
	overlay_veneer MapGrid_GetCellBehaviorFar
	.global MapGrid_SetAttributeBAt
	.thumb_func
MapGrid_SetAttributeBAt:
	overlay_veneer MapGrid_SetAttributeBAtFar
	.global Sound_WaitForJingle
	.thumb_func
Sound_WaitForJingle:
	overlay_veneer Sound_WaitForJingleFar
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
