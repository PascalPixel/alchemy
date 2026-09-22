.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_08014d78, 0x08014d78
	.set sub_08016ca4, 0x08016ca4
	.set sub_08038030, 0x08038030
	.set sub_08038068, 0x08038068
	.set sub_08038078, 0x08038078
	.set sub_08038080, 0x08038080
	.set sub_08038090, 0x08038090
	.set sub_08038098, 0x08038098
	.set sub_080380b0, 0x080380b0
	.set sub_080380b8, 0x080380b8
	.set sub_08038120, 0x08038120
	.set sub_08038250, 0x08038250
	.set sub_08038270, 0x08038270
	.set sub_080ad1a0, 0x080ad1a0
	.set sub_080ad200, 0x080ad200
	.set sub_080ad250, 0x080ad250
	.set sub_080f9224, 0x080f9224
	.set sub_081054e0, 0x081054e0
	.set sub_08105580, 0x08105580
	.global Overlay_080ff1f4
Overlay_080ff1f4:
	push	{r5, r6, r7, lr}
	mov	ip, r3
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	sub	sp, #8
	mov	lr, r3
	cmp	r0, #0
	bne.n	.L_080ff23c
	lsls	r3, r1, #1
	adds	r5, r3, #5
	ldrb	r3, [r2, #0]
	movs	r7, #0
	movs	r6, #5
	movs	r4, #0
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_080ff226
	cmp	r1, #0
	bne.n	.L_080ff224
	ldr	r3, [pc, #88]
	ldrb	r6, [r3, #0]
	b.n	.L_080ff24e
.L_080ff224:
	adds	r4, #1
.L_080ff226:
	adds	r0, #1
	cmp	r0, #4
	bgt.n	.L_080ff24e
	ldrb	r3, [r2, r0]
	cmp	r3, #0
	beq.n	.L_080ff226
	cmp	r1, r4
	bne.n	.L_080ff224
	ldr	r3, [pc, #64]
	ldrb	r6, [r3, r0]
	b.n	.L_080ff24e
.L_080ff23c:
	cmp	r1, #3
	bgt.n	.L_080ff248
	adds	r5, r1, #0
	movs	r7, #5
	movs	r6, #13
	b.n	.L_080ff24e
.L_080ff248:
	adds	r5, r1, #4
	movs	r7, #8
	movs	r6, #20
.L_080ff24e:
	movs	r1, #1
	mov	r2, ip
	adds	r3, r1, #0
	eors	r3, r2
	negs	r2, r3
	orrs	r2, r3
	lsrs	r2, r2, #31
	movs	r3, #15
	subs	r3, r3, r2
	mov	r2, lr
	ldr	r0, [r2, #40]
	str	r1, [sp, #0]
	str	r3, [sp, #4]
	adds	r1, r7, #0
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	sub_080f9224
	add	sp, #8
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x5a38
	.2byte 0x0810
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	sl, r2
	movs	r7, #0
	movs	r6, #0
.L_080ff290:
	mov	r2, sl
	ldrb	r3, [r2, r6]
	cmp	r3, #0
	beq.n	.L_080ff2c0
	cmp	r9, r7
	bne.n	.L_080ff2be
	ldr	r3, [pc, #64]
	lsls	r5, r6, #1
	adds	r5, r5, r3
	movs	r3, #1
	adds	r0, r5, #0
	mov	r1, r8
	movs	r2, #0
	negs	r3, r3
	adds	r5, #1
	bl	sub_08038078
	adds	r0, r5, #0
	mov	r1, r8
	movs	r2, #0
	movs	r3, #15
	bl	sub_08038078
.L_080ff2be:
	adds	r7, #1
.L_080ff2c0:
	adds	r6, #1
	cmp	r6, #4
	ble.n	.L_080ff290
	cmp	r7, #0
	bne.n	.L_080ff2d6
	ldr	r0, [pc, #24]
	mov	r1, r8
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038078
.L_080ff2d6:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x0000110d
	.2byte 0x110b
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	mov	r8, r0
	adds	r6, r1, #0
	ldr	r3, [r3, #0]
	cmp	r2, #0
	bne.n	.L_080ff304
	cmp	r6, #3
	ble.n	.L_080ff304
	adds	r6, #1
.L_080ff304:
	cmp	r6, #1
	bne.n	.L_080ff33c
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #22
	adds	r7, r3, r2
	ldrb	r0, [r7, #0]
	bl	sub_08016ca4
	adds	r5, r0, #0
	ldrb	r3, [r5, #15]
	cmp	r3, #99
	bne.n	.L_080ff322
	movs	r6, #8
	b.n	.L_080ff33c
.L_080ff322:
	ldrb	r1, [r5, #15]
	ldrb	r0, [r7, #0]
	adds	r1, #1
	bl	sub_080ad200
	movs	r2, #146
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	movs	r1, #5
	subs	r0, r0, r3
	bl	sub_08038120
.L_080ff33c:
	movs	r0, #128
	bl	sub_08014d78
	adds	r5, r0, #0
	ldr	r0, [pc, #36]
	adds	r1, r5, #0
	adds	r0, r6, r0
	movs	r2, #64
	bl	sub_08038030
	movs	r3, #1
	adds	r0, r5, #0
	negs	r3, r3
	mov	r1, r8
	movs	r2, #0
	bl	sub_08038250
	adds	r0, r5, #0
	bl	sub_08013164
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x1117
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #40
	str	r2, [sp, #24]
	str	r1, [sp, #28]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	adds	r7, r0, #0
	movs	r0, #1
	negs	r0, r0
	mov	r8, r3
	bl	sub_080ad250
	negs	r3, r0
	orrs	r3, r0
	lsrs	r3, r3, #31
	ldr	r0, [sp, #28]
	str	r3, [sp, #8]
	bl	sub_08016ca4
	ldr	r2, [sp, #24]
	movs	r3, #255
	ands	r3, r2
	movs	r2, #7
	str	r0, [sp, #20]
	str	r2, [sp, #12]
	cmp	r3, #1
	beq.n	.L_080ff3ba
	movs	r3, #10
	str	r3, [sp, #12]
.L_080ff3ba:
	movs	r3, #184
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r2, [r3, #0]
	movs	r3, #1
	strb	r3, [r2, #5]
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #24]
	adds	r0, r7, #0
	bl	.L_080ff5bc
	add	r6, sp, #32
	ldr	r2, [sp, #28]
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08105580
	ldr	r2, [sp, #24]
	movs	r5, #128
	lsls	r5, r5, #1
	ands	r5, r2
	cmp	r5, #0
	bne.n	.L_080ff3f6
	movs	r3, #96
	adds	r0, r7, #0
	movs	r1, #0
	movs	r2, #40
	str	r3, [sp, #0]
	bl	sub_08038068
.L_080ff3f6:
	adds	r3, r6, #0
	adds	r0, r7, #0
	movs	r1, #0
	movs	r2, #40
	bl	sub_081054e0
	movs	r3, #135
	lsls	r3, r3, #2
	add	r3, r8
	ldrh	r3, [r3, #0]
	cmp	r3, #3
	bne.n	.L_080ff410
	b.n	.L_080ff59c
.L_080ff410:
	cmp	r5, #0
	bne.n	.L_080ff42a
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #96
	str	r3, [sp, #0]
	adds	r0, r7, #0
	movs	r1, #64
	movs	r2, #56
	movs	r3, #224
	bl	sub_08038068
.L_080ff42a:
	movs	r0, #15
	bl	sub_080380b8
	ldr	r3, [sp, #24]
	cmp	r3, #1
	beq.n	.L_080ff43c
	ldr	r2, [sp, #8]
	cmp	r2, #1
	bne.n	.L_080ff476
.L_080ff43c:
	movs	r5, #4
	ldr	r3, [sp, #12]
	adds	r0, r7, #0
	movs	r1, #1
	movs	r2, #15
	str	r5, [sp, #0]
	bl	sub_08038270
	adds	r0, r7, #0
	ldr	r3, [sp, #12]
	movs	r1, #2
	movs	r2, #19
	str	r5, [sp, #0]
	bl	sub_08038270
	adds	r0, r7, #0
	movs	r1, #3
	movs	r2, #23
	ldr	r3, [sp, #12]
	str	r5, [sp, #0]
	bl	sub_08038270
	adds	r0, r7, #0
	movs	r1, #4
	movs	r2, #27
	ldr	r3, [sp, #12]
	str	r5, [sp, #0]
	bl	sub_08038270
.L_080ff476:
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq.n	.L_080ff48e
	ldr	r2, [sp, #12]
	ldr	r0, [pc, #300]
	lsls	r6, r2, #3
	adds	r3, r6, #0
	adds	r3, #8
	adds	r1, r7, #0
	movs	r2, #64
	bl	sub_08038080
.L_080ff48e:
	ldr	r3, [sp, #24]
	cmp	r3, #1
	bne.n	.L_080ff4d2
	ldr	r2, [sp, #8]
	cmp	r2, #0
	bne.n	.L_080ff4a0
	ldr	r3, [sp, #12]
	subs	r3, #1
	str	r3, [sp, #12]
.L_080ff4a0:
	ldr	r2, [sp, #12]
	ldr	r0, [pc, #268]
	lsls	r6, r2, #3
	adds	r3, r6, #0
	adds	r3, #16
	adds	r1, r7, #0
	movs	r2, #64
	bl	sub_08038090
	ldr	r5, [pc, #256]
	adds	r3, r6, #0
	adds	r0, r5, #0
	adds	r3, #24
	adds	r1, r7, #0
	movs	r2, #64
	bl	sub_08038080
	adds	r5, #1
	adds	r3, r6, #0
	adds	r3, #32
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #64
	bl	sub_08038080
.L_080ff4d2:
	ldr	r2, [sp, #12]
	movs	r3, #0
	lsls	r2, r2, #3
	str	r3, [sp, #16]
	ldr	r3, [sp, #20]
	str	r2, [sp, #4]
	adds	r2, #8
	mov	fp, r2
	movs	r2, #104
	adds	r3, #72
	mov	r9, r2
	ldr	r2, [sp, #20]
	mov	r8, r3
	movs	r3, #120
	mov	sl, r3
	adds	r3, #160
	adds	r5, r2, r3
.L_080ff4f4:
	ldr	r2, [sp, #8]
	cmp	r2, #0
	beq.n	.L_080ff50a
	mov	r3, fp
	ldrb	r0, [r5, #0]
	movs	r1, #1
	str	r3, [sp, #0]
	adds	r2, r7, #0
	mov	r3, sl
	bl	sub_080380b0
.L_080ff50a:
	ldr	r2, [sp, #24]
	movs	r3, #255
	ands	r3, r2
	cmp	r3, #1
	bne.n	.L_080ff586
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq.n	.L_080ff53c
	mov	r2, fp
	ldrb	r0, [r5, #4]
	movs	r1, #1
	str	r2, [sp, #0]
	mov	r3, r9
	adds	r2, r7, #0
	ldr	r6, [sp, #4]
	bl	sub_080380b0
	mov	r2, sl
	subs	r2, #8
	ldr	r0, [pc, #132]
	adds	r1, r7, #0
	mov	r3, fp
	bl	sub_08038098
	b.n	.L_080ff540
.L_080ff53c:
	ldr	r3, [sp, #12]
	lsls	r6, r3, #3
.L_080ff540:
	ldr	r1, [sp, #16]
	ldr	r0, [sp, #28]
	bl	sub_080ad1a0
	adds	r2, r6, #0
	adds	r2, #16
	mov	r3, sl
	str	r2, [sp, #0]
	subs	r3, #8
	adds	r2, r7, #0
	movs	r1, #2
	bl	sub_080380b0
	mov	r3, r8
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	adds	r3, r6, #0
	adds	r3, #24
	adds	r2, r7, #0
	str	r3, [sp, #0]
	movs	r1, #3
	mov	r3, r9
	bl	sub_080380b0
	mov	r3, r8
	movs	r2, #2
	ldrsh	r0, [r3, r2]
	adds	r3, r6, #0
	adds	r3, #32
	str	r3, [sp, #0]
	movs	r1, #3
	adds	r2, r7, #0
	mov	r3, r9
	bl	sub_080380b0
.L_080ff586:
	movs	r2, #4
	add	r8, r2
	ldr	r2, [sp, #16]
	movs	r3, #32
	adds	r2, #1
	add	r9, r3
	add	sl, r3
	adds	r5, #1
	str	r2, [sp, #16]
	cmp	r2, #3
	ble.n	.L_080ff4f4
.L_080ff59c:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0000102c
	.4byte 0x08105974
	.4byte 0x0000102d
	.2byte 0x5978
	.2byte 0x0810
.L_080ff5bc:
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
	adds	r6, r0, #0
	adds	r0, r1, #0
	mov	r8, r2
	ldr	r5, [r3, #0]
	sub	sp, #4
	bl	sub_08016ca4
	movs	r2, #184
	lsls	r2, r2, #1
	adds	r5, r5, r2
	ldr	r2, [r5, #0]
	movs	r3, #1
	strb	r3, [r2, #5]
	mov	r2, r8
	adds	r3, #255
	ands	r2, r3
	adds	r7, r0, #0
	mov	r8, r2
	cmp	r2, #0
	bne.n	.L_080ff608
	movs	r3, #40
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #128
	bl	sub_08038068
.L_080ff608:
	adds	r0, r7, #0
	adds	r1, r6, #0
	movs	r2, #40
	movs	r3, #0
	bl	sub_08038090
	movs	r2, #42
	adds	r2, #255
	adds	r3, r7, r2
	ldrb	r0, [r3, #0]
	ldr	r3, [pc, #380]
	adds	r1, r6, #0
	adds	r0, r0, r3
	movs	r2, #0
	movs	r3, #32
	bl	sub_08038080
	adds	r1, r6, #0
	movs	r2, #104
	movs	r3, #0
	ldr	r0, [pc, #364]
	bl	sub_08038090
	movs	r0, #15
	bl	sub_080380b8
	movs	r3, #0
	ldrb	r0, [r7, #15]
	movs	r1, #2
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r3, #128
	bl	sub_080380b0
	movs	r2, #40
	ldr	r0, [pc, #340]
	adds	r1, r6, #0
	movs	r3, #16
	bl	sub_08038090
	movs	r3, #16
	movs	r2, #56
	ldrsh	r0, [r7, r2]
	mov	r9, r3
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r1, #4
	movs	r3, #72
	bl	sub_080380b0
	mov	r3, r9
	movs	r2, #52
	ldrsh	r0, [r7, r2]
	movs	r1, #4
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r3, #112
	bl	sub_080380b0
	ldr	r5, [pc, #296]
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #104
	movs	r3, #16
	bl	sub_08038098
	movs	r2, #40
	ldr	r0, [pc, #284]
	adds	r1, r6, #0
	movs	r3, #24
	bl	sub_08038090
	movs	r3, #24
	movs	r2, #58
	ldrsh	r0, [r7, r2]
	mov	sl, r3
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r1, #4
	movs	r3, #72
	bl	sub_080380b0
	mov	r3, sl
	movs	r2, #54
	ldrsh	r0, [r7, r2]
	movs	r1, #4
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r3, #112
	bl	sub_080380b0
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #104
	movs	r3, #24
	bl	sub_08038098
	ldr	r5, [pc, #228]
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #40
	movs	r3, #8
	bl	sub_08038080
	movs	r2, #146
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	movs	r3, #8
	str	r3, [sp, #0]
	adds	r2, r6, #0
	mov	fp, r3
	movs	r1, #8
	movs	r3, #80
	bl	sub_080380b0
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_080ff70c
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #40
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #144
	movs	r2, #0
	movs	r3, #224
	bl	sub_08038068
.L_080ff70c:
	adds	r0, r5, #0
	adds	r1, r6, #0
	subs	r0, #23
	movs	r2, #152
	movs	r3, #0
	bl	sub_08038080
	adds	r0, r5, #0
	adds	r1, r6, #0
	subs	r0, #22
	movs	r2, #152
	movs	r3, #8
	bl	sub_08038080
	adds	r0, r5, #0
	adds	r1, r6, #0
	subs	r0, #21
	movs	r2, #152
	movs	r3, #16
	bl	sub_08038080
	adds	r0, r5, #0
	adds	r1, r6, #0
	subs	r0, #20
	movs	r2, #152
	movs	r3, #24
	bl	sub_08038080
	movs	r3, #0
	ldrh	r0, [r7, #60]
	adds	r2, r6, #0
	str	r3, [sp, #0]
	movs	r1, #3
	movs	r3, #200
	bl	sub_080380b0
	mov	r2, fp
	ldrh	r0, [r7, #62]
	movs	r1, #3
	str	r2, [sp, #0]
	movs	r3, #200
	adds	r2, r6, #0
	bl	sub_080380b0
	adds	r3, r7, #0
	adds	r3, #64
	ldrh	r0, [r3, #0]
	mov	r3, r9
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r1, #3
	movs	r3, #200
	bl	sub_080380b0
	adds	r3, r7, #0
	mov	r2, sl
	adds	r3, #66
	ldrb	r0, [r3, #0]
	movs	r1, #3
	str	r2, [sp, #0]
	movs	r3, #200
	adds	r2, r6, #0
	bl	sub_080380b0
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00000b63
	.4byte 0x08105974
	.4byte 0x0810597c
	.4byte 0x08105978
	.4byte 0x08105980
	.2byte 0x103d
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
	ble.n	.L_080ff7fa
	subs	r6, r7, #1
.L_080ff7fa:
	cmp	r7, #0
	bne.n	.L_080ff800
	movs	r6, #0
.L_080ff800:
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
	beq.n	.L_080ff82c
	adds	r5, #1
.L_080ff82c:
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
