@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08078bc0
	.thumb_func
Func_08078bc0:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	Func_08077394
	ldr	r1, [pc, #32]
	movs	r2, #0
	adds	r0, #88
.L2:
	ldrh	r3, [r0, #0]
	ands	r3, r1
	adds	r0, #4
	cmp	r3, r5
	bne.n	.L0
	movs	r0, #1
	b.n	.L1
.L0:
	adds	r2, #1
	cmp	r2, #31
	ble.n	.L2
	movs	r0, #0
.L1:
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00003fff
