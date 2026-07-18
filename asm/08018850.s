@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08018850
	.thumb_func
Func_08018850:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	str	r1, [sp, #12]
	str	r2, [sp, #8]
	adds	r6, r3, #0
	ldr	r3, [pc, #392]
	ldr	r4, [r3, #0]
	ldr	r3, [pc, #392]
	movs	r2, #0
	mov	sl, r2
	mov	lr, r3
	movs	r2, #24
	movs	r3, #16
	movs	r1, #15
	movs	r5, #0
	add	r2, sp
	add	r3, sp
	mov	fp, r1
	movs	r7, #0
	movs	r1, #0
	mov	r8, r2
	mov	ip, r5
	mov	r9, r3
.L2:
	movs	r2, #235
	lsls	r3, r0, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldrh	r2, [r4, r3]
	ldr	r3, [pc, #352]
	adds	r0, #1
	ands	r0, r3
	cmp	r2, #31
	bls.n	.L0
	cmp	r2, #32
	bne.n	.L1
	adds	r1, #5
	adds	r5, #1
	b.n	.L2
.L1:
	ldr	r3, [pc, #336]
	subs	r2, #32
	lsls	r2, r2, #5
	ldrh	r2, [r3, r2]
	ldr	r3, [pc, #332]
	adds	r3, r4, r3
	ldrh	r3, [r3, #0]
	str	r3, [sp, #0]
	cmp	r3, #1
	beq.n	.L3
	cmp	r3, #5
	bne.n	.L4
.L3:
	adds	r2, #1
.L4:
	adds	r1, r1, r2
	b.n	.L2
.L0:
	cmp	r2, #28
	bhi.n	.L2
	lsls	r3, r2, #2
	mov	r2, lr
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080189a2
	.4byte 0x080189a2
	.4byte 0x0801888a
	.4byte 0x08018948
	.4byte 0x0801888a
	.4byte 0x0801888a
	.4byte 0x0801888a
	.4byte 0x0801888a
	.4byte 0x0801897e
	.4byte 0x08018986
	.4byte 0x0801897e
	.4byte 0x0801888a
	.4byte 0x0801888a
	.4byte 0x0801888a
	.4byte 0x08018978
	.4byte 0x0801897e
	.4byte 0x0801888a
	.4byte 0x0801897e
	.4byte 0x0801888a
	.4byte 0x0801888a
	.4byte 0x0801888a
	.4byte 0x0801888a
	.4byte 0x0801888a
	.4byte 0x0801888a
	.4byte 0x0801888a
	.4byte 0x0801888a
	.4byte 0x0801888a
	.4byte 0x0801888a
	.4byte 0x08018978
	mov	r3, r8
	mov	r2, ip
	adds	r5, #1
	strh	r5, [r3, r2]
	mov	r3, r9
	strh	r1, [r3, r2]
	cmp	r7, r1
	bcs.n	.L5
	adds	r7, r1, #0
.L5:
	mov	r3, sl
	cmp	r3, #2
	bhi.n	.L6
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	lsls	r2, r2, #1
	mov	ip, r2
.L6:
	ldr	r2, [pc, #136]
	movs	r3, #15
	movs	r5, #0
	movs	r1, #0
	add	fp, r3
	mov	lr, r2
	b.n	.L2
	ldr	r3, [pc, #124]
	adds	r0, #1
	ands	r0, r3
	ldr	r3, [pc, #120]
	adds	r0, #1
	ands	r0, r3
	b.n	.L2
	movs	r2, #235
	lsls	r3, r0, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldrh	r2, [r4, r3]
	ldr	r3, [pc, #108]
	adds	r3, r3, r4
	strh	r2, [r3, #0]
	ldr	r3, [pc, #96]
	ldr	r2, [pc, #88]
	adds	r0, #1
	ands	r0, r3
	mov	lr, r2
	b.n	.L2
	mov	r3, r8
	mov	r2, ip
	adds	r5, #1
	strh	r5, [r3, r2]
	mov	r3, r9
	strh	r1, [r3, r2]
	cmp	r7, r1
	bcs.n	.L7
	adds	r7, r1, #0
.L7:
	ldr	r1, [pc, #76]
	adds	r3, r4, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L8
	adds	r7, #2
.L8:
	ldr	r2, [sp, #12]
	str	r7, [r2, #0]
	ldr	r1, [sp, #8]
	mov	r3, fp
	str	r3, [r1, #0]
	adds	r3, r7, #0
	adds	r3, #19
	lsrs	r7, r3, #3
	lsls	r3, r7, #3
	adds	r7, r3, #0
	subs	r7, #16
	cmp	r6, #0
	beq.n	.L9
	movs	r2, #0
	movs	r5, #0
.L14:
	mov	r1, r8
	ldrh	r3, [r5, r1]
	cmp	r3, #1
	bhi.n	.L10
	ldr	r3, [pc, #4]
	strh	r3, [r6, #0]
	b.n	.L11
	.4byte 0x00000000
	.4byte 0x03001e8c
	.4byte 0x080188d4
	.4byte 0x000001ff
	.4byte 0x08032224
	.4byte 0x00000eac
	.4byte 0x00000ea4
.L10:
	mov	r1, r9
	ldrh	r3, [r5, r1]
	subs	r0, r7, r3
	subs	r0, #4
	cmp	r0, #0
	bge.n	.L12
	movs	r0, #0
.L12:
	mov	r3, r8
	ldrh	r1, [r5, r3]
	lsls	r0, r0, #8
	subs	r1, #1
	str	r2, [sp, #4]
	bl	Func_080022ec
	movs	r1, #192
	lsls	r1, r1, #4
	ldr	r2, [sp, #4]
	cmp	r0, r1
	bls.n	.L13
	movs	r0, #128
	lsls	r0, r0, #2
.L13:
	strh	r0, [r6, #0]
.L11:
	adds	r6, #2
	adds	r2, #1
	adds	r5, #2
	cmp	r2, sl
	bls.n	.L14
.L9:
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
