@ Far-call stub table at 0x08020058: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global ResourceMetadata_RegisterFar
	.thumb_func
ResourceMetadata_RegisterFar:
	overlay_veneer ResourceMetadata_Register
	.global ResourceMetadata_UnregisterFar
	.thumb_func
ResourceMetadata_UnregisterFar:
	overlay_veneer ResourceMetadata_Unregister
	.global ResourceMetadata_ReleaseSlotFar
	.thumb_func
ResourceMetadata_ReleaseSlotFar:
	overlay_veneer ResourceMetadata_ReleaseSlot
	.global ResourceMetadata_ClearRecordFar
	.thumb_func
ResourceMetadata_ClearRecordFar:
	overlay_veneer ResourceMetadata_ClearRecord
	.global Animation_InitWorkFromMetadataFar
	.thumb_func
Animation_InitWorkFromMetadataFar:
	overlay_veneer Animation_InitWorkFromMetadata
