.syntax unified
	.thumb
	.set sub_0800206c, 0x0800206c
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_08014878, 0x08014878
	.set sub_08014d00, 0x08014d00
	.set sub_08014d78, 0x08014d78
	.set sub_08015b4e, 0x08015b4e
	.set sub_08015b84, 0x08015b84
	.set sub_08015bbc, 0x08015bbc
	.set sub_08015bf8, 0x08015bf8
	.set sub_08015ec8, 0x08015ec8
	.set sub_08015f0c, 0x08015f0c
	.set sub_08015ffc, 0x08015ffc
	.set sub_08016ea4, 0x08016ea4
	.set sub_08016f5c, 0x08016f5c
	.set sub_0801713c, 0x0801713c
	.set sub_080171fc, 0x080171fc
	.global Func_08015778
	.thumb_func
Func_08015778:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #12
	ldr	r3, [pc, #148]
	adds	r7, r1, #0
	mov	r1, sp
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x466e
	ldr	r3, [r6, #8]
	ldr	r0, [pc, #140]
	negs	r1, r3
	ldr	r3, [r0, #4]
	mov	sl, r0
	cmp	r1, r3
	blt.n	.L_08015806
	ldr	r3, [r0, #8]
	cmp	r1, r3
	bgt.n	.L_08015806
	asrs	r3, r1, #16
	str	r3, [r7, #8]
	ldr	r0, [r0, #0]
	cmp	r0, #0
	beq.n	.L_080157bc
	lsrs	r1, r1, #11
	lsls	r0, r0, #5
	ldr	r3, [pc, #112]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c05
	b.n	.L_080157be
.L_080157bc:
	ldr	r5, [pc, #104]
.L_080157be:
	ldr	r1, [pc, #108]
	ldr	r0, [r6, #0]
	mov	r9, r1
	adds	r1, r5, #0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x2800
	bge.n	.L_080157d6
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r0, r0, r2
.L_080157d6:
	mov	r4, sl
	ldr	r2, [r4, #12]
	asrs	r3, r0, #16
	adds	r2, r2, r3
	ldr	r0, [r6, #4]
	adds	r1, r5, #0
	mov	r8, r2
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x2800
	bge.n	.L_080157f4
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r0, r0, r1
.L_080157f4:
	mov	r4, sl
	ldr	r3, [r4, #16]
	asrs	r2, r0, #16
	subs	r3, r3, r2
	mov	r0, r8
	str	r0, [r7, #0]
	str	r3, [r7, #4]
	adds	r0, r5, #0
	b.n	.L_08015810
.L_08015806:
	adds	r2, r7, #0
	adds	r3, r6, #0
	ldmia	r3!, {r0, r1, r4}
	stmia	r2!, {r0, r1, r4}
	movs	r0, #0
.L_08015810:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x03000354
	.4byte 0x030011e0
	.4byte 0x03000534
	.4byte 0x000151eb
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #60]
	adds	r6, r1, #0
	adds	r2, r3, #1
	adds	r3, #4
	lsrs	r3, r3, #2
	lsls	r3, r3, #2
	mov	r1, sp
	subs	r1, r1, r3
	movs	r4, #132
	movs	r3, #128
	lsls	r4, r4, #24
	lsrs	r2, r2, #2
	lsls	r3, r3, #19
	mov	r7, sp
	adds	r5, r0, #0
	mov	r8, sp
	adds	r3, #212
	mov	sp, r1
	ldr	r0, [pc, #28]
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r5, #0
	adds	r1, r6, #0
	mov	lr, sp
	.2byte 0xf800
	.2byte 0x46c5
	mov	sp, r7
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x00000057
	.2byte 0x1ffc
	.2byte 0x0800
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	mov	r8, r0
	mov	sl, r1
	ldr	r5, [pc, #56]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #40]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r0, r8
	mov	r1, sl
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c05
	adds	r0, r6, #0
	bl	sub_08013164
	adds	r0, r5, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x000002c8
	.2byte 0x28a8
	.2byte 0x0801
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	mov	r8, r0
	mov	sl, r1
	ldr	r5, [pc, #56]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #40]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r0, r8
	mov	r1, sl
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c05
	adds	r0, r6, #0
	bl	sub_08013164
	adds	r0, r5, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x000004f0
	.2byte 0x2b70
	.2byte 0x0801
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	mov	r8, r0
	mov	sl, r1
	ldr	r5, [pc, #56]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #40]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r0, r8
	mov	r1, sl
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c05
	adds	r0, r6, #0
	bl	sub_08013164
	adds	r0, r5, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x0000025c
	.2byte 0x1658
	.2byte 0x0800
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #196
	lsls	r1, r1, #6
	adds	r1, #68
	movs	r0, #204
	sub	sp, #36
	bl	sub_08014d00
	movs	r3, #0
	mov	fp, r0
	add	r0, sp, #16
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	mov	r1, fp
	ldr	r2, [pc, #48]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r1, [pc, #48]
	movs	r0, #2
	bl	sub_08016f5c
	movs	r0, #0
	mov	r8, r0
	b.n	.L_080159b6
.L_080159ac:
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #1
	add	r8, r1
.L_080159b6:
	mov	r2, r8
	cmp	r2, #7
	bhi.n	.L_080159c8
	bl	sub_08016ea4
	lsls	r0, r0, #16
	cmp	r0, #0
	bne.n	.L_080159ac
	b.n	.L_080159d4
.L_080159c8:
	movs	r0, #1
	b.n	.L_08015abe
	.4byte 0x85000c51
	.2byte 0x01f8
	.2byte 0x0300
.L_080159d4:
	mov	r1, fp
	adds	r1, #2
	mov	r0, fp
	mov	r2, fp
	movs	r3, #0
	adds	r0, #3
	str	r1, [sp, #12]
	adds	r2, #60
	movs	r1, #12
	mov	r8, r3
	str	r0, [sp, #8]
	movs	r3, #20
	mov	r7, fp
	movs	r0, #30
	str	r2, [sp, #4]
	str	r1, [sp, #0]
	add	r3, sp
	adds	r0, r0, r7
	mov	sl, r3
	mov	r9, r0
.L_080159fc:
	movs	r3, #0
	strb	r3, [r7, #0]
	ldr	r2, [sp, #0]
	ldr	r0, [sp, #8]
	movs	r3, #16
	strb	r3, [r2, r0]
	ldr	r3, [pc, #32]
	mov	r1, r9
	strh	r3, [r1, #0]
	mov	r0, r8
	bl	.L_08015c08
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r5, r0, #0
	adds	r3, #212
	ldr	r0, [sp, #4]
	add	r1, sp, #20
	adds	r2, #4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L_08015a30
	.2byte 0x0000
	.2byte 0x0000
.L_08015a30:
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #212
	ldr	r3, [r2, #8]
	movs	r0, #128
	lsls	r0, r0, #24
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_08015a30
	mov	r0, sl
	ldr	r1, [pc, #132]
	movs	r2, #7
	bl	sub_08015ffc
	cmp	r0, #0
	bne.n	.L_08015aa6
	mov	r1, sl
	ldrh	r3, [r1, #10]
	mov	r2, r9
	strh	r3, [r2, #0]
	ldrb	r2, [r1, #7]
	adds	r1, r2, #0
	cmp	r1, #15
	bhi.n	.L_08015aa6
	cmp	r5, #0
	bne.n	.L_08015aa6
	movs	r3, #1
	strb	r3, [r7, #0]
	ldr	r3, [sp, #0]
	ldr	r0, [sp, #8]
	strb	r2, [r3, r0]
	cmp	r5, r8
	bcs.n	.L_08015aa6
	mov	lr, r1
	ldr	r1, [sp, #8]
	movs	r6, #0
	mov	r0, fp
	movs	r4, #28
	adds	r1, #12
.L_08015a7e:
	ldrb	r3, [r1, #0]
	adds	r1, #1
	mov	ip, r3
	cmp	ip, lr
	bne.n	.L_08015a9c
	ldr	r3, [sp, #12]
	ldrh	r2, [r3, r4]
	mov	r3, sl
	ldrh	r3, [r3, #10]
	mov	ip, r3
	cmp	r2, ip
	bcs.n	.L_08015a9a
	strb	r6, [r0, #0]
	b.n	.L_08015a9c
.L_08015a9a:
	strb	r6, [r7, #0]
.L_08015a9c:
	adds	r5, #1
	adds	r0, #1
	adds	r4, #2
	cmp	r5, r8
	bcc.n	.L_08015a7e
.L_08015aa6:
	ldr	r1, [sp, #0]
	movs	r2, #3
	add	r8, r2
	movs	r0, #6
	adds	r1, #3
	mov	r3, r8
	adds	r7, #3
	add	r9, r0
	str	r1, [sp, #0]
	cmp	r3, #14
	bls.n	.L_080159fc
	movs	r0, #0
.L_08015abe:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x7d08
	.2byte 0x0801
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #204
	ldr	r2, [r3, #0]
	sub	sp, #60
	movs	r6, #0
	movs	r5, #0
	mov	r1, sp
.L_08015ae2:
	ldrb	r3, [r2, #0]
	adds	r2, #3
	cmp	r3, #0
	bne.n	.L_08015aee
	stmia	r1!, {r5}
	adds	r6, #1
.L_08015aee:
	adds	r5, #3
	cmp	r5, #14
	bls.n	.L_08015ae2
	movs	r5, #15
	cmp	r6, #0
	beq.n	.L_08015b1e
	cmp	r6, #1
	bne.n	.L_08015b0c
	ldr	r5, [sp, #0]
	bl	sub_08015f0c
	cmp	r0, #15
	bne.n	.L_08015b1e
	movs	r5, #15
	b.n	.L_08015b1e
.L_08015b0c:
	bl	sub_08014878
	adds	r1, r6, #0
	bl	sub_0800206c
	adds	r5, r0, #0
	mov	r2, sp
	lsls	r3, r5, #2
	ldr	r5, [r2, r3]
.L_08015b1e:
	adds	r0, r5, #0
	add	sp, #60
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #204
	ldr	r3, [r3, #0]
	mov	sl, r0
	mov	r8, r3
	movs	r3, #196
	lsls	r3, r3, #6
	adds	r3, #60
	add	r3, r8
	ldr	r5, [r3, #0]
	cmp	r5, #0
	beq.n	sub_08015b4e
	movs	r0, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4a2d
	mov	r7, r8
	mov	r9, r2
	mov	r2, sl
	lsls	r3, r2, #16
	lsrs	r6, r3, #16
	mov	r2, r9
	adds	r7, #60
	ldr	r3, [r2, #0]
	adds	r0, r6, #0
	adds	r1, r7, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x0400
	cmp	r0, #0
	bne.n	.L_08015bea
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_080171fc
	cmp	r0, #0
	bne.n	.L_08015bea
	cmp	r5, #0
	beq.n	sub_08015b84
	movs	r0, #1
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4653
	movs	r7, #128
	adds	r3, #1
	lsls	r7, r7, #5
	lsls	r3, r3, #16
	adds	r7, #60
	lsrs	r6, r3, #16
	mov	r2, r9
	add	r7, r8
	ldr	r3, [r2, #0]
	adds	r0, r6, #0
	adds	r1, r7, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x0400
	cmp	r0, #0
	bne.n	.L_08015bea
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_080171fc
	cmp	r0, #0
	bne.n	.L_08015bea
	cmp	r5, #0
	beq.n	sub_08015bbc
	movs	r0, #1
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4653
	movs	r7, #128
	adds	r3, #2
	lsls	r7, r7, #6
	lsls	r3, r3, #16
	adds	r7, #60
	lsrs	r6, r3, #16
	mov	r2, r9
	add	r7, r8
	ldr	r3, [r2, #0]
	adds	r0, r6, #0
	adds	r1, r7, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x0400
	cmp	r0, #0
	bne.n	.L_08015bea
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_080171fc
	cmp	r0, #0
	beq.n	.L_08015bee
.L_08015bea:
	movs	r0, #1
	b.n	.L_08015bfa
.L_08015bee:
	cmp	r5, #0
	beq.n	sub_08015bf8
	movs	r0, #1
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2000
.L_08015bfa:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x7404
	.2byte 0x0200
.L_08015c08:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #204
	ldr	r6, [r3, #0]
	movs	r3, #60
	adds	r5, r0, #0
	adds	r3, r3, r6
	mov	sl, r3
	lsls	r0, r5, #16
	movs	r3, #128
	lsls	r3, r3, #5
	mov	r2, sl
	lsrs	r0, r0, #16
	movs	r1, #0
	sub	sp, #16
	mov	r8, r3
	bl	sub_0801713c
	movs	r3, #128
	adds	r0, r5, #1
	lsls	r3, r3, #5
	adds	r3, #60
	lsls	r0, r0, #16
	adds	r2, r6, r3
	lsrs	r0, r0, #16
	mov	r3, r8
	movs	r1, #0
	bl	sub_0801713c
	movs	r3, #128
	adds	r5, #2
	lsls	r3, r3, #6
	adds	r3, #60
	lsls	r5, r5, #16
	adds	r6, r6, r3
	lsrs	r5, r5, #16
	adds	r0, r5, #0
	adds	r2, r6, #0
	mov	r3, r8
	movs	r1, #0
	bl	sub_0801713c
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	mov	r0, sl
	mov	r1, sp
	adds	r2, #4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #19
	movs	r2, #128
	adds	r1, #212
	lsls	r2, r2, #24
.L_08015c82:
	ldr	r3, [r1, #8]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08015c82
	bl	sub_08015ec8
	mov	r3, sp
	ldrh	r3, [r3, #8]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	subs	r0, r0, r3
	add	sp, #16
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.align 2, 0
