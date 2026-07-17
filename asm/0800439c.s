@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800439c
	.thumb_func
Func_0800439c:
	push	{r5, r6, r7, lr}
	movs	r5, #1
	ldr	r4, [pc, #52]
	negs	r5, r5
	ldr	r3, [pc, #52]
	ldrh	r2, [r3, #0]
	adds	r7, r2, #0
	strh	r3, [r3, #0]
	movs	r1, #0
	movs	r6, #254
.L2:
	cmp	r0, #0
	beq.n	.L0
	ldr	r3, [r4, #0]
	cmp	r3, r0
	bne.n	.L1
.L0:
	ldrb	r2, [r4, #5]
	adds	r3, r6, #0
	ands	r3, r2
	strb	r3, [r4, #5]
	adds	r5, r1, #0
.L1:
	adds	r1, #1
	adds	r4, #8
	cmp	r1, #19
	ble.n	.L2
	ldr	r3, [pc, #12]
	strh	r7, [r3, #0]
	adds	r0, r5, #0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001a20
	.4byte 0x04000208
