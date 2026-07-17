@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08019bfc
	.thumb_func
Func_08019bfc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [r0, #0]
	movs	r2, #255
	lsrs	r3, r5, #8
	ands	r5, r2
	ldr	r2, [pc, #244]
	lsls	r3, r3, #3
	ldr	r1, [r2, r3]
	adds	r3, #4
	ldr	r2, [r2, r3]
	lsls	r3, r5, #1
	ldrh	r3, [r3, r2]
	adds	r1, r1, r3
	mov	sl, r0
	ldr	r6, [r0, #4]
	subs	r2, r1, #1
	movs	r3, #0
	movs	r5, #128
	ldr	r0, [r0, #8]
	movs	r4, #1
	mov	r9, r2
	mov	lr, r3
	movs	r7, #1
	mov	r8, r5
	b.n	.L0
.L7:
	adds	r2, r0, #0
	movs	r3, #1
	ands	r2, r3
	asrs	r0, r0, #1
	cmp	r2, #0
	beq.n	.L0
	cmp	r0, #0
	bne.n	.L1
	ldrb	r0, [r6, #0]
	adds	r2, r0, #0
	ands	r2, r3
	asrs	r0, r0, #1
	mov	r3, r8
	adds	r6, #1
	orrs	r0, r3
.L1:
	cmp	r2, #0
	beq.n	.L0
	movs	r5, #1
	movs	r2, #128
	movs	r3, #0
	mov	fp, r5
	mov	ip, r2
.L6:
	adds	r2, r4, #0
	mov	r5, fp
	ands	r2, r5
	asrs	r4, r4, #1
	cmp	r2, #0
	beq.n	.L2
	cmp	r4, #0
	bne.n	.L3
	ldrb	r4, [r1, #0]
	adds	r2, r4, #0
	ands	r2, r5
	asrs	r4, r4, #1
	mov	r5, ip
	adds	r1, #1
	orrs	r4, r5
.L3:
	cmp	r2, #0
	bne.n	.L4
.L2:
	adds	r3, #1
	b.n	.L5
.L4:
	movs	r2, #1
	add	lr, r2
	subs	r3, #1
.L5:
	cmp	r3, #0
	bge.n	.L6
.L0:
	adds	r2, r4, #0
	ands	r2, r7
	asrs	r4, r4, #1
	cmp	r2, #0
	beq.n	.L7
	cmp	r4, #0
	bne.n	.L8
	ldrb	r4, [r1, #0]
	mov	r3, r8
	adds	r2, r4, #0
	asrs	r4, r4, #1
	adds	r1, #1
	ands	r2, r7
	orrs	r4, r3
.L8:
	cmp	r2, #0
	beq.n	.L7
	mov	r5, lr
	lsls	r3, r5, #1
	adds	r1, r3, r5
	lsls	r3, r1, #2
	movs	r2, #7
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L9
	mov	r2, r9
	lsrs	r3, r1, #1
	subs	r3, r2, r3
	ldrb	r2, [r3, #0]
	subs	r3, #1
	lsls	r5, r2, #4
	ldrb	r2, [r3, #0]
	lsrs	r3, r2, #4
	orrs	r5, r3
	b.n	.L10
.L9:
	lsrs	r3, r1, #1
	mov	r5, r9
	subs	r3, r5, r3
	ldrb	r2, [r3, #0]
	movs	r1, #15
	subs	r3, #1
	ands	r2, r1
	ldrb	r5, [r3, #0]
	lsls	r2, r2, #8
	orrs	r5, r2
.L10:
	mov	r2, sl
	str	r0, [r2, #8]
	str	r5, [r2, #0]
	str	r6, [r2, #4]
	adds	r0, r5, #0
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x0803842c
