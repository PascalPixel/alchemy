@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080bad7c
	.thumb_func
Func_080bad7c:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #80]
	sub	sp, #16
	movs	r4, #0
	ldr	r1, [r3, #0]
	cmp	r0, #0
	beq.n	.L0
	movs	r3, #88
	ldrsh	r3, [r1, r3]
	movs	r7, #0
	cmp	r3, #255
	beq.n	.L1
	adds	r5, r1, #0
	adds	r5, #88
	add	r6, sp, #4
.L3:
	movs	r1, #0
	ldrsh	r0, [r5, r1]
	cmp	r0, #254
	beq.n	.L2
	str	r4, [sp, #0]
	bl	Func_08077008
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	ldr	r4, [sp, #0]
	cmp	r3, #0
	beq.n	.L2
	ldr	r2, [pc, #24]
	adds	r3, r7, #0
	orrs	r3, r2
	strh	r3, [r6, #0]
	adds	r4, #1
	adds	r6, #2
.L2:
	adds	r5, #2
	movs	r0, #0
	ldrsh	r3, [r5, r0]
	adds	r7, #1
	cmp	r3, #255
	bne.n	.L3
	b.n	.L1
	.4byte 0x00000100
	.4byte 0x03001e74
.L0:
	movs	r3, #100
	adds	r2, r1, #2
	ldrsh	r3, [r2, r3]
	movs	r7, #0
	cmp	r3, #255
	beq.n	.L1
	adds	r0, r1, #0
	lsls	r3, r4, #1
	add	r1, sp, #16
	adds	r3, r3, r1
	adds	r0, #102
	adds	r1, r3, #0
	ldr	r5, [pc, #40]
	adds	r2, r0, #0
	subs	r1, #12
.L5:
	ldrh	r3, [r2, #0]
	movs	r6, #254
	lsls	r3, r3, #16
	lsls	r6, r6, #16
	adds	r2, #2
	cmp	r3, r6
	beq.n	.L4
	adds	r3, r7, #0
	orrs	r3, r5
	strh	r3, [r1, #0]
	adds	r4, #1
	adds	r1, #2
.L4:
	adds	r0, #2
	movs	r6, #0
	ldrsh	r3, [r0, r6]
	adds	r7, #1
	cmp	r3, #255
	bne.n	.L5
	b.n	.L1
	.4byte 0x00000180
.L1:
	movs	r0, #0
	cmp	r4, #0
	beq.n	.L6
	add	r5, sp, #4
	str	r4, [sp, #0]
	bl	Func_08004458
	ldr	r4, [sp, #0]
	adds	r3, r4, #0
	muls	r3, r0
	lsrs	r3, r3, #16
	lsls	r3, r3, #1
	ldrh	r0, [r5, r3]
.L6:
	add	sp, #16
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
