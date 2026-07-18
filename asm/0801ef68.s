@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801ef68
	.thumb_func
Func_0801ef68:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #124]
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	sub	sp, #4
	mov	r9, r3
	movs	r3, #0
	str	r3, [sp, #0]
	ldrh	r3, [r6, #8]
	subs	r3, #1
	movs	r2, #1
	mov	fp, r3
	adds	r3, r1, #0
	ands	r3, r2
	ldrh	r7, [r6, #10]
	cmp	r3, #0
	bne.n	.L0
	movs	r3, #3
	negs	r3, r3
	ands	r1, r3
.L0:
	movs	r3, #2
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L1
	movs	r2, #5
	str	r2, [sp, #0]
	movs	r2, #0
.L1:
	ldr	r1, [pc, #76]
	adds	r5, r2, #0
	b.n	.L2
.L9:
	ldrsb	r3, [r2, r5]
	ldr	r2, [sp, #0]
	adds	r0, r3, r2
	cmp	r0, fp
	bcs.n	.L3
	movs	r4, #0
	cmp	r7, #0
	beq.n	.L3
	ldr	r3, [pc, #56]
	subs	r2, r7, #1
	mov	ip, r2
	ldr	r2, [pc, #56]
	mov	sl, r3
	adds	r3, #1
	mov	r8, r3
	mov	lr, r2
.L8:
	ldrh	r2, [r6, #14]
	ldrh	r3, [r6, #12]
	adds	r2, r2, r4
	adds	r3, r3, r0
	lsls	r2, r2, #5
	adds	r2, r2, r3
	lsls	r2, r2, #1
	mov	r3, r9
	adds	r1, r2, r3
	cmp	r4, #0
	bne.n	.L4
	mov	r2, sl
	b.n	.L5
.L4:
	cmp	r4, ip
	bne.n	.L6
	mov	r3, r8
	strh	r3, [r1, #0]
	b.n	.L7
	.4byte 0x03001e8c
	.4byte 0x080371c4
	.4byte 0x0000f018
	.4byte 0x0000f00f
.L6:
	mov	r2, lr
.L5:
	strh	r2, [r1, #0]
.L7:
	adds	r4, #1
	cmp	r4, r7
	bne.n	.L8
	ldr	r1, [pc, #92]
.L3:
	adds	r5, #1
.L2:
	adds	r2, r1, #0
	ldrsb	r3, [r2, r5]
	cmp	r3, #0
	bge.n	.L9
	ldr	r3, [pc, #84]
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L10
	ldrh	r3, [r6, #10]
	ldrh	r2, [r6, #14]
	adds	r2, r2, r3
	ldrh	r3, [r6, #12]
	lsls	r2, r2, #6
	lsls	r3, r3, #1
	add	r2, r9
	adds	r2, r2, r3
	adds	r1, r2, #0
	ldr	r3, [pc, #40]
	subs	r1, #64
	movs	r0, #1
	strh	r3, [r1, #0]
	adds	r1, #2
	cmp	r0, fp
	bcs.n	.L11
	ldr	r3, [pc, #28]
.L12:
	adds	r0, #1
	strh	r3, [r1, #0]
	adds	r1, #2
	cmp	r0, fp
	bcc.n	.L12
.L11:
	ldr	r3, [pc, #20]
	strh	r3, [r1, #0]
.L10:
	ldr	r2, [pc, #28]
	movs	r3, #1
	add	r2, r9
	strb	r3, [r2, #0]
	add	sp, #4
	b.n	.L13
	.4byte 0x0000f080
	.4byte 0x0000f081
	.4byte 0x0000f082
	.4byte 0x080371c4
	.4byte 0x00000ea5
	.4byte 0x00000ea3
.L13:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
