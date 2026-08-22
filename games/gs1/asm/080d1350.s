@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080d1350
	.thumb_func
Func_080d1350:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #884]
	adds	r3, r6, #0
	ldmia	r3!, {r1}
	sub	sp, #64
	str	r1, [sp, #48]
	ldr	r3, [r3, #0]
	str	r3, [sp, #44]
	ldr	r2, [r6, #8]
	adds	r3, r6, #0
	str	r2, [sp, #24]
	subs	r3, #108
	ldr	r3, [r3, #0]
	str	r3, [sp, #20]
	ldr	r3, [pc, #860]
	adds	r1, r1, r3
	str	r0, [r1, #0]
	movs	r0, #1
	mov	r8, r1
	bl	Func_080cd594
	ldr	r0, [pc, #852]
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #844]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	Func_080072f0
	ldr	r0, [pc, #840]
	bl	Func_08002f40
	ldr	r1, [sp, #24]
	bl	Func_08005340
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	Func_080ed408
	movs	r1, #239
	ldr	r0, [sp, #48]
	lsls	r1, r1, #7
	adds	r3, r0, r1
	str	r5, [r3, #0]
	ldr	r3, [pc, #804]
	ldr	r6, [r6, #28]
	adds	r2, r0, r3
	movs	r1, #144
	movs	r3, #50
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	mov	r5, r8
	ldr	r0, [pc, #792]
	str	r6, [sp, #32]
	bl	Func_080041d8
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	Func_080b5098
	ldr	r3, [r5, #0]
	ldr	r0, [r0, #0]
	mov	sl, r0
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	bl	Func_080b5098
	movs	r5, #225
	movs	r2, #0
	ldr	r3, [sp, #48]
	ldr	r7, [r0, #0]
	lsls	r5, r5, #7
	str	r2, [sp, #28]
	adds	r6, r3, r5
.L0:
	mov	r1, sl
	ldr	r3, [r1, #8]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	ldr	r0, [sp, #28]
	str	r3, [r6, #0]
	movs	r2, #240
	ldr	r3, [r1, #12]
	lsls	r2, r2, #15
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r3, [r1, #16]
	str	r3, [r6, #8]
	lsls	r5, r0, #3
	bl	Func_08004458
	movs	r3, #127
	ands	r3, r0
	subs	r3, #64
	ldr	r0, [r7, #8]
	lsls	r3, r3, #16
	adds	r0, r0, r3
	ldr	r3, [r6, #0]
	movs	r1, #12
	subs	r0, r0, r3
	bl	Func_080022ec
	str	r0, [r6, #12]
	ldr	r0, [r7, #12]
	ldr	r3, [r6, #4]
	subs	r0, r0, r3
	movs	r3, #160
	lsls	r3, r3, #13
	adds	r0, r0, r3
	movs	r1, #12
	bl	Func_080022ec
	str	r0, [r6, #16]
	ldr	r3, [r6, #8]
	ldr	r0, [r7, #16]
	movs	r1, #12
	subs	r0, r0, r3
	bl	Func_080022ec
	str	r0, [r6, #20]
	bl	Func_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, r3, r5
	str	r3, [r6, #24]
	ldr	r5, [sp, #28]
	adds	r5, #1
	adds	r6, #28
	str	r5, [sp, #28]
	cmp	r5, #8
	bne.n	.L0
	movs	r0, #0
	str	r0, [sp, #40]
.L16:
	ldr	r1, [pc, #632]
	ldr	r0, [sp, #40]
	ldr	r2, [pc, #632]
	movs	r3, #0
	bl	Func_080d40ec
	ldr	r1, [sp, #40]
	cmp	r1, #96
	bne.n	.L1
	movs	r0, #134
	bl	Func_080b50e8
.L1:
	movs	r2, #0
	ldr	r3, [sp, #48]
	movs	r5, #225
	lsls	r5, r5, #7
	str	r2, [sp, #28]
	str	r2, [sp, #16]
	adds	r7, r3, r5
.L14:
	ldr	r3, [r7, #24]
	ldr	r0, [sp, #40]
	cmp	r0, r3
	bge.n	.L2
	b.n	.L3
.L2:
	bl	Func_080049ac
	ldr	r0, [sp, #20]
	adds	r1, r0, #0
	adds	r1, #12
	bl	Func_080051d8
	add	r5, sp, #52
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	Func_080e3944
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	adds	r3, #8
	cmp	r3, #135
	bls.n	.L4
	b.n	.L5
.L4:
	ldr	r3, [r5, #4]
	cmp	r3, #127
	ble.n	.L6
	b.n	.L5
.L6:
	movs	r1, #8
	negs	r1, r1
	cmp	r3, r1
	bge.n	.L7
	b.n	.L5
.L7:
	ldr	r3, [sp, #28]
	lsls	r2, r3, #2
	adds	r2, r2, r3
	lsls	r3, r2, #3
	mov	sl, r5
	subs	r3, r3, r2
	ldr	r5, [pc, #524]
	ldr	r0, [pc, #528]
	lsls	r3, r3, #3
	movs	r4, #0
	mov	r8, r5
	adds	r1, r3, r0
.L8:
	ldr	r3, [pc, #520]
	adds	r6, r4, #0
	muls	r6, r3
	ldr	r2, [sp, #40]
	ldr	r0, [r7, #24]
	subs	r0, r2, r0
	lsls	r0, r0, #11
	subs	r0, r6, r0
	str	r1, [sp, #12]
	str	r4, [sp, #8]
	bl	Func_08002322
	ldr	r4, [sp, #8]
	movs	r5, #1
	ands	r5, r4
	mov	r2, r8
	ldrb	r3, [r2, r5]
	adds	r2, r3, #0
	muls	r2, r0
	mov	r0, sl
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r0, #0]
	ldr	r1, [sp, #12]
	asrs	r2, r2, #17
	adds	r3, r3, r2
	str	r3, [r1, #12]
	ldr	r2, [sp, #40]
	ldr	r3, [r7, #24]
	subs	r3, r2, r3
	lsls	r3, r3, #11
	subs	r6, r6, r3
	adds	r0, r6, #0
	bl	Func_0800231c
	mov	r2, r8
	ldrb	r3, [r2, r5]
	adds	r2, r3, #0
	muls	r2, r0
	mov	r5, sl
	ldr	r3, [r5, #4]
	ldr	r4, [sp, #8]
	ldr	r1, [sp, #12]
	asrs	r2, r2, #16
	subs	r3, r3, r2
	adds	r4, #1
	str	r3, [r1, #16]
	adds	r1, #28
	cmp	r4, #10
	bne.n	.L8
	ldr	r0, [sp, #16]
	movs	r4, #0
	mov	fp, r0
.L10:
	mov	r1, fp
	adds	r2, r4, r1
	lsls	r3, r2, #3
	adds	r4, #1
	subs	r3, r3, r2
	ldr	r2, [pc, #400]
	lsls	r3, r3, #2
	mov	r9, r4
	adds	r2, r2, r3
	mov	r0, r9
	movs	r1, #10
	mov	sl, r2
	bl	Func_080022fc
	add	r0, fp
	lsls	r3, r0, #3
	subs	r3, r3, r0
	ldr	r5, [pc, #376]
	lsls	r3, r3, #2
	adds	r5, r5, r3
	mov	r8, r5
	movs	r4, #0
.L9:
	mov	r0, r8
	mov	r1, sl
	ldr	r6, [r1, #12]
	ldr	r3, [r0, #12]
	subs	r3, r3, r6
	adds	r0, r4, #0
	muls	r0, r3
	movs	r1, #12
	str	r4, [sp, #8]
	bl	Func_080022ec
	mov	r2, r8
	adds	r6, r6, r0
	mov	r0, sl
	ldr	r3, [r2, #16]
	ldr	r5, [r0, #16]
	ldr	r4, [sp, #8]
	subs	r3, r3, r5
	adds	r0, r4, #0
	muls	r0, r3
	movs	r1, #12
	bl	Func_080022ec
	ldr	r2, [pc, #328]
	movs	r3, #4
	subs	r3, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #24]
	adds	r5, r5, r0
	movs	r0, #1
	adds	r1, r3, r1
	subs	r6, r6, r0
	subs	r5, #2
	movs	r2, #2
	movs	r3, #4
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	ldr	r0, [sp, #44]
	adds	r3, r5, #0
	adds	r2, r6, #0
	ldr	r5, [sp, #32]
	bl	Func_080072f8
	ldr	r4, [sp, #8]
	adds	r4, #1
	cmp	r4, #12
	bne.n	.L9
	mov	r4, r9
	cmp	r4, #10
	bne.n	.L10
.L5:
	ldr	r3, [r7, #4]
	ldr	r0, [pc, #276]
	cmp	r3, r0
	bgt.n	.L11
	ldr	r3, [r7, #16]
	negs	r3, r3
	str	r3, [r7, #16]
	ldr	r3, [r7, #12]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #12]
	ldr	r3, [r7, #20]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #20]
	ldr	r1, [sp, #48]
	ldr	r3, [pc, #248]
	adds	r2, r1, r3
	movs	r3, #4
	str	r3, [r2, #0]
	movs	r0, #134
	bl	Func_080f9010
	ldr	r3, [pc, #180]
	ldr	r5, [sp, #48]
	ldr	r3, [r5, r3]
	ldr	r3, [r3, #20]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L11
	ldr	r0, [pc, #168]
	movs	r6, #36
	adds	r5, r5, r0
.L12:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r4, #0
	movs	r2, #5
	str	r4, [sp, #8]
	bl	Func_080d6888
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #8]
	ldr	r3, [r3, #20]
	adds	r4, #1
	adds	r6, #2
	cmp	r4, r3
	bne.n	.L12
.L11:
	ldr	r3, [r7, #0]
	ldr	r2, [r7, #12]
	adds	r3, r3, r2
	str	r3, [r7, #0]
	ldr	r2, [r7, #16]
	ldr	r3, [r7, #4]
	adds	r3, r3, r2
	str	r3, [r7, #4]
	ldr	r2, [r7, #20]
	ldr	r3, [r7, #8]
	adds	r3, r3, r2
	str	r3, [r7, #8]
.L3:
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #28]
	adds	r2, #10
	adds	r3, #1
	str	r2, [sp, #16]
	adds	r7, #28
	str	r3, [sp, #28]
	cmp	r3, #8
	beq.n	.L13
	b.n	.L14
.L13:
	movs	r1, #4
	movs	r0, #4
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r0, [pc, #128]
	ldr	r5, [sp, #48]
	movs	r3, #1
	adds	r2, r5, r0
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [sp, #40]
	adds	r1, #1
	str	r1, [sp, #40]
	cmp	r1, #128
	beq.n	.L15
	b.n	.L16
.L15:
	ldr	r0, [pc, #64]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
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
	.4byte 0x00000079
	.4byte 0x03001388
	.4byte 0x00000073
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x0000aaab
	.4byte 0x00005555
	.4byte 0x080ee158
	.4byte 0x02010000
	.4byte 0x0000199a
	.4byte 0x080ede48
	.4byte 0x001dffff
	.4byte 0x000077a8
	.4byte 0x00007824
