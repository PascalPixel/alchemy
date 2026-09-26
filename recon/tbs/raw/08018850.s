.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.global UiText_MeasureEntryDimensions
	.global Func_08018850
	.thumb_func
UiText_MeasureEntryDimensions:
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
.L_0801888a:
	movs	r2, #235
	lsls	r3, r0, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldrh	r2, [r4, r3]
	ldr	r3, [pc, #352]
	adds	r0, #1
	ands	r0, r3
	cmp	r2, #31
	bls.n	.L_080188c6
	cmp	r2, #32
	bne.n	.L_080188a8
	adds	r1, #5
	adds	r5, #1
	b.n	.L_0801888a
.L_080188a8:
	ldr	r3, [pc, #336]
	subs	r2, #32
	lsls	r2, r2, #5
	ldrh	r2, [r3, r2]
	ldr	r3, [pc, #332]
	adds	r3, r4, r3
	ldrh	r3, [r3, #0]
	str	r3, [sp, #0]
	cmp	r3, #1
	beq.n	.L_080188c0
	cmp	r3, #5
	bne.n	.L_080188c2
.L_080188c0:
	adds	r2, #1
.L_080188c2:
	adds	r1, r1, r2
	b.n	.L_0801888a
.L_080188c6:
	cmp	r2, #28
	bhi.n	.L_0801888a
	lsls	r3, r2, #2
	mov	r2, lr
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	ldrh	r2, [r4, #12]
	lsrs	r1, r0, #32
	ldrh	r2, [r4, #12]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r0, [r1, #10]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r6, [r7, #10]
	lsrs	r1, r0, #32
	ldrh	r6, [r0, #12]
	lsrs	r1, r0, #32
	ldrh	r6, [r7, #10]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r0, [r7, #10]
	lsrs	r1, r0, #32
	ldrh	r6, [r7, #10]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r6, [r7, #10]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r0, [r7, #10]
	lsrs	r1, r0, #32
	mov	r3, r8
	mov	r2, ip
	adds	r5, #1
	strh	r5, [r3, r2]
	mov	r3, r9
	strh	r1, [r3, r2]
	cmp	r7, r1
	bcs.n	.L_0801895a
	adds	r7, r1, #0
.L_0801895a:
	mov	r3, sl
	cmp	r3, #2
	bhi.n	.L_0801896a
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	lsls	r2, r2, #1
	mov	ip, r2
.L_0801896a:
	ldr	r2, [pc, #136]
	movs	r3, #15
	movs	r5, #0
	movs	r1, #0
	add	fp, r3
	mov	lr, r2
	b.n	.L_0801888a
	ldr	r3, [pc, #124]
	adds	r0, #1
	ands	r0, r3
	ldr	r3, [pc, #120]
	adds	r0, #1
	ands	r0, r3
	b.n	.L_0801888a
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
	b.n	.L_0801888a
	mov	r3, r8
	mov	r2, ip
	adds	r5, #1
	strh	r5, [r3, r2]
	mov	r3, r9
	strh	r1, [r3, r2]
	cmp	r7, r1
	bcs.n	.L_080189b4
	adds	r7, r1, #0
.L_080189b4:
	ldr	r1, [pc, #76]
	adds	r3, r4, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080189c0
	adds	r7, #2
.L_080189c0:
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
	beq.n	.L_08018a3e
	movs	r2, #0
	movs	r5, #0
.L_080189de:
	mov	r1, r8
	ldrh	r3, [r5, r1]
	cmp	r3, #1
	bhi.n	.L_08018a08
	ldr	r3, [pc, #4]
	strh	r3, [r6, #0]
	b.n	.L_08018a34
	.4byte 0x00000000
	.4byte 0x03001e8c
	.4byte 0x080188d4
	.4byte 0x000001ff
	.4byte 0x08032224
	.4byte 0x00000eac
	.2byte 0x0ea4
	.2byte 0x0000
.L_08018a08:
	mov	r1, r9
	ldrh	r3, [r5, r1]
	subs	r0, r7, r3
	subs	r0, #4
	cmp	r0, #0
	bge.n	.L_08018a16
	movs	r0, #0
.L_08018a16:
	mov	r3, r8
	ldrh	r1, [r5, r3]
	lsls	r0, r0, #8
	subs	r1, #1
	str	r2, [sp, #4]
	bl	sub_080022ec
	movs	r1, #192
	lsls	r1, r1, #4
	ldr	r2, [sp, #4]
	cmp	r0, r1
	bls.n	.L_08018a32
	movs	r0, #128
	lsls	r0, r0, #2
.L_08018a32:
	strh	r0, [r6, #0]
.L_08018a34:
	adds	r6, #2
	adds	r2, #1
	adds	r5, #2
	cmp	r2, sl
	bls.n	.L_080189de
.L_08018a3e:
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
