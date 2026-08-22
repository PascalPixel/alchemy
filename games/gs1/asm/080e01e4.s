@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080e01e4
	.thumb_func
Func_080e01e4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #96]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #40
	str	r3, [sp, #28]
	ldr	r3, [pc, #88]
	mov	r9, r1
	ldr	r2, [r2, #8]
	add	r3, r9
	str	r2, [sp, #24]
	str	r0, [r3, #0]
	movs	r0, #0
	bl	Func_080cd594
	ldr	r3, [pc, #60]
	ldr	r2, [pc, #72]
	strh	r3, [r2, #0]
	mov	r2, sp
	adds	r2, #32
	adds	r1, r2, #0
	movs	r0, #0
	str	r2, [sp, #20]
	bl	Func_080cef64
	ldr	r0, [pc, #56]
	ldr	r1, [sp, #24]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	ldr	r0, [pc, #48]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r1, #200
	lsls	r1, r1, #2
	ldr	r0, [pc, #36]
	add	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	b.n	.L0
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x00000073
	.4byte 0x00000090
	.4byte 0x00000089
.L0:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #640]
	movs	r3, #75
	add	r2, r9
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #632]
	lsls	r1, r1, #3
	movs	r5, #225
	bl	Func_080041d8
	lsls	r5, r5, #7
	movs	r3, #0
	mov	sl, r3
	movs	r6, #63
	add	r5, r9
.L1:
	bl	Func_08004458
	ands	r0, r6
	adds	r0, #64
	str	r0, [r5, #0]
	bl	Func_08004458
	movs	r7, #1
	ands	r0, r6
	subs	r0, #80
	add	sl, r7
	str	r0, [r5, #4]
	mov	r0, sl
	adds	r5, #28
	cmp	r0, #32
	bne.n	.L1
	movs	r1, #0
	movs	r2, #128
	ldr	r3, [pc, #580]
	mov	sl, r1
	lsls	r2, r2, #2
	subs	r1, #1
.L2:
	movs	r7, #1
	add	sl, r7
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	sl, r2
	bne.n	.L2
	movs	r0, #171
	bl	Func_080f9010
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, #0
	str	r1, [sp, #16]
	mov	fp, r0
.L14:
	mov	r2, fp
	cmp	r2, #56
	bne.n	.L3
	movs	r0, #133
	bl	Func_080b50e8
.L3:
	mov	r3, fp
	cmp	r3, #95
	bgt.n	.L4
	ldr	r0, [sp, #16]
	bl	Func_08002322
	mov	r7, fp
	lsls	r3, r7, #1
	movs	r5, #64
	subs	r5, r5, r3
	adds	r6, r5, #0
	muls	r6, r0
	ldr	r0, [sp, #16]
	bl	Func_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	movs	r2, #20
	asrs	r6, r6, #17
	asrs	r3, r3, #16
	adds	r6, #86
	str	r2, [sp, #0]
	movs	r2, #40
	str	r2, [sp, #4]
	adds	r3, #28
	ldr	r4, [sp, #32]
	ldr	r0, [sp, #28]
	mov	r1, r9
	adds	r2, r6, #0
	bl	Func_080072f4
.L4:
	movs	r1, #225
	movs	r0, #0
	lsls	r1, r1, #7
	add	r1, r9
	str	r0, [sp, #12]
	mov	sl, r0
	mov	r8, r1
.L9:
	mov	r2, sl
	lsls	r3, r2, #2
	adds	r3, #8
	cmp	fp, r3
	blt.n	.L5
	mov	r7, r8
	ldr	r3, [r7, #4]
	cmp	r3, #95
	bgt.n	.L5
	movs	r1, #40
	ldr	r2, [r7, #0]
	str	r1, [sp, #0]
	movs	r1, #64
	str	r1, [sp, #4]
	movs	r1, #200
	lsls	r1, r1, #2
	subs	r3, #32
	subs	r2, #20
	ldr	r4, [sp, #32]
	ldr	r0, [sp, #28]
	add	r1, r9
	bl	Func_080072f4
	ldr	r3, [r7, #0]
	subs	r3, #6
	str	r3, [r7, #0]
	ldr	r3, [r7, #4]
	adds	r3, #12
	str	r3, [r7, #4]
	cmp	r3, #95
	ble.n	.L5
	ldr	r0, [sp, #12]
	ldr	r1, [pc, #396]
	movs	r4, #0
	adds	r7, r0, r1
.L6:
	str	r4, [sp, #8]
	bl	Func_08004458
	ldr	r3, [pc, #388]
	adds	r6, r0, #0
	ands	r6, r3
	bl	Func_08004458
	ldr	r5, [pc, #384]
	ands	r5, r0
	mov	r0, r8
	ldr	r3, [r0, #0]
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ldr	r3, [r0, #4]
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #1
	str	r3, [r7, #4]
	adds	r0, r6, #0
	adds	r5, r5, r2
	bl	Func_08002322
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	Func_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	Func_08004458
	movs	r3, #15
	ldr	r4, [sp, #8]
	ands	r3, r0
	adds	r3, #32
	adds	r4, #1
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r4, #32
	bne.n	.L6
	movs	r0, #133
	bl	Func_080f9010
	ldr	r2, [pc, #308]
	movs	r3, #4
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r3, [pc, #304]
	mov	r1, r9
	ldr	r3, [r1, r3]
	ldr	r3, [r3, #20]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L5
	ldr	r5, [pc, #288]
	movs	r6, #36
	add	r5, r9
.L7:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r4, #0
	movs	r2, #5
	str	r4, [sp, #8]
	bl	Func_080d6888
	ldr	r3, [r5, #0]
	movs	r1, #6
	ldrsh	r0, [r3, r6]
	bl	Func_080b5088
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #8]
	ldr	r3, [r3, #20]
	adds	r4, #1
	adds	r6, #2
	cmp	r4, r3
	bne.n	.L7
.L5:
	ldr	r1, [sp, #12]
	movs	r2, #224
	movs	r3, #1
	lsls	r2, r2, #2
	add	sl, r3
	movs	r0, #28
	adds	r1, r1, r2
	mov	r7, sl
	add	r8, r0
	str	r1, [sp, #12]
	cmp	r7, #8
	beq.n	.L8
	b.n	.L9
.L8:
	movs	r0, #0
	ldr	r5, [pc, #196]
	ldr	r6, [pc, #212]
	mov	sl, r0
.L12:
	movs	r1, #1
	ldr	r0, [r5, #24]
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L10
	cmp	r0, #0
	bge.n	.L11
	adds	r0, #15
.L11:
	asrs	r0, r0, #4
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r2, [sp, #24]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #20]
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #28]
	bl	Func_080072f4
	movs	r2, #128
	adds	r0, r5, #0
	movs	r1, #62
	lsls	r2, r2, #6
	bl	Func_080e3908
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L10:
	movs	r1, #1
	movs	r2, #128
	add	sl, r1
	lsls	r2, r2, #2
	adds	r5, #28
	cmp	sl, r2
	bne.n	.L12
	movs	r1, #4
	movs	r0, #4
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r2, [pc, #108]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r7, [pc, #96]
	ldr	r3, [sp, #16]
	movs	r0, #1
	add	fp, r0
	adds	r3, r3, r7
	mov	r1, fp
	str	r3, [sp, #16]
	cmp	r1, #96
	beq.n	.L13
	b.n	.L14
.L13:
	ldr	r0, [pc, #40]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x02010018
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x000001ff
	.4byte 0x000077a8
	.4byte 0x00007828
	.4byte 0x080ede48
	.4byte 0x00007824
	.4byte 0xfffff800
