@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08002f10
	.thumb_func
Func_08002f10:
	push	{lr}
	ldr	r0, [pc, #24]
	bl	Func_08002f40
	ldr	r4, [pc, #20]
	ldr	r3, [pc, #24]
	adds	r1, r4, #0
	ldr	r2, [pc, #24]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #0
	str	r3, [r4, #12]
	pop	{r0}
	bx	r0
	.4byte 0x00000002
	.4byte 0x03007804
	.4byte 0x040000d4
	.4byte 0x84000003
