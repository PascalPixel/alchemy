@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08016f2c
	.thumb_func
Func_08016f2c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #200]
	movs	r1, #160
	ldr	r7, [r3, #0]
	lsls	r1, r1, #3
	movs	r2, #0
	adds	r5, r7, r1
	mov	r8, r2
.L4:
	ldrh	r6, [r5, #22]
	cmp	r6, #0
	beq.n	.L0
	movs	r4, #24
	ldrsh	r3, [r5, r4]
	cmp	r3, #0
	beq.n	.L1
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_08017004
	ldrh	r3, [r5, #24]
	subs	r3, #1
	strh	r3, [r5, #24]
	b.n	.L2
.L1:
	movs	r1, #26
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	beq.n	.L2
	adds	r0, r5, #0
	bl	Func_08016230
	b.n	.L2
.L0:
	movs	r3, #26
	ldrsh	r2, [r5, r3]
	cmp	r2, #0
	beq.n	.L2
	movs	r4, #24
	ldrsh	r3, [r5, r4]
	cmp	r3, r2
	beq.n	.L3
	movs	r1, #28
	ldrsh	r0, [r5, r1]
	movs	r2, #30
	ldrsh	r1, [r5, r2]
	movs	r3, #32
	ldrsh	r2, [r5, r3]
	movs	r4, #34
	ldrsh	r3, [r5, r4]
	bl	Func_08016178
	movs	r1, #1
	adds	r0, r5, #0
	bl	Func_08017004
	ldrh	r3, [r5, #24]
	ldr	r1, [pc, #96]
	adds	r3, #1
	strh	r3, [r5, #24]
	movs	r2, #1
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	b.n	.L2
.L3:
	movs	r3, #30
	ldrsh	r1, [r5, r3]
	movs	r2, #28
	ldrsh	r0, [r5, r2]
	movs	r4, #34
	ldrsh	r3, [r5, r4]
	movs	r4, #32
	ldrsh	r2, [r5, r4]
	bl	Func_08016178
	ldr	r1, [pc, #64]
	movs	r3, #1
	adds	r2, r7, r1
	str	r6, [r5, #0]
	str	r6, [r5, #4]
	strh	r6, [r5, #8]
	strh	r6, [r5, #10]
	strh	r6, [r5, #12]
	strh	r6, [r5, #14]
	strh	r6, [r5, #16]
	strh	r6, [r5, #18]
	strh	r6, [r5, #20]
	strh	r6, [r5, #22]
	strh	r6, [r5, #24]
	strh	r6, [r5, #26]
	strh	r6, [r5, #28]
	strh	r6, [r5, #30]
	strh	r6, [r5, #32]
	strh	r6, [r5, #34]
	strb	r3, [r2, #0]
.L2:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r5, #36
	cmp	r3, #8
	bne.n	.L4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
	.4byte 0x00000ea3
