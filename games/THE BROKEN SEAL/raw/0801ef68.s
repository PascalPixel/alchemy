.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080045e8, 0x080045e8
	.set sub_08016178, 0x08016178
	.set sub_08016498, 0x08016498
	.set sub_080170f8, 0x080170f8
	.set sub_08018efc, 0x08018efc
	.set sub_08019000, 0x08019000
	.set sub_0801e71c, 0x0801e71c
	.set sub_0801e8b0, 0x0801e8b0
	.set sub_0801ea3c, 0x0801ea3c
	.set sub_0801eea0, 0x0801eea0
	.set sub_08077008, 0x08077008
	.set sub_08077148, 0x08077148
	.set sub_080b50c8, 0x080b50c8
	.set sub_080b5130, 0x080b5130
	.global Overlay_0801ef68
Overlay_0801ef68:
.L_0801ef68:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #124]
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	sub	sp, #4
	mov	r9, r3
	movs	r3, #0
	str	r3, [sp, #0]
	ldrh	r3, [r6, #8]
	subs	r3, #1
	movs	r2, #1
	mov	fp, r3
	adds	r3, r1, #0
	ands	r3, r2
	ldrh	r7, [r6, #10]
	cmp	r3, #0
	bne.n	.L_0801ef9c
	movs	r3, #3
	negs	r3, r3
	ands	r1, r3
.L_0801ef9c:
	movs	r3, #2
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0801efaa
	movs	r2, #5
	str	r2, [sp, #0]
	movs	r2, #0
.L_0801efaa:
	ldr	r1, [pc, #76]
	adds	r5, r2, #0
	b.n	.L_0801f012
.L_0801efb0:
	ldrsb	r3, [r2, r5]
	ldr	r2, [sp, #0]
	adds	r0, r3, r2
	cmp	r0, fp
	bcs.n	.L_0801f010
	movs	r4, #0
	cmp	r7, #0
	beq.n	.L_0801f010
	ldr	r3, [pc, #56]
	subs	r2, r7, #1
	mov	ip, r2
	ldr	r2, [pc, #56]
	mov	sl, r3
	adds	r3, #1
	mov	r8, r3
	mov	lr, r2
.L_0801efd0:
	ldrh	r2, [r6, #14]
	ldrh	r3, [r6, #12]
	adds	r2, r2, r4
	adds	r3, r3, r0
	lsls	r2, r2, #5
	adds	r2, r2, r3
	lsls	r2, r2, #1
	mov	r3, r9
	adds	r1, r2, r3
	cmp	r4, #0
	bne.n	.L_0801efea
	mov	r2, sl
	b.n	.L_0801f006
.L_0801efea:
	cmp	r4, ip
	bne.n	.L_0801f004
	mov	r3, r8
	strh	r3, [r1, #0]
	b.n	.L_0801f008
	.4byte 0x03001e8c
	.4byte 0x080371c4
	.4byte 0x0000f018
	.2byte 0xf00f
	.2byte 0x0000
.L_0801f004:
	mov	r2, lr
.L_0801f006:
	strh	r2, [r1, #0]
.L_0801f008:
	adds	r4, #1
	cmp	r4, r7
	bne.n	.L_0801efd0
	ldr	r1, [pc, #92]
.L_0801f010:
	adds	r5, #1
.L_0801f012:
	adds	r2, r1, #0
	ldrsb	r3, [r2, r5]
	cmp	r3, #0
	bge.n	.L_0801efb0
	ldr	r3, [pc, #84]
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0801f054
	ldrh	r3, [r6, #10]
	ldrh	r2, [r6, #14]
	adds	r2, r2, r3
	ldrh	r3, [r6, #12]
	lsls	r2, r2, #6
	lsls	r3, r3, #1
	add	r2, r9
	adds	r2, r2, r3
	adds	r1, r2, #0
	ldr	r3, [pc, #40]
	subs	r1, #64
	movs	r0, #1
	strh	r3, [r1, #0]
	adds	r1, #2
	cmp	r0, fp
	bcs.n	.L_0801f050
	ldr	r3, [pc, #28]
.L_0801f046:
	adds	r0, #1
	strh	r3, [r1, #0]
	adds	r1, #2
	cmp	r0, fp
	bcc.n	.L_0801f046
.L_0801f050:
	ldr	r3, [pc, #20]
	strh	r3, [r1, #0]
.L_0801f054:
	ldr	r2, [pc, #28]
	movs	r3, #1
	add	r2, r9
	strb	r3, [r2, #0]
	add	sp, #4
	b.n	.L_0801f078
	.4byte 0x0000f080
	.4byte 0x0000f081
	.4byte 0x0000f082
	.4byte 0x080371c4
	.4byte 0x00000ea5
	.2byte 0x0ea3
	.2byte 0x0000
.L_0801f078:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
.L_0801f088:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	str	r1, [sp, #16]
	mov	r9, r3
	ldr	r3, [pc, #300]
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	ldr	r1, [pc, #300]
	mov	r0, r9
	str	r3, [sp, #8]
	str	r0, [sp, #4]
	adds	r3, r3, r1
	ldrb	r3, [r3, #0]
	adds	r5, r2, #0
	cmp	r3, #0
	bne.n	.L_0801f0ca
	bl	sub_080045e8
	ldr	r3, [pc, #280]
	ldr	r1, [pc, #284]
	ldr	r2, [pc, #284]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #284]
	ldr	r2, [pc, #284]
	ldrh	r3, [r3, #0]
	strh	r3, [r2, #0]
.L_0801f0ca:
	ldrh	r3, [r6, #12]
	ldr	r2, [sp, #16]
	adds	r2, r2, r3
	str	r2, [sp, #16]
	ldrh	r3, [r6, #14]
	adds	r5, r5, r3
	lsls	r5, r5, #5
	movs	r3, #4
	str	r5, [sp, #0]
	str	r3, [sp, #12]
.L_0801f0de:
	ldr	r0, [sp, #0]
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #8]
	adds	r3, r0, r1
	lsls	r3, r3, #1
	ldrh	r3, [r2, r3]
	mov	sl, r3
	ldr	r3, [pc, #248]
	mov	r8, r3
	ldr	r3, [pc, #248]
	mov	r1, sl
	ldr	r0, [pc, #248]
	ands	r1, r3
	mov	r2, r9
	mov	ip, r0
	mov	sl, r1
	cmp	r2, #7
	ble.n	.L_0801f10a
	ldr	r3, [pc, #240]
	ldr	r0, [pc, #240]
	mov	r8, r3
	b.n	.L_0801f134
.L_0801f10a:
	mov	r1, r9
	cmp	r1, #0
	blt.n	.L_0801f136
	lsls	r1, r1, #2
	mov	r2, r8
	lsls	r2, r1
	mov	r8, r2
	ldr	r3, [pc, #216]
	movs	r2, #32
	subs	r2, r2, r1
	lsrs	r3, r2
	mov	r0, r8
	orrs	r0, r3
	mov	r3, ip
	lsls	r3, r1
	mov	ip, r3
	ldr	r3, [pc, #204]
	mov	r8, r0
	lsrs	r3, r2
	mov	r0, ip
	orrs	r0, r3
.L_0801f134:
	mov	ip, r0
.L_0801f136:
	ldr	r2, [pc, #196]
	movs	r1, #0
	mov	lr, r1
	mov	fp, r2
	movs	r7, #0
	b.n	.L_0801f190
.L_0801f142:
	mov	r3, sl
	lsls	r6, r3, #5
	mov	r0, fp
	subs	r3, r6, r7
	ldr	r4, [r3, r0]
	movs	r1, #0
	movs	r0, #0
	movs	r5, #15
.L_0801f152:
	adds	r2, r4, #0
	ands	r2, r5
	cmp	r2, #14
	bne.n	.L_0801f164
	lsls	r2, r1, #2
	adds	r3, r5, #0
	lsls	r3, r2
	mov	r2, r8
	b.n	.L_0801f170
.L_0801f164:
	cmp	r2, #1
	bne.n	.L_0801f176
	lsls	r2, r1, #2
	adds	r3, r5, #0
	lsls	r3, r2
	mov	r2, ip
.L_0801f170:
	ands	r3, r2
	orrs	r0, r3
	b.n	.L_0801f17c
.L_0801f176:
	lsls	r3, r1, #2
	lsls	r2, r3
	orrs	r0, r2
.L_0801f17c:
	adds	r1, #1
	lsrs	r4, r4, #4
	cmp	r1, #7
	ble.n	.L_0801f152
	subs	r3, r6, r7
	mov	r1, fp
	movs	r2, #1
	str	r0, [r3, r1]
	adds	r7, #4
	add	lr, r2
.L_0801f190:
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq.n	.L_0801f19e
	mov	r0, lr
	cmp	r0, #2
	ble.n	.L_0801f142
	b.n	.L_0801f1a4
.L_0801f19e:
	mov	r1, lr
	cmp	r1, #0
	ble.n	.L_0801f142
.L_0801f1a4:
	ldr	r3, [sp, #12]
	ldr	r0, [sp, #16]
	movs	r2, #8
	negs	r2, r2
	subs	r3, #1
	adds	r0, #1
	add	r9, r2
	str	r3, [sp, #12]
	str	r0, [sp, #16]
	cmp	r3, #0
	bge.n	.L_0801f0de
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e8c
	.4byte 0x00000ea5
	.4byte 0x040000d4
	.4byte 0x050001c0
	.4byte 0x80000010
	.4byte 0x050001e8
	.4byte 0x050001dc
	.4byte 0x22222222
	.4byte 0x000003ff
	.4byte 0xcccccccc
	.4byte 0x88888888
	.4byte 0xdddddddd
	.2byte 0x001c
	.2byte 0x0600
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #172]
	ldr	r6, [r3, #0]
	adds	r2, r3, #0
	subs	r3, #4
	ldr	r3, [r3, #0]
	sub	sp, #52
	subs	r2, #28
	movs	r5, #0
	ldr	r1, [pc, #160]
	ldr	r7, [r2, #0]
	mov	fp, r0
	ldr	r0, [r6, #0]
	str	r3, [sp, #32]
	str	r5, [sp, #24]
	adds	r3, r3, r1
	ldrb	r3, [r3, #0]
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L_0801f288
	movs	r0, #0
	bl	sub_080b50c8
	movs	r2, #1
	movs	r3, #0
	negs	r2, r2
	mov	sl, r3
	str	r0, [sp, #28]
	str	r2, [sp, #24]
	cmp	sl, r0
	bcs.n	.L_0801f2d8
	mov	r4, sp
	adds	r4, #40
	str	r4, [sp, #16]
	movs	r3, #88
	ldrh	r3, [r7, r3]
	adds	r0, r4, #0
	movs	r1, #255
	strh	r3, [r0, #0]
	lsls	r1, r1, #16
	lsls	r3, r3, #16
	cmp	r3, r1
	beq.n	.L_0801f2d8
	adds	r2, r7, #0
	ldr	r0, [sp, #16]
	adds	r2, #88
	movs	r1, #0
.L_0801f26a:
	movs	r3, #1
	ldr	r4, [sp, #28]
	add	sl, r3
	adds	r1, #2
	cmp	sl, r4
	bcs.n	.L_0801f2d8
	adds	r2, #2
	ldrh	r3, [r2, #0]
	movs	r4, #255
	strh	r3, [r1, r0]
	lsls	r4, r4, #16
	lsls	r3, r3, #16
	cmp	r3, r4
	bne.n	.L_0801f26a
	b.n	.L_0801f2d8
.L_0801f288:
	bl	sub_08077148
	str	r0, [sp, #28]
	ldr	r1, [sp, #28]
	movs	r0, #0
	mov	sl, r0
	cmp	sl, r1
	bcs.n	.L_0801f2c8
	mov	r2, sp
	ldr	r3, [pc, #40]
	movs	r4, #252
	adds	r2, #40
	lsls	r4, r4, #1
	str	r2, [sp, #16]
	adds	r1, r2, #0
	adds	r2, r3, r4
.L_0801f2a8:
	ldrb	r3, [r2, #0]
	strh	r3, [r1, #0]
	movs	r0, #1
	ldr	r3, [sp, #28]
	add	sl, r0
	adds	r2, #1
	adds	r1, #2
	cmp	sl, r3
	bcc.n	.L_0801f2a8
	b.n	.L_0801f2ce
	.4byte 0x03001e90
	.4byte 0x00000ea5
	.2byte 0x0240
	.2byte 0x0200
.L_0801f2c8:
	mov	r4, sp
	adds	r4, #40
	str	r4, [sp, #16]
.L_0801f2ce:
	mov	r0, sl
	ldr	r3, [pc, #48]
	ldr	r1, [sp, #16]
	lsls	r2, r0, #1
	strh	r3, [r1, r2]
.L_0801f2d8:
	movs	r3, #1
	mov	r2, sl
	negs	r3, r3
	str	r2, [sp, #28]
	cmp	fp, r3
	bne.n	.L_0801f2e8
	ldrh	r4, [r6, #12]
	mov	fp, r4
.L_0801f2e8:
	movs	r3, #1
	mov	r0, fp
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_0801f2fa
	movs	r3, #3
	negs	r3, r3
	ands	r0, r3
	mov	fp, r0
.L_0801f2fa:
	ldr	r1, [sp, #32]
	ldr	r2, [pc, #8]
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	b.n	.L_0801f30c
	.4byte 0x000000ff
	.2byte 0x0ea5
	.2byte 0x0000
.L_0801f30c:
	cmp	r3, #0
	beq.n	.L_0801f31c
	movs	r0, #0
	movs	r1, #0
	bl	sub_080b5130
	cmp	r0, #0
	bne.n	.L_0801f326
.L_0801f31c:
	movs	r3, #3
	mov	r4, fp
	negs	r3, r3
	ands	r4, r3
	mov	fp, r4
.L_0801f326:
	mov	r0, fp
	cmp	r0, #9
	bne.n	.L_0801f33a
	ldrh	r0, [r6, #4]
	ldrh	r1, [r6, #6]
	ldrh	r2, [r6, #8]
	ldrh	r3, [r6, #10]
	bl	sub_08016178
	b.n	.L_0801f5a6
.L_0801f33a:
	ldr	r1, [sp, #32]
	ldr	r2, [pc, #632]
	adds	r3, r1, r2
	movs	r2, #1
	strb	r2, [r3, #0]
	ldrh	r3, [r6, #12]
	cmp	r3, fp
	bne.n	.L_0801f35a
	mov	r0, r8
	bl	sub_08016498
	mov	r0, r8
	mov	r1, fp
	bl	.L_0801ef68
	b.n	.L_0801f392
.L_0801f35a:
	ldrh	r1, [r6, #6]
	ldrh	r2, [r6, #8]
	ldrh	r3, [r6, #10]
	ldrh	r0, [r6, #4]
	bl	sub_08016178
	mov	r0, fp
	bl	sub_0801eea0
	ldrh	r3, [r6, #8]
	mov	r4, r8
	strh	r3, [r4, #8]
	ldrh	r3, [r6, #10]
	mov	r0, r8
	strh	r3, [r0, #10]
	ldrh	r3, [r6, #4]
	mov	r1, r8
	strh	r3, [r1, #12]
	ldrh	r0, [r6, #4]
	ldrh	r1, [r6, #6]
	ldrh	r2, [r6, #8]
	ldrh	r3, [r6, #10]
	bl	sub_080170f8
	mov	r0, r8
	mov	r1, fp
	bl	.L_0801ef68
.L_0801f392:
	movs	r3, #2
	mov	r2, fp
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0801f39e
	movs	r5, #5
.L_0801f39e:
	ldr	r4, [sp, #28]
	movs	r3, #0
	mov	sl, r3
	cmp	r4, #0
	bne.n	.L_0801f3aa
	b.n	.L_0801f4ea
.L_0801f3aa:
	ldr	r1, [sp, #24]
	mov	r0, sp
	lsls	r1, r1, #3
	adds	r2, r5, #1
	adds	r0, #40
	lsls	r5, r5, #3
	str	r0, [sp, #16]
	str	r1, [sp, #12]
	str	r1, [sp, #20]
	str	r2, [sp, #8]
	str	r3, [sp, #4]
	mov	r9, r5
.L_0801f3c2:
	ldr	r3, [sp, #4]
	ldr	r4, [sp, #16]
	ldrh	r0, [r3, r4]
	bl	sub_08077008
	adds	r5, r0, #0
	movs	r0, #56
	ldrsh	r7, [r5, r0]
	movs	r1, #52
	ldrsh	r3, [r5, r1]
	cmp	r7, #0
	bne.n	.L_0801f3e2
	movs	r0, #2
	bl	sub_0801e71c
	b.n	.L_0801f3fc
.L_0801f3e2:
	cmp	r3, #0
	bge.n	.L_0801f3e8
	adds	r3, #3
.L_0801f3e8:
	asrs	r3, r3, #2
	cmp	r7, r3
	bgt.n	.L_0801f3f6
	movs	r0, #4
	bl	sub_0801e71c
	b.n	.L_0801f3fc
.L_0801f3f6:
	movs	r0, #15
	bl	sub_0801e71c
.L_0801f3fc:
	ldr	r2, [sp, #32]
	ldr	r3, [pc, #444]
	adds	r6, r2, r3
	movs	r3, #14
	strb	r3, [r6, #0]
	ldr	r4, [sp, #32]
	ldr	r0, [pc, #436]
	adds	r3, r4, r0
	ldrb	r3, [r3, #0]
	movs	r2, #0
	cmp	r3, #0
	beq.n	.L_0801f418
	movs	r3, #5
	strb	r3, [r6, #0]
.L_0801f418:
	ldr	r3, [sp, #20]
	str	r2, [sp, #0]
	mov	r1, r8
	mov	r2, r9
	adds	r3, #8
	adds	r0, r7, #0
	bl	sub_0801ea3c
	movs	r3, #15
	strb	r3, [r6, #0]
	mov	r1, r8
	mov	r2, r9
	adds	r0, r5, #0
	ldr	r3, [sp, #20]
	bl	sub_0801e8b0
	movs	r0, #15
	bl	sub_0801e71c
	movs	r2, #52
	ldrsh	r1, [r5, r2]
	cmp	r1, #0
	beq.n	.L_0801f46c
	movs	r3, #56
	ldrsh	r6, [r5, r3]
	lsls	r0, r6, #2
	adds	r0, r0, r6
	lsls	r0, r0, #3
	bl	sub_080022ec
	adds	r3, r0, #0
	cmp	r3, #0
	bne.n	.L_0801f460
	cmp	r6, #0
	beq.n	.L_0801f460
	movs	r3, #1
.L_0801f460:
	ldr	r2, [sp, #24]
	mov	r0, r8
	ldr	r1, [sp, #8]
	adds	r2, #2
	bl	.L_0801f088
.L_0801f46c:
	movs	r1, #1
	mov	r3, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0801f4ce
	ldr	r4, [sp, #32]
	ldr	r0, [pc, #320]
	movs	r3, #14
	adds	r2, r4, r0
	strb	r3, [r2, #0]
	subs	r0, #2
	adds	r3, r4, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0801f48e
	movs	r3, #5
	strb	r3, [r2, #0]
.L_0801f48e:
	ldr	r3, [sp, #12]
	movs	r2, #58
	ldrsh	r0, [r5, r2]
	adds	r3, #16
	str	r1, [sp, #0]
	mov	r2, r9
	mov	r1, r8
	bl	sub_0801ea3c
	movs	r3, #54
	ldrsh	r1, [r5, r3]
	cmp	r1, #0
	beq.n	.L_0801f4ce
	movs	r4, #58
	ldrsh	r6, [r5, r4]
	lsls	r0, r6, #2
	adds	r0, r0, r6
	lsls	r0, r0, #3
	bl	sub_080022ec
	adds	r3, r0, #0
	cmp	r3, #0
	bne.n	.L_0801f4c2
	cmp	r6, #0
	beq.n	.L_0801f4c2
	movs	r3, #1
.L_0801f4c2:
	ldr	r2, [sp, #24]
	mov	r0, r8
	ldr	r1, [sp, #8]
	adds	r2, #3
	bl	.L_0801f088
.L_0801f4ce:
	ldr	r0, [sp, #8]
	ldr	r2, [sp, #4]
	movs	r3, #1
	ldr	r4, [sp, #28]
	adds	r0, #6
	movs	r1, #48
	adds	r2, #2
	add	sl, r3
	str	r0, [sp, #8]
	add	r9, r1
	str	r2, [sp, #4]
	cmp	sl, r4
	beq.n	.L_0801f4ea
	b.n	.L_0801f3c2
.L_0801f4ea:
	ldr	r0, [sp, #32]
	ldr	r1, [pc, #204]
	movs	r3, #0
	adds	r2, r0, r1
	mov	sl, r3
	ldr	r4, [pc, #200]
	movs	r3, #15
	strb	r3, [r2, #0]
	adds	r3, r0, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0801f59c
	movs	r3, #2
	mov	r0, fp
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0801f59c
	movs	r3, #1
	ands	r3, r0
	ldr	r7, [sp, #24]
	cmp	r3, #0
	beq.n	.L_0801f518
	adds	r7, #1
.L_0801f518:
	add	r6, sp, #36
	adds	r1, r6, #0
	movs	r0, #0
	bl	sub_080b5130
	mov	r2, sl
	str	r2, [sp, #0]
	ldr	r1, [pc, #156]
	mov	r0, r8
	movs	r2, #0
	adds	r3, r7, #0
	bl	sub_08019000
	mov	r3, sl
	str	r3, [sp, #0]
	ldr	r1, [pc, #144]
	mov	r0, r8
	movs	r2, #2
	adds	r3, r7, #0
	bl	sub_08019000
	adds	r5, r7, #1
	mov	r4, sl
	ldr	r1, [pc, #132]
	mov	r0, r8
	movs	r2, #0
	adds	r3, r5, #0
	str	r4, [sp, #0]
	bl	sub_08019000
	mov	r0, sl
	str	r0, [sp, #0]
	ldr	r1, [pc, #116]
	mov	r0, r8
	movs	r2, #2
	adds	r3, r5, #0
	bl	sub_08019000
	ldrb	r1, [r6, #0]
	movs	r2, #1
	adds	r1, #48
	adds	r3, r7, #0
	mov	r0, r8
	bl	sub_08018efc
	ldrb	r1, [r6, #1]
	movs	r2, #3
	adds	r1, #48
	adds	r3, r7, #0
	mov	r0, r8
	bl	sub_08018efc
	ldrb	r1, [r6, #2]
	movs	r2, #1
	adds	r1, #48
	adds	r3, r5, #0
	mov	r0, r8
	bl	sub_08018efc
	ldrb	r1, [r6, #3]
	mov	r0, r8
	adds	r1, #48
	movs	r2, #3
	adds	r3, r5, #0
	bl	sub_08018efc
.L_0801f59c:
	ldr	r1, [sp, #32]
	ldr	r3, [pc, #24]
	adds	r2, r1, r3
	movs	r3, #0
	strb	r3, [r2, #0]
.L_0801f5a6:
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00000ea6
	.4byte 0x00000ea7
	.4byte 0x00000ea5
	.4byte 0x00005001
	.4byte 0x00005002
	.4byte 0x00005003
	.4byte 0x00005004
