.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08020258
	.thumb_func
Func_08020258:
	overlay_veneer Func_0802da24
	.global ObjectDispatch_RegisterChildMetadataFar
	.thumb_func
ObjectDispatch_RegisterChildMetadataFar:
	overlay_veneer ObjectDispatch_RegisterChildMetadata
