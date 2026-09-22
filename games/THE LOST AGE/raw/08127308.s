.syntax unified
	.thumb
	.set sub_08013438, 0x08013438
	.set sub_08013560, 0x08013560
	.set sub_08013ba4, 0x08013ba4
	.set sub_08118410, 0x08118410
	.set sub_0811c2b4, 0x0811c2b4
	.set sub_08126218, 0x08126218
	.set sub_0812628c, 0x0812628c
	.set sub_08127068, 0x08127068
	.set sub_081272b0, 0x081272b0
	.set sub_08138020, 0x08138020
	.set sub_08138030, 0x08138030
	.set sub_08138038, 0x08138038
	.set sub_08138040, 0x08138040
	.global Overlay_08127308
Overlay_08127308:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	sub	sp, #216
	mov	r9, r0
	movs	r0, #1
	str	r3, [sp, #4]
	mov	fp, r1
	adds	r5, r2, #0
	bl	sub_08013560
	ldr	r0, [sp, #4]
	movs	r1, #206
	lsls	r1, r1, #3
	adds	r3, r0, r1
	movs	r2, #0
	ldrh	r1, [r3, #0]
	movs	r0, #1
	bl	sub_0812628c
	movs	r1, #128
	ldr	r3, [pc, #536]
	lsls	r1, r1, #7
	ldr	r0, [pc, #536]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x21dc
	movs	r0, #128
	lsls	r1, r1, #6
	lsls	r0, r0, #19
	adds	r1, #65
	bl	sub_08013ba4
	movs	r0, #128
	movs	r1, #224
	lsls	r0, r0, #19
	lsls	r1, r1, #3
	adds	r0, #12
	adds	r1, #132
	bl	sub_08013ba4
	movs	r0, #128
	movs	r1, #252
	lsls	r0, r0, #19
	lsls	r1, r1, #6
	adds	r1, #68
	adds	r0, #80
	bl	sub_08013ba4
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #240
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #64
	strh	r2, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r2, #136
	adds	r3, #4
	strh	r2, [r3, #0]
	movs	r2, #63
	adds	r3, #4
	strh	r2, [r3, #0]
	movs	r2, #17
	adds	r3, #2
	strh	r2, [r3, #0]
	cmp	r5, #0
	bne.n	.L_08127488
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #19
	lsls	r1, r1, #5
	adds	r1, #14
	adds	r0, #82
	bl	sub_08013ba4
	mov	r0, fp
	bl	sub_08127068
	movs	r3, #192
	ldr	r2, [sp, #4]
	lsls	r3, r3, #3
	adds	r3, #108
	adds	r3, r2, r3
	ldr	r0, [pc, #412]
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r7, #0
	add	r6, sp, #204
	mov	r8, r0
	mov	sl, r1
.L_081273d2:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #92]
	cmp	r7, #24
	bgt.n	.L_081273fe
	ldr	r0, [sp, #0]
	movs	r2, #128
	mov	r3, sl
	lsls	r2, r2, #9
	subs	r2, r2, r3
	str	r2, [r0, #0]
	ldr	r1, [sp, #4]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #108
	adds	r0, r1, r3
	movs	r1, #160
	lsls	r1, r1, #19
	adds	r1, #192
	movs	r3, #128
	bl	sub_08118410
.L_081273fe:
	adds	r1, r6, #0
	mov	r0, r9
	bl	sub_0811c2b4
	ldr	r3, [r6, #0]
	movs	r1, #152
	movs	r2, #64
	lsls	r1, r1, #5
	adds	r1, #196
	subs	r3, r2, r3
	adds	r0, r5, r1
	lsls	r3, r3, #8
	str	r3, [r0, #0]
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #200
	adds	r2, r5, r3
	ldr	r3, [r6, #4]
	movs	r1, #64
	subs	r3, r1, r3
	ldr	r1, [pc, #320]
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	mov	r2, r8
	ldrh	r3, [r2, #0]
	adds	r4, r3, #0
	mov	r3, r8
	strh	r3, [r2, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_0812745c
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r1
	adds	r3, #4
	adds	r2, #1
	stmia	r3!, {r0}
	strh	r2, [r1, #0]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #40
	stmia	r3!, {r2}
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #2
	str	r2, [r3, #0]
.L_0812745c:
	mov	r3, r8
	strh	r4, [r3, #0]
	movs	r0, #152
	lsls	r0, r0, #5
	adds	r0, #204
	adds	r2, r5, r0
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #68
	adds	r7, #1
	add	sl, r1
	cmp	r7, #44
	ble.n	.L_081273d2
	mov	r0, fp
	bl	sub_081272b0
	b.n	.L_0812754e
.L_08127488:
	cmp	r5, #1
	bne.n	.L_08127508
	mov	r0, fp
	bl	sub_08138038
	ldr	r0, [pc, #212]
	movs	r2, #8
	ldr	r5, [pc, #204]
	add	r2, sp
	movs	r3, #64
	mov	r8, r2
	add	r6, sp, #192
	mov	fp, r3
	mov	sl, r0
	movs	r7, #39
.L_081274a6:
	adds	r1, r6, #0
	mov	r0, r9
	bl	sub_0811c2b4
	ldr	r3, [r6, #0]
	mov	r1, fp
	subs	r3, r1, r3
	lsls	r3, r3, #8
	str	r3, [sp, #8]
	mov	r2, r8
	ldr	r3, [r6, #4]
	subs	r3, r1, r3
	lsls	r3, r3, #8
	str	r3, [r2, #4]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	mov	r3, sl
	ldrh	r2, [r3, #0]
	cmp	r2, #31
	bgt.n	.L_081274f4
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	add	r3, sl
	mov	r0, sl
	adds	r3, #4
	strh	r2, [r0, #0]
	mov	r2, r8
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #40
	stmia	r3!, {r2}
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #2
	str	r2, [r3, #0]
.L_081274f4:
	strh	r1, [r5, #0]
	movs	r0, #1
	subs	r7, #1
	bl	sub_08013560
	cmp	r7, #0
	bge.n	.L_081274a6
	bl	sub_08138040
	b.n	.L_0812754e
.L_08127508:
	cmp	r5, #2
	bne.n	.L_0812752e
	add	r0, sp, #104
	movs	r3, #0
	str	r3, [r0, #28]
	mov	r3, fp
	mov	r1, r9
	str	r3, [r0, #0]
	mov	r2, r9
	movs	r3, #1
	str	r6, [r0, #24]
	str	r1, [r0, #8]
	strh	r2, [r0, #36]
	str	r1, [r0, #12]
	str	r3, [r0, #20]
	str	r3, [r0, #16]
	bl	sub_08138020
	b.n	.L_0812754e
.L_0812752e:
	add	r0, sp, #16
	movs	r3, #0
	str	r3, [r0, #28]
	str	r3, [r0, #24]
	mov	r3, r9
	mov	r2, r9
	mov	r1, fp
	strh	r3, [r0, #36]
	movs	r3, #1
	str	r1, [r0, #0]
	str	r2, [r0, #8]
	str	r2, [r0, #12]
	str	r3, [r0, #20]
	str	r3, [r0, #16]
	bl	sub_08138030
.L_0812754e:
	add	sp, #216
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03000258
	.4byte 0x06004000
	.4byte 0x04000208
	.2byte 0x38e0
	.2byte 0x0200
	push	{lr}
	movs	r0, #0
	movs	r1, #0
	bl	sub_08126218
	pop	{pc}
	push	{lr}
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl	sub_08013438
	pop	{pc}
