.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08014878, 0x08014878
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_080ad010, 0x080ad010
	.set sub_080ad0e8, 0x080ad0e8
	.set sub_0812764c, 0x0812764c
	.set sub_0812786c, 0x0812786c
	.set sub_08127a0c, 0x08127a0c
	.set sub_08127adc, 0x08127adc
	.set sub_08127c2c, 0x08127c2c
	.global Overlay_08127c80
Overlay_08127c80:
	push	{r5, r6, lr}
	movs	r3, #165
	lsls	r3, r3, #2
	sub	sp, #20
	movs	r5, #0
	cmp	r0, r3
	bcc.n	.L_08127c90
	movs	r0, #1
.L_08127c90:
	lsls	r3, r0, #1
	ldr	r2, [pc, #60]
	adds	r3, r3, r0
	lsls	r3, r3, #3
	adds	r0, r3, r2
	mov	r6, sp
	adds	r2, r0, #0
	adds	r2, #15
	adds	r4, r6, #0
	movs	r1, #4
.L_08127ca4:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	cmp	r3, #0
	beq.n	.L_08127cb4
	ldrh	r3, [r0, #0]
	adds	r5, #1
	adds	r3, #8
	stmia	r4!, {r3}
.L_08127cb4:
	subs	r1, #1
	adds	r0, #2
	cmp	r1, #0
	bge.n	.L_08127ca4
	bl	sub_08014878
	adds	r3, r5, #0
	muls	r3, r0
	lsrs	r3, r3, #16
	lsls	r3, r3, #2
	ldr	r0, [r6, r3]
	add	sp, #20
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0xce7c
	.2byte 0x0812
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	sub	sp, #124
	str	r3, [sp, #24]
	movs	r2, #0
	adds	r5, r0, #0
	adds	r3, #64
	mov	r1, sp
	movs	r0, #116
	str	r2, [sp, #32]
	adds	r1, #96
	strb	r2, [r3, #0]
	adds	r0, #255
	str	r1, [sp, #28]
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08127d10
	add	r0, sp, #32
	bl	sub_0812764c
	adds	r5, r0, #0
.L_08127d10:
	movs	r2, #165
	lsls	r2, r2, #2
	cmp	r5, r2
	bcc.n	.L_08127d1a
	movs	r5, #1
.L_08127d1a:
	lsls	r3, r5, #1
	ldr	r2, [pc, #584]
	adds	r3, r3, r5
	lsls	r3, r3, #3
	adds	r3, r3, r2
	mov	fp, r3
	movs	r3, #2
	add	r3, fp
	mov	sl, r3
	ldrb	r3, [r3, #8]
	movs	r1, #0
	cmp	r3, #0
	bne.n	.L_08127d46
	mov	r2, fp
	adds	r2, #10
.L_08127d38:
	adds	r1, #1
	cmp	r1, #4
	bgt.n	.L_08127d46
	adds	r2, #1
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08127d38
.L_08127d46:
	cmp	r1, #5
	bne.n	.L_08127d54
	ldr	r4, [pc, #544]
	movs	r6, #2
	mov	fp, r4
	add	r6, fp
	mov	sl, r6
.L_08127d54:
	movs	r1, #6
	movs	r2, #0
	str	r1, [sp, #20]
	str	r2, [sp, #16]
	mov	r5, sl
	movs	r7, #0
	mov	r6, fp
	adds	r5, #8
.L_08127d64:
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_08127d86
	ldrh	r0, [r6, #0]
	adds	r0, #8
	bl	.L_081280fc
	negs	r3, r0
	orrs	r3, r0
	movs	r2, #2
	lsrs	r3, r3, #31
	subs	r3, r2, r3
	ldrb	r2, [r5, #0]
	ldr	r4, [sp, #20]
	muls	r3, r2
	subs	r4, r4, r3
	str	r4, [sp, #20]
.L_08127d86:
	adds	r7, #1
	adds	r6, #2
	adds	r5, #1
	cmp	r7, #4
	bls.n	.L_08127d64
	mov	r2, fp
	mov	r6, sp
	movs	r1, #76
	movs	r3, #15
	str	r2, [sp, #8]
	adds	r6, #56
	add	r1, sp
	add	r3, fp
	movs	r4, #8
	str	r6, [sp, #12]
	movs	r7, #0
	mov	r8, r1
	mov	r9, r3
	add	sl, r4
	movs	r6, #0
.L_08127dae:
	mov	r1, sl
	ldrb	r2, [r1, #0]
	movs	r3, #1
	mov	r1, r9
	ldr	r4, [sp, #12]
	add	sl, r3
	ldrb	r3, [r1, #0]
	str	r2, [r6, r4]
	subs	r5, r3, r2
	movs	r4, #1
	add	r9, r4
	cmp	r5, #0
	ble.n	.L_08127df8
	ldr	r1, [sp, #8]
	ldrh	r0, [r1, #0]
	adds	r0, #8
	bl	.L_081280fc
	negs	r1, r0
	orrs	r1, r0
	lsrs	r1, r1, #31
	movs	r3, #2
	subs	r1, r3, r1
	ldr	r0, [sp, #20]
	bl	sub_08002054
	cmp	r0, r5
	bge.n	.L_08127de8
	adds	r5, r0, #0
.L_08127de8:
	bl	sub_08014878
	adds	r3, r5, #1
	muls	r3, r0
	mov	r2, r8
	lsrs	r3, r3, #16
	str	r3, [r6, r2]
	b.n	.L_08127dfe
.L_08127df8:
	movs	r3, #0
	mov	r4, r8
	str	r3, [r6, r4]
.L_08127dfe:
	ldr	r1, [sp, #8]
	adds	r7, #1
	adds	r1, #2
	str	r1, [sp, #8]
	adds	r6, #4
	cmp	r7, #4
	bls.n	.L_08127dae
	ldr	r4, [sp, #12]
	mov	r1, r8
.L_08127e10:
	movs	r2, #0
	mov	sl, r2
	movs	r7, #0
	movs	r5, #0
	mov	r6, fp
.L_08127e1a:
	ldrh	r3, [r6, #0]
	mov	r2, r8
	adds	r0, r3, #0
	ldr	r3, [r5, r2]
	adds	r6, #2
	adds	r0, #8
	cmp	r3, #0
	beq.n	.L_08127e66
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	bl	.L_081280fc
	negs	r3, r0
	orrs	r3, r0
	lsrs	r3, r3, #31
	adds	r2, r3, #0
	movs	r3, #2
	subs	r2, r3, r2
	ldr	r3, [sp, #20]
	ldr	r1, [sp, #4]
	ldr	r4, [sp, #0]
	cmp	r2, r3
	ble.n	.L_08127e50
	movs	r3, #0
	mov	r2, r8
	str	r3, [r5, r2]
	b.n	.L_08127e66
.L_08127e50:
	ldr	r3, [r5, r4]
	adds	r3, #1
	str	r3, [r5, r4]
	ldr	r3, [r5, r1]
	subs	r3, #1
	str	r3, [r5, r1]
	ldr	r3, [sp, #20]
	subs	r3, r3, r2
	str	r3, [sp, #20]
	movs	r2, #1
	mov	sl, r2
.L_08127e66:
	adds	r7, #1
	adds	r5, #4
	cmp	r7, #4
	bls.n	.L_08127e1a
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_08127e10
	ldr	r2, [sp, #24]
	mov	r4, fp
	ldrb	r3, [r4, #20]
	adds	r2, #66
	strb	r3, [r2, #0]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08127e8c
	cmp	r3, #1
	beq.n	.L_08127f0a
	movs	r7, #0
	b.n	.L_08127f9e
.L_08127e8c:
	add	r6, sp, #36
	mov	r8, r6
	movs	r7, #0
	mov	r3, r8
.L_08127e94:
	stmia	r3!, {r7}
	adds	r7, #1
	cmp	r7, #4
	bls.n	.L_08127e94
	movs	r7, #0
	mov	r6, r8
.L_08127ea0:
	bl	sub_08014878
	lsls	r5, r0, #2
	adds	r5, r5, r0
	bl	sub_08014878
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsrs	r5, r5, #16
	lsrs	r3, r3, #16
	lsls	r5, r5, #2
	lsls	r3, r3, #2
	ldr	r1, [r6, r5]
	ldr	r2, [r6, r3]
	adds	r7, #1
	str	r2, [r6, r5]
	str	r1, [r6, r3]
	cmp	r7, #9
	bls.n	.L_08127ea0
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #16]
	movs	r7, #0
	mov	ip, r1
	mov	r5, r8
	lsls	r0, r2, #1
.L_08127ed2:
	ldr	r2, [r5, #0]
	mov	r4, ip
	lsls	r1, r2, #2
	ldr	r3, [r4, r1]
	cmp	r3, #0
	ble.n	.L_08127f00
	ldr	r6, [sp, #12]
	ldr	r3, [sp, #28]
	ldr	r1, [r6, r1]
	lsls	r4, r2, #1
	adds	r2, r0, r3
.L_08127ee8:
	mov	r6, fp
	ldrh	r3, [r6, r4]
	subs	r1, #1
	adds	r3, #8
	strh	r3, [r2, #0]
	ldr	r3, [sp, #16]
	adds	r2, #2
	adds	r3, #1
	adds	r0, #2
	str	r3, [sp, #16]
	cmp	r1, #0
	bne.n	.L_08127ee8
.L_08127f00:
	adds	r7, #1
	adds	r5, #4
	cmp	r7, #4
	bls.n	.L_08127ed2
	b.n	.L_08127fa2
.L_08127f0a:
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #28]
	movs	r6, #36
	ldr	r4, [sp, #12]
	add	r6, sp
	lsls	r3, r1, #1
	mov	r8, r6
	adds	r6, r3, r2
.L_08127f1a:
	movs	r5, #0
	movs	r7, #0
	adds	r1, r4, #0
	add	r2, sp, #36
.L_08127f22:
	ldmia	r1!, {r3}
	cmp	r3, #0
	beq.n	.L_08127f2c
	stmia	r2!, {r7}
	adds	r5, #1
.L_08127f2c:
	adds	r7, #1
	cmp	r7, #4
	bls.n	.L_08127f22
	cmp	r5, #0
	beq.n	.L_08127fa2
	str	r4, [sp, #0]
	bl	sub_08014878
	adds	r3, r5, #0
	muls	r3, r0
	lsrs	r3, r3, #16
	lsls	r3, r3, #2
	mov	r1, r8
	ldr	r2, [r1, r3]
	mov	r1, fp
	lsls	r3, r2, #1
	ldrh	r3, [r1, r3]
	ldr	r4, [sp, #0]
	adds	r3, #8
	strh	r3, [r6, #0]
	ldr	r3, [sp, #16]
	lsls	r2, r2, #2
	adds	r3, #1
	str	r3, [sp, #16]
	adds	r6, #2
	ldr	r3, [r4, r2]
	subs	r3, #1
	str	r3, [r4, r2]
	b.n	.L_08127f1a
	movs	r0, r0
	.4byte 0x0812ce7c
	.2byte 0xce94
	.2byte 0x0812
.L_08127f70:
	ldr	r4, [sp, #12]
	lsls	r3, r7, #2
	ldr	r3, [r4, r3]
	cmp	r3, #0
	ble.n	.L_08127f9c
	ldr	r6, [sp, #16]
	ldr	r4, [sp, #28]
	adds	r1, r3, #0
	lsls	r3, r6, #1
	lsls	r0, r7, #1
	adds	r2, r3, r4
.L_08127f86:
	mov	r6, fp
	ldrh	r3, [r6, r0]
	subs	r1, #1
	adds	r3, #8
	strh	r3, [r2, #0]
	ldr	r3, [sp, #16]
	adds	r2, #2
	adds	r3, #1
	str	r3, [sp, #16]
	cmp	r1, #0
	bne.n	.L_08127f86
.L_08127f9c:
	adds	r7, #1
.L_08127f9e:
	cmp	r7, #4
	bls.n	.L_08127f70
.L_08127fa2:
	ldr	r4, [sp, #16]
	ldr	r6, [sp, #28]
	ldr	r3, [pc, #24]
	lsls	r2, r4, #1
	strh	r3, [r2, r6]
	ldr	r1, [sp, #24]
	movs	r3, #6
	strh	r3, [r1, #60]
	ldr	r3, [sp, #24]
	movs	r2, #0
	strh	r2, [r3, #62]
	ldr	r5, [pc, #8]
	movs	r7, #128
	b.n	.L_08127fc8
	movs	r0, r0
	.4byte 0x00000000
	.2byte 0x0258
	.2byte 0x0300
.L_08127fc8:
	adds	r0, r7, #0
	bl	sub_08016ca4
	movs	r1, #166
	lsls	r1, r1, #1
	adds	r7, #1
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2f85
	bls.n	.L_08127fc8
	ldr	r4, [sp, #28]
	movs	r7, #0
	ldrh	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L_08128090
	movs	r2, #0
.L_08127fe8:
	ldr	r6, [sp, #28]
	movs	r1, #1
	adds	r5, r2, r6
	ldrh	r0, [r5, #0]
	bl	sub_08127adc
	movs	r3, #128
	adds	r4, r0, #0
	lsls	r3, r3, #8
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_0812800a
	ldrh	r0, [r5, #0]
	str	r4, [sp, #0]
	bl	sub_08127c2c
	ldr	r4, [sp, #0]
.L_0812800a:
	movs	r2, #254
	adds	r6, r7, #0
	lsls	r2, r2, #7
	adds	r6, #128
	adds	r2, #255
	ldrh	r1, [r5, #0]
	ands	r2, r4
	adds	r0, r6, #0
	bl	sub_080ad0e8
	adds	r0, r6, #0
	bl	sub_08016ca4
	ldr	r1, [sp, #32]
	mov	r8, r0
	cmp	r1, #0
	beq.n	.L_08128034
	adds	r0, r6, #0
	bl	sub_0812786c
	b.n	.L_08128050
.L_08128034:
	movs	r0, #116
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_08128050
	movs	r0, #46
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08128050
	adds	r0, r6, #0
	bl	sub_08127a0c
.L_08128050:
	ldr	r1, [sp, #24]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r2, #104
	movs	r3, #0
	adds	r5, r1, r2
	strb	r3, [r5, #0]
	movs	r3, #165
	lsls	r3, r3, #1
	add	r3, r8
	ldrh	r0, [r3, #0]
	cmp	r0, #103
	bgt.n	.L_0812807e
	cmp	r0, #101
	blt.n	.L_0812807e
	movs	r0, #116
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_0812807e
	movs	r3, #1
	strb	r3, [r5, #0]
.L_0812807e:
	adds	r7, #1
	cmp	r7, #5
	bhi.n	.L_08128090
	ldr	r4, [sp, #28]
	lsls	r3, r7, #1
	adds	r2, r3, #0
	ldrh	r3, [r2, r4]
	cmp	r3, #0
	bne.n	.L_08127fe8
.L_08128090:
	adds	r0, r7, #0
	add	sp, #124
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{lr}
	ldr	r3, [pc, #20]
	lsls	r0, r0, #3
	adds	r0, r0, r3
	ldrb	r3, [r0, #3]
	lsrs	r0, r3, #5
	cmp	r0, #4
	ble.n	.L_081280b4
	movs	r0, #1
	negs	r0, r0
.L_081280b4:
	pop	{pc}
	movs	r0, r0
	.2byte 0x0d0c
	.2byte 0x0813
	push	{lr}
	movs	r3, #193
	lsls	r3, r3, #1
	cmp	r0, r3
	bls.n	.L_081280cc
	ldr	r3, [pc, #12]
	ldrh	r0, [r3, #0]
	b.n	.L_081280d2
.L_081280cc:
	ldr	r3, [pc, #4]
	lsls	r2, r0, #3
	ldrh	r0, [r3, r2]
.L_081280d2:
	pop	{pc}
	.2byte 0x0d0c
	.2byte 0x0813
	push	{lr}
	movs	r3, #193
	lsls	r3, r3, #1
	cmp	r0, r3
	bls.n	.L_081280e8
	ldr	r3, [pc, #20]
	ldrh	r0, [r3, #0]
	b.n	.L_081280f4
.L_081280e8:
	ldr	r3, [pc, #12]
	lsls	r2, r0, #3
	adds	r2, r2, r3
	ldrb	r0, [r2, #3]
	lsls	r0, r0, #27
	lsrs	r0, r0, #28
.L_081280f4:
	pop	{pc}
	movs	r0, r0
	.2byte 0x0d0c
	.2byte 0x0813
.L_081280fc:
	push	{lr}
	movs	r3, #193
	lsls	r3, r3, #1
	cmp	r0, r3
	bls.n	.L_0812810a
	movs	r0, #0
	b.n	.L_0812811e
.L_0812810a:
	ldr	r3, [pc, #20]
	lsls	r2, r0, #3
	adds	r2, r2, r3
	ldrb	r3, [r2, #2]
	movs	r1, #0
	lsls	r3, r3, #31
	cmp	r3, #0
	beq.n	.L_0812811c
	movs	r1, #1
.L_0812811c:
	adds	r0, r1, #0
.L_0812811e:
	pop	{pc}
	.2byte 0x0d0c
	.2byte 0x0813
	push	{lr}
	movs	r3, #193
	lsls	r3, r3, #1
	cmp	r0, r3
	bls.n	.L_08128132
	movs	r0, #1
	b.n	.L_08128146
.L_08128132:
	ldr	r3, [pc, #20]
	lsls	r2, r0, #3
	adds	r2, r2, r3
	ldrb	r3, [r2, #2]
	lsls	r3, r3, #27
	lsrs	r3, r3, #28
	adds	r0, r3, #0
	cmp	r3, #0
	bne.n	.L_08128146
	movs	r0, #1
.L_08128146:
	pop	{pc}
	.2byte 0x0d0c
	.2byte 0x0813
	push	{lr}
	movs	r3, #193
	lsls	r3, r3, #1
	cmp	r0, r3
	bls.n	.L_0812815a
	movs	r0, #0
	b.n	.L_0812816c
.L_0812815a:
	ldr	r3, [pc, #20]
	lsls	r2, r0, #3
	adds	r2, r2, r3
	ldrb	r3, [r2, #2]
	lsrs	r3, r3, #5
	adds	r0, r3, #0
	cmp	r3, #0
	bne.n	.L_0812816c
	movs	r0, #0
.L_0812816c:
	pop	{pc}
	movs	r0, r0
	.2byte 0x0d0c
	.2byte 0x0813
	push	{lr}
	movs	r3, #193
	lsls	r3, r3, #1
	cmp	r0, r3
	bls.n	.L_08128182
	movs	r0, #0
	b.n	.L_0812818e
.L_08128182:
	ldr	r3, [pc, #12]
	lsls	r2, r0, #3
	adds	r2, r2, r3
	ldrb	r0, [r2, #3]
	lsls	r0, r0, #31
	lsrs	r0, r0, #31
.L_0812818e:
	pop	{pc}
	.2byte 0x0d0c
	.2byte 0x0813
	push	{lr}
	movs	r3, #193
	lsls	r3, r3, #1
	cmp	r0, r3
	bls.n	.L_081281a2
	movs	r0, #0
	b.n	.L_081281aa
.L_081281a2:
	ldr	r3, [pc, #8]
	lsls	r2, r0, #3
	adds	r2, #4
	ldrb	r0, [r3, r2]
.L_081281aa:
	pop	{pc}
	.2byte 0x0d0c
	.2byte 0x0813
	push	{r5, r6, lr}
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r6, r0, #0
	adds	r3, #255
	ands	r6, r3
	movs	r5, #0
	movs	r0, #0
	cmp	r6, #0
	beq.n	.L_081281e8
	adds	r0, r6, #0
	bl	sub_080ad010
	ldrb	r2, [r0, #3]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081281d6
	movs	r5, #1
.L_081281d6:
	movs	r3, #4
	ands	r3, r2
	lsls	r5, r5, #1
	cmp	r3, #0
	beq.n	.L_081281e2
	adds	r5, #1
.L_081281e2:
	lsls	r5, r5, #9
	adds	r5, r5, r6
	adds	r0, r5, #0
.L_081281e8:
	pop	{r5, r6, pc}
