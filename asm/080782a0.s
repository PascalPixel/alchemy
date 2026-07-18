@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080782a0
	.thumb_func
Func_080782a0:
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r2, #52
	ldrsh	r3, [r5, r2]
	adds	r0, r3, #0
	cmp	r1, r3
	bgt.n	.L0
	movs	r0, #0
	cmp	r1, #0
	blt.n	.L0
	adds	r0, r1, #0
.L0:
	strh	r0, [r5, #56]
	lsls	r0, r0, #16
	movs	r3, #52
	ldrsh	r1, [r5, r3]
	asrs	r0, r0, #2
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L1
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L1
	adds	r3, r0, #0
.L1:
	strh	r3, [r5, #20]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L2
	movs	r2, #56
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L2
	movs	r3, #1
	strh	r3, [r5, #20]
.L2:
	movs	r3, #58
	ldrsh	r0, [r5, r3]
	movs	r2, #54
	ldrsh	r1, [r5, r2]
	lsls	r0, r0, #14
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L3
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L3
	adds	r3, r0, #0
.L3:
	strh	r3, [r5, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L4
	movs	r2, #58
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L4
	movs	r3, #1
	strh	r3, [r5, #22]
.L4:
	pop	{r5}
	pop	{r0}
	bx	r0
