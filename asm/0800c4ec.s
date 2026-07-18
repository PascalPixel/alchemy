@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800c4ec
	.thumb_func
Func_0800c4ec:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	movs	r2, #4
	ldrsh	r3, [r5, r2]
	ldr	r2, [r5, #0]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	movs	r6, #0
	cmp	r3, #16
	beq.n	.L0
.L1:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [pc, #28]
	adds	r6, #1
	cmp	r6, r3
	bgt.n	.L0
	movs	r2, #4
	ldrsh	r3, [r5, r2]
	ldr	r2, [r5, #0]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	cmp	r3, #16
	bne.n	.L1
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000012b
