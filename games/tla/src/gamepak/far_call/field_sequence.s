.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Resource_FarCall006
Resource_FarCall006:
	.global FieldSequence_RunFar
	.thumb_func
FieldSequence_RunFar:
	overlay_veneer FieldSequence_Run
