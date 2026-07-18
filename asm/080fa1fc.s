@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fa1fc
	.thumb_func
Func_080fa1fc:
	push	{r4, r5, r6, r7, lr}
	mov	ip, r0
	lsls	r1, r1, #24
	lsrs	r6, r1, #24
	lsls	r7, r2, #24
	cmp	r6, #178
	bls.n	.L0
	movs	r6, #178
	movs	r7, #255
	lsls	r7, r7, #24
.L0:
	ldr	r3, [pc, #68]
	adds	r0, r6, r3
	ldrb	r5, [r0, #0]
	ldr	r4, [pc, #68]
	movs	r2, #15
	adds	r0, r5, #0
	ands	r0, r2
	lsls	r0, r0, #2
	adds	r0, r0, r4
	lsrs	r1, r5, #4
	ldr	r5, [r0, #0]
	lsrs	r5, r1
	adds	r0, r6, #1
	adds	r0, r0, r3
	ldrb	r1, [r0, #0]
	adds	r0, r1, #0
	ands	r0, r2
	lsls	r0, r0, #2
	adds	r0, r0, r4
	lsrs	r1, r1, #4
	ldr	r0, [r0, #0]
	lsrs	r0, r1
	mov	r1, ip
	ldr	r4, [r1, #4]
	subs	r0, r0, r5
	adds	r1, r7, #0
	bl	Func_080f95e0
	adds	r1, r0, #0
	adds	r1, r5, r1
	adds	r0, r4, #0
	bl	Func_080f95e0
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x080fb830
	.4byte 0x080fb8e4
	bx	lr
