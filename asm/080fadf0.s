@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fadf0
	.thumb_func
Func_080fadf0:
	push	{r4, lr}
	adds	r1, r0, #0
	ldrb	r0, [r1, #2]
	lsls	r2, r0, #24
	lsrs	r4, r2, #24
	ldrb	r3, [r1, #3]
	lsls	r0, r3, #24
	lsrs	r3, r0, #24
	cmp	r4, r3
	bcc.n	.L0
	lsrs	r0, r2, #25
	cmp	r0, r3
	bcc.n	.L1
	movs	r0, #15
	strb	r0, [r1, #27]
	b.n	.L2
.L0:
	lsrs	r0, r0, #25
	cmp	r0, r4
	bcc.n	.L1
	movs	r0, #240
	strb	r0, [r1, #27]
	b.n	.L2
.L1:
	movs	r0, #255
	strb	r0, [r1, #27]
	ldrb	r2, [r1, #3]
	ldrb	r3, [r1, #2]
	adds	r0, r2, r3
	lsrs	r0, r0, #4
	b.n	.L3
.L2:
	ldrb	r2, [r1, #3]
	ldrb	r3, [r1, #2]
	adds	r0, r2, r3
	lsrs	r0, r0, #4
	strb	r0, [r1, #10]
	cmp	r0, #15
	bls.n	.L4
	movs	r0, #15
.L3:
	strb	r0, [r1, #10]
.L4:
	ldrb	r2, [r1, #6]
	ldrb	r3, [r1, #10]
	adds	r0, r2, #0
	muls	r0, r3
	adds	r0, #15
	asrs	r0, r0, #4
	strb	r0, [r1, #25]
	ldrb	r0, [r1, #28]
	ldrb	r2, [r1, #27]
	ands	r0, r2
	strb	r0, [r1, #27]
	pop	{r4}
	pop	{r0}
	bx	r0
