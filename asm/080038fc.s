@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080038fc
	.thumb_func
Func_080038fc:
	push	{r5, r6, lr}
	ldr	r4, [pc, #52]
	adds	r6, r0, #0
	adds	r0, r1, #0
	ldr	r1, [pc, #48]
	ldrh	r3, [r1, #0]
	adds	r5, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r4, #0]
	cmp	r2, #31
	bgt.n	.L0
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r4
	adds	r3, #4
	stmia	r3!, {r0}
	adds	r2, #1
	strh	r2, [r4, #0]
	stmia	r3!, {r6}
	movs	r2, #160
	lsls	r2, r2, #11
	str	r2, [r3, #0]
.L0:
	strh	r5, [r1, #0]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02002090
	.4byte 0x04000208
