@ Vector and matrix transforms of TLA's resident runtime against its current
@ transform (IWRAM_RUNTIME_MATRIX.S, run 0x03000478), each behind a 4-byte
@ Thumb entry (bx pc). Both form that matrix's address from the program
@ counter (add r0, pc, #0x110; add r5, pc, #144, #128 and #112;
@ add r1, pc, #64) and accumulate 64-bit products with smull and smlal. The
@ matrix transform skips zero input coefficients (movs; beq), keeps its
@ first three product rows in a 36-byte stack buffer, pops that buffer into
@ r0 and r2-r9 (pop {r0, r2-r9}) and stores all four rows back over the
@ matrix with stmdb r1, {r0, r2-r9, sl, fp, ip}. GCC 2.96 loads the address
@ of data from a literal-pool word rather than adding an offset to pc, and
@ does not read a stack buffer back by popping it. These are TLA's own
@ variants of TBS SYSTEM/RUNTIME_TRANSFORM.S: the vector transform differs
@ from TBS's only in the matrix offset. The entries are TLA's own and carry
@ no credit.
@ credit: handwritten — TLA IWRAM runtime vector and matrix transforms
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x0800090c
	.set AlchemyRunAddress, 0x03000354
AlchemyUncredited_0800090c:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_0800090c:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	ldm	r0, {r2, r3, r4}
	add	r0, pc, #0x110
	ldm	r0!, {r5, r6, r7}
	smull	r8, r9, r2, r5
	smull	sl, fp, r2, r6
	smull	ip, lr, r2, r7
	ldm	r0!, {r5, r6, r7}
	smlal	r8, r9, r3, r5
	smlal	sl, fp, r3, r6
	smlal	ip, lr, r3, r7
	ldm	r0!, {r5, r6, r7}
	smlal	r8, r9, r4, r5
	smlal	sl, fp, r4, r6
	smlal	ip, lr, r4, r7
	lsr	r8, r8, #16
	orr	r8, r8, r9, lsl #16
	lsr	sl, sl, #16
	orr	sl, sl, fp, lsl #16
	lsr	ip, ip, #16
	orr	ip, ip, lr, lsl #16
	ldm	r0!, {r5, r6, r7}
	add	r5, r5, r8
	add	r6, r6, sl
	add	r7, r7, ip
	stmia	r1!, {r5, r6, r7}
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
	.thumb
AlchemyUncredited_08000980:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08000980:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	sub	sp, sp, #36
	mov	r4, sp
.L_030003d8:
	ldm	r0!, {r1, r2, r3}
	movs	r1, r1
	add	r5, pc, #144
	ldmne	r5, {r5, r6, r7}
	smull	r8, r9, r5, r1
	smull	sl, fp, r6, r1
	smull	ip, lr, r7, r1
	movs	r2, r2
	beq	.L_03000410
	add	r5, pc, #128
	ldmne	r5, {r5, r6, r7}
	smlalne	r8, r9, r5, r2
	smlalne	sl, fp, r6, r2
	smlalne	ip, lr, r7, r2
.L_03000410:
	movs	r3, r3
	beq	.L_0300042c
	add	r5, pc, #112
	ldmne	r5, {r5, r6, r7}
	smlalne	r8, r9, r5, r3
	smlalne	sl, fp, r6, r3
	smlalne	ip, lr, r7, r3
.L_0300042c:
	lsr	r8, r8, #16
	orr	r8, r8, r9, lsl #16
	lsr	sl, sl, #16
	orr	sl, sl, fp, lsl #16
	lsr	ip, ip, #16
	orr	ip, ip, lr, lsl #16
	sub	r2, r4, sp
	cmp	r2, #36
	stmiane	r4!, {r8, sl, ip}
	bne	.L_030003d8
	add	r1, pc, #64
	ldm	r1!, {r5, r6, r7}
	add	ip, ip, r7
	add	fp, sl, r6
	add	sl, r8, r5
	pop	{r0, r2, r3, r4, r5, r6, r7, r8, r9}
	stmdb	r1, {r0, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
