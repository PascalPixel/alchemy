.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_08020268
	.thumb_func
Func_08020268:
	overlay_veneer MapAnimation_Start
	.global Func_08020270
	.thumb_func
Func_08020270:
	overlay_veneer Func_0802c9c8
	.global Animation_ApplyChildValuesFar
	.thumb_func
Animation_ApplyChildValuesFar:
	overlay_veneer Animation_ApplyChildValues
	.global Animation_ApplyChildValuesToRecordFar
	.thumb_func
Animation_ApplyChildValuesToRecordFar:
	overlay_veneer Animation_ApplyChildValuesToRecord
	.global Animation_ApplyChildValueToFirstFar
	.thumb_func
Animation_ApplyChildValueToFirstFar:
	overlay_veneer Animation_ApplyChildValueToFirst
	.global ObjectDispatch_SetSpritePriorityFar
	.thumb_func
ObjectDispatch_SetSpritePriorityFar:
	overlay_veneer ObjectDispatch_SetSpritePriority
	.global ScriptObject_CheckOverlapFar
	.thumb_func
ScriptObject_CheckOverlapFar:
	overlay_veneer ScriptObject_CheckOverlap
	.global Func_080202a0
	.thumb_func
Func_080202a0:
	overlay_veneer Map_SetWorkFlagBits9To11
