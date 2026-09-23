.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_080132cc, 0x080132cc
	.set sub_08013560, 0x08013560
	.set sub_08013eb4, 0x08013eb4
	.set sub_08014368, 0x08014368
	.set sub_080144c0, 0x080144c0
	.set sub_080145a8, 0x080145a8
	.set sub_0801489c, 0x0801489c
	.set sub_08014c6c, 0x08014c6c
	.set sub_08014d00, 0x08014d00
	.set sub_08014d78, 0x08014d78
	.set sub_08021918, 0x08021918
	.set sub_080227e0, 0x080227e0
	.set sub_080229ac, 0x080229ac
	.set sub_08022d40, 0x08022d40
	.set sub_080230e0, 0x080230e0
	.set sub_0802e6d8, 0x0802e6d8
	.set sub_0802e6fc, 0x0802e6fc
	.set sub_0802e76c, 0x0802e76c
	.global Func_0802dd08
	.thumb_func
Func_0802dd08:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	ldr	r3, [pc, #72]
	mov	sl, r0
	mov	r9, r1
	mov	r8, r3
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #52]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #44]
	movs	r3, #128
	lsls	r3, r3, #5
	add	r8, r3
	mov	r0, sl
	mov	r1, r9
	mov	r3, r8
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c30
	bl	sub_08013164
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x0201c000
	.4byte 0x00000268
	.4byte 0x08021070
	.2byte 0xc000
	.2byte 0x0203
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #40
	str	r0, [sp, #12]
	adds	r7, r2, #0
	ldr	r3, [r1, #0]
	add	r0, sp, #28
	movs	r2, #0
	str	r2, [r0, #4]
	str	r3, [r0, #0]
	ldr	r3, [r1, #8]
	add	r1, sp, #16
	str	r3, [r0, #8]
	ldr	r3, [pc, #224]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x466a
	adds	r2, #16
	str	r2, [sp, #0]
	ldr	r3, [pc, #216]
	ldr	r0, [r2, #8]
	ldr	r1, [sp, #12]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9a00
	ldr	r3, [r2, #4]
	movs	r2, #0
	subs	r3, r3, r0
	str	r3, [sp, #8]
	ldr	r3, [pc, #200]
	mov	fp, r2
	mov	r9, r3
	ldr	r3, [r3, #0]
	negs	r3, r3
	str	r3, [sp, #4]
	b.n	.L_0802ddc6
.L_0802ddc2:
	ldr	r3, [pc, #188]
	mov	r9, r3
.L_0802ddc6:
	mov	r2, r9
	ldr	r1, [r2, #16]
	ldr	r2, [pc, #184]
	mov	r3, fp
	subs	r1, r1, r3
	lsls	r1, r1, #16
	mov	r8, r2
	ldr	r0, [sp, #4]
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x9b03
	adds	r6, r0, #0
	subs	r0, r6, r3
	cmp	r0, #0
	bne.n	.L_0802dde6
	movs	r0, #1
.L_0802dde6:
	ldr	r1, [sp, #8]
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c05
	cmp	r5, #0
	bge.n	.L_0802de52
	ldr	r2, [pc, #136]
	movs	r1, #128
	mov	sl, r2
	negs	r0, r5
	lsls	r1, r1, #8
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x464b
	adds	r1, r0, #0
	ldr	r0, [r3, #0]
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c31
	str	r0, [r7, #0]
	adds	r0, r5, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x9a00
	adds	r6, r0, #0
	ldr	r1, [r2, #8]
	ldr	r3, [r2, #4]
	subs	r1, r1, r5
	asrs	r1, r1, #4
	adds	r0, r1, #0
	subs	r6, r6, r3
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x1136
	adds	r5, r0, #0
	adds	r1, r6, #0
	adds	r0, r6, #0
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x182d
	ldr	r3, [pc, #80]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x0300
	cmp	r6, #0
	bge.n	.L_0802de46
	negs	r0, r0
.L_0802de46:
	movs	r1, #128
	lsls	r1, r1, #8
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x6078
	b.n	.L_0802de58
.L_0802de52:
	movs	r3, #0
	str	r3, [r7, #0]
	str	r3, [r7, #4]
.L_0802de58:
	movs	r3, #0
	str	r3, [r7, #8]
	str	r3, [r7, #12]
	movs	r3, #1
	add	fp, r3
	mov	r2, fp
	adds	r7, #20
	cmp	r2, #159
	ble.n	.L_0802ddc2
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03000354
	.4byte 0x0300021c
	.4byte 0x030011e0
	.4byte 0x03000230
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #60
	movs	r0, #144
	movs	r1, #96
	movs	r2, #1
	movs	r3, #0
	str	r0, [sp, #20]
	str	r1, [sp, #16]
	movs	r0, #36
	movs	r1, #160
	mov	fp, r2
	mov	r8, r3
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	bl	sub_08014d00
	ldr	r2, [pc, #128]
	movs	r3, #3
	str	r0, [sp, #4]
	strb	r3, [r2, #0]
	ldr	r0, [sp, #8]
	add	r4, sp, #24
	movs	r3, #128
	movs	r2, #133
	str	r0, [r4, #0]
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r4, #0
	ldr	r1, [sp, #4]
	adds	r2, #1
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r1, [sp, #8]
	movs	r2, #133
	lsls	r2, r2, #24
	str	r1, [r4, #0]
	adds	r0, r4, #0
	add	r1, sp, #28
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	movs	r1, #1
	negs	r0, r0
	bl	.L_0802e444
	add	r2, sp, #28
	ldr	r1, [pc, #52]
	mov	sl, r2
	movs	r6, #0
	movs	r2, #1
	mov	r3, sl
.L_0802df02:
	adds	r6, #1
	strh	r2, [r3, #2]
	strb	r1, [r3, #5]
	strh	r0, [r3, #0]
	adds	r3, #8
	cmp	r6, #3
	bls.n	.L_0802df02
	ldr	r2, [pc, #44]
	movs	r3, #1
	mov	r0, sl
	strb	r3, [r0, #4]
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r3, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #16]
	movs	r2, #160
	lsls	r2, r2, #19
	b.n	.L_0802df44
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x00003f42
	.4byte 0x00007c00
	.4byte 0x03001174
	.2byte 0x1238
	.2byte 0x0300
.L_0802df44:
	strh	r3, [r2, #0]
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013eb4
.L_0802df54:
	bl	sub_08014c6c
	bl	sub_080144c0
	movs	r1, #160
	movs	r0, #36
	bl	sub_08014d00
	str	r0, [sp, #4]
	bl	sub_08014368
	movs	r0, #2
	bl	sub_080230e0
	mov	r0, sl
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r1, [pc, #16]
	movs	r3, #0
	movs	r0, #0
	bl	sub_080227e0
	mov	r2, sl
	b.n	.L_0802df8c
	.4byte 0x00001140
	.2byte 0x0000
	.2byte 0x0201
.L_0802df8c:
	movs	r1, #0
	ldrsh	r0, [r2, r1]
	bl	sub_08021918
	ldrb	r3, [r0, #4]
	cmp	r3, #20
	bne.n	.L_0802dfac
	mov	r0, sl
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r1, [pc, #732]
	adds	r2, #1
	movs	r0, #1
	movs	r3, #0
	bl	sub_080227e0
.L_0802dfac:
	movs	r6, #0
	mov	r7, sl
.L_0802dfb0:
	movs	r1, #0
	ldrsh	r0, [r7, r1]
	bl	sub_08021918
	ldrb	r3, [r0, #4]
	movs	r5, #0
	cmp	r3, #20
	bne.n	.L_0802dfca
	movs	r3, #1
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_0802dfca
	movs	r5, #1
.L_0802dfca:
	movs	r2, #0
	ldrsh	r0, [r7, r2]
	lsls	r3, r5, #12
	adds	r0, r0, r5
	adds	r0, r0, r3
	bl	sub_08022d40
	movs	r3, #8
	ldrsh	r1, [r7, r3]
	adds	r5, r0, #0
	bl	sub_080229ac
	movs	r0, #16
	ldrsh	r1, [r7, r0]
	adds	r0, r5, #0
	bl	sub_080229ac
	movs	r2, #24
	ldrsh	r1, [r7, r2]
	adds	r0, r5, #0
	bl	sub_080229ac
	add	r3, sp, #12
	ldrb	r3, [r3, #0]
	adds	r6, #1
	strb	r3, [r5, #26]
	cmp	r6, #9
	bls.n	.L_0802dfb0
	mov	r7, sl
	movs	r0, #4
	movs	r6, #0
	mov	r5, sl
	adds	r7, #4
	mov	r9, r0
.L_0802e00e:
	mov	r1, r9
	mov	r2, sl
	ldrb	r3, [r1, r2]
	cmp	r3, #0
	beq.n	.L_0802e024
	movs	r1, #1
	ldrsb	r1, [r7, r1]
	adds	r0, r6, #0
	bl	sub_0802e6d8
	b.n	.L_0802e02c
.L_0802e024:
	adds	r0, r6, #0
	movs	r1, #8
	bl	sub_0802e6d8
.L_0802e02c:
	movs	r1, #6
	ldrsb	r1, [r5, r1]
	adds	r0, r6, #0
	bl	.L_0802e6b4
	movs	r3, #2
	ldrsh	r1, [r5, r3]
	adds	r0, r6, #0
	bl	sub_0802e6fc
	adds	r6, #1
	movs	r0, #8
	adds	r5, #8
	adds	r7, #8
	add	r9, r0
	cmp	r6, #3
	bls.n	.L_0802e00e
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #4]
	bl	.L_0802e4c8
	movs	r1, #144
	ldr	r0, [pc, #552]
	lsls	r1, r1, #3
	bl	sub_080145a8
.L_0802e062:
	movs	r0, #1
	bl	sub_08013560
	ldr	r4, [pc, #540]
	mov	r9, sl
.L_0802e06c:
	adds	r6, r4, #0
	ldr	r2, [r6, #0]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802e0ca
	ldr	r2, [r6, #0]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802e088
	ldr	r1, [sp, #20]
	subs	r1, #1
	str	r1, [sp, #20]
.L_0802e088:
	ldr	r2, [r6, #0]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802e098
	ldr	r2, [sp, #20]
	adds	r2, #1
	str	r2, [sp, #20]
.L_0802e098:
	ldr	r2, [r6, #0]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802e0a8
	ldr	r3, [sp, #16]
	subs	r3, #1
	str	r3, [sp, #16]
.L_0802e0a8:
	ldr	r2, [r6, #0]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802e0b8
	ldr	r0, [sp, #16]
	adds	r0, #1
	str	r0, [sp, #16]
.L_0802e0b8:
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #4]
	str	r4, [sp, #0]
	bl	.L_0802e4c8
	ldr	r6, [pc, #448]
	ldr	r4, [sp, #0]
	b.n	.L_0802e126
.L_0802e0ca:
	ldr	r3, [r6, #12]
	ldr	r2, [r6, #12]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802e0e4
	movs	r1, #1
	negs	r1, r1
	add	r8, r1
	mov	r2, r8
	movs	r3, #3
	ands	r2, r3
	mov	r8, r2
.L_0802e0e4:
	ldr	r2, [r6, #12]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802e0fa
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	movs	r2, #3
	ands	r1, r2
	mov	r8, r1
.L_0802e0fa:
	ldr	r2, [r6, #12]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802e110
	subs	r3, #33
	add	fp, r3
	mov	r0, fp
	movs	r1, #3
	ands	r0, r1
	mov	fp, r0
.L_0802e110:
	ldr	r2, [r6, #12]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802e126
	movs	r2, #1
	add	fp, r2
	mov	r3, fp
	movs	r0, #3
	ands	r3, r0
	mov	fp, r3
.L_0802e126:
	ldr	r2, [r4, #4]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802e150
	ldr	r1, [sp, #12]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #16]
	movs	r3, #1
	eors	r1, r3
	str	r1, [sp, #12]
	movs	r6, #0
.L_0802e140:
	add	r3, sp, #12
	ldrb	r3, [r3, #0]
	adds	r6, #1
	strb	r3, [r2, #26]
	adds	r2, #56
	cmp	r6, #9
	bls.n	.L_0802e140
	ldr	r6, [pc, #312]
.L_0802e150:
	mov	r0, fp
	cmp	r0, #1
	beq.n	.L_0802e1e0
	cmp	r0, #1
	bcc.n	.L_0802e168
	cmp	r0, #2
	bne.n	.L_0802e160
	b.n	.L_0802e28c
.L_0802e160:
	cmp	r0, #3
	bne.n	.L_0802e166
	b.n	.L_0802e2fe
.L_0802e166:
	b.n	.L_0802e378
.L_0802e168:
	mov	r1, r8
	cmp	r1, #1
	bne.n	.L_0802e170
	b.n	.L_0802e378
.L_0802e170:
	ldr	r2, [r4, #12]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802e1a4
	lsls	r3, r1, #3
	mov	r0, sl
	adds	r2, r0, r3
	ldrh	r3, [r2, #2]
	movs	r1, #0
	subs	r3, #1
	strh	r3, [r2, #2]
	lsls	r3, r3, #16
	cmp	r3, #0
	bge.n	.L_0802e192
	strh	r1, [r2, #2]
.L_0802e192:
	mov	r1, r8
	cmp	r1, #0
	beq.n	.L_0802e19a
	b.n	.L_0802e062
.L_0802e19a:
	mov	r2, sl
	ldrh	r3, [r2, #2]
	mov	r0, sl
	strh	r3, [r0, #10]
	b.n	.L_0802e062
.L_0802e1a4:
	ldr	r2, [r6, #12]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_0802e1b2
	b.n	.L_0802e378
.L_0802e1b2:
	mov	r1, r8
	lsls	r3, r1, #3
	mov	r0, sl
	adds	r2, r0, r3
	ldrh	r3, [r2, #2]
	movs	r1, #198
	adds	r3, #1
	strh	r3, [r2, #2]
	lsls	r1, r1, #15
	lsls	r3, r3, #16
	cmp	r3, r1
	ble.n	.L_0802e1ce
	movs	r3, #99
	strh	r3, [r2, #2]
.L_0802e1ce:
	mov	r2, r8
	cmp	r2, #0
	beq.n	.L_0802e1d6
	b.n	.L_0802e062
.L_0802e1d6:
	mov	r0, sl
	ldrh	r3, [r0, #2]
	mov	r1, sl
	strh	r3, [r1, #10]
	b.n	.L_0802e062
.L_0802e1e0:
	ldr	r2, [r4, #0]
	movs	r3, #8
	ands	r2, r3
	movs	r1, #0
	movs	r7, #1
	cmp	r2, #0
	beq.n	.L_0802e1f0
	movs	r7, #10
.L_0802e1f0:
	ldr	r2, [r6, #12]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802e214
	mov	r2, r8
	lsls	r5, r2, #3
	mov	r3, r9
	ldrsh	r0, [r3, r5]
	negs	r1, r7
	str	r4, [sp, #0]
	bl	.L_0802e444
	mov	r2, r9
	strh	r0, [r2, r5]
	ldr	r4, [sp, #0]
	movs	r1, #1
.L_0802e214:
	ldr	r2, [r6, #12]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802e238
	mov	r3, r8
	lsls	r5, r3, #3
	mov	r1, r9
	ldrsh	r0, [r1, r5]
	adds	r1, r7, #0
	str	r4, [sp, #0]
	bl	.L_0802e444
	mov	r3, r9
	strh	r0, [r3, r5]
	ldr	r4, [sp, #0]
	movs	r1, #1
.L_0802e238:
	cmp	r1, #0
	bne.n	.L_0802e23e
	b.n	.L_0802e376
.L_0802e23e:
	mov	r0, r8
	cmp	r0, #0
	bne.n	.L_0802e246
	b.n	.L_0802df54
.L_0802e246:
	lsls	r6, r0, #3
	adds	r5, r6, #4
	mov	r1, sl
	ldrb	r3, [r1, r5]
	cmp	r3, #0
	bne.n	.L_0802e254
	b.n	.L_0802e062
.L_0802e254:
	ldrsh	r1, [r1, r6]
	add	r5, sl
	bl	sub_0802e76c
	movs	r1, #1
	ldrsb	r1, [r5, r1]
	mov	r0, r8
	bl	sub_0802e6d8
	movs	r1, #2
	ldrsb	r1, [r5, r1]
	mov	r0, r8
	bl	.L_0802e6b4
	mov	r0, sl
	adds	r3, r0, r6
	movs	r2, #2
	ldrsh	r1, [r3, r2]
	mov	r0, r8
	bl	sub_0802e6fc
	b.n	.L_0802e062
	.4byte 0x02018000
	.4byte 0x0802e7a9
	.2byte 0x1150
	.2byte 0x0300
.L_0802e28c:
	ldr	r2, [r4, #12]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	movs	r1, #0
	cmp	r2, #0
	beq.n	.L_0802e2b6
	mov	r0, r8
	lsls	r3, r0, #3
	adds	r3, #4
	mov	r1, sl
	adds	r2, r1, r3
	ldrb	r3, [r2, #1]
	subs	r3, #1
	strb	r3, [r2, #1]
	lsls	r3, r3, #24
	cmp	r3, #0
	bge.n	.L_0802e2b4
	movs	r3, #3
	strb	r3, [r2, #1]
.L_0802e2b4:
	movs	r1, #1
.L_0802e2b6:
	ldr	r2, [r4, #12]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802e2e2
	mov	r0, r8
	lsls	r3, r0, #3
	adds	r3, #4
	mov	r1, sl
	adds	r2, r1, r3
	ldrb	r3, [r2, #1]
	movs	r0, #192
	adds	r3, #1
	strb	r3, [r2, #1]
	lsls	r0, r0, #18
	lsls	r3, r3, #24
	movs	r1, #0
	cmp	r3, r0
	ble.n	.L_0802e2e0
	strb	r1, [r2, #1]
.L_0802e2e0:
	movs	r1, #1
.L_0802e2e2:
	cmp	r1, #0
	beq.n	.L_0802e376
	mov	r1, r8
	lsls	r3, r1, #3
	adds	r2, r3, #4
	mov	r0, sl
	ldrb	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L_0802e2f6
	b.n	.L_0802e062
.L_0802e2f6:
	adds	r3, r0, r2
	movs	r1, #1
	ldrsb	r1, [r3, r1]
	b.n	.L_0802e3e2
.L_0802e2fe:
	ldr	r2, [r4, #12]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	movs	r1, #0
	cmp	r2, #0
	beq.n	.L_0802e328
	mov	r1, r8
	lsls	r3, r1, #3
	adds	r3, #4
	mov	r0, sl
	adds	r2, r0, r3
	ldrb	r3, [r2, #2]
	subs	r3, #1
	strb	r3, [r2, #2]
	lsls	r3, r3, #24
	cmp	r3, #0
	bge.n	.L_0802e326
	movs	r3, #15
	strb	r3, [r2, #2]
.L_0802e326:
	movs	r1, #1
.L_0802e328:
	ldr	r2, [r4, #12]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802e354
	mov	r1, r8
	lsls	r3, r1, #3
	adds	r3, #4
	mov	r0, sl
	adds	r2, r0, r3
	ldrb	r3, [r2, #2]
	movs	r0, #240
	adds	r3, #1
	strb	r3, [r2, #2]
	lsls	r0, r0, #20
	lsls	r3, r3, #24
	movs	r1, #0
	cmp	r3, r0
	ble.n	.L_0802e352
	strb	r1, [r2, #2]
.L_0802e352:
	movs	r1, #1
.L_0802e354:
	cmp	r1, #0
	beq.n	.L_0802e376
	mov	r1, r8
	lsls	r3, r1, #3
	adds	r2, r3, #4
	mov	r0, sl
	ldrb	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L_0802e368
	b.n	.L_0802e062
.L_0802e368:
	adds	r3, r0, r2
	movs	r1, #2
	ldrsb	r1, [r3, r1]
	mov	r0, r8
	bl	.L_0802e6b4
	b.n	.L_0802e062
.L_0802e376:
	ldr	r6, [pc, #188]
.L_0802e378:
	ldr	r2, [r4, #12]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802e3ea
	mov	r1, fp
	cmp	r1, #0
	bne.n	.L_0802e3be
	mov	r2, r8
	cmp	r2, #1
	beq.n	.L_0802e3ea
	lsls	r2, r2, #3
	adds	r3, r2, #4
	mov	r0, sl
	ldrb	r3, [r0, r3]
	cmp	r3, #0
	bne.n	.L_0802e39c
	b.n	.L_0802e062
.L_0802e39c:
	adds	r3, r0, r2
	movs	r2, #2
	ldrsh	r1, [r3, r2]
	mov	r0, r8
	bl	sub_0802e6fc
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_0802e3b0
	b.n	.L_0802e062
.L_0802e3b0:
	mov	r2, sl
	movs	r0, #10
	ldrsh	r1, [r2, r0]
	movs	r0, #1
	bl	sub_0802e6fc
	b.n	.L_0802e062
.L_0802e3be:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_0802e3ea
	lsls	r3, r3, #3
	adds	r1, r3, #4
	mov	r0, r9
	ldrb	r2, [r0, r1]
	movs	r3, #1
	eors	r2, r3
	strb	r2, [r0, r1]
	cmp	r2, #0
	beq.n	.L_0802e3e0
	mov	r2, sl
	adds	r3, r2, r1
	movs	r1, #1
	ldrsb	r1, [r3, r1]
	b.n	.L_0802e3e2
.L_0802e3e0:
	movs	r1, #8
.L_0802e3e2:
	mov	r0, r8
	bl	sub_0802e6d8
	b.n	.L_0802e062
.L_0802e3ea:
	ldr	r2, [r4, #12]
	movs	r3, #4
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802e402
	bl	sub_080144c0
	ldr	r0, [pc, #60]
	ldr	r1, [pc, #64]
	bl	sub_080132cc
	b.n	.L_0802df54
.L_0802e402:
	ldr	r2, [r6, #4]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802e426
	ldr	r3, [sp, #8]
	ldr	r2, [pc, #48]
	adds	r3, #1
	str	r3, [sp, #8]
	ldr	r0, [sp, #8]
	movs	r3, #7
	ands	r0, r3
	str	r0, [sp, #8]
	lsls	r3, r0, #1
	ldrh	r3, [r2, r3]
	movs	r1, #160
	lsls	r1, r1, #19
	strh	r3, [r1, #0]
.L_0802e426:
	movs	r0, #1
	str	r4, [sp, #0]
	bl	sub_08013560
	ldr	r4, [sp, #0]
	b.n	.L_0802e06c
	movs	r0, r0
	.4byte 0x03001150
	.4byte 0x00000012
	.4byte 0x08300000
	.2byte 0xf054
	.2byte 0x0802
.L_0802e444:
	push	{lr}
	cmp	r1, #0
	beq.n	.L_0802e4c0
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_0802e452
	negs	r3, r1
.L_0802e452:
	cmp	r3, #1
	bne.n	.L_0802e46e
	adds	r0, r0, r1
	cmp	r0, #0
	bge.n	.L_0802e462
	movs	r0, #230
	lsls	r0, r0, #1
	adds	r0, #255
.L_0802e462:
	movs	r3, #179
	lsls	r3, r3, #2
	cmp	r0, r3
	blt.n	.L_0802e4c0
	movs	r0, #0
	b.n	.L_0802e4c0
.L_0802e46e:
	cmp	r1, #0
	ble.n	.L_0802e498
	ldr	r1, [pc, #80]
	movs	r2, #0
	ldrh	r3, [r1, r2]
	b.n	.L_0802e484
.L_0802e47a:
	adds	r2, #1
	cmp	r2, #9
	bhi.n	.L_0802e4c0
	lsls	r3, r2, #1
	ldrh	r3, [r1, r3]
.L_0802e484:
	cmp	r0, r3
	bge.n	.L_0802e47a
	adds	r0, r3, #0
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	cmp	r0, r3
	bne.n	.L_0802e4c0
	movs	r0, #0
	b.n	.L_0802e4c0
.L_0802e498:
	cmp	r0, #0
	bne.n	.L_0802e4a2
	ldr	r3, [pc, #36]
	ldrh	r0, [r3, #16]
	b.n	.L_0802e4c0
.L_0802e4a2:
	ldr	r1, [pc, #32]
	movs	r2, #8
	ldrh	r3, [r1, #16]
	cmp	r0, r3
	ble.n	.L_0802e4b0
	adds	r0, r3, #0
	b.n	.L_0802e4c0
.L_0802e4b0:
	subs	r2, #1
	cmp	r2, #0
	blt.n	.L_0802e4c0
	lsls	r3, r2, #1
	ldrh	r3, [r1, r3]
	cmp	r0, r3
	ble.n	.L_0802e4b0
	adds	r0, r3, #0
.L_0802e4c0:
	pop	{pc}
	movs	r0, r0
	.2byte 0xf064
	.2byte 0x0802
.L_0802e4c8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #16]
	adds	r7, r2, #0
	ldr	r3, [r3, #40]
	movs	r2, #2
	ldrb	r4, [r3, #4]
	ldr	r3, [pc, #452]
	sub	sp, #4
	ldr	r3, [r3, #0]
	movs	r5, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802e4fa
	ldr	r3, [pc, #444]
	ldr	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r5, r3, #16
.L_0802e4fa:
	cmp	r4, #6
	beq.n	.L_0802e5ae
	cmp	r4, #6
	bhi.n	.L_0802e510
	cmp	r4, #4
	beq.n	.L_0802e5ae
	cmp	r4, #4
	bhi.n	.L_0802e56c
	cmp	r4, #3
	beq.n	.L_0802e528
	b.n	.L_0802e654
.L_0802e510:
	cmp	r4, #20
	beq.n	.L_0802e5ea
	cmp	r4, #20
	bhi.n	.L_0802e51e
	cmp	r4, #8
	beq.n	.L_0802e56c
	b.n	.L_0802e654
.L_0802e51e:
	cmp	r4, #44
	beq.n	.L_0802e56c
	cmp	r4, #88
	beq.n	.L_0802e56c
	b.n	.L_0802e654
.L_0802e528:
	lsls	r0, r0, #16
	mov	r8, r0
	lsls	r6, r1, #16
	movs	r4, #5
.L_0802e530:
	lsls	r5, r5, #16
	movs	r3, #0
	mov	r2, r8
	lsrs	r5, r5, #16
	movs	r0, #224
	str	r2, [r7, #0]
	str	r3, [r7, #4]
	str	r6, [r7, #8]
	adds	r1, r5, #0
	adds	r2, r7, #0
	lsls	r0, r0, #14
	str	r4, [sp, #0]
	bl	sub_0801489c
	movs	r3, #168
	lsls	r3, r3, #6
	ldr	r4, [sp, #0]
	adds	r3, #170
	adds	r5, r5, r3
	lsls	r5, r5, #16
	subs	r4, #1
	adds	r7, #16
	asrs	r5, r5, #16
	cmp	r4, #0
	bge.n	.L_0802e530
	movs	r4, #6
.L_0802e564:
	adds	r4, #1
	cmp	r4, #9
	ble.n	.L_0802e564
	b.n	.L_0802e69e
.L_0802e56c:
	lsls	r0, r0, #16
	mov	r8, r0
	lsls	r6, r1, #16
	movs	r4, #7
.L_0802e574:
	lsls	r5, r5, #16
	movs	r3, #0
	mov	r2, r8
	lsrs	r5, r5, #16
	movs	r0, #224
	str	r2, [r7, #0]
	str	r3, [r7, #4]
	str	r6, [r7, #8]
	adds	r1, r5, #0
	adds	r2, r7, #0
	lsls	r0, r0, #14
	str	r4, [sp, #0]
	bl	sub_0801489c
	movs	r3, #128
	ldr	r4, [sp, #0]
	lsls	r3, r3, #6
	adds	r5, r5, r3
	lsls	r5, r5, #16
	subs	r4, #1
	adds	r7, #16
	asrs	r5, r5, #16
	cmp	r4, #0
	bge.n	.L_0802e574
	movs	r4, #8
.L_0802e5a6:
	adds	r4, #1
	cmp	r4, #9
	ble.n	.L_0802e5a6
	b.n	.L_0802e69e
.L_0802e5ae:
	lsls	r0, r0, #16
	movs	r4, #0
	mov	r8, r0
	lsls	r6, r1, #16
.L_0802e5b6:
	lsls	r5, r5, #16
	movs	r3, #0
	mov	r2, r8
	lsrs	r5, r5, #16
	movs	r0, #224
	str	r2, [r7, #0]
	str	r3, [r7, #4]
	str	r6, [r7, #8]
	adds	r1, r5, #0
	adds	r2, r7, #0
	lsls	r0, r0, #14
	str	r4, [sp, #0]
	bl	sub_0801489c
	movs	r3, #200
	lsls	r3, r3, #5
	ldr	r4, [sp, #0]
	adds	r3, #153
	adds	r5, r5, r3
	lsls	r5, r5, #16
	adds	r4, #1
	adds	r7, #16
	asrs	r5, r5, #16
	cmp	r4, #9
	ble.n	.L_0802e5b6
	b.n	.L_0802e69e
.L_0802e5ea:
	movs	r2, #128
	lsls	r3, r5, #16
	lsls	r2, r2, #23
	adds	r3, r3, r2
	asrs	r5, r3, #16
	movs	r3, #160
	movs	r4, #0
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	lsls	r3, r3, #14
	mov	sl, r0
	mov	fp, r4
	mov	r8, r1
	mov	r9, r3
	adds	r6, r7, #0
.L_0802e608:
	mov	r2, sl
	lsls	r5, r5, #16
	str	r2, [r6, #0]
	mov	r3, fp
	lsrs	r5, r5, #16
	mov	r2, r8
	str	r3, [r6, #4]
	str	r2, [r6, #8]
	adds	r1, r5, #0
	adds	r2, r7, #0
	mov	r0, r9
	str	r4, [sp, #0]
	bl	sub_0801489c
	mov	r3, sl
	str	r3, [r6, #16]
	mov	r3, fp
	str	r3, [r6, #20]
	adds	r2, r7, #0
	mov	r3, r8
	adds	r2, #16
	str	r3, [r6, #24]
	adds	r1, r5, #0
	mov	r0, r9
	bl	sub_0801489c
	movs	r2, #128
	ldr	r4, [sp, #0]
	lsls	r2, r2, #8
	adds	r5, r5, r2
	lsls	r5, r5, #16
	adds	r4, #1
	adds	r6, #32
	adds	r7, #32
	asrs	r5, r5, #16
	cmp	r4, #1
	ble.n	.L_0802e608
	b.n	.L_0802e69e
.L_0802e654:
	movs	r2, #128
	lsls	r3, r5, #16
	lsls	r2, r2, #22
	adds	r3, r3, r2
	lsls	r0, r0, #16
	asrs	r5, r3, #16
	mov	r8, r0
	lsls	r6, r1, #16
	movs	r4, #3
.L_0802e666:
	mov	r3, r8
	lsls	r5, r5, #16
	str	r3, [r7, #0]
	lsrs	r5, r5, #16
	movs	r3, #0
	movs	r0, #224
	adds	r2, r7, #0
	str	r3, [r7, #4]
	str	r6, [r7, #8]
	adds	r1, r5, #0
	lsls	r0, r0, #14
	str	r4, [sp, #0]
	bl	sub_0801489c
	movs	r2, #128
	ldr	r4, [sp, #0]
	lsls	r2, r2, #7
	adds	r5, r5, r2
	lsls	r5, r5, #16
	subs	r4, #1
	adds	r7, #16
	asrs	r5, r5, #16
	cmp	r4, #0
	bge.n	.L_0802e666
	movs	r4, #5
.L_0802e698:
	subs	r4, #1
	cmp	r4, #0
	bge.n	.L_0802e698
.L_0802e69e:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03001150
	.2byte 0x1100
	.2byte 0x0300
.L_0802e6b4:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #16]
	movs	r3, #3
	ands	r3, r0
	lsls	r3, r3, #2
	adds	r4, r3, #0
	adds	r4, #40
	movs	r0, #9
.L_0802e6c8:
	ldr	r3, [r2, r4]
	subs	r0, #1
	strb	r1, [r3, #5]
	adds	r2, #56
	cmp	r0, #0
	bge.n	.L_0802e6c8
	pop	{pc}
	.align 2, 0
