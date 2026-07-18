@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.pyでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_0800651c
	.thumb_func
Func_0800651c:
	ldr	r1, [pc, #36]
	ldr	r0, [pc, #40]
	ldrh	r4, [r0, #0]
	strh	r0, [r0, #0]
	movs	r2, #0
	movs	r3, #128
	strb	r3, [r1, #1]
	ldr	r3, [pc, #32]
	str	r2, [r3, #0]
	ldr	r3, [pc, #32]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #32]
	str	r2, [r3, #0]
	ldr	r3, [pc, #32]
	strb	r2, [r1, #3]
	strb	r2, [r1, #2]
	strh	r2, [r3, #0]
	strh	r4, [r0, #0]
	bx	lr
	movs	r0, r0
	.4byte 0x02002220
	.4byte 0x04000208
	.4byte 0x02002080
	.4byte 0x02002008
	.4byte 0x020023ac
	.4byte 0x02002238
