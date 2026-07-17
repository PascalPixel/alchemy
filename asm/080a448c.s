@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a448c
	.thumb_func
Func_080a448c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #296]
	movs	r1, #188
	ldr	r7, [r3, #0]
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldrh	r3, [r3, #0]
	adds	r5, r0, #0
	ldr	r0, [pc, #284]
	ands	r0, r3
	bl	Func_08077018
	ldrb	r3, [r0, #2]
	mov	r8, r0
	cmp	r3, #0
	bne.n	.L0
	movs	r2, #1
	movs	r3, #1
	negs	r2, r2
	strb	r3, [r5, #0]
	adds	r3, r2, #0
	b.n	.L1
.L0:
	movs	r1, #1
	negs	r1, r1
	adds	r3, r1, #0
	strb	r3, [r5, #0]
	movs	r3, #1
.L1:
	strb	r3, [r5, #1]
	ldr	r2, [pc, #248]
	movs	r1, #188
	adds	r3, r7, r2
	lsls	r1, r1, #1
	ldrb	r0, [r3, #0]
	adds	r3, r7, r1
	ldrh	r1, [r3, #0]
	bl	Func_080a46b4
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L2
	movs	r3, #1
	strb	r3, [r5, #0]
	b.n	.L3
.L2:
	strb	r0, [r5, #0]
.L3:
	movs	r3, #188
	lsls	r3, r3, #1
	adds	r6, r7, r3
	ldrh	r2, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L4
	movs	r1, #1
	negs	r1, r1
	adds	r3, r1, #0
	strb	r3, [r5, #0]
	ldrh	r2, [r6, #0]
.L4:
	ldr	r1, [pc, #188]
	adds	r3, r7, r1
	subs	r1, #27
	ldrb	r0, [r3, #0]
	ands	r1, r2
	bl	Func_08077218
	cmp	r0, #0
	bne.n	.L5
	movs	r2, #1
	negs	r2, r2
	adds	r3, r2, #0
	strb	r3, [r5, #1]
.L5:
	movs	r1, #1
	strb	r1, [r5, #3]
	strb	r1, [r5, #5]
	strb	r1, [r5, #2]
	movs	r3, #128
	ldrh	r2, [r6, #0]
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L6
	strb	r1, [r5, #4]
	movs	r1, #1
	negs	r1, r1
	adds	r3, r1, #0
	strb	r3, [r5, #1]
	b.n	.L7
.L6:
	movs	r2, #1
	negs	r2, r2
	adds	r3, r2, #0
	strb	r3, [r5, #4]
.L7:
	mov	r3, r8
	ldrb	r2, [r3, #3]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L8
	movs	r2, #1
	negs	r2, r2
	adds	r1, r2, #0
	movs	r2, #188
	lsls	r2, r2, #1
	strb	r1, [r5, #4]
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L8
	strb	r1, [r5, #3]
	strb	r1, [r5, #5]
.L8:
	movs	r1, #188
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #68]
	ands	r0, r3
	bl	Func_0808a490
	cmp	r0, #0
	beq.n	.L9
	movs	r3, #1
	strb	r3, [r5, #0]
.L9:
	ldr	r2, [pc, #60]
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bhi.n	.L10
	movs	r1, #1
	negs	r1, r1
	adds	r3, r1, #0
	strb	r3, [r5, #3]
.L10:
	mov	r3, r8
	ldrb	r2, [r3, #3]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L11
	movs	r1, #1
	negs	r1, r1
	adds	r3, r1, #0
	strb	r3, [r5, #5]
.L11:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x000001ff
	.4byte 0x0000021a
	.4byte 0x00000219
