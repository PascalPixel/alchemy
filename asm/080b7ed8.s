@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b7ed8
	.thumb_func
Func_080b7ed8:
	push	{r5, lr}
	ldr	r3, [pc, #52]
	ldr	r5, [r3, #0]
	bl	Func_080049ac
	ldr	r0, [pc, #48]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L0
	ldr	r0, [pc, #40]
	ldr	r3, [pc, #44]
	bl	Func_080072f0
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	Func_080051e8
	b.n	.L1
.L0:
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	Func_080051d8
.L1:
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x03001e80
	.4byte 0x0000016b
	.4byte 0x080c2a7c
	.4byte 0x030002c0
