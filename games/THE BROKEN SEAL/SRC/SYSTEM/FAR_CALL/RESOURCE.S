.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global FarCall_ResourceTable
FarCall_ResourceTable:
	.global Resource_GetMetadataRecordFar
	.global Func_08185000
	.thumb_func
Resource_GetMetadataRecordFar:
Func_08185000:
	overlay_veneer Resource_GetMetadataRecord