.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Resource_FarCall00D
Resource_FarCall00D:
	.global LuckyDice_StartFar
	.thumb_func
LuckyDice_StartFar:
	overlay_veneer LuckyDice_Start
