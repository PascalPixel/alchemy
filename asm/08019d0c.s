@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.pyでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_08019d0c
	.thumb_func
Func_08019d0c:
	ldr	r3, [pc, #20]
	ldr	r0, [pc, #24]
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #12]
	adds	r1, r3, r0
	adds	r0, #2
	strh	r2, [r1, #0]
	adds	r1, r3, r0
	strh	r2, [r1, #0]
	bx	lr
	.4byte 0x000003e7
	.4byte 0x03001e8c
	.4byte 0x000012ec
