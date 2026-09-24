.syntax unified
	.thumb
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
	.global BattlePres_RunBurstScene
	.global Func_080e2974
	.thumb_func
BattlePres_RunBurstScene:
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
	.4byte 0x080cd261
