@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.pyでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_08079358
	.thumb_func
Func_08079358:
	movs	r3, #7
	ands	r3, r0
	movs	r2, #1
	ldr	r1, [pc, #16]
	lsls	r2, r3
	lsls	r3, r0, #20
	lsrs	r0, r3, #23
	ldrb	r3, [r1, r0]
	orrs	r2, r3
	strb	r2, [r1, r0]
	bx	lr
	movs	r0, r0
	.4byte 0x02000040
