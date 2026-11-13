.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
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
	.global GameFlag_Get
	.thumb_func
GameFlag_Get:
	overlay_veneer GameFlag_GetFar
	.global GameFlag_Set
	.thumb_func
GameFlag_Set:
	overlay_veneer GameFlag_SetFar
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
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Map_CenterOnFollowedPoint
	.thumb_func
Map_CenterOnFollowedPoint:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global MapGrid_CopyCellPictureIndicesRect
	.thumb_func
MapGrid_CopyCellPictureIndicesRect:
	overlay_veneer MapGrid_CopyCellPictureIndicesRectFar
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
	.global MapGrid_SetScreenShake
	.thumb_func
MapGrid_SetScreenShake:
	overlay_veneer MapGrid_SetScreenShakeFar
	.global MapGrid_GetTerrainKind
	.thumb_func
MapGrid_GetTerrainKind:
	overlay_veneer MapGrid_GetTerrainKindFar
	.global MapGrid_SetCellHeight
	.thumb_func
MapGrid_SetCellHeight:
	overlay_veneer MapGrid_SetCellHeightFar
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
	.global Actor_SetHorizontalPositionWithTerrain
	.thumb_func
Actor_SetHorizontalPositionWithTerrain:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_MoveToObject
	.thumb_func
Actor_MoveToObject:
	overlay_veneer Actor_MoveToObjectFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_SetSpriteColor
	.thumb_func
Actor_SetSpriteColor:
	overlay_veneer Actor_SetSpriteColorFar
	.global Actor_DisableById
	.thumb_func
Actor_DisableById:
	overlay_veneer Actor_DisableByIdFar
	.global Actor_CopyGraphics
	.thumb_func
Actor_CopyGraphics:
	overlay_veneer Actor_CopyGraphicsFar
	.global FieldCamera_FollowActor
	.thumb_func
FieldCamera_FollowActor:
	overlay_veneer FieldCamera_FollowActorFar
	.global Import_FieldTryJumpForward
	.thumb_func
Import_FieldTryJumpForward:
	overlay_veneer Field_TryJumpForwardFar
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
	.global FlagBlock_PlaceAll
	.thumb_func
FlagBlock_PlaceAll:
	overlay_veneer FlagBlock_PlaceAllFar
	.global FlagBlock_ApplyFlags
	.thumb_func
FlagBlock_ApplyFlags:
	overlay_veneer FlagBlock_ApplyFlagsFar
	.global Actor_GetCellIndex
	.thumb_func
Actor_GetCellIndex:
	overlay_veneer Actor_GetCellIndexFar
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
	.global MapCollision_FindOrAddTag
	.thumb_func
MapCollision_FindOrAddTag:
	overlay_veneer MapCollision_FindOrAddTagFar
	.global Actor_FindAtCell
	.thumb_func
Actor_FindAtCell:
	overlay_veneer Actor_FindAtCellFar
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
