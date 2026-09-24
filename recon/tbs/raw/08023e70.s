.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08002df0, 0x08002df0
	.set sub_080030f8, 0x080030f8
	.set sub_0800352c, 0x0800352c
	.set sub_08003dec, 0x08003dec
	.set sub_08003f3c, 0x08003f3c
	.set sub_080040b4, 0x080040b4
	.set sub_080040d0, 0x080040d0
	.set sub_08004970, 0x08004970
	.set sub_080072f0, 0x080072f0
	.set sub_080072fc, 0x080072fc
	.set sub_080162d4, 0x080162d4
	.set sub_08016418, 0x08016418
	.set sub_08016498, 0x08016498
	.set sub_080164ac, 0x080164ac
	.set sub_08016738, 0x08016738
	.set sub_08017aa4, 0x08017aa4
	.set sub_08019000, 0x08019000
	.set sub_0801965c, 0x0801965c
	.set sub_080198dc, 0x080198dc
	.set sub_08019908, 0x08019908
	.set sub_0801e318, 0x0801e318
	.set sub_0801e3c8, 0x0801e3c8
	.set sub_0801e71c, 0x0801e71c
	.set sub_0801e7c0, 0x0801e7c0
	.set sub_0801e9d4, 0x0801e9d4
	.set sub_08022768, 0x08022768
	.set sub_08022b44, 0x08022b44
	.set sub_08077000, 0x08077000
	.set sub_08077008, 0x08077008
	.set sub_080f9010, 0x080f9010
	.global Func_08023e70
	.thumb_func
Func_08023e70:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #224
	str	r0, [sp, #76]
	ldr	r5, [pc, #768]
	movs	r3, #1
	ldr	r1, [r5, #0]
	movs	r2, #0
	negs	r3, r3
	movs	r0, #128
	str	r1, [sp, #72]
	str	r2, [sp, #68]
	str	r3, [sp, #64]
	bl	sub_080040b4
	str	r0, [sp, #60]
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08004970
	ldr	r2, [sp, #64]
	movs	r1, #0
	movs	r3, #42
	movs	r6, #0
	str	r0, [sp, #56]
	str	r1, [sp, #52]
	str	r2, [sp, #48]
	str	r3, [sp, #0]
	movs	r1, #4
	movs	r2, #30
	movs	r3, #4
	movs	r0, #0
	str	r6, [sp, #40]
	str	r6, [sp, #32]
	str	r6, [sp, #80]
	str	r6, [sp, #28]
	str	r6, [sp, #24]
	bl	sub_080162d4
	str	r0, [sp, #44]
	movs	r0, #1
	bl	sub_0801e3c8
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #9
	movs	r2, #9
	movs	r3, #11
	movs	r0, #21
	bl	sub_080162d4
	mov	r9, r0
	adds	r5, #168
	ldr	r3, [r5, #0]
	ldr	r1, [r3, #52]
	ldr	r2, [r3, #48]
	ldr	r3, [r3, #56]
	mov	fp, r1
	mov	sl, r2
	str	r3, [sp, #36]
	ldr	r0, [sp, #76]
	bl	sub_08077008
	adds	r0, #248
	movs	r7, #0
	mov	r8, r0
.L_08023efe:
	ldr	r1, [sp, #52]
	ldr	r2, [sp, #56]
	lsls	r3, r1, #2
	movs	r6, #0
	adds	r5, r3, r2
.L_08023f08:
	mov	r1, r8
	movs	r2, #1
	ldr	r3, [r1, #16]
	lsls	r2, r6
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08023f24
	lsls	r3, r7, #8
	orrs	r3, r6
	stmia	r5!, {r3}
	ldr	r2, [sp, #52]
	adds	r2, #1
	str	r2, [sp, #52]
	b.n	.L_08023fba
.L_08023f24:
	mov	r1, r8
	ldr	r3, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08023fba
	ldr	r2, [sp, #76]
	movs	r0, #0
	cmp	r2, #7
	bls.n	.L_08023f38
	movs	r0, #1
.L_08023f38:
	bl	sub_08077000
	movs	r2, #132
	adds	r3, r0, #0
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	movs	r1, #0
	adds	r0, #8
	movs	r4, #0
	cmp	r1, r3
	bge.n	.L_08023f94
	ldrb	r3, [r0, #2]
	ldr	r2, [sp, #76]
	cmp	r3, r2
	bne.n	.L_08023f64
	ldrb	r3, [r0, #0]
	cmp	r3, r7
	bne.n	.L_08023f64
	ldrb	r3, [r0, #1]
	cmp	r3, r6
	beq.n	.L_08023f8e
.L_08023f64:
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldr	r3, [r3, #0]
	adds	r1, #1
	cmp	r1, r3
	bge.n	.L_08023f92
	lsls	r4, r1, #2
	adds	r2, r0, r4
	ldrb	r3, [r2, #2]
	mov	ip, r3
	ldr	r3, [sp, #76]
	cmp	ip, r3
	bne.n	.L_08023f64
	ldrb	r3, [r2, #0]
	cmp	r3, r7
	bne.n	.L_08023f64
	ldrb	r3, [r2, #1]
	cmp	r3, r6
	bne.n	.L_08023f64
	b.n	.L_08023f94
.L_08023f8e:
	movs	r4, #0
	b.n	.L_08023f94
.L_08023f92:
	lsls	r4, r1, #2
.L_08023f94:
	lsls	r2, r7, #8
	movs	r3, #128
	lsls	r3, r3, #9
	orrs	r2, r6
	orrs	r2, r3
	str	r2, [r5, #0]
	adds	r3, r0, r4
	ldrb	r3, [r3, #3]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_08023fb2
	lsls	r3, r3, #17
	orrs	r2, r3
	str	r2, [r5, #0]
.L_08023fb2:
	ldr	r1, [sp, #52]
	adds	r1, #1
	str	r1, [sp, #52]
	adds	r5, #4
.L_08023fba:
	adds	r6, #1
	cmp	r6, #19
	ble.n	.L_08023f08
	movs	r2, #4
	adds	r7, #1
	add	r8, r2
	cmp	r7, #3
	ble.n	.L_08023efe
	ldr	r3, [sp, #52]
	ldr	r1, [sp, #56]
	lsls	r2, r3, #2
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r2, r1]
	ldr	r3, [sp, #72]
	ldr	r1, [pc, #428]
	adds	r2, r3, r1
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r2, sp
	adds	r2, #212
	str	r2, [sp, #4]
	ldr	r1, [sp, #72]
	ldr	r2, [pc, #412]
	mov	r3, sp
	adds	r3, #84
	adds	r2, r1, r2
	str	r3, [sp, #12]
	str	r2, [sp, #8]
.L_08023ff4:
	ldr	r3, [sp, #48]
	cmp	fp, r3
	bne.n	.L_08024008
	ldr	r1, [sp, #64]
	cmp	sl, r1
	bne.n	.L_08024008
	ldr	r2, [sp, #28]
	cmp	r2, #0
	bne.n	.L_08024008
	b.n	.L_08024328
.L_08024008:
	mov	r3, fp
	ldr	r1, [sp, #56]
	add	r3, sl
	lsls	r3, r3, #2
	ldr	r5, [r3, r1]
	ldr	r3, [sp, #72]
	ldr	r1, [pc, #372]
	movs	r2, #0
	str	r2, [sp, #40]
	adds	r2, r3, r1
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r2, r9
	ldrh	r0, [r2, #12]
	ldrh	r1, [r2, #14]
	ldr	r2, [sp, #64]
	lsls	r3, r2, #1
	adds	r1, r1, r3
	mov	r3, r9
	ldrh	r2, [r3, #8]
	movs	r3, #15
	adds	r1, #1
	str	r3, [sp, #0]
	adds	r0, #1
	subs	r2, #2
	movs	r3, #1
	bl	sub_08022768
	ldr	r1, [sp, #32]
	cmp	r1, #0
	beq.n	.L_08024064
	ldr	r0, [sp, #44]
	movs	r1, #1
	bl	sub_08016418
	movs	r3, #42
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #4
	movs	r2, #30
	movs	r3, #4
	bl	sub_080162d4
	str	r0, [sp, #44]
	bl	sub_08016738
.L_08024064:
	ldr	r3, [sp, #52]
	movs	r2, #0
	str	r2, [sp, #28]
	cmp	r3, #0
	bne.n	.L_08024070
	b.n	.L_080241a0
.L_08024070:
	bl	sub_080198dc
	movs	r1, #0
	str	r1, [sp, #24]
	ldr	r0, [pc, #276]
	ldr	r3, [r0, #0]
	movs	r2, #228
	ldr	r3, [r3, r2]
	ldr	r1, [sp, #28]
	cmp	r3, r5
	bne.n	.L_0802408c
	movs	r2, #1
	str	r2, [sp, #24]
	b.n	.L_080240a2
.L_0802408c:
	adds	r1, #1
	cmp	r1, #7
	bgt.n	.L_080240a2
	ldr	r3, [r0, #0]
	lsls	r2, r1, #2
	adds	r2, #228
	ldr	r3, [r3, r2]
	cmp	r3, r5
	bne.n	.L_0802408c
	movs	r3, #1
	str	r3, [sp, #24]
.L_080240a2:
	ldr	r1, [sp, #24]
	cmp	r1, #0
	beq.n	.L_080240ca
	ldr	r6, [sp, #12]
	movs	r2, #52
	ldr	r0, [pc, #228]
	adds	r1, r6, #0
	bl	sub_0801965c
	ldr	r2, [sp, #68]
	cmp	r2, #0
	beq.n	.L_080241ac
	adds	r0, r2, #0
	movs	r1, #1
	bl	sub_08016418
	movs	r3, #0
	str	r3, [sp, #68]
	str	r3, [sp, #32]
	b.n	.L_080241ac
.L_080240ca:
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_0802414c
	movs	r0, #248
	lsls	r0, r0, #14
	ands	r0, r5
	cmp	r0, #0
	beq.n	.L_0802411e
	lsrs	r0, r0, #17
	movs	r1, #5
	bl	sub_08019908
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r5
	lsrs	r3, r3, #8
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #255
	ands	r5, r3
	ldr	r6, [sp, #12]
	ldr	r3, [pc, #156]
	lsls	r0, r0, #2
	adds	r0, r0, r5
	adds	r1, r6, #0
	adds	r0, r0, r3
	movs	r2, #52
	bl	sub_0801965c
	ldr	r1, [sp, #68]
	cmp	r1, #0
	beq.n	.L_080241ac
	adds	r0, r1, #0
	movs	r1, #1
	bl	sub_08016418
	movs	r2, #0
	str	r2, [sp, #68]
	str	r2, [sp, #32]
	b.n	.L_080241ac
.L_0802411e:
	add	r3, sp, #80
	str	r3, [sp, #0]
	ldr	r1, [sp, #76]
	adds	r2, r5, #0
	ldr	r3, [sp, #32]
	ldr	r0, [sp, #68]
	bl	sub_08022b44
	ldr	r6, [sp, #12]
	str	r0, [sp, #68]
	adds	r1, r6, #0
	ldr	r0, [pc, #100]
	movs	r2, #52
	bl	sub_0801965c
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r5, r3
	lsrs	r3, r5, #8
	movs	r1, #1
	lsls	r1, r3
	str	r1, [sp, #40]
	b.n	.L_080241ac
.L_0802414c:
	add	r3, sp, #80
	adds	r2, r5, #0
	str	r3, [sp, #0]
	ldr	r1, [sp, #76]
	ldr	r3, [sp, #32]
	ldr	r0, [sp, #68]
	bl	sub_08022b44
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r5
	lsrs	r3, r3, #8
	str	r0, [sp, #68]
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #255
	ands	r5, r3
	lsls	r0, r0, #2
	ldr	r3, [pc, #36]
	ldr	r6, [sp, #12]
	adds	r0, r0, r5
	adds	r0, r0, r3
	adds	r1, r6, #0
	movs	r2, #52
	bl	sub_0801965c
	b.n	.L_080241ac
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x00000ea3
	.4byte 0x00000ea6
	.4byte 0x03001f34
	.4byte 0x000008ef
	.4byte 0x00000666
	.2byte 0x0899
	.2byte 0x0000
.L_080241a0:
	ldr	r6, [sp, #12]
	ldr	r0, [pc, #824]
	adds	r1, r6, #0
	movs	r2, #52
	bl	sub_0801965c
.L_080241ac:
	ldr	r2, [sp, #72]
	ldr	r3, [pc, #816]
	movs	r1, #0
	adds	r5, r2, r3
	strb	r1, [r5, #0]
	ldr	r2, [sp, #32]
	cmp	r2, #0
	bne.n	.L_080241e0
	movs	r3, #1
	strb	r3, [r5, #0]
	ldr	r0, [sp, #44]
	movs	r1, #1
	bl	sub_08016418
	movs	r3, #42
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r3, #4
	movs	r1, #4
	movs	r2, #30
	bl	sub_080162d4
	str	r0, [sp, #44]
	add	r3, sp, #32
	ldrb	r3, [r3, #0]
	strb	r3, [r5, #0]
.L_080241e0:
	ldr	r1, [sp, #44]
	movs	r2, #0
	adds	r0, r6, #0
	movs	r3, #4
	bl	sub_08017aa4
	ldr	r2, [sp, #48]
	mov	r1, sl
	str	r1, [sp, #64]
	cmp	fp, r2
	beq.n	.L_080242ae
	mov	r0, r9
	bl	sub_08016498
	mov	r1, fp
	ldr	r2, [sp, #56]
	lsls	r3, r1, #2
	adds	r3, r3, r2
	movs	r1, #128
	ldr	r6, [r3, #0]
	lsls	r1, r1, #24
	movs	r7, #0
	cmp	r6, r1
	beq.n	.L_080242a8
	mov	r8, r3
.L_08024212:
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r1, r6, #0
	ands	r1, r2
	ldr	r3, [pc, #712]
	lsrs	r1, r1, #8
	adds	r1, r1, r3
	movs	r2, #0
	lsls	r3, r7, #1
	mov	r0, r9
	str	r2, [sp, #0]
	bl	sub_08019000
	movs	r3, #248
	lsls	r3, r3, #14
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_0802423e
	movs	r0, #4
	bl	sub_0801e71c
	b.n	.L_0802424e
.L_0802423e:
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_0802424e
	movs	r0, #2
	bl	sub_0801e71c
.L_0802424e:
	movs	r1, #240
	lsls	r1, r1, #4
	adds	r3, r6, #0
	ands	r3, r1
	lsrs	r3, r3, #8
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #255
	ands	r3, r6
	lsls	r0, r0, #2
	adds	r0, r0, r3
	ldr	r3, [pc, #640]
	lsls	r5, r7, #4
	adds	r0, r0, r3
	mov	r1, r9
	movs	r2, #8
	adds	r3, r5, #0
	bl	sub_0801e7c0
	movs	r0, #248
	lsls	r0, r0, #14
	ands	r0, r6
	cmp	r0, #0
	beq.n	.L_0802428c
	lsrs	r0, r0, #17
	movs	r1, #1
	mov	r2, r9
	movs	r3, #48
	str	r5, [sp, #0]
	bl	sub_0801e9d4
.L_0802428c:
	movs	r0, #15
	adds	r7, #1
	bl	sub_0801e71c
	cmp	r7, #4
	bgt.n	.L_080242a8
	movs	r2, #4
	add	r8, r2
	mov	r3, r8
	movs	r1, #128
	ldr	r6, [r3, #0]
	lsls	r1, r1, #24
	cmp	r6, r1
	bne.n	.L_08024212
.L_080242a8:
	mov	ip, fp
	mov	r2, ip
	str	r2, [sp, #48]
.L_080242ae:
	ldr	r1, [sp, #52]
	cmp	r1, #5
	ble.n	.L_080242f8
	movs	r7, #0
	adds	r1, #4
	mov	r8, r1
	b.n	.L_080242ea
.L_080242bc:
	ldr	r2, [pc, #556]
	mov	r0, fp
	movs	r1, #5
	adds	r6, r7, r2
	bl	sub_080022ec
	cmp	r7, r0
	bne.n	.L_080242d0
	ldr	r3, [pc, #544]
	adds	r6, r7, r3
.L_080242d0:
	mov	r1, r9
	ldrh	r2, [r1, #8]
	subs	r2, r2, r5
	adds	r2, r2, r7
	movs	r3, #0
	str	r3, [sp, #0]
	subs	r2, #2
	mov	r0, r9
	adds	r1, r6, #0
	subs	r3, #1
	bl	sub_08019000
	adds	r7, #1
.L_080242ea:
	mov	r0, r8
	movs	r1, #5
	bl	sub_080022ec
	adds	r5, r0, #0
	cmp	r7, r5
	blt.n	.L_080242bc
.L_080242f8:
	mov	r1, r9
	ldrh	r0, [r1, #12]
	mov	r2, sl
	ldrh	r1, [r1, #14]
	lsls	r3, r2, #1
	adds	r1, r1, r3
	mov	r3, r9
	ldrh	r2, [r3, #8]
	movs	r3, #14
	adds	r1, #1
	subs	r2, #2
	str	r3, [sp, #0]
	adds	r0, #1
	movs	r3, #1
	bl	sub_08022768
	ldr	r1, [sp, #8]
	movs	r3, #1
	strb	r3, [r1, #0]
	ldr	r2, [sp, #72]
	ldr	r1, [pc, #444]
	adds	r3, r2, r1
	movs	r2, #0
	strb	r2, [r3, #0]
.L_08024328:
	ldr	r3, [sp, #52]
	cmp	r3, #5
	ble.n	.L_08024412
	movs	r7, #0
	adds	r3, #4
	mov	r8, r3
	b.n	.L_0802438a
.L_08024336:
	ldr	r3, [pc, #444]
	movs	r2, #128
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #428]
	lsls	r2, r2, #1
	ands	r3, r2
	adds	r6, r7, r1
	cmp	r3, #0
	bne.n	.L_08024354
	ldr	r3, [pc, #428]
	ldr	r3, [r3, #0]
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #11
	bhi.n	.L_08024364
.L_08024354:
	mov	r0, fp
	movs	r1, #5
	bl	sub_080022ec
	cmp	r7, r0
	bne.n	.L_08024364
	ldr	r2, [pc, #396]
	adds	r6, r7, r2
.L_08024364:
	mov	r3, r9
	movs	r1, #5
	mov	r0, r8
	ldrh	r5, [r3, #8]
	bl	sub_080022ec
	subs	r5, r5, r0
	adds	r5, r5, r7
	movs	r1, #0
	subs	r5, #2
	movs	r3, #1
	str	r1, [sp, #0]
	mov	r0, r9
	adds	r1, r6, #0
	adds	r2, r5, #0
	negs	r3, r3
	bl	sub_08019000
	adds	r7, #1
.L_0802438a:
	mov	r0, r8
	movs	r1, #5
	bl	sub_080022ec
	cmp	r7, r0
	blt.n	.L_08024336
	ldr	r3, [pc, #348]
	ldr	r5, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r5, r3
	cmp	r5, #0
	bne.n	.L_080243d0
	mov	r3, r9
	ldrh	r2, [r3, #8]
	movs	r3, #1
	subs	r2, r2, r0
	subs	r2, #3
	mov	r0, r9
	ldr	r1, [pc, #328]
	negs	r3, r3
	str	r5, [sp, #0]
	bl	sub_08019000
	mov	r1, r9
	ldrh	r2, [r1, #8]
	movs	r3, #1
	subs	r2, #2
	mov	r0, r9
	ldr	r1, [pc, #312]
	negs	r3, r3
	str	r5, [sp, #0]
	bl	sub_08019000
	b.n	.L_080243fe
.L_080243d0:
	mov	r3, r9
	ldrh	r2, [r3, #8]
	movs	r1, #0
	subs	r2, r2, r0
	movs	r3, #1
	subs	r2, #3
	str	r1, [sp, #0]
	mov	r0, r9
	ldr	r1, [pc, #288]
	negs	r3, r3
	bl	sub_08019000
	mov	r3, r9
	ldrh	r2, [r3, #8]
	movs	r1, #0
	movs	r3, #1
	str	r1, [sp, #0]
	subs	r2, #2
	mov	r0, r9
	ldr	r1, [pc, #272]
	negs	r3, r3
	bl	sub_08019000
.L_080243fe:
	mov	r2, r9
	ldrh	r3, [r2, #14]
	ldr	r1, [sp, #8]
	subs	r3, #1
	lsrs	r3, r3, #2
	movs	r2, #2
	lsls	r2, r3
	ldrb	r3, [r1, #0]
	orrs	r2, r3
	strb	r2, [r1, #0]
.L_08024412:
	ldr	r3, [pc, #248]
	ldr	r1, [r3, #0]
	ldr	r3, [pc, #248]
	ldr	r0, [pc, #248]
	ldr	r7, [r3, #0]
	ldr	r3, [pc, #212]
	ldr	r2, [r0, #0]
	ldr	r3, [r3, #0]
	mov	r8, r3
	adds	r3, r2, #0
	adds	r3, #216
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0802444a
	adds	r2, #220
	ldr	r3, [r2, #0]
	movs	r1, #0
	movs	r7, #0
	mov	r8, r1
	cmp	r3, #0
	bne.n	.L_08024446
	movs	r3, #60
	str	r3, [r2, #0]
	movs	r7, #1
	movs	r1, #1
	b.n	.L_0802444a
.L_08024446:
	subs	r3, #1
	str	r3, [r2, #0]
.L_0802444a:
	ldr	r2, [r0, #0]
	ldr	r3, [r2, #76]
	cmp	r3, #0
	beq.n	.L_0802445a
	movs	r3, #2
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08024466
.L_0802445a:
	movs	r0, #113
	movs	r6, #1
	bl	sub_080f9010
	negs	r6, r6
	b.n	.L_080248a0
.L_08024466:
	movs	r3, #1
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0802451c
	ldr	r3, [sp, #52]
	cmp	r3, #0
	beq.n	.L_080244d6
	mov	r3, fp
	add	r3, sl
	ldr	r1, [sp, #56]
	lsls	r3, r3, #2
	ldr	r0, [r3, r1]
	movs	r6, #248
	lsls	r6, r6, #14
	adds	r5, r0, #0
	ands	r5, r6
	cmp	r5, #0
	bne.n	.L_080244a0
	ldr	r3, [sp, #24]
	cmp	r3, #0
	bne.n	.L_080244ce
	mov	r1, fp
	mov	r3, sl
	str	r1, [r2, #52]
	str	r3, [r2, #48]
	ldr	r1, [sp, #36]
	adds	r6, r0, #0
	str	r1, [r2, #56]
	b.n	.L_080248a0
.L_080244a0:
	ldr	r2, [sp, #24]
	cmp	r2, #0
	bne.n	.L_080244ce
	ands	r5, r6
	bl	sub_08016738
	bl	sub_080198dc
	lsrs	r0, r5, #17
	movs	r1, #5
	bl	sub_08019908
	movs	r2, #52
	ldr	r1, [sp, #12]
	ldr	r0, [pc, #88]
	bl	sub_0801965c
	movs	r2, #0
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #44]
	movs	r3, #4
	bl	sub_08017aa4
.L_080244ce:
	movs	r0, #114
	bl	sub_080f9010
	b.n	.L_0802451c
.L_080244d6:
	movs	r6, #1
	negs	r6, r6
	b.n	.L_080248a0
	.4byte 0x000008ed
	.4byte 0x00000ea6
	.4byte 0x00005001
	.4byte 0x0000045f
	.4byte 0x0000f301
	.4byte 0x0000f30b
	.4byte 0x03001ae8
	.4byte 0x03001e40
	.4byte 0x0000f334
	.4byte 0x0000f335
	.4byte 0x0000f011
	.4byte 0x0000f012
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x03001f34
	.2byte 0x0898
	.2byte 0x0000
.L_0802451c:
	ldr	r3, [sp, #52]
	cmp	r3, #0
	bne.n	.L_08024524
	b.n	.L_08024766
.L_08024524:
	movs	r3, #128
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08024550
	movs	r0, #111
	bl	sub_080f9010
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #5
	beq.n	.L_08024546
	mov	r3, fp
	ldr	r1, [sp, #52]
	add	r3, sl
	cmp	r3, r1
	bne.n	.L_0802454a
.L_08024546:
	movs	r2, #0
	mov	sl, r2
.L_0802454a:
	mov	r3, sl
	str	r3, [sp, #36]
	b.n	.L_08024766
.L_08024550:
	movs	r3, #64
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08024590
	movs	r0, #111
	bl	sub_080f9010
	movs	r1, #1
	negs	r1, r1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #0
	bge.n	.L_0802458a
	ldr	r0, [sp, #52]
	movs	r1, #5
	subs	r0, #1
	bl	sub_080022ec
	lsls	r3, r0, #2
	adds	r3, r3, r0
	cmp	fp, r3
	bne.n	.L_08024586
	ldr	r1, [sp, #52]
	mov	r2, fp
	subs	r3, r1, r2
	subs	r3, #1
	b.n	.L_08024588
.L_08024586:
	movs	r3, #4
.L_08024588:
	mov	sl, r3
.L_0802458a:
	mov	r1, sl
	str	r1, [sp, #36]
	b.n	.L_08024766
.L_08024590:
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08024694
	ldr	r3, [sp, #68]
	cmp	r3, #0
	beq.n	.L_08024622
	ldr	r0, [sp, #80]
	movs	r5, #0
	cmp	r5, r0
	bge.n	.L_080245e6
.L_080245aa:
	ldr	r3, [pc, #384]
	ldr	r2, [pc, #384]
	ldr	r3, [r3, #0]
	adds	r1, r5, r2
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #11
	bhi.n	.L_080245c8
	ldr	r3, [sp, #32]
	subs	r3, #1
	cmp	r5, r3
	bne.n	.L_080245c8
	ldr	r3, [sp, #32]
	ldr	r2, [pc, #364]
	adds	r1, r3, r2
.L_080245c8:
	ldr	r3, [sp, #68]
	ldrh	r2, [r3, #8]
	subs	r2, r2, r0
	adds	r2, r2, r5
	movs	r3, #0
	str	r3, [sp, #0]
	ldr	r0, [sp, #68]
	subs	r2, #2
	subs	r3, #1
	bl	sub_08019000
	ldr	r0, [sp, #80]
	adds	r5, #1
	cmp	r5, r0
	blt.n	.L_080245aa
.L_080245e6:
	ldr	r1, [sp, #68]
	ldrh	r2, [r1, #8]
	movs	r3, #0
	subs	r2, r2, r0
	str	r3, [sp, #0]
	adds	r0, r1, #0
	subs	r2, #3
	ldr	r1, [pc, #320]
	subs	r3, #1
	bl	sub_08019000
	ldr	r1, [sp, #68]
	ldrh	r2, [r1, #8]
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r0, r1, #0
	subs	r2, #2
	ldr	r1, [pc, #304]
	subs	r3, #1
	bl	sub_08019000
	ldr	r1, [sp, #68]
	ldrh	r2, [r1, #14]
	ldr	r1, [sp, #8]
	lsrs	r2, r2, #2
	movs	r3, #2
	lsls	r3, r2
	ldrb	r2, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
.L_08024622:
	ldr	r2, [sp, #32]
	cmp	r2, #0
	bne.n	.L_08024642
	ldr	r0, [sp, #80]
	cmp	r0, #0
	beq.n	.L_08024644
	ldr	r3, [sp, #68]
	cmp	r3, #0
	beq.n	.L_0802463a
	adds	r0, r3, #0
	bl	sub_080164ac
.L_0802463a:
	movs	r1, #1
	str	r1, [sp, #32]
	str	r1, [sp, #28]
	b.n	.L_08024766
.L_08024642:
	ldr	r0, [sp, #80]
.L_08024644:
	ldr	r2, [sp, #32]
	cmp	r2, r0
	ble.n	.L_0802464c
	str	r0, [sp, #32]
.L_0802464c:
	ldr	r3, [sp, #32]
	cmp	r3, #0
	bne.n	.L_08024654
	b.n	.L_08024766
.L_08024654:
	movs	r3, #16
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08024672
	movs	r0, #111
	bl	sub_080f9010
	ldr	r1, [sp, #32]
	ldr	r3, [sp, #80]
	adds	r1, #1
	str	r1, [sp, #32]
	cmp	r1, r3
	ble.n	.L_0802468e
	movs	r2, #1
	b.n	.L_0802468c
.L_08024672:
	movs	r3, #32
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08024766
	movs	r0, #111
	bl	sub_080f9010
	ldr	r1, [sp, #32]
	subs	r1, #1
	str	r1, [sp, #32]
	cmp	r1, #0
	bgt.n	.L_0802468e
	ldr	r2, [sp, #80]
.L_0802468c:
	str	r2, [sp, #32]
.L_0802468e:
	movs	r3, #1
	str	r3, [sp, #28]
	b.n	.L_08024766
.L_08024694:
	ldr	r1, [sp, #32]
	cmp	r1, #0
	beq.n	.L_080246b0
	ldr	r2, [sp, #68]
	cmp	r2, #0
	beq.n	.L_080246a6
	adds	r0, r2, #0
	bl	sub_080164ac
.L_080246a6:
	movs	r3, #0
	movs	r1, #1
	str	r3, [sp, #32]
	str	r1, [sp, #28]
	b.n	.L_08024766
.L_080246b0:
	movs	r3, #16
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08024708
	movs	r0, #111
	bl	sub_080f9010
	bl	sub_0800352c
	mov	r3, fp
	ldr	r2, [sp, #52]
	adds	r3, #5
	cmp	r3, r2
	blt.n	.L_080246dc
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L_08024766
	ldr	r2, [sp, #36]
	movs	r1, #0
	mov	fp, r1
	mov	sl, r2
	b.n	.L_08024766
.L_080246dc:
	ldr	r0, [sp, #52]
	mov	fp, r3
	ldr	r3, [sp, #36]
	subs	r0, #1
	movs	r1, #5
	mov	sl, r3
	bl	sub_080022ec
	lsls	r3, r0, #2
	adds	r3, r3, r0
	cmp	fp, r3
	bne.n	.L_08024766
	ldr	r1, [sp, #52]
	mov	r2, fp
	subs	r3, r1, r2
	subs	r3, #1
	mov	sl, r3
	ldr	r3, [sp, #36]
	cmp	sl, r3
	ble.n	.L_08024766
	mov	sl, r3
	b.n	.L_08024766
.L_08024708:
	movs	r3, #32
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08024766
	movs	r0, #111
	bl	sub_080f9010
	bl	sub_0800352c
	mov	r1, fp
	cmp	r1, #0
	beq.n	.L_08024740
	movs	r2, #5
	ldr	r3, [sp, #36]
	negs	r2, r2
	add	fp, r2
	mov	sl, r3
	b.n	.L_08024766
	.4byte 0x03001e40
	.4byte 0x0000f301
	.4byte 0x0000f30a
	.4byte 0x0000f334
	.2byte 0xf335
	.2byte 0x0000
.L_08024740:
	ldr	r0, [sp, #52]
	movs	r1, #5
	subs	r0, #1
	bl	sub_080022ec
	lsls	r3, r0, #2
	ldr	r1, [sp, #36]
	adds	r3, r3, r0
	mov	fp, r3
	mov	sl, r1
	cmp	r3, #0
	beq.n	.L_08024766
	ldr	r2, [sp, #52]
	subs	r3, r2, r3
	subs	r3, #1
	mov	sl, r3
	cmp	sl, r1
	ble.n	.L_08024766
	mov	sl, r1
.L_08024766:
	mov	r1, r9
	ldrh	r3, [r1, #12]
	lsls	r3, r3, #3
	mov	r2, sl
	subs	r3, #2
	str	r3, [sp, #16]
	lsls	r3, r2, #1
	ldrh	r2, [r1, #14]
	adds	r3, r3, r2
	lsls	r3, r3, #3
	adds	r3, #20
	ldr	r1, [sp, #4]
	str	r3, [sp, #20]
	movs	r3, #128
	lsls	r3, r3, #23
	movs	r2, #0
	str	r3, [r1, #4]
	str	r2, [r1, #8]
	ldr	r0, [sp, #60]
	ldr	r1, [pc, #72]
	bl	sub_080040d0
	ldr	r3, [pc, #52]
	ldr	r1, [sp, #4]
	ands	r0, r3
	ldr	r2, [pc, #48]
	ldrh	r3, [r1, #8]
	ldr	r6, [pc, #60]
	ands	r3, r2
	orrs	r3, r0
	adds	r2, r1, #0
	strh	r3, [r2, #8]
	ldr	r2, [r6, #0]
	movs	r5, #4
	ldr	r3, [sp, #16]
	ands	r2, r5
	ldr	r1, [pc, #48]
	lsrs	r2, r2, #1
	adds	r2, r3, r2
	adds	r2, r2, r1
	ldr	r3, [pc, #24]
	ldr	r1, [sp, #4]
	ands	r2, r3
	ldrh	r3, [r1, #6]
	ldr	r1, [pc, #20]
	ands	r3, r1
	orrs	r3, r2
	b.n	.L_080247e4
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x080310a4
	.4byte 0x03001e40
	.2byte 0xfffa
	.2byte 0x0000
.L_080247e4:
	ldr	r2, [sp, #4]
	strh	r3, [r2, #6]
	ldr	r3, [r6, #0]
	ldr	r1, [sp, #20]
	ands	r3, r5
	lsrs	r3, r3, #2
	subs	r3, r1, r3
	adds	r3, #248
	strb	r3, [r2, #4]
	ldr	r2, [sp, #52]
	cmp	r2, #0
	beq.n	.L_08024804
	ldr	r0, [sp, #4]
	movs	r1, #242
	bl	sub_08003dec
.L_08024804:
	ldr	r3, [pc, #260]
	ldr	r3, [r3, #0]
	ldrh	r2, [r3, #12]
	ldr	r6, [r6, #0]
	ldr	r7, [r3, #0]
	movs	r3, #2
	ands	r3, r2
	ands	r6, r5
	cmp	r3, #0
	beq.n	.L_08024858
	movs	r5, #0
.L_0802481a:
	negs	r3, r6
	orrs	r3, r6
	lsrs	r3, r3, #31
	adds	r2, r3, #0
	movs	r3, #15
	subs	r2, r3, r2
	ldr	r1, [sp, #40]
	movs	r3, #1
	lsls	r3, r5
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_08024834
	movs	r2, #15
.L_08024834:
	ldr	r3, [pc, #216]
	ldrh	r0, [r7, #12]
	ldrb	r3, [r3, r5]
	adds	r0, r0, r3
	ldr	r3, [pc, #212]
	ldrh	r1, [r7, #14]
	ldrb	r3, [r3, r5]
	adds	r1, r1, r3
	str	r2, [sp, #0]
	adds	r0, #1
	adds	r1, #1
	movs	r2, #2
	movs	r3, #2
	adds	r5, #1
	bl	sub_08022768
	cmp	r5, #3
	ble.n	.L_0802481a
.L_08024858:
	ldr	r3, [pc, #188]
	ldr	r3, [r3, #0]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802487e
	ldr	r5, [pc, #180]
	movs	r2, #32
	adds	r1, r5, #0
	ldr	r6, [pc, #180]
	ldr	r0, [pc, #180]
	bl	sub_080072fc
	ldr	r0, [pc, #180]
	adds	r1, r5, #0
	movs	r2, #32
	bl	sub_080072fc
	b.n	.L_08024896
.L_0802487e:
	ldr	r3, [pc, #172]
	movs	r1, #32
	ldr	r2, [pc, #172]
	ldr	r0, [pc, #156]
	bl	sub_080072f0
	ldr	r3, [pc, #148]
	ldr	r0, [pc, #152]
	ldr	r1, [pc, #140]
	movs	r2, #32
	bl	sub_080072f0
.L_08024896:
	movs	r0, #1
	bl	sub_080030f8
	bl	.L_08023ff4
.L_080248a0:
	ldr	r3, [pc, #104]
	ldr	r1, [r3, #0]
	ldrh	r2, [r1, #12]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080248c4
	ldr	r3, [r1, #0]
	ldrh	r0, [r3, #12]
	ldrh	r1, [r3, #14]
	movs	r3, #15
	str	r3, [sp, #0]
	adds	r0, #1
	adds	r1, #1
	movs	r2, #4
	movs	r3, #4
	bl	sub_08022768
.L_080248c4:
	ldr	r0, [sp, #60]
	bl	sub_08003f3c
	movs	r1, #1
	ldr	r0, [sp, #44]
	bl	sub_08016418
	movs	r1, #1
	ldr	r0, [sp, #68]
	bl	sub_08016418
	movs	r1, #1
	mov	r0, r9
	bl	sub_08016418
	bl	sub_0801e318
	movs	r0, #0
	bl	sub_0801e3c8
	ldr	r0, [sp, #56]
	bl	sub_08002df0
	movs	r0, #1
	bl	sub_080030f8
	adds	r0, r6, #0
	add	sp, #224
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e90
	.4byte 0x080373e7
	.4byte 0x080373eb
	.4byte 0x03001e40
	.4byte 0x08037308
	.4byte 0x03001388
	.4byte 0x06006500
	.4byte 0x06006520
	.4byte 0x03000168
	.4byte 0x44444444
