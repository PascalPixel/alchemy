@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f3844
	.thumb_func
Func_080f3844:
	push	{lr}
	ldr	r3, [pc, #12]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L0
	strh	r0, [r3, #0]
.L0:
	pop	{r0}
	bx	r0
	.4byte 0x03001ed0
