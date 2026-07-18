@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a65e4
	.thumb_func
Func_080a65e4:
	push	{lr}
	ldr	r3, [pc, #32]
	lsls	r0, r0, #10
	ands	r3, r1
	orrs	r0, r3
	cmp	r2, #0
	bne.n	.L0
	ldr	r3, [pc, #24]
	movs	r2, #136
	lsls	r2, r2, #2
	b.n	.L1
.L0:
	ldr	r3, [pc, #16]
	ldr	r2, [pc, #16]
.L1:
	adds	r3, r3, r2
	strh	r0, [r3, #0]
	movs	r0, #1
	pop	{r1}
	bx	r1
	.4byte 0x00003fff
	.4byte 0x02000240
	.4byte 0x00000222
