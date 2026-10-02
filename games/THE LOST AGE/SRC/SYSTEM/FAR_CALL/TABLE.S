@ Far-call stub table at 0x08020010: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
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
	.global InitializeAnimationObjectsFar
	.thumb_func
InitializeAnimationObjectsFar:
	.ifdef TLA_EDITION_DE
	overlay_veneer ResourceMetadata_Register
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer ResourceMetadata_Register
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer ResourceMetadata_Register
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer ResourceMetadata_Register
	.else
	overlay_veneer InitializeAnimationObjects
	.endif
	.endif
	.endif
	.endif
