@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08017e88
	.thumb_func
Func_08017e88:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #32
	mov	sl, r0
	adds	r5, r1, #0
	adds	r0, r2, #0
	ldr	r1, [sp, #60]
	ldr	r2, [sp, #68]
	adds	r7, r3, #0
	mov	r3, sl
	mov	r8, r1
	mov	lr, r2
	cmp	r3, #0
	beq.n	.L0
	ldr	r3, [pc, #48]
	lsls	r2, r0, #1
	strh	r3, [r2, r7]
	ldr	r2, [pc, #48]
	adds	r0, #1
	ands	r0, r2
	lsls	r3, r0, #1
	ldr	r1, [pc, #36]
	adds	r0, #1
	ands	r0, r2
	strh	r1, [r3, r7]
	lsls	r3, r0, #1
	strh	r1, [r3, r7]
	adds	r0, #1
	ands	r0, r2
.L0:
	mov	r4, r8
	cmp	r4, #1
	beq.n	.L1
	cmp	r4, #3
	bne.n	.L2
	ldr	r6, [sp, #64]
	cmp	r6, #0
	bne.n	.L2
	b.n	.L1
	movs	r0, r0
	.4byte 0x00000020
	.4byte 0x0000000a
	.4byte 0x000001ff
.L1:
	mov	r9, sp
	ldr	r3, [pc, #184]
	movs	r1, #0
	mov	r2, r9
	mov	ip, r1
	ldmia	r3!, {r1, r4, r6}
	stmia	r2!, {r1, r4, r6}
	ldmia	r3!, {r1, r4, r6}
	stmia	r2!, {r1, r4, r6}
	ldmia	r3!, {r4, r6}
	stmia	r2!, {r4, r6}
	ldrh	r2, [r5, #0]
	cmp	r2, #29
	bne.n	.L3
	ldrh	r3, [r5, #2]
	subs	r3, #1
	mov	ip, r3
	adds	r5, #4
.L3:
	mov	r1, ip
	cmp	r1, #0
	bne.n	.L4
	cmp	r2, #65
	beq.n	.L5
	cmp	r2, #73
	beq.n	.L5
	cmp	r2, #85
	beq.n	.L5
	cmp	r2, #69
	beq.n	.L5
	movs	r3, #1
	mov	ip, r3
	cmp	r2, #79
	bne.n	.L4
.L5:
	movs	r4, #2
	mov	ip, r4
.L4:
	movs	r3, #7
	mov	r6, ip
	ands	r6, r3
	lsls	r3, r6, #2
	mov	r1, r9
	ldr	r4, [r1, r3]
	ldrb	r3, [r4, #0]
	lsls	r1, r3, #24
	movs	r6, #0
	adds	r4, #1
	cmp	r1, #0
	beq.n	.L6
	ldr	r2, [pc, #96]
	mov	ip, r2
.L7:
	lsls	r2, r0, #1
	asrs	r3, r1, #24
	strh	r3, [r2, r7]
	adds	r0, #1
	mov	r3, ip
	adds	r6, #1
	ands	r0, r3
	cmp	r6, #7
	bgt.n	.L6
	ldrb	r3, [r4, #0]
	lsls	r3, r3, #24
	adds	r4, #1
	adds	r1, r3, #0
	cmp	r3, #0
	bne.n	.L7
	b.n	.L6
.L2:
	ldrh	r3, [r5, #0]
	ldrh	r2, [r5, #0]
	cmp	r3, #29
	bne.n	.L8
	adds	r5, #4
.L6:
	ldrh	r2, [r5, #0]
.L8:
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L9
	ldr	r6, [pc, #40]
	movs	r4, #1
	movs	r1, #0
.L13:
	lsls	r2, r2, #16
	asrs	r2, r2, #16
	lsls	r3, r0, #1
	strh	r2, [r3, r7]
	lsls	r2, r2, #16
	adds	r0, #1
	lsrs	r2, r2, #16
	adds	r5, #2
	ands	r0, r6
	cmp	r2, #83
	beq.n	.L10
	cmp	r2, #115
	bne.n	.L11
.L10:
	mov	r2, lr
	str	r4, [r2, #0]
	b.n	.L12
	movs	r0, r0
	.4byte 0x08033e40
	.4byte 0x000001ff
.L11:
	mov	r3, lr
	str	r1, [r3, #0]
.L12:
	ldrh	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L13
.L9:
	mov	r4, r8
	cmp	r4, #2
	beq.n	.L14
	cmp	r4, #3
	bne.n	.L15
	ldr	r6, [sp, #64]
	cmp	r6, #0
	beq.n	.L15
.L14:
	mov	r1, lr
	ldr	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L16
	ldr	r2, [pc, #48]
	lsls	r3, r0, #1
	strh	r2, [r3, r7]
	ldr	r3, [pc, #60]
	adds	r0, #1
	ands	r0, r3
.L16:
	ldr	r2, [pc, #40]
	lsls	r3, r0, #1
	strh	r2, [r3, r7]
	ldr	r3, [pc, #48]
	adds	r0, #1
	ands	r0, r3
.L15:
	mov	r2, sl
	cmp	r2, #0
	beq.n	.L17
	ldr	r1, [pc, #36]
	ldr	r3, [pc, #24]
	lsls	r2, r0, #1
	adds	r0, #1
	strh	r3, [r2, r7]
	ands	r0, r1
	ldr	r3, [pc, #20]
	lsls	r2, r0, #1
	adds	r0, #1
	strh	r3, [r2, r7]
	b.n	.L18
	.4byte 0x00000065
	.4byte 0x00000073
	.4byte 0x0000000a
	.4byte 0x00000008
	.4byte 0x000001ff
.L18:
	ands	r0, r1
	ldr	r3, [pc, #24]
	lsls	r2, r0, #1
	strh	r3, [r2, r7]
	adds	r0, #1
	ands	r0, r1
.L17:
	add	sp, #32
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000020
