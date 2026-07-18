@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.pyでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_080f9b74
	.thumb_func
Func_080f9b74:
	mov	ip, lr
	ldr	r2, [r1, #64]
	ldrb	r3, [r2, #0]
	adds	r2, #1
	str	r2, [r1, #64]
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r2, r2, #2
	ldr	r3, [r0, #48]
	adds	r2, r2, r3
	ldr	r3, [r2, #0]
	bl	Func_080f9a9a
	str	r3, [r1, #36]
	ldr	r3, [r2, #4]
	bl	Func_080f9a9a
	str	r3, [r1, #40]
	ldr	r3, [r2, #8]
	bl	Func_080f9a9a
	str	r3, [r1, #44]
	bx	ip
