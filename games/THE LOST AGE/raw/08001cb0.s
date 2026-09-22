.syntax unified
	.thumb
	.global Overlay_08001cb0
Overlay_08001cb0:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #128
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	mov	lr, sp
	.2byte 0xf800
	.2byte 0xb020
	pop	{r5, r6, r7, pc}
