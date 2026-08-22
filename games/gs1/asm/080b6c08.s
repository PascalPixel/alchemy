@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b6c08
	.thumb_func
Func_080b6c08:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #128]
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	movs	r3, #1
	ands	r3, r6
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L0
	movs	r3, #88
	ldrsh	r3, [r0, r3]
	cmp	r3, #255
	beq.n	.L0
	adds	r2, r0, #0
	adds	r2, #88
.L3:
	movs	r7, #0
	ldrsh	r3, [r2, r7]
	ldrh	r4, [r2, #0]
	cmp	r3, #254
	beq.n	.L1
	cmp	r1, #0
	beq.n	.L2
	strh	r4, [r1, #0]
	adds	r1, #2
.L2:
	adds	r5, #1
.L1:
	adds	r2, #2
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	cmp	r3, #255
	bne.n	.L3
.L0:
	movs	r3, #2
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L4
	adds	r2, r0, #2
	movs	r3, #100
	ldrsh	r3, [r2, r3]
	mov	ip, r2
	cmp	r3, #255
	beq.n	.L4
	movs	r0, #100
.L7:
	ldrsh	r3, [r2, r0]
	ldrh	r4, [r2, r0]
	cmp	r3, #254
	beq.n	.L5
	cmp	r1, #0
	beq.n	.L6
	strh	r4, [r1, #0]
	adds	r1, #2
.L6:
	adds	r5, #1
.L5:
	adds	r0, #2
	mov	r2, ip
	ldrsh	r3, [r2, r0]
	cmp	r3, #255
	bne.n	.L7
.L4:
	cmp	r1, #0
	beq.n	.L8
	ldr	r3, [pc, #12]
	strh	r3, [r1, #0]
.L8:
	adds	r0, r5, #0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000000ff
	.4byte 0x03001e74
