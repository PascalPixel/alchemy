.syntax unified
	.thumb
	.global Overlay_080019f4
Overlay_080019f4:
	push	{lr}
	add	r1, pc, #16
	ldmia	r1, {r1, r2, r4}
	sub	sp, #320
	mov	r3, sp
	stmia	r1!, {r2, r3, r4}
	mov	lr, sp
	.2byte 0xf800
	.2byte 0xb050
	pop	{pc}
