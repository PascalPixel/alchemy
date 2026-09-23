.syntax unified
	.thumb
	.global Func_0815e1b8
	.thumb_func
Func_0815e1b8:
	push	{r5, lr}
	ldr	r3, [r0, #0]
	adds	r5, r2, #0
	ldr	r2, [r0, #12]
	ldr	r4, [r0, #16]
	adds	r3, r3, r2
	str	r3, [r0, #0]
	ldr	r3, [r0, #4]
	adds	r3, r3, r4
	str	r3, [r0, #4]
	adds	r3, r1, #0
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L_0815e1d6
	adds	r3, #63
.L_0815e1d6:
	asrs	r3, r3, #6
	str	r3, [r0, #12]
	adds	r3, r4, r5
	muls	r1, r3
	cmp	r1, #0
	bge.n	.L_0815e1e4
	adds	r1, #63
.L_0815e1e4:
	asrs	r3, r1, #6
	str	r3, [r0, #16]
	pop	{r5, pc}
	.align 2, 0
