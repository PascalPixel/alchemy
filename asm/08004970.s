@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08004970
	.thumb_func
Func_08004970:
	push	{lr}
	ldr	r1, [pc, #44]
	adds	r3, r0, #3
	lsrs	r3, r3, #2
	ldr	r2, [r1, #0]
	lsls	r0, r3, #2
	movs	r4, #129
	adds	r3, r2, r0
	lsls	r4, r4, #18
	cmp	r3, r4
	bcc.n	.L0
	ldr	r2, [r1, #4]
	ldr	r3, [pc, #24]
	adds	r0, r2, r0
	cmp	r0, r3
	bls.n	.L1
	movs	r0, #0
	b.n	.L2
.L1:
	str	r0, [r1, #4]
	b.n	.L3
.L0:
	str	r3, [r1, #0]
.L3:
	adds	r0, r2, #0
.L2:
	pop	{r1}
	bx	r1
	.4byte 0x03001e50
	.4byte 0x030077ff
	bx	lr
