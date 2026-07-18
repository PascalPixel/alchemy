@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ad69c
	.thumb_func
Func_080ad69c:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #44]
	ldr	r1, [pc, #44]
	ldr	r2, [r3, #0]
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	movs	r6, #0
	cmp	r6, r3
	bge.n	.L0
	movs	r3, #138
	lsls	r3, r3, #1
	adds	r7, r2, r1
	adds	r5, r2, r3
.L1:
	ldmia	r5!, {r0}
	movs	r1, #1
	bl	Func_08009020
	ldrb	r3, [r7, #0]
	adds	r6, #1
	cmp	r6, r3
	blt.n	.L1
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
	.4byte 0x00000219
