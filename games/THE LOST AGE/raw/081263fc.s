.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_080145a8, 0x080145a8
	.set sub_08014d78, 0x08014d78
	.set sub_08016ce4, 0x08016ce4
	.set sub_08118410, 0x08118410
	.set sub_08125b78, 0x08125b78
	.set sub_08125bb8, 0x08125bb8
	.global Overlay_081263fc
Overlay_081263fc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r5, #192
	lsls	r5, r5, #18
	adds	r3, r5, #0
	adds	r3, #176
	ldr	r3, [r3, #0]
	mov	fp, r0
	adds	r0, r1, #0
	adds	r7, r2, #0
	mov	r9, r3
	bl	sub_08013300
	ldr	r5, [r5, #36]
	mov	r8, r0
	mov	sl, r5
	ldr	r5, [pc, #248]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #228]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #128
	lsls	r0, r0, #1
	ldr	r1, [pc, #220]
	add	r0, r8
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c30
	bl	sub_08013164
	movs	r4, #160
	lsls	r4, r4, #3
	adds	r4, #108
	movs	r3, #128
	movs	r2, #132
	add	r4, sl
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	mov	r0, r8
	adds	r1, r4, #0
	adds	r2, #64
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	cmp	r7, #0
	blt.n	.L_081264a0
	lsls	r3, r7, #4
	adds	r3, r3, r7
	movs	r0, #192
	lsls	r3, r3, #4
	lsls	r0, r0, #3
	adds	r3, r3, r7
	movs	r2, #128
	lsls	r3, r3, #2
	movs	r1, #160
	adds	r0, #108
	lsls	r2, r2, #9
	add	r0, sl
	subs	r2, r2, r3
	lsls	r1, r1, #19
	str	r2, [r0, #0]
	adds	r1, #192
	adds	r0, r4, #0
	movs	r3, #128
	bl	sub_08118410
.L_081264a0:
	movs	r3, #237
	lsls	r3, r3, #3
	adds	r3, #255
	add	r3, sl
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_081264d2
	movs	r3, #128
	movs	r1, #160
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r1, r1, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #108]
	adds	r1, #160
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #104]
	movs	r3, #160
	ldrh	r2, [r2, #0]
	lsls	r3, r3, #19
	adds	r3, #188
	strh	r2, [r3, #0]
.L_081264d2:
	ldr	r0, [pc, #96]
	bl	sub_08125b78
	ldr	r0, [pc, #92]
	bl	sub_08125bb8
	ldr	r3, [pc, #92]
	ldr	r0, [pc, #92]
	movs	r1, #64
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x464a
	ldr	r3, [r2, #8]
	cmp	r3, #0
	bne.n	.L_081264fc
	movs	r1, #192
	lsls	r1, r1, #4
	ldr	r0, [pc, #76]
	adds	r1, #255
	bl	sub_080145a8
.L_081264fc:
	mov	r3, fp
	mov	r2, r9
	str	r3, [r2, #8]
	cmp	r3, #1
	bne.n	.L_08126510
	movs	r2, #128
	ldr	r3, [pc, #16]
	lsls	r2, r2, #19
	adds	r2, #10
	strh	r3, [r2, #0]
.L_08126510:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00001f83
	.4byte 0x00000234
	.4byte 0x08118160
	.4byte 0x06008000
	.4byte 0x05000200
	.4byte 0x050001e8
	.4byte 0x06003800
	.4byte 0x0600f800
	.4byte 0x03000258
	.4byte 0x0600ffc0
	.2byte 0x5c0d
	.2byte 0x0812
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #24
	str	r3, [sp, #8]
	movs	r3, #192
	str	r2, [sp, #12]
	str	r0, [sp, #20]
	str	r1, [sp, #16]
	lsls	r3, r3, #18
	adds	r2, r3, #0
	adds	r2, #176
	ldr	r0, [r3, #40]
	ldr	r1, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #4
	ldr	r5, [sp, #56]
	ldr	r3, [r3, #48]
	str	r2, [sp, #4]
	movs	r2, #128
	mov	fp, r0
	lsls	r2, r2, #9
	movs	r0, #0
	str	r0, [sp, #0]
	cmp	r5, r2
	blt.n	.L_0812659c
	movs	r0, #128
	lsls	r0, r0, #6
	str	r0, [sp, #0]
	movs	r0, #54
	ldrsh	r2, [r3, r0]
	negs	r2, r2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	movs	r2, #208
	lsls	r2, r2, #7
	adds	r2, r3, r2
	str	r2, [sp, #4]
.L_0812659c:
	mov	r3, fp
	cmp	r3, #0
	bne.n	.L_081265a4
	b.n	.L_081266f0
.L_081265a4:
	ldr	r0, [r1, #8]
	cmp	r0, #1
	beq.n	.L_081265b0
	ldr	r3, [r1, #12]
	cmp	r3, #1
	bne.n	.L_081265be
.L_081265b0:
	ldr	r3, [r1, #16]
	cmp	r3, #0
	bne.n	.L_081265be
	ldr	r1, [sp, #4]
	ldr	r2, [pc, #224]
	asrs	r3, r1, #8
	strh	r3, [r2, #4]
.L_081265be:
	cmp	r0, #2
	beq.n	.L_081265c4
	b.n	.L_081266f0
.L_081265c4:
	mov	r3, fp
	ldr	r2, [r3, #0]
	movs	r3, #1
	eors	r2, r3
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #6
	movs	r1, #128
	add	r3, fp
	adds	r0, r5, #0
	lsls	r1, r1, #9
	ldr	r2, [pc, #196]
	adds	r7, r3, #0
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x465e
	adds	r6, #16
	movs	r3, #0
	strh	r3, [r6, #2]
	strh	r3, [r6, #4]
	ldr	r3, [pc, #180]
	asrs	r2, r0, #8
	mov	sl, r0
	ldr	r0, [pc, #180]
	adds	r3, r3, r5
	strh	r2, [r6, #0]
	strh	r2, [r6, #6]
	mov	r9, r3
	mov	r8, r0
	mov	r1, r9
	ldr	r0, [sp, #20]
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c01
	mov	r0, sl
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x4649
	adds	r5, r0, #0
	ldr	r0, [sp, #16]
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c01
	mov	r0, sl
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x23fe
	lsls	r3, r3, #7
	ldr	r1, [sp, #12]
	adds	r3, #255
	ldr	r2, [sp, #4]
	adds	r5, r5, r3
	asrs	r5, r5, #8
	adds	r5, r5, r1
	adds	r5, r5, r2
	str	r5, [r6, #8]
	adds	r0, r0, r3
	ldr	r3, [sp, #8]
	ldr	r1, [pc, #112]
	asrs	r0, r0, #8
	adds	r0, r0, r3
	movs	r2, #6
	ldrsh	r3, [r6, r2]
	adds	r0, r0, r1
	movs	r1, #128
	lsls	r1, r1, #7
	str	r0, [r6, #12]
	subs	r1, r1, r0
	adds	r0, r3, #0
	ldr	r3, [pc, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1400
	adds	r6, r0, #1
	movs	r0, #108
	adds	r0, #255
	adds	r7, #32
	movs	r5, #0
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_08126674
	ldr	r3, [pc, #40]
.L_0812666a:
	adds	r5, #1
	strh	r3, [r7, #0]
	adds	r7, #2
	cmp	r5, #15
	bls.n	.L_0812666a
.L_08126674:
	cmp	r6, #136
	bls.n	.L_0812667a
	movs	r6, #136
.L_0812667a:
	cmp	r5, r6
	bcs.n	.L_081266b0
	ldr	r0, [sp, #0]
	ldr	r3, [pc, #20]
	lsls	r2, r0, #16
	lsrs	r2, r2, #16
	orrs	r2, r3
.L_08126688:
	adds	r5, #1
	strh	r2, [r7, #0]
	adds	r7, #2
	cmp	r5, r6
	bcc.n	.L_08126688
	b.n	.L_081266b0
	.4byte 0x00003f8e
	.4byte 0x0000478a
	.4byte 0x03001120
	.4byte 0x03000230
	.4byte 0xffff0000
	.4byte 0x0300021c
	.2byte 0xf000
	.2byte 0xffff
.L_081266b0:
	cmp	r5, #135
	bhi.n	.L_081266c8
	ldr	r1, [sp, #0]
	ldr	r3, [pc, #36]
	lsls	r2, r1, #16
	lsrs	r2, r2, #16
	orrs	r2, r3
.L_081266be:
	adds	r5, #1
	strh	r2, [r7, #0]
	adds	r7, #2
	cmp	r5, #135
	bls.n	.L_081266be
.L_081266c8:
	cmp	r5, #159
	bhi.n	.L_081266e4
	ldr	r3, [pc, #16]
.L_081266ce:
	adds	r5, #1
	strh	r3, [r7, #0]
	adds	r7, #2
	cmp	r5, #159
	bls.n	.L_081266ce
	b.n	.L_081266e4
	movs	r0, r0
	.4byte 0x0000478e
	.2byte 0x3f8e
	.2byte 0x0000
.L_081266e4:
	mov	r2, fp
	ldr	r3, [r2, #0]
	movs	r2, #1
	eors	r3, r2
	mov	r0, fp
	str	r3, [r0, #0]
.L_081266f0:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
