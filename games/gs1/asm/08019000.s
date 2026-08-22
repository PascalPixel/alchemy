@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08019000
	.thumb_func
Func_08019000:
	push	{r5, r6, r7, lr}
	adds	r4, r3, #0
	ldr	r3, [pc, #128]
	ldr	r6, [r3, #0]
	ldrh	r3, [r0, #10]
	adds	r7, r1, #0
	adds	r4, #1
	adds	r1, r2, #0
	subs	r3, #1
	ldr	r2, [sp, #16]
	adds	r1, #1
	cmp	r4, r3
	bhi.n	.L0
	ldrh	r3, [r0, #8]
	subs	r3, #1
	cmp	r1, r3
	bhi.n	.L0
	movs	r5, #240
	lsls	r5, r5, #8
	cmp	r2, #3
	beq.n	.L1
	cmp	r2, #3
	bhi.n	.L2
	movs	r5, #224
	lsls	r5, r5, #8
	cmp	r2, #2
	beq.n	.L1
	b.n	.L3
.L2:
	movs	r5, #128
	lsls	r5, r5, #5
	cmp	r2, #4
	beq.n	.L1
.L3:
	movs	r5, #0
.L1:
	cmp	r2, #1
	beq.n	.L0
	cmp	r2, #1
	bcc.n	.L4
	cmp	r2, #4
	bhi.n	.L4
	ldrh	r2, [r0, #14]
	ldrh	r3, [r0, #12]
	adds	r2, r2, r4
	adds	r3, r3, r1
	lsls	r2, r2, #5
	adds	r0, r2, r3
	movs	r3, #160
	lsls	r3, r3, #2
	cmp	r0, r3
	bcs.n	.L0
	lsls	r3, r0, #1
	orrs	r5, r7
	strh	r5, [r6, r3]
	b.n	.L0
.L4:
	ldrh	r2, [r0, #14]
	ldrh	r3, [r0, #12]
	adds	r2, r2, r4
	adds	r3, r3, r1
	lsls	r2, r2, #5
	adds	r0, r2, r3
	movs	r3, #160
	lsls	r3, r3, #2
	cmp	r0, r3
	bcs.n	.L0
	lsls	r3, r0, #1
	strh	r7, [r6, r3]
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
