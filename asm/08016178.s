@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08016178
	.thumb_func
Func_08016178:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r3, #0
	ldr	r3, [pc, #156]
	ldr	r3, [r3, #0]
	mov	sl, r3
	lsls	r3, r1, #5
	adds	r3, r3, r0
	lsls	r3, r3, #1
	adds	r6, r2, #0
	mov	r2, sl
	adds	r5, r3, r2
	movs	r4, #240
	adds	r3, r1, r7
	sub	sp, #4
	mov	r8, r1
	lsls	r4, r4, #8
	cmp	r3, #20
	bls.n	.L0
	movs	r3, #20
	subs	r7, r3, r1
.L0:
	cmp	r6, #1
	bhi.n	.L1
	movs	r6, #2
.L1:
	cmp	r6, #30
	bls.n	.L2
	movs	r6, #30
.L2:
	cmp	r7, #1
	bhi.n	.L3
	movs	r7, #2
.L3:
	cmp	r7, #30
	bls.n	.L4
	movs	r7, #30
.L4:
	adds	r2, r6, #0
	mov	r1, r8
	adds	r3, r7, #0
	str	r4, [sp, #0]
	bl	Func_0801e260
	movs	r2, #0
	ldr	r4, [sp, #0]
	cmp	r2, r7
	bcs.n	.L5
	ldr	r0, [pc, #80]
	movs	r3, #32
	subs	r3, r3, r6
	add	r0, sl
	lsls	r1, r3, #1
.L9:
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L6
	mov	r4, r8
	adds	r3, r4, r2
	ldr	r4, [pc, #64]
	cmp	r3, #16
	bhi.n	.L6
	movs	r4, #240
	lsls	r4, r4, #8
.L6:
	movs	r3, #0
	cmp	r3, r6
	bcs.n	.L7
.L8:
	adds	r3, #1
	strh	r4, [r5, #0]
	adds	r5, #2
	cmp	r3, r6
	bcc.n	.L8
.L7:
	adds	r2, #1
	adds	r5, r5, r1
	cmp	r2, r7
	bcc.n	.L9
.L5:
	ldr	r2, [pc, #32]
	movs	r3, #1
	add	r2, sl
	strb	r3, [r2, #0]
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x00000ea5
	.4byte 0x0000f07f
	.4byte 0x00000ea3
