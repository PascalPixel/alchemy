@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.pyでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_080fa280
	.thumb_func
Func_080fa280:
	adds	r2, r0, #0
	lsls	r1, r1, #16
	lsrs	r1, r1, #16
	ldr	r3, [r2, #52]
	ldr	r0, [pc, #16]
	cmp	r3, r0
	bne.n	.L0
	strh	r1, [r2, #38]
	strh	r1, [r2, #36]
	movs	r0, #128
	lsls	r0, r0, #1
	strh	r0, [r2, #40]
.L0:
	bx	lr
	movs	r0, r0
	.4byte 0x68736d53
