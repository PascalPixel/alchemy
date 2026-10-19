.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Resource_FarCall006
Resource_FarCall006:
	.global Field_RunSceneFar
	.thumb_func
Field_RunSceneFar:
	overlay_veneer Field_RunScene
