@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080903bc
	.thumb_func
Func_080903bc:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #168]
	ldr	r1, [pc, #168]
	ldr	r6, [r3, #0]
	adds	r4, r6, r1
	movs	r2, #0
	ldrsb	r2, [r4, r2]
	subs	r3, #92
	ldr	r7, [r3, #0]
	cmp	r2, #0
	beq.n	.L0
	ldr	r3, [pc, #156]
	adds	r1, r6, r3
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	ldrb	r0, [r1, #0]
	cmp	r3, r2
	blt.n	.L1
	movs	r3, #0
	strb	r3, [r4, #0]
	ldr	r0, [pc, #140]
	bl	Func_08004278
	movs	r0, #1
	movs	r1, #0
	movs	r2, #0
	bl	Func_0800307c
	b.n	.L2
.L1:
	ldr	r2, [pc, #128]
	adds	r3, r6, r2
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	ldr	r3, [pc, #124]
	adds	r5, r6, r3
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	subs	r2, r2, r3
	adds	r3, r0, #1
	strb	r3, [r1, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r0, r3, #0
	muls	r0, r2
	movs	r1, #0
	ldrsb	r1, [r4, r1]
	ldr	r3, [pc, #100]
	bl	Func_080072f0
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	ldr	r1, [pc, #96]
	adds	r3, r3, r0
	adds	r2, r6, r1
	strh	r3, [r2, #0]
.L0:
	ldr	r2, [pc, #88]
	adds	r3, r6, r2
	ldrh	r2, [r3, #0]
	cmp	r2, #79
	bls.n	.L3
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r3, r7, r1
	movs	r2, #200
	strh	r2, [r3, #0]
	movs	r3, #129
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #250
	strh	r3, [r2, #0]
	b.n	.L2
.L3:
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r3, r7, r1
	strh	r2, [r3, #0]
	ldr	r3, [pc, #16]
	subs	r3, r3, r2
	movs	r2, #129
	lsls	r2, r2, #1
	adds	r1, r7, r2
	strh	r3, [r1, #0]
.L2:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0000009f
	.4byte 0x03001ecc
	.4byte 0x0000053c
	.4byte 0x0000053d
	.4byte 0x080903bd
	.4byte 0x0000053b
	.4byte 0x0000053a
	.4byte 0x03000380
	.4byte 0x0000052a
