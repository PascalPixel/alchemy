@ Word clearing and filling, integer square root and bit-run expansion of
@ the resident runtime. IwramClearWords sets the fill value to zero and
@ falls into the fill, which jumps into its unrolled stmia run with
@ add pc, pc, ip, lsr #3 and saves r5-r9 without lr; IwramSqrt returns early
@ with bxmi lr; IwramExpandBitRuns stores r2 at the bytes that the set bits
@ of 14 halfwords select, 16 bytes per halfword. GCC never jumps into
@ unrolled code. These 236 bytes equal TLA SYSTEM/IWRAM_RUNTIME_MATH.S
@ (0x08000810), where a Thumb entry stub stands in front of each routine.
@ credit: library — TBS/TLA common IWRAM runtime fill, square root and bit-run helpers
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x080008d4
	.set AlchemyRunAddress, 0x03000164

	.global IwramClearWords
IwramClearWords:
	mov	r2, #0
	push	{r5, r6, r7, r8, r9}
	mov	r3, r2
	mov	r4, r2
	mov	r5, r2
	mov	r6, r2
	mov	r7, r2
	mov	r8, r2
	mov	r9, r2
	ands	ip, r1, #0xe0
	rsb	ip, ip, #0xe0
	add	pc, pc, ip, lsr #3
.L_clear_256:
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	subs	r1, r1, #0x100
	bpl	.L_clear_256
	ands	r1, r1, #0x1c
	beq	.L_clear_done
.L_clear_word:
	stmia	r0!, {r2}
	subs	r1, r1, #4
	bgt	.L_clear_word
.L_clear_done:
	pop	{r5, r6, r7, r8, r9}
	bx	lr

	.global IwramSqrt
IwramSqrt:
	movs	r1, r0
	mov	r0, #0
	bxmi	lr
	mov	r2, #0x8000
.L_sqrt_coarse:
	add	r0, r0, r2
	mul	r3, r0, r0
	cmp	r3, r1
	subhi	r0, r0, r2
	add	r0, r0, r2, lsr #1
	mul	r3, r0, r0
	cmp	r3, r1
	subhi	r0, r0, r2, lsr #1
	lsrs	r2, r2, #2
	bne	.L_sqrt_coarse
	bx	lr

	.global IwramExpandBitRuns
IwramExpandBitRuns:
	add	ip, r0, #28
.L_expand_halfword:
	mov	r4, r1
	ldrh	r3, [r0], #2
	bic	r3, r3, #3
	lsls	r3, r3, #17
	bcc	.L_expand_zero
.L_expand_store:
	strb	r2, [r4], #1
.L_expand_shift:
	lsls	r3, r3, #1
	bcs	.L_expand_store
.L_expand_zero:
	add	r4, r4, #1
	bne	.L_expand_shift
	add	r1, r1, #16
	cmp	r0, ip
	bne	.L_expand_halfword
	bx	lr
