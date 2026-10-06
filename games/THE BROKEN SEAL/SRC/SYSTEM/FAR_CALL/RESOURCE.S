.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global FarCall_ResourceTable
FarCall_ResourceTable:
	.global Resource_GetMetadataRecordFar
	.thumb_func
Resource_GetMetadataRecordFar:
	overlay_veneer Resource_GetMetadataRecord
