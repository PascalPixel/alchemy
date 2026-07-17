@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801d108
	.thumb_func
Func_0801d108:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #836]
	ldr	r3, [r3, #0]
	sub	sp, #8
	movs	r5, #2
	movs	r1, #5
	movs	r2, #28
	mov	r9, r3
	movs	r0, #1
	movs	r3, #14
	str	r5, [sp, #0]
	bl	Func_080162d4
	movs	r1, #0
	movs	r2, #2
	movs	r3, #27
	str	r5, [sp, #0]
	mov	fp, r0
	bl	Func_0801e41c
	movs	r3, #4
	str	r3, [sp, #0]
	mov	r0, fp
	movs	r1, #0
	movs	r2, #4
	movs	r3, #27
	bl	Func_0801e41c
	movs	r3, #7
	str	r3, [sp, #0]
	mov	r0, fp
	movs	r1, #0
	movs	r2, #7
	movs	r3, #27
	bl	Func_0801e41c
	movs	r3, #10
	str	r3, [sp, #0]
	mov	r0, fp
	movs	r1, #0
	movs	r2, #10
	movs	r3, #27
	bl	Func_0801e41c
	ldr	r5, [pc, #752]
	mov	r1, fp
	adds	r0, r5, #0
	movs	r2, #8
	movs	r3, #0
	adds	r5, #1
	bl	Func_0801e7c0
	adds	r0, r5, #0
	mov	r1, fp
	movs	r2, #8
	movs	r3, #16
	bl	Func_0801e7c0
	ldr	r5, [pc, #728]
	mov	r1, fp
	adds	r0, r5, #0
	movs	r2, #8
	movs	r3, #32
	adds	r5, #1
	bl	Func_0801e7c0
	adds	r0, r5, #0
	mov	r1, fp
	movs	r2, #32
	movs	r3, #40
	bl	Func_0801e7c0
	ldr	r0, [pc, #704]
	mov	r1, fp
	movs	r2, #8
	movs	r3, #64
	bl	Func_0801e7c0
	ldr	r0, [pc, #696]
	mov	r1, fp
	movs	r2, #8
	movs	r3, #88
	bl	Func_0801e7c0
	bl	Func_08004080
	adds	r6, r0, #0
	cmp	r6, #95
	bgt.n	.L0
	ldr	r2, [pc, #680]
	movs	r1, #128
	bl	Func_08003fa4
	movs	r1, #128
	movs	r3, #0
	lsls	r1, r1, #23
	adds	r0, r6, #0
	mov	r2, fp
	str	r3, [sp, #0]
	bl	Func_0801eadc
	adds	r4, r0, #0
	ldr	r0, [pc, #656]
	add	r0, r9
	str	r4, [r0, #0]
	mov	r1, fp
	ldrh	r3, [r1, #12]
	lsls	r6, r3, #3
	ldrh	r3, [r1, #14]
	lsls	r3, r3, #3
	adds	r7, r3, #0
	adds	r7, #12
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	Func_080b0038
.L0:
	bl	Func_08004080
	adds	r6, r0, #0
	cmp	r6, #95
	bgt.n	.L1
	ldr	r3, [pc, #624]
	ldr	r0, [pc, #624]
	ldr	r1, [pc, #628]
	ldr	r2, [pc, #628]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #1
	ldr	r2, [pc, #624]
	adds	r0, r6, #0
	bl	Func_08003fa4
	ldr	r2, [pc, #620]
	mov	r8, r2
	movs	r7, #0
	mov	r1, r8
	mov	r2, fp
	movs	r3, #134
	adds	r0, r6, #0
	str	r7, [sp, #0]
	bl	Func_0801eadc
	adds	r4, r0, #0
	ldrb	r2, [r4, #25]
	movs	r5, #15
	movs	r1, #224
	mov	sl, r1
	adds	r3, r5, #0
	ands	r3, r2
	mov	r2, sl
	orrs	r3, r2
	strb	r3, [r4, #25]
	mov	r1, r8
	mov	r2, fp
	movs	r3, #166
	adds	r0, r6, #0
	str	r7, [sp, #0]
	bl	Func_0801eadc
	adds	r4, r0, #0
	ldrh	r1, [r4, #24]
	lsls	r2, r1, #22
	ldr	r3, [pc, #564]
	lsrs	r2, r2, #22
	adds	r2, #4
	ands	r2, r3
	ldr	r3, [pc, #560]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r4, #24]
	ldrb	r3, [r4, #25]
	mov	r1, sl
	ands	r5, r3
	orrs	r5, r1
	strb	r5, [r4, #25]
	movs	r7, #16
	mov	r1, r8
	mov	r2, fp
	movs	r3, #134
	adds	r0, r6, #0
	str	r7, [sp, #0]
	bl	Func_0801eadc
	adds	r4, r0, #0
	ldrb	r3, [r4, #25]
	movs	r5, #240
	orrs	r3, r5
	strb	r3, [r4, #25]
	mov	r1, r8
	mov	r2, fp
	movs	r3, #166
	adds	r0, r6, #0
	str	r7, [sp, #0]
	bl	Func_0801eadc
	adds	r4, r0, #0
	ldrh	r2, [r4, #24]
	lsls	r3, r2, #22
	ldr	r1, [pc, #492]
	lsrs	r3, r3, #22
	adds	r3, #4
	ands	r3, r1
	ldr	r1, [pc, #488]
	ands	r1, r2
	orrs	r1, r3
	str	r1, [sp, #4]
	add	r2, sp, #4
	ldrh	r2, [r2, #0]
	strh	r2, [r4, #24]
	ldrb	r3, [r4, #25]
	orrs	r3, r5
	strb	r3, [r4, #25]
.L1:
	bl	Func_08004080
	adds	r6, r0, #0
	cmp	r6, #95
	bgt.n	.L2
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	bl	Func_08003fa4
	movs	r1, #128
	movs	r3, #0
	lsls	r1, r1, #23
	adds	r0, r6, #0
	mov	r2, fp
	str	r3, [sp, #0]
	bl	Func_0801eadc
	adds	r4, r0, #0
	ldrb	r3, [r4, #21]
	ldr	r5, [pc, #432]
	movs	r2, #32
	orrs	r3, r2
	add	r5, r9
	strb	r3, [r4, #21]
	str	r4, [r5, #0]
	mov	r1, fp
	ldrh	r3, [r1, #12]
	lsls	r3, r3, #3
	adds	r6, r3, #0
	ldr	r3, [pc, #416]
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	lsls	r0, r3, #4
	subs	r0, r0, r3
	ldr	r3, [pc, #404]
	add	r3, r9
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	lsls	r0, r0, #2
	bl	Func_080022ec
	mov	r2, fp
	ldrh	r3, [r2, #14]
	adds	r6, #140
	lsls	r3, r3, #3
	adds	r6, r6, r0
	adds	r7, r3, #4
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	Func_080b0038
.L2:
	bl	Func_08004080
	adds	r6, r0, #0
	cmp	r6, #95
	bgt.n	.L3
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	bl	Func_08003fa4
	movs	r1, #128
	movs	r3, #0
	lsls	r1, r1, #23
	adds	r0, r6, #0
	mov	r2, fp
	str	r3, [sp, #0]
	bl	Func_0801eadc
	adds	r4, r0, #0
	ldrb	r3, [r4, #21]
	ldr	r5, [pc, #332]
	movs	r2, #32
	orrs	r3, r2
	add	r5, r9
	strb	r3, [r4, #21]
	str	r4, [r5, #0]
	mov	r1, fp
	ldrh	r3, [r1, #12]
	lsls	r3, r3, #3
	adds	r6, r3, #0
	ldr	r3, [pc, #316]
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	lsls	r0, r3, #4
	subs	r0, r0, r3
	ldr	r3, [pc, #308]
	add	r3, r9
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	lsls	r0, r0, #2
	bl	Func_080022ec
	mov	r2, fp
	ldrh	r3, [r2, #14]
	lsls	r3, r3, #3
	adds	r6, #140
	adds	r7, r3, #0
	adds	r6, r6, r0
	adds	r7, #20
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	Func_080b0038
.L3:
	ldr	r5, [pc, #272]
	movs	r7, #28
	movs	r0, #0
	ldrsb	r0, [r5, r0]
	mov	r2, fp
	movs	r1, #0
	movs	r3, #84
	str	r7, [sp, #0]
	bl	Func_08021750
	ldr	r3, [pc, #256]
	add	r3, r9
	str	r0, [r3, #0]
	mov	r2, fp
	movs	r0, #1
	ldrsb	r0, [r5, r0]
	movs	r1, #0
	movs	r3, #108
	str	r7, [sp, #0]
	bl	Func_08021750
	movs	r3, #190
	lsls	r3, r3, #3
	add	r3, r9
	str	r0, [r3, #0]
	mov	r2, fp
	movs	r0, #2
	ldrsb	r0, [r5, r0]
	movs	r1, #0
	movs	r3, #132
	str	r7, [sp, #0]
	bl	Func_08021750
	ldr	r3, [pc, #216]
	ldr	r5, [pc, #216]
	add	r3, r9
	str	r0, [r3, #0]
	movs	r7, #52
	movs	r0, #0
	ldrsb	r0, [r5, r0]
	mov	r2, fp
	movs	r1, #0
	movs	r3, #100
	str	r7, [sp, #0]
	bl	Func_08021750
	movs	r3, #191
	lsls	r3, r3, #3
	add	r3, r9
	str	r0, [r3, #0]
	mov	r2, fp
	movs	r0, #1
	ldrsb	r0, [r5, r0]
	movs	r1, #0
	movs	r3, #124
	str	r7, [sp, #0]
	bl	Func_08021750
	ldr	r3, [pc, #172]
	ldr	r5, [pc, #176]
	add	r3, r9
	str	r0, [r3, #0]
	movs	r7, #76
	movs	r0, #0
	ldrsb	r0, [r5, r0]
	mov	r2, fp
	movs	r1, #0
	movs	r3, #100
	str	r7, [sp, #0]
	bl	Func_08021750
	ldr	r3, [pc, #156]
	add	r3, r9
	str	r0, [r3, #0]
	movs	r1, #0
	movs	r0, #1
	ldrsb	r0, [r5, r0]
	mov	r2, fp
	movs	r3, #124
	str	r7, [sp, #0]
	bl	Func_08021750
	movs	r3, #193
	lsls	r3, r3, #3
	add	r3, r9
	str	r0, [r3, #0]
	add	sp, #8
	mov	r0, fp
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001ea0
	.4byte 0x00000c07
	.4byte 0x00000c0d
	.4byte 0x00000c0f
	.4byte 0x00000c12
	.4byte 0x080310a4
	.4byte 0x000005a4
	.4byte 0x040000d4
	.4byte 0x08073812
	.4byte 0x050003c0
	.4byte 0x80000020
	.4byte 0x08029910
	.4byte 0x40004000
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000005b4
	.4byte 0x00000594
	.4byte 0x00000599
	.4byte 0x000005c4
	.4byte 0x00000595
	.4byte 0x0000059a
	.4byte 0x080367c9
	.4byte 0x000005ec
	.4byte 0x000005f4
	.4byte 0x080367cc
	.4byte 0x000005fc
	.4byte 0x080367ce
	.4byte 0x00000604
