.syntax unified
	.thumb
	.global Overlay_08138000
Overlay_08138000:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x6405
	.2byte 0x0819
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xe321
	.2byte 0x0815
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xc995
	.2byte 0x0814
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xcb61
	.2byte 0x0814
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xe3d1
	.2byte 0x0813
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xeb71
	.2byte 0x0813
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xf519
	.2byte 0x0813
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xfa89
	.2byte 0x0813
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xfd41
	.2byte 0x0813
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x63ed
	.2byte 0x0819
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x2945
	.2byte 0x0814
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
	bge.n	.L_08138070
	adds	r5, #63
.L_08138070:
	asrs	r5, r5, #6
	muls	r6, r1
	bge.n	.L_08138078
	adds	r6, #63
.L_08138078:
	asrs	r6, r6, #6
	muls	r7, r1
	bge.n	.L_08138080
	adds	r7, #63
.L_08138080:
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
	bge.n	.L_0813809e
	adds	r5, #63
.L_0813809e:
	asrs	r5, r5, #6
	muls	r6, r1
	bge.n	.L_081380a6
	adds	r6, #63
.L_081380a6:
	asrs	r6, r6, #6
	stmia	r0!, {r5, r6}
	pop	{r5, r6, pc}
