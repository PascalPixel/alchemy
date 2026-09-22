.syntax unified
	.thumb
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_08004bd4, 0x08004bd4
	.set sub_08004c1c, 0x08004c1c
	.set sub_08004cb4, 0x08004cb4
	.set sub_080051d8, 0x080051d8
	.set sub_08005258, 0x08005258
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080108e4, 0x080108e4
	.set sub_080110e0, 0x080110e0
	.set sub_08011164, 0x08011164
	.set sub_080123f4, 0x080123f4
	.global Overlay_080111b4
Overlay_080111b4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #504]
	ldr	r1, [r3, #0]
	sub	sp, #36
	str	r1, [sp, #20]
	adds	r2, r3, #0
	subs	r2, #20
	ldr	r2, [r2, #0]
	str	r2, [sp, #16]
	subs	r3, #16
	ldr	r6, [r3, #0]
	adds	r1, #12
	ldr	r2, [r6, #0]
	ldr	r3, [sp, #16]
	mov	sl, r1
	movs	r1, #200
	lsls	r1, r1, #4
	str	r2, [sp, #12]
	adds	r1, r3, r1
	movs	r2, #210
	str	r1, [sp, #8]
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	movs	r1, #211
	str	r3, [sp, #4]
	lsls	r1, r1, #2
	adds	r3, r6, r1
	ldr	r3, [r3, #0]
	str	r3, [sp, #0]
	bl	.L_080114a0
	ldr	r2, [sp, #12]
	cmp	r2, #0
	bne.n	.L_08011208
	b.n	.L_080112e0
.L_08011208:
	ldr	r3, [r2, #8]
	mov	r8, r3
	ldr	r3, [r6, #4]
	ldr	r7, [r2, #0]
	cmp	r3, #0
	beq.n	.L_0801123a
	bl	sub_08004458
	adds	r5, r0, #0
	bl	sub_08004458
	ldr	r4, [r6, #4]
	adds	r1, r0, #0
	ldr	r3, [pc, #412]
	adds	r0, r4, #0
	subs	r1, r5, r1
	mov	ip, pc
	bx	r3
	adds	r7, r7, r0
	ldr	r1, [r6, #12]
	adds	r0, r4, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	str	r0, [r6, #4]
.L_0801123a:
	ldr	r3, [r6, #8]
	cmp	r3, #0
	beq.n	.L_08011266
	bl	sub_08004458
	adds	r5, r0, #0
	bl	sub_08004458
	ldr	r4, [r6, #8]
	adds	r1, r0, #0
	ldr	r3, [pc, #368]
	adds	r0, r4, #0
	subs	r1, r5, r1
	mov	ip, pc
	bx	r3
	add	r8, r0
	ldr	r1, [r6, #12]
	adds	r0, r4, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	str	r0, [r6, #8]
.L_08011266:
	adds	r0, r7, #0
	cmp	r7, #0
	bge.n	.L_08011270
	ldr	r1, [pc, #340]
	adds	r0, r7, r1
.L_08011270:
	asrs	r4, r0, #20
	mov	r0, r8
	cmp	r0, #0
	bge.n	.L_0801127c
	ldr	r0, [pc, #328]
	add	r0, r8
.L_0801127c:
	movs	r2, #228
	adds	r2, r2, r6
	ldr	r1, [r2, #0]
	mov	fp, r2
	adds	r3, r1, #0
	movs	r2, #128
	eors	r3, r7
	lsls	r2, r2, #13
	asrs	r0, r0, #20
	ands	r3, r2
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_080112ac
	cmp	r1, r7
	bge.n	.L_080112a4
	adds	r0, r4, #0
	adds	r0, #16
	bl	sub_08011164
	b.n	.L_080112ac
.L_080112a4:
	adds	r0, r4, #0
	subs	r0, #16
	bl	sub_08011164
.L_080112ac:
	adds	r5, r6, #0
	adds	r5, #232
	ldr	r1, [r5, #0]
	mov	r2, r8
	adds	r3, r1, #0
	eors	r3, r2
	movs	r2, #128
	lsls	r2, r2, #13
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080112d8
	cmp	r1, r8
	bge.n	.L_080112d0
	mov	r0, r9
	adds	r0, #12
	bl	sub_080110e0
	b.n	.L_080112d8
.L_080112d0:
	mov	r0, r9
	subs	r0, #18
	bl	sub_080110e0
.L_080112d8:
	mov	r3, fp
	mov	r1, r8
	str	r7, [r3, #0]
	str	r1, [r5, #0]
.L_080112e0:
	ldr	r2, [pc, #228]
	movs	r3, #120
	str	r3, [r2, #12]
	movs	r3, #96
	str	r3, [r2, #16]
	ldr	r2, [sp, #0]
	lsrs	r1, r2, #31
	adds	r1, r2, r1
	ldr	r0, [sp, #4]
	asrs	r1, r1, #1
	lsls	r2, r2, #1
	bl	sub_08005258
	ldr	r2, [sp, #12]
	ldmia	r2!, {r3}
	adds	r1, r2, #0
	str	r1, [sp, #12]
	movs	r7, #0
	mov	r1, sl
	str	r3, [r1, #0]
	str	r7, [r1, #4]
	ldr	r3, [r2, #4]
	str	r3, [r1, #8]
	bl	sub_080049ac
	mov	r0, sl
	bl	sub_08004cb4
	movs	r2, #141
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldrh	r0, [r3, #0]
	bl	sub_08004c1c
	movs	r3, #140
	lsls	r3, r3, #1
	adds	r6, r6, r3
	ldrh	r0, [r6, #0]
	bl	sub_08004bd4
	add	r0, sp, #24
	str	r7, [r0, #0]
	str	r7, [r0, #4]
	ldr	r1, [sp, #0]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r1, r2
	str	r3, [r0, #8]
	ldr	r1, [sp, #20]
	ldr	r3, [pc, #136]
	bl	sub_080072f0
	bl	sub_080049ac
	mov	r1, sl
	ldr	r0, [sp, #20]
	bl	sub_080051d8
	ldr	r3, [pc, #120]
	ldrh	r0, [r6, #0]
	mov	r8, r3
	ldr	r3, [r3, #0]
	cmp	r3, r0
	beq.n	.L_08011388
	bl	sub_0800231c
	adds	r5, r0, #0
	ldrh	r0, [r6, #0]
	bl	sub_08002322
	ldr	r3, [pc, #100]
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080072f0
	mov	r1, sl
	ldr	r2, [sp, #16]
	bl	sub_080123f4
	ldr	r3, [pc, #88]
	str	r7, [r3, #0]
	ldrh	r3, [r6, #0]
	mov	r1, r8
	str	r3, [r1, #0]
.L_08011388:
	ldr	r3, [pc, #80]
	ldr	r2, [r3, #0]
	movs	r3, #1
	ands	r2, r3
	ldr	r1, [pc, #76]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [sp, #8]
	adds	r1, #184
	lsls	r3, r3, #10
	adds	r3, r2, r3
	ldr	r4, [r1, #0]
	ldr	r2, [sp, #16]
	ldr	r0, [sp, #20]
	mov	r1, sl
	bl	sub_080072f4
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e80
	.4byte 0x03000118
	.4byte 0x000fffff
	.4byte 0x03001ce0
	.4byte 0x03000250
	.4byte 0x03001af4
	.4byte 0x0300013c
	.4byte 0x03001f60
	.4byte 0x03001e40
	.2byte 0x1e50
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #164]
	ldr	r3, [r3, #0]
	sub	sp, #16
	movs	r1, #0
	str	r3, [sp, #12]
	str	r1, [sp, #8]
	str	r1, [sp, #4]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0801140e
	ldmia	r3!, {r2}
	str	r2, [sp, #8]
	ldr	r3, [r3, #4]
	str	r3, [sp, #4]
.L_0801140e:
	ldr	r1, [sp, #8]
	movs	r2, #255
	lsls	r2, r2, #24
	adds	r3, r1, r2
	ldr	r1, [sp, #4]
	ldr	r2, [pc, #128]
	asrs	r3, r3, #25
	str	r3, [sp, #8]
	adds	r3, r1, r2
	asrs	r3, r3, #25
	str	r3, [sp, #4]
	movs	r3, #0
	mov	r9, r3
	mov	fp, r3
.L_0801142a:
	movs	r1, #0
	ldr	r6, [sp, #4]
	mov	sl, r1
.L_08011430:
	adds	r3, r6, #0
	movs	r2, #15
	ands	r3, r2
	movs	r5, #0
	mov	r8, r6
	lsls	r7, r3, #4
.L_0801143c:
	ldr	r3, [sp, #8]
	adds	r1, r3, r5
	adds	r3, r1, #0
	movs	r2, #15
	ands	r3, r2
	adds	r4, r7, r3
	movs	r2, #156
	lsls	r3, r4, #1
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #12]
	ldrh	r4, [r2, r3]
	movs	r3, #1
	add	r4, fp
	str	r3, [sp, #0]
	mov	r0, r9
	mov	r2, r8
	adds	r3, r4, #0
	adds	r5, #1
	bl	sub_080108e4
	cmp	r5, #1
	bls.n	.L_0801143c
	movs	r3, #1
	add	sl, r3
	mov	r1, sl
	adds	r6, #1
	cmp	r1, #1
	bls.n	.L_08011430
	movs	r2, #160
	add	r9, r3
	lsls	r2, r2, #1
	mov	r3, r9
	add	fp, r2
	cmp	r3, #1
	bls.n	.L_0801142a
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e70
	.2byte 0x0000
	.2byte 0xfec0
.L_080114a0:
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #176]
	ldr	r3, [r3, #0]
	sub	sp, #20
	movs	r1, #0
	str	r3, [sp, #16]
	str	r1, [sp, #12]
	str	r1, [sp, #8]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080114ca
	ldmia	r3!, {r2}
	str	r2, [sp, #12]
	ldr	r3, [r3, #4]
	str	r3, [sp, #8]
.L_080114ca:
	ldr	r1, [sp, #12]
	movs	r2, #255
	lsls	r2, r2, #24
	adds	r3, r1, r2
	ldr	r1, [sp, #8]
	ldr	r2, [pc, #140]
	asrs	r3, r3, #25
	str	r3, [sp, #12]
	adds	r3, r1, r2
	asrs	r3, r3, #25
	str	r3, [sp, #8]
	movs	r3, #0
	str	r3, [sp, #4]
	mov	r9, r3
.L_080114e6:
	ldr	r2, [sp, #4]
	movs	r1, #0
	ldr	r6, [sp, #8]
	mov	sl, r1
	mov	fp, r2
.L_080114f0:
	adds	r3, r6, #0
	movs	r1, #15
	ands	r3, r1
	movs	r5, #0
	mov	r8, r6
	lsls	r7, r3, #4
.L_080114fc:
	ldr	r2, [sp, #12]
	adds	r1, r2, r5
	adds	r3, r1, #0
	movs	r2, #15
	ands	r3, r2
	adds	r4, r7, r3
	movs	r2, #156
	lsls	r3, r4, #1
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #16]
	ldrh	r4, [r2, r3]
	movs	r3, #0
	add	r4, fp
	str	r3, [sp, #0]
	mov	r0, r9
	mov	r2, r8
	adds	r3, r4, #0
	bl	sub_080108e4
	cmp	r0, #0
	bne.n	.L_0801154e
	adds	r5, #1
	cmp	r5, #1
	bls.n	.L_080114fc
	movs	r3, #1
	add	sl, r3
	mov	r1, sl
	adds	r6, #1
	cmp	r1, #1
	bls.n	.L_080114f0
	ldr	r2, [sp, #4]
	movs	r3, #160
	lsls	r3, r3, #1
	movs	r1, #1
	adds	r2, r2, r3
	add	r9, r1
	str	r2, [sp, #4]
	mov	r2, r9
	cmp	r2, #1
	bls.n	.L_080114e6
.L_0801154e:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e70
	.4byte 0xfec00000
