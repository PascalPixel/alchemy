.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08003dec, 0x08003dec
	.set sub_08003f3c, 0x08003f3c
	.set sub_08003fa4, 0x08003fa4
	.set sub_08004080, 0x08004080
	.set sub_0800479c, 0x0800479c
	.set sub_080162d4, 0x080162d4
	.set sub_08016418, 0x08016418
	.set sub_08016478, 0x08016478
	.set sub_080165d8, 0x080165d8
	.set sub_08019908, 0x08019908
	.set sub_08019ba0, 0x08019ba0
	.set sub_08019d2c, 0x08019d2c
	.set sub_08019da8, 0x08019da8
	.set sub_08019e48, 0x08019e48
	.set sub_0801a4fc, 0x0801a4fc
	.set sub_0801e41c, 0x0801e41c
	.set sub_0801e7c0, 0x0801e7c0
	.set sub_0801eadc, 0x0801eadc
	.set sub_080209d0, 0x080209d0
	.set sub_08020a60, 0x08020a60
	.set sub_08020b14, 0x08020b14
	.set sub_08020b64, 0x08020b64
	.set sub_08077008, 0x08077008
	.set sub_080b0020, 0x080b0020
	.set sub_080b0028, 0x080b0028
	.set sub_080b0030, 0x080b0030
	.set sub_080b0038, 0x080b0038
	.set sub_080f9010, 0x080f9010
	.set sub_080f9048, 0x080f9048
	.global Func_08020bd8
	.thumb_func
Func_08020bd8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #96
	mov	r2, sp
	movs	r1, #0
	adds	r2, #81
	str	r1, [sp, #36]
	str	r1, [sp, #32]
	str	r1, [sp, #28]
	add	r6, sp, #80
	str	r2, [sp, #24]
	str	r0, [sp, #44]
	bl	sub_08077008
	ldr	r3, [pc, #340]
	str	r0, [sp, #20]
	ldr	r3, [r3, #0]
	str	r3, [sp, #16]
	movs	r3, #1
	str	r3, [sp, #12]
	mov	r9, r3
	bl	sub_0800479c
	movs	r5, #2
	movs	r1, #6
	movs	r2, #24
	movs	r3, #9
	movs	r0, #3
	str	r5, [sp, #0]
	bl	sub_080162d4
	movs	r1, #3
	mov	r8, r0
	movs	r2, #8
	movs	r3, #3
	movs	r0, #8
	str	r5, [sp, #0]
	bl	sub_080162d4
	str	r0, [sp, #40]
	ldr	r0, [sp, #44]
	bl	sub_08019d2c
	movs	r2, #3
	movs	r3, #1
	movs	r1, #0
	bl	sub_08019da8
	ldr	r1, [pc, #276]
	mov	r0, r8
	bl	sub_080209d0
	movs	r3, #7
	str	r3, [sp, #0]
	mov	r0, r8
	movs	r1, #18
	movs	r2, #0
	movs	r3, #18
	bl	sub_0801e41c
	ldr	r2, [pc, #256]
	ldr	r1, [sp, #16]
	adds	r3, r1, r2
	add	r1, sp, #12
	add	r2, sp, #36
	ldrb	r1, [r1, #0]
	ldrb	r2, [r2, #0]
	strb	r1, [r3, #0]
	strb	r2, [r6, #0]
	mov	r0, sp
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #20]
	adds	r0, #94
.L_08020c74:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	strb	r3, [r1, #0]
	adds	r1, #1
	cmp	r3, #0
	beq.n	.L_08020c8c
	ldr	r3, [sp, #32]
	adds	r3, #1
	str	r3, [sp, #32]
	ldr	r3, [sp, #28]
	adds	r3, #1
	str	r3, [sp, #28]
.L_08020c8c:
	cmp	r1, r0
	ble.n	.L_08020c74
	ldr	r1, [sp, #24]
	movs	r3, #0
	strb	r3, [r1, #14]
	ldr	r0, [sp, #40]
	ldr	r1, [sp, #20]
	bl	sub_08020b64
	bl	sub_08004080
	adds	r5, r0, #0
	movs	r6, #18
	movs	r7, #5
	cmp	r5, #95
	bgt.n	.L_08020ce2
	ldr	r2, [pc, #176]
	movs	r1, #128
	bl	sub_08003fa4
	movs	r1, #128
	movs	r3, #0
	lsls	r1, r1, #23
	adds	r0, r5, #0
	mov	r2, r8
	str	r3, [sp, #0]
	bl	sub_0801eadc
	add	r2, sp, #64
	adds	r5, r0, #0
	str	r5, [r2, #0]
	mov	r3, r8
	mov	fp, r2
	ldrh	r1, [r3, #12]
	ldrh	r2, [r3, #14]
	lsls	r1, r1, #3
	lsls	r2, r2, #3
	adds	r1, #140
	adds	r2, #52
	mov	r0, fp
	bl	sub_080b0038
	b.n	.L_08020ce6
.L_08020ce2:
	add	r1, sp, #64
	mov	fp, r1
.L_08020ce6:
	bl	sub_08004080
	adds	r5, r0, #0
	cmp	r5, #95
	bgt.n	.L_08020d68
	ldr	r2, [pc, #112]
	movs	r1, #128
	bl	sub_08003fa4
	movs	r1, #128
	lsls	r1, r1, #23
	movs	r3, #0
	adds	r0, r5, #0
	mov	r2, r8
	str	r3, [sp, #0]
	bl	sub_0801eadc
	movs	r2, #48
	adds	r5, r0, #0
	add	r2, sp
	str	r5, [r2, #0]
	movs	r3, #255
	strb	r3, [r5, #15]
	mov	sl, r2
	movs	r3, #13
	ldrb	r2, [r5, #25]
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r5, #25]
	ldr	r0, [sp, #24]
	bl	sub_08020b14
	adds	r1, r0, #0
	adds	r1, #70
	mov	r0, sl
	movs	r2, #22
	bl	sub_080b0038
	b.n	.L_08020d6c
.L_08020d34:
	mov	r3, sl
	ldr	r2, [r3, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	ldr	r0, [sp, #40]
	bl	sub_08016478
	ldr	r0, [sp, #40]
	ldr	r1, [sp, #20]
	bl	sub_08020b64
	movs	r0, #10
	bl	sub_080030f8
	b.n	.L_08021034
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x08073864
	.4byte 0x00000ea3
	.4byte 0x080310a4
	.2byte 0x17e4
	.2byte 0x0803
.L_08020d68:
	add	r1, sp, #48
	mov	sl, r1
.L_08020d6c:
	ldr	r4, [pc, #48]
	ldr	r3, [pc, #52]
	ldr	r0, [pc, #52]
	adds	r1, r4, #0
	ldr	r2, [pc, #52]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #32]
	strh	r3, [r4, #8]
	ldr	r2, [sp, #32]
	ldr	r3, [sp, #24]
	adds	r2, r2, r3
	str	r2, [sp, #8]
.L_08020d86:
	movs	r5, #1
	cmp	r6, #18
	bne.n	.L_08020db0
	cmp	r7, #4
	bne.n	.L_08020d92
	movs	r5, #3
.L_08020d92:
	cmp	r7, #5
	bne.n	.L_08020db0
	movs	r5, #3
	b.n	.L_08020db0
	movs	r0, r0
	.4byte 0x00006318
	.4byte 0x050001c0
	.4byte 0x040000d4
	.4byte 0x050001e0
	.2byte 0x0008
	.2byte 0x8400
.L_08020db0:
	movs	r1, #1
	movs	r3, #14
	str	r1, [sp, #0]
	str	r3, [sp, #4]
	adds	r1, r6, #0
	adds	r2, r7, #0
	adds	r3, r5, #0
	mov	r0, r8
	bl	sub_08020a60
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #1
	movs	r3, #15
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	mov	r0, r8
	adds	r3, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	sub_08020a60
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L_08020e02
	movs	r1, #0
	mov	r2, r8
	mov	r9, r1
	ldrh	r1, [r2, #12]
	ldrh	r2, [r2, #14]
	adds	r1, r1, r6
	adds	r2, r2, r7
	lsls	r1, r1, #3
	lsls	r2, r2, #3
	subs	r1, #7
	adds	r2, #15
	mov	r0, fp
	movs	r3, #3
	bl	sub_080b0030
.L_08020e02:
	ldr	r3, [sp, #12]
	cmp	r3, #0
	beq.n	.L_08020e20
	movs	r1, #0
	ldr	r0, [sp, #24]
	str	r1, [sp, #12]
	bl	sub_08020b14
	adds	r1, r0, #0
	adds	r1, #70
	mov	r0, sl
	movs	r2, #22
	movs	r3, #3
	bl	sub_080b0030
.L_08020e20:
	mov	r0, fp
	bl	sub_080b0020
	mov	r0, sl
	bl	sub_080b0028
	ldr	r3, [pc, #68]
	ldr	r0, [r3, #0]
	mov	r3, sl
	ldr	r5, [r3, #0]
	movs	r2, #7
	ldr	r4, [pc, #64]
	lsrs	r0, r0, #1
	ands	r0, r2
	ldrsb	r3, [r4, r0]
	ldrh	r1, [r5, #6]
	adds	r1, r1, r3
	ldr	r3, [pc, #40]
	ldr	r2, [pc, #40]
	ands	r1, r3
	ldrh	r3, [r5, #22]
	ands	r3, r2
	orrs	r3, r1
	adds	r0, #5
	movs	r1, #7
	ands	r0, r1
	strh	r3, [r5, #22]
	ldrb	r2, [r5, #8]
	ldrb	r3, [r4, r0]
	adds	r2, r2, r3
	strb	r2, [r5, #20]
	ldr	r5, [pc, #28]
	ldr	r2, [r5, #0]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08020eaa
	b.n	.L_08020e80
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x03001800
	.4byte 0x080371f6
	.2byte 0x1b04
	.2byte 0x0300
.L_08020e80:
	movs	r0, #111
	bl	sub_080f9010
	movs	r2, #1
	mov	r9, r2
	subs	r7, #1
	cmp	r6, #18
	beq.n	.L_08020e9c
	movs	r3, #1
	negs	r3, r3
	cmp	r7, r3
	bne.n	.L_08020eaa
	movs	r7, #5
	b.n	.L_08020eaa
.L_08020e9c:
	movs	r3, #3
	eors	r3, r7
	negs	r2, r3
	orrs	r2, r3
	lsrs	r7, r2, #31
	movs	r3, #5
	subs	r7, r3, r7
.L_08020eaa:
	ldr	r2, [r5, #0]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08020ed8
	movs	r0, #111
	bl	sub_080f9010
	movs	r1, #1
	mov	r9, r1
	adds	r7, #1
	cmp	r6, #18
	beq.n	.L_08020ecc
	cmp	r7, #6
	bne.n	.L_08020ed8
	movs	r7, #0
	b.n	.L_08020ed8
.L_08020ecc:
	movs	r2, #6
	eors	r2, r7
	negs	r3, r2
	orrs	r3, r2
	lsrs	r7, r3, #31
	adds	r7, #4
.L_08020ed8:
	ldr	r2, [r5, #0]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08020f12
	movs	r0, #111
	bl	sub_080f9010
	movs	r3, #1
	movs	r2, #1
	subs	r6, #1
	negs	r3, r3
	mov	r9, r2
	cmp	r6, r3
	bne.n	.L_08020f02
	subs	r3, r7, #4
	movs	r6, #18
	cmp	r3, #1
	bls.n	.L_08020f12
	movs	r6, #16
	b.n	.L_08020f12
.L_08020f02:
	cmp	r6, #5
	beq.n	.L_08020f0e
	cmp	r6, #11
	beq.n	.L_08020f0e
	cmp	r6, #17
	bne.n	.L_08020f12
.L_08020f0e:
	ldr	r5, [pc, #340]
	subs	r6, #1
.L_08020f12:
	ldr	r2, [r5, #0]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08020f4a
	movs	r0, #111
	bl	sub_080f9010
	adds	r6, #1
	movs	r1, #1
	mov	r9, r1
	cmp	r6, #19
	bne.n	.L_08020f30
	movs	r6, #0
	b.n	.L_08020f3e
.L_08020f30:
	cmp	r6, #5
	beq.n	.L_08020f3c
	cmp	r6, #11
	beq.n	.L_08020f3c
	cmp	r6, #17
	bne.n	.L_08020f3e
.L_08020f3c:
	adds	r6, #1
.L_08020f3e:
	cmp	r6, #18
	bne.n	.L_08020f4a
	subs	r3, r7, #4
	cmp	r3, #1
	bls.n	.L_08020f4a
	movs	r6, #0
.L_08020f4a:
	ldr	r3, [pc, #284]
	ldr	r2, [r3, #0]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08020f64
	movs	r0, #111
	bl	sub_080f9010
	movs	r2, #1
	mov	r9, r2
	movs	r6, #18
	movs	r7, #5
.L_08020f64:
	ldr	r2, [pc, #252]
	ldr	r5, [r2, #0]
	movs	r3, #2
	ands	r5, r3
	cmp	r5, #0
	beq.n	.L_08020fa6
	movs	r0, #113
	bl	sub_080f9010
.L_08020f76:
	ldr	r3, [sp, #28]
	cmp	r3, #0
	beq.n	.L_08020f9e
	ldr	r1, [sp, #8]
	subs	r3, #1
	str	r3, [sp, #28]
	subs	r1, #1
	movs	r3, #0
	str	r1, [sp, #8]
	strb	r3, [r1, #0]
	ldr	r0, [sp, #40]
	bl	sub_08016478
	ldr	r0, [sp, #40]
	ldr	r1, [sp, #24]
	bl	sub_08020b64
	movs	r2, #1
	str	r2, [sp, #12]
	b.n	.L_08020d86
.L_08020f9e:
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #36]
	b.n	.L_08021034
.L_08020fa6:
	ldr	r3, [r2, #0]
	movs	r1, #1
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_08020fb2
	b.n	.L_08020d86
.L_08020fb2:
	movs	r0, #112
	bl	sub_080f9010
	cmp	r6, #18
	bne.n	.L_08020fe6
	cmp	r7, #5
	bne.n	.L_08020fde
	ldr	r2, [sp, #28]
	cmp	r2, #0
	bne.n	.L_08020fc8
	b.n	.L_08020d34
.L_08020fc8:
	ldr	r2, [sp, #20]
	ldr	r1, [sp, #24]
	movs	r0, #0
.L_08020fce:
	ldrb	r3, [r1, #0]
	adds	r0, #1
	strb	r3, [r2, #0]
	adds	r1, #1
	adds	r2, #1
	cmp	r0, #14
	ble.n	.L_08020fce
	b.n	.L_08021034
.L_08020fde:
	cmp	r7, #4
	beq.n	.L_08020fe4
	b.n	.L_08020d86
.L_08020fe4:
	b.n	.L_08020f76
.L_08020fe6:
	mov	r3, r8
	ldrh	r2, [r3, #12]
	ldrh	r3, [r3, #14]
	adds	r3, r3, r7
	adds	r2, r2, r6
	adds	r3, #1
	adds	r2, #1
	lsls	r3, r3, #5
	adds	r3, r3, r2
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #28]
	lsls	r3, r3, #1
	ldrb	r3, [r3, r1]
	cmp	r2, #5
	bne.n	.L_08021006
	b.n	.L_08020d86
.L_08021006:
	ldr	r1, [sp, #8]
	adds	r2, #1
	strb	r3, [r1, #0]
	adds	r1, #1
	str	r1, [sp, #8]
	strb	r5, [r1, #0]
	str	r2, [sp, #28]
	cmp	r2, #5
	bne.n	.L_08021020
	movs	r2, #1
	movs	r6, #18
	movs	r7, #5
	mov	r9, r2
.L_08021020:
	ldr	r0, [sp, #40]
	bl	sub_08016478
	ldr	r0, [sp, #40]
	ldr	r1, [sp, #24]
	bl	sub_08020b64
	movs	r3, #1
	str	r3, [sp, #12]
	b.n	.L_08020d86
.L_08021034:
	mov	r0, r8
	movs	r1, #2
	bl	sub_08016418
	movs	r1, #2
	ldr	r0, [sp, #40]
	bl	sub_08016418
	ldr	r0, [sp, #44]
	bl	sub_08019e48
	movs	r0, #1
	bl	sub_080030f8
	ldr	r0, [sp, #36]
	add	sp, #96
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001b04
	.2byte 0x1c94
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	movs	r2, #1
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #13
	movs	r3, #7
	mov	sl, r2
	movs	r0, #7
	movs	r2, #18
	bl	sub_080162d4
	ldr	r5, [pc, #112]
	adds	r6, r0, #0
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #8
	movs	r3, #0
	bl	sub_0801e7c0
	adds	r0, r5, #1
	adds	r1, r6, #0
	movs	r2, #8
	movs	r3, #16
	adds	r5, #2
	bl	sub_0801e7c0
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #8
	movs	r3, #32
	bl	sub_0801e7c0
	bl	sub_08004080
	movs	r7, #0
	str	r0, [sp, #8]
	cmp	r0, #95
	bgt.n	.L_0802110c
	ldr	r2, [pc, #64]
	movs	r1, #128
	bl	sub_08003fa4
	movs	r1, #128
	lsls	r1, r1, #23
	adds	r2, r6, #0
	movs	r3, #0
	ldr	r0, [sp, #8]
	str	r7, [sp, #0]
	bl	sub_0801eadc
	add	r3, sp, #12
	str	r0, [r3, #0]
	ldrh	r1, [r6, #12]
	ldrh	r2, [r6, #14]
	mov	r8, r3
	lsls	r1, r1, #3
	lsls	r2, r2, #3
	subs	r1, #3
	adds	r2, #9
	mov	r0, r8
	bl	sub_080b0038
	b.n	.L_08021110
.L_080210f8:
	movs	r0, #113
	movs	r7, #1
	bl	sub_080f9010
	negs	r7, r7
	b.n	.L_080211fa
	.4byte 0x00002080
	.2byte 0x10a4
	.2byte 0x0803
.L_0802110c:
	add	r2, sp, #12
	mov	r8, r2
.L_08021110:
	ldr	r4, [pc, #28]
	ldr	r3, [pc, #32]
	ldr	r0, [pc, #32]
	adds	r1, r4, #0
	ldr	r2, [pc, #32]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #12]
	ldr	r2, [pc, #28]
	strh	r3, [r4, #8]
	movs	r3, #1
	mov	fp, r3
	mov	r9, r2
	b.n	.L_08021144
	.4byte 0x00006318
	.4byte 0x050001c0
	.4byte 0x040000d4
	.4byte 0x050001e0
	.4byte 0x84000008
	.2byte 0x1b04
	.2byte 0x0300
.L_08021144:
	lsls	r5, r7, #1
	mov	r3, fp
	str	r3, [sp, #0]
	movs	r1, #1
	movs	r3, #14
	adds	r2, r5, #0
	adds	r0, r6, #0
	str	r3, [sp, #4]
	bl	sub_08020a60
	movs	r0, #1
	bl	sub_080030f8
	mov	r2, fp
	movs	r3, #15
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r3, #14
	movs	r1, #1
	adds	r2, r5, #0
	bl	sub_08020a60
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_08021192
	movs	r2, #0
	mov	sl, r2
	ldrh	r2, [r6, #14]
	ldrh	r1, [r6, #12]
	adds	r2, r2, r5
	lsls	r1, r1, #3
	lsls	r2, r2, #3
	subs	r1, #3
	adds	r2, #9
	mov	r0, r8
	movs	r3, #3
	bl	sub_080b0030
.L_08021192:
	mov	r0, r8
	bl	sub_080b0020
	mov	r1, r9
	ldr	r3, [r1, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080211bc
	movs	r0, #111
	bl	sub_080f9010
	movs	r2, #1
	movs	r3, #1
	subs	r7, #1
	negs	r2, r2
	mov	sl, r3
	cmp	r7, r2
	bne.n	.L_080211ba
	movs	r7, #2
.L_080211ba:
	ldr	r1, [pc, #104]
.L_080211bc:
	mov	r2, r9
	ldr	r3, [r2, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080211dc
	movs	r0, #111
	bl	sub_080f9010
	adds	r7, #1
	movs	r3, #1
	mov	sl, r3
	cmp	r7, #3
	bne.n	.L_080211da
	movs	r7, #0
.L_080211da:
	ldr	r1, [pc, #72]
.L_080211dc:
	mov	r2, r9
	ldr	r3, [r2, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080211ea
	b.n	.L_080210f8
.L_080211ea:
	ldr	r3, [r1, #0]
	mov	r2, fp
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08021144
	movs	r0, #112
	bl	sub_080f9010
.L_080211fa:
	movs	r1, #2
	adds	r0, r6, #0
	bl	sub_08016418
	movs	r0, #1
	bl	sub_080030f8
	ldr	r0, [sp, #8]
	bl	sub_08003f3c
	adds	r0, r7, #0
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.2byte 0x1b04
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	str	r2, [sp, #8]
	ldr	r3, [pc, #256]
	adds	r6, r1, #0
	ldr	r5, [r3, #0]
	movs	r3, #3
	ldr	r2, [pc, #252]
	ands	r3, r6
	lsls	r3, r3, #1
	mov	fp, r0
	movs	r0, #0
	mov	r9, r0
	mov	sl, r0
	ldrsh	r1, [r2, r3]
	mov	r8, r1
	mov	r1, r9
	str	r1, [sp, #0]
	movs	r0, #2
	movs	r1, #1
	movs	r2, #26
	movs	r3, #5
	add	r7, sp, #20
	bl	sub_080162d4
	mov	r9, r0
	cmp	r0, #0
	beq.n	.L_0802132a
	movs	r3, #4
	movs	r1, #4
	movs	r2, #0
	str	r3, [sp, #0]
	bl	sub_0801e41c
	ldr	r3, [pc, #200]
	adds	r2, r5, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r0, r8
	bl	sub_08019d2c
	movs	r1, #14
	add	r2, sp, #16
	add	r3, sp, #12
	str	r1, [sp, #0]
	mov	r1, sl
	str	r1, [sp, #4]
	bl	sub_0801a4fc
	mov	r2, sl
	ldr	r3, [pc, #172]
	str	r2, [r7, #0]
	str	r3, [sp, #24]
	movs	r2, #224
	ldr	r3, [sp, #12]
	lsls	r2, r2, #8
	ldr	r0, [pc, #164]
	orrs	r3, r2
	ldr	r2, [pc, #164]
	str	r3, [sp, #28]
	mov	r1, sl
	adds	r3, r5, r0
	strh	r1, [r3, #0]
	mov	r0, sl
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	movs	r1, #1
	mov	r0, fp
	bl	sub_08019908
	lsls	r0, r6, #2
	ldr	r1, [sp, #8]
	adds	r0, r0, r6
	lsls	r0, r0, #2
	movs	r2, #150
	lsls	r2, r2, #1
	adds	r0, r0, r1
	adds	r0, r0, r2
	movs	r1, #4
	bl	sub_08019908
	ldr	r0, [pc, #124]
	adds	r0, r6, r0
	bl	sub_08019ba0
	mov	r3, sl
	adds	r1, r0, #0
	str	r3, [sp, #0]
	movs	r2, #36
	movs	r3, #2
	mov	r0, r9
	bl	sub_080165d8
	movs	r0, #81
	bl	sub_080f9010
	ldr	r5, [pc, #96]
	ldr	r6, [pc, #100]
.L_080212f8:
	adds	r0, r7, #0
	movs	r1, #250
	bl	sub_08003dec
	movs	r0, #1
	bl	sub_080030f8
	bl	sub_080f9048
	cmp	r0, #0
	beq.n	.L_08021316
	ldr	r3, [r6, #0]
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080212f8
.L_08021316:
	mov	r0, r9
	movs	r1, #2
	bl	sub_08016418
	movs	r0, #1
	bl	sub_080030f8
	ldr	r0, [sp, #16]
	bl	sub_08003f3c
.L_0802132a:
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
	.4byte 0x080371fe
	.4byte 0x00000ea3
	.4byte 0x8014000c
	.4byte 0x000012f4
	.4byte 0x000012f6
	.4byte 0x00000980
	.4byte 0x00000303
	.4byte 0x03001c94
