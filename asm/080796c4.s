@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080796c4
	.thumb_func
Func_080796c4:
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r0, #0
	cmp	r5, #0
	beq.n	.L0
	bl	Func_080795fc
	movs	r1, #0
	cmp	r0, #0
	beq.n	.L1
	ldr	r3, [pc, #32]
	movs	r4, #252
	lsls	r4, r4, #1
	adds	r2, r3, r4
.L2:
	ldrb	r3, [r2, #0]
	adds	r1, #1
	strh	r3, [r5, #0]
	adds	r2, #1
	adds	r5, #2
	cmp	r1, r0
	bne.n	.L2
.L1:
	ldr	r3, [pc, #8]
	strh	r3, [r5, #0]
.L0:
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x000000ff
	.4byte 0x02000240
