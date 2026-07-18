@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080d89ac
	.thumb_func
Func_080d89ac:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #176]
	adds	r3, r2, #0
	adds	r6, r0, #0
	ldmia	r3!, {r0}
	sub	sp, #128
	str	r0, [sp, #72]
	ldr	r3, [r3, #0]
	str	r3, [sp, #68]
	ldr	r2, [r2, #8]
	str	r2, [sp, #56]
	ldr	r2, [pc, #160]
	mov	sl, r1
	adds	r5, r0, r2
	movs	r1, #0
	str	r1, [sp, #48]
	movs	r0, #0
	str	r6, [r5, #0]
	bl	Func_080cd594
	ldr	r5, [r5, #0]
	ldr	r3, [r5, #28]
	cmp	r3, #1
	bne.n	.L0
	movs	r3, #6
	mov	r0, sl
	eors	r3, r0
	negs	r2, r3
	orrs	r2, r3
	lsrs	r1, r2, #31
	movs	r3, #2
	subs	r1, r3, r1
	cmp	r0, #6
	beq.n	.L1
	cmp	r0, #0
	bne.n	.L2
.L1:
	add	r3, sp, #88
	ldr	r2, [r5, #4]
	str	r3, [sp, #0]
	add	r3, sp, #84
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r3, #0
	bl	Func_080de2f8
	b.n	.L3
.L2:
	add	r3, sp, #88
	ldr	r2, [r5, #4]
	str	r3, [sp, #0]
	add	r3, sp, #84
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r3, #1
	bl	Func_080de2f8
.L3:
	ldr	r1, [sp, #72]
	ldr	r2, [pc, #68]
	adds	r3, r1, r2
	ldr	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r2, #24]
.L0:
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L4
	ldr	r0, [sp, #72]
	ldr	r1, [pc, #52]
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	add	r5, sp, #116
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	adds	r1, r5, #0
	bl	Func_080e396c
	ldr	r2, [r5, #0]
	movs	r3, #64
	subs	r3, r3, r2
	ldr	r2, [pc, #32]
	str	r3, [sp, #48]
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	ldr	r3, [pc, #12]
	subs	r2, #8
	strh	r3, [r2, #0]
	movs	r3, #0
	str	r3, [sp, #44]
	b.n	.L5
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000028
.L4:
	movs	r0, #1
	str	r0, [sp, #44]
.L5:
	ldr	r1, [sp, #56]
	ldr	r0, [pc, #836]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	ldr	r0, [pc, #828]
	ldr	r1, [sp, #72]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	mov	r1, sl
	cmp	r1, #1
	bls.n	.L6
	cmp	r1, #3
	beq.n	.L6
	cmp	r1, #4
	beq.n	.L6
	cmp	r1, #5
	bne.n	.L7
.L6:
	ldr	r2, [sp, #72]
	ldr	r0, [pc, #800]
	adds	r3, r2, r0
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #0
	bne.n	.L8
	ldr	r0, [pc, #792]
	b.n	.L9
.L8:
	ldr	r0, [pc, #792]
	b.n	.L9
.L7:
	mov	r1, sl
	cmp	r1, #6
	bne.n	.L10
	ldr	r0, [pc, #788]
	b.n	.L9
.L10:
	ldr	r0, [pc, #788]
.L9:
	bl	Func_08002f40
	adds	r2, r0, #0
	movs	r0, #160
	adds	r1, r2, #0
	ldr	r3, [pc, #780]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	Func_080072f0
	ldr	r2, [sp, #44]
	cmp	r2, #0
	bne.n	.L11
	mov	r3, sl
	cmp	r3, #6
	bne.n	.L12
	ldr	r0, [pc, #748]
	b.n	.L13
.L12:
	ldr	r0, [pc, #756]
	b.n	.L13
.L11:
	mov	r0, sl
	cmp	r0, #6
	bne.n	.L14
	ldr	r0, [pc, #752]
	b.n	.L13
.L14:
	ldr	r0, [pc, #752]
.L13:
	bl	Func_08002f40
	adds	r2, r0, #0
	ldr	r3, [sp, #72]
	movs	r0, #128
	adds	r2, #128
	lsls	r0, r0, #5
	adds	r1, r3, r0
	adds	r0, r2, #0
	bl	Func_08005340
	ldr	r2, [pc, #696]
	ldr	r1, [sp, #72]
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	ldr	r0, [r3, #4]
	mov	r3, sp
	adds	r3, #76
	adds	r1, r3, #0
	str	r3, [sp, #40]
	bl	Func_080cef64
	mov	r0, sl
	cmp	r0, #0
	beq.n	.L15
	cmp	r0, #6
	bne.n	.L16
.L15:
	movs	r1, #0
	ldr	r5, [pc, #696]
	mov	r8, r1
	movs	r6, #255
.L17:
	bl	Func_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	Func_08004458
	movs	r3, #127
	ands	r3, r0
	adds	r3, #64
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	bl	Func_08004458
	movs	r3, #0
	ands	r0, r6
	str	r3, [r5, #24]
	subs	r0, #127
	movs	r2, #1
	movs	r3, #128
	lsls	r0, r0, #15
	add	r8, r2
	lsls	r3, r3, #2
	str	r0, [r5, #8]
	adds	r5, #28
	cmp	r8, r3
	bne.n	.L17
	ldr	r0, [sp, #72]
	ldr	r1, [pc, #596]
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #3
	adds	r3, #88
	b.n	.L18
.L16:
	mov	r2, sl
	cmp	r2, #1
	bne.n	.L19
	movs	r3, #0
	ldr	r5, [pc, #612]
	mov	r8, r3
	movs	r6, #255
.L20:
	bl	Func_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	Func_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #4]
	bl	Func_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #8]
	movs	r1, #128
	movs	r0, #1
	movs	r3, #0
	add	r8, r0
	lsls	r1, r1, #2
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r8, r1
	bne.n	.L20
	ldr	r2, [sp, #72]
	ldr	r0, [pc, #512]
	adds	r3, r2, r0
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #3
	adds	r3, #88
	b.n	.L18
.L19:
	mov	r1, sl
	cmp	r1, #2
	bne.n	.L21
	movs	r2, #0
	ldr	r7, [pc, #528]
	mov	r8, r2
.L22:
	bl	Func_08004458
	ldr	r3, [pc, #524]
	adds	r6, r0, #0
	ands	r6, r3
	bl	Func_08004458
	movs	r5, #63
	ands	r5, r0
	adds	r0, r6, #0
	bl	Func_08002322
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	str	r3, [r7, #0]
	ldr	r3, [pc, #500]
	adds	r0, r6, #0
	str	r3, [r7, #4]
	bl	Func_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	str	r3, [r7, #8]
	bl	Func_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #13
	str	r3, [r7, #16]
	movs	r3, #0
	str	r3, [r7, #24]
	movs	r0, #128
	movs	r3, #1
	add	r8, r3
	lsls	r0, r0, #2
	adds	r7, #28
	cmp	r8, r0
	bne.n	.L22
	ldr	r1, [sp, #72]
	ldr	r2, [pc, #404]
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #3
	adds	r3, #88
	b.n	.L18
.L21:
	mov	r3, sl
	cmp	r3, #3
	bne.n	.L23
	movs	r0, #0
	ldr	r5, [pc, #420]
	mov	r8, r0
	movs	r6, #255
.L24:
	bl	Func_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	Func_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #14
	str	r0, [r5, #4]
	bl	Func_08004458
	ands	r0, r6
	subs	r0, #127
	movs	r1, #1
	movs	r2, #128
	lsls	r0, r0, #15
	movs	r3, #0
	add	r8, r1
	lsls	r2, r2, #2
	str	r0, [r5, #8]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r8, r2
	bne.n	.L24
	ldr	r0, [sp, #72]
	ldr	r1, [pc, #320]
	adds	r3, r0, r1
	b.n	.L25
.L23:
	movs	r2, #0
	ldr	r5, [pc, #348]
	mov	r8, r2
	movs	r6, #255
.L26:
	bl	Func_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	Func_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #4]
	bl	Func_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	movs	r3, #0
	str	r0, [r5, #8]
	str	r3, [r5, #24]
	movs	r0, #128
	movs	r3, #1
	add	r8, r3
	lsls	r0, r0, #2
	adds	r5, #28
	cmp	r8, r0
	bne.n	.L26
	ldr	r1, [sp, #72]
	ldr	r2, [pc, #252]
	adds	r3, r1, r2
.L25:
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #3
	adds	r3, #72
.L18:
	str	r3, [sp, #64]
	movs	r3, #64
	ldr	r0, [sp, #72]
	ldr	r1, [pc, #232]
	str	r3, [sp, #52]
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #0
	bne.n	.L27
	movs	r2, #32
	str	r2, [sp, #52]
	b.n	.L28
.L27:
	cmp	r3, #2
	bne.n	.L28
	movs	r3, #128
	str	r3, [sp, #52]
.L28:
	ldr	r0, [sp, #72]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r0, r1
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r3, [pc, #240]
	movs	r1, #144
	adds	r2, r0, r3
	movs	r3, #75
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	ldr	r0, [pc, #232]
	bl	Func_080041d8
	ldr	r1, [sp, #64]
	movs	r0, #0
	mov	fp, r0
	cmp	r1, #0
	bne.n	.L29
	b.n	.L30
.L29:
	str	r0, [sp, #12]
.L67:
	ldr	r3, [pc, #216]
	ldr	r3, [r3, #0]
	mov	r2, fp
	str	r3, [sp, #36]
	cmp	r2, #40
	bne.n	.L31
	movs	r0, #0
	bl	Func_080b50e8
.L31:
	ldr	r0, [sp, #72]
	ldr	r1, [pc, #140]
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #28]
	cmp	r3, #1
	beq.n	.L32
	b.n	.L33
.L32:
	ldr	r2, [sp, #44]
	cmp	r2, #0
	bne.n	.L34
	ldr	r0, [sp, #12]
	bl	Func_08002322
	lsls	r3, r0, #2
	adds	r3, r3, r0
	ldr	r2, [sp, #88]
	lsls	r3, r3, #2
	ldr	r0, [sp, #48]
	asrs	r3, r3, #16
	adds	r3, r3, r2
	adds	r3, r3, r0
	subs	r3, #20
	ldr	r0, [sp, #12]
	mov	r8, r3
	bl	Func_0800231c
	lsls	r0, r0, #2
	ldr	r3, [sp, #84]
	asrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r5, r0, #0
	mov	r1, fp
	subs	r5, #24
	cmp	r1, #32
	ble.n	.L35
	lsls	r3, r1, #1
	subs	r3, r5, r3
	adds	r5, r3, #0
	adds	r5, #64
.L35:
	ldr	r2, [sp, #72]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r7, r2, r3
	movs	r6, #40
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	ldr	r4, [sp, #76]
	ldr	r0, [sp, #68]
	adds	r1, r7, #0
	mov	r2, r8
	adds	r3, r5, #0
	bl	Func_080072f4
	mov	r0, fp
	cmp	r0, #3
	bgt.n	.L33
	ldr	r1, [sp, #40]
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [sp, #68]
	ldr	r4, [r1, #4]
	mov	r2, r8
	adds	r1, r7, #0
	adds	r3, r5, #0
	bl	Func_080072f4
	b.n	.L33
	movs	r0, r0
	.4byte 0x00000073
	.4byte 0x000000ba
	.4byte 0x00007828
	.4byte 0x000000b3
	.4byte 0x000000b9
	.4byte 0x0000008d
	.4byte 0x000000c0
	.4byte 0x03001388
	.4byte 0x00000091
	.4byte 0x0000008e
	.4byte 0x00000092
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0xffce0000
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x03001e80
.L34:
	ldr	r0, [sp, #12]
	bl	Func_08002322
	ldr	r3, [sp, #88]
	lsls	r2, r0, #2
	adds	r2, r2, r0
	lsrs	r1, r3, #31
	adds	r3, r3, r1
	lsls	r2, r2, #1
	asrs	r3, r3, #1
	asrs	r2, r2, #16
	adds	r2, r2, r3
	subs	r2, #10
	ldr	r0, [sp, #12]
	mov	r9, r2
	bl	Func_0800231c
	lsls	r0, r0, #2
	ldr	r3, [sp, #84]
	asrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r5, r0, #0
	mov	r2, fp
	subs	r5, #24
	cmp	r2, #32
	ble.n	.L36
	lsls	r3, r2, #1
	subs	r3, r5, r3
	adds	r5, r3, #0
	adds	r5, #64
.L36:
	ldr	r3, [sp, #72]
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r6, r3, r0
	movs	r1, #20
	movs	r7, #40
	str	r1, [sp, #0]
	mov	r2, r9
	mov	r8, r1
	str	r7, [sp, #4]
	ldr	r4, [sp, #76]
	ldr	r0, [sp, #68]
	adds	r1, r6, #0
	adds	r3, r5, #0
	bl	Func_080072f4
	mov	r2, fp
	cmp	r2, #3
	bgt.n	.L33
	mov	r3, r8
	str	r3, [sp, #0]
	ldr	r0, [sp, #40]
	str	r7, [sp, #4]
	adds	r1, r6, #0
	ldr	r4, [r0, #4]
	mov	r2, r9
	ldr	r0, [sp, #68]
	adds	r3, r5, #0
	bl	Func_080072f4
.L33:
	movs	r1, #0
	str	r1, [sp, #60]
	ldr	r2, [pc, #732]
	ldr	r0, [sp, #72]
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L37
	b.n	.L38
.L37:
	ldr	r1, [sp, #36]
	mov	r3, sp
	adds	r1, #12
	adds	r3, #92
	mov	r0, fp
	str	r1, [sp, #32]
	str	r3, [sp, #28]
	lsls	r0, r0, #9
	movs	r1, #36
	movs	r3, #0
	str	r0, [sp, #20]
	str	r1, [sp, #16]
	str	r3, [sp, #8]
.L66:
	ldr	r0, [sp, #72]
	adds	r6, r0, r2
	ldr	r1, [sp, #16]
	ldr	r3, [r6, #0]
	ldrsh	r0, [r3, r1]
	bl	Func_080b5098
	ldr	r3, [sp, #60]
	lsls	r3, r3, #3
	ldr	r5, [r0, #0]
	mov	r9, r3
	bl	Func_080049ac
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #32]
	bl	Func_080051d8
	ldr	r3, [r5, #8]
	ldr	r0, [sp, #28]
	str	r3, [r0, #0]
	movs	r3, #160
	lsls	r3, r3, #14
	str	r3, [r0, #4]
	ldr	r3, [r5, #16]
	str	r3, [r0, #8]
	ldr	r0, [sp, #28]
	bl	Func_08004cb4
	mov	r3, r9
	adds	r3, #20
	cmp	fp, r3
	bne.n	.L39
	movs	r0, #126
	bl	Func_080f9010
.L39:
	mov	r3, r9
	adds	r3, #36
	cmp	fp, r3
	bne.n	.L40
	ldr	r3, [r6, #0]
	ldr	r1, [sp, #16]
	ldrsh	r0, [r3, r1]
	movs	r3, #28
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	ldr	r3, [sp, #60]
	bl	Func_080d6888
.L40:
	cmp	fp, r9
	bgt.n	.L41
	b.n	.L42
.L41:
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L43
	cmp	r3, #6
	beq.n	.L43
	mov	r0, sl
	cmp	r0, #1
	bne.n	.L44
	mov	r1, fp
	lsls	r5, r1, #9
	adds	r0, r5, #0
	bl	Func_08004bd4
	adds	r0, r5, #0
	bl	Func_08004c6c
	b.n	.L45
.L44:
	mov	r2, sl
	cmp	r2, #2
	bne.n	.L46
	ldr	r3, [sp, #60]
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #3
	mov	r1, fp
	subs	r0, r1, r0
	lsls	r0, r0, #9
	bl	Func_08004c1c
	b.n	.L45
.L46:
	mov	r2, sl
	cmp	r2, #3
	bne.n	.L47
.L43:
	ldr	r0, [sp, #20]
	bl	Func_08004c1c
	b.n	.L45
.L47:
	ldr	r0, [sp, #20]
	bl	Func_08004c1c
	ldr	r0, [sp, #20]
	bl	Func_08004bd4
.L45:
	ldr	r0, [sp, #52]
	movs	r3, #0
	mov	r8, r3
	cmp	r0, #0
	bne.n	.L48
	b.n	.L42
.L48:
	mov	r1, sl
	subs	r1, #3
	ldr	r2, [sp, #8]
	ldr	r3, [pc, #488]
	str	r1, [sp, #24]
	adds	r6, r2, r3
.L65:
	ldr	r0, [sp, #24]
	cmp	r0, #2
	bhi.n	.L49
	mov	r1, r8
	lsrs	r3, r1, #31
	add	r3, r8
	asrs	r3, r3, #1
	add	r3, r9
	adds	r2, r3, #0
	adds	r2, #32
	b.n	.L50
.L49:
	movs	r2, #128
	lsls	r2, r2, #9
.L50:
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L51
	adds	r3, #3
.L51:
	asrs	r3, r3, #2
	add	r3, r9
	cmp	fp, r3
	bgt.n	.L52
	b.n	.L53
.L52:
	cmp	fp, r2
	blt.n	.L54
	b.n	.L53
.L54:
	ldr	r3, [r6, #0]
	asrs	r3, r3, #8
	adds	r0, r3, #0
	muls	r0, r3
	ldr	r3, [r6, #4]
	asrs	r3, r3, #8
	adds	r2, r3, #0
	muls	r2, r3
	ldr	r3, [r6, #8]
	asrs	r3, r3, #8
	adds	r1, r3, #0
	muls	r1, r3
	adds	r0, r0, r2
	adds	r3, r1, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #404]
	bl	Func_080072f0
	asrs	r7, r0, #9
	cmp	r7, #0
	beq.n	.L53
	add	r5, sp, #104
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_080e3944
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L55
	ldr	r3, [r5, #0]
	ldr	r0, [sp, #48]
	adds	r3, r3, r0
	b.n	.L56
.L55:
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
.L56:
	str	r3, [r5, #0]
	ldr	r3, [r5, #4]
	adds	r3, #16
	str	r3, [r5, #4]
	ldr	r1, [pc, #360]
	ldr	r3, [r5, #8]
	cmp	r3, r1
	bgt.n	.L57
	movs	r3, #157
	lsls	r3, r3, #1
	str	r3, [r5, #8]
.L57:
	ldr	r2, [pc, #352]
	cmp	r3, r2
	ble.n	.L58
	str	r2, [r5, #8]
	adds	r3, r2, #0
.L58:
	ldr	r0, [pc, #344]
	adds	r2, r3, r0
	cmp	r2, #0
	bge.n	.L59
	adds	r2, r3, #0
	subs	r2, #187
.L59:
	asrs	r2, r2, #7
	movs	r3, #3
	mov	r1, sl
	subs	r4, r3, r2
	cmp	r1, #0
	bne.n	.L60
	mov	r2, r8
	lsls	r0, r2, #2
	add	r0, fp
	movs	r1, #9
	bl	Func_080022fc
	adds	r4, r0, #0
	b.n	.L61
.L60:
	mov	r3, sl
	cmp	r3, #3
	beq.n	.L61
	cmp	r3, #4
	beq.n	.L61
	cmp	r3, #5
	bne.n	.L62
.L61:
	ldr	r2, [pc, #296]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r0, [sp, #72]
	ldr	r3, [pc, #292]
	adds	r1, r0, r1
	ldrb	r0, [r3, r4]
	ldr	r2, [r5, #0]
	lsrs	r4, r0, #1
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r0, [sp, #40]
	subs	r2, r2, r4
	subs	r3, r3, r4
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #68]
	bl	Func_080072f4
	b.n	.L63
.L62:
	lsls	r0, r4, #1
	ldr	r2, [pc, #260]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #56]
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #4]
	str	r4, [sp, #0]
	ldr	r0, [sp, #40]
	subs	r3, r3, r4
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #68]
	bl	Func_080072f4
.L63:
	mov	r1, sl
	cmp	r1, #2
	bls.n	.L64
	cmp	r1, #6
	bne.n	.L53
.L64:
	ldr	r5, [r6, #0]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	Func_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #0]
	ldr	r5, [r6, #4]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	Func_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #4]
	ldr	r5, [r6, #8]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	Func_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #8]
.L53:
	movs	r2, #1
	ldr	r3, [sp, #52]
	add	r8, r2
	adds	r6, #28
	cmp	r8, r3
	beq.n	.L42
	b.n	.L65
.L42:
	ldr	r0, [sp, #20]
	ldr	r1, [pc, #160]
	adds	r0, r0, r1
	str	r0, [sp, #20]
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #8]
	ldr	r1, [sp, #60]
	movs	r0, #224
	lsls	r0, r0, #3
	adds	r3, r3, r0
	adds	r2, #2
	adds	r1, #1
	str	r2, [sp, #16]
	str	r3, [sp, #8]
	str	r1, [sp, #60]
	ldr	r2, [pc, #96]
	ldr	r0, [sp, #72]
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	cmp	r1, r3
	beq.n	.L38
	b.n	.L66
.L38:
	bl	Func_080cd52c
	ldr	r3, [pc, #116]
	ldr	r1, [sp, #72]
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #128
	ldr	r0, [sp, #12]
	lsls	r1, r1, #4
	movs	r2, #1
	ldr	r3, [sp, #64]
	adds	r0, r0, r1
	add	fp, r2
	str	r0, [sp, #12]
	cmp	fp, r3
	beq.n	.L30
	b.n	.L67
.L30:
	ldr	r0, [pc, #84]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #128
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00007828
	.4byte 0x02010000
	.4byte 0x030001d8
	.4byte 0x00000139
	.4byte 0x0000027a
	.4byte 0xfffffec6
	.4byte 0x080ede84
	.4byte 0x080ede96
	.4byte 0x080ede48
	.4byte 0xfffff000
	.4byte 0x00007824
	.4byte 0x080cd261
