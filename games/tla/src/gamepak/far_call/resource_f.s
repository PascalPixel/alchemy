.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Resource_FarCall00F
Resource_FarCall00F:
	.global SuperLuckyDice_StartFar
	.thumb_func
SuperLuckyDice_StartFar:
	overlay_veneer SuperLuckyDice_Start
