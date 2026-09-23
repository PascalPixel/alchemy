@ Fixed-point vector integrators for three and two axes: each adds the
@ velocity to the position, adds its third argument to the second velocity
@ component, then multiplies every velocity component by its second argument
@ and divides by 64, rounding toward zero. Each muls sets the N flag that the
@ next bge tests with no compare between them, where both approved compilers
@ emit each Thumb conditional branch directly after its own cmp or cmn; the
@ vector is loaded with ldmia r0! and rewound with subs r0, and the third
@ argument is kept in lr. TBS has no such routine; the battle-effect kernel
@ bank links a second copy (KERNEL_BANK_INTEGRATE_VECTORS.S).
@ credit: handwritten — three- and two-axis fixed-point vector integrators (instance 1)
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x080c8978
	.global BattleFx_IntegrateVector3
	.thumb_func
BattleFx_IntegrateVector3:
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
	.global BattleFx_IntegrateVector2
	.thumb_func
BattleFx_IntegrateVector2:
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
