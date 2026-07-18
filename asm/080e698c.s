@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080e698c
	.thumb_func
Func_080e698c:
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
	ldr	r6, [pc, #92]
	ldr	r3, [r3, #0]
	sub	sp, #36
	mov	r9, r1
	str	r3, [sp, #12]
	add	r6, r9
	str	r0, [r6, #0]
	movs	r0, #0
	ldr	r5, [r2, #8]
	bl	Func_080cd594
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #60]
	ldr	r0, [pc, #76]
	strh	r3, [r2, #0]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	ldr	r0, [pc, #64]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r1, #250
	lsls	r1, r1, #6
	movs	r2, #1
	ldr	r0, [pc, #52]
	add	r1, r9
	movs	r3, #0
	bl	Func_080e0524
	ldr	r3, [r6, #0]
	add	r1, sp, #16
	ldr	r0, [r3, #4]
	bl	Func_080cef64
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	b.n	.L0
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x00000073
	.4byte 0x00000061
	.4byte 0x0000006d
.L0:
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #732]
	movs	r3, #50
	add	r2, r9
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #724]
	bl	Func_080041d8
	movs	r2, #0
	mov	r8, r2
	ldr	r3, [pc, #720]
	movs	r2, #128
	movs	r1, #0
	lsls	r2, r2, #3
.L1:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L1
	ldr	r5, [pc, #704]
	add	r5, r9
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	Func_080b5098
	ldr	r3, [r5, #0]
	ldr	r6, [r0, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	bl	Func_080b5098
	ldr	r0, [r0, #0]
	ldr	r2, [r6, #8]
	ldr	r3, [r0, #8]
	subs	r3, r3, r2
	mov	r8, r0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r1, #100
	lsls	r0, r0, #4
	mov	sl, r2
	bl	Func_080022ec
	mov	r4, r8
	ldr	r3, [r4, #16]
	adds	r5, r0, #0
	ldr	r0, [r6, #16]
	subs	r3, r3, r0
	mov	r8, r0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r1, #100
	lsls	r0, r0, #4
	bl	Func_080022ec
	add	sl, r5
	add	r8, r0
	asrs	r5, r5, #8
	asrs	r0, r0, #8
	adds	r2, r0, #0
	muls	r2, r0
	adds	r3, r5, #0
	muls	r3, r5
	adds	r3, r3, r2
	adds	r0, r3, #0
	ldr	r2, [pc, #616]
	bl	Func_080072ec
	movs	r1, #20
	lsls	r0, r0, #8
	bl	Func_080022ec
	adds	r3, r6, #0
	movs	r2, #1
	adds	r3, #88
	str	r0, [r6, #52]
	str	r0, [r6, #48]
	strb	r2, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #11
	str	r3, [r6, #40]
	ldr	r3, [pc, #588]
	str	r3, [r6, #72]
	adds	r3, r6, #0
	movs	r1, #0
	adds	r3, #90
	str	r1, [r6, #68]
	adds	r0, r6, #0
	strb	r2, [r3, #0]
	bl	Func_08009140
	mov	r1, sl
	movs	r2, #0
	mov	r3, r8
	adds	r0, r6, #0
	bl	Func_08009150
	movs	r1, #2
	adds	r0, r6, #0
	bl	Func_08009080
	ldr	r3, [pc, #540]
	movs	r1, #0
	add	r2, sp, #24
	add	r3, r9
	str	r1, [sp, #8]
	mov	fp, r2
	mov	sl, r3
.L12:
	mov	r4, sl
	ldr	r3, [r4, #0]
	mov	r1, fp
	ldr	r0, [r3, #8]
	bl	Func_080e3980
	mov	r6, fp
	ldr	r2, [r6, #0]
	movs	r3, #80
	subs	r3, r3, r2
	ldr	r1, [pc, #516]
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	ldr	r2, [sp, #8]
	subs	r2, #8
	cmp	r2, #15
	bhi.n	.L2
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r5, r3, #1
	cmp	r5, #6
	ble.n	.L3
	movs	r5, #6
.L3:
	mov	r0, sl
	ldr	r3, [r0, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L4
	ldr	r2, [pc, #484]
	lsls	r3, r5, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #484]
	ldrsb	r2, [r3, r5]
	ldr	r3, [pc, #484]
	mov	r4, fp
	ldr	r0, [r4, #4]
	ldrsb	r3, [r3, r5]
	adds	r3, r3, r0
	ldr	r0, [pc, #476]
	ldrb	r0, [r0, r5]
	str	r0, [sp, #0]
	ldr	r0, [pc, #476]
	ldrb	r0, [r0, r5]
	add	r1, r9
	str	r0, [sp, #4]
	adds	r2, #30
	subs	r3, #60
	ldr	r4, [sp, #16]
	ldr	r0, [sp, #12]
	bl	Func_080072f4
	b.n	.L2
.L4:
	ldr	r2, [pc, #436]
	lsls	r3, r5, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #436]
	ldrsb	r2, [r3, r5]
	ldr	r3, [pc, #440]
	ldrb	r4, [r3, r5]
	ldr	r3, [pc, #432]
	mov	r6, fp
	ldr	r0, [r6, #4]
	ldrsb	r3, [r3, r5]
	str	r4, [sp, #0]
	adds	r3, r3, r0
	ldr	r0, [pc, #428]
	negs	r2, r2
	ldrb	r0, [r0, r5]
	subs	r2, r2, r4
	str	r0, [sp, #4]
	add	r1, r9
	adds	r2, #108
	subs	r3, #60
	ldr	r4, [sp, #16]
	ldr	r0, [sp, #12]
	bl	Func_080072f4
.L2:
	ldr	r0, [sp, #8]
	cmp	r0, #18
	bne.n	.L5
	movs	r0, #134
	bl	Func_080b50e8
	mov	r1, sl
	ldr	r3, [r1, #0]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r1, #7
	movs	r3, #0
	bl	Func_080d6888
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r1, #6
	movs	r6, #36
	ldrsh	r0, [r3, r6]
	bl	Func_080b5088
	ldr	r2, [pc, #360]
	movs	r3, #4
	add	r2, r9
	movs	r0, #0
	str	r3, [r2, #0]
	ldr	r7, [pc, #352]
	mov	r8, r0
.L6:
	bl	Func_08004458
	movs	r5, #63
	movs	r1, #128
	lsls	r1, r1, #1
	ands	r5, r0
	adds	r5, r5, r1
	bl	Func_08004458
	ldr	r3, [pc, #336]
	adds	r6, r0, #0
	ands	r6, r3
	movs	r3, #128
	lsls	r3, r3, #15
	str	r3, [r7, #0]
	movs	r3, #160
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	adds	r0, r6, #0
	bl	Func_08002322
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	Func_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	Func_08004458
	movs	r3, #15
	ands	r3, r0
	movs	r2, #1
	adds	r3, #16
	add	r8, r2
	str	r3, [r7, #24]
	mov	r3, r8
	adds	r7, #28
	cmp	r3, #16
	bne.n	.L6
.L5:
	movs	r4, #0
	ldr	r5, [pc, #256]
	mov	r8, r4
.L10:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	ble.n	.L7
	subs	r3, #1
	str	r3, [r5, #24]
	adds	r0, r5, #0
	movs	r1, #60
	movs	r2, #0
	bl	Func_080e3908
	movs	r6, #208
	ldr	r3, [r5, #4]
	lsls	r6, r6, #15
	cmp	r3, r6
	ble.n	.L8
	ldr	r3, [r5, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #16]
	b.n	.L7
.L8:
	ldr	r2, [r5, #0]
	ldr	r0, [pc, #216]
	cmp	r2, r0
	bhi.n	.L7
	cmp	r3, #0
	blt.n	.L7
	ldr	r0, [sp, #8]
	add	r0, r8
	asrs	r6, r2, #16
	asrs	r7, r3, #16
	cmp	r0, #0
	bge.n	.L9
	adds	r0, #3
.L9:
	movs	r1, #6
	asrs	r0, r0, #2
	bl	Func_080022fc
	adds	r1, r0, #0
	lsls	r1, r1, #8
	movs	r2, #250
	lsls	r2, r2, #6
	add	r1, r9
	movs	r0, #16
	adds	r1, r1, r2
	adds	r3, r7, #0
	adds	r2, r6, #0
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #8
	subs	r3, #8
	ldr	r4, [sp, #16]
	ldr	r0, [sp, #12]
	bl	Func_080072f4
.L7:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r5, #28
	cmp	r4, #128
	bne.n	.L10
	movs	r0, #8
	movs	r1, #8
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r2, [pc, #128]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r6, [sp, #8]
	adds	r6, #1
	str	r6, [sp, #8]
	cmp	r6, #70
	beq.n	.L11
	b.n	.L12
.L11:
	ldr	r0, [pc, #44]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x02010018
	.4byte 0x00007828
	.4byte 0x030001d8
	.4byte 0x0000deb8
	.4byte 0x04000028
	.4byte 0x080eee02
	.4byte 0x080eee10
	.4byte 0x080eee17
	.4byte 0x080eedf4
	.4byte 0x080eedfb
	.4byte 0x000077a8
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x007effff
	.4byte 0x00007824
