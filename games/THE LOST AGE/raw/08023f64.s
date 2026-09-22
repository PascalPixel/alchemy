.syntax unified
	.thumb
	.set sub_080148e8, 0x080148e8
	.set sub_080149e0, 0x080149e0
	.set sub_08024f20, 0x08024f20
	.set sub_0802d45c, 0x0802d45c
	.global Overlay_08023f64
Overlay_08023f64:
.L_08023f64:
	push	{lr}
	adds	r3, r0, #0
	adds	r3, #85
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_08023f7e
	ldr	r3, [r0, #56]
	movs	r2, #128
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_08023f90
	ldr	r2, [r0, #60]
	b.n	.L_08023f84
.L_08023f7e:
	ldr	r2, [r0, #56]
	movs	r3, #128
	lsls	r3, r3, #24
.L_08023f84:
	cmp	r2, r3
	bne.n	.L_08023f90
	ldr	r3, [r0, #64]
	movs	r0, #1
	cmp	r3, r2
	beq.n	.L_08023f92
.L_08023f90:
	movs	r0, #0
.L_08023f92:
	pop	{pc}
	push	{lr}
	bl	.L_08023f64
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #20]
	sub	sp, #44
	adds	r1, r7, #0
	movs	r0, #63
	adds	r1, #85
	str	r0, [sp, #28]
	str	r1, [sp, #0]
.L_08023fbc:
	movs	r2, #0
	str	r2, [sp, #8]
	ldr	r1, [r7, #0]
	cmp	r1, #0
	bne.n	.L_08023fc8
	b.n	.L_08024690
.L_08023fc8:
	ldr	r3, [r7, #108]
	cmp	r3, #0
	beq.n	.L_08023fd6
	adds	r0, r7, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x6839
.L_08023fd6:
	cmp	r1, #0
	bne.n	.L_08023fdc
	b.n	.L_08024690
.L_08023fdc:
	ldr	r4, [sp, #0]
	ldrb	r3, [r4, #6]
	cmp	r3, #0
	beq.n	.L_08023fee
	ldr	r6, [sp, #8]
	str	r6, [r7, #112]
	str	r6, [r7, #116]
	str	r6, [r7, #120]
	b.n	.L_08024690
.L_08023fee:
	adds	r2, r7, #0
	adds	r2, #94
	movs	r4, #0
	ldrsh	r3, [r2, r4]
	ldrh	r0, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08024002
	subs	r3, r0, #1
	strh	r3, [r2, #0]
	b.n	.L_08024030
.L_08024002:
	ldr	r5, [pc, #768]
	b.n	.L_0802400c
.L_08024006:
	adds	r3, r2, #1
	strh	r3, [r7, #4]
.L_0802400a:
	ldr	r1, [r7, #0]
.L_0802400c:
	movs	r6, #4
	ldrsh	r3, [r7, r6]
	ldrh	r2, [r7, #4]
	lsls	r3, r3, #2
	ldr	r3, [r1, r3]
	cmp	r3, #63
	bhi.n	.L_08024006
	lsls	r3, r3, #2
	ldr	r3, [r5, r3]
	adds	r0, r7, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2800
	bne.n	.L_0802400a
	ldr	r3, [r7, #0]
	cmp	r3, #0
	bne.n	.L_08024030
	b.n	.L_08024690
.L_08024030:
	ldr	r0, [r7, #8]
	ldr	r4, [sp, #0]
	str	r0, [sp, #24]
	ldr	r1, [r7, #12]
	str	r1, [sp, #20]
	ldr	r2, [r7, #16]
	str	r2, [sp, #16]
	ldrb	r3, [r4, #12]
	cmp	r3, #0
	beq.n	.L_08024046
	b.n	.L_0802453c
.L_08024046:
	str	r4, [sp, #4]
	ldrb	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L_08024050
	b.n	.L_080241c2
.L_08024050:
	ldr	r3, [r7, #56]
	movs	r6, #128
	lsls	r6, r6, #24
	cmp	r3, r6
	beq.n	.L_0802414e
	subs	r0, r3, r0
	cmp	r0, #0
	bge.n	.L_08024068
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r0, r0, r1
.L_08024068:
	ldr	r3, [r7, #60]
	ldr	r2, [sp, #20]
	asrs	r0, r0, #16
	mov	sl, r0
	subs	r0, r3, r2
	cmp	r0, #0
	bge.n	.L_0802407e
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r0, r0, r3
.L_0802407e:
	ldr	r3, [r7, #64]
	ldr	r4, [sp, #16]
	asrs	r0, r0, #16
	mov	fp, r0
	subs	r0, r3, r4
	cmp	r0, #0
	bge.n	.L_08024094
	movs	r6, #255
	lsls	r6, r6, #8
	adds	r6, #255
	adds	r0, r0, r6
.L_08024094:
	asrs	r0, r0, #16
	mov	r9, r0
	mov	r2, fp
	mov	r1, sl
	mov	r3, fp
	muls	r3, r2
	mov	r0, sl
	muls	r0, r1
	mov	r4, r9
	mov	r2, r9
	muls	r2, r4
	adds	r0, r0, r3
	adds	r0, r0, r2
	ldr	r3, [pc, #600]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2800
	bne.n	.L_080240c6
	ldr	r6, [r7, #56]
	str	r6, [sp, #24]
	ldr	r0, [r7, #60]
	str	r0, [sp, #20]
	ldr	r1, [r7, #64]
	str	r1, [sp, #16]
	b.n	.L_0802453c
.L_080240c6:
	ldr	r1, [r7, #52]
	ldr	r2, [pc, #576]
	lsls	r0, r0, #16
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x1c05
	mov	r3, sl
	muls	r3, r5
	ldr	r0, [r7, #36]
	ldr	r1, [r7, #40]
	adds	r0, r0, r3
	mov	r3, fp
	muls	r3, r5
	mov	r2, r9
	muls	r2, r5
	adds	r1, r1, r3
	ldr	r3, [r7, #44]
	mov	sl, r0
	adds	r3, r3, r2
	mov	r9, r3
	str	r3, [r7, #44]
	ldr	r3, [pc, #540]
	mov	r8, r1
	mov	fp, r3
	str	r1, [r7, #40]
	str	r0, [r7, #36]
	mov	r1, sl
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x4641
	adds	r5, r0, #0
	mov	r0, r8
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x4649
	adds	r6, r0, #0
	mov	r0, r9
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x19ad
	adds	r5, r5, r0
	adds	r0, r5, #0
	bl	sub_080149e0
	ldr	r1, [r7, #48]
	cmp	r0, r1
	bgt.n	.L_08024126
	b.n	.L_0802453c
.L_08024126:
	ldr	r4, [pc, #484]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c05
	adds	r1, r5, #0
	mov	r0, sl
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x1c29
	str	r0, [r7, #36]
	mov	r0, r8
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x1c29
	str	r0, [r7, #40]
	mov	r0, r9
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x62f8
	b.n	.L_0802453c
.L_0802414e:
	ldr	r6, [r7, #36]
	ldr	r2, [pc, #444]
	ldr	r0, [r7, #40]
	ldr	r1, [r7, #44]
	mov	sl, r6
	mov	fp, r0
	mov	r9, r1
	mov	r8, r2
	mov	r1, sl
	mov	r0, sl
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x4659
	adds	r5, r0, #0
	mov	r0, fp
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x4649
	adds	r6, r0, #0
	mov	r0, r9
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x19ad
	adds	r5, r5, r0
	adds	r0, r5, #0
	bl	sub_080149e0
	cmp	r0, #0
	beq.n	.L_080241ba
	ldr	r3, [r7, #52]
	subs	r1, r0, r3
	cmp	r1, #0
	bge.n	.L_08024192
	movs	r1, #0
.L_08024192:
	ldr	r3, [pc, #376]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c05
	adds	r1, r5, #0
	mov	r0, sl
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c29
	str	r0, [r7, #36]
	mov	r0, fp
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c29
	str	r0, [r7, #40]
	mov	r0, r9
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x62f8
	b.n	.L_0802453c
.L_080241ba:
	str	r0, [r7, #36]
	str	r0, [r7, #40]
	str	r0, [r7, #44]
	b.n	.L_0802453c
.L_080241c2:
	ldr	r3, [r7, #56]
	movs	r4, #128
	lsls	r4, r4, #24
	cmp	r3, r4
	beq.n	.L_080242b6
	ldr	r6, [sp, #24]
	subs	r0, r3, r6
	cmp	r0, #0
	bge.n	.L_080241dc
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r0, r0, r1
.L_080241dc:
	ldr	r3, [r7, #64]
	ldr	r2, [sp, #16]
	asrs	r0, r0, #16
	mov	sl, r0
	subs	r0, r3, r2
	cmp	r0, #0
	bge.n	.L_080241f2
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r0, r0, r3
.L_080241f2:
	asrs	r0, r0, #16
	mov	r9, r0
	mov	r4, sl
	mov	r6, r9
	mov	r0, sl
	muls	r0, r4
	mov	r3, r9
	muls	r3, r6
	adds	r0, r0, r3
	ldr	r3, [pc, #256]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4942
	lsls	r0, r0, #16
	cmp	r0, r1
	bgt.n	.L_0802423e
	ldr	r3, [r7, #56]
	ldr	r2, [sp, #24]
	ldr	r4, [sp, #16]
	subs	r2, r3, r2
	ldr	r3, [r7, #64]
	mov	sl, r2
	subs	r4, r3, r4
	ldr	r6, [pc, #236]
	mov	r1, sl
	mov	r0, sl
	mov	r9, r4
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4649
	adds	r5, r0, #0
	mov	r0, r9
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x182d
	adds	r0, r5, #0
	bl	sub_080149e0
.L_0802423e:
	cmp	r0, #0
	bne.n	.L_0802424c
	ldr	r6, [r7, #56]
	str	r6, [sp, #24]
	ldr	r0, [r7, #64]
	str	r0, [sp, #16]
	b.n	.L_0802431c
.L_0802424c:
	ldr	r2, [pc, #188]
	ldr	r1, [r7, #52]
	mov	r8, r2
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c05
	ldr	r6, [pc, #180]
	adds	r1, r5, #0
	mov	r0, sl
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x6a7b
	adds	r1, r5, #0
	adds	r3, r3, r0
	str	r3, [r7, #36]
	mov	r0, r9
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x6a7c
	ldr	r3, [r7, #44]
	mov	sl, r4
	adds	r3, r3, r0
	str	r3, [r7, #44]
	mov	r1, sl
	mov	r0, sl
	mov	r9, r3
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4649
	adds	r5, r0, #0
	mov	r0, r9
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x182d
	adds	r0, r5, #0
	bl	sub_080149e0
	ldr	r1, [r7, #48]
	cmp	r0, r1
	ble.n	.L_0802431c
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c05
	adds	r1, r5, #0
	mov	r0, sl
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c29
	str	r0, [r7, #36]
	mov	r0, r9
	mov	lr, r6
	.2byte 0xf800
	.2byte 0xe031
.L_080242b6:
	ldr	r6, [r7, #36]
	ldr	r0, [r7, #44]
	mov	sl, r6
	mov	r9, r0
	ldr	r6, [pc, #80]
	mov	r1, sl
	mov	r0, sl
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4649
	adds	r5, r0, #0
	mov	r0, r9
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x182d
	adds	r0, r5, #0
	bl	sub_080149e0
	cmp	r0, #0
	beq.n	.L_08024318
	ldr	r3, [r7, #52]
	subs	r1, r0, r3
	cmp	r1, #0
	bge.n	.L_080242e8
	movs	r1, #0
.L_080242e8:
	ldr	r3, [pc, #32]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c05
	adds	r1, r5, #0
	mov	r0, sl
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c29
	str	r0, [r7, #36]
	mov	r0, r9
	mov	lr, r6
	.2byte 0xf800
	.2byte 0xe00a
	.4byte 0x0802f204
	.4byte 0x030002d4
	.4byte 0x03000230
	.4byte 0x0300021c
	.2byte 0xffff
	.2byte 0x00ff
.L_08024318:
	str	r0, [r7, #36]
	str	r0, [r7, #44]
.L_0802431c:
	ldr	r1, [sp, #4]
	movs	r3, #1
	ldrb	r2, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0802432a
	b.n	.L_08024496
.L_0802432a:
	ldr	r3, [r7, #36]
	ldr	r2, [sp, #24]
	ldr	r4, [sp, #16]
	adds	r2, r2, r3
	ldr	r3, [r7, #44]
	movs	r6, #0
	str	r6, [r7, #124]
	adds	r4, r4, r3
	adds	r3, r7, #0
	adds	r3, #35
	mov	sl, r2
	ldrb	r2, [r3, #0]
	movs	r3, #64
	ands	r3, r2
	mov	r9, r4
	mov	fp, r6
	cmp	r3, #0
	beq.n	.L_080243c0
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r0, #128
	ldr	r1, [r3, #20]
	ldr	r5, [pc, #624]
	lsls	r0, r0, #13
	movs	r4, #0
	mov	ip, r0
.L_0802435e:
	ldr	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_080243b8
	adds	r3, r1, #0
	adds	r3, #35
	ldrb	r2, [r3, #0]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080243b8
	cmp	r1, r7
	beq.n	.L_080243b8
	ldr	r3, [r1, #8]
	mov	r2, sl
	movs	r6, #128
	subs	r3, r2, r3
	lsls	r6, r6, #12
	adds	r3, r3, r6
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_080243b8
	ldr	r3, [r1, #16]
	mov	r0, r9
	subs	r3, r0, r3
	adds	r3, r3, r6
	asrs	r3, r3, #20
	cmp	r3, #0
	bne.n	.L_080243b8
	ldr	r3, [sp, #20]
	ldr	r0, [r1, #12]
	ldr	r6, [pc, #560]
	subs	r2, r3, r0
	adds	r3, r2, r6
	cmp	r3, #0
	bge.n	.L_080243a8
	mov	r6, ip
	subs	r3, r6, r2
.L_080243a8:
	cmp	r3, r5
	bgt.n	.L_080243b8
	movs	r2, #1
	add	r0, ip
	str	r0, [sp, #12]
	mov	fp, r2
	str	r1, [r7, #124]
	b.n	.L_080243c0
.L_080243b8:
	adds	r4, #1
	adds	r1, #128
	cmp	r4, #63
	ble.n	.L_0802435e
.L_080243c0:
	mov	r3, fp
	cmp	r3, #0
	bne.n	.L_080243d6
	adds	r3, r7, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	mov	r1, sl
	mov	r2, r9
	bl	sub_0802d45c
	str	r0, [sp, #12]
.L_080243d6:
	ldr	r3, [r7, #20]
	ldr	r4, [sp, #12]
	ldr	r6, [sp, #12]
	ldr	r0, [sp, #20]
	ldr	r1, [pc, #496]
	subs	r4, r4, r3
	subs	r3, r6, r0
	mov	r8, r4
	cmp	r3, r1
	ble.n	.L_080243ee
	add	r0, r8
	str	r0, [sp, #20]
.L_080243ee:
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L_080243f8
	negs	r2, r2
	mov	r8, r2
.L_080243f8:
	ldr	r3, [r7, #52]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	cmp	r8, r3
	ble.n	.L_08024406
	mov	r8, r3
.L_08024406:
	mov	r4, r8
	lsls	r3, r4, #1
	add	r8, r3
	mov	r6, r8
	cmp	r6, #0
	beq.n	.L_08024492
	ldr	r0, [sp, #4]
	movs	r3, #16
	ldrb	r2, [r0, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08024492
	mov	r1, fp
	cmp	r1, #0
	bne.n	.L_08024492
	ldr	r2, [r7, #36]
	ldr	r3, [r7, #40]
	ldr	r4, [r7, #44]
	mov	sl, r2
	ldr	r6, [pc, #420]
	mov	r1, sl
	mov	r0, sl
	mov	fp, r3
	mov	r9, r4
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4659
	adds	r5, r0, #0
	mov	r0, fp
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4649
	adds	r6, r0, #0
	ldr	r2, [pc, #392]
	mov	r0, r9
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x19ad
	adds	r5, r5, r0
	adds	r0, r5, #0
	bl	sub_080149e0
	cmp	r0, #0
	beq.n	.L_08024492
	mov	r3, r8
	subs	r1, r0, r3
	cmp	r1, #0
	bge.n	.L_08024468
	movs	r1, #0
.L_08024468:
	ldr	r3, [pc, #364]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c05
	ldr	r4, [pc, #352]
	adds	r1, r5, #0
	mov	r0, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c29
	str	r0, [r7, #36]
	ldr	r6, [pc, #340]
	mov	r0, fp
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c29
	str	r0, [r7, #40]
	mov	r0, r9
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x62f8
.L_08024492:
	ldr	r0, [sp, #12]
	str	r0, [r7, #20]
.L_08024496:
	ldr	r1, [sp, #4]
	movs	r3, #2
	ldrb	r2, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080244f0
	ldr	r2, [r7, #20]
	ldr	r3, [sp, #20]
	str	r2, [sp, #12]
	cmp	r3, r2
	ble.n	.L_080244b4
	ldr	r3, [r7, #40]
	ldr	r2, [r7, #72]
	subs	r3, r3, r2
	str	r3, [r7, #40]
.L_080244b4:
	ldr	r0, [r7, #40]
	ldr	r4, [sp, #20]
	ldr	r6, [sp, #12]
	adds	r3, r4, r0
	cmp	r3, r6
	bgt.n	.L_080244f0
	cmp	r0, #0
	bge.n	.L_080244ec
	ldr	r1, [r7, #68]
	ldr	r3, [pc, #268]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4243
	subs	r1, r6, r3
	adds	r2, r3, #0
	str	r3, [r7, #40]
	str	r1, [sp, #20]
	cmp	r2, #0
	bge.n	.L_080244dc
	adds	r2, r0, #0
.L_080244dc:
	ldr	r3, [r7, #72]
	cmp	r2, r3
	bgt.n	.L_080244f0
	ldr	r2, [sp, #12]
	movs	r3, #0
	str	r2, [sp, #20]
	str	r3, [r7, #40]
	b.n	.L_080244f0
.L_080244ec:
	ldr	r3, [sp, #12]
	str	r3, [sp, #20]
.L_080244f0:
	ldr	r4, [sp, #4]
	movs	r3, #4
	ldrb	r2, [r4, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802453c
	ldr	r1, [r7, #68]
	movs	r3, #63
	ands	r1, r3
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08024520
	ldr	r2, [pc, #208]
	lsrs	r3, r1, #1
	lsls	r3, r3, #2
	ldr	r2, [r2, r3]
	ldr	r3, [r7, #72]
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L_0802451c
	adds	r3, #15
.L_0802451c:
	asrs	r3, r3, #4
	b.n	.L_08024534
.L_08024520:
	ldr	r2, [pc, #184]
	lsrs	r3, r1, #1
	lsls	r3, r3, #2
	ldr	r2, [r2, r3]
	ldr	r3, [r7, #72]
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L_08024532
	adds	r3, #63
.L_08024532:
	asrs	r3, r3, #6
.L_08024534:
	str	r3, [r7, #40]
	ldr	r3, [r7, #68]
	adds	r3, #1
	str	r3, [r7, #68]
.L_0802453c:
	ldr	r3, [r7, #36]
	ldr	r6, [sp, #24]
	ldr	r0, [sp, #20]
	adds	r6, r6, r3
	str	r6, [sp, #24]
	ldr	r3, [r7, #40]
	ldr	r1, [sp, #16]
	adds	r0, r0, r3
	str	r0, [sp, #20]
	ldr	r3, [r7, #44]
	adds	r1, r1, r3
	str	r1, [sp, #16]
	ldr	r3, [sp, #0]
	ldrb	r2, [r3, #4]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08024582
	add	r1, sp, #32
	str	r0, [r1, #4]
	str	r6, [r1, #0]
	ldr	r4, [sp, #16]
	adds	r0, r7, #0
	str	r4, [r1, #8]
	bl	sub_08024f20
	cmp	r0, #0
	beq.n	.L_0802457e
	ldr	r6, [sp, #0]
	ldrb	r3, [r6, #11]
	adds	r3, #1
	strb	r3, [r6, #11]
	b.n	.L_08024690
.L_0802457e:
	ldr	r1, [sp, #0]
	strb	r0, [r1, #11]
.L_08024582:
	ldr	r2, [sp, #0]
	adds	r1, r7, #0
	ldrb	r3, [r2, #1]
	adds	r1, #86
	cmp	r3, #17
	beq.n	.L_080245ac
	cmp	r3, #17
	bgt.n	.L_08024598
	cmp	r3, #16
	beq.n	.L_0802459e
	b.n	.L_080245fa
.L_08024598:
	cmp	r3, #18
	beq.n	.L_080245e0
	b.n	.L_080245fa
.L_0802459e:
	ldr	r2, [r7, #56]
	ldr	r3, [sp, #24]
	cmp	r3, r2
	beq.n	.L_080245f6
	ldr	r3, [r7, #8]
	ldr	r4, [sp, #24]
	b.n	.L_080245ec
.L_080245ac:
	ldr	r2, [r7, #60]
	ldr	r0, [sp, #20]
	cmp	r0, r2
	beq.n	.L_080245c0
	ldr	r3, [r7, #12]
	subs	r3, r3, r2
	subs	r2, r0, r2
	eors	r3, r2
	cmp	r3, #0
	bge.n	.L_080245fa
.L_080245c0:
	movs	r2, #1
	str	r2, [sp, #8]
	b.n	.L_080245fa
	movs	r0, r0
	.4byte 0x0007ffff
	.4byte 0xfff00000
	.4byte 0xfffc0000
	.4byte 0x0300021c
	.4byte 0x03000230
	.2byte 0xebc8
	.2byte 0x0802
.L_080245e0:
	ldr	r2, [r7, #64]
	ldr	r3, [sp, #16]
	cmp	r3, r2
	beq.n	.L_080245f6
	ldr	r3, [r7, #16]
	ldr	r4, [sp, #16]
.L_080245ec:
	subs	r3, r3, r2
	subs	r2, r4, r2
	eors	r3, r2
	cmp	r3, #0
	bge.n	.L_080245fa
.L_080245f6:
	movs	r6, #1
	str	r6, [sp, #8]
.L_080245fa:
	ldr	r0, [sp, #8]
	cmp	r0, #0
	beq.n	.L_08024630
	ldr	r2, [sp, #0]
	ldrb	r3, [r2, #3]
	cmp	r3, #0
	beq.n	.L_08024622
	ldr	r3, [r7, #56]
	str	r3, [sp, #24]
	ldr	r4, [r7, #64]
	movs	r3, #0
	str	r3, [r7, #36]
	str	r4, [sp, #16]
	str	r3, [r7, #44]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_08024622
	ldr	r6, [r7, #60]
	str	r6, [sp, #20]
	str	r3, [r7, #40]
.L_08024622:
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #60]
	str	r3, [r7, #64]
	movs	r3, #0
	strb	r3, [r1, #0]
.L_08024630:
	ldr	r0, [sp, #24]
	ldr	r3, [r7, #8]
	subs	r3, r0, r3
	str	r3, [r7, #112]
	ldr	r1, [sp, #20]
	ldr	r3, [r7, #12]
	subs	r3, r1, r3
	str	r3, [r7, #116]
	ldr	r2, [sp, #16]
	ldr	r3, [r7, #16]
	str	r0, [r7, #8]
	subs	r3, r2, r3
	str	r3, [r7, #120]
	str	r1, [r7, #12]
	str	r2, [r7, #16]
	ldr	r3, [sp, #0]
	ldrb	r2, [r3, #5]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08024690
	ldr	r4, [r7, #36]
	str	r4, [sp, #24]
	ldr	r6, [r7, #44]
	str	r6, [sp, #16]
	cmp	r4, #0
	bne.n	.L_0802466a
	cmp	r6, #0
	beq.n	.L_08024690
.L_0802466a:
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #24]
	bl	sub_080148e8
	ldrh	r3, [r7, #6]
	movs	r2, #128
	subs	r0, r0, r3
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	lsls	r2, r2, #5
	cmp	r0, r2
	ble.n	.L_08024684
	adds	r0, r2, #0
.L_08024684:
	ldr	r2, [pc, #44]
	cmp	r0, r2
	bge.n	.L_0802468c
	adds	r0, r2, #0
.L_0802468c:
	adds	r3, r3, r0
	strh	r3, [r7, #6]
.L_08024690:
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #0]
	subs	r0, #1
	adds	r1, #128
	str	r0, [sp, #28]
	str	r1, [sp, #0]
	adds	r7, #128
	cmp	r0, #0
	blt.n	.L_080246a4
	b.n	.L_08023fbc
.L_080246a4:
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xf000
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #20]
	movs	r7, #63
.L_080246c2:
	ldr	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_08024712
	adds	r3, r1, #0
	adds	r3, #35
	ldrb	r2, [r3, #0]
	movs	r3, #64
	ands	r3, r2
	ldr	r5, [r1, #8]
	ldr	r4, [r1, #12]
	ldr	r6, [r1, #16]
	cmp	r3, #0
	beq.n	.L_0802470c
	ldr	r0, [r1, #124]
	cmp	r0, #0
	beq.n	.L_0802470c
	ldr	r2, [r0, #116]
	cmp	r2, #0
	beq.n	.L_08024704
	ldr	r3, [r1, #20]
	cmp	r4, r3
	bne.n	.L_080246fc
	ldr	r3, [r0, #12]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r1, #20]
	adds	r4, r3, #0
	b.n	.L_08024704
.L_080246fc:
	adds	r3, r3, r2
	str	r3, [r1, #20]
	ldr	r3, [r0, #116]
	adds	r4, r4, r3
.L_08024704:
	ldr	r3, [r0, #112]
	adds	r5, r5, r3
	ldr	r3, [r0, #120]
	adds	r6, r6, r3
.L_0802470c:
	str	r5, [r1, #8]
	str	r4, [r1, #12]
	str	r6, [r1, #16]
.L_08024712:
	subs	r7, #1
	adds	r1, #128
	cmp	r7, #0
	bge.n	.L_080246c2
	pop	{r5, r6, r7, pc}
