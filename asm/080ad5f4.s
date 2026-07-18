@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.tsでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_080ad5f4
	.thumb_func
Func_080ad5f4:
	ldr	r3, [pc, #12]
	movs	r2, #145
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	lsls	r0, r0, #2
	adds	r0, r0, r2
	str	r1, [r3, r0]
	bx	lr
	.4byte 0x03001f2c
