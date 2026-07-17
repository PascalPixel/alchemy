@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08078698
	.thumb_func
Func_08078698:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #100]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r5, r3, r2
	mov	r8, r0
	mov	r1, r8
	ldr	r0, [r5, #0]
	sub	sp, #24
	bl	Func_08078664
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L0
	ldr	r0, [r5, #0]
	b.n	.L1
.L3:
	adds	r0, r5, #0
	b.n	.L1
.L0:
	add	r5, sp, #4
	adds	r0, r5, #0
	bl	Func_080796c4
	movs	r6, #0
	adds	r7, r0, #0
	adds	r3, r5, #0
	cmp	r6, r7
	bge.n	.L2
.L4:
	movs	r2, #0
	ldrsh	r5, [r3, r2]
	mov	r1, r8
	adds	r3, #2
	adds	r0, r5, #0
	str	r3, [sp, #0]
	bl	Func_08078664
	movs	r2, #1
	negs	r2, r2
	ldr	r3, [sp, #0]
	cmp	r0, r2
	bne.n	.L3
	adds	r6, #1
	cmp	r6, r7
	blt.n	.L4
.L2:
	movs	r0, #1
	negs	r0, r0
.L1:
	add	sp, #24
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x02000240
