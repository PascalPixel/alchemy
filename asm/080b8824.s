@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b8824
	.thumb_func
Func_080b8824:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	bl	Func_08004458
	lsls	r0, r0, #4
	lsrs	r0, r0, #16
	cmp	r0, #0
	beq.n	.L0
	mov	r5, sp
	movs	r0, #1
	adds	r1, r5, #0
	bl	Func_080b6b40
	adds	r6, r0, #0
	movs	r7, #0
	cmp	r6, #0
	beq.n	.L1
	mov	r8, r5
	movs	r5, #0
.L2:
	mov	r2, r8
	ldrsh	r0, [r5, r2]
	bl	Func_080b8064
	adds	r7, #1
	movs	r0, #8
	bl	Func_080030f8
	adds	r5, #2
	cmp	r7, r6
	bne.n	.L2
.L1:
	movs	r0, #22
	bl	Func_080030f8
	movs	r0, #1
	b.n	.L3
.L0:
	ldr	r0, [pc, #20]
	bl	Func_080151c8
	movs	r0, #0
.L3:
	add	sp, #28
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000844
