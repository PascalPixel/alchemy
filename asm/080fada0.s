@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.tsでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_080fada0
	.thumb_func
Func_080fada0:
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	adds	r1, r0, #0
	cmp	r0, #2
	beq.n	.L0
	cmp	r0, #2
	bgt.n	.L1
	cmp	r0, #1
	beq.n	.L2
	b.n	.L3
.L1:
	cmp	r1, #3
	beq.n	.L4
	b.n	.L3
.L2:
	ldr	r1, [pc, #8]
	movs	r0, #8
	strb	r0, [r1, #0]
	adds	r1, #2
	b.n	.L5
	.4byte 0x04000063
.L0:
	ldr	r1, [pc, #0]
	b.n	.L6
	.4byte 0x04000069
.L4:
	ldr	r1, [pc, #4]
	movs	r0, #0
	b.n	.L7
	movs	r0, r0
	.4byte 0x04000070
.L3:
	ldr	r1, [pc, #12]
.L6:
	movs	r0, #8
	strb	r0, [r1, #0]
	adds	r1, #4
.L5:
	movs	r0, #128
.L7:
	strb	r0, [r1, #0]
	bx	lr
	movs	r0, r0
	.4byte 0x04000079
