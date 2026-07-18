@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08012078
	.thumb_func
Func_08012078:
	push	{r5, r6, lr}
	adds	r6, r3, #0
	ldr	r3, [pc, #48]
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	adds	r4, r2, #0
	asrs	r1, r1, #20
	asrs	r4, r4, #20
	cmp	r0, #0
	beq.n	.L0
	movs	r2, #3
	ands	r2, r5
	lsls	r3, r2, #1
	adds	r3, r3, r2
	movs	r2, #152
	lsls	r2, r2, #1
	lsls	r3, r3, #4
	adds	r3, r3, r2
	ldr	r2, [r0, r3]
	lsls	r3, r4, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
	strb	r6, [r2, #2]
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e70
