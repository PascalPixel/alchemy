.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.ifdef TLA_EDITION_DE
	.global ResourceObject_CreateFar
	.thumb_func
ResourceObject_CreateFar:
	overlay_veneer ResourceObject_Create
	.else
	.ifdef TLA_EDITION_ES
	.global ResourceObject_CreateFar
	.thumb_func
ResourceObject_CreateFar:
	overlay_veneer ResourceObject_Create
	.else
	.ifdef TLA_EDITION_FR
	.global ResourceObject_CreateFar
	.thumb_func
ResourceObject_CreateFar:
	overlay_veneer ResourceObject_Create
	.else
	.ifdef TLA_EDITION_IT
	.global ResourceObject_CreateFar
	.thumb_func
ResourceObject_CreateFar:
	overlay_veneer ResourceObject_Create
	.else
	.global Render_ApplyProjectedPlacementFar
	.thumb_func
Render_ApplyProjectedPlacementFar:
	overlay_veneer Render_ApplyProjectedPlacement
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_DE
	.global ResourceObject_ReleaseFar
	.thumb_func
ResourceObject_ReleaseFar:
	overlay_veneer ResourceObject_Release
	.else
	.ifdef TLA_EDITION_ES
	.global ResourceObject_ReleaseFar
	.thumb_func
ResourceObject_ReleaseFar:
	overlay_veneer ResourceObject_Release
	.else
	.ifdef TLA_EDITION_FR
	.global ResourceObject_ReleaseFar
	.thumb_func
ResourceObject_ReleaseFar:
	overlay_veneer ResourceObject_Release
	.else
	.ifdef TLA_EDITION_IT
	.global ResourceObject_ReleaseFar
	.thumb_func
ResourceObject_ReleaseFar:
	overlay_veneer ResourceObject_Release
	.else
	.global Func_08020018
	.thumb_func
Func_08020018:
	overlay_veneer Func_08022318
	.endif
	.endif
	.endif
	.endif
	.global Func_08020020
	.thumb_func
Func_08020020:
	overlay_veneer Func_0802254c
	.ifdef TLA_EDITION_DE
	.global ResourceMetadata_RegisterFar
	.thumb_func
ResourceMetadata_RegisterFar:
	overlay_veneer ResourceMetadata_Register
	.else
	.ifdef TLA_EDITION_ES
	.global ResourceMetadata_RegisterFar
	.thumb_func
ResourceMetadata_RegisterFar:
	overlay_veneer ResourceMetadata_Register
	.else
	.ifdef TLA_EDITION_FR
	.global ResourceMetadata_RegisterFar
	.thumb_func
ResourceMetadata_RegisterFar:
	overlay_veneer ResourceMetadata_Register
	.else
	.ifdef TLA_EDITION_IT
	.global ResourceMetadata_RegisterFar
	.thumb_func
ResourceMetadata_RegisterFar:
	overlay_veneer ResourceMetadata_Register
	.else
	.global InitializeAnimationObjectsFar
	.thumb_func
InitializeAnimationObjectsFar:
	overlay_veneer InitializeAnimationObjects
	.endif
	.endif
	.endif
	.endif
