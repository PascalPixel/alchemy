@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800d880
	.thumb_func
Func_0800d880:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r2, #4
	ldrsh	r3, [r6, r2]
	ldr	r2, [r6, #0]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r5, [r3, #4]
	adds	r0, r5, #0
	bl	Func_080770c0
	adds	r3, r6, #0
	adds	r3, #87
	strb	r0, [r3, #0]
	movs	r3, #128
	lsls	r0, r0, #24
	lsls	r3, r3, #17
	cmp	r0, r3
	bne.n	.L0
	adds	r0, r5, #0
	bl	Func_080770d0
	b.n	.L1
.L0:
	adds	r0, r5, #0
	bl	Func_080770c8
.L1:
	ldrh	r3, [r6, #4]
	adds	r3, #2
	strh	r3, [r6, #4]
	movs	r0, #1
	pop	{r5, r6}
	pop	{r1}
	bx	r1
