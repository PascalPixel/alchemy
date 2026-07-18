@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fa83c
	.thumb_func
Func_080fa83c:
	push	{r4, r5, lr}
	adds	r3, r0, #0
	ldr	r0, [pc, #132]
	ldr	r5, [r0, #0]
	ldr	r1, [r5, #0]
	ldr	r0, [pc, #132]
	cmp	r1, r0
	bne.n	.L0
	adds	r0, r1, #1
	str	r0, [r5, #0]
	movs	r4, #255
	ands	r4, r3
	cmp	r4, #0
	beq.n	.L1
	movs	r0, #127
	ands	r4, r0
	strb	r4, [r5, #5]
.L1:
	movs	r4, #240
	lsls	r4, r4, #4
	ands	r4, r3
	cmp	r4, #0
	beq.n	.L2
	lsrs	r0, r4, #8
	strb	r0, [r5, #6]
	movs	r4, #12
	adds	r0, r5, #0
	adds	r0, #80
	movs	r1, #0
.L3:
	strb	r1, [r0, #0]
	subs	r4, #1
	adds	r0, #64
	cmp	r4, #0
	bne.n	.L3
.L2:
	movs	r4, #240
	lsls	r4, r4, #8
	ands	r4, r3
	cmp	r4, #0
	beq.n	.L4
	lsrs	r0, r4, #12
	strb	r0, [r5, #7]
.L4:
	movs	r4, #176
	lsls	r4, r4, #16
	ands	r4, r3
	cmp	r4, #0
	beq.n	.L5
	movs	r0, #192
	lsls	r0, r0, #14
	ands	r0, r4
	lsrs	r4, r0, #14
	ldr	r2, [pc, #48]
	ldrb	r1, [r2, #0]
	movs	r0, #63
	ands	r0, r1
	orrs	r0, r4
	strb	r0, [r2, #0]
.L5:
	movs	r4, #240
	lsls	r4, r4, #12
	ands	r4, r3
	cmp	r4, #0
	beq.n	.L6
	bl	Func_080fa928
	adds	r0, r4, #0
	bl	Func_080fa798
.L6:
	ldr	r0, [pc, #12]
	str	r0, [r5, #0]
.L0:
	pop	{r4, r5}
	pop	{r0}
	bx	r0
	.4byte 0x03007ff0
	.4byte 0x68736d53
	.4byte 0x04000089
