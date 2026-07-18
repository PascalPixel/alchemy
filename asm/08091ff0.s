@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08091ff0
	.thumb_func
Func_08091ff0:
	push	{r5, lr}
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #44]
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	adds	r3, r3, r2
	strh	r5, [r3, #0]
	movs	r2, #1
	lsls	r3, r5, #16
	asrs	r3, r3, #16
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L0
	ldr	r5, [pc, #28]
.L0:
	movs	r0, #149
	lsls	r0, r0, #1
	bl	Func_080f9010
	adds	r0, r5, #0
	bl	Func_080f9010
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.4byte 0x00000cc8
	.4byte 0x00000121
