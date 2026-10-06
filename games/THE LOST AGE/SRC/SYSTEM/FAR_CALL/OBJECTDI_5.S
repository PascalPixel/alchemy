.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Map_ApplyWorkOriginAndSpanFar
	.thumb_func
Map_ApplyWorkOriginAndSpanFar:
	overlay_veneer Func_0802aa74
	.global Map_UpdateCurrentTileBlockFar
	.thumb_func
Map_UpdateCurrentTileBlockFar:
	overlay_veneer Func_0802c4d8
	.global WorldMap_LoadGraphicsFar
	.thumb_func
WorldMap_LoadGraphicsFar:
	overlay_veneer Func_0802be4c
	.global Object_ResetMotion
	.thumb_func
Object_ResetMotion:
	overlay_veneer ObjectDispatch_InitFromTable6
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
	overlay_veneer Func_08023088
	.global WorldMap_UpdateViewFar
	.thumb_func
WorldMap_UpdateViewFar:
	overlay_veneer Func_0802c240
	.global Map_LoadDefaultCellsAndUpdateBlockFar
	.thumb_func
Map_LoadDefaultCellsAndUpdateBlockFar:
	overlay_veneer Func_0802cb08
	.global Map_PlayMetatileCopySequenceFar
	.thumb_func
Map_PlayMetatileCopySequenceFar:
	overlay_veneer Func_0802b2d4
	.global Map_CopyMetatileIndicesRectFar
	.thumb_func
Map_CopyMetatileIndicesRectFar:
	overlay_veneer Map_CopyMetatileIndicesRect
