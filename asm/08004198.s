@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08004198
	.thumb_func
Func_08004198:
	push	{r5, lr}
	movs	r5, #1
	ldr	r4, [pc, #48]
	negs	r5, r5
	ldr	r3, [pc, #48]
	ldrh	r2, [r3, #0]
	strh	r3, [r3, #0]
	movs	r1, #0
	ldr	r3, [r4, #0]
	cmp	r3, r0
	bne.n	.L0
	movs	r5, #0
	b.n	.L1
.L0:
	adds	r1, #1
	adds	r4, #8
	cmp	r1, #19
	bgt.n	.L1
	ldr	r3, [r4, #0]
	cmp	r3, r0
	bne.n	.L0
	adds	r5, r1, #0
.L1:
	ldr	r3, [pc, #16]
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001a20
	.4byte 0x04000208
