@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.pyでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_080f9b40
	.thumb_func
Func_080f9b40:
	mov	ip, lr
	bl	Func_080f9ab4
	strb	r3, [r1, #29]
	bx	ip
