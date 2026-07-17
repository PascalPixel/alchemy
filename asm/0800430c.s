@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800430c
	.thumb_func
Func_0800430c:
	push	{r5, r6, r7, lr}
	movs	r5, #1
	ldr	r1, [pc, #60]
	negs	r5, r5
	ldr	r3, [pc, #60]
	ldrh	r2, [r3, #0]
	adds	r6, r2, #0
	strh	r3, [r3, #0]
	movs	r4, #0
	movs	r7, #1
.L1:
	ldrb	r3, [r1, #3]
	cmp	r3, #2
	bne.n	.L0
	ldrb	r2, [r1, #6]
	adds	r3, r7, #0
	ands	r3, r2
	movs	r0, #1
	cmp	r3, #0
	bne.n	.L0
	ldrb	r3, [r1, #5]
	orrs	r3, r0
	strb	r3, [r1, #5]
	adds	r5, r4, #0
.L0:
	adds	r4, #1
	adds	r1, #8
	cmp	r4, #19
	ble.n	.L1
	ldr	r3, [pc, #16]
	strh	r6, [r3, #0]
	adds	r0, r5, #0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001a20
	.4byte 0x04000208
