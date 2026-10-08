.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Resource_FarCall003
Resource_FarCall003:
	.global Resource_GetMetadataRecordFarFar
	.thumb_func
Resource_GetMetadataRecordFarFar:
	overlay_veneer Resource_GetMetadataRecordFar
	.global Func_08020008
	.thumb_func
Func_08020008:
	overlay_veneer Func_08022bd8
