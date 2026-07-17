@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b9ec0
	.thumb_func
Func_080b9ec0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #124
	add	r7, sp, #12
	str	r1, [sp, #8]
	adds	r1, r7, #0
	mov	r9, r0
	ldr	r5, [pc, #56]
	bl	Func_080b9d34
	mov	r0, r9
	ldrb	r0, [r0, #0]
	str	r0, [sp, #4]
	mov	r1, r9
	ldrb	r1, [r1, #2]
	str	r1, [sp, #0]
	mov	r2, r9
	ldr	r3, [r2, #88]
	movs	r2, #128
	lsls	r2, r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	adds	r3, r5, #0
	adds	r3, #140
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #6
	cmp	r0, #7
	bls.n	.L1
	movs	r3, #160
	lsls	r3, r3, #7
.L1:
	str	r3, [r2, #0]
	movs	r3, #60
	str	r3, [r2, #4]
	b.n	.L2
	movs	r0, r0
	.4byte 0x03001e74
.L0:
	adds	r3, r5, #0
	adds	r3, #140
	ldr	r1, [r3, #0]
	ldr	r3, [sp, #4]
	ldr	r2, [pc, #84]
	cmp	r3, #7
	bhi.n	.L3
	movs	r2, #128
	lsls	r2, r2, #6
.L3:
	ldr	r3, [r1, #0]
	cmp	r3, r2
	beq.n	.L2
	str	r2, [r1, #0]
.L2:
	movs	r1, #0
	movs	r0, #0
	bl	Func_080c10e8
	ldr	r3, [pc, #64]
	ldr	r3, [r3, #0]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	movs	r3, #2
	negs	r3, r3
	ands	r0, r3
	bl	Func_08015130
	ldr	r0, [sp, #4]
	bl	Func_080b7dd0
	ldr	r0, [r0, #0]
	mov	fp, r0
	add	r0, sp, #96
	ldr	r2, [pc, #36]
	ldr	r3, [pc, #24]
	mov	sl, r0
	strh	r3, [r2, #0]
	movs	r0, #3
	mov	r1, sl
	bl	Func_080b6c08
	movs	r6, #0
	mov	r8, r0
	cmp	r0, #0
	beq.n	.L4
	movs	r5, #0
	b.n	.L5
	.4byte 0x00003f40
	.4byte 0xffffe000
	.4byte 0x03001e74
	.4byte 0x04000050
.L5:
	mov	r1, sl
	ldrh	r3, [r5, r1]
	cmp	r3, #254
	beq.n	.L6
	ldr	r2, [sp, #4]
	adds	r0, r3, #0
	cmp	r0, r2
	beq.n	.L7
	ldr	r3, [sp, #0]
	movs	r2, #0
	cmp	r3, #7
	bhi.n	.L8
	movs	r2, #1
.L8:
	movs	r3, #0
	cmp	r0, #7
	bhi.n	.L9
	movs	r3, #1
.L9:
	cmp	r2, r3
	beq.n	.L6
	movs	r1, #1
	bl	Func_080c0f98
	b.n	.L6
.L7:
	mov	r0, fp
	movs	r1, #3
	bl	Func_08009080
.L6:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r8
	bne.n	.L5
.L4:
	movs	r0, #154
	bl	Func_080f9010
	mov	r2, r9
	ldr	r1, [r2, #80]
	movs	r3, #0
	ldr	r0, [r7, #8]
	movs	r2, #0
	bl	Func_080c1798
	ldr	r0, [sp, #8]
	movs	r3, #1
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L10
	ldr	r0, [sp, #4]
	movs	r1, #1
	bl	Func_080c0f98
.L10:
	ldr	r1, [pc, #40]
	ldr	r5, [pc, #44]
	movs	r6, #0
	mov	fp, r1
.L11:
	mov	r2, fp
	subs	r3, r2, r6
	ldr	r0, [pc, #36]
	orrs	r3, r5
	strh	r3, [r0, #0]
	adds	r6, #1
	movs	r0, #1
	bl	Func_080030f8
	cmp	r6, #16
	bne.n	.L11
	mov	r1, r9
	ldr	r3, [r1, #92]
	cmp	r3, #0
	beq.n	.L12
	cmp	r3, #1
	bne.n	.L13
	b.n	.L14
	.4byte 0x00000010
	.4byte 0x00001000
	.4byte 0x04000052
.L14:
	ldrb	r1, [r1, #0]
	movs	r0, #0
	bl	Func_080bbabc
	ldr	r1, [pc, #72]
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L15
.L13:
	ldr	r1, [pc, #68]
	movs	r0, #4
	bl	Func_080bbabc
.L15:
	bl	Func_080bb938
	bl	Func_080c1a14
	b.n	.L16
.L12:
	movs	r6, #0
	movs	r2, #0
	cmp	r6, r8
	bcs.n	.L17
	ldr	r0, [sp, #8]
	movs	r3, #1
	ands	r0, r3
	mov	ip, r0
	mov	r5, sl
	mov	r1, sl
.L23:
	ldrh	r3, [r5, #0]
	ldr	r0, [sp, #4]
	adds	r5, #2
	cmp	r3, r0
	bne.n	.L18
	mov	r3, ip
	cmp	r3, #0
	bne.n	.L19
	add	r0, sp, #4
	ldrh	r0, [r0, #0]
	adds	r2, #1
	strh	r0, [r1, #0]
	b.n	.L20
	movs	r0, r0
	.4byte 0x00000856
	.4byte 0x00000855
.L18:
	ldr	r0, [sp, #0]
	movs	r4, #0
	cmp	r0, #7
	bls.n	.L21
	movs	r4, #1
.L21:
	movs	r0, #0
	cmp	r3, #7
	bhi.n	.L22
	movs	r0, #1
.L22:
	cmp	r4, r0
	beq.n	.L19
	strh	r3, [r1, #0]
	adds	r2, #1
.L20:
	adds	r1, #2
.L19:
	adds	r6, #1
	cmp	r6, r8
	bcc.n	.L23
.L17:
	ldr	r3, [pc, #36]
	lsls	r2, r2, #1
	mov	r1, sl
	strh	r3, [r1, r2]
	mov	r0, sl
	movs	r1, #0
	bl	Func_080b7b6c
	mov	r2, r9
	movs	r3, #1
	ldrsb	r3, [r2, r3]
	movs	r0, #0
	cmp	r0, r3
	bge.n	.L24
	mov	ip, r3
	mov	r1, sl
	adds	r2, #2
	mov	r0, ip
	b.n	.L25
	.4byte 0x000000ff
.L25:
	ldrb	r3, [r2, #0]
	subs	r0, #1
	strh	r3, [r1, #0]
	adds	r2, #1
	adds	r1, #2
	cmp	r0, #0
	bne.n	.L25
	mov	r0, ip
.L24:
	ldr	r2, [pc, #20]
	lsls	r3, r0, #1
	mov	r0, sl
	strh	r2, [r0, r3]
	ldr	r3, [r7, #20]
	movs	r6, #0
	adds	r2, r7, #0
	cmp	r3, #0
	beq.n	.L26
	movs	r5, #0
	b.n	.L27
	movs	r0, r0
	.4byte 0x000000ff
.L27:
	lsls	r3, r6, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	Func_080b7dd0
	movs	r1, #0
	ldr	r0, [r0, #0]
	bl	Func_080b7f70
	adds	r3, r0, #0
	adds	r3, #39
	ldrb	r3, [r3, #0]
	subs	r3, #1
	movs	r1, #0
	cmp	r3, #0
	beq.n	.L28
	mov	ip, r3
	adds	r3, r5, r7
	adds	r2, r3, #0
	adds	r2, #52
	adds	r0, #40
.L29:
	ldmia	r0!, {r3}
	ldrb	r3, [r3, #5]
	adds	r1, #1
	strb	r3, [r2, #0]
	adds	r2, #1
	cmp	r1, ip
	bne.n	.L29
.L28:
	ldr	r3, [r7, #20]
	adds	r6, #1
	adds	r5, #4
	adds	r2, r7, #0
	cmp	r6, r3
	bne.n	.L27
.L26:
	mov	r3, r9
	ldr	r2, [r3, #88]
	movs	r3, #128
	lsls	r3, r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L30
	ldr	r0, [sp, #0]
	cmp	r0, #7
	bls.n	.L31
	movs	r3, #0
	str	r3, [r7, #4]
	b.n	.L32
.L30:
	mov	r1, r9
	ldrb	r3, [r1, #2]
	cmp	r3, #7
	bhi.n	.L33
.L31:
	movs	r3, #1
	str	r3, [r7, #4]
	b.n	.L32
.L33:
	str	r2, [r7, #4]
.L32:
	mov	r2, r9
	ldr	r3, [r2, #88]
	movs	r2, #128
	lsls	r2, r2, #10
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L34
	ldr	r3, [r7, #4]
	movs	r2, #1
	eors	r3, r2
	str	r3, [r7, #4]
.L34:
	movs	r1, #200
	ldr	r0, [pc, #48]
	lsls	r1, r1, #4
	bl	Func_080041d8
	mov	r3, r9
	ldr	r0, [r3, #88]
	movs	r3, #128
	lsls	r3, r3, #8
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L35
	adds	r0, r7, #0
	bl	Func_080c9010
	b.n	.L36
.L35:
	movs	r3, #128
	lsls	r3, r3, #7
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L37
	adds	r0, r7, #0
	bl	Func_080c9008
	b.n	.L36
	movs	r0, r0
	.4byte 0x080bd899
.L37:
	adds	r0, r7, #0
	bl	Func_080c9018
.L36:
	bl	Func_080be02c
.L16:
	bl	Func_080b6c90
	movs	r0, #3
	mov	r1, sl
	bl	Func_080b6c08
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #40]
	mov	r8, r0
	strh	r3, [r2, #0]
	movs	r6, #0
	cmp	r0, #0
	beq.n	.L38
	movs	r5, #0
.L42:
	mov	r0, sl
	ldrh	r3, [r5, r0]
	cmp	r3, #254
	beq.n	.L39
	ldr	r1, [sp, #4]
	adds	r0, r3, #0
	cmp	r0, r1
	beq.n	.L39
	ldr	r3, [sp, #0]
	movs	r2, #0
	cmp	r3, #7
	bhi.n	.L40
	movs	r2, #1
	b.n	.L40
	movs	r0, r0
	.4byte 0x00003f40
	.4byte 0x04000050
.L40:
	movs	r3, #0
	cmp	r0, #7
	bhi.n	.L41
	movs	r3, #1
.L41:
	cmp	r2, r3
	beq.n	.L39
	movs	r1, #1
	bl	Func_080c0f98
.L39:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r8
	bne.n	.L42
.L38:
	ldr	r7, [pc, #40]
	ldr	r5, [pc, #32]
	movs	r6, #0
.L43:
	adds	r3, r6, #0
	orrs	r3, r5
	strh	r3, [r7, #0]
	movs	r0, #1
	adds	r6, #1
	bl	Func_080030f8
	cmp	r6, #16
	bne.n	.L43
	mov	r0, r8
	movs	r6, #0
	cmp	r0, #0
	beq.n	.L44
	mov	r5, sl
	b.n	.L45
	movs	r0, r0
	.4byte 0x00001000
	.4byte 0x04000052
.L45:
	ldrh	r0, [r5, #0]
	movs	r1, #0
	adds	r6, #1
	adds	r5, #2
	bl	Func_080c0f98
	cmp	r6, r8
	bne.n	.L45
.L44:
	movs	r1, #0
	movs	r2, #0
	movs	r3, #100
	movs	r0, #0
	bl	Func_080c0cec
	movs	r0, #1
	bl	Func_080030f8
	movs	r0, #0
	add	sp, #124
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
