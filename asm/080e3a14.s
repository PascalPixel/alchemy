@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080e3a14
	.thumb_func
Func_080e3a14:
	push	{lr}
	ldr	r3, [pc, #24]
	ldr	r2, [pc, #24]
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	movs	r2, #36
	ldrsh	r3, [r3, r2]
	cmp	r3, #127
	ble.n	.L0
	ldr	r3, [pc, #12]
	ldr	r3, [r3, #0]
.L0:
	pop	{r0}
	bx	r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x03001ae8
