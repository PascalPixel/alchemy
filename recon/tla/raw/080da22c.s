.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_080148e8, 0x080148e8
	.set sub_08014d78, 0x08014d78
	.set sub_08020090, 0x08020090
	.set sub_080cad84, 0x080cad84
	.set sub_080d8d40, 0x080d8d40
	.global Func_080da22c
	.thumb_func
Func_080da22c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r1, [sp, #12]
	str	r2, [sp, #8]
	mov	fp, r3
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #160
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #48]
	mov	sl, r3
	movs	r3, #0
	str	r3, [sp, #4]
	ldr	r3, [sp, #12]
	ldr	r6, [sp, #52]
	subs	r1, r1, r3
	ldr	r3, [sp, #8]
	ldr	r2, [sp, #56]
	subs	r6, r6, r3
	mov	r3, fp
	mov	r9, r0
	subs	r0, r2, r3
	ldr	r2, [pc, #196]
	asrs	r1, r1, #8
	mov	r8, r2
	asrs	r7, r0, #8
	adds	r0, r1, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1236
	adds	r1, r6, #0
	adds	r5, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c39
	adds	r6, r0, #0
	adds	r0, r7, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x19ad
	adds	r5, r5, r0
	ldr	r3, [pc, #160]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2106
	bl	sub_08002054
	mov	r2, r9
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	add	sl, r3
	movs	r6, #128
	movs	r3, #36
	movs	r2, #0
	adds	r0, #1
	add	sl, r3
	movs	r7, #0
	lsls	r6, r6, #11
	mov	r9, r2
	str	r0, [sp, #0]
	b.n	.L_080da300
.L_080da2b8:
	adds	r0, r7, #0
	bl	sub_08002090
	adds	r1, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x9b03
	adds	r0, r3, r0
	str	r0, [r5, #4]
	ldr	r2, [sp, #8]
	adds	r0, r7, #0
	str	r2, [r5, #8]
	bl	sub_08002096
	adds	r1, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x4458
	movs	r3, #2
	mov	r2, sl
	str	r0, [r5, #12]
	movs	r0, #192
	strb	r3, [r5, #16]
	adds	r1, r6, #0
	str	r5, [r2, #0]
	lsls	r0, r0, #11
	bl	sub_080148e8
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	adds	r7, r7, r0
	movs	r3, #1
	lsls	r0, r0, #2
	mov	sl, r5
	adds	r6, r6, r0
	add	r9, r3
.L_080da300:
	ldr	r2, [sp, #0]
	cmp	r9, r2
	bgt.n	.L_080da316
	bl	sub_080d8d40
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_080da2b8
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #4]
.L_080da316:
	movs	r3, #0
	mov	r2, sl
	str	r3, [r2, #0]
	ldr	r0, [sp, #4]
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	sl, r0
	adds	r0, r1, #0
	adds	r5, r2, #0
	sub	sp, #16
	bl	sub_080cad84
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_080cad84
	adds	r6, r0, #0
	cmp	r7, #0
	beq.n	.L_080da3ac
	cmp	r6, #0
	beq.n	.L_080da3ac
	ldr	r2, [pc, #92]
	movs	r5, #128
	ldr	r0, [r2, #0]
	mov	r8, r2
	bl	sub_08002096
	ldr	r4, [r6, #8]
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	str	r4, [sp, #0]
	lsls	r5, r5, #12
	ldr	r4, [r6, #12]
	lsls	r0, r0, #2
	adds	r4, r4, r5
	str	r4, [sp, #4]
	adds	r2, r2, r5
	ldr	r4, [r6, #16]
	str	r0, [sp, #12]
	mov	r0, sl
	str	r4, [sp, #8]
	bl	.L_080da3bc
	mov	r2, r8
	ldr	r3, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	mov	r2, r8
	str	r3, [r2, #0]
	movs	r0, #6
	bl	sub_08013560
	adds	r0, r7, #0
	movs	r1, #2
	bl	sub_08020090
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08020090
.L_080da3ac:
	add	sp, #16
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x3948
	.2byte 0x080f
.L_080da3bc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #88
	str	r1, [sp, #80]
	str	r2, [sp, #76]
	str	r3, [sp, #72]
	str	r0, [sp, #84]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #160
	ldr	r3, [r3, #0]
	ldr	r2, [sp, #120]
	str	r3, [sp, #68]
	ldr	r3, [sp, #80]
	ldr	r1, [sp, #124]
	subs	r2, r2, r3
	str	r2, [sp, #64]
	asrs	r2, r2, #8
	mov	r9, r2
	ldr	r2, [sp, #76]
	ldr	r3, [sp, #72]
	ldr	r0, [sp, #128]
	subs	r1, r1, r2
	str	r1, [sp, #60]
	subs	r0, r0, r3
	asrs	r1, r1, #8
	mov	fp, r1
	adds	r1, r0, #0
	asrs	r1, r1, #8
	str	r0, [sp, #56]
	mov	sl, r1
	ldr	r6, [pc, #592]
	mov	r1, r9
	mov	r0, r9
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4659
	adds	r5, r0, #0
	mov	r0, fp
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4651
	mov	r8, r0
	mov	r0, sl
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4445
	adds	r5, r5, r0
	ldr	r3, [pc, #564]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2106
	bl	sub_08002054
	ldr	r2, [sp, #84]
	ldr	r1, [sp, #84]
	lsls	r2, r2, #3
	adds	r0, #1
	subs	r3, r2, r1
	str	r0, [sp, #52]
	str	r2, [sp, #44]
	ldr	r2, [sp, #68]
	lsls	r3, r3, #2
	movs	r0, #128
	adds	r3, #36
	lsls	r0, r0, #2
	ldr	r7, [r2, r3]
	bl	sub_08014d78
	ldr	r1, [sp, #52]
	str	r0, [sp, #20]
	adds	r3, r0, #0
	cmp	r1, #0
	blt.n	.L_080da470
	adds	r1, #1
	mov	r8, r1
.L_080da460:
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	mov	r1, r8
	stmia	r3!, {r7}
	ldr	r7, [r7, #0]
	cmp	r1, #0
	bne.n	.L_080da460
.L_080da470:
	mov	r1, fp
	mov	r0, r9
	bl	sub_080148e8
	adds	r5, r0, #0
	lsls	r5, r5, #16
	lsrs	r5, r5, #16
	adds	r0, r5, #0
	bl	sub_08002096
	str	r0, [sp, #40]
	adds	r0, r5, #0
	bl	sub_08002090
	ldr	r3, [sp, #52]
	movs	r2, #1
	lsls	r3, r3, #1
	str	r0, [sp, #36]
	str	r2, [sp, #48]
	str	r3, [sp, #16]
	cmp	r2, r3
	ble.n	.L_080da49e
	b.n	.L_080da624
.L_080da49e:
	ldr	r1, [sp, #56]
	ldr	r2, [sp, #60]
	ldr	r3, [sp, #64]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	str	r3, [sp, #0]
.L_080da4aa:
	ldr	r1, [sp, #52]
	mov	r8, r1
	cmp	r1, #0
	bge.n	.L_080da4b4
	b.n	.L_080da5f8
.L_080da4b4:
	ldr	r2, [sp, #84]
	adds	r2, #1
	str	r2, [sp, #12]
.L_080da4ba:
	ldr	r2, [sp, #20]
	mov	r1, r8
	lsls	r3, r1, #2
	ldr	r7, [r3, r2]
	add	r3, sp, #12
	ldrb	r3, [r3, #0]
	strb	r3, [r7, #17]
	ldr	r1, [sp, #52]
	cmp	r8, r1
	bne.n	.L_080da546
	ldr	r2, [sp, #48]
	mov	r1, r8
	lsls	r0, r2, #14
	bl	sub_08002054
	bl	sub_08002096
	movs	r1, #128
	ldr	r3, [pc, #376]
	lsls	r1, r1, #12
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4a5c
	ldr	r1, [sp, #36]
	adds	r6, r0, #0
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x9b00
	adds	r5, r0, #0
	lsrs	r0, r3, #31
	adds	r0, r3, r0
	mov	r1, r8
	asrs	r0, r0, #1
	bl	sub_08002054
	ldr	r1, [sp, #80]
	ldr	r2, [pc, #340]
	adds	r0, r1, r0
	adds	r0, r0, r5
	str	r0, [r7, #4]
	ldr	r1, [sp, #40]
	str	r0, [sp, #32]
	adds	r0, r6, #0
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x9b01
	adds	r5, r0, #0
	lsrs	r0, r3, #31
	adds	r0, r3, r0
	mov	r1, r8
	asrs	r0, r0, #1
	bl	sub_08002054
	ldr	r1, [sp, #76]
	adds	r0, r1, r0
	adds	r0, r0, r5
	str	r0, [r7, #8]
	ldr	r2, [sp, #8]
	str	r0, [sp, #28]
	lsrs	r0, r2, #31
	adds	r0, r2, r0
	asrs	r0, r0, #1
	mov	r1, r8
	bl	sub_08002054
	ldr	r3, [sp, #72]
	adds	r0, r3, r0
	str	r0, [r7, #12]
	str	r0, [sp, #24]
	b.n	.L_080da5ea
.L_080da546:
	ldr	r1, [sp, #32]
	ldr	r3, [r7, #4]
	ldr	r2, [sp, #28]
	subs	r3, r1, r3
	asrs	r3, r3, #8
	mov	r9, r3
	ldr	r3, [r7, #8]
	ldr	r1, [sp, #24]
	subs	r3, r2, r3
	asrs	r3, r3, #8
	mov	fp, r3
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #248]
	subs	r3, r1, r3
	asrs	r3, r3, #8
	mov	r1, r9
	mov	r0, r9
	mov	sl, r3
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x4b3a
	adds	r5, r0, #0
	mov	r1, fp
	mov	r0, fp
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4651
	adds	r6, r0, #0
	ldr	r2, [pc, #216]
	mov	r0, sl
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x19ad
	adds	r5, r5, r0
	ldr	r3, [pc, #208]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c06
	cmp	r6, #6
	ble.n	.L_080da5f8
	ldr	r1, [sp, #32]
	ldr	r3, [r7, #4]
	subs	r3, r3, r1
	lsls	r0, r3, #1
	adds	r0, r0, r3
	adds	r1, r6, #0
	lsls	r0, r0, #1
	bl	sub_08002054
	ldr	r2, [sp, #32]
	adds	r0, r2, r0
	str	r0, [r7, #4]
	str	r0, [sp, #32]
	ldr	r1, [sp, #28]
	ldr	r3, [r7, #8]
	subs	r3, r3, r1
	lsls	r0, r3, #1
	adds	r0, r0, r3
	adds	r1, r6, #0
	lsls	r0, r0, #1
	bl	sub_08002054
	ldr	r2, [sp, #28]
	adds	r0, r2, r0
	str	r0, [r7, #8]
	str	r0, [sp, #28]
	ldr	r1, [sp, #24]
	ldr	r3, [r7, #12]
	subs	r3, r3, r1
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #1
	adds	r1, r6, #0
	bl	sub_08002054
	ldr	r2, [sp, #24]
	movs	r3, #1
	adds	r0, r2, r0
	str	r0, [r7, #12]
	str	r0, [sp, #24]
	strb	r3, [r7, #16]
.L_080da5ea:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r1, r8
	cmp	r1, #0
	blt.n	.L_080da5f8
	b.n	.L_080da4ba
.L_080da5f8:
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #56]
	ldr	r1, [sp, #4]
	adds	r2, r2, r3
	str	r2, [sp, #8]
	ldr	r2, [sp, #60]
	ldr	r3, [sp, #0]
	adds	r1, r1, r2
	str	r1, [sp, #4]
	ldr	r1, [sp, #64]
	ldr	r2, [sp, #48]
	adds	r3, r3, r1
	str	r3, [sp, #0]
	ldr	r3, [sp, #16]
	adds	r2, #1
	str	r2, [sp, #48]
	cmp	r2, r3
	bgt.n	.L_080da624
	b.n	.L_080da4aa
.L_080da624:
	ldr	r1, [sp, #44]
	ldr	r2, [sp, #84]
	subs	r3, r1, r2
	ldr	r1, [sp, #68]
	lsls	r3, r3, #2
	adds	r3, r1, r3
	adds	r3, #12
	movs	r2, #1
	str	r2, [r3, #20]
	movs	r2, #128
	lsls	r2, r2, #11
	str	r2, [r3, #8]
	movs	r2, #128
	lsls	r2, r2, #7
	str	r2, [r3, #16]
	ldr	r0, [sp, #20]
	bl	sub_08013164
	movs	r0, #0
	add	sp, #88
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #20
	str	r2, [sp, #16]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #160
	mov	sl, r0
	adds	r0, r1, #0
	ldr	r5, [r3, #0]
	bl	sub_080cad84
	adds	r7, r0, #0
	ldr	r0, [sp, #16]
	bl	sub_080cad84
	adds	r6, r0, #0
	cmp	r7, #0
	beq.n	.L_080da6d0
	cmp	r6, #0
	beq.n	.L_080da6d0
	ldr	r2, [pc, #76]
	str	r7, [r5, #0]
	ldr	r0, [r2, #0]
	str	r6, [r5, #4]
	mov	r8, r2
	bl	sub_08002096
	ldr	r4, [r6, #8]
	movs	r3, #128
	lsls	r3, r3, #12
	ldr	r2, [r7, #12]
	ldr	r1, [r7, #8]
	mov	ip, r3
	ldr	r3, [r7, #16]
	str	r4, [sp, #0]
	lsls	r0, r0, #2
	ldr	r4, [r6, #12]
	add	r2, ip
	add	r4, ip
	str	r4, [sp, #4]
	ldr	r4, [r6, #16]
	str	r0, [sp, #12]
	mov	r0, sl
	str	r4, [sp, #8]
	bl	.L_080da6e0
	mov	r2, r8
	ldr	r3, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r2
	mov	r2, r8
	str	r3, [r2, #0]
.L_080da6d0:
	add	sp, #20
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x394c
	.2byte 0x080f
.L_080da6e0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #44
	str	r1, [sp, #40]
	str	r2, [sp, #36]
	str	r3, [sp, #32]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #160
	ldr	r3, [r3, #0]
	ldr	r2, [sp, #76]
	str	r3, [sp, #28]
	movs	r3, #0
	mov	r9, r3
	ldr	r3, [sp, #40]
	ldr	r1, [sp, #80]
	subs	r2, r2, r3
	ldr	r3, [sp, #36]
	mov	sl, r0
	subs	r1, r1, r3
	ldr	r0, [sp, #84]
	asrs	r4, r1, #8
	ldr	r1, [sp, #32]
	asrs	r2, r2, #8
	subs	r0, r0, r1
	str	r4, [sp, #0]
	adds	r1, r2, #0
	ldr	r7, [pc, #476]
	asrs	r6, r0, #8
	adds	r0, r2, #0
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x9c00
	adds	r5, r0, #0
	adds	r1, r4, #0
	adds	r0, r4, #0
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x1c31
	mov	r8, r0
	adds	r0, r6, #0
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x4445
	adds	r5, r5, r0
	ldr	r3, [pc, #444]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2106
	bl	sub_08002054
	mov	r2, sl
	lsls	r5, r2, #3
	subs	r5, r5, r2
	adds	r0, #1
	str	r0, [sp, #24]
	ldr	r1, [sp, #28]
	lsls	r5, r5, #2
	adds	r3, r5, #0
	movs	r0, #128
	adds	r3, #36
	lsls	r0, r0, #2
	ldr	r7, [r1, r3]
	bl	sub_08014d78
	ldr	r2, [sp, #28]
	adds	r5, #32
	mov	r1, r9
	str	r0, [sp, #8]
	str	r1, [r2, r5]
	ldr	r2, [sp, #24]
	adds	r3, r0, #0
	cmp	r2, #0
	blt.n	.L_080da794
	adds	r2, #1
	mov	r8, r2
.L_080da784:
	movs	r1, #1
	negs	r1, r1
	add	r8, r1
	mov	r2, r8
	stmia	r3!, {r7}
	ldr	r7, [r7, #0]
	cmp	r2, #0
	bne.n	.L_080da784
.L_080da794:
	ldr	r3, [sp, #28]
	movs	r1, #3
	ldr	r0, [r3, #0]
	bl	sub_08020090
	ldr	r1, [sp, #28]
	ldr	r0, [r1, #4]
	movs	r1, #3
	bl	sub_08020090
	movs	r0, #4
	bl	sub_08013560
	ldr	r3, [sp, #24]
	movs	r2, #0
	str	r2, [sp, #20]
	cmp	r2, r3
	ble.n	.L_080da7ba
	b.n	.L_080da8ea
.L_080da7ba:
	ldr	r1, [sp, #20]
	ldr	r3, [sp, #24]
	movs	r2, #0
	lsls	r1, r1, #12
	movs	r5, #128
	mov	r8, r2
	mov	sl, r1
	lsls	r5, r5, #11
	cmp	r8, r3
	ble.n	.L_080da7d0
	b.n	.L_080da8d6
.L_080da7d0:
	ldr	r1, [pc, #300]
	mov	r9, r1
.L_080da7d4:
	ldr	r1, [sp, #8]
	mov	r2, r8
	lsls	r3, r2, #2
	ldr	r7, [r3, r1]
	movs	r3, #0
	strb	r3, [r7, #17]
	ldr	r2, [sp, #20]
	cmp	r8, r2
	bgt.n	.L_080da82e
	mov	r0, sl
	bl	sub_08002090
	adds	r1, r5, #0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x9b0a
	adds	r0, r3, r0
	str	r0, [r7, #4]
	ldr	r1, [sp, #36]
	str	r0, [sp, #16]
	str	r1, [r7, #8]
	mov	r0, sl
	str	r1, [sp, #12]
	bl	sub_08002096
	adds	r1, r5, #0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x9a08
	movs	r3, #2
	adds	r0, r2, r0
	str	r0, [r7, #12]
	mov	fp, r0
	movs	r0, #192
	adds	r1, r5, #0
	strb	r3, [r7, #16]
	lsls	r0, r0, #11
	bl	sub_080148e8
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	add	sl, r0
	lsls	r0, r0, #2
	adds	r5, r5, r0
	b.n	.L_080da8ca
.L_080da82e:
	ldr	r1, [sp, #16]
	ldr	r3, [r7, #4]
	subs	r3, r1, r3
	asrs	r2, r3, #8
	ldr	r1, [sp, #12]
	ldr	r3, [r7, #8]
	adds	r0, r2, #0
	subs	r3, r1, r3
	asrs	r4, r3, #8
	ldr	r3, [r7, #12]
	mov	r1, fp
	subs	r3, r1, r3
	str	r4, [sp, #0]
	adds	r1, r2, #0
	asrs	r6, r3, #8
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x9c00
	adds	r5, r0, #0
	adds	r1, r4, #0
	adds	r0, r4, #0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x1c31
	str	r0, [sp, #4]
	adds	r0, r6, #0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x9a01
	ldr	r3, [pc, #152]
	adds	r5, r5, r2
	adds	r5, r5, r0
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c05
	cmp	r5, #6
	ble.n	.L_080da8d6
	ldr	r1, [sp, #16]
	ldr	r3, [r7, #4]
	subs	r3, r3, r1
	lsls	r0, r3, #1
	adds	r0, r0, r3
	adds	r1, r5, #0
	lsls	r0, r0, #1
	bl	sub_08002054
	ldr	r2, [sp, #16]
	adds	r0, r2, r0
	str	r0, [r7, #4]
	str	r0, [sp, #16]
	ldr	r1, [sp, #12]
	ldr	r3, [r7, #8]
	subs	r3, r3, r1
	lsls	r0, r3, #1
	adds	r0, r0, r3
	adds	r1, r5, #0
	lsls	r0, r0, #1
	bl	sub_08002054
	ldr	r2, [sp, #12]
	mov	r1, fp
	adds	r0, r2, r0
	str	r0, [r7, #8]
	str	r0, [sp, #12]
	ldr	r3, [r7, #12]
	subs	r3, r3, r1
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #1
	adds	r1, r5, #0
	bl	sub_08002054
	movs	r3, #2
	add	r0, fp
	str	r0, [r7, #12]
	strb	r3, [r7, #16]
	mov	fp, r0
.L_080da8ca:
	ldr	r3, [sp, #24]
	movs	r2, #1
	add	r8, r2
	cmp	r8, r3
	bgt.n	.L_080da8d6
	b.n	.L_080da7d4
.L_080da8d6:
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #24]
	adds	r1, #1
	str	r1, [sp, #20]
	cmp	r1, r2
	bgt.n	.L_080da8ea
	b.n	.L_080da7ba
.L_080da8ea:
	ldr	r0, [sp, #8]
	bl	sub_08013164
	movs	r0, #0
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.4byte 0x030002d4
