@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809b5dc
	.thumb_func
Func_0809b5dc:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #100
	movs	r1, #0
	ldrsh	r6, [r3, r1]
	adds	r1, r5, #0
	adds	r1, #102
	ldrh	r3, [r1, #0]
	adds	r2, r3, #1
	lsls	r3, r3, #16
	strh	r2, [r1, #0]
	asrs	r0, r3, #16
	movs	r2, #237
	ldr	r3, [pc, #68]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L0
	movs	r1, #7
	bl	Func_080022fc
	cmp	r0, #0
	bne.n	.L1
	adds	r0, r5, #0
	bl	Func_0809b450
	b.n	.L1
.L0:
	movs	r1, #5
	bl	Func_080022fc
	cmp	r0, #0
	bne.n	.L1
	adds	r0, r5, #0
	bl	Func_0809b450
.L1:
	cmp	r6, #1
	bne.n	.L2
	ldrh	r3, [r5, #6]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r3, r3, r2
	strh	r3, [r5, #6]
.L2:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x00000001
