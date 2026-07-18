@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.tsでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_080fb410
	.thumb_func
Func_080fb410:
	adds	r1, r0, #0
	movs	r2, #0
	movs	r0, #0
	strb	r0, [r1, #26]
	strb	r0, [r1, #22]
	ldrb	r0, [r1, #24]
	cmp	r0, #0
	bne.n	.L0
	movs	r0, #12
	b.n	.L1
.L0:
	movs	r0, #3
.L1:
	ldrb	r2, [r1, #0]
	orrs	r0, r2
	strb	r0, [r1, #0]
	bx	lr
