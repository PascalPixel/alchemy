@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_085bbf02
	.thumb_func
Func_085bbf02:
	push	{r1, r2, r3, r4, lr}
	cmp	r2, #102
	lsls	r6, r0, #20
	str	r7, [sp, #396]
	cmp	r1, #159
	ldr	r5, [sp, #208]
	asrs	r0, r4, #15
	ldr	r4, [sp, #352]
	adds	r3, #122
	str	r3, [r6, #36]
	ldmia	r0!, {r1, r2, r7}
	adds	r3, #62
	ldrb	r4, [r0, #25]
	ldrh	r1, [r0, #54]
	subs	r5, #210
	str	r2, [r1, #52]
	beq.n	Func_085bbf7e
	b.n	Func_085bbb98
	ldmia	r4, {r2, r4, r5, r6}
	bhi.n	Func_085bbfe0
	cmp	fp, r0
	add	sp, #384
	bge.n	Func_085bbfc0
	subs	r5, #201
	subs	r0, r7, #4
	strh	r4, [r7, #8]
	subs	r1, r7, #4
	cmp	r4, #115
	ldr	r6, [r4, r6]
	ldmia	r3, {r2, r3, r6, r7}
	strh	r0, [r3, #14]
	ldr	r0, [sp, #316]
	lsls	r4, r3, #19
	stmia	r0!, {r0, r1, r2, r3, r4, r7}
	pop	{r0, r7, pc}
