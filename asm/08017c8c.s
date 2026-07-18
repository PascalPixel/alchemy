@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08017c8c
	.thumb_func
Func_08017c8c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r4, r3, #0
	ldr	r3, [pc, #92]
	ldr	r3, [r3, #0]
	adds	r7, r2, #0
	mov	r8, r3
	lsls	r3, r7, #16
	asrs	r3, r3, #16
	adds	r6, r0, #0
	sub	sp, #8
	mov	r9, r1
	mov	sl, r3
	cmp	r6, #0
	bne.n	.L0
	ldr	r1, [pc, #72]
	add	r1, r8
	ldrh	r3, [r1, #0]
	movs	r2, #235
	lsls	r2, r2, #4
	mov	r0, r8
	lsls	r3, r3, #1
	adds	r3, r3, r2
	adds	r6, r0, r2
	ldr	r2, [pc, #44]
	strh	r2, [r0, r3]
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #40]
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r1, #0]
.L0:
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	lsls	r1, r3, #16
	adds	r6, #2
	cmp	r1, #0
	beq.n	.L1
.L5:
	lsrs	r5, r1, #16
	cmp	r5, #30
	bhi.n	.L2
	subs	r1, r5, #3
	cmp	r1, #26
	bhi.n	.L3
	ldr	r2, [pc, #20]
	lsls	r3, r1, #2
	ldr	r3, [r3, r2]
	b.n	.L4
	.4byte 0x00000000
	.4byte 0x000001ff
	.4byte 0x03001e8c
	.4byte 0x000012b2
	.4byte 0x08017d08
.L4:
	mov	pc, r3
	movs	r0, r0
	.4byte 0x08017d74
	.4byte 0x08017da4
	.4byte 0x08017da4
	.4byte 0x08017da4
	.4byte 0x08017d7c
	.4byte 0x08017d7c
	.4byte 0x08017d7c
	.4byte 0x08017d7c
	.4byte 0x08017d7c
	.4byte 0x08017d7c
	.4byte 0x08017da4
	.4byte 0x08017d7a
	.4byte 0x08017d7a
	.4byte 0x08017da4
	.4byte 0x08017d7c
	.4byte 0x08017da4
	.4byte 0x08017da4
	.4byte 0x08017da4
	.4byte 0x08017da4
	.4byte 0x08017da4
	.4byte 0x08017da4
	.4byte 0x08017da4
	.4byte 0x08017da4
	.4byte 0x08017da4
	.4byte 0x08017da4
	.4byte 0x08017d7a
	.4byte 0x08017d7c
	mov	r7, sl
	adds	r4, #1
	b.n	.L3
	adds	r6, #2
	adds	r6, #2
	b.n	.L3
.L2:
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r2, r7, #0
	adds	r3, r4, #0
	mov	r0, r9
	adds	r1, r5, #0
	str	r4, [sp, #4]
	bl	Func_08018efc
	ldr	r0, [pc, #56]
	movs	r2, #128
	adds	r3, r5, r0
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	ldr	r4, [sp, #4]
	cmp	r3, r2
	bls.n	.L3
	adds	r7, #1
.L3:
	movs	r0, #0
	ldrsh	r3, [r6, r0]
	lsls	r3, r3, #16
	adds	r6, #2
	adds	r1, r3, #0
	cmp	r3, #0
	bne.n	.L5
.L1:
	ldr	r2, [pc, #28]
	movs	r3, #1
	add	r2, r8
	strb	r3, [r2, #0]
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000ff22
	.4byte 0x00000ea3
