.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_08014274, 0x08014274
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_0801489c, 0x0801489c
	.set sub_08014cc0, 0x08014cc0
	.set sub_0801587c, 0x0801587c
	.set sub_080cda84, 0x080cda84
	.set sub_080ce458, 0x080ce458
	.set sub_080ceafc, 0x080ceafc
	.set sub_080db9c0, 0x080db9c0
	.set sub_080db9cc, 0x080db9cc
	.set sub_080dbda8, 0x080dbda8
	.set sub_080dbdc8, 0x080dbdc8
	.set sub_080dc294, 0x080dc294
	.set sub_080dc384, 0x080dc384
	.set sub_080eaf98, 0x080eaf98
	.set sub_080eb824, 0x080eb824
	.set sub_080eb930, 0x080eb930
	.set sub_081c0010, 0x081c0010
	.global Overlay_080e1f2c
Overlay_080e1f2c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #52
	movs	r0, #92
	sub	sp, #32
	bl	sub_08014cc0
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #224
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #108]
	adds	r6, r0, #0
	mov	fp, r3
	str	r2, [sp, #16]
	adds	r7, r6, #4
	bl	sub_080dc294
	movs	r1, #0
.L_080e1f62:
	negs	r3, r1
	adds	r1, #1
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r1, #31
	ble.n	.L_080e1f62
	movs	r5, #192
	lsls	r5, r5, #5
	adds	r1, r5, #0
	movs	r0, #96
	bl	sub_08014cc0
	str	r0, [r6, #0]
	ldr	r0, [pc, #724]
	bl	sub_08013300
	ldr	r1, [r6, #0]
	bl	sub_0801587c
	bl	sub_080143ac
	str	r0, [sp, #8]
	adds	r1, r5, #0
	ldr	r2, [r6, #0]
	bl	sub_080142d4
	movs	r1, #133
	lsls	r1, r1, #3
	movs	r2, #249
	adds	r3, r6, r1
	lsls	r2, r2, #2
	adds	r7, r6, r2
	strh	r0, [r3, #0]
	movs	r3, #128
	str	r0, [sp, #0]
	lsls	r3, r3, #24
	adds	r0, r7, #0
	movs	r1, #16
	movs	r2, #16
	bl	sub_080eaf98
	ldrb	r3, [r7, #5]
	ldrb	r2, [r7, #9]
	movs	r1, #15
	movs	r0, #32
	mov	r8, r1
	orrs	r3, r0
	strb	r3, [r7, #5]
	mov	r3, r8
	ands	r3, r2
	mov	r2, fp
	strb	r3, [r7, #9]
	ldr	r0, [r2, #16]
	bl	sub_080db9c0
	movs	r1, #13
	ldrb	r2, [r7, #9]
	negs	r1, r1
	mov	r9, r1
	movs	r3, #3
	ands	r0, r3
	mov	r3, r9
	lsls	r0, r0, #2
	ands	r3, r2
	orrs	r3, r0
	mov	r2, fp
	strb	r3, [r7, #9]
	ldr	r0, [r2, #16]
	bl	sub_080db9cc
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	adds	r0, #1
	strh	r0, [r7, #30]
	str	r3, [r7, #20]
	str	r3, [r7, #24]
	mov	r0, fp
	ldrh	r3, [r0, #2]
	ldr	r2, [r0, #16]
	strh	r3, [r7, #28]
	add	r5, sp, #20
	ldr	r3, [r2, #8]
	movs	r1, #242
	str	r3, [r5, #0]
	lsls	r1, r1, #2
	ldr	r3, [r2, #16]
	movs	r0, #128
	str	r3, [r5, #8]
	mov	r2, fp
	adds	r7, r6, r1
	lsls	r0, r0, #12
	ldrh	r1, [r2, #2]
	adds	r2, r5, #0
	bl	sub_0801489c
	ldr	r3, [r5, #0]
	ldr	r0, [pc, #560]
	str	r3, [r7, #0]
	movs	r3, #128
	lsls	r3, r3, #13
	str	r3, [r7, #4]
	mov	sl, r3
	ldr	r3, [r5, #8]
	str	r3, [r7, #8]
	bl	sub_08013300
	ldr	r1, [r6, #0]
	bl	sub_0801587c
	bl	sub_080143ac
	str	r0, [sp, #12]
	movs	r1, #128
	lsls	r1, r1, #1
	ldr	r2, [r6, #0]
	bl	sub_080142d4
	movs	r1, #232
	lsls	r1, r1, #2
	adds	r7, r6, r1
	movs	r3, #128
	str	r0, [sp, #0]
	lsls	r3, r3, #23
	adds	r0, r7, #0
	movs	r1, #12
	movs	r2, #8
	bl	sub_080eaf98
	ldrb	r3, [r7, #5]
	movs	r2, #32
	orrs	r3, r2
	strb	r3, [r7, #5]
	ldrb	r3, [r7, #9]
	mov	r0, r8
	ands	r0, r3
	mov	r1, fp
	strb	r0, [r7, #9]
	mov	r8, r0
	ldr	r0, [r1, #16]
	bl	sub_080db9c0
	ldrb	r3, [r7, #9]
	movs	r2, #3
	mov	r1, r9
	ands	r0, r2
	adds	r2, r1, #0
	ands	r2, r3
	lsls	r0, r0, #2
	orrs	r2, r0
	mov	r1, fp
	strb	r2, [r7, #9]
	ldr	r0, [r1, #16]
	bl	sub_080db9cc
	movs	r2, #0
	adds	r0, #2
	strh	r0, [r7, #30]
	str	r2, [r7, #20]
	str	r2, [r7, #24]
	mov	r0, fp
	ldrh	r3, [r0, #2]
	ldr	r2, [r0, #16]
	strh	r3, [r7, #28]
	movs	r1, #225
	ldr	r3, [r2, #8]
	lsls	r1, r1, #2
	str	r3, [r5, #0]
	movs	r0, #144
	ldr	r3, [r2, #16]
	mov	r2, fp
	str	r3, [r5, #8]
	adds	r7, r6, r1
	lsls	r0, r0, #13
	ldrh	r1, [r2, #2]
	adds	r2, r5, #0
	bl	sub_0801489c
	ldr	r3, [r5, #0]
	movs	r0, #96
	str	r3, [r7, #0]
	mov	r3, sl
	str	r3, [r7, #4]
	ldr	r3, [r5, #8]
	str	r3, [r7, #8]
	bl	sub_0801314c
	bl	sub_080eb824
	movs	r3, #192
	mov	r1, fp
	lsls	r3, r3, #18
	adds	r3, #240
	ldr	r0, [r1, #16]
	ldr	r5, [r3, #0]
	bl	sub_080db9c0
	adds	r3, r5, #0
	adds	r3, #191
	mov	r2, fp
	strb	r0, [r3, #0]
	ldr	r0, [r2, #16]
	bl	sub_080db9cc
	adds	r3, r5, #0
	adds	r0, #3
	adds	r3, #190
	adds	r2, r5, #0
	strb	r0, [r3, #0]
	adds	r2, #192
	movs	r3, #2
	strb	r3, [r2, #0]
	adds	r3, r5, #0
	mov	r1, fp
	adds	r3, #193
	movs	r0, #0
	strb	r0, [r3, #0]
	ldrh	r3, [r1, #2]
	subs	r2, #8
	str	r3, [r2, #0]
	ldr	r0, [r1, #16]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #12
	adds	r2, r6, r3
	ldr	r3, [r0, #8]
	movs	r1, #130
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r3, [r0, #12]
	adds	r2, r6, r1
	str	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #20
	adds	r2, r6, r3
	ldr	r3, [r0, #16]
	adds	r1, #8
	str	r3, [r2, #0]
	adds	r2, r6, r1
	ldr	r3, [r0, #8]
	movs	r5, #248
	str	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #28
	adds	r1, r6, r3
	ldr	r3, [r0, #12]
	lsls	r5, r5, #7
	str	r3, [r1, #0]
	movs	r3, #132
	lsls	r3, r3, #3
	adds	r1, r6, r3
	ldr	r3, [r0, #16]
	movs	r0, #160
	str	r3, [r1, #0]
	mov	r3, fp
	lsls	r0, r0, #15
	ldrh	r1, [r3, #2]
	bl	sub_0801489c
	ldr	r4, [pc, #236]
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #2
.L_080e2174:
	adds	r2, r1, #0
	adds	r2, #16
	adds	r3, r0, #0
	orrs	r3, r5
	orrs	r3, r2
	strh	r3, [r4, #0]
	adds	r4, #2
	adds	r1, #1
	adds	r0, #32
	cmp	r1, #15
	ble.n	.L_080e2174
	movs	r0, #130
	bl	sub_081c0010
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #128
	lsls	r0, r0, #3
	movs	r1, #128
	adds	r0, #44
	lsls	r1, r1, #3
	movs	r2, #0
	adds	r3, r6, r0
	adds	r1, #45
	strb	r2, [r3, #0]
	adds	r0, #6
	adds	r3, r6, r1
	strb	r2, [r3, #0]
	adds	r3, r6, r0
	strb	r2, [r3, #0]
	movs	r3, #134
	lsls	r3, r3, #3
	adds	r1, r6, r3
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r1, #0]
	movs	r1, #128
	lsls	r1, r1, #3
	subs	r0, #14
	adds	r1, #38
	adds	r3, r6, r1
	adds	r5, r6, r0
	strh	r2, [r5, #0]
	ldr	r0, [pc, #144]
	strh	r2, [r3, #0]
	adds	r1, #90
	bl	sub_080145a8
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	movs	r2, #186
	lsls	r2, r2, #2
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_080e21f6
	mov	r8, r2
.L_080e21e8:
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #0
	ldrsh	r3, [r5, r0]
	cmp	r3, r8
	bne.n	.L_080e21e8
.L_080e21f6:
	ldr	r0, [pc, #104]
	bl	sub_08014644
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #50
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e222c
	movs	r2, #134
	lsls	r2, r2, #3
	adds	r3, r6, r2
	ldrh	r2, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	beq.n	.L_080e222c
	ldr	r0, [sp, #16]
	movs	r1, #211
	lsls	r1, r1, #4
	adds	r3, r0, r1
	strb	r2, [r3, #0]
.L_080e222c:
	ldr	r0, [sp, #12]
	bl	sub_08014274
	ldr	r0, [sp, #8]
	bl	sub_08014274
	bl	sub_080dc384
	bl	sub_080eb930
	movs	r0, #92
	bl	sub_0801314c
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x000001e7
	.4byte 0x000001e6
	.4byte 0x050003c0
	.2byte 0x1679
	.2byte 0x080e
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r2, r3, #0
	adds	r2, #224
	ldr	r2, [r2, #0]
	ldr	r1, [r3, #108]
	mov	fp, r2
	ldr	r3, [r3, #92]
	movs	r2, #204
	mov	r8, r1
	lsls	r2, r2, #4
	sub	sp, #44
	add	r2, r8
	str	r3, [sp, #28]
	str	r2, [sp, #24]
	movs	r1, #0
	mov	r3, fp
	ldr	r3, [r3, #16]
	ldr	r2, [sp, #28]
	str	r1, [sp, #16]
	movs	r1, #216
	lsls	r1, r1, #5
	adds	r1, #52
	mov	r9, r3
	adds	r3, r2, r1
	add	r2, sp, #16
	ldrb	r2, [r2, #0]
	mov	r1, r9
	strb	r2, [r3, #0]
	movs	r2, #208
	lsls	r2, r2, #4
	movs	r3, #255
	adds	r2, #60
	lsls	r3, r3, #8
	add	r2, r8
	adds	r3, #255
	strh	r3, [r2, #0]
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #62
	movs	r3, #1
	negs	r3, r3
	add	r2, r8
	strh	r3, [r2, #0]
	movs	r2, #212
	lsls	r2, r2, #4
	add	r2, r8
	strh	r3, [r2, #0]
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #66
	add	r2, r8
	strh	r3, [r2, #0]
	movs	r3, #0
	str	r3, [sp, #20]
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r1, #34
	adds	r3, #60
	str	r1, [sp, #8]
	add	r3, r8
	movs	r1, #0
	movs	r2, #32
	str	r3, [sp, #12]
	ldr	r7, [sp, #24]
	str	r1, [sp, #4]
	add	r2, sp
	mov	sl, r2
.L_080e22fa:
	ldr	r2, [sp, #4]
	ldr	r1, [pc, #404]
	ldrsb	r3, [r2, r1]
	mov	r1, r9
	ldr	r2, [r1, #8]
	lsls	r3, r3, #20
	adds	r5, r2, r3
	ldr	r3, [sp, #4]
	ldr	r2, [pc, #392]
	adds	r3, #1
	ldrsb	r3, [r3, r2]
	ldr	r2, [r1, #16]
	lsls	r3, r3, #20
	adds	r6, r2, r3
	ldr	r3, [sp, #8]
	adds	r1, r6, #0
	ldrb	r2, [r3, #0]
	adds	r0, r5, #0
	bl	sub_080dbda8
	mov	r1, sl
	adds	r4, r0, #0
	str	r5, [r1, #0]
	str	r6, [r1, #8]
	cmp	r4, #1
	bne.n	.L_080e237a
	mov	r3, fp
	movs	r2, #30
	ldrsh	r0, [r3, r2]
	mov	r1, r9
	mov	r2, sl
	str	r4, [sp, #0]
	bl	sub_080cda84
	movs	r1, #1
	negs	r1, r1
	ldr	r4, [sp, #0]
	cmp	r0, r1
	beq.n	.L_080e237a
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_080e2352
	ldr	r2, [pc, #328]
	adds	r3, r5, r2
.L_080e2352:
	asrs	r3, r3, #20
	strh	r3, [r7, #0]
	adds	r2, r7, #2
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_080e2362
	ldr	r1, [pc, #312]
	adds	r3, r6, r1
.L_080e2362:
	asrs	r3, r3, #20
	strh	r3, [r2, #0]
	ldr	r2, [sp, #16]
	ldr	r1, [sp, #28]
	adds	r2, #1
	str	r2, [sp, #16]
	movs	r2, #216
	lsls	r2, r2, #5
	adds	r2, #52
	adds	r3, r1, r2
	strb	r4, [r3, #0]
	adds	r7, #4
.L_080e237a:
	cmp	r4, #2
	bne.n	.L_080e23f0
	mov	r1, fp
	movs	r3, #30
	ldrsh	r0, [r1, r3]
	mov	r2, sl
	mov	r1, r9
	str	r4, [sp, #0]
	bl	sub_080cda84
	movs	r2, #1
	negs	r2, r2
	ldr	r4, [sp, #0]
	cmp	r0, r2
	beq.n	.L_080e23f0
	ldr	r1, [sp, #12]
	asrs	r3, r5, #16
	strh	r3, [r1, #0]
	movs	r1, #208
	lsls	r1, r1, #4
	adds	r1, #62
	add	r1, r8
	asrs	r3, r6, #16
	strh	r3, [r1, #0]
	ldr	r3, [sp, #12]
	ldrh	r2, [r3, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #240
	ands	r3, r2
	ldr	r2, [sp, #12]
	adds	r3, #8
	strh	r3, [r2, #0]
	movs	r3, #255
	ldrh	r2, [r1, #0]
	lsls	r3, r3, #8
	adds	r3, #240
	ands	r3, r2
	adds	r3, #8
	strh	r3, [r1, #0]
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_080e23d4
	ldr	r1, [pc, #196]
	adds	r3, r5, r1
.L_080e23d4:
	asrs	r3, r3, #20
	strh	r3, [r7, #0]
	adds	r2, r7, #2
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_080e23e4
	ldr	r1, [pc, #180]
	adds	r3, r6, r1
.L_080e23e4:
	asrs	r3, r3, #20
	strh	r3, [r2, #0]
	ldr	r2, [sp, #16]
	adds	r7, #4
	adds	r2, #1
	str	r2, [sp, #16]
.L_080e23f0:
	cmp	r4, #3
	bne.n	.L_080e2462
	mov	r1, fp
	movs	r3, #30
	ldrsh	r0, [r1, r3]
	mov	r2, sl
	mov	r1, r9
	bl	sub_080cda84
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L_080e2462
	movs	r1, #212
	movs	r0, #208
	lsls	r1, r1, #4
	lsls	r0, r0, #4
	add	r1, r8
	asrs	r3, r5, #16
	adds	r0, #66
	strh	r3, [r1, #0]
	add	r0, r8
	asrs	r3, r6, #16
	strh	r3, [r0, #0]
	movs	r3, #255
	ldrh	r2, [r1, #0]
	lsls	r3, r3, #8
	adds	r3, #240
	ands	r3, r2
	adds	r3, #8
	strh	r3, [r1, #0]
	movs	r3, #255
	ldrh	r2, [r0, #0]
	lsls	r3, r3, #8
	adds	r3, #240
	ands	r3, r2
	adds	r3, #8
	strh	r3, [r0, #0]
	adds	r0, r5, #0
	cmp	r0, #0
	bge.n	.L_080e2446
	ldr	r3, [pc, #84]
	adds	r0, r0, r3
.L_080e2446:
	asrs	r3, r0, #20
	adds	r1, r6, #0
	strh	r3, [r7, #0]
	adds	r2, r7, #2
	cmp	r1, #0
	bge.n	.L_080e2456
	ldr	r3, [pc, #68]
	adds	r1, r1, r3
.L_080e2456:
	asrs	r3, r1, #20
	strh	r3, [r2, #0]
	ldr	r1, [sp, #16]
	adds	r7, #4
	adds	r1, #1
	str	r1, [sp, #16]
.L_080e2462:
	ldr	r2, [sp, #4]
	ldr	r3, [sp, #20]
	adds	r2, #2
	adds	r3, #1
	str	r2, [sp, #4]
	str	r3, [sp, #20]
	cmp	r3, #20
	bhi.n	.L_080e2474
	b.n	.L_080e22fa
.L_080e2474:
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #24]
	lsls	r3, r1, #2
	adds	r3, r3, r2
	ldr	r2, [pc, #16]
	add	sp, #44
	strh	r2, [r3, #0]
	strh	r2, [r3, #2]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffffffff
	.4byte 0x080f0f64
	.2byte 0xffff
	.2byte 0x000f
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r1, #204
	mov	sl, r3
	ldr	r3, [r2, #108]
	lsls	r1, r1, #4
	adds	r0, r3, r1
	movs	r4, #0
	adds	r3, r0, #0
	mov	r2, sl
	mov	r8, r4
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	ldr	r1, [r2, #16]
	movs	r2, #1
	negs	r2, r2
	sub	sp, #56
	cmp	r3, r2
	beq.n	.L_080e2536
	adds	r1, #34
	add	r7, sp, #8
	mov	r9, r1
	mov	fp, r2
	adds	r6, r0, #0
	adds	r4, r7, #0
.L_080e24e2:
	movs	r1, #0
	ldrsh	r0, [r6, r1]
	movs	r2, #2
	ldrsh	r1, [r6, r2]
	mov	r3, r9
	lsls	r0, r0, #20
	lsls	r1, r1, #20
	ldrb	r2, [r3, #0]
	str	r4, [sp, #4]
	bl	sub_080dbdc8
	movs	r5, #0
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	ldr	r4, [sp, #4]
	cmp	r5, r8
	bge.n	.L_080e251e
	movs	r1, #0
	ldrsh	r3, [r7, r1]
	cmp	r3, r0
	beq.n	.L_080e251e
	adds	r2, r7, #0
.L_080e250e:
	adds	r5, #1
	cmp	r5, r8
	bge.n	.L_080e251e
	adds	r2, #2
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, r0
	bne.n	.L_080e250e
.L_080e251e:
	cmp	r5, r8
	bne.n	.L_080e252a
	adds	r5, #1
	strh	r0, [r4, #0]
	mov	r8, r5
	adds	r4, #2
.L_080e252a:
	adds	r6, #4
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, fp
	bne.n	.L_080e24e2
	b.n	.L_080e2538
.L_080e2536:
	add	r7, sp, #8
.L_080e2538:
	mov	r3, r8
	lsls	r2, r3, #1
	ldr	r3, [pc, #40]
	mov	r4, r8
	strh	r3, [r7, r2]
	cmp	r4, #1
	ble.n	.L_080e259a
	mov	r2, r8
	movs	r3, #0
	subs	r2, #1
	cmp	r3, r2
	bge.n	.L_080e259a
	mov	r9, r2
.L_080e2552:
	adds	r6, r3, #1
	cmp	r6, r8
	bge.n	.L_080e2594
	lsls	r3, r3, #1
	mov	ip, r3
	mov	r0, r8
	adds	r4, r7, #0
	mov	lr, ip
	lsls	r2, r6, #1
	subs	r5, r0, r6
	b.n	.L_080e256c
	.2byte 0xffff
	.2byte 0xffff
.L_080e256c:
	mov	r3, lr
	ldrsh	r1, [r4, r3]
	ldrh	r3, [r2, r4]
	movs	r0, #2
	add	r0, sp
	strh	r3, [r0, #0]
	ldrsh	r3, [r2, r4]
	mov	fp, r3
	cmp	r1, fp
	ble.n	.L_080e258c
	movs	r3, #2
	add	r3, sp
	ldrh	r0, [r3, #0]
	mov	r3, ip
	strh	r0, [r7, r3]
	strh	r1, [r2, r7]
.L_080e258c:
	subs	r5, #1
	adds	r2, #2
	cmp	r5, #0
	bne.n	.L_080e256c
.L_080e2594:
	adds	r3, r6, #0
	cmp	r3, r9
	blt.n	.L_080e2552
.L_080e259a:
	movs	r4, #0
	ldrsh	r3, [r7, r4]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	beq.n	.L_080e25d8
	mov	r8, r2
	adds	r6, r7, #0
	movs	r5, #0
.L_080e25ac:
	ldrsh	r2, [r5, r6]
	movs	r0, #160
	mov	r4, sl
	lsls	r0, r0, #23
	movs	r3, #30
	ldrsh	r1, [r4, r3]
	adds	r0, #5
	bl	sub_080ce458
	cmp	r0, #0
	beq.n	.L_080e25d0
	mov	r3, sl
	movs	r2, #24
	ldrsh	r1, [r3, r2]
	movs	r4, #26
	ldrsh	r2, [r3, r4]
	bl	sub_080ceafc
.L_080e25d0:
	adds	r5, #2
	ldrsh	r3, [r5, r6]
	cmp	r3, r8
	bne.n	.L_080e25ac
.L_080e25d8:
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
