@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08078664
	.thumb_func
Func_08078664:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	Func_08077394
	ldr	r4, [pc, #36]
	movs	r1, #0
	adds	r0, #216
.L2:
	ldrh	r2, [r0, #0]
	adds	r3, r4, #0
	ands	r3, r2
	adds	r0, #2
	cmp	r3, r5
	bne.n	.L0
	adds	r0, r1, #0
	b.n	.L1
.L0:
	adds	r1, #1
	cmp	r1, #14
	ble.n	.L2
	movs	r0, #1
	negs	r0, r0
.L1:
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000001ff
