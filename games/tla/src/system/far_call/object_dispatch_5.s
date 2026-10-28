.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
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
	.global Object_ResetMotion
	.thumb_func
Object_ResetMotion:
	overlay_veneer ObjectDispatch_StartIdle
	.global Object_RefreshSelector
	.thumb_func
Object_RefreshSelector:
	overlay_veneer ObjectDispatch_WaitForCommandEnd
	.global Object_SetPosition
	.thumb_func
Object_SetPosition:
	overlay_veneer Object_SetMoveTarget
	.global Object_CommitPosition
	.thumb_func
Object_CommitPosition:
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
