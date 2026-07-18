@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.pyでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_08004838
	.thumb_func
Func_08004838:
	ldr	r3, [pc, #12]
	ldr	r0, [pc, #16]
	ldr	r1, [pc, #16]
	ldr	r2, [pc, #20]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	movs	r0, r0
	.4byte 0x040000d4
	.4byte 0x0800779c
	.4byte 0x05000200
	.4byte 0x800000e0
