.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_08002304, 0x08002304
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080072fc, 0x080072fc
	.set sub_08007304, 0x08007304
	.set sub_0800730c, 0x0800730c
	.set sub_08009008, 0x08009008
	.set sub_08009020, 0x08009020
	.set sub_08009030, 0x08009030
	.set sub_08009038, 0x08009038
	.set sub_080b5078, 0x080b5078
	.set sub_080b5088, 0x080b5088
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080de2f8, 0x080de2f8
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3944, 0x080e3944
	.set sub_080e396c, 0x080e396c
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Func_080e2974
	.thumb_func
Func_080e2974:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #140
	ldr	r2, [pc, #96]
	str	r1, [sp, #84]
	adds	r3, r2, #0
	adds	r6, r0, #0
	ldmia	r3!, {r0}
	str	r0, [sp, #80]
	ldr	r3, [r3, #0]
	str	r3, [sp, #76]
	ldr	r1, [r2, #8]
	str	r1, [sp, #68]
	subs	r2, #108
	ldr	r2, [r2, #0]
	str	r2, [sp, #64]
	ldr	r2, [pc, #76]
	adds	r5, r0, r2
	str	r6, [r5, #0]
	movs	r0, #0
	bl	sub_080cd594
	ldr	r3, [pc, #56]
	ldr	r2, [pc, #64]
	ldr	r5, [r5, #0]
	strh	r3, [r2, #0]
	ldr	r3, [r5, #28]
	cmp	r3, #1
	bne.n	.L_080e29cc
	add	r3, sp, #100
	ldr	r2, [r5, #4]
	str	r3, [sp, #0]
	add	r3, sp, #96
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r1, #7
	movs	r3, #2
	bl	sub_080de2f8
.L_080e29cc:
	ldr	r1, [sp, #68]
	ldr	r0, [pc, #36]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	movs	r3, #0
	ldr	r0, [pc, #28]
	ldr	r1, [sp, #80]
	movs	r2, #1
	b.n	.L_080e29fc
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x00000073
	.2byte 0x0099
	.2byte 0x0000
.L_080e29fc:
	bl	sub_080e0524
	movs	r3, #162
	movs	r4, #144
	lsls	r3, r3, #7
	lsls	r4, r4, #1
	movs	r5, #0
	mov	ip, r3
	mov	lr, r4
	movs	r7, #0
	movs	r6, #0
.L_080e2a12:
	adds	r3, r6, r5
	ldr	r2, [sp, #80]
	lsls	r3, r3, #3
	movs	r0, #0
	mov	r8, r7
	adds	r1, r3, r2
.L_080e2a1e:
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	ldrb	r2, [r1, #0]
	add	r3, r8
	ldr	r4, [sp, #80]
	add	r3, ip
	adds	r0, #1
	adds	r1, #1
	strb	r2, [r4, r3]
	cmp	r0, #40
	bne.n	.L_080e2a1e
	adds	r5, #1
	adds	r7, #20
	adds	r6, #4
	cmp	r5, lr
	bne.n	.L_080e2a12
	ldr	r5, [sp, #84]
	ldr	r0, [sp, #84]
	lsls	r5, r5, #3
	ldr	r3, [pc, #900]
	str	r5, [sp, #60]
	subs	r2, r5, r0
	ldrb	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_080e2a60
	ldr	r0, [pc, #892]
	ldr	r1, [sp, #80]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	b.n	.L_080e2a6c
.L_080e2a60:
	ldr	r0, [pc, #880]
	ldr	r1, [sp, #80]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
.L_080e2a6c:
	ldr	r1, [pc, #872]
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #872]
	bl	sub_080e0524
	ldr	r1, [sp, #60]
	ldr	r4, [sp, #84]
	ldr	r2, [pc, #844]
	subs	r3, r1, r4
	adds	r3, #1
	ldrb	r3, [r2, r3]
	cmp	r3, #1
	beq.n	.L_080e2a9c
	cmp	r3, #1
	bgt.n	.L_080e2a92
	cmp	r3, #0
	beq.n	.L_080e2a98
	b.n	.L_080e2aa4
.L_080e2a92:
	cmp	r3, #2
	beq.n	.L_080e2aa0
	b.n	.L_080e2aa4
.L_080e2a98:
	ldr	r0, [pc, #836]
	b.n	.L_080e2aa6
.L_080e2a9c:
	ldr	r0, [pc, #836]
	b.n	.L_080e2aa6
.L_080e2aa0:
	ldr	r0, [pc, #816]
	b.n	.L_080e2aa6
.L_080e2aa4:
	ldr	r0, [pc, #832]
.L_080e2aa6:
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	movs	r2, #128
	ldr	r3, [pc, #824]
	lsls	r0, r0, #19
	bl	sub_080072f0
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [pc, #816]
	ldr	r0, [sp, #80]
	adds	r5, r0, r1
	ldr	r3, [r5, #0]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	mov	r3, sp
	adds	r3, #116
	adds	r1, r3, #0
	str	r3, [sp, #56]
	bl	sub_080e396c
	movs	r0, #239
	ldr	r4, [sp, #80]
	lsls	r0, r0, #7
	ldr	r1, [pc, #788]
	adds	r2, r4, r0
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r4, r1
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #776]
	bl	sub_080041d8
	ldr	r3, [r5, #0]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_080b5098
	ldr	r5, [pc, #764]
	movs	r3, #0
	ldr	r6, [r0, #0]
	mov	r8, r3
	movs	r7, #255
.L_080e2b08:
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	movs	r4, #200
	ldr	r3, [r6, #12]
	lsls	r4, r4, #13
	adds	r3, r3, r4
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_08004458
	ands	r0, r7
	lsls	r0, r0, #12
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #127
	lsls	r0, r0, #12
	str	r0, [r5, #16]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #127
	ldr	r3, [r5, #0]
	lsls	r0, r0, #12
	str	r0, [r5, #20]
	cmp	r3, #0
	ble.n	.L_080e2b48
	ldr	r3, [r5, #12]
	negs	r3, r3
	str	r3, [r5, #12]
.L_080e2b48:
	movs	r3, #1
	movs	r0, #1
	movs	r1, #192
	negs	r3, r3
	add	r8, r0
	lsls	r1, r1, #2
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r8, r1
	bne.n	.L_080e2b08
	ldr	r2, [sp, #80]
	ldr	r4, [pc, #656]
	adds	r3, r2, r4
	ldr	r3, [r3, #0]
	movs	r2, #4
	ldr	r0, [r3, #8]
	movs	r5, #36
	ldrsh	r1, [r3, r5]
	movs	r3, #0
	bl	sub_080b5078
	ldr	r2, [sp, #60]
	ldr	r4, [sp, #84]
	movs	r0, #0
	str	r0, [sp, #72]
	ldr	r1, [pc, #592]
	subs	r3, r2, r4
	adds	r3, #5
	ldrb	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L_080e2b88
	b.n	.L_080e2fe6
.L_080e2b88:
	mov	r5, sp
	adds	r5, #128
	mov	r0, sp
	adds	r0, #88
	str	r5, [sp, #24]
	ldr	r3, [sp, #64]
	ldr	r5, [pc, #600]
	ldr	r4, [sp, #80]
	str	r0, [sp, #28]
	ldr	r0, [pc, #612]
	adds	r3, #12
	adds	r5, r4, r5
	str	r3, [sp, #20]
	str	r5, [sp, #48]
	str	r0, [sp, #44]
.L_080e2ba6:
	ldr	r4, [sp, #84]
	subs	r3, r2, r4
	adds	r3, #2
	ldrb	r1, [r1, r3]
	ldr	r5, [sp, #48]
	str	r1, [sp, #52]
	ldr	r3, [r5, #0]
	ldr	r1, [sp, #24]
	ldr	r0, [r3, #8]
	bl	sub_080e396c
	ldr	r0, [sp, #24]
	ldr	r3, [r0, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r0, #0]
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080e2bf2
	movs	r1, #2
	str	r1, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #3
	movs	r0, #46
	bl	sub_080ed408
	movs	r2, #2
	str	r2, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #11
	bl	sub_080ed408
	b.n	.L_080e2c12
.L_080e2bf2:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #46
	bl	sub_080ed408
	movs	r4, #2
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #15
	str	r4, [sp, #0]
	bl	sub_080ed408
.L_080e2c12:
	ldr	r3, [sp, #44]
	adds	r3, #184
	ldr	r3, [r3, #0]
	ldr	r5, [pc, #488]
	str	r3, [sp, #88]
	ldr	r0, [sp, #28]
	ldr	r3, [r5, #0]
	str	r3, [r0, #4]
	ldr	r2, [sp, #52]
	movs	r1, #0
	mov	r8, r1
	cmp	r2, #0
	bne.n	.L_080e2c2e
	b.n	.L_080e2ed6
.L_080e2c2e:
	ldr	r3, [sp, #60]
	ldr	r4, [sp, #84]
	subs	r3, r3, r4
	str	r3, [sp, #40]
	ldr	r0, [sp, #40]
	ldr	r5, [sp, #56]
	adds	r3, #6
	adds	r0, #3
	str	r3, [sp, #36]
	str	r0, [sp, #32]
	str	r1, [sp, #16]
	mov	fp, r5
.L_080e2c46:
	ldr	r3, [sp, #40]
	ldr	r1, [pc, #384]
	adds	r3, #4
	ldrb	r3, [r1, r3]
	mov	r2, r8
	muls	r2, r3
	ldr	r3, [sp, #72]
	mov	sl, r2
	cmp	r3, sl
	bge.n	.L_080e2c5c
	b.n	.L_080e2d66
.L_080e2c5c:
	mov	r3, sl
	ldr	r4, [sp, #72]
	adds	r3, #6
	cmp	r4, r3
	bge.n	.L_080e2d66
	movs	r3, #3
	mov	r0, r8
	mov	r5, sl
	ands	r3, r0
	subs	r2, r4, r5
	cmp	r3, #1
	ble.n	.L_080e2c7c
	ldr	r4, [sp, #40]
	ldrb	r3, [r1, r4]
	cmp	r3, #1
	bne.n	.L_080e2cee
.L_080e2c7c:
	ldr	r5, [sp, #48]
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080e2cba
	lsls	r1, r2, #3
	subs	r1, r1, r2
	lsls	r1, r1, #2
	subs	r1, r1, r2
	ldr	r2, [sp, #80]
	mov	r3, fp
	lsls	r1, r1, #7
	adds	r1, r2, r1
	ldr	r2, [r3, #0]
	movs	r0, #1
	mov	r4, r8
	ands	r4, r0
	lsrs	r3, r2, #31
	mov	r5, fp
	movs	r0, #48
	adds	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	movs	r0, #72
	str	r0, [sp, #4]
	ldr	r5, [sp, #28]
	lsls	r4, r4, #2
	asrs	r2, r2, #1
	subs	r2, #16
	subs	r3, #40
	b.n	.L_080e2d26
.L_080e2cba:
	lsls	r1, r2, #3
	subs	r1, r1, r2
	lsls	r1, r1, #2
	subs	r1, r1, r2
	ldr	r2, [sp, #80]
	mov	r3, fp
	lsls	r1, r1, #7
	adds	r1, r2, r1
	ldr	r2, [r3, #0]
	movs	r0, #1
	mov	r4, r8
	ands	r4, r0
	lsrs	r3, r2, #31
	mov	r5, fp
	movs	r0, #48
	adds	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	movs	r0, #72
	str	r0, [sp, #4]
	ldr	r5, [sp, #28]
	lsls	r4, r4, #2
	asrs	r2, r2, #1
	subs	r2, #32
	subs	r3, #40
	b.n	.L_080e2d26
.L_080e2cee:
	ldr	r0, [sp, #48]
	ldr	r3, [r0, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080e2d30
	movs	r1, #1
	mov	r4, r8
	ands	r4, r1
	lsls	r1, r2, #1
	adds	r1, r1, r2
	ldr	r3, [sp, #56]
	ldr	r2, [pc, #208]
	lsls	r1, r1, #8
	adds	r1, r1, r2
	ldr	r2, [r3, #0]
	ldr	r5, [sp, #24]
	lsrs	r3, r2, #31
	movs	r0, #48
	adds	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	movs	r0, #16
	str	r0, [sp, #4]
	ldr	r5, [sp, #28]
	lsls	r4, r4, #2
	asrs	r2, r2, #1
	subs	r2, #16
	subs	r3, #8
.L_080e2d26:
	ldr	r4, [r4, r5]
	ldr	r0, [sp, #76]
	bl	sub_080072f4
	b.n	.L_080e2d66
.L_080e2d30:
	lsls	r1, r2, #1
	adds	r1, r1, r2
	ldr	r3, [sp, #56]
	ldr	r2, [pc, #160]
	lsls	r1, r1, #8
	adds	r1, r1, r2
	ldr	r2, [r3, #0]
	movs	r0, #1
	ldr	r5, [sp, #24]
	mov	r4, r8
	ands	r4, r0
	lsrs	r3, r2, #31
	movs	r0, #48
	adds	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	movs	r0, #16
	str	r0, [sp, #4]
	ldr	r5, [sp, #28]
	lsls	r4, r4, #2
	asrs	r2, r2, #1
	subs	r2, #32
	subs	r3, #8
	ldr	r4, [r4, r5]
	ldr	r0, [sp, #76]
	bl	sub_080072f4
.L_080e2d66:
	movs	r0, #2
	add	r0, sl
	ldr	r1, [sp, #72]
	mov	r9, r0
	cmp	r1, r9
	beq.n	.L_080e2d74
	b.n	.L_080e2e72
.L_080e2d74:
	ldr	r2, [pc, #84]
	ldr	r4, [sp, #36]
	ldrb	r3, [r2, r4]
	cmp	r3, #1
	bne.n	.L_080e2d8c
	movs	r1, #128
	ldr	r3, [pc, #132]
	ldr	r0, [sp, #76]
	lsls	r1, r1, #7
	ldr	r2, [pc, #132]
	bl	sub_080072f0
.L_080e2d8c:
	ldr	r0, [sp, #80]
	ldr	r1, [pc, #96]
	adds	r5, r0, r1
	ldr	r3, [r5, #0]
	movs	r6, #4
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r1, #7
	movs	r3, #0
	movs	r2, #5
	str	r6, [sp, #0]
	bl	sub_080d6888
	ldr	r3, [sp, #52]
	subs	r3, #1
	cmp	r8, r3
	bne.n	.L_080e2e14
	ldr	r3, [r5, #0]
	movs	r1, #4
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	bl	sub_080b5088
	ldr	r0, [pc, #84]
	ldr	r5, [sp, #80]
	movs	r3, #8
	adds	r2, r5, r0
	str	r3, [r2, #0]
	movs	r0, #134
	bl	sub_080b50e8
	b.n	.L_080e2e38
	.4byte 0x080eed3e
	.4byte 0x000000b5
	.4byte 0x000000b6
	.4byte 0x02015e00
	.4byte 0x0000006b
	.4byte 0x0000008d
	.4byte 0x000000a0
	.4byte 0x000000b4
	.4byte 0x03001388
	.4byte 0x00007828
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x02010000
	.4byte 0x03001e50
	.4byte 0x03001f0c
	.4byte 0x03000168
	.4byte 0x2f2f2f2f
	.2byte 0x77a8
	.2byte 0x0000
.L_080e2e14:
	mov	r3, r8
	movs	r1, #1
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080e2e2a
	ldr	r3, [r5, #0]
	movs	r1, #7
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_080b5088
.L_080e2e2a:
	ldr	r4, [sp, #80]
	ldr	r5, [pc, #468]
	adds	r3, r4, r5
	str	r6, [r3, #0]
	movs	r0, #134
	bl	sub_080f9010
.L_080e2e38:
	ldr	r0, [pc, #460]
	ldr	r1, [sp, #32]
	ldrb	r3, [r0, r1]
	movs	r6, #0
	cmp	r3, #0
	beq.n	.L_080e2e72
	ldr	r2, [sp, #60]
	ldr	r4, [sp, #84]
	adds	r1, r0, #0
	subs	r3, r2, r4
	ldr	r0, [sp, #16]
	ldr	r2, [pc, #444]
	adds	r7, r3, #3
	adds	r5, r0, r2
	movs	r2, #7
.L_080e2e56:
	str	r1, [sp, #12]
	str	r2, [sp, #8]
	bl	sub_08004458
	ldr	r2, [sp, #8]
	ldr	r1, [sp, #12]
	ands	r0, r2
	ldrb	r3, [r1, r7]
	adds	r0, #15
	adds	r6, #1
	str	r0, [r5, #0]
	adds	r5, #28
	cmp	r6, r3
	bne.n	.L_080e2e56
.L_080e2e72:
	ldr	r3, [sp, #72]
	cmp	r3, r9
	blt.n	.L_080e2ec0
	mov	r3, sl
	ldr	r4, [sp, #72]
	adds	r3, #14
	cmp	r4, r3
	bge.n	.L_080e2ec0
	mov	r5, sl
	subs	r3, r4, r5
	subs	r3, #2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r1, r3, #4
	subs	r1, r1, r3
	ldr	r0, [sp, #80]
	lsls	r1, r1, #6
	movs	r2, #162
	adds	r1, r0, r1
	lsls	r2, r2, #7
	mov	r3, fp
	adds	r1, r1, r2
	ldr	r2, [r3, #0]
	mov	r4, fp
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r4, #4]
	movs	r0, #20
	asrs	r2, r2, #1
	movs	r5, #48
	str	r0, [sp, #0]
	subs	r2, #10
	subs	r3, #24
	str	r5, [sp, #4]
	ldr	r4, [sp, #88]
	ldr	r0, [sp, #76]
	bl	sub_080072f4
.L_080e2ec0:
	ldr	r0, [sp, #16]
	movs	r1, #224
	lsls	r1, r1, #2
	movs	r2, #1
	ldr	r3, [sp, #52]
	adds	r0, r0, r1
	add	r8, r2
	str	r0, [sp, #16]
	cmp	r8, r3
	beq.n	.L_080e2ed6
	b.n	.L_080e2c46
.L_080e2ed6:
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080049ac
	ldr	r0, [sp, #64]
	ldr	r1, [sp, #20]
	bl	sub_080051d8
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	movs	r0, #46
	str	r3, [sp, #0]
	bl	sub_080ed408
	movs	r4, #2
	movs	r1, #7
	movs	r3, #3
	movs	r0, #47
	movs	r2, #7
	str	r4, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [sp, #44]
	adds	r3, #184
	ldr	r3, [r3, #0]
	ldr	r5, [pc, #252]
	str	r3, [sp, #88]
	ldr	r0, [sp, #28]
	ldr	r3, [r5, #0]
	movs	r1, #0
	str	r3, [r0, #4]
	ldr	r6, [pc, #244]
	mov	r8, r1
	add	r7, sp, #104
.L_080e2f24:
	ldr	r5, [r6, #24]
	cmp	r5, #0
	ble.n	.L_080e2f98
	adds	r1, r7, #0
	adds	r0, r6, #0
	bl	sub_080e3944
	ldr	r2, [r7, #0]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	mov	r3, r8
	lsrs	r0, r3, #31
	asrs	r5, r5, #3
	adds	r5, #1
	add	r0, r8
	lsls	r4, r5, #1
	asrs	r2, r2, #1
	movs	r3, #1
	ldr	r1, [pc, #204]
	asrs	r0, r0, #1
	str	r2, [r7, #0]
	ands	r0, r3
	subs	r3, r4, #2
	ldrh	r1, [r1, r3]
	ldr	r3, [sp, #68]
	adds	r1, r3, r1
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	str	r5, [sp, #0]
	subs	r3, r3, r5
	str	r4, [sp, #4]
	ldr	r5, [sp, #28]
	lsls	r0, r0, #2
	ldr	r4, [r0, r5]
	ldr	r0, [sp, #76]
	bl	sub_080072f4
	adds	r0, r6, #0
	movs	r1, #60
	ldr	r2, [pc, #160]
	bl	sub_080e38b8
	ldr	r3, [r6, #4]
	ldr	r0, [pc, #156]
	cmp	r3, r0
	bgt.n	.L_080e2f92
	ldr	r3, [r6, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
.L_080e2f92:
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L_080e2f98:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #2
	adds	r6, #28
	cmp	r8, r2
	bne.n	.L_080e2f24
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	movs	r1, #8
	movs	r0, #8
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r4, [pc, #100]
	ldr	r3, [sp, #80]
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r5, [sp, #72]
	ldr	r2, [sp, #60]
	ldr	r0, [sp, #84]
	adds	r5, #1
	str	r5, [sp, #72]
	ldr	r1, [pc, #44]
	subs	r3, r2, r0
	adds	r3, #5
	ldrb	r3, [r1, r3]
	cmp	r5, r3
	beq.n	.L_080e2fe6
	b.n	.L_080e2ba6
.L_080e2fe6:
	ldr	r0, [pc, #64]
	bl	sub_08004278
	bl	sub_080cdbc0
	add	sp, #140
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000077a8
	.4byte 0x080eed3e
	.4byte 0x02010018
	.4byte 0x03001f0c
	.4byte 0x02010000
	.4byte 0x080ede48
	.4byte 0xfffffc00
	.4byte 0x0007ffff
	.4byte 0x00007824
	.2byte 0xd261
	.2byte 0x080c
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #48]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	sub	sp, #72
	str	r1, [sp, #44]
	ldr	r3, [r3, #0]
	str	r3, [sp, #40]
	ldr	r2, [r2, #8]
	str	r2, [sp, #28]
	ldr	r2, [pc, #32]
	adds	r5, r1, r2
	str	r0, [r5, #0]
	movs	r0, #0
	bl	sub_080cd594
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #0
	bne.n	.L_080e3078
	ldr	r2, [pc, #16]
	ldr	r3, [pc, #4]
	b.n	.L_080e3080
	movs	r0, r0
	.4byte 0x000000cc
	.4byte 0x03001eec
	.4byte 0x00007828
	.2byte 0x0020
	.2byte 0x0400
.L_080e3078:
	cmp	r3, #1
	bne.n	.L_080e3082
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #44]
.L_080e3080:
	strh	r3, [r2, #0]
.L_080e3082:
	ldr	r4, [sp, #44]
	ldr	r6, [pc, #44]
	adds	r3, r4, r6
	ldr	r2, [r3, #0]
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_080e30b8
	movs	r0, #8
	str	r0, [sp, #24]
	ldr	r3, [r2, #24]
	movs	r1, #40
	str	r1, [sp, #20]
	cmp	r3, #0
	beq.n	.L_080e30c4
	movs	r2, #36
	str	r2, [sp, #20]
	cmp	r3, #1
	beq.n	.L_080e30c4
	movs	r3, #40
	str	r3, [sp, #20]
	b.n	.L_080e30c4
	.4byte 0x000000aa
	.4byte 0x04000020
	.2byte 0x7828
	.2byte 0x0000
.L_080e30b8:
	movs	r4, #16
	movs	r6, #12
	negs	r4, r4
	negs	r6, r6
	str	r4, [sp, #24]
	str	r6, [sp, #20]
.L_080e30c4:
	ldr	r1, [sp, #44]
	ldr	r2, [pc, #108]
	adds	r3, r1, r2
	ldr	r1, [r3, #0]
	ldr	r3, [r1, #4]
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r3, [r1, #24]
	ldr	r0, [pc, #96]
	adds	r3, r3, r2
	ldrsb	r3, [r0, r3]
	ldr	r4, [pc, #96]
	lsls	r3, r3, #8
	str	r3, [r4, #0]
	ldr	r1, [pc, #92]
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #92]
	bl	sub_080e0524
	ldr	r0, [pc, #88]
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #84]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	sub_080072f0
	ldr	r5, [pc, #76]
	ldr	r6, [pc, #44]
	movs	r7, #0
.L_080e3106:
	ldrh	r2, [r5, #0]
	lsls	r3, r2, #16
	lsrs	r0, r3, #26
	ands	r0, r6
	lsrs	r1, r3, #21
	movs	r4, #31
	ands	r1, r6
	ands	r4, r2
	subs	r0, #8
	subs	r1, #8
	subs	r4, #8
	cmp	r0, #0
	bge.n	.L_080e3122
	movs	r0, #0
.L_080e3122:
	cmp	r1, #0
	bge.n	.L_080e3128
	movs	r1, #0
.L_080e3128:
	cmp	r4, #0
	bge.n	.L_080e3154
	movs	r4, #0
	b.n	.L_080e3154
	.4byte 0x0000001f
	.4byte 0x00007828
	.4byte 0x080eeda6
	.4byte 0x04000028
	.4byte 0x02010000
	.4byte 0x0000007b
	.4byte 0x0000007c
	.4byte 0x03001388
	.2byte 0x0002
	.2byte 0x0500
.L_080e3154:
	lsls	r3, r0, #10
	lsls	r2, r1, #5
	orrs	r3, r2
	orrs	r3, r4
	adds	r7, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r7, #63
	bne.n	.L_080e3106
	ldr	r2, [pc, #388]
	ldr	r5, [pc, #388]
	mov	lr, r2
	movs	r4, #0
	movs	r6, #0
	mov	sl, lr
.L_080e3172:
	mov	r3, lr
	ldrb	r3, [r3, #0]
	movs	r7, #0
	mov	ip, r3
.L_080e317a:
	movs	r0, #0
	mov	r1, ip
	mov	r8, r0
	cmp	r1, #0
	beq.n	.L_080e31a0
	ldrb	r0, [r2, r6]
	ldr	r2, [sp, #44]
	adds	r1, r4, r2
	adds	r2, r5, #0
.L_080e318c:
	ldrb	r3, [r2, #0]
	strb	r3, [r1, #0]
	movs	r3, #1
	add	r8, r3
	adds	r2, #1
	adds	r1, #1
	adds	r4, #1
	cmp	r8, r0
	bne.n	.L_080e318c
	mov	r2, sl
.L_080e31a0:
	adds	r7, #1
	cmp	r7, #32
	bne.n	.L_080e317a
	mov	r0, lr
	ldrb	r3, [r0, #0]
	movs	r1, #1
	adds	r6, #1
	adds	r5, r5, r3
	add	lr, r1
	cmp	r6, #9
	bne.n	.L_080e3172
	movs	r6, #0
.L_080e31b8:
	adds	r0, r5, #0
	movs	r7, #0
	adds	r0, #48
.L_080e31be:
	ldr	r2, [sp, #44]
	adds	r1, r4, r2
	adds	r2, r5, #0
.L_080e31c4:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	strb	r3, [r1, #0]
	adds	r4, #1
	adds	r1, #1
	cmp	r2, r0
	bne.n	.L_080e31c4
	adds	r7, #1
	cmp	r7, #3
	bne.n	.L_080e31be
	adds	r6, #1
	adds	r5, #48
	cmp	r6, #32
	bne.n	.L_080e31b8
	ldr	r3, [sp, #44]
	movs	r1, #252
	movs	r7, #0
	lsls	r1, r1, #2
	adds	r2, r4, r3
.L_080e31ea:
	ldrb	r3, [r5, #0]
	adds	r7, #1
	strb	r3, [r2, #0]
	adds	r5, #1
	adds	r2, #1
	adds	r4, #1
	cmp	r7, r1
	bne.n	.L_080e31ea
	ldr	r1, [pc, #248]
	ldr	r0, [pc, #248]
	adds	r6, r1, #3
.L_080e3200:
	movs	r2, #0
	mov	r8, r2
	ldrb	r3, [r0, #0]
	ldrb	r2, [r1, #0]
	muls	r3, r2
	cmp	r3, #0
	beq.n	.L_080e3226
	mov	ip, r3
	ldr	r3, [sp, #44]
	adds	r2, r4, r3
.L_080e3214:
	ldrb	r3, [r5, #0]
	strb	r3, [r2, #0]
	movs	r3, #1
	add	r8, r3
	adds	r2, #1
	adds	r5, #1
	adds	r4, #1
	cmp	r8, ip
	bne.n	.L_080e3214
.L_080e3226:
	adds	r1, #1
	adds	r0, #1
	cmp	r1, r6
	bne.n	.L_080e3200
	ldr	r0, [pc, #204]
	ldr	r1, [sp, #28]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	movs	r4, #13
	negs	r4, r4
	ldr	r5, [pc, #192]
	movs	r7, #0
	adds	r6, r4, #0
.L_080e3244:
	movs	r0, #195
	lsls	r0, r0, #1
	bl	sub_08009030
	ldr	r1, [sp, #44]
	str	r0, [r5, r1]
	cmp	r0, #0
	beq.n	.L_080e3278
	adds	r2, r0, #0
	adds	r2, #38
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r7, #0
	cmp	r7, #0
	bge.n	.L_080e3264
	adds	r1, r7, #3
.L_080e3264:
	asrs	r1, r1, #2
	bl	sub_08009020
	ldr	r2, [sp, #44]
	ldr	r1, [r5, r2]
	ldrb	r3, [r1, #9]
	movs	r2, #4
	ands	r3, r6
	orrs	r3, r2
	strb	r3, [r1, #9]
.L_080e3278:
	adds	r7, #1
	adds	r5, #4
	cmp	r7, #11
	bne.n	.L_080e3244
	movs	r6, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r6, [sp, #0]
	bl	sub_080ed408
	ldr	r5, [pc, #112]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r1, #7
	str	r3, [sp, #32]
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	str	r6, [sp, #0]
	bl	sub_080ed408
	ldr	r2, [pc, #92]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r4, [sp, #44]
	movs	r0, #239
	lsls	r0, r0, #7
	ldr	r1, [pc, #80]
	adds	r3, r4, r0
	adds	r5, #188
	str	r6, [r3, #0]
	adds	r2, r4, r1
	ldr	r5, [r5, #0]
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #64]
	lsls	r1, r1, #3
	str	r5, [sp, #36]
	bl	sub_080041d8
	ldr	r3, [pc, #60]
	ldr	r2, [sp, #44]
	movs	r7, #0
	movs	r6, #15
	adds	r5, r2, r3
	b.n	.L_080e3318
	movs	r0, r0
	.4byte 0x00003f46
	.4byte 0x00001010
	.4byte 0x080eed90
	.4byte 0x02010000
	.4byte 0x080eeda0
	.4byte 0x080eeda3
	.4byte 0x00000073
	.4byte 0x000077d8
	.4byte 0x03001e50
	.4byte 0x04000050
	.4byte 0x00007784
	.4byte 0x080cd261
	.2byte 0x7198
	.2byte 0x0000
.L_080e3318:
	bl	sub_08004458
	ands	r0, r6
	movs	r3, #128
	adds	r0, #88
	str	r3, [r5, #4]
	str	r0, [r5, #0]
	bl	sub_08004458
	movs	r3, #1
	str	r3, [r5, #16]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #20]
	lsls	r2, r7, #2
	ands	r0, r6
	movs	r3, #44
	str	r7, [r5, #12]
	adds	r0, #2
	subs	r3, r3, r2
	adds	r7, #1
	str	r0, [r5, #8]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #11
	bne.n	.L_080e3318
	ldr	r4, [sp, #44]
	ldr	r6, [pc, #148]
	ldr	r1, [pc, #148]
	movs	r7, #0
	adds	r2, r4, r6
.L_080e3356:
	ldrb	r3, [r1, #0]
	adds	r7, #1
	str	r3, [r2, #0]
	adds	r1, #1
	adds	r2, #28
	cmp	r7, #6
	bne.n	.L_080e3356
	ldr	r5, [pc, #132]
	movs	r7, #0
	movs	r6, #63
.L_080e336a:
	bl	sub_08004458
	ldr	r1, [sp, #24]
	ands	r0, r6
	adds	r0, r0, r1
	adds	r0, #32
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #96
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08004458
	ands	r0, r6
	adds	r0, #32
	lsls	r0, r0, #13
	str	r0, [r5, #16]
	bl	sub_08004458
	movs	r3, #31
	movs	r2, #128
	ands	r3, r0
	adds	r7, #1
	lsls	r2, r2, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, r2
	bne.n	.L_080e336a
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #0]
	ldr	r3, [sp, #44]
	ldr	r4, [pc, #64]
	adds	r2, r3, r4
	movs	r3, #250
	str	r3, [r2, #0]
	ldr	r0, [sp, #24]
	adds	r0, #64
	movs	r6, #0
	str	r0, [sp, #16]
	mov	fp, r6
.L_080e33c4:
	mov	r1, fp
	cmp	r1, #0
	bne.n	.L_080e33d0
	movs	r0, #212
	bl	sub_080f9010
.L_080e33d0:
	mov	r2, fp
	cmp	r2, #40
	bne.n	.L_080e33f8
	movs	r0, #141
	bl	sub_080f9010
	b.n	.L_080e33f8
	movs	r0, r0
	.4byte 0x00000785
	.4byte 0x00007240
	.4byte 0x080eedac
	.4byte 0x02010000
	.4byte 0x0400000c
	.2byte 0x77a8
	.2byte 0x0000
.L_080e33f8:
	mov	r3, fp
	cmp	r3, #96
	bne.n	.L_080e3404
	movs	r0, #145
	bl	sub_080f9010
.L_080e3404:
	mov	r4, fp
	cmp	r4, #120
	bne.n	.L_080e3410
	movs	r0, #134
	bl	sub_080b50e8
.L_080e3410:
	mov	r6, fp
	cmp	r6, #81
	bgt.n	.L_080e347a
	mov	r3, fp
	cmp	r6, #0
	bge.n	.L_080e341e
	adds	r3, #3
.L_080e341e:
	asrs	r7, r3, #2
	cmp	r7, #2
	ble.n	.L_080e342a
	movs	r3, #1
	ands	r3, r7
	adds	r7, r3, #1
.L_080e342a:
	ldr	r0, [pc, #876]
	ldr	r3, [pc, #876]
	lsls	r1, r7, #1
	ldrb	r4, [r3, r7]
	ldr	r2, [sp, #44]
	ldr	r6, [sp, #24]
	mov	r8, r1
	ldrh	r1, [r0, r1]
	adds	r1, r2, r1
	subs	r2, r6, r4
	ldr	r6, [pc, #864]
	mov	sl, r0
	ldrb	r0, [r6, r7]
	movs	r5, #116
	mov	r9, r3
	adds	r2, #64
	subs	r3, r5, r0
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #32]
	ldr	r0, [sp, #40]
	bl	sub_080072f4
	mov	r0, sl
	mov	r2, r8
	ldrh	r1, [r0, r2]
	ldr	r3, [sp, #44]
	ldrb	r2, [r6, r7]
	mov	r4, r9
	adds	r1, r3, r1
	ldrb	r3, [r4, r7]
	subs	r5, r5, r2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	ldr	r0, [sp, #40]
	ldr	r2, [sp, #16]
	adds	r3, r5, #0
	ldr	r6, [sp, #36]
	bl	sub_080072fc
.L_080e347a:
	mov	r3, fp
	subs	r3, #12
	cmp	r3, #75
	bhi.n	.L_080e3504
	mov	r0, fp
	subs	r0, #64
	movs	r1, #3
	bl	sub_080022ec
	adds	r5, r0, #0
	cmp	r5, #0
	bge.n	.L_080e3494
	movs	r5, #0
.L_080e3494:
	cmp	r5, #7
	ble.n	.L_080e349a
	movs	r5, #7
.L_080e349a:
	ldr	r0, [sp, #24]
	lsls	r1, r5, #1
	ldr	r2, [pc, #772]
	adds	r0, #64
	movs	r3, #32
	movs	r4, #12
	str	r1, [sp, #8]
	movs	r7, #0
	mov	r8, r0
	mov	r9, r2
	mov	sl, r3
	negs	r4, r4
.L_080e34b2:
	ldr	r0, [sp, #8]
	ldr	r6, [pc, #752]
	ldrh	r1, [r6, r0]
	mov	r6, r9
	ldrb	r3, [r6, r5]
	ldr	r2, [sp, #44]
	ldr	r0, [sp, #24]
	adds	r1, r2, r1
	str	r3, [sp, #0]
	subs	r2, r0, r3
	mov	r3, sl
	str	r3, [sp, #4]
	str	r4, [sp, #12]
	adds	r3, r4, #0
	ldr	r6, [sp, #32]
	adds	r2, #64
	ldr	r0, [sp, #40]
	bl	sub_080072fc
	ldr	r0, [pc, #716]
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #44]
	ldrh	r1, [r0, r2]
	mov	r6, r9
	ldr	r4, [sp, #12]
	adds	r1, r3, r1
	ldrb	r3, [r6, r5]
	mov	r0, sl
	str	r3, [sp, #0]
	str	r0, [sp, #4]
	adds	r3, r4, #0
	ldr	r0, [sp, #40]
	mov	r2, r8
	ldr	r6, [sp, #36]
	bl	sub_080072fc
	ldr	r4, [sp, #12]
	adds	r7, #1
	adds	r4, #32
	cmp	r7, #4
	bne.n	.L_080e34b2
.L_080e3504:
	mov	r0, fp
	subs	r0, #160
	cmp	r0, #23
	bhi.n	.L_080e358c
	movs	r1, #3
	bl	sub_080022ec
	movs	r3, #7
	subs	r5, r3, r0
	cmp	r5, #0
	bge.n	.L_080e351c
	movs	r5, #0
.L_080e351c:
	cmp	r5, #7
	ble.n	.L_080e3522
	movs	r5, #7
.L_080e3522:
	ldr	r0, [sp, #24]
	lsls	r1, r5, #1
	ldr	r2, [pc, #636]
	adds	r0, #64
	movs	r3, #32
	movs	r4, #12
	str	r1, [sp, #8]
	movs	r7, #0
	mov	r8, r0
	mov	r9, r2
	mov	sl, r3
	negs	r4, r4
.L_080e353a:
	ldr	r0, [sp, #8]
	ldr	r6, [pc, #616]
	ldrh	r1, [r6, r0]
	mov	r6, r9
	ldrb	r3, [r6, r5]
	ldr	r2, [sp, #44]
	ldr	r0, [sp, #24]
	adds	r1, r2, r1
	str	r3, [sp, #0]
	subs	r2, r0, r3
	mov	r3, sl
	str	r3, [sp, #4]
	str	r4, [sp, #12]
	adds	r3, r4, #0
	ldr	r6, [sp, #32]
	adds	r2, #64
	ldr	r0, [sp, #40]
	bl	sub_080072fc
	ldr	r0, [pc, #580]
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #44]
	ldrh	r1, [r0, r2]
	mov	r6, r9
	ldr	r4, [sp, #12]
	adds	r1, r3, r1
	ldrb	r3, [r6, r5]
	mov	r0, sl
	str	r3, [sp, #0]
	str	r0, [sp, #4]
	adds	r3, r4, #0
	ldr	r0, [sp, #40]
	mov	r2, r8
	ldr	r6, [sp, #36]
	bl	sub_080072fc
	ldr	r4, [sp, #12]
	adds	r7, #1
	adds	r4, #32
	cmp	r7, #4
	bne.n	.L_080e353a
.L_080e358c:
	mov	r3, fp
	subs	r3, #88
	cmp	r3, #71
	bhi.n	.L_080e35fc
	ldr	r0, [sp, #44]
	movs	r1, #158
	ldr	r4, [sp, #32]
	ldr	r2, [sp, #24]
	lsls	r1, r1, #5
	adds	r6, r0, r1
	movs	r5, #96
	movs	r3, #48
	str	r3, [sp, #0]
	str	r5, [sp, #4]
	mov	sl, r4
	adds	r2, #16
	ldr	r0, [sp, #40]
	adds	r1, r6, #0
	movs	r3, #0
	mov	r9, r2
	bl	sub_0800730c
	ldr	r1, [sp, #36]
	movs	r0, #48
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	ldr	r2, [sp, #16]
	mov	r8, r1
	ldr	r0, [sp, #40]
	adds	r1, r6, #0
	movs	r3, #0
	bl	sub_08007304
	movs	r3, #151
	ldr	r2, [sp, #44]
	lsls	r3, r3, #6
	adds	r6, r2, r3
	movs	r4, #48
	movs	r5, #21
	adds	r1, r6, #0
	mov	r2, r9
	movs	r3, #96
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #40]
	bl	sub_0800730c
	movs	r0, #48
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #40]
	adds	r1, r6, #0
	ldr	r2, [sp, #16]
	movs	r3, #96
	bl	sub_08007304
.L_080e35fc:
	mov	r1, fp
	cmp	r1, #87
	ble.n	.L_080e3662
	ldr	r2, [pc, #424]
	ldr	r5, [pc, #424]
	movs	r7, #0
	mov	r8, r2
.L_080e360a:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L_080e3656
	movs	r0, #3
	ands	r0, r7
	adds	r0, #5
	lsls	r4, r0, #1
	subs	r3, r4, #2
	mov	r6, r8
	ldrh	r1, [r6, r3]
	ldr	r2, [sp, #28]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #1
	subs	r2, r2, r3
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #40]
	ldr	r4, [sp, #32]
	bl	sub_080072f4
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #16]
	subs	r2, r2, r3
	str	r2, [r5, #4]
	cmp	r2, #0
	bge.n	.L_080e365a
	mov	r6, fp
	cmp	r6, #159
	bgt.n	.L_080e365a
	movs	r3, #192
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	b.n	.L_080e365a
.L_080e3656:
	subs	r3, #1
	str	r3, [r5, #24]
.L_080e365a:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #64
	bne.n	.L_080e360a
.L_080e3662:
	mov	r0, fp
	cmp	r0, #4
	bgt.n	.L_080e366a
	b.n	.L_080e37d4
.L_080e366a:
	movs	r1, #6
	mov	sl, r1
	cmp	r0, #71
	ble.n	.L_080e3676
	movs	r2, #11
	mov	sl, r2
.L_080e3676:
	movs	r3, #0
	mov	r4, sl
	mov	r8, r3
	cmp	r4, #0
	bne.n	.L_080e3682
	b.n	.L_080e37d4
.L_080e3682:
	ldr	r6, [sp, #44]
	ldr	r0, [pc, #300]
	adds	r5, r6, r0
.L_080e3688:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	beq.n	.L_080e3690
	b.n	.L_080e37c4
.L_080e3690:
	ldr	r3, [pc, #292]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	mov	r1, fp
	str	r3, [sp, #48]
	str	r4, [sp, #52]
	cmp	r1, #71
	ble.n	.L_080e36bc
	mov	r3, r8
	movs	r4, #128
	ldr	r6, [sp, #44]
	ldr	r0, [pc, #276]
	lsls	r2, r3, #12
	lsls	r4, r4, #8
	adds	r2, r2, r4
	adds	r3, r6, r0
	str	r2, [sp, #48]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #24]
	lsls	r3, r3, #14
	adds	r3, r3, r2
	b.n	.L_080e36c0
.L_080e36bc:
	movs	r3, #128
	lsls	r3, r3, #8
.L_080e36c0:
	str	r3, [sp, #48]
	ldr	r3, [sp, #48]
	add	r7, sp, #48
	str	r3, [r7, #4]
	add	r6, sp, #56
	movs	r3, #0
	str	r3, [r6, #12]
	ldr	r1, [sp, #20]
	ldr	r3, [r5, #0]
	lsls	r2, r1, #1
	adds	r3, r3, r2
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	ldr	r3, [r5, #4]
	movs	r2, #128
	lsls	r2, r2, #18
	lsls	r3, r3, #16
	subs	r3, r2, r3
	str	r2, [r6, #8]
	mov	r2, fp
	lsrs	r0, r2, #31
	add	r0, fp
	asrs	r0, r0, #1
	str	r3, [r6, #4]
	add	r0, r8
	movs	r1, #11
	bl	sub_080022fc
	movs	r4, #1
	negs	r4, r4
	cmp	r0, r4
	beq.n	.L_080e3718
	lsls	r3, r0, #2
	ldr	r0, [pc, #188]
	ldr	r1, [sp, #44]
	adds	r3, r3, r0
	ldr	r0, [r1, r3]
	adds	r2, r7, #0
	adds	r1, r6, #0
	movs	r3, #0
	str	r4, [sp, #12]
	bl	sub_08009008
	ldr	r4, [sp, #12]
.L_080e3718:
	ldr	r3, [r5, #4]
	ldr	r2, [r5, #8]
	subs	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	adds	r2, r2, r3
	str	r2, [r5, #12]
	cmp	r2, #12
	ble.n	.L_080e3732
	adds	r3, r2, #0
	subs	r3, #12
	str	r3, [r5, #12]
.L_080e3732:
	ldr	r3, [r5, #4]
	cmp	r3, #0
	bge.n	.L_080e37c8
	mov	r2, fp
	cmp	r2, #159
	ble.n	.L_080e3742
	str	r4, [r5, #24]
	b.n	.L_080e37c8
.L_080e3742:
	mov	r3, fp
	cmp	r3, #87
	ble.n	.L_080e3790
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #8
	str	r3, [r5, #8]
	ldr	r4, [sp, #44]
	ldr	r6, [pc, #100]
	adds	r3, r4, r6
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #0
	bne.n	.L_080e3770
	bl	sub_08004458
	movs	r1, #96
	bl	sub_08002304
	adds	r0, #42
	b.n	.L_080e378e
.L_080e3770:
	cmp	r3, #1
	bne.n	.L_080e3782
	bl	sub_08004458
	movs	r1, #112
	bl	sub_08002304
	adds	r0, #34
	b.n	.L_080e378e
.L_080e3782:
	bl	sub_08004458
	movs	r1, #160
	bl	sub_08002304
	adds	r0, #10
.L_080e378e:
	str	r0, [r5, #0]
.L_080e3790:
	movs	r3, #128
	str	r3, [r5, #4]
	movs	r3, #8
	b.n	.L_080e37c6
	.4byte 0x080eed9a
	.4byte 0x080eeda0
	.4byte 0x080eeda3
	.4byte 0x080eed90
	.4byte 0x080eed7e
	.4byte 0x080ede48
	.4byte 0x02010000
	.4byte 0x00007198
	.4byte 0x080edab0
	.4byte 0x00007828
	.2byte 0x77d8
	.2byte 0x0000
.L_080e37c4:
	subs	r3, #1
.L_080e37c6:
	str	r3, [r5, #24]
.L_080e37c8:
	movs	r0, #1
	add	r8, r0
	adds	r5, #28
	cmp	r8, sl
	beq.n	.L_080e37d4
	b.n	.L_080e3688
.L_080e37d4:
	mov	r1, fp
	cmp	r1, #158
	bgt.n	.L_080e3832
	ldr	r3, [sp, #44]
	ldr	r4, [pc, #200]
	adds	r2, r3, r4
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_080e3832
	movs	r6, #3
	ands	r1, r6
	adds	r5, r2, #0
	mov	r8, r1
	movs	r6, #36
.L_080e37f4:
	mov	r0, fp
	cmp	r0, #85
	ble.n	.L_080e3826
	movs	r1, #12
	bl	sub_080022fc
	cmp	r0, #0
	bne.n	.L_080e3816
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	adds	r3, r7, #0
	bl	sub_080d6888
.L_080e3816:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_080e3826
	ldr	r3, [r5, #0]
	movs	r1, #5
	ldrsh	r0, [r3, r6]
	bl	sub_080b5088
.L_080e3826:
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	adds	r7, #1
	adds	r6, #2
	cmp	r7, r3
	bne.n	.L_080e37f4
.L_080e3832:
	mov	r3, fp
	subs	r3, #90
	cmp	r3, #70
	bls.n	.L_080e3844
	movs	r0, #2
	movs	r1, #2
	bl	sub_080e155c
	b.n	.L_080e384c
.L_080e3844:
	movs	r0, #8
	movs	r1, #8
	bl	sub_080e155c
.L_080e384c:
	bl	sub_080cd52c
	ldr	r0, [pc, #88]
	ldr	r6, [sp, #44]
	movs	r3, #1
	adds	r2, r6, r0
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r1, #1
	add	fp, r1
	mov	r2, fp
	cmp	r2, #192
	beq.n	.L_080e386c
	b.n	.L_080e33c4
.L_080e386c:
	ldr	r0, [pc, #64]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r3, [pc, #52]
	movs	r7, #0
	adds	r5, r6, r3
.L_080e3884:
	ldmia	r5!, {r0}
	adds	r7, #1
	bl	sub_08009038
	cmp	r7, #11
	bne.n	.L_080e3884
	bl	sub_080cdbc0
	add	sp, #72
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00007828
	.4byte 0x00007824
	.4byte 0x080cd261
	.4byte 0x000077d8
