@ C2: Every entry uses overlay_veneer, an aligned 8-byte Thumb ldr r4/bx
@ r4/target-word stub.
.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.global FarCall_ResourceTable
FarCall_ResourceTable:
	.global Resource_GetMetadataRecordFar
	.thumb_func
Resource_GetMetadataRecordFar:
	overlay_veneer Resource_GetMetadataRecord
