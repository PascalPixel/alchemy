.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_080140d8, 0x080140d8
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014d00, 0x08014d00
	.set sub_0801591c, 0x0801591c
	.set sub_08016ce4, 0x08016ce4
	.set sub_080201c0, 0x080201c0
	.global Func_080d7024
	.thumb_func
Func_080d7024:
	push	{lr}
	ldr	r0, [pc, #8]
	bl	sub_08014644
	pop	{pc}
	movs	r0, r0
	.2byte 0x6675
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
	sub	sp, #28
	movs	r1, #0
	str	r0, [sp, #24]
	str	r1, [sp, #20]
	adds	r7, r0, #0
	ldr	r3, [r3, #32]
	str	r1, [sp, #4]
	str	r3, [sp, #12]
	adds	r3, #228
	str	r3, [sp, #8]
	str	r1, [sp, #0]
	str	r1, [sp, #16]
	adds	r7, #8
	mov	r9, r1
	mov	fp, r1
	mov	r8, r1
.L_080d7068:
	ldrh	r3, [r7, #28]
	cmp	r3, #0
	bne.n	.L_080d7070
	b.n	.L_080d717c
.L_080d7070:
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #8]
	ldr	r2, [r2, #0]
	ldr	r3, [r3, #4]
	movs	r0, #179
	lsls	r0, r0, #1
	mov	r8, r2
	mov	sl, r3
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080d708e
	ldrh	r3, [r7, #28]
	adds	r3, #1
	strh	r3, [r7, #28]
.L_080d708e:
	ldrh	r3, [r7, #28]
	ldr	r2, [pc, #164]
	lsrs	r3, r3, #1
	lsls	r3, r3, #2
	adds	r6, r3, r2
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	ldr	r2, [r7, #12]
	movs	r3, #1
	ands	r0, r3
	mov	r1, r8
	ands	r3, r5
	subs	r2, r2, r1
	adds	r3, r3, r0
	asrs	r2, r2, #16
	lsrs	r3, r3, #1
	adds	r1, r2, r3
	subs	r2, r1, #1
	mov	fp, r2
	ldr	r3, [r7, #20]
	ldr	r2, [r7, #16]
	mov	r0, sl
	subs	r3, r3, r2
	subs	r3, r3, r0
	cmp	r3, #0
	bge.n	.L_080d70d0
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r3, r3, r2
.L_080d70d0:
	movs	r0, #0
	ldrsh	r2, [r6, r0]
	asrs	r3, r3, #16
	adds	r3, r3, r2
	mov	r8, r3
	adds	r3, r1, #0
	adds	r3, #15
	adds	r6, #2
	cmp	r3, #255
	bhi.n	.L_080d7166
	movs	r1, #32
	negs	r1, r1
	cmp	r8, r1
	blt.n	.L_080d7166
	mov	r2, r8
	cmp	r2, #159
	bgt.n	.L_080d7166
	ldr	r3, [sp, #24]
	ldr	r2, [pc, #48]
	ldr	r1, [r3, #4]
	ldrh	r3, [r6, #0]
	mov	r0, r8
	adds	r1, r1, r3
	ldr	r3, [pc, #44]
	strb	r0, [r7, #4]
	ands	r1, r3
	ldrh	r3, [r7, #8]
	movs	r0, #63
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r7, #8]
	ldr	r3, [pc, #32]
	mov	r1, fp
	ands	r1, r3
	ldr	r2, [pc, #28]
	ldrh	r3, [r7, #6]
	negs	r0, r0
	ands	r3, r2
	ldrb	r2, [r7, #5]
	orrs	r3, r1
	movs	r1, #63
	strh	r3, [r7, #6]
	b.n	.L_080d713c
	movs	r0, r0
	.4byte 0xfffffc00
	.4byte 0x000003ff
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.2byte 0x0b7c
	.2byte 0x080f
.L_080d713c:
	adds	r3, r1, #0
	ands	r3, r2
	strb	r3, [r7, #5]
	ldrb	r3, [r7, #7]
	ldr	r2, [pc, #44]
	ands	r1, r3
	movs	r3, #64
	orrs	r1, r3
	ldr	r3, [pc, #40]
	ldr	r3, [r3, #0]
	lsrs	r3, r3, #1
	ands	r3, r2
	adds	r2, r0, #0
	lsls	r3, r3, #4
	ands	r1, r2
	orrs	r1, r3
	strb	r1, [r7, #7]
	adds	r0, r7, #0
	movs	r1, #240
	bl	sub_080140d8
.L_080d7166:
	ldrh	r3, [r7, #28]
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r3, r1
	strh	r3, [r7, #28]
	b.n	.L_080d717c
	.4byte 0x00000001
	.2byte 0x122c
	.2byte 0x0300
.L_080d717c:
	ldr	r2, [sp, #20]
	cmp	r2, #3
	bhi.n	.L_080d721e
	ldrh	r3, [r7, #28]
	cmp	r3, #0
	bne.n	.L_080d721e
	ldr	r0, [sp, #24]
	movs	r1, #129
	lsls	r1, r1, #4
	adds	r3, r0, r1
	ldr	r5, [r3, #0]
	cmp	r5, #0
	bne.n	.L_080d721e
	mov	r2, r9
	cmp	r2, #0
	beq.n	.L_080d71cc
	ldr	r3, [sp, #4]
	mov	r1, fp
	str	r3, [r7, #12]
	ldr	r0, [sp, #0]
	mov	r2, r8
	str	r0, [r7, #20]
	movs	r0, #0
	bl	sub_080201c0
	ldr	r3, [pc, #24]
	mov	r1, r9
	subs	r3, r3, r1
	strh	r3, [r7, #28]
	str	r0, [r7, #16]
	str	r5, [r7, #24]
	ldr	r2, [sp, #20]
	movs	r3, #4
	adds	r2, #1
	str	r2, [sp, #20]
	add	r9, r3
	b.n	.L_080d721e
	movs	r0, r0
	.2byte 0x003e
	.2byte 0x0000
.L_080d71cc:
	bl	sub_08014878
	movs	r3, #255
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_080d721e
	ldr	r0, [sp, #12]
	ldr	r5, [pc, #96]
	ldr	r6, [r0, #0]
	bl	sub_08014878
	ldr	r3, [r6, #0]
	lsls	r0, r0, #8
	adds	r3, r3, r0
	adds	r3, r3, r5
	str	r3, [sp, #4]
	bl	sub_08014878
	ldr	r3, [r6, #8]
	ldr	r1, [sp, #4]
	lsls	r0, r0, #8
	adds	r3, r3, r0
	adds	r3, r3, r5
	str	r3, [sp, #0]
	mov	r2, r8
	str	r1, [r7, #12]
	str	r3, [r7, #20]
	movs	r0, #0
	mov	r1, fp
	bl	sub_080201c0
	movs	r3, #30
	mov	r2, r9
	str	r0, [r7, #16]
	strh	r3, [r7, #28]
	str	r2, [r7, #24]
	ldr	r3, [sp, #20]
	movs	r0, #4
	adds	r3, #1
	str	r3, [sp, #20]
	mov	r9, r0
.L_080d721e:
	ldr	r1, [sp, #16]
	adds	r7, #32
	adds	r1, #1
	str	r1, [sp, #16]
	cmp	r1, #63
	bhi.n	.L_080d722c
	b.n	.L_080d7068
.L_080d722c:
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
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
	ldr	r2, [pc, #128]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #3
	movs	r0, #56
	bl	sub_08014d00
	adds	r6, r0, #0
	adds	r1, r6, #0
	ldr	r0, [pc, #112]
	bl	sub_0801591c
	bl	sub_080143ac
	movs	r1, #128
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
	movs	r6, #0
.L_080d72a4:
	mov	r2, r8
	ldr	r3, [r2, #32]
	adds	r1, r7, #0
	stmia	r1!, {r6}
	ldr	r2, [r3, #0]
	ldr	r3, [pc, #68]
	movs	r0, #0
	stmia	r1!, {r3}
	movs	r3, #212
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	ldr	r1, [r2, #0]
	ldr	r2, [r2, #8]
	str	r6, [r7, #12]
	str	r6, [r7, #20]
	bl	sub_080201c0
	ldr	r2, [pc, #32]
	adds	r3, r5, #0
	ands	r3, r2
	adds	r3, #1
	adds	r5, #1
	str	r0, [r7, #16]
	strh	r3, [r7, #28]
	adds	r7, #32
	cmp	r5, #63
	bls.n	.L_080d72a4
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #24]
	bl	sub_080145a8
	add	sp, #4
	b.n	.L_080d72fc
	.4byte 0x0000000f
	.4byte 0x85000205
	.4byte 0x080f38b2
	.4byte 0x40000400
	.2byte 0x7035
	.2byte 0x080d
.L_080d72fc:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
