@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080170c4
	.thumb_func
Func_080170c4:
	push	{r5, lr}
	adds	r4, r2, #0
	sub	sp, #4
	adds	r5, r0, #0
	cmp	r4, #0
	ble.n	.L0
	mov	r0, sp
	movs	r2, #129
	adds	r0, #2
	lsls	r2, r2, #24
	strh	r1, [r0, #0]
	ldr	r3, [pc, #24]
	adds	r1, r5, #0
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	lsls	r3, r4, #1
	adds	r5, r5, r3
.L0:
	adds	r0, r5, #0
	add	sp, #4
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x040000d4
