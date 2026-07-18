@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809b7f8
	.thumb_func
Func_0809b7f8:
	push	{r0, r2, r3, r4, r6, r7, lr}
	lsrs	r1, r1, #32
	push	{r0, r3, r7, lr}
	lsrs	r1, r1, #32
	lsrs	r2, r4, #4
	movs	r0, r0
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r5, #0
	adds	r6, #69
	movs	r3, #0
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	beq.n	.L0
	ldrh	r3, [r5, #56]
	adds	r3, #1
	strh	r3, [r5, #56]
	movs	r1, #58
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r5, #58]
	cmp	r3, #0
	beq.n	.L1
	subs	r3, r2, #1
	strh	r3, [r5, #58]
	b.n	.L2
.L1:
	ldr	r3, [r5, #52]
	cmp	r3, #0
	beq.n	.L2
	adds	r0, r5, #0
	bl	Func_080072f0
.L2:
	movs	r3, #0
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	beq.n	.L0
	adds	r3, r5, #0
	adds	r3, #67
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L3
	adds	r0, r5, #0
	bl	Func_0809b8f4
.L3:
	adds	r3, r5, #0
	adds	r3, #68
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L0
	adds	r0, r5, #0
	bl	Func_0809b86c
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
