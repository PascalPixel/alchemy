.syntax unified
	.thumb
	.global Overlay_080c8970
Overlay_080c8970:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x17e9
	.2byte 0x080d
	push	{r5, r6, r7, lr}
	mov	lr, r2
	ldmia	r0!, {r2, r3, r4, r5, r6, r7}
	subs	r0, #24
	adds	r2, r2, r5
	adds	r3, r3, r6
	adds	r4, r4, r7
	stmia	r0!, {r2, r3, r4}
	add	r6, lr
	muls	r5, r1
	bge.n	.L_080c8990
	adds	r5, #63
.L_080c8990:
	asrs	r5, r5, #6
	muls	r6, r1
	bge.n	.L_080c8998
	adds	r6, #63
.L_080c8998:
	asrs	r6, r6, #6
	muls	r7, r1
	bge.n	.L_080c89a0
	adds	r7, #63
.L_080c89a0:
	asrs	r7, r7, #6
	stmia	r0!, {r5, r6, r7}
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	mov	lr, r2
	ldmia	r0!, {r2, r3, r4, r5, r6}
	subs	r0, #20
	adds	r2, r2, r5
	adds	r3, r3, r6
	stmia	r0!, {r2, r3}
	adds	r0, #4
	add	r6, lr
	muls	r5, r1
	bge.n	.L_080c89be
	adds	r5, #63
.L_080c89be:
	asrs	r5, r5, #6
	muls	r6, r1
	bge.n	.L_080c89c6
	adds	r6, #63
.L_080c89c6:
	asrs	r6, r6, #6
	stmia	r0!, {r5, r6}
	pop	{r5, r6, pc}
