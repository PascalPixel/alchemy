@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080947e4
	.thumb_func
Func_080947e4:
	push	{lr}
	ldr	r0, [pc, #36]
	bl	Func_08004278
	ldr	r0, [pc, #32]
	bl	Func_08004278
	ldr	r2, [pc, #32]
	ldr	r3, [pc, #32]
	ldrh	r1, [r2, #10]
	ands	r3, r1
	strh	r3, [r2, #10]
	ldr	r3, [pc, #28]
	ldrh	r1, [r2, #10]
	ands	r3, r1
	strh	r3, [r2, #10]
	ldrh	r3, [r2, #10]
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080944ed
	.4byte 0x08094545
	.4byte 0x040000b0
	.4byte 0x0000c5ff
	.4byte 0x00007fff
