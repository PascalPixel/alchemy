.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_08002322, 0x08002322
	.set sub_08002df0, 0x08002df0
	.set sub_080030f8, 0x080030f8
	.set sub_0800352c, 0x0800352c
	.set sub_080041d8, 0x080041d8
	.set sub_08004970, 0x08004970
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080072f8, 0x080072f8
	.set sub_08015018, 0x08015018
	.set sub_08015038, 0x08015038
	.set sub_08015048, 0x08015048
	.set sub_08015060, 0x08015060
	.set sub_08015068, 0x08015068
	.set sub_08015080, 0x08015080
	.set sub_080150b8, 0x080150b8
	.set sub_08015120, 0x08015120
	.set sub_08015270, 0x08015270
	.set sub_08015278, 0x08015278
	.set sub_08015280, 0x08015280
	.set sub_080152a8, 0x080152a8
	.set sub_08077008, 0x08077008
	.set sub_08077010, 0x08077010
	.set sub_080771a8, 0x080771a8
	.set sub_080771b0, 0x080771b0
	.set sub_080771b8, 0x080771b8
	.set sub_080a10d0, 0x080a10d0
	.set sub_080a1114, 0x080a1114
	.set sub_080a1a40, 0x080a1a40
	.set sub_080a1ac0, 0x080a1ac0
	.set sub_080aa538, 0x080aa538
	.set sub_080aae14, 0x080aae14
	.set sub_080aca04, 0x080aca04
	.set sub_080acab8, 0x080acab8
	.set sub_080ad5b4, 0x080ad5b4
	.set sub_080ad5f4, 0x080ad5f4
	.set sub_080f9010, 0x080f9010
	.global Func_080ad6d4
	.thumb_func
Func_080ad6d4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #112]
	ldr	r3, [r3, #0]
	sub	sp, #136
	mov	r9, r3
	movs	r1, #1
	movs	r2, #0
	movs	r3, #2
	mov	fp, r0
	movs	r0, #0
	str	r0, [sp, #100]
	str	r1, [sp, #96]
	str	r3, [sp, #88]
	str	r2, [sp, #92]
	mov	r4, r9
	ldr	r2, [r4, #20]
	movs	r3, #13
	mov	r0, sp
	strb	r3, [r2, #5]
	movs	r7, #0
	adds	r0, #128
	movs	r3, #165
	str	r7, [sp, #128]
	str	r0, [sp, #24]
	lsls	r3, r3, #1
	str	r7, [r0, #4]
	ldr	r1, [pc, #48]
	movs	r2, #3
	add	r3, r9
.L_080ad71a:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L_080ad71a
	mov	r1, r9
	movs	r3, #29
	ldrsb	r3, [r1, r3]
	movs	r4, #154
	lsls	r4, r4, #1
	ldr	r2, [pc, #28]
	lsls	r3, r3, #1
	adds	r3, r3, r4
	mov	r0, r9
	strh	r2, [r0, r3]
	movs	r3, #29
	ldrsb	r3, [r0, r3]
	movs	r0, #162
	ldr	r1, [pc, #16]
	lsls	r0, r0, #1
	lsls	r3, r3, #1
	b.n	.L_080ad758
	movs	r0, r0
	.4byte 0x000000c8
	.4byte 0x00000078
	.4byte 0x00000020
	.2byte 0x1f2c
	.2byte 0x0300
.L_080ad758:
	adds	r3, r3, r0
	mov	r2, r9
	strh	r1, [r2, r3]
	movs	r3, #28
	ldrsb	r3, [r2, r3]
	ldr	r2, [pc, #60]
	lsls	r3, r3, #1
	adds	r3, r3, r4
	mov	r4, r9
	strh	r2, [r4, r3]
	movs	r3, #28
	ldrsb	r3, [r4, r3]
	lsls	r3, r3, #1
	adds	r3, r3, r0
	mov	r0, r9
	strh	r1, [r0, r3]
	movs	r5, #134
	ldr	r0, [r0, #48]
	lsls	r5, r5, #1
	bl	sub_08015278
	add	r5, r9
	movs	r0, #1
	bl	sub_080030f8
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_080a1114
	movs	r3, #5
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r1, #0
	b.n	.L_080ad7a4
	movs	r0, r0
	.2byte 0x0010
	.2byte 0x0000
.L_080ad7a4:
	movs	r2, #0
	adds	r0, r5, #0
	movs	r3, #30
	bl	sub_080a10d0
	mov	r1, sp
	movs	r7, #1
	adds	r1, #120
	mov	r2, fp
	str	r7, [sp, #120]
	str	r1, [sp, #28]
	str	r7, [r1, #4]
	cmp	r2, #1
	bls.n	.L_080ad7c2
	b.n	.L_080ad922
.L_080ad7c2:
	movs	r0, #96
	bl	sub_08004970
	movs	r5, #166
	lsls	r5, r5, #1
	str	r0, [sp, #84]
	adds	r0, r5, #0
	bl	sub_08004970
	movs	r3, #0
	str	r3, [sp, #76]
	str	r3, [sp, #72]
	ldr	r3, [pc, #932]
	add	r3, r9
	ldrb	r6, [r3, #0]
	ldr	r3, [pc, #928]
	add	r3, r9
	ldrb	r3, [r3, #0]
	str	r3, [sp, #80]
	movs	r3, #149
	lsls	r3, r3, #2
	movs	r4, #31
	add	r3, r9
	mov	r8, r4
	ldrb	r3, [r3, #0]
	mov	sl, r0
	mov	r0, r8
	ands	r0, r3
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #8
	ands	r3, r2
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	mov	r8, r0
	adds	r0, r6, #0
	str	r3, [sp, #68]
	bl	sub_08077008
	adds	r7, r0, #0
	ldr	r3, [pc, #876]
	adds	r1, r7, #0
	adds	r2, r5, #0
	mov	r0, sl
	bl	sub_080072f0
	adds	r0, r6, #0
	ldr	r1, [sp, #80]
	mov	r2, r8
	bl	sub_080771b8
	mov	r1, fp
	cmp	r1, #0
	bne.n	.L_080ad870
	ldr	r3, [pc, #852]
	add	r3, r9
	ldrb	r3, [r3, #0]
	str	r3, [sp, #76]
	ldr	r3, [pc, #848]
	add	r3, r9
	ldrb	r3, [r3, #0]
	movs	r2, #31
	ands	r2, r3
	movs	r3, #189
	str	r2, [sp, #72]
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	movs	r5, #128
	lsls	r5, r5, #8
	ands	r5, r3
	lsls	r5, r5, #16
	lsrs	r5, r5, #16
	adds	r0, r6, #0
	ldr	r1, [sp, #76]
	bl	sub_080771a8
	cmp	r5, #0
	beq.n	.L_080ad870
	adds	r0, r6, #0
	ldr	r1, [sp, #76]
	ldr	r2, [sp, #72]
	bl	sub_080771b0
.L_080ad870:
	adds	r0, r6, #0
	bl	sub_08077010
	mov	r3, sl
	mov	r4, sp
	mov	r0, sp
	adds	r4, #116
	adds	r3, #88
	adds	r0, #112
	adds	r1, r7, #0
	str	r3, [sp, #64]
	str	r0, [sp, #56]
	str	r0, [sp, #0]
	ldr	r2, [sp, #84]
	adds	r1, #88
	adds	r0, r3, #0
	adds	r3, r4, #0
	str	r4, [sp, #60]
	bl	sub_080aae14
	movs	r2, #166
	str	r0, [sp, #120]
	mov	r1, sl
	lsls	r2, r2, #1
	ldr	r5, [pc, #740]
	adds	r0, r7, #0
	bl	sub_080072f8
	ldr	r3, [pc, #744]
	add	r3, r9
	ldrb	r6, [r3, #0]
	adds	r0, r6, #0
	bl	sub_08077008
	movs	r2, #166
	adds	r7, r0, #0
	adds	r1, r7, #0
	mov	r0, sl
	lsls	r2, r2, #1
	bl	sub_080072f8
	mov	r1, fp
	cmp	r1, #0
	bne.n	.L_080ad8d2
	adds	r0, r6, #0
	ldr	r1, [sp, #76]
	ldr	r2, [sp, #72]
	bl	sub_080771b8
.L_080ad8d2:
	mov	r2, r8
	adds	r0, r6, #0
	ldr	r1, [sp, #80]
	bl	sub_080771a8
	ldr	r2, [sp, #68]
	cmp	r2, #0
	beq.n	.L_080ad8ec
	adds	r0, r6, #0
	ldr	r1, [sp, #80]
	mov	r2, r8
	bl	sub_080771b0
.L_080ad8ec:
	adds	r0, r6, #0
	bl	sub_08077010
	ldr	r3, [sp, #56]
	adds	r1, r7, #0
	str	r3, [sp, #0]
	ldr	r2, [sp, #84]
	ldr	r0, [sp, #64]
	ldr	r3, [sp, #60]
	adds	r1, #88
	bl	sub_080aae14
	ldr	r4, [sp, #28]
	movs	r2, #166
	str	r0, [r4, #4]
	mov	r1, sl
	lsls	r2, r2, #1
	adds	r0, r7, #0
	bl	sub_080072f8
	mov	r0, sl
	bl	sub_08002df0
	ldr	r0, [sp, #84]
	bl	sub_08002df0
	b.n	.L_080ad9d8
.L_080ad922:
	mov	r3, fp
	subs	r3, #2
	cmp	r3, #1
	bhi.n	.L_080ad9d8
	movs	r0, #96
	bl	sub_08004970
	str	r0, [sp, #52]
	movs	r0, #166
	lsls	r0, r0, #1
	bl	sub_08004970
	ldr	r3, [pc, #580]
	add	r3, r9
	ldrb	r6, [r3, #0]
	ldr	r3, [pc, #576]
	add	r3, r9
	ldrb	r3, [r3, #0]
	str	r3, [sp, #48]
	movs	r3, #149
	lsls	r3, r3, #2
	mov	sl, r0
	add	r3, r9
	movs	r0, #31
	ldrb	r3, [r3, #0]
	mov	r8, r0
	mov	r1, r8
	ands	r1, r3
	mov	r2, fp
	movs	r3, #3
	eors	r3, r2
	negs	r5, r3
	adds	r0, r6, #0
	mov	r8, r1
	orrs	r5, r3
	bl	sub_08077008
	lsrs	r5, r5, #31
	subs	r5, r7, r5
	movs	r2, #166
	adds	r7, r0, #0
	adds	r1, r7, #0
	lsls	r2, r2, #1
	mov	r0, sl
	ldr	r3, [pc, #524]
	bl	sub_080072f0
	adds	r0, r6, #0
	ldr	r1, [sp, #48]
	mov	r2, r8
	bl	sub_080771b8
	cmp	r5, #0
	beq.n	.L_080ad998
	adds	r0, r6, #0
	ldr	r1, [sp, #48]
	mov	r2, r8
	bl	sub_080771b0
.L_080ad998:
	adds	r0, r6, #0
	bl	sub_08077010
	mov	r0, sl
	add	r2, sp, #104
	adds	r1, r7, #0
	add	r3, sp, #108
	str	r2, [sp, #0]
	adds	r1, #88
	ldr	r2, [sp, #52]
	adds	r0, #88
	bl	sub_080aae14
	movs	r2, #166
	str	r0, [sp, #120]
	mov	r1, sl
	lsls	r2, r2, #1
	ldr	r4, [pc, #460]
	adds	r0, r7, #0
	bl	sub_080072f4
	ldr	r0, [sp, #28]
	ldr	r3, [sp, #120]
	movs	r1, #0
	str	r3, [r0, #4]
	mov	r0, sl
	str	r1, [sp, #88]
	bl	sub_08002df0
	ldr	r0, [sp, #52]
	bl	sub_08002df0
.L_080ad9d8:
	ldr	r0, [sp, #120]
	movs	r1, #5
	subs	r0, #1
	bl	sub_080022ec
	adds	r0, #1
	str	r0, [sp, #120]
	cmp	r0, #0
	bne.n	.L_080ad9ee
	movs	r3, #1
	str	r3, [sp, #120]
.L_080ad9ee:
	ldr	r2, [sp, #28]
	ldr	r0, [r2, #4]
	movs	r1, #5
	subs	r0, #1
	bl	sub_080022ec
	ldr	r3, [sp, #28]
	adds	r0, #1
	str	r0, [r3, #4]
	cmp	r0, #0
	bne.n	.L_080ada0a
	ldr	r4, [sp, #28]
	movs	r3, #1
	str	r3, [r4, #4]
.L_080ada0a:
	mov	r0, r9
	movs	r5, #2
	adds	r0, #36
	movs	r6, #15
	movs	r1, #0
	movs	r2, #5
	movs	r3, #15
	str	r5, [sp, #4]
	str	r0, [sp, #44]
	str	r6, [sp, #0]
	bl	sub_080a10d0
	mov	r1, r9
	str	r5, [sp, #4]
	movs	r5, #134
	adds	r1, #52
	lsls	r5, r5, #1
	str	r1, [sp, #40]
	movs	r2, #5
	adds	r0, r1, #0
	movs	r3, #15
	movs	r1, #15
	add	r5, r9
	str	r6, [sp, #0]
	bl	sub_080a10d0
	ldr	r0, [r5, #0]
	bl	sub_08015270
	mov	r2, fp
	cmp	r2, #2
	bne.n	.L_080ada4e
	ldr	r0, [pc, #332]
	b.n	.L_080ada56
.L_080ada4e:
	mov	r3, fp
	cmp	r3, #3
	bne.n	.L_080ada6e
	ldr	r0, [pc, #324]
.L_080ada56:
	ldr	r1, [r5, #0]
	movs	r2, #96
	movs	r3, #0
	bl	sub_08015080
	ldr	r0, [pc, #316]
	ldr	r1, [r5, #0]
	movs	r2, #96
	movs	r3, #16
	bl	sub_08015080
	b.n	.L_080adaa6
.L_080ada6e:
	mov	r4, fp
	cmp	r4, #0
	bne.n	.L_080ada8e
	ldr	r0, [pc, #300]
	ldr	r1, [r5, #0]
	movs	r2, #128
	movs	r3, #0
	bl	sub_08015080
	ldr	r0, [pc, #284]
	ldr	r1, [r5, #0]
	movs	r2, #128
	movs	r3, #8
	bl	sub_08015080
	b.n	.L_080adaa6
.L_080ada8e:
	ldr	r0, [pc, #280]
	ldr	r1, [r5, #0]
	movs	r2, #128
	movs	r3, #0
	bl	sub_08015080
	ldr	r0, [pc, #260]
	ldr	r1, [r5, #0]
	movs	r2, #128
	movs	r3, #8
	bl	sub_08015080
.L_080adaa6:
	movs	r1, #1
	mov	r2, fp
	movs	r7, #188
	eors	r2, r1
	lsls	r7, r7, #1
	movs	r0, #128
	negs	r3, r2
	add	r7, r9
	lsls	r0, r0, #8
	orrs	r3, r2
	mov	r8, r0
	ldrh	r2, [r7, #0]
	lsrs	r5, r3, #31
	mov	r3, r8
	ands	r3, r2
	subs	r5, r1, r5
	cmp	r3, #0
	bne.n	.L_080adad2
	movs	r0, #2
	bl	sub_080150b8
	ldrh	r2, [r7, #0]
.L_080adad2:
	movs	r3, #224
	ands	r3, r2
	lsrs	r3, r3, #5
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r6, #134
	movs	r3, #31
	ands	r3, r2
	ldr	r1, [pc, #200]
	lsls	r0, r0, #2
	lsls	r6, r6, #1
	adds	r0, r0, r3
	add	r6, r9
	adds	r0, r0, r1
	lsls	r3, r5, #3
	ldr	r1, [r6, #0]
	movs	r2, #48
	bl	sub_08015080
	ldrh	r3, [r7, #0]
	movs	r1, #224
	ands	r1, r3
	ldr	r2, [pc, #176]
	movs	r3, #0
	lsrs	r1, r1, #5
	ldr	r0, [r6, #0]
	adds	r1, r1, r2
	str	r3, [sp, #0]
	movs	r2, #5
	mov	sl, r3
	adds	r3, r5, #0
	bl	sub_08015280
	movs	r0, #15
	bl	sub_080150b8
	mov	r4, fp
	cmp	r4, #0
	bne.n	.L_080adbba
	movs	r5, #189
	lsls	r5, r5, #1
	add	r5, r9
	ldrh	r2, [r5, #0]
	mov	r3, r8
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080adb38
	movs	r0, #2
	bl	sub_080150b8
	ldrh	r2, [r5, #0]
.L_080adb38:
	movs	r3, #224
	ands	r3, r2
	lsrs	r3, r3, #5
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #31
	ands	r3, r2
	ldr	r1, [pc, #100]
	lsls	r0, r0, #2
	adds	r0, r0, r3
	adds	r0, r0, r1
	movs	r2, #48
	ldr	r1, [r6, #0]
	movs	r3, #16
	bl	sub_08015080
	ldrh	r3, [r5, #0]
	movs	r1, #224
	ands	r1, r3
	ldr	r2, [pc, #80]
	mov	r3, fp
	lsrs	r1, r1, #5
	ldr	r0, [r6, #0]
	adds	r1, r1, r2
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #2
	bl	sub_08015280
	movs	r0, #15
	bl	sub_080150b8
	b.n	.L_080adc24
.L_080adb7a:
	movs	r7, #1
	b.n	.L_080ae216
	movs	r0, r0
	.4byte 0x0000021a
	.4byte 0x00000256
	.4byte 0x03001388
	.4byte 0x00000257
	.4byte 0x00000255
	.4byte 0x0000021b
	.4byte 0x00000ba5
	.4byte 0x00000ba4
	.4byte 0x00000bc1
	.4byte 0x00000ba7
	.4byte 0x00000ba6
	.4byte 0x0000045f
	.2byte 0x5001
	.2byte 0x0000
.L_080adbb4:
	movs	r0, #113
	movs	r7, #2
	b.n	.L_080ae15e
.L_080adbba:
	mov	r3, fp
	subs	r3, #2
	cmp	r3, #1
	bhi.n	.L_080adc24
	ldrh	r2, [r7, #0]
	mov	r3, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080adbd4
	movs	r0, #2
	bl	sub_080150b8
	ldrh	r2, [r7, #0]
.L_080adbd4:
	movs	r3, #224
	ands	r3, r2
	lsrs	r3, r3, #5
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #31
	ldr	r4, [pc, #824]
	ands	r3, r2
	lsls	r0, r0, #2
	adds	r0, r0, r3
	adds	r0, r0, r4
	ldr	r1, [r6, #0]
	movs	r2, #48
	movs	r3, #16
	bl	sub_08015080
	ldrh	r3, [r7, #0]
	movs	r1, #224
	ands	r1, r3
	ldr	r2, [pc, #804]
	mov	r3, sl
	lsrs	r1, r1, #5
	adds	r1, r1, r2
	ldr	r0, [r6, #0]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r3, #2
	bl	sub_08015280
	movs	r0, #15
	bl	sub_080150b8
	ldr	r0, [r6, #0]
	mov	r4, sl
	ldr	r1, [pc, #776]
	movs	r2, #7
	movs	r3, #1
	str	r4, [sp, #0]
	bl	sub_08015280
.L_080adc24:
	ldr	r3, [pc, #768]
	ldr	r3, [r3, #0]
	mov	sl, r3
	movs	r3, #194
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #760]
	ldr	r3, [r3, #0]
	str	r3, [sp, #36]
	ldr	r3, [pc, #756]
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #756]
	str	r3, [sp, #32]
	adds	r5, r6, r0
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_080adc4a
	b.n	.L_080ade0c
.L_080adc4a:
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_08004970
	movs	r1, #0
	ldr	r3, [pc, #736]
	str	r1, [sp, #36]
	str	r1, [sp, #32]
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	ldr	r3, [r5, #0]
	subs	r3, #5
	mov	r8, r0
	cmp	r3, #18
	bls.n	.L_080adc6e
	b.n	.L_080ade06
.L_080adc6e:
	ldr	r2, [pc, #716]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080adcc4
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080adce6
	.4byte 0x080add72
	.4byte 0x080adcc4
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.2byte 0xdcc4
	.2byte 0x080a
	ldr	r4, [pc, #624]
	adds	r2, r6, r4
	ldr	r3, [r2, #0]
	cmp	r3, #100
	beq.n	.L_080adcd0
	b.n	.L_080ade06
.L_080adcd0:
	movs	r0, #1
	ldr	r1, [pc, #608]
	str	r0, [sp, #32]
	str	r0, [sp, #36]
	movs	r3, #0
	str	r3, [r2, #0]
	adds	r2, r6, r1
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	b.n	.L_080ade06
	ldr	r2, [pc, #592]
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	cmp	r3, #60
	beq.n	.L_080adcf2
	b.n	.L_080ade06
.L_080adcf2:
	movs	r2, #128
	mov	r1, sl
	ldr	r3, [pc, #584]
	mov	r0, r8
	lsls	r2, r2, #2
	bl	sub_080072f0
	movs	r1, #8
	movs	r2, #0
	movs	r3, #1
	ldr	r0, [pc, #572]
	bl	sub_08015038
	adds	r7, r0, #0
	mov	r4, r9
	ldr	r3, [r4, #20]
	movs	r5, #1
	strb	r5, [r3, #5]
	movs	r0, #2
	movs	r1, #96
	bl	sub_080a1ac0
	ldr	r3, [pc, #552]
	movs	r0, #131
	lsls	r0, r0, #2
	adds	r3, r3, r0
	strb	r5, [r3, #0]
	b.n	.L_080add30
.L_080add2a:
	movs	r0, #1
	bl	sub_080030f8
.L_080add30:
	bl	sub_08015048
	cmp	r0, #0
	beq.n	.L_080add2a
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08015018
	movs	r2, #128
	mov	r1, r8
	ldr	r3, [pc, #504]
	lsls	r2, r2, #2
	mov	r0, sl
	bl	sub_080072f0
	bl	sub_080152a8
	ldr	r2, [pc, #504]
	movs	r3, #1
	add	r2, sl
	strb	r3, [r2, #0]
	ldr	r2, [pc, #476]
	str	r3, [sp, #96]
	movs	r1, #0
	adds	r3, r6, r2
	str	r1, [r3, #0]
	ldr	r3, [pc, #460]
	adds	r2, r6, r3
	movs	r3, #11
	str	r3, [r2, #0]
	mov	r4, r9
	ldr	r2, [r4, #20]
	b.n	.L_080ade02
	ldr	r0, [pc, #452]
	adds	r3, r6, r0
	ldr	r3, [r3, #0]
	cmp	r3, #60
	bne.n	.L_080ade06
	movs	r2, #128
	mov	r1, sl
	ldr	r3, [pc, #444]
	mov	r0, r8
	lsls	r2, r2, #2
	bl	sub_080072f0
	movs	r1, #8
	movs	r2, #0
	movs	r3, #1
	ldr	r0, [pc, #444]
	bl	sub_08015038
	adds	r7, r0, #0
	mov	r1, r9
	ldr	r3, [r1, #20]
	movs	r5, #1
	strb	r5, [r3, #5]
	movs	r0, #106
	movs	r1, #56
	bl	sub_080a1ac0
	ldr	r3, [pc, #412]
	movs	r2, #131
	lsls	r2, r2, #2
	adds	r3, r3, r2
	strb	r5, [r3, #0]
	b.n	.L_080addba
.L_080addb4:
	movs	r0, #1
	bl	sub_080030f8
.L_080addba:
	bl	sub_08015048
	cmp	r0, #0
	beq.n	.L_080addb4
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08015018
	movs	r2, #128
	mov	r1, r8
	ldr	r3, [pc, #368]
	lsls	r2, r2, #2
	mov	r0, sl
	bl	sub_080072f0
	bl	sub_080152a8
	ldr	r2, [pc, #364]
	movs	r3, #1
	add	r2, sl
	strb	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r4, [pc, #332]
	movs	r3, #1
	ldr	r0, [pc, #324]
	str	r3, [sp, #96]
	movs	r5, #0
	adds	r3, r6, r4
	str	r5, [r3, #0]
	adds	r2, r6, r0
	movs	r3, #12
	str	r3, [r2, #0]
	mov	r1, r9
	ldr	r2, [r1, #20]
.L_080ade02:
	movs	r3, #13
	strb	r3, [r2, #5]
.L_080ade06:
	mov	r0, r8
	bl	sub_08002df0
.L_080ade0c:
	ldr	r2, [sp, #96]
	cmp	r2, #0
	bne.n	.L_080ade14
	b.n	.L_080adfbc
.L_080ade14:
	movs	r3, #1
	mov	r4, fp
	mov	r8, r3
	cmp	r4, #1
	bhi.n	.L_080ade78
	movs	r5, #134
	lsls	r5, r5, #1
	movs	r3, #24
	add	r5, r9
	ldr	r0, [r5, #0]
	movs	r1, #128
	str	r3, [sp, #0]
	movs	r2, #16
	movs	r3, #224
	bl	sub_08015068
	ldr	r0, [sp, #88]
	cmp	r0, #1
	bne.n	.L_080ade48
	ldr	r0, [pc, #280]
	ldr	r1, [r5, #0]
	movs	r2, #128
	movs	r3, #16
	bl	sub_08015080
	b.n	.L_080ade78
.L_080ade48:
	ldr	r1, [sp, #88]
	movs	r2, #2
	eors	r2, r1
	negs	r3, r2
	orrs	r3, r2
	movs	r2, #134
	lsls	r2, r2, #2
	lsrs	r3, r3, #31
	adds	r3, r3, r2
	mov	r2, r9
	adds	r2, #2
	ldrb	r0, [r2, r3]
	movs	r1, #1
	bl	sub_08015120
	movs	r3, #134
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #232]
	movs	r2, #128
	movs	r3, #16
	bl	sub_08015080
.L_080ade78:
	ldr	r2, [pc, #224]
	movs	r3, #1
	add	r2, sl
	strb	r3, [r2, #0]
	mov	r2, r9
	ldr	r0, [r2, #36]
	bl	sub_08015060
	mov	r3, r9
	ldr	r0, [r3, #52]
	bl	sub_08015060
	ldr	r4, [sp, #96]
	lsrs	r3, r4, #1
	cmp	r3, #0
	beq.n	.L_080ade9e
	movs	r0, #1
	bl	sub_080030f8
.L_080ade9e:
	mov	r0, fp
	cmp	r0, #3
	bne.n	.L_080adeac
	movs	r1, #0
	movs	r7, #0
	mov	r8, r1
	b.n	.L_080adebc
.L_080adeac:
	mov	r2, fp
	cmp	r2, #2
	bne.n	.L_080adeba
	movs	r3, #0
	movs	r7, #1
	mov	r8, r3
	b.n	.L_080adebc
.L_080adeba:
	movs	r7, #2
.L_080adebc:
	ldr	r4, [sp, #88]
	cmp	r4, #0
	beq.n	.L_080adf64
	cmp	r4, #1
	beq.n	.L_080aded0
	mov	r0, fp
	movs	r1, #0
	bl	sub_080aca04
	b.n	.L_080adfb4
.L_080aded0:
	mov	r0, fp
	cmp	r0, #1
	bne.n	.L_080aded8
	movs	r7, #4
.L_080aded8:
	ldr	r6, [pc, #132]
	ldr	r2, [sp, #88]
	mov	r1, r9
	add	r6, r9
	ldr	r0, [r1, #36]
	ldrb	r3, [r6, #0]
	movs	r5, #0
	str	r2, [sp, #0]
	movs	r1, #0
	movs	r2, #0
	str	r5, [sp, #4]
	str	r7, [sp, #8]
	str	r5, [sp, #12]
	str	r5, [sp, #16]
	bl	sub_080acab8
	ldr	r4, [sp, #88]
	mov	r3, r9
	ldr	r0, [r3, #52]
	ldr	r1, [sp, #24]
	ldrb	r3, [r6, #0]
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	str	r7, [sp, #8]
	ldr	r2, [r1, #4]
	adds	r2, #1
	str	r2, [sp, #12]
	movs	r1, #0
	movs	r2, #0
	str	r5, [sp, #16]
	bl	sub_080acab8
	b.n	.L_080adfb4
	movs	r0, r0
	.4byte 0x0000045f
	.4byte 0x00005001
	.4byte 0x0000f296
	.4byte 0x03001e8c
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x0000212c
	.4byte 0x00002128
	.4byte 0x080adc78
	.4byte 0x03001388
	.4byte 0x00000c43
	.4byte 0x02000240
	.4byte 0x00000ea3
	.4byte 0x00000c42
	.4byte 0x00000ba1
	.4byte 0x00000ba0
	.4byte 0x00000ea6
	.2byte 0x021b
	.2byte 0x0000
.L_080adf64:
	mov	r2, fp
	cmp	r2, #1
	bne.n	.L_080adf70
	movs	r3, #0
	movs	r7, #1
	mov	r8, r3
.L_080adf70:
	movs	r5, #150
	ldr	r1, [sp, #88]
	lsls	r5, r5, #2
	mov	r4, r9
	add	r5, r9
	mov	r2, r8
	ldr	r0, [r4, #36]
	ldrb	r3, [r5, #0]
	movs	r6, #1
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #12]
	movs	r2, #0
	movs	r1, #0
	str	r7, [sp, #8]
	str	r6, [sp, #16]
	bl	sub_080acab8
	ldr	r2, [sp, #128]
	mov	r3, r9
	mov	r1, r8
	adds	r2, #1
	ldr	r4, [sp, #88]
	ldr	r0, [r3, #52]
	ldrb	r3, [r5, #0]
	str	r1, [sp, #4]
	str	r2, [sp, #12]
	movs	r1, #0
	movs	r2, #0
	str	r4, [sp, #0]
	str	r7, [sp, #8]
	str	r6, [sp, #16]
	bl	sub_080acab8
.L_080adfb4:
	ldr	r2, [pc, #764]
	movs	r3, #0
	add	r2, sl
	strb	r3, [r2, #0]
.L_080adfbc:
	ldr	r2, [sp, #88]
	cmp	r2, #1
	bgt.n	.L_080ae050
	ldr	r0, [sp, #28]
	mov	r3, r9
	lsls	r4, r2, #2
	ldr	r7, [r3, #52]
	adds	r3, r4, r0
	ldr	r2, [r3, #0]
	cmp	r2, #1
	ble.n	.L_080ae050
	movs	r5, #0
	cmp	r5, r2
	bge.n	.L_080ae012
	adds	r6, r3, #0
.L_080adfda:
	ldr	r2, [pc, #732]
	adds	r1, r5, r2
	cmp	r5, #9
	ble.n	.L_080adfe4
	ldr	r1, [pc, #728]
.L_080adfe4:
	ldr	r0, [sp, #24]
	ldr	r3, [r4, r0]
	cmp	r5, r3
	bne.n	.L_080adff0
	ldr	r2, [pc, #720]
	adds	r1, r1, r2
.L_080adff0:
	ldr	r3, [r6, #0]
	ldrh	r2, [r7, #8]
	subs	r2, r2, r3
	adds	r2, r2, r5
	movs	r3, #0
	str	r3, [sp, #0]
	subs	r2, #2
	subs	r3, #1
	adds	r0, r7, #0
	str	r4, [sp, #20]
	bl	sub_08015280
	ldr	r3, [r6, #0]
	adds	r5, #1
	ldr	r4, [sp, #20]
	cmp	r5, r3
	blt.n	.L_080adfda
.L_080ae012:
	ldr	r0, [sp, #28]
	ldrh	r2, [r7, #8]
	ldr	r3, [r4, r0]
	movs	r6, #1
	negs	r6, r6
	subs	r2, r2, r3
	movs	r5, #0
	adds	r0, r7, #0
	adds	r3, r6, #0
	ldr	r1, [pc, #668]
	subs	r2, #3
	str	r5, [sp, #0]
	bl	sub_08015280
	ldrh	r2, [r7, #8]
	ldr	r1, [pc, #660]
	subs	r2, #2
	adds	r0, r7, #0
	adds	r3, r6, #0
	str	r5, [sp, #0]
	bl	sub_08015280
	ldr	r1, [pc, #652]
	ldrh	r2, [r7, #14]
	add	r1, sl
	lsrs	r2, r2, #2
	movs	r3, #2
	lsls	r3, r2
	ldrb	r2, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
.L_080ae050:
	ldr	r1, [sp, #92]
	adds	r1, #1
	str	r1, [sp, #92]
	adds	r0, r1, #0
	movs	r1, #60
	bl	sub_080022fc
	subs	r6, r0, #5
	cmp	r6, #0
	bge.n	.L_080ae066
	movs	r6, #0
.L_080ae066:
	cmp	r6, #29
	ble.n	.L_080ae06c
	movs	r6, #29
.L_080ae06c:
	ldr	r5, [pc, #608]
	movs	r0, #0
	adds	r1, r5, #0
	bl	sub_080ad5f4
	movs	r0, #1
	adds	r1, r5, #0
	bl	sub_080ad5f4
	mov	r2, fp
	cmp	r2, #1
	bhi.n	.L_080ae0ec
	movs	r1, #30
	adds	r0, r6, #0
	bl	sub_080022fc
	adds	r3, r0, #0
	lsls	r0, r3, #4
	adds	r0, r0, r3
	lsls	r0, r0, #4
	adds	r0, r0, r3
	lsls	r0, r0, #2
	bl	sub_08002322
	ldr	r3, [pc, #564]
	adds	r1, r0, #0
	movs	r0, #16
	movs	r0, r0
	mov	ip, pc
	bx	r3
	movs	r3, #6
	negs	r5, r0
	negs	r3, r3
	cmp	r5, r3
	bge.n	.L_080ae0b4
	adds	r5, r3, #0
.L_080ae0b4:
	cmp	r5, #12
	ble.n	.L_080ae0ba
	movs	r5, #12
.L_080ae0ba:
	adds	r0, r6, #0
	movs	r1, #35
	bl	sub_080022fc
	lsls	r6, r0, #1
	adds	r1, r6, #0
	adds	r2, r5, #0
	movs	r3, #0
	adds	r1, #34
	adds	r2, #20
	movs	r0, #0
	bl	sub_080ad5b4
	mov	r3, fp
	cmp	r3, #0
	bne.n	.L_080ae0f8
	movs	r1, #99
	movs	r2, #36
	subs	r1, r1, r6
	subs	r2, r2, r5
	movs	r0, #1
	movs	r3, #0
	bl	sub_080ad5b4
	b.n	.L_080ae0f8
.L_080ae0ec:
	movs	r0, #0
	movs	r1, #32
	movs	r2, #30
	movs	r3, #0
	bl	sub_080ad5b4
.L_080ae0f8:
	ldr	r4, [sp, #96]
	cmp	r4, #0
	beq.n	.L_080ae10c
	movs	r0, #0
	str	r0, [sp, #96]
	movs	r1, #2
	ldr	r0, [sp, #100]
	bl	sub_080aa538
	str	r0, [sp, #100]
.L_080ae10c:
	ldr	r1, [sp, #100]
	lsls	r0, r1, #3
	subs	r0, r0, r1
	lsls	r0, r0, #3
	adds	r0, #80
	movs	r1, #16
	bl	sub_080a1a40
	ldr	r2, [sp, #92]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080ae148
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ae13c
	ldr	r1, [pc, #424]
	ldr	r3, [pc, #424]
	ldr	r0, [pc, #428]
	movs	r2, #32
	bl	sub_080072f0
	b.n	.L_080ae148
.L_080ae13c:
	ldr	r3, [pc, #420]
	ldr	r0, [pc, #416]
	movs	r1, #32
	ldr	r2, [pc, #420]
	bl	sub_080072f0
.L_080ae148:
	ldr	r4, [sp, #36]
	movs	r3, #1
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_080ae166
	ldr	r0, [sp, #100]
	cmp	r0, #0
	bne.n	.L_080ae15a
	b.n	.L_080adb7a
.L_080ae15a:
	movs	r0, #113
	movs	r7, #1
.L_080ae15e:
	bl	sub_080f9010
	negs	r7, r7
	b.n	.L_080ae216
.L_080ae166:
	ldr	r1, [sp, #36]
	movs	r3, #8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ae172
	b.n	.L_080adbb4
.L_080ae172:
	movs	r3, #2
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_080ae15a
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ae1a2
	mov	r2, fp
	cmp	r2, #1
	bhi.n	.L_080ae20e
	ldr	r0, [sp, #88]
	movs	r1, #3
	adds	r0, #1
	bl	sub_080022fc
	movs	r3, #2
	str	r0, [sp, #88]
	movs	r0, #111
	str	r3, [sp, #96]
	bl	sub_080f9010
	b.n	.L_080ae20e
.L_080ae1a2:
	ldr	r4, [sp, #32]
	movs	r3, #32
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_080ae1d8
	ldr	r0, [sp, #88]
	cmp	r0, #1
	bgt.n	.L_080ae20e
	ldr	r1, [sp, #24]
	lsls	r3, r0, #2
	adds	r5, r3, r1
	ldr	r0, [r5, #0]
	subs	r0, #1
	str	r0, [r5, #0]
	ldr	r2, [sp, #28]
	ldr	r1, [r3, r2]
	bl	sub_080aa538
	str	r0, [r5, #0]
	movs	r0, #111
	bl	sub_080f9010
	bl	sub_0800352c
	movs	r3, #1
	str	r3, [sp, #96]
	b.n	.L_080ae20e
.L_080ae1d8:
	ldr	r4, [sp, #32]
	movs	r3, #16
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_080ae20e
	ldr	r0, [sp, #88]
	cmp	r0, #1
	bgt.n	.L_080ae20e
	movs	r0, #111
	bl	sub_080f9010
	bl	sub_0800352c
	ldr	r1, [sp, #88]
	ldr	r2, [sp, #24]
	lsls	r3, r1, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	adds	r0, #1
	str	r0, [r5, #0]
	movs	r4, #1
	str	r4, [sp, #96]
	ldr	r2, [sp, #28]
	ldr	r1, [r3, r2]
	bl	sub_080aa538
	str	r0, [r5, #0]
.L_080ae20e:
	movs	r0, #1
	bl	sub_080030f8
	b.n	.L_080adc24
.L_080ae216:
	movs	r1, #0
	movs	r0, #0
	bl	sub_080ad5f4
	movs	r1, #0
	movs	r0, #1
	bl	sub_080ad5f4
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #192]
	bl	sub_080041d8
	ldr	r3, [pc, #188]
	movs	r5, #134
	ldr	r2, [r3, #0]
	ldr	r6, [pc, #124]
	lsls	r5, r5, #1
	mov	r8, r3
	add	r5, r9
	movs	r3, #1
	movs	r4, #0
	strb	r3, [r2, r6]
	adds	r0, r5, #0
	movs	r1, #1
	mov	sl, r4
	bl	sub_080a1114
	movs	r0, #1
	bl	sub_080030f8
	movs	r3, #5
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r2, #0
	movs	r3, #17
	adds	r0, r5, #0
	movs	r1, #13
	bl	sub_080a10d0
	movs	r1, #1
	ldr	r0, [sp, #44]
	bl	sub_080a1114
	movs	r1, #1
	ldr	r0, [sp, #40]
	bl	sub_080a1114
	mov	r1, r9
	ldr	r0, [r1, #48]
	bl	sub_08015270
	mov	r2, r9
	ldr	r0, [r2, #40]
	bl	sub_08015270
	mov	r3, r9
	ldr	r0, [r3, #16]
	bl	sub_08015270
	mov	r4, r8
	ldr	r3, [r4, #0]
	mov	r0, sl
	adds	r3, r3, r6
	strb	r0, [r3, #0]
	movs	r0, #1
	bl	sub_080030f8
	adds	r0, r7, #0
	add	sp, #136
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000ea6
	.4byte 0x0000f031
	.4byte 0x0000f030
	.4byte 0xfffff000
	.4byte 0x0000f128
	.4byte 0x0000f129
	.4byte 0x00000ea3
	.4byte 0xffff4000
	.4byte 0x03000118
	.4byte 0x080af26c
	.4byte 0x03001388
	.4byte 0x060052c0
	.4byte 0x03000168
	.4byte 0x44444444
	.4byte 0x080a19a1
	.4byte 0x03001e8c
