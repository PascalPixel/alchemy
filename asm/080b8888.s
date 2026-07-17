@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b8888
	.thumb_func
Func_080b8888:
	push	{r5, r6, lr}
	movs	r2, #0
	ldrsh	r5, [r0, r2]
	adds	r0, r5, #0
	bl	Func_08077008
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	Func_080b8808
	cmp	r0, #0
	bge.n	.L0
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L0:
	movs	r2, #56
	ldrsh	r3, [r6, r2]
	movs	r0, #0
	cmp	r3, #0
	ble.n	.L1
	bl	Func_08015118
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #12]
	bl	Func_080151c8
	movs	r0, #0
.L1:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000816
