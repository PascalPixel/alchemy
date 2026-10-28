.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Resource_FarCall006
Resource_FarCall006:
	.global FieldSequence_RunFar
	.thumb_func
FieldSequence_RunFar:
	overlay_veneer FieldSequence_Run
