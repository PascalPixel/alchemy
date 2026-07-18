@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08091254
	.thumb_func
Func_08091254:
	push	{r5, lr}
	ldr	r3, [pc, #52]
	ldr	r4, [r3, #0]
	adds	r5, r0, #0
	cmp	r4, #0
	beq.n	.L0
	ldr	r1, [pc, #44]
	adds	r3, r4, r1
	adds	r1, #1
	movs	r2, #0
	strb	r5, [r3, #0]
	adds	r3, r4, r1
	strb	r2, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #4
	movs	r2, #224
	adds	r1, r4, r3
	movs	r3, #196
	lsls	r2, r2, #2
	lsls	r3, r3, #5
	adds	r0, r4, r2
	adds	r2, r4, r3
	adds	r3, r5, #0
	bl	Func_0809088c
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x03001ed0
	.4byte 0x00002a01
