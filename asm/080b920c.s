@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b920c
	.thumb_func
Func_080b920c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r0, [sp, #12]
	movs	r0, #17
	bl	Func_08004970
	str	r0, [sp, #8]
	movs	r0, #9
	bl	Func_08004970
	str	r0, [sp, #4]
	ldr	r1, [sp, #4]
	movs	r0, #1
	bl	Func_080b6b40
	movs	r2, #0
	adds	r5, r0, #0
	mov	r8, r2
	mov	r9, r2
	cmp	r8, r5
	bge.n	.L0
	ldr	r6, [sp, #4]
	mov	sl, r5
.L6:
	ldrh	r0, [r6, #0]
	bl	Func_08077008
	movs	r3, #67
	adds	r3, r3, r0
	mov	ip, r3
	ldrb	r3, [r3, #0]
	movs	r5, #0
	cmp	r5, r3
	bge.n	.L1
	movs	r7, #158
	movs	r2, #156
	lsls	r7, r7, #1
	lsls	r2, r2, #1
	adds	r7, r7, r0
	adds	r2, r0, r2
	mov	lr, r7
	str	r2, [sp, #0]
	mov	r7, r9
	ldr	r2, [sp, #8]
	lsls	r3, r7, #1
	adds	r1, r3, r2
	mov	r3, r8
	lsls	r3, r3, #4
	ldr	r2, [sp, #12]
	ldr	r7, [pc, #64]
	mov	fp, r3
	add	r2, fp
	adds	r4, r6, #0
	mov	fp, r7
.L5:
	mov	r7, lr
	ldrb	r3, [r7, #0]
	cmp	r3, #0
	bne.n	.L2
	ldr	r7, [sp, #0]
	ldr	r3, [r7, #0]
	mov	r7, fp
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L3
.L2:
	ldrh	r3, [r4, #0]
	strh	r3, [r2, #0]
	adds	r3, r0, #0
	adds	r3, #64
	ldrh	r3, [r3, #0]
	strh	r3, [r2, #4]
	movs	r3, #8
	strh	r3, [r2, #6]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #8]
	movs	r3, #192
	lsls	r3, r3, #1
	movs	r7, #1
	strh	r3, [r2, #10]
	add	r8, r7
	adds	r2, #16
	b.n	.L4
	.4byte 0x00000000
	.4byte 0xffffff00
.L3:
	ldrh	r3, [r4, #0]
	strh	r3, [r1, #0]
	movs	r3, #1
	adds	r1, #2
	add	r9, r3
.L4:
	mov	r7, ip
	ldrb	r3, [r7, #0]
	adds	r5, #1
	cmp	r5, r3
	blt.n	.L5
.L1:
	movs	r2, #1
	negs	r2, r2
	add	sl, r2
	mov	r3, sl
	adds	r6, #2
	cmp	r3, #0
	bne.n	.L6
.L0:
	ldr	r2, [sp, #12]
	mov	r7, r8
	lsls	r3, r7, #4
	adds	r2, r2, r3
	str	r2, [sp, #12]
	adds	r0, r2, #0
	ldr	r1, [sp, #8]
	mov	r2, r9
	bl	Func_080151c0
	movs	r5, #1
	negs	r5, r5
	cmp	r0, #0
	blt.n	.L7
	mov	r3, r8
	adds	r5, r3, r0
.L7:
	ldr	r0, [sp, #4]
	bl	Func_08002df0
	ldr	r0, [sp, #8]
	bl	Func_08002df0
	adds	r0, r5, #0
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
