@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.tsでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_08091e3c
	.thumb_func
Func_08091e3c:
	ldr	r3, [pc, #32]
	movs	r2, #184
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [pc, #28]
	movs	r4, #224
	strh	r2, [r3, #0]
	ldr	r2, [pc, #24]
	lsls	r4, r4, #1
	adds	r3, r2, r4
	strh	r0, [r3, #0]
	movs	r0, #225
	lsls	r0, r0, #1
	adds	r3, r2, r0
	strh	r1, [r3, #0]
	bx	lr
	movs	r0, r0
	.4byte 0x03001ebc
	.4byte 0x000003e7
	.4byte 0x02000240
