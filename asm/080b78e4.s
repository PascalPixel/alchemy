@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b78e4
	.thumb_func
Func_080b78e4:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	Func_08077008
	adds	r2, r0, #0
	ldr	r0, [pc, #148]
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r1, #0
	cmp	r3, #1
	bne.n	.L0
	movs	r1, #1
.L0:
	cmp	r3, #2
	bne.n	.L1
	orrs	r1, r3
.L1:
	movs	r0, #156
	lsls	r0, r0, #1
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L2
	movs	r3, #32
	orrs	r1, r3
.L2:
	ldr	r0, [pc, #112]
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L3
	movs	r3, #4
	subs	r0, #19
	orrs	r1, r3
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #121
	beq.n	.L4
	cmp	r3, #148
	bne.n	.L3
.L4:
	movs	r3, #5
	negs	r3, r3
	ands	r1, r3
.L3:
	ldr	r0, [pc, #80]
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L5
	movs	r3, #8
	orrs	r1, r3
.L5:
	movs	r0, #160
	lsls	r0, r0, #1
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L6
	movs	r3, #64
	orrs	r1, r3
.L6:
	movs	r0, #158
	lsls	r0, r0, #1
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L7
	movs	r3, #16
	orrs	r1, r3
.L7:
	ldr	r3, [pc, #40]
	adds	r2, r2, r3
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L8
	adds	r2, r3, #0
	adds	r2, #6
	movs	r3, #1
	lsls	r3, r2
	orrs	r1, r3
.L8:
	strh	r1, [r5, #28]
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000131
	.4byte 0x0000013b
	.4byte 0x0000013d
	.4byte 0x00000141
