.syntax unified
	.thumb
	.set sub_080044d0, 0x080044d0
	.set sub_080045d4, 0x080045d4
	.set sub_080072ec, 0x080072ec
	.set sub_080072f0, 0x080072f0
	.set sub_08007310, 0x08007310
	.set sub_0800d924, 0x0800d924
	.set sub_08011f54, 0x08011f54
	.global Object_UpdateAllThumb
	.global Func_0800cacc
	.thumb_func
Object_UpdateAllThumb:
Func_0800cacc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #676]
	ldr	r6, [r3, #0]
	adds	r1, r6, #0
	adds	r2, r6, #0
	sub	sp, #48
	movs	r0, #63
	adds	r1, #85
	adds	r2, #86
	str	r0, [sp, #32]
	str	r1, [sp, #4]
	str	r2, [sp, #0]
.L_0800caf0:
	movs	r3, #0
	str	r3, [sp, #12]
	ldr	r2, [r6, #0]
	cmp	r2, #0
	bne.n	.L_0800cafc
	b.n	.L_0800d0fe
.L_0800cafc:
	ldr	r3, [r6, #108]
	cmp	r3, #0
	beq.n	.L_0800cb0a
	adds	r0, r6, #0
	bl	sub_080072f0
	ldr	r2, [r6, #0]
.L_0800cb0a:
	cmp	r2, #0
	bne.n	.L_0800cb10
	b.n	.L_0800d0fe
.L_0800cb10:
	ldr	r4, [sp, #4]
	ldrb	r3, [r4, #6]
	cmp	r3, #0
	beq.n	.L_0800cb1a
	b.n	.L_0800d0fe
.L_0800cb1a:
	ldr	r0, [sp, #0]
	movs	r4, #8
	ldrsh	r3, [r0, r4]
	ldrh	r1, [r0, #8]
	cmp	r3, #0
	beq.n	.L_0800cb2c
	subs	r3, r1, #1
	strh	r3, [r0, #8]
	b.n	.L_0800cb5a
.L_0800cb2c:
	ldr	r5, [pc, #596]
	b.n	.L_0800cb36
.L_0800cb30:
	adds	r3, r1, #1
	strh	r3, [r6, #4]
.L_0800cb34:
	ldr	r2, [r6, #0]
.L_0800cb36:
	movs	r4, #4
	ldrsh	r3, [r6, r4]
	lsls	r3, r3, #2
	ldr	r3, [r2, r3]
	ldrh	r1, [r6, #4]
	cmp	r3, #63
	bhi.n	.L_0800cb30
	lsls	r3, r3, #2
	ldr	r3, [r5, r3]
	adds	r0, r6, #0
	bl	sub_080072f0
	cmp	r0, #0
	bne.n	.L_0800cb34
	ldr	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L_0800cb5a
	b.n	.L_0800d0fe
.L_0800cb5a:
	ldr	r0, [r6, #8]
	str	r0, [sp, #28]
	ldr	r1, [r6, #12]
	str	r1, [sp, #24]
	ldr	r2, [r6, #16]
	ldr	r4, [sp, #4]
	str	r2, [sp, #20]
	ldrb	r3, [r4, #12]
	cmp	r3, #0
	beq.n	.L_0800cb70
	b.n	.L_0800cfd6
.L_0800cb70:
	str	r4, [sp, #8]
	ldrb	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L_0800cb7a
	b.n	.L_0800ccf8
.L_0800cb7a:
	movs	r0, #128
	ldr	r3, [r6, #56]
	lsls	r0, r0, #24
	cmp	r3, r0
	bne.n	.L_0800cb86
	b.n	.L_0800cc7c
.L_0800cb86:
	ldr	r1, [sp, #28]
	subs	r0, r3, r1
	cmp	r0, #0
	bge.n	.L_0800cb92
	ldr	r2, [pc, #504]
	adds	r0, r0, r2
.L_0800cb92:
	ldr	r3, [r6, #60]
	ldr	r4, [sp, #24]
	asrs	r0, r0, #16
	mov	r8, r0
	subs	r0, r3, r4
	cmp	r0, #0
	bge.n	.L_0800cba4
	ldr	r1, [pc, #484]
	adds	r0, r0, r1
.L_0800cba4:
	ldr	r3, [r6, #64]
	ldr	r2, [sp, #20]
	asrs	r0, r0, #16
	mov	fp, r0
	subs	r0, r3, r2
	cmp	r0, #0
	bge.n	.L_0800cbb6
	ldr	r3, [pc, #468]
	adds	r0, r0, r3
.L_0800cbb6:
	asrs	r0, r0, #16
	mov	r9, r0
	mov	r4, r8
	mov	r1, fp
	mov	r0, r8
	muls	r0, r4
	mov	r3, fp
	muls	r3, r1
	mov	r4, r9
	mov	r2, r9
	muls	r2, r4
	adds	r0, r0, r3
	adds	r0, r0, r2
	ldr	r3, [pc, #440]
	bl	sub_080072f0
	cmp	r0, #0
	bne.n	.L_0800cbe8
	ldr	r0, [r6, #56]
	str	r0, [sp, #28]
	ldr	r1, [r6, #60]
	str	r1, [sp, #24]
	ldr	r2, [r6, #64]
	str	r2, [sp, #20]
	b.n	.L_0800cfd6
.L_0800cbe8:
	ldr	r1, [r6, #52]
	ldr	r3, [pc, #420]
	lsls	r0, r0, #16
	bl	sub_080072f0
	adds	r5, r0, #0
	mov	r3, r8
	muls	r3, r5
	ldr	r0, [r6, #36]
	adds	r0, r0, r3
	mov	r3, fp
	muls	r3, r5
	mov	r2, r9
	muls	r2, r5
	ldr	r1, [r6, #40]
	adds	r1, r1, r3
	ldr	r3, [r6, #44]
	ldr	r4, [pc, #392]
	adds	r3, r3, r2
	mov	sl, r0
	str	r1, [r6, #40]
	adds	r7, r1, #0
	str	r0, [r6, #36]
	str	r3, [r6, #44]
	mov	r9, r3
	mov	r8, r4
	mov	r1, sl
	movs	r0, r0
	mov	ip, pc
	bx	r8
	adds	r3, r0, #0
	adds	r1, r7, #0
	adds	r0, r7, #0
	movs	r0, r0
	mov	ip, pc
	bx	r8
	adds	r4, r0, #0
	mov	r1, r9
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r8
	adds	r3, r3, r4
	adds	r3, r3, r0
	adds	r0, r3, #0
	bl	sub_080045d4
	ldr	r1, [r6, #48]
	cmp	r0, r1
	bgt.n	.L_0800cc4e
	b.n	.L_0800cfd6
.L_0800cc4e:
	ldr	r2, [pc, #320]
	bl	sub_080072ec
	adds	r5, r0, #0
	adds	r1, r5, #0
	mov	r0, sl
	movs	r0, r0
	mov	ip, pc
	bx	r8
	str	r0, [r6, #36]
	adds	r1, r5, #0
	adds	r0, r7, #0
	movs	r0, r0
	mov	ip, pc
	bx	r8
	str	r0, [r6, #40]
	adds	r1, r5, #0
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r8
	str	r0, [r6, #44]
	b.n	.L_0800cfd6
.L_0800cc7c:
	ldr	r3, [r6, #36]
	ldr	r0, [r6, #44]
	ldr	r4, [r6, #40]
	mov	r8, r3
	mov	r9, r0
	mov	fp, r4
	ldr	r7, [pc, #264]
	mov	r0, r8
	mov	r1, r8
	movs	r0, r0
	mov	ip, pc
	bx	r7
	adds	r3, r0, #0
	mov	r1, fp
	mov	r0, fp
	movs	r0, r0
	mov	ip, pc
	bx	r7
	adds	r4, r0, #0
	mov	r1, r9
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r7
	adds	r3, r3, r4
	adds	r3, r3, r0
	adds	r0, r3, #0
	bl	sub_080045d4
	cmp	r0, #0
	beq.n	.L_0800ccf0
	ldr	r3, [r6, #52]
	subs	r1, r0, r3
	cmp	r1, #0
	bge.n	.L_0800ccc4
	movs	r1, #0
.L_0800ccc4:
	ldr	r3, [pc, #200]
	bl	sub_080072f0
	adds	r5, r0, #0
	adds	r1, r5, #0
	mov	r0, r8
	mov	ip, pc
	bx	r7
	str	r0, [r6, #36]
	adds	r1, r5, #0
	mov	r0, fp
	movs	r0, r0
	mov	ip, pc
	bx	r7
	str	r0, [r6, #40]
	adds	r1, r5, #0
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r7
	str	r0, [r6, #44]
	b.n	.L_0800cfd6
.L_0800ccf0:
	str	r0, [r6, #36]
	str	r0, [r6, #40]
	str	r0, [r6, #44]
	b.n	.L_0800cfd6
.L_0800ccf8:
	movs	r1, #128
	ldr	r3, [r6, #56]
	lsls	r1, r1, #24
	cmp	r3, r1
	bne.n	.L_0800cd04
	b.n	.L_0800ce0e
.L_0800cd04:
	ldr	r2, [sp, #28]
	subs	r0, r3, r2
	cmp	r0, #0
	bge.n	.L_0800cd10
	ldr	r3, [pc, #120]
	adds	r0, r0, r3
.L_0800cd10:
	ldr	r3, [r6, #64]
	ldr	r4, [sp, #20]
	asrs	r0, r0, #16
	mov	r8, r0
	subs	r0, r3, r4
	cmp	r0, #0
	bge.n	.L_0800cd22
	ldr	r1, [pc, #104]
	adds	r0, r0, r1
.L_0800cd22:
	asrs	r0, r0, #16
	mov	r9, r0
	mov	r2, r8
	mov	r4, r9
	mov	r3, r9
	muls	r3, r4
	mov	r0, r8
	muls	r0, r2
	adds	r0, r0, r3
	ldr	r3, [pc, #84]
	bl	sub_080072f0
	ldr	r1, [pc, #92]
	lsls	r0, r0, #16
	cmp	r0, r1
	bgt.n	.L_0800cd70
	ldr	r3, [r6, #56]
	ldr	r2, [sp, #28]
	ldr	r4, [sp, #20]
	subs	r2, r3, r2
	ldr	r3, [r6, #64]
	mov	r8, r2
	subs	r4, r3, r4
	mov	r9, r4
	mov	r0, r8
	ldr	r4, [pc, #60]
	mov	r1, r8
	mov	ip, pc
	bx	r4
	adds	r3, r0, #0
	mov	r1, r9
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r4
	adds	r3, r3, r0
	adds	r0, r3, #0
	bl	sub_080045d4
.L_0800cd70:
	cmp	r0, #0
	bne.n	.L_0800cd9c
	ldr	r0, [r6, #56]
	str	r0, [sp, #28]
	ldr	r1, [r6, #64]
	str	r1, [sp, #20]
	b.n	.L_0800ce66
	movs	r0, r0
	.4byte 0x03001e64
	.4byte 0x08013624
	.4byte 0x0000ffff
	.4byte 0x030001d8
	.4byte 0x0300013c
	.4byte 0x03000118
	.2byte 0xffff
	.2byte 0x00ff
.L_0800cd9c:
	ldr	r2, [pc, #688]
	ldr	r1, [r6, #52]
	mov	fp, r2
	bl	sub_08007310
	adds	r5, r0, #0
	ldr	r7, [pc, #680]
	mov	r0, r8
	adds	r1, r5, #0
	movs	r0, r0
	mov	ip, pc
	bx	r7
	ldr	r4, [r6, #36]
	adds	r4, r4, r0
	str	r4, [r6, #36]
	mov	r0, r9
	adds	r1, r5, #0
	movs	r0, r0
	mov	ip, pc
	bx	r7
	ldr	r3, [r6, #44]
	mov	sl, r4
	adds	r3, r3, r0
	str	r3, [r6, #44]
	mov	r9, r3
	mov	r0, sl
	mov	r1, sl
	movs	r0, r0
	mov	ip, pc
	bx	r7
	adds	r3, r0, #0
	mov	r1, r9
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r7
	adds	r3, r3, r0
	adds	r0, r3, #0
	bl	sub_080045d4
	ldr	r1, [r6, #48]
	cmp	r0, r1
	ble.n	.L_0800ce66
	bl	sub_08007310
	adds	r5, r0, #0
	adds	r1, r5, #0
	mov	r0, sl
	mov	ip, pc
	bx	r7
	str	r0, [r6, #36]
	adds	r1, r5, #0
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r7
	b.n	.L_0800ce64
.L_0800ce0e:
	ldr	r3, [r6, #36]
	ldr	r4, [r6, #44]
	mov	r8, r3
	mov	r9, r4
	ldr	r7, [pc, #572]
	mov	r0, r8
	mov	r1, r8
	mov	ip, pc
	bx	r7
	adds	r3, r0, #0
	mov	r1, r9
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r7
	adds	r3, r3, r0
	adds	r0, r3, #0
	bl	sub_080045d4
	cmp	r0, #0
	beq.n	.L_0800ce62
	ldr	r3, [r6, #52]
	subs	r1, r0, r3
	cmp	r1, #0
	bge.n	.L_0800ce42
	movs	r1, #0
.L_0800ce42:
	ldr	r3, [pc, #524]
	bl	sub_080072f0
	adds	r5, r0, #0
	adds	r1, r5, #0
	mov	r0, r8
	movs	r0, r0
	mov	ip, pc
	bx	r7
	str	r0, [r6, #36]
	adds	r1, r5, #0
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r7
	b.n	.L_0800ce64
.L_0800ce62:
	str	r0, [r6, #36]
.L_0800ce64:
	str	r0, [r6, #44]
.L_0800ce66:
	ldr	r0, [sp, #8]
	ldrb	r2, [r0, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0800ce74
	b.n	.L_0800cf42
.L_0800ce74:
	ldr	r3, [r6, #36]
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #20]
	adds	r1, r1, r3
	ldr	r3, [r6, #44]
	adds	r2, r2, r3
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	mov	sl, r1
	mov	r9, r2
	bl	sub_08011f54
	str	r0, [sp, #16]
	ldr	r4, [sp, #24]
	ldr	r3, [r6, #20]
	subs	r7, r0, r3
	subs	r3, r0, r4
	ldr	r0, [pc, #444]
	cmp	r3, r0
	ble.n	.L_0800cea2
	adds	r4, r4, r7
	str	r4, [sp, #24]
.L_0800cea2:
	cmp	r7, #0
	bge.n	.L_0800cea8
	negs	r7, r7
.L_0800cea8:
	ldr	r3, [r6, #52]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	cmp	r7, r3
	ble.n	.L_0800ceb6
	adds	r7, r3, #0
.L_0800ceb6:
	lsls	r3, r7, #1
	adds	r7, r3, r7
	cmp	r7, #0
	beq.n	.L_0800cf3e
	ldr	r1, [sp, #8]
	ldrb	r2, [r1, #0]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0800cf3e
	ldr	r2, [r6, #36]
	ldr	r0, [pc, #388]
	ldr	r3, [r6, #40]
	ldr	r4, [r6, #44]
	mov	r8, r2
	mov	sl, r0
	mov	fp, r3
	mov	r9, r4
	mov	r0, r8
	mov	r1, r8
	movs	r0, r0
	mov	ip, pc
	bx	sl
	adds	r3, r0, #0
	mov	r1, fp
	mov	r0, fp
	movs	r0, r0
	mov	ip, pc
	bx	sl
	adds	r4, r0, #0
	mov	r1, r9
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	sl
	adds	r3, r3, r4
	adds	r3, r3, r0
	adds	r0, r3, #0
	bl	sub_080045d4
	cmp	r0, #0
	beq.n	.L_0800cf3e
	subs	r1, r0, r7
	cmp	r1, #0
	bge.n	.L_0800cf12
	movs	r1, #0
.L_0800cf12:
	ldr	r3, [pc, #316]
	bl	sub_080072f0
	adds	r5, r0, #0
	adds	r1, r5, #0
	mov	r0, r8
	movs	r0, r0
	mov	ip, pc
	bx	sl
	str	r0, [r6, #36]
	adds	r1, r5, #0
	mov	r0, fp
	movs	r0, r0
	mov	ip, pc
	bx	sl
	str	r0, [r6, #40]
	adds	r1, r5, #0
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	sl
	str	r0, [r6, #44]
.L_0800cf3e:
	ldr	r1, [sp, #16]
	str	r1, [r6, #20]
.L_0800cf42:
	ldr	r3, [sp, #8]
	ldrb	r2, [r3, #0]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0800cf8a
	ldr	r4, [r6, #20]
	ldr	r0, [sp, #24]
	str	r4, [sp, #16]
	cmp	r0, r4
	ble.n	.L_0800cf60
	ldr	r3, [r6, #40]
	ldr	r2, [r6, #72]
	subs	r3, r3, r2
	b.n	.L_0800cf88
.L_0800cf60:
	ldr	r0, [r6, #40]
	cmp	r0, #0
	bge.n	.L_0800cf8a
	ldr	r1, [sp, #16]
	str	r1, [sp, #24]
	ldr	r3, [pc, #232]
	ldr	r1, [r6, #68]
	movs	r0, r0
	mov	ip, pc
	bx	r3
	negs	r3, r0
	adds	r2, r3, #0
	str	r3, [r6, #40]
	cmp	r2, #0
	bge.n	.L_0800cf80
	adds	r2, r0, #0
.L_0800cf80:
	ldr	r3, [r6, #72]
	cmp	r2, r3
	bgt.n	.L_0800cf8a
	movs	r3, #0
.L_0800cf88:
	str	r3, [r6, #40]
.L_0800cf8a:
	ldr	r3, [sp, #8]
	ldrb	r2, [r3, #0]
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0800cfd6
	ldr	r1, [r6, #68]
	movs	r3, #63
	ands	r1, r3
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0800cfba
	ldr	r2, [pc, #180]
	lsrs	r3, r1, #1
	lsls	r3, r3, #2
	ldr	r2, [r2, r3]
	ldr	r3, [r6, #72]
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L_0800cfb6
	adds	r3, #15
.L_0800cfb6:
	asrs	r3, r3, #4
	b.n	.L_0800cfce
.L_0800cfba:
	ldr	r2, [pc, #160]
	lsrs	r3, r1, #1
	lsls	r3, r3, #2
	ldr	r2, [r2, r3]
	ldr	r3, [r6, #72]
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L_0800cfcc
	adds	r3, #63
.L_0800cfcc:
	asrs	r3, r3, #6
.L_0800cfce:
	str	r3, [r6, #40]
	ldr	r3, [r6, #68]
	adds	r3, #1
	str	r3, [r6, #68]
.L_0800cfd6:
	ldr	r3, [r6, #36]
	ldr	r4, [sp, #28]
	adds	r4, r4, r3
	str	r4, [sp, #28]
	ldr	r0, [sp, #24]
	ldr	r3, [r6, #40]
	adds	r0, r0, r3
	str	r0, [sp, #24]
	ldr	r1, [sp, #20]
	ldr	r3, [r6, #44]
	adds	r1, r1, r3
	str	r1, [sp, #20]
	ldr	r3, [sp, #4]
	ldrb	r2, [r3, #4]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0800d01c
	add	r1, sp, #36
	str	r0, [r1, #4]
	str	r4, [r1, #0]
	ldr	r4, [sp, #20]
	adds	r0, r6, #0
	str	r4, [r1, #8]
	bl	sub_0800d924
	cmp	r0, #0
	beq.n	.L_0800d018
	ldr	r0, [sp, #4]
	ldrb	r3, [r0, #11]
	adds	r3, #1
	strb	r3, [r0, #11]
	b.n	.L_0800d0fe
.L_0800d018:
	ldr	r1, [sp, #4]
	strb	r0, [r1, #11]
.L_0800d01c:
	ldr	r1, [sp, #0]
	ldrb	r3, [r1, #0]
	cmp	r3, #17
	beq.n	.L_0800d042
	cmp	r3, #17
	bgt.n	.L_0800d02e
	cmp	r3, #16
	beq.n	.L_0800d034
	b.n	.L_0800d07a
.L_0800d02e:
	cmp	r3, #18
	beq.n	.L_0800d060
	b.n	.L_0800d07a
.L_0800d034:
	ldr	r2, [r6, #56]
	ldr	r3, [sp, #28]
	cmp	r3, r2
	beq.n	.L_0800d076
	ldr	r3, [r6, #8]
	ldr	r4, [sp, #28]
	b.n	.L_0800d06c
.L_0800d042:
	ldr	r2, [r6, #60]
	ldr	r3, [sp, #24]
	cmp	r3, r2
	beq.n	.L_0800d076
	ldr	r3, [r6, #12]
	ldr	r4, [sp, #24]
	b.n	.L_0800d06c
	.4byte 0x0300013c
	.4byte 0x03000118
	.4byte 0xfffc0000
	.2byte 0x31c0
	.2byte 0x0801
.L_0800d060:
	ldr	r2, [r6, #64]
	ldr	r3, [sp, #20]
	cmp	r3, r2
	beq.n	.L_0800d076
	ldr	r3, [r6, #16]
	ldr	r4, [sp, #20]
.L_0800d06c:
	subs	r3, r3, r2
	subs	r2, r4, r2
	eors	r3, r2
	cmp	r3, #0
	bge.n	.L_0800d07a
.L_0800d076:
	movs	r0, #1
	str	r0, [sp, #12]
.L_0800d07a:
	ldr	r2, [sp, #12]
	cmp	r2, #0
	beq.n	.L_0800d0b0
	ldr	r4, [sp, #4]
	ldrb	r3, [r4, #3]
	cmp	r3, #0
	beq.n	.L_0800d0a2
	ldr	r0, [r6, #56]
	str	r0, [sp, #28]
	ldr	r2, [r6, #64]
	movs	r3, #0
	str	r3, [r6, #36]
	str	r2, [sp, #20]
	str	r3, [r6, #44]
	ldrb	r3, [r4, #0]
	cmp	r3, #0
	bne.n	.L_0800d0a2
	ldr	r4, [r6, #60]
	str	r4, [sp, #24]
	str	r3, [r6, #40]
.L_0800d0a2:
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r6, #56]
	str	r3, [r6, #60]
	str	r3, [r6, #64]
	movs	r3, #0
	strb	r3, [r1, #0]
.L_0800d0b0:
	ldr	r0, [sp, #28]
	str	r0, [r6, #8]
	ldr	r1, [sp, #24]
	str	r1, [r6, #12]
	ldr	r2, [sp, #20]
	str	r2, [r6, #16]
	ldr	r3, [sp, #4]
	ldrb	r2, [r3, #5]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0800d0fe
	ldr	r4, [r6, #36]
	str	r4, [sp, #28]
	ldr	r0, [r6, #44]
	str	r0, [sp, #20]
	cmp	r4, #0
	bne.n	.L_0800d0d8
	cmp	r0, #0
	beq.n	.L_0800d0fe
.L_0800d0d8:
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #28]
	bl	sub_080044d0
	ldrh	r3, [r6, #6]
	subs	r0, r0, r3
	lsls	r0, r0, #16
	movs	r2, #128
	asrs	r0, r0, #16
	lsls	r2, r2, #5
	cmp	r0, r2
	ble.n	.L_0800d0f2
	adds	r0, r2, #0
.L_0800d0f2:
	ldr	r2, [pc, #56]
	cmp	r0, r2
	bge.n	.L_0800d0fa
	adds	r0, r2, #0
.L_0800d0fa:
	adds	r3, r3, r0
	strh	r3, [r6, #6]
.L_0800d0fe:
	ldr	r1, [sp, #32]
	ldr	r2, [sp, #4]
	ldr	r3, [sp, #0]
	subs	r1, #1
	adds	r2, #112
	adds	r3, #112
	str	r1, [sp, #32]
	str	r2, [sp, #4]
	str	r3, [sp, #0]
	adds	r6, #112
	cmp	r1, #0
	blt.n	.L_0800d118
	b.n	.L_0800caf0
.L_0800d118:
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0xfffff000
