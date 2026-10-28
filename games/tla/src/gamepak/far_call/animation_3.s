.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
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
	.global Actor_SetSpritePriorityDirectFar
	.thumb_func
Actor_SetSpritePriorityDirectFar:
	overlay_veneer Actor_SetSpritePriorityDirect
	.global ScriptObject_CheckOverlapFar
	.thumb_func
ScriptObject_CheckOverlapFar:
	overlay_veneer ScriptObject_CheckOverlap
	.global Func_080202a0
	.thumb_func
Func_080202a0:
	overlay_veneer Map_SetShownLayers
