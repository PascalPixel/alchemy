@ 呼出しグラフで未到達だったコード間隙断片の再構築サム逆アセンブル。
@ 先頭のFunc_08005e70はC一致で分離済み。探索では未到達。
@ build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08005e88
	.thumb_func
Func_08005e88:
	ldr	r0, [pc, #56]
	ldr	r4, [pc, #40]
	ldr	r1, [pc, #56]
	strh	r4, [r0, #0]
	ldr	r3, [pc, #56]
	ldrh	r2, [r1, #0]
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r3, [pc, #28]
	ldr	r2, [pc, #52]
	strh	r3, [r0, #0]
	ldr	r3, [pc, #28]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	subs	r2, #28
	str	r3, [r2, #0]
	ldr	r3, [pc, #20]
	adds	r2, #246
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	strb	r4, [r3, #8]
	b.n	.L1
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00002003
	.4byte 0x000000c0
	.4byte 0x04000208
	.4byte 0x04000200
	.4byte 0x0000ff3f
	.4byte 0x04000128
	.4byte 0x0000c963
	.4byte 0x02002240
.L1:
	bx	lr
