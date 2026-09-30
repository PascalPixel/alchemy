.syntax unified
	.thumb
	.global BattleFx_PrepareBufferInterpolation
	.thumb_func
BattleFx_PrepareBufferInterpolation:
	push {lr}
	movs r0, #8
	bl Field_BeginPaletteTransition
	pop {pc}
	.2byte 0x0000
