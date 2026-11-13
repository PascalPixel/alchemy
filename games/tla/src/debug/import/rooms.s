.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global Djinn_JoinParty
	.thumb_func
Djinn_JoinParty:
	overlay_veneer Djinn_JoinPartyFar
