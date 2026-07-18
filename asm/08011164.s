@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08011164
	.thumb_func
Func_08011164:
	push	{r5, lr}
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	movs	r2, #31
	asrs	r3, r3, #1
	ands	r3, r2
	ldr	r2, [pc, #52]
	lsls	r3, r3, #2
	adds	r4, r3, r2
	ldr	r5, [pc, #52]
	movs	r3, #62
	ands	r3, r0
	adds	r1, r3, r5
	movs	r0, #0
.L0:
	ldrh	r2, [r4, #0]
	ldr	r5, [pc, #44]
	lsls	r2, r2, #2
	adds	r3, r2, r5
	ldrh	r3, [r3, #0]
	adds	r5, #2
	strh	r3, [r1, #0]
	adds	r3, r2, r5
	ldrh	r3, [r3, #0]
	adds	r2, r1, #0
	adds	r2, #64
	adds	r0, #1
	strh	r3, [r2, #0]
	adds	r1, #128
	adds	r4, #128
	cmp	r0, #63
	bls.n	.L0
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x02020000
	.4byte 0x06004000
	.4byte 0x02010000
