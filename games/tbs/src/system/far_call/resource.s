.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.global FarCall_ResourceTable
FarCall_ResourceTable:
	.global Resource_GetMetadataRecordFar
	.thumb_func
Resource_GetMetadataRecordFar:
	overlay_veneer Resource_GetMetadataRecord
