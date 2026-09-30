.syntax unified
	.thumb
	.global Func_080f2028
	.thumb_func
Func_080f2028:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_080f21d4
	ldr r6, [r3]
	ldr r3, .L_080f21d8
	ldrb r3, [r3]
	movs r1, #0
	mov r8, r1
	cmp r3, #0
	bne .L_080f2056
	ldr r2, [r6, #12]
	movs r3, #3
	adds r2, #1
	ands r3, r2
	str r2, [r6, #12]
	cmp r3, #0
	bne .L_080f2056
	ldr r3, [r6, #20]
	adds r3, #1
	str r3, [r6, #20]
.L_080f2056:
	ldr r3, .L_080f21dc
	ldrh r2, [r3, #6]
	movs r3, #48
	subs r3, r3, r2
	ldr r2, [r6, #20]
	mov r10, r3
	movs r3, #144
	subs r1, r3, r2
	movs r2, #140
	ldr r3, [r6, #8]
	lsls r2, r2, #1
	cmp r3, r2
	blt .L_080f2072
	b .L_080f23ae
.L_080f2072:
	ldr r3, [r6, #12]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080f207e
	b .L_080f2208
.L_080f207e:
	ldr r3, .L_080f21e0
	mov r2, r10
	ldrb r3, [r3]
	subs r2, r1, r2
	adds r7, r3, #0
	mov r9, r2
	subs r3, #104
	mov r0, r9
	muls r0, r3
	movs r1, #80
	bl FixedPoint_Ratio
	add r0, r10
	adds r5, r0, #0
	subs r5, #16
	subs r7, #16
	cmp r5, #255
	ble .L_080f20aa
	ldr r3, .L_080f21e4
.L_080f20a4:
	adds r5, r5, r3
	cmp r5, #255
	bgt .L_080f20a4
.L_080f20aa:
	cmp r5, #0
	bge .L_080f20b8
	movs r3, #128
	lsls r3, r3, #1
.L_080f20b2:
	adds r5, r5, r3
	cmp r5, #0
	blt .L_080f20b2
.L_080f20b8:
	adds r0, r7, #4
	lsls r0, r0, #16
	adds r3, r0, #0
	ldr r2, .L_080f21e8
	orrs r3, r5
	orrs r3, r2
	movs r1, #24
	str r3, [r6, r1]
	adds r1, r7, #0
	adds r1, #20
	lsls r1, r1, #16
	adds r3, r1, #0
	ldr r2, .L_080f21ec
	orrs r3, r5
	orrs r3, r2
	adds r2, r5, #0
	adds r2, #16
	movs r4, #32
	lsls r2, r2, #24
	str r3, [r6, r4]
	lsrs r2, r2, #24
	ldr r3, .L_080f21f0
	orrs r0, r2
	orrs r0, r3
	ldr r3, .L_080f21f4
	movs r4, #40
	orrs r1, r2
	str r0, [r6, r4]
	movs r2, #232
	orrs r1, r3
	movs r0, #48
	movs r3, #28
	str r1, [r6, r0]
	str r2, [r6, r3]
	movs r3, #36
	str r2, [r6, r3]
	movs r3, #44
	str r2, [r6, r3]
	movs r3, #52
	str r2, [r6, r3]
	ldr r3, .L_080f21e0
	ldrb r3, [r3, #2]
	adds r7, r3, #0
	subs r3, #104
	mov r0, r9
	muls r0, r3
	movs r1, #80
	bl FixedPoint_Ratio
	add r0, r10
	adds r5, r0, #0
	subs r5, #16
	subs r7, #16
	cmp r5, #255
	ble .L_080f212e
	ldr r3, .L_080f21e4
.L_080f2128:
	adds r5, r5, r3
	cmp r5, #255
	bgt .L_080f2128
.L_080f212e:
	cmp r5, #0
	bge .L_080f213c
	movs r3, #128
	lsls r3, r3, #1
.L_080f2136:
	adds r5, r5, r3
	cmp r5, #0
	blt .L_080f2136
.L_080f213c:
	adds r3, r7, #4
	ldr r2, .L_080f21f8
	lsls r3, r3, #16
	orrs r3, r5
	orrs r3, r2
	movs r1, #56
	str r3, [r6, r1]
	movs r2, #60
	movs r3, #128
	str r3, [r6, r2]
	movs r3, #5
	mov r8, r3
	ldr r3, .L_080f21e0
	ldrb r3, [r3, #4]
	adds r7, r3, #0
	subs r3, #104
	mov r0, r9
	muls r0, r3
	movs r1, #80
	bl FixedPoint_Ratio
	add r0, r10
	adds r5, r0, #0
	subs r5, #32
	subs r7, #32
	cmp r5, #255
	ble .L_080f217a
	ldr r3, .L_080f21e4
.L_080f2174:
	adds r5, r5, r3
	cmp r5, #255
	bgt .L_080f2174
.L_080f217a:
	cmp r5, #0
	bge .L_080f2188
	movs r3, #128
	lsls r3, r3, #1
.L_080f2182:
	adds r5, r5, r3
	cmp r5, #0
	blt .L_080f2182
.L_080f2188:
	adds r0, r7, #4
	lsls r0, r0, #16
	mov r1, r8
	adds r3, r0, #0
	ldr r2, .L_080f21f8
	lsls r1, r1, #3
	orrs r3, r5
	mov r12, r1
	orrs r3, r2
	adds r1, #24
	str r3, [r6, r1]
	adds r1, r7, #0
	adds r1, #36
	lsls r1, r1, #16
	adds r3, r1, #0
	ldr r2, .L_080f21fc
	orrs r3, r5
	orrs r3, r2
	adds r2, r5, #0
	mov r4, r12
	adds r2, #32
	adds r4, #32
	lsls r2, r2, #24
	str r3, [r6, r4]
	lsrs r2, r2, #24
	ldr r3, .L_080f2200
	orrs r0, r2
	orrs r0, r3
	ldr r3, .L_080f2204
	orrs r1, r2
	adds r4, #8
	str r0, [r6, r4]
	orrs r1, r3
	mov r0, r12
	mov r3, r12
	movs r2, #192
	b .L_080f2396
	.2byte 0x0000
.L_080f21d4:
	.4byte gDisp + 0x4
.L_080f21d8:
	.4byte gDebugPaused
.L_080f21dc:
	.4byte gBgScroll
.L_080f21e0:
	.4byte Data_080f39ab
.L_080f21e4:
	.4byte 0xffffff00
.L_080f21e8:
	.4byte 0x40002400
.L_080f21ec:
	.4byte 0x50002400
.L_080f21f0:
	.4byte 0x60002400
.L_080f21f4:
	.4byte 0x70002400
.L_080f21f8:
	.4byte 0x80002400
.L_080f21fc:
	.4byte 0x90002400
.L_080f2200:
	.4byte 0xa0002400
.L_080f2204:
	.4byte 0xb0002400
.L_080f2208:
	ldr r3, .L_080f2404
	mov r2, r10
	ldrb r3, [r3, #1]
	subs r2, r1, r2
	adds r7, r3, #0
	mov r9, r2
	subs r3, #104
	mov r0, r9
	muls r0, r3
	movs r1, #80
	bl FixedPoint_Ratio
	add r0, r10
	adds r5, r0, #0
	subs r5, #16
	subs r7, #16
	cmp r5, #255
	ble .L_080f2234
	ldr r3, .L_080f2408
.L_080f222e:
	adds r5, r5, r3
	cmp r5, #255
	bgt .L_080f222e
.L_080f2234:
	cmp r5, #0
	bge .L_080f2242
	movs r3, #128
	lsls r3, r3, #1
.L_080f223c:
	adds r5, r5, r3
	cmp r5, #0
	blt .L_080f223c
.L_080f2242:
	adds r0, r7, #4
	lsls r0, r0, #16
	adds r3, r0, #0
	ldr r2, .L_080f240c
	orrs r3, r5
	orrs r3, r2
	movs r1, #24
	str r3, [r6, r1]
	adds r1, r7, #0
	adds r1, #20
	lsls r1, r1, #16
	adds r3, r1, #0
	ldr r2, .L_080f2410
	orrs r3, r5
	orrs r3, r2
	adds r2, r5, #0
	adds r2, #16
	movs r4, #32
	lsls r2, r2, #24
	str r3, [r6, r4]
	lsrs r2, r2, #24
	ldr r3, .L_080f2414
	orrs r0, r2
	orrs r0, r3
	ldr r3, .L_080f2418
	movs r4, #40
	orrs r1, r2
	str r0, [r6, r4]
	movs r2, #232
	orrs r1, r3
	movs r0, #48
	movs r3, #28
	str r1, [r6, r0]
	str r2, [r6, r3]
	movs r3, #36
	str r2, [r6, r3]
	movs r3, #44
	str r2, [r6, r3]
	movs r3, #52
	str r2, [r6, r3]
	ldr r3, .L_080f2404
	ldrb r3, [r3, #3]
	adds r7, r3, #0
	subs r3, #104
	mov r0, r9
	muls r0, r3
	movs r1, #80
	bl FixedPoint_Ratio
	add r0, r10
	adds r5, r0, #0
	subs r5, #16
	subs r7, #16
	cmp r5, #255
	ble .L_080f22b8
	ldr r3, .L_080f2408
.L_080f22b2:
	adds r5, r5, r3
	cmp r5, #255
	bgt .L_080f22b2
.L_080f22b8:
	cmp r5, #0
	bge .L_080f22c6
	movs r3, #128
	lsls r3, r3, #1
.L_080f22c0:
	adds r5, r5, r3
	cmp r5, #0
	blt .L_080f22c0
.L_080f22c6:
	adds r0, r7, #4
	lsls r0, r0, #16
	adds r3, r0, #0
	ldr r2, .L_080f240c
	orrs r3, r5
	orrs r3, r2
	movs r1, #56
	str r3, [r6, r1]
	adds r1, r7, #0
	adds r1, #20
	lsls r1, r1, #16
	adds r3, r1, #0
	ldr r2, .L_080f2410
	orrs r3, r5
	orrs r3, r2
	adds r2, r5, #0
	adds r2, #16
	movs r4, #64
	lsls r2, r2, #24
	str r3, [r6, r4]
	lsrs r2, r2, #24
	ldr r3, .L_080f2414
	orrs r0, r2
	orrs r0, r3
	ldr r3, .L_080f2418
	movs r4, #72
	orrs r1, r2
	str r0, [r6, r4]
	movs r2, #224
	orrs r1, r3
	movs r0, #80
	movs r3, #60
	str r1, [r6, r0]
	str r2, [r6, r3]
	movs r3, #68
	str r2, [r6, r3]
	movs r3, #76
	str r2, [r6, r3]
	movs r3, #84
	str r2, [r6, r3]
	movs r3, #8
	mov r8, r3
	ldr r3, .L_080f2404
	ldrb r3, [r3, #5]
	adds r7, r3, #0
	subs r3, #104
	mov r0, r9
	muls r0, r3
	movs r1, #80
	bl FixedPoint_Ratio
	add r0, r10
	adds r5, r0, #0
	subs r5, #32
	subs r7, #32
	cmp r5, #255
	ble .L_080f2340
	ldr r3, .L_080f2408
.L_080f233a:
	adds r5, r5, r3
	cmp r5, #255
	bgt .L_080f233a
.L_080f2340:
	cmp r5, #0
	bge .L_080f234e
	movs r3, #128
	lsls r3, r3, #1
.L_080f2348:
	adds r5, r5, r3
	cmp r5, #0
	blt .L_080f2348
.L_080f234e:
	adds r0, r7, #4
	lsls r0, r0, #16
	mov r1, r8
	adds r3, r0, #0
	ldr r2, .L_080f241c
	lsls r1, r1, #3
	orrs r3, r5
	mov r12, r1
	orrs r3, r2
	adds r1, #24
	str r3, [r6, r1]
	adds r1, r7, #0
	adds r1, #36
	lsls r1, r1, #16
	adds r3, r1, #0
	ldr r2, .L_080f2420
	orrs r3, r5
	orrs r3, r2
	adds r2, r5, #0
	mov r4, r12
	adds r2, #32
	adds r4, #32
	lsls r2, r2, #24
	str r3, [r6, r4]
	lsrs r2, r2, #24
	ldr r3, .L_080f2424
	orrs r0, r2
	orrs r0, r3
	ldr r3, .L_080f2428
	orrs r1, r2
	adds r4, #8
	str r0, [r6, r4]
	orrs r1, r3
	mov r0, r12
	mov r3, r12
	movs r2, #160
.L_080f2396:
	adds r3, #28
	adds r0, #48
	str r1, [r6, r0]
	str r2, [r6, r3]
	adds r3, #8
	str r2, [r6, r3]
	adds r3, #8
	str r2, [r6, r3]
	adds r3, #8
	str r2, [r6, r3]
	movs r2, #4
	add r8, r2
.L_080f23ae:
	mov r3, r8
	cmp r3, #119
	bhi .L_080f23c8
	lsls r3, r3, #3
	ldr r2, .L_080f242c
	adds r3, #24
.L_080f23ba:
	movs r1, #1
	add r8, r1
	mov r1, r8
	str r2, [r6, r3]
	adds r3, #8
	cmp r1, #119
	bls .L_080f23ba
.L_080f23c8:
	ldr r2, .L_080f2430
	ldr r3, .L_080f23fc
	strh r3, [r2]
	ldr r3, .L_080f2400
	adds r2, #2
	strh r3, [r2]
	mov r2, r8
	lsls r5, r2, #3
	adds r7, r6, #0
	movs r4, #132
	lsrs r2, r5, #2
	lsls r4, r4, #24
	adds r7, #24
	movs r1, #224
	ldr r3, .L_080f2434
	adds r0, r7, #0
	lsls r1, r1, #19
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_080f2438
	ldrh r2, [r3, #6]
	movs r3, #32
	subs r2, r3, r2
	b .L_080f243c
	.2byte 0x0000
.L_080f23fc:
	.4byte 0x00003f50
.L_080f2400:
	.4byte 0x00000e0e
.L_080f2404:
	.4byte Data_080f39ab
.L_080f2408:
	.4byte 0xffffff00
.L_080f240c:
	.4byte 0x40002400
.L_080f2410:
	.4byte 0x50002400
.L_080f2414:
	.4byte 0x60002400
.L_080f2418:
	.4byte 0x70002400
.L_080f241c:
	.4byte 0x80002400
.L_080f2420:
	.4byte 0x90002400
.L_080f2424:
	.4byte 0xa0002400
.L_080f2428:
	.4byte 0xb0002400
.L_080f242c:
	.4byte 0x400020a0
.L_080f2430:
	.4byte 0x04000050
.L_080f2434:
	.4byte 0x040000d4
.L_080f2438:
	.4byte gBgScroll
.L_080f243c:
	cmp r2, #255
	ble .L_080f2448
	ldr r3, .L_080f248c
.L_080f2442:
	adds r2, r2, r3
	cmp r2, #255
	bgt .L_080f2442
.L_080f2448:
	cmp r2, #0
	bge .L_080f2456
	movs r3, #128
	lsls r3, r3, #1
.L_080f2450:
	adds r2, r2, r3
	cmp r2, #0
	blt .L_080f2450
.L_080f2456:
	ldr r3, .L_080f2490
	movs r4, #224
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #4
	lsls r4, r4, #19
	adds r0, r6, #0
	str r2, [r6, #120]
	adds r0, #120
	str r3, [r6, #124]
	adds r1, r5, r4
	ldr r3, .L_080f2494
	ldr r2, .L_080f2498
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r7, #0
	adds r1, r4, #0
	ldr r2, .L_080f249c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080f248c:
	.4byte 0xffffff00
.L_080f2490:
	.4byte 0xc05c2000
.L_080f2494:
	.4byte 0x040000d4
.L_080f2498:
	.4byte 0x84000002
.L_080f249c:
	.4byte 0x84000008
