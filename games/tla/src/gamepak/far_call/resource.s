.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Resource_FarCall003
Resource_FarCall003:
	.global Resource_GetMetadataRecordFarFar
	.thumb_func
Resource_GetMetadataRecordFarFar:
	overlay_veneer Resource_GetMetadataRecordFar
	.global ObjectSystem_ConfigureFar
	.thumb_func
ObjectSystem_ConfigureFar:
	overlay_veneer ObjectSystem_Configure
