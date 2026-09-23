@ Interpolation templates, sample fetch and channel mixing of TLA's
@ resident mixer. IWRAM_RUNTIME_MIX_LANES.S copies the templates at its
@ start into its loop; the fetch code returns two instructions past its
@ call with addne pc, lr, #8; the block mixing copies two-word templates
@ over the eight nop words of its own loop (add r1, pc, #52;
@ sub r0, pc, #108) each time before the loop runs, so those nops never run
@ as stored and carry no credit. It ends in Thumb,
@ looping back to IWRAM_RUNTIME_MIX.S for the next channel. A return past
@ the call and self-rewriting loops are beyond both approved compilers.
@ credit: handwritten — TLA IWRAM runtime channel mixing
.syntax unified
	.thumb_func
	.set	.L_030007b4, 0x030007b4
	.arm
	.set AlchemyLoadAddress, 0x080010b4
	.set AlchemyRunAddress, 0x03000afc
	asr	r9, r9, #22
	adds	r9, r9, r6, lsl #1
	adds	r7, r7, r4
	addpl	r6, ip, r6
	ldrsbpl	ip, [r3, #1]!
	subspl	ip, ip, r6
	adds	r9, r6, r9, asr #23
	add	r6, ip, r6
	adds	r7, r7, r4
	ldrsbpl	r6, [r3, #1]!
	ldrsb	ip, [r3, #1]!
	subs	ip, ip, r6
	adds	r9, r6, r9, asr #23
	add	r7, r7, r4
	add	r3, r3, r7, lsr #23
	ldrsb	r6, [r3]
	ldrsb	ip, [r3, #1]!
	subs	ip, ip, r6
.L_03000b44:
	add	r5, r5, r8, lsl #2
.L_03000b48:
	mul	r9, r7, ip
	asr	r9, r9, #22
	adds	r9, r9, r6, lsl #1
	ldrne	r0, [r5, -r8, lsl #2]
	mlane	r0, fp, r9, r0
	strne	r0, [r5, -r8, lsl #2]
	add	r7, r7, r4
	lsrs	r9, r7, #23
	beq	.L_03000b8c
	subs	r2, r2, r7, lsr #23
	blle	.L_03000ba4
	subs	r9, r9, #1
	addeq	r6, ip, r6
	ldrsbne	r6, [r3, r9]!
	ldrsb	ip, [r3, #1]!
	sub	ip, ip, r6
	bic	r7, r7, #0x3f800000
.L_03000b8c:
	subs	r8, r8, #1
	bgt	.L_03000b48
.L_03000b94:
	sub	r3, r3, #1
	pop	{r4, r9, ip}
	str	r7, [r4, #28]
	b	.L_03000cec
.L_03000ba4:
	add	r3, sp, #24
	ldm	r3, {r3, r6}
	cmp	r6, #0
	rsbne	r9, r2, #0
	addne	r2, r6, r2
	addne	pc, lr, #8
	pop	{r4, r9, ip}
	b	.L_03000bd8
.L_03000bc4:
	ldr	r2, [sp, #24]
	movs	r6, r2
	ldrne	r3, [sp, #20]
	bxne	lr
	pop	{r4, r9}
.L_03000bd8:
	strb	r6, [r4]
	b	.L_03000cf4
	lsls	r6, sl, #24
	asrs	r6, r6, #24
	lsls	r6, sl, #16
	asrs	r6, r6, #24
	lsls	r6, sl, #8
	asrs	r6, r6, #24
	asrs	r6, sl, #24
	ldm	r3!, {sl}
	lsls	r6, sl, #24
	asrs	r6, r6, #24
	lsls	r6, sl, #16
	asrs	r6, r6, #24
	lsls	r6, sl, #8
	asrs	r6, r6, #24
	pop	{r4, r9, ip}
.L_03000c1c:
	push	{r4, r9}
.L_03000c20:
	mov	lr, r2
	cmp	r2, r8
	movgt	lr, r8
	sub	lr, lr, #1
	lsrs	lr, lr, #2
	beq	.L_03000cbc
	sub	r8, r8, lr, lsl #2
	sub	r2, r2, lr, lsl #2
	add	r1, pc, #52
	sub	r0, pc, #108
	lsl	r9, r3, #30
	add	r0, r0, r9, lsr #27
	ldm	r0!, {r6, r7, r9, sl}
	stm	r1, {r6, r7}
	add	r1, r1, #12
	stm	r1, {r9, sl}
	add	r1, r1, #12
	ldm	r0, {r6, r7, r9, sl}
	stm	r1, {r6, r7}
	add	r1, r1, #12
	stm	r1, {r9, sl}
	ldm	r3!, {sl}
.L_03000c78:
	ldm	r5, {r0, r1, r7, r9}
AlchemyUncredited_08001234:
	nop
	nop
AlchemyUncreditedEnd_08001234:
	mlane	r0, fp, r6, r0
AlchemyUncredited_08001240:
	nop
	nop
AlchemyUncreditedEnd_08001240:
	mlane	r1, fp, r6, r1
AlchemyUncredited_0800124c:
	nop
	nop
AlchemyUncreditedEnd_0800124c:
	mlane	r7, fp, r6, r7
AlchemyUncredited_08001258:
	nop
	nop
AlchemyUncreditedEnd_08001258:
	mlane	r9, fp, r6, r9
	stmia	r5!, {r0, r1, r7, r9}
	subs	lr, lr, #1
	bne	.L_03000c78
	sub	r3, r3, #4
.L_03000cbc:
	mov	r1, #4
.L_03000cc0:
	ldr	r0, [r5]
	ldrsb	r6, [r3], #1
	mla	r0, fp, r6, r0
	str	r0, [r5], #4
	subs	r2, r2, #1
	bleq	.L_03000bc4
	subs	r1, r1, #1
	bgt	.L_03000cc0
	subs	r8, r8, #4
	bgt	.L_03000c20
	pop	{r4, r9}
.L_03000cec:
	str	r2, [r4, #24]
	str	r3, [r4, #40]
.L_03000cf4:
	add	r0, pc, #1
	bx	r0
	.thumb
.L_03000cfc:
	ldr	r0, [sp, #4]
	subs	r0, #1
	ble.n	.L_03000d06
	adds	r4, #64
	b.n	.L_030007b4
.L_03000d06:
	pop	{r4, r5, r6}
	add	r5, pc, #20
	stmia	r5!, {r4, r6}
	add	sp, #16
	pop	{r0, r1, r2, r3, r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	sl, r2
	mov	fp, r3
	movs	r0, #0
	pop	{pc}
