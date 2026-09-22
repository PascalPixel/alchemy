.syntax unified
	.thumb
	.set sub_08002064, 0x08002064
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014de4, 0x08014de4
	.set sub_080156e8, 0x080156e8
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e21c, 0x0815e21c
	.set sub_081963ec, 0x081963ec
	.set sub_081c0010, 0x081c0010
	.global Overlay_0813e3d0
Overlay_0813e3d0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #246
	sub	sp, #72
	lsls	r1, r1, #7
	str	r0, [sp, #32]
	adds	r1, #124
	movs	r0, #92
	bl	sub_08014cc0
	movs	r1, #128
	mov	r9, r0
	lsls	r1, r1, #7
	movs	r0, #96
	bl	sub_08014cc0
	movs	r1, #192
	lsls	r1, r1, #3
	str	r0, [sp, #28]
	adds	r1, #14
	movs	r0, #100
	bl	sub_08014cc0
	str	r0, [sp, #16]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #48]
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #228
	str	r3, [sp, #12]
	add	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r3, #240
	ldr	r1, [sp, #32]
	lsls	r3, r3, #7
	adds	r3, #240
	add	r3, r9
	str	r1, [r3, #0]
	movs	r0, #0
	bl	sub_081435e0
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #180
	add	r2, r9
	movs	r3, #24
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #184
	add	r2, r9
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r5, [pc, #52]
	ldr	r3, [pc, #44]
	movs	r1, #224
	subs	r2, #50
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	add	r1, r9
	adds	r0, r5, #0
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #28]
	ldr	r1, [sp, #16]
	bl	sub_08157cf4
	ldr	r2, [sp, #32]
	ldr	r3, [r2, #0]
	cmp	r3, #1
	beq.n	.L_0813e4a4
	b.n	.L_0813e490
	.4byte 0x0000100c
	.4byte 0x00000100
	.4byte 0x00000119
	.2byte 0x0137
	.2byte 0x0000
.L_0813e490:
	cmp	r3, #1
	bgt.n	.L_0813e49a
	cmp	r3, #0
	beq.n	.L_0813e4a0
	b.n	.L_0813e4b4
.L_0813e49a:
	cmp	r3, #2
	beq.n	.L_0813e4a8
	b.n	.L_0813e4b4
.L_0813e4a0:
	ldr	r0, [pc, #8]
	b.n	.L_0813e4b6
.L_0813e4a4:
	adds	r0, r5, #0
	b.n	.L_0813e4b6
.L_0813e4a8:
	ldr	r0, [pc, #4]
	b.n	.L_0813e4b6
	.4byte 0x0000010a
	.2byte 0x0109
	.2byte 0x0000
.L_0813e4b4:
	ldr	r0, [pc, #264]
.L_0813e4b6:
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #260]
	lsls	r0, r0, #19
	movs	r2, #128
	ldr	r5, [pc, #256]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2300
	mov	r8, r3
.L_0813e4ce:
	movs	r3, #0
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r4, r8
	adds	r3, #255
	lsls	r2, r4, #1
	ands	r3, r0
	movs	r1, #1
	adds	r3, r3, r2
	add	r8, r1
	str	r3, [r5, #8]
	mov	r2, r8
	negs	r3, r4
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #128
	bne.n	.L_0813e4ce
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	movs	r3, #75
	add	r2, r9
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #172]
	bl	sub_080145a8
	movs	r0, #104
	movs	r1, #3
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	ldr	r4, [sp, #32]
	str	r3, [sp, #20]
	ldr	r3, [r4, #24]
	adds	r3, #1
	str	r3, [r4, #24]
	cmp	r3, #0
	bgt.n	.L_0813e544
	movs	r3, #1
	str	r3, [r4, #24]
.L_0813e544:
	ldr	r1, [sp, #32]
	ldr	r3, [r1, #24]
	cmp	r3, #4
	ble.n	.L_0813e550
	movs	r3, #4
	str	r3, [r1, #24]
.L_0813e550:
	movs	r0, #212
	bl	sub_081c0010
	ldr	r4, [sp, #12]
	movs	r3, #60
	adds	r4, #12
	str	r4, [sp, #8]
	movs	r2, #0
	add	r3, sp
	mov	sl, r2
	mov	fp, r3
.L_0813e566:
	ldr	r1, [sp, #32]
	ldr	r0, [r1, #8]
	mov	r1, fp
	bl	sub_0815e21c
	mov	r4, fp
	ldr	r3, [r4, #0]
	movs	r2, #128
	movs	r1, #64
	lsls	r2, r2, #19
	subs	r3, r1, r3
	adds	r2, #40
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	mov	r2, sl
	cmp	r2, #49
	ble.n	.L_0813e59a
	ldr	r3, [pc, #44]
	lsls	r2, r2, #1
	subs	r3, r3, r2
	ldr	r2, [pc, #44]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #82
	orrs	r3, r2
	strh	r3, [r1, #0]
.L_0813e59a:
	mov	r3, sl
	cmp	r3, #16
	bne.n	.L_0813e5d0
	ldr	r1, [sp, #32]
	movs	r3, #20
	movs	r2, #1
	movs	r4, #36
	ldrsh	r0, [r1, r4]
	negs	r2, r2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	bl	sub_0814cd48
	b.n	.L_0813e5d0
	.4byte 0x00000070
	.4byte 0x00001000
	.4byte 0x00000108
	.4byte 0x03000730
	.4byte 0x02010000
	.2byte 0x3001
	.2byte 0x0814
.L_0813e5d0:
	mov	r2, sl
	cmp	r2, #55
	ble.n	.L_0813e5d8
	b.n	.L_0813e6da
.L_0813e5d8:
	lsrs	r3, r2, #31
	add	r3, sl
	asrs	r6, r3, #1
	adds	r0, r6, #0
	cmp	r6, #0
	bge.n	.L_0813e5e6
	adds	r0, r6, #3
.L_0813e5e6:
	asrs	r5, r0, #2
	lsls	r3, r5, #2
	movs	r1, #19
	movs	r0, #188
	subs	r5, r6, r3
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	lsls	r1, r5, #4
	adds	r3, #188
	mov	r2, fp
	adds	r1, r1, r5
	ldr	r7, [r3, #0]
	lsls	r1, r1, #6
	ldr	r3, [r2, #4]
	movs	r4, #224
	lsls	r4, r4, #3
	add	r1, r9
	adds	r1, r1, r4
	movs	r2, #64
	movs	r4, #17
	str	r2, [sp, #4]
	str	r7, [sp, #24]
	subs	r3, #64
	str	r4, [sp, #0]
	ldr	r0, [sp, #28]
	movs	r2, #47
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x4650
	cmp	r0, #0
	bge.n	.L_0813e62a
	adds	r0, #3
.L_0813e62a:
	asrs	r0, r0, #2
	movs	r1, #3
	mov	r8, r0
	bl	sub_08002064
	adds	r5, r0, #0
	lsls	r1, r5, #7
	adds	r1, r1, r5
	lsls	r1, r1, #3
	movs	r3, #192
	lsls	r3, r3, #5
	mov	r4, fp
	add	r1, r9
	adds	r1, r1, r3
	ldr	r3, [r4, #4]
	movs	r2, #24
	movs	r4, #43
	str	r2, [sp, #0]
	subs	r3, #36
	movs	r2, #40
	str	r4, [sp, #4]
	ldr	r0, [sp, #28]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r1, #23
	movs	r0, #188
	bl	sub_081963ec
	movs	r1, #192
	lsls	r1, r1, #18
	adds	r1, #188
	ldr	r7, [r1, #0]
	adds	r2, r6, #0
	str	r7, [sp, #24]
	adds	r0, r2, #0
	cmp	r2, #0
	bge.n	.L_0813e67c
	adds	r0, r2, #3
.L_0813e67c:
	asrs	r5, r0, #2
	lsls	r3, r5, #2
	subs	r5, r6, r3
	lsls	r1, r5, #4
	adds	r1, r1, r5
	mov	r4, fp
	ldr	r3, [r4, #4]
	lsls	r1, r1, #6
	movs	r2, #224
	lsls	r2, r2, #3
	add	r1, r9
	movs	r4, #64
	adds	r1, r1, r2
	movs	r2, #17
	str	r2, [sp, #0]
	str	r4, [sp, #4]
	subs	r3, #64
	ldr	r0, [sp, #28]
	movs	r2, #64
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x2103
	mov	r0, r8
	bl	sub_08002064
	adds	r5, r0, #0
	lsls	r1, r5, #7
	mov	r4, fp
	adds	r1, r1, r5
	ldr	r3, [r4, #4]
	lsls	r1, r1, #3
	movs	r2, #192
	lsls	r2, r2, #5
	add	r1, r9
	adds	r1, r1, r2
	movs	r4, #43
	movs	r2, #24
	str	r2, [sp, #0]
	subs	r3, #36
	str	r4, [sp, #4]
	ldr	r0, [sp, #28]
	movs	r2, #64
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
.L_0813e6da:
	bl	sub_08014de4
	ldr	r1, [sp, #8]
	ldr	r0, [sp, #12]
	bl	sub_080156e8
	ldr	r5, [pc, #236]
	movs	r1, #0
	mov	r8, r1
	add	r7, sp, #36
	add	r6, sp, #48
.L_0813e6f0:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_0813e770
	ldr	r0, [r5, #0]
	bl	sub_08002096
	ldr	r3, [r5, #8]
	muls	r3, r0
	asrs	r3, r3, #4
	str	r3, [r7, #0]
	ldr	r0, [r5, #0]
	bl	sub_08002090
	ldr	r3, [r5, #8]
	movs	r2, #128
	muls	r3, r0
	asrs	r3, r3, #4
	negs	r3, r3
	str	r3, [r7, #8]
	lsls	r2, r2, #3
	ldr	r3, [r5, #4]
	movs	r4, #160
	str	r3, [r7, #4]
	lsls	r4, r4, #11
	ldr	r3, [r5, #0]
	adds	r1, r6, #0
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r3, [r5, #4]
	adds	r0, r7, #0
	adds	r3, r3, r4
	str	r3, [r5, #4]
	ldr	r3, [r5, #8]
	adds	r3, #64
	str	r3, [r5, #8]
	bl	sub_0815e1ec
	ldr	r2, [r6, #0]
	mov	r1, r8
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	str	r2, [r6, #0]
	ldr	r4, [sp, #32]
	movs	r0, #1
	ldr	r3, [r4, #24]
	ands	r0, r1
	adds	r0, r0, r3
	ldr	r1, [pc, #132]
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r1, r3]
	ldr	r3, [sp, #16]
	subs	r2, r2, r0
	adds	r1, r3, r1
	ldr	r3, [r6, #4]
	str	r4, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #28]
	ldr	r4, [sp, #20]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69ab
.L_0813e770:
	movs	r1, #1
	add	r8, r1
	adds	r3, #1
	mov	r2, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #32
	bne.n	.L_0813e6f0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	cmp	r4, #56
	beq.n	.L_0813e7a2
	b.n	.L_0813e566
.L_0813e7a2:
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #48]
	bl	sub_08014644
	bl	sub_08143bb8
	movs	r0, #100
	bl	sub_0801314c
	movs	r0, #96
	bl	sub_0801314c
	movs	r0, #92
	bl	sub_0801314c
	add	sp, #72
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x08197424
	.4byte 0x08143001
