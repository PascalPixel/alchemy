@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b0664
	.thumb_func
Func_080b0664:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #40]
	ldr	r5, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #2
	adds	r5, r5, r3
	adds	r7, r0, #0
	ldr	r0, [r5, #0]
	ldrb	r6, [r0, #5]
	bl	Func_080a1038
	adds	r2, r7, #0
	movs	r1, #7
	movs	r0, #7
	bl	Func_080153b0
	ldr	r3, [r5, #0]
	adds	r7, r0, #0
	strb	r6, [r3, #5]
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
