@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08091814
	.thumb_func
Func_08091814:
	push	{r5, r6, lr}
	lsrs	r5, r0, #10
	movs	r3, #15
	ldr	r6, [pc, #56]
	ands	r5, r3
	ands	r6, r0
	cmp	r5, #7
	ble.n	.L0
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L0:
	adds	r0, r5, #0
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L2
	movs	r0, #2
	negs	r0, r0
	b.n	.L1
.L2:
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	Func_08077090
	cmp	r0, #0
	bne.n	.L3
	movs	r0, #3
	negs	r0, r0
	b.n	.L1
.L3:
	movs	r0, #0
.L1:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x000003ff
