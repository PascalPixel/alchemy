@ Fixed-point vector integrators for three and two axes: each adds the
@ velocity to the position, adds its third argument to the second velocity
@ component, then multiplies every velocity component by its second argument
@ and divides by 64, rounding toward zero. Each muls sets the N flag that the
@ next bge tests with no compare between them, where both approved compilers
@ emit each Thumb conditional branch directly after its own cmp or cmn; the
@ vector is loaded with ldmia r0! and rewound with subs r0, and the third
@ argument is kept in lr. These 84 bytes equal INTEGRATE_VECTORS.S
@ (0x080c8978); this second link sits in the battle-effect kernel bank. TBS
@ has no such routine.
@ credit: handwritten — three- and two-axis fixed-point vector integrators (instance 2)
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08138058
	.global BattleFxKernels_IntegrateVector3
	.thumb_func
BattleFxKernels_IntegrateVector3:
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
	.global BattleFxKernels_IntegrateVector2
	.thumb_func
BattleFxKernels_IntegrateVector2:
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
