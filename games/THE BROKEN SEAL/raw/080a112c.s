.syntax unified
	.thumb
	.set sub_08002df0, 0x08002df0
	.set sub_080030f8, 0x080030f8
	.set sub_08004938, 0x08004938
	.set sub_080072f0, 0x080072f0
	.set sub_08015068, 0x08015068
	.set sub_08015080, 0x08015080
	.set sub_08015090, 0x08015090
	.set sub_080150b0, 0x080150b0
	.set sub_080150d8, 0x080150d8
	.set sub_08015278, 0x08015278
	.set sub_08077008, 0x08077008
	.set sub_08077010, 0x08077010
	.set sub_08077018, 0x08077018
	.set sub_08077050, 0x08077050
	.set sub_08077218, 0x08077218
	.set sub_080a10d0, 0x080a10d0
	.set sub_080a153c, 0x080a153c
	.set sub_080a15f0, 0x080a15f0
	.set sub_080a8b10, 0x080a8b10
	.global Overlay_080a112c
Overlay_080a112c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #36
	str	r2, [sp, #24]
	movs	r2, #0
	str	r2, [sp, #20]
	mov	fp, r3
	ldr	r3, [pc, #860]
	mov	r9, r1
	ldr	r3, [r3, #0]
	mov	r0, r9
	mov	sl, r3
	bl	sub_08077008
	ldr	r3, [sp, #24]
	lsls	r3, r3, #1
	str	r3, [sp, #8]
	adds	r3, #216
	ldrh	r3, [r0, r3]
	mov	r8, r0
	ldr	r0, [pc, #840]
	ands	r0, r3
	str	r3, [sp, #12]
	bl	sub_08077018
	movs	r5, #128
	lsls	r5, r5, #1
	mov	r2, fp
	ands	r5, r2
	str	r0, [sp, #16]
	cmp	r5, #0
	bne.n	.L_080a118e
	movs	r3, #12
	str	r3, [sp, #0]
	mov	r0, sl
	adds	r3, #246
	str	r3, [sp, #4]
	adds	r0, #36
	movs	r1, #0
	movs	r2, #5
	movs	r3, #13
	bl	sub_080a10d0
	str	r0, [sp, #20]
.L_080a118e:
	mov	r3, sl
	ldr	r7, [r3, #36]
	cmp	r5, #0
	bne.n	.L_080a1248
	ldr	r5, [sp, #20]
	cmp	r5, #0
	bne.n	.L_080a11b4
	movs	r0, #1
	bl	sub_080030f8
	mov	r2, sl
	movs	r3, #32
	ldr	r0, [r2, #36]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #0
	movs	r3, #88
	bl	sub_08015068
.L_080a11b4:
	movs	r3, #0
	mov	r0, r8
	adds	r1, r7, #0
	movs	r2, #32
	bl	sub_08015090
	add	r6, sp, #28
	adds	r0, r6, #0
	movs	r1, #1
	mov	r2, r9
	bl	sub_080a8b10
	ldrb	r3, [r6, #1]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_080a11e2
	ldr	r0, [pc, #724]
	adds	r1, r7, #0
	movs	r2, #32
	movs	r3, #8
	bl	sub_08015080
	movs	r5, #1
.L_080a11e2:
	ldrb	r3, [r6, #2]
	cmp	r3, #0
	beq.n	.L_080a11f8
	lsls	r3, r5, #3
	ldr	r0, [pc, #708]
	adds	r3, #8
	adds	r1, r7, #0
	movs	r2, #32
	bl	sub_08015080
	adds	r5, #1
.L_080a11f8:
	ldrb	r3, [r6, #3]
	cmp	r3, #0
	beq.n	.L_080a120e
	lsls	r3, r5, #3
	ldr	r0, [pc, #688]
	adds	r3, #8
	adds	r1, r7, #0
	movs	r2, #32
	bl	sub_08015080
	adds	r5, #1
.L_080a120e:
	ldrb	r3, [r6, #4]
	cmp	r3, #0
	beq.n	.L_080a1224
	lsls	r3, r5, #3
	ldr	r0, [pc, #672]
	adds	r3, #8
	adds	r1, r7, #0
	movs	r2, #32
	bl	sub_08015080
	adds	r5, #1
.L_080a1224:
	cmp	r5, #1
	bgt.n	.L_080a1248
	mov	r3, r8
	ldrb	r6, [r3, #15]
	ldr	r0, [pc, #652]
	adds	r1, r7, #0
	movs	r2, #40
	movs	r3, #16
	bl	sub_08015090
	movs	r3, #16
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #4
	adds	r2, r7, #0
	movs	r3, #56
	bl	sub_080150b0
.L_080a1248:
	ldr	r5, [sp, #20]
	cmp	r5, #0
	bne.n	.L_080a1266
	movs	r0, #1
	bl	sub_080030f8
	mov	r2, sl
	movs	r3, #80
	ldr	r0, [r2, #36]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #32
	movs	r3, #88
	bl	sub_08015068
.L_080a1266:
	adds	r0, r7, #0
	bl	sub_08015278
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r5, fp
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L_080a1290
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	mov	r0, r9
	adds	r3, r7, #0
	movs	r1, #0
	movs	r2, #0
	bl	sub_080150d8
	movs	r3, #190
	lsls	r3, r3, #1
	add	r3, sl
	str	r0, [r3, #0]
.L_080a1290:
	movs	r3, #255
	mov	r2, fp
	ands	r3, r2
	cmp	r3, #8
	bls.n	.L_080a129c
	b.n	.L_080a1490
.L_080a129c:
	ldr	r2, [pc, #544]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080a12c8
	.4byte 0x080a1490
	.4byte 0x080a132c
	.4byte 0x080a132c
	.4byte 0x080a13ac
	.4byte 0x080a1490
	.4byte 0x080a130c
	.4byte 0x080a1490
	.2byte 0x140a
	.2byte 0x080a
	ldr	r3, [pc, #504]
	add	r3, r8
	ldrb	r2, [r3, #0]
	ldr	r3, [pc, #504]
	adds	r6, r2, r3
	adds	r0, r6, #0
	movs	r2, #0
	movs	r3, #32
	adds	r1, r7, #0
	bl	sub_08015080
	mov	r0, r8
	adds	r1, r7, #0
	bl	sub_080a153c
	movs	r3, #146
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r6, [r3, #0]
	ldr	r0, [pc, #476]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #64
	bl	sub_08015080
	movs	r3, #72
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #8
	adds	r2, r7, #0
	movs	r3, #24
	bl	sub_080150b0
	b.n	.L_080a1490
	ldr	r3, [pc, #436]
	add	r3, r8
	ldrb	r2, [r3, #0]
	ldr	r3, [pc, #436]
	adds	r6, r2, r3
	adds	r0, r6, #0
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #32
	bl	sub_08015080
	mov	r0, r8
	adds	r1, r7, #0
	bl	sub_080a153c
	b.n	.L_080a1490
	mov	r0, r9
	ldr	r1, [sp, #12]
	bl	sub_08077218
	cmp	r0, #0
	bne.n	.L_080a1346
	ldr	r0, [pc, #404]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #48
	bl	sub_08015080
	b.n	.L_080a1490
.L_080a1346:
	movs	r5, #166
	lsls	r5, r5, #1
	adds	r0, r5, #0
	bl	sub_08004938
	ldr	r3, [pc, #384]
	mov	r1, r8
	adds	r2, r5, #0
	adds	r6, r0, #0
	bl	sub_080072f0
	movs	r3, #151
	lsls	r3, r3, #2
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080a137e
	ldr	r1, [sp, #8]
	mov	r3, r8
	adds	r1, #216
	ldrh	r2, [r3, r1]
	ldr	r3, [pc, #352]
	mov	r5, r8
	ands	r3, r2
	strh	r3, [r5, r1]
	b.n	.L_080a1386
.L_080a137e:
	mov	r0, r9
	ldr	r1, [sp, #24]
	bl	sub_08077050
.L_080a1386:
	mov	r0, r9
	bl	sub_08077010
	mov	r0, r8
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	sub_080a15f0
	movs	r2, #166
	ldr	r3, [pc, #312]
	mov	r0, r8
	adds	r1, r6, #0
	lsls	r2, r2, #1
	bl	sub_080072f0
	adds	r0, r6, #0
	bl	sub_08002df0
	b.n	.L_080a1490
	ldr	r2, [sp, #16]
	movs	r3, #88
	mov	r5, r8
	ldrh	r4, [r2, #40]
	ldrh	r2, [r5, r3]
	ldr	r3, [pc, #292]
	ands	r3, r2
	movs	r0, #0
	movs	r1, #0
	b.n	.L_080a13d2
.L_080a13c0:
	adds	r1, #1
	cmp	r1, #31
	bgt.n	.L_080a13d8
	lsls	r3, r1, #2
	adds	r3, #88
	mov	r5, r8
	ldrh	r2, [r5, r3]
	ldr	r3, [pc, #268]
	ands	r3, r2
.L_080a13d2:
	cmp	r3, r4
	bne.n	.L_080a13c0
	movs	r0, #1
.L_080a13d8:
	cmp	r0, #0
	beq.n	.L_080a13ee
	ldr	r0, [pc, #256]
	adds	r1, r7, #0
	adds	r0, r4, r0
	movs	r2, #0
	movs	r3, #48
	bl	sub_08015080
	ldr	r0, [pc, #248]
	b.n	.L_080a13fe
.L_080a13ee:
	ldr	r0, [pc, #240]
	adds	r1, r7, #0
	adds	r0, r4, r0
	movs	r2, #0
	movs	r3, #48
	bl	sub_08015080
	ldr	r0, [pc, #232]
.L_080a13fe:
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #56
	bl	sub_08015080
	b.n	.L_080a1490
	ldr	r5, [pc, #224]
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #40
	bl	sub_08015080
	mov	r2, r8
	ldrh	r6, [r2, #60]
	movs	r3, #40
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #3
	adds	r2, r7, #0
	movs	r3, #64
	bl	sub_080150b0
	adds	r0, r5, #1
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #48
	bl	sub_08015080
	mov	r3, r8
	ldrh	r6, [r3, #62]
	movs	r3, #48
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #3
	adds	r2, r7, #0
	movs	r3, #64
	bl	sub_080150b0
	adds	r0, r5, #4
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #56
	bl	sub_08015080
	mov	r3, r8
	adds	r3, #64
	ldrh	r6, [r3, #0]
	movs	r3, #56
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #3
	adds	r2, r7, #0
	movs	r3, #64
	adds	r5, #3
	bl	sub_080150b0
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #64
	bl	sub_08015080
	mov	r3, r8
	adds	r3, #66
	ldrb	r6, [r3, #0]
	movs	r1, #3
	movs	r3, #64
	adds	r0, r6, #0
	adds	r2, r7, #0
	str	r3, [sp, #0]
	bl	sub_080150b0
.L_080a1490:
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x000001ff
	.4byte 0x00000bd6
	.4byte 0x00000bd7
	.4byte 0x00000bd8
	.4byte 0x00000bd9
	.4byte 0x080af20c
	.4byte 0x080a12a4
	.4byte 0x00000129
	.4byte 0x00000741
	.4byte 0x00000b0e
	.4byte 0x00000b21
	.4byte 0x03001388
	.4byte 0x0000fdff
	.4byte 0x00003fff
	.4byte 0x00000333
	.4byte 0x00000b23
	.4byte 0x00000b22
	.4byte 0x00000b1c
