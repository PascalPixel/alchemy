@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080064b8
	.thumb_func
Func_080064b8:
	push	{r5, lr}
	ldr	r3, [pc, #44]
	ldr	r3, [r3, #0]
	movs	r5, #0
	b.n	.L0
.L2:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [pc, #32]
	adds	r5, #1
	cmp	r5, r3
	bhi.n	.L1
	ldr	r3, [pc, #20]
	ldr	r3, [r3, #0]
.L0:
	cmp	r3, #0
	bne.n	.L2
	ldr	r3, [pc, #20]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L2
.L1:
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02002080
	.4byte 0x000927bf
	.4byte 0x020023ac
