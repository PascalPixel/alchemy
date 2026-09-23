.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_0813ba50, 0x0813ba50
	.set sub_0813c5f4, 0x0813c5f4
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_081963ec, 0x081963ec
	.set sub_081c0010, 0x081c0010
	.global Func_0813bba0
	.thumb_func
Func_0813bba0:
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #176
	ldrh	r1, [r3, #10]
	movs	r2, #197
	lsls	r2, r2, #8
	adds	r2, #255
	ands	r2, r1
	strh	r2, [r3, #10]
	movs	r2, #254
	ldrh	r1, [r3, #10]
	lsls	r2, r2, #7
	adds	r2, #255
	ands	r2, r1
	strh	r2, [r3, #10]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r3, #10]
	ldr	r0, [pc, #8]
	adds	r1, #64
	ldr	r2, [pc, #8]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	.4byte 0x02010000
	.2byte 0x0001
	.2byte 0xa260
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #96]
	sub	sp, #36
	ldr	r1, [r3, #92]
	str	r2, [sp, #24]
	mov	sl, r0
	ldr	r3, [r3, #100]
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #1
	str	r3, [sp, #16]
	mov	r9, r1
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r1, #224
	adds	r2, #32
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	ldr	r0, [pc, #56]
	add	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #44]
	ldr	r1, [sp, #16]
	bl	sub_08157cf4
	bl	sub_0813ba50
	ldr	r3, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #12]
	subs	r2, #8
	b.n	.L_0813bc54
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00003f44
	.4byte 0x00003337
	.4byte 0x00000177
	.2byte 0x017e
	.2byte 0x0000
.L_0813bc54:
	movs	r7, #128
	negs	r7, r7
	strh	r3, [r2, #0]
	movs	r3, #0
	mov	fp, r7
	mov	r8, r3
	adds	r7, #112
	mov	r6, r9
.L_0813bc64:
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	movs	r2, #63
	movs	r3, #7
	ands	r3, r0
	ands	r2, r5
	mov	r1, sl
	adds	r2, r2, r3
	ldr	r3, [r1, #4]
	adds	r2, #24
	cmp	r3, #1
	bne.n	.L_0813bc8a
	adds	r3, r2, r7
	adds	r2, r3, #0
	adds	r2, #24
	b.n	.L_0813bc90
.L_0813bc8a:
	subs	r3, r2, r7
	adds	r2, r3, #0
	adds	r2, #80
.L_0813bc90:
	lsls	r3, r2, #3
	mov	r2, fp
	str	r2, [r6, #4]
	movs	r1, #64
	movs	r2, #1
	str	r3, [r6, #0]
	negs	r1, r1
	movs	r3, #1
	add	r8, r2
	negs	r3, r3
	add	fp, r1
	mov	r1, r8
	str	r3, [r6, #24]
	subs	r7, #8
	adds	r6, #28
	cmp	r1, #32
	bne.n	.L_0813bc64
	movs	r2, #0
	mov	r8, r2
	adds	r2, r3, #0
	movs	r3, #230
	lsls	r3, r3, #2
	add	r3, r9
.L_0813bcbe:
	movs	r7, #1
	add	r8, r7
	mov	r1, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r1, #32
	bne.n	.L_0813bcbe
	mov	r2, sl
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_0813bce6
	movs	r1, #18
	movs	r0, #104
	bl	sub_081963ec
	movs	r0, #188
	movs	r1, #2
	bl	sub_081963ec
	b.n	.L_0813bcf6
.L_0813bce6:
	movs	r1, #22
	movs	r0, #104
	bl	sub_081963ec
	movs	r0, #188
	movs	r1, #6
	bl	sub_081963ec
.L_0813bcf6:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #104]
	adds	r3, #188
	str	r2, [sp, #28]
	movs	r7, #28
	ldr	r3, [r3, #0]
	add	r7, sp
	mov	r1, sl
	str	r3, [r7, #4]
	ldr	r3, [r1, #4]
	mov	fp, r7
	cmp	r3, #0
	bne.n	.L_0813bd66
	movs	r2, #0
	mov	r8, r2
	ldr	r6, [pc, #40]
	ldr	r2, [pc, #48]
	ldr	r5, [pc, #40]
	ldr	r4, [pc, #40]
	ldr	r0, [pc, #48]
	movs	r1, #224
	lsls	r1, r1, #7
.L_0813bd24:
	mov	r3, r8
	subs	r3, #8
	cmp	r3, #95
	bhi.n	.L_0813bd36
	mov	r7, r8
	subs	r3, r6, r7
	orrs	r3, r1
	strh	r3, [r2, #0]
	b.n	.L_0813bd56
.L_0813bd36:
	mov	r3, r8
	cmp	r3, #135
	bgt.n	.L_0813bd54
	strh	r5, [r2, #0]
	b.n	.L_0813bd56
	.4byte 0x000000f0
	.4byte 0x00000888
	.4byte 0x00000100
	.4byte 0x02010000
	.2byte 0xff00
	.2byte 0xffff
.L_0813bd54:
	strh	r4, [r2, #0]
.L_0813bd56:
	movs	r7, #1
	add	r8, r7
	mov	r3, r8
	adds	r2, #2
	adds	r1, r1, r0
	cmp	r3, #160
	bne.n	.L_0813bd24
	b.n	.L_0813bdb0
.L_0813bd66:
	ldr	r5, [pc, #44]
	ldr	r4, [pc, #44]
	ldr	r2, [pc, #48]
	movs	r7, #0
	movs	r1, #192
	movs	r0, #128
	mov	r8, r7
	lsls	r1, r1, #5
	lsls	r0, r0, #1
.L_0813bd78:
	mov	r3, r8
	subs	r3, #8
	cmp	r3, #87
	bhi.n	.L_0813bd88
	adds	r3, #160
	orrs	r3, r1
	strh	r3, [r2, #0]
	b.n	.L_0813bda2
.L_0813bd88:
	mov	r3, r8
	cmp	r3, #135
	bgt.n	.L_0813bda0
	strh	r5, [r2, #0]
	b.n	.L_0813bda2
	movs	r0, r0
	.4byte 0x000078f8
	.4byte 0x00000100
	.2byte 0x0000
	.2byte 0x0201
.L_0813bda0:
	strh	r4, [r2, #0]
.L_0813bda2:
	movs	r7, #1
	add	r8, r7
	mov	r3, r8
	adds	r2, #2
	adds	r1, r1, r0
	cmp	r3, #160
	bne.n	.L_0813bd78
.L_0813bdb0:
	movs	r1, #200
	ldr	r0, [pc, #752]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r2, #239
	lsls	r2, r2, #7
	movs	r3, #2
	add	r2, r9
	str	r3, [r2, #0]
	mov	r7, sl
	ldr	r3, [r7, #24]
	cmp	r3, #1
	bne.n	.L_0813bdd8
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, r9
	movs	r3, #75
	b.n	.L_0813bde2
.L_0813bdd8:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, r9
	movs	r3, #50
.L_0813bde2:
	str	r3, [r2, #0]
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #700]
	bl	sub_080145a8
	movs	r1, #0
	str	r1, [sp, #20]
	mov	r2, sl
	ldr	r4, [r2, #24]
	ldr	r5, [pc, #692]
	adds	r2, r4, #0
	lsls	r3, r2, #1
	adds	r3, #1
	ldrb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L_0813be06
	b.n	.L_0813c074
.L_0813be06:
	lsls	r3, r2, #1
	adds	r3, #1
	ldrb	r3, [r5, r3]
	ldr	r7, [sp, #20]
	subs	r3, #16
	cmp	r7, r3
	bne.n	.L_0813be1e
	movs	r0, #133
	bl	sub_081180e8
	mov	r1, sl
	ldr	r4, [r1, #24]
.L_0813be1e:
	lsls	r3, r4, #1
	ldrb	r3, [r5, r3]
	movs	r2, #0
	mov	r8, r2
	cmp	r3, #0
	bne.n	.L_0813be2c
	b.n	.L_0813bfb8
.L_0813be2c:
	mov	r7, r9
.L_0813be2e:
	ldr	r0, [r7, #24]
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_0813beec
	ldr	r2, [r7, #0]
	cmp	r2, #0
	bge.n	.L_0813be40
	adds	r2, #7
.L_0813be40:
	ldr	r3, [r7, #4]
	asrs	r2, r2, #3
	cmp	r3, #0
	bge.n	.L_0813be4a
	adds	r3, #7
.L_0813be4a:
	asrs	r5, r3, #3
	movs	r1, #4
	cmp	r4, #2
	beq.n	.L_0813be54
	movs	r1, #0
.L_0813be54:
	movs	r3, #32
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	mov	r3, fp
	ldr	r4, [r1, r3]
	movs	r1, #224
	lsls	r1, r1, #3
	add	r1, r9
	adds	r3, r5, #0
	ldr	r0, [sp, #24]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x21c0
	ldr	r3, [r7, #4]
	lsls	r1, r1, #1
	adds	r1, #255
	cmp	r3, r1
	bgt.n	.L_0813be94
	mov	r2, sl
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_0813be86
	ldr	r3, [r7, #0]
	subs	r3, #64
	b.n	.L_0813be8a
.L_0813be86:
	ldr	r3, [r7, #0]
	adds	r3, #64
.L_0813be8a:
	str	r3, [r7, #0]
	ldr	r3, [r7, #4]
	adds	r3, #64
	str	r3, [r7, #4]
	b.n	.L_0813bee2
.L_0813be94:
	movs	r3, #3
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_0813bea4
	movs	r0, #115
	bl	sub_081c0010
.L_0813bea4:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r3, #0
	str	r3, [r7, #24]
	mov	r2, sl
	ldr	r3, [r2, #20]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0813bee2
	movs	r4, #8
	movs	r6, #36
.L_0813bec2:
	mov	r3, sl
	ldrsh	r0, [r6, r3]
	movs	r2, #5
	adds	r3, r5, #0
	movs	r1, #9
	str	r4, [sp, #0]
	str	r4, [sp, #8]
	bl	sub_0814cd48
	mov	r2, sl
	ldr	r3, [r2, #20]
	adds	r5, #1
	adds	r6, #2
	ldr	r4, [sp, #8]
	cmp	r5, r3
	bne.n	.L_0813bec2
.L_0813bee2:
	ldr	r0, [r7, #24]
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_0813bfa2
.L_0813beec:
	ldr	r2, [r7, #0]
	cmp	r2, #0
	bge.n	.L_0813bef4
	adds	r2, #7
.L_0813bef4:
	ldr	r3, [r7, #4]
	asrs	r2, r2, #3
	cmp	r3, #0
	bge.n	.L_0813befe
	adds	r3, #7
.L_0813befe:
	asrs	r6, r3, #3
	subs	r3, r0, #1
	cmp	r3, #13
	bhi.n	.L_0813bf3c
	mov	r1, sl
	ldr	r3, [r1, #24]
	movs	r5, #4
	cmp	r3, #2
	beq.n	.L_0813bf12
	movs	r5, #0
.L_0813bf12:
	movs	r1, #3
	str	r2, [sp, #12]
	bl	sub_08002054
	adds	r1, r0, #0
	lsls	r1, r1, #10
	movs	r3, #176
	lsls	r3, r3, #4
	add	r1, r9
	adds	r1, r1, r3
	movs	r3, #32
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	add	r5, fp
	ldr	r4, [r5, #0]
	ldr	r0, [sp, #24]
	ldr	r2, [sp, #12]
	adds	r3, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69b8
.L_0813bf3c:
	adds	r3, r0, #0
	subs	r3, #9
	cmp	r3, #2
	bhi.n	.L_0813bf9a
	movs	r6, #224
	lsls	r6, r6, #2
	movs	r5, #0
	add	r6, r9
.L_0813bf4c:
	ldr	r3, [r6, #24]
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L_0813bf92
	movs	r3, #18
	str	r3, [r6, #24]
	bl	sub_08014878
	movs	r3, #31
	ands	r0, r3
	ldr	r3, [r7, #0]
	cmp	r3, #0
	bge.n	.L_0813bf6a
	adds	r3, #7
.L_0813bf6a:
	asrs	r3, r3, #3
	adds	r3, r0, r3
	lsls	r3, r3, #3
	adds	r3, #8
	str	r3, [r6, #0]
	bl	sub_08014878
	movs	r3, #15
	ands	r0, r3
	ldr	r3, [r7, #4]
	cmp	r3, #0
	bge.n	.L_0813bf84
	adds	r3, #7
.L_0813bf84:
	asrs	r3, r3, #3
	adds	r3, r0, r3
	subs	r3, #15
	lsls	r3, r3, #3
	str	r3, [r6, #4]
	ldr	r0, [r7, #24]
	b.n	.L_0813bf9a
.L_0813bf92:
	adds	r5, #1
	adds	r6, #28
	cmp	r5, #32
	bne.n	.L_0813bf4c
.L_0813bf9a:
	cmp	r0, #14
	bgt.n	.L_0813bfa2
	adds	r3, r0, #1
	str	r3, [r7, #24]
.L_0813bfa2:
	mov	r1, sl
	ldr	r4, [r1, #24]
	ldr	r3, [pc, #260]
	movs	r2, #1
	add	r8, r2
	lsls	r2, r4, #1
	ldrb	r3, [r3, r2]
	adds	r7, #28
	cmp	r8, r3
	beq.n	.L_0813bfb8
	b.n	.L_0813be2e
.L_0813bfb8:
	movs	r5, #224
	movs	r2, #0
	lsls	r5, r5, #2
	mov	r8, r2
	add	r5, r9
.L_0813bfc2:
	ldr	r2, [r5, #24]
	movs	r3, #1
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L_0813c02e
	cmp	r2, #17
	bgt.n	.L_0813c022
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	ldr	r2, [r5, #0]
	asrs	r0, r3, #1
	cmp	r2, #0
	bge.n	.L_0813bfde
	adds	r2, #7
.L_0813bfde:
	ldr	r6, [pc, #208]
	asrs	r2, r2, #3
	ldrb	r3, [r6, r0]
	lsrs	r1, r3, #1
	ldr	r3, [r5, #4]
	subs	r2, r2, r1
	mov	ip, r2
	cmp	r3, #0
	bge.n	.L_0813bff2
	adds	r3, #7
.L_0813bff2:
	asrs	r3, r3, #3
	subs	r7, r3, r1
	mov	r1, sl
	ldr	r3, [r1, #24]
	movs	r4, #4
	cmp	r3, #2
	beq.n	.L_0813c002
	movs	r4, #0
.L_0813c002:
	ldr	r2, [pc, #176]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldrb	r3, [r6, r0]
	ldr	r2, [sp, #16]
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	mov	r3, fp
	adds	r1, r2, r1
	ldr	r4, [r4, r3]
	mov	r2, ip
	ldr	r0, [sp, #24]
	adds	r3, r7, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69aa
.L_0813c022:
	movs	r7, #1
	negs	r7, r7
	cmp	r2, r7
	ble.n	.L_0813c02e
	subs	r3, r2, #1
	str	r3, [r5, #24]
.L_0813c02e:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r5, #28
	cmp	r2, #32
	bne.n	.L_0813bfc2
	bl	sub_081434f8
	movs	r1, #4
	movs	r0, #4
	bl	sub_08158ce0
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [sp, #20]
	mov	r7, sl
	adds	r3, #1
	str	r3, [sp, #20]
	ldr	r2, [r7, #24]
	ldr	r5, [pc, #72]
	lsls	r3, r2, #1
	adds	r3, #1
	ldrb	r3, [r5, r3]
	ldr	r1, [sp, #20]
	adds	r4, r2, #0
	cmp	r1, r3
	beq.n	.L_0813c074
	b.n	.L_0813be06
.L_0813c074:
	ldr	r0, [pc, #48]
	bl	sub_08014644
	ldr	r0, [pc, #40]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	bl	sub_0813ba50
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0813bba1
	.4byte 0x08143001
	.4byte 0x08197521
	.4byte 0x0819745e
	.2byte 0x744c
	.2byte 0x0819
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r5, #192
	lsls	r5, r5, #18
	mov	sl, r0
	ldr	r0, [r5, #92]
	sub	sp, #36
	str	r0, [sp, #32]
	movs	r0, #128
	ldr	r1, [r5, #96]
	lsls	r0, r0, #6
	adds	r0, #1
	str	r1, [sp, #28]
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r2, [sp, #32]
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #4
	adds	r1, r2, r3
	ldr	r0, [pc, #44]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r4, [sp, #32]
	movs	r7, #224
	lsls	r7, r7, #3
	adds	r1, r4, r7
	ldr	r0, [pc, #32]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	bl	sub_0813ba50
	ldr	r3, [pc, #8]
	movs	r2, #128
	lsls	r2, r2, #19
	b.n	.L_0813c12c
	.4byte 0x00000100
	.4byte 0x00003f44
	.4byte 0x00000190
	.2byte 0x0137
	.2byte 0x0000
.L_0813c12c:
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #8
	strh	r3, [r2, #0]
	movs	r1, #18
	movs	r0, #104
	bl	sub_081963ec
	ldr	r0, [r5, #104]
	movs	r1, #2
	str	r0, [sp, #16]
	movs	r0, #188
	bl	sub_081963ec
	adds	r5, #188
	ldr	r5, [r5, #0]
	ldr	r3, [pc, #20]
	str	r5, [sp, #20]
	movs	r1, #0
	movs	r2, #128
	mov	r8, r1
	lsls	r2, r2, #2
	subs	r1, #1
	b.n	.L_0813c168
	movs	r0, r0
	.4byte 0x00003337
	.2byte 0x0158
	.2byte 0x0201
.L_0813c168:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_0813c168
	movs	r5, #0
	mov	r8, r5
	ldr	r6, [pc, #156]
	ldr	r5, [sp, #32]
.L_0813c17c:
	bl	sub_08014878
	mov	r7, sl
	movs	r3, #63
	ands	r0, r3
	ldr	r3, [r7, #24]
	lsls	r3, r3, #2
	adds	r3, #2
	ldrb	r3, [r6, r3]
	mov	r1, r8
	muls	r1, r3
	adds	r3, r1, #0
	adds	r3, #16
	negs	r2, r3
	ldr	r3, [r7, #4]
	cmp	r3, #1
	bne.n	.L_0813c1ac
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	adds	r3, r0, r3
	adds	r0, r3, #0
	subs	r0, #48
	b.n	.L_0813c1b8
.L_0813c1ac:
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	subs	r3, r0, r3
	adds	r0, r3, #0
	adds	r0, #72
.L_0813c1b8:
	lsls	r3, r0, #3
	str	r3, [r5, #0]
	lsls	r3, r2, #3
	str	r3, [r5, #4]
	movs	r2, #1
	movs	r3, #1
	negs	r3, r3
	add	r8, r2
	str	r3, [r5, #24]
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #64
	bne.n	.L_0813c17c
	mov	r4, sl
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_0813c238
	movs	r5, #0
	mov	r8, r5
	ldr	r1, [pc, #60]
	ldr	r5, [pc, #40]
	ldr	r4, [pc, #44]
	ldr	r0, [pc, #44]
.L_0813c1e6:
	mov	r3, r8
	subs	r3, #8
	cmp	r3, #95
	bhi.n	.L_0813c202
	mov	r7, r8
	lsrs	r3, r7, #31
	add	r3, r8
	asrs	r3, r3, #1
	subs	r2, r5, r3
	lsls	r2, r2, #8
	subs	r3, r4, r3
	orrs	r2, r3
	strh	r2, [r1, #0]
	b.n	.L_0813c224
.L_0813c202:
	mov	r2, r8
	cmp	r2, #135
	bgt.n	.L_0813c220
	strh	r0, [r1, #0]
	b.n	.L_0813c224
	.4byte 0x00000034
	.4byte 0x000000b4
	.4byte 0x00000080
	.4byte 0x08197527
	.2byte 0x0000
	.2byte 0x0201
.L_0813c220:
	ldr	r3, [pc, #16]
	strh	r3, [r1, #0]
.L_0813c224:
	movs	r3, #1
	add	r8, r3
	mov	r7, r8
	adds	r1, #2
	cmp	r7, #160
	bne.n	.L_0813c1e6
	b.n	.L_0813c286
	movs	r0, r0
	.2byte 0x0100
	.2byte 0x0000
.L_0813c238:
	movs	r0, #0
	mov	r8, r0
	ldr	r4, [pc, #44]
	ldr	r0, [pc, #48]
	ldr	r1, [pc, #48]
.L_0813c242:
	mov	r3, r8
	subs	r3, #8
	cmp	r3, #95
	bhi.n	.L_0813c260
	mov	r2, r8
	lsrs	r3, r2, #31
	add	r3, r8
	asrs	r3, r3, #1
	adds	r2, r3, #0
	adds	r2, #60
	lsls	r2, r2, #8
	adds	r3, #188
	orrs	r2, r3
	strh	r2, [r1, #0]
	b.n	.L_0813c27a
.L_0813c260:
	mov	r3, r8
	cmp	r3, #135
	bgt.n	.L_0813c278
	strh	r4, [r1, #0]
	b.n	.L_0813c27a
	movs	r0, r0
	.4byte 0x000070f0
	.4byte 0x00000100
	.2byte 0x0000
	.2byte 0x0201
.L_0813c278:
	strh	r0, [r1, #0]
.L_0813c27a:
	movs	r5, #1
	add	r8, r5
	mov	r7, r8
	adds	r1, #2
	cmp	r7, #160
	bne.n	.L_0813c242
.L_0813c286:
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #776]
	bl	sub_080145a8
	mov	r0, sl
	ldr	r1, [r0, #24]
	cmp	r1, #0
	bne.n	.L_0813c2b2
	ldr	r2, [sp, #32]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r3, r2, r4
	movs	r2, #1
	str	r2, [r3, #0]
	ldr	r5, [sp, #32]
	movs	r7, #238
	lsls	r7, r7, #7
	adds	r7, #132
	adds	r3, r5, r7
	str	r1, [r3, #0]
	b.n	.L_0813c2e6
.L_0813c2b2:
	cmp	r1, #1
	bne.n	.L_0813c2ce
	ldr	r0, [sp, #32]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r0, r1
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r0, r3
	movs	r3, #50
	b.n	.L_0813c2e4
.L_0813c2ce:
	ldr	r4, [sp, #32]
	movs	r5, #239
	movs	r7, #238
	lsls	r5, r5, #7
	lsls	r7, r7, #7
	adds	r2, r4, r5
	movs	r3, #2
	adds	r7, #132
	str	r3, [r2, #0]
	adds	r2, r4, r7
	movs	r3, #75
.L_0813c2e4:
	str	r3, [r2, #0]
.L_0813c2e6:
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #684]
	bl	sub_080145a8
	movs	r0, #0
	str	r0, [sp, #24]
	mov	r2, sl
	ldr	r1, [r2, #24]
	ldr	r5, [pc, #672]
	adds	r2, r1, #0
	lsls	r3, r2, #2
	adds	r3, #3
	ldrb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L_0813c308
	b.n	.L_0813c564
.L_0813c308:
	lsls	r3, r2, #2
	adds	r3, #3
	ldrb	r3, [r5, r3]
	ldr	r4, [sp, #24]
	subs	r3, #64
	cmp	r4, r3
	bne.n	.L_0813c320
	movs	r0, #133
	mov	r7, sl
	bl	sub_081180e8
	ldr	r1, [r7, #24]
.L_0813c320:
	lsls	r3, r1, #2
	ldrb	r3, [r5, r3]
	movs	r0, #0
	mov	r8, r0
	cmp	r3, #0
	bne.n	.L_0813c32e
	b.n	.L_0813c4d0
.L_0813c32e:
	ldr	r6, [sp, #32]
.L_0813c330:
	ldr	r2, [r6, #0]
	cmp	r2, #0
	bge.n	.L_0813c338
	adds	r2, #7
.L_0813c338:
	ldr	r3, [r6, #4]
	asrs	r4, r2, #3
	cmp	r3, #0
	bge.n	.L_0813c342
	adds	r3, #7
.L_0813c342:
	asrs	r5, r3, #3
	ldr	r3, [r6, #24]
	movs	r1, #1
	negs	r1, r1
	adds	r2, r3, #0
	cmp	r3, r1
	beq.n	.L_0813c352
	b.n	.L_0813c470
.L_0813c352:
	movs	r3, #24
	ldr	r2, [sp, #32]
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #4
	adds	r1, r2, r3
	str	r4, [sp, #8]
	adds	r2, r4, #0
	adds	r3, r5, #0
	ldr	r0, [sp, #28]
	ldr	r7, [sp, #16]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x20c0
	ldr	r3, [r6, #4]
	lsls	r0, r0, #1
	adds	r0, #255
	ldr	r4, [sp, #8]
	cmp	r3, r0
	bgt.n	.L_0813c39a
	mov	r1, sl
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_0813c38c
	ldr	r3, [r6, #0]
	subs	r3, #32
	b.n	.L_0813c390
.L_0813c38c:
	ldr	r3, [r6, #0]
	adds	r3, #32
.L_0813c390:
	str	r3, [r6, #0]
	ldr	r3, [r6, #4]
	adds	r3, #64
	str	r3, [r6, #4]
	b.n	.L_0813c4ba
.L_0813c39a:
	movs	r3, #0
	str	r3, [r6, #24]
	mov	r3, sl
	ldr	r2, [r3, #24]
	ldr	r1, [pc, #504]
	lsls	r3, r2, #2
	adds	r3, #1
	ldrb	r3, [r1, r3]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_0813c434
	adds	r4, #12
	str	r4, [sp, #12]
	lsls	r5, r5, #16
	movs	r4, #255
	mov	fp, r5
	mov	r9, r4
.L_0813c3bc:
	lsls	r3, r2, #2
	adds	r3, #1
	ldrb	r3, [r1, r3]
	ldr	r0, [pc, #476]
	mov	r2, r8
	muls	r2, r3
	adds	r2, r2, r7
	ldr	r1, [sp, #12]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r5, r3, r0
	mov	r2, fp
	lsls	r3, r1, #16
	str	r3, [r5, #0]
	str	r2, [r5, #4]
	bl	sub_08014878
	mov	r3, r9
	ands	r0, r3
	subs	r0, #128
	lsls	r0, r0, #9
	str	r0, [r5, #12]
	mov	r4, sl
	ldr	r3, [r4, #24]
	cmp	r3, #2
	bne.n	.L_0813c408
	bl	sub_08014878
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r3, r0
	ldr	r0, [pc, #420]
	adds	r3, r3, r0
	lsls	r3, r3, #10
	str	r3, [r5, #16]
	b.n	.L_0813c416
.L_0813c408:
	bl	sub_08014878
	mov	r1, r9
	ands	r0, r1
	subs	r0, #255
	lsls	r0, r0, #10
	str	r0, [r5, #16]
.L_0813c416:
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #16
	str	r3, [r5, #24]
	mov	r3, sl
	ldr	r2, [r3, #24]
	ldr	r1, [pc, #372]
	lsls	r3, r2, #2
	adds	r3, #1
	ldrb	r3, [r1, r3]
	adds	r7, #1
	cmp	r7, r3
	bne.n	.L_0813c3bc
.L_0813c434:
	movs	r3, #3
	mov	r4, r8
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L_0813c444
	movs	r0, #132
	bl	sub_081c0010
.L_0813c444:
	mov	r5, sl
	ldr	r3, [r5, #20]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_0813c4ba
	movs	r5, #36
.L_0813c450:
	mov	r1, sl
	movs	r3, #2
	ldrsh	r0, [r5, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	adds	r3, r7, #0
	movs	r2, #5
	bl	sub_0814cd48
	mov	r4, sl
	ldr	r3, [r4, #20]
	adds	r7, #1
	adds	r5, #2
	cmp	r7, r3
	bne.n	.L_0813c450
	b.n	.L_0813c4ba
.L_0813c470:
	cmp	r2, #3
	bhi.n	.L_0813c48c
	ldr	r7, [sp, #32]
	movs	r2, #240
	lsls	r2, r2, #4
	movs	r3, #24
	adds	r2, #68
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	adds	r1, r7, r2
	adds	r3, r5, #0
	adds	r2, r4, #0
	ldr	r0, [sp, #28]
	b.n	.L_0813c4aa
.L_0813c48c:
	cmp	r2, #7
	bgt.n	.L_0813c4b2
	adds	r3, r5, #0
	movs	r7, #136
	ldr	r5, [sp, #32]
	lsls	r7, r7, #5
	movs	r1, #42
	adds	r2, r4, #0
	adds	r7, #132
	subs	r3, #9
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	subs	r2, #9
	ldr	r0, [sp, #28]
	adds	r1, r5, r7
.L_0813c4aa:
	ldr	r4, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69b3
.L_0813c4b2:
	cmp	r3, #14
	bgt.n	.L_0813c4ba
	adds	r3, #1
	str	r3, [r6, #24]
.L_0813c4ba:
	mov	r7, sl
	ldr	r3, [r7, #24]
	ldr	r0, [pc, #220]
	lsls	r3, r3, #2
	ldrb	r3, [r0, r3]
	movs	r5, #1
	add	r8, r5
	adds	r6, #28
	cmp	r8, r3
	beq.n	.L_0813c4d0
	b.n	.L_0813c330
.L_0813c4d0:
	ldr	r6, [pc, #212]
	ldr	r5, [pc, #204]
	movs	r1, #0
	mov	r8, r1
.L_0813c4d8:
	ldr	r0, [r5, #24]
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L_0813c524
	adds	r4, r0, #1
	cmp	r4, #6
	ble.n	.L_0813c4ea
	movs	r4, #6
.L_0813c4ea:
	lsls	r0, r4, #1
	subs	r3, r0, #2
	ldrh	r1, [r6, r3]
	ldr	r3, [sp, #32]
	movs	r7, #224
	adds	r1, r3, r1
	lsls	r7, r7, #3
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	adds	r1, r1, r7
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	subs	r2, r2, r4
	subs	r3, r3, r4
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #20]
	ldr	r0, [sp, #28]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r5, #0
	movs	r1, #60
	lsls	r2, r2, #6
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_0813c524:
	movs	r7, #1
	movs	r0, #128
	add	r8, r7
	lsls	r0, r0, #2
	adds	r5, #28
	cmp	r8, r0
	bne.n	.L_0813c4d8
	bl	sub_081434f8
	movs	r3, #240
	ldr	r1, [sp, #32]
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r4, [sp, #24]
	mov	r7, sl
	adds	r4, #1
	str	r4, [sp, #24]
	ldr	r2, [r7, #24]
	ldr	r5, [pc, #68]
	lsls	r3, r2, #2
	adds	r3, #3
	ldrb	r3, [r5, r3]
	adds	r1, r2, #0
	cmp	r4, r3
	beq.n	.L_0813c564
	b.n	.L_0813c308
.L_0813c564:
	ldr	r0, [pc, #44]
	bl	sub_08014644
	ldr	r0, [pc, #44]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	bl	sub_0813ba50
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0813bba1
	.4byte 0x08143001
	.4byte 0x08197527
	.4byte 0x02010140
	.4byte 0xfffffe80
	.2byte 0x7424
	.2byte 0x0819
	push	{lr}
	movs	r1, #0
	bl	sub_0813c5f4
	pop	{pc}
	.align 2, 0
