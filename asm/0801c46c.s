@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801c46c
	.thumb_func
Func_0801c46c:
	push	{lr}
	ldr	r2, [pc, #36]
	ldr	r1, [pc, #36]
	adds	r3, r1, r2
	ldrb	r2, [r3, #0]
	movs	r3, #32
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L0
	adds	r3, r2, #0
	adds	r3, #255
	b.n	.L1
.L0:
	adds	r3, r2, #1
.L1:
	lsls	r3, r3, #24
	lsrs	r2, r3, #24
	ldr	r0, [pc, #8]
	adds	r3, r1, r0
	strb	r2, [r3, #0]
	pop	{r0}
	bx	r0
	.4byte 0x00000205
	.4byte 0x02000240
