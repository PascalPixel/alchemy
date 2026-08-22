@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080dfa48
	.thumb_func
Func_080dfa48:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #824]
	adds	r3, r2, #0
	adds	r6, r1, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #72
	str	r3, [sp, #32]
	adds	r3, r2, #0
	subs	r3, #108
	ldr	r3, [r3, #0]
	str	r3, [sp, #20]
	ldr	r5, [pc, #804]
	mov	r9, r1
	ldr	r2, [r2, #8]
	add	r5, r9
	str	r2, [sp, #16]
	str	r0, [r5, #0]
	movs	r0, #0
	bl	Func_080cd594
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L0
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	Func_080ed408
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #11
	str	r5, [sp, #0]
	bl	Func_080ed408
	b.n	.L1
.L0:
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #46
	str	r5, [sp, #0]
	bl	Func_080ed408
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #15
	str	r5, [sp, #0]
	bl	Func_080ed408
.L1:
	ldr	r3, [pc, #720]
	adds	r2, r3, #0
	adds	r2, #184
	adds	r3, #188
	ldr	r2, [r2, #0]
	ldr	r3, [r3, #0]
	str	r2, [sp, #24]
	str	r3, [sp, #28]
	ldr	r1, [sp, #16]
	ldr	r0, [pc, #708]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	ldr	r0, [pc, #700]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	movs	r3, #144
	lsls	r3, r3, #1
	mov	r0, r9
	ldr	r1, [pc, #688]
	movs	r2, #40
	bl	Func_080df9d0
	ldr	r0, [pc, #684]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	cmp	r6, #1
	beq.n	.L2
	cmp	r6, #1
	bgt.n	.L3
	cmp	r6, #0
	beq.n	.L4
	b.n	.L5
.L3:
	cmp	r6, #2
	beq.n	.L6
	b.n	.L5
.L4:
	ldr	r0, [pc, #656]
	b.n	.L7
.L2:
	ldr	r0, [pc, #656]
	b.n	.L7
.L6:
	ldr	r0, [pc, #656]
	b.n	.L7
.L5:
	ldr	r0, [pc, #656]
.L7:
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #648]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	Func_080072f0
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #632]
	ldr	r5, [pc, #588]
	add	r2, r9
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	add	r5, r9
	lsls	r1, r1, #3
	ldr	r0, [pc, #620]
	bl	Func_080041d8
	ldr	r3, [r5, #0]
	movs	r2, #36
	ldrsh	r1, [r3, r2]
	ldr	r0, [r3, #8]
	movs	r2, #10
	bl	Func_080df90c
	ldr	r3, [r5, #0]
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	bl	Func_080b5098
	movs	r5, #225
	ldr	r6, [r0, #0]
	lsls	r5, r5, #7
	movs	r0, #0
	mov	r8, r0
	movs	r7, #255
	add	r5, r9
.L9:
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	movs	r1, #160
	ldr	r3, [r6, #12]
	lsls	r1, r1, #12
	adds	r3, r3, r1
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	Func_08004458
	ldr	r3, [pc, #560]
	ands	r3, r0
	lsls	r3, r3, #11
	str	r3, [r5, #12]
	bl	Func_08004458
	ands	r0, r7
	subs	r0, #64
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	Func_08004458
	ands	r0, r7
	subs	r0, #128
	ldr	r3, [r5, #0]
	lsls	r0, r0, #11
	str	r0, [r5, #20]
	cmp	r3, #0
	ble.n	.L8
	ldr	r3, [r5, #12]
	negs	r3, r3
	str	r3, [r5, #12]
.L8:
	mov	r2, r8
	lsrs	r3, r2, #31
	add	r3, r8
	asrs	r3, r3, #1
	adds	r3, #16
	str	r3, [r5, #24]
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r5, #28
	cmp	r4, #64
	bne.n	.L9
	ldr	r3, [pc, #440]
	add	r3, r9
	ldr	r3, [r3, #0]
	mov	r2, sp
	adds	r2, #60
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	adds	r1, r2, #0
	str	r2, [sp, #12]
	bl	Func_080e3980
	ldr	r4, [pc, #416]
	movs	r0, #48
	add	r4, r9
	movs	r3, #0
	add	r0, sp
	str	r4, [sp, #8]
	mov	fp, r3
	mov	sl, r0
.L17:
	mov	r1, fp
	cmp	r1, #14
	bgt.n	.L10
	ldr	r2, [sp, #8]
	ldr	r3, [r2, #0]
	mov	r1, sl
	ldr	r0, [r3, #8]
	bl	Func_080e3980
	mov	r3, sl
	ldr	r2, [r3, #0]
	mov	r4, sl
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r4, #4]
	movs	r0, #40
	movs	r1, #32
	asrs	r2, r2, #1
	subs	r2, #16
	subs	r3, #48
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	ldr	r4, [sp, #24]
	mov	r1, r9
	ldr	r0, [sp, #32]
	bl	Func_080072f4
	mov	r0, sl
	ldr	r2, [r0, #0]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r0, #4]
	movs	r1, #40
	movs	r4, #32
	asrs	r2, r2, #1
	str	r1, [sp, #0]
	str	r4, [sp, #4]
	subs	r2, #16
	subs	r3, #16
	ldr	r0, [sp, #32]
	mov	r1, r9
	ldr	r4, [sp, #28]
	bl	Func_080072f4
.L10:
	mov	r0, fp
	cmp	r0, #10
	bne.n	.L11
	ldr	r1, [sp, #8]
	ldr	r3, [r1, #0]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r1, #7
	movs	r3, #0
	bl	Func_080d6888
	ldr	r4, [sp, #8]
	ldr	r3, [r4, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r1, #4
	bl	Func_080b5088
	movs	r0, #134
	bl	Func_080b50e8
	ldr	r3, [pc, #320]
	movs	r2, #8
	add	r3, r9
	str	r2, [r3, #0]
.L11:
	mov	r5, fp
	subs	r5, #8
	cmp	r5, #11
	bhi.n	.L12
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	ldr	r4, [sp, #12]
	lsls	r1, r3, #4
	ldr	r2, [r4, #0]
	subs	r1, r1, r3
	ldr	r3, [pc, #252]
	lsls	r1, r1, #6
	mov	r0, sl
	adds	r1, r1, r3
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r0, #4]
	movs	r0, #20
	asrs	r2, r2, #1
	str	r0, [sp, #0]
	movs	r0, #48
	str	r0, [sp, #4]
	subs	r2, #16
	subs	r3, #40
	ldr	r0, [sp, #32]
	ldr	r4, [sp, #24]
	bl	Func_080072f4
.L12:
	cmp	r5, #55
	bhi.n	.L13
	bl	Func_080049ac
	ldr	r0, [sp, #20]
	adds	r1, r0, #0
	adds	r1, #12
	bl	Func_080051d8
	movs	r6, #225
	movs	r0, #0
	lsls	r6, r6, #7
	mov	r8, r0
	add	r7, sp, #36
	add	r6, r9
.L15:
	ldr	r5, [r6, #24]
	cmp	r5, #0
	ble.n	.L14
	adds	r1, r7, #0
	adds	r0, r6, #0
	bl	Func_080e3944
	asrs	r5, r5, #4
	ldr	r2, [r7, #0]
	adds	r5, #2
	lsls	r0, r5, #1
	ldr	r4, [pc, #208]
	asrs	r2, r2, #1
	str	r2, [r7, #0]
	subs	r3, r0, #2
	ldrh	r1, [r4, r3]
	ldr	r3, [sp, #16]
	adds	r1, r3, r1
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	ldr	r4, [sp, #24]
	subs	r3, r3, r5
	str	r0, [sp, #4]
	str	r5, [sp, #0]
	ldr	r0, [sp, #32]
	bl	Func_080072f4
	adds	r0, r6, #0
	movs	r1, #60
	ldr	r2, [pc, #168]
	bl	Func_080e38b8
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L14:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r6, #28
	cmp	r1, #64
	bne.n	.L15
.L13:
	movs	r0, #8
	movs	r1, #8
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r2, [pc, #136]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #1
	add	fp, r2
	mov	r3, fp
	cmp	r3, #60
	beq.n	.L16
	b.n	.L17
.L16:
	ldr	r0, [pc, #92]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #72
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
	.4byte 0x03001e50
	.4byte 0x00000073
	.4byte 0x00000099
	.4byte 0x02010000
	.4byte 0x000000bd
	.4byte 0x000000c2
	.4byte 0x000000b9
	.4byte 0x000000bb
	.4byte 0x000000c0
	.4byte 0x03001388
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x000001ff
	.4byte 0x000077a8
	.4byte 0x080ede48
	.4byte 0xfffffe00
	.4byte 0x00007824
