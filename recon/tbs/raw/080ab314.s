.syntax unified
	.thumb
	.set sub_080022f4, 0x080022f4
	.set sub_08002304, 0x08002304
	.set sub_080030f8, 0x080030f8
	.set sub_08003f3c, 0x08003f3c
	.set sub_080041d8, 0x080041d8
	.set sub_08015010, 0x08015010
	.set sub_08015018, 0x08015018
	.set sub_08015038, 0x08015038
	.set sub_08015048, 0x08015048
	.set sub_08015060, 0x08015060
	.set sub_08015068, 0x08015068
	.set sub_08015078, 0x08015078
	.set sub_08015080, 0x08015080
	.set sub_08015090, 0x08015090
	.set sub_080150b0, 0x080150b0
	.set sub_08015120, 0x08015120
	.set sub_08015270, 0x08015270
	.set sub_08015278, 0x08015278
	.set sub_08015280, 0x08015280
	.set sub_080152a8, 0x080152a8
	.set sub_080153f8, 0x080153f8
	.set sub_08077008, 0x08077008
	.set sub_08077010, 0x08077010
	.set sub_080771b0, 0x080771b0
	.set sub_080771b8, 0x080771b8
	.set sub_080771c0, 0x080771c0
	.set sub_080771c8, 0x080771c8
	.set sub_08077208, 0x08077208
	.set sub_08077210, 0x08077210
	.set sub_080a1a40, 0x080a1a40
	.set sub_080a1ac0, 0x080a1ac0
	.set sub_080aa538, 0x080aa538
	.set sub_080aafb8, 0x080aafb8
	.set sub_080ab1f4, 0x080ab1f4
	.set sub_080ab21c, 0x080ab21c
	.set sub_080ab2ec, 0x080ab2ec
	.set sub_080ad5b4, 0x080ad5b4
	.set sub_080ad5f4, 0x080ad5f4
	.set sub_080ad608, 0x080ad608
	.set sub_080ae714, 0x080ae714
	.set sub_080b50f8, 0x080b50f8
	.set sub_080f9010, 0x080f9010
	.global Func_080ab314
	.thumb_func
Func_080ab314:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #660]
	ldr	r1, [r3, #0]
	sub	sp, #32
	str	r1, [sp, #20]
	subs	r3, #160
	ldr	r3, [r3, #0]
	movs	r2, #0
	str	r3, [sp, #16]
	str	r2, [sp, #12]
	str	r2, [sp, #8]
	ldr	r0, [r1, #48]
	bl	sub_08015278
	movs	r0, #1
	bl	sub_080030f8
	movs	r1, #134
	ldr	r3, [sp, #20]
	lsls	r1, r1, #1
	adds	r6, r3, r1
	ldr	r0, [r6, #0]
	bl	sub_08015060
	ldr	r5, [pc, #616]
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	adds	r5, #1
	bl	sub_08015080
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #16
	bl	sub_08015080
	movs	r5, #6
	movs	r0, #1
	movs	r1, #1
	movs	r2, #11
	movs	r3, #3
	str	r5, [sp, #0]
	bl	sub_080ab21c
	ldr	r2, [sp, #20]
	movs	r3, #10
	ldr	r0, [r2, #48]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #0
	movs	r3, #28
	str	r5, [sp, #4]
	bl	sub_080ab2ec
	movs	r1, #9
	movs	r2, #8
	movs	r3, #10
	movs	r0, #0
	str	r5, [sp, #0]
	bl	sub_08015010
	movs	r5, #2
	movs	r1, #12
	movs	r2, #22
	movs	r3, #7
	adds	r6, r0, #0
	movs	r0, #8
	str	r5, [sp, #0]
	bl	sub_08015010
	movs	r3, #3
	str	r0, [sp, #28]
	movs	r1, #9
	movs	r2, #22
	movs	r0, #8
	str	r5, [sp, #0]
	bl	sub_08015010
	str	r0, [sp, #24]
	bl	sub_080152a8
	ldr	r3, [pc, #504]
	movs	r7, #0
	movs	r5, #0
	mov	r8, r3
.L_080ab3ce:
	mov	r1, r8
	adds	r0, r5, r1
	lsls	r3, r5, #3
	adds	r1, r6, #0
	movs	r2, #0
	adds	r5, #1
	bl	sub_08015080
	cmp	r5, #6
	ble.n	.L_080ab3ce
	ldr	r3, [pc, #480]
	movs	r2, #1
	movs	r1, #0
	mov	fp, r2
	mov	r9, r3
	mov	r8, r1
.L_080ab3ee:
	ldr	r0, [sp, #24]
	bl	sub_08015060
	ldr	r0, [pc, #456]
	ldr	r1, [sp, #24]
	movs	r2, #0
	movs	r3, #0
	adds	r0, r7, r0
	bl	sub_08015078
	ldr	r1, [pc, #452]
	ldr	r0, [sp, #28]
	adds	r1, r7, r1
	bl	sub_080153f8
	mov	r2, fp
	movs	r3, #15
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	ldr	r2, [sp, #8]
	movs	r1, #0
	movs	r3, #6
	mov	sl, r0
	adds	r0, r6, #0
	bl	sub_080ab1f4
	mov	r3, fp
	str	r3, [sp, #0]
	movs	r3, #14
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r1, #0
	adds	r2, r7, #0
	movs	r3, #6
	bl	sub_080ab1f4
	str	r7, [sp, #8]
	b.n	.L_080ab4b8
.L_080ab43a:
	mov	r1, r9
	ldr	r2, [r1, #0]
	movs	r3, #96
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ab45a
	subs	r7, #1
	adds	r0, r7, #0
	movs	r1, #7
	bl	sub_080aa538
	adds	r7, r0, #0
	movs	r0, #111
	bl	sub_080f9010
	b.n	.L_080ab4ec
.L_080ab45a:
	ldr	r1, [pc, #368]
	ldr	r2, [r1, #0]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ab474
	movs	r0, #113
	bl	sub_080f9010
	movs	r2, #2
	negs	r2, r2
	str	r2, [sp, #12]
	b.n	.L_080ab4ec
.L_080ab474:
	ldr	r2, [r1, #0]
	movs	r3, #6
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ab48c
	movs	r0, #113
	bl	sub_080f9010
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #12]
	b.n	.L_080ab4ec
.L_080ab48c:
	ldr	r3, [r1, #0]
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ab4b8
	bl	sub_08015048
	cmp	r0, #0
	beq.n	.L_080ab4b2
	adds	r7, #1
	adds	r0, r7, #0
	movs	r1, #7
	bl	sub_080aa538
	adds	r7, r0, #0
	movs	r0, #112
	bl	sub_080f9010
	b.n	.L_080ab4ec
.L_080ab4b2:
	movs	r0, #111
	bl	sub_080f9010
.L_080ab4b8:
	ldrh	r1, [r6, #14]
	adds	r1, r1, r7
	lsls	r1, r1, #3
	movs	r0, #12
	adds	r1, #8
	negs	r0, r0
	bl	sub_080a1a40
	movs	r0, #1
	bl	sub_080030f8
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #144
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ab43a
	adds	r7, #1
	adds	r0, r7, #0
	movs	r1, #7
	bl	sub_080aa538
	adds	r7, r0, #0
	movs	r0, #111
	bl	sub_080f9010
.L_080ab4ec:
	ldr	r1, [sp, #16]
	ldr	r2, [pc, #224]
	adds	r5, r1, r2
	ldrh	r3, [r5, #0]
	cmp	r3, #99
	beq.n	.L_080ab502
	adds	r0, r3, #0
	bl	sub_08003f3c
	movs	r3, #99
	strh	r3, [r5, #0]
.L_080ab502:
	ldr	r5, [pc, #208]
	ldr	r1, [pc, #208]
	ldr	r3, [r5, #0]
	mov	r2, r8
	adds	r3, r3, r1
	strb	r2, [r3, #0]
	ldr	r0, [sp, #28]
	bl	sub_08015060
	mov	r1, sl
	ldr	r3, [r1, #0]
	mov	r2, r8
	mov	r1, r8
	strh	r1, [r3, #26]
	strh	r2, [r3, #24]
	strh	r2, [r3, #20]
	mov	r1, sl
	mov	r3, r8
	str	r3, [r1, #0]
	ldr	r2, [sp, #12]
	cmp	r2, #0
	bne.n	.L_080ab530
	b.n	.L_080ab3ee
.L_080ab530:
	ldr	r1, [pc, #168]
	ldr	r3, [r5, #0]
	movs	r2, #1
	adds	r3, r3, r1
	strb	r2, [r3, #0]
	ldr	r0, [sp, #24]
	bl	sub_08015278
	ldr	r0, [sp, #28]
	bl	sub_08015278
	movs	r0, #1
	bl	sub_080030f8
	movs	r1, #1
	ldr	r0, [sp, #24]
	bl	sub_08015018
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08015018
	movs	r1, #1
	ldr	r0, [sp, #28]
	bl	sub_08015018
	bl	sub_080152a8
	movs	r3, #2
	ldr	r2, [sp, #12]
	negs	r3, r3
	cmp	r2, r3
	bne.n	.L_080ab59a
	ldr	r1, [sp, #20]
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r0, [r3, #0]
	bl	sub_08015060
	ldr	r3, [sp, #20]
	ldr	r0, [r3, #48]
	bl	sub_08015060
	ldr	r1, [sp, #20]
	ldr	r0, [r1, #16]
	bl	sub_08015060
	ldr	r2, [pc, #72]
	ldr	r3, [r5, #0]
	adds	r3, r3, r2
	movs	r2, #0
	strb	r2, [r3, #0]
.L_080ab59a:
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #64]
	bl	sub_080041d8
	ldr	r0, [sp, #12]
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x00000c30
	.4byte 0x00000c32
	.4byte 0x03001b04
	.4byte 0x00000c39
	.4byte 0x03001c94
	.4byte 0x000012b6
	.4byte 0x03001e8c
	.4byte 0x000012f8
	.4byte 0x00000ea6
	.2byte 0x19a1
	.2byte 0x080a
