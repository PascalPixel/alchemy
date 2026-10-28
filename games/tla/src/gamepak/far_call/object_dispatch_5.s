.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Map_CenterOnFollowedPointFar
	.thumb_func
Map_CenterOnFollowedPointFar:
	overlay_veneer Func_0802aa74
	.global Map_UpdateCurrentTileBlockFar
	.thumb_func
Map_UpdateCurrentTileBlockFar:
	overlay_veneer Map_UpdateCurrentTileBlock
	.global WorldMap_LoadGraphicsFar
	.thumb_func
WorldMap_LoadGraphicsFar:
	overlay_veneer Func_0802be4c
	.global Actor_ResetMotion
	.thumb_func
Actor_ResetMotion:
	overlay_veneer Actor_StartIdle
	.global Actor_RefreshSelector
	.thumb_func
Actor_RefreshSelector:
	overlay_veneer Actor_WaitForCommandEnd
	.global Actor_SetTargetPosition
	.thumb_func
Actor_SetTargetPosition:
	overlay_veneer Actor_SetMoveTarget
	.global Actor_CommitPosition
	.thumb_func
Actor_CommitPosition:
	overlay_veneer Script_WaitForEventTimeout
	.global Map_GetScreenRelativePositionFar
	.thumb_func
Map_GetScreenRelativePositionFar:
	overlay_veneer Map_GetScreenRelativePosition
	.global WorldMap_UpdateViewFar
	.thumb_func
WorldMap_UpdateViewFar:
	overlay_veneer Func_0802c240
	.global Map_LoadDefaultCellsAndUpdateBlockFar
	.thumb_func
Map_LoadDefaultCellsAndUpdateBlockFar:
	overlay_veneer Map_LoadDefaultCellsAndUpdateBlock
	.global Map_PlayMapEditFar
	.thumb_func
Map_PlayMapEditFar:
	overlay_veneer Map_PlayMapEdit
	.global Map_CopyCellPictureIndicesRectFar
	.thumb_func
Map_CopyCellPictureIndicesRectFar:
	overlay_veneer Map_CopyCellPictureIndicesRect
