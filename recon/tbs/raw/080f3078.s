.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080072f0, 0x080072f0
	.set sub_080072fc, 0x080072fc
	.set sub_080f3898, 0x080f3898
	.set sub_080f38ac, 0x080f38ac
	.global Unnamed_080f3078
	.global Func_080f3078
	.thumb_func
Unnamed_080f3078:
Func_080f3078:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	sl, r1
	mov	r8, r2
	movs	r1, #128
	adds	r2, r3, #0
	movs	r3, #128
	sub	sp, #40
	lsls	r1, r1, #2
	lsls	r3, r3, #8
	str	r1, [sp, #36]
	cmp	r0, r3
	bne.n	.L_080f30a0
	mov	r1, sl
	ldrh	r0, [r1, #0]
.L_080f30a0:
	cmp	r2, #1
	bne.n	.L_080f30ac
	movs	r3, #128
	lsls	r3, r3, #1
	str	r3, [sp, #36]
	b.n	.L_080f30c2
.L_080f30ac:
	cmp	r2, #2
	bne.n	.L_080f30c2
	movs	r1, #192
	lsls	r1, r1, #3
	add	r8, r1
	movs	r1, #128
	movs	r3, #128
	lsls	r1, r1, #1
	lsls	r3, r3, #2
	str	r1, [sp, #36]
	add	sl, r3
.L_080f30c2:
	movs	r3, #128
	lsls	r3, r3, #8
	cmp	r0, r3
	bcs.n	.L_080f311a
	ldr	r2, [pc, #56]
	adds	r3, r0, #0
	ands	r3, r2
	movs	r2, #2
	mov	r1, r8
	add	r8, r2
	ldr	r2, [pc, #48]
	strh	r3, [r1, #0]
	adds	r3, r0, #0
	ands	r3, r2
	mov	r1, r8
	lsls	r3, r3, #5
	strh	r3, [r1, #0]
	ldr	r3, [pc, #36]
	movs	r2, #2
	add	r8, r2
	ands	r0, r3
	lsls	r3, r0, #10
	mov	r1, r8
	strh	r3, [r1, #0]
	ldr	r3, [sp, #36]
	subs	r3, #1
	add	r8, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r2, r2, #1
	movs	r4, #128
	lsls	r4, r4, #24
	b.n	.L_080f3110
	.4byte 0x00007c00
	.4byte 0x000003e0
	.2byte 0x001f
	.2byte 0x0000
.L_080f3110:
	mov	r0, r8
	lsrs	r2, r2, #1
	ldr	r3, [pc, #824]
	subs	r0, #6
	b.n	.L_080f375e
.L_080f311a:
	movs	r3, #128
	lsls	r3, r3, #13
	cmp	r0, r3
	bcc.n	.L_080f3124
	b.n	.L_080f3546
.L_080f3124:
	ldr	r1, [pc, #812]
	adds	r0, r0, r1
	cmp	r0, #6
	bls.n	.L_080f312e
	b.n	.L_080f34f8
.L_080f312e:
	ldr	r2, [pc, #808]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080f3154
	.4byte 0x080f31a2
	.4byte 0x080f323e
	.4byte 0x080f32bc
	.4byte 0x080f3356
	.4byte 0x080f33d8
	.2byte 0x346c
	.2byte 0x080f
	movs	r2, #0
	ldr	r3, [sp, #36]
	mov	r9, r2
	cmp	r9, r3
	bcc.n	.L_080f3160
	b.n	.L_080f3766
.L_080f3160:
	ldr	r6, [pc, #760]
	mov	r5, r8
.L_080f3164:
	mov	r1, sl
	ldrh	r4, [r1, #0]
	movs	r3, #248
	lsls	r0, r4, #11
	lsls	r3, r3, #8
	movs	r2, #2
	ands	r0, r3
	movs	r3, #248
	lsls	r3, r3, #9
	add	sl, r2
	lsls	r2, r4, #7
	ands	r2, r3
	movs	r3, #248
	lsls	r3, r3, #7
	ands	r3, r4
	adds	r0, r0, r2
	adds	r0, r0, r3
	movs	r1, #7
	bl	sub_080072fc
	adds	r4, r0, #0
	strh	r4, [r5, #0]
	strh	r4, [r5, #2]
	strh	r4, [r5, #4]
	movs	r3, #1
	ldr	r1, [sp, #36]
	add	r9, r3
	adds	r5, #6
	cmp	r9, r1
	bcc.n	.L_080f3164
	b.n	.L_080f3766
	movs	r2, #0
	ldr	r3, [sp, #36]
	mov	r9, r2
	cmp	r9, r3
	bcc.n	.L_080f31ae
	b.n	.L_080f3766
.L_080f31ae:
	movs	r1, #31
	ldr	r2, [pc, #684]
	mov	fp, r1
.L_080f31b4:
	mov	r3, sl
	ldrh	r4, [r3, #0]
	movs	r1, #2
	adds	r7, r4, #0
	mov	r3, fp
	lsrs	r0, r4, #5
	ands	r7, r3
	ands	r0, r3
	add	sl, r1
	lsrs	r3, r4, #10
	mov	r1, fp
	ands	r3, r1
	adds	r0, r7, r0
	adds	r0, r0, r3
	str	r2, [sp, #0]
	ldr	r3, [pc, #648]
	movs	r1, #10
	bl	sub_080072f0
	adds	r4, r0, #0
	lsls	r3, r4, #2
	adds	r7, r3, #5
	lsls	r3, r4, #1
	adds	r3, r3, r4
	adds	r5, r3, #5
	adds	r6, r5, #0
	ldr	r2, [sp, #0]
	cmp	r7, #7
	bgt.n	.L_080f31f0
	movs	r7, #8
.L_080f31f0:
	cmp	r5, #7
	bgt.n	.L_080f31fc
	movs	r6, #8
	cmp	r5, #7
	bgt.n	.L_080f31fc
	movs	r5, #8
.L_080f31fc:
	cmp	r7, #28
	ble.n	.L_080f3202
	movs	r7, #28
.L_080f3202:
	cmp	r6, #28
	ble.n	.L_080f3208
	movs	r6, #28
.L_080f3208:
	cmp	r5, #28
	ble.n	.L_080f320e
	movs	r5, #28
.L_080f320e:
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r3, #2
	add	r8, r3
	lsls	r3, r6, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r3, #2
	add	r8, r3
	lsls	r3, r7, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r3, #2
	add	r8, r3
	movs	r1, #1
	ldr	r3, [sp, #36]
	add	r9, r1
	cmp	r9, r3
	bcc.n	.L_080f31b4
	b.n	.L_080f3766
	movs	r1, #0
	ldr	r2, [sp, #36]
	mov	r9, r1
	cmp	r9, r2
	bcc.n	.L_080f324a
	b.n	.L_080f3766
.L_080f324a:
	movs	r3, #31
	mov	fp, r3
.L_080f324e:
	mov	r1, sl
	ldrh	r4, [r1, #0]
	mov	r3, fp
	adds	r7, r4, #0
	lsrs	r6, r4, #5
	lsrs	r5, r4, #10
	ands	r6, r3
	ands	r7, r3
	movs	r2, #2
	movs	r1, #3
	ands	r5, r3
	adds	r0, r6, #0
	lsrs	r3, r7, #1
	subs	r7, r7, r3
	add	sl, r2
	bl	sub_080022ec
	adds	r7, #6
	subs	r6, r6, r0
	adds	r0, r7, #0
	bl	sub_080f3898
	adds	r6, #4
	adds	r7, r0, #0
	adds	r0, r6, #0
	bl	sub_080f3898
	subs	r5, #6
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_080f3898
	ldr	r2, [pc, #468]
	adds	r5, r0, #0
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	ldr	r2, [pc, #452]
	lsls	r3, r6, #1
	ldrh	r3, [r2, r3]
	mov	r2, r8
	strh	r3, [r2, #2]
	ldr	r2, [pc, #448]
	lsls	r3, r7, #1
	ldrh	r3, [r2, r3]
	strh	r3, [r1, #4]
	ldr	r1, [sp, #36]
	movs	r3, #1
	movs	r2, #6
	add	r9, r3
	add	r8, r2
	cmp	r9, r1
	bcc.n	.L_080f324e
	b.n	.L_080f3766
	movs	r2, #0
	ldr	r3, [sp, #36]
	mov	r9, r2
	cmp	r9, r3
	bcc.n	.L_080f32c8
	b.n	.L_080f3766
.L_080f32c8:
	ldr	r1, [pc, #412]
	mov	fp, r1
.L_080f32cc:
	mov	r2, sl
	ldrh	r4, [r2, #0]
	movs	r1, #31
	adds	r7, r4, #0
	movs	r3, #2
	lsrs	r6, r4, #5
	lsrs	r5, r4, #10
	ands	r7, r1
	add	sl, r3
	ands	r6, r1
	ands	r5, r1
	cmp	r7, #9
	bgt.n	.L_080f32e8
	movs	r7, #10
.L_080f32e8:
	cmp	r6, #15
	bgt.n	.L_080f32ee
	movs	r6, #16
.L_080f32ee:
	cmp	r5, #15
	bgt.n	.L_080f32f4
	movs	r5, #16
.L_080f32f4:
	cmp	r7, #28
	ble.n	.L_080f32fa
	movs	r7, #28
.L_080f32fa:
	cmp	r6, #24
	ble.n	.L_080f3300
	movs	r6, #24
.L_080f3300:
	cmp	r5, #26
	ble.n	.L_080f3306
	movs	r5, #26
.L_080f3306:
	adds	r0, r7, #0
	bl	sub_080f3898
	adds	r6, #2
	adds	r7, r0, #0
	adds	r0, r6, #0
	bl	sub_080f3898
	adds	r5, #2
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_080f3898
	adds	r5, r0, #0
	mov	r2, fp
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r2, #2
	mov	r1, fp
	lsls	r3, r6, #1
	ldrh	r3, [r1, r3]
	add	r8, r2
	mov	r2, r8
	strh	r3, [r2, #0]
	movs	r3, #2
	add	r8, r3
	lsls	r3, r7, #1
	ldrh	r3, [r1, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	ldr	r1, [sp, #36]
	movs	r3, #1
	movs	r2, #2
	add	r9, r3
	add	r8, r2
	cmp	r9, r1
	bcc.n	.L_080f32cc
	b.n	.L_080f3766
	movs	r2, #0
	ldr	r3, [sp, #36]
	mov	r9, r2
	cmp	r9, r3
	bcc.n	.L_080f3362
	b.n	.L_080f3766
.L_080f3362:
	ldr	r1, [pc, #256]
	mov	fp, r1
.L_080f3366:
	mov	r2, sl
	ldrh	r4, [r2, #0]
	movs	r1, #31
	adds	r7, r4, #0
	lsrs	r6, r4, #5
	lsrs	r5, r4, #10
	ands	r7, r1
	ands	r6, r1
	ands	r5, r1
	adds	r0, r7, r6
	movs	r1, #3
	movs	r3, #2
	adds	r0, r0, r5
	add	sl, r3
	bl	sub_080022ec
	bl	sub_080f3898
	asrs	r3, r7, #1
	adds	r7, r3, r0
	asrs	r3, r6, #1
	adds	r6, r3, r0
	asrs	r3, r5, #1
	adds	r5, r3, r0
	adds	r0, r7, #0
	bl	sub_080f3898
	adds	r7, r0, #0
	adds	r0, r6, #0
	bl	sub_080f3898
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_080f3898
	adds	r5, r0, #0
	mov	r2, fp
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	lsls	r3, r6, #1
	ldrh	r3, [r2, r3]
	mov	r2, r8
	strh	r3, [r2, #2]
	mov	r1, fp
	lsls	r3, r7, #1
	ldrh	r3, [r1, r3]
	strh	r3, [r2, #4]
	movs	r1, #1
	ldr	r2, [sp, #36]
	movs	r3, #6
	add	r9, r1
	add	r8, r3
	cmp	r9, r2
	bcc.n	.L_080f3366
	b.n	.L_080f3766
	movs	r3, #0
	ldr	r1, [sp, #36]
	mov	r9, r3
	cmp	r9, r1
	bcc.n	.L_080f33e4
	b.n	.L_080f3766
.L_080f33e4:
	movs	r2, #31
	mov	fp, r2
.L_080f33e8:
	mov	r3, sl
	ldrh	r4, [r3, #0]
	mov	r2, fp
	lsrs	r6, r4, #5
	lsrs	r5, r4, #10
	adds	r7, r4, #0
	ands	r6, r2
	ands	r5, r2
	ands	r7, r2
	asrs	r3, r6, #3
	asrs	r2, r5, #3
	adds	r3, r3, r2
	adds	r7, r7, r3
	movs	r1, #2
	adds	r0, r7, #0
	add	sl, r1
	bl	sub_080f3898
	movs	r1, #3
	adds	r7, r0, #0
	adds	r0, r6, #0
	bl	sub_080022ec
	movs	r1, #3
	subs	r6, r6, r0
	adds	r0, r5, #0
	bl	sub_080022ec
	ldr	r1, [pc, #68]
	subs	r5, r5, r0
	lsls	r3, r5, #1
	ldrh	r3, [r1, r3]
	mov	r2, r8
	strh	r3, [r2, #0]
	lsls	r3, r6, #1
	ldrh	r3, [r1, r3]
	mov	r1, r8
	strh	r3, [r1, #2]
	ldr	r2, [pc, #40]
	lsls	r3, r7, #1
	ldrh	r3, [r2, r3]
	mov	r2, r8
	strh	r3, [r2, #4]
	movs	r1, #1
	ldr	r2, [sp, #36]
	movs	r3, #6
	add	r9, r1
	add	r8, r3
	cmp	r9, r2
	bcc.n	.L_080f33e8
	b.n	.L_080f3766
	movs	r0, r0
	.4byte 0x040000d4
	.4byte 0xfffeffff
	.4byte 0x080f3138
	.4byte 0x03000380
	.4byte 0x080f3a2e
	.4byte 0x080f3a6e
	.2byte 0x39ee
	.2byte 0x080f
	movs	r3, #0
	ldr	r1, [sp, #36]
	mov	r9, r3
	cmp	r9, r1
	bcc.n	.L_080f3478
	b.n	.L_080f3766
.L_080f3478:
	movs	r2, #31
	mov	fp, r2
.L_080f347c:
	mov	r3, sl
	ldrh	r4, [r3, #0]
	mov	r2, fp
	adds	r7, r4, #0
	lsrs	r6, r4, #5
	ands	r7, r2
	ands	r6, r2
	movs	r1, #2
	lsrs	r3, r7, #1
	lsrs	r5, r4, #10
	adds	r0, r6, #0
	add	sl, r1
	movs	r1, #3
	ands	r5, r2
	subs	r7, r7, r3
	bl	sub_080022ec
	adds	r7, #6
	subs	r6, r6, r0
	adds	r0, r7, #0
	bl	sub_080f3898
	adds	r6, #4
	adds	r7, r0, #0
	adds	r0, r6, #0
	bl	sub_080f3898
	subs	r5, #6
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_080f3898
	ldr	r2, [pc, #44]
	adds	r5, r0, #0
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	ldr	r2, [pc, #36]
	lsls	r3, r6, #1
	ldrh	r3, [r2, r3]
	mov	r2, r8
	strh	r3, [r2, #2]
	ldr	r2, [pc, #32]
	lsls	r3, r7, #1
	ldrh	r3, [r2, r3]
	strh	r3, [r1, #4]
	ldr	r1, [sp, #36]
	movs	r3, #1
	movs	r2, #6
	add	r9, r3
	add	r8, r2
	cmp	r9, r1
	bcc.n	.L_080f347c
	b.n	.L_080f3766
	movs	r0, r0
	.4byte 0x080f3a6e
	.4byte 0x080f3a2e
	.2byte 0x39ee
	.2byte 0x080f
.L_080f34f8:
	movs	r2, #0
	ldr	r3, [sp, #36]
	mov	r9, r2
	cmp	r9, r3
	bcc.n	.L_080f3504
	b.n	.L_080f3766
.L_080f3504:
	ldr	r5, [pc, #8]
	ldr	r0, [pc, #12]
	ldr	r2, [pc, #12]
	mov	r1, r8
	b.n	.L_080f351c
	movs	r0, r0
	.4byte 0x00007c00
	.4byte 0x000003e0
	.2byte 0x001f
	.2byte 0x0000
.L_080f351c:
	mov	r3, sl
	ldrh	r4, [r3, #0]
	movs	r3, #2
	add	sl, r3
	adds	r3, r4, #0
	ands	r3, r5
	strh	r3, [r1, #0]
	adds	r3, r4, #0
	ands	r3, r0
	lsls	r3, r3, #5
	ands	r4, r2
	strh	r3, [r1, #2]
	lsls	r3, r4, #10
	strh	r3, [r1, #4]
	movs	r3, #1
	add	r9, r3
	ldr	r3, [sp, #36]
	adds	r1, #6
	cmp	r9, r3
	bcc.n	.L_080f351c
	b.n	.L_080f3766
.L_080f3546:
	movs	r3, #128
	lsls	r3, r3, #14
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080f35e6
	movs	r3, #31
	str	r0, [sp, #32]
	adds	r1, r0, #0
	lsrs	r2, r0, #5
	lsrs	r0, r0, #10
	ands	r1, r3
	mov	fp, r0
	ands	r2, r3
	str	r1, [sp, #32]
	mov	r1, fp
	ands	r1, r3
	str	r2, [sp, #28]
	ldr	r3, [sp, #36]
	movs	r2, #0
	mov	r9, r2
	mov	fp, r1
	cmp	r9, r3
	bcc.n	.L_080f3576
	b.n	.L_080f3766
.L_080f3576:
	mov	r1, sl
	ldrh	r4, [r1, #0]
	movs	r3, #248
	lsls	r0, r4, #11
	lsls	r3, r3, #8
	movs	r2, #2
	ands	r0, r3
	movs	r3, #248
	lsls	r3, r3, #9
	add	sl, r2
	lsls	r2, r4, #7
	ands	r2, r3
	movs	r3, #248
	lsls	r3, r3, #7
	ands	r3, r4
	adds	r0, r0, r2
	adds	r0, r0, r3
	movs	r1, #96
	ldr	r3, [pc, #328]
	bl	sub_080072f0
	ldr	r1, [sp, #32]
	adds	r4, r0, #0
	adds	r7, r1, #0
	muls	r7, r4
	ldr	r2, [sp, #28]
	adds	r0, r7, #0
	adds	r6, r2, #0
	muls	r6, r4
	mov	r5, fp
	muls	r5, r4
	bl	sub_080f38ac
	adds	r7, r0, #0
	adds	r0, r6, #0
	bl	sub_080f38ac
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_080f38ac
	mov	r3, r8
	mov	r1, r8
	mov	r2, r8
	adds	r5, r0, #0
	strh	r5, [r3, #0]
	strh	r6, [r1, #2]
	strh	r7, [r2, #4]
	movs	r1, #1
	ldr	r2, [sp, #36]
	movs	r3, #6
	add	r9, r1
	add	r8, r3
	cmp	r9, r2
	bcc.n	.L_080f3576
	b.n	.L_080f3766
.L_080f35e6:
	movs	r3, #128
	lsls	r3, r3, #15
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_080f35f2
	b.n	.L_080f36f0
.L_080f35f2:
	movs	r3, #31
	str	r0, [sp, #24]
	adds	r1, r0, #0
	lsrs	r2, r0, #5
	lsrs	r0, r0, #10
	ands	r1, r3
	mov	fp, r0
	ands	r2, r3
	str	r1, [sp, #24]
	mov	r1, fp
	ands	r1, r3
	str	r2, [sp, #20]
	ldr	r3, [sp, #36]
	movs	r2, #0
	mov	r9, r2
	mov	fp, r1
	cmp	r9, r3
	bcc.n	.L_080f3618
	b.n	.L_080f3766
.L_080f3618:
	ldr	r2, [sp, #20]
	ldr	r1, [sp, #24]
	ldr	r3, [sp, #24]
	adds	r1, r1, r2
	str	r1, [sp, #4]
	lsls	r1, r2, #16
	mov	r2, fp
	lsls	r3, r3, #16
	lsls	r2, r2, #16
	str	r3, [sp, #16]
	str	r1, [sp, #12]
	str	r2, [sp, #8]
.L_080f3630:
	mov	r3, sl
	ldrh	r4, [r3, #0]
	movs	r2, #31
	adds	r7, r4, #0
	lsrs	r0, r4, #5
	ands	r7, r2
	ands	r0, r2
	lsrs	r3, r4, #10
	movs	r1, #2
	ands	r3, r2
	add	sl, r1
	adds	r0, r7, r0
	ldr	r1, [sp, #4]
	adds	r0, r0, r3
	add	r1, fp
	ldr	r3, [pc, #148]
	lsls	r0, r0, #4
	bl	sub_080072f0
	ldr	r3, [sp, #24]
	adds	r4, r0, #0
	adds	r0, r3, #0
	muls	r0, r4
	ldr	r2, [sp, #16]
	lsrs	r0, r0, #4
	lsls	r0, r0, #16
	asrs	r1, r2, #4
	ldr	r3, [pc, #128]
	mov	ip, pc
	bx	r3
	ldr	r1, [sp, #20]
	adds	r7, r0, #0
	adds	r0, r1, #0
	muls	r0, r4
	ldr	r2, [sp, #12]
	lsrs	r0, r0, #4
	lsls	r0, r0, #16
	asrs	r1, r2, #4
	mov	ip, pc
	bx	r3
	adds	r6, r0, #0
	mov	r0, fp
	muls	r0, r4
	ldr	r3, [sp, #8]
	lsrs	r0, r0, #4
	asrs	r1, r3, #4
	lsls	r0, r0, #16
	ldr	r3, [pc, #88]
	mov	ip, pc
	bx	r3
	adds	r5, r0, #0
	lsrs	r0, r7, #16
	bl	sub_080f3898
	adds	r7, r0, #0
	lsrs	r0, r6, #16
	bl	sub_080f3898
	adds	r6, r0, #0
	lsrs	r0, r5, #16
	bl	sub_080f3898
	ldr	r1, [pc, #60]
	adds	r5, r0, #0
	lsls	r3, r5, #1
	ldrh	r3, [r1, r3]
	mov	r2, r8
	strh	r3, [r2, #0]
	movs	r3, #2
	add	r8, r3
	lsls	r3, r6, #1
	ldrh	r3, [r1, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	ldr	r1, [pc, #36]
	movs	r2, #2
	lsls	r3, r7, #1
	ldrh	r3, [r1, r3]
	add	r8, r2
	mov	r2, r8
	strh	r3, [r2, #0]
	movs	r1, #1
	ldr	r2, [sp, #36]
	movs	r3, #2
	add	r9, r1
	add	r8, r3
	cmp	r9, r2
	bcc.n	.L_080f3630
	b.n	.L_080f3766
	movs	r0, r0
	.4byte 0x03000380
	.4byte 0x03000118
	.2byte 0x39ee
	.2byte 0x080f
.L_080f36f0:
	movs	r3, #128
	lsls	r3, r3, #16
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080f3746
	movs	r3, #0
	ldr	r1, [sp, #36]
	mov	r9, r3
	cmp	r9, r1
	bcs.n	.L_080f3766
	ldr	r5, [pc, #8]
	ldr	r0, [pc, #12]
	ldr	r2, [pc, #12]
	mov	r1, r8
	b.n	.L_080f371c
	movs	r0, r0
	.4byte 0x00007c00
	.4byte 0x000003e0
	.2byte 0x001f
	.2byte 0x0000
.L_080f371c:
	mov	r3, sl
	ldrh	r4, [r3, #0]
	movs	r3, #2
	add	sl, r3
	adds	r3, r4, #0
	ands	r3, r5
	strh	r3, [r1, #0]
	adds	r3, r4, #0
	ands	r3, r0
	lsls	r3, r3, #5
	ands	r4, r2
	strh	r3, [r1, #2]
	lsls	r3, r4, #10
	strh	r3, [r1, #4]
	movs	r3, #1
	add	r9, r3
	ldr	r3, [sp, #36]
	adds	r1, #6
	cmp	r9, r3
	bcc.n	.L_080f371c
	b.n	.L_080f3766
.L_080f3746:
	cmp	r2, #2
	bne.n	.L_080f3750
	movs	r1, #192
	lsls	r1, r1, #3
	adds	r0, r0, r1
.L_080f3750:
	ldr	r3, [sp, #36]
	lsls	r2, r3, #1
	adds	r2, r2, r3
	movs	r4, #132
	lsls	r4, r4, #24
	lsrs	r2, r2, #1
	ldr	r3, [pc, #24]
.L_080f375e:
	mov	r1, r8
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_080f3766:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x040000d4
