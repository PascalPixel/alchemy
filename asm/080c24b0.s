@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c24b0
	.thumb_func
Func_080c24b0:
	push	{lr}
	ldr	r3, [pc, #48]
	movs	r2, #166
	ldr	r0, [r3, #0]
	lsls	r2, r2, #3
	adds	r1, r0, r2
	ldr	r3, [pc, #40]
	movs	r2, #143
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r2, #0
	strh	r2, [r3, #0]
	str	r2, [r1, #0]
	str	r2, [r1, #4]
	str	r2, [r1, #8]
	ldr	r1, [pc, #28]
	movs	r3, #3
	adds	r0, r0, r1
.L0:
	subs	r3, #1
	strh	r2, [r0, #0]
	subs	r0, #2
	cmp	r3, #0
	bge.n	.L0
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e74
	.4byte 0x02000240
	.4byte 0x00000542
