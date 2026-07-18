@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800d7b4
	.thumb_func
Func_0800d7b4:
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r2, #4
	ldrsh	r3, [r5, r2]
	ldr	r2, [r5, #0]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #4]
	adds	r3, r5, #0
	adds	r3, #87
	ldrb	r3, [r3, #0]
	ldrh	r0, [r5, #4]
	cmp	r3, #0
	bne.n	.L0
	adds	r0, r5, #0
	bl	Func_0800d6d8
	strh	r0, [r5, #4]
	b.n	.L1
.L0:
	adds	r3, r0, #2
	strh	r3, [r5, #4]
.L1:
	movs	r0, #1
	pop	{r5}
	pop	{r1}
	bx	r1
