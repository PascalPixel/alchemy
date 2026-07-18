@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08015ec0
	.thumb_func
Func_08015ec0:
	push	{lr}
	ldr	r3, [pc, #40]
	movs	r1, #211
	ldr	r2, [r3, #0]
	lsls	r1, r1, #3
	adds	r3, r2, r1
	cmp	r0, r3
	bcc.n	.L0
	ldr	r1, [pc, #28]
	adds	r3, r2, r1
	cmp	r0, r3
	bcs.n	.L0
	adds	r1, #4
	adds	r3, r2, r1
	ldr	r2, [r3, #0]
	str	r0, [r3, #0]
	movs	r3, #0
	str	r0, [r2, #0]
	str	r3, [r0, #0]
.L0:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x00000d98
