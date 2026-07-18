@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809641c
	.thumb_func
Func_0809641c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #320]
	movs	r1, #250
	lsls	r1, r1, #1
	adds	r3, r3, r1
	adds	r7, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl	Func_08092054
	movs	r2, #64
	adds	r2, r2, r7
	movs	r1, #0
	ldrsb	r1, [r2, r1]
	adds	r5, r0, #0
	mov	r8, r2
	ldrb	r3, [r2, #0]
	mov	sl, r1
	cmp	r1, #0
	bne.n	.L0
	ldr	r3, [r5, #8]
	mov	r6, sp
	str	r3, [r6, #0]
	bl	Func_08004458
	ldr	r3, [r5, #12]
	lsls	r2, r0, #2
	adds	r2, r2, r0
	adds	r3, r3, r2
	movs	r2, #240
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	adds	r0, r6, #0
	str	r3, [r6, #8]
	bl	Func_080974d8
	bl	Func_08004458
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r3, #128
	lsls	r3, r3, #10
	lsls	r5, r5, #1
	adds	r5, r5, r3
	bl	Func_08004458
	adds	r2, r6, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_0800447c
	ldr	r2, [r6, #0]
	str	r2, [r7, #12]
	ldr	r1, [pc, #216]
	ldr	r3, [r6, #8]
	movs	r5, #192
	str	r3, [r7, #16]
	lsls	r5, r5, #10
	adds	r3, r3, r1
	str	r2, [r7, #4]
	str	r3, [r7, #8]
	str	r5, [r7, #36]
	bl	Func_08004458
	lsls	r3, r0, #1
	adds	r3, r3, r0
	adds	r3, r3, r5
	str	r3, [r7, #32]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #40]
	str	r3, [r7, #44]
	adds	r3, r7, #0
	adds	r3, #66
	mov	r2, sl
	strb	r2, [r3, #0]
	adds	r2, r7, #0
	movs	r3, #1
	adds	r2, #65
	strb	r3, [r2, #0]
	mov	r1, r8
	ldrb	r3, [r1, #0]
	adds	r3, #1
	strb	r3, [r1, #0]
	b.n	.L1
.L0:
	subs	r3, #1
	movs	r2, #128
	lsls	r3, r3, #24
	lsls	r2, r2, #17
	cmp	r3, r2
	bhi.n	.L2
	adds	r0, r7, #0
	bl	Func_0809ba34
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L1
	ldr	r3, [r7, #4]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r7, #8]
	str	r3, [r5, #8]
	bl	Func_08004458
	adds	r1, r0, #0
	movs	r0, #192
	adds	r2, r5, #0
	lsls	r0, r0, #12
	bl	Func_0800447c
	ldr	r3, [r5, #0]
	str	r3, [r7, #12]
	ldr	r3, [r5, #8]
	str	r3, [r7, #16]
	adds	r3, r7, #0
	adds	r3, #65
	strb	r6, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #28]
	str	r6, [r7, #36]
	bl	Func_08004458
	ldr	r3, [pc, #80]
	adds	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #8
	str	r0, [r7, #32]
	str	r3, [r7, #40]
	str	r3, [r7, #44]
	movs	r0, #143
	bl	Func_080f9010
	mov	r1, r8
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	ldrb	r2, [r1, #0]
	cmp	r3, #1
	bne.n	.L3
	subs	r3, r2, #1
	strb	r3, [r1, #0]
	b.n	.L4
.L3:
	adds	r3, r2, #1
	mov	r2, r8
	strb	r3, [r2, #0]
.L4:
	movs	r3, #6
	strh	r3, [r7, #58]
	b.n	.L1
.L2:
	mov	r3, sl
	cmp	r3, #3
	bne.n	.L1
	adds	r0, r7, #0
	bl	Func_0809bb34
.L1:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0xff9c0000
	.4byte 0x00023333
	ldr	r4, [r0, #104]
	ldr	r1, [r0, #8]
	ldr	r3, [r4, #8]
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r1, r1, r3
	str	r1, [r0, #8]
	ldr	r1, [r0, #12]
	ldr	r3, [r4, #12]
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r1, r1, r3
	str	r1, [r0, #12]
	ldr	r1, [r0, #16]
	ldr	r3, [r4, #16]
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r1, r1, r3
	str	r1, [r0, #16]
	bx	lr
