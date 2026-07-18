@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080bd7a4
	.thumb_func
Func_080bd7a4:
	push	{lr}
	movs	r2, #132
	ldr	r3, [pc, #40]
	movs	r0, #0
	movs	r1, #0
	lsls	r2, r2, #24
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	movs	r0, #0
	lsls	r2, r2, #24
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	movs	r0, #0
	lsls	r2, r2, #24
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #12]
	ldr	r0, [r3, #0]
	bl	Func_080072e4
	pop	{r0}
	bx	r0
	.4byte 0x040000d4
	.4byte 0x030000c4
