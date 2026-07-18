@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08004938
	.thumb_func
Func_08004938:
	push	{lr}
	ldr	r1, [pc, #44]
	adds	r3, r0, #3
	lsrs	r3, r3, #2
	ldr	r2, [r1, #4]
	lsls	r0, r3, #2
	ldr	r4, [pc, #36]
	adds	r3, r2, r0
	cmp	r3, r4
	bls.n	.L0
	ldr	r2, [r1, #0]
	movs	r3, #129
	adds	r0, r2, r0
	lsls	r3, r3, #18
	cmp	r0, r3
	bcc.n	.L1
	movs	r0, #0
	b.n	.L2
.L1:
	str	r0, [r1, #0]
	b.n	.L3
.L0:
	str	r3, [r1, #4]
.L3:
	adds	r0, r2, #0
.L2:
	pop	{r1}
	bx	r1
	.4byte 0x03001e50
	.4byte 0x030077ff
