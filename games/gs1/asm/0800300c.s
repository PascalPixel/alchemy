@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800300c
	.thumb_func
Func_0800300c:
	push	{r5, r6, lr}
	ldr	r6, [pc, #60]
	movs	r5, #0
	strh	r5, [r6, #0]
	ldr	r4, [pc, #56]
	ldr	r3, [pc, #60]
	ldr	r0, [pc, #60]
	adds	r1, r4, #0
	ldr	r2, [pc, #60]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #60]
	ldr	r0, [pc, #60]
	str	r4, [r3, #0]
	ldr	r1, [pc, #60]
	ldr	r3, [pc, #40]
	ldr	r2, [pc, #60]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	subs	r3, #208
	strh	r5, [r3, #0]
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #56]
	strh	r2, [r3, #0]
	ldr	r2, [pc, #56]
	adds	r3, #206
	strh	r2, [r3, #0]
	movs	r3, #1
	strh	r3, [r6, #0]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x04000208
	.4byte 0x03000000
	.4byte 0x040000d4
	.4byte 0x08000770
	.4byte 0x84000500
	.4byte 0x03007ffc
	.4byte 0x08007320
	.4byte 0x030000e0
	.4byte 0x8400000e
	.4byte 0x0000c3ff
	.4byte 0x04000132
	.4byte 0x00001001
