@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.tsでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_08003bb4
	.thumb_func
Func_08003bb4:
	ldr	r3, [pc, #36]
	movs	r4, #0
	strb	r4, [r3, #0]
	ldr	r2, [pc, #36]
	ldr	r3, [pc, #24]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #32]
	ldr	r1, [pc, #36]
	ldrb	r3, [r2, #0]
	strb	r3, [r1, #0]
	ldr	r3, [pc, #32]
	strb	r4, [r2, #0]
	strb	r0, [r3, #0]
	ldr	r2, [pc, #32]
	ldrb	r3, [r3, #0]
	strb	r3, [r2, #0]
	b.n	.L0
	movs	r0, r0
	.4byte 0x0000003e
	.4byte 0x03001cd4
	.4byte 0x03001cf8
	.4byte 0x03001ca8
	.4byte 0x03001aec
	.4byte 0x03001c98
	.4byte 0x03001ac0
.L0:
	bx	lr
