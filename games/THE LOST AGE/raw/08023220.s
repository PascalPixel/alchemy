.syntax unified
	.thumb
	.set sub_08021918, 0x08021918
	.set sub_08022d40, 0x08022d40
	.set sub_080231a4, 0x080231a4
	.set sub_0802471c, 0x0802471c
	.global Overlay_08023220
Overlay_08023220:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	adds	r7, r0, #0
	mov	fp, r3
	mov	r9, r1
	str	r2, [sp, #0]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_08023246
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r2, #255
	adds	r3, r7, r2
.L_08023246:
	asrs	r5, r3, #12
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #255
	ands	r7, r3
	bl	sub_080231a4
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_0802325c
	b.n	.L_08023396
.L_0802325c:
	movs	r3, #16
	strh	r3, [r6, #32]
	cmp	r5, #0
	beq.n	.L_0802326a
	cmp	r5, #2
	beq.n	.L_0802329c
	b.n	.L_0802330a
.L_0802326a:
	adds	r0, r7, #0
	bl	sub_08022d40
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0802328e
	adds	r2, r6, #0
	movs	r3, #1
	adds	r2, #84
	strb	r3, [r2, #0]
	adds	r0, r7, #0
	str	r5, [r6, #80]
	bl	sub_08021918
	ldrb	r3, [r0, #9]
	lsrs	r3, r3, #1
	strh	r3, [r6, #32]
	b.n	.L_0802330a
.L_0802328e:
	ldr	r3, [pc, #8]
	adds	r2, r6, #0
	adds	r2, #84
	strb	r3, [r2, #0]
	b.n	.L_0802330a
	.2byte 0x0000
	.2byte 0x0000
.L_0802329c:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #24]
	adds	r0, r1, #0
	adds	r0, #24
	ldr	r3, [r0, #0]
	lsls	r2, r3, #2
	adds	r3, #1
	str	r3, [r0, #0]
	adds	r1, r1, r2
	adds	r3, r6, #0
	mov	sl, r1
	adds	r3, #84
	movs	r2, #8
	add	r2, sl
	strb	r5, [r3, #0]
	add	r0, sp, #4
	movs	r3, #0
	mov	r8, r2
	str	r2, [r6, #80]
	str	r3, [r0, #0]
	movs	r2, #133
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	mov	r1, r8
	adds	r2, #4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r7, #0
	bl	sub_08022d40
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080232fa
	adds	r0, r7, #0
	bl	sub_08021918
	ldrb	r3, [r0, #9]
	movs	r2, #12
	lsrs	r3, r3, #1
	strh	r3, [r6, #32]
	mov	r3, r8
	str	r5, [r3, #0]
	add	r2, sl
	mov	r8, r2
.L_080232fa:
	adds	r0, r7, #1
	bl	sub_08022d40
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0802330a
	mov	r3, r8
	str	r5, [r3, #0]
.L_0802330a:
	cmp	r6, #0
	beq.n	.L_08023396
	adds	r0, r6, #0
	mov	r1, r9
	ldr	r2, [sp, #0]
	mov	r3, fp
	bl	sub_0802471c
	ldr	r3, [pc, #72]
	movs	r2, #128
	str	r3, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #10
	adds	r0, r6, #0
	lsls	r2, r2, #9
	str	r3, [r6, #48]
	ldr	r1, [pc, #52]
	movs	r4, #0
	movs	r3, #3
	adds	r0, #85
	str	r2, [r6, #24]
	str	r2, [r6, #28]
	str	r2, [r6, #52]
	strh	r4, [r6, #4]
	strb	r3, [r0, #0]
	str	r2, [r6, #72]
	adds	r3, r6, #0
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r3, #89
	str	r2, [r6, #68]
	strb	r1, [r3, #0]
	adds	r1, r6, #0
	adds	r1, #90
	movs	r3, #1
	strb	r3, [r1, #0]
	mov	r1, r9
	strh	r2, [r6, #6]
	str	r4, [r6, #76]
	cmp	r1, #0
	bge.n	.L_08023370
	b.n	.L_08023368
	movs	r0, r0
	.4byte 0x00000000
	.2byte 0xf200
	.2byte 0x0802
.L_08023368:
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r1, r1, r2
.L_08023370:
	adds	r3, r6, #0
	adds	r3, #100
	asrs	r2, r1, #16
	strh	r2, [r3, #0]
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_08023386
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r3, r3, r2
.L_08023386:
	adds	r2, r6, #0
	asrs	r3, r3, #16
	adds	r2, #102
	strh	r3, [r2, #0]
	str	r4, [r6, #124]
	str	r4, [r6, #112]
	str	r4, [r6, #116]
	str	r4, [r6, #120]
.L_08023396:
	adds	r0, r6, #0
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
