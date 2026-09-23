.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014d78, 0x08014d78
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016d18, 0x08016d18
	.set sub_08020030, 0x08020030
	.set sub_08038010, 0x08038010
	.set sub_08038068, 0x08038068
	.set sub_08038070, 0x08038070
	.set sub_08038080, 0x08038080
	.set sub_08038090, 0x08038090
	.set sub_080380a8, 0x080380a8
	.set sub_080380b8, 0x080380b8
	.set sub_080380d0, 0x080380d0
	.set sub_08038108, 0x08038108
	.set sub_08038120, 0x08038120
	.set sub_08038260, 0x08038260
	.set sub_08038268, 0x08038268
	.set sub_08038288, 0x08038288
	.set sub_080383e8, 0x080383e8
	.set sub_080ad078, 0x080ad078
	.set sub_080ad100, 0x080ad100
	.set sub_080c8508, 0x080c8508
	.set sub_080f80e0, 0x080f80e0
	.set sub_080f811c, 0x080f811c
	.set sub_080f8170, 0x080f8170
	.set sub_080f8888, 0x080f8888
	.set sub_080f88c4, 0x080f88c4
	.set sub_080f8a44, 0x080f8a44
	.set sub_080f8ab4, 0x080f8ab4
	.set sub_080f8bcc, 0x080f8bcc
	.set sub_080f8f9c, 0x080f8f9c
	.set sub_080f9170, 0x080f9170
	.set sub_080f9224, 0x080f9224
	.set sub_080f92dc, 0x080f92dc
	.set sub_080f9374, 0x080f9374
	.set sub_080fa3d4, 0x080fa3d4
	.set sub_080fa478, 0x080fa478
	.set sub_080facd8, 0x080facd8
	.set sub_080fe580, 0x080fe580
	.set sub_080fee04, 0x080fee04
	.set sub_08104c00, 0x08104c00
	.set sub_08104d5c, 0x08104d5c
	.set sub_08104ef8, 0x08104ef8
	.set sub_081c0010, 0x081c0010
	.global Func_080fcf14
	.thumb_func
Func_080fcf14:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r2, #182
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	movs	r0, #252
	lsls	r0, r0, #6
	adds	r0, #255
	ands	r0, r3
	bl	sub_080ad078
	adds	r5, r0, #0
	ldrb	r0, [r5, #6]
	bl	sub_080c8508
	cmp	r0, #0
	beq.n	.L_080fcf42
	movs	r0, #0
	b.n	.L_080fcf5a
.L_080fcf42:
	ldrb	r3, [r5, #8]
	movs	r0, #2
	cmp	r3, #255
	beq.n	.L_080fcf5a
	ldrb	r3, [r5, #0]
	movs	r2, #2
	eors	r3, r2
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	movs	r3, #1
	subs	r0, r3, r0
.L_080fcf5a:
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	adds	r4, r0, #0
	lsls	r0, r0, #2
	ldr	r7, [r3, #0]
	mov	r8, r0
	mov	r3, r8
	adds	r3, #20
	ldr	r0, [r7, r3]
	movs	r5, #0
	movs	r3, #1
	strb	r3, [r0, #5]
	strh	r5, [r0, #12]
	movs	r0, #134
	lsls	r0, r0, #2
	adds	r3, r7, r0
	ldr	r2, [r3, #0]
	subs	r0, #3
	movs	r3, #13
	strb	r3, [r2, #5]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	adds	r4, #28
	adds	r2, r7, #2
	ldrsb	r1, [r7, r4]
	strb	r3, [r2, r4]
	movs	r2, #1
	negs	r2, r2
	cmp	r1, r2
	bne.n	.L_080fcfac
	ldr	r3, [pc, #4]
	movs	r6, #0
	strb	r3, [r7, r4]
	b.n	.L_080fcfba
	.2byte 0x0000
	.2byte 0x0000
.L_080fcfac:
	lsls	r6, r1, #1
	adds	r0, r6, r1
	lsls	r0, r0, #3
	subs	r0, #10
	movs	r1, #16
	bl	sub_080f8ab4
.L_080fcfba:
	movs	r5, #129
	lsls	r5, r5, #2
	adds	r3, r6, r5
	ldrh	r0, [r7, r3]
	bl	sub_08016ca4
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r6, r7, r3
	adds	r1, r6, #0
	movs	r2, #2
	bl	.L_080fd6f0
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r7, r2
	adds	r5, r7, r5
	adds	r1, r6, #0
	strb	r0, [r3, #0]
	adds	r0, r5, #0
	bl	.L_080fd000
	mov	r3, r8
	adds	r3, #20
	adds	r5, r0, #0
	ldr	r0, [r7, r3]
	bl	sub_080f8888
	movs	r0, #1
	bl	sub_08013560
	adds	r0, r5, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
.L_080fd000:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	sub	sp, #24
	movs	r3, #28
	ldrsb	r3, [r7, r3]
	movs	r2, #0
	str	r3, [sp, #20]
	mov	r8, r0
	movs	r1, #30
	ldrsb	r1, [r7, r1]
	str	r2, [sp, #12]
	str	r1, [sp, #16]
	str	r2, [sp, #8]
	add	r2, sp, #12
	ldrb	r2, [r2, #0]
	movs	r1, #155
	lsls	r1, r1, #2
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	ldr	r3, [sp, #20]
	adds	r5, r7, #0
	lsls	r3, r3, #1
	ldrh	r0, [r3, r0]
	bl	sub_08016ca4
	adds	r5, #36
	movs	r3, #10
	movs	r6, #2
	mov	fp, r0
	str	r3, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #13
	movs	r2, #3
	movs	r3, #17
	str	r6, [sp, #4]
	bl	sub_080f811c
	cmp	r0, #0
	beq.n	.L_080fd068
	ldr	r1, [r5, #0]
	adds	r0, r7, #0
	bl	sub_080fa3d4
.L_080fd068:
	adds	r5, r7, #0
	movs	r3, #4
	adds	r5, #44
	str	r3, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #13
	movs	r2, #13
	movs	r3, #17
	str	r6, [sp, #4]
	bl	sub_080f811c
	cmp	r0, #0
	beq.n	.L_080fd09e
	ldr	r3, [sp, #12]
	ldr	r2, [r5, #0]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r0, #2
	movs	r3, #0
	bl	sub_080380d0
	movs	r1, #134
	lsls	r1, r1, #2
	adds	r3, r7, r1
	str	r0, [r3, #0]
	movs	r3, #13
	strb	r3, [r0, #5]
.L_080fd09e:
	movs	r2, #155
	ldr	r3, [pc, #492]
	lsls	r2, r2, #2
	adds	r2, r2, r7
	mov	sl, r2
	mov	r9, r3
	b.n	.L_080fd240
.L_080fd0ac:
	cmp	r6, #0
	beq.n	.L_080fd150
	ldr	r3, [sp, #20]
	mov	r1, r8
	lsls	r3, r3, #1
	ldrh	r0, [r3, r1]
	ldr	r5, [r7, #40]
	bl	sub_08016ca4
	ldr	r3, [sp, #20]
	mov	r2, r8
	lsls	r3, r3, #1
	mov	fp, r0
	ldrh	r0, [r3, r2]
	bl	.L_080fd294
	ldr	r3, [sp, #20]
	mov	r2, r8
	lsls	r3, r3, #1
	ldrh	r1, [r3, r2]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_080f8170
	ldr	r3, [sp, #20]
	mov	r2, r8
	lsls	r3, r3, #1
	ldrh	r1, [r3, r2]
	ldr	r0, [r7, #44]
	bl	.L_080fd55c
	ldr	r3, [sp, #20]
	mov	r2, r8
	lsls	r3, r3, #1
	ldrh	r1, [r3, r2]
	adds	r0, r7, #0
	bl	sub_080f88c4
	cmp	r6, #2
	bne.n	.L_080fd114
	ldr	r0, [sp, #20]
	cmp	r0, #0
	bge.n	.L_080fd106
	adds	r0, #3
.L_080fd106:
	asrs	r0, r0, #2
	lsls	r0, r0, #2
	bl	sub_08104ef8
	movs	r0, #1
	bl	sub_08013560
.L_080fd114:
	ldr	r0, [r7, #16]
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #16]
	bl	sub_08104d5c
	movs	r0, #82
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080fd148
	ldr	r3, [sp, #8]
	cmp	r3, #0
	bne.n	.L_080fd148
	ldr	r0, [r7, #48]
	bl	sub_08038268
	ldr	r0, [r7, #48]
	bl	sub_08038260
	ldr	r0, [r7, #48]
	bl	sub_080f9374
	movs	r1, #1
	str	r1, [sp, #8]
	b.n	.L_080fd150
.L_080fd148:
	movs	r0, #82
	adds	r0, #255
	bl	sub_08016d18
.L_080fd150:
	ldr	r2, [sp, #20]
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080fd15a
	adds	r3, r2, #3
.L_080fd15a:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #3
	movs	r1, #16
	subs	r0, #10
	bl	sub_080f8a44
	movs	r0, #1
	bl	sub_08013560
	mov	r1, r9
	ldr	r3, [r1, #4]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fd1a6
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080fd19e
	movs	r0, #112
	bl	sub_081c0010
	ldr	r2, [sp, #20]
	mov	r1, r8
	lsls	r3, r2, #1
	ldrh	r3, [r3, r1]
	str	r3, [sp, #12]
	b.n	.L_080fd250
.L_080fd19e:
	movs	r0, #114
	bl	sub_081c0010
	ldr	r1, [pc, #232]
.L_080fd1a6:
	mov	r2, r9
	ldr	r3, [r2, #4]
	movs	r0, #128
	lsls	r0, r0, #2
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_080fd1c0
	ldr	r3, [r1, #4]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fd21a
.L_080fd1c0:
	ldr	r3, [sp, #20]
	mov	r2, r8
	lsls	r3, r3, #1
	ldrh	r3, [r3, r2]
	str	r3, [sp, #12]
	ldr	r3, [r1, #4]
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080fd1da
	movs	r3, #1
	mov	r1, sl
	strb	r3, [r1, #0]
	b.n	.L_080fd1e0
.L_080fd1da:
	movs	r3, #2
	mov	r2, sl
	strb	r3, [r2, #0]
.L_080fd1e0:
	movs	r0, #64
	bl	sub_08014d78
	adds	r6, r0, #0
	adds	r1, r6, #0
	movs	r2, #1
	mov	r0, fp
	bl	.L_080fd6f0
	adds	r5, r0, #0
	lsls	r5, r5, #24
	lsrs	r5, r5, #24
	lsls	r5, r5, #24
	adds	r0, r6, #0
	asrs	r5, r5, #24
	bl	sub_08013164
	cmp	r5, #0
	bne.n	.L_080fd212
	mov	r3, sl
	strb	r5, [r3, #0]
	movs	r0, #114
	bl	sub_081c0010
	b.n	.L_080fd21a
.L_080fd212:
	movs	r0, #112
	bl	sub_081c0010
	b.n	.L_080fd24e
.L_080fd21a:
	mov	r1, r9
	ldr	r3, [r1, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fd234
	movs	r0, #113
	bl	sub_081c0010
	movs	r2, #1
	negs	r2, r2
	str	r2, [sp, #12]
	b.n	.L_080fd24e
.L_080fd234:
	add	r0, sp, #20
	ldr	r1, [sp, #16]
	movs	r2, #4
	bl	sub_08104c00
	adds	r6, r0, #0
.L_080fd240:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080fd24e
	b.n	.L_080fd0ac
.L_080fd24e:
	ldr	r2, [sp, #20]
.L_080fd250:
	strb	r2, [r7, #28]
	ldr	r3, [sp, #20]
	mov	r1, r8
	lsls	r3, r3, #1
	ldrh	r3, [r3, r1]
	str	r3, [r7, #8]
	ldr	r3, [sp, #20]
	lsls	r3, r3, #1
	ldrh	r2, [r3, r1]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #22
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r3, [r3, #0]
	movs	r2, #13
	subs	r1, #154
	strb	r2, [r3, #5]
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	strb	r2, [r3, #5]
	ldr	r0, [sp, #12]
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x1150
	.2byte 0x0300
.L_080fd294:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	bl	sub_08016ca4
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r6, r7, r3
	adds	r1, r6, #0
	movs	r2, #2
	bl	.L_080fd6f0
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r7, r3
	strb	r0, [r5, #0]
	ldr	r0, [r7, #36]
	bl	sub_08038260
	movs	r0, #108
	movs	r1, #32
	movs	r2, #8
	bl	sub_080f8bcc
	adds	r0, r6, #0
	bl	.L_080fd6b0
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_080fd2e0
	ldr	r0, [pc, #12]
	ldr	r1, [r7, #36]
	movs	r2, #0
	movs	r3, #24
	bl	sub_08038080
.L_080fd2e0:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x1021
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	sub	sp, #4
	movs	r3, #29
	ldrsb	r3, [r6, r3]
	movs	r1, #139
	str	r3, [sp, #0]
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	subs	r1, #17
	mov	r8, r3
	movs	r3, #28
	ldrsb	r3, [r6, r3]
	movs	r2, #0
	lsls	r3, r3, #1
	adds	r3, r3, r1
	mov	fp, r0
	ldrh	r0, [r6, r3]
	mov	r9, r2
	mov	sl, r2
	bl	sub_08016ca4
	ldr	r3, [sp, #0]
	movs	r1, #16
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #3
	subs	r0, #10
	movs	r7, #2
	bl	sub_080f8ab4
	b.n	.L_080fd49a
.L_080fd33e:
	cmp	r7, #0
	bne.n	.L_080fd344
	b.n	.L_080fd430
.L_080fd344:
	ldr	r3, [sp, #0]
	movs	r2, #129
	lsls	r2, r2, #2
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r0, [r6, r3]
	bl	sub_08016ca4
	ldr	r4, [sp, #0]
	ldr	r5, [r6, #24]
	adds	r3, r4, #0
	cmp	r4, #0
	bge.n	.L_080fd360
	adds	r3, r4, #3
.L_080fd360:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r4, r3
	lsls	r1, r3, #1
	adds	r1, r1, r3
	ldr	r3, [r6, #16]
	ldrh	r2, [r3, #12]
	ldr	r3, [pc, #32]
	adds	r2, r2, r1
	lsls	r2, r2, #3
	subs	r2, #2
	strh	r2, [r5, #6]
	ands	r2, r3
	ldr	r3, [pc, #24]
	ldr	r1, [pc, #24]
	ands	r2, r3
	ldrh	r3, [r5, #22]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #22]
	mov	r3, fp
	cmp	r3, #0
	bne.n	.L_080fd430
	b.n	.L_080fd39c
	.4byte 0x0000ffff
	.4byte 0x000001ff
	.2byte 0xfe00
	.2byte 0xffff
.L_080fd39c:
	movs	r1, #129
	lsls	r3, r4, #1
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldrh	r1, [r6, r3]
	ldr	r0, [r6, #40]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080f8170
	ldr	r3, [sp, #0]
	movs	r2, #129
	lsls	r3, r3, #1
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldrh	r1, [r6, r3]
	adds	r0, r6, #0
	bl	sub_080f88c4
	cmp	r7, #2
	bne.n	.L_080fd3dc
	ldr	r0, [sp, #0]
	cmp	r0, #0
	bge.n	.L_080fd3ce
	adds	r0, #3
.L_080fd3ce:
	asrs	r0, r0, #2
	lsls	r0, r0, #2
	bl	sub_08104ef8
	movs	r0, #1
	bl	sub_08013560
.L_080fd3dc:
	ldr	r0, [r6, #16]
	ldr	r1, [sp, #0]
	mov	r2, r8
	bl	sub_08104d5c
	movs	r0, #82
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080fd428
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_080fd428
	ldr	r0, [r6, #48]
	bl	sub_08038260
	movs	r1, #182
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #24]
	movs	r2, #0
	ands	r0, r3
	ldr	r3, [pc, #20]
	ldr	r1, [r6, #48]
	adds	r0, r0, r3
	movs	r3, #0
	bl	sub_08038080
	movs	r2, #1
	mov	sl, r2
	b.n	.L_080fd430
	movs	r0, r0
	.4byte 0x00003fff
	.2byte 0x0885
	.2byte 0x0000
.L_080fd428:
	movs	r0, #82
	adds	r0, #255
	bl	sub_08016d18
.L_080fd430:
	ldr	r4, [sp, #0]
	adds	r3, r4, #0
	cmp	r4, #0
	bge.n	.L_080fd43a
	adds	r3, r4, #3
.L_080fd43a:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r4, r3
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #3
	movs	r1, #16
	subs	r0, #10
	bl	sub_080f8a44
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [pc, #208]
	movs	r2, #1
	ldr	r3, [r1, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fd476
	movs	r0, #112
	bl	sub_081c0010
	ldr	r3, [sp, #0]
	movs	r1, #129
	lsls	r3, r3, #1
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldrh	r3, [r6, r3]
	mov	r9, r3
	b.n	.L_080fd4a8
.L_080fd476:
	ldr	r3, [r1, #4]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fd48e
	movs	r0, #113
	bl	sub_081c0010
	movs	r2, #1
	negs	r2, r2
	mov	r9, r2
	b.n	.L_080fd4a8
.L_080fd48e:
	mov	r0, sp
	mov	r1, r8
	movs	r2, #4
	bl	sub_08104c00
	adds	r7, r0, #0
.L_080fd49a:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080fd4a8
	b.n	.L_080fd33e
.L_080fd4a8:
	ldr	r5, [r6, #24]
	movs	r7, #13
	adds	r0, r5, #0
	bl	sub_080f8888
	strb	r7, [r5, #5]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [sp, #0]
	movs	r2, #129
	strb	r3, [r6, #29]
	ldr	r3, [sp, #0]
	lsls	r2, r2, #2
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r3, [r6, r3]
	movs	r1, #140
	str	r3, [r6, #8]
	ldr	r3, [sp, #0]
	lsls	r1, r1, #1
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r2, [r6, r3]
	adds	r1, #255
	adds	r3, r6, r1
	strb	r2, [r3, #0]
	movs	r3, #28
	ldrsb	r3, [r6, r3]
	str	r3, [sp, #0]
	movs	r2, #30
	ldrsb	r2, [r6, r2]
	mov	r8, r2
	cmp	r3, #0
	bge.n	.L_080fd4f0
	adds	r3, #3
.L_080fd4f0:
	asrs	r0, r3, #2
	lsls	r0, r0, #2
	bl	sub_08104ef8
	ldr	r0, [r6, #16]
	ldr	r1, [sp, #0]
	mov	r2, r8
	bl	sub_08104d5c
	movs	r1, #188
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldr	r3, [r3, #0]
	movs	r2, #190
	lsls	r2, r2, #1
	strb	r7, [r3, #5]
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	mov	r0, r9
	strb	r7, [r3, #5]
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x1150
	.2byte 0x0300
.L_080fd52c:
	push	{lr}
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	lsls	r0, r0, #10
	ands	r3, r1
	orrs	r0, r3
	cmp	r2, #0
	bne.n	.L_080fd546
	ldr	r3, [pc, #24]
	movs	r2, #144
	lsls	r2, r2, #2
	b.n	.L_080fd54e
.L_080fd546:
	ldr	r3, [pc, #16]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #66
.L_080fd54e:
	adds	r3, r3, r2
	strh	r0, [r3, #0]
	movs	r0, #1
	pop	{pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
.L_080fd55c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #292]
	movs	r1, #144
	lsls	r1, r1, #2
	adds	r3, r2, r1
	ldrh	r3, [r3, #0]
	sub	sp, #20
	adds	r5, r0, #0
	cmp	r3, #0
	beq.n	.L_080fd596
	adds	r1, #2
	adds	r3, r2, r1
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080fd596
	movs	r3, #8
	ldr	r0, [pc, #264]
	negs	r3, r3
	adds	r1, r5, #0
	movs	r2, #0
	bl	sub_08038080
	b.n	.L_080fd5a4
.L_080fd596:
	movs	r3, #8
	ldr	r0, [pc, #252]
	negs	r3, r3
	adds	r1, r5, #0
	movs	r2, #0
	bl	sub_08038080
.L_080fd5a4:
	ldr	r3, [pc, #232]
	movs	r2, #144
	lsls	r2, r2, #2
	adds	r6, r3, r2
	movs	r7, #192
	ldrh	r2, [r6, #0]
	lsls	r7, r7, #2
	adds	r7, #255
	adds	r3, r7, #0
	ands	r3, r2
	ldr	r2, [pc, #224]
	movs	r1, #12
	adds	r0, r3, r2
	add	r3, sp, #16
	movs	r2, #8
	add	r1, sp
	add	r2, sp
	mov	sl, r3
	add	r3, sp, #4
	mov	r9, r1
	mov	fp, r2
	str	r3, [sp, #0]
	mov	r2, r9
	mov	r8, r3
	mov	r1, sl
	mov	r3, fp
	bl	sub_08038108
	ldrh	r2, [r6, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080fd60e
	adds	r0, r7, #0
	ands	r0, r2
	movs	r1, #4
	bl	sub_08038120
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #168]
	bl	sub_08038080
	ldrh	r0, [r6, #0]
	lsrs	r0, r0, #10
	bl	sub_08016ca4
	adds	r1, r5, #0
	movs	r2, #80
	movs	r3, #0
	bl	sub_08038090
	b.n	.L_080fd61a
.L_080fd60e:
	ldr	r0, [pc, #148]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
.L_080fd61a:
	ldr	r3, [pc, #116]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #66
	adds	r6, r3, r1
	movs	r7, #192
	ldrh	r2, [r6, #0]
	lsls	r7, r7, #2
	adds	r7, #255
	adds	r3, r7, #0
	ands	r3, r2
	ldr	r2, [pc, #104]
	mov	r1, sl
	adds	r0, r3, r2
	mov	r2, r8
	str	r2, [sp, #0]
	mov	r3, fp
	mov	r2, r9
	bl	sub_08038108
	ldrh	r2, [r6, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080fd674
	adds	r0, r7, #0
	ands	r0, r2
	movs	r1, #4
	bl	sub_08038120
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #8
	ldr	r0, [pc, #76]
	bl	sub_08038080
	ldrh	r0, [r6, #0]
	lsrs	r0, r0, #10
	bl	sub_08016ca4
	adds	r1, r5, #0
	movs	r2, #80
	movs	r3, #8
	bl	sub_08038090
	b.n	.L_080fd680
.L_080fd674:
	ldr	r0, [pc, #52]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #8
	bl	sub_08038080
.L_080fd680:
	movs	r0, #1
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x00001013
	.4byte 0x0000100f
	.4byte 0x000005a7
	.4byte 0x00001016
	.4byte 0x00001014
	.4byte 0x00001017
	.2byte 0x1015
	.2byte 0x0000
.L_080fd6b0:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	r8, r0
	adds	r6, r3, #0
	adds	r6, #76
	mov	r5, r8
	movs	r7, #31
.L_080fd6c8:
	ldrh	r1, [r5, #0]
	adds	r5, #2
	cmp	r1, #0
	beq.n	.L_080fd6dc
	ldr	r3, [r6, #0]
	movs	r0, #4
	ldrb	r2, [r3, #14]
	movs	r3, #0
	bl	sub_08038288
.L_080fd6dc:
	subs	r7, #1
	adds	r6, #4
	cmp	r7, #0
	bge.n	.L_080fd6c8
	mov	r0, r8
	bl	sub_080facd8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
.L_080fd6f0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r1
	mov	sl, r2
	ldr	r2, [pc, #48]
	mov	r3, r8
	sub	sp, #8
	adds	r4, r0, #0
	movs	r1, #0
	adds	r3, #62
	mov	ip, r8
.L_080fd70c:
	strh	r2, [r3, #0]
	subs	r3, #2
	cmp	r3, ip
	bge.n	.L_080fd70c
	mov	r2, sl
	cmp	r2, #1
	bne.n	.L_080fd764
	lsls	r3, r1, #1
	mov	r2, r8
	adds	r5, r3, r2
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	movs	r7, #0
	movs	r6, #88
	mov	r8, r3
	b.n	.L_080fd734
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_080fd734:
	ldrh	r2, [r6, r4]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080fd75a
	mov	r0, r8
	ands	r0, r2
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	bl	sub_080ad078
	ldrb	r3, [r0, #6]
	ldr	r1, [sp, #4]
	ldr	r4, [sp, #0]
	cmp	r3, #0
	beq.n	.L_080fd75a
	ldrh	r3, [r4, r6]
	adds	r1, #1
	strh	r3, [r5, #0]
	adds	r5, #2
.L_080fd75a:
	adds	r7, #1
	adds	r6, #4
	cmp	r7, #31
	ble.n	.L_080fd734
	b.n	.L_080fd7fe
.L_080fd764:
	lsls	r3, r1, #1
	mov	r2, r8
	adds	r5, r3, r2
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	movs	r6, #88
	mov	r9, r3
	movs	r7, #31
.L_080fd776:
	ldrh	r2, [r6, r4]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080fd7a6
	mov	r0, r9
	ands	r0, r2
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	bl	sub_080ad078
	ldrb	r3, [r0, #6]
	ldr	r1, [sp, #4]
	ldr	r4, [sp, #0]
	cmp	r3, #0
	bne.n	.L_080fd79e
	ldrb	r2, [r0, #1]
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fd7a6
.L_080fd79e:
	ldrh	r3, [r6, r4]
	adds	r1, #1
	strh	r3, [r5, #0]
	adds	r5, #2
.L_080fd7a6:
	subs	r7, #1
	adds	r6, #4
	cmp	r7, #0
	bge.n	.L_080fd776
	mov	r2, sl
	cmp	r2, #2
	beq.n	.L_080fd7fe
	lsls	r3, r1, #1
	mov	r2, r8
	adds	r5, r3, r2
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	movs	r6, #88
	mov	r8, r3
	movs	r7, #31
.L_080fd7c6:
	ldrh	r2, [r6, r4]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080fd7f6
	mov	r0, r8
	ands	r0, r2
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	bl	sub_080ad078
	ldrb	r3, [r0, #6]
	ldr	r1, [sp, #4]
	ldr	r4, [sp, #0]
	cmp	r3, #0
	bne.n	.L_080fd7f6
	ldrb	r2, [r0, #1]
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080fd7f6
	ldrh	r3, [r6, r4]
	adds	r1, #1
	strh	r3, [r5, #0]
	adds	r5, #2
.L_080fd7f6:
	subs	r7, #1
	adds	r6, #4
	cmp	r7, #0
	bge.n	.L_080fd7c6
.L_080fd7fe:
	adds	r0, r1, #0
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
.L_080fd80c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	movs	r2, #133
	lsls	r2, r2, #2
	mov	sl, r2
	adds	r5, r1, #0
	adds	r3, r6, #2
	add	r5, sl
	mov	r9, r0
	ldrb	r0, [r3, r5]
	mov	r8, r3
	bl	sub_08016ca4
	mov	r2, sl
	ldrb	r7, [r6, r2]
	mov	r2, r8
	ldrb	r3, [r2, r5]
	movs	r2, #153
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldrsb	r6, [r6, r3]
	mov	fp, r0
	adds	r3, r6, #1
	cmp	r3, r7
	ble.n	.L_080fd852
	subs	r6, r7, #1
.L_080fd852:
	movs	r1, #5
	adds	r0, r6, #0
	bl	sub_08002054
	movs	r1, #5
	mov	sl, r0
	adds	r0, r6, #0
	bl	sub_08002064
	movs	r1, #5
	mov	r8, r0
	adds	r0, r7, #0
	bl	sub_08002054
	movs	r1, #5
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	sub_08002064
	cmp	r0, #0
	beq.n	.L_080fd87e
	adds	r5, #1
.L_080fd87e:
	mov	r2, r9
	mov	r3, fp
	str	r3, [r2, #0]
	mov	r3, sl
	str	r3, [r2, #8]
	mov	r3, r8
	str	r5, [r2, #12]
	str	r3, [r2, #16]
	str	r7, [r2, #20]
	str	r6, [r2, #24]
	movs	r0, #1
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
.L_080fd8a0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	r8, r2
	movs	r3, #192
	ldr	r2, [r2, #8]
	lsls	r3, r3, #18
	adds	r3, #220
	mov	r1, r8
	ldr	r7, [r3, #0]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [r1, #16]
	movs	r0, #82
	adds	r3, r3, r2
	str	r3, [r1, #24]
	adds	r0, #255
	sub	sp, #8
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080fd906
	ldr	r0, [r7, #48]
	bl	sub_08038260
	movs	r0, #1
	bl	sub_08013560
	mov	r2, r8
	ldr	r3, [r2, #24]
	movs	r1, #226
	lsls	r3, r3, #1
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldrh	r2, [r7, r3]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080fd910
	movs	r0, #128
	ldr	r3, [pc, #112]
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r2
	adds	r0, r0, r3
	ldr	r1, [r7, #48]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	b.n	.L_080fd910
.L_080fd906:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #255
	bl	sub_08016d18
.L_080fd910:
	movs	r2, #1
	movs	r6, #0
	mov	sl, r2
	movs	r5, #1
.L_080fd918:
	mov	r1, r8
	ldr	r3, [r1, #16]
	cmp	r6, r3
	bne.n	.L_080fd934
	mov	r2, sl
	ldr	r0, [r7, #36]
	movs	r3, #14
	str	r2, [sp, #0]
	movs	r1, #1
	adds	r2, r5, #0
	str	r3, [sp, #4]
	bl	sub_080f9224
	b.n	.L_080fd948
.L_080fd934:
	mov	r3, sl
	ldr	r0, [r7, #36]
	str	r3, [sp, #0]
	movs	r3, #15
	str	r3, [sp, #4]
	movs	r1, #1
	adds	r2, r5, #0
	movs	r3, #14
	bl	sub_080f9224
.L_080fd948:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, #4
	ble.n	.L_080fd918
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #1
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0885
	.2byte 0x0000
.L_080fd968:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	sub	sp, #4
	mov	sl, r3
	mov	r8, r0
	adds	r6, r2, #0
	bl	sub_08038260
	movs	r3, #11
	str	r3, [sp, #0]
	movs	r2, #11
	movs	r3, #16
	mov	r0, r8
	movs	r1, #0
	bl	sub_08038070
	movs	r3, #135
	lsls	r3, r3, #2
	add	r3, sl
	ldrh	r2, [r3, #0]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fd9b8
	ldr	r0, [pc, #280]
	mov	r1, r8
	movs	r2, #0
	movs	r3, #88
	bl	sub_08038080
	b.n	.L_080fd9c4
.L_080fd9b8:
	ldr	r0, [pc, #268]
	mov	r1, r8
	movs	r2, #0
	movs	r3, #88
	bl	sub_08038080
.L_080fd9c4:
	ldr	r2, [r6, #8]
	lsls	r3, r2, #2
	adds	r5, r3, r2
	ldr	r3, [r6, #20]
	subs	r3, r3, r5
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	fp, r3
	cmp	r3, #5
	bls.n	.L_080fd9dc
	movs	r1, #5
	mov	fp, r1
.L_080fd9dc:
	movs	r3, #34
	str	r3, [sp, #0]
	movs	r0, #5
	adds	r1, r5, #0
	mov	r2, r8
	movs	r3, #112
	bl	sub_080f92dc
	movs	r2, #15
	ldr	r1, [r6, #20]
	ldr	r3, [r6, #8]
	mov	r0, r8
	str	r2, [sp, #0]
	movs	r2, #5
	bl	sub_080f9170
	movs	r2, #96
	movs	r3, #0
	ldr	r0, [pc, #200]
	mov	r1, r8
	bl	sub_08038080
	movs	r2, #0
	mov	r3, fp
	mov	r9, r2
	cmp	r3, #0
	bls.n	.L_080fdab4
	movs	r1, #226
	lsls	r3, r5, #1
	lsls	r1, r1, #1
	adds	r6, r3, r1
.L_080fda1a:
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, sl
	ldrb	r0, [r3, #0]
	bl	sub_08016ca4
	mov	r2, sl
	ldrh	r3, [r6, r2]
	adds	r5, r0, #0
	movs	r0, #252
	lsls	r0, r0, #6
	adds	r0, #255
	ands	r0, r3
	bl	sub_080ad078
	adds	r7, r0, #0
	ldrb	r2, [r7, #9]
	movs	r1, #58
	ldrsh	r3, [r5, r1]
	cmp	r2, r3
	ble.n	.L_080fda4e
	movs	r0, #2
	bl	sub_080380b8
	b.n	.L_080fda70
.L_080fda4e:
	mov	r2, sl
	ldrh	r3, [r6, r2]
	movs	r0, #252
	lsls	r0, r0, #6
	adds	r0, #255
	ands	r0, r3
	bl	.L_080fe164
	cmp	r0, #0
	beq.n	.L_080fda6a
	movs	r0, #4
	bl	sub_080380b8
	b.n	.L_080fda70
.L_080fda6a:
	movs	r0, #15
	bl	sub_080380b8
.L_080fda70:
	mov	r1, sl
	ldrh	r3, [r6, r1]
	movs	r0, #252
	lsls	r0, r0, #6
	adds	r0, #255
	mov	r2, r9
	ands	r0, r3
	ldr	r3, [pc, #80]
	lsls	r5, r2, #4
	adds	r5, #8
	adds	r0, r0, r3
	mov	r1, r8
	movs	r2, #16
	adds	r3, r5, #0
	bl	sub_08038080
	ldrb	r0, [r7, #9]
	movs	r3, #104
	movs	r1, #2
	mov	r2, r8
	str	r5, [sp, #0]
	bl	sub_080380a8
	movs	r0, #15
	bl	sub_080380b8
	mov	r3, r9
	adds	r3, #1
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	r9, r3
	adds	r6, #2
	cmp	fp, r9
	bhi.n	.L_080fda1a
.L_080fdab4:
	movs	r0, #1
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00001010
	.4byte 0x000010ba
	.4byte 0x0000101c
	.2byte 0x05a7
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #104
	str	r0, [sp, #52]
	ldr	r1, [sp, #52]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	lsls	r1, r1, #2
	movs	r0, #0
	adds	r3, r1, #0
	str	r0, [sp, #44]
	str	r0, [sp, #32]
	str	r0, [sp, #28]
	str	r1, [sp, #24]
	adds	r3, #20
	ldr	r2, [r7, r3]
	movs	r3, #13
	strb	r3, [r2, #5]
	adds	r5, r7, #0
	movs	r3, #14
	str	r3, [sp, #0]
	adds	r5, #56
	movs	r3, #2
	str	r3, [sp, #4]
	adds	r0, r5, #0
	movs	r1, #13
	movs	r2, #3
	movs	r3, #17
	bl	sub_080f811c
	ldr	r5, [r5, #0]
	movs	r2, #0
	mov	r8, r5
	str	r2, [sp, #36]
	adds	r3, r7, #2
	ldr	r0, [sp, #52]
	str	r3, [sp, #16]
	lsls	r0, r0, #1
	add	r4, sp, #56
	str	r0, [sp, #12]
	b.n	.L_080fe0d8
.L_080fdb34:
	ldr	r1, [sp, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldr	r1, [sp, #16]
	str	r4, [sp, #8]
	ldrb	r0, [r1, r3]
	bl	sub_08016ca4
	movs	r2, #155
	str	r0, [sp, #48]
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	ldr	r4, [sp, #8]
	cmp	r3, #0
	beq.n	.L_080fdbc8
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r1, r7, r3
	movs	r2, #1
	b.n	.L_080fdbd4
.L_080fdb60:
	movs	r0, #130
	b.n	.L_080fde92
.L_080fdb64:
	movs	r0, #113
	str	r4, [sp, #8]
	bl	sub_081c0010
	movs	r0, #1
	negs	r0, r0
	movs	r1, #1
	str	r0, [sp, #44]
	str	r1, [sp, #36]
	ldr	r4, [sp, #8]
	b.n	.L_080fe0d8
.L_080fdb7a:
	movs	r0, #130
	str	r4, [sp, #8]
	bl	sub_081c0010
	ldr	r4, [sp, #8]
	movs	r2, #226
	ldr	r3, [r4, #24]
	lsls	r2, r2, #1
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r3, [r7, r3]
	str	r3, [sp, #44]
	movs	r3, #155
	lsls	r3, r3, #2
	adds	r2, r7, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	str	r3, [sp, #36]
	b.n	.L_080fe0d8
.L_080fdba0:
	movs	r0, #130
	str	r4, [sp, #8]
	bl	sub_081c0010
	ldr	r4, [sp, #8]
	movs	r0, #226
	ldr	r3, [r4, #24]
	lsls	r0, r0, #1
	lsls	r3, r3, #1
	adds	r3, r3, r0
	ldrh	r3, [r7, r3]
	movs	r1, #155
	lsls	r1, r1, #2
	str	r3, [sp, #44]
	adds	r2, r7, r1
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r2, #1
	str	r2, [sp, #36]
	b.n	.L_080fe0d8
.L_080fdbc8:
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r1, r7, r3
	ldr	r0, [sp, #48]
	movs	r2, #2
	str	r4, [sp, #8]
.L_080fdbd4:
	bl	.L_080fd6f0
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r7, r1
	strb	r0, [r3, #0]
	ldr	r4, [sp, #8]
	movs	r2, #226
	lsls	r2, r2, #1
	adds	r0, r7, r2
	str	r4, [sp, #8]
	bl	.L_080fd6b0
	ldr	r4, [sp, #8]
	ldr	r1, [sp, #52]
	adds	r0, r4, #0
	bl	.L_080fd80c
	movs	r3, #1
	str	r3, [sp, #40]
	mov	fp, r3
	ldr	r3, [sp, #24]
	ldr	r1, [pc, #380]
	adds	r3, #20
	ldr	r3, [r7, r3]
	movs	r2, #4
	mov	r0, fp
	mov	sl, r1
	mov	r9, r2
	strb	r0, [r3, #5]
	ldr	r4, [sp, #8]
	b.n	.L_080fe0c4
.L_080fdc14:
	ldr	r1, [r4, #16]
	movs	r0, #88
	lsls	r1, r1, #4
	adds	r1, #36
	str	r4, [sp, #8]
	bl	sub_080f8a44
	mov	r3, fp
	ldr	r4, [sp, #8]
	cmp	r3, #0
	beq.n	.L_080fdce4
	ldr	r1, [sp, #32]
	movs	r2, #226
	lsls	r3, r1, #1
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r7, r3]
	movs	r0, #0
	mov	fp, r0
	cmp	r3, #0
	beq.n	.L_080fdc4a
	lsls	r3, r1, #2
	adds	r3, #76
	ldr	r0, [r7, r3]
	bl	sub_080f8888
	ldr	r4, [sp, #8]
.L_080fdc4a:
	ldr	r3, [sp, #40]
	cmp	r3, #0
	beq.n	.L_080fdc6a
	movs	r0, #0
	str	r0, [sp, #40]
	movs	r0, #1
	str	r4, [sp, #8]
	bl	sub_08013560
	ldr	r4, [sp, #8]
	mov	r0, r8
	adds	r2, r4, #0
	movs	r1, #0
	bl	.L_080fd968
	ldr	r4, [sp, #8]
.L_080fdc6a:
	adds	r2, r4, #0
	add	r1, sp, #84
	mov	r0, r8
	str	r4, [sp, #8]
	bl	.L_080fd8a0
	ldr	r1, [sp, #12]
	ldr	r4, [sp, #8]
	movs	r3, #182
	lsls	r3, r3, #1
	adds	r2, r1, r3
	ldr	r3, [r4, #24]
	movs	r0, #226
	lsls	r0, r0, #1
	lsls	r3, r3, #1
	adds	r3, r3, r0
	ldrh	r3, [r7, r3]
	movs	r1, #134
	strh	r3, [r7, r2]
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldr	r2, [r3, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	ldr	r2, [r4, #24]
	lsls	r3, r2, #1
	adds	r3, r3, r0
	ldrh	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L_080fdcb6
	lsls	r3, r2, #2
	adds	r3, #76
	ldr	r0, [r7, r3]
	movs	r3, #9
	strb	r3, [r0, #5]
	movs	r3, #250
	strh	r6, [r0, #12]
	strb	r3, [r0, #15]
.L_080fdcb6:
	movs	r3, #139
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r7, r3
	ldrb	r3, [r2, #0]
	movs	r5, #0
	cmp	r6, r3
	bcs.n	.L_080fdce4
	adds	r6, r2, #0
.L_080fdcc8:
	lsls	r3, r5, #2
	adds	r3, #248
	ldr	r0, [r7, r3]
	movs	r1, #1
	str	r4, [sp, #8]
	bl	sub_08020030
	adds	r3, r5, #1
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
	ldrb	r3, [r6, #0]
	ldr	r4, [sp, #8]
	cmp	r5, r3
	bcc.n	.L_080fdcc8
.L_080fdce4:
	movs	r0, #1
	str	r4, [sp, #8]
	bl	sub_08013560
	ldr	r4, [sp, #8]
	mov	r1, sl
	ldr	r0, [r4, #24]
	mov	r2, r9
	str	r0, [sp, #32]
	ldr	r3, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080fdd14
	add	r3, sp, #64
	ldr	r1, [r4, #20]
	movs	r0, #0
	str	r3, [sp, #0]
	movs	r2, #5
	add	r3, sp, #72
	bl	sub_080f8f9c
	ldr	r1, [pc, #112]
	ldr	r4, [sp, #8]
	b.n	.L_080fdd18
.L_080fdd14:
	movs	r0, #1
	negs	r0, r0
.L_080fdd18:
	cmp	r0, #1
	bne.n	.L_080fdd22
	movs	r3, #1
	str	r3, [sp, #40]
	mov	fp, r3
.L_080fdd22:
	cmp	r0, #0
	bne.n	.L_080fdd2a
	movs	r2, #1
	mov	fp, r2
.L_080fdd2a:
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_080fdd36
	movs	r0, #0
	mov	fp, r0
.L_080fdd36:
	movs	r2, #155
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080fde20
	mov	r0, sl
	ldr	r3, [r0, #4]
	mov	r2, r9
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fddcc
	ldr	r3, [sp, #28]
	cmp	r3, #0
	bne.n	.L_080fddcc
	ldr	r3, [r4, #24]
	movs	r0, #226
	lsls	r0, r0, #1
	lsls	r3, r3, #1
	adds	r3, r3, r0
	ldrh	r3, [r7, r3]
	ldr	r0, [pc, #24]
	str	r4, [sp, #8]
	ands	r0, r3
	bl	sub_080ad078
	ldrb	r3, [r0, #6]
	ldr	r4, [sp, #8]
	cmp	r3, #0
	bne.n	.L_080fdd84
	movs	r0, #114
	bl	sub_081c0010
	b.n	.L_080fddba
	movs	r0, r0
	.4byte 0x00003fff
	.2byte 0x1150
	.2byte 0x0300
.L_080fdd84:
	movs	r0, #174
	str	r4, [sp, #8]
	bl	sub_081c0010
	movs	r1, #1
	str	r1, [sp, #28]
	movs	r2, #135
	lsls	r2, r2, #2
	adds	r1, r7, r2
	ldrh	r2, [r1, #0]
	ldr	r3, [pc, #36]
	mov	r0, r8
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r3, #96
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #88
	movs	r3, #120
	bl	sub_08038068
	mov	r1, r8
	ldr	r0, [pc, #16]
	movs	r2, #0
	movs	r3, #88
	bl	sub_08038080
.L_080fddba:
	ldr	r1, [pc, #12]
	ldr	r4, [sp, #8]
	b.n	.L_080fddcc
	.4byte 0x00000002
	.4byte 0x00001010
	.2byte 0x1150
	.2byte 0x0300
.L_080fddcc:
	mov	r0, sl
	ldr	r3, [r0, #0]
	mov	r2, r9
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080fde20
	ldr	r3, [sp, #28]
	cmp	r3, #1
	bne.n	.L_080fde20
	movs	r0, #0
	str	r0, [sp, #28]
	movs	r2, #135
	lsls	r2, r2, #2
	adds	r1, r7, r2
	ldrh	r2, [r1, #0]
	ldr	r3, [pc, #40]
	mov	r0, r8
	ands	r2, r3
	movs	r3, #96
	strh	r2, [r1, #0]
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #88
	movs	r3, #120
	str	r4, [sp, #8]
	bl	sub_08038068
	mov	r1, r8
	ldr	r0, [pc, #16]
	movs	r2, #0
	movs	r3, #88
	bl	sub_08038080
	ldr	r1, [pc, #12]
	ldr	r4, [sp, #8]
	b.n	.L_080fde20
	.4byte 0x0000fffd
	.4byte 0x000010ba
	.2byte 0x1150
	.2byte 0x0300
.L_080fde20:
	mov	r3, sl
	ldr	r2, [r3, #4]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080fdeae
	movs	r0, #155
	lsls	r0, r0, #2
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080fde3a
	b.n	.L_080fdb60
.L_080fde3a:
	ldr	r3, [r4, #24]
	subs	r0, #168
	lsls	r3, r3, #1
	adds	r2, r3, r0
	ldrh	r3, [r7, r2]
	cmp	r3, #0
	beq.n	.L_080fdeae
	adds	r0, r3, #0
	str	r4, [sp, #8]
	bl	.L_080fe164
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_080fde58
	b.n	.L_080fe088
.L_080fde58:
	ldr	r3, [r4, #24]
	movs	r1, #226
	lsls	r1, r1, #1
	lsls	r3, r3, #1
	adds	r3, r3, r1
	ldrh	r3, [r7, r3]
	ldr	r0, [pc, #32]
	str	r4, [sp, #8]
	ands	r0, r3
	bl	sub_080ad078
	ldr	r1, [sp, #48]
	ldrb	r2, [r0, #9]
	movs	r0, #58
	ldrsh	r3, [r1, r0]
	ldr	r4, [sp, #8]
	cmp	r2, r3
	ble.n	.L_080fde90
	movs	r0, #114
	bl	sub_081c0010
	ldr	r1, [pc, #8]
	ldr	r4, [sp, #8]
	b.n	.L_080fdeae
	.4byte 0x00003fff
	.2byte 0x1150
	.2byte 0x0300
.L_080fde90:
	movs	r0, #173
.L_080fde92:
	str	r4, [sp, #8]
	bl	sub_081c0010
	ldr	r4, [sp, #8]
	movs	r2, #226
	ldr	r3, [r4, #24]
	lsls	r2, r2, #1
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r3, [r7, r3]
	str	r3, [sp, #44]
	movs	r3, #1
	str	r3, [sp, #36]
	b.n	.L_080fe0d8
.L_080fdeae:
	mov	r0, sl
	ldr	r2, [r0, #4]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080fdebc
	b.n	.L_080fdb64
.L_080fdebc:
	ldr	r2, [r1, #12]
	adds	r3, #254
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080fded4
	ldr	r2, [r1, #12]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080fded4
	b.n	.L_080fdfdc
.L_080fded4:
	ldr	r3, [r1, #0]
	mov	r2, r9
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fdee0
	b.n	.L_080fdfdc
.L_080fdee0:
	ldr	r3, [sp, #52]
	movs	r0, #139
	adds	r3, #28
	str	r3, [sp, #20]
	lsls	r0, r0, #1
	adds	r0, #255
	ldrsb	r6, [r7, r3]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	movs	r1, #155
	lsls	r1, r1, #2
	mov	fp, r3
	adds	r3, r7, r1
	ldrb	r2, [r3, #0]
	movs	r0, #111
	negs	r3, r2
	orrs	r3, r2
	lsrs	r3, r3, #31
	mov	r9, r3
	mov	r2, r9
	movs	r3, #2
	subs	r2, r3, r2
	mov	r9, r2
	str	r4, [sp, #8]
	bl	sub_081c0010
	ldr	r0, [sp, #52]
	ldr	r2, [sp, #16]
	movs	r1, #133
	lsls	r1, r1, #2
	ldr	r4, [sp, #8]
	adds	r3, r0, r1
	ldrb	r3, [r2, r3]
	movs	r0, #153
	ldr	r2, [r4, #24]
	lsls	r0, r0, #2
	adds	r3, r3, r0
	strb	r2, [r7, r3]
.L_080fdf2c:
	mov	r1, sl
	ldr	r3, [r1, #12]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fdf3e
	adds	r6, #1
	b.n	.L_080fdf40
.L_080fdf3e:
	subs	r6, #1
.L_080fdf40:
	mov	r2, fp
	adds	r0, r6, r2
	mov	r1, fp
	str	r4, [sp, #8]
	bl	sub_08002064
	adds	r6, r0, #0
	movs	r0, #129
	lsls	r3, r6, #1
	lsls	r0, r0, #2
	adds	r5, r3, r0
	ldrh	r3, [r7, r5]
	movs	r1, #128
	str	r3, [r7, #8]
	ldrh	r2, [r7, r5]
	lsls	r1, r1, #2
	adds	r1, #22
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	ldrb	r0, [r3, #0]
	bl	sub_08016ca4
	movs	r2, #226
	lsls	r2, r2, #1
	adds	r1, r7, r2
	mov	r2, r9
	bl	.L_080fd6f0
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r7, r1
	strb	r0, [r3, #0]
	lsls	r0, r0, #24
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_080fdf2c
	ldr	r2, [sp, #20]
	adds	r0, r6, #0
	strb	r6, [r7, r2]
	cmp	r6, #0
	bge.n	.L_080fdf94
	adds	r0, r6, #3
.L_080fdf94:
	asrs	r0, r0, #2
	lsls	r0, r0, #2
	str	r4, [sp, #8]
	bl	sub_08104ef8
	ldr	r0, [r7, #16]
	adds	r1, r6, #0
	mov	r2, fp
	bl	sub_08104d5c
	movs	r0, #188
	lsls	r0, r0, #1
	adds	r3, r7, r0
	ldr	r3, [r3, #0]
	movs	r1, #190
	movs	r2, #13
	lsls	r1, r1, #1
	strb	r2, [r3, #5]
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	movs	r0, #1
	strb	r2, [r3, #5]
	bl	sub_08013560
	ldr	r0, [r7, #40]
	ldrh	r1, [r7, r5]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080f8170
	ldrh	r1, [r7, r5]
	adds	r0, r7, #0
	bl	sub_080f88c4
	ldr	r4, [sp, #8]
	b.n	.L_080fe0d8
.L_080fdfdc:
	mov	r3, sl
	ldr	r2, [r3, #4]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080fe052
	ldr	r3, [r1, #0]
	mov	r0, r9
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080fe052
	ldr	r3, [r4, #24]
	movs	r1, #226
	lsls	r1, r1, #1
	lsls	r3, r3, #1
	adds	r3, r3, r1
	ldrh	r3, [r7, r3]
	ldr	r0, [pc, #28]
	str	r4, [sp, #8]
	ands	r0, r3
	bl	sub_080ad078
	ldrb	r3, [r0, #6]
	ldr	r4, [sp, #8]
	cmp	r3, #0
	bne.n	.L_080fe028
	movs	r0, #114
	bl	sub_081c0010
	ldr	r1, [pc, #8]
	ldr	r4, [sp, #8]
	b.n	.L_080fe052
	movs	r0, r0
	.4byte 0x00003fff
	.2byte 0x1150
	.2byte 0x0300
.L_080fe028:
	ldr	r2, [sp, #52]
	ldr	r1, [sp, #16]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r2, r0
	ldrb	r0, [r1, r3]
	ldr	r3, [r4, #24]
	movs	r2, #226
	lsls	r2, r2, #1
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r1, [r7, r3]
	movs	r2, #0
	str	r4, [sp, #8]
	bl	.L_080fd52c
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_080fe050
	b.n	.L_080fdb7a
.L_080fe050:
	ldr	r1, [pc, #68]
.L_080fe052:
	mov	r3, sl
	ldr	r2, [r3, #4]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080fe0c4
	ldr	r3, [r1, #0]
	mov	r0, r9
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080fe0c4
	ldr	r3, [r4, #24]
	movs	r1, #226
	lsls	r1, r1, #1
	lsls	r3, r3, #1
	adds	r3, r3, r1
	ldrh	r3, [r7, r3]
	ldr	r0, [pc, #28]
	str	r4, [sp, #8]
	ands	r0, r3
	bl	sub_080ad078
	ldrb	r3, [r0, #6]
	ldr	r4, [sp, #8]
	cmp	r3, #0
	bne.n	.L_080fe09c
.L_080fe088:
	movs	r0, #114
	bl	sub_081c0010
	ldr	r4, [sp, #8]
	b.n	.L_080fe0c4
	movs	r0, r0
	.4byte 0x00003fff
	.2byte 0x1150
	.2byte 0x0300
.L_080fe09c:
	ldr	r2, [sp, #52]
	ldr	r1, [sp, #16]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r2, r0
	ldrb	r0, [r1, r3]
	ldr	r3, [r4, #24]
	movs	r2, #226
	lsls	r2, r2, #1
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r1, [r7, r3]
	movs	r2, #1
	str	r4, [sp, #8]
	bl	.L_080fd52c
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_080fe0c4
	b.n	.L_080fdba0
.L_080fe0c4:
	movs	r0, #168
	lsls	r0, r0, #1
	str	r4, [sp, #8]
	bl	sub_08016ce4
	adds	r6, r0, #0
	ldr	r4, [sp, #8]
	cmp	r6, #0
	bne.n	.L_080fe0d8
	b.n	.L_080fdc14
.L_080fe0d8:
	ldr	r3, [sp, #36]
	cmp	r3, #0
	bne.n	.L_080fe0f0
	movs	r0, #168
	lsls	r0, r0, #1
	str	r4, [sp, #8]
	bl	sub_08016ce4
	ldr	r4, [sp, #8]
	cmp	r0, #0
	bne.n	.L_080fe0f0
	b.n	.L_080fdb34
.L_080fe0f0:
	movs	r0, #135
	lsls	r0, r0, #2
	adds	r1, r7, r0
	ldrh	r2, [r1, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #253
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r0, [r7, #72]
	str	r4, [sp, #8]
	bl	sub_080f8888
	ldr	r1, [sp, #12]
	ldr	r4, [sp, #8]
	movs	r3, #180
	lsls	r3, r3, #1
	adds	r2, r1, r3
	ldr	r3, [r4, #24]
	movs	r1, #133
	strh	r3, [r7, r2]
	ldr	r0, [sp, #52]
	ldr	r2, [sp, #16]
	lsls	r1, r1, #2
	adds	r3, r0, r1
	ldrb	r3, [r2, r3]
	movs	r0, #153
	ldr	r2, [r4, #24]
	lsls	r0, r0, #2
	adds	r3, r3, r0
	strb	r2, [r7, r3]
	ldr	r1, [sp, #12]
	add	r0, sp, #44
	ldrh	r0, [r0, #0]
	movs	r2, #182
	lsls	r2, r2, #1
	adds	r3, r1, r2
	strh	r0, [r7, r3]
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080fe14e
	movs	r1, #1
	negs	r1, r1
	str	r1, [sp, #44]
.L_080fe14e:
	movs	r0, #1
	bl	sub_08013560
	ldr	r0, [sp, #44]
	add	sp, #104
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
.L_080fe164:
	push	{lr}
	lsls	r0, r0, #18
	lsrs	r0, r0, #18
	bl	sub_080ad078
	ldrb	r3, [r0, #6]
	cmp	r3, #0
	bne.n	.L_080fe180
	ldrb	r2, [r0, #1]
	movs	r3, #192
	ands	r3, r2
	movs	r0, #1
	cmp	r3, #192
	bne.n	.L_080fe182
.L_080fe180:
	movs	r0, #0
.L_080fe182:
	pop	{pc}
	push	{r5, r6, r7, lr}
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #236
	movs	r0, #220
	sub	sp, #4
	bl	sub_08014cc0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #24]
	movs	r3, #1
	adds	r7, r0, #0
	movs	r1, #0
	strh	r3, [r2, #4]
	movs	r0, #0
	movs	r3, #20
	movs	r2, #30
	bl	sub_080383e8
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #0
	bl	sub_080f80e0
	movs	r2, #129
	lsls	r2, r2, #2
	adds	r0, r7, r2
	bl	sub_080ad100
	movs	r2, #139
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r7, r2
	strb	r0, [r3, #0]
	movs	r1, #3
	movs	r0, #0
	movs	r2, #0
	movs	r3, #7
	bl	sub_080fee04
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #17
	movs	r3, #5
	movs	r0, #13
	bl	sub_08038010
	adds	r3, r7, #0
	adds	r3, #240
	str	r0, [r3, #0]
	ldr	r1, [pc, #12]
	movs	r0, #151
	lsls	r0, r0, #1
	movs	r2, #3
	adds	r3, r7, r0
	b.n	.L_080fe200
	movs	r0, r0
	.2byte 0x001a
	.2byte 0x0000
.L_080fe200:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L_080fe200
	movs	r3, #135
	lsls	r3, r3, #2
	adds	r2, r7, r3
	movs	r3, #3
	strh	r3, [r2, #0]
	bl	.L_080fe240
	adds	r6, r0, #0
	ldr	r0, [r7, #40]
	bl	sub_08038268
	bl	sub_080fa478
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #24]
	movs	r5, #0
	strh	r5, [r3, #4]
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #220
	bl	sub_0801314c
	adds	r0, r6, #0
	add	sp, #4
	pop	{r5, r6, r7, pc}
.L_080fe240:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r5, [r3, #0]
	movs	r2, #0
	movs	r1, #180
	lsls	r1, r1, #1
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	movs	r0, #0
	bl	sub_080fe580
	movs	r3, #1
	negs	r3, r3
	adds	r2, r0, #0
	cmp	r0, r3
	beq.n	.L_080fe26e
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #22
	adds	r3, r5, r1
	ldrb	r2, [r3, #0]
.L_080fe26e:
	adds	r0, r2, #0
	pop	{r5, pc}
	.2byte 0x0000
