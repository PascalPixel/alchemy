.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_0803a084, 0x0803a084
	.set sub_0803a2b0, 0x0803a2b0
	.set sub_0803a404, 0x0803a404
	.global Func_080391c4
	.thumb_func
Func_080391c4:
.L_080391c4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r2, #12
	ldrsh	r5, [r0, r2]
	ldrh	r2, [r0, #10]
	ldr	r3, [r3, #60]
	mov	r8, r2
	ldrh	r2, [r0, #22]
	mov	sl, r3
	movs	r3, #14
	ldrsh	r6, [r0, r3]
	movs	r3, #0
	strh	r3, [r0, #26]
	movs	r3, #8
	ands	r3, r2
	sub	sp, #4
	ldrh	r7, [r0, #8]
	cmp	r3, #0
	beq.n	.L_08039236
	movs	r3, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08039216
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	mov	r3, r8
	bl	sub_0803a084
	movs	r1, #240
	ldr	r3, [pc, #72]
	ldr	r0, [pc, #76]
	lsls	r1, r1, #4
	ldr	r2, [pc, #76]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe006
.L_08039216:
	movs	r1, #240
	ldr	r3, [pc, #56]
	ldr	r0, [pc, #60]
	lsls	r1, r1, #4
	movs	r2, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2300
	str	r3, [sp, #0]
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	mov	r3, r8
	bl	sub_0803a2b0
	b.n	.L_08039242
.L_08039236:
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	mov	r3, r8
	bl	sub_0803a084
.L_08039242:
	movs	r3, #1
	mov	r2, sl
	strb	r3, [r2, #3]
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03000260
	.4byte 0x06002500
	.2byte 0x4444
	.2byte 0x4444
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	lr, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	mov	ip, r2
	movs	r2, #161
	lsls	r2, r2, #3
	adds	r4, r3, r2
	ldrh	r2, [r4, #22]
	movs	r3, #1
	ands	r3, r2
	adds	r7, r1, #0
	ldr	r6, [sp, #20]
	movs	r5, #0
	movs	r1, #0
	b.n	.L_08039294
.L_08039286:
	adds	r1, #1
	adds	r4, #36
	cmp	r1, #12
	beq.n	.L_080392a2
	ldrh	r2, [r4, #22]
	movs	r3, #1
	ands	r3, r2
.L_08039294:
	cmp	r3, #0
	bne.n	.L_08039286
	movs	r2, #26
	ldrsh	r3, [r4, r2]
	cmp	r3, #0
	bne.n	.L_08039286
	adds	r5, r4, #0
.L_080392a2:
	cmp	r5, #0
	beq.n	.L_0803936c
	movs	r3, #0
	mov	r8, r3
	mov	r2, ip
	mov	r3, lr
	strh	r7, [r5, #14]
	strh	r2, [r5, #8]
	strh	r3, [r5, #10]
	mov	r2, r8
	mov	r3, r8
	movs	r7, #1
	strh	r0, [r5, #12]
	strh	r3, [r5, #20]
	str	r2, [r5, #0]
	str	r4, [r5, #4]
	strh	r7, [r5, #16]
	strh	r7, [r5, #22]
	bl	sub_0803a404
	movs	r0, #8
	adds	r3, r6, #0
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080392dc
	ldrh	r3, [r5, #22]
	ldr	r2, [pc, #40]
	orrs	r3, r2
	strh	r3, [r5, #22]
.L_080392dc:
	movs	r3, #32
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_080392ec
	ldrh	r3, [r5, #22]
	ldr	r2, [pc, #28]
	orrs	r3, r2
	strh	r3, [r5, #22]
.L_080392ec:
	movs	r3, #64
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_0803930c
	ldrh	r3, [r5, #22]
	ldr	r2, [pc, #16]
	orrs	r3, r2
	strh	r3, [r5, #22]
	b.n	.L_0803930c
	movs	r0, r0
	.4byte 0x00000008
	.4byte 0x00000020
	.2byte 0x0040
	.2byte 0x0000
.L_0803930c:
	movs	r3, #128
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_0803931c
	ldrh	r3, [r5, #22]
	ldr	r2, [pc, #48]
	orrs	r3, r2
	strh	r3, [r5, #22]
.L_0803931c:
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r3, r6, #0
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08039330
	ldrh	r2, [r5, #22]
	adds	r3, r1, #0
	orrs	r3, r2
	strh	r3, [r5, #22]
.L_08039330:
	movs	r3, #2
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_0803935a
	ldrh	r3, [r5, #22]
	ldr	r2, [pc, #16]
	strh	r7, [r5, #26]
	orrs	r3, r2
	mov	r2, r8
	strh	r3, [r5, #22]
	b.n	.L_08039350
	movs	r0, r0
	.4byte 0x00000080
	.2byte 0x0002
	.2byte 0x0000
.L_08039350:
	strh	r2, [r5, #24]
	adds	r0, r5, #0
	bl	.L_080391c4
	b.n	.L_0803936c
.L_0803935a:
	movs	r3, #7
	strh	r0, [r5, #26]
	strh	r3, [r5, #24]
	adds	r0, r5, #0
	bl	.L_08039374
	movs	r0, #1
	bl	sub_08013560
.L_0803936c:
	adds	r0, r5, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
.L_08039374:
	push	{r5, lr}
	adds	r5, r0, #0
	ldrh	r2, [r5, #22]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08039398
	movs	r2, #26
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L_08039398
.L_0803938a:
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #26
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	bne.n	.L_0803938a
.L_08039398:
	pop	{r5, pc}
	.align 2, 0
