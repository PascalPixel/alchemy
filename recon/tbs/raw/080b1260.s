.syntax unified
	.thumb
	.set sub_08015060, 0x08015060
	.set sub_08015070, 0x08015070
	.set sub_08015078, 0x08015078
	.set sub_08015080, 0x08015080
	.set sub_080150b0, 0x080150b0
	.set sub_080150c8, 0x080150c8
	.set sub_08077008, 0x08077008
	.set sub_08077010, 0x08077010
	.set sub_08077018, 0x08077018
	.set sub_08077218, 0x08077218
	.set sub_08077228, 0x08077228
	.global Func_080b1260
	.thumb_func
Func_080b1260:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #56
	str	r1, [sp, #20]
	ldr	r3, [pc, #168]
	ldr	r3, [r3, #0]
	mov	sl, r0
	adds	r0, r1, #0
	mov	r9, r2
	str	r3, [sp, #16]
	bl	sub_08077008
	adds	r7, r0, #0
	mov	r0, r9
	bl	sub_08077018
	adds	r5, r0, #0
	movs	r0, #1
	negs	r0, r0
	mov	r1, sl
	str	r0, [sp, #12]
	cmp	r1, #0
	bne.n	.L_080b129a
	b.n	.L_080b1450
.L_080b129a:
	mov	r0, sl
	bl	sub_08015060
	mov	r1, r9
	ldr	r0, [sp, #20]
	bl	sub_08077218
	cmp	r0, #0
	bne.n	.L_080b12ba
	ldr	r0, [pc, #112]
	mov	r1, sl
	movs	r2, #8
	movs	r3, #24
	bl	sub_08015078
	b.n	.L_080b1450
.L_080b12ba:
	ldrb	r1, [r5, #2]
	ldr	r0, [sp, #20]
	bl	sub_08077228
	ldr	r2, [sp, #12]
	cmp	r0, r2
	bne.n	.L_080b1324
	movs	r3, #216
	movs	r1, #128
	ldrh	r2, [r7, r3]
	lsls	r1, r1, #2
	adds	r3, r1, #0
	ands	r3, r2
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_080b12f2
	mov	ip, r1
	adds	r1, r7, #0
	adds	r1, #216
.L_080b12e0:
	adds	r5, #1
	cmp	r5, #14
	bgt.n	.L_080b12f2
	adds	r1, #2
	ldrh	r2, [r1, #0]
	mov	r3, ip
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080b12e0
.L_080b12f2:
	cmp	r5, #15
	bne.n	.L_080b1318
	adds	r6, r7, #0
	movs	r5, #0
	adds	r6, #216
	b.n	.L_080b1300
.L_080b12fe:
	adds	r5, #1
.L_080b1300:
	cmp	r5, #14
	bgt.n	.L_080b1312
	ldrh	r0, [r6, #0]
	bl	sub_08077018
	ldrb	r3, [r0, #2]
	adds	r6, #2
	cmp	r3, #6
	bne.n	.L_080b12fe
.L_080b1312:
	cmp	r5, #15
	bne.n	.L_080b1318
	movs	r5, #0
.L_080b1318:
	lsls	r0, r5, #1
	b.n	.L_080b1332
	.4byte 0x03001f2c
	.2byte 0x0c8e
	.2byte 0x0000
.L_080b1324:
	lsls	r0, r0, #1
	adds	r3, r0, #0
	adds	r3, #216
	ldrh	r3, [r7, r3]
	ldr	r1, [pc, #60]
	ands	r1, r3
	str	r1, [sp, #12]
.L_080b1332:
	ldr	r3, [pc, #52]
	adds	r5, r0, #0
	mov	r0, r9
	orrs	r0, r3
	mov	r9, r0
	mov	r1, r9
	adds	r5, #216
	ldrh	r2, [r7, r5]
	strh	r1, [r7, r5]
	ldr	r0, [sp, #20]
	mov	r8, r2
	bl	sub_08077010
	ldrh	r3, [r7, #60]
	add	r2, sp, #24
	str	r3, [r2, #0]
	ldrh	r3, [r7, #62]
	adds	r6, r7, #0
	str	r3, [r2, #4]
	adds	r6, #64
	ldrh	r3, [r6, #0]
	str	r3, [r2, #8]
	adds	r3, r7, #0
	adds	r3, #66
	str	r3, [sp, #8]
	b.n	.L_080b1370
	movs	r0, r0
	.4byte 0x00000200
	.2byte 0x01ff
	.2byte 0x0000
.L_080b1370:
	ldrb	r3, [r3, #0]
	mov	r0, r8
	str	r3, [r2, #12]
	strh	r0, [r7, r5]
	ldr	r0, [sp, #20]
	mov	fp, r2
	bl	sub_08077010
	ldrh	r3, [r7, #60]
	add	r1, sp, #40
	str	r3, [r1, #0]
	ldrh	r3, [r7, #62]
	str	r3, [r1, #4]
	ldrh	r3, [r6, #0]
	str	r3, [r1, #8]
	ldr	r2, [sp, #8]
	ldrb	r3, [r2, #0]
	str	r3, [r1, #12]
	movs	r3, #2
	movs	r5, #0
	str	r3, [sp, #4]
	mov	r9, r1
	mov	r8, r5
	movs	r7, #0
.L_080b13a0:
	mov	r0, r8
	mov	r1, r9
	ldr	r2, [r0, r1]
	mov	r1, fp
	ldr	r3, [r0, r1]
	cmp	r2, r3
	ble.n	.L_080b13b6
	ldr	r2, [sp, #16]
	ldr	r0, [pc, #176]
	adds	r3, r2, r0
	b.n	.L_080b13c2
.L_080b13b6:
	cmp	r2, r3
	bge.n	.L_080b13dc
	ldr	r1, [sp, #16]
	movs	r2, #230
	lsls	r2, r2, #2
	adds	r3, r1, r2
.L_080b13c2:
	ldrh	r0, [r3, #0]
	movs	r1, #128
	subs	r3, r7, #4
	str	r3, [sp, #0]
	lsls	r1, r1, #23
	movs	r3, #56
	mov	r2, sl
	bl	sub_080150c8
	movs	r3, #0
	adds	r6, r7, #0
	strb	r3, [r0, #4]
	b.n	.L_080b13de
.L_080b13dc:
	lsls	r6, r5, #4
.L_080b13de:
	add	r1, sp, #40
	mov	r3, r8
	ldr	r0, [r3, r1]
	mov	r2, sl
	movs	r1, #3
	movs	r3, #32
	str	r7, [sp, #0]
	bl	sub_080150b0
	mov	r2, r8
	add	r0, sp, #40
	mov	r1, fp
	ldr	r3, [r2, r0]
	ldr	r0, [r2, r1]
	cmp	r3, r0
	beq.n	.L_080b140a
	movs	r1, #3
	mov	r2, sl
	movs	r3, #72
	str	r6, [sp, #0]
	bl	sub_080150b0
.L_080b140a:
	ldr	r0, [pc, #92]
	mov	r1, sl
	adds	r0, r5, r0
	movs	r2, #0
	adds	r3, r6, #0
	bl	sub_08015080
	ldr	r2, [sp, #4]
	mov	r0, sl
	movs	r3, #13
	movs	r1, #0
	str	r2, [sp, #0]
	bl	sub_08015070
	ldr	r3, [sp, #4]
	movs	r0, #4
	adds	r3, #2
	adds	r5, #1
	str	r3, [sp, #4]
	add	r8, r0
	adds	r7, #16
	cmp	r5, #2
	ble.n	.L_080b13a0
	movs	r2, #1
	ldr	r1, [sp, #12]
	negs	r2, r2
	cmp	r1, r2
	beq.n	.L_080b1450
	ldr	r0, [pc, #40]
	movs	r2, #0
	adds	r0, r1, r0
	movs	r3, #48
	mov	r1, sl
	bl	sub_08015080
.L_080b1450:
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000039a
	.4byte 0x00000c98
	.4byte 0x00000182
