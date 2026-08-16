@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.tsでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_08004a44
	.thumb_func
Func_08004a44:
	ldr	r3, [pc, #8]
	ldr	r1, [pc, #12]
	ldr	r2, [pc, #12]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	.4byte 0x040000d4
	.4byte 0x03000350
	.4byte 0x8400000c
