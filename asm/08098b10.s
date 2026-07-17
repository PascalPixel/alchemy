@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08098b10
	.thumb_func
Func_08098b10:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #232]
	adds	r7, r0, #0
	ldr	r3, [r3, #0]
	movs	r1, #64
	adds	r1, r1, r7
	sub	sp, #12
	mov	sl, r3
	mov	r8, r1
.L4:
	mov	r2, r8
	movs	r6, #0
	ldrsb	r6, [r2, r6]
	cmp	r6, #0
	bne.n	.L0
	ldr	r3, [r7, #20]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r7, #24]
	str	r3, [r5, #8]
	bl	Func_08004458
	adds	r1, r0, #0
	lsls	r1, r1, #16
	movs	r0, #200
	lsrs	r1, r1, #16
	lsls	r0, r0, #13
	adds	r2, r5, #0
	bl	Func_0800447c
	ldr	r3, [r5, #0]
	str	r3, [r7, #12]
	ldr	r3, [r5, #8]
	str	r3, [r7, #16]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r7, #36]
	str	r3, [r7, #32]
	adds	r3, r7, #0
	adds	r3, #66
	strb	r6, [r3, #0]
	b.n	.L1
.L0:
	cmp	r6, #1
	bne.n	.L2
	adds	r0, r7, #0
	bl	Func_0809ba34
	cmp	r0, #0
	bne.n	.L3
	mov	r2, r8
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	b.n	.L4
.L2:
	cmp	r6, #2
	bne.n	.L5
	mov	r3, sl
	ldr	r2, [r3, #16]
	ldr	r3, [r2, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	movs	r1, #128
	ldr	r3, [r2, #12]
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r5, #4]
	ldr	r3, [r2, #16]
	str	r3, [r5, #8]
	mov	r2, sl
	movs	r0, #128
	ldr	r1, [r2, #0]
	lsls	r0, r0, #12
	adds	r2, r5, #0
	bl	Func_0800447c
	adds	r0, r5, #0
	bl	Func_080974d8
	bl	Func_08004458
	adds	r1, r0, #0
	movs	r0, #128
	adds	r2, r5, #0
	lsls	r0, r0, #11
	bl	Func_0800447c
	ldr	r3, [r5, #0]
	str	r3, [r7, #12]
	ldr	r3, [r5, #8]
	str	r3, [r7, #16]
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r2, r7, #0
	strh	r3, [r7, #50]
	adds	r2, #66
	movs	r3, #1
	strb	r3, [r2, #0]
.L1:
	mov	r1, r8
	ldrb	r3, [r1, #0]
	adds	r3, #1
	strb	r3, [r1, #0]
	b.n	.L3
.L5:
	cmp	r6, #3
	bne.n	.L3
	adds	r0, r7, #0
	bl	Func_0809ba34
	cmp	r0, #0
	bne.n	.L3
	adds	r0, r7, #0
	bl	Func_0809bb34
.L3:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f30
