@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.tsでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_080251d4
	.thumb_func
Func_080251d4:
	adds	r3, r0, #0
	ldr	r0, [pc, #28]
	movs	r2, #192
	ands	r1, r0
	ands	r0, r3
	lsls	r2, r2, #19
	lsls	r0, r0, #5
	lsls	r1, r1, #5
	adds	r0, r0, r2
	adds	r1, r1, r2
	ldr	r3, [pc, #12]
	ldr	r2, [pc, #16]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x040000d4
	.4byte 0x84000008
