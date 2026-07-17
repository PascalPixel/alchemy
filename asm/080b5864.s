@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b5864
	.thumb_func
Func_080b5864:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #228]
	adds	r2, r3, #0
	ldr	r6, [r3, #0]
	subs	r3, #12
	ldr	r1, [r3, #0]
	adds	r3, r1, #0
	adds	r3, #68
	ldrb	r3, [r3, #0]
	adds	r2, #128
	sub	sp, #16
	ldr	r7, [r2, #0]
	cmp	r3, #0
	beq.n	.L0
	ldr	r3, [pc, #204]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L1
	adds	r2, r1, #0
	adds	r2, #81
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	movs	r2, #192
	lsls	r3, r3, #24
	lsls	r2, r2, #21
	cmp	r3, r2
	bls.n	.L0
	adds	r2, r1, #0
	adds	r2, #82
	movs	r3, #1
	b.n	.L2
.L1:
	ldr	r3, [pc, #168]
	adds	r2, r1, #0
	ldr	r3, [r3, #0]
	adds	r2, #80
	lsls	r3, r3, #26
	ldrb	r2, [r2, #0]
	lsrs	r3, r3, #30
	cmp	r2, r3
	beq.n	.L3
	adds	r2, r1, #0
	adds	r2, #82
	movs	r3, #1
	strb	r3, [r2, #0]
.L3:
	adds	r2, r1, #0
	adds	r2, #81
	movs	r3, #0
.L2:
	strb	r3, [r2, #0]
.L0:
	ldr	r3, [r7, #4]
	cmp	r3, #0
	beq.n	.L4
	ldr	r3, [r7, #0]
	ldrh	r1, [r6, #54]
	subs	r3, r3, r1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L5
	adds	r2, #15
.L5:
	asrs	r3, r2, #4
	adds	r3, r1, r3
	strh	r3, [r6, #54]
	ldr	r3, [r7, #4]
	subs	r3, #1
	str	r3, [r7, #4]
.L4:
	ldr	r3, [r6, #28]
	adds	r5, r6, #0
	adds	r5, #12
	cmp	r3, #0
	beq.n	.L6
	adds	r5, r3, #0
.L6:
	bl	Func_080049ac
	adds	r0, r5, #0
	bl	Func_08004cb4
	movs	r3, #54
	ldrsh	r0, [r6, r3]
	bl	Func_08004c1c
	movs	r2, #52
	ldrsh	r0, [r6, r2]
	bl	Func_08004bd4
	movs	r3, #0
	add	r0, sp, #4
	str	r3, [r0, #0]
	str	r3, [r0, #4]
	ldr	r3, [r6, #32]
	adds	r1, r6, #0
	str	r3, [r0, #8]
	ldr	r3, [pc, #52]
	bl	Func_080072f0
	ldr	r3, [r7, #20]
	cmp	r3, #0
	bne.n	.L7
	movs	r1, #240
	movs	r3, #128
	lsls	r3, r3, #9
	lsls	r1, r1, #15
	str	r3, [sp, #0]
	adds	r0, r1, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_080c0a24
.L7:
	add	sp, #16
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e80
	.4byte 0x03001f64
	.4byte 0x04000128
	.4byte 0x03000250
