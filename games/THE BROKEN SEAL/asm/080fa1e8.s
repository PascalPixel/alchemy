@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.tsでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_080fa1e8
	.thumb_func
Func_080fa1e8:
	mov	ip, lr
	bl	Func_080fa1c8
	strb	r3, [r1, #23]
	cmp	r3, #0
	bne.n	.L0
	bl	Func_080fa1ac
.L0:
	bx	ip
