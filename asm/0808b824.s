@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0808b824
	.thumb_func
Func_0808b824:
	push	{lr}
	ldr	r3, [pc, #40]
	ldr	r3, [r3, #0]
	adds	r1, r3, #0
	movs	r0, #7
	movs	r2, #8
	adds	r1, #52
.L1:
	ldmia	r1!, {r3}
	cmp	r3, #0
	beq.n	.L0
	adds	r0, r2, #0
.L0:
	adds	r2, #1
	cmp	r2, #65
	ble.n	.L1
	adds	r0, #1
	cmp	r0, #66
	bne.n	.L2
	movs	r0, #1
	negs	r0, r0
.L2:
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001ebc
