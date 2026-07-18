@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800ea60
	.thumb_func
Func_0800ea60:
	push	{r5, r6, r7, lr}
	ldr	r7, [pc, #124]
	ldr	r3, [pc, #124]
	lsrs	r6, r0, #14
	ands	r7, r0
	ldr	r0, [pc, #124]
	ldr	r5, [r3, #0]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L0
	movs	r3, #193
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #250
	strh	r3, [r2, #0]
	b.n	.L1
.L0:
	movs	r2, #207
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L2
	ldr	r1, [pc, #88]
	movs	r2, #128
	ldr	r3, [r1, #0]
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	movs	r3, #193
	lsls	r3, r3, #1
	adds	r2, r5, r3
	ldr	r3, [pc, #72]
	strh	r3, [r2, #0]
	b.n	.L1
.L3:
	ldr	r3, [r1, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	movs	r3, #193
	lsls	r3, r3, #1
	adds	r2, r5, r3
	ldr	r3, [pc, #52]
	strh	r3, [r2, #0]
	b.n	.L1
.L2:
	cmp	r6, #0
	beq.n	.L4
	cmp	r6, #1
	beq.n	.L5
	b.n	.L1
.L4:
	movs	r2, #191
	b.n	.L6
.L5:
	movs	r2, #192
.L6:
	lsls	r2, r2, #1
	adds	r3, r5, r2
	strh	r7, [r3, #0]
.L1:
	adds	r0, r7, #0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00003fff
	.4byte 0x03001ebc
	.4byte 0x00000107
	.4byte 0x03001c94
	.4byte 0x0000fc88
	.4byte 0x0000fc87
