@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a46b4
	.thumb_func
Func_080a46b4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #140]
	adds	r6, r1, #0
	ands	r6, r3
	mov	sl, r0
	adds	r0, r6, #0
	bl	Func_08077018
	movs	r7, #1
	adds	r5, r0, #0
	adds	r0, r6, #0
	negs	r7, r7
	bl	Func_0808a490
	cmp	r0, #0
	beq.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	ldrh	r3, [r5, #40]
	ldr	r0, [pc, #108]
	ands	r0, r3
	bl	Func_08077080
	ldrh	r3, [r5, #40]
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L2
	ldrb	r3, [r5, #2]
	cmp	r3, #0
	beq.n	.L3
	ldrb	r3, [r5, #12]
	cmp	r3, #3
	beq.n	.L4
	mov	r0, sl
	adds	r1, r6, #0
	bl	Func_08077218
	cmp	r0, #0
	beq.n	.L4
.L3:
	movs	r7, #1
.L4:
	cmp	r7, #1
	bne.n	.L2
	mov	r3, r8
	ldrb	r2, [r3, #1]
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L5
	mov	r3, r8
	ldrb	r2, [r3, #8]
	movs	r3, #255
	eors	r2, r3
	negs	r3, r2
	orrs	r3, r2
	lsrs	r7, r3, #31
	movs	r3, #2
	b.n	.L6
.L5:
	movs	r3, #128
	ands	r3, r2
	negs	r2, r3
	orrs	r2, r3
	lsrs	r2, r2, #31
	adds	r7, r2, #0
	movs	r3, #0
.L6:
	subs	r7, r3, r7
.L2:
	adds	r0, r7, #0
.L1:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x00003fff
