.syntax unified
	.thumb
	.global Func_080439c4
	.thumb_func
Func_080439c4:
	push {lr}
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080439d4
	bl Func_080145a8
	pop {pc}
	.2byte 0x0000
.L_080439d4:
	.4byte PaletteGlow_UpdateSine
