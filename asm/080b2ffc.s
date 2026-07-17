@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b2ffc
	.thumb_func
Func_080b2ffc:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #72]
	movs	r2, #236
	ldr	r7, [r3, #0]
	lsls	r2, r2, #2
	adds	r5, r7, r2
	movs	r6, #23
.L0:
	adds	r0, r5, #0
	subs	r6, #1
	bl	Func_0808a500
	adds	r5, #72
	cmp	r6, #0
	bge.n	.L0
	ldr	r2, [pc, #48]
	adds	r3, r7, r2
	movs	r5, #0
	ldrsb	r5, [r3, r5]
	movs	r3, #1
	negs	r3, r3
	cmp	r5, r3
	beq.n	.L1
	bl	Func_08004458
	movs	r2, #138
	lsls	r1, r0, #3
	lsls	r3, r5, #2
	lsls	r2, r2, #1
	subs	r1, r1, r0
	adds	r3, r3, r2
	lsrs	r1, r1, #16
	ldr	r0, [r7, r3]
	bl	Func_08009248
.L1:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x000003ab
