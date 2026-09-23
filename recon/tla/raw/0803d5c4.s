.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0801314c, 0x0801314c
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_08014cc0, 0x08014cc0
	.set sub_0803d4bc, 0x0803d4bc
	.set sub_0803db54, 0x0803db54
	.set sub_080ad010, 0x080ad010
	.global Func_0803d5c4
	.thumb_func
Func_0803d5c4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r1, #0
	movs	r1, #0
	mov	sl, r1
	movs	r1, #193
	adds	r6, r0, #0
	lsls	r1, r1, #3
	movs	r0, #68
	mov	r8, r2
	mov	r9, r3
	bl	sub_08014cc0
	adds	r5, r0, #0
	bl	sub_0803d4bc
	cmp	r6, r0
	bcc.n	.L_0803d5f0
	movs	r6, #0
.L_0803d5f0:
	cmp	r7, #0
	beq.n	.L_0803d61e
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #4
	adds	r2, r5, r3
	ldr	r3, [pc, #120]
	movs	r1, #192
	ldr	r3, [r3, #8]
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	movs	r2, #2
	adds	r3, r5, r1
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_0803db54
	movs	r2, #1
	mov	sl, r2
.L_0803d61e:
	movs	r3, #192
	ldr	r2, [pc, #88]
	lsls	r3, r3, #3
	adds	r3, #4
	adds	r1, r5, r3
	lsls	r3, r6, #2
	ldr	r3, [r2, r3]
	movs	r2, #2
	str	r3, [r1, #0]
	movs	r1, #192
	lsls	r1, r1, #3
	adds	r3, r5, r1
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	mov	r1, sl
	bl	sub_0803db54
	ldr	r2, [sp, #28]
	cmp	r2, #0
	bne.n	.L_0803d654
	bl	sub_080143ac
	mov	r3, r8
	str	r0, [r3, #0]
.L_0803d654:
	movs	r3, #128
	mov	r1, r8
	lsls	r3, r3, #3
	ldr	r0, [r1, #0]
	adds	r2, r5, r3
	movs	r1, #128
	bl	sub_080142d4
	mov	r1, r9
	str	r0, [r1, #0]
	movs	r0, #68
	bl	sub_0801314c
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x0804e684
	.2byte 0xeb58
	.2byte 0x0804
.L_0803d680:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r0, [sp, #8]
	ldr	r2, [sp, #8]
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	mov	fp, r1
	ands	r0, r2
	movs	r1, #0
	str	r1, [sp, #4]
	mov	sl, r1
	bl	sub_080ad010
	str	r0, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #68]
	cmp	r5, #0
	bne.n	.L_0803d6ba
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0803d88c
.L_0803d6ba:
	movs	r3, #1
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0803d6ee
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #4
	adds	r2, r5, r3
	ldr	r3, [pc, #460]
	movs	r1, #192
	ldr	r3, [r3, #8]
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	movs	r2, #2
	adds	r3, r5, r1
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_0803db54
	movs	r2, #1
	str	r2, [sp, #4]
.L_0803d6ee:
	movs	r3, #192
	ldr	r1, [sp, #0]
	lsls	r3, r3, #3
	adds	r3, #4
	adds	r3, r3, r5
	mov	r9, r3
	ldrh	r3, [r1, #6]
	ldr	r2, [pc, #416]
	lsls	r3, r3, #2
	ldr	r3, [r2, r3]
	mov	r2, r9
	str	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #3
	movs	r2, #192
	adds	r3, r3, r5
	lsls	r2, r2, #3
	mov	r8, r3
	adds	r2, #2
	movs	r6, #2
	mov	r1, r8
	adds	r7, r5, r2
	strh	r6, [r1, #0]
	strh	r6, [r7, #0]
	ldr	r1, [sp, #4]
	adds	r0, r5, #0
	bl	sub_0803db54
	movs	r3, #8
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0803d752
	ldr	r2, [sp, #8]
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803d752
	ldr	r3, [pc, #356]
	mov	r1, r9
	ldr	r3, [r3, #4]
	mov	r2, r8
	str	r3, [r1, #0]
	adds	r0, r5, #0
	strh	r6, [r2, #0]
	movs	r1, #1
	strh	r6, [r7, #0]
	bl	sub_0803db54
.L_0803d752:
	movs	r3, #16
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0803d78e
	ldr	r2, [sp, #8]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803d78e
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #4
	adds	r2, r5, r3
	ldr	r3, [pc, #304]
	movs	r1, #192
	ldr	r3, [r3, #0]
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	adds	r3, r5, r1
	movs	r2, #2
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_0803db54
.L_0803d78e:
	movs	r3, #32
	mov	r2, fp
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803d7de
	ldr	r1, [sp, #8]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0803d7de
	ldr	r2, [sp, #0]
	movs	r3, #1
	ldrb	r0, [r2, #3]
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0803d7de
	movs	r1, #2
	adds	r3, r1, #0
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0803d7de
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #4
	adds	r2, r5, r3
	ldr	r3, [pc, #224]
	adds	r0, r5, #0
	ldr	r3, [r3, #8]
	str	r3, [r2, #0]
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r3, r5, r2
	adds	r2, #2
	strh	r1, [r3, #0]
	adds	r3, r5, r2
	strh	r1, [r3, #0]
	movs	r1, #1
	bl	sub_0803db54
.L_0803d7de:
	movs	r3, #2
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0803d802
	ldr	r2, [sp, #8]
	movs	r3, #248
	lsls	r3, r3, #8
	ands	r3, r2
	lsrs	r3, r3, #11
	mov	sl, r3
	movs	r3, #1
	add	sl, r3
	mov	r1, sl
	cmp	r1, #1
	bgt.n	.L_0803d802
	movs	r2, #0
	mov	sl, r2
.L_0803d802:
	movs	r3, #4
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0803d81c
	ldr	r2, [sp, #8]
	movs	r3, #248
	lsls	r3, r3, #8
	ands	r3, r2
	lsrs	r3, r3, #11
	mov	sl, r3
	movs	r3, #1
	add	sl, r3
.L_0803d81c:
	mov	r1, sl
	cmp	r1, #0
	beq.n	.L_0803d888
	cmp	r1, #30
	bgt.n	.L_0803d888
	movs	r1, #10
	mov	r0, sl
	bl	sub_08002064
	ldr	r3, [pc, #120]
	lsls	r0, r0, #2
	movs	r2, #192
	lsls	r2, r2, #3
	mov	fp, r3
	ldr	r3, [r3, r0]
	adds	r2, #4
	adds	r2, r2, r5
	movs	r1, #192
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	movs	r3, #192
	adds	r1, r1, r5
	lsls	r3, r3, #3
	mov	r8, r1
	adds	r3, #2
	movs	r6, #2
	adds	r7, r5, r3
	mov	r9, r2
	mov	r2, r8
	strh	r6, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #1
	strh	r6, [r7, #0]
	bl	sub_0803db54
	mov	r0, sl
	movs	r1, #10
	bl	sub_08002054
	cmp	r0, #0
	beq.n	.L_0803d888
	lsls	r3, r0, #2
	mov	r1, fp
	adds	r3, #36
	ldr	r3, [r1, r3]
	mov	r2, r9
	str	r3, [r2, #0]
	mov	r3, r8
	strh	r6, [r3, #0]
	adds	r0, r5, #0
	strh	r6, [r7, #0]
	movs	r1, #1
	bl	sub_0803db54
.L_0803d888:
	movs	r0, #128
	lsls	r0, r0, #1
.L_0803d88c:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0804e684
	.4byte 0x0804eb58
	.4byte 0x0804e740
	.2byte 0xe7dc
	.2byte 0x0804
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	mov	r8, r1
	movs	r1, #193
	adds	r6, r0, #0
	lsls	r1, r1, #3
	movs	r0, #68
	mov	sl, r2
	bl	sub_08014cc0
	mov	r1, r8
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	.L_0803d680
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r5, r5, r3
	movs	r1, #128
	adds	r2, r5, #0
	mov	r0, sl
	bl	sub_080142d4
	movs	r0, #68
	bl	sub_0801314c
	movs	r0, #1
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
