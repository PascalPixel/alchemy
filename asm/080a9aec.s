@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a9aec
	.thumb_func
Func_080a9aec:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #56]
	mov	r8, r3
	movs	r3, #14
	adds	r6, r0, #0
	adds	r7, r1, #0
	mov	sl, r3
.L6:
	ldrh	r0, [r7, #0]
	ldr	r3, [pc, #36]
	ands	r3, r0
	adds	r7, #2
	cmp	r3, #0
	beq.n	.L0
	ldr	r3, [pc, #28]
	adds	r5, r3, #0
	ands	r5, r0
	adds	r0, r5, #0
	bl	Func_08077018
	ldrb	r3, [r0, #2]
	cmp	r3, #2
	beq.n	.L1
	cmp	r3, #2
	bgt.n	.L2
	cmp	r3, #1
	beq.n	.L3
	b.n	.L0
	.4byte 0x00000200
	.4byte 0x000001ff
	.4byte 0x00000182
.L2:
	cmp	r3, #3
	beq.n	.L4
	cmp	r3, #4
	beq.n	.L5
	b.n	.L0
.L3:
	mov	r3, r8
	adds	r0, r5, r3
	adds	r1, r6, #0
	movs	r2, #8
	movs	r3, #8
	bl	Func_08015080
	b.n	.L0
.L1:
	mov	r3, r8
	adds	r0, r5, r3
	adds	r1, r6, #0
	movs	r2, #8
	movs	r3, #56
	bl	Func_08015080
	b.n	.L0
.L4:
	mov	r3, r8
	adds	r0, r5, r3
	adds	r1, r6, #0
	movs	r2, #8
	movs	r3, #40
	bl	Func_08015080
	b.n	.L0
.L5:
	mov	r3, r8
	adds	r0, r5, r3
	adds	r1, r6, #0
	movs	r2, #8
	movs	r3, #24
	bl	Func_08015080
.L0:
	movs	r3, #1
	negs	r3, r3
	add	sl, r3
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L6
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
