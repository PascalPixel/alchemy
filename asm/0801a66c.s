@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801a66c
	.thumb_func
Func_0801a66c:
	push	{r5, r6, r7, lr}
	movs	r1, #249
	lsls	r1, r1, #2
	movs	r0, #18
	bl	Func_080048f4
	movs	r2, #210
	adds	r7, r0, #0
	lsls	r2, r2, #2
	movs	r5, #0
	adds	r3, r7, r2
	adds	r2, #4
	str	r5, [r3, #0]
	adds	r3, r7, r2
	adds	r2, #4
	str	r5, [r3, #0]
	adds	r3, r7, r2
	adds	r2, #74
	str	r5, [r3, #0]
	adds	r3, r7, r2
	adds	r2, #2
	strh	r5, [r3, #0]
	adds	r3, r7, r2
	strh	r5, [r3, #0]
	ldr	r3, [pc, #204]
	adds	r2, r7, r3
	movs	r3, #128
	strh	r3, [r2, #0]
	movs	r3, #232
	lsls	r3, r3, #2
	adds	r2, r7, r3
	movs	r3, #32
	strh	r3, [r2, #0]
	movs	r2, #229
	lsls	r2, r2, #2
	adds	r3, r7, r2
	strh	r5, [r3, #0]
	movs	r3, #238
	lsls	r3, r3, #2
	adds	r2, r7, r3
	adds	r3, #47
	strh	r3, [r2, #0]
	movs	r3, #239
	lsls	r3, r3, #1
	adds	r2, r7, r3
	adds	r3, r7, #0
	movs	r1, #0
	movs	r0, #0
	adds	r3, #114
.L0:
	adds	r1, #1
	strh	r0, [r3, #0]
	strh	r0, [r2, #0]
	adds	r3, #52
	adds	r2, #52
	cmp	r1, #5
	bne.n	.L0
	movs	r2, #186
	lsls	r2, r2, #1
	movs	r5, #0
	adds	r3, r7, r2
	adds	r2, #52
	strh	r5, [r3, #2]
	adds	r3, r7, r2
	strh	r5, [r3, #2]
	adds	r3, r7, #0
	adds	r3, #70
	strh	r5, [r7, #10]
	strh	r5, [r7, #62]
	strh	r5, [r7, #18]
	strh	r5, [r3, #0]
	ldr	r6, [pc, #116]
	bl	Func_08004080
	movs	r2, #185
	lsls	r2, r2, #2
	adds	r3, r7, r2
	strh	r0, [r3, #0]
	movs	r1, #128
	adds	r2, r6, #0
	ldrh	r0, [r3, #0]
	lsls	r1, r1, #1
	bl	Func_08003fa4
	ldr	r2, [pc, #96]
	adds	r3, r7, r2
	subs	r2, #4
	strh	r0, [r3, #0]
	adds	r3, r7, r2
	adds	r2, #24
	strh	r5, [r3, #0]
	adds	r3, r7, r2
	adds	r2, #28
	strh	r5, [r3, #0]
	adds	r3, r7, r2
	strh	r5, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r5, r7, r3
	movs	r0, #13
	ldrb	r3, [r5, #5]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r2, r3
	movs	r3, #17
	negs	r3, r3
	ands	r2, r3
	movs	r3, #32
	orrs	r2, r3
	movs	r3, #4
	ldrb	r1, [r5, #7]
	negs	r3, r3
	ands	r2, r3
	subs	r3, #59
	movs	r4, #63
	ands	r3, r1
	ands	r3, r4
	movs	r1, #64
	orrs	r3, r1
	strb	r3, [r5, #7]
	ldrb	r3, [r5, #9]
	ands	r2, r4
	ands	r0, r3
	strb	r2, [r5, #5]
	strb	r0, [r5, #9]
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000039e
	.4byte 0x080346f8
	.4byte 0x000002e6
