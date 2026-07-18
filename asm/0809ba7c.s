@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.tsでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_0809ba7c
	.thumb_func
Func_0809ba7c:
	movs	r3, #0
	ldr	r2, [pc, #12]
	str	r1, [r0, #52]
	strh	r3, [r0, #58]
	strh	r3, [r0, #56]
	adds	r0, #64
	strb	r2, [r0, #0]
	bx	lr
	.4byte 0x00000000
