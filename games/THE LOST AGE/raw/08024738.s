.syntax unified
	.thumb
	.set sub_080148e8, 0x080148e8
	.set sub_080149e0, 0x080149e0
	.global Overlay_08024738
Overlay_08024738:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	str	r3, [sp, #4]
	mov	sl, r0
	ldr	r3, [r0, #8]
	mov	r9, r1
	subs	r0, r1, r3
	mov	fp, r2
	cmp	r0, #0
	bge.n	.L_08024760
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r0, r0, r2
.L_08024760:
	mov	r2, sl
	ldr	r3, [r2, #12]
	mov	r2, fp
	asrs	r1, r0, #16
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_08024776
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r0, r0, r3
.L_08024776:
	mov	r2, sl
	ldr	r3, [r2, #16]
	ldr	r2, [sp, #4]
	asrs	r4, r0, #16
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_0802478c
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r0, r0, r3
.L_0802478c:
	adds	r3, r4, #0
	muls	r3, r4
	asrs	r7, r0, #16
	adds	r0, r1, #0
	muls	r0, r1
	adds	r2, r7, #0
	muls	r2, r7
	adds	r0, r0, r3
	adds	r0, r0, r2
	ldr	r3, [pc, #372]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2280
	lsls	r5, r0, #16
	lsls	r2, r2, #13
	cmp	r5, r2
	bge.n	.L_080247f2
	mov	r2, sl
	ldr	r3, [r2, #8]
	mov	r2, r9
	subs	r1, r2, r3
	mov	r2, sl
	ldr	r3, [r2, #12]
	mov	r2, fp
	subs	r4, r2, r3
	mov	r2, sl
	ldr	r3, [r2, #16]
	ldr	r2, [sp, #4]
	str	r4, [sp, #0]
	ldr	r6, [pc, #336]
	adds	r0, r1, #0
	subs	r7, r2, r3
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x9c00
	adds	r5, r0, #0
	adds	r1, r4, #0
	adds	r0, r4, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c39
	mov	r8, r0
	adds	r0, r7, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4445
	adds	r5, r5, r0
	adds	r0, r5, #0
	bl	sub_080149e0
	adds	r5, r0, #0
.L_080247f2:
	movs	r3, #128
	lsls	r3, r3, #9
	cmp	r5, r3
	bge.n	.L_08024816
	mov	r3, sl
	mov	r2, r9
	str	r2, [r3, #8]
	mov	r2, fp
	str	r2, [r3, #12]
	ldr	r2, [sp, #4]
	str	r2, [r3, #16]
	movs	r3, #128
	lsls	r3, r3, #24
	mov	r2, sl
	str	r3, [r2, #56]
	str	r3, [r2, #60]
	str	r3, [r2, #64]
	b.n	.L_08024906
.L_08024816:
	mov	r3, sl
	adds	r3, #88
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_08024898
	mov	r3, sl
	ldr	r1, [r3, #48]
	ldr	r3, [pc, #240]
	adds	r0, r1, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4652
	adds	r1, r0, #0
	ldr	r3, [pc, #232]
	ldr	r0, [r2, #52]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c01
	cmp	r5, r1
	ble.n	.L_08024848
	lsrs	r3, r1, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
	subs	r1, r5, r3
	b.n	.L_0802484e
.L_08024848:
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r1, r3, #1
.L_0802484e:
	ldr	r3, [pc, #204]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4653
	adds	r6, r0, #0
	ldr	r0, [r3, #8]
	mov	r2, r9
	ldr	r5, [pc, #184]
	subs	r0, r2, r0
	adds	r1, r6, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4652
	ldr	r3, [r2, #8]
	adds	r1, r6, #0
	adds	r3, r3, r0
	ldr	r0, [r2, #12]
	mov	r9, r3
	mov	r3, fp
	subs	r0, r3, r0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4652
	ldr	r3, [r2, #12]
	adds	r1, r6, #0
	adds	r3, r3, r0
	mov	fp, r3
	ldr	r0, [r2, #16]
	ldr	r3, [sp, #4]
	subs	r0, r3, r0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4652
	ldr	r3, [r2, #16]
	adds	r3, r3, r0
	str	r3, [sp, #4]
.L_08024898:
	mov	r2, sl
	mov	r3, r9
	str	r3, [r2, #56]
	mov	r3, fp
	str	r3, [r2, #60]
	ldr	r3, [sp, #4]
	str	r3, [r2, #64]
	ldr	r3, [r2, #8]
	mov	r2, r9
	subs	r1, r2, r3
	mov	r2, sl
	ldr	r3, [r2, #12]
	mov	r2, fp
	subs	r4, r2, r3
	mov	r2, sl
	ldr	r3, [r2, #16]
	ldr	r2, [sp, #4]
	subs	r7, r2, r3
	movs	r3, #86
	add	r3, sl
	mov	ip, r3
	mov	r2, ip
	movs	r3, #16
	strb	r3, [r2, #0]
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_080248d0
	negs	r2, r1
.L_080248d0:
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_080248d8
	negs	r3, r7
.L_080248d8:
	cmp	r2, r3
	bge.n	.L_080248e4
	movs	r3, #18
	mov	r2, ip
	strb	r3, [r2, #0]
	adds	r1, r7, #0
.L_080248e4:
	mov	r3, sl
	adds	r3, #85
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_08024906
	cmp	r1, #0
	bge.n	.L_080248f4
	negs	r1, r1
.L_080248f4:
	adds	r0, r4, #0
	cmp	r0, #0
	bge.n	.L_080248fc
	negs	r0, r0
.L_080248fc:
	cmp	r1, r0
	bge.n	.L_08024906
	movs	r3, #17
	mov	r2, ip
	strb	r3, [r2, #0]
.L_08024906:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x030002d4
	.4byte 0x0300021c
	.2byte 0x0230
	.2byte 0x0300
.L_08024920:
	push	{lr}
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r2, r0, r3
.L_08024928:
	ldrh	r3, [r0, #2]
	cmp	r3, #0
	beq.n	.L_08024930
	stmia	r1!, {r0}
.L_08024930:
	adds	r0, #128
	cmp	r0, r2
	bne.n	.L_08024928
	movs	r3, #0
	str	r3, [r1, #0]
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #20]
	sub	sp, #72
	add	r5, sp, #12
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	.L_08024920
	ldr	r6, [r5, #0]
	mov	r0, sp
	adds	r0, #16
	str	r0, [sp, #8]
	cmp	r6, #0
	bne.n	.L_0802496a
	b.n	.L_08024c2e
.L_0802496a:
	ldr	r3, [r6, #12]
	ldr	r2, [r6, #8]
	str	r3, [sp, #4]
	adds	r3, r6, #0
	adds	r3, #97
	ldr	r0, [r6, #16]
	ldrb	r3, [r3, #0]
	mov	fp, r2
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_08024982
	b.n	.L_08024b32
.L_08024982:
	movs	r2, #0
	str	r2, [sp, #0]
	movs	r0, #128
	ldr	r3, [r6, #56]
	lsls	r0, r0, #24
	cmp	r3, r0
	beq.n	.L_08024a7e
	mov	r2, fp
	subs	r0, r3, r2
	cmp	r0, #0
	bge.n	.L_080249a0
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r0, r0, r3
.L_080249a0:
	ldr	r3, [r6, #64]
	mov	r2, r9
	asrs	r7, r0, #16
	subs	r0, r3, r2
	cmp	r0, #0
	bge.n	.L_080249b4
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r0, r0, r3
.L_080249b4:
	asrs	r0, r0, #16
	mov	r8, r0
	mov	r2, r8
	mov	r3, r8
	muls	r3, r2
	adds	r0, r7, #0
	muls	r0, r7
	adds	r0, r0, r3
	ldr	r3, [pc, #628]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4a9d
	lsls	r0, r0, #16
	cmp	r0, r2
	bgt.n	.L_08024a02
	ldr	r3, [r6, #56]
	mov	r0, fp
	subs	r7, r3, r0
	ldr	r3, [r6, #64]
	mov	r2, r9
	subs	r2, r3, r2
	adds	r1, r7, #0
	ldr	r3, [pc, #608]
	adds	r0, r7, #0
	mov	r8, r2
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4641
	adds	r5, r0, #0
	ldr	r2, [pc, #596]
	mov	r0, r8
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x182d
	adds	r0, r5, #0
	ldr	r3, [pc, #576]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x0200
.L_08024a02:
	cmp	r0, #0
	bne.n	.L_08024a10
	ldr	r0, [r6, #56]
	ldr	r2, [r6, #64]
	mov	fp, r0
	mov	r9, r2
	b.n	.L_08024ae4
.L_08024a10:
	ldr	r3, [pc, #564]
	ldr	r1, [r6, #52]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c05
	ldr	r0, [pc, #552]
	adds	r1, r5, #0
	mov	sl, r0
	adds	r0, r7, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x6a73
	adds	r1, r5, #0
	adds	r3, r3, r0
	str	r3, [r6, #36]
	mov	r0, r8
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x6af3
	ldr	r7, [r6, #36]
	adds	r3, r3, r0
	str	r3, [r6, #44]
	adds	r1, r7, #0
	adds	r0, r7, #0
	mov	r8, r3
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x4641
	adds	r5, r0, #0
	mov	r0, r8
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x182d
	adds	r0, r5, #0
	ldr	r2, [pc, #484]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x6b31
	lsls	r0, r0, #8
	cmp	r0, r1
	ble.n	.L_08024ae4
	ldr	r3, [pc, #484]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c05
	adds	r1, r5, #0
	adds	r0, r7, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x1c29
	str	r0, [r6, #36]
	mov	r0, r8
	mov	lr, sl
	.2byte 0xf800
	.2byte 0xe031
.L_08024a7e:
	ldr	r7, [r6, #36]
	ldr	r0, [r6, #44]
	adds	r3, r7, #0
	orrs	r3, r0
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L_08024ae4
	ldr	r2, [pc, #436]
	adds	r1, r7, #0
	mov	sl, r2
	adds	r0, r7, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x4641
	adds	r5, r0, #0
	mov	r0, r8
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x182d
	ldr	r3, [pc, #404]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x0200
	cmp	r0, #0
	beq.n	.L_08024ade
	ldr	r3, [r6, #52]
	subs	r1, r0, r3
	cmp	r1, #0
	bge.n	.L_08024ac2
	ldr	r3, [sp, #0]
	str	r3, [r6, #36]
	str	r3, [r6, #44]
	b.n	.L_08024ae4
.L_08024ac2:
	ldr	r3, [pc, #388]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c05
	adds	r1, r5, #0
	adds	r0, r7, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x1c29
	str	r0, [r6, #36]
	mov	r0, r8
	mov	lr, sl
	.2byte 0xf800
	.2byte 0xe001
.L_08024ade:
	ldr	r0, [sp, #0]
	str	r0, [r6, #36]
	str	r0, [r6, #44]
.L_08024ae4:
	adds	r3, r6, #0
	adds	r3, #85
	ldrb	r2, [r3, #0]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08024b32
	ldr	r3, [r6, #20]
	ldr	r2, [sp, #4]
	cmp	r2, r3
	ble.n	.L_08024b06
	ldr	r2, [r6, #40]
	ldr	r3, [r6, #72]
	subs	r2, r2, r3
	str	r2, [r6, #40]
	adds	r0, r2, #0
	b.n	.L_08024b34
.L_08024b06:
	ldr	r0, [r6, #40]
	cmp	r0, #0
	bge.n	.L_08024b34
	str	r3, [sp, #4]
	ldr	r3, [pc, #308]
	ldr	r1, [r6, #68]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c03
	negs	r0, r3
	adds	r1, r0, #0
	str	r0, [r6, #40]
	cmp	r1, #0
	bge.n	.L_08024b24
	adds	r1, r3, #0
.L_08024b24:
	ldr	r3, [r6, #72]
	cmp	r1, r3
	bgt.n	.L_08024b34
	movs	r3, #0
	str	r3, [r6, #40]
	movs	r0, #0
	b.n	.L_08024b34
.L_08024b32:
	ldr	r0, [r6, #40]
.L_08024b34:
	ldr	r2, [sp, #4]
	ldr	r3, [r6, #36]
	adds	r2, r2, r0
	str	r2, [sp, #4]
	add	fp, r3
	ldr	r3, [r6, #44]
	adds	r1, r6, #0
	adds	r1, #86
	add	r9, r3
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_08024b92
	cmp	r3, #17
	beq.n	.L_08024b6c
	cmp	r3, #17
	bgt.n	.L_08024b5a
	cmp	r3, #16
	beq.n	.L_08024b60
	b.n	.L_08024b92
.L_08024b5a:
	cmp	r3, #18
	beq.n	.L_08024b7a
	b.n	.L_08024b92
.L_08024b60:
	ldr	r2, [r6, #56]
	cmp	fp, r2
	beq.n	.L_08024b8e
	ldr	r3, [r6, #8]
	mov	r0, fp
	b.n	.L_08024b84
.L_08024b6c:
	ldr	r2, [r6, #60]
	ldr	r3, [sp, #4]
	cmp	r3, r2
	beq.n	.L_08024b8e
	ldr	r3, [r6, #12]
	ldr	r0, [sp, #4]
	b.n	.L_08024b84
.L_08024b7a:
	ldr	r2, [r6, #64]
	cmp	r9, r2
	beq.n	.L_08024b8e
	ldr	r3, [r6, #16]
	mov	r0, r9
.L_08024b84:
	subs	r3, r3, r2
	subs	r2, r0, r2
	eors	r3, r2
	cmp	r3, #0
	bge.n	.L_08024b92
.L_08024b8e:
	movs	r2, #1
	str	r2, [sp, #0]
.L_08024b92:
	ldr	r3, [sp, #0]
	cmp	r3, #0
	beq.n	.L_08024bce
	adds	r3, r6, #0
	adds	r3, #88
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08024bc0
	movs	r3, #0
	str	r3, [r6, #36]
	str	r3, [r6, #44]
	adds	r3, r6, #0
	adds	r3, #85
	ldr	r0, [r6, #56]
	ldr	r2, [r6, #64]
	ldrb	r3, [r3, #0]
	mov	fp, r0
	mov	r9, r2
	cmp	r3, #0
	bne.n	.L_08024bc0
	ldr	r0, [r6, #60]
	str	r0, [sp, #4]
	str	r3, [r6, #40]
.L_08024bc0:
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r6, #56]
	str	r3, [r6, #60]
	str	r3, [r6, #64]
	movs	r3, #0
	strb	r3, [r1, #0]
.L_08024bce:
	mov	r2, fp
	str	r2, [r6, #8]
	ldr	r3, [sp, #4]
	mov	r0, r9
	str	r3, [r6, #12]
	str	r0, [r6, #16]
	adds	r3, r6, #0
	adds	r3, #90
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08024c20
	ldr	r2, [r6, #36]
	ldr	r3, [r6, #44]
	mov	fp, r2
	mov	r9, r3
	cmp	r2, #0
	bne.n	.L_08024bf8
	cmp	r3, #0
	beq.n	.L_08024c20
.L_08024bf8:
	mov	r0, r9
	mov	r1, fp
	bl	sub_080148e8
	ldrh	r3, [r6, #6]
	movs	r2, #128
	subs	r0, r0, r3
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	lsls	r2, r2, #5
	cmp	r0, r2
	ble.n	.L_08024c14
	movs	r0, #128
	lsls	r0, r0, #5
.L_08024c14:
	ldr	r2, [pc, #52]
	cmp	r0, r2
	bge.n	.L_08024c1c
	ldr	r0, [pc, #48]
.L_08024c1c:
	adds	r3, r3, r0
	strh	r3, [r6, #6]
.L_08024c20:
	ldr	r0, [sp, #8]
	ldmia	r0!, {r6}
	adds	r3, r0, #0
	str	r3, [sp, #8]
	cmp	r6, #0
	beq.n	.L_08024c2e
	b.n	.L_0802496a
.L_08024c2e:
	add	sp, #72
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x030002d4
	.4byte 0x00ffffff
	.4byte 0x0300021c
	.4byte 0x03000230
	.4byte 0xfffff000
