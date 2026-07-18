@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800fec8
	.thumb_func
Func_0800fec8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #112]
	lsls	r0, r0, #11
	adds	r3, r3, r0
	mov	ip, r3
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	movs	r4, #127
	asrs	r3, r3, #1
	ands	r3, r4
	movs	r0, #30
	ands	r2, r0
	lsls	r7, r3, #7
	lsrs	r3, r1, #31
	lsls	r2, r2, #5
	adds	r3, r1, r3
	mov	sl, r2
	asrs	r5, r3, #1
	movs	r6, #127
	movs	r2, #30
	ands	r5, r4
	ands	r0, r1
	movs	r4, #0
	mov	r8, r6
	mov	lr, r2
.L0:
	adds	r3, r7, r5
	ldr	r6, [pc, #68]
	lsls	r3, r3, #2
	adds	r3, r3, r6
	ldr	r1, [r3, #0]
	ldr	r2, [pc, #64]
	lsls	r1, r1, #20
	lsrs	r1, r1, #17
	mov	r6, sl
	adds	r3, r1, r2
	adds	r2, r6, r0
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	add	r2, ip
	ldr	r6, [pc, #48]
	str	r3, [r2, #0]
	adds	r3, r1, r6
	ldr	r3, [r3, #0]
	adds	r5, #1
	str	r3, [r2, #64]
	adds	r0, #2
	mov	r2, r8
	mov	r3, lr
	adds	r4, #1
	ands	r5, r2
	ands	r0, r3
	cmp	r4, #15
	bls.n	.L0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x06002800
	.4byte 0x02010000
	.4byte 0x02020000
	.4byte 0x02020004
