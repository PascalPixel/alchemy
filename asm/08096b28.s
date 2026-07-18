@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08096b28
	.thumb_func
Func_08096b28:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r6, r2, #0
	adds	r0, r1, #0
	cmp	r5, #0
	beq.n	.L0
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq.n	.L1
	movs	r2, #128
	lsls	r2, r2, #9
	cmp	r3, r2
	bge.n	.L2
	bl	Func_080916b0
	ldr	r0, [r5, #8]
	bl	Func_08092b94
	adds	r0, r6, #0
	movs	r1, #0
	bl	Func_08092f84
	bl	Func_08091750
	b.n	.L1
.L2:
	adds	r1, r6, #0
	bl	Func_080072f0
.L1:
	movs	r0, #161
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L0
	bl	Func_080916b0
	ldr	r0, [pc, #16]
	movs	r1, #1
	bl	Func_08015040
	bl	Func_08091750
.L0:
	movs	r0, #0
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x00000927
