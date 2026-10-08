.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global ResourceMetadata_SumCommandLengthsFar
	.thumb_func
ResourceMetadata_SumCommandLengthsFar:
	overlay_veneer ResourceMetadata_SumCommandLengths
	.global GetWorldMapTerrainBehaviorFar
	.thumb_func
GetWorldMapTerrainBehaviorFar:
	overlay_veneer GetWorldMapTerrainBehavior
	.global ResourceSlot_LoadFar
	.thumb_func
ResourceSlot_LoadFar:
	overlay_veneer ResourceSlot_Load
	.global SetMapCellCollisionFar
	.thumb_func
SetMapCellCollisionFar:
	overlay_veneer SetMapCellCollision
	.global Animation_ApplyChildValueFar
	.thumb_func
Animation_ApplyChildValueFar:
	overlay_veneer Animation_ApplyChildValue
	.global Runtime_RemapBytesByTableFar
	.thumb_func
Runtime_RemapBytesByTableFar:
	overlay_veneer Runtime_RemapBytesByTable
	.global Map_EnableUpdateCallbackFar
	.thumb_func
Map_EnableUpdateCallbackFar:
	overlay_veneer Map_EnableUpdateCallback
	.global Map_DisableUpdateCallbackFar
	.thumb_func
Map_DisableUpdateCallbackFar:
	overlay_veneer Map_DisableUpdateCallback
	.global ScriptObject_FindOverlappingEntryFar
	.thumb_func
ScriptObject_FindOverlappingEntryFar:
	overlay_veneer ScriptObject_FindOverlappingEntry
	.global Func_080202f0
	.thumb_func
Func_080202f0:
	overlay_veneer Func_0802d658
	.global Func_080202f8
	.thumb_func
Func_080202f8:
	overlay_veneer Func_08023f94
	.global Func_08020300
	.thumb_func
Func_08020300:
	overlay_veneer Func_0802372c
