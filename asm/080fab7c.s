@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080fab7c
	.thumb_func
Func_080fab7c:
	push	{r4, r5, r6, r7, lr}
	adds	r6, r0, #0
	ldrh	r1, [r6, #36]
	cmp	r1, #0
	beq.n	.L0
	ldrh	r0, [r6, #38]
	subs	r0, #1
	strh	r0, [r6, #38]
	ldr	r3, [pc, #44]
	adds	r2, r3, #0
	lsls	r0, r0, #16
	lsrs	r3, r0, #16
	cmp	r3, #0
	bne.n	.L0
	strh	r1, [r6, #38]
	ldrh	r1, [r6, #40]
	movs	r0, #2
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L1
	adds	r0, r1, #0
	adds	r0, #16
	strh	r0, [r6, #40]
	ands	r0, r2
	cmp	r0, #255
	bls.n	.L2
	movs	r0, #128
	lsls	r0, r0, #1
	strh	r0, [r6, #40]
	strh	r3, [r6, #36]
	b.n	.L2
	movs	r0, r0
	.4byte 0x0000ffff
.L1:
	adds	r0, r1, #0
	subs	r0, #16
	strh	r0, [r6, #40]
	ands	r0, r2
	lsls	r0, r0, #16
	cmp	r0, #0
	bgt.n	.L2
	ldrb	r5, [r6, #8]
	ldr	r4, [r6, #44]
	cmp	r5, #0
	ble.n	.L3
.L5:
	adds	r0, r6, #0
	adds	r1, r4, #0
	bl	Func_080f9ef8
	movs	r0, #1
	ldrh	r7, [r6, #40]
	ands	r0, r7
	cmp	r0, #0
	bne.n	.L4
	strb	r0, [r4, #0]
.L4:
	subs	r5, #1
	adds	r4, #80
	cmp	r5, #0
	bgt.n	.L5
.L3:
	movs	r0, #1
	ldrh	r1, [r6, #40]
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L6
	ldr	r0, [r6, #4]
	movs	r1, #128
	lsls	r1, r1, #24
	orrs	r0, r1
	b.n	.L7
.L6:
	movs	r0, #128
	lsls	r0, r0, #24
.L7:
	str	r0, [r6, #4]
	movs	r0, #0
	strh	r0, [r6, #36]
	b.n	.L0
.L2:
	ldrb	r5, [r6, #8]
	ldr	r4, [r6, #44]
	cmp	r5, #0
	ble.n	.L0
	movs	r3, #128
	movs	r7, #0
	movs	r2, #3
.L9:
	ldrb	r1, [r4, #0]
	adds	r0, r3, #0
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L8
	ldrh	r7, [r6, #40]
	lsrs	r0, r7, #2
	strb	r0, [r4, #19]
	adds	r0, r1, #0
	orrs	r0, r2
	strb	r0, [r4, #0]
.L8:
	subs	r5, #1
	adds	r4, #80
	cmp	r5, #0
	bgt.n	.L9
.L0:
	pop	{r4, r5, r6, r7}
	pop	{r0}
	bx	r0
