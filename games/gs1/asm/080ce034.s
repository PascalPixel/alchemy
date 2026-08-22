@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ce034
	.thumb_func
Func_080ce034:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #940]
	adds	r3, r6, #0
	ldmia	r3!, {r1}
	sub	sp, #60
	str	r1, [sp, #36]
	ldr	r3, [r3, #0]
	str	r3, [sp, #32]
	ldr	r2, [r6, #8]
	str	r2, [sp, #28]
	ldr	r2, [pc, #924]
	adds	r3, r1, r2
	str	r0, [r3, #0]
	movs	r0, #0
	bl	Func_080cd594
	ldr	r0, [pc, #916]
	bl	Func_08002f40
	ldr	r1, [sp, #28]
	bl	Func_08005340
	ldr	r0, [pc, #908]
	bl	Func_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #904]
	adds	r1, r5, #0
	movs	r2, #128
	lsls	r0, r0, #19
	bl	Func_080072f0
	adds	r5, #128
	ldr	r1, [sp, #36]
	adds	r0, r5, #0
	bl	Func_08005340
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	Func_080ed408
	ldr	r3, [r6, #28]
	movs	r1, #7
	str	r3, [sp, #40]
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	str	r5, [sp, #0]
	bl	Func_080ed408
	ldr	r3, [r6, #32]
	mov	r0, sp
	adds	r0, #40
	str	r0, [sp, #8]
	str	r3, [r0, #4]
	ldr	r1, [sp, #36]
	movs	r2, #239
	lsls	r2, r2, #7
	adds	r3, r1, r2
	str	r5, [r3, #0]
	ldr	r3, [pc, #832]
	adds	r2, r1, r3
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #824]
	bl	Func_080041d8
	movs	r0, #0
	movs	r2, #128
	ldr	r3, [pc, #816]
	mov	r8, r0
	movs	r1, #0
	lsls	r2, r2, #3
.L0:
	movs	r0, #1
	add	r8, r0
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L0
	ldr	r1, [sp, #36]
	ldr	r2, [pc, #772]
	adds	r5, r1, r2
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	Func_080b5098
	ldr	r3, [r5, #0]
	ldr	r0, [r0, #0]
	mov	sl, r0
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	bl	Func_080b5098
	ldr	r0, [r0, #0]
	str	r0, [sp, #24]
	mov	r2, sl
	ldr	r3, [r2, #8]
	ldr	r0, [pc, #764]
	str	r0, [sp, #20]
	cmp	r3, #0
	bgt.n	.L1
	movs	r1, #240
	lsls	r1, r1, #12
	str	r1, [sp, #20]
.L1:
	ldr	r0, [pc, #724]
	ldr	r3, [sp, #36]
	mov	r1, sp
	adds	r0, r3, r0
	adds	r1, #48
	movs	r2, #0
	str	r0, [sp, #16]
	str	r1, [sp, #12]
	mov	fp, r2
.L21:
	ldr	r3, [pc, #736]
	ldr	r5, [r3, #0]
	bl	Func_080049ac
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	Func_080051d8
	mov	r2, fp
	cmp	r2, #17
	bgt.n	.L2
	cmp	r2, #0
	bne.n	.L3
.L2:
	ldr	r0, [sp, #16]
	ldr	r5, [sp, #12]
	ldr	r3, [r0, #0]
	adds	r1, r5, #0
	ldr	r0, [r3, #8]
	bl	Func_080e3980
	ldr	r3, [r5, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #0]
.L3:
	mov	r3, fp
	subs	r3, #2
	cmp	r3, #1
	bhi.n	.L4
	ldr	r5, [sp, #12]
	movs	r1, #32
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	str	r1, [sp, #0]
	movs	r1, #64
	str	r1, [sp, #4]
	subs	r2, #16
	subs	r3, #64
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #36]
	bl	Func_080072f4
.L4:
	mov	r2, fp
	subs	r2, #4
	cmp	r2, #11
	bhi.n	.L5
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	movs	r1, #0
	lsls	r3, r3, #11
	mov	r8, r1
	add	r7, sp, #48
	mov	r9, r3
.L6:
	mov	r2, r8
	lsls	r6, r2, #12
	adds	r0, r6, #0
	bl	Func_08002322
	mov	r3, fp
	muls	r3, r0
	ldr	r5, [r7, #0]
	asrs	r3, r3, #16
	adds	r0, r6, #0
	adds	r5, r5, r3
	bl	Func_0800231c
	mov	r2, fp
	muls	r2, r0
	ldr	r3, [r7, #4]
	asrs	r2, r2, #16
	adds	r3, r3, r2
	mov	r0, fp
	movs	r2, #32
	ldr	r1, [sp, #36]
	subs	r3, r3, r0
	str	r2, [sp, #0]
	subs	r5, #16
	movs	r2, #64
	str	r2, [sp, #4]
	add	r1, r9
	adds	r2, r5, #0
	subs	r3, #64
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #32]
	bl	Func_080072f4
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #16
	bne.n	.L6
.L5:
	mov	r3, fp
	cmp	r3, #4
	bne.n	.L7
	movs	r3, #160
	mov	r0, sl
	lsls	r3, r3, #13
	str	r3, [r0, #40]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #52]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r0, #48]
	ldr	r3, [pc, #528]
	str	r3, [r0, #72]
	mov	r3, sl
	movs	r2, #0
	adds	r3, #90
	strb	r2, [r3, #0]
	subs	r3, #2
	strb	r2, [r3, #0]
	ldr	r3, [r0, #8]
	lsls	r1, r3, #1
	adds	r1, r1, r3
	ldr	r3, [r0, #16]
	bl	Func_08009150
	mov	r0, sl
	movs	r1, #2
	bl	Func_08009080
	ldr	r2, [pc, #496]
	ldr	r1, [sp, #36]
	mov	r0, fp
	adds	r3, r1, r2
	str	r0, [r3, #0]
	movs	r0, #136
	bl	Func_080f9010
.L7:
	mov	r1, fp
	cmp	r1, #16
	bne.n	.L8
	ldr	r0, [pc, #476]
	bl	Func_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #436]
	adds	r1, r5, #0
	movs	r2, #128
	lsls	r0, r0, #19
	bl	Func_080072f0
	adds	r5, #128
	adds	r0, r5, #0
	ldr	r1, [sp, #36]
	bl	Func_08005340
	movs	r3, #0
	mov	r2, sl
	str	r3, [r2, #72]
	str	r3, [r2, #36]
	str	r3, [r2, #40]
	ldr	r0, [sp, #24]
	ldr	r3, [r0, #16]
	mov	r0, sl
	str	r3, [r2, #16]
	bl	Func_08009140
.L8:
	mov	r1, fp
	cmp	r1, #17
	bgt.n	.L9
	b.n	.L10
.L9:
	mov	r3, sl
	ldr	r2, [r3, #12]
	cmp	r2, #0
	ble.n	.L11
	ldr	r0, [sp, #20]
	ldr	r3, [r3, #8]
	adds	r3, r3, r0
	ldr	r0, [pc, #404]
	mov	r1, sl
	str	r3, [r1, #8]
	adds	r3, r2, r0
	str	r3, [r1, #12]
	ldr	r1, [sp, #16]
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L12
	ldr	r5, [sp, #12]
	movs	r1, #40
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	str	r1, [sp, #0]
	movs	r1, #64
	subs	r2, #20
	subs	r3, #52
	str	r1, [sp, #4]
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #36]
	bl	Func_080072f4
	ldr	r3, [r5, #0]
	subs	r3, #8
	str	r3, [r5, #0]
	mov	r3, sl
	ldr	r2, [r3, #12]
	b.n	.L11
.L12:
	ldr	r5, [sp, #12]
	movs	r1, #40
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	str	r1, [sp, #0]
	movs	r1, #64
	str	r1, [sp, #4]
	ldr	r0, [sp, #8]
	subs	r2, #26
	subs	r3, #52
	ldr	r4, [r0, #4]
	ldr	r1, [sp, #36]
	ldr	r0, [sp, #32]
	bl	Func_080072f4
	ldr	r3, [r5, #4]
	adds	r3, #8
	str	r3, [r5, #4]
	mov	r1, sl
	ldr	r2, [r1, #12]
.L11:
	cmp	r2, #0
	bge.n	.L10
	movs	r3, #0
	mov	r2, sl
	str	r3, [r2, #12]
	mov	r8, r3
	add	r3, sp, #48
	ldr	r7, [pc, #296]
	mov	r9, r3
.L13:
	bl	Func_08004458
	ldr	r5, [pc, #292]
	ands	r5, r0
	bl	Func_08004458
	ldr	r3, [pc, #288]
	adds	r6, r0, #0
	mov	r0, r9
	ands	r6, r3
	ldr	r3, [r0, #0]
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ldr	r3, [r0, #4]
	subs	r3, #24
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	adds	r0, r6, #0
	bl	Func_08002322
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #8]
	adds	r0, r6, #0
	bl	Func_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	Func_08004458
	movs	r3, #7
	ands	r3, r0
	movs	r1, #1
	movs	r2, #128
	adds	r3, #32
	add	r8, r1
	lsls	r2, r2, #1
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r8, r2
	bne.n	.L13
	ldr	r0, [sp, #36]
	ldr	r1, [pc, #184]
	movs	r5, #8
	adds	r3, r0, r1
	str	r5, [r3, #0]
	movs	r0, #145
	bl	Func_080b50e8
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r1, #4
	bl	Func_080b5088
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	movs	r2, #5
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r1, #7
	movs	r3, #0
	str	r5, [sp, #0]
	bl	Func_080d6888
.L10:
	movs	r2, #0
	ldr	r6, [pc, #144]
	mov	r8, r2
.L19:
	ldr	r4, [r6, #24]
	cmp	r4, #0
	ble.n	.L14
	ldr	r2, [r6, #8]
	ldr	r3, [r6, #0]
	adds	r3, r3, r2
	mov	ip, r3
	str	r3, [r6, #0]
	ldr	r1, [r6, #16]
	ldr	r3, [r6, #4]
	adds	r7, r3, r1
	lsls	r3, r2, #3
	subs	r3, r3, r2
	subs	r0, r4, #1
	lsls	r3, r3, #3
	str	r0, [r6, #24]
	str	r7, [r6, #4]
	cmp	r3, #0
	bge.n	.L15
	adds	r3, #63
.L15:
	asrs	r3, r3, #6
	str	r3, [r6, #8]
	lsls	r3, r1, #3
	subs	r3, r3, r1
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L16
	adds	r3, #63
.L16:
	movs	r1, #128
	asrs	r3, r3, #6
	lsls	r1, r1, #6
	movs	r2, #224
	adds	r3, r3, r1
	lsls	r2, r2, #15
	str	r3, [r6, #16]
	cmp	r7, r2
	ble.n	.L17
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
	b.n	.L14
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x00000073
	.4byte 0x0000007d
	.4byte 0x03001388
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x02010018
	.4byte 0xfff10000
	.4byte 0x03001e80
	.4byte 0x0000ab85
	.4byte 0x000077a8
	.4byte 0x00000089
	.4byte 0xfff80000
	.4byte 0x02010000
	.4byte 0x000003ff
	.4byte 0x0000ffff
.L17:
	ldr	r3, [pc, #160]
	cmp	ip, r3
	bhi.n	.L14
	cmp	r7, #0
	blt.n	.L14
	cmp	r0, #0
	bge.n	.L18
	adds	r0, r4, #6
.L18:
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r5, r0, #1
	ldr	r2, [pc, #144]
	mov	r1, r8
	subs	r3, r5, #2
	movs	r4, #1
	ands	r4, r1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #28]
	mov	r3, ip
	adds	r1, r2, r1
	asrs	r2, r3, #16
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	asrs	r3, r7, #16
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r5, [sp, #4]
	ldr	r0, [sp, #8]
	lsls	r4, r4, #2
	ldr	r4, [r4, r0]
	ldr	r0, [sp, #32]
	bl	Func_080072f4
.L14:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #1
	adds	r6, #28
	cmp	r8, r2
	bne.n	.L19
	movs	r1, #16
	movs	r0, #16
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r0, [pc, #72]
	ldr	r3, [sp, #36]
	adds	r2, r3, r0
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #1
	add	fp, r1
	mov	r2, fp
	cmp	r2, #88
	beq.n	.L20
	b.n	.L21
.L20:
	ldr	r0, [pc, #48]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #60
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x007effff
	.4byte 0x080ede48
	.4byte 0x00007824
	.4byte 0x080cd261
