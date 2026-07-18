@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809ba90
	.thumb_func
Func_0809ba90:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	adds	r5, r0, #0
	adds	r6, r2, #0
	mov	r0, sp
	movs	r2, #0
	adds	r4, r1, #0
	adds	r7, r3, #0
	mov	r8, r2
	str	r2, [r0, #0]
	ldr	r3, [pc, #128]
	adds	r1, r5, #0
	ldr	r2, [pc, #128]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r4, #0
	bl	Func_08009030
	str	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L0
	ldrb	r2, [r0, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r0, #9]
.L0:
	adds	r1, r6, #0
	adds	r0, r5, #0
	adds	r2, r7, #0
	bl	Func_0809ba5c
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #32]
	movs	r2, #128
	ldr	r3, [r5, #0]
	lsls	r2, r2, #9
	str	r2, [r5, #40]
	str	r2, [r5, #44]
	str	r2, [r5, #36]
	adds	r3, #38
	mov	r2, r8
	str	r6, [r5, #20]
	str	r7, [r5, #24]
	strb	r2, [r3, #0]
	adds	r3, r5, #0
	movs	r2, #1
	adds	r3, #65
	strb	r2, [r3, #0]
	adds	r3, #1
	strb	r2, [r3, #0]
	adds	r3, #1
	strb	r2, [r3, #0]
	adds	r3, #1
	strb	r2, [r3, #0]
	adds	r3, #1
	strb	r2, [r3, #0]
	bl	Func_08004458
	adds	r3, r5, #0
	adds	r3, #70
	adds	r2, r5, #0
	strb	r0, [r3, #0]
	adds	r2, #71
	movs	r3, #4
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_0809ba70
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x040000d4
	.4byte 0x85000012
