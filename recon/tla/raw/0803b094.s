.syntax unified
	.thumb
	.global UiText_BuildRenderEntries
	.thumb_func
UiText_BuildRenderEntries:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #132
	str r0, [sp, #48]
	movs r0, #192
	lsls r0, r0, #18
	mov r11, r1
	ldr r1, [r0, #60]
	movs r2, #1
	movs r3, #0
	movs r5, #152
	str r1, [sp, #44]
	str r2, [sp, #40]
	str r3, [sp, #36]
	lsls r5, r5, #5
	adds r5, #66
	adds r3, r1, r5
	ldrh r3, [r3]
	str r2, [sp, #20]
	str r3, [sp, #32]
	adds r6, r3, #0
	movs r2, #244
	ldr r3, [sp, #48]
	lsls r2, r2, #4
	movs r5, #1
	mov r10, r0
	movs r7, #0
	movs r0, #0
	adds r1, r1, r2
	negs r5, r5
	str r0, [sp, #28]
	str r0, [sp, #24]
	str r7, [sp, #52]
	mov r8, r1
	str r0, [sp, #16]
	cmp r3, r5
	bne .L_0803b0f8
	ldr r0, [sp, #44]
	movs r1, #152
	lsls r1, r1, #5
	adds r1, #68
	adds r3, r0, r1
	ldrh r3, [r3]
	str r3, [sp, #32]
	b .L_0803b778
.L_0803b0f8:
	ldr r5, .L_0803b1e0
	movs r0, #200
	adds r1, r5, #0
	bl Runtime_AllocateHeapBlock
	movs r2, #132
	movs r3, #128
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r1, r0, #0
	adds r3, #212
	ldr r0, .L_0803b1e4
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r3, r10
	adds r3, #200
	movs r2, #56
	ldr r3, [r3]
	add r2, sp
	mov r10, r2
	mov r0, r10
	ldr r1, [sp, #48]
	mov r9, r3
	bl Func_0803d178
	mov r3, sp
	adds r3, #84
	str r3, [sp, #12]
.L_0803b134:
	mov r0, r10
	mov lr, r9
	.2byte 0xf800
	adds r5, r7, #0
	adds r7, r0, #0
	cmp r7, #255
	bls .L_0803b144
	movs r7, #64
.L_0803b144:
	ldr r0, [sp, #16]
	cmp r0, #0
	beq .L_0803b1e8
	cmp r7, #31
	bls .L_0803b154
	cmp r7, #176
	beq .L_0803b154
	b .L_0803b71c
.L_0803b154:
	cmp r7, #18
	beq .L_0803b1ce
	cmp r7, #18
	bhi .L_0803b17e
	cmp r7, #9
	bhi .L_0803b172
	cmp r7, #8
	bcs .L_0803b1ce
	cmp r7, #1
	beq .L_0803b1d6
	cmp r7, #1
	bcc .L_0803b1ac
	cmp r7, #2
	beq .L_0803b1ac
	b .L_0803b71c
.L_0803b172:
	cmp r7, #16
	bne .L_0803b178
	b .L_0803b71c
.L_0803b178:
	cmp r7, #17
	beq .L_0803b1ce
	b .L_0803b71c
.L_0803b17e:
	cmp r7, #22
	beq .L_0803b1b2
	cmp r7, #22
	bhi .L_0803b198
	cmp r7, #20
	beq .L_0803b1b6
	cmp r7, #20
	bhi .L_0803b1c0
	mov r0, r10
	mov lr, r9
	.2byte 0xf800
	movs r0, #3
	b .L_0803b1c6
.L_0803b198:
	cmp r7, #29
	beq .L_0803b1ce
	cmp r7, #29
	bhi .L_0803b1a6
	cmp r7, #23
	beq .L_0803b1c4
	b .L_0803b71c
.L_0803b1a6:
	cmp r7, #30
	beq .L_0803b1ac
	b .L_0803b71c
.L_0803b1ac:
	movs r1, #0
	str r1, [sp, #20]
	b .L_0803b71c
.L_0803b1b2:
	movs r0, #5
	b .L_0803b1c6
.L_0803b1b6:
	mov r0, r10
	mov lr, r9
	.2byte 0xf800
	movs r0, #2
	b .L_0803b1c6
.L_0803b1c0:
	movs r0, #4
	b .L_0803b1c6
.L_0803b1c4:
	movs r0, #6
.L_0803b1c6:
	mov r1, r11
	bl Func_0803cd08
	b .L_0803b71c
.L_0803b1ce:
	mov r0, r10
	mov lr, r9
	.2byte 0xf800
	b .L_0803b71c
.L_0803b1d6:
	movs r2, #0
	str r2, [sp, #20]
	movs r7, #2
	b .L_0803b71c
	.2byte 0x0000
.L_0803b1e0:
	.4byte 0x00000144
.L_0803b1e4:
	.4byte Text_DecodeSymbolCode
.L_0803b1e8:
	ldr r0, [sp, #44]
	movs r1, #152
	lsls r1, r1, #5
	adds r1, #138
	adds r3, r0, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0803b220
	ldr r2, [sp, #40]
	cmp r2, #0
	bne .L_0803b220
	cmp r7, #222
	beq .L_0803b220
	cmp r7, #223
	beq .L_0803b220
	ldr r3, .L_0803b21c
	lsls r2, r6, #1
	mov r0, r8
	movs r1, #128
	lsls r1, r1, #1
	strh r3, [r2, r0]
	adds r6, #1
	adds r1, #255
	ands r6, r1
	b .L_0803b220
	.2byte 0x0000
.L_0803b21c:
	.4byte 0x00000005
.L_0803b220:
	ldr r2, [sp, #44]
	movs r0, #152
	lsls r0, r0, #5
	adds r0, #139
	adds r3, r2, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0803b27c
	ldr r1, [sp, #40]
	cmp r1, #0
	bne .L_0803b27c
	cmp r7, #222
	beq .L_0803b27c
	cmp r7, #223
	beq .L_0803b27c
	movs r2, #128
	lsls r2, r2, #1
	cmp r5, r2
	bhi .L_0803b27c
	cmp r5, #127
	bls .L_0803b27c
	cmp r5, #222
	beq .L_0803b27c
	cmp r5, #223
	beq .L_0803b27c
	cmp r5, #32
	beq .L_0803b27c
	cmp r5, #165
	beq .L_0803b27c
	cmp r5, #161
	beq .L_0803b27c
	cmp r5, #164
	beq .L_0803b27c
	ldr r3, .L_0803b278
	lsls r2, r6, #1
	mov r5, r8
	movs r0, #128
	lsls r0, r0, #1
	strh r3, [r2, r5]
	adds r6, #1
	adds r0, #255
	ands r6, r0
	b .L_0803b27c
	.2byte 0x0000
.L_0803b278:
	.4byte 0x000000de
.L_0803b27c:
	cmp r7, #31
	bls .L_0803b2fc
	cmp r7, #176
	beq .L_0803b2fc
	ldr r1, [sp, #44]
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #138
	adds r3, r1, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0803b2d4
	cmp r7, #32
	beq .L_0803b29e
	ldr r3, [sp, #28]
	cmp r3, #10
	bls .L_0803b2d4
.L_0803b29e:
	movs r0, #128
	ldr r2, .L_0803b2d0
	lsls r0, r0, #1
	adds r0, #255
	lsls r3, r6, #1
	adds r6, #1
	ands r6, r0
	mov r5, r8
	strh r2, [r3, r5]
	lsls r3, r6, #1
	adds r6, #1
	ands r6, r0
	mov r1, r8
	strh r2, [r3, r1]
	lsls r3, r6, #1
	strh r2, [r3, r5]
	ldr r1, [sp, #28]
	adds r6, #1
	ands r6, r0
	movs r0, #1
	str r0, [sp, #16]
	cmp r1, #10
	bls .L_0803b2d4
	movs r7, #32
	b .L_0803b2d4
.L_0803b2d0:
	.4byte 0x0000002e
.L_0803b2d4:
	cmp r7, #34
	bne .L_0803b2e6
	ldr r2, [sp, #36]
	movs r3, #1
	eors r2, r3
	str r2, [sp, #36]
	cmp r2, #0
	beq .L_0803b2e6
	movs r7, #142
.L_0803b2e6:
	movs r0, #128
	lsls r0, r0, #1
	lsls r3, r6, #1
	mov r5, r8
	adds r6, #1
	adds r0, #255
	movs r1, #0
	strh r7, [r3, r5]
	ands r6, r0
	str r1, [sp, #40]
	b .L_0803b71c
.L_0803b2fc:
	cmp r7, #20
	bne .L_0803b302
	b .L_0803b464
.L_0803b302:
	cmp r7, #20
	bhi .L_0803b342
	cmp r7, #9
	bhi .L_0803b324
	cmp r7, #8
	bcs .L_0803b39c
	cmp r7, #1
	bne .L_0803b314
	b .L_0803b6f8
.L_0803b314:
	cmp r7, #1
	bcc .L_0803b396
	cmp r7, #2
	beq .L_0803b396
	cmp r7, #3
	bne .L_0803b322
	b .L_0803b6f8
.L_0803b322:
	b .L_0803b6fc
.L_0803b324:
	cmp r7, #17
	bne .L_0803b32a
	b .L_0803b57c
.L_0803b32a:
	cmp r7, #17
	bhi .L_0803b336
	cmp r7, #16
	bne .L_0803b334
	b .L_0803b51e
.L_0803b334:
	b .L_0803b6fc
.L_0803b336:
	cmp r7, #18
	bne .L_0803b33c
	b .L_0803b542
.L_0803b33c:
	cmp r7, #19
	beq .L_0803b432
	b .L_0803b6fc
.L_0803b342:
	cmp r7, #27
	bne .L_0803b348
	b .L_0803b6c8
.L_0803b348:
	cmp r7, #27
	bhi .L_0803b370
	cmp r7, #23
	bne .L_0803b352
	b .L_0803b4dc
.L_0803b352:
	cmp r7, #23
	bhi .L_0803b362
	cmp r7, #21
	bne .L_0803b35c
	b .L_0803b49e
.L_0803b35c:
	cmp r7, #22
	beq .L_0803b3cc
	b .L_0803b6fc
.L_0803b362:
	cmp r7, #25
	bne .L_0803b368
	b .L_0803b68c
.L_0803b368:
	cmp r7, #26
	bne .L_0803b36e
	b .L_0803b640
.L_0803b36e:
	b .L_0803b6fc
.L_0803b370:
	cmp r7, #30
	beq .L_0803b396
	cmp r7, #30
	bhi .L_0803b384
	cmp r7, #28
	bne .L_0803b37e
	b .L_0803b5b6
.L_0803b37e:
	cmp r7, #29
	beq .L_0803b39c
	b .L_0803b6fc
.L_0803b384:
	cmp r7, #176
	bne .L_0803b38a
	b .L_0803b66c
.L_0803b38a:
	movs r2, #1
	negs r2, r2
	cmp r7, r2
	bne .L_0803b394
	b .L_0803b71c
.L_0803b394:
	b .L_0803b6fc
.L_0803b396:
	movs r3, #0
	str r3, [sp, #20]
	b .L_0803b71c
.L_0803b39c:
	movs r0, #128
	lsls r0, r0, #1
	lsls r3, r6, #1
	adds r0, #255
	mov r5, r8
	adds r6, #1
	ands r6, r0
	strh r7, [r3, r5]
	mov r0, r10
	mov lr, r9
	.2byte 0xf800
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	lsls r3, r6, #1
	adds r0, r0, r1
	mov r2, r8
	strh r0, [r3, r2]
	movs r3, #128
	lsls r3, r3, #1
	adds r6, #1
	adds r3, #255
	ands r6, r3
	b .L_0803b71c
.L_0803b3cc:
	mov r1, r11
	movs r0, #5
	bl Func_0803cd08
	adds r1, r0, #0
	adds r3, r1, #0
	cmp r1, #0
	bge .L_0803b3de
	negs r3, r1
.L_0803b3de:
	movs r5, #1
	str r5, [sp, #24]
	cmp r3, #1
	bgt .L_0803b3ea
	movs r0, #0
	str r0, [sp, #24]
.L_0803b3ea:
	add r5, sp, #68
	adds r0, r5, #0
	movs r2, #0
	bl Func_0803ae14
	subs r4, r0, r5
	cmp r4, #16
	bne .L_0803b3fc
	b .L_0803b71c
.L_0803b3fc:
	ldrb r3, [r5, r4]
	cmp r3, #0
	bne .L_0803b404
	b .L_0803b71c
.L_0803b404:
	movs r1, #128
	lsls r1, r1, #1
	adds r1, #255
	adds r0, r4, r5
	mov r12, r1
	adds r1, r0, #0
.L_0803b410:
	ldrb r3, [r1]
	lsls r2, r6, #1
	mov r5, r8
	strh r3, [r2, r5]
	adds r6, #1
	mov r2, r12
	adds r4, #1
	adds r1, #1
	ands r6, r2
	cmp r4, #16
	bne .L_0803b428
	b .L_0803b71c
.L_0803b428:
	adds r0, #1
	ldrb r3, [r0]
	cmp r3, #0
	bne .L_0803b410
	b .L_0803b71c
.L_0803b432:
	mov r0, r10
	mov lr, r9
	.2byte 0xf800
	mov r1, r11
	subs r5, r0, #1
	movs r0, #3
	bl Func_0803cd08
	adds r2, r0, #0
	ldr r0, .L_0803b62c
	ldr r1, [sp, #12]
	adds r0, r2, r0
	movs r2, #24
	bl UiText_DecodeMessage
	ldr r3, [sp, #24]
	adds r2, r6, #0
	str r3, [sp, #4]
	add r3, sp, #52
	str r3, [sp, #8]
	movs r0, #0
	ldr r1, [sp, #12]
	mov r3, r8
	str r5, [sp, #0]
	b .L_0803b5ae
.L_0803b464:
	mov r0, r10
	mov lr, r9
	.2byte 0xf800
	mov r1, r11
	subs r5, r0, #1
	movs r0, #2
	bl Func_0803cd08
	adds r2, r0, #0
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r2, r0
	ldr r0, .L_0803b630
	ldr r1, [sp, #12]
	adds r0, r2, r0
	movs r2, #24
	bl UiText_DecodeMessage
	ldr r1, [sp, #24]
	add r3, sp, #52
	str r1, [sp, #4]
	str r3, [sp, #8]
	adds r2, r6, #0
	movs r0, #0
	ldr r1, [sp, #12]
	mov r3, r8
	str r5, [sp, #0]
	b .L_0803b5ae
.L_0803b49e:
	mov r1, r11
	movs r0, #4
	bl Func_0803cd08
	adds r2, r0, #0
	ldr r0, .L_0803b634
	ldr r1, [sp, #12]
	adds r0, r2, r0
	movs r2, #24
	bl UiText_DecodeMessage
	ldr r1, [sp, #12]
	adds r0, r6, #0
	ldrh r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0803b5b2
	movs r4, #128
	lsls r4, r4, #1
	adds r4, #255
.L_0803b4c6:
	lsls r3, r0, #1
	mov r5, r8
	strh r2, [r3, r5]
	adds r1, #2
	ldrh r2, [r1]
	adds r0, #1
	adds r3, r2, #0
	ands r0, r4
	cmp r3, #0
	bne .L_0803b4c6
	b .L_0803b5b2
.L_0803b4dc:
	mov r1, r11
	movs r0, #6
	bl Func_0803cd08
	movs r1, #1
	bl Func_080c8648
	ldr r3, .L_0803b638
	ldr r1, [sp, #12]
	adds r0, r0, r3
	movs r2, #24
	bl UiText_DecodeMessage
	ldr r1, [sp, #12]
	adds r0, r6, #0
	ldrh r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0803b5b2
	movs r4, #128
	lsls r4, r4, #1
	adds r4, #255
.L_0803b508:
	lsls r3, r0, #1
	mov r5, r8
	strh r2, [r3, r5]
	adds r1, #2
	ldrh r2, [r1]
	adds r0, #1
	adds r3, r2, #0
	ands r0, r4
	cmp r3, #0
	bne .L_0803b508
	b .L_0803b5b2
.L_0803b51e:
	ldr r3, .L_0803b63c
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Owner_GetState
	add r1, sp, #84
	adds r2, r1, #0
	movs r4, #0
.L_0803b532:
	ldrb r3, [r0]
	adds r4, #1
	strh r3, [r2]
	adds r0, #1
	adds r2, #2
	cmp r4, #14
	bls .L_0803b532
	b .L_0803b59e
.L_0803b542:
	mov r0, r10
	mov lr, r9
	.2byte 0xf800
	mov r1, r11
	subs r5, r0, #1
	movs r0, #1
	bl Func_0803cd08
	bl Owner_GetState
	add r1, sp, #84
	adds r2, r1, #0
	movs r4, #0
.L_0803b55c:
	ldrb r3, [r0]
	adds r4, #1
	strh r3, [r2]
	adds r0, #1
	adds r2, #2
	cmp r4, #14
	bls .L_0803b55c
	ldr r2, [sp, #24]
	add r3, sp, #52
	str r2, [sp, #4]
	str r3, [sp, #8]
	adds r2, r6, #0
	movs r0, #0
	mov r3, r8
	str r5, [sp, #0]
	b .L_0803b5ae
.L_0803b57c:
	mov r0, r10
	mov lr, r9
	.2byte 0xf800
	subs r2, r0, #1
	adds r0, r2, #0
	bl Owner_GetState
	add r1, sp, #84
	adds r2, r1, #0
	movs r4, #0
.L_0803b590:
	ldrb r3, [r0]
	adds r4, #1
	strh r3, [r2]
	adds r0, #1
	adds r2, #2
	cmp r4, #14
	bls .L_0803b590
.L_0803b59e:
	movs r3, #0
	str r3, [sp, #0]
	str r3, [sp, #4]
	add r3, sp, #52
	str r3, [sp, #8]
	adds r2, r6, #0
	movs r0, #0
	mov r3, r8
.L_0803b5ae:
	bl Func_0803aed8
.L_0803b5b2:
	adds r6, r0, #0
	b .L_0803b71c
.L_0803b5b6:
	mov r0, r10
	mov lr, r9
	.2byte 0xf800
	subs r2, r0, #1
	adds r0, r2, #0
	bl Owner_GetState
	adds r5, r0, #0
	mov r0, r10
	mov lr, r9
	.2byte 0xf800
	movs r4, #0
	mov r12, r0
	add r1, sp, #84
	adds r0, r5, #0
	adds r2, r1, #0
	cmp r4, r12
	bcs .L_0803b5fe
.L_0803b5da:
	ldrb r3, [r0]
	adds r0, #1
	strh r3, [r2]
	movs r5, #128
	ldrb r3, [r0]
	lsls r5, r5, #17
	adds r3, #34
	lsls r3, r3, #24
	adds r2, #2
	cmp r3, r5
	bhi .L_0803b5f8
	ldrb r3, [r0]
	adds r0, #1
	strh r3, [r2]
	adds r2, #2
.L_0803b5f8:
	adds r4, #1
	cmp r4, r12
	bcc .L_0803b5da
.L_0803b5fe:
	ldr r3, .L_0803b628
	adds r0, r6, #0
	strh r3, [r2]
	ldrh r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0803b5b2
	movs r4, #128
	lsls r4, r4, #1
	adds r4, #255
.L_0803b612:
	lsls r3, r0, #1
	mov r5, r8
	strh r2, [r3, r5]
	adds r1, #2
	ldrh r2, [r1]
	adds r0, #1
	adds r3, r2, #0
	ands r0, r4
	cmp r3, #0
	bne .L_0803b612
	b .L_0803b5b2
.L_0803b628:
	.4byte 0x00000000
.L_0803b62c:
	.4byte 0x00000b63
.L_0803b630:
	.4byte 0x0000025f
.L_0803b634:
	.4byte 0x000005a7
.L_0803b638:
	.4byte 0x00000e58
.L_0803b63c:
	.4byte gPartyState
.L_0803b640:
	mov r0, r10
	mov lr, r9
	.2byte 0xf800
	subs r0, #1
	lsls r0, r0, #1
	adds r2, r0, #0
	lsls r3, r6, #1
	adds r2, #128
	mov r1, r8
	strh r2, [r3, r1]
	movs r2, #128
	lsls r2, r2, #1
	adds r2, #255
	adds r6, #1
	ands r6, r2
	lsls r3, r6, #1
	adds r0, #129
	mov r5, r8
	adds r6, #1
	strh r0, [r3, r5]
	ands r6, r2
	b .L_0803b71c
.L_0803b66c:
	ldr r3, .L_0803b684
	movs r1, #128
	lsls r2, r6, #1
	mov r0, r8
	lsls r1, r1, #1
	strh r3, [r2, r0]
	adds r1, #255
	adds r6, #1
	ldr r3, .L_0803b688
	ands r6, r1
	b .L_0803b6e2
	.2byte 0x0000
.L_0803b684:
	.4byte 0x0000008f
.L_0803b688:
	.4byte 0x0000002d
.L_0803b68c:
	ldr r0, [sp, #24]
	cmp r0, #0
	beq .L_0803b71c
	ldr r3, [sp, #52]
	cmp r3, #0
	beq .L_0803b6aa
	ldr r3, .L_0803b6c0
	lsls r2, r6, #1
	mov r1, r8
	strh r3, [r2, r1]
	movs r2, #128
	lsls r2, r2, #1
	adds r6, #1
	adds r2, #255
	ands r6, r2
.L_0803b6aa:
	ldr r3, .L_0803b6c4
	movs r0, #128
	lsls r0, r0, #1
	lsls r2, r6, #1
	mov r5, r8
	adds r6, #1
	adds r0, #255
	strh r3, [r2, r5]
	ands r6, r0
	b .L_0803b71c
	.2byte 0x0000
.L_0803b6c0:
	.4byte 0x00000065
.L_0803b6c4:
	.4byte 0x00000073
.L_0803b6c8:
	ldr r2, .L_0803b6f0
	lsls r3, r6, #1
	mov r1, r8
	strh r2, [r3, r1]
	movs r1, #128
	ldr r3, [sp, #52]
	lsls r1, r1, #1
	adds r6, #1
	adds r1, #255
	ands r6, r1
	cmp r3, #0
	bne .L_0803b71c
	ldr r3, .L_0803b6f4
.L_0803b6e2:
	lsls r2, r6, #1
	mov r5, r8
	adds r6, #1
	strh r3, [r2, r5]
	ands r6, r1
	b .L_0803b71c
	.2byte 0x0000
.L_0803b6f0:
	.4byte 0x00000027
.L_0803b6f4:
	.4byte 0x00000073
.L_0803b6f8:
	movs r0, #1
	str r0, [sp, #40]
.L_0803b6fc:
	movs r2, #128
	lsls r2, r2, #1
	lsls r3, r6, #1
	mov r1, r8
	adds r6, #1
	adds r2, #255
	strh r7, [r3, r1]
	ands r6, r2
	cmp r7, #115
	beq .L_0803b714
	cmp r7, #83
	bne .L_0803b718
.L_0803b714:
	movs r3, #1
	b .L_0803b71a
.L_0803b718:
	movs r3, #0
.L_0803b71a:
	str r3, [sp, #52]
.L_0803b71c:
	ldr r3, [sp, #28]
	ldr r5, [sp, #20]
	adds r3, #1
	str r3, [sp, #28]
	cmp r5, #0
	beq .L_0803b734
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	cmp r3, r0
	bhi .L_0803b734
	b .L_0803b134
.L_0803b734:
	lsls r3, r6, #1
	mov r1, r8
	strh r7, [r3, r1]
	movs r1, #128
	lsls r1, r1, #1
	adds r1, #255
	ldr r3, .L_0803b774
	adds r6, #1
	ands r6, r1
	lsls r2, r6, #1
	mov r5, r8
	strh r3, [r2, r5]
	adds r3, r6, #1
	ands r3, r1
	ldr r0, [sp, #44]
	movs r1, #152
	lsls r1, r1, #5
	adds r1, #66
	adds r2, r0, r1
	strh r3, [r2]
	movs r0, #200
	bl Runtime_ReleaseHeapBlock
	movs r5, #152
	ldr r2, [sp, #44]
	add r0, sp, #32
	lsls r5, r5, #5
	ldrh r0, [r0]
	adds r5, #68
	adds r3, r2, r5
	strh r0, [r3]
	b .L_0803b778
.L_0803b774:
	.4byte 0x00000000
.L_0803b778:
	mov r1, r11
	cmp r1, #0
	beq .L_0803b782
	bl Func_0803cca8
.L_0803b782:
	ldr r0, [sp, #32]
	add sp, #132
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
