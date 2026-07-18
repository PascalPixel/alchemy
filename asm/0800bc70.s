@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800bc70
	.thumb_func
Func_0800bc70:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #0
	mov	r8, r1
	mov	sl, r0
	bl	Func_08185000
	adds	r7, r0, #0
	bl	Func_08004080
	ldr	r3, [pc, #284]
	ldr	r5, [r3, #0]
	ldrb	r3, [r7, #0]
	adds	r6, r0, #0
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L0
	b.n	.L1
.L0:
	adds	r3, r5, #0
	adds	r3, #32
	ldrb	r3, [r3, #0]
	movs	r2, #0
	b.n	.L2
.L4:
	adds	r2, #1
	adds	r5, #56
	cmp	r2, #63
	bgt.n	.L3
	adds	r3, r5, #0
	adds	r3, #32
	ldrb	r3, [r3, #0]
.L2:
	cmp	r3, #0
	bne.n	.L4
	mov	r8, r5
.L3:
	mov	r2, r8
	movs	r0, #0
	cmp	r2, #0
	beq.n	.L1
	cmp	r6, #96
	beq.n	.L1
	adds	r0, r6, #0
	movs	r1, #0
	movs	r2, #0
	bl	Func_08003fa4
	mov	ip, r0
	cmp	r0, #0
	bne.n	.L5
	movs	r0, #0
	b.n	.L1
.L5:
	movs	r3, #0
	mov	r2, r8
	mov	r1, r8
	strb	r6, [r1, #28]
	strh	r3, [r2, #30]
	adds	r2, #38
	movs	r3, #1
	strb	r3, [r2, #0]
	ldrb	r3, [r7, #0]
	ldrb	r2, [r7, #1]
	lsls	r3, r3, #8
	adds	r0, r3, r2
	movs	r3, #129
	lsls	r3, r3, #5
	ldr	r4, [pc, #184]
	cmp	r0, r3
	beq.n	.L6
	cmp	r0, r3
	bhi.n	.L7
	movs	r3, #129
	movs	r4, #128
	lsls	r3, r3, #4
	lsls	r4, r4, #8
	cmp	r0, r3
	beq.n	.L6
	cmp	r0, r3
	bhi.n	.L8
	subs	r3, #8
	movs	r4, #0
	b.n	.L9
.L8:
	ldr	r1, [pc, #156]
	movs	r4, #128
	lsls	r4, r4, #7
	cmp	r0, r1
	beq.n	.L6
	ldr	r2, [pc, #148]
	movs	r4, #128
	lsls	r4, r4, #23
	cmp	r0, r2
	beq.n	.L6
	b.n	.L10
.L7:
	movs	r3, #129
	lsls	r3, r3, #6
	ldr	r4, [pc, #136]
	cmp	r0, r3
	beq.n	.L6
	cmp	r0, r3
	bhi.n	.L11
	subs	r3, #48
	ldr	r4, [pc, #128]
	cmp	r0, r3
	beq.n	.L6
	ldr	r1, [pc, #128]
	movs	r4, #128
	lsls	r4, r4, #24
	cmp	r0, r1
	beq.n	.L6
	b.n	.L10
.L11:
	ldr	r2, [pc, #120]
	ldr	r4, [pc, #120]
	cmp	r0, r2
	beq.n	.L6
	ldr	r3, [pc, #120]
	movs	r4, #192
	lsls	r4, r4, #24
.L9:
	cmp	r0, r3
	beq.n	.L6
.L10:
	movs	r4, #0
.L6:
	mov	r2, r8
	movs	r1, #0
	movs	r3, #128
	stmia	r2!, {r1}
	lsls	r3, r3, #6
	orrs	r4, r3
	movs	r0, #128
	stmia	r2!, {r4}
	lsls	r0, r0, #4
	mov	r3, ip
	orrs	r3, r0
	stmia	r2!, {r3}
	movs	r3, #192
	stmia	r2!, {r1}
	lsls	r3, r3, #7
	stmia	r2!, {r3}
	movs	r1, #187
	ldr	r3, [pc, #76]
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldrh	r3, [r3, #0]
	lsrs	r3, r3, #5
	orrs	r3, r0
	str	r3, [r2, #0]
	adds	r0, r5, #0
	mov	r1, sl
	bl	Func_0800b8ac
	movs	r2, #1
	negs	r2, r2
	mov	r0, r8
.L1:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e60
	.4byte 0x80008000
	.4byte 0x00001008
	.4byte 0x00001010
	.4byte 0xc0008000
	.4byte 0x80004000
	.4byte 0x00002020
	.4byte 0x00004020
	.4byte 0xc0004000
	.4byte 0x00004040
	.4byte 0x03001b10
