@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800b074
	.thumb_func
Func_0800b074:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	ip, r0
	mov	fp, r3
	mov	r3, ip
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsrs	r0, r3, #1
	movs	r3, #33
	add	r3, ip
	mov	lr, r3
	ldrb	r3, [r3, #0]
	sub	sp, #8
	lsrs	r4, r3, #1
	movs	r3, #8
	str	r3, [sp, #4]
	movs	r3, #4
	str	r3, [sp, #0]
	mov	sl, r2
	ldr	r2, [sp, #44]
	movs	r3, #1
	mov	r9, r3
	ldmia	r2!, {r3}
	ldr	r5, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #9
	cmp	r3, r2
	bgt.n	.L0
	cmp	r5, r2
	ble.n	.L1
.L0:
	movs	r2, #3
	mov	r9, r2
	movs	r3, #16
	movs	r2, #8
	str	r3, [sp, #4]
	str	r2, [sp, #0]
	lsls	r0, r0, #1
	lsls	r4, r4, #1
.L1:
	asrs	r1, r1, #16
	subs	r7, r1, r0
	mov	r3, fp
	mov	r0, sl
	mov	r2, lr
	mov	r8, r1
	subs	r1, r3, r0
	ldrb	r3, [r2, #0]
	mov	r2, ip
	adds	r2, #35
	ldrb	r2, [r2, #0]
	lsls	r2, r2, #24
	asrs	r2, r2, #24
	lsrs	r3, r3, #1
	subs	r3, r3, r2
	muls	r3, r5
	asrs	r1, r1, #16
	subs	r1, r1, r4
	ldr	r4, [pc, #88]
	adds	r3, r3, r4
	asrs	r3, r3, #16
	subs	r6, r1, r3
	mov	r0, ip
	movs	r1, #4
	ldrb	r2, [r0, #5]
	negs	r1, r1
	adds	r3, r1, #0
	mov	r5, r9
	ands	r3, r2
	orrs	r3, r5
	ldr	r2, [pc, #68]
	strb	r3, [r0, #5]
	ldr	r4, [pc, #56]
	ldrh	r0, [r0, #6]
	adds	r3, r2, #0
	ands	r7, r4
	ands	r3, r0
	orrs	r3, r7
	mov	r0, ip
	strh	r3, [r0, #6]
	strb	r6, [r0, #4]
	ldr	r0, [sp, #4]
	mov	r3, r8
	subs	r7, r3, r0
	ldr	r3, [sp, #40]
	mov	r0, fp
	subs	r3, r0, r3
	ldr	r0, [sp, #0]
	asrs	r3, r3, #16
	subs	r6, r3, r0
	mov	r3, ip
	adds	r3, #12
	ldrb	r0, [r3, #5]
	ands	r1, r0
	orrs	r1, r5
	strb	r1, [r3, #5]
	ldrh	r1, [r3, #6]
	ands	r7, r4
	ands	r2, r1
	orrs	r2, r7
	b.n	.L2
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0xfffffe00
.L2:
	strh	r2, [r3, #6]
	strb	r6, [r3, #4]
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
