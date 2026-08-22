@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c0130
	.thumb_func
Func_080c0130:
	push	{lr}
	ldr	r2, [pc, #60]
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #8]
	cmp	r3, #2
	bne.n	.L0
	adds	r3, r2, #0
	subs	r3, #136
	ldr	r4, [r3, #0]
	ldr	r3, [r4, #0]
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #6
	adds	r0, r4, r0
	ldrh	r3, [r0, #32]
	ldr	r1, [pc, #36]
	strh	r3, [r1, #0]
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #36]
	adds	r0, #34
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r4, #0
	adds	r3, #36
	adds	r0, #16
	adds	r1, #20
	ldr	r2, [pc, #24]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L0:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f00
	.4byte 0x0400000c
	.4byte 0x040000b0
	.4byte 0xa2600001
	.4byte 0x84000004
