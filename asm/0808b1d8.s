@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0808b1d8
	.thumb_func
Func_0808b1d8:
	push	{r5, r6, r7, lr}
	ldr	r0, [pc, #100]
	movs	r1, #224
	lsls	r1, r1, #1
	adds	r3, r0, r1
	movs	r4, #225
	lsls	r4, r4, #1
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	ldr	r2, [pc, #88]
	adds	r3, r0, r4
	movs	r7, #0
	ldrsh	r5, [r3, r7]
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	movs	r4, #1
	negs	r4, r4
	ldrh	r1, [r2, #0]
	cmp	r3, r4
	beq.n	.L0
	movs	r7, #226
	movs	r3, #227
	lsls	r7, r7, #1
	lsls	r3, r3, #1
	mov	ip, r4
	adds	r4, r0, r7
	adds	r0, r0, r3
.L3:
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	cmp	r3, r6
	bne.n	.L1
	movs	r7, #2
	ldrsh	r3, [r2, r7]
	cmp	r3, ip
	beq.n	.L2
	cmp	r3, r5
	bne.n	.L1
.L2:
	ldrh	r3, [r2, #4]
	strh	r3, [r4, #0]
	ldrh	r3, [r2, #6]
	strh	r3, [r0, #0]
	b.n	.L0
.L1:
	adds	r2, #8
	movs	r7, #0
	ldrsh	r3, [r2, r7]
	ldrh	r1, [r2, #0]
	cmp	r3, ip
	bne.n	.L3
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x0809e1d8
