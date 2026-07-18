@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08017aa4
	.thumb_func
Func_08017aa4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r3
	ldr	r3, [pc, #56]
	adds	r7, r2, #0
	ldr	r6, [r3, #0]
	lsls	r3, r7, #16
	asrs	r3, r3, #16
	adds	r5, r0, #0
	sub	sp, #4
	mov	sl, r1
	movs	r4, #0
	mov	r9, r3
	cmp	r5, #0
	beq.n	.L0
	b.n	.L1
.L0:
	ldr	r3, [pc, #36]
	adds	r1, r6, r3
	ldrh	r3, [r1, #0]
	movs	r2, #235
	lsls	r2, r2, #4
	lsls	r3, r3, #1
	adds	r3, r3, r2
	strh	r4, [r6, r3]
	ldrh	r3, [r1, #0]
	adds	r5, r6, r2
	ldr	r2, [pc, #8]
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r1, #0]
	b.n	.L1
	.4byte 0x000001ff
	.4byte 0x03001e8c
	.4byte 0x000012b2
.L8:
	cmp	r4, #30
	bhi.n	.L2
	subs	r1, r4, #3
	cmp	r1, #26
	bhi.n	.L1
	ldr	r2, [pc, #264]
	lsls	r3, r1, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x08017b8e
	.4byte 0x08017be6
	.4byte 0x08017be6
	.4byte 0x08017be6
	.4byte 0x08017b88
	.4byte 0x08017b74
	.4byte 0x08017b78
	.4byte 0x08017b7c
	.4byte 0x08017b84
	.4byte 0x08017b84
	.4byte 0x08017be6
	.4byte 0x08017b96
	.4byte 0x08017b96
	.4byte 0x08017be6
	.4byte 0x08017b84
	.4byte 0x08017be6
	.4byte 0x08017be6
	.4byte 0x08017be6
	.4byte 0x08017be6
	.4byte 0x08017be6
	.4byte 0x08017be6
	.4byte 0x08017be6
	.4byte 0x08017be6
	.4byte 0x08017be6
	.4byte 0x08017be6
	.4byte 0x08017b96
	.4byte 0x08017b84
	ldr	r1, [pc, #148]
	b.n	.L3
	ldr	r1, [pc, #148]
	b.n	.L3
	ldr	r1, [pc, #148]
.L3:
	ldrh	r2, [r5, #0]
	adds	r3, r6, r1
	strh	r2, [r3, #0]
.L4:
	adds	r5, #2
	b.n	.L1
	bl	Func_080173ac
	b.n	.L1
	movs	r2, #15
	mov	r7, r9
	add	r8, r2
	b.n	.L1
	adds	r5, #2
	b.n	.L4
.L2:
	mov	r3, sl
	ldrh	r2, [r3, #22]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L5
	ldrh	r0, [r5, #0]
	cmp	r4, #32
	bls.n	.L5
	cmp	r0, #32
	bls.n	.L5
	adds	r3, r4, #0
	adds	r2, r0, #0
	ldr	r1, [pc, #96]
	subs	r3, #32
	subs	r2, #32
	lsls	r3, r3, #5
	lsls	r2, r2, #5
	ldrh	r3, [r1, r3]
	ldrh	r2, [r1, r2]
	movs	r1, #240
	adds	r3, r3, r2
	lsls	r3, r3, #16
	lsls	r1, r1, #12
	cmp	r3, r1
	bhi.n	.L5
	lsls	r3, r0, #8
	orrs	r4, r3
	adds	r5, #2
.L5:
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r2, r7, #0
	mov	r0, sl
	adds	r1, r4, #0
	mov	r3, r8
	bl	Func_08018cac
	adds	r7, r7, r0
.L1:
	ldrh	r4, [r5, #0]
	adds	r5, #2
	cmp	r4, #255
	bls.n	.L6
	movs	r4, #64
.L6:
	cmp	r4, #0
	beq.n	.L7
	b.n	.L8
.L7:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x08017b08
	.4byte 0x00000eae
	.4byte 0x00000eac
	.4byte 0x00000ea8
	.4byte 0x08032224
