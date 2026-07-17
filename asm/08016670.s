@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08016670
	.thumb_func
Func_08016670:
	push	{r5, r6, lr}
	ldr	r3, [pc, #164]
	movs	r4, #196
	ldr	r3, [r3, #0]
	lsls	r4, r4, #3
	adds	r6, r1, #0
	movs	r5, #0
	adds	r1, r3, r4
	movs	r4, #0
	b.n	.L0
.L3:
	adds	r1, #40
	adds	r4, #1
.L0:
	cmp	r4, #3
	beq.n	.L1
	ldr	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L2
	ldrh	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L3
.L2:
	adds	r5, r1, #0
.L1:
	cmp	r5, #0
	beq.n	.L4
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L5
	movs	r3, #160
	lsls	r3, r3, #4
	strh	r3, [r5, #6]
	movs	r3, #192
	lsls	r3, r3, #2
	str	r0, [r5, #0]
	b.n	.L6
.L5:
	cmp	r2, #0
	bne.n	.L7
	ldrh	r3, [r5, #6]
	adds	r2, r3, #0
	cmp	r2, #0
	bne.n	.L8
	movs	r3, #160
	lsls	r3, r3, #4
	strh	r3, [r5, #6]
	b.n	.L9
.L8:
	movs	r1, #208
	lsls	r1, r1, #4
	cmp	r2, r1
	bcs.n	.L10
	adds	r3, r3, r1
	strh	r3, [r5, #6]
	b.n	.L9
.L10:
	adds	r0, r5, #0
	bl	Func_080167d8
.L9:
	movs	r3, #192
	lsls	r3, r3, #2
.L6:
	strh	r3, [r5, #4]
.L7:
	movs	r3, #192
	lsls	r3, r3, #2
	strh	r3, [r5, #30]
	ldr	r3, [r5, #0]
	movs	r2, #0
	strh	r2, [r3, #20]
	movs	r3, #15
	strh	r3, [r5, #22]
	movs	r3, #10
	strh	r3, [r5, #26]
	strh	r6, [r5, #18]
	adds	r3, r5, #0
	strh	r2, [r5, #20]
	strh	r2, [r5, #24]
	strh	r2, [r5, #16]
	strh	r2, [r5, #32]
	movs	r4, #0
	adds	r3, #8
.L11:
	adds	r4, #1
	strh	r2, [r3, #0]
	adds	r3, #2
	cmp	r4, #3
	bls.n	.L11
.L4:
	adds	r0, r5, #0
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e8c
