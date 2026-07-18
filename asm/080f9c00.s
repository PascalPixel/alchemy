@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.pyでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_080f9c00
	.thumb_func
Func_080f9c00:
	mov	ip, lr
	bl	Func_080f9ab4
	ldrb	r0, [r1, #24]
	cmp	r0, r3
	beq.n	.L0
	strb	r3, [r1, #24]
	ldrb	r3, [r1, #0]
	movs	r2, #15
	orrs	r3, r2
	strb	r3, [r1, #0]
.L0:
	bx	ip
