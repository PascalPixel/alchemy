@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080215e0
	.thumb_func
Func_080215e0:
	push	{r5, r6, r7, lr}
	adds	r7, r1, #0
	movs	r1, #128
	adds	r5, r0, #0
	lsls	r1, r1, #3
	movs	r0, #14
	bl	Func_080048f4
	ldr	r3, [pc, #40]
	lsls	r5, r5, #2
	adds	r6, r0, #0
	ldr	r0, [r3, r5]
	cmp	r7, #95
	bgt.n	.L0
	adds	r1, r6, #0
	bl	Func_080053e8
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r0, r7, #0
	adds	r2, r6, #0
	bl	Func_08003fa4
	movs	r0, #14
	bl	Func_08002dd8
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x08031864
