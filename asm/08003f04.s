@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08003f04
	.thumb_func
Func_08003f04:
	push	{r5, lr}
	movs	r4, #0
	cmp	r0, #95
	bhi.n	.L0
	movs	r1, #128
	ldr	r2, [pc, #40]
	movs	r5, #255
	lsls	r1, r1, #2
.L2:
	ldrb	r3, [r2, #0]
	cmp	r3, r0
	bne.n	.L1
	strb	r5, [r2, #0]
	adds	r4, #1
.L1:
	subs	r1, #1
	adds	r2, #1
	cmp	r1, #0
	bne.n	.L2
	cmp	r4, #0
	beq.n	.L3
.L0:
	movs	r0, #1
	negs	r0, r0
	b.n	.L4
.L3:
	movs	r0, #0
.L4:
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x03001810
