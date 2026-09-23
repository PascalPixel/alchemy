.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_080140d8, 0x080140d8
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_080145a8, 0x080145a8
	.set sub_08014878, 0x08014878
	.set sub_08014d00, 0x08014d00
	.set sub_0801591c, 0x0801591c
	.set sub_08016ce4, 0x08016ce4
	.set sub_080201c0, 0x080201c0
	.set sub_080d2c64, 0x080d2c64
	.global Func_080d6b90
	.thumb_func
Func_080d6b90:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r1, #20
	movs	r0, #116
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #128
	adds	r5, r0, #0
	adds	r7, r5, #0
	movs	r2, #0
	mov	r0, sp
	lsls	r3, r3, #19
	str	r2, [r0, #0]
	adds	r7, #8
	adds	r3, #212
	adds	r1, r5, #0
	ldr	r2, [pc, #148]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #3
	movs	r0, #56
	bl	sub_08014d00
	adds	r6, r0, #0
	adds	r1, r6, #0
	ldr	r0, [pc, #132]
	bl	sub_0801591c
	bl	sub_080143ac
	movs	r1, #192
	adds	r2, r6, #0
	lsls	r1, r1, #2
	str	r0, [r5, #0]
	bl	sub_080142d4
	str	r0, [r5, #4]
	movs	r0, #56
	bl	sub_0801314c
	movs	r6, #0
	mov	r8, r6
.L_080d6bee:
	adds	r2, r7, #0
	mov	r3, r8
	stmia	r2!, {r3}
	ldr	r3, [pc, #96]
	stmia	r2!, {r3}
	movs	r3, #212
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	bl	sub_08014878
	movs	r3, #200
	adds	r5, r0, #0
	muls	r5, r3
	movs	r3, #144
	lsls	r3, r3, #17
	adds	r5, r5, r3
	bl	sub_08014878
	lsls	r3, r0, #2
	adds	r3, r3, r0
	movs	r2, #160
	lsls	r3, r3, #2
	lsls	r2, r2, #15
	subs	r2, r2, r3
	str	r5, [r7, #12]
	str	r2, [r7, #20]
	movs	r0, #0
	adds	r1, r5, #0
	bl	sub_080201c0
	mov	r3, r8
	str	r3, [r7, #24]
	lsls	r3, r6, #1
	adds	r6, #1
	str	r0, [r7, #16]
	strh	r3, [r7, #28]
	adds	r7, #32
	cmp	r6, #63
	bls.n	.L_080d6bee
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #24]
	bl	sub_080145a8
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x85000205
	.4byte 0x080f385e
	.4byte 0x40000400
	.2byte 0x69f5
	.2byte 0x080d
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #116]
	ldr	r3, [r3, #32]
	sub	sp, #12
	movs	r1, #0
	mov	fp, r0
	str	r3, [sp, #8]
	str	r1, [sp, #4]
	str	r1, [sp, #0]
	mov	r7, fp
	adds	r7, #8
.L_080d6c86:
	ldrh	r3, [r7, #28]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r1, r3, r2
	adds	r3, r1, #0
	ands	r3, r2
	strh	r1, [r7, #28]
	cmp	r3, r2
	bne.n	.L_080d6c9c
	b.n	.L_080d6dea
.L_080d6c9c:
	ldr	r3, [sp, #8]
	movs	r0, #179
	adds	r3, #228
	ldr	r6, [r3, #0]
	ldr	r3, [r3, #4]
	lsls	r0, r0, #1
	mov	sl, r3
	lsls	r3, r1, #16
	asrs	r3, r3, #16
	mov	r9, r3
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080d6cc4
	ldrh	r3, [r7, #28]
	adds	r3, #1
	strh	r3, [r7, #28]
	ldr	r3, [r7, #24]
	subs	r3, #1
	str	r3, [r7, #24]
.L_080d6cc4:
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	ldr	r1, [r7, #12]
	movs	r3, #1
	ands	r0, r3
	ands	r3, r5
	adds	r3, r3, r0
	subs	r1, r1, r6
	lsrs	r3, r3, #1
	asrs	r1, r1, #16
	adds	r1, r1, r3
	subs	r2, r1, #1
	ldr	r3, [r7, #16]
	mov	r8, r2
	ldr	r2, [r7, #20]
	mov	r0, r9
	subs	r2, r2, r3
	mov	r3, sl
	subs	r2, r2, r3
	lsls	r3, r0, #16
	asrs	r2, r2, #16
	lsrs	r3, r3, #16
	adds	r1, #15
	subs	r4, r2, r3
	cmp	r1, #255
	bls.n	.L_080d6d00
	b.n	.L_080d6de6
.L_080d6d00:
	movs	r1, #32
	negs	r1, r1
	cmp	r4, r1
	blt.n	.L_080d6de6
	cmp	r4, #159
	bgt.n	.L_080d6de6
	ldrh	r3, [r7, #28]
	cmp	r3, #59
	bhi.n	.L_080d6d32
	mov	r2, fp
	ldr	r3, [r2, #4]
	movs	r0, #192
	ldrh	r2, [r7, #8]
	ldr	r1, [pc, #60]
	lsls	r0, r0, #2
	adds	r3, #16
	adds	r0, #255
	ands	r3, r0
	ands	r2, r1
	orrs	r2, r3
	ldr	r3, [r7, #24]
	strh	r2, [r7, #8]
	adds	r3, #3
	str	r3, [r7, #24]
	b.n	.L_080d6d72
.L_080d6d32:
	cmp	r3, #89
	bhi.n	.L_080d6d5c
	mov	r2, fp
	ldr	r3, [r2, #4]
	movs	r0, #192
	ldrh	r2, [r7, #8]
	ldr	r1, [pc, #24]
	lsls	r0, r0, #2
	adds	r3, #8
	adds	r0, #255
	ands	r3, r0
	ands	r2, r1
	orrs	r2, r3
	ldr	r3, [r7, #24]
	strh	r2, [r7, #8]
	adds	r3, #1
	str	r3, [r7, #24]
	b.n	.L_080d6d72
	movs	r0, r0
	.2byte 0xfc00
	.2byte 0xffff
.L_080d6d5c:
	mov	r3, fp
	ldr	r2, [r3, #4]
	movs	r0, #192
	ldrh	r3, [r7, #8]
	ldr	r1, [pc, #116]
	lsls	r0, r0, #2
	adds	r0, #255
	ands	r2, r0
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #8]
.L_080d6d72:
	ldr	r3, [pc, #108]
	movs	r2, #1
	ldr	r3, [r3, #0]
	lsrs	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d6d98
	ldrh	r2, [r7, #8]
	movs	r0, #192
	lsls	r3, r2, #22
	ldr	r1, [pc, #84]
	lsrs	r3, r3, #22
	lsls	r0, r0, #2
	adds	r3, #4
	adds	r0, #255
	ands	r3, r0
	ands	r2, r1
	orrs	r2, r3
	strh	r2, [r7, #8]
.L_080d6d98:
	ldr	r3, [pc, #56]
	mov	r2, r8
	ands	r2, r3
	mov	r8, r2
	ldrh	r3, [r7, #6]
	ldr	r2, [pc, #52]
	mov	r0, r8
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r7, #6]
	ldr	r3, [r7, #24]
	ldrb	r1, [r7, #5]
	asrs	r3, r3, #2
	subs	r3, r4, r3
	movs	r2, #63
	strb	r3, [r7, #4]
	adds	r3, r2, #0
	ands	r3, r1
	strb	r3, [r7, #5]
	ldrb	r3, [r7, #7]
	adds	r0, r7, #0
	ands	r2, r3
	movs	r3, #64
	orrs	r2, r3
	strb	r2, [r7, #7]
	movs	r1, #240
	bl	sub_080140d8
	b.n	.L_080d6de4
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0xfffffc00
	.2byte 0x122c
	.2byte 0x0300
.L_080d6de4:
	b.n	.L_080d6dea
.L_080d6de6:
	movs	r3, #0
	strh	r3, [r7, #28]
.L_080d6dea:
	ldr	r1, [sp, #4]
	cmp	r1, #7
	bhi.n	.L_080d6e38
	ldrh	r2, [r7, #28]
	mov	sl, r2
	cmp	r2, #0
	bne.n	.L_080d6e38
	ldr	r3, [sp, #8]
	ldr	r5, [pc, #88]
	ldr	r6, [r3, #0]
	bl	sub_08014878
	ldr	r3, [r6, #0]
	lsls	r0, r0, #8
	adds	r3, r3, r0
	adds	r3, r3, r5
	mov	r8, r3
	bl	sub_08014878
	ldr	r3, [r6, #8]
	lsls	r0, r0, #8
	adds	r3, r3, r0
	adds	r4, r3, r5
	mov	r0, r8
	str	r0, [r7, #12]
	mov	r1, r8
	adds	r2, r4, #0
	str	r4, [r7, #20]
	movs	r0, #0
	bl	sub_080201c0
	movs	r3, #120
	mov	r1, sl
	str	r0, [r7, #16]
	strh	r3, [r7, #28]
	str	r1, [r7, #24]
	ldr	r2, [sp, #4]
	adds	r2, #1
	str	r2, [sp, #4]
.L_080d6e38:
	ldr	r3, [sp, #0]
	adds	r7, #32
	adds	r3, #1
	str	r3, [sp, #0]
	cmp	r3, #63
	bhi.n	.L_080d6e46
	b.n	.L_080d6c86
.L_080d6e46:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xff80
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r1, #20
	movs	r0, #116
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #128
	adds	r5, r0, #0
	adds	r7, r5, #0
	movs	r1, #0
	mov	r0, sp
	lsls	r3, r3, #19
	str	r1, [r0, #0]
	adds	r7, #8
	adds	r3, #212
	adds	r1, r5, #0
	ldr	r2, [pc, #152]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #3
	movs	r0, #56
	bl	sub_08014d00
	adds	r6, r0, #0
	adds	r1, r6, #0
	ldr	r0, [pc, #136]
	bl	sub_0801591c
	bl	sub_080143ac
	movs	r1, #192
	adds	r2, r6, #0
	str	r0, [r5, #0]
	lsls	r1, r1, #2
	bl	sub_080142d4
	str	r0, [r5, #4]
	movs	r0, #56
	bl	sub_0801314c
	movs	r2, #192
	lsls	r2, r2, #18
	movs	r5, #0
	mov	r8, r2
.L_080d6eba:
	mov	r2, r8
	ldr	r3, [r2, #32]
	adds	r1, r7, #0
	movs	r6, #0
	stmia	r1!, {r6}
	ldr	r2, [r3, #0]
	ldr	r3, [pc, #92]
	movs	r0, #0
	stmia	r1!, {r3}
	movs	r3, #212
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	ldr	r1, [r2, #0]
	ldr	r2, [r2, #8]
	str	r1, [r7, #12]
	str	r2, [r7, #20]
	bl	sub_080201c0
	ldr	r2, [pc, #56]
	adds	r3, r5, #0
	ands	r3, r2
	adds	r3, #1
	adds	r5, #1
	str	r0, [r7, #16]
	strh	r3, [r7, #28]
	adds	r7, #32
	cmp	r5, #63
	bls.n	.L_080d6eba
	movs	r3, #128
	movs	r2, #252
	lsls	r3, r3, #19
	lsls	r2, r2, #6
	adds	r3, #80
	strh	r2, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r2, #8
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r6, [r3, #0]
	ldr	r0, [pc, #24]
	movs	r1, #144
	lsls	r1, r1, #3
	bl	sub_080145a8
	b.n	.L_080d6f2c
	.4byte 0x0000000f
	.4byte 0x85000205
	.4byte 0x080f385e
	.4byte 0x40000400
	.2byte 0x6c61
	.2byte 0x080d
.L_080d6f2c:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #128
	lsls	r1, r1, #4
	mov	sl, r0
	adds	r1, #20
	movs	r0, #116
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #128
	adds	r5, r0, #0
	adds	r7, r5, #0
	movs	r1, #0
	mov	r0, sp
	lsls	r3, r3, #19
	str	r1, [r0, #0]
	adds	r7, #8
	adds	r3, #212
	adds	r1, r5, #0
	ldr	r2, [pc, #152]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #3
	movs	r0, #56
	bl	sub_08014d00
	adds	r6, r0, #0
	adds	r1, r6, #0
	ldr	r0, [pc, #136]
	bl	sub_0801591c
	bl	sub_080143ac
	movs	r1, #192
	adds	r2, r6, #0
	str	r0, [r5, #0]
	lsls	r1, r1, #2
	bl	sub_080142d4
	str	r0, [r5, #4]
	movs	r0, #56
	bl	sub_0801314c
	movs	r2, #192
	lsls	r2, r2, #18
	movs	r5, #0
	mov	r8, r2
.L_080d6f9a:
	mov	r2, r8
	ldr	r3, [r2, #32]
	adds	r1, r7, #0
	movs	r6, #0
	stmia	r1!, {r6}
	ldr	r2, [r3, #0]
	ldr	r3, [pc, #92]
	movs	r0, #0
	stmia	r1!, {r3}
	movs	r3, #212
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	ldr	r1, [r2, #0]
	ldr	r2, [r2, #8]
	str	r1, [r7, #12]
	str	r2, [r7, #20]
	bl	sub_080201c0
	ldr	r2, [pc, #56]
	adds	r3, r5, #0
	ands	r3, r2
	adds	r3, #1
	adds	r5, #1
	str	r0, [r7, #16]
	strh	r3, [r7, #28]
	adds	r7, #32
	cmp	r5, #63
	bls.n	.L_080d6f9a
	movs	r3, #128
	movs	r2, #252
	lsls	r3, r3, #19
	lsls	r2, r2, #6
	adds	r3, #80
	strh	r2, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r2, #16
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r6, [r3, #0]
	ldr	r0, [pc, #24]
	movs	r1, #144
	lsls	r1, r1, #3
	bl	sub_080145a8
	b.n	.L_080d700c
	.4byte 0x0000000b
	.4byte 0x85000205
	.4byte 0x080f3901
	.4byte 0x40000400
	.2byte 0x6675
	.2byte 0x080d
.L_080d700c:
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_080d7018
	movs	r0, #137
	bl	sub_080d2c64
.L_080d7018:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
