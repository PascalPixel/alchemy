@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b06c0
	.thumb_func
Func_080b06c0:
	push	{lr}
	lsls	r3, r1, #4
	adds	r1, r3, #1
	cmp	r0, #0
	ble.n	.L0
	ldr	r4, [pc, #28]
.L1:
	ldrh	r3, [r4, #0]
	subs	r0, #1
	adds	r3, r2, r3
	adds	r4, #2
	strb	r1, [r3, #4]
	strb	r1, [r3, #8]
	strb	r1, [r3, #12]
	strb	r1, [r3, #16]
	strb	r1, [r3, #20]
	strb	r1, [r3, #24]
	cmp	r0, #0
	bne.n	.L1
.L0:
	pop	{r0}
	bx	r0
	.4byte 0x080b4100
