.syntax unified
	.thumb
	.set sub_08017054, 0x08017054
	.set sub_08017500, 0x08017500
	.set sub_08017884, 0x08017884
	.global Overlay_080171fc
Overlay_080171fc:
	push	{r4, r5, lr}
	sub	sp, #256
	adds	r5, r1, #0
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	ldr	r2, [pc, #32]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #32]
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
	ldr	r3, [pc, #24]
	movs	r0, #1
	eors	r3, r0
	mov	r2, sp
	ldr	r0, [pc, #20]
	ldr	r1, [pc, #16]
	subs	r0, r0, r1
	lsls	r0, r0, #15
	b.n	.L_08017244
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x080171b9
	.2byte 0x71fd
	.2byte 0x0801
.L_08017238:
	ldrh	r0, [r3, #0]
	strh	r0, [r2, #0]
	adds	r3, #2
	adds	r2, #2
	subs	r0, r1, #1
	lsls	r0, r0, #16
.L_08017244:
	lsrs	r1, r0, #16
	cmp	r1, #0
	bne.n	.L_08017238
	mov	r3, sp
	adds	r3, #1
	ldr	r0, [pc, #28]
	ldrb	r1, [r0, #28]
	lsls	r4, r1
	adds	r1, r4, #0
	movs	r2, #224
	lsls	r2, r2, #20
	adds	r1, r1, r2
	ldrh	r2, [r0, #24]
	adds	r0, r5, #0
	bl	sub_08017884
	add	sp, #256
	pop	{r4, r5}
	pop	{r1}
	bx	r1
	.2byte 0x7e14
	.2byte 0x0801
	push	{r4, r5, r6, lr}
	sub	sp, #64
	ldr	r5, [pc, #84]
	ldrh	r1, [r5, #0]
	ldr	r6, [pc, #84]
	ands	r1, r6
	ldr	r0, [pc, #84]
	ldr	r0, [r0, #0]
	ldrh	r0, [r0, #16]
	orrs	r0, r1
	strh	r0, [r5, #0]
	ldr	r1, [pc, #80]
	movs	r4, #170
	strb	r4, [r1, #0]
	ldr	r3, [pc, #76]
	movs	r2, #85
	strb	r2, [r3, #0]
	movs	r0, #128
	strb	r0, [r1, #0]
	strb	r4, [r1, #0]
	strb	r2, [r3, #0]
	movs	r0, #16
	strb	r0, [r1, #0]
	mov	r0, sp
	bl	sub_08017054
	ldr	r0, [pc, #56]
	movs	r1, #224
	lsls	r1, r1, #20
	ldr	r3, [r0, #0]
	movs	r0, #3
	movs	r2, #255
	bl	sub_08017884
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	ldrh	r1, [r5, #0]
	ands	r1, r6
	movs	r2, #3
	orrs	r1, r2
	strh	r1, [r5, #0]
	add	sp, #64
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x02007408
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.2byte 0x7400
	.2byte 0x0200
.L_080172e4:
	push	{r4, r5, r6, lr}
	mov	r6, r8
	push	{r6}
	sub	sp, #64
	lsls	r0, r0, #16
	lsrs	r3, r0, #16
	cmp	r3, #15
	bhi.n	.L_0801736c
	ldr	r6, [pc, #92]
	ldrh	r1, [r6, #0]
	ldr	r0, [pc, #92]
	mov	r8, r0
	ands	r1, r0
	ldr	r0, [pc, #92]
	ldr	r2, [r0, #0]
	ldrh	r0, [r2, #16]
	orrs	r0, r1
	strh	r0, [r6, #0]
	ldrb	r4, [r2, #8]
	lsls	r3, r4
	adds	r4, r3, #0
	movs	r2, #224
	lsls	r2, r2, #20
	adds	r4, r4, r2
	ldr	r1, [pc, #72]
	movs	r5, #170
	strb	r5, [r1, #0]
	ldr	r3, [pc, #72]
	movs	r2, #85
	strb	r2, [r3, #0]
	movs	r0, #128
	strb	r0, [r1, #0]
	strb	r5, [r1, #0]
	strb	r2, [r3, #0]
	movs	r0, #48
	strb	r0, [r4, #0]
	mov	r0, sp
	bl	sub_08017054
	ldr	r0, [pc, #52]
	ldr	r3, [r0, #0]
	movs	r0, #2
	adds	r1, r4, #0
	movs	r2, #255
	bl	sub_08017884
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	ldrh	r1, [r6, #0]
	mov	r2, r8
	ands	r1, r2
	movs	r2, #3
	orrs	r1, r2
	strh	r1, [r6, #0]
	b.n	.L_0801736e
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x02007408
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.2byte 0x7400
	.2byte 0x0200
.L_0801736c:
	ldr	r0, [pc, #12]
.L_0801736e:
	add	sp, #64
	pop	{r3}
	mov	r8, r3
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.2byte 0x80ff
	.2byte 0x0000
.L_08017380:
	push	{r4, lr}
	ldr	r4, [pc, #40]
	movs	r2, #170
	strb	r2, [r4, #0]
	ldr	r3, [pc, #36]
	movs	r2, #85
	strb	r2, [r3, #0]
	movs	r2, #160
	strb	r2, [r4, #0]
	ldrb	r2, [r0, #0]
	strb	r2, [r1, #0]
	ldr	r3, [pc, #28]
	ldrb	r2, [r0, #0]
	ldr	r3, [r3, #0]
	movs	r0, #1
	bl	sub_08017884
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	pop	{r4}
	pop	{r1}
	bx	r1
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.2byte 0x7400
	.2byte 0x0200
	push	{r4, r5, r6, r7, lr}
	mov	r7, r9
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #96
	mov	r9, r1
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	mov	r8, r0
	cmp	r0, #15
	bls.n	.L_080173d8
	ldr	r0, [pc, #4]
	b.n	.L_080174c6
	movs	r0, r0
	.2byte 0x80ff
	.2byte 0x0000
.L_080173d8:
	ldr	r0, [pc, #28]
	ldr	r0, [r0, #0]
	ldrb	r0, [r0, #8]
	mov	r7, r8
	lsls	r7, r0
	movs	r0, #224
	lsls	r0, r0, #20
	adds	r7, r7, r0
	ldr	r1, [pc, #16]
	movs	r0, #1
	adds	r3, r1, #0
	eors	r3, r0
	mov	r2, sp
	ldr	r0, [pc, #12]
	subs	r0, r0, r1
	b.n	.L_0801740e
	.4byte 0x02007408
	.4byte 0x080174dd
	.2byte 0x7501
	.2byte 0x0801
.L_08017404:
	ldrh	r0, [r3, #0]
	strh	r0, [r2, #0]
	adds	r3, #2
	adds	r2, #2
	subs	r0, r1, #2
.L_0801740e:
	lsls	r0, r0, #16
	lsrs	r1, r0, #16
	cmp	r1, #0
	bne.n	.L_08017404
	movs	r4, #0
	b.n	.L_08017424
.L_0801741a:
	adds	r0, r4, #1
	lsls	r0, r0, #24
	lsrs	r4, r0, #24
	cmp	r4, #81
	beq.n	.L_080174c4
.L_08017424:
	mov	r0, r8
	bl	.L_080172e4
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	cmp	r5, #0
	bne.n	.L_0801741a
	adds	r0, r7, #0
	mov	r1, sp
	adds	r1, #1
	bl	sub_08017500
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	cmp	r5, #0
	bne.n	.L_0801741a
	movs	r6, #1
	cmp	r4, #0
	beq.n	.L_0801744c
	movs	r6, #6
.L_0801744c:
	movs	r4, #1
	cmp	r4, r6
	bhi.n	.L_08017462
.L_08017452:
	mov	r0, r8
	bl	.L_080172e4
	adds	r0, r4, #1
	lsls	r0, r0, #24
	lsrs	r4, r0, #24
	cmp	r4, r6
	bls.n	.L_08017452
.L_08017462:
	mov	r0, sp
	bl	sub_08017054
	ldr	r3, [pc, #24]
	ldrh	r1, [r3, #0]
	ldr	r0, [pc, #24]
	ands	r1, r0
	ldr	r0, [pc, #24]
	ldr	r2, [r0, #0]
	ldrh	r0, [r2, #16]
	orrs	r0, r1
	strh	r0, [r3, #0]
	ldr	r1, [pc, #20]
	ldr	r0, [r2, #4]
	strh	r0, [r1, #0]
	adds	r4, r1, #0
	b.n	.L_080174a0
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x02007408
	.2byte 0x740c
	.2byte 0x0200
.L_08017494:
	ldrh	r0, [r4, #0]
	subs	r0, #1
	strh	r0, [r4, #0]
	movs	r0, #1
	add	r9, r0
	adds	r7, #1
.L_080174a0:
	ldrh	r0, [r4, #0]
	cmp	r0, #0
	beq.n	.L_080174b6
	mov	r0, r9
	adds	r1, r7, #0
	bl	.L_08017380
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	cmp	r5, #0
	beq.n	.L_08017494
.L_080174b6:
	ldr	r2, [pc, #28]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #28]
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
.L_080174c4:
	adds	r0, r5, #0
.L_080174c6:
	add	sp, #96
	pop	{r3, r4}
	mov	r8, r3
	mov	r9, r4
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x04000204
	.4byte 0x0000fffc
