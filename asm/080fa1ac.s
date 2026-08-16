@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.tsでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_080fa1ac
	.thumb_func
Func_080fa1ac:
	movs	r2, #0
	strb	r2, [r1, #22]
	strb	r2, [r1, #26]
	ldrb	r2, [r1, #24]
	cmp	r2, #0
	bne.n	.L0
	movs	r2, #12
	b.n	.L1
.L0:
	movs	r2, #3
.L1:
	ldrb	r3, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	lr
