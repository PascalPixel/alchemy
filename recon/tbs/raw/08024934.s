.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_0800352c, 0x0800352c
	.set sub_08003dec, 0x08003dec
	.set sub_08003f3c, 0x08003f3c
	.set sub_080040b4, 0x080040b4
	.set sub_080040d0, 0x080040d0
	.set sub_080162d4, 0x080162d4
	.set sub_08016418, 0x08016418
	.set sub_08016498, 0x08016498
	.set sub_08016738, 0x08016738
	.set sub_08017aa4, 0x08017aa4
	.set sub_08018efc, 0x08018efc
	.set sub_08019000, 0x08019000
	.set sub_0801965c, 0x0801965c
	.set sub_0801a3d0, 0x0801a3d0
	.set sub_0801e71c, 0x0801e71c
	.set sub_0801e7c0, 0x0801e7c0
	.set sub_08022768, 0x08022768
	.set sub_080771d8, 0x080771d8
	.set sub_080771e0, 0x080771e0
	.set sub_080f9010, 0x080f9010
	.global Func_08024934
	.thumb_func
Func_08024934:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #372
	str	r2, [sp, #84]
	ldr	r5, [pc, #184]
	ldr	r0, [r5, #0]
	movs	r1, #1
	str	r0, [sp, #72]
	negs	r1, r1
	movs	r0, #128
	str	r1, [sp, #68]
	mov	r9, r1
	bl	sub_080040b4
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	movs	r3, #42
	str	r0, [sp, #64]
	str	r3, [sp, #0]
	movs	r1, #4
	movs	r2, #30
	movs	r3, #4
	movs	r0, #0
	bl	sub_080162d4
	movs	r6, #6
	str	r0, [sp, #60]
	movs	r1, #8
	movs	r2, #10
	movs	r3, #3
	movs	r0, #20
	str	r6, [sp, #0]
	bl	sub_080162d4
	movs	r2, #0
	str	r0, [sp, #56]
	str	r2, [sp, #52]
	adds	r5, #168
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #52]
	ldr	r1, [r3, #48]
	ldr	r3, [r3, #56]
	mov	fp, r0
	mov	sl, r1
	str	r3, [sp, #48]
	str	r6, [sp, #0]
	movs	r2, #17
	movs	r3, #9
	movs	r0, #13
	movs	r1, #11
	bl	sub_080162d4
	movs	r2, #156
	lsls	r2, r2, #1
	add	r2, sp
	ldr	r3, [pc, #84]
	movs	r7, #128
	str	r0, [sp, #76]
	str	r2, [sp, #28]
	movs	r4, #0
	mov	ip, r3
	adds	r5, r2, #0
	lsls	r7, r7, #23
	movs	r6, #0
.L_080249be:
	lsls	r0, r4, #1
	str	r7, [r5, #4]
	str	r6, [r5, #8]
	ldr	r1, [sp, #76]
	ldrh	r2, [r1, #12]
	ldr	r3, [pc, #48]
	lsls	r2, r2, #3
	ldrh	r1, [r5, #6]
	adds	r2, #8
	ands	r2, r3
	mov	r3, ip
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #6]
	ldr	r2, [sp, #76]
	ldrh	r3, [r2, #14]
	adds	r0, r0, r3
	lsls	r0, r0, #3
	adds	r0, #4
	adds	r4, #1
	strb	r0, [r5, #4]
	adds	r5, #12
	cmp	r4, #3
	ble.n	.L_080249be
	ldr	r3, [pc, #24]
	ldr	r7, [sp, #28]
	movs	r5, #8
	add	r6, sp, #96
	mov	r8, r3
	movs	r4, #3
	b.n	.L_08024a0c
	.4byte 0x000001ff
	.4byte 0x03001e8c
	.4byte 0xfffffe00
	.2byte 0xfc00
	.2byte 0xffff
.L_08024a0c:
	movs	r0, #128
	str	r4, [sp, #4]
	bl	sub_080040b4
	movs	r1, #1
	negs	r1, r1
	stmia	r6!, {r0}
	bl	sub_080040d0
	ldr	r3, [pc, #24]
	ands	r0, r3
	ldrh	r3, [r5, r7]
	mov	r1, r8
	ldr	r4, [sp, #4]
	ands	r3, r1
	orrs	r3, r0
	subs	r4, #1
	strh	r3, [r5, r7]
	adds	r5, #12
	cmp	r4, #0
	bge.n	.L_08024a0c
	b.n	.L_08024a3c
	.2byte 0x03ff
	.2byte 0x0000
.L_08024a3c:
	movs	r2, #138
	lsls	r2, r2, #1
	add	r2, sp
	mov	r8, r2
	mov	r0, r8
	bl	sub_080771d8
	str	r0, [sp, #80]
	movs	r7, #0
	adds	r3, r0, #0
	subs	r3, #1
	str	r3, [sp, #20]
	cmp	r3, #0
	blt.n	.L_08024aa0
	mov	r0, sp
	adds	r0, #240
	adds	r5, r3, #0
	str	r0, [sp, #32]
	add	r5, r8
.L_08024a62:
	ldrb	r6, [r5, #0]
	adds	r0, r6, #0
	bl	sub_080771e0
	ldr	r1, [sp, #84]
	adds	r0, #4
	ldrb	r2, [r0, #0]
	ldrb	r3, [r1, #0]
	movs	r4, #0
	cmp	r2, r3
	bhi.n	.L_08024a8a
.L_08024a78:
	adds	r4, #1
	cmp	r4, #3
	bgt.n	.L_08024a8a
	adds	r0, #1
	adds	r1, #1
	ldrb	r2, [r0, #0]
	ldrb	r3, [r1, #0]
	cmp	r2, r3
	bls.n	.L_08024a78
.L_08024a8a:
	cmp	r4, #4
	bne.n	.L_08024a98
	ldr	r2, [sp, #32]
	movs	r3, #32
	strb	r6, [r2, r7]
	strb	r3, [r5, #0]
	adds	r7, #1
.L_08024a98:
	subs	r5, #1
	cmp	r5, r8
	bge.n	.L_08024a62
	b.n	.L_08024aa6
.L_08024aa0:
	mov	r3, sp
	adds	r3, #240
	str	r3, [sp, #32]
.L_08024aa6:
	ldr	r0, [sp, #80]
	cmp	r0, #0
	ble.n	.L_08024ac8
	ldr	r2, [sp, #32]
	adds	r1, r7, r2
	ldr	r2, [sp, #80]
	mov	r0, r8
.L_08024ab4:
	ldrb	r3, [r0, #0]
	adds	r0, #1
	cmp	r3, #32
	beq.n	.L_08024ac2
	strb	r3, [r1, #0]
	adds	r7, #1
	adds	r1, #1
.L_08024ac2:
	subs	r2, #1
	cmp	r2, #0
	bne.n	.L_08024ab4
.L_08024ac8:
	ldr	r0, [sp, #32]
	movs	r3, #32
	strb	r3, [r0, r7]
	movs	r1, #180
	ldr	r3, [sp, #64]
	lsls	r1, r1, #1
	mov	r2, sl
	add	r1, sp
	lsls	r2, r2, #1
	lsls	r3, r3, #16
	str	r1, [sp, #24]
	str	r2, [sp, #16]
	str	r3, [sp, #12]
.L_08024ae2:
	cmp	fp, r9
	bne.n	.L_08024aee
	ldr	r0, [sp, #68]
	cmp	sl, r0
	bne.n	.L_08024aee
	b.n	.L_08024d90
.L_08024aee:
	ldr	r1, [sp, #72]
	ldr	r2, [pc, #316]
	movs	r0, #1
	adds	r3, r1, r2
	strb	r0, [r3, #0]
	ldr	r1, [sp, #76]
	ldr	r2, [sp, #68]
	ldrh	r0, [r1, #12]
	ldrh	r1, [r1, #14]
	lsls	r3, r2, #1
	adds	r1, r1, r3
	ldr	r3, [sp, #76]
	ldrh	r2, [r3, #8]
	movs	r3, #15
	str	r3, [sp, #0]
	subs	r2, #2
	adds	r1, #1
	movs	r3, #1
	adds	r0, #1
	bl	sub_08022768
	bl	sub_08016738
	mov	r3, fp
	ldr	r1, [sp, #32]
	add	r3, sl
	ldrb	r0, [r1, r3]
	bl	sub_080771e0
	adds	r6, r0, #0
	ldrh	r0, [r6, #0]
	ldr	r3, [pc, #260]
	add	r5, sp, #112
	adds	r0, r0, r3
	adds	r1, r5, #0
	movs	r2, #52
	bl	sub_0801965c
	movs	r2, #0
	ldr	r1, [sp, #60]
	movs	r3, #4
	adds	r0, r5, #0
	bl	sub_08017aa4
	movs	r3, #0
	mov	r2, sl
	str	r3, [sp, #52]
	str	r2, [sp, #68]
	movs	r1, #1
	movs	r2, #0
	adds	r6, #4
.L_08024b54:
	ldrb	r3, [r6, #0]
	adds	r6, #1
	cmp	r3, #0
	beq.n	.L_08024b66
	ldr	r0, [sp, #52]
	adds	r3, r1, #0
	lsls	r3, r2
	orrs	r0, r3
	str	r0, [sp, #52]
.L_08024b66:
	adds	r2, #1
	cmp	r2, #3
	ble.n	.L_08024b54
	cmp	fp, r9
	bne.n	.L_08024b72
	b.n	.L_08024d06
.L_08024b72:
	ldr	r0, [sp, #76]
	bl	sub_08016498
	movs	r5, #0
	movs	r7, #0
	movs	r6, #1
.L_08024b7e:
	ldr	r2, [pc, #184]
	ldr	r0, [sp, #56]
	adds	r1, r5, r2
	movs	r3, #0
	lsls	r2, r5, #1
	str	r7, [sp, #0]
	bl	sub_08019000
	ldr	r3, [sp, #84]
	ldrb	r1, [r3, r5]
	adds	r2, r6, #0
	adds	r1, #48
	ldr	r0, [sp, #56]
	movs	r3, #0
	adds	r5, #1
	str	r7, [sp, #0]
	adds	r6, #2
	bl	sub_08018efc
	cmp	r5, #3
	ble.n	.L_08024b7e
	ldr	r0, [sp, #32]
	mov	r1, fp
	ldrb	r6, [r0, r1]
	movs	r4, #0
	cmp	r6, #32
	bne.n	.L_08024bb6
	b.n	.L_08024ce8
.L_08024bb6:
	mov	r2, sp
	adds	r2, #88
	str	r2, [sp, #8]
.L_08024bbc:
	adds	r0, r6, #0
	str	r4, [sp, #4]
	bl	sub_080771e0
	str	r0, [sp, #36]
	adds	r1, r0, #0
	ldr	r0, [sp, #84]
	adds	r1, #4
	ldrb	r2, [r1, #0]
	ldrb	r3, [r0, #0]
	movs	r7, #0
	ldr	r4, [sp, #4]
	cmp	r2, r3
	bhi.n	.L_08024bea
.L_08024bd8:
	adds	r7, #1
	cmp	r7, #3
	bgt.n	.L_08024bea
	adds	r1, #1
	adds	r0, #1
	ldrb	r2, [r1, #0]
	ldrb	r3, [r0, #0]
	cmp	r2, r3
	bls.n	.L_08024bd8
.L_08024bea:
	movs	r3, #4
	eors	r3, r7
	ldr	r2, [sp, #36]
	negs	r5, r3
	orrs	r5, r3
	ldr	r0, [pc, #48]
	ldrh	r3, [r2, #0]
	movs	r1, #1
	ands	r0, r3
	add	r2, sp, #96
	lsls	r3, r4, #2
	lsrs	r5, r5, #31
	adds	r2, r2, r3
	str	r1, [sp, #0]
	ldr	r3, [sp, #8]
	subs	r5, r1, r5
	movs	r1, #0
	str	r4, [sp, #4]
	bl	sub_0801a3d0
	ldr	r4, [sp, #4]
	lsls	r3, r4, #1
	adds	r1, r3, r4
	ldr	r2, [sp, #28]
	mov	r8, r3
	lsls	r1, r1, #2
	ldr	r3, [pc, #12]
	ldr	r0, [sp, #88]
	adds	r1, #8
	ands	r0, r3
	b.n	.L_08024c3c
	.4byte 0x00003fff
	.4byte 0x000003ff
	.4byte 0x00000ea6
	.4byte 0x0000053a
	.2byte 0x5001
	.2byte 0x0000
.L_08024c3c:
	ldrh	r3, [r2, r1]
	ldr	r2, [pc, #56]
	ands	r3, r2
	orrs	r3, r0
	ldr	r0, [sp, #28]
	strh	r3, [r0, r1]
	cmp	r5, #0
	bne.n	.L_08024c54
	movs	r0, #2
	bl	sub_0801e71c
	ldr	r4, [sp, #4]
.L_08024c54:
	adds	r0, r6, #0
	str	r4, [sp, #4]
	bl	sub_080771e0
	ldr	r3, [pc, #28]
	ldr	r4, [sp, #4]
	ldrh	r0, [r0, #0]
	ldr	r1, [sp, #76]
	adds	r0, r0, r3
	movs	r2, #16
	lsls	r3, r4, #4
	bl	sub_0801e7c0
	movs	r1, #0
	ldr	r6, [sp, #36]
	lsls	r3, r1, #1
	b.n	.L_08024c80
	movs	r0, r0
	.4byte 0xfffffc00
	.2byte 0x0333
	.2byte 0x0000
.L_08024c80:
	adds	r5, r3, #0
	ldr	r4, [sp, #4]
	movs	r7, #0
	mov	r9, r1
	adds	r6, #4
	adds	r5, #13
.L_08024c8c:
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_08024cbc
	ldr	r2, [pc, #128]
	mov	r3, r9
	adds	r1, r7, r2
	str	r3, [sp, #0]
	ldr	r0, [sp, #76]
	adds	r2, r5, #0
	mov	r3, r8
	str	r4, [sp, #4]
	bl	sub_08019000
	ldrb	r1, [r6, #0]
	mov	r0, r9
	adds	r2, r5, #1
	str	r0, [sp, #0]
	adds	r1, #48
	ldr	r0, [sp, #76]
	mov	r3, r8
	bl	sub_08018efc
	ldr	r4, [sp, #4]
	adds	r5, #2
.L_08024cbc:
	adds	r7, #1
	adds	r6, #1
	cmp	r7, #3
	ble.n	.L_08024c8c
	movs	r0, #15
	str	r4, [sp, #4]
	bl	sub_0801e71c
	ldr	r4, [sp, #4]
	add	r3, sp, #92
	movs	r1, #1
	strb	r1, [r3, r4]
	adds	r4, #1
	cmp	r4, #3
	bgt.n	.L_08024d04
	mov	r2, fp
	ldr	r0, [sp, #32]
	adds	r3, r2, r4
	ldrb	r6, [r0, r3]
	cmp	r6, #32
	beq.n	.L_08024ce8
	b.n	.L_08024bbc
.L_08024ce8:
	cmp	r4, #3
	bgt.n	.L_08024d04
	add	r2, sp, #372
	ldr	r0, [pc, #40]
	adds	r3, r4, r2
	adds	r2, r3, r0
	movs	r3, #4
	movs	r1, #0
	subs	r4, r3, r4
.L_08024cfa:
	subs	r4, #1
	strb	r1, [r2, #0]
	adds	r2, #1
	cmp	r4, #0
	bne.n	.L_08024cfa
.L_08024d04:
	mov	r9, fp
.L_08024d06:
	ldr	r1, [sp, #80]
	cmp	r1, #4
	ble.n	.L_08024d60
	movs	r4, #0
	adds	r5, r1, #0
	adds	r5, #3
	b.n	.L_08024d50
	.4byte 0x00005001
	.2byte 0xfee8
	.2byte 0xffff
.L_08024d1c:
	ldr	r2, [pc, #256]
	mov	r3, fp
	adds	r1, r4, r2
	cmp	r3, #0
	bge.n	.L_08024d28
	adds	r3, #3
.L_08024d28:
	asrs	r3, r3, #2
	cmp	r4, r3
	bne.n	.L_08024d32
	ldr	r3, [pc, #244]
	adds	r1, r4, r3
.L_08024d32:
	ldr	r3, [sp, #76]
	ldrh	r2, [r3, #8]
	subs	r2, r2, r0
	movs	r0, #0
	adds	r2, r2, r4
	str	r0, [sp, #0]
	adds	r0, r3, #0
	movs	r3, #1
	subs	r2, #2
	negs	r3, r3
	str	r4, [sp, #4]
	bl	sub_08019000
	ldr	r4, [sp, #4]
	adds	r4, #1
.L_08024d50:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_08024d5a
	ldr	r3, [sp, #80]
	adds	r3, #6
.L_08024d5a:
	asrs	r0, r3, #2
	cmp	r4, r0
	blt.n	.L_08024d1c
.L_08024d60:
	ldr	r1, [sp, #76]
	ldr	r2, [sp, #16]
	ldrh	r0, [r1, #12]
	ldr	r3, [sp, #76]
	ldrh	r1, [r1, #14]
	adds	r1, r1, r2
	ldrh	r2, [r3, #8]
	movs	r3, #14
	adds	r0, #1
	adds	r1, #1
	subs	r2, #2
	str	r3, [sp, #0]
	movs	r3, #1
	bl	sub_08022768
	ldr	r1, [pc, #168]
	ldr	r0, [sp, #72]
	movs	r2, #1
	adds	r3, r0, r1
	adds	r1, #3
	strb	r2, [r3, #0]
	adds	r3, r0, r1
	movs	r2, #0
	strb	r2, [r3, #0]
.L_08024d90:
	ldr	r6, [sp, #28]
	movs	r4, #0
	add	r5, sp, #92
.L_08024d96:
	ldrb	r3, [r5, #0]
	adds	r5, #1
	cmp	r3, #0
	beq.n	.L_08024daa
	adds	r0, r6, #0
	movs	r1, #240
	str	r4, [sp, #4]
	bl	sub_08003dec
	ldr	r4, [sp, #4]
.L_08024daa:
	adds	r4, #1
	adds	r6, #12
	cmp	r4, #3
	ble.n	.L_08024d96
	ldr	r0, [sp, #76]
	ldrh	r3, [r0, #12]
	lsls	r3, r3, #3
	subs	r3, #2
	ldr	r1, [sp, #16]
	str	r3, [sp, #40]
	ldrh	r3, [r0, #14]
	adds	r3, r1, r3
	lsls	r3, r3, #3
	adds	r3, #20
	ldr	r2, [sp, #24]
	str	r3, [sp, #44]
	movs	r3, #128
	lsls	r3, r3, #23
	str	r3, [r2, #4]
	movs	r3, #0
	str	r3, [r2, #8]
	ldr	r1, [sp, #12]
	lsrs	r0, r1, #16
	ldr	r1, [pc, #80]
	bl	sub_080040d0
	ldr	r3, [pc, #48]
	ldr	r2, [sp, #24]
	ands	r0, r3
	ldrh	r3, [r2, #8]
	ldr	r2, [pc, #44]
	ldr	r1, [pc, #68]
	ands	r3, r2
	orrs	r3, r0
	ldr	r0, [sp, #24]
	ldr	r2, [r1, #0]
	strh	r3, [r0, #8]
	movs	r0, #4
	ldr	r3, [sp, #40]
	ands	r2, r0
	ldr	r1, [pc, #56]
	lsrs	r2, r2, #1
	adds	r2, r3, r2
	adds	r2, r2, r1
	ldr	r3, [pc, #20]
	ldr	r1, [sp, #24]
	ands	r2, r3
	ldrh	r3, [r1, #6]
	ldr	r1, [pc, #16]
	ands	r3, r1
	b.n	.L_08024e38
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x0000f301
	.4byte 0x0000f30b
	.4byte 0x00000ea3
	.4byte 0x080310a4
	.4byte 0x03001e40
	.2byte 0xfffc
	.2byte 0x0000
.L_08024e38:
	orrs	r3, r2
	ldr	r1, [pc, #800]
	ldr	r2, [sp, #24]
	strh	r3, [r2, #6]
	ldr	r3, [r1, #0]
	ldr	r2, [sp, #44]
	ands	r3, r0
	lsrs	r3, r3, #2
	ldr	r0, [sp, #24]
	subs	r3, r2, r3
	adds	r3, #248
	strb	r3, [r0, #4]
	movs	r1, #242
	ldr	r0, [sp, #24]
	bl	sub_08003dec
	ldr	r1, [pc, #768]
	ldr	r6, [r1, #0]
	movs	r3, #8
	ands	r6, r3
	movs	r5, #0
.L_08024e62:
	negs	r3, r6
	orrs	r3, r6
	lsrs	r3, r3, #31
	adds	r2, r3, #0
	movs	r3, #15
	subs	r2, r3, r2
	ldr	r0, [sp, #52]
	movs	r3, #1
	lsls	r3, r5
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_08024e7c
	movs	r2, #15
.L_08024e7c:
	ldr	r1, [sp, #56]
	ldrh	r0, [r1, #12]
	lsls	r3, r5, #1
	ldrh	r1, [r1, #14]
	adds	r0, r0, r3
	str	r2, [sp, #0]
	adds	r0, #1
	adds	r1, #1
	movs	r2, #2
	movs	r3, #1
	adds	r5, #1
	bl	sub_08022768
	cmp	r5, #3
	ble.n	.L_08024e62
	ldr	r2, [sp, #80]
	cmp	r2, #4
	ble.n	.L_08024f48
	movs	r4, #0
	adds	r5, r2, #0
	adds	r5, #3
	b.n	.L_08024ef2
.L_08024ea8:
	ldr	r3, [pc, #692]
	ldr	r0, [pc, #688]
	adds	r1, r4, r3
	ldr	r3, [r0, #0]
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #11
	bhi.n	.L_08024eca
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_08024ec0
	adds	r3, #3
.L_08024ec0:
	asrs	r3, r3, #2
	cmp	r4, r3
	bne.n	.L_08024eca
	ldr	r2, [pc, #668]
	adds	r1, r4, r2
.L_08024eca:
	ldr	r0, [sp, #76]
	adds	r2, r5, #0
	ldrh	r3, [r0, #8]
	cmp	r5, #0
	bge.n	.L_08024ed8
	ldr	r2, [sp, #80]
	adds	r2, #6
.L_08024ed8:
	asrs	r2, r2, #2
	subs	r2, r3, r2
	adds	r2, r2, r4
	movs	r3, #0
	str	r3, [sp, #0]
	subs	r2, #2
	ldr	r0, [sp, #76]
	subs	r3, #1
	str	r4, [sp, #4]
	bl	sub_08019000
	ldr	r4, [sp, #4]
	adds	r4, #1
.L_08024ef2:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_08024efc
	ldr	r3, [sp, #80]
	adds	r3, #6
.L_08024efc:
	asrs	r2, r3, #2
	cmp	r4, r2
	blt.n	.L_08024ea8
	ldr	r0, [sp, #76]
	ldrh	r3, [r0, #8]
	movs	r5, #1
	negs	r5, r5
	subs	r2, r3, r2
	movs	r1, #0
	str	r1, [sp, #0]
	ldr	r0, [sp, #76]
	adds	r3, r5, #0
	subs	r2, #3
	ldr	r1, [pc, #592]
	bl	sub_08019000
	ldr	r3, [sp, #76]
	ldrh	r2, [r3, #8]
	movs	r0, #0
	str	r0, [sp, #0]
	subs	r2, #2
	adds	r0, r3, #0
	ldr	r1, [pc, #576]
	adds	r3, r5, #0
	bl	sub_08019000
	ldr	r2, [sp, #72]
	ldr	r3, [pc, #572]
	ldr	r0, [sp, #76]
	adds	r1, r2, r3
	ldrh	r3, [r0, #14]
	subs	r3, #1
	lsrs	r3, r3, #2
	movs	r2, #2
	lsls	r2, r3
	ldrb	r3, [r1, #0]
	orrs	r2, r3
	strb	r2, [r1, #0]
.L_08024f48:
	ldr	r3, [pc, #552]
	ldr	r2, [r3, #0]
	mov	r1, fp
	mov	r3, sl
	str	r1, [r2, #52]
	str	r3, [r2, #48]
	ldr	r0, [sp, #48]
	str	r0, [r2, #56]
	ldr	r3, [pc, #540]
	ldr	r1, [r3, #0]
	ldr	r3, [pc, #540]
	ldr	r0, [r3, #0]
	adds	r3, r2, #0
	adds	r3, #216
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08024f84
	adds	r2, #220
	ldr	r3, [r2, #0]
	movs	r0, #0
	movs	r1, #0
	cmp	r3, #0
	bne.n	.L_08024f80
	movs	r3, #120
	str	r3, [r2, #0]
	movs	r0, #1
	movs	r1, #1
	b.n	.L_08024f84
.L_08024f80:
	subs	r3, #1
	str	r3, [r2, #0]
.L_08024f84:
	adds	r3, r1, #0
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08024f98
	mov	r3, fp
	ldr	r0, [sp, #32]
	add	r3, sl
	ldrb	r6, [r0, r3]
	b.n	.L_08025106
.L_08024f98:
	ldr	r3, [pc, #472]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #76]
	cmp	r3, #0
	beq.n	.L_08024faa
	movs	r3, #2
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08024fb6
.L_08024faa:
	movs	r0, #113
	movs	r6, #1
	bl	sub_080f9010
	negs	r6, r6
	b.n	.L_08025106
.L_08024fb6:
	movs	r3, #128
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_08024fe6
	movs	r0, #111
	bl	sub_080f9010
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #4
	beq.n	.L_08024fd8
	mov	r3, fp
	ldr	r0, [sp, #80]
	add	r3, sl
	cmp	r3, r0
	bne.n	.L_08024fdc
.L_08024fd8:
	movs	r1, #0
	mov	sl, r1
.L_08024fdc:
	mov	r3, sl
	mov	r2, sl
	lsls	r3, r3, #1
	str	r2, [sp, #48]
	b.n	.L_080250fc
.L_08024fe6:
	movs	r3, #64
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0802502c
	movs	r0, #111
	bl	sub_080f9010
	movs	r0, #1
	negs	r0, r0
	add	sl, r0
	mov	r1, sl
	cmp	r1, #0
	bge.n	.L_08025022
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bge.n	.L_0802500a
	ldr	r3, [sp, #80]
	adds	r3, #2
.L_0802500a:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	cmp	fp, r3
	bne.n	.L_0802501e
	ldr	r2, [sp, #80]
	mov	r0, fp
	subs	r3, r2, r0
	subs	r3, #1
	mov	sl, r3
	b.n	.L_08025022
.L_0802501e:
	movs	r1, #3
	mov	sl, r1
.L_08025022:
	mov	r3, sl
	mov	r2, sl
	lsls	r3, r3, #1
	str	r2, [sp, #48]
	b.n	.L_080250fc
.L_0802502c:
	movs	r3, #16
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0802508a
	movs	r0, #111
	bl	sub_080f9010
	bl	sub_0800352c
	mov	r3, fp
	ldr	r0, [sp, #80]
	adds	r3, #4
	cmp	r3, r0
	blt.n	.L_0802505e
	mov	r1, fp
	cmp	r1, #0
	beq.n	.L_080250fe
	ldr	r3, [sp, #48]
	mov	sl, r3
	mov	r0, sl
	movs	r2, #0
	lsls	r0, r0, #1
	mov	fp, r2
	str	r0, [sp, #16]
	b.n	.L_080250fe
.L_0802505e:
	mov	fp, r3
	ldr	r1, [sp, #48]
	ldr	r3, [sp, #20]
	mov	sl, r1
	cmp	r3, #0
	bge.n	.L_0802506e
	ldr	r3, [sp, #80]
	adds	r3, #2
.L_0802506e:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	cmp	fp, r3
	bne.n	.L_080250e0
	ldr	r2, [sp, #80]
	mov	r0, fp
	subs	r3, r2, r0
	subs	r3, #1
	ldr	r1, [sp, #48]
	mov	sl, r3
	cmp	sl, r1
	ble.n	.L_080250e8
	mov	sl, r1
	b.n	.L_080250f0
.L_0802508a:
	movs	r3, #32
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080250fe
	movs	r0, #111
	bl	sub_080f9010
	bl	sub_0800352c
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L_080250b4
	ldr	r1, [sp, #48]
	mov	sl, r1
	movs	r0, #4
	mov	r2, sl
	negs	r0, r0
	lsls	r2, r2, #1
	add	fp, r0
	str	r2, [sp, #16]
	b.n	.L_080250fe
.L_080250b4:
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bge.n	.L_080250be
	ldr	r3, [sp, #80]
	adds	r3, #2
.L_080250be:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	mov	fp, r3
	ldr	r3, [sp, #48]
	mov	r0, fp
	mov	sl, r3
	cmp	r0, #0
	beq.n	.L_080250f0
	ldr	r1, [sp, #80]
	subs	r3, r1, r0
	subs	r3, #1
	ldr	r2, [sp, #48]
	mov	sl, r3
	cmp	sl, r2
	ble.n	.L_080250f8
	mov	sl, r2
	b.n	.L_080250f8
.L_080250e0:
	mov	r0, sl
	lsls	r0, r0, #1
	str	r0, [sp, #16]
	b.n	.L_080250fe
.L_080250e8:
	mov	r1, sl
	lsls	r1, r1, #1
	str	r1, [sp, #16]
	b.n	.L_080250fe
.L_080250f0:
	mov	r2, sl
	lsls	r2, r2, #1
	str	r2, [sp, #16]
	b.n	.L_080250fe
.L_080250f8:
	mov	r3, sl
	lsls	r3, r3, #1
.L_080250fc:
	str	r3, [sp, #16]
.L_080250fe:
	movs	r0, #1
	bl	sub_080030f8
	b.n	.L_08024ae2
.L_08025106:
	movs	r0, #1
	bl	sub_080030f8
	movs	r4, #3
	add	r5, sp, #96
.L_08025110:
	ldmia	r5!, {r0}
	str	r4, [sp, #4]
	bl	sub_08003f3c
	ldr	r4, [sp, #4]
	subs	r4, #1
	cmp	r4, #0
	bge.n	.L_08025110
	ldr	r1, [sp, #12]
	lsrs	r0, r1, #16
	bl	sub_08003f3c
	movs	r1, #1
	ldr	r0, [sp, #56]
	bl	sub_08016418
	movs	r1, #1
	ldr	r0, [sp, #60]
	bl	sub_08016418
	movs	r1, #1
	ldr	r0, [sp, #76]
	bl	sub_08016418
	movs	r0, #1
	bl	sub_080030f8
	adds	r0, r6, #0
	add	sp, #372
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e40
	.4byte 0x0000f301
	.4byte 0x0000f30b
	.4byte 0x0000f334
	.4byte 0x0000f335
	.4byte 0x00000ea3
	.4byte 0x03001f34
	.4byte 0x03001c94
	.4byte 0x03001b04
