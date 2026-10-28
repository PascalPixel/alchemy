.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
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
	.global Animation_InitializeWorkFromMetadataFar
	.thumb_func
Animation_InitializeWorkFromMetadataFar:
	overlay_veneer Animation_InitializeWorkFromMetadata
