.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08015018, 0x08015018
	.set sub_08015038, 0x08015038
	.set sub_08015048, 0x08015048
	.set sub_08015068, 0x08015068
	.set sub_08015078, 0x08015078
	.set sub_08015080, 0x08015080
	.set sub_08015090, 0x08015090
	.set sub_080150b0, 0x080150b0
	.set sub_08015120, 0x08015120
	.set sub_08015270, 0x08015270
	.set sub_08015278, 0x08015278
	.set sub_08015280, 0x08015280
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
	.set sub_080ad5b4, 0x080ad5b4
	.set sub_080ad5f4, 0x080ad5f4
	.set sub_080ad608, 0x080ad608
	.set sub_080b50f8, 0x080b50f8
	.set sub_080f9010, 0x080f9010
	.global Unnamed_080ab834
	.global Func_080ab834
	.thumb_func
Unnamed_080ab834:
Func_080ab834:
.L_080ab834:
	ldr	r7, [sp, #72]
	cmp	r7, #0
	bne.n	.L_080ab83c
	b.n	.L_080abc9a
.L_080ab83c:
	movs	r1, #1
	movs	r0, #0
	negs	r1, r1
	ldr	r3, [sp, #28]
	str	r0, [sp, #72]
	str	r1, [sp, #60]
	ldr	r5, [sp, #56]
	ldrb	r2, [r3, r5]
	movs	r3, #1
	ands	r3, r2
	mov	sl, r7
	cmp	r3, #0
	bne.n	.L_080ab85a
	ldr	r7, [sp, #48]
	str	r7, [sp, #60]
.L_080ab85a:
	ldr	r1, [sp, #56]
	movs	r2, #130
	ldr	r0, [sp, #76]
	lsls	r3, r1, #1
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r6, [r0, #16]
	ldrh	r0, [r0, r3]
	bl	sub_08077008
	ldr	r7, [pc, #736]
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_08015270
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08015090
	adds	r3, r5, r7
	ldrb	r0, [r3, #0]
	ldr	r3, [pc, #712]
	adds	r1, r6, #0
	adds	r0, r0, r3
	movs	r2, #0
	movs	r3, #8
	bl	sub_08015080
	ldr	r0, [pc, #704]
	adds	r1, r6, #0
	movs	r2, #48
	movs	r3, #0
	bl	sub_08015090
	ldr	r1, [sp, #72]
	ldrb	r0, [r5, #15]
	adds	r2, r6, #0
	str	r1, [sp, #0]
	movs	r3, #72
	movs	r1, #2
	bl	sub_080150b0
	ldr	r2, [sp, #80]
	cmp	r2, #0
	bne.n	.L_080ab8c4
	ldr	r0, [pc, #672]
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #16
	bl	sub_08015080
.L_080ab8c4:
	movs	r7, #1
	ldr	r3, [sp, #60]
	negs	r7, r7
	cmp	r3, r7
	beq.n	.L_080ab8e2
	ldr	r5, [sp, #56]
	lsls	r3, r5, #2
	adds	r3, r3, r5
	ldr	r0, [sp, #60]
	lsls	r3, r3, #1
	adds	r3, r3, r0
	lsls	r3, r3, #1
	mov	r1, r9
	ldrh	r3, [r1, r3]
	str	r3, [sp, #40]
.L_080ab8e2:
	ldr	r2, [sp, #76]
	movs	r3, #134
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldr	r0, [r2, #0]
	mov	r8, r2
	bl	sub_08015270
	ldr	r5, [sp, #80]
	cmp	r5, #1
	bne.n	.L_080ab980
	ldr	r0, [sp, #76]
	ldr	r1, [pc, #612]
	adds	r3, r0, r1
	ldrb	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08015120
	ldr	r4, [pc, #604]
	mov	r2, r8
	adds	r0, r4, #0
	ldr	r1, [r2, #0]
	movs	r3, #0
	movs	r2, #0
	str	r4, [sp, #8]
	bl	sub_08015080
	movs	r5, #188
	ldr	r3, [sp, #76]
	lsls	r5, r5, #1
	adds	r6, r3, r5
	ldrh	r2, [r6, #0]
	movs	r5, #224
	adds	r3, r5, #0
	ands	r3, r2
	lsrs	r3, r3, #5
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #31
	ands	r3, r2
	lsls	r0, r0, #2
	movs	r1, #150
	adds	r0, r0, r3
	lsls	r1, r1, #1
	adds	r0, r0, r1
	movs	r1, #4
	bl	sub_08015120
	ldrh	r3, [r6, #0]
	ands	r5, r3
	ldr	r3, [pc, #544]
	ldr	r1, [sp, #72]
	lsrs	r5, r5, #5
	mov	r2, r8
	adds	r5, r5, r3
	ldr	r0, [r2, #0]
	movs	r3, #0
	str	r1, [sp, #0]
	movs	r2, #6
	adds	r1, r5, #0
	bl	sub_08015280
	ldr	r4, [sp, #8]
	mov	r2, r8
	adds	r0, r4, #1
	ldr	r1, [r2, #0]
	movs	r3, #0
	movs	r2, #56
	bl	sub_08015080
	ldr	r4, [sp, #8]
	mov	r3, r8
	adds	r4, #2
	ldr	r1, [r3, #0]
	adds	r0, r4, #0
	movs	r2, #0
	movs	r3, #8
	bl	sub_08015080
.L_080ab980:
	ldr	r5, [sp, #60]
	cmp	r5, r7
	bne.n	.L_080ab994
	ldr	r0, [sp, #80]
	movs	r1, #0
	movs	r2, #200
	movs	r3, #0
	bl	sub_080ad5b4
	b.n	.L_080abc14
.L_080ab994:
	ldr	r7, [sp, #80]
	cmp	r7, #0
	beq.n	.L_080ab99c
	b.n	.L_080abb7c
.L_080ab99c:
	ldr	r0, [sp, #36]
	cmp	r0, #0
	beq.n	.L_080aba6a
	ldr	r1, [sp, #32]
	cmp	r1, #0
	bne.n	.L_080ab9b8
	mov	r2, r8
	ldr	r1, [r2, #0]
	ldr	r0, [pc, #444]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08015080
	b.n	.L_080ab9c6
.L_080ab9b8:
	mov	r3, r8
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #432]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08015080
.L_080ab9c6:
	ldr	r5, [sp, #40]
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r5
	lsrs	r6, r3, #8
	ldr	r0, [sp, #40]
	movs	r3, #224
	ands	r3, r5
	movs	r7, #31
	ands	r7, r0
	lsrs	r5, r3, #5
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r7, #0
	bl	sub_08077210
	cmp	r0, #0
	bne.n	.L_080ab9f8
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r7, #0
	bl	sub_08077208
	cmp	r0, #0
	beq.n	.L_080aba32
.L_080ab9f8:
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r7, #0
	bl	sub_08077210
	cmp	r0, #0
	beq.n	.L_080aba12
	ldr	r0, [sp, #80]
	adds	r1, r5, #0
	movs	r2, #1
	bl	sub_080ad608
	b.n	.L_080aba1c
.L_080aba12:
	ldr	r0, [sp, #80]
	adds	r1, r5, #0
	movs	r2, #2
	bl	sub_080ad608
.L_080aba1c:
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #56]
	subs	r1, r2, r3
	lsls	r1, r1, #3
	adds	r1, #48
	ldr	r0, [sp, #80]
	movs	r2, #62
	movs	r3, #0
	bl	sub_080ad5b4
	b.n	.L_080aba64
.L_080aba32:
	ldr	r7, [sp, #76]
	movs	r1, #134
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #308]
	movs	r3, #16
	movs	r2, #0
	bl	sub_08015080
	adds	r1, r5, #0
	movs	r2, #1
	ldr	r0, [sp, #80]
	bl	sub_080ad608
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #56]
	subs	r1, r2, r3
	lsls	r1, r1, #3
	adds	r1, #48
	ldr	r0, [sp, #80]
	movs	r2, #62
	movs	r3, #1
	bl	sub_080ad5b4
.L_080aba64:
	mov	r5, sl
	lsrs	r3, r5, #1
	b.n	.L_080abb3a
.L_080aba6a:
	ldr	r4, [pc, #268]
	mov	r7, r8
	adds	r0, r4, #0
	ldr	r1, [r7, #0]
	movs	r2, #0
	movs	r3, #0
	str	r4, [sp, #8]
	bl	sub_08015080
	movs	r3, #240
	ldr	r0, [sp, #40]
	lsls	r3, r3, #4
	ands	r3, r0
	lsrs	r5, r3, #8
	movs	r3, #224
	ands	r3, r0
	movs	r6, #31
	lsrs	r7, r3, #5
	ands	r6, r0
	adds	r1, r7, #0
	adds	r0, r5, #0
	adds	r2, r6, #0
	bl	sub_08077210
	ldr	r4, [sp, #8]
	cmp	r0, #0
	bne.n	.L_080abab0
	adds	r0, r5, #0
	adds	r1, r7, #0
	adds	r2, r6, #0
	bl	sub_08077208
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_080abb0a
.L_080abab0:
	adds	r0, r5, #0
	adds	r1, r7, #0
	adds	r2, r6, #0
	str	r4, [sp, #8]
	bl	sub_08077210
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_080abadc
	mov	r2, r8
	ldr	r1, [r2, #0]
	adds	r0, r4, #3
	movs	r2, #0
	movs	r3, #16
	bl	sub_08015080
	movs	r0, #0
	adds	r1, r7, #0
	movs	r2, #1
	bl	sub_080ad608
	b.n	.L_080abaf4
.L_080abadc:
	mov	r3, r8
	ldr	r1, [r3, #0]
	adds	r0, r4, #2
	movs	r2, #0
	movs	r3, #16
	bl	sub_08015080
	movs	r0, #0
	adds	r1, r7, #0
	movs	r2, #2
	bl	sub_080ad608
.L_080abaf4:
	ldr	r5, [sp, #24]
	ldr	r7, [sp, #56]
	subs	r1, r5, r7
	lsls	r1, r1, #3
	adds	r1, #48
	ldr	r0, [sp, #80]
	movs	r2, #62
	movs	r3, #0
	bl	sub_080ad5b4
	b.n	.L_080abb36
.L_080abb0a:
	mov	r2, r8
	adds	r0, r4, #4
	ldr	r1, [r2, #0]
	movs	r3, #16
	movs	r2, #0
	bl	sub_08015080
	adds	r1, r7, #0
	movs	r2, #1
	movs	r0, #0
	bl	sub_080ad608
	ldr	r3, [sp, #24]
	ldr	r5, [sp, #56]
	subs	r1, r3, r5
	lsls	r1, r1, #3
	adds	r1, #48
	movs	r0, #0
	movs	r2, #62
	movs	r3, #1
	bl	sub_080ad5b4
.L_080abb36:
	mov	r7, sl
	lsrs	r3, r7, #1
.L_080abb3a:
	cmp	r3, #0
	beq.n	.L_080abc14
	ldr	r0, [sp, #80]
	movs	r1, #0
	bl	sub_080ad5f4
	b.n	.L_080abc14
	lsrs	r1, r6, #14
	movs	r0, r0
	lsls	r1, r3, #8
	movs	r0, r0
	.4byte 0x00000129
	.4byte 0x00000741
	.4byte 0x080af28c
	.4byte 0x00000ba9
	.4byte 0x0000021a
	.4byte 0x00000bb2
	.4byte 0x00005001
	.4byte 0x00000b98
	.4byte 0x00000b99
	.4byte 0x00000b9e
	.2byte 0x0b9a
	.2byte 0x0000
.L_080abb7c:
	ldr	r0, [sp, #40]
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r0
	lsrs	r6, r3, #8
	movs	r3, #224
	ands	r3, r0
	movs	r7, #31
	ands	r7, r0
	lsrs	r5, r3, #5
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r7, #0
	bl	sub_08077210
	cmp	r0, #0
	bne.n	.L_080abbac
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r7, #0
	bl	sub_08077208
	cmp	r0, #0
	beq.n	.L_080abbe6
.L_080abbac:
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r7, #0
	bl	sub_08077210
	cmp	r0, #0
	beq.n	.L_080abbc6
	ldr	r0, [sp, #80]
	adds	r1, r5, #0
	movs	r2, #1
	bl	sub_080ad608
	b.n	.L_080abbd0
.L_080abbc6:
	ldr	r0, [sp, #80]
	adds	r1, r5, #0
	movs	r2, #2
	bl	sub_080ad608
.L_080abbd0:
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #56]
	subs	r1, r2, r3
	lsls	r1, r1, #3
	adds	r1, #48
	ldr	r0, [sp, #80]
	movs	r2, #54
	movs	r3, #0
	bl	sub_080ad5b4
	b.n	.L_080abc04
.L_080abbe6:
	adds	r1, r5, #0
	movs	r2, #1
	ldr	r0, [sp, #80]
	bl	sub_080ad608
	ldr	r5, [sp, #24]
	ldr	r7, [sp, #56]
	subs	r1, r5, r7
	lsls	r1, r1, #3
	adds	r1, #48
	ldr	r0, [sp, #80]
	movs	r2, #54
	movs	r3, #1
	bl	sub_080ad5b4
.L_080abc04:
	mov	r0, sl
	lsrs	r3, r0, #1
	cmp	r3, #0
	beq.n	.L_080abc14
	ldr	r0, [sp, #80]
	movs	r1, #0
	bl	sub_080ad5f4
.L_080abc14:
	ldr	r1, [sp, #76]
	ldr	r0, [r1, #48]
	bl	sub_08015278
	movs	r3, #1
	ldr	r2, [sp, #60]
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L_080abc66
	ldr	r5, [sp, #76]
	ldr	r0, [pc, #908]
	ldr	r1, [r5, #48]
	movs	r2, #0
	movs	r3, #80
	bl	sub_08015080
	movs	r3, #104
	ldr	r0, [r5, #48]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #96
	movs	r3, #224
	bl	sub_08015068
	ldr	r7, [sp, #40]
	movs	r3, #224
	ands	r3, r7
	lsrs	r3, r3, #5
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #31
	ands	r3, r7
	lsls	r0, r0, #2
	adds	r0, r0, r3
	ldr	r3, [pc, #864]
	ldr	r1, [r5, #48]
	adds	r0, r0, r3
	movs	r2, #0
	movs	r3, #96
	bl	sub_08015080
.L_080abc66:
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #56]
	movs	r5, #1
	ldrb	r2, [r0, r1]
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080abc90
	ldr	r2, [sp, #76]
	ldr	r3, [sp, #24]
	ldr	r0, [r2, #48]
	ldr	r2, [sp, #48]
	subs	r1, r3, r1
	movs	r3, #14
	str	r3, [sp, #4]
	adds	r1, #1
	adds	r2, #2
	movs	r3, #6
	str	r5, [sp, #0]
	bl	sub_080ab1f4
.L_080abc90:
	ldr	r3, [pc, #812]
	ldr	r7, [pc, #816]
	ldr	r3, [r3, #0]
	adds	r3, r3, r7
	strb	r5, [r3, #0]
.L_080abc9a:
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #56]
	ldrb	r2, [r0, r1]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080abcb8
	ldr	r2, [sp, #24]
	subs	r0, r2, r1
	lsls	r0, r0, #3
	subs	r0, #8
	movs	r1, #52
	bl	sub_080a1a40
	b.n	.L_080abccc
.L_080abcb8:
	ldr	r3, [sp, #24]
	ldr	r5, [sp, #56]
	ldr	r7, [sp, #48]
	subs	r0, r3, r5
	lsls	r0, r0, #3
	lsls	r1, r7, #3
	subs	r0, #8
	adds	r1, #60
	bl	sub_080a1a40
.L_080abccc:
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [pc, #756]
	movs	r2, #128
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080abcea
	ldr	r3, [pc, #744]
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080abcfa
.L_080abcea:
	ldr	r0, [sp, #36]
	cmp	r0, #0
	beq.n	.L_080abcf4
	movs	r1, #1
	str	r1, [sp, #72]
.L_080abcf4:
	movs	r2, #0
	str	r2, [sp, #36]
	str	r2, [sp, #32]
.L_080abcfa:
	ldr	r3, [pc, #724]
	ldr	r3, [r3, #0]
	mov	fp, r3
	ldr	r3, [pc, #720]
	ldr	r4, [r3, #0]
	ldr	r3, [pc, #720]
	add	r3, r9
	ldr	r1, [r3, #0]
	cmp	r1, #0
	bne.n	.L_080abd10
	b.n	.L_080ac1b8
.L_080abd10:
	ldr	r2, [pc, #712]
	add	r2, r9
	ldr	r3, [r2, #0]
	adds	r3, #1
	movs	r4, #0
	str	r3, [r2, #0]
	subs	r3, r1, #1
	mov	fp, r4
	cmp	r3, #27
	bls.n	.L_080abd26
	b.n	.L_080ac1b8
.L_080abd26:
	ldr	r2, [pc, #696]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080abe82
	.4byte 0x080abede
	.4byte 0x080abefc
	.4byte 0x080abede
	.4byte 0x080ac1b8
	.4byte 0x080abf1c
	.4byte 0x080abf1c
	.4byte 0x080abede
	.4byte 0x080abede
	.4byte 0x080ac1b8
	.4byte 0x080ac1b8
	.4byte 0x080ac1b8
	.4byte 0x080abf78
	.4byte 0x080abf9a
	.4byte 0x080abff8
	.4byte 0x080abf9a
	.4byte 0x080abf9a
	.4byte 0x080ac160
	.4byte 0x080ac1b8
	.4byte 0x080ac160
	.4byte 0x080ac17e
	.4byte 0x080abede
	.4byte 0x080ac1b8
	.4byte 0x080ac19c
	.4byte 0x080ac1b8
	.4byte 0x080ac1b8
	.4byte 0x080abdcc
	.2byte 0xbda0
	.2byte 0x080a
	ldr	r1, [pc, #560]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080abdc6
	adds	r6, r1, #0
	movs	r5, #1
.L_080abdb0:
	movs	r0, #150
	movs	r1, #26
	bl	sub_080a1a40
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [r6, #0]
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080abdb0
.L_080abdc6:
	movs	r4, #2
	mov	fp, r4
	b.n	.L_080ac1b8
	ldr	r3, [pc, #524]
	add	r3, r9
	ldr	r3, [r3, #0]
	cmp	r3, #60
	beq.n	.L_080abdd8
	b.n	.L_080ac1b8
.L_080abdd8:
	movs	r2, #9
	movs	r3, #1
	ldr	r0, [pc, #516]
	movs	r1, #9
	bl	sub_08015038
	ldr	r2, [pc, #512]
	movs	r3, #131
	lsls	r3, r3, #2
	adds	r2, r2, r3
	adds	r5, r0, #0
	movs	r3, #1
	strb	r3, [r2, #0]
	b.n	.L_080abdfa
.L_080abdf4:
	movs	r0, #1
	bl	sub_080030f8
.L_080abdfa:
	bl	sub_08015048
	cmp	r0, #0
	beq.n	.L_080abdf4
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_08015018
	mov	r0, r9
	bl	sub_080aafb8
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #9
	movs	r3, #1
	ldr	r0, [pc, #464]
	movs	r1, #9
	bl	sub_08015038
	movs	r7, #131
	ldr	r2, [pc, #448]
	lsls	r7, r7, #2
	adds	r2, r2, r7
	movs	r3, #1
	adds	r5, r0, #0
	strb	r3, [r2, #0]
	b.n	.L_080abe38
.L_080abe32:
	movs	r0, #1
	bl	sub_080030f8
.L_080abe38:
	bl	sub_08015048
	cmp	r0, #0
	beq.n	.L_080abe32
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_08015018
	mov	r0, r9
	bl	sub_080aafb8
	ldr	r2, [pc, #396]
	movs	r3, #0
	add	r2, r9
	str	r3, [r2, #0]
	bl	sub_080b50f8
	bl	sub_080b50f8
	bl	sub_080b50f8
	movs	r1, #0
	movs	r2, #0
	movs	r0, #0
	bl	sub_080771b8
	movs	r1, #0
	movs	r2, #0
	movs	r0, #0
	bl	sub_080771c8
	movs	r0, #0
	bl	sub_08077010
	movs	r0, #2
	mov	fp, r0
	b.n	.L_080ac1b6
	ldr	r3, [pc, #344]
	add	r3, r9
	ldr	r3, [r3, #0]
	cmp	r3, #60
	beq.n	.L_080abe8e
	b.n	.L_080ac1b8
.L_080abe8e:
	movs	r1, #9
	movs	r2, #9
	movs	r3, #1
	ldr	r0, [pc, #344]
	str	r4, [sp, #8]
	bl	sub_08015038
	ldr	r2, [pc, #328]
	movs	r1, #131
	lsls	r1, r1, #2
	adds	r2, r2, r1
	movs	r3, #1
	adds	r5, r0, #0
	strb	r3, [r2, #0]
	b.n	.L_080abeb4
.L_080abeac:
	movs	r0, #1
	str	r4, [sp, #8]
	bl	sub_080030f8
.L_080abeb4:
	ldr	r4, [sp, #8]
	str	r4, [sp, #8]
	bl	sub_08015048
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_080abeac
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08015018
	mov	r0, r9
	bl	sub_080aafb8
	ldr	r2, [pc, #264]
	movs	r3, #0
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r2, [pc, #252]
	movs	r3, #2
	b.n	.L_080ac158
	ldr	r2, [pc, #252]
	add	r2, r9
	ldr	r3, [r2, #0]
	cmp	r3, #90
	beq.n	.L_080abeea
	b.n	.L_080ac1b8
.L_080abeea:
	movs	r3, #1
	mov	fp, r3
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [pc, #228]
	add	r2, r9
	ldr	r3, [r2, #0]
	adds	r3, #1
	b.n	.L_080ac178
	ldr	r2, [pc, #220]
	add	r2, r9
	ldr	r3, [r2, #0]
	cmp	r3, #90
	beq.n	.L_080abf08
	b.n	.L_080ac1b8
.L_080abf08:
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [pc, #200]
	movs	r5, #16
	add	r2, r9
	movs	r3, #4
	mov	fp, r5
	movs	r4, #16
	str	r3, [r2, #0]
	b.n	.L_080ac1b8
	ldr	r3, [pc, #188]
	add	r3, r9
	ldr	r3, [r3, #0]
	cmp	r3, #60
	beq.n	.L_080abf28
	b.n	.L_080ac1b8
.L_080abf28:
	movs	r1, #9
	movs	r2, #9
	movs	r3, #1
	ldr	r0, [pc, #196]
	str	r4, [sp, #8]
	bl	sub_08015038
	movs	r7, #131
	ldr	r2, [pc, #172]
	lsls	r7, r7, #2
	adds	r2, r2, r7
	movs	r3, #1
	adds	r5, r0, #0
	strb	r3, [r2, #0]
	b.n	.L_080abf4e
.L_080abf46:
	movs	r0, #1
	str	r4, [sp, #8]
	bl	sub_080030f8
.L_080abf4e:
	ldr	r4, [sp, #8]
	str	r4, [sp, #8]
	bl	sub_08015048
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_080abf46
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08015018
	mov	r0, r9
	bl	sub_080aafb8
	ldr	r2, [pc, #112]
	movs	r3, #0
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r2, [pc, #100]
	movs	r3, #8
	b.n	.L_080ac158
	ldr	r2, [pc, #96]
	add	r2, r9
	ldr	r3, [r2, #0]
	cmp	r3, #40
	beq.n	.L_080abf84
	b.n	.L_080ac1b8
.L_080abf84:
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [pc, #76]
	add	r2, r9
	ldr	r3, [r2, #0]
	movs	r0, #2
	adds	r3, #1
	mov	fp, r0
	movs	r4, #2
	str	r3, [r2, #0]
	b.n	.L_080ac1b8
	ldr	r2, [pc, #64]
	add	r2, r9
	ldr	r3, [r2, #0]
	cmp	r3, #40
	beq.n	.L_080abfa6
	b.n	.L_080ac1b8
.L_080abfa6:
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [pc, #44]
	add	r2, r9
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	b.n	.L_080ac1b8
	movs	r0, r0
	.4byte 0x00000bad
	.4byte 0x00000666
	.4byte 0x03001e8c
	.4byte 0x00000ea3
	.4byte 0x03001ae8
	.4byte 0x03001af8
	.4byte 0x03001b04
	.4byte 0x03001c94
	.4byte 0x0000212c
	.4byte 0x00002128
	.4byte 0x080abd30
	.4byte 0x00000c4c
	.4byte 0x02000240
	.4byte 0x00000c4d
	.4byte 0x00000c40
	.2byte 0x0c41
	.2byte 0x0000
	ldr	r3, [pc, #844]
	add	r3, r9
	ldr	r3, [r3, #0]
	cmp	r3, #60
	beq.n	.L_080ac004
	b.n	.L_080ac1b8
.L_080ac004:
	ldr	r3, [pc, #836]
	movs	r1, #131
	lsls	r1, r1, #2
	adds	r3, r3, r1
	movs	r2, #1
	strb	r2, [r3, #0]
	movs	r1, #9
	movs	r2, #9
	movs	r3, #1
	ldr	r0, [pc, #824]
	str	r4, [sp, #8]
	bl	sub_08015038
	movs	r1, #146
	adds	r5, r0, #0
	movs	r0, #2
	bl	sub_080a1ac0
	b.n	.L_080ac032
.L_080ac02a:
	movs	r0, #1
	str	r4, [sp, #8]
	bl	sub_080030f8
.L_080ac032:
	ldr	r4, [sp, #8]
	str	r4, [sp, #8]
	bl	sub_08015048
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_080ac02a
	ldr	r1, [pc, #784]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080ac06a
	adds	r7, r1, #0
	movs	r6, #1
.L_080ac050:
	movs	r0, #2
	movs	r1, #146
	str	r4, [sp, #8]
	bl	sub_080a1a40
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [r7, #0]
	ands	r3, r6
	ldr	r4, [sp, #8]
	cmp	r3, #0
	beq.n	.L_080ac050
.L_080ac06a:
	movs	r1, #1
	adds	r0, r5, #0
	str	r4, [sp, #8]
	bl	sub_08015018
	mov	r0, r9
	bl	sub_080aafb8
	movs	r0, #1
	bl	sub_080030f8
	movs	r1, #9
	movs	r2, #9
	movs	r3, #1
	ldr	r0, [pc, #720]
	bl	sub_08015038
	adds	r5, r0, #0
	b.n	.L_080ac098
.L_080ac090:
	movs	r0, #1
	str	r4, [sp, #8]
	bl	sub_080030f8
.L_080ac098:
	ldr	r4, [sp, #8]
	str	r4, [sp, #8]
	bl	sub_08015048
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_080ac090
	ldr	r1, [pc, #684]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080ac0d0
	adds	r7, r1, #0
	movs	r6, #1
.L_080ac0b6:
	movs	r0, #2
	movs	r1, #146
	str	r4, [sp, #8]
	bl	sub_080a1a40
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [r7, #0]
	ands	r3, r6
	ldr	r4, [sp, #8]
	cmp	r3, #0
	beq.n	.L_080ac0b6
.L_080ac0d0:
	movs	r1, #1
	adds	r0, r5, #0
	str	r4, [sp, #8]
	bl	sub_08015018
	mov	r0, r9
	bl	sub_080aafb8
	movs	r0, #1
	bl	sub_080030f8
	movs	r1, #9
	movs	r2, #9
	movs	r3, #1
	ldr	r0, [pc, #620]
	bl	sub_08015038
	adds	r5, r0, #0
	b.n	.L_080ac0fe
.L_080ac0f6:
	movs	r0, #1
	str	r4, [sp, #8]
	bl	sub_080030f8
.L_080ac0fe:
	ldr	r4, [sp, #8]
	str	r4, [sp, #8]
	bl	sub_08015048
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_080ac0f6
	ldr	r1, [pc, #580]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080ac136
	adds	r7, r1, #0
	movs	r6, #1
.L_080ac11c:
	movs	r0, #2
	movs	r1, #146
	str	r4, [sp, #8]
	bl	sub_080a1a40
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [r7, #0]
	ands	r3, r6
	ldr	r4, [sp, #8]
	cmp	r3, #0
	beq.n	.L_080ac11c
.L_080ac136:
	movs	r1, #1
	adds	r0, r5, #0
	str	r4, [sp, #8]
	bl	sub_08015018
	mov	r0, r9
	bl	sub_080aafb8
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [pc, #504]
	movs	r3, #0
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r2, [pc, #520]
	movs	r3, #16
.L_080ac158:
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r4, [sp, #8]
	b.n	.L_080ac1b8
	ldr	r2, [pc, #484]
	add	r2, r9
	ldr	r3, [r2, #0]
	cmp	r3, #90
	bne.n	.L_080ac1b8
	movs	r3, #1
	mov	fp, r3
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [pc, #492]
	movs	r3, #21
	add	r2, r9
.L_080ac178:
	movs	r4, #1
	str	r3, [r2, #0]
	b.n	.L_080ac1b8
	ldr	r2, [pc, #456]
	add	r2, r9
	ldr	r3, [r2, #0]
	cmp	r3, #90
	bne.n	.L_080ac1b8
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [pc, #464]
	movs	r5, #32
	add	r2, r9
	movs	r3, #22
	mov	fp, r5
	movs	r4, #32
	str	r3, [r2, #0]
	b.n	.L_080ac1b8
	ldr	r2, [pc, #424]
	add	r2, r9
	ldr	r3, [r2, #0]
	cmp	r3, #60
	bne.n	.L_080ac1b8
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [pc, #436]
	movs	r3, #25
	add	r2, r9
	movs	r7, #2
	str	r3, [r2, #0]
	mov	fp, r7
.L_080ac1b6:
	movs	r4, #2
.L_080ac1b8:
	ldr	r0, [sp, #80]
	cmp	r0, #0
	beq.n	.L_080ac1c0
	b.n	.L_080ac304
.L_080ac1c0:
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L_080ac1cc
	b.n	.L_080ac2ea
.L_080ac1cc:
	movs	r2, #1
	ldr	r1, [sp, #60]
	negs	r2, r2
	cmp	r1, r2
	bne.n	.L_080ac1d8
	b.n	.L_080ac33c
.L_080ac1d8:
	movs	r3, #0
	ldr	r5, [sp, #40]
	str	r3, [sp, #68]
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r5
	ldr	r0, [sp, #40]
	lsrs	r7, r3, #8
	movs	r3, #224
	ands	r3, r5
	movs	r6, #31
	lsrs	r5, r3, #5
	ands	r6, r0
	adds	r1, r5, #0
	adds	r0, r7, #0
	adds	r2, r6, #0
	str	r4, [sp, #8]
	bl	sub_08077210
	ldr	r4, [sp, #8]
	cmp	r0, #0
	bne.n	.L_080ac214
	adds	r0, r7, #0
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl	sub_08077208
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L_080ac218
.L_080ac214:
	movs	r1, #1
	str	r1, [sp, #68]
.L_080ac218:
	movs	r2, #1
	str	r2, [sp, #36]
	ldr	r2, [pc, #324]
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r3, [sp, #68]
	cmp	r3, #0
	bne.n	.L_080ac25a
	movs	r0, #114
	bl	sub_080f9010
	ldr	r5, [sp, #76]
	ldr	r0, [r5, #48]
	bl	sub_08015278
	movs	r3, #104
	ldr	r0, [r5, #48]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #80
	movs	r3, #216
	bl	sub_08015068
	ldr	r0, [pc, #288]
	ldr	r1, [r5, #48]
	movs	r2, #0
	movs	r3, #96
	bl	sub_08015078
	movs	r7, #1
	str	r7, [sp, #72]
	bl	.L_080ab834
.L_080ac25a:
	ldr	r0, [sp, #40]
	lsrs	r3, r0, #15
	cmp	r3, #0
	beq.n	.L_080ac298
	movs	r0, #175
	str	r4, [sp, #8]
	bl	sub_080f9010
	movs	r5, #240
	ldr	r1, [sp, #40]
	lsls	r5, r5, #4
	movs	r6, #224
	ands	r5, r1
	ands	r6, r1
	movs	r3, #31
	ands	r3, r1
	lsrs	r5, r5, #8
	lsrs	r6, r6, #5
	adds	r2, r3, #0
	adds	r1, r6, #0
	adds	r0, r5, #0
	str	r3, [sp, #12]
	bl	sub_080771b8
	ldr	r3, [sp, #12]
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r3, #0
	bl	sub_080771c8
	b.n	.L_080ac2cc
.L_080ac298:
	movs	r0, #139
	str	r4, [sp, #8]
	bl	sub_080f9010
	movs	r5, #240
	ldr	r2, [sp, #40]
	lsls	r5, r5, #4
	movs	r6, #224
	ands	r5, r2
	ands	r6, r2
	movs	r3, #31
	ands	r3, r2
	lsrs	r5, r5, #8
	lsrs	r6, r6, #5
	adds	r2, r3, #0
	adds	r1, r6, #0
	adds	r0, r5, #0
	str	r3, [sp, #12]
	bl	sub_080771b0
	ldr	r3, [sp, #12]
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r3, #0
	bl	sub_080771c0
.L_080ac2cc:
	ldr	r4, [sp, #8]
	ldr	r3, [sp, #40]
	movs	r0, #240
	lsls	r0, r0, #4
	ands	r0, r3
	lsrs	r0, r0, #8
	str	r4, [sp, #8]
	bl	sub_08077010
	mov	r0, r9
	bl	sub_080aafb8
	movs	r5, #1
	str	r5, [sp, #72]
	ldr	r4, [sp, #8]
.L_080ac2ea:
	ldr	r7, [sp, #80]
	cmp	r7, #0
	bne.n	.L_080ac304
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_080ac304
	movs	r0, #112
	movs	r5, #7
	bl	sub_080f9010
	b.n	.L_080ac86a
.L_080ac304:
	movs	r3, #1
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L_080ac320
	ldr	r0, [sp, #80]
	cmp	r0, #1
	beq.n	.L_080ac314
	b.n	.L_080ac41e
.L_080ac314:
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L_080ac320
	b.n	.L_080ac41e
.L_080ac320:
	movs	r1, #1
	ldr	r3, [sp, #28]
	str	r1, [sp, #68]
	ldr	r5, [sp, #56]
	ldrb	r2, [r3, r5]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080ac3a4
	movs	r0, #1
	ldr	r7, [sp, #60]
	negs	r0, r0
	cmp	r7, r0
	bne.n	.L_080ac36c
.L_080ac33c:
	movs	r0, #114
	bl	sub_080f9010
	bl	.L_080ab834
	movs	r0, r0
	movs	r1, #40
	movs	r0, r0
	lsls	r0, r0, #9
	lsls	r0, r0, #8
	lsrs	r4, r0, #17
	movs	r0, r0
	adds	r4, r2, #2
	lsls	r0, r0, #12
	lsrs	r5, r0, #17
	movs	r0, r0
	lsrs	r6, r0, #17
	movs	r0, r0
	movs	r1, #44
	movs	r0, r0
	subs	r0, r7, r3
	lsls	r0, r0, #12
	lsrs	r6, r7, #14
	movs	r0, r0
.L_080ac36c:
	ldr	r2, [sp, #40]
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r2
	lsrs	r7, r3, #8
	movs	r3, #224
	ands	r3, r2
	movs	r5, #31
	movs	r1, #0
	lsrs	r6, r3, #5
	ands	r5, r2
	str	r1, [sp, #68]
	adds	r0, r7, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	sub_08077210
	cmp	r0, #0
	bne.n	.L_080ac3a0
	adds	r0, r7, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	sub_08077208
	cmp	r0, #0
	beq.n	.L_080ac3a4
.L_080ac3a0:
	movs	r3, #1
	str	r3, [sp, #68]
.L_080ac3a4:
	ldr	r5, [sp, #68]
	cmp	r5, #0
	bne.n	.L_080ac3d8
	movs	r0, #114
	bl	sub_080f9010
	ldr	r7, [sp, #76]
	ldr	r0, [r7, #48]
	bl	sub_08015278
	movs	r3, #104
	ldr	r0, [r7, #48]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #80
	movs	r3, #216
	bl	sub_08015068
	ldr	r0, [pc, #828]
	ldr	r1, [r7, #48]
	movs	r2, #0
	movs	r3, #96
	bl	sub_08015078
	bl	.L_080ab834
.L_080ac3d8:
	ldr	r0, [sp, #80]
	cmp	r0, #1
	bne.n	.L_080ac414
	ldr	r3, [sp, #56]
	ldr	r1, [sp, #28]
	ldrb	r2, [r1, r3]
	adds	r3, r0, #0
	ands	r3, r2
	movs	r5, #4
	cmp	r3, #0
	beq.n	.L_080ac416
	ldr	r5, [sp, #76]
	ldr	r7, [sp, #56]
	movs	r3, #28
	ldrsb	r3, [r5, r3]
	cmp	r7, r3
	bne.n	.L_080ac410
	movs	r0, #188
	lsls	r0, r0, #1
	adds	r3, r5, r0
	ldrh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #8
	ands	r3, r2
	movs	r5, #2
	cmp	r3, #0
	bne.n	.L_080ac416
	b.n	.L_080ac414
.L_080ac410:
	movs	r5, #3
	b.n	.L_080ac416
.L_080ac414:
	movs	r5, #1
.L_080ac416:
	movs	r0, #112
	bl	sub_080f9010
	b.n	.L_080ac86a
.L_080ac41e:
	movs	r3, #8
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_080ac42c
	movs	r0, #113
	movs	r5, #2
	b.n	.L_080ac438
.L_080ac42c:
	movs	r3, #2
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_080ac440
	movs	r0, #113
	movs	r5, #1
.L_080ac438:
	bl	sub_080f9010
	negs	r5, r5
	b.n	.L_080ac86a
.L_080ac440:
	ldr	r1, [sp, #80]
	cmp	r1, #0
	beq.n	.L_080ac448
	b.n	.L_080ac5d2
.L_080ac448:
	movs	r3, #4
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L_080ac452
	b.n	.L_080ac5d2
.L_080ac452:
	ldr	r2, [sp, #36]
	cmp	r2, #0
	bne.n	.L_080ac45a
	b.n	.L_080ac5aa
.L_080ac45a:
	ldr	r5, [sp, #32]
	movs	r3, #1
	eors	r5, r3
	str	r5, [sp, #32]
	cmp	r5, #0
	beq.n	.L_080ac46e
	movs	r0, #139
	bl	sub_080f9010
	b.n	.L_080ac474
.L_080ac46e:
	movs	r0, #175
	bl	sub_080f9010
.L_080ac474:
	movs	r7, #0
	ldr	r0, [sp, #76]
	ldr	r1, [pc, #656]
	str	r7, [sp, #68]
	adds	r3, r0, r1
	ldrb	r3, [r3, #0]
	cmp	r7, r3
	blt.n	.L_080ac486
	b.n	.L_080ac59e
.L_080ac486:
	movs	r2, #160
	movs	r3, #0
	str	r2, [sp, #20]
	str	r3, [sp, #16]
.L_080ac48e:
	movs	r5, #0
	str	r5, [sp, #64]
	ldr	r7, [sp, #20]
	mov	r0, r9
	ldrsb	r3, [r7, r0]
	cmp	r5, r3
	bge.n	.L_080ac57e
	ldr	r1, [sp, #16]
	lsls	r3, r1, #1
	add	r3, r9
	mov	sl, r3
.L_080ac4a4:
	mov	r2, sl
	ldrh	r7, [r2, #0]
	movs	r3, #2
	movs	r0, #240
	lsls	r0, r0, #4
	add	sl, r3
	adds	r3, r7, #0
	ands	r3, r0
	lsrs	r4, r3, #8
	movs	r5, #0
	movs	r1, #224
	adds	r3, r7, #0
	ands	r3, r1
	movs	r2, #31
	mov	r8, r5
	adds	r5, r7, #0
	lsrs	r6, r3, #5
	ands	r5, r2
	adds	r0, r4, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	str	r4, [sp, #8]
	bl	sub_08077210
	ldr	r4, [sp, #8]
	cmp	r0, #0
	bne.n	.L_080ac4e8
	adds	r0, r4, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	sub_08077208
	cmp	r0, #0
	beq.n	.L_080ac4ec
.L_080ac4e8:
	movs	r3, #1
	mov	r8, r3
.L_080ac4ec:
	mov	r5, r8
	cmp	r5, #0
	beq.n	.L_080ac56e
	ldr	r0, [sp, #32]
	cmp	r0, #0
	beq.n	.L_080ac534
	lsrs	r3, r7, #15
	cmp	r3, #0
	bne.n	.L_080ac56e
	movs	r1, #240
	lsls	r1, r1, #4
	movs	r2, #224
	adds	r5, r7, #0
	adds	r6, r7, #0
	ands	r5, r1
	ands	r6, r2
	movs	r3, #31
	ands	r3, r7
	lsrs	r5, r5, #8
	lsrs	r6, r6, #5
	adds	r2, r3, #0
	adds	r1, r6, #0
	adds	r0, r5, #0
	str	r3, [sp, #12]
	bl	sub_080771b0
	ldr	r3, [sp, #12]
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r3, #0
	bl	sub_080771c0
	adds	r0, r5, #0
	bl	sub_08077010
	b.n	.L_080ac56e
.L_080ac534:
	lsrs	r3, r7, #15
	cmp	r3, #0
	beq.n	.L_080ac56e
	movs	r3, #240
	lsls	r3, r3, #4
	movs	r0, #224
	adds	r5, r7, #0
	adds	r6, r7, #0
	ands	r5, r3
	ands	r6, r0
	movs	r3, #31
	ands	r3, r7
	lsrs	r5, r5, #8
	lsrs	r6, r6, #5
	adds	r2, r3, #0
	adds	r1, r6, #0
	adds	r0, r5, #0
	str	r3, [sp, #12]
	bl	sub_080771b8
	ldr	r3, [sp, #12]
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r3, #0
	bl	sub_080771c8
	adds	r0, r5, #0
	bl	sub_08077010
.L_080ac56e:
	ldr	r1, [sp, #64]
	adds	r1, #1
	str	r1, [sp, #64]
	ldr	r2, [sp, #20]
	mov	r5, r9
	ldrsb	r3, [r2, r5]
	cmp	r1, r3
	blt.n	.L_080ac4a4
.L_080ac57e:
	ldr	r7, [sp, #20]
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #68]
	adds	r7, #1
	adds	r0, #10
	adds	r1, #1
	ldr	r2, [sp, #76]
	ldr	r5, [pc, #380]
	str	r7, [sp, #20]
	str	r0, [sp, #16]
	str	r1, [sp, #68]
	adds	r3, r2, r5
	ldrb	r3, [r3, #0]
	cmp	r1, r3
	bge.n	.L_080ac59e
	b.n	.L_080ac48e
.L_080ac59e:
	mov	r0, r9
	bl	sub_080aafb8
	movs	r7, #1
	str	r7, [sp, #72]
	b.n	.L_080ac5d2
.L_080ac5aa:
	ldr	r1, [sp, #76]
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #56]
	ldr	r0, [r1, #48]
	subs	r1, r2, r3
	ldr	r2, [sp, #48]
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #15
	str	r3, [sp, #4]
	adds	r1, #1
	adds	r2, #2
	movs	r3, #6
	bl	sub_080ab1f4
	movs	r0, #112
	movs	r5, #10
	bl	sub_080f9010
	b.n	.L_080ac86a
.L_080ac5d2:
	movs	r3, #64
	mov	r5, fp
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080ac682
	movs	r0, #111
	bl	sub_080f9010
	ldr	r7, [sp, #28]
	ldr	r0, [sp, #56]
	movs	r5, #4
	ldrb	r2, [r7, r0]
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ac5f6
	bl	.L_080ab834
.L_080ac5f6:
	ldr	r1, [sp, #76]
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #56]
	ldr	r0, [r1, #48]
	subs	r1, r2, r3
	ldr	r2, [sp, #48]
	movs	r3, #15
	adds	r2, #2
	str	r3, [sp, #4]
	adds	r1, #1
	movs	r3, #6
	movs	r6, #1
	str	r6, [sp, #0]
	bl	sub_080ab1f4
	ldr	r0, [sp, #56]
	ldrb	r2, [r7, r0]
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ac624
	bl	.L_080ab834
.L_080ac624:
	adds	r3, r6, #0
	ands	r3, r2
	movs	r1, #1
	cmp	r3, #0
	beq.n	.L_080ac63c
	movs	r3, #2
	negs	r3, r3
	ands	r3, r2
	movs	r1, #0
	strb	r3, [r7, r0]
	str	r1, [sp, #48]
	b.n	.L_080ac65c
.L_080ac63c:
	ldr	r3, [sp, #48]
	cmp	r3, #0
	bne.n	.L_080ac65c
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ac65c
	adds	r3, r2, #0
	ldr	r5, [sp, #28]
	ldr	r7, [sp, #56]
	orrs	r3, r1
	movs	r0, #2
	strb	r3, [r5, r7]
	str	r0, [sp, #72]
	bl	.L_080ab834
.L_080ac65c:
	ldr	r1, [sp, #48]
	ldr	r3, [sp, #56]
	subs	r1, #1
	str	r1, [sp, #48]
	adds	r3, #160
	mov	r2, r9
	ldrsb	r1, [r2, r3]
	cmp	r1, #0
	bne.n	.L_080ac670
	movs	r1, #1
.L_080ac670:
	ldr	r0, [sp, #48]
	bl	sub_080aa538
	movs	r3, #2
	str	r0, [sp, #48]
	str	r0, [sp, #44]
	str	r3, [sp, #72]
	bl	.L_080ab834
.L_080ac682:
	movs	r3, #128
	mov	r5, fp
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080ac738
	movs	r0, #111
	bl	sub_080f9010
	ldr	r7, [sp, #28]
	ldr	r0, [sp, #56]
	movs	r5, #4
	ldrb	r2, [r7, r0]
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ac6a6
	bl	.L_080ab834
.L_080ac6a6:
	ldr	r1, [sp, #76]
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #56]
	ldr	r0, [r1, #48]
	subs	r1, r2, r3
	ldr	r2, [sp, #48]
	movs	r3, #15
	adds	r1, #1
	str	r3, [sp, #4]
	adds	r2, #2
	movs	r3, #6
	movs	r6, #1
	str	r6, [sp, #0]
	bl	sub_080ab1f4
	ldr	r7, [sp, #48]
	ldr	r3, [sp, #56]
	adds	r7, #1
	str	r7, [sp, #48]
	adds	r3, #160
	mov	r0, r9
	ldrsb	r1, [r0, r3]
	cmp	r1, #0
	bne.n	.L_080ac6d8
	movs	r1, #1
.L_080ac6d8:
	ldr	r0, [sp, #48]
	bl	sub_080aa538
	ldr	r3, [sp, #56]
	str	r0, [sp, #48]
	ldr	r1, [sp, #28]
	ldrb	r2, [r1, r3]
	adds	r3, r6, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ac710
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080ac710
	movs	r3, #2
	negs	r3, r3
	ldr	r5, [sp, #56]
	ands	r3, r2
	movs	r7, #0
	strb	r3, [r1, r5]
	str	r7, [sp, #48]
	b.n	.L_080ac72c
	movs	r0, r0
	.4byte 0x00000bbe
	.2byte 0x0219
	.2byte 0x0000
.L_080ac710:
	ldr	r0, [sp, #48]
	cmp	r0, #0
	bne.n	.L_080ac72c
	ldr	r3, [sp, #56]
	ldr	r1, [sp, #28]
	ldrb	r2, [r1, r3]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ac72c
	movs	r3, #1
	ldr	r5, [sp, #56]
	orrs	r3, r2
	strb	r3, [r1, r5]
.L_080ac72c:
	ldr	r7, [sp, #48]
	movs	r0, #2
	str	r7, [sp, #44]
	str	r0, [sp, #72]
	bl	.L_080ab834
.L_080ac738:
	movs	r3, #32
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ac7bc
	movs	r0, #111
	bl	sub_080f9010
	ldr	r3, [sp, #28]
	ldr	r5, [sp, #56]
	ldrb	r2, [r3, r5]
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080ac772
	ldr	r2, [sp, #24]
	ldr	r7, [sp, #76]
	subs	r1, r2, r5
	movs	r3, #1
	ldr	r2, [sp, #48]
	ldr	r0, [r7, #48]
	str	r3, [sp, #0]
	movs	r3, #15
	str	r3, [sp, #4]
	adds	r1, #1
	adds	r2, #2
	movs	r3, #6
	bl	sub_080ab1f4
.L_080ac772:
	ldr	r3, [sp, #56]
	ldr	r7, [sp, #76]
	subs	r3, #1
	ldr	r0, [pc, #380]
	str	r3, [sp, #56]
	adds	r5, r7, r0
	ldrb	r1, [r5, #0]
	adds	r0, r3, #0
	bl	sub_080aa538
	ldr	r1, [sp, #80]
	str	r0, [sp, #56]
	cmp	r1, #0
	bne.n	.L_080ac842
	movs	r2, #0
	str	r2, [sp, #68]
	ldrb	r3, [r5, #0]
	cmp	r1, r3
	bge.n	.L_080ac842
.L_080ac798:
	ldr	r7, [sp, #28]
	ldr	r0, [sp, #56]
	ldrsb	r3, [r7, r0]
	cmp	r3, #4
	bne.n	.L_080ac7ae
	subs	r0, #1
	str	r0, [sp, #56]
	ldrb	r1, [r5, #0]
	bl	sub_080aa538
	str	r0, [sp, #56]
.L_080ac7ae:
	ldr	r1, [sp, #68]
	adds	r1, #1
	str	r1, [sp, #68]
	ldrb	r3, [r5, #0]
	cmp	r1, r3
	blt.n	.L_080ac798
	b.n	.L_080ac842
.L_080ac7bc:
	movs	r3, #16
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_080ac7ca
	bl	.L_080ab834
.L_080ac7ca:
	movs	r0, #111
	bl	sub_080f9010
	ldr	r3, [sp, #28]
	ldr	r5, [sp, #56]
	ldrb	r2, [r3, r5]
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080ac7fa
	ldr	r2, [sp, #24]
	ldr	r7, [sp, #76]
	subs	r1, r2, r5
	movs	r3, #1
	ldr	r2, [sp, #48]
	ldr	r0, [r7, #48]
	str	r3, [sp, #0]
	movs	r3, #15
	str	r3, [sp, #4]
	adds	r1, #1
	adds	r2, #2
	movs	r3, #6
	bl	sub_080ab1f4
.L_080ac7fa:
	ldr	r3, [sp, #56]
	ldr	r7, [sp, #76]
	adds	r3, #1
	ldr	r0, [pc, #244]
	str	r3, [sp, #56]
	adds	r5, r7, r0
	ldrb	r1, [r5, #0]
	adds	r0, r3, #0
	bl	sub_080aa538
	ldr	r1, [sp, #80]
	str	r0, [sp, #56]
	cmp	r1, #0
	bne.n	.L_080ac842
	movs	r2, #0
	str	r2, [sp, #68]
	ldrb	r3, [r5, #0]
	cmp	r1, r3
	bge.n	.L_080ac842
.L_080ac820:
	ldr	r7, [sp, #28]
	ldr	r0, [sp, #56]
	ldrsb	r3, [r7, r0]
	cmp	r3, #4
	bne.n	.L_080ac836
	adds	r0, #1
	str	r0, [sp, #56]
	ldrb	r1, [r5, #0]
	bl	sub_080aa538
	str	r0, [sp, #56]
.L_080ac836:
	ldr	r1, [sp, #68]
	adds	r1, #1
	str	r1, [sp, #68]
	ldrb	r3, [r5, #0]
	cmp	r1, r3
	blt.n	.L_080ac820
.L_080ac842:
	ldr	r2, [sp, #44]
	ldr	r3, [sp, #56]
	str	r2, [sp, #48]
	adds	r3, #160
	mov	r5, r9
	ldrsb	r1, [r5, r3]
	cmp	r1, #0
	bne.n	.L_080ac854
	movs	r1, #1
.L_080ac854:
	ldr	r0, [sp, #48]
	bl	sub_080aa538
	str	r0, [sp, #48]
	ldr	r0, [sp, #56]
	movs	r7, #2
	lsls	r0, r0, #3
	str	r7, [sp, #72]
	str	r0, [sp, #24]
	bl	.L_080ab834
.L_080ac86a:
	ldr	r3, [sp, #80]
	add	r1, sp, #56
	ldrb	r2, [r1, #0]
	ldr	r1, [sp, #76]
	adds	r3, #28
	strb	r2, [r1, r3]
	ldr	r2, [sp, #60]
	movs	r3, #1
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L_080ac8cc
	ldr	r2, [sp, #56]
	ldr	r7, [sp, #52]
	movs	r0, #188
	lsls	r0, r0, #1
	lsls	r3, r2, #2
	adds	r1, r7, r0
	adds	r3, r3, r2
	ldr	r7, [sp, #60]
	lsls	r3, r3, #1
	adds	r3, r3, r7
	lsls	r3, r3, #1
	mov	r0, r9
	ldrh	r2, [r0, r3]
	ldr	r3, [sp, #76]
	strh	r2, [r3, r1]
	ldr	r7, [sp, #80]
	movs	r1, #149
	lsls	r1, r1, #2
	adds	r0, r7, r1
	movs	r3, #31
	ldr	r7, [sp, #76]
	ands	r3, r2
	strb	r3, [r7, r0]
	movs	r3, #224
	adds	r1, r7, #0
	ands	r3, r2
	adds	r1, #2
	lsrs	r3, r3, #5
	strb	r3, [r1, r0]
	ldr	r0, [sp, #80]
	movs	r3, #150
	lsls	r3, r3, #2
	adds	r1, r0, r3
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r2
	lsrs	r3, r3, #8
	strb	r3, [r7, r1]
.L_080ac8cc:
	ldr	r1, [sp, #48]
	ldr	r7, [sp, #52]
	movs	r0, #186
	lsls	r0, r0, #1
	lsls	r3, r1, #2
	adds	r2, r7, r0
	adds	r3, r3, r1
	ldr	r7, [sp, #56]
	ldr	r0, [sp, #76]
	lsls	r3, r3, #1
	adds	r3, r7, r3
	strh	r3, [r0, r2]
	add	sp, #108
	adds	r0, r5, #0
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000219
