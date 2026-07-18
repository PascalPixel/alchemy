@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800679c
	.thumb_func
Func_0800679c:
	push	{r5, r6, lr}
	movs	r0, #3
	sub	sp, #4
	bl	Func_080f9010
	bl	Func_08005d10
	ldr	r2, [pc, #140]
	ldr	r3, [pc, #140]
	movs	r1, #19
.L0:
	subs	r1, #1
	strh	r3, [r2, #0]
	subs	r2, #2
	subs	r3, #1
	cmp	r1, #0
	bge.n	.L0
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	ldr	r1, [pc, #124]
	ldr	r2, [pc, #124]
	bl	Func_08006864
	movs	r0, #3
	bl	Func_08006384
.L6:
	ldr	r0, [pc, #108]
	bl	Func_08006408
	ldr	r6, [pc, #112]
.L7:
	ldr	r3, [r6, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	movs	r0, #128
	movs	r1, #160
	lsls	r0, r0, #20
	lsls	r1, r1, #2
	bl	Func_080063bc
.L1:
	ldr	r3, [r6, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
	movs	r1, #160
	ldr	r0, [pc, #80]
	lsls	r1, r1, #2
	bl	Func_080063bc
.L2:
	ldr	r3, [r6, #0]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	ldr	r5, [pc, #64]
.L4:
	subs	r5, #1
	bl	Func_08006798
	cmp	r5, #0
	bge.n	.L4
.L3:
	ldr	r3, [pc, #56]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L5
	ldr	r3, [pc, #52]
	ldr	r0, [pc, #28]
	ldr	r1, [pc, #52]
	ldr	r2, [pc, #56]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L6
.L5:
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L7
	movs	r0, r0
	.4byte 0x06002426
	.4byte 0xfffff093
	.4byte 0x02010000
	.4byte 0x05000100
	.4byte 0x03001ae8
	.4byte 0x08001000
	.4byte 0x0000270f
	.4byte 0x020023ac
	.4byte 0x040000d4
	.4byte 0x06001000
	.4byte 0x840000a0
