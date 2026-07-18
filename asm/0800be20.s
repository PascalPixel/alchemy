@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800be20
	.thumb_func
Func_0800be20:
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r5, r2, #0
	bl	Func_08185000
	ldrb	r3, [r0, #5]
	movs	r7, #0
	cmp	r6, r3
	bcc.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	ldr	r2, [r0, #16]
	lsls	r3, r6, #2
	ldr	r0, [r3, r2]
.L4:
	ldrb	r2, [r0, #0]
	ldrb	r3, [r0, #1]
	adds	r0, #2
	cmp	r2, #254
	beq.n	.L2
	cmp	r2, #241
	beq.n	.L2
	cmp	r2, #253
	beq.n	.L2
	cmp	r2, #239
	beq.n	.L2
	cmp	r2, #245
	beq.n	.L3
	cmp	r2, #255
	beq.n	.L3
	cmp	r2, #238
	bhi.n	.L4
.L3:
	subs	r5, #1
	adds	r7, r7, r3
	cmp	r5, #0
	bne.n	.L4
.L2:
	adds	r0, r7, #0
.L1:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
