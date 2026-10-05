.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_GameFlagClear
	.thumb_func
Engine_GameFlagClear:
	overlay_veneer GameFlag_ClearBitFar
	.global BattleFx_RunPageEffectForSlot
	.thumb_func
BattleFx_RunPageEffectForSlot:
	overlay_veneer BattleFx_RunPageEffectForSlotFar
