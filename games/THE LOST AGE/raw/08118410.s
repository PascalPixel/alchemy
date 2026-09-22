.syntax unified
	.thumb
	.global Overlay_08118410
Overlay_08118410:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #124
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	mov	lr, sp
	.2byte 0xf800
	.2byte 0xb01f
	pop	{r5, r6, r7, pc}
