@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800b7c0
	.thumb_func
Func_0800b7c0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	movs	r2, #39
	adds	r2, r2, r7
	ldrb	r3, [r2, #0]
	movs	r1, #0
	sub	sp, #8
	mov	r8, r2
	cmp	r1, r3
	bge.n	.L0
	adds	r3, r7, #0
	adds	r3, #40
	str	r3, [sp, #4]
	mov	sl, r1
.L4:
	ldr	r3, [sp, #4]
	ldmia	r3!, {r6}
	adds	r2, r3, #0
	str	r2, [sp, #4]
	movs	r2, #0
	ldrsh	r0, [r6, r2]
	str	r1, [sp, #0]
	bl	Func_08185000
	adds	r5, r0, #0
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	ldr	r1, [sp, #0]
	cmp	r3, #0
	beq.n	.L1
	cmp	r1, #0
	bne.n	.L2
	adds	r3, r7, #0
	adds	r3, #32
	strb	r2, [r3, #0]
	ldrb	r3, [r5, #1]
	adds	r2, r7, #0
	adds	r2, #33
	strb	r3, [r2, #0]
	ldrh	r3, [r5, #2]
	lsls	r3, r3, #8
	str	r3, [r7, #24]
	ldrb	r3, [r5, #7]
	adds	r2, #2
	strb	r3, [r2, #0]
	ldrb	r3, [r5, #6]
	subs	r2, #1
	strb	r3, [r2, #0]
.L2:
	ldr	r0, [r5, #12]
	cmp	r0, #0
	bne.n	.L3
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	str	r1, [sp, #0]
	bl	Func_0800b798
	ldr	r1, [sp, #0]
.L3:
	ldrb	r3, [r5, #4]
	strb	r3, [r6, #4]
	ldr	r3, [r5, #16]
	str	r0, [r6, #8]
	str	r3, [r6, #12]
	ldrb	r3, [r5, #10]
	mov	r2, sl
	strb	r3, [r6, #7]
	movs	r3, #255
	strb	r3, [r6, #22]
	str	r2, [r6, #16]
	strb	r2, [r6, #20]
.L1:
	mov	r2, r8
	ldrb	r3, [r2, #0]
	adds	r1, #1
	cmp	r1, r3
	blt.n	.L4
.L0:
	movs	r0, #0
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
