@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801ce48
	.thumb_func
Func_0801ce48:
	push	{lr}
	ldr	r1, [pc, #24]
	adds	r0, r0, r1
	ldrh	r2, [r0, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L0
	movs	r3, #2
	b.n	.L1
.L0:
	ldr	r1, [pc, #12]
	adds	r3, r2, r1
.L1:
	strh	r3, [r0, #0]
	pop	{r0}
	bx	r0
	.4byte 0x00000574
	.4byte 0x0000ffff
