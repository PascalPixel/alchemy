@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08097868
	.thumb_func
Func_08097868:
	push	{lr}
	ldr	r3, [pc, #64]
	movs	r2, #165
	ldr	r4, [r3, #0]
	lsls	r2, r2, #2
	adds	r3, r4, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L0
	subs	r2, #10
	adds	r3, r4, r2
	ldrb	r3, [r3, #0]
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #4
	adds	r0, r0, r3
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #40]
	ldrh	r1, [r3, #10]
	ands	r2, r1
	strh	r2, [r3, #10]
	ldr	r2, [pc, #36]
	ldrh	r1, [r3, #10]
	ands	r2, r1
	strh	r2, [r3, #10]
	lsls	r0, r0, #2
	ldrh	r2, [r3, #10]
	adds	r0, r4, r0
	ldr	r1, [pc, #24]
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L0:
	pop	{r0}
	bx	r0
	.4byte 0x03001ea8
	.4byte 0x040000b0
	.4byte 0x0000c5ff
	.4byte 0x00007fff
	.4byte 0x04000010
	.4byte 0xa2600001
