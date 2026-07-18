@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.pyでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_080167ac
	.thumb_func
Func_080167ac:
	ldr	r3, [pc, #28]
	ldr	r4, [pc, #32]
	ldr	r2, [r3, #0]
	ldrh	r1, [r0, #22]
	adds	r3, r2, r4
	strh	r1, [r3, #0]
	subs	r4, #2
	ldrh	r1, [r0, #24]
	adds	r3, r2, r4
	strh	r1, [r3, #0]
	ldr	r1, [pc, #16]
	ldrh	r3, [r0, #26]
	adds	r2, r2, r1
	strh	r3, [r2, #0]
	bx	lr
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x00000eae
	.4byte 0x00000ea8
