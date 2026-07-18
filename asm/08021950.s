@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08021950
	.thumb_func
Func_08021950:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r3, #0
	sub	sp, #8
	negs	r3, r6
	str	r0, [sp, #4]
	adds	r7, r2, #0
	movs	r0, #0
	lsls	r3, r3, #2
	lsls	r2, r6, #2
	str	r1, [sp, #0]
	mov	ip, r0
	mov	r8, r3
	mov	lr, r2
.L5:
	ldr	r0, [sp, #0]
	ldmia	r0!, {r4}
	adds	r3, r0, #0
	str	r3, [sp, #0]
	ldr	r0, [sp, #4]
	ldmia	r0!, {r1}
	adds	r3, r0, #0
	movs	r2, #0
	str	r3, [sp, #4]
	cmp	r6, #0
	bge.n	.L0
	mov	r3, r8
	lsrs	r4, r3
	b.n	.L1
.L0:
	mov	r0, lr
	lsls	r4, r0
.L1:
	ldr	r5, [pc, #52]
	movs	r0, #7
.L4:
	lsls	r2, r2, #4
	cmp	r4, r5
	bls.n	.L2
	lsrs	r3, r4, #28
	b.n	.L3
.L2:
	lsrs	r3, r1, #28
.L3:
	adds	r2, r2, r3
	subs	r0, #1
	lsls	r4, r4, #4
	lsls	r1, r1, #4
	cmp	r0, #0
	bge.n	.L4
	stmia	r7!, {r2}
	movs	r2, #1
	add	ip, r2
	mov	r3, ip
	cmp	r3, #7
	ble.n	.L5
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0fffffff
