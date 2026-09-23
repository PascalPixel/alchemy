.syntax unified
	.thumb
	.set sub_08002064, 0x08002064
	.set sub_08013560, 0x08013560
	.set sub_080143ac, 0x080143ac
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_080393fc, 0x080393fc
	.set sub_0803d3c0, 0x0803d3c0
	.set sub_0803d5c4, 0x0803d5c4
	.set sub_0803d8f0, 0x0803d8f0
	.set sub_0803d9bc, 0x0803d9bc
	.set sub_0803dab0, 0x0803dab0
	.set sub_08042010, 0x08042010
	.set sub_08042188, 0x08042188
	.set sub_08042244, 0x08042244
	.set sub_08042314, 0x08042314
	.global Func_0804e1d8
	.thumb_func
Func_0804e1d8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	movs	r3, #1
	str	r3, [sp, #4]
	movs	r2, #0
	movs	r3, #192
	mov	sl, r2
	mov	fp, r2
	mov	r9, r2
	lsls	r3, r3, #18
	add	r2, sp, #4
	ldr	r3, [r3, #24]
	ldrh	r2, [r2, #0]
	movs	r0, #1
	strh	r2, [r3, #4]
	bl	sub_08013560
.L_0804e206:
	ldr	r2, [pc, #468]
	ldr	r3, [r2, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804e21c
	movs	r2, #1
	movs	r3, #1
	str	r2, [sp, #4]
	negs	r3, r3
	add	fp, r3
.L_0804e21c:
	ldr	r2, [pc, #444]
	ldr	r3, [r2, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804e22e
	movs	r3, #1
	str	r3, [sp, #4]
	add	fp, r3
.L_0804e22e:
	ldr	r2, [pc, #428]
	ldr	r3, [r2, #12]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804e246
	movs	r2, #1
	movs	r3, #1
	str	r2, [sp, #4]
	negs	r3, r3
	add	r9, r3
.L_0804e246:
	ldr	r2, [pc, #404]
	ldr	r3, [r2, #12]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804e25a
	movs	r3, #1
	str	r3, [sp, #4]
	add	r9, r3
.L_0804e25a:
	ldr	r2, [pc, #384]
	ldr	r3, [r2, #12]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804e268
	b.n	.L_0804e3ba
.L_0804e268:
	ldr	r2, [pc, #368]
	movs	r5, #2
	ldr	r3, [r2, #12]
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_0804e276
	b.n	.L_0804e3ba
.L_0804e276:
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne.n	.L_0804e27e
	b.n	.L_0804e3b2
.L_0804e27e:
	mov	r0, fp
	movs	r2, #0
	movs	r1, #12
	adds	r0, #12
	str	r2, [sp, #4]
	bl	sub_08002064
	mov	fp, r0
	mov	r0, r9
	movs	r1, #3
	adds	r0, #3
	bl	sub_08002064
	movs	r1, #2
	mov	r9, r0
	mov	r0, sl
	bl	sub_0803939c
	movs	r3, #12
	movs	r0, #10
	movs	r1, #0
	movs	r2, #18
	str	r5, [sp, #0]
	bl	sub_08039260
	mov	r3, r9
	mov	sl, r0
	cmp	r3, #0
	bne.n	.L_0804e2bc
	ldr	r0, [pc, #292]
	b.n	.L_0804e2c4
.L_0804e2bc:
	mov	r2, r9
	cmp	r2, #1
	bne.n	.L_0804e2d0
	ldr	r0, [pc, #288]
.L_0804e2c4:
	mov	r1, sl
	movs	r2, #0
	movs	r3, #0
	bl	sub_08042188
	b.n	.L_0804e2dc
.L_0804e2d0:
	ldr	r0, [pc, #276]
	mov	r1, sl
	movs	r2, #0
	movs	r3, #0
	bl	sub_08042188
.L_0804e2dc:
	ldr	r0, [pc, #268]
	mov	r1, sl
	movs	r2, #0
	movs	r3, #8
	bl	sub_08042188
	movs	r3, #8
	str	r3, [sp, #0]
	mov	r0, fp
	movs	r1, #0
	mov	r2, sl
	movs	r3, #40
	bl	sub_08042244
	mov	r2, fp
	lsls	r2, r2, #5
	mov	r8, r2
	movs	r3, #8
	str	r3, [sp, #0]
	mov	r0, r8
	movs	r1, #3
	mov	r2, sl
	movs	r3, #64
	bl	sub_08042244
	ldr	r0, [pc, #224]
	mov	r1, sl
	movs	r2, #88
	movs	r3, #8
	bl	sub_08042188
	movs	r2, #8
	mov	r0, r8
	str	r2, [sp, #0]
	adds	r0, #31
	movs	r1, #3
	mov	r2, sl
	movs	r3, #96
	bl	sub_08042244
	movs	r5, #0
.L_0804e32e:
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #12]
	adds	r2, r5, #0
	cmp	r5, #0
	bge.n	.L_0804e33c
	adds	r2, r5, #7
.L_0804e33c:
	asrs	r2, r2, #3
	lsls	r3, r2, #3
	subs	r3, r5, r3
	lsls	r2, r2, #4
	lsls	r7, r3, #4
	adds	r6, r2, #0
	mov	r3, r9
	adds	r6, #16
	cmp	r3, #0
	bne.n	.L_0804e362
	mov	r2, r8
	adds	r0, r2, r5
	str	r3, [sp, #0]
	movs	r1, #1
	add	r2, sp, #12
	add	r3, sp, #8
	bl	sub_0803d5c4
	b.n	.L_0804e37a
.L_0804e362:
	mov	r3, r9
	cmp	r3, #1
	bne.n	.L_0804e38c
	mov	r2, r8
	movs	r3, #0
	adds	r0, r2, r5
	str	r3, [sp, #0]
	movs	r1, #1
	add	r2, sp, #12
	add	r3, sp, #8
	bl	sub_0803d9bc
.L_0804e37a:
	movs	r1, #128
	ldr	r0, [sp, #12]
	lsls	r1, r1, #23
	mov	r2, sl
	adds	r3, r7, #0
	str	r6, [sp, #0]
	bl	sub_08042314
	b.n	.L_0804e3ac
.L_0804e38c:
	bl	sub_080143ac
	movs	r1, #0
	adds	r2, r0, #0
	adds	r0, r5, #0
	str	r2, [sp, #12]
	bl	sub_0803d8f0
	movs	r1, #128
	ldr	r0, [sp, #12]
	lsls	r1, r1, #23
	mov	r2, sl
	adds	r3, r7, #0
	str	r6, [sp, #0]
	bl	sub_08042314
.L_0804e3ac:
	adds	r5, #1
	cmp	r5, #31
	ble.n	.L_0804e32e
.L_0804e3b2:
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_0804e206
.L_0804e3ba:
	mov	r0, sl
	movs	r1, #2
	bl	sub_0803939c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #24]
	movs	r3, #0
	movs	r0, #0
	strh	r3, [r2, #4]
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03001150
	.4byte 0x0805f8f0
	.4byte 0x0805f8f8
	.4byte 0x0805f900
	.4byte 0x0805f908
	.2byte 0xf910
	.2byte 0x0805
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	sub	sp, #24
	movs	r0, #1
	str	r0, [sp, #8]
	movs	r1, #0
	movs	r2, #10
	mov	fp, r3
	movs	r0, #0
	movs	r3, #5
	bl	sub_0803d3c0
	movs	r3, #2
	movs	r2, #14
	str	r0, [sp, #12]
	str	r3, [sp, #0]
	movs	r1, #10
	movs	r3, #3
	movs	r0, #10
	bl	sub_08039260
	adds	r7, r0, #0
	ldr	r0, [pc, #320]
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	movs	r2, #1
	negs	r2, r2
	movs	r1, #0
	cmp	r3, r2
	beq.n	.L_0804e450
	mov	ip, r2
	adds	r2, r0, #0
.L_0804e444:
	adds	r2, #4
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	adds	r1, #1
	cmp	r3, ip
	bne.n	.L_0804e444
.L_0804e450:
	ldr	r0, [pc, #292]
	mov	r8, r1
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	beq.n	.L_0804e470
	mov	ip, r2
	adds	r2, r0, #0
.L_0804e464:
	adds	r2, #4
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	adds	r1, #1
	cmp	r3, ip
	bne.n	.L_0804e464
.L_0804e470:
	add	r1, r8
	ldr	r6, [pc, #264]
	mov	sl, r1
	movs	r1, #2
	mov	r9, r1
.L_0804e47a:
	ldr	r3, [r6, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804e48a
	movs	r2, #1
	str	r2, [sp, #8]
	subs	r5, #1
.L_0804e48a:
	ldr	r3, [r6, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804e49a
	movs	r3, #1
	str	r3, [sp, #8]
	adds	r5, #1
.L_0804e49a:
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804e4ac
	movs	r0, #1
	str	r0, [sp, #8]
	subs	r5, #10
.L_0804e4ac:
	ldr	r3, [r6, #12]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804e4be
	movs	r1, #1
	str	r1, [sp, #8]
	adds	r5, #10
.L_0804e4be:
	ldr	r3, [r6, #12]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0804e54c
	ldr	r3, [r6, #12]
	mov	r2, r9
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0804e54c
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq.n	.L_0804e544
	movs	r0, #0
	mov	r1, sl
	str	r0, [sp, #8]
	adds	r0, r5, r1
	bl	sub_08002064
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	sub_080393fc
	cmp	r5, r8
	bge.n	.L_0804e4fa
	ldr	r2, [pc, #128]
	lsls	r3, r5, #2
	adds	r3, #2
	ldrsh	r0, [r2, r3]
	b.n	.L_0804e50a
.L_0804e4fa:
	mov	r0, r8
	subs	r2, r5, r0
	ldr	r3, [pc, #120]
	lsls	r2, r2, #2
	adds	r2, #2
	ldrsh	r3, [r3, r2]
	adds	r0, r3, #0
	adds	r0, #128
.L_0804e50a:
	movs	r1, #152
	lsls	r1, r1, #5
	mov	r2, fp
	adds	r1, #130
	ldrh	r3, [r2, r1]
	movs	r2, #15
	str	r3, [sp, #20]
	movs	r3, #1
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	add	r2, sp, #20
	add	r3, sp, #16
	movs	r1, #0
	bl	sub_0803dab0
	movs	r3, #0
	adds	r0, r5, #0
	movs	r1, #2
	adds	r2, r7, #0
	str	r3, [sp, #0]
	bl	sub_08042244
	ldr	r0, [pc, #72]
	adds	r1, r7, #0
	adds	r0, r5, r0
	movs	r2, #24
	movs	r3, #0
	bl	sub_08042010
.L_0804e544:
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_0804e47a
.L_0804e54c:
	adds	r0, r7, #0
	movs	r1, #2
	bl	sub_0803939c
	movs	r1, #2
	ldr	r0, [sp, #12]
	bl	sub_0803939c
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #0
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0805eb58
	.4byte 0x0805eb7c
	.4byte 0x03001150
	.4byte 0x00001342
