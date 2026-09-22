.syntax unified
	.thumb
	.set sub_08004458, 0x08004458
	.set sub_08077008, 0x08077008
	.set sub_08077198, 0x08077198
	.set sub_080772b8, 0x080772b8
	.global Overlay_080bae40
Overlay_080bae40:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #92
	str	r0, [sp, #28]
	ldr	r3, [pc, #44]
	movs	r0, #0
	adds	r7, r1, #0
	ldr	r1, [r3, #0]
	str	r0, [sp, #20]
	ldrb	r3, [r7, #0]
	mov	r8, r0
	cmp	r3, #0
	bne.n	.L_080bae66
	b.n	.L_080baf46
.L_080bae66:
	movs	r2, #0
	cmp	r3, #2
	beq.n	.L_080bae70
	cmp	r3, #4
	bne.n	.L_080bae72
.L_080bae70:
	movs	r2, #1
.L_080bae72:
	ldr	r3, [sp, #28]
	cmp	r3, #7
	bls.n	.L_080bae84
	cmp	r2, #0
	beq.n	.L_080bae88
	b.n	.L_080baee8
	movs	r0, r0
	.2byte 0x1e74
	.2byte 0x0300
.L_080bae84:
	cmp	r2, #0
	beq.n	.L_080baee8
.L_080bae88:
	movs	r4, #0
	str	r4, [sp, #24]
	movs	r3, #88
	ldrsh	r3, [r1, r3]
	cmp	r3, #255
	beq.n	.L_080baf46
	mov	r0, r8
	lsls	r3, r0, #1
	add	r2, sp, #92
	adds	r3, r3, r2
	mov	r4, r8
	adds	r0, r3, #0
	lsls	r3, r4, #2
	adds	r3, r3, r2
	adds	r4, r3, #0
	ldr	r5, [pc, #28]
	adds	r1, #88
	subs	r0, #12
	subs	r4, #36
.L_080baeae:
	movs	r6, #0
	ldrsh	r2, [r1, r6]
	cmp	r2, #254
	beq.n	.L_080baed6
	ldrb	r3, [r7, #0]
	cmp	r3, #4
	bne.n	.L_080baec8
	ldr	r3, [sp, #28]
	cmp	r2, r3
	bne.n	.L_080baed6
	b.n	.L_080baec8
	.2byte 0x0100
	.2byte 0x0000
.L_080baec8:
	stmia	r4!, {r2}
	ldr	r3, [sp, #24]
	movs	r6, #1
	orrs	r3, r5
	strh	r3, [r0, #0]
	add	r8, r6
	adds	r0, #2
.L_080baed6:
	ldr	r2, [sp, #24]
	adds	r2, #1
	str	r2, [sp, #24]
	adds	r1, #2
	movs	r6, #0
	ldrsh	r3, [r1, r6]
	cmp	r3, #255
	bne.n	.L_080baeae
	b.n	.L_080baf46
.L_080baee8:
	movs	r0, #0
	str	r0, [sp, #24]
	adds	r2, r1, #2
	movs	r3, #100
	ldrsh	r3, [r2, r3]
	mov	ip, r2
	cmp	r3, #255
	beq.n	.L_080baf46
	mov	r4, r8
	lsls	r3, r4, #1
	add	r6, sp, #92
	adds	r3, r3, r6
	adds	r1, r3, #0
	lsls	r3, r4, #2
	adds	r3, r3, r6
	adds	r4, r3, #0
	ldr	r5, [pc, #24]
	movs	r0, #100
	subs	r1, #12
	subs	r4, #36
.L_080baf10:
	ldrsh	r2, [r2, r0]
	cmp	r2, #254
	beq.n	.L_080baf36
	ldrb	r3, [r7, #0]
	cmp	r3, #4
	bne.n	.L_080baf28
	ldr	r6, [sp, #28]
	cmp	r2, r6
	bne.n	.L_080baf36
	b.n	.L_080baf28
	.2byte 0x0180
	.2byte 0x0000
.L_080baf28:
	stmia	r4!, {r2}
	ldr	r3, [sp, #24]
	movs	r2, #1
	orrs	r3, r5
	strh	r3, [r1, #0]
	add	r8, r2
	adds	r1, #2
.L_080baf36:
	ldr	r3, [sp, #24]
	adds	r3, #1
	str	r3, [sp, #24]
	adds	r0, #2
	mov	r2, ip
	ldrsh	r3, [r2, r0]
	cmp	r3, #255
	bne.n	.L_080baf10
.L_080baf46:
	mov	r0, r8
	cmp	r0, #0
	bne.n	.L_080baf52
	movs	r0, #2
	negs	r0, r0
	b.n	.L_080bb572
.L_080baf52:
	movs	r1, #0
	str	r1, [sp, #24]
	cmp	r1, r8
	blt.n	.L_080baf5c
	b.n	.L_080bb3b6
.L_080baf5c:
	add	r2, sp, #56
	mov	sl, r2
.L_080baf60:
	ldr	r3, [sp, #24]
	mov	r4, sl
	lsls	r3, r3, #2
	ldr	r0, [r4, r3]
	mov	r9, r3
	bl	sub_08077008
	adds	r5, r0, #0
	ldrb	r0, [r7, #3]
	movs	r6, #0
	cmp	r0, #64
	bls.n	.L_080baf7a
	b.n	.L_080bb142
.L_080baf7a:
	ldr	r2, [pc, #860]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080bb304
	.4byte 0x080bb304
	.4byte 0x080bb304
	.4byte 0x080bb132
	.4byte 0x080bb146
	.4byte 0x080bb2c8
	.4byte 0x080bb088
	.4byte 0x080bb088
	.4byte 0x080bb0ac
	.4byte 0x080bb0ac
	.4byte 0x080bb0c2
	.4byte 0x080bb0c2
	.4byte 0x080bb0dc
	.4byte 0x080bb0dc
	.4byte 0x080bb0fa
	.4byte 0x080bb0fa
	.4byte 0x080bb114
	.4byte 0x080bb114
	.4byte 0x080bb290
	.4byte 0x080bb29c
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb2ac
	.4byte 0x080bb2b2
	.4byte 0x080bb142
	.4byte 0x080bb2ba
	.4byte 0x080bb142
	.4byte 0x080bb28a
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb1ac
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb2c8
	.4byte 0x080bb2c8
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb198
	.4byte 0x080bb198
	.4byte 0x080bb142
	.2byte 0xb222
	.2byte 0x080b
	ldr	r1, [pc, #592]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r3, #1
	cmp	r3, #4
	bgt.n	.L_080bb09a
.L_080bb098:
	movs	r6, #1
.L_080bb09a:
	movs	r2, #153
	lsls	r2, r2, #1
	adds	r3, r5, r2
.L_080bb0a0:
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L_080bb0a8
	b.n	.L_080bb304
.L_080bb0a8:
	adds	r6, #1
	b.n	.L_080bb304
	ldr	r4, [pc, #556]
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r1, #4
	subs	r3, #1
	negs	r1, r1
	cmp	r3, r1
	blt.n	.L_080bb09a
	b.n	.L_080bb098
	ldr	r4, [pc, #540]
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r3, #1
	cmp	r3, #4
	bgt.n	.L_080bb0d4
	movs	r6, #1
.L_080bb0d4:
	movs	r1, #154
	lsls	r1, r1, #1
	adds	r3, r5, r1
	b.n	.L_080bb0a0
	ldr	r2, [pc, #512]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r4, #4
	subs	r3, #1
	negs	r4, r4
	cmp	r3, r4
	blt.n	.L_080bb0f2
	movs	r6, #1
.L_080bb0f2:
	movs	r1, #154
	lsls	r1, r1, #1
	adds	r3, r5, r1
	b.n	.L_080bb0a0
	ldr	r2, [pc, #488]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r3, #1
	cmp	r3, #4
	bgt.n	.L_080bb10c
	movs	r6, #1
.L_080bb10c:
	movs	r4, #155
	lsls	r4, r4, #1
	adds	r3, r5, r4
	b.n	.L_080bb0a0
	ldr	r1, [pc, #460]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r2, #4
	subs	r3, #1
	negs	r2, r2
	cmp	r3, r2
	blt.n	.L_080bb12a
	movs	r6, #1
.L_080bb12a:
	movs	r4, #155
	lsls	r4, r4, #1
	adds	r3, r5, r4
	b.n	.L_080bb0a0
	ldr	r1, [pc, #436]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080bb142
	b.n	.L_080bb304
.L_080bb142:
	movs	r6, #1
	b.n	.L_080bb304
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080bb154
	movs	r6, #1
.L_080bb154:
	ldr	r4, [pc, #404]
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080bb160
	adds	r6, #1
.L_080bb160:
	movs	r1, #157
	lsls	r1, r1, #1
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080bb16e
	adds	r6, #1
.L_080bb16e:
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080bb17c
	adds	r6, #1
.L_080bb17c:
	ldr	r4, [pc, #368]
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080bb188
	adds	r6, #1
.L_080bb188:
	ldr	r1, [pc, #360]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
.L_080bb18e:
	cmp	r3, #0
	bne.n	.L_080bb194
	b.n	.L_080bb304
.L_080bb194:
	adds	r6, #1
	b.n	.L_080bb304
	movs	r3, #56
	ldrsh	r2, [r5, r3]
	movs	r4, #52
	ldrsh	r3, [r5, r4]
	ldrh	r1, [r5, #56]
	cmp	r2, r3
	blt.n	.L_080bb1a8
	b.n	.L_080bb306
.L_080bb1a8:
	movs	r6, #1
	b.n	.L_080bb306
	ldr	r1, [pc, #300]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_080bb1bc
	movs	r6, #1
.L_080bb1bc:
	ldr	r2, [pc, #288]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_080bb1cc
	adds	r6, #1
.L_080bb1cc:
	ldr	r4, [pc, #276]
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_080bb1dc
	adds	r6, #1
.L_080bb1dc:
	movs	r1, #150
	lsls	r1, r1, #1
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_080bb1ee
	adds	r6, #1
.L_080bb1ee:
	ldr	r2, [pc, #264]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_080bb1fe
	adds	r6, #1
.L_080bb1fe:
	movs	r4, #151
	lsls	r4, r4, #1
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_080bb210
	adds	r6, #1
.L_080bb210:
	ldr	r1, [pc, #232]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_080bb304
	adds	r6, #1
	b.n	.L_080bb304
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080bb230
	movs	r6, #1
.L_080bb230:
	ldr	r4, [pc, #184]
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080bb23c
	adds	r6, #1
.L_080bb23c:
	movs	r1, #157
	lsls	r1, r1, #1
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080bb24a
	adds	r6, #1
.L_080bb24a:
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080bb258
	adds	r6, #1
.L_080bb258:
	ldr	r4, [pc, #148]
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080bb264
	adds	r6, #1
.L_080bb264:
	ldr	r1, [pc, #140]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080bb270
	adds	r6, #1
.L_080bb270:
	movs	r2, #160
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080bb27e
	adds	r6, #1
.L_080bb27e:
	ldr	r4, [pc, #104]
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	b.n	.L_080bb18e
	ldr	r1, [pc, #104]
	adds	r3, r5, r1
	b.n	.L_080bb2c0
	ldr	r2, [pc, #84]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	b.n	.L_080bb2c2
	ldr	r4, [pc, #72]
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #1
	bgt.n	.L_080bb304
	b.n	.L_080bb142
	ldr	r1, [pc, #80]
	adds	r3, r5, r1
	b.n	.L_080bb2c0
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r5, r2
	b.n	.L_080bb2c0
	movs	r4, #160
	lsls	r4, r4, #1
	adds	r3, r5, r4
.L_080bb2c0:
	ldrb	r3, [r3, #0]
.L_080bb2c2:
	cmp	r3, #0
	bne.n	.L_080bb304
	b.n	.L_080bb142
	movs	r2, #56
	ldrsh	r3, [r5, r2]
	ldrh	r1, [r5, #56]
	cmp	r3, #0
	bne.n	.L_080bb316
	movs	r6, #100
	b.n	.L_080bb306
	movs	r0, r0
	.4byte 0x080baf84
	.4byte 0x00000133
	.4byte 0x00000135
	.4byte 0x00000137
	.4byte 0x00000131
	.4byte 0x00000139
	.4byte 0x0000013d
	.4byte 0x00000141
	.4byte 0x0000012d
	.4byte 0x0000012f
	.2byte 0x013b
	.2byte 0x0000
.L_080bb304:
	ldrh	r1, [r5, #56]
.L_080bb306:
	lsls	r3, r1, #16
	cmp	r3, #0
	bne.n	.L_080bb316
	bl	sub_080772b8
	cmp	r0, #0
	bne.n	.L_080bb316
	movs	r6, #0
.L_080bb316:
	cmp	r6, #0
	bne.n	.L_080bb38a
	ldrb	r2, [r7, #1]
	movs	r3, #15
	ands	r3, r2
	subs	r3, #1
	cmp	r3, #9
	bhi.n	.L_080bb386
	ldr	r2, [pc, #604]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080bb364
	.4byte 0x080bb376
	.4byte 0x080bb358
	.4byte 0x080bb358
	.4byte 0x080bb376
	.4byte 0x080bb376
	.4byte 0x080bb386
	.4byte 0x080bb376
	.4byte 0x080bb376
	.2byte 0xb35e
	.2byte 0x080b
	movs	r4, #56
	ldrsh	r3, [r5, r4]
	b.n	.L_080bb380
	movs	r1, #58
	ldrsh	r3, [r5, r1]
	b.n	.L_080bb380
	movs	r3, #56
	ldrsh	r2, [r5, r3]
	cmp	r2, #0
	beq.n	.L_080bb386
	movs	r4, #52
	ldrsh	r3, [r5, r4]
	cmp	r2, r3
	bge.n	.L_080bb386
	b.n	.L_080bb384
	ldrh	r3, [r7, #10]
	cmp	r3, #0
	beq.n	.L_080bb386
	movs	r0, #56
	ldrsh	r3, [r5, r0]
.L_080bb380:
	cmp	r3, #0
	beq.n	.L_080bb386
.L_080bb384:
	adds	r6, #1
.L_080bb386:
	cmp	r6, #0
	beq.n	.L_080bb3aa
.L_080bb38a:
	ldr	r2, [sp, #20]
	mov	r4, sl
	mov	r6, r9
	ldr	r3, [r4, r6]
	lsls	r1, r2, #2
	str	r3, [r4, r1]
	ldr	r4, [sp, #24]
	add	r2, sp, #80
	lsls	r3, r4, #1
	ldrh	r3, [r2, r3]
	mov	r0, sp
	adds	r0, #32
	str	r3, [r0, r1]
	ldr	r6, [sp, #20]
	adds	r6, #1
	str	r6, [sp, #20]
.L_080bb3aa:
	ldr	r0, [sp, #24]
	adds	r0, #1
	str	r0, [sp, #24]
	cmp	r0, r8
	bge.n	.L_080bb3b6
	b.n	.L_080baf60
.L_080bb3b6:
	ldr	r1, [sp, #20]
	cmp	r1, #0
	bne.n	.L_080bb3c2
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080bb572
.L_080bb3c2:
	ldrb	r3, [r7, #0]
	mov	r2, sp
	adds	r2, #32
	str	r2, [sp, #12]
	cmp	r3, #1
	beq.n	.L_080bb3d0
	b.n	.L_080bb560
.L_080bb3d0:
	ldrb	r3, [r7, #8]
	cmp	r3, #1
	beq.n	.L_080bb3d8
	b.n	.L_080bb54a
.L_080bb3d8:
	ldr	r0, [sp, #28]
	bl	sub_08077008
	movs	r3, #148
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldrb	r0, [r0, #0]
	bl	sub_08077198
	adds	r0, #53
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	cmp	r3, #2
	bne.n	.L_080bb3f6
	b.n	.L_080bb552
.L_080bb3f6:
	ldrb	r2, [r7, #1]
	movs	r3, #15
	ands	r3, r2
	subs	r3, #3
	cmp	r3, #2
	bls.n	.L_080bb404
	b.n	.L_080bb55a
.L_080bb404:
	ldr	r6, [sp, #20]
	movs	r1, #1
	movs	r4, #0
	negs	r1, r1
	str	r4, [sp, #24]
	cmp	r4, r6
	bge.n	.L_080bb4d2
	mov	r0, sp
	adds	r0, #32
	subs	r6, #1
	str	r0, [sp, #12]
	str	r6, [sp, #8]
.L_080bb41c:
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #8]
	cmp	r2, r3
	bge.n	.L_080bb4c4
	mov	r4, sp
	lsls	r3, r2, #2
	adds	r4, #56
	adds	r6, r3, r4
	str	r4, [sp, #16]
	adds	r0, r3, #4
	adds	r5, r3, #0
	ldr	r4, [sp, #24]
	ldr	r3, [sp, #8]
	ldr	r2, [sp, #12]
	ldr	r7, [sp, #16]
	mov	sl, r0
	subs	r3, r3, r4
	str	r6, [sp, #4]
	mov	r9, r2
	mov	fp, r3
	add	r7, sl
.L_080bb446:
	ldr	r6, [sp, #16]
	ldr	r0, [r5, r6]
	str	r1, [sp, #0]
	bl	sub_08077008
	mov	r8, r0
	ldr	r0, [r7, #0]
	bl	sub_08077008
	adds	r6, r0, #0
	ldr	r0, [sp, #28]
	bl	sub_08077008
	movs	r2, #148
	lsls	r2, r2, #1
	adds	r0, r0, r2
	ldrb	r0, [r0, #0]
	bl	sub_08077198
	adds	r0, #53
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	ldr	r1, [sp, #0]
	cmp	r3, #0
	bne.n	.L_080bb484
	mov	r0, r8
	movs	r4, #56
	ldrsh	r3, [r0, r4]
	movs	r2, #56
	ldrsh	r0, [r6, r2]
	b.n	.L_080bb48e
.L_080bb484:
	mov	r0, r8
	movs	r4, #52
	ldrsh	r3, [r0, r4]
	movs	r2, #52
	ldrsh	r0, [r6, r2]
.L_080bb48e:
	cmp	r3, r0
	bge.n	.L_080bb4aa
	ldr	r3, [sp, #16]
	ldr	r4, [sp, #4]
	ldr	r2, [r5, r3]
	ldr	r3, [r7, #0]
	str	r3, [r4, #0]
	str	r2, [r7, #0]
	mov	r6, r9
	mov	r0, sl
	ldr	r2, [r6, r5]
	ldr	r3, [r6, r0]
	str	r3, [r6, r5]
	str	r2, [r6, r0]
.L_080bb4aa:
	movs	r4, #1
	ldr	r2, [sp, #4]
	negs	r4, r4
	add	fp, r4
	adds	r2, #4
	movs	r3, #4
	mov	r6, fp
	str	r2, [sp, #4]
	adds	r7, #4
	add	sl, r3
	adds	r5, #4
	cmp	r6, #0
	bne.n	.L_080bb446
.L_080bb4c4:
	ldr	r0, [sp, #24]
	ldr	r2, [sp, #20]
	adds	r0, #1
	str	r0, [sp, #24]
	cmp	r0, r2
	blt.n	.L_080bb41c
	b.n	.L_080bb4d8
.L_080bb4d2:
	mov	r3, sp
	adds	r3, #32
	str	r3, [sp, #12]
.L_080bb4d8:
	ldr	r4, [sp, #20]
	cmp	r4, #2
	beq.n	.L_080bb4f4
	cmp	r4, #2
	bgt.n	.L_080bb4e8
	cmp	r4, #1
	beq.n	.L_080bb52c
	b.n	.L_080bb53e
.L_080bb4e8:
	ldr	r6, [sp, #20]
	cmp	r6, #3
	beq.n	.L_080bb508
	cmp	r6, #4
	beq.n	.L_080bb51c
	b.n	.L_080bb53e
.L_080bb4f4:
	bl	sub_08004458
	movs	r3, #11
	muls	r3, r0
	lsrs	r3, r3, #16
	movs	r1, #0
	cmp	r3, #5
	bls.n	.L_080bb53e
.L_080bb504:
	movs	r1, #1
	b.n	.L_080bb53e
.L_080bb508:
	bl	sub_08004458
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsrs	r1, r3, #16
	cmp	r1, #5
	ble.n	.L_080bb52c
	cmp	r1, #10
	bgt.n	.L_080bb538
	b.n	.L_080bb504
.L_080bb51c:
	bl	sub_08004458
	lsls	r3, r0, #3
	adds	r3, r3, r0
	lsls	r3, r3, #1
	lsrs	r1, r3, #16
	cmp	r1, #5
	bgt.n	.L_080bb530
.L_080bb52c:
	movs	r1, #0
	b.n	.L_080bb53e
.L_080bb530:
	cmp	r1, #10
	ble.n	.L_080bb504
	cmp	r1, #14
	bgt.n	.L_080bb53c
.L_080bb538:
	movs	r1, #2
	b.n	.L_080bb53e
.L_080bb53c:
	movs	r1, #3
.L_080bb53e:
	cmp	r1, #0
	blt.n	.L_080bb560
	lsls	r3, r1, #2
	ldr	r1, [sp, #12]
	ldr	r0, [r1, r3]
	b.n	.L_080bb572
.L_080bb54a:
	mov	r2, sp
	adds	r2, #32
	str	r2, [sp, #12]
	b.n	.L_080bb560
.L_080bb552:
	mov	r3, sp
	adds	r3, #32
	str	r3, [sp, #12]
	b.n	.L_080bb560
.L_080bb55a:
	mov	r4, sp
	adds	r4, #32
	str	r4, [sp, #12]
.L_080bb560:
	bl	sub_08004458
	ldr	r6, [sp, #20]
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r1, [sp, #12]
	lsrs	r3, r3, #16
	lsls	r3, r3, #2
	ldr	r0, [r1, r3]
.L_080bb572:
	add	sp, #92
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x080bb330
