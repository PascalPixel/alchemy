.syntax unified
	.thumb
	.set sub_081049c8, 0x081049c8
	.global Overlay_0810498c
Overlay_0810498c:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	sub	sp, #16
	mov	r5, sp
	adds	r6, r0, #0
	adds	r0, r5, #0
	mov	r8, r1
	bl	sub_081049c8
	ldrb	r3, [r5, r6]
	mov	r2, r8
	subs	r3, #1
	strb	r3, [r5, r6]
	movs	r0, #1
	ldrb	r3, [r5, r2]
	adds	r3, #1
	strb	r3, [r5, r2]
	lsls	r3, r3, #24
	ldrsb	r2, [r5, r6]
	asrs	r3, r3, #24
	adds	r2, #2
	cmp	r2, r3
	bgt.n	.L_081049be
	movs	r0, #0
.L_081049be:
	add	sp, #16
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
