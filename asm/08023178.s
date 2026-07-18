@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08023178
	.thumb_func
Func_08023178:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #352
	str	r0, [sp, #92]
	str	r2, [sp, #84]
	str	r1, [sp, #88]
	ldr	r5, [pc, #360]
	movs	r3, #1
	ldr	r1, [r5, #0]
	movs	r0, #128
	movs	r2, #1
	negs	r3, r3
	lsls	r0, r0, #2
	str	r1, [sp, #80]
	mov	sl, r2
	str	r3, [sp, #68]
	bl	Func_080040b4
	movs	r4, #0
	str	r0, [sp, #52]
	ldr	r0, [sp, #68]
	str	r4, [sp, #48]
	bl	Func_08077290
	str	r0, [sp, #44]
	adds	r5, #168
	ldr	r5, [r5, #0]
	ldr	r0, [r5, #68]
	mov	r1, sl
	str	r1, [r5, #72]
	cmp	r0, #0
	beq.n	.L0
	bl	Func_08016418
	ldr	r2, [sp, #48]
	str	r2, [r5, #68]
.L0:
	movs	r0, #112
	bl	Func_080f9010
	movs	r6, #4
	movs	r2, #0
	add	r3, sp, #100
.L1:
	subs	r6, #1
	strb	r2, [r3, #0]
	subs	r3, #1
	cmp	r6, #0
	bge.n	.L1
	movs	r3, #0
	add	r2, sp, #96
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	str	r3, [r2, #16]
	movs	r0, #128
	bl	Func_080040b4
	movs	r3, #142
	movs	r4, #136
	lsls	r3, r3, #1
	lsls	r4, r4, #1
	add	r3, sp
	add	r4, sp
	movs	r1, #1
	negs	r1, r1
	str	r0, [sp, #64]
	str	r3, [sp, #16]
	str	r4, [sp, #20]
	mov	r8, r1
	adds	r5, r4, #0
	adds	r7, r3, #0
	movs	r6, #10
.L2:
	movs	r0, #128
	bl	Func_080040b4
	subs	r6, #1
	mov	r2, r8
	strb	r2, [r5, #0]
	stmia	r7!, {r0}
	adds	r5, #1
	cmp	r6, #0
	bge.n	.L2
	ldr	r3, [sp, #88]
	cmp	r3, #0
	beq.n	.L3
	ldr	r4, [sp, #92]
	ldrh	r3, [r4, #0]
	movs	r6, #0
	cmp	r3, #255
	beq.n	.L3
	cmp	r3, #254
	beq.n	.L4
	ldr	r1, [sp, #84]
	cmp	r3, r1
	beq.n	.L5
.L4:
	adds	r6, #1
	cmp	r6, #5
	bgt.n	.L3
	ldr	r2, [sp, #92]
	lsls	r3, r6, #1
	ldrh	r3, [r3, r2]
	cmp	r3, #255
	beq.n	.L3
	cmp	r3, #254
	beq.n	.L4
	ldr	r4, [sp, #84]
	cmp	r3, r4
	bne.n	.L4
.L5:
	str	r6, [sp, #68]
.L3:
	movs	r3, #6
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #30
	movs	r3, #20
	movs	r0, #0
	str	r1, [sp, #56]
	str	r1, [sp, #60]
	bl	Func_080162d4
	movs	r3, #10
	str	r0, [sp, #76]
	str	r3, [sp, #0]
	movs	r1, #14
	movs	r2, #30
	movs	r3, #6
	movs	r0, #0
	bl	Func_080162d4
	str	r0, [sp, #72]
	bl	Func_0801e318
	movs	r3, #170
	movs	r4, #164
	mov	r2, sp
	lsls	r3, r3, #1
	lsls	r4, r4, #1
	mov	r1, sp
	adds	r2, #128
	add	r3, sp
	add	r4, sp
	adds	r1, #140
	str	r2, [sp, #28]
	str	r3, [sp, #8]
	str	r4, [sp, #12]
	str	r1, [sp, #24]
.L88:
	ldr	r3, [pc, #88]
	ldr	r3, [r3, #0]
	ldr	r2, [sp, #28]
	str	r3, [sp, #40]
	ldr	r3, [pc, #84]
	str	r3, [r2, #4]
	movs	r3, #0
	str	r3, [r2, #8]
	ldr	r1, [sp, #52]
	ldr	r0, [sp, #84]
	bl	Func_08021b80
	ldr	r3, [pc, #52]
	ands	r0, r3
	ldr	r3, [sp, #28]
	ldrh	r2, [r3, #8]
	ldr	r3, [pc, #64]
	ldr	r4, [sp, #28]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r4, #8]
	ldrh	r2, [r4, #6]
	ldr	r3, [pc, #56]
	ands	r3, r2
	ldr	r2, [pc, #32]
	ldr	r1, [sp, #28]
	orrs	r3, r2
	strh	r3, [r1, #6]
	ldrb	r2, [r1, #9]
	movs	r3, #24
	strb	r3, [r1, #4]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #224
	orrs	r3, r2
	strb	r3, [r1, #9]
	ldr	r0, [sp, #28]
	movs	r1, #240
	b.n	.L6
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x00000008
	.4byte 0x03001e8c
	.4byte 0x03001b04
	.4byte 0x80000400
	.4byte 0xfffffc00
	.4byte 0xfffffe00
.L6:
	bl	Func_08003dec
	ldr	r3, [sp, #104]
	cmp	r3, #21
	bhi.n	.L7
	ldr	r2, [pc, #516]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023390
	.4byte 0x08023390
	.4byte 0x08023390
	.4byte 0x08023390
	.4byte 0x0802339c
	.4byte 0x0802339c
	.4byte 0x0802339c
	.4byte 0x0802339c
	.4byte 0x0802339c
	.4byte 0x0802339c
	.4byte 0x0802339c
	.4byte 0x0802339c
.L7:
	movs	r3, #0
	add	r2, sp, #96
	str	r3, [r2, #16]
	ldr	r3, [sp, #44]
	cmp	r3, #0
	beq.n	.L8
	movs	r3, #9
	str	r3, [r2, #20]
	b.n	.L9
.L8:
	movs	r3, #7
	str	r3, [r2, #20]
	b.n	.L9
	add	r2, sp, #96
	movs	r3, #1
	str	r3, [r2, #16]
	movs	r3, #4
	str	r3, [r2, #20]
	b.n	.L9
	add	r2, sp, #96
	movs	r3, #2
	str	r3, [r2, #16]
	ldr	r4, [sp, #48]
	str	r4, [r2, #20]
.L9:
	ldr	r1, [sp, #76]
	ldr	r3, [sp, #76]
	ldrh	r0, [r1, #12]
	ldrh	r2, [r3, #8]
	movs	r4, #15
	ldrh	r1, [r1, #14]
	ldrh	r3, [r3, #10]
	str	r4, [sp, #0]
	bl	Func_08022768
	ldr	r4, [sp, #48]
	cmp	r4, #0
	bne.n	.L10
	b.n	.L11
.L10:
	add	r6, sp, #96
	ldr	r3, [r6, #16]
	cmp	r3, #1
	bhi.n	.L12
	ldr	r1, [sp, #40]
	ldrsb	r5, [r6, r3]
	movs	r3, #128
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L13
	movs	r2, #0
	movs	r0, #111
	str	r2, [sp, #40]
	bl	Func_080f9010
	ldr	r3, [r6, #20]
	adds	r5, #1
	cmp	r5, r3
	blt.n	.L14
	ldr	r3, [r6, #16]
	movs	r5, #0
	b.n	.L15
.L13:
	ldr	r4, [sp, #40]
	movs	r3, #64
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L16
	movs	r1, #0
	movs	r0, #111
	subs	r5, #1
	str	r1, [sp, #40]
	bl	Func_080f9010
	cmp	r5, #0
	bge.n	.L14
	ldr	r3, [r6, #20]
	subs	r5, r3, #1
	ldr	r3, [r6, #16]
.L15:
	cmp	r3, #1
	bne.n	.L14
	movs	r3, #2
	str	r3, [r6, #16]
	ldrsb	r5, [r6, r3]
	b.n	.L14
.L16:
	ldr	r2, [sp, #40]
	movs	r3, #49
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L14
	movs	r3, #0
	movs	r0, #111
	str	r3, [sp, #40]
	bl	Func_080f9010
	ldr	r1, [r6, #16]
	movs	r3, #2
	eors	r1, r3
	str	r1, [r6, #16]
	ldr	r0, [pc, #232]
	ldr	r3, [r0, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L17
	ldrsb	r5, [r6, r1]
	b.n	.L14
.L17:
	ldr	r2, [r0, #0]
	movs	r3, #32
	ldr	r5, [sp, #48]
	ands	r2, r3
	subs	r5, #1
	cmp	r2, #0
	bne.n	.L14
	movs	r5, #0
	b.n	.L14
.L12:
	cmp	r3, #2
	bne.n	.L11
	ldrsb	r5, [r6, r3]
	ldr	r2, [r6, #20]
	cmp	r5, r2
	blt.n	.L18
	subs	r5, r2, #1
.L18:
	cmp	r5, #0
	bge.n	.L19
	movs	r3, #0
	str	r3, [r6, #16]
	movs	r5, #0
	ldrsb	r5, [r6, r5]
	b.n	.L14
.L19:
	ldr	r4, [sp, #40]
	movs	r3, #16
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L20
	movs	r1, #0
	adds	r5, #1
	str	r1, [sp, #40]
	cmp	r5, r2
	blt.n	.L21
	str	r1, [r6, #16]
	b.n	.L22
.L20:
	ldr	r2, [sp, #40]
	movs	r3, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L23
	movs	r3, #0
	subs	r5, #1
	str	r3, [sp, #40]
	cmp	r5, #0
	bge.n	.L21
	str	r3, [r6, #16]
.L22:
	movs	r5, #0
	ldrsb	r5, [r6, r5]
.L21:
	movs	r0, #111
	bl	Func_080f9010
	b.n	.L14
.L23:
	ldr	r4, [sp, #40]
	movs	r3, #193
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L14
	movs	r1, #0
	str	r1, [sp, #40]
	movs	r0, #111
	str	r1, [r6, #16]
	movs	r5, #0
	ldrsb	r5, [r6, r5]
	bl	Func_080f9010
.L14:
	add	r2, sp, #96
	ldr	r3, [r2, #16]
	strb	r5, [r2, r3]
.L11:
	add	r0, sp, #96
	ldr	r2, [r0, #16]
	cmp	r2, #0
	bne.n	.L24
	ldr	r3, [sp, #44]
	ldrsb	r2, [r0, r2]
	cmp	r3, #0
	bne.n	.L25
	adds	r2, #9
.L25:
	ldr	r3, [pc, #64]
	lsls	r2, r2, #3
	adds	r2, r2, r3
	ldrb	r3, [r2, #0]
	str	r3, [r0, #8]
	ldrb	r3, [r2, #1]
	str	r3, [r0, #24]
	ldrb	r3, [r2, #2]
	str	r3, [r0, #28]
	ldr	r4, [sp, #76]
	ldrb	r3, [r2, #3]
	ldrh	r0, [r4, #12]
	ldrh	r1, [r4, #14]
	adds	r0, r0, r3
	ldrb	r3, [r2, #4]
	adds	r1, r1, r3
	movs	r3, #14
	ldrb	r2, [r2, #5]
	adds	r0, #1
	str	r3, [sp, #0]
	adds	r1, #1
	movs	r3, #1
	bl	Func_08022768
	b.n	.L26
.L24:
	cmp	r2, #1
	bne.n	.L27
	ldrsb	r3, [r0, r2]
	ldr	r1, [pc, #12]
	b.n	.L28
	.4byte 0x08023320
	.4byte 0x03001b04
	.4byte 0x08037328
	.4byte 0x080373a8
.L27:
	cmp	r2, #2
	bne.n	.L26
	ldrsb	r3, [r0, r2]
	ldr	r1, [pc, #156]
.L28:
	lsls	r3, r3, #2
	ldrb	r2, [r1, r3]
	adds	r3, r3, r1
	str	r2, [r0, #8]
	ldrb	r2, [r3, #1]
	ldrb	r3, [r3, #2]
	str	r2, [r0, #24]
	str	r3, [r0, #28]
.L26:
	add	r2, sp, #96
	ldr	r3, [r2, #12]
	ldr	r1, [r2, #8]
	cmp	r3, r1
	beq.n	.L29
	str	r1, [r2, #12]
	movs	r1, #2
	mov	sl, r1
.L29:
	ldr	r3, [r2, #24]
	lsls	r3, r3, #3
	str	r3, [sp, #56]
	ldr	r3, [r2, #28]
	ldr	r0, [sp, #84]
	lsls	r3, r3, #3
	str	r3, [sp, #60]
	bl	Func_080b5098
	ldr	r3, [r0, #0]
	ldr	r4, [sp, #8]
	ldr	r2, [r3, #80]
	ldr	r3, [pc, #100]
	movs	r7, #0
	str	r3, [r4, #4]
	str	r7, [r4, #8]
	ldr	r1, [pc, #96]
	ldrh	r2, [r2, #8]
	ldr	r5, [pc, #96]
	mov	r8, r1
	ldrh	r1, [r4, #8]
	adds	r3, r5, #0
	lsls	r2, r2, #22
	lsrs	r2, r2, #22
	ands	r3, r1
	orrs	r3, r2
	ldr	r2, [sp, #8]
	strh	r3, [r2, #8]
	ldr	r3, [sp, #8]
	ldr	r6, [pc, #80]
	ldrh	r2, [r3, #6]
	adds	r3, r6, #0
	ands	r3, r2
	ldr	r2, [pc, #52]
	ldr	r4, [sp, #8]
	orrs	r3, r2
	strh	r3, [r4, #6]
	movs	r3, #56
	strb	r3, [r4, #4]
	ldr	r0, [sp, #8]
	movs	r1, #240
	bl	Func_08003dec
	ldr	r3, [pc, #56]
	ldr	r1, [sp, #12]
	str	r3, [r1, #4]
	str	r7, [r1, #8]
	ldr	r0, [sp, #64]
	ldr	r1, [pc, #48]
	bl	Func_080040d0
	ldr	r4, [sp, #12]
	ldrh	r3, [r4, #8]
	mov	r2, r8
	ands	r2, r0
	ands	r5, r3
	adds	r1, r4, #0
	orrs	r5, r2
	strh	r5, [r1, #8]
	b.n	.L30
	.4byte 0x000000ac
	.4byte 0x080373b8
	.4byte 0xc0002400
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0xfffffe00
	.4byte 0x40000400
	.4byte 0x080310a4
.L30:
	ldr	r3, [sp, #76]
	ldr	r0, [pc, #84]
	ldrh	r2, [r3, #12]
	ldr	r3, [r0, #0]
	ldr	r4, [sp, #56]
	movs	r1, #4
	ands	r3, r1
	lsls	r2, r2, #3
	adds	r2, r4, r2
	lsrs	r3, r3, #2
	ldr	r4, [sp, #12]
	subs	r2, r2, r3
	ldr	r3, [pc, #56]
	adds	r2, #16
	ands	r2, r3
	ldrh	r3, [r4, #6]
	ands	r6, r3
	orrs	r6, r2
	adds	r2, r4, #0
	strh	r6, [r2, #6]
	ldr	r3, [sp, #76]
	ldrh	r2, [r3, #14]
	ldr	r3, [r0, #0]
	ldr	r4, [sp, #60]
	ands	r3, r1
	lsls	r2, r2, #3
	adds	r2, r4, r2
	lsrs	r3, r3, #2
	ldr	r1, [sp, #12]
	subs	r2, r2, r3
	adds	r2, #16
	strb	r2, [r1, #4]
	movs	r3, #63
	ldrb	r2, [r1, #7]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #16
	orrs	r3, r2
	strb	r3, [r1, #7]
	ldr	r0, [sp, #12]
	b.n	.L31
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x03001e40
.L31:
	movs	r1, #241
	bl	Func_08003dec
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L32
	b.n	.L33
.L32:
	ldr	r0, [sp, #84]
	bl	Func_08077008
	mov	r9, r0
	bl	Func_0801e318
	bl	Func_08016738
	movs	r3, #1
	mov	r4, sl
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L34
	b.n	.L35
.L34:
	ldr	r0, [sp, #76]
	bl	Func_08016498
	ldr	r1, [sp, #72]
	ldr	r3, [sp, #72]
	ldrh	r0, [r1, #12]
	ldrh	r2, [r3, #8]
	ldrh	r1, [r1, #14]
	ldrh	r3, [r3, #10]
	str	r7, [sp, #0]
	bl	Func_08017248
	movs	r3, #14
	str	r3, [sp, #0]
	ldr	r0, [sp, #76]
	movs	r1, #0
	movs	r2, #14
	movs	r3, #29
	bl	Func_0801e41c
	mov	r0, r9
	ldr	r1, [sp, #76]
	movs	r2, #0
	movs	r3, #0
	bl	Func_0801e8b0
	ldr	r0, [pc, #616]
	ldr	r1, [sp, #76]
	movs	r2, #56
	movs	r3, #0
	bl	Func_0801e8b0
	mov	r4, r9
	ldr	r2, [sp, #76]
	ldrb	r0, [r4, #15]
	movs	r1, #2
	movs	r3, #72
	str	r7, [sp, #0]
	bl	Func_0801ea08
	ldr	r7, [pc, #592]
	ldr	r1, [sp, #76]
	adds	r0, r7, #0
	movs	r2, #0
	movs	r3, #8
	bl	Func_0801e7c0
	movs	r3, #146
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r0, [r3, #0]
	movs	r3, #8
	str	r3, [sp, #0]
	ldr	r2, [sp, #76]
	movs	r1, #8
	movs	r3, #40
	bl	Func_0801ea08
	ldr	r0, [pc, #560]
	ldr	r1, [sp, #76]
	movs	r2, #40
	movs	r3, #24
	bl	Func_0801e8b0
	movs	r3, #24
	mov	r2, r9
	movs	r1, #56
	ldrsh	r0, [r2, r1]
	mov	r8, r3
	str	r3, [sp, #0]
	ldr	r2, [sp, #76]
	movs	r1, #4
	movs	r3, #56
	bl	Func_0801ea08
	ldr	r5, [pc, #528]
	ldr	r1, [sp, #76]
	adds	r0, r5, #0
	movs	r2, #88
	movs	r3, #24
	bl	Func_0801e8b0
	mov	r1, r9
	mov	r2, r8
	movs	r4, #52
	ldrsh	r0, [r1, r4]
	movs	r3, #96
	str	r2, [sp, #0]
	movs	r1, #4
	ldr	r2, [sp, #76]
	bl	Func_0801ea08
	movs	r3, #32
	ldr	r1, [sp, #76]
	ldr	r0, [pc, #496]
	movs	r2, #40
	bl	Func_0801e8b0
	mov	r4, r9
	ldr	r2, [sp, #76]
	movs	r3, #58
	ldrsh	r0, [r4, r3]
	movs	r6, #32
	movs	r1, #4
	movs	r3, #56
	str	r6, [sp, #0]
	bl	Func_0801ea08
	adds	r0, r5, #0
	ldr	r1, [sp, #76]
	movs	r2, #88
	movs	r3, #32
	bl	Func_0801e8b0
	mov	r2, r9
	movs	r1, #54
	ldrsh	r0, [r2, r1]
	movs	r3, #96
	ldr	r2, [sp, #76]
	movs	r1, #4
	str	r6, [sp, #0]
	bl	Func_0801ea08
	adds	r0, r7, #0
	ldr	r1, [sp, #76]
	subs	r0, #10
	movs	r2, #136
	movs	r3, #16
	bl	Func_0801e7c0
	mov	r3, r9
	ldrh	r0, [r3, #60]
	movs	r3, #16
	str	r3, [sp, #0]
	ldr	r2, [sp, #76]
	movs	r1, #3
	movs	r3, #184
	bl	Func_0801ea08
	adds	r0, r7, #0
	ldr	r1, [sp, #76]
	subs	r0, #9
	movs	r2, #136
	movs	r3, #24
	bl	Func_0801e7c0
	mov	r4, r9
	mov	r1, r8
	ldrh	r0, [r4, #62]
	ldr	r2, [sp, #76]
	str	r1, [sp, #0]
	movs	r3, #184
	movs	r1, #3
	bl	Func_0801ea08
	adds	r0, r7, #0
	ldr	r1, [sp, #76]
	subs	r0, #8
	movs	r2, #136
	movs	r3, #32
	bl	Func_0801e7c0
	mov	r3, r9
	adds	r3, #64
	ldr	r2, [sp, #76]
	ldrh	r0, [r3, #0]
	movs	r1, #3
	movs	r3, #184
	str	r6, [sp, #0]
	bl	Func_0801ea08
	subs	r0, r7, #7
	ldr	r1, [sp, #76]
	movs	r2, #136
	movs	r3, #40
	bl	Func_0801e7c0
	mov	r3, r9
	adds	r3, #66
	ldrb	r0, [r3, #0]
	movs	r3, #40
	str	r3, [sp, #0]
	ldr	r2, [sp, #76]
	movs	r1, #3
	movs	r3, #184
	bl	Func_0801ea08
	ldr	r3, [pc, #312]
	add	r3, r9
	ldrb	r0, [r3, #0]
	ldr	r3, [pc, #312]
	movs	r2, #0
	adds	r0, r0, r3
	ldr	r1, [sp, #76]
	movs	r3, #48
	bl	Func_0801e7c0
	ldr	r2, [sp, #44]
	cmp	r2, #0
	beq.n	.L36
	subs	r0, r7, #1
	ldr	r1, [sp, #76]
	movs	r2, #0
	movs	r3, #72
	bl	Func_0801e7c0
.L36:
	ldr	r1, [sp, #76]
	subs	r0, r7, #5
	movs	r2, #0
	movs	r3, #80
	bl	Func_0801e7c0
	subs	r0, r7, #4
	ldr	r1, [sp, #76]
	movs	r2, #0
	movs	r3, #88
	bl	Func_0801e7c0
	movs	r6, #140
	subs	r0, r7, #3
	ldr	r1, [sp, #76]
	movs	r2, #0
	movs	r3, #96
	bl	Func_0801e7c0
	lsls	r6, r6, #1
	movs	r3, #72
	movs	r1, #40
	movs	r2, #7
	movs	r4, #0
	mov	fp, r3
	mov	r8, r1
	add	r6, r9
	movs	r7, #48
	mov	sl, r2
.L39:
	ldr	r1, [sp, #44]
	movs	r3, #1
	cmp	r1, #0
	beq.n	.L37
	movs	r3, #0
.L37:
	ldr	r2, [pc, #216]
	adds	r1, r4, r2
	movs	r2, #0
	adds	r3, #8
	str	r2, [sp, #0]
	ldr	r0, [sp, #76]
	mov	r2, sl
	lsls	r5, r4, #2
	str	r4, [sp, #4]
	bl	Func_08019000
	ldr	r3, [sp, #44]
	ldr	r4, [sp, #4]
	cmp	r3, #0
	beq.n	.L38
	mov	r1, fp
	ldr	r2, [sp, #76]
	ldrb	r0, [r6, #4]
	mov	r3, r8
	str	r1, [sp, #0]
	movs	r1, #1
	bl	Func_0801ea08
	ldr	r0, [pc, #152]
	ldr	r1, [sp, #76]
	adds	r2, r7, #0
	movs	r3, #72
	bl	Func_0801e8b0
	mov	r2, fp
	adds	r3, r7, #0
	ldrb	r0, [r6, #0]
	adds	r3, #8
	str	r2, [sp, #0]
	movs	r1, #1
	ldr	r2, [sp, #76]
	bl	Func_0801ea08
	ldr	r4, [sp, #4]
.L38:
	adds	r1, r4, #0
	ldr	r0, [sp, #84]
	str	r4, [sp, #4]
	bl	Func_080771f8
	movs	r3, #80
	str	r3, [sp, #0]
	ldr	r2, [sp, #76]
	movs	r1, #2
	adds	r3, r7, #0
	bl	Func_0801ea08
	adds	r5, #72
	mov	r3, r9
	ldrsh	r0, [r3, r5]
	movs	r3, #88
	ldr	r2, [sp, #76]
	str	r3, [sp, #0]
	movs	r1, #3
	mov	r3, r8
	bl	Func_0801ea08
	add	r5, r9
	movs	r3, #96
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	movs	r1, #3
	str	r3, [sp, #0]
	ldr	r2, [sp, #76]
	mov	r3, r8
	bl	Func_0801ea08
	ldr	r4, [sp, #4]
	movs	r3, #32
	movs	r1, #4
	adds	r4, #1
	add	r8, r3
	adds	r6, #1
	adds	r7, #32
	add	sl, r1
	cmp	r4, #3
	ble.n	.L39
	mov	r4, r9
	movs	r2, #56
	ldrsh	r3, [r4, r2]
	movs	r6, #0
	cmp	r3, #0
	bne.n	.L40
	ldr	r1, [sp, #20]
	movs	r3, #16
	strb	r3, [r1, #0]
	movs	r6, #1
.L40:
	ldr	r3, [sp, #20]
	adds	r2, r6, r3
	b.n	.L41
	movs	r0, r0
	.4byte 0x080373d8
	.4byte 0x000008ba
	.4byte 0x080373dc
	.4byte 0x080373e0
	.4byte 0x080373e4
	.4byte 0x00000129
	.4byte 0x00000741
	.4byte 0x00005001
.L65:
	ldr	r1, [pc, #812]
	add	r1, r9
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #1
	bne.n	.L42
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L42:
	cmp	r6, #7
	ble.n	.L43
	b.n	.L44
.L43:
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #2
	bne.n	.L45
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L45:
	cmp	r6, #7
	ble.n	.L46
	b.n	.L44
.L46:
	ldr	r3, [pc, #772]
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L47
	movs	r3, #4
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L47:
	cmp	r6, #7
	ble.n	.L48
	b.n	.L44
.L48:
	ldr	r3, [pc, #752]
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L49
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L49:
	cmp	r6, #7
	ble.n	.L50
	b.n	.L44
.L50:
	movs	r3, #158
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L51
	movs	r3, #5
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L51:
	cmp	r6, #7
	ble.n	.L52
	b.n	.L44
.L52:
	movs	r3, #160
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L53
	movs	r3, #7
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L53:
	cmp	r6, #7
	ble.n	.L54
	b.n	.L44
.L54:
	movs	r3, #156
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L55
	movs	r3, #6
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L55:
	cmp	r6, #7
	bgt.n	.L44
	movs	r3, #153
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L56
	ldr	r3, [pc, #644]
	add	r3, r9
	ldrb	r1, [r3, #0]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L57
	movs	r3, #9
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L57:
	lsls	r3, r1, #24
	cmp	r3, #0
	bge.n	.L56
	movs	r3, #10
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L56:
	cmp	r6, #7
	bgt.n	.L44
	movs	r3, #154
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L58
	ldr	r3, [pc, #596]
	add	r3, r9
	ldrb	r1, [r3, #0]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L59
	movs	r3, #11
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L59:
	lsls	r3, r1, #24
	cmp	r3, #0
	bge.n	.L58
	movs	r3, #12
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L58:
	cmp	r6, #7
	bgt.n	.L44
	movs	r3, #155
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L60
	ldr	r3, [pc, #544]
	add	r3, r9
	ldrb	r1, [r3, #0]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L61
	movs	r3, #13
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L61:
	lsls	r3, r1, #24
	cmp	r3, #0
	bge.n	.L60
	movs	r3, #14
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L60:
	cmp	r6, #7
	bgt.n	.L44
	ldr	r3, [pc, #508]
	add	r3, r9
	ldrb	r1, [r3, #0]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L62
	movs	r3, #17
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L62:
	lsls	r3, r1, #24
	cmp	r3, #0
	bge.n	.L44
	movs	r3, #18
	strb	r3, [r2, #0]
	adds	r6, #1
	b.n	.L44
.L41:
	movs	r3, #152
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L63
	movs	r3, #15
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L63:
	cmp	r6, #7
	bgt.n	.L44
	ldr	r3, [pc, #444]
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L64
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L64:
	cmp	r6, #7
	bgt.n	.L44
	b.n	.L65
.L44:
	cmp	r6, #0
	ble.n	.L66
	ldr	r7, [sp, #16]
	ldr	r5, [sp, #20]
	adds	r4, r6, #0
.L67:
	ldrb	r0, [r5, #0]
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	ldmia	r7!, {r1}
	str	r4, [sp, #4]
	bl	Func_08021ab0
	ldr	r4, [sp, #4]
	subs	r4, #1
	adds	r5, #1
	cmp	r4, #0
	bne.n	.L67
.L66:
	cmp	r6, #0
	bne.n	.L68
	ldr	r4, [sp, #20]
	strb	r6, [r4, #0]
	movs	r6, #1
.L68:
	cmp	r6, #10
	bgt.n	.L69
	ldr	r3, [sp, #20]
	movs	r2, #1
	negs	r2, r2
	adds	r1, r2, #0
	adds	r2, r6, r3
	movs	r3, #11
	subs	r4, r3, r6
.L70:
	subs	r4, #1
	strb	r1, [r2, #0]
	adds	r2, #1
	cmp	r4, #0
	bne.n	.L70
.L69:
	str	r6, [sp, #48]
	ldr	r4, [sp, #20]
	movs	r3, #0
	ldrsb	r3, [r4, r3]
	cmp	r3, #0
	bne.n	.L35
	mov	r2, r9
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	beq.n	.L71
	ldr	r0, [pc, #332]
	b.n	.L72
.L71:
	ldr	r0, [pc, #332]
.L72:
	ldr	r1, [sp, #76]
	movs	r2, #112
	movs	r3, #0
	bl	Func_0801e7c0
.L35:
	movs	r0, #128
	lsls	r0, r0, #1
	bl	Func_08004938
	ldr	r3, [sp, #104]
	str	r0, [sp, #36]
	cmp	r3, #13
	bhi.n	.L73
	b.n	.L74
.L73:
	ldr	r4, [sp, #20]
	subs	r3, #14
	ldrsb	r3, [r4, r3]
	str	r3, [sp, #32]
	cmp	r3, #0
	bne.n	.L75
	mov	r2, r9
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	bne.n	.L75
	movs	r3, #16
	str	r3, [sp, #32]
.L75:
	movs	r4, #166
	lsls	r4, r4, #1
	mov	r8, r4
	mov	r0, r8
	bl	Func_08004938
	movs	r4, #0
	str	r4, [sp, #4]
	mov	r1, r9
	mov	r2, r8
	ldr	r3, [pc, #260]
	adds	r6, r0, #0
	bl	Func_080072f0
	ldr	r3, [pc, #224]
	ldr	r4, [sp, #4]
	mov	r5, r9
	adds	r5, #64
	add	r3, r9
	mov	r1, r9
	mov	r2, r9
	ldrh	r1, [r1, #60]
	ldrh	r2, [r2, #62]
	ldrh	r7, [r5, #0]
	strb	r4, [r3, #0]
	ldr	r3, [pc, #204]
	add	r3, r9
	strb	r4, [r3, #0]
	ldr	r3, [pc, #208]
	add	r3, r9
	strb	r4, [r3, #0]
	ldr	r0, [sp, #84]
	mov	sl, r1
	mov	fp, r2
	bl	Func_08077010
	mov	r1, r9
	ldrh	r3, [r1, #60]
	mov	r2, sl
	subs	r2, r2, r3
	ldrh	r3, [r1, #62]
	mov	r1, fp
	subs	r1, r1, r3
	ldrh	r3, [r5, #0]
	mov	sl, r2
	mov	fp, r1
	mov	r2, r8
	adds	r1, r6, #0
	subs	r7, r7, r3
	mov	r0, r9
	ldr	r3, [pc, #180]
	bl	Func_080072f0
	adds	r0, r6, #0
	bl	Func_08002df0
	ldr	r3, [sp, #32]
	subs	r3, #8
	ldr	r4, [sp, #4]
	cmp	r3, #10
	bhi.n	.L76
	ldr	r2, [pc, #160]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08023c1c
	.4byte 0x08023c24
	.4byte 0x08023c28
	.4byte 0x08023c2e
	.4byte 0x08023c32
	.4byte 0x08023c40
	.4byte 0x08023c40
	.4byte 0x08023c4e
	.4byte 0x08023c4e
	.4byte 0x08023c38
	.4byte 0x08023c3c
	ldr	r3, [pc, #92]
	add	r3, r9
	ldrb	r4, [r3, #0]
	b.n	.L76
	mov	r4, sl
	b.n	.L76
	mov	r1, sl
	negs	r4, r1
	b.n	.L76
	mov	r4, fp
	b.n	.L76
	mov	r2, fp
	negs	r4, r2
	b.n	.L76
	adds	r4, r7, #0
	b.n	.L76
	negs	r4, r7
	b.n	.L76
	ldr	r3, [pc, #48]
	add	r3, r9
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r4, r3, #2
.L76:
	adds	r0, r4, #0
	movs	r1, #5
	bl	Func_08019908
	ldr	r0, [pc, #56]
	ldr	r3, [sp, #32]
	ldr	r1, [sp, #36]
	adds	r0, r3, r0
	b.n	.L77
	.4byte 0x00000131
	.4byte 0x0000013d
	.4byte 0x0000013b
	.4byte 0x00000133
	.4byte 0x00000135
	.4byte 0x00000137
	.4byte 0x00000147
	.4byte 0x00000141
	.4byte 0x000008bd
	.4byte 0x000008be
	.4byte 0x03001388
	.4byte 0x08023bf0
	.4byte 0x000008d2
.L74:
	cmp	r3, #2
	bne.n	.L78
	mov	r4, r9
	ldrb	r3, [r4, #15]
	cmp	r3, #98
	bhi.n	.L78
	adds	r1, r3, #0
	adds	r1, #1
	ldr	r0, [sp, #84]
	bl	Func_08077258
	movs	r3, #146
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	movs	r1, #5
	subs	r0, r0, r3
	bl	Func_08019908
	ldr	r0, [pc, #12]
	ldr	r1, [sp, #36]
.L77:
	movs	r2, #128
	bl	Func_0801965c
	b.n	.L79
	movs	r0, r0
	.4byte 0x000008bf
.L78:
	ldr	r0, [sp, #104]
	ldr	r3, [pc, #128]
	ldr	r1, [sp, #36]
	adds	r0, r0, r3
	movs	r2, #128
	bl	Func_0801965c
.L79:
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #72]
	movs	r2, #0
	movs	r3, #4
	bl	Func_08017aa4
	ldr	r0, [sp, #36]
	bl	Func_08002df0
	ldr	r4, [pc, #100]
	ldr	r3, [sp, #80]
	adds	r2, r3, r4
	movs	r3, #1
	movs	r1, #0
	strb	r3, [r2, #0]
	mov	sl, r1
.L33:
	ldr	r1, [pc, #92]
	ldr	r5, [sp, #24]
	ldr	r7, [sp, #16]
	movs	r6, #0
	mov	r8, r1
	movs	r4, #112
.L82:
	ldr	r3, [pc, #84]
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #8]
	ldmia	r7!, {r3}
	lsls	r3, r3, #2
	add	r3, r8
	ldrh	r2, [r3, #2]
	ldr	r1, [pc, #44]
	ldrh	r3, [r5, #8]
	lsls	r2, r2, #17
	lsrs	r2, r2, #22
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #8]
	ldr	r3, [pc, #32]
	adds	r1, r4, #0
	ands	r1, r3
	ldr	r2, [pc, #32]
	ldrh	r3, [r5, #6]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r5, #6]
	ldr	r2, [sp, #76]
	ldrh	r3, [r2, #14]
	lsls	r3, r3, #3
	adds	r3, #8
	strb	r3, [r5, #4]
	ldr	r1, [sp, #20]
	ldrsb	r3, [r1, r6]
	b.n	.L80
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x000008c0
	.4byte 0x00000ea3
	.4byte 0x03001b10
	.4byte 0x40000400
.L80:
	cmp	r3, #0
	ble.n	.L81
	adds	r0, r5, #0
	movs	r1, #240
	str	r4, [sp, #4]
	bl	Func_08003dec
	ldr	r4, [sp, #4]
.L81:
	adds	r6, #1
	adds	r4, #15
	adds	r5, #12
	cmp	r6, #10
	ble.n	.L82
	ldr	r3, [pc, #232]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #76]
	cmp	r3, #0
	beq.n	.L83
	ldr	r3, [pc, #224]
	ldr	r3, [r3, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L83
	ldr	r2, [sp, #88]
	cmp	r2, #0
	beq.n	.L84
	movs	r3, #128
	ldr	r4, [sp, #40]
	lsls	r3, r3, #1
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L85
	ldr	r1, [sp, #68]
	adds	r1, #1
	str	r1, [sp, #68]
	cmp	r1, r2
	blt.n	.L86
	movs	r2, #0
	str	r2, [sp, #68]
.L86:
	ldr	r4, [sp, #68]
	ldr	r1, [sp, #92]
	lsls	r3, r4, #1
	ldrh	r3, [r3, r1]
	movs	r2, #1
	movs	r0, #111
	str	r3, [sp, #84]
	mov	sl, r2
	bl	Func_080f9010
	b.n	.L84
.L85:
	movs	r3, #128
	ldr	r4, [sp, #40]
	lsls	r3, r3, #2
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L84
	ldr	r1, [sp, #68]
	subs	r1, #1
	str	r1, [sp, #68]
	cmp	r1, #0
	bge.n	.L87
	ldr	r2, [sp, #88]
	subs	r2, #1
	str	r2, [sp, #68]
.L87:
	ldr	r4, [sp, #68]
	ldr	r1, [sp, #92]
	lsls	r3, r4, #1
	ldrh	r3, [r3, r1]
	movs	r2, #1
	movs	r0, #111
	str	r3, [sp, #84]
	mov	sl, r2
	bl	Func_080f9010
.L84:
	movs	r0, #1
	bl	Func_080030f8
	bl	.L88
.L83:
	ldr	r5, [sp, #16]
	movs	r6, #10
.L89:
	ldmia	r5!, {r0}
	subs	r6, #1
	bl	Func_08003f3c
	cmp	r6, #0
	bge.n	.L89
	ldr	r0, [sp, #52]
	bl	Func_08003f3c
	ldr	r0, [sp, #64]
	bl	Func_08003f3c
	movs	r0, #1
	bl	Func_080030f8
	bl	Func_0801e318
	movs	r1, #1
	ldr	r0, [sp, #76]
	bl	Func_08016418
	movs	r1, #1
	ldr	r0, [sp, #72]
	bl	Func_08016418
	ldr	r5, [pc, #52]
	ldr	r3, [r5, #0]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	adds	r5, #192
	bl	Func_0801f200
	ldr	r2, [r5, #0]
	movs	r3, #0
	str	r3, [r2, #72]
	movs	r0, #1
	bl	Func_080030f8
	movs	r0, #0
	add	sp, #352
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f34
	.4byte 0x03001c94
	.4byte 0x03001e74
