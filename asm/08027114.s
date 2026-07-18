@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08027114
	.thumb_func
Func_08027114:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #100
	str	r0, [sp, #88]
	movs	r0, #128
	lsls	r0, r0, #1
	str	r0, [sp, #68]
	movs	r0, #128
	str	r1, [sp, #84]
	lsls	r0, r0, #3
	movs	r1, #0
	str	r2, [sp, #80]
	str	r1, [sp, #60]
	str	r1, [sp, #56]
	bl	Func_080040b4
	str	r0, [sp, #52]
	movs	r0, #128
	lsls	r0, r0, #2
	bl	Func_080040b4
	movs	r1, #130
	str	r0, [sp, #48]
	lsls	r1, r1, #1
	movs	r0, #57
	bl	Func_080048f4
	mov	r2, sp
	adds	r2, #92
	str	r2, [sp, #36]
	str	r0, [r2, #0]
	movs	r1, #0
	ldr	r0, [sp, #52]
	bl	Func_08021c64
	ldr	r0, [pc, #136]
	bl	Func_08021a18
	ldr	r0, [pc, #136]
	bl	Func_080219c8
	bl	Func_08021848
	ldr	r3, [pc, #128]
	ldr	r2, [sp, #36]
	ldr	r0, [r3, #0]
	ldr	r3, [r2, #0]
	movs	r1, #128
	lsls	r1, r1, #24
	adds	r3, #228
	movs	r2, #7
.L0:
	subs	r2, #1
	stmia	r3!, {r1}
	cmp	r2, #0
	bge.n	.L0
	ldr	r2, [sp, #36]
	ldr	r3, [r2, #0]
	movs	r1, #0
	adds	r3, #36
	movs	r2, #2
.L1:
	subs	r2, #1
	strb	r1, [r3, #0]
	adds	r3, #1
	cmp	r2, #0
	bge.n	.L1
	ldr	r2, [sp, #36]
	ldr	r1, [r2, #0]
	movs	r2, #1
	movs	r3, #0
	negs	r2, r2
	str	r3, [r1, #40]
	str	r3, [r1, #44]
	str	r3, [r1, #60]
	str	r3, [r1, #64]
	str	r3, [r1, #80]
	str	r3, [r1, #72]
	str	r3, [r1, #68]
	str	r2, [r1, #76]
	adds	r3, r0, #0
	adds	r3, #68
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L2
	movs	r3, #1
	ldr	r2, [pc, #52]
	str	r3, [r1, #80]
	ldr	r3, [pc, #24]
	strh	r3, [r2, #8]
	ldr	r3, [pc, #24]
	strh	r3, [r2, #10]
	strh	r3, [r2, #12]
	ldr	r3, [pc, #20]
	movs	r5, #0
	strh	r3, [r2, #14]
	adds	r7, r0, #0
	adds	r6, r0, #0
	adds	r7, #80
	adds	r6, #82
	b.n	.L3
	.4byte 0x00000056
	.4byte 0x00000053
	.4byte 0x00000054
	.4byte 0x06006000
	.4byte 0x06006680
	.4byte 0x03001e74
	.4byte 0x02002224
.L10:
	ldr	r3, [pc, #432]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L4
	adds	r5, #1
	cmp	r5, #24
	ble.n	.L5
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #80]
	ldr	r1, [sp, #36]
	ldr	r3, [r1, #0]
	str	r0, [r3, #80]
	b.n	.L2
.L4:
	ldrh	r2, [r1, #8]
	adds	r3, r2, #0
	movs	r5, #0
	cmp	r3, #86
	bne.n	.L6
	ldrh	r3, [r1, #10]
	cmp	r3, #83
	bne.n	.L6
	ldrh	r3, [r1, #12]
	cmp	r3, #83
	bne.n	.L6
	ldrh	r3, [r1, #14]
	cmp	r3, #84
	beq.n	.L2
.L6:
	adds	r3, r2, #0
	cmp	r3, #69
	bne.n	.L7
	ldrh	r3, [r1, #10]
	cmp	r3, #68
	bne.n	.L7
	ldrh	r3, [r1, #12]
	cmp	r3, #86
	bne.n	.L7
	ldrh	r3, [r1, #14]
	cmp	r3, #83
	beq.n	.L2
.L7:
	ldrh	r3, [r1, #0]
	cmp	r3, #69
	bne.n	.L8
	ldrh	r3, [r1, #2]
	cmp	r3, #88
	bne.n	.L8
	ldrh	r3, [r1, #4]
	cmp	r3, #69
	bne.n	.L8
	ldrh	r3, [r1, #6]
	cmp	r3, #67
	beq.n	.L5
.L8:
	movs	r2, #1
	negs	r2, r2
	str	r2, [sp, #80]
	b.n	.L9
.L5:
	movs	r0, #1
	bl	Func_080030f8
.L3:
	ldrb	r2, [r7, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldrb	r0, [r6, #0]
	ldr	r3, [pc, #304]
	lsls	r2, r2, #3
	adds	r1, r2, r3
	cmp	r0, #0
	beq.n	.L10
	movs	r1, #1
	negs	r1, r1
	str	r1, [sp, #80]
.L9:
	ldr	r0, [sp, #36]
	ldr	r3, [r0, #0]
	movs	r2, #0
	str	r2, [r3, #80]
.L2:
	movs	r1, #200
	ldr	r0, [pc, #280]
	lsls	r1, r1, #4
	bl	Func_080041d8
.L17:
	add	r1, sp, #100
	mov	r9, r1
	bl	Func_080270ac
	ldr	r2, [sp, #36]
	ldr	r0, [r2, #0]
	adds	r3, r0, #0
	adds	r3, #38
	movs	r1, #0
	strb	r1, [r3, #0]
	adds	r2, r0, #0
	movs	r3, #1
	negs	r3, r3
	adds	r2, #224
	str	r3, [r2, #0]
	adds	r3, r0, #0
	adds	r3, #216
	movs	r0, #183
	str	r1, [r3, #0]
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L11
	ldr	r3, [sp, #36]
	ldr	r2, [r3, #0]
	adds	r1, r2, #0
	movs	r3, #1
	adds	r1, #216
	str	r3, [r1, #0]
	adds	r2, #220
	movs	r3, #60
	str	r3, [r2, #0]
.L11:
	ldr	r0, [sp, #80]
	cmp	r0, #0
	ble.n	.L12
	movs	r0, #0
	bl	Func_08021e6c
	adds	r6, r0, #0
	b.n	.L13
.L12:
	movs	r6, #14
.L13:
	cmp	r6, #7
	bne.n	.L14
	movs	r0, #12
	bl	Func_08004938
	ldr	r3, [pc, #184]
	ldrb	r3, [r3, #0]
	adds	r6, r0, #0
	cmp	r3, #0
	beq.n	.L15
	ldr	r3, [pc, #176]
	ldr	r2, [r3, #0]
	movs	r3, #8
	ands	r2, r3
	movs	r0, #2
	cmp	r2, #0
	bne.n	.L16
.L15:
	movs	r0, #1
.L16:
	adds	r1, r6, #0
	bl	Func_080b5090
	adds	r5, r0, #0
	movs	r0, #1
	bl	Func_080030f8
	ldrh	r2, [r6, #0]
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_08023178
	adds	r0, r6, #0
	bl	Func_08002df0
	b.n	.L17
.L14:
	cmp	r6, #4
	bne.n	.L18
	bl	Func_08026fa8
	cmp	r0, #0
	bne.n	.L17
	ldr	r2, [sp, #88]
	movs	r1, #1
	str	r2, [sp, #76]
	str	r1, [sp, #80]
	ldr	r1, [sp, #84]
	ldrh	r3, [r1, #0]
	strh	r3, [r2, #0]
	ldr	r1, [sp, #76]
	ldr	r3, [pc, #104]
	strh	r3, [r1, #4]
	ldr	r2, [sp, #76]
	movs	r3, #99
	strh	r3, [r2, #6]
	ldr	r3, [sp, #76]
	strh	r0, [r3, #8]
	ldr	r0, [sp, #76]
	movs	r3, #128
	lsls	r3, r3, #1
	strh	r3, [r0, #10]
	bl	.L19
.L18:
	cmp	r6, #14
	beq.n	.L20
	bl	.L19
.L20:
	movs	r0, #154
	bl	Func_080f9010
	ldr	r2, [sp, #80]
	movs	r1, #0
	str	r1, [sp, #44]
	cmp	r1, r2
	blt.n	.L21
	bl	.L19
.L21:
	mov	r3, sp
	adds	r3, #96
	str	r1, [sp, #28]
	str	r1, [sp, #24]
	str	r1, [sp, #32]
	str	r3, [sp, #20]
.L28:
	ldr	r0, [sp, #44]
	cmp	r0, #0
	bne.n	.L22
	ldr	r2, [sp, #36]
	ldr	r1, [r2, #0]
	movs	r0, #0
	adds	r1, #84
	bl	Func_080b5130
	b.n	.L23
	movs	r0, r0
	.4byte 0x03001f64
	.4byte 0x02002024
	.4byte 0x08026e81
	.4byte 0x03001f54
	.4byte 0x03001ae8
	.4byte 0x00007ffe
.L22:
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #28]
	ldr	r3, [r0, #0]
	adds	r3, r1, r3
	adds	r2, r3, #0
	adds	r2, #80
	movs	r1, #3
.L24:
	ldrb	r3, [r2, #0]
	subs	r1, #1
	strb	r3, [r2, #4]
	adds	r2, #1
	cmp	r1, #0
	bge.n	.L24
.L23:
	ldr	r3, [sp, #24]
	ldr	r2, [sp, #88]
	adds	r2, r2, r3
	str	r2, [sp, #76]
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #84]
	ldrh	r0, [r0, r1]
	str	r0, [sp, #64]
	bl	Func_08077008
	ldr	r3, [pc, #104]
	str	r0, [sp, #72]
	ldr	r5, [r3, #0]
	ldr	r2, [sp, #64]
	adds	r3, r5, #0
	adds	r3, #224
	str	r2, [r3, #0]
	movs	r2, #0
	ldr	r3, [pc, #92]
	str	r2, [r5, #64]
	adds	r5, #24
	str	r3, [r5, #4]
	str	r2, [r5, #8]
	ldr	r0, [sp, #72]
	movs	r1, #148
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldrb	r0, [r3, #0]
	ldr	r1, [sp, #48]
	bl	Func_08021b80
	ldr	r3, [pc, #56]
	ldrh	r2, [r5, #8]
	ands	r0, r3
	ldr	r3, [pc, #64]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r5, #8]
	ldrh	r2, [r5, #6]
	ldr	r3, [pc, #56]
	ands	r3, r2
	strh	r3, [r5, #6]
	ldrb	r2, [r5, #9]
	movs	r3, #128
	strb	r3, [r5, #4]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #224
	orrs	r3, r2
	strb	r3, [r5, #9]
	ldr	r3, [sp, #36]
	ldr	r2, [r3, #0]
	movs	r3, #1
	adds	r2, #38
	strb	r3, [r2, #0]
.L39:
	bl	Func_0801e318
	ldr	r5, [pc, #8]
	ldr	r1, [r5, #0]
	b.n	.L25
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x03001f34
	.4byte 0x80000400
	.4byte 0xfffffc00
	.4byte 0xfffffe00
.L25:
	adds	r3, r1, #0
	adds	r3, #36
	movs	r7, #0
	strb	r7, [r3, #0]
	ldr	r2, [sp, #28]
	movs	r3, #128
	adds	r2, #228
	lsls	r3, r3, #24
	str	r3, [r1, r2]
	add	r1, sp, #64
	ldr	r2, [sp, #20]
	ldrh	r1, [r1, #0]
	strh	r1, [r2, #0]
	ldr	r2, [sp, #20]
	movs	r3, #255
	ldr	r0, [pc, #48]
	strh	r3, [r2, #2]
	movs	r1, #1
	mov	r8, r0
	ldr	r0, [sp, #20]
	bl	Func_080b50e0
	ldr	r0, [sp, #20]
	bl	Func_0802281c
	movs	r0, #1
	bl	Func_080030f8
	movs	r0, #1
	bl	Func_08021e6c
	adds	r6, r0, #0
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #2
	negs	r3, r3
	cmp	r6, r3
	bne.n	.L26
	b.n	.L27
	.4byte 0x00000000
.L27:
	movs	r0, #12
	bl	Func_08004938
	adds	r5, r0, #0
	adds	r1, r5, #0
	movs	r0, #1
	bl	Func_080b5090
	adds	r6, r0, #0
	ldr	r0, [sp, #36]
	ldr	r3, [r0, #0]
	mov	r1, r8
	adds	r3, #38
	strb	r1, [r3, #0]
	movs	r0, #1
	bl	Func_080030f8
	adds	r0, r5, #0
	ldr	r2, [sp, #64]
	adds	r1, r6, #0
	bl	Func_08023178
	ldr	r2, [sp, #36]
	ldr	r3, [r2, #0]
	movs	r2, #1
	adds	r3, #38
	strb	r2, [r3, #0]
	adds	r0, r5, #0
	bl	Func_08002df0
	b.n	.L28
.L26:
	ldr	r0, [sp, #20]
	movs	r1, #0
	bl	Func_080b50e0
	movs	r3, #1
	negs	r3, r3
	cmp	r6, r3
	bne.n	.L29
	ldr	r0, [sp, #44]
	cmp	r0, #0
	bne.n	.L30
	bl	.L31
.L30:
	subs	r0, #1
	lsls	r1, r0, #2
	lsls	r2, r0, #4
	lsls	r3, r0, #1
	str	r0, [sp, #44]
	str	r1, [sp, #28]
	str	r2, [sp, #24]
	str	r3, [sp, #32]
	b.n	.L28
.L29:
	ldr	r5, [r5, #0]
	ldr	r3, [r5, #76]
	cmp	r3, #0
	bne.n	.L32
	movs	r6, #3
.L32:
	ldr	r3, [pc, #48]
	str	r7, [r5, #8]
	str	r3, [r5, #4]
	ldr	r0, [sp, #52]
	adds	r1, r6, #0
	bl	Func_08021c64
	ldr	r3, [pc, #28]
	ldrh	r2, [r5, #8]
	ands	r0, r3
	ldr	r3, [pc, #32]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r5, #8]
	cmp	r6, #15
	bne.n	.L33
	ldrh	r3, [r5, #6]
	ldr	r2, [pc, #24]
	ands	r2, r3
	ldr	r3, [pc, #8]
	b.n	.L34
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x00000080
	.4byte 0x80002400
	.4byte 0xfffffc00
	.4byte 0xfffffe00
.L33:
	ldrh	r3, [r5, #6]
	ldr	r2, [pc, #44]
	ands	r2, r3
	ldr	r3, [pc, #36]
.L34:
	orrs	r2, r3
	strh	r2, [r5, #6]
	movs	r3, #136
	strb	r3, [r5, #4]
	ldr	r0, [sp, #36]
	ldr	r2, [r0, #0]
	movs	r3, #1
	adds	r2, #36
	strb	r3, [r2, #0]
	cmp	r6, #16
	bls.n	.L35
	bl	.L36
.L35:
	ldr	r2, [pc, #16]
	lsls	r3, r6, #2
	ldr	r3, [r3, r2]
	b.n	.L37
	movs	r0, r0
	.4byte 0x00000060
	.4byte 0xfffffe00
	.4byte 0x080275b8
.L37:
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080275fc
	.4byte 0x08027670
	.4byte 0x08027d6a
	.4byte 0x08027f7e
	.4byte 0x08027f82
	.4byte 0x08027f82
	.4byte 0x08027f82
	.4byte 0x08027f82
	.4byte 0x08027f82
	.4byte 0x08027f82
	.4byte 0x08027f82
	.4byte 0x08027f82
	.4byte 0x08027f82
	.4byte 0x08027f82
	.4byte 0x08027f82
	.4byte 0x08027b5c
	.4byte 0x080278e4
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #17
	movs	r2, #11
	movs	r3, #3
	movs	r0, #11
	bl	Func_080162d4
	mov	fp, r0
	mov	r1, fp
	ldr	r0, [pc, #68]
	movs	r2, #16
	movs	r3, #0
	bl	Func_0801e7c0
	ldr	r2, [sp, #36]
	ldr	r1, [r2, #0]
	ldr	r3, [pc, #60]
	ldrh	r2, [r1, #6]
	ands	r3, r2
	ldr	r2, [pc, #44]
	orrs	r3, r2
	strh	r3, [r1, #6]
	movs	r0, #112
	bl	Func_080f9010
	movs	r3, #0
	ldr	r0, [sp, #64]
	movs	r1, #1
	movs	r2, #1
	bl	Func_08026080
	movs	r1, #1
	adds	r6, r0, #0
	mov	r0, fp
	bl	Func_08016418
	movs	r3, #1
	negs	r3, r3
	cmp	r6, r3
	bne.n	.L38
	b.n	.L39
.L38:
	b.n	.L40
	movs	r0, r0
	.4byte 0x00000040
	.4byte 0x0000001f
	.4byte 0xfffffe00
.L40:
	ldr	r1, [sp, #76]
	movs	r0, #0
	movs	r3, #1
	str	r0, [sp, #60]
	str	r6, [sp, #68]
	strh	r3, [r1, #12]
	bl	.L36
	movs	r0, #112
	bl	Func_080f9010
	movs	r6, #0
	ldr	r3, [pc, #68]
	ldr	r3, [r3, #0]
	str	r6, [r3, #52]
	str	r6, [r3, #48]
	str	r6, [r3, #56]
.L51:
	ldr	r2, [sp, #36]
	ldr	r1, [r2, #0]
	movs	r3, #150
	adds	r3, r3, r1
	ldrh	r2, [r1, #6]
	mov	r8, r3
	ldr	r3, [pc, #52]
	ands	r3, r2
	ldr	r2, [pc, #40]
	orrs	r3, r2
	strh	r3, [r1, #6]
	ldr	r2, [sp, #72]
	movs	r3, #88
	ldrh	r3, [r2, r3]
	ldr	r2, [pc, #40]
	movs	r0, #116
	adds	r5, r2, #0
	adds	r0, r0, r1
	ands	r5, r3
	mov	sl, r0
	movs	r4, #0
	movs	r1, #0
	cmp	r5, #0
	beq.n	.L41
	ldr	r7, [sp, #72]
	mov	r6, r8
	adds	r7, #88
	mov	r9, r2
	b.n	.L42
	.4byte 0x00000030
	.4byte 0x03001f34
	.4byte 0xfffffe00
	.4byte 0x00003fff
.L42:
	adds	r0, r5, #0
	str	r1, [sp, #16]
	str	r4, [sp, #4]
	bl	Func_08077080
	ldrb	r2, [r0, #1]
	movs	r3, #128
	ands	r3, r2
	ldr	r1, [sp, #16]
	ldr	r4, [sp, #4]
	cmp	r3, #0
	beq.n	.L43
	mov	r3, sl
	strb	r1, [r3, r4]
	strh	r5, [r6, #0]
	adds	r4, #1
	adds	r6, #2
.L43:
	adds	r1, #1
	cmp	r1, #32
	beq.n	.L41
	adds	r7, #4
	ldrh	r3, [r7, #0]
	mov	r5, r9
	ands	r5, r3
	cmp	r5, #0
	bne.n	.L42
.L41:
	movs	r3, #0
	mov	r0, sl
	strb	r3, [r0, r4]
	ldr	r3, [pc, #56]
	lsls	r2, r4, #1
	mov	r1, r8
	strh	r3, [r2, r1]
	ldr	r0, [sp, #64]
	adds	r2, r4, #0
	bl	Func_0802592c
	movs	r2, #1
	adds	r6, r0, #0
	negs	r2, r2
	cmp	r6, r2
	bne.n	.L44
	b.n	.L39
.L44:
	mov	r0, sl
	ldrb	r3, [r0, r6]
	ldr	r1, [sp, #72]
	lsls	r3, r3, #2
	adds	r3, #88
	ldrh	r3, [r1, r3]
	ldr	r2, [pc, #20]
	ands	r2, r3
	adds	r0, r2, #0
	str	r2, [sp, #56]
	bl	Func_08077080
	adds	r6, r0, #0
	ldr	r0, [sp, #36]
	b.n	.L45
	.4byte 0x00000000
	.4byte 0x00003fff
.L45:
	ldrb	r3, [r6, #8]
	ldr	r5, [r0, #0]
	movs	r0, #128
	mov	r8, r3
	bl	Func_080040b4
	ldr	r3, [pc, #92]
	ldr	r7, [r3, #0]
	movs	r3, #6
	str	r3, [sp, #0]
	mov	sl, r0
	movs	r1, #17
	movs	r2, #18
	movs	r3, #3
	movs	r0, #8
	bl	Func_080162d4
	ldr	r1, [sp, #36]
	mov	fp, r0
	ldr	r0, [r1, #0]
	ldr	r1, [pc, #68]
	ldrh	r2, [r0, #6]
	adds	r3, r1, #0
	ands	r3, r2
	ldr	r2, [pc, #48]
	orrs	r3, r2
	strh	r3, [r0, #6]
	ldr	r3, [pc, #60]
	adds	r5, #12
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #8]
	mov	r3, fp
	ldrh	r2, [r3, #12]
	ldr	r3, [pc, #32]
	lsls	r2, r2, #3
	adds	r2, #8
	ands	r2, r3
	ldrh	r3, [r5, #6]
	mov	r0, fp
	ands	r1, r3
	ldrh	r3, [r0, #14]
	lsls	r3, r3, #3
	orrs	r1, r2
	adds	r3, #4
	strh	r1, [r5, #6]
	strb	r3, [r5, #4]
	mov	r1, sl
	b.n	.L46
	movs	r0, r0
	.4byte 0x00000028
	.4byte 0x000001ff
	.4byte 0x03001e8c
	.4byte 0xfffffe00
	.4byte 0x40000400
.L46:
	ldr	r0, [sp, #56]
	bl	Func_08021b30
	ldr	r3, [pc, #52]
	ldrh	r2, [r5, #8]
	ands	r0, r3
	ldr	r3, [pc, #48]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r5, #8]
	ldr	r1, [sp, #36]
	ldr	r2, [r1, #0]
	movs	r3, #1
	adds	r2, #37
	strb	r3, [r2, #0]
	ldr	r3, [pc, #36]
	adds	r2, r7, r3
	movs	r3, #5
	strb	r3, [r2, #0]
	ldr	r1, [sp, #72]
	ldrb	r2, [r6, #9]
	movs	r0, #58
	ldrsh	r3, [r1, r0]
	cmp	r2, r3
	ble.n	.L47
	movs	r0, #2
	bl	Func_0801e71c
	b.n	.L48
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x00000ea7
.L47:
	ldr	r2, [sp, #72]
	ldr	r0, [pc, #192]
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L48
	movs	r0, #9
	bl	Func_0801e71c
.L48:
	ldr	r1, [sp, #56]
	ldr	r0, [pc, #176]
	movs	r2, #16
	adds	r0, r1, r0
	movs	r3, #0
	mov	r1, fp
	bl	Func_0801e7c0
	movs	r5, #0
	ldrb	r0, [r6, #9]
	movs	r1, #2
	mov	r2, fp
	movs	r3, #104
	str	r5, [sp, #0]
	bl	Func_0801e9d4
	ldr	r3, [pc, #152]
	adds	r2, r7, r3
	movs	r3, #15
	strb	r3, [r2, #0]
	movs	r0, #15
	bl	Func_0801e71c
	ldr	r1, [pc, #140]
	mov	r0, fp
	movs	r2, #11
	movs	r3, #0
	str	r5, [sp, #0]
	bl	Func_08019000
	movs	r3, #0
	ldr	r1, [pc, #128]
	mov	r0, fp
	movs	r2, #12
	str	r5, [sp, #0]
	bl	Func_08019000
	ldrb	r3, [r6, #2]
	cmp	r3, #4
	beq.n	.L49
	ldr	r0, [pc, #116]
	adds	r1, r3, #0
	adds	r1, r1, r0
	movs	r2, #15
	mov	r0, fp
	movs	r3, #0
	str	r5, [sp, #0]
	bl	Func_08019000
.L49:
	ldr	r2, [sp, #76]
	mov	r1, r8
	strh	r1, [r2, #12]
	movs	r0, #112
	bl	Func_080f9010
	adds	r0, r6, #0
	bl	Func_0802706c
	ldrb	r1, [r6, #0]
	adds	r3, r0, #0
	mov	r2, r8
	ldr	r0, [sp, #64]
	bl	Func_08026080
	adds	r6, r0, #0
	ldr	r0, [sp, #36]
	ldr	r3, [r0, #0]
	ldr	r5, [pc, #36]
	adds	r3, #37
	strb	r5, [r3, #0]
	mov	r0, sl
	bl	Func_08003f3c
	movs	r1, #1
	mov	r0, fp
	bl	Func_08016418
	movs	r1, #1
	negs	r1, r1
	cmp	r6, r1
	bne.n	.L50
	b.n	.L51
.L50:
	movs	r2, #1
	str	r2, [sp, #60]
.L67:
	str	r6, [sp, #68]
	b.n	.L36
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x0000013d
	.4byte 0x00000333
	.4byte 0x00000ea7
	.4byte 0x0000f01f
	.4byte 0x0000f01e
	.4byte 0x00005001
	movs	r0, #112
	bl	Func_080f9010
	ldr	r3, [pc, #76]
	ldr	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r2, #52]
	str	r3, [r2, #48]
	str	r3, [r2, #56]
.L64:
	ldr	r3, [sp, #36]
	ldr	r1, [r3, #0]
	ldr	r3, [pc, #64]
	ldrh	r2, [r1, #6]
	ands	r3, r2
	ldr	r2, [pc, #48]
	orrs	r3, r2
	strh	r3, [r1, #6]
	ldr	r0, [sp, #20]
	bl	Func_0802281c
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #28]
	ldr	r2, [r0, #0]
	adds	r2, r2, r1
	adds	r2, #84
	movs	r1, #0
	movs	r0, #0
	bl	Func_08024934
	adds	r6, r0, #0
	ldr	r0, [sp, #20]
	bl	Func_0802281c
	movs	r2, #1
	negs	r2, r2
	cmp	r6, r2
	bne.n	.L52
	b.n	.L39
.L52:
	b.n	.L53
	movs	r0, r0
	.4byte 0x00000050
	.4byte 0x03001f34
	.4byte 0xfffffe00
.L53:
	movs	r3, #6
	movs	r0, #1
	str	r3, [sp, #60]
	str	r6, [sp, #56]
	bl	Func_080030f8
	ldr	r0, [sp, #36]
	ldr	r3, [r0, #0]
	adds	r0, r6, #0
	adds	r3, #12
	mov	r8, r3
	bl	Func_080771e0
	mov	r9, r0
	ldrh	r0, [r0, #0]
	bl	Func_08077080
	mov	sl, r0
	movs	r0, #128
	bl	Func_080040b4
	ldr	r1, [sp, #60]
	str	r0, [sp, #40]
	str	r1, [sp, #0]
	movs	r2, #17
	movs	r1, #17
	movs	r3, #3
	movs	r0, #10
	bl	Func_080162d4
	ldr	r2, [sp, #36]
	ldr	r4, [sp, #28]
	ldr	r3, [r2, #0]
	mov	r1, r9
	adds	r1, #4
	adds	r4, #84
	ldrb	r2, [r1, #0]
	ldrb	r3, [r3, r4]
	mov	fp, r0
	movs	r7, #0
	cmp	r2, r3
	bhi.n	.L54
	ldr	r5, [sp, #36]
	adds	r0, r1, #0
	adds	r1, r4, #0
.L55:
	adds	r7, #1
	adds	r1, #1
	cmp	r7, #3
	bgt.n	.L54
	ldr	r3, [r5, #0]
	adds	r0, #1
	ldrb	r2, [r0, #0]
	ldrb	r3, [r3, r1]
	cmp	r2, r3
	bls.n	.L55
.L54:
	movs	r3, #4
	eors	r3, r7
	negs	r2, r3
	orrs	r2, r3
	ldr	r3, [sp, #36]
	ldr	r0, [r3, #0]
	ldr	r1, [pc, #68]
	lsrs	r4, r2, #31
	ldrh	r2, [r0, #6]
	adds	r3, r1, #0
	ands	r3, r2
	ldr	r2, [pc, #48]
	orrs	r3, r2
	strh	r3, [r0, #6]
	ldr	r3, [pc, #56]
	mov	r0, r8
	movs	r5, #1
	str	r3, [r0, #4]
	movs	r3, #0
	subs	r4, r5, r4
	str	r3, [r0, #8]
	mov	r3, fp
	ldrh	r2, [r3, #12]
	ldr	r3, [pc, #28]
	lsls	r2, r2, #3
	adds	r2, #8
	ands	r2, r3
	ldrh	r3, [r0, #6]
	ands	r1, r3
	orrs	r1, r2
	strh	r1, [r0, #6]
	mov	r1, fp
	ldrh	r3, [r1, #14]
	lsls	r3, r3, #3
	mov	r2, r9
	b.n	.L56
	movs	r0, r0
	.4byte 0x00000038
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x40000400
.L56:
	adds	r3, #4
	strb	r3, [r0, #4]
	ldrh	r3, [r2, #0]
	ldr	r0, [pc, #68]
	ldr	r1, [sp, #40]
	ands	r0, r3
	str	r4, [sp, #4]
	bl	Func_08021b30
	ldr	r3, [pc, #52]
	ands	r0, r3
	mov	r3, r8
	ldrh	r2, [r3, #8]
	ldr	r3, [pc, #52]
	ands	r3, r2
	orrs	r3, r0
	mov	r0, r8
	strh	r3, [r0, #8]
	ldr	r1, [sp, #36]
	ldr	r3, [r1, #0]
	ldr	r4, [sp, #4]
	adds	r3, #37
	strb	r5, [r3, #0]
	cmp	r4, #0
	bne.n	.L57
	movs	r0, #2
	bl	Func_0801e71c
	ldr	r4, [sp, #4]
.L57:
	adds	r0, r6, #0
	str	r4, [sp, #4]
	bl	Func_080771e0
	ldr	r3, [pc, #16]
	ldrh	r0, [r0, #0]
	b.n	.L58
	.4byte 0x000003ff
	.4byte 0x00003fff
	.4byte 0xfffffc00
	.4byte 0x00000333
.L58:
	movs	r2, #16
	adds	r0, r0, r3
	mov	r1, fp
	movs	r3, #0
	bl	Func_0801e7c0
	movs	r2, #0
	lsls	r3, r2, #1
	mov	r6, r9
	adds	r5, r3, #0
	ldr	r4, [sp, #4]
	movs	r7, #0
	mov	r8, r2
	adds	r6, #4
	adds	r5, #13
.L60:
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L59
	ldr	r3, [pc, #60]
	mov	r0, r8
	adds	r1, r7, r3
	str	r0, [sp, #0]
	adds	r2, r5, #0
	mov	r0, fp
	movs	r3, #0
	str	r4, [sp, #4]
	bl	Func_08019000
	ldrb	r1, [r6, #0]
	mov	r3, r8
	adds	r2, r5, #1
	str	r3, [sp, #0]
	adds	r1, #48
	mov	r0, fp
	movs	r3, #0
	bl	Func_08018efc
	ldr	r4, [sp, #4]
	adds	r5, #2
.L59:
	adds	r7, #1
	adds	r6, #1
	cmp	r7, #3
	ble.n	.L60
	cmp	r4, #0
	beq.n	.L61
	movs	r0, #112
	bl	Func_080f9010
	b.n	.L62
	movs	r0, r0
	.4byte 0x00005001
.L61:
	movs	r0, #114
	bl	Func_080f9010
.L62:
	mov	r0, sl
	bl	Func_0802706c
	adds	r3, r0, #0
	mov	r0, sl
	ldrb	r1, [r0, #0]
	ldrb	r2, [r0, #8]
	ldr	r0, [sp, #64]
	bl	Func_08026080
	mov	r1, sl
	ldrb	r3, [r1, #8]
	adds	r6, r0, #0
	ldr	r0, [sp, #76]
	strh	r3, [r0, #12]
	ldr	r1, [sp, #36]
	ldr	r3, [r1, #0]
	ldr	r2, [pc, #28]
	adds	r3, #37
	strb	r2, [r3, #0]
	ldr	r0, [sp, #40]
	bl	Func_08003f3c
	mov	r0, fp
	movs	r1, #1
	bl	Func_08016418
	movs	r2, #1
	negs	r2, r2
	cmp	r6, r2
	bne.n	.L63
	b.n	.L64
.L63:
	b.n	.L65
	.4byte 0x00000000
.L65:
	ldr	r3, [sp, #36]
	ldr	r2, [sp, #28]
	movs	r0, #0
	mov	lr, r0
	adds	r2, #84
	mov	r0, r9
	ldr	r1, [r3, #0]
	adds	r0, #4
	adds	r5, r2, #0
	mov	ip, r3
	ldrb	r4, [r0, #0]
	ldrb	r3, [r1, r5]
	movs	r7, #0
	cmp	r4, r3
	bls.n	.L66
	mov	r2, lr
	strb	r2, [r1, r5]
	b.n	.L67
.L66:
	subs	r3, r3, r4
	adds	r7, #1
	strb	r3, [r1, r5]
	adds	r0, #1
	adds	r2, #1
	cmp	r7, #3
	ble.n	.L68
	b.n	.L67
.L68:
	mov	r3, ip
	ldr	r1, [r3, #0]
	adds	r5, r2, #0
	ldrb	r4, [r0, #0]
	ldrb	r3, [r1, r5]
	cmp	r4, r3
	bls.n	.L66
	mov	r0, lr
	strb	r0, [r1, r5]
	b.n	.L67
	movs	r0, #112
	bl	Func_080f9010
	ldr	r3, [pc, #84]
	ldr	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r2, #52]
	str	r3, [r2, #48]
	str	r3, [r2, #56]
.L77:
	ldr	r2, [sp, #36]
	ldr	r0, [pc, #72]
	ldr	r1, [r2, #0]
	mov	sl, r0
	ldrh	r3, [r1, #6]
	mov	r2, sl
	ands	r2, r3
	ldr	r3, [pc, #52]
	orrs	r2, r3
	strh	r2, [r1, #6]
	ldr	r7, [pc, #52]
	ldr	r5, [sp, #28]
	ldr	r2, [r7, #0]
	movs	r3, #128
	lsls	r3, r3, #24
	adds	r5, #228
	str	r3, [r2, r5]
	movs	r1, #1
	ldr	r0, [sp, #64]
	bl	Func_08023e70
	movs	r1, #0
	adds	r6, r0, #0
	movs	r0, #1
	mov	r8, r1
	ldr	r3, [sp, #76]
	negs	r0, r0
	mov	r2, r8
	mov	r9, r0
	strh	r2, [r3, #12]
	cmp	r6, r9
	bne.n	.L69
	bl	.L39
.L69:
	b.n	.L70
	.4byte 0x00000090
	.4byte 0x03001f34
	.4byte 0xfffffe00
.L70:
	movs	r1, #5
	str	r1, [sp, #60]
	str	r6, [sp, #56]
	ldr	r3, [r7, #0]
	str	r6, [r3, r5]
	ldr	r2, [sp, #56]
	movs	r3, #15
	asrs	r7, r2, #8
	movs	r4, #255
	ands	r4, r2
	ands	r7, r3
	adds	r2, r4, #0
	ldr	r0, [sp, #64]
	adds	r1, r7, #0
	str	r4, [sp, #4]
	bl	Func_08077208
	adds	r5, r0, #0
	ldr	r4, [sp, #4]
	cmp	r5, #0
	beq.n	.L71
	adds	r1, r4, #0
	adds	r0, r7, #0
	bl	Func_080771e8
	bl	Func_08077080
	movs	r3, #6
	adds	r5, r0, #0
	ldrb	r6, [r5, #8]
	movs	r1, #17
	str	r3, [sp, #0]
	movs	r2, #10
	movs	r3, #3
	movs	r0, #11
	bl	Func_080162d4
	ldr	r3, [sp, #36]
	ldr	r1, [r3, #0]
	ldrh	r2, [r1, #6]
	mov	r3, sl
	ands	r3, r2
	ldr	r2, [pc, #60]
	mov	fp, r0
	ldr	r0, [pc, #60]
	orrs	r3, r2
	strh	r3, [r1, #6]
	mov	r2, r8
	adds	r1, r7, r0
	movs	r3, #0
	mov	r0, fp
	str	r2, [sp, #0]
	bl	Func_08019000
	lsls	r0, r7, #2
	ldr	r4, [sp, #4]
	adds	r0, r0, r7
	ldr	r3, [pc, #40]
	lsls	r0, r0, #2
	adds	r0, r0, r4
	mov	r1, fp
	movs	r2, #16
	adds	r0, r0, r3
	movs	r3, #0
	bl	Func_0801e7c0
	ldr	r3, [sp, #76]
	movs	r0, #1
	strh	r6, [r3, #12]
	bl	Func_080030f8
	movs	r0, #112
	b.n	.L72
	movs	r0, r0
	.4byte 0x00000040
	.4byte 0x00005001
	.4byte 0x0000045f
.L72:
	bl	Func_080f9010
	adds	r0, r5, #0
	bl	Func_0802706c
	adds	r2, r6, #0
	adds	r3, r0, #0
	ldrb	r1, [r5, #0]
	ldr	r0, [sp, #64]
	bl	Func_08026080
	adds	r6, r0, #0
	ldr	r0, [sp, #36]
	ldr	r3, [r0, #0]
	adds	r3, #216
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L73
	ldr	r0, [pc, #72]
	movs	r1, #15
	movs	r2, #8
	bl	Func_08021e48
	adds	r5, r0, #0
	b.n	.L74
.L75:
	movs	r0, #1
	bl	Func_080030f8
.L74:
	bl	Func_08017364
	cmp	r0, #0
	beq.n	.L75
	movs	r1, #1
	adds	r0, r5, #0
	bl	Func_08016418
	ldr	r2, [sp, #36]
	ldr	r1, [r2, #0]
	adds	r2, r1, #0
	adds	r2, #216
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	adds	r1, #220
	movs	r3, #45
	str	r3, [r1, #0]
.L73:
	mov	r0, fp
	movs	r1, #1
	bl	Func_08016418
	movs	r3, #1
	negs	r3, r3
	cmp	r6, r3
	bne.n	.L76
	b.n	.L77
.L76:
	b.n	.L67
	.4byte 0x00000c4e
.L71:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #17
	movs	r2, #10
	movs	r3, #3
	movs	r0, #11
	str	r4, [sp, #4]
	bl	Func_080162d4
	mov	fp, r0
	ldr	r0, [sp, #36]
	ldr	r1, [r0, #0]
	ldrh	r2, [r1, #6]
	mov	r3, sl
	ands	r3, r2
	ldr	r2, [pc, #60]
	orrs	r3, r2
	strh	r3, [r1, #6]
	movs	r0, #2
	bl	Func_0801e71c
	ldr	r2, [pc, #52]
	mov	r0, fp
	adds	r1, r7, r2
	movs	r3, #0
	movs	r2, #0
	str	r5, [sp, #0]
	bl	Func_08019000
	lsls	r0, r7, #2
	ldr	r4, [sp, #4]
	adds	r0, r0, r7
	ldr	r3, [pc, #32]
	lsls	r0, r0, #2
	adds	r0, r0, r4
	mov	r1, fp
	movs	r2, #16
	adds	r0, r0, r3
	movs	r3, #0
	bl	Func_0801e7c0
	movs	r0, #15
	bl	Func_0801e71c
	ldr	r0, [sp, #76]
	b.n	.L78
	.4byte 0x00000040
	.4byte 0x00005001
	.4byte 0x0000045f
.L78:
	movs	r3, #1
	strh	r3, [r0, #12]
	movs	r0, #1
	bl	Func_080030f8
	movs	r0, #112
	bl	Func_080f9010
	movs	r1, #4
	ldr	r0, [sp, #64]
	movs	r2, #0
	movs	r3, #7
	bl	Func_08026080
	movs	r1, #1
	adds	r6, r0, #0
	mov	r0, fp
	bl	Func_08016418
	cmp	r6, r9
	bne.n	.L79
	b.n	.L77
.L79:
	b.n	.L67
	movs	r0, #112
	bl	Func_080f9010
	movs	r6, #0
	ldr	r3, [pc, #64]
	ldr	r3, [r3, #0]
	str	r6, [r3, #52]
	str	r6, [r3, #48]
	str	r6, [r3, #56]
.L91:
	ldr	r2, [sp, #36]
	ldr	r1, [r2, #0]
	ldr	r3, [pc, #52]
	ldrh	r2, [r1, #6]
	ands	r3, r2
	ldr	r2, [pc, #40]
	orrs	r3, r2
	strh	r3, [r1, #6]
	movs	r3, #116
	adds	r3, r3, r1
	adds	r1, #150
	mov	sl, r1
	ldr	r1, [sp, #72]
	mov	r9, r3
	movs	r3, #216
	ldrh	r5, [r1, r3]
	movs	r0, #0
	mov	r8, r0
	movs	r4, #0
	cmp	r5, #0
	beq.n	.L80
	adds	r3, r1, #0
	adds	r3, #216
	mov	r7, r9
	mov	r6, sl
	b.n	.L81
	.4byte 0x00000060
	.4byte 0x03001f34
	.4byte 0xfffffe00
.L81:
	adds	r0, r5, #0
	str	r3, [sp, #8]
	str	r4, [sp, #4]
	bl	Func_08077018
	adds	r1, r5, #0
	ldr	r0, [sp, #64]
	bl	Func_08025180
	ldr	r3, [sp, #8]
	ldr	r4, [sp, #4]
	cmp	r0, #0
	bne.n	.L82
	movs	r2, #1
	strh	r5, [r6, #0]
	add	r8, r2
	strb	r4, [r7, #0]
	adds	r6, #2
	adds	r7, #1
.L82:
	adds	r4, #1
	cmp	r4, #15
	beq.n	.L80
	adds	r3, #2
	ldrh	r5, [r3, #0]
	cmp	r5, #0
	bne.n	.L81
.L80:
	ldr	r0, [sp, #72]
	movs	r3, #216
	ldrh	r5, [r0, r3]
	movs	r4, #0
	cmp	r5, #0
	beq.n	.L83
	mov	r1, r8
	adds	r2, r0, #0
	mov	r7, r8
	lsls	r3, r1, #1
	mov	r0, sl
	adds	r2, #216
	add	r7, r9
	adds	r6, r3, r0
.L85:
	adds	r0, r5, #0
	str	r2, [sp, #12]
	str	r4, [sp, #4]
	bl	Func_08077018
	adds	r1, r5, #0
	ldr	r0, [sp, #64]
	bl	Func_08025180
	ldr	r2, [sp, #12]
	ldr	r4, [sp, #4]
	cmp	r0, #0
	beq.n	.L84
	movs	r1, #1
	strh	r5, [r6, #0]
	add	r8, r1
	strb	r4, [r7, #0]
	adds	r6, #2
	adds	r7, #1
.L84:
	adds	r4, #1
	cmp	r4, #15
	beq.n	.L83
	adds	r2, #2
	ldrh	r5, [r2, #0]
	cmp	r5, #0
	bne.n	.L85
.L83:
	mov	r2, r8
	ldr	r1, [pc, #32]
	lsls	r3, r2, #1
	mov	r0, sl
	strh	r1, [r3, r0]
	ldr	r0, [sp, #64]
	mov	r1, sl
	bl	Func_08025200
	movs	r7, #1
	adds	r6, r0, #0
	negs	r7, r7
	cmp	r6, r7
	bne.n	.L86
	bl	.L39
.L86:
	b.n	.L87
	movs	r0, r0
	.4byte 0x00000000
.L87:
	mov	r2, r9
	ldrb	r6, [r2, r6]
	ldr	r3, [sp, #72]
	str	r6, [sp, #56]
	lsls	r6, r6, #1
	adds	r6, #216
	ldrh	r0, [r3, r6]
	bl	Func_08077018
	ldrh	r0, [r0, #40]
	bl	Func_08077080
	mov	r8, r0
	ldrb	r0, [r0, #8]
	ldr	r1, [sp, #36]
	mov	sl, r0
	movs	r0, #128
	ldr	r5, [r1, #0]
	bl	Func_080040b4
	movs	r3, #6
	str	r3, [sp, #0]
	mov	r9, r0
	movs	r1, #17
	movs	r2, #15
	movs	r3, #3
	movs	r0, #9
	bl	Func_080162d4
	ldr	r2, [sp, #36]
	mov	fp, r0
	ldr	r0, [r2, #0]
	ldr	r1, [pc, #60]
	ldrh	r2, [r0, #6]
	adds	r3, r1, #0
	ands	r3, r2
	ldr	r2, [pc, #44]
	orrs	r3, r2
	strh	r3, [r0, #6]
	ldr	r3, [pc, #52]
	adds	r5, #12
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #8]
	mov	r0, fp
	ldrh	r3, [r0, #12]
	ldr	r4, [pc, #28]
	ldrh	r2, [r5, #6]
	lsls	r3, r3, #3
	adds	r3, #8
	ands	r3, r4
	ands	r1, r2
	orrs	r1, r3
	ldrh	r3, [r0, #14]
	lsls	r3, r3, #3
	adds	r3, #4
	strb	r3, [r5, #4]
	strh	r1, [r5, #6]
	ldr	r1, [sp, #72]
	b.n	.L88
	.4byte 0x00000030
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x40000400
.L88:
	ldrh	r0, [r1, r6]
	mov	r1, r9
	str	r4, [sp, #4]
	bl	Func_08021af0
	ldr	r3, [pc, #56]
	ldrh	r2, [r5, #8]
	ands	r0, r3
	ldr	r3, [pc, #52]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r5, #8]
	ldr	r3, [sp, #36]
	ldr	r2, [r3, #0]
	movs	r3, #1
	adds	r2, #37
	strb	r3, [r2, #0]
	ldr	r1, [sp, #72]
	ldr	r4, [sp, #4]
	ldrh	r0, [r1, r6]
	ldr	r3, [pc, #32]
	ands	r0, r4
	mov	r1, fp
	adds	r0, r0, r3
	movs	r2, #24
	movs	r3, #0
	bl	Func_0801e7c0
	ldr	r3, [sp, #76]
	mov	r2, sl
	strh	r2, [r3, #12]
	movs	r0, #112
	b.n	.L89
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x00000182
.L89:
	bl	Func_080f9010
	mov	r0, r8
	bl	Func_0802706c
	adds	r3, r0, #0
	mov	r0, r8
	ldrb	r1, [r0, #0]
	mov	r2, sl
	ldr	r0, [sp, #64]
	bl	Func_08026080
	ldr	r1, [sp, #36]
	ldr	r3, [r1, #0]
	movs	r2, #0
	adds	r3, #37
	adds	r6, r0, #0
	strb	r2, [r3, #0]
	mov	r0, r9
	bl	Func_08003f3c
	mov	r0, fp
	movs	r1, #1
	bl	Func_08016418
	cmp	r6, r7
	bne.n	.L90
	b.n	.L91
.L90:
	movs	r3, #2
	str	r3, [sp, #60]
	b.n	.L67
	movs	r0, #3
	str	r0, [sp, #60]
.L36:
	movs	r0, #110
	bl	Func_080f9010
	add	r1, sp, #64
	ldrh	r1, [r1, #0]
	ldr	r2, [sp, #76]
	strh	r1, [r2, #0]
	ldr	r5, [sp, #72]
	adds	r5, #64
	ldrh	r6, [r5, #0]
	cmp	r6, #0
	beq.n	.L92
	bl	Func_08004458
	ldrh	r3, [r5, #0]
	muls	r3, r0
	lsrs	r3, r3, #20
	adds	r6, r6, r3
.L92:
	ldr	r2, [sp, #76]
	strh	r6, [r2, #4]
	ldr	r3, [sp, #44]
	cmp	r3, #0
	beq.n	.L93
	ldr	r1, [sp, #84]
	ldr	r0, [sp, #32]
	adds	r3, r0, r1
	subs	r2, r3, #2
	ldrh	r1, [r3, #0]
	ldrh	r3, [r2, #0]
	cmp	r1, r3
	bne.n	.L93
	lsls	r2, r6, #16
	asrs	r3, r2, #16
	lsrs	r2, r2, #31
	adds	r3, r3, r2
	ldr	r2, [sp, #76]
	asrs	r3, r3, #1
	strh	r3, [r2, #4]
.L93:
	ldr	r1, [sp, #76]
	movs	r0, #4
	ldrsh	r3, [r1, r0]
	cmp	r3, #0
	bge.n	.L94
	movs	r3, #250
	lsls	r3, r3, #3
	adds	r2, r1, #0
	strh	r3, [r2, #4]
.L94:
	add	r3, sp, #60
	ldrh	r3, [r3, #0]
	ldr	r0, [sp, #76]
	strh	r3, [r0, #6]
	add	r0, sp, #56
	ldrh	r0, [r0, #0]
	ldr	r1, [sp, #76]
	strh	r0, [r1, #8]
	add	r1, sp, #68
	ldrh	r1, [r1, #0]
	ldr	r2, [sp, #76]
	strh	r1, [r2, #10]
	ldr	r2, [sp, #44]
	adds	r2, #1
	lsls	r3, r2, #2
	str	r3, [sp, #28]
	ldr	r3, [sp, #80]
	lsls	r0, r2, #4
	lsls	r1, r2, #1
	str	r2, [sp, #44]
	str	r0, [sp, #24]
	str	r1, [sp, #32]
	cmp	r2, r3
	bge.n	.L31
	bl	.L28
.L31:
	ldr	r0, [sp, #44]
	ldr	r1, [sp, #80]
	cmp	r0, r1
	bge.n	.L19
	bl	.L17
.L19:
	ldr	r2, [sp, #36]
	ldr	r0, [r2, #0]
	ldr	r3, [r0, #80]
	cmp	r3, #0
	beq.n	.L95
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #8]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #10]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #12]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #14]
.L95:
	ldr	r0, [r0, #68]
	cmp	r0, #0
	beq.n	.L96
	movs	r1, #1
	bl	Func_08016418
.L96:
	ldr	r0, [sp, #48]
	bl	Func_08003f3c
	ldr	r0, [sp, #52]
	bl	Func_08003f3c
	ldr	r0, [pc, #28]
	bl	Func_08004278
	ldr	r3, [sp, #36]
	ldr	r2, [r3, #0]
	b.n	.L97
	.4byte 0x00000045
	.4byte 0x00000044
	.4byte 0x00000056
	.4byte 0x00000053
	.4byte 0x02002224
	.4byte 0x08026e81
.L97:
	ldr	r3, [r2, #80]
	cmp	r3, #0
	beq.n	.L98
	ldr	r3, [pc, #264]
	ldr	r5, [r3, #0]
	ldr	r3, [r2, #68]
	movs	r6, #0
	cmp	r3, #0
	bne.n	.L99
	adds	r7, r5, #0
	adds	r7, #82
	ldrb	r3, [r7, #0]
	cmp	r3, #0
	bne.n	.L100
	movs	r3, #42
	str	r3, [sp, #0]
	movs	r2, #30
	movs	r1, #16
	movs	r3, #4
	movs	r0, #0
	bl	Func_080162d4
	ldr	r1, [sp, #36]
	ldr	r3, [r1, #0]
	str	r0, [r3, #68]
	bl	Func_08016738
	add	r2, sp, #100
	mov	r9, r2
	bl	Func_080270d8
	b.n	.L101
.L99:
	adds	r7, r5, #0
	adds	r7, #82
.L100:
	ldr	r3, [sp, #36]
	ldr	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r2, #68]
.L101:
	adds	r5, #80
	ldrb	r2, [r5, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r3, [pc, #184]
	lsls	r2, r2, #3
	adds	r1, r2, r3
	ldrb	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L102
	movs	r0, #1
	negs	r0, r0
	str	r0, [sp, #80]
	b.n	.L103
.L102:
	ldr	r3, [pc, #168]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L104
	adds	r6, #1
	cmp	r6, #24
	ble.n	.L105
	movs	r1, #1
	negs	r1, r1
	str	r1, [sp, #80]
	b.n	.L103
.L104:
	ldrh	r2, [r1, #8]
	adds	r3, r2, #0
	movs	r6, #0
	cmp	r3, #69
	bne.n	.L106
	ldrh	r3, [r1, #10]
	cmp	r3, #68
	bne.n	.L106
	ldrh	r3, [r1, #12]
	cmp	r3, #86
	bne.n	.L106
	ldrh	r3, [r1, #14]
	cmp	r3, #83
	beq.n	.L103
.L106:
	adds	r3, r2, #0
	cmp	r3, #86
	bne.n	.L107
	ldrh	r3, [r1, #10]
	cmp	r3, #83
	bne.n	.L107
	ldrh	r3, [r1, #12]
	cmp	r3, #83
	bne.n	.L107
	ldrh	r3, [r1, #14]
	cmp	r3, #84
	beq.n	.L105
.L107:
	movs	r6, #1
.L105:
	movs	r0, #1
	bl	Func_080030f8
	ldrb	r2, [r5, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r3, [pc, #68]
	lsls	r2, r2, #3
	adds	r1, r2, r3
	ldrb	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L102
	movs	r2, #1
	negs	r2, r2
	str	r2, [sp, #80]
.L103:
	ldr	r0, [sp, #36]
	ldr	r3, [r0, #0]
	ldr	r0, [r3, #68]
	cmp	r0, #0
	beq.n	.L98
	movs	r1, #1
	bl	Func_08016418
.L98:
	movs	r0, #0
	bl	Func_080b50d0
	movs	r0, #57
	bl	Func_08002dd8
	ldr	r0, [sp, #80]
	add	sp, #100
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e74
	.4byte 0x02002024
	.4byte 0x03001f64
