@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08005ee0
	.thumb_func
Func_08005ee0:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r7, [pc, #20]
	ldr	r6, [pc, #20]
	ldrb	r3, [r7, #1]
	mov	r8, r0
	mov	ip, r1
	ldr	r5, [r6, #0]
	cmp	r3, #0
	beq.n	.L0
	cmp	r3, #1
	beq.n	.L1
	b.n	.L2
	.4byte 0x02002240
	.4byte 0x04000128
.L0:
	movs	r2, #48
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L3
	movs	r3, #136
	adds	r4, r5, #0
	ands	r4, r3
	cmp	r4, #8
	bne.n	.L2
	movs	r2, #4
	adds	r3, r5, #0
	ands	r3, r2
	lsls	r3, r3, #24
	lsrs	r2, r3, #24
	cmp	r2, #0
	bne.n	.L3
	movs	r1, #1
	ldr	r3, [r7, #20]
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L3
	ldr	r0, [pc, #92]
	strh	r2, [r0, #0]
	ldr	r1, [pc, #92]
	ldrh	r2, [r1, #0]
	movs	r3, #129
	negs	r3, r3
	ands	r3, r2
	movs	r2, #64
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r3, #1
	strh	r3, [r0, #0]
	ldrb	r2, [r6, #1]
	subs	r3, #66
	ands	r3, r2
	strb	r3, [r6, #1]
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #68]
	subs	r2, #246
	str	r3, [r2, #0]
	strb	r4, [r7, #0]
.L3:
	movs	r3, #1
	strb	r3, [r7, #1]
.L1:
	mov	r0, ip
	bl	Func_0800615c
	mov	r0, r8
	bl	Func_080060e8
	ldr	r7, [pc, #48]
.L2:
	ldrb	r3, [r7, #11]
	adds	r3, #1
	strb	r3, [r7, #11]
	ldrb	r3, [r7, #2]
	ldrb	r2, [r7, #3]
	lsls	r3, r3, #8
	orrs	r2, r3
	ldrb	r3, [r7, #0]
	cmp	r3, #8
	bne.n	.L4
	movs	r3, #128
	orrs	r2, r3
	b.n	.L4
	movs	r0, r0
	.4byte 0x000000c0
	.4byte 0x04000208
	.4byte 0x04000200
	.4byte 0x04000202
	.4byte 0x0000c963
	.4byte 0x02002240
.L4:
	ldrb	r3, [r7, #9]
	adds	r0, r2, #0
	cmp	r3, #0
	beq.n	.L5
	movs	r3, #128
	lsls	r3, r3, #5
	orrs	r0, r3
.L5:
	lsls	r3, r5, #26
	lsrs	r3, r3, #30
	cmp	r3, #1
	bls.n	.L6
	movs	r3, #128
	lsls	r3, r3, #6
	orrs	r0, r3
.L6:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
