@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08004080
	.thumb_func
Func_08004080:
	push	{lr}
	ldr	r1, [pc, #40]
	ldr	r4, [pc, #40]
	ldrh	r3, [r1, #2]
	movs	r0, #96
	movs	r2, #0
	cmp	r3, r4
	bne.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	adds	r2, #1
	adds	r1, #4
	cmp	r2, #95
	bgt.n	.L1
	ldrh	r3, [r1, #2]
	ldr	r4, [pc, #16]
	cmp	r3, r4
	bne.n	.L0
	adds	r0, r2, #0
.L1:
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001b10
	.4byte 0x0000ffff
