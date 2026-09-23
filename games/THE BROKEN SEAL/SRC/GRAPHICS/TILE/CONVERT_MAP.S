@ Unpacks a map for drawing: it spreads 16,384 halfword cells to every other
@ halfword of the output, numbering the cells marked 0xfff consecutively, then
@ fills the gaps by interleaving two 16 KB byte planes. It runs from a heap
@ copy of exactly these 156 bytes. It saves r5-r8 without lr and stores with
@ strh ip, [r1], #4, a post-increment larger than the access, which GCC 2.96
@ does not emit for halfwords. Every TBS and TLA edition has these bytes.
@ credit: library — TBS/TLA common ARM graphics and runtime kernel bank
.syntax unified
	.arm
	.global Func_0800a37c
Func_0800a37c:
	push	{r5, r6, r7, r8}
	mov	r5, #-16777216
	orr	r5, r5, r5, lsr #4
	orr	r4, r5, r5, lsr #8
	add	r4, r4, #1
	mov	r6, #16384
.L_0800a394:
	ldrh	ip, [r0], #2
	cmp	r5, ip, lsl #20
	bne	.L_0800a3ac
	add	ip, ip, r4
	cmp	r4, #0
	addne	r4, r4, #1
.L_0800a3ac:
	strh	ip, [r1], #4
	subs	r6, r6, #1
	bne	.L_0800a394
	add	r0, r2, #16384
	sub	r1, r1, #65536
	add	r1, r1, #2
	mov	r5, #255
	orr	r5, r5, r5, lsl #16
	mov	r8, #4096
.L_0800a3d0:
	ldr	r4, [r2], #4
	ldr	ip, [r0], #4
	and	r3, r4, r5
	and	r6, ip, r5
	orr	r3, r3, r6, lsl #8
	and	r7, r4, r5, lsl #8
	and	r6, ip, r5, lsl #8
	orr	r7, r6, r7, lsr #8
	strh	r3, [r1], #4
	strh	r7, [r1], #4
	ror	r3, r3, #16
	ror	r7, r7, #16
	strh	r3, [r1], #4
	strh	r7, [r1], #4
	subs	r8, r8, #1
	bne	.L_0800a3d0
	pop	{r5, r6, r7, r8}
	bx	lr
