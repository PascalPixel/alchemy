@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800b8ac
	.thumb_func
Func_0800b8ac:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	ldr	r5, [r6, #40]
	mov	r8, r1
	movs	r7, #0
	cmp	r5, #0
	beq.n	.L0
	adds	r3, r6, #0
	adds	r3, #40
.L1:
	adds	r7, #1
	cmp	r7, #3
	bgt.n	.L0
	adds	r3, #4
	ldr	r5, [r3, #0]
	cmp	r5, #0
	bne.n	.L1
.L0:
	cmp	r7, #4
	bne.n	.L2
	movs	r0, #1
	negs	r0, r0
	b.n	.L3
.L2:
	mov	r0, r8
	bl	Func_0800bbc0
	adds	r5, r0, #0
	movs	r0, #0
	cmp	r5, #0
	beq.n	.L3
	lsls	r3, r7, #2
	adds	r3, #40
	mov	r0, r8
	str	r5, [r6, r3]
	bl	Func_08185000
	movs	r2, #39
	adds	r2, r2, r6
	ldrb	r3, [r2, #0]
	mov	ip, r2
	cmp	r3, #0
	bne.n	.L4
	ldrb	r2, [r0, #0]
	adds	r3, r6, #0
	adds	r3, #32
	strb	r2, [r3, #0]
	ldrb	r3, [r0, #1]
	adds	r2, r6, #0
	adds	r2, #33
	strb	r3, [r2, #0]
	ldrh	r3, [r0, #2]
	lsls	r3, r3, #8
	str	r3, [r6, #24]
	ldrb	r3, [r0, #7]
	adds	r2, #2
	strb	r3, [r2, #0]
	adds	r1, r6, #0
	ldrb	r3, [r0, #6]
	adds	r1, #34
	strb	r3, [r1, #0]
.L4:
	mov	r2, ip
	ldrb	r3, [r2, #0]
	cmp	r7, r3
	bne.n	.L5
	adds	r3, r7, #1
	strb	r3, [r2, #0]
.L5:
	adds	r0, r5, #0
.L3:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
