@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08097644
	.thumb_func
Func_08097644:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #24]
	movs	r0, #165
	ldr	r7, [r3, #0]
	lsls	r0, r0, #2
	adds	r1, r7, r0
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	sub	sp, #24
	cmp	r3, #0
	beq.n	.L0
	adds	r3, #255
	strb	r3, [r1, #0]
	b.n	.L1
	.4byte 0x03001ea8
.L0:
	ldr	r1, [pc, #420]
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	movs	r2, #1
	eors	r2, r3
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #4
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r6, r7, r3
	movs	r5, #0
.L2:
	movs	r2, #162
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldrh	r0, [r3, #0]
	lsls	r3, r5, #3
	adds	r0, r0, r3
	movs	r1, #160
	lsls	r0, r0, #16
	bl	Func_080022f4
	bl	Func_08002322
	adds	r5, #1
	asrs	r0, r0, #14
	strh	r0, [r6, #0]
	adds	r6, #2
	cmp	r5, #159
	bls.n	.L2
	movs	r3, #162
	lsls	r3, r3, #2
	adds	r2, r7, r3
	ldrh	r3, [r2, #0]
	ldr	r4, [pc, #352]
	adds	r3, #4
	strh	r3, [r2, #0]
	adds	r1, r7, r4
	ldrb	r3, [r1, #0]
	movs	r2, #1
	eors	r3, r2
	strb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L3
	ldr	r0, [pc, #336]
	movs	r1, #163
	adds	r3, r7, r0
	lsls	r1, r1, #2
	movs	r0, #0
	ldrsb	r0, [r3, r0]
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r2, [pc, #320]
	lsls	r3, r3, #5
	lsls	r0, r0, #10
	orrs	r0, r3
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	orrs	r0, r3
	movs	r3, #128
	lsls	r3, r3, #14
	orrs	r0, r3
	movs	r1, #1
	bl	Func_08091200
	movs	r0, #1
	bl	Func_08091254
	bl	Func_080978c4
.L3:
	movs	r4, #164
	lsls	r4, r4, #2
	adds	r3, r7, r4
	ldrh	r0, [r3, #0]
	bl	Func_08092054
	movs	r1, #0
	bl	Func_0808e0b0
	ldr	r0, [pc, #268]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L4
	cmp	r3, #8
	beq.n	.L4
	cmp	r3, #16
	bne.n	.L5
.L4:
	movs	r1, #164
	lsls	r1, r1, #2
	adds	r5, r7, r1
	ldrh	r0, [r5, #0]
	bl	Func_08092054
	ldr	r2, [pc, #240]
	adds	r2, r2, r7
	adds	r6, r0, #0
	ldrh	r0, [r2, #0]
	mov	fp, r2
	bl	Func_08092054
	mov	r9, r0
	cmp	r6, #0
	beq.n	.L5
	cmp	r0, #0
	beq.n	.L5
	add	r3, sp, #12
	mov	sl, r3
	ldr	r3, [r6, #8]
	mov	r4, sl
	str	r3, [r4, #0]
	ldrh	r0, [r5, #0]
	bl	Func_0808d394
	movs	r1, #0
	ldrsh	r0, [r0, r1]
	bl	Func_08185000
	movs	r2, #8
	ldrsb	r2, [r0, r2]
	ldr	r3, [r6, #12]
	lsls	r2, r2, #16
	ldr	r5, [pc, #188]
	adds	r3, r3, r2
	adds	r3, r3, r5
	mov	r2, sl
	str	r3, [r2, #4]
	ldr	r3, [r6, #16]
	str	r3, [r2, #8]
	mov	r4, r9
	ldr	r3, [r4, #8]
	mov	r1, fp
	ldrh	r0, [r1, #0]
	mov	r8, sp
	str	r3, [sp, #0]
	bl	Func_0808d394
	movs	r2, #0
	ldrsh	r0, [r0, r2]
	bl	Func_08185000
	mov	r4, r9
	movs	r3, #8
	ldrsb	r3, [r0, r3]
	ldr	r2, [r4, #12]
	lsls	r3, r3, #16
	adds	r2, r2, r3
	mov	r0, r8
	adds	r2, r2, r5
	str	r2, [r0, #4]
	ldr	r3, [r4, #16]
	str	r3, [r0, #8]
	mov	r4, r8
	ldr	r0, [pc, #132]
	ldr	r1, [r4, #0]
	bl	Func_080090c8
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L5
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	ldr	r5, [r6, #80]
	strb	r3, [r2, #0]
	ldr	r3, [pc, #112]
	str	r3, [r6, #48]
	str	r3, [r6, #52]
	mov	r2, r8
	mov	r1, sl
	ldr	r3, [r2, #8]
	ldr	r0, [r1, #8]
	ldr	r1, [r1, #0]
	subs	r0, r0, r3
	ldr	r3, [r2, #0]
	subs	r1, r1, r3
	bl	Func_080044d0
	ldr	r3, [pc, #88]
	ldr	r2, [pc, #52]
	str	r3, [r6, #108]
	adds	r3, r5, #0
	adds	r3, #38
	strb	r2, [r3, #0]
	movs	r3, #13
	ldrb	r2, [r5, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strh	r0, [r6, #6]
	strb	r3, [r5, #9]
	mov	r3, sl
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #4]
	adds	r0, r6, #0
	ldr	r3, [r3, #8]
	bl	Func_08009150
.L5:
	ldr	r4, [pc, #28]
	adds	r5, r7, r4
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L6
	b.n	.L7
	.4byte 0x00000000
	.4byte 0x0000028a
	.4byte 0x0000028d
	.4byte 0x0000028b
	.4byte 0x00000295
	.4byte 0x00000292
	.4byte 0xfffe0000
	.4byte 0x00000119
	.4byte 0x0000a3d7
	.4byte 0x08097a55
.L7:
	movs	r0, #130
	bl	Func_080f9010
	ldrb	r2, [r5, #0]
.L6:
	adds	r3, r2, #1
	movs	r0, #240
	strb	r3, [r5, #0]
	lsls	r0, r0, #22
	lsls	r3, r3, #24
	cmp	r3, r0
	bls.n	.L1
	movs	r3, #0
	strb	r3, [r5, #0]
.L1:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
