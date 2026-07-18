@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.pyでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_08011e2c
	.thumb_func
Func_08011e2c:
	adds	r3, r0, #0
	movs	r0, #0
	ldrsb	r0, [r3, r0]
	movs	r4, #1
	ldrsb	r4, [r3, r4]
	lsls	r2, r2, #4
	ldr	r3, [pc, #16]
	adds	r1, r1, r2
	lsls	r0, r0, #19
	ldrb	r3, [r3, r1]
	lsls	r4, r4, #19
	subs	r4, r4, r0
	muls	r3, r4
	adds	r0, r0, r3
	bx	lr
	movs	r0, r0
	.4byte 0x080132fc
