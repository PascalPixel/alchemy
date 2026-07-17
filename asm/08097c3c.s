@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08097c3c
	.thumb_func
Func_08097c3c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #772]
	ldr	r3, [r3, #0]
	sub	sp, #52
	str	r3, [sp, #24]
	ldr	r0, [r3, #16]
	str	r0, [sp, #20]
	movs	r7, #128
	ldr	r6, [r3, #20]
	ldr	r3, [r3, #0]
	lsls	r7, r7, #8
	adds	r3, r3, r7
	movs	r1, #0
	str	r3, [sp, #8]
	str	r1, [sp, #4]
	cmp	r6, #0
	bne.n	.L0
	b.n	.L1
.L0:
	bl	Func_08097384
	ldr	r2, [sp, #20]
	str	r6, [r2, #104]
	ldr	r0, [sp, #20]
	ldr	r1, [pc, #732]
	bl	Func_08009098
	ldr	r0, [sp, #20]
	bl	Func_08098070
	mov	sl, r0
	cmp	r0, #0
	bne.n	.L2
	bl	Func_0809748c
	b.n	.L1
.L2:
	mov	r3, sl
	str	r6, [r3, #104]
	movs	r0, #40
	ldr	r3, [r6, #8]
	add	r0, sp
	str	r3, [r0, #0]
	movs	r5, #128
	ldr	r3, [r6, #12]
	lsls	r5, r5, #13
	adds	r3, r3, r5
	str	r3, [r0, #4]
	ldr	r3, [r6, #16]
	mov	r9, r0
	str	r3, [r0, #8]
	ldr	r1, [sp, #8]
	adds	r0, r5, #0
	mov	r2, r9
	bl	Func_0800447c
	mov	r2, r9
	mov	r0, r9
	ldr	r1, [r2, #0]
	ldr	r3, [r0, #8]
	ldr	r2, [r2, #4]
	mov	r0, sl
	bl	Func_08009150
	mov	r0, sl
	bl	Func_08098184
	movs	r3, #128
	mov	r1, sl
	lsls	r3, r3, #11
	str	r3, [r1, #48]
	str	r7, [r1, #52]
	movs	r3, #4
	adds	r1, #85
	str	r1, [sp, #0]
	strb	r3, [r1, #0]
	ldr	r3, [pc, #632]
	str	r3, [r6, #108]
	ldr	r3, [pc, #632]
	str	r3, [r6, #48]
	ldr	r3, [pc, #632]
	add	r2, sp, #4
	str	r3, [r6, #52]
	ldrb	r2, [r2, #0]
	adds	r3, r6, #0
	adds	r3, #90
	strb	r2, [r3, #0]
	adds	r2, r6, #0
	adds	r2, #34
	movs	r3, #2
	mov	r7, r9
	mov	fp, r5
	strb	r3, [r2, #0]
	b.n	.L3
.L11:
	ldr	r3, [pc, #608]
	ldr	r0, [r3, #0]
	bl	Func_08097b54
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	ldr	r3, [pc, #600]
	mov	r8, r0
	cmp	r8, r3
	bne.n	.L4
	ldr	r3, [r6, #8]
	str	r3, [r7, #0]
	ldr	r3, [r6, #12]
	add	r3, fp
	str	r3, [r7, #4]
	ldr	r3, [r6, #16]
	str	r3, [r7, #8]
	ldr	r1, [sp, #8]
	mov	r0, fp
	adds	r2, r7, #0
	bl	Func_0800447c
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	mov	r0, sl
	bl	Func_08009150
	mov	r0, sl
	movs	r1, #1
	bl	Func_08009080
	mov	r0, sl
	str	r5, [r0, #36]
	str	r5, [r0, #40]
	str	r5, [r0, #44]
	b.n	.L3
.L4:
	ldr	r3, [r6, #8]
	str	r3, [r7, #0]
	ldr	r3, [r6, #12]
	add	r3, fp
	str	r3, [r7, #4]
	ldr	r3, [r6, #16]
	str	r3, [r7, #8]
	ldr	r1, [sp, #8]
	mov	r0, fp
	adds	r2, r7, #0
	bl	Func_0800447c
	movs	r0, #128
	lsls	r0, r0, #10
	mov	r1, r8
	adds	r2, r7, #0
	bl	Func_0800447c
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	mov	r0, sl
	bl	Func_08009150
	mov	r0, sl
	bl	Func_08009158
	ldr	r3, [r6, #8]
	str	r3, [r7, #0]
	ldr	r3, [r6, #12]
	str	r3, [r7, #4]
	ldr	r3, [r6, #16]
	mov	r0, fp
	str	r3, [r7, #8]
	mov	r1, r8
	adds	r2, r7, #0
	bl	Func_0800447c
	ldr	r3, [r6, #8]
	add	r5, sp, #28
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	movs	r0, #128
	lsls	r0, r0, #14
	mov	r1, r8
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	Func_0800447c
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	Func_080091d8
	cmp	r0, #0
	bgt.n	.L5
	adds	r0, r6, #0
	mov	r1, r9
	bl	Func_080092a0
	cmp	r0, #0
	beq.n	.L6
	ldr	r1, [sp, #20]
	cmp	r0, r1
	bne.n	.L5
	ldr	r2, [sp, #20]
	ldr	r3, [sp, #20]
	mov	r1, r9
	ldr	r0, [r2, #8]
	ldr	r4, [r3, #16]
	ldr	r2, [pc, #400]
	ldr	r3, [r1, #0]
	ands	r0, r2
	ands	r3, r2
	ands	r4, r2
	cmp	r0, r3
	bne.n	.L7
	ldr	r3, [r1, #8]
	ands	r3, r2
	cmp	r4, r3
	beq.n	.L5
.L7:
	ldr	r1, [r5, #0]
	ldr	r2, [pc, #376]
	adds	r3, r1, #0
	ands	r3, r2
	mov	ip, r2
	cmp	r0, r3
	bne.n	.L6
	ldr	r2, [r5, #8]
	mov	r0, ip
	adds	r3, r2, #0
	ands	r3, r0
	cmp	r4, r3
	bne.n	.L6
	ldr	r3, [sp, #20]
	adds	r3, #34
	ldrb	r0, [r3, #0]
	bl	Func_080092a8
	cmp	r0, #0
	beq.n	.L8
.L5:
	mov	r0, sl
	movs	r1, #4
	bl	Func_08009080
	ldr	r3, [pc, #336]
	ldr	r3, [r3, #0]
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L3
	movs	r0, #114
	bl	Func_080f9010
	b.n	.L3
.L8:
	movs	r1, #1
	str	r1, [sp, #4]
.L6:
	movs	r0, #175
	bl	Func_080f9010
	ldr	r2, [r7, #0]
	str	r2, [sp, #16]
	ldr	r0, [sp, #8]
	ldr	r3, [r7, #8]
	mov	r1, r8
	str	r3, [sp, #12]
	subs	r3, r0, r1
	ldr	r2, [pc, #296]
	lsls	r3, r3, #16
	lsrs	r3, r3, #30
	ldrb	r1, [r2, r3]
	mov	r0, sl
	bl	Func_08009080
	movs	r0, #15
	bl	Func_080030f8
	adds	r3, r6, #0
	adds	r3, #91
	movs	r2, #0
	strb	r2, [r3, #0]
	ldr	r3, [pc, #248]
	str	r3, [r6, #48]
	str	r3, [r6, #52]
	mov	r9, r3
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	adds	r0, r6, #0
	bl	Func_08009150
	ldr	r1, [sp, #0]
	mov	r3, sl
	mov	r2, r9
	movs	r0, #0
	strb	r0, [r1, #0]
	str	r2, [r3, #48]
	str	r2, [r3, #52]
	mov	r0, fp
	mov	r1, r8
	adds	r2, r7, #0
	bl	Func_0800447c
	ldr	r2, [r7, #4]
	mov	r0, sl
	ldr	r1, [r7, #0]
	add	r2, fp
	ldr	r3, [r7, #8]
	bl	Func_08009150
	ldr	r0, [sp, #4]
	cmp	r0, #1
	bne.n	.L9
	ldr	r2, [sp, #24]
	movs	r1, #24
	ldrsh	r0, [r2, r1]
	bl	Func_08092054
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	ldr	r0, [sp, #20]
	mov	r3, r9
	str	r3, [r0, #48]
	str	r3, [r0, #52]
	ldr	r0, [sp, #20]
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	bl	Func_08009150
.L9:
	adds	r0, r6, #0
	bl	Func_08009158
	ldr	r1, [sp, #16]
	str	r1, [r6, #8]
	ldr	r2, [sp, #12]
	movs	r3, #0
	str	r2, [r6, #16]
	str	r3, [r6, #36]
	str	r3, [r6, #44]
	b.n	.L10
.L3:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [pc, #140]
	ldr	r5, [r3, #0]
	ldr	r3, [pc, #140]
	ands	r5, r3
	cmp	r5, #0
	bne.n	.L10
	b.n	.L11
.L10:
	ldr	r3, [sp, #24]
	adds	r3, #68
	ldrb	r1, [r3, #0]
	adds	r0, r6, #0
	bl	Func_08009240
	ldr	r0, [sp, #24]
	ldr	r1, [r0, #60]
	adds	r0, r6, #0
	bl	Func_08009098
	ldr	r1, [sp, #24]
	ldr	r3, [r1, #56]
	str	r3, [r6, #108]
	bl	Func_08097174
	ldr	r2, [sp, #4]
	cmp	r2, #1
	bne.n	.L12
	ldr	r1, [sp, #24]
	movs	r3, #24
	ldrsh	r0, [r1, r3]
	bl	Func_08092054
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
.L12:
	bl	Func_0809748c
	mov	r0, sl
	bl	Func_080981b0
.L1:
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f30
	.4byte 0x0809f0bc
	.4byte 0x08096b89
	.4byte 0x00006666
	.4byte 0x00003333
	.4byte 0x03001ae8
	.4byte 0x0000ffff
	.4byte 0xfff00000
	.4byte 0x03001e40
	.4byte 0x0809f118
	.4byte 0x03001c94
	.4byte 0x00000303
