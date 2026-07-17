@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800c2d8
	.thumb_func
Func_0800c2d8:
	push	{lr}
	cmp	r0, #0
	beq.n	.L0
	movs	r2, #0
	strh	r2, [r0, #4]
	ldr	r3, [pc, #20]
	adds	r2, r0, #0
	adds	r2, #91
	str	r1, [r0, #0]
	strb	r3, [r2, #0]
	adds	r2, #2
	strb	r3, [r2, #0]
	subs	r2, #6
	strb	r3, [r2, #0]
	b.n	.L0
	movs	r0, r0
	.4byte 0x00000000
.L0:
	pop	{r0}
	bx	r0
