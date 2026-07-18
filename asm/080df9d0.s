@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080df9d0
	.thumb_func
Func_080df9d0:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #144
	lsls	r3, r3, #1
	mov	lr, r0
	adds	r6, r1, #0
	mov	ip, r2
	movs	r7, #0
	mov	r8, r3
	movs	r5, #0
.L1:
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r0, r3, #1
	mov	r3, lr
	movs	r1, #0
	adds	r4, r5, r3
.L0:
	lsrs	r3, r1, #31
	adds	r3, r1, r3
	ldrb	r2, [r4, #0]
	asrs	r3, r3, #1
	adds	r3, r0, r3
	adds	r1, #1
	adds	r4, #1
	strb	r2, [r6, r3]
	cmp	r1, #40
	bne.n	.L0
	adds	r7, #1
	add	r5, ip
	cmp	r7, r8
	bne.n	.L1
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
