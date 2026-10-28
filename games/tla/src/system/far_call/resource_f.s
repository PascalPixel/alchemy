.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Resource_FarCall00F
Resource_FarCall00F:
	.global Func_081b83c4_StartFar
	.thumb_func
Func_081b83c4_StartFar:
	overlay_veneer Func_081b83c4_Start
