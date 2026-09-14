@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.tsでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_08004760
	.thumb_func
Func_08004760:
	sub	sp, #4
	ldr	r3, [pc, #32]
	mov	r0, sp
	str	r3, [r0, #0]
	ldr	r1, [pc, #28]
	ldr	r3, [pc, #32]
	ldr	r2, [pc, #32]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #32]
	ldr	r3, [pc, #16]
	str	r3, [r2, #0]
	movs	r2, #128
	ldr	r3, [pc, #28]
	lsls	r2, r2, #3
	strh	r2, [r3, #0]
	add	sp, #4
	bx	lr
	.4byte 0xf000f000
	.4byte 0x06002000
	.4byte 0x040000d4
	.4byte 0x85000140
	.4byte 0x03001cbc
	.4byte 0x04000008
