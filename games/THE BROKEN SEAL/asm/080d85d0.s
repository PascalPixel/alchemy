@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080d85d0
	.thumb_func
Func_080d85d0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #808]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #64
	str	r3, [sp, #48]
	ldr	r3, [r2, #8]
	str	r3, [sp, #40]
	subs	r2, #108
	ldr	r2, [r2, #0]
	str	r2, [sp, #36]
	ldr	r3, [r0, #24]
	negs	r5, r3
	orrs	r5, r3
	ldr	r3, [pc, #784]
	mov	r9, r1
	add	r3, r9
	str	r0, [r3, #0]
	movs	r0, #1
	bl	Func_080cd594
	lsrs	r5, r5, #31
	ldr	r0, [pc, #772]
	ldr	r1, [sp, #40]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	cmp	r5, #0
	bne.n	.L0
	ldr	r0, [pc, #760]
	b.n	.L1
.L0:
	ldr	r0, [pc, #760]
.L1:
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #752]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	Func_080072f0
	movs	r1, #1
	movs	r2, #128
	ldr	r3, [pc, #744]
	movs	r7, #0
	negs	r1, r1
	lsls	r2, r2, #3
.L2:
	adds	r7, #1
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r7, r2
	bne.n	.L2
	ldr	r2, [pc, #704]
	mov	r0, r9
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	movs	r4, #0
	mov	r8, r4
	cmp	r3, #0
	beq.n	.L3
	movs	r3, #36
	movs	r1, #255
	str	r3, [sp, #24]
	str	r4, [sp, #16]
	mov	fp, r1
.L5:
	mov	r4, r9
	adds	r5, r4, r2
	ldr	r3, [r5, #0]
	ldr	r1, [sp, #24]
	ldrsh	r0, [r3, r1]
	bl	Func_080b5098
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #24]
	ldr	r6, [r0, #0]
	ldrsh	r0, [r3, r4]
	bl	Func_080b5070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r2, [sp, #16]
	ldr	r3, [pc, #672]
	asrs	r0, r0, #1
	mov	sl, r0
	movs	r7, #0
	adds	r5, r2, r3
.L4:
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	add	r3, sl
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	Func_08004458
	mov	r4, fp
	ands	r0, r4
	subs	r0, #128
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	bl	Func_08004458
	mov	r1, fp
	ands	r0, r1
	subs	r0, #128
	lsls	r0, r0, #10
	str	r0, [r5, #16]
	bl	Func_08004458
	mov	r2, fp
	ands	r0, r2
	subs	r0, #128
	lsls	r0, r0, #10
	movs	r3, #0
	adds	r7, #1
	str	r0, [r5, #20]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #128
	bne.n	.L4
	ldr	r3, [sp, #24]
	ldr	r4, [sp, #16]
	movs	r0, #224
	lsls	r0, r0, #4
	adds	r3, #2
	adds	r4, r4, r0
	str	r4, [sp, #16]
	str	r3, [sp, #24]
	ldr	r2, [pc, #552]
	mov	r4, r9
	ldr	r3, [r4, r2]
	movs	r1, #1
	ldr	r3, [r3, #20]
	add	r8, r1
	cmp	r8, r3
	bne.n	.L5
.L3:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	Func_080ed408
	ldr	r3, [pc, #548]
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r2, #239
	lsls	r2, r2, #7
	str	r3, [sp, #44]
	add	r2, r9
	movs	r3, #3
	str	r3, [r2, #0]
	ldr	r2, [pc, #536]
	ldr	r3, [pc, #536]
	add	r2, r9
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #532]
	bl	Func_080041d8
	movs	r0, #142
	bl	Func_080f9010
	ldr	r1, [pc, #480]
	mov	r2, r9
	ldr	r3, [r2, r1]
	ldr	r2, [r3, #20]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	movs	r4, #72
	movs	r0, #0
	lsls	r3, r3, #2
	negs	r4, r4
	mov	fp, r0
	cmp	r3, r4
	bne.n	.L6
	b.n	.L7
.L6:
	ldr	r0, [sp, #36]
	adds	r0, #12
	str	r0, [sp, #28]
.L17:
	mov	r2, r9
	adds	r5, r2, r1
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	Func_080b5098
	ldr	r3, [r5, #0]
	ldr	r0, [r0, #0]
	mov	sl, r0
	ldr	r0, [r3, #8]
	bl	Func_080b5070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	mov	r3, fp
	str	r0, [sp, #32]
	cmp	r3, #64
	bne.n	.L8
	movs	r0, #133
	bl	Func_080b50e8
.L8:
	bl	Func_080049ac
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #28]
	bl	Func_080051d8
	mov	r4, fp
	cmp	r4, #40
	bne.n	.L9
	ldr	r3, [r5, #0]
	movs	r2, #1
	ldr	r0, [r3, #8]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	subs	r3, #1
	bl	Func_080d6888
.L9:
	ldr	r1, [r5, #0]
	ldr	r2, [r1, #20]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, #52
	cmp	fp, r3
	bne.n	.L10
	movs	r3, #0
	movs	r2, #1
	ldr	r0, [r1, #8]
	str	r3, [sp, #0]
	movs	r1, #0
	negs	r2, r2
	subs	r3, #1
	bl	Func_080d6888
	ldr	r1, [r5, #0]
.L10:
	ldr	r3, [r1, #20]
	movs	r0, #0
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L11
	str	r0, [sp, #8]
	str	r0, [sp, #20]
.L16:
	ldr	r1, [sp, #8]
	cmp	fp, r1
	bne.n	.L12
	ldr	r3, [pc, #308]
	mov	r4, r8
	add	r3, r9
	ldr	r2, [r3, #0]
	lsls	r3, r4, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	movs	r3, #42
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, r8
	bl	Func_080d6888
.L12:
	ldr	r2, [sp, #8]
	cmp	fp, r2
	ble.n	.L13
	ldr	r3, [sp, #20]
	ldr	r4, [pc, #296]
	movs	r7, #0
	add	r6, sp, #52
	adds	r5, r3, r4
.L15:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L14
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	Func_080e3944
	ldr	r2, [r6, #0]
	ldr	r3, [pc, #292]
	asrs	r2, r2, #1
	str	r2, [r6, #0]
	ldrh	r1, [r3, #10]
	ldr	r0, [sp, #40]
	ldr	r3, [r6, #4]
	adds	r1, r0, r1
	movs	r4, #6
	movs	r0, #12
	subs	r3, #6
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #3
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #44]
	bl	Func_080072f4
	adds	r0, r5, #0
	movs	r1, #62
	movs	r2, #0
	bl	Func_080e38b8
	ldr	r0, [sp, #8]
	adds	r3, r0, r7
	adds	r3, #10
	cmp	fp, r3
	ble.n	.L14
	mov	r1, sl
	ldr	r0, [r1, #8]
	ldr	r3, [r5, #0]
	ldr	r2, [sp, #32]
	ldr	r1, [r1, #12]
	subs	r0, r0, r3
	ldr	r3, [r5, #4]
	adds	r1, r1, r2
	subs	r1, r1, r3
	mov	r3, sl
	ldr	r2, [r3, #16]
	ldr	r3, [r5, #8]
	subs	r2, r2, r3
	ldr	r3, [r5, #12]
	asrs	r0, r0, #8
	adds	r3, r3, r0
	str	r3, [r5, #12]
	ldr	r3, [r5, #16]
	asrs	r1, r1, #8
	adds	r3, r3, r1
	str	r3, [r5, #16]
	ldr	r4, [pc, #200]
	ldr	r3, [r5, #20]
	asrs	r2, r2, #8
	ldr	r1, [pc, #196]
	adds	r3, r3, r2
	adds	r0, r0, r4
	str	r3, [r5, #20]
	cmp	r0, r1
	bhi.n	.L14
	adds	r3, r2, r4
	cmp	r3, r1
	bhi.n	.L14
	movs	r0, #1
	negs	r0, r0
	str	r0, [r5, #24]
.L14:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #32
	bne.n	.L15
.L13:
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #20]
	movs	r3, #224
	lsls	r3, r3, #4
	adds	r2, r2, r3
	adds	r1, #20
	ldr	r3, [pc, #100]
	str	r1, [sp, #8]
	str	r2, [sp, #20]
	add	r3, r9
	ldr	r3, [r3, #0]
	movs	r4, #1
	ldr	r3, [r3, #20]
	add	r8, r4
	cmp	r8, r3
	bne.n	.L16
.L11:
	bl	Func_080cd52c
	ldr	r2, [pc, #132]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [pc, #64]
	mov	r2, r9
	ldr	r3, [r2, r1]
	ldr	r2, [r3, #20]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	movs	r0, #1
	lsls	r3, r3, #2
	add	fp, r0
	adds	r3, #72
	cmp	fp, r3
	beq.n	.L7
	b.n	.L17
.L7:
	ldr	r0, [pc, #76]
	bl	Func_08004278
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #64
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x00000073
	.4byte 0x000000b9
	.4byte 0x000000c0
	.4byte 0x03001388
	.4byte 0x02010018
	.4byte 0x02010000
	.4byte 0x03001e50
	.4byte 0x00007784
	.4byte 0x04040404
	.4byte 0x080cd261
	.4byte 0x080ede48
	.4byte 0x00000fff
	.4byte 0x00001ffe
	.4byte 0x00007824
