.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {r5, r6, lr}
	adds r4, r1, #0
	movs r1, #192
	lsls r1, r1, #18
	adds r1, #128
	ldr r6, [r1]
	ldr r1, .L_020080dc
	adds r5, r0, #0
	str r5, [r1]
	ldr r1, .L_020080e0
	str r4, [r1]
	ldr r1, .L_020080e4
	str r2, [r1]
	ldr r2, .L_020080e8
	str r3, [r2]
	movs r2, #255
	ldrh r3, [r5]
	b .L_02008082
.L_0200805c:
	ldrh r0, [r4]
	adds r4, #2
	ldrh r2, [r4]
	adds r4, #2
	ldrh r1, [r5]
	ldrh r3, [r4]
	adds r5, #2
	adds r4, #2
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	movs r2, #160
	lsls r2, r2, #19
	lsls r1, r1, #1
	orrs r3, r0
	adds r1, r1, r2
	strh r3, [r1]
	ldrh r3, [r5]
	movs r2, #255
.L_02008082:
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_02008090
	ldrh r3, [r4]
	cmp r3, r2
	bne .L_0200805c
.L_02008090:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r0, #160
	lsls r2, r2, #24
	adds r3, #212
	lsls r0, r0, #19
	adds r1, r6, #0
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #224
	lsls r2, r2, #1
	adds r1, r6, r2
	movs r2, #132
	lsls r2, r2, #24
	ldr r0, .L_020080ec
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_020041cc
	ldr r3, .L_020080f0
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #2
	bne .L_020080da
	bl Func_02000328
.L_020080da:
	pop {r5, r6, pc}
.L_020080dc:
	.4byte gOverlayArea + 0x6010
.L_020080e0:
	.4byte gOverlayArea + 0x6014
.L_020080e4:
	.4byte gOverlayArea + 0x6018
.L_020080e8:
	.4byte gOverlayArea + 0x6004
.L_020080ec:
	.4byte 0x05000200
.L_020080f0:
	.4byte gPartyState
	.section .text.x020080f4,"ax",%progbits
	.global Func_020000f4
	.thumb_func
Func_020000f4:
	push {r5, lr}
	ldr r2, .L_02008138
	ldr r3, .L_02008128
	ldr r5, .L_0200813c
	strh r3, [r2]
	ldr r3, .L_02008140
	ldr r0, [r3]
	bl Func_020001d4
	ldr r2, .L_02008144
	ldr r3, .L_0200812c
	strh r0, [r5]
	strh r3, [r2]
	ldr r2, .L_02008148
	ldr r3, .L_02008130
	movs r1, #144
	strh r3, [r2]
	ldr r2, .L_0200814c
	ldr r3, .L_02008134
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_02008150
	bl Func_02003f7c
	b .L_02008154
	.2byte 0x0000
.L_02008128:
	.4byte 0x00000000
.L_0200812c:
	.4byte 0x0000000f
.L_02008130:
	.4byte 0x00000010
.L_02008134:
	.4byte 0x00000001
.L_02008138:
	.4byte gOverlayArea + 0x6020
.L_0200813c:
	.4byte gOverlayArea + 0x601c
.L_02008140:
	.4byte gOverlayArea + 0x6010
.L_02008144:
	.4byte gOverlayArea + 0x600c
.L_02008148:
	.4byte gOverlayArea + 0x6008
.L_0200814c:
	.4byte gOverlayArea + 0x6000
.L_02008150:
	.4byte Func_020001f8
.L_02008154:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008158,"ax",%progbits
	.global Func_02000158
	.thumb_func
Func_02000158:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	ldr r0, .L_02008174
	bl Scheduler_RemoveCallbackFar
	pop {pc}
	.2byte 0x0000
.L_02008174:
	.4byte Func_020001f8
	.section .text.x02008178,"ax",%progbits
	.global Func_02000178
	.thumb_func
Func_02000178:
	push {r5, lr}
	ldr r2, .L_020081b4
	ldr r3, .L_020081a8
	ldr r5, .L_020081b8
	strh r3, [r2]
	ldr r3, .L_020081bc
	ldr r0, [r3]
	bl Func_020001d4
	ldr r2, .L_020081ac
	ldr r3, .L_020081c0
	strh r0, [r5]
	strh r2, [r3]
	ldr r3, .L_020081c4
	movs r1, #144
	strh r2, [r3]
	ldr r2, .L_020081c8
	ldr r3, .L_020081b0
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_020081cc
	bl Func_02003f7c
	b .L_020081d0
.L_020081a8:
	.4byte 0x00000000
.L_020081ac:
	.4byte 0x00000002
.L_020081b0:
	.4byte 0x00000001
.L_020081b4:
	.4byte gOverlayArea + 0x6020
.L_020081b8:
	.4byte gOverlayArea + 0x601c
.L_020081bc:
	.4byte gOverlayArea + 0x6010
.L_020081c0:
	.4byte gOverlayArea + 0x600c
.L_020081c4:
	.4byte gOverlayArea + 0x6008
.L_020081c8:
	.4byte gOverlayArea + 0x6000
.L_020081cc:
	.4byte Func_020001f8
.L_020081d0:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020081d4,"ax",%progbits
	.global Func_020001d4
	.thumb_func
Func_020001d4:
	push {lr}
	ldr r1, .L_020081ec
	ldrh r3, [r0]
	movs r2, #0
	cmp r3, r1
	beq .L_020081f0
.L_020081e0:
	adds r0, #2
	ldrh r3, [r0]
	adds r2, #1
	cmp r3, r1
	bne .L_020081e0
	b .L_020081f0
.L_020081ec:
	.4byte 0x0000ffff
.L_020081f0:
	subs r2, #1
	adds r0, r2, #0
	pop {pc}
	.2byte 0x0000
	.section .text.x020081f8,"ax",%progbits
	.global Func_020001f8
	.thumb_func
Func_020001f8:
	push {r5, r6, r7, lr}
	ldr r1, .L_020082a8
	movs r4, #0
	ldrh r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_02008232
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r2, r0
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02008232
	ldr r0, .L_020082ac
	movs r4, #1
	ldrh r2, [r0]
	strh r2, [r1]
	movs r1, #128
	lsls r3, r2, #16
	lsls r1, r1, #10
	cmp r3, r1
	bls .L_02008232
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
	strh r3, [r0]
.L_02008232:
	cmp r4, #0
	bne .L_02008238
	b .L_02008324
.L_02008238:
	ldr r3, .L_020082b0
	ldr r6, .L_020082b4
	ldr r1, [r3]
	ldrh r3, [r6]
	movs r5, #0
	cmp r5, r3
	bcs .L_02008286
	ldr r3, .L_020082b8
	ldr r2, .L_020082bc
	ldr r7, [r3]
	mov lr, r2
	mov r12, r6
.L_02008250:
	mov r3, lr
	ldrh r2, [r3]
	ldrh r3, [r6]
	movs r0, #160
	muls r3, r2
	adds r3, r3, r5
	lsls r3, r3, #1
	ldrh r3, [r3, r7]
	lsls r0, r0, #19
	lsls r3, r3, #1
	adds r4, r3, r0
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	adds r1, #2
	ldrh r3, [r1]
	adds r1, #2
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	adds r5, #1
	mov r2, r12
	ldrh r3, [r2]
	cmp r5, r3
	bcc .L_02008250
.L_02008286:
	ldr r3, .L_020082b4
	movs r0, #160
	ldrh r1, [r3]
	ldr r3, .L_020082c0
	lsls r2, r1, #1
	ldr r3, [r3]
	lsls r0, r0, #19
	ldrh r3, [r2, r3]
	adds r2, r2, r1
	lsls r3, r3, #1
	adds r4, r3, r0
	ldr r3, .L_020082c4
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_020082cc
	ldr r3, .L_020082c8
	b .L_020082ce
.L_020082a8:
	.4byte gOverlayArea + 0x6008
.L_020082ac:
	.4byte gOverlayArea + 0x600c
.L_020082b0:
	.4byte gOverlayArea + 0x6018
.L_020082b4:
	.4byte gOverlayArea + 0x601c
.L_020082b8:
	.4byte gOverlayArea + 0x6004
.L_020082bc:
	.4byte gOverlayArea + 0x6020
.L_020082c0:
	.4byte gOverlayArea + 0x6010
.L_020082c4:
	.4byte gOverlayArea + 0x6000
.L_020082c8:
	.4byte gOverlayArea + 0x6014
.L_020082cc:
	ldr r3, .L_02008314
.L_020082ce:
	lsls r2, r2, #1
	ldr r3, [r3]
	adds r1, r3, r2
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	ldrh r3, [r1, #2]
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	ldr r1, .L_02008318
	ldr r2, .L_0200830c
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	ldr r1, .L_0200831c
	ldr r2, .L_02008320
	ldrh r3, [r1]
	adds r3, #1
	strh r3, [r1]
	lsls r3, r3, #16
	ldrh r2, [r2]
	lsrs r3, r3, #16
	cmp r3, r2
	bne .L_02008324
	ldr r3, .L_02008310
	strh r3, [r1]
	b .L_02008324
	.2byte 0x0000
.L_0200830c:
	.4byte 0x00000001
.L_02008310:
	.4byte 0x00000000
.L_02008314:
	.4byte gOverlayArea + 0x6018
.L_02008318:
	.4byte gOverlayArea + 0x6000
.L_0200831c:
	.4byte gOverlayArea + 0x6020
.L_02008320:
	.4byte gOverlayArea + 0x601c
.L_02008324:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008328,"ax",%progbits
	.global Func_02000328
	.thumb_func
Func_02000328:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	movs r6, #192
	lsls r6, r6, #18
	ldr r5, [r6, #108]
	movs r1, #214
	lsls r1, r1, #1
	mov r8, r1
	add r5, r8
	ldr r2, [r5]
	ldr r0, .L_020083a8
	movs r1, #1
	mov r10, r2
	bl Func_020041cc
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	bl Func_020041cc
	ldr r2, [r6, #108]
	movs r3, #128
	lsls r3, r3, #1
	mov r1, r8
	str r3, [r2, r1]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02000178
	bl Func_02004284
	movs r0, #40
	bl WaitFrames
	bl Func_02000158
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl Func_020041cc
	movs r0, #16
	bl Func_020041dc
	movs r0, #16
	bl WaitFrames
	ldr r3, .L_020083ac
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	movs r2, #0
	strb r2, [r3]
	mov r3, r10
	str r3, [r5]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_020083a8:
	.4byte 0x00202108
.L_020083ac:
	.4byte gPartyState
	.section .text.x020083b0,"ax",%progbits
	.global Func_020003b0
	.thumb_func
Func_020003b0:
	push {r5, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	cmp r5, #0
	beq .L_020083f4
	adds r3, r5, #0
	adds r3, #84
	ldrb r2, [r3]
	movs r3, #15
	ands r3, r2
	cmp r3, #0
	beq .L_020083f4
	ldr r1, [r5, #80]
	movs r2, #13
	ldrb r0, [r1, #9]
	movs r3, #3
	negs r2, r2
	ands r4, r3
	adds r3, r2, #0
	lsls r4, r4, #2
	ands r3, r0
	orrs r3, r4
	strb r3, [r1, #9]
	adds r1, #37
	ldrb r3, [r1]
	ands r2, r3
	orrs r2, r4
	strb r2, [r1]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
.L_020083f4:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020083f8,"ax",%progbits
	.global Func_020003f8
	.thumb_func
Func_020003f8:
	ldr r3, [r0, #8]
	ldr r2, [r0, #68]
	ldr r1, [r0, #80]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, [r0, #72]
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r2, [r0, #76]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
	ldr r2, [r0, #48]
	ldr r3, [r0, #24]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [r0, #52]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	adds r0, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r0]
	adds r3, r3, r2
	strh r3, [r1, #18]
	bx lr
	.2byte 0x0000
	.section .text.x02008430,"ax",%progbits
	.global Func_02000430
	.thumb_func
Func_02000430:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_020085e8
	sub sp, #4
	mov r10, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r1, #0
	ldr r1, [sp, #44]
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	mov r8, r1
	ldr r7, [sp, #48]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	mov r1, r8
	ands r3, r1
	mov r9, r0
	cmp r3, #0
	beq .L_02008478
	cmp r7, #0
	beq .L_02008478
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_02008480
.L_02008478:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_02008480:
	mov r3, r10
	bl Func_02003fec
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200848e
	b .L_020085da
.L_0200848e:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_02003fdc
	ldr r2, .L_020085ec
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02003fe4
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_020085f0
	mov r1, r9
	str r3, [r6, #108]
	mov r3, r11
	str r3, [r6, #68]
	ldr r3, [sp, #36]
	adds r0, r6, #0
	str r3, [r6, #72]
	ldr r3, [sp, #40]
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl Func_020003b0
	movs r2, #100
	adds r2, r2, r6
	mov r9, r2
	mov r3, r9
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r3]
	ldr r3, .L_020085f4
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_020085da
	cmp r7, #0
	beq .L_020085da
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_02008510
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Func_02004124
.L_02008510:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008530
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Func_020003b0
.L_02008530:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_02008544
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02008544:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200858a
	ldr r3, .L_020085ec
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_02008572
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_02008584
.L_02008572:
	ldr r2, .L_020085f4
	adds r0, r3, r2
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_020085f4
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_02008584:
	bl Engine_MathDivide
	str r0, [r6, #52]
.L_0200858a:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_020085a6
	adds r0, r6, #0
	movs r1, #1
	bl Func_02003fdc
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02003fe4
.L_020085a6:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020085b8
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_020085b8:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020085ca
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_020085ca:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020085da
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_020085da:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020085e8:
	.4byte gPartyState
.L_020085ec:
	.4byte Data_0200474c
.L_020085f0:
	.4byte Func_020003f8
.L_020085f4:
	.4byte 0xffff0000
	.section .text.x020085f8,"ax",%progbits
	.global Func_020005f8
	.thumb_func
Func_020005f8:
	push {lr}
	movs r0, #16
	movs r1, #0
	movs r2, #14
	bl Func_02004214
	pop {pc}
	.2byte 0x0000
	.section .text.x02008608,"ax",%progbits
	.global Func_02000608
	.thumb_func
Func_02000608:
	push {lr}
	movs r0, #17
	movs r1, #2
	movs r2, #15
	bl Func_02004214
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #230
	bl Func_02003fcc
	pop {pc}
	.section .text.x02008620,"ax",%progbits
	.global Func_02000620
	.thumb_func
Func_02000620:
	push {r5, lr}
	ldmia r0!, {r5}
	ldmia r1!, {r3}
	ldmia r0!, {r4}
	subs r5, r5, r3
	ldmia r1!, {r3}
	asrs r5, r5, #16
	ldr r2, [r1]
	subs r4, r4, r3
	ldr r3, [r0]
	asrs r4, r4, #16
	subs r3, r3, r2
	asrs r3, r3, #16
	adds r0, r5, #0
	muls r0, r5
	adds r2, r4, #0
	muls r2, r4
	adds r1, r3, #0
	muls r1, r3
	adds r0, r0, r2
	adds r3, r1, #0
	adds r0, r0, r3
	ldr r3, .L_02008654
	mov lr, r3
	.2byte 0xf800
	pop {r5, pc}
.L_02008654:
	.4byte IwramFillWords + 0x74
	.section .text.x02008658,"ax",%progbits
	.global Func_02000658
	.thumb_func
Func_02000658:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r0
	adds r6, r1, #0
	adds r7, r6, #0
	mov r5, r8
	adds r7, #8
	adds r5, #8
	mov r10, r2
	adds r0, r7, #0
	movs r2, #0
	adds r1, r5, #0
	mov r9, r2
	bl Func_02000620
	cmp r0, r10
	bge .L_020086c0
	mov r2, r8
	ldr r3, [r2, #16]
	ldr r0, [r6, #16]
	ldr r1, [r7]
	subs r0, r0, r3
	ldr r3, [r5]
	movs r5, #128
	subs r1, r1, r3
	bl Func_02003f94
	lsls r0, r0, #16
	lsrs r0, r0, #16
	ldr r3, .L_020086cc
	lsls r5, r5, #5
	adds r1, r0, r5
	mov r5, r8
	ldrh r2, [r5, #6]
	adds r4, r0, r3
	movs r3, #240
	lsls r3, r3, #8
	ands r4, r3
	ands r1, r3
	ands r0, r3
	ands r3, r2
	cmp r0, r3
	beq .L_020086bc
	cmp r1, r3
	beq .L_020086bc
	cmp r4, r3
	bne .L_020086c0
.L_020086bc:
	movs r2, #1
	mov r9, r2
.L_020086c0:
	mov r0, r9
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_020086cc:
	.4byte 0xfffff000
	.section .text.x020086d0,"ax",%progbits
	.global Func_020006d0
	.thumb_func
Func_020006d0:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	ldr r3, .L_020087a0
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	adds r6, r0, #0
	ldr r0, [r7]
	bl Object_GetById
	bl Func_02004254
	adds r1, r0, #0
	adds r0, r6, #0
	bl Func_0200402c
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrh r2, [r3]
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	adds r5, r6, #0
	ands r3, r2
	adds r5, #91
	cmp r3, #141
	bne .L_02008726
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r2, #224
	lsls r2, r2, #3
	adds r2, #19
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0200878e
.L_02008726:
	adds r3, r6, #0
	adds r3, #99
	ldrb r3, [r3]
	cmp r3, #1
	beq .L_0200878e
	ldr r0, [r7]
	bl Object_GetById
	adds r1, r0, #0
	adds r0, r6, #0
	adds r0, #8
	adds r1, #8
	bl Func_02000620
	cmp r0, #11
	ble .L_02008784
	adds r3, r6, #0
	adds r3, #98
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_02008754
	movs r0, #15
	b .L_02008756
.L_02008754:
	movs r0, #14
.L_02008756:
	bl Object_GetById
	adds r1, r0, #0
	adds r0, r6, #0
	movs r2, #18
	bl Func_02000658
	cmp r0, #0
	bne .L_0200878e
	ldr r3, .L_020087a0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r2, #26
	adds r1, r0, #0
	adds r0, r6, #0
	bl Func_02000658
	cmp r0, #0
	bne .L_0200878e
.L_02008784:
	movs r3, #0
	adds r0, r6, #0
	strb r3, [r5]
	movs r1, #2
	b .L_02008796
.L_0200878e:
	movs r3, #1
	adds r0, r6, #0
	strb r3, [r5]
	movs r1, #1
.L_02008796:
	bl Func_02003fdc
	movs r0, #0
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020087a0:
	.4byte gPartyState
	.section .text.x020087a4,"ax",%progbits
	.global Func_020007a4
	.thumb_func
Func_020007a4:
	push {lr}
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #11
	bl Func_02004174
	movs r0, #0
	pop {pc}
	.2byte 0x0000
	.section .text.x020087b8,"ax",%progbits
	.global Func_020007b8
	.thumb_func
Func_020007b8:
	ldr r0, .L_020087bc
	bx lr
.L_020087bc:
	.4byte Data_02004bc0
	.section .text.x020087c0,"ax",%progbits
	.global Func_020007c0
	.thumb_func
Func_020007c0:
	movs r0, #0
	bx lr
	.section .text.x020087c4,"ax",%progbits
	.global Func_020007c4
	.thumb_func
Func_020007c4:
	ldr r0, .L_020087c8
	bx lr
.L_020087c8:
	.4byte Data_02004bf0
	.section .text.x020087cc,"ax",%progbits
	.global Func_020007cc
	.thumb_func
Func_020007cc:
	push {lr}
	ldr r1, .L_02008844
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02008848
	cmp r2, r3
	bne .L_020087e4
	ldr r0, .L_0200884c
	b .L_02008842
.L_020087e4:
	ldr r3, .L_02008850
	cmp r2, r3
	bne .L_020087ee
	ldr r0, .L_02008854
	b .L_02008842
.L_020087ee:
	ldr r3, .L_02008858
	cmp r2, r3
	bne .L_02008836
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #4
	bne .L_02008832
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl Func_02003fc4
	cmp r0, #0
	beq .L_02008814
	ldr r0, .L_0200885c
	b .L_02008842
.L_02008814:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #33
	bl Func_02003fc4
	cmp r0, #0
	beq .L_02008832
	movs r0, #157
	lsls r0, r0, #4
	bl Func_02003fc4
	cmp r0, #0
	bne .L_02008832
	ldr r0, .L_02008860
	b .L_02008842
.L_02008832:
	ldr r0, .L_02008864
	b .L_02008842
.L_02008836:
	ldr r3, .L_02008868
	cmp r2, r3
	bne .L_02008840
	ldr r0, .L_0200886c
	b .L_02008842
.L_02008840:
	ldr r0, .L_02008870
.L_02008842:
	pop {pc}
.L_02008844:
	.4byte gPartyState
.L_02008848:
	.4byte 0x000000ec
.L_0200884c:
	.4byte Data_02004c88
.L_02008850:
	.4byte 0x000000ef
.L_02008854:
	.4byte Data_02004d90
.L_02008858:
	.4byte 0x000000ed
.L_0200885c:
	.4byte Data_02005318
.L_02008860:
	.4byte Data_02005228
.L_02008864:
	.4byte Data_02005018
.L_02008868:
	.4byte 0x000000ee
.L_0200886c:
	.4byte Data_02005390
.L_02008870:
	.4byte Data_02004c70
	.section .text.x02008874,"ax",%progbits
	.global Func_02000874
	.thumb_func
Func_02000874:
	push {lr}
	ldr r1, .L_020088c0
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_020088c4
	cmp r2, r3
	bne .L_0200888c
	ldr r0, .L_020088c8
	b .L_020088be
.L_0200888c:
	ldr r3, .L_020088cc
	cmp r2, r3
	bne .L_02008896
	ldr r0, .L_020088d0
	b .L_020088be
.L_02008896:
	ldr r3, .L_020088d4
	cmp r2, r3
	bne .L_020088b2
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #4
	bne .L_020088ae
	ldr r0, .L_020088d8
	b .L_020088be
.L_020088ae:
	ldr r0, .L_020088dc
	b .L_020088be
.L_020088b2:
	ldr r3, .L_020088e0
	cmp r2, r3
	bne .L_020088bc
	ldr r0, .L_020088e4
	b .L_020088be
.L_020088bc:
	ldr r0, .L_020088e8
.L_020088be:
	pop {pc}
.L_020088c0:
	.4byte gPartyState
.L_020088c4:
	.4byte 0x000000ec
.L_020088c8:
	.4byte Data_020054ec
.L_020088cc:
	.4byte 0x000000ef
.L_020088d0:
	.4byte Data_02005870
.L_020088d4:
	.4byte 0x000000ed
.L_020088d8:
	.4byte Data_02005e70
.L_020088dc:
	.4byte Data_02005a44
.L_020088e0:
	.4byte 0x000000ee
.L_020088e4:
	.4byte Data_02005ed0
.L_020088e8:
	.4byte Data_020054e0
	.section .text.x020088ec,"ax",%progbits
	.global Func_020008ec
	.thumb_func
Func_020008ec:
	push {lr}
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	ldr r0, .L_02008908
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02004074
	pop {pc}
	.2byte 0x0000
.L_02008908:
	.4byte 0x00002924
	.section .text.x0200890c,"ax",%progbits
	.global Func_0200090c
	.thumb_func
Func_0200090c:
	push {lr}
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	ldr r0, .L_02008928
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02004074
	pop {pc}
	.2byte 0x0000
.L_02008928:
	.4byte 0x00002925
	.section .text.x0200892c,"ax",%progbits
	.global Func_0200092c
	.thumb_func
Func_0200092c:
	push {lr}
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	ldr r0, .L_02008948
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02004074
	pop {pc}
	.2byte 0x0000
.L_02008948:
	.4byte 0x00002926
	.section .text.x0200894c,"ax",%progbits
	.global Func_0200094c
	.thumb_func
Func_0200094c:
	push {lr}
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	ldr r0, .L_02008968
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02004074
	pop {pc}
	.2byte 0x0000
.L_02008968:
	.4byte 0x00002927
	.section .text.x0200896c,"ax",%progbits
	.global Func_0200096c
	.thumb_func
Func_0200096c:
	push {lr}
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	ldr r0, .L_02008988
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02004074
	pop {pc}
	.2byte 0x0000
.L_02008988:
	.4byte 0x00002928
	.section .text.x0200898c,"ax",%progbits
	.global Func_0200098c
	.thumb_func
Func_0200098c:
	push {lr}
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	ldr r0, .L_020089a8
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02004074
	pop {pc}
	.2byte 0x0000
.L_020089a8:
	.4byte 0x00002936
	.section .text.x020089ac,"ax",%progbits
	.global Func_020009ac
	.thumb_func
Func_020009ac:
	push {lr}
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	ldr r0, .L_020089cc
	bl Func_0200412c
	movs r1, #0
	movs r0, #8
	bl Func_0200414c
	bl Func_02004074
	pop {pc}
.L_020089cc:
	.4byte 0x000028d3
	.section .text.x020089d0,"ax",%progbits
	.global Func_020009d0
	.thumb_func
Func_020009d0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r7, r0, #0
	bl Random16Far
	ldrh r6, [r7, #6]
	movs r1, #128
	lsls r1, r1, #10
	adds r5, r0, #0
	adds r0, r6, #0
	adds r5, r5, r1
	bl Math_Cosine
	ldr r2, .L_02008a7c
	adds r1, r0, #0
	mov r8, r2
	adds r0, r5, #0
	mov lr, r8
	.2byte 0xf800
	mov r10, r0
	adds r0, r6, #0
	bl Math_Sine
	adds r1, r0, #0
	adds r0, r5, #0
	mov lr, r8
	.2byte 0xf800
	ldr r3, [r7, #8]
	movs r1, #255
	add r3, r10
	str r3, [r7, #8]
	ldr r3, [r7, #16]
	lsls r1, r1, #8
	adds r3, r3, r0
	str r3, [r7, #16]
	ldrh r3, [r7, #6]
	adds r1, #240
	adds r3, r3, r1
	strh r3, [r7, #6]
	adds r5, r7, #0
	adds r5, #102
	movs r1, #0
	ldrsh r3, [r5, r1]
	ldrh r2, [r5]
	cmp r3, #0
	beq .L_02008a40
	subs r3, r2, #1
	strh r3, [r5]
	ldrh r3, [r7, #6]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	strh r3, [r7, #6]
	b .L_02008a58
.L_02008a40:
	bl Random16Far
	lsls r0, r0, #5
	lsrs r0, r0, #16
	cmp r0, #0
	bne .L_02008a58
	bl Random16Far
	lsls r0, r0, #4
	lsrs r0, r0, #16
	adds r0, #8
	strh r0, [r5]
.L_02008a58:
	adds r2, r7, #0
	adds r2, #100
	ldrh r3, [r2]
	movs r1, #202
	adds r3, #1
	strh r3, [r2]
	lsls r1, r1, #15
	lsls r3, r3, #16
	cmp r3, r1
	bne .L_02008a72
	adds r0, r7, #0
	bl Func_02003ff4
.L_02008a72:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008a7c:
	.4byte IwramMulQ16
	.section .text.x02008a80,"ax",%progbits
	.global Func_02000a80
	.thumb_func
Func_02000a80:
	push {r5, r6, lr}
	ldr r3, .L_02008ae8
	ldr r6, [r3]
	movs r3, #7
	ands r6, r3
	cmp r6, #0
	bne .L_02008ae4
	movs r2, #204
	movs r0, #154
	lsls r2, r2, #8
	movs r3, #232
	lsls r0, r0, #1
	ldr r1, .L_02008aec
	adds r2, #204
	lsls r3, r3, #15
	bl Func_02004234
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008ae4
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_02008af0
	str r3, [r5, #108]
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	bl Random16Far
	lsls r0, r0, #4
	lsrs r0, r0, #16
	negs r0, r0
	lsls r0, r0, #1
	adds r3, r5, #0
	adds r0, #20
	adds r3, #100
	strh r0, [r3]
	adds r3, #2
	strh r6, [r3]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r5, #72]
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r5, #40]
	bl Random16Far
	strh r0, [r5, #6]
.L_02008ae4:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008ae8:
	.4byte Data_0300122c
.L_02008aec:
	.4byte 0x02c20000
.L_02008af0:
	.4byte Func_020009d0
	.section .text.x02008af4,"ax",%progbits
	.global Func_02000af4
	.thumb_func
Func_02000af4:
	push {r5, r6, lr}
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl Func_02003fc4
	cmp r0, #0
	beq .L_02008b18
	ldr r0, .L_02008ca8
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	b .L_02008b20
.L_02008b18:
	ldr r0, .L_02008cac
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
.L_02008b20:
	movs r0, #17
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_020041d4
	movs r0, #129
	lsls r0, r0, #14
	movs r1, #1
	adds r0, #132
	bl Func_020041cc
	movs r0, #16
	bl Func_020041dc
	bl Func_02004264
	movs r0, #17
	bl Object_GetById
	movs r1, #2
	bl Func_0200427c
	movs r0, #140
	movs r3, #232
	lsls r0, r0, #1
	ldr r1, .L_02008cb0
	movs r2, #0
	lsls r3, r3, #15
	bl Func_02003fec
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02008baa
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r6, #0
	movs r1, #2
	bl Func_02003fdc
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r6, #24]
	str r3, [r6, #28]
	movs r5, #0
.L_02008b8c:
	ldr r3, [r6, #24]
	movs r2, #192
	lsls r2, r2, #3
	adds r2, #102
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r5, #1
	bl WaitFrames
	cmp r5, #27
	bls .L_02008b8c
.L_02008baa:
	ldr r5, .L_02008cb4
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r5, #0
	bl Func_02003f7c
	movs r0, #120
	bl Battle_WaitMode0
	adds r0, r5, #0
	bl Scheduler_RemoveCallbackFar
	cmp r6, #0
	beq .L_02008bf2
	movs r5, #0
.L_02008bc8:
	ldr r3, [r6, #24]
	ldr r2, .L_02008cb8
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	adds r5, #1
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r6, #12]
	bl WaitFrames
	cmp r5, #27
	bls .L_02008bc8
	adds r0, r6, #0
	bl Func_02003ff4
.L_02008bf2:
	movs r0, #17
	bl Object_GetById
	movs r1, #0
	bl Func_0200427c
	bl Func_02004274
	bl Func_0200426c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	bl Func_020041cc
	movs r0, #16
	bl Func_020041dc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl Func_02003fc4
	cmp r0, #0
	beq .L_02008c2c
	ldr r0, .L_02008cbc
	bl Func_0200412c
	b .L_02008c32
.L_02008c2c:
	ldr r0, .L_02008cc0
	bl Func_0200412c
.L_02008c32:
	movs r0, #17
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #17
	movs r1, #0
	bl Func_02004144
	movs r0, #17
	movs r1, #1
	bl Object_SetModeById
	movs r1, #4
	adds r1, #255
	movs r2, #0
	movs r0, #14
	bl Func_02004174
	movs r1, #128
	movs r2, #20
	movs r0, #14
	lsls r1, r1, #8
	bl Func_02004154
	movs r0, #14
	movs r1, #0
	bl Func_02004144
	movs r0, #15
	movs r1, #0
	bl Func_0200415c
	movs r0, #15
	movs r1, #0
	bl Func_02004144
	movs r0, #14
	movs r1, #3
	bl Object_SetModeById
	movs r0, #15
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #208
	movs r2, #0
	movs r0, #15
	lsls r1, r1, #8
	bl Func_02004154
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #6
	bl Func_0200415c
	bl Func_02004074
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008ca8:
	.4byte 0x00002bfd
.L_02008cac:
	.4byte 0x000028f2
.L_02008cb0:
	.4byte 0x02c20000
.L_02008cb4:
	.4byte Func_02000a80
.L_02008cb8:
	.4byte 0xfffff99a
.L_02008cbc:
	.4byte 0x00002bfe
.L_02008cc0:
	.4byte 0x000028f3
	.section .text.x02008cc4,"ax",%progbits
	.global Func_02000cc4
	.thumb_func
Func_02000cc4:
	push {lr}
	ldr r3, .L_02008cec
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r2, #190
	ldrh r3, [r0, #6]
	lsls r2, r2, #7
	adds r2, #255
	adds r3, r3, r2
	ldr r2, .L_02008cf0
	lsls r3, r3, #16
	movs r0, #1
	cmp r3, r2
	bls .L_02008cea
	movs r0, #0
.L_02008cea:
	pop {pc}
.L_02008cec:
	.4byte gPartyState
.L_02008cf0:
	.4byte 0x3ffe0000
	.section .text.x02008cf4,"ax",%progbits
	.global Func_02000cf4
	.thumb_func
Func_02000cf4:
	push {lr}
	bl Func_02000cc4
	cmp r0, #0
	beq .L_02008d08
	movs r0, #28
	movs r1, #13
	bl Func_02004294
	b .L_02008d3a
.L_02008d08:
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl Func_02003fc4
	cmp r0, #0
	beq .L_02008d28
	ldr r0, .L_02008d3c
	bl Func_0200412c
	b .L_02008d2e
.L_02008d28:
	ldr r0, .L_02008d40
	bl Func_0200412c
.L_02008d2e:
	movs r0, #13
	movs r1, #0
	bl Func_02004144
	bl Func_02004074
.L_02008d3a:
	pop {pc}
.L_02008d3c:
	.4byte 0x00002c3b
.L_02008d40:
	.4byte 0x000028fe
	.section .text.x02008d44,"ax",%progbits
	.global Func_02000d44
	.thumb_func
Func_02000d44:
	push {lr}
	bl Func_02000cc4
	cmp r0, #0
	beq .L_02008d58
	movs r0, #26
	movs r1, #18
	bl Func_02004294
	b .L_02008d8a
.L_02008d58:
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl Func_02003fc4
	cmp r0, #0
	beq .L_02008d78
	ldr r0, .L_02008d8c
	bl Func_0200412c
	b .L_02008d7e
.L_02008d78:
	ldr r0, .L_02008d90
	bl Func_0200412c
.L_02008d7e:
	movs r0, #18
	movs r1, #0
	bl Func_02004144
	bl Func_02004074
.L_02008d8a:
	pop {pc}
.L_02008d8c:
	.4byte 0x00002c37
.L_02008d90:
	.4byte 0x000028fa
	.section .text.x02008d94,"ax",%progbits
	.global Func_02000d94
	.thumb_func
Func_02000d94:
	push {lr}
	bl Func_02000cc4
	cmp r0, #0
	beq .L_02008da8
	movs r0, #27
	movs r1, #19
	bl Func_02004294
	b .L_02008dda
.L_02008da8:
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl Func_02003fc4
	cmp r0, #0
	beq .L_02008dc8
	ldr r0, .L_02008ddc
	bl Func_0200412c
	b .L_02008dce
.L_02008dc8:
	ldr r0, .L_02008de0
	bl Func_0200412c
.L_02008dce:
	movs r0, #19
	movs r1, #0
	bl Func_02004144
	bl Func_02004074
.L_02008dda:
	pop {pc}
.L_02008ddc:
	.4byte 0x00002c39
.L_02008de0:
	.4byte 0x000028fc
	.section .text.x02008de4,"ax",%progbits
	.global Func_02000de4
	.thumb_func
Func_02000de4:
	push {lr}
	bl Func_02000cc4
	cmp r0, #0
	beq .L_02008df8
	movs r0, #12
	movs r1, #20
	bl Func_020042a4
	b .L_02008e40
.L_02008df8:
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl Func_02003fc4
	cmp r0, #0
	beq .L_02008e18
	ldr r0, .L_02008e44
	bl Func_0200412c
	b .L_02008e34
.L_02008e18:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #33
	bl Func_02003fc4
	cmp r0, #0
	beq .L_02008e2e
	ldr r0, .L_02008e48
	bl Func_0200412c
	b .L_02008e34
.L_02008e2e:
	ldr r0, .L_02008e4c
	bl Func_0200412c
.L_02008e34:
	movs r0, #20
	movs r1, #0
	bl Func_02004144
	bl Func_02004074
.L_02008e40:
	pop {pc}
	.2byte 0x0000
.L_02008e44:
	.4byte 0x00002c3d
.L_02008e48:
	.4byte 0x00002bae
.L_02008e4c:
	.4byte 0x00002900
	.section .text.x02008e50,"ax",%progbits
	.global Func_02000e50
	.thumb_func
Func_02000e50:
	push {lr}
	bl Func_02000cc4
	cmp r0, #0
	beq .L_02008e62
	movs r0, #28
	bl Func_0200429c
	b .L_02008eaa
.L_02008e62:
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl Func_02003fc4
	cmp r0, #0
	beq .L_02008e82
	ldr r0, .L_02008eac
	bl Func_0200412c
	b .L_02008e9e
.L_02008e82:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #33
	bl Func_02003fc4
	cmp r0, #0
	beq .L_02008e98
	ldr r0, .L_02008eb0
	bl Func_0200412c
	b .L_02008e9e
.L_02008e98:
	ldr r0, .L_02008eb4
	bl Func_0200412c
.L_02008e9e:
	movs r0, #28
	movs r1, #0
	bl Func_02004144
	bl Func_02004074
.L_02008eaa:
	pop {pc}
.L_02008eac:
	.4byte 0x00002c45
.L_02008eb0:
	.4byte 0x00002bb6
.L_02008eb4:
	.4byte 0x0000290c
	.section .text.x02008eb8,"ax",%progbits
	.global Func_02000eb8
	.thumb_func
Func_02000eb8:
	push {lr}
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	ldr r0, .L_02008ed8
	bl Func_0200412c
	movs r1, #0
	movs r0, #10
	bl Func_0200414c
	bl Func_02004074
	pop {pc}
.L_02008ed8:
	.4byte 0x000013d8
	.section .text.x02008edc,"ax",%progbits
	.global Func_02000edc
	.thumb_func
Func_02000edc:
	push {r5, lr}
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	ldr r5, .L_02008f3c
	movs r1, #1
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #1
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #2
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #3
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #4
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #5
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #6
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #7
	movs r1, #1
	adds r5, #8
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02004074
	pop {r5, pc}
	.2byte 0x0000
.L_02008f3c:
	.4byte 0x000013cf
	.section .text.x02008f40,"ax",%progbits
	.global Func_02000f40
	.thumb_func
Func_02000f40:
	push {r5, lr}
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	ldr r5, .L_02008f78
	movs r1, #1
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #1
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #2
	movs r1, #1
	adds r5, #3
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02004074
	pop {r5, pc}
	.2byte 0x0000
.L_02008f78:
	.4byte 0x00001389
	.section .text.x02008f7c,"ax",%progbits
	.global Func_02000f7c
	.thumb_func
Func_02000f7c:
	push {r5, lr}
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	ldr r5, .L_02008fc4
	movs r1, #1
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #1
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #2
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #3
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #4
	movs r1, #1
	adds r5, #5
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02004074
	pop {r5, pc}
	.2byte 0x0000
.L_02008fc4:
	.4byte 0x0000138d
	.section .text.x02008fc8,"ax",%progbits
	.global Func_02000fc8
	.thumb_func
Func_02000fc8:
	push {lr}
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	movs r0, #0
	bl Func_02001938
	bl Func_02004074
	pop {pc}
	.section .text.x02008fe0,"ax",%progbits
	.global Func_02000fe0
	.thumb_func
Func_02000fe0:
	push {r5, lr}
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	ldr r5, .L_02009028
	movs r1, #1
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #1
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #2
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #3
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #4
	movs r1, #1
	adds r5, #5
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02004074
	pop {r5, pc}
	.2byte 0x0000
.L_02009028:
	.4byte 0x000013b0
	.section .text.x0200902c,"ax",%progbits
	.global Func_0200102c
	.thumb_func
Func_0200102c:
	push {r5, lr}
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	ldr r5, .L_0200906c
	movs r1, #1
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #1
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #2
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #3
	movs r1, #1
	adds r5, #4
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02004074
	pop {r5, pc}
	.2byte 0x0000
.L_0200906c:
	.4byte 0x000013b6
	.section .text.x02009070,"ax",%progbits
	.global Func_02001070
	.thumb_func
Func_02001070:
	push {lr}
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	ldr r0, .L_02009090
	bl Func_0200412c
	movs r1, #0
	movs r0, #14
	bl Func_0200414c
	bl Func_02004074
	pop {pc}
.L_02009090:
	.4byte 0x0000291a
	.section .text.x02009094,"ax",%progbits
	.global Func_02001094
	.thumb_func
Func_02001094:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #14
	ldr r7, [r3, #108]
	bl Object_GetById
	adds r5, r0, #0
	movs r3, #6
	ldrsh r2, [r5, r3]
	adds r6, r5, #0
	mov r8, r2
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	movs r2, #179
	movs r3, #1
	lsls r2, r2, #1
	adds r6, #99
	strb r3, [r6]
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02009114
	ldr r3, .L_02009170
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r0, #14
	ldr r1, [r3]
	movs r2, #0
	bl Func_02004114
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl Func_02003fc4
	cmp r0, #0
	beq .L_020090f6
	ldr r0, .L_02009174
	bl Func_0200412c
	b .L_02009146
.L_020090f6:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #33
	bl Func_02003fc4
	cmp r0, #0
	beq .L_0200910c
	ldr r0, .L_02009178
	bl Func_0200412c
	b .L_02009146
.L_0200910c:
	ldr r0, .L_0200917c
	bl Func_0200412c
	b .L_02009146
.L_02009114:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl Func_02003fc4
	cmp r0, #0
	beq .L_0200912a
	ldr r0, .L_02009180
	bl Func_0200412c
	b .L_02009146
.L_0200912a:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #33
	bl Func_02003fc4
	cmp r0, #0
	beq .L_02009140
	ldr r0, .L_02009184
	bl Func_0200412c
	b .L_02009146
.L_02009140:
	ldr r0, .L_02009188
	bl Func_0200412c
.L_02009146:
	movs r0, #14
	movs r1, #0
	bl Func_02004144
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02009160
	mov r3, r8
	strh r3, [r5, #6]
.L_02009160:
	movs r3, #0
	strb r3, [r6]
	bl Func_02004074
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009170:
	.4byte gPartyState
.L_02009174:
	.4byte 0x00002c5e
.L_02009178:
	.4byte 0x00002c5a
.L_0200917c:
	.4byte 0x000028db
.L_02009180:
	.4byte 0x00002c60
.L_02009184:
	.4byte 0x00002c5c
.L_02009188:
	.4byte 0x000028e3
	.section .text.x0200918c,"ax",%progbits
	.global Func_0200118c
	.thumb_func
Func_0200118c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #15
	ldr r7, [r3, #108]
	bl Object_GetById
	adds r5, r0, #0
	movs r3, #6
	ldrsh r2, [r5, r3]
	adds r6, r5, #0
	mov r8, r2
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	movs r2, #179
	movs r3, #1
	lsls r2, r2, #1
	adds r6, #99
	strb r3, [r6]
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020091f6
	ldr r3, .L_0200923c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r0, #15
	ldr r1, [r3]
	movs r2, #0
	bl Func_02004114
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl Func_02003fc4
	cmp r0, #0
	beq .L_020091ee
	ldr r0, .L_02009240
	bl Func_0200412c
	b .L_02009212
.L_020091ee:
	ldr r0, .L_02009244
	bl Func_0200412c
	b .L_02009212
.L_020091f6:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl Func_02003fc4
	cmp r0, #0
	beq .L_0200920c
	ldr r0, .L_02009248
	bl Func_0200412c
	b .L_02009212
.L_0200920c:
	ldr r0, .L_0200924c
	bl Func_0200412c
.L_02009212:
	movs r0, #15
	movs r1, #0
	bl Func_02004144
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200922c
	mov r3, r8
	strh r3, [r5, #6]
.L_0200922c:
	movs r3, #0
	strb r3, [r6]
	bl Func_02004074
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200923c:
	.4byte gPartyState
.L_02009240:
	.4byte 0x00002be9
.L_02009244:
	.4byte 0x000028dc
.L_02009248:
	.4byte 0x00002bef
.L_0200924c:
	.4byte 0x000028e4
	.section .text.x02009250,"ax",%progbits
	.global Func_02001250
	.thumb_func
Func_02001250:
	push {lr}
	bl Func_020000f4
	bl Func_0200428c
	ldr r3, .L_0200927c
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	movs r1, #99
	ldr r0, .L_02009280
	bl Func_020041ac
	movs r0, #40
	bl Battle_WaitMode0
	bl Func_02000158
	pop {pc}
.L_0200927c:
	.4byte gPartyState
.L_02009280:
	.4byte 0x0000012f
	.section .text.x02009284,"ax",%progbits
	.global Func_02001284
	.thumb_func
Func_02001284:
	push {lr}
	sub sp, #12
	movs r3, #8
	movs r2, #54
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #3
	movs r1, #50
	movs r2, #1
	movs r3, #1
	bl Func_0200423c
	add sp, #12
	pop {pc}
	.section .text.x020092a4,"ax",%progbits
	.global Func_020012a4
	.thumb_func
Func_020012a4:
	push {lr}
	sub sp, #12
	movs r3, #2
	movs r2, #43
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #4
	movs r1, #43
	movs r2, #2
	movs r3, #1
	bl Func_0200423c
	add sp, #12
	pop {pc}
	.section .text.x020092c4,"ax",%progbits
	.global Func_020012c4
	.thumb_func
Func_020012c4:
	push {r5, r6, lr}
	ldr r3, .L_02009350
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r3, #17
	movs r2, #37
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	adds r5, r0, #0
	movs r1, #37
	movs r0, #16
	movs r2, #1
	movs r3, #1
	bl Func_0200423c
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #199
	bl Func_02003fc4
	cmp r0, #0
	bne .L_0200934c
	movs r1, #140
	movs r2, #166
	movs r0, #66
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_020040d4
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #17
	bne .L_0200934c
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #41
	bne .L_0200934c
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_0200417c
	movs r2, #16
	ldr r0, [r6]
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_02004074
.L_0200934c:
	add sp, #12
	pop {r5, r6, pc}
.L_02009350:
	.4byte gPartyState
	.section .text.x02009354,"ax",%progbits
	.global Func_02001354
	.thumb_func
Func_02001354:
	push {lr}
	sub sp, #12
	movs r3, #23
	movs r2, #47
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #29
	movs r1, #35
	movs r2, #1
	movs r3, #2
	bl Func_0200423c
	add sp, #12
	pop {pc}
	.section .text.x02009374,"ax",%progbits
	.global Func_02001374
	.thumb_func
Func_02001374:
	push {lr}
	sub sp, #12
	movs r3, #29
	movs r2, #58
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #32
	movs r1, #55
	movs r2, #2
	movs r3, #3
	bl Func_0200423c
	add sp, #12
	pop {pc}
	.section .text.x02009394,"ax",%progbits
	.global Func_02001394
	.thumb_func
Func_02001394:
	push {lr}
	sub sp, #12
	movs r3, #3
	movs r2, #56
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #3
	movs r1, #60
	movs r2, #1
	movs r3, #1
	bl Func_0200423c
	add sp, #12
	pop {pc}
	.section .text.x020093b4,"ax",%progbits
	.global Func_020013b4
	.thumb_func
Func_020013b4:
	push {lr}
	sub sp, #12
	movs r3, #7
	movs r2, #58
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #7
	movs r1, #62
	movs r2, #2
	movs r3, #1
	bl Func_0200423c
	add sp, #12
	pop {pc}
	.section .text.x020093d4,"ax",%progbits
	.global Func_020013d4
	.thumb_func
Func_020013d4:
	push {lr}
	sub sp, #12
	movs r3, #10
	movs r2, #59
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #10
	movs r1, #63
	movs r2, #2
	movs r3, #1
	bl Func_0200423c
	add sp, #12
	pop {pc}
	.section .text.x020093f4,"ax",%progbits
	.global Func_020013f4
	.thumb_func
Func_020013f4:
	push {lr}
	sub sp, #12
	movs r3, #40
	movs r2, #57
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #32
	movs r1, #55
	movs r2, #3
	movs r3, #1
	bl Func_0200423c
	add sp, #12
	pop {pc}
	.section .text.x02009414,"ax",%progbits
	.global Func_02001414
	.thumb_func
Func_02001414:
	push {lr}
	sub sp, #12
	movs r3, #39
	movs r2, #58
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #32
	movs r1, #55
	movs r2, #5
	movs r3, #1
	bl Func_0200423c
	add sp, #12
	pop {pc}
	.section .text.x02009434,"ax",%progbits
	.global Func_02001434
	.thumb_func
Func_02001434:
	push {lr}
	sub sp, #12
	movs r3, #39
	movs r2, #59
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #32
	movs r1, #55
	movs r2, #5
	movs r3, #1
	bl Func_0200423c
	add sp, #12
	pop {pc}
	.section .text.x02009454,"ax",%progbits
	.global Func_02001454
	.thumb_func
Func_02001454:
	push {lr}
	sub sp, #12
	movs r3, #39
	movs r2, #60
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #32
	movs r1, #55
	movs r2, #5
	movs r3, #1
	bl Func_0200423c
	add sp, #12
	pop {pc}
	.section .text.x02009474,"ax",%progbits
	.global Func_02001474
	.thumb_func
Func_02001474:
	push {lr}
	sub sp, #12
	movs r3, #40
	movs r2, #61
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #32
	movs r1, #55
	movs r2, #3
	movs r3, #1
	bl Func_0200423c
	add sp, #12
	pop {pc}
	.section .text.x02009494,"ax",%progbits
	.global Func_02001494
	.thumb_func
Func_02001494:
	push {lr}
	movs r1, #128
	movs r2, #220
	movs r3, #128
	movs r0, #0
	lsls r1, r1, #18
	lsls r2, r2, #18
	lsls r3, r3, #19
	bl Func_020041a4
	movs r0, #128
	lsls r0, r0, #2
	bl Func_02003fcc
	pop {pc}
	.2byte 0x0000
	.section .text.x020094b4,"ax",%progbits
	.global Func_020014b4
	.thumb_func
Func_020014b4:
	push {lr}
	movs r1, #128
	movs r2, #128
	movs r3, #128
	movs r0, #0
	lsls r1, r1, #18
	lsls r2, r2, #18
	lsls r3, r3, #19
	bl Func_020041a4
	movs r0, #128
	lsls r0, r0, #2
	bl Func_02003fd4
	pop {pc}
	.2byte 0x0000
	.section .text.x020094d4,"ax",%progbits
	.global Func_020014d4
	.thumb_func
Func_020014d4:
	push {r5, lr}
	movs r0, #62
	bl Func_02003fc4
	cmp r0, #0
	bne .L_02009524
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	bl Func_0200406c
	movs r0, #1
	bl Func_02004204
	movs r1, #166
	movs r2, #238
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r0, #16
	bl Func_020040d4
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_02009528
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #16
	bl Func_02004114
	movs r3, #181
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #200
	strh r3, [r2]
	bl Func_02004074
.L_02009524:
	pop {r5, pc}
	.2byte 0x0000
.L_02009528:
	.4byte gPartyState
	.section .text.x0200952c,"ax",%progbits
	.global Func_0200152c
	.thumb_func
Func_0200152c:
	push {r5, r6, lr}
	sub sp, #8
	movs r5, #1
	movs r6, #2
	movs r0, #27
	movs r1, #4
	movs r2, #48
	movs r3, #12
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004004
	movs r0, #27
	movs r1, #6
	movs r2, #49
	movs r3, #12
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004004
	movs r3, #48
	movs r2, #19
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #48
	movs r1, #1
	movs r2, #2
	movs r3, #1
	bl Func_0200400c
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x0200956c,"ax",%progbits
	.global Func_0200156c
	.thumb_func
Func_0200156c:
	push {r5, r6, lr}
	sub sp, #8
	movs r5, #1
	movs r6, #2
	movs r0, #27
	movs r1, #8
	movs r2, #48
	movs r3, #12
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004004
	movs r0, #27
	movs r1, #10
	movs r2, #49
	movs r3, #12
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004004
	movs r3, #48
	movs r2, #19
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #48
	movs r1, #0
	movs r2, #2
	movs r3, #1
	bl Func_0200400c
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x020095ac,"ax",%progbits
	.global Func_020015ac
	.thumb_func
Func_020015ac:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #17
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	movs r1, #224
	lsls r1, r1, #14
	ldr r2, .L_0200971c
	movs r0, #17
	bl Func_020040d4
	movs r0, #1
	bl WaitFrames
	add r2, sp, #28
	movs r3, #2
	str r3, [r2]
	movs r0, #188
	mov r9, r2
	bl Func_020042b4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_0200401c
	movs r3, #0
	mov r10, r3
.L_020095fe:
	mov r4, r10
	lsls r6, r4, #12
	adds r0, r6, #0
	bl Math_Cosine
	movs r3, #0
	add r5, sp, #16
	str r3, [r5, #4]
	str r0, [r5]
	adds r0, r6, #0
	bl Math_Sine
	ldr r6, [r5]
	mov r8, r0
	str r0, [r5, #8]
	movs r1, #3
	adds r0, r6, #0
	bl Engine_MathDivide
	ldr r3, [r5, #4]
	movs r4, #200
	lsls r4, r4, #5
	adds r6, r6, r0
	adds r4, #153
	str r6, [r5]
	adds r3, r3, r4
	ldr r2, [r7, #16]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	str r3, [sp, #0]
	mov r3, r8
	str r3, [sp, #4]
	movs r4, #128
	mov r3, r9
	lsls r4, r4, #10
	str r3, [sp, #12]
	adds r3, r6, #0
	mov r8, r4
	str r4, [sp, #8]
	bl Func_02000430
	movs r4, #1
	add r10, r4
	mov r2, r10
	cmp r2, #16
	bls .L_020095fe
	movs r0, #17
	bl Object_GetById
	movs r1, #0
	bl Func_02004124
	movs r0, #17
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #192
	movs r2, #192
	movs r0, #17
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r3, #192
	lsls r3, r3, #12
	movs r2, #170
	lsls r2, r2, #2
	str r3, [r7, #40]
	movs r1, #40
	movs r0, #17
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #10
	bl Battle_WaitMode0
	ldr r3, .L_02009720
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r3, r4
	ldr r0, [r5]
	bl Object_GetById
	ldr r2, [r7, #8]
	ldr r3, [r0, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020096e0
	ldr r2, [r7, #16]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020096e0
	ldr r0, [r5]
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #128
	ldr r0, [r5]
	mov r1, r8
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r2, #170
	ldr r0, [r5]
	movs r1, #68
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndCommit
.L_020096e0:
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_0200401c
	ldr r3, .L_02009720
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #17
	movs r2, #0
	bl Object_LinkPair
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #231
	bl Func_02003fcc
	bl Func_02004074
	add sp, #68
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200971c:
	.4byte 0x02ba0000
.L_02009720:
	.4byte gPartyState
	.section .text.x02009724,"ax",%progbits
	.global Func_02001724
	.thumb_func
Func_02001724:
	push {r5, r6, lr}
	sub sp, #8
	movs r5, #1
	movs r6, #2
	movs r0, #0
	movs r1, #1
	movs r2, #26
	movs r3, #42
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004004
	movs r0, #0
	movs r1, #1
	movs r2, #26
	movs r3, #48
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004004
	movs r0, #0
	movs r1, #1
	movs r2, #11
	movs r3, #54
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004004
	movs r0, #0
	movs r1, #1
	movs r2, #16
	movs r3, #54
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004004
	movs r3, #45
	str r3, [sp, #4]
	movs r5, #26
	movs r0, #0
	movs r1, #32
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_0200400c
	movs r3, #51
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #32
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_0200400c
	movs r3, #11
	str r3, [sp, #0]
	movs r5, #57
	movs r0, #0
	movs r1, #32
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_0200400c
	movs r3, #16
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #32
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_0200400c
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x020097bc,"ax",%progbits
	.global Func_020017bc
	.thumb_func
Func_020017bc:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl Object_GetById
	movs r1, #3
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_0200416c
	adds r2, r6, #0
	adds r2, #89
	movs r3, #8
	strb r3, [r2]
	adds r0, r6, #0
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	adds r3, r6, #0
	movs r1, #0
	adds r3, #85
	strb r1, [r3]
	movs r3, #128
	lsls r3, r3, #15
	str r3, [r6, #12]
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020097f4,"ax",%progbits
	.global Func_020017f4
	.thumb_func
Func_020017f4:
	push {r5, lr}
	movs r0, #19
	movs r1, #0
	bl Object_SetModeById
	movs r0, #20
	movs r1, #1
	bl Object_SetModeById
	movs r0, #21
	movs r1, #2
	bl Object_SetModeById
	movs r0, #22
	movs r1, #0
	bl Object_SetModeById
	movs r0, #23
	movs r1, #1
	bl Object_SetModeById
	movs r0, #24
	movs r1, #2
	bl Object_SetModeById
	movs r0, #25
	movs r1, #3
	bl Object_SetModeById
	movs r1, #4
	movs r0, #26
	bl Object_SetModeById
	movs r0, #19
	bl Func_020017bc
	movs r0, #20
	bl Func_020017bc
	movs r0, #21
	bl Func_020017bc
	movs r0, #22
	bl Func_020017bc
	movs r0, #23
	bl Func_020017bc
	movs r0, #24
	bl Func_020017bc
	movs r0, #25
	bl Func_020017bc
	movs r0, #26
	bl Func_020017bc
	movs r0, #27
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #28
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #29
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #30
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #31
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #32
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #33
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #27
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #8
	orrs r3, r5
	strb r3, [r0]
	movs r0, #28
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #29
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #30
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #31
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #32
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #33
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r1, #0
	orrs r5, r3
	strb r5, [r0]
	movs r0, #27
	bl Object_SetModeById
	movs r0, #32
	movs r1, #0
	bl Object_SetModeById
	movs r0, #33
	movs r1, #0
	bl Object_SetModeById
	pop {r5, pc}
	.section .text.x02009938,"ax",%progbits
	.global Func_02001938
	.thumb_func
Func_02001938:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	movs r0, #228
	bl PartyInventory_CountItem
	adds r6, r0, #0
	bl Func_0200422c
	cmp r5, #0
	bne .L_020099d2
	ldr r7, .L_02009a40
	adds r0, r7, #0
	bl Func_0200412c
	movs r0, #18
	movs r1, #0
	bl Func_02004144
	cmp r6, #0
	beq .L_020099fa
	adds r0, r7, #2
	bl Func_0200412c
	adds r0, r6, #0
	movs r1, #5
	bl Func_0200403c
	movs r1, #0
	movs r0, #18
	bl Func_02004134
	ldr r3, .L_02009a44
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	beq .L_020099fa
	bl PartyInventory_CountFreeSlots
	adds r5, r0, #0
	cmp r5, #0
	bne .L_020099a4
	adds r0, r7, #4
	bl Func_0200412c
	movs r0, #18
	movs r1, #0
	bl Func_02004134
	b .L_020099b6
.L_020099a4:
	cmp r5, #6
	bgt .L_02009a04
	adds r0, r7, #5
	bl Func_0200412c
	movs r0, #18
	movs r1, #0
	bl Func_02004134
.L_020099b6:
	cmp r5, #6
	bgt .L_02009a04
	ldr r3, .L_02009a44
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	bne .L_02009a04
	ldr r0, .L_02009a48
	b .L_020099f6
.L_020099d2:
	cmp r6, #0
	bne .L_020099da
	ldr r0, .L_02009a4c
	b .L_020099f6
.L_020099da:
	ldr r0, .L_02009a50
	bl Func_0200412c
	movs r1, #0
	movs r0, #18
	bl Func_02004134
	movs r0, #0
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	bne .L_02009a04
	ldr r0, .L_02009a54
.L_020099f6:
	bl Func_0200412c
.L_020099fa:
	movs r0, #18
	movs r1, #0
	bl Func_02004144
	b .L_02009a3c
.L_02009a04:
	ldr r0, .L_02009a58
	bl Func_0200412c
	movs r1, #0
	movs r0, #18
	bl Func_02004144
	movs r0, #10
	adds r0, #255
	bl Func_02003fcc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #229
	bl Func_02003fcc
	movs r0, #1
	bl WaitFrames
	movs r0, #254
	lsls r0, r0, #1
	movs r1, #0
	bl Func_020041c4
	ldr r0, .L_02009a5c
	movs r1, #20
	bl Func_020041bc
.L_02009a3c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009a40:
	.4byte 0x0000139f
.L_02009a44:
	.4byte gPartyState
.L_02009a48:
	.4byte 0x000013a5
.L_02009a4c:
	.4byte 0x000013ae
.L_02009a50:
	.4byte 0x000013af
.L_02009a54:
	.4byte 0x000013ad
.L_02009a58:
	.4byte 0x000013a6
.L_02009a5c:
	.4byte 0x000000ee
	.section .text.x02009a60,"ax",%progbits
	.global Func_02001a60
	.thumb_func
Func_02001a60:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r0, #0
	cmp r7, #0
	blt .L_02009aae
	cmp r7, #5
	bne .L_02009a7c
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsrs r7, r3, #16
.L_02009a7c:
	ldr r3, .L_02009ab4
	mov r8, r3
	movs r3, #170
	lsls r3, r3, #1
	adds r6, r7, r3
	mov r3, r8
	ldrsb r5, [r3, r6]
	bl Random16Far
	lsls r0, r0, #1
	lsrs r0, r0, #16
	adds r5, r5, r0
	adds r5, #4
	adds r0, r5, #0
	movs r1, #3
	bl Engine_MathRemainder
	mov r3, r8
	strb r0, [r3, r6]
	lsls r3, r7, #1
	ldr r2, .L_02009ab8
	adds r3, r3, r7
	adds r3, r3, r0
	lsls r3, r3, #2
	ldr r0, [r2, r3]
.L_02009aae:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02009ab4:
	.4byte gPartyState
.L_02009ab8:
	.4byte Data_02004630
	.section .text.x02009abc,"ax",%progbits
	.global Func_02001abc
	.thumb_func
Func_02001abc:
	push {r5, r6, lr}
	bl Func_0200404c
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r5, r3, #1
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	ldr r6, .L_02009b70
	ldr r3, [r6, #16]
	cmp r3, r5
	bcs .L_02009aea
	ldr r0, .L_02009b74
	bl Func_0200412c
	movs r0, #14
	movs r1, #0
	bl Func_02004134
	b .L_02009b6e
.L_02009aea:
	ldr r0, .L_02009b78
	bl Func_0200412c
	adds r0, r5, #0
	movs r1, #5
	bl Func_0200403c
	movs r1, #0
	movs r0, #14
	bl Func_02004134
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r0, [r3]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009b50
	ldr r3, .L_02009b7c
	ldr r2, [r6, #16]
	str r2, [r3]
	bl Func_0200422c
	movs r1, #0
	movs r0, #14
	bl Func_02004144
	movs r0, #10
	adds r0, #255
	bl Func_02003fcc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #229
	bl Func_02003fcc
	movs r0, #1
	bl WaitFrames
	movs r0, #254
	adds r0, #255
	movs r1, #0
	bl Func_020041c4
	ldr r0, .L_02009b80
	movs r1, #19
	bl Func_020041bc
	b .L_02009b6a
.L_02009b50:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #14
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_02004144
.L_02009b6a:
	bl Func_02004074
.L_02009b6e:
	pop {r5, r6, pc}
.L_02009b70:
	.4byte gPartyState
.L_02009b74:
	.4byte 0x00001386
.L_02009b78:
	.4byte 0x00001382
.L_02009b7c:
	.4byte gSceneState
.L_02009b80:
	.4byte 0x000000ee
	.section .text.x02009b84,"ax",%progbits
	.global Func_02001b84
	.thumb_func
Func_02001b84:
	push {r5, r6, lr}
	bl Func_0200404c
	lsls r5, r0, #2
	adds r5, r5, r0
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	ldr r6, .L_02009c30
	lsls r5, r5, #1
	ldr r3, [r6, #16]
	cmp r3, r5
	bcs .L_02009bb2
	ldr r0, .L_02009c34
	bl Func_0200412c
	movs r0, #14
	movs r1, #0
	bl Func_02004134
	b .L_02009c2e
.L_02009bb2:
	ldr r0, .L_02009c38
	bl Func_0200412c
	movs r1, #0
	movs r0, #8
	bl Func_02004134
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r0, [r3]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009c10
	ldr r3, .L_02009c3c
	ldr r2, [r6, #16]
	str r2, [r3]
	bl Func_0200422c
	movs r1, #0
	movs r0, #8
	bl Func_02004144
	movs r0, #10
	adds r0, #255
	bl Func_02003fcc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #229
	bl Func_02003fcc
	movs r0, #1
	bl WaitFrames
	movs r0, #252
	adds r0, #255
	movs r1, #0
	bl Func_020041c4
	ldr r0, .L_02009c40
	movs r1, #18
	bl Func_020041bc
	b .L_02009c2a
.L_02009c10:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #8
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_02004144
.L_02009c2a:
	bl Func_02004074
.L_02009c2e:
	pop {r5, r6, pc}
.L_02009c30:
	.4byte gPartyState
.L_02009c34:
	.4byte 0x000013e2
.L_02009c38:
	.4byte 0x000013cb
.L_02009c3c:
	.4byte gSceneState
.L_02009c40:
	.4byte 0x000000ee
	.section .text.x02009c44,"ax",%progbits
	.global Func_02001c44
	.thumb_func
Func_02001c44:
	push {lr}
	bl Object_GetById
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r0, #24]
	str r3, [r0, #28]
	movs r3, #8
	strh r3, [r0, #32]
	pop {pc}
	.2byte 0x0000
	.section .text.x02009c5c,"ax",%progbits
	.global Func_02001c5c
	.thumb_func
Func_02001c5c:
	push {r5, r6, lr}
	mov r6, r11
	mov r5, r10
	push {r5, r6}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	sub sp, #8
	mov r11, r3
	mov r9, r0
	adds r5, r1, #0
	adds r6, r2, #0
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	movs r0, #188
	bl Func_020042b4
	movs r3, #1
	mov r8, r3
	movs r3, #2
	mov r10, r3
	mov r3, r8
	str r3, [sp, #0]
	mov r3, r10
	str r3, [sp, #4]
	mov r0, r9
	adds r1, r5, #0
	adds r2, r6, #0
	mov r3, r11
	bl Func_02004004
	mov r3, r8
	str r3, [sp, #0]
	adds r5, #2
	mov r3, r10
	adds r6, #1
	str r3, [sp, #4]
	adds r1, r5, #0
	adds r2, r6, #0
	mov r3, r11
	mov r0, r9
	bl Func_02004004
	movs r0, #20
	bl Battle_WaitMode0
	ldr r5, .L_02009d00
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	ldr r0, [r5]
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_02009d04
	ldr r0, [r5]
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #12
	bl WaitFrames
	movs r0, #123
	bl Func_020042b4
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r3}
	mov r11, r3
	pop {r5, r6, pc}
.L_02009d00:
	.4byte gPartyState
.L_02009d04:
	.4byte Data_0200466c
	.section .text.x02009d08,"ax",%progbits
	.global Func_02001d08
	.thumb_func
Func_02001d08:
	push {lr}
	movs r0, #27
	movs r1, #0
	movs r2, #42
	movs r3, #13
	bl Func_02001c5c
	movs r0, #22
	bl Func_020041b4
	pop {pc}
	.2byte 0x0000
	.section .text.x02009d20,"ax",%progbits
	.global Func_02001d20
	.thumb_func
Func_02001d20:
	push {lr}
	movs r0, #27
	movs r1, #4
	movs r2, #48
	movs r3, #12
	bl Func_02001c5c
	movs r0, #23
	bl Func_020041b4
	pop {pc}
	.2byte 0x0000
	.section .text.x02009d38,"ax",%progbits
	.global Func_02001d38
	.thumb_func
Func_02001d38:
	push {lr}
	movs r0, #27
	movs r1, #0
	movs r2, #54
	movs r3, #13
	bl Func_02001c5c
	movs r0, #24
	bl Func_020041b4
	pop {pc}
	.2byte 0x0000
	.section .text.x02009d50,"ax",%progbits
	.global Func_02001d50
	.thumb_func
Func_02001d50:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #8
	sub sp, #28
	bl Object_GetById
	adds r7, r0, #0
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	movs r0, #78
	bl Func_020042b4
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r5, .L_0200a174
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r1, [r5]
	movs r0, #0
	bl Object_LinkObjectAndSetCallback
	ldr r1, [r5]
	movs r0, #1
	bl Object_LinkObjectAndSetCallback
	ldr r1, [r5]
	movs r0, #3
	bl Object_LinkObjectAndSetCallback
	ldr r1, [r5]
	movs r0, #2
	bl Object_LinkObjectAndSetCallback
	ldr r1, [r5]
	movs r0, #8
	bl Object_LinkObjectAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #191
	ldr r0, [r5]
	movs r1, #160
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #0
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Func_02004154
	ldr r1, [r5]
	movs r0, #5
	bl Func_020040e4
	ldr r1, [r5]
	movs r0, #7
	bl Func_020040e4
	ldr r1, [r5]
	movs r0, #6
	bl Func_020040e4
	ldr r1, [r5]
	movs r0, #9
	bl Func_020040e4
	movs r0, #1
	bl WaitFrames
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #5
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #7
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	movs r0, #6
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_0200a178
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a17c
	movs r0, #9
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a180
	movs r0, #6
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a184
	movs r0, #7
	bl Object_SetActionCallbackAndRefreshById
	ldr r0, [r5]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #3
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #2
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #8
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r2, r2
	negs r1, r1
	negs r0, r0
	bl Func_02004194
	movs r0, #43
	bl Func_020042b4
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #0
	bl Motion_SetModeAndWaitAnimation
	ldr r0, .L_0200a188
	bl Func_0200412c
	movs r0, #0
	movs r1, #0
	bl Func_02004144
	movs r0, #1
	movs r1, #3
	bl Object_SetModeById
	movs r0, #1
	movs r1, #0
	bl Func_02004144
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #5
	bl Func_0200417c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Func_02004144
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #0
	bl Func_02004144
	movs r0, #3
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #3
	movs r1, #0
	bl Func_02004144
	movs r0, #2
	movs r1, #4
	bl Object_SetModeById
	movs r0, #2
	movs r1, #0
	bl Func_02004144
	movs r1, #129
	movs r0, #7
	lsls r1, r1, #1
	bl Func_0200417c
	movs r0, #7
	movs r1, #0
	bl Func_02004144
	movs r1, #4
	movs r2, #0
	adds r1, #255
	movs r0, #0
	bl Func_02004174
	movs r0, #0
	movs r1, #0
	bl Func_02004144
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	movs r1, #192
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #8
	bl Func_02004154
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	bl Func_0200415c
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_0200418c
	movs r0, #148
	movs r1, #1
	movs r2, #184
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	bl Func_02004194
	movs r2, #171
	movs r0, #9
	movs r1, #172
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #224
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #224
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r2, #171
	lsls r2, r2, #1
	movs r0, #9
	movs r1, #146
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_0200415c
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	movs r2, #10
	movs r0, #1
	movs r1, #2
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #1
	movs r1, #0
	bl Func_02004144
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	movs r0, #3
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #3
	movs r1, #0
	bl Func_02004144
	movs r0, #2
	movs r1, #0
	bl Func_02004144
	movs r1, #6
	movs r2, #20
	adds r1, #255
	movs r0, #5
	bl Func_02004174
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	bl Func_0200415c
	movs r0, #5
	movs r1, #0
	bl Func_02004144
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl Func_02004154
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #0
	bl Func_02004174
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #1
	bl Func_02004174
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #1
	movs r1, #0
	bl Func_02004144
	movs r0, #9
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	movs r1, #192
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #8
	bl Func_02004154
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	bl Func_0200415c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #5
	movs r1, #0
	bl Func_02004144
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl Func_02004154
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #1
	bl Func_02004174
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #0
	bl Func_02004174
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	movs r0, #5
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	movs r1, #0
	bl Func_02004144
	movs r0, #1
	movs r1, #3
	bl Object_SetModeById
	movs r0, #0
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	movs r1, #10
	adds r1, #255
	movs r2, #20
	movs r0, #0
	bl Func_02004174
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r0, #0
	movs r1, #0
	movs r2, #40
	bl Func_0200413c
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #2
	bl Func_02004174
	movs r0, #2
	movs r1, #0
	bl Func_02004144
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #3
	bl Func_02004174
	movs r1, #128
	movs r0, #3
	b .L_0200a18c
.L_0200a174:
	.4byte gPartyState
.L_0200a178:
	.4byte Data_020048a0
.L_0200a17c:
	.4byte Data_02004980
.L_0200a180:
	.4byte Data_0200493c
.L_0200a184:
	.4byte Data_020048f8
.L_0200a188:
	.4byte 0x00002b5d
.L_0200a18c:
	lsls r1, r1, #7
	bl Func_0200415c
	movs r0, #3
	movs r1, #0
	bl Func_02004144
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #2
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #224
	movs r0, #2
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #0
	bl Func_02004174
	movs r0, #0
	movs r1, #0
	bl Func_02004144
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl Func_02004154
	movs r1, #2
	movs r2, #0
	adds r1, #255
	movs r0, #1
	bl Func_02004174
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #3
	bl Func_02004174
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #6
	bl Func_0200415c
	movs r1, #0
	movs r0, #3
	bl Func_02004134
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004154
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004154
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02004154
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	ldr r5, [r5]
	cmp r0, #0
	bne .L_0200a28a
	adds r0, r5, #0
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #2
	movs r1, #3
	bl Object_SetModeById
	movs r0, #2
	movs r1, #0
	bl Func_02004144
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a2b2
.L_0200a28a:
	adds r0, r5, #0
	bl ObjectMotion_WaitForAnimationChange
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #2
	adds r3, #1
	movs r1, #4
	strh r3, [r2]
	bl Object_SetModeById
	movs r0, #2
	movs r1, #0
	bl Func_02004144
.L_0200a2b2:
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_0200415c
	movs r0, #9
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	ldr r5, .L_0200a4f4
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl Func_02004154
	movs r1, #192
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #8
	bl Func_02004154
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	bl Func_0200415c
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #9
	bl Func_02004174
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #0
	bl Func_02004174
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #224
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #8
	bl Func_02004154
	movs r0, #0
	movs r1, #0
	bl Func_02004144
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #7
	bl Func_0200415c
	movs r0, #7
	movs r1, #4
	bl Object_SetModeById
	movs r0, #7
	movs r1, #0
	bl Func_02004144
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02004154
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl Func_02004154
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #0
	bl Func_02004174
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #1
	bl Func_02004174
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #3
	bl Func_02004174
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #2
	bl Func_02004174
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #6
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #6
	movs r1, #0
	bl Func_02004144
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004154
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl Func_02004154
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #3
	bl Func_02004174
	movs r0, #3
	movs r1, #0
	bl Func_02004144
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #10
	movs r2, #40
	adds r1, #255
	movs r0, #9
	bl Func_02004174
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl Func_0200417c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #1
	movs r1, #0
	bl Func_02004144
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #80
	movs r0, #9
	movs r1, #0
	bl Func_0200413c
	movs r0, #0
	movs r1, #0
	bl Func_02004144
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	bl Func_0200415c
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #1
	bl Func_02004134
	movs r1, #160
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004154
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02004154
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	ldr r5, [r5]
	cmp r0, #0
	bne .L_0200a4f8
	adds r0, r5, #0
	bl ObjectMotion_WaitForAnimationChange
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl Func_0200417c
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #1
	movs r1, #0
	bl Func_02004144
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a528
.L_0200a4f4:
	.4byte gPartyState
.L_0200a4f8:
	adds r0, r5, #0
	bl ObjectMotion_WaitForAnimationChange
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #1
	bl Func_02004174
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #7
	strh r3, [r2]
	adds r0, #1
	movs r1, #0
	bl Func_02004144
.L_0200a528:
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02004154
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #0
	bl Func_02004174
	movs r0, #0
	movs r1, #0
	bl Func_02004144
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_0200415c
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #0
	bl Func_02004174
	movs r1, #6
	movs r2, #20
	adds r1, #255
	movs r0, #1
	bl Func_02004174
	movs r0, #0
	bl Func_020042b4
	movs r0, #8
	bl Object_GetById
	movs r1, #15
	bl Func_02004124
	movs r2, #176
	lsls r2, r2, #8
	mov r8, r2
	movs r1, #160
	movs r2, #233
	lsls r2, r2, #17
	mov r3, r8
	lsls r1, r1, #16
	movs r0, #8
	bl Func_020040dc
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	ldr r6, .L_0200a9d0
	movs r3, #133
	lsls r3, r3, #2
	adds r6, r6, r3
	ldr r0, [r6]
	movs r1, #8
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #8
	bl Object_LinkObjectAndSetCallback
	movs r0, #7
	movs r1, #8
	bl Object_LinkObjectAndSetCallback
	movs r0, #6
	movs r1, #8
	bl Object_LinkObjectAndSetCallback
	movs r0, #0
	movs r1, #8
	bl Object_LinkObjectAndSetCallback
	movs r0, #1
	movs r1, #8
	bl Object_LinkObjectAndSetCallback
	movs r0, #3
	movs r1, #8
	bl Object_LinkObjectAndSetCallback
	movs r1, #8
	movs r0, #2
	bl Object_LinkObjectAndSetCallback
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl Func_02004124
	movs r1, #152
	lsls r1, r1, #7
	ldr r0, .L_0200a9d4
	adds r1, #204
	bl Func_0200418c
	movs r0, #180
	movs r1, #1
	movs r2, #205
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	bl Func_02004194
	bl Func_0200419c
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_0200418c
	movs r0, #148
	movs r1, #1
	movs r2, #184
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	bl Func_02004194
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #8
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #203
	movs r0, #8
	movs r1, #146
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #3
	bl Func_02004174
	movs r1, #0
	movs r0, #3
	bl Func_02004144
	ldr r0, [r6]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #5
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #7
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #6
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #0
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #3
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #2
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #225
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020042b4
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	ldr r0, [r6]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #1
	movs r1, #3
	bl Object_SetModeById
	movs r0, #3
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #2
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #9
	bl Func_02004174
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	bl Func_0200415c
	movs r1, #3
	movs r0, #0
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #10
	movs r2, #8
	movs r1, #9
	movs r0, #20
	movs r4, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	str r0, [sp, #12]
	str r3, [sp, #20]
	str r3, [sp, #24]
	movs r0, #0
	movs r3, #8
	movs r1, #5
	movs r2, #5
	str r4, [sp, #16]
	bl Func_02004164
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r6]
	bl Func_02004174
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_02004174
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_02004174
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_02004174
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #0
	bl Func_02004174
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #1
	bl Func_02004174
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #3
	bl Func_02004174
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #2
	bl Func_02004174
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02004174
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl Func_0200417c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #1
	movs r1, #0
	bl Func_02004144
	movs r0, #3
	movs r1, #0
	bl Func_02004144
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #6
	movs r1, #0
	bl Func_02004144
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #8
	bl Func_02004174
	movs r0, #8
	movs r1, #0
	bl Func_0200415c
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #8
	movs r1, #0
	bl Func_02004144
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #2
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #200
	movs r0, #2
	movs r1, #130
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #0
	movs r0, #2
	movs r1, #0
	bl Func_02004154
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #8
	movs r1, #0
	bl Func_02004144
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #2
	bl Func_02004174
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #8
	movs r1, #0
	bl Func_02004144
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl Func_02004154
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004154
	movs r1, #192
	movs r2, #40
	movs r0, #6
	lsls r1, r1, #8
	bl Func_02004154
	movs r1, #208
	movs r0, #8
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02004154
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004154
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004154
	movs r1, #192
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #7
	bl Func_02004154
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl Func_0200413c
	ldr r5, .L_0200a9d8
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_02003e54
	movs r0, #80
	bl Battle_WaitMode0
	adds r0, r7, #0
	bl Func_02003f54
	movs r0, #8
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	movs r1, #129
	movs r0, #7
	lsls r1, r1, #1
	bl Func_0200417c
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_0200413c
	adds r1, r5, #0
	adds r0, r7, #0
	bl Func_02003e54
	b .L_0200a9dc
	.2byte 0x0000
.L_0200a9d0:
	.4byte gPartyState
.L_0200a9d4:
	.4byte 0x00026666
.L_0200a9d8:
	.4byte gOverlayArea + 0x6030
.L_0200a9dc:
	movs r0, #80
	bl Battle_WaitMode0
	adds r0, r7, #0
	bl Func_02003f54
	movs r1, #208
	movs r0, #8
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl Func_0200417c
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl Func_0200417c
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	bl Func_0200417c
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_0200417c
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	bl Func_0200417c
	movs r1, #129
	movs r0, #7
	lsls r1, r1, #1
	bl Func_0200417c
	movs r1, #129
	movs r0, #6
	lsls r1, r1, #1
	bl Func_0200417c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #2
	bl Func_0200417c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #8
	mov r1, r8
	bl Func_0200415c
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02004154
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl Func_02004154
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl Func_02004154
	movs r1, #224
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r2, #10
	movs r0, #9
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #1
	movs r1, #3
	bl Object_SetModeById
	movs r0, #3
	movs r1, #3
	bl Object_SetModeById
	movs r0, #2
	movs r1, #3
	bl Object_SetModeById
	ldr r0, [r6]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #8
	bl Func_02004174
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	movs r1, #0
	movs r0, #8
	bl Func_02004144
	movs r0, #78
	bl Func_020042b4
	movs r0, #180
	movs r1, #1
	movs r2, #205
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	bl Func_02004194
	movs r2, #233
	movs r0, #8
	movs r1, #160
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #0
	movs r0, #8
	movs r1, #0
	bl Func_020040d4
	movs r1, #152
	lsls r1, r1, #7
	ldr r0, .L_0200ae0c
	adds r1, #204
	bl Func_0200418c
	movs r0, #148
	movs r1, #1
	movs r2, #184
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	bl Func_02004194
	bl Func_0200419c
	bl Func_0200420c
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #5
	bl Func_02004174
	movs r0, #5
	movs r1, #0
	bl Func_02004144
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004154
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl Func_02004154
	movs r2, #0
	movs r0, #1
	movs r1, #0
	bl Func_02004154
	movs r0, #0
	movs r1, #0
	bl Func_0200415c
	movs r0, #1
	movs r1, #3
	bl Object_SetModeById
	movs r0, #0
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #0
	movs r0, #5
	movs r1, #2
	bl ObjectMotion_Launch
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	bl Func_0200415c
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #0
	bl Func_02004144
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	bl Func_0200415c
	movs r1, #129
	movs r0, #6
	lsls r1, r1, #1
	bl Func_0200417c
	movs r0, #6
	movs r1, #0
	bl Func_02004144
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02004154
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #3
	bl Func_02004174
	movs r0, #3
	movs r1, #0
	bl Func_02004144
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl Func_02004154
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #0
	bl Func_02004144
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #1
	bl Func_02004174
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #1
	movs r1, #0
	bl Func_02004144
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #0
	movs r1, #0
	bl Func_02004144
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_02004174
	movs r1, #128
	movs r2, #20
	movs r0, #5
	lsls r1, r1, #7
	bl Func_02004154
	movs r1, #0
	movs r0, #5
	bl Func_02004134
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200ad02
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	bl Func_0200415c
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #9
	movs r1, #0
	bl Func_02004144
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200ad32
.L_0200ad02:
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	bl Func_0200415c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #9
	adds r3, #1
	movs r1, #4
	strh r3, [r2]
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #9
	movs r1, #0
	bl Func_02004144
.L_0200ad32:
	movs r2, #184
	movs r0, #9
	movs r1, #146
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #40
	bl Func_02004154
	movs r2, #40
	movs r0, #9
	movs r1, #0
	bl Func_02004154
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_0200415c
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #9
	movs r1, #0
	bl Func_02004144
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #1
	movs r1, #3
	bl Object_SetModeById
	movs r0, #3
	movs r1, #3
	bl Object_SetModeById
	movs r0, #2
	movs r1, #3
	bl Object_SetModeById
	ldr r3, .L_0200ae10
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	ldr r5, .L_0200ae14
	movs r0, #0
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #1
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #3
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #2
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #9
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #6
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #7
	bl Object_SetActionCallbackAndRefreshById
	bl Func_02004044
	movs r0, #157
	lsls r0, r0, #4
	bl Func_02003fcc
	bl Func_02004074
	add sp, #28
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200ae0c:
	.4byte 0x00026666
.L_0200ae10:
	.4byte gPartyState
.L_0200ae14:
	.4byte Data_020049c4
	.section .text.x0200ae18,"ax",%progbits
	.global Func_02002e18
	.thumb_func
Func_02002e18:
	push {r5, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl Func_02003fc4
	cmp r0, #0
	bne .L_0200ae2c
	bl .L_0200b68a
.L_0200ae2c:
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_0200418c
	movs r0, #160
	movs r1, #1
	movs r2, #184
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	bl Func_02004194
	ldr r5, .L_0200b254
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #197
	ldr r0, [r5]
	movs r1, #158
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r2, #0
	movs r0, #8
	lsls r1, r1, #7
	bl Func_02004154
	ldr r1, [r5]
	movs r0, #2
	bl Func_020040e4
	movs r0, #1
	bl WaitFrames
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #2
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #187
	lsls r2, r2, #1
	movs r0, #2
	movs r1, #144
	bl ObjectMotion_SetPositionAndReset
	movs r1, #224
	movs r0, #2
	lsls r1, r1, #8
	bl Func_0200415c
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #8
	bl Func_02004174
	ldr r0, .L_0200b258
	bl Func_0200412c
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	movs r0, #2
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #2
	movs r1, #0
	bl Func_02004144
	movs r0, #2
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #2
	movs r1, #0
	bl Func_02004144
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #40
	bl Func_02004154
	movs r2, #182
	movs r1, #180
	lsls r2, r2, #1
	movs r0, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #194
	movs r1, #186
	lsls r2, r2, #1
	movs r0, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #40
	bl Battle_WaitMode0
	movs r2, #190
	movs r0, #2
	movs r1, #146
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #2
	bl Func_02004174
	movs r0, #2
	movs r1, #0
	bl Func_02004144
	movs r1, #4
	movs r2, #20
	adds r1, #255
	movs r0, #8
	bl Func_02004174
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #2
	bl Func_02004174
	movs r1, #224
	movs r2, #40
	movs r0, #2
	lsls r1, r1, #8
	bl Func_02004154
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #2
	bl Func_02004174
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #2
	movs r1, #0
	bl Func_02004144
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	bl Func_0200417c
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #2
	movs r1, #0
	bl Func_02004144
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #8
	bl Func_02004174
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	ldr r1, [r5]
	movs r0, #0
	bl Func_020040e4
	movs r0, #1
	bl WaitFrames
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #0
	bl Func_02004174
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #0
	ldr r1, .L_0200b25c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #190
	lsls r2, r2, #1
	movs r0, #0
	movs r1, #168
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #0
	movs r1, #0
	bl Func_02004144
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #8
	bl Func_02004174
	movs r1, #192
	movs r2, #0
	movs r0, #8
	lsls r1, r1, #6
	bl Func_02004154
	ldr r1, [r5]
	movs r0, #9
	bl Func_020040e4
	movs r0, #1
	bl WaitFrames
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	ldr r0, [r5]
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	ldr r1, .L_0200b260
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #189
	movs r0, #9
	movs r1, #182
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #180
	lsls r2, r2, #1
	movs r0, #9
	movs r1, #182
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	bl Func_0200415c
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #9
	bl Func_02004174
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #9
	movs r1, #0
	bl Func_02004144
	movs r2, #0
	movs r0, #8
	movs r1, #0
	bl Func_02004154
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	bl Func_0200417c
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #9
	movs r1, #0
	bl Func_02004144
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #8
	movs r1, #0
	bl Func_02004144
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #9
	bl Func_02004174
	movs r1, #2
	adds r1, #255
	movs r2, #0
	ldr r0, [r5]
	bl Func_02004174
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #0
	bl Func_02004174
	movs r1, #2
	movs r2, #40
	adds r1, #255
	movs r0, #2
	bl Func_02004174
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	bl Func_0200415c
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #9
	movs r1, #0
	bl Func_02004144
	movs r0, #8
	movs r1, #0
	bl Func_0200415c
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #6
	movs r2, #40
	bl Func_02004154
	movs r2, #0
	ldr r0, [r5]
	movs r1, #0
	bl Func_02004154
	movs r1, #224
	movs r0, #2
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #2
	movs r1, #0
	bl Func_02004144
	movs r1, #4
	adds r1, #255
	movs r2, #0
	movs r0, #8
	bl Func_02004174
	movs r1, #160
	movs r2, #20
	movs r0, #8
	lsls r1, r1, #7
	bl Func_02004154
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #40
	bl Func_02004154
	movs r1, #128
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02004154
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #0
	movs r1, #0
	bl Func_02004144
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	bl Func_0200417c
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	bl Func_0200415c
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #8
	movs r1, #0
	bl Func_02004144
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r5]
	bl Func_02004174
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #0
	bl Func_02004174
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #2
	bl Func_02004174
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02004174
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	bl Func_0200415c
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #160
	b .L_0200b264
.L_0200b254:
	.4byte gPartyState
.L_0200b258:
	.4byte 0x00002c05
.L_0200b25c:
	.4byte 0x00013333
.L_0200b260:
	.4byte Data_02004ac0
.L_0200b264:
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004154
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	movs r2, #40
	ldr r0, [r5]
	lsls r1, r1, #6
	bl Func_02004154
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #0
	bl Func_0200417c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #0
	movs r1, #0
	bl Func_02004144
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl Func_02004154
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #224
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r1, #10
	adds r1, #255
	movs r2, #0
	movs r0, #8
	bl Func_02004174
	movs r1, #192
	movs r2, #0
	movs r0, #8
	lsls r1, r1, #6
	bl Func_02004154
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #8
	movs r1, #0
	bl Func_02004144
	movs r0, #2
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #2
	movs r1, #0
	bl Func_02004144
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	bl Func_0200415c
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	movs r1, #6
	adds r1, #255
	movs r2, #20
	movs r0, #9
	bl Func_02004174
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	movs r0, #8
	movs r1, #0
	movs r2, #80
	bl Func_02004154
	movs r1, #6
	adds r1, #255
	movs r2, #40
	movs r0, #9
	bl Func_02004174
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #0
	bl Func_02004174
	movs r1, #2
	adds r1, #255
	movs r2, #0
	ldr r0, [r5]
	bl Func_02004174
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #2
	bl Func_02004174
	movs r1, #131
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02004174
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #9
	movs r1, #0
	bl Func_02004144
	movs r0, #128
	lsls r0, r0, #5
	movs r1, #0
	bl Func_02004144
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #9
	bl Func_02004174
	movs r1, #160
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02004154
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	movs r2, #10
	movs r0, #2
	movs r1, #2
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #2
	movs r1, #0
	bl Func_02004144
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_0200415c
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r0, #9
	bl Func_02004134
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #2
	bl Func_02004174
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200b42e
	movs r1, #224
	movs r0, #2
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #2
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #2
	movs r1, #0
	bl Func_02004144
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200b45e
.L_0200b42e:
	movs r1, #224
	movs r0, #2
	lsls r1, r1, #8
	bl Func_0200415c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #2
	adds r3, #1
	movs r1, #4
	strh r3, [r2]
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #2
	movs r1, #0
	bl Func_02004144
.L_0200b45e:
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_0200415c
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r1, #0
	movs r0, #9
	bl Func_02004134
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #0
	bl Func_02004174
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004154
	ldr r3, .L_0200b68c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200b4d4
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #5
	movs r1, #0
	bl Func_02004144
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200b502
.L_0200b4d4:
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	bl Func_0200415c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #0
	adds r3, #1
	movs r1, #4
	strh r3, [r2]
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #5
	movs r1, #0
	bl Func_02004144
.L_0200b502:
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #9
	bl Func_02004174
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	bl Func_0200417c
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #2
	movs r1, #0
	bl Func_02004144
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_0200415c
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #5
	movs r1, #0
	bl Func_02004144
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #8
	bl Func_02004174
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #8
	movs r1, #0
	bl Func_02004144
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #9
	movs r1, #0
	bl Func_02004144
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_0200415c
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #9
	bl Func_02004174
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #0
	movs r1, #0
	bl Func_02004144
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	bl Func_0200417c
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #2
	movs r1, #0
	bl Func_02004144
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	bl Func_0200415c
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #0
	bl Func_02004144
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	bl Func_0200415c
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02004144
	ldr r3, .L_0200b68c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #2
	ldr r1, .L_0200b690
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #0
	ldr r1, .L_0200b690
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #9
	ldr r1, .L_0200b690
	bl ObjectMotion_SetSpeedParameters
	ldr r5, .L_0200b694
	movs r0, #2
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #0
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #9
	adds r1, r5, #0
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #209
	bl Func_02003fcc
	bl Func_02004074
.L_0200b68a:
	pop {r5, pc}
.L_0200b68c:
	.4byte gPartyState
.L_0200b690:
	.4byte 0x00013333
.L_0200b694:
	.4byte Data_02004b18
	.section .text.x0200b698,"ax",%progbits
	.global Func_02003698
	.thumb_func
Func_02003698:
	push {lr}
	ldr r3, .L_0200b6d8
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200b6dc
	cmp r2, r3
	bne .L_0200b6b2
	bl Func_02003720
	b .L_0200b6d4
.L_0200b6b2:
	ldr r3, .L_0200b6e0
	cmp r2, r3
	bne .L_0200b6be
	bl Func_02003880
	b .L_0200b6d4
.L_0200b6be:
	ldr r3, .L_0200b6e4
	cmp r2, r3
	bne .L_0200b6ca
	bl Func_020039d4
	b .L_0200b6d4
.L_0200b6ca:
	ldr r3, .L_0200b6e8
	cmp r2, r3
	bne .L_0200b6d4
	bl Func_02003a8c
.L_0200b6d4:
	movs r0, #0
	pop {pc}
.L_0200b6d8:
	.4byte gPartyState
.L_0200b6dc:
	.4byte 0x000000ec
.L_0200b6e0:
	.4byte 0x000000ef
.L_0200b6e4:
	.4byte 0x000000ed
.L_0200b6e8:
	.4byte 0x000000ee
	.section .text.x0200b6ec,"ax",%progbits
	.global Func_020036ec
	.thumb_func
Func_020036ec:
	movs r0, #0
	bx lr
	.section .text.x0200b6f0,"ax",%progbits
	.global Func_020036f0
	.thumb_func
Func_020036f0:
	push {lr}
	movs r0, #65
	bl Object_GetById
	cmp r0, #0
	beq .L_0200b718
	ldr r3, .L_0200b71c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #80]
	movs r0, #65
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl Func_0200416c
.L_0200b718:
	pop {pc}
	.2byte 0x0000
.L_0200b71c:
	.4byte gPartyState
	.section .text.x0200b720,"ax",%progbits
	.global Func_02003720
	.thumb_func
Func_02003720:
	push {r5, lr}
	movs r0, #14
	bl Object_GetById
	movs r1, #144
	adds r5, r0, #0
	lsls r1, r1, #3
	ldr r0, .L_0200b864
	bl Func_02003f7c
	adds r1, r5, #0
	movs r3, #1
	adds r1, #98
	strb r3, [r1]
	adds r3, r5, #0
	adds r3, #99
	movs r2, #0
	strb r2, [r3]
	ldr r3, .L_0200b868
	movs r0, #160
	lsls r0, r0, #4
	str r3, [r5, #108]
	adds r0, #33
	bl Func_02003fc4
	cmp r0, #0
	beq .L_0200b784
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl Func_02003fc4
	cmp r0, #0
	bne .L_0200b784
	movs r0, #15
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	bl Func_02001724
	b .L_0200b79c
.L_0200b784:
	movs r0, #15
	bl Object_GetById
	adds r5, r0, #0
	adds r2, r5, #0
	movs r3, #0
	adds r2, #98
	strb r3, [r2]
	adds r2, #1
	strb r3, [r2]
	ldr r3, .L_0200b868
	str r3, [r5, #108]
.L_0200b79c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #231
	bl Func_02003fc4
	cmp r0, #0
	beq .L_0200b7c8
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #230
	bl Func_02003fc4
	cmp r0, #0
	bne .L_0200b7c8
	movs r1, #128
	movs r2, #177
	movs r0, #17
	lsls r1, r1, #14
	lsls r2, r2, #18
	bl Func_020040d4
	b .L_0200b7e0
.L_0200b7c8:
	movs r0, #17
	bl Object_GetById
	movs r1, #15
	bl Func_02004124
	movs r0, #17
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
.L_0200b7e0:
	movs r0, #10
	adds r0, #255
	bl Func_02003fc4
	cmp r0, #0
	bne .L_0200b81c
	ldr r3, .L_0200b86c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #4
	bgt .L_0200b80a
	cmp r3, #1
	blt .L_0200b80a
	movs r0, #128
	lsls r0, r0, #2
	bl Func_02003fcc
	b .L_0200b81c
.L_0200b80a:
	movs r1, #128
	movs r2, #128
	movs r3, #128
	movs r0, #0
	lsls r1, r1, #18
	lsls r2, r2, #18
	lsls r3, r3, #19
	bl Func_020041a4
.L_0200b81c:
	movs r0, #128
	lsls r0, r0, #2
	bl Func_02003fc4
	cmp r0, #0
	beq .L_0200b854
	movs r1, #128
	movs r2, #220
	movs r3, #128
	movs r0, #0
	lsls r1, r1, #18
	lsls r2, r2, #18
	lsls r3, r3, #19
	bl Func_020041a4
	ldr r3, .L_0200b86c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	bl Func_02004184
	bl Func_02003ffc
	movs r0, #1
	bl WaitFrames
.L_0200b854:
	ldr r0, .L_0200b870
	ldr r1, .L_0200b874
	ldr r2, .L_0200b878
	ldr r3, .L_0200b87c
	bl Func_02000038
	pop {r5, pc}
	.2byte 0x0000
.L_0200b864:
	.4byte Func_020036f0
.L_0200b868:
	.4byte Func_020006d0
.L_0200b86c:
	.4byte gPartyState
.L_0200b870:
	.4byte Data_0200469c
.L_0200b874:
	.4byte Data_020046ac
.L_0200b878:
	.4byte Data_020046d8
.L_0200b87c:
	.4byte Data_02004704
	.section .text.x0200b880,"ax",%progbits
	.global Func_02003880
	.thumb_func
Func_02003880:
	push {lr}
	movs r0, #18
	sub sp, #8
	bl Func_02001c44
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #33
	bl Func_02003fc4
	cmp r0, #0
	bne .L_0200b89a
	b .L_0200b9cc
.L_0200b89a:
	movs r3, #44
	movs r2, #36
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #3
	movs r0, #44
	movs r1, #40
	movs r2, #8
	bl Func_0200400c
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #24
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #31
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #32
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #33
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl Func_02003fc4
	cmp r0, #0
	bne .L_0200b9b8
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	b .L_0200b9d0
.L_0200b9b8:
	movs r0, #10
	bl Object_GetById
	movs r3, #208
	lsls r3, r3, #8
	strh r3, [r0, #6]
	movs r0, #1
	bl WaitFrames
	b .L_0200b9d0
.L_0200b9cc:
	bl Func_020017f4
.L_0200b9d0:
	add sp, #8
	pop {pc}
	.section .text.x0200b9d4,"ax",%progbits
	.global Func_020039d4
	.thumb_func
Func_020039d4:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	ldr r3, .L_0200ba84
	subs r2, #39
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #4
	bne .L_0200ba14
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #33
	bl Func_02003fc4
	cmp r0, #0
	beq .L_0200ba80
	movs r0, #157
	lsls r0, r0, #4
	bl Func_02003fc4
	cmp r0, #0
	bne .L_0200ba80
	bl Func_02001d50
	b .L_0200ba80
.L_0200ba14:
	cmp r3, #21
	bne .L_0200ba22
	movs r0, #48
	adds r0, #255
	bl Func_02003fd4
	b .L_0200ba80
.L_0200ba22:
	movs r1, #2
	movs r0, #17
	bl Object_SetModeById
	movs r0, #17
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #178
	bl Func_02003fc4
	cmp r0, #0
	beq .L_0200ba4c
	ldr r1, .L_0200ba88
	movs r0, #11
	bl ObjectMotion_EnableActionAndSetCallback
.L_0200ba4c:
	movs r0, #157
	lsls r0, r0, #4
	bl Func_02003fc4
	cmp r0, #0
	beq .L_0200ba80
	movs r0, #24
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020040d4
.L_0200ba80:
	pop {pc}
	.2byte 0x0000
.L_0200ba84:
	.4byte gPartyState
.L_0200ba88:
	.4byte Data_02004a1c
	.section .text.x0200ba8c,"ax",%progbits
	.global Func_02003a8c
	.thumb_func
Func_02003a8c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	movs r0, #13
	bl Func_02001c44
	movs r0, #16
	bl Func_02001c44
	movs r0, #17
	bl Func_02001c44
	movs r0, #19
	bl Func_02001c44
	movs r0, #20
	bl Func_02001c44
	movs r0, #14
	bl Object_GetById
	movs r1, #1
	bl Func_02004124
	movs r0, #15
	bl Object_GetById
	movs r1, #2
	bl Func_02004124
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl Func_02004124
	movs r0, #9
	bl Object_GetById
	movs r1, #3
	bl Func_02004124
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #229
	bl Func_02003fc4
	cmp r0, #0
	bne .L_0200bb00
	b .L_0200bcb2
.L_0200bb00:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #229
	bl Func_02003fd4
	ldr r2, .L_0200bcb4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #18
	bne .L_0200bb4e
	ldr r3, .L_0200bcb8
	ldr r2, [r2, #16]
	ldr r3, [r3]
	subs r5, r2, r3
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	cmp r5, #0
	ble .L_0200bbbc
	movs r2, #156
	lsls r2, r2, #7
	adds r2, #31
	cmp r5, r2
	bgt .L_0200bb7a
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #135
	cmp r5, r3
	bgt .L_0200bb8c
	b .L_0200bb94
.L_0200bb4e:
	cmp r3, #19
	bne .L_0200bbdc
	ldr r3, .L_0200bcb8
	ldr r2, [r2, #16]
	ldr r3, [r3]
	subs r5, r2, r3
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	cmp r5, #0
	ble .L_0200bbbc
	movs r1, #156
	lsls r1, r1, #7
	adds r1, #31
	cmp r5, r1
	ble .L_0200bb82
.L_0200bb7a:
	movs r0, #93
	bl Func_020042b4
	b .L_0200bb9a
.L_0200bb82:
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #135
	cmp r5, r2
	ble .L_0200bb94
.L_0200bb8c:
	movs r0, #92
	bl Func_020042b4
	b .L_0200bb9a
.L_0200bb94:
	movs r0, #91
	bl Func_020042b4
.L_0200bb9a:
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, .L_0200bcbc
	bl Func_0200412c
	adds r0, r5, #0
	movs r1, #5
	bl Func_0200403c
	movs r0, #14
	movs r1, #0
	bl Func_02004144
	bl Func_020042ac
	b .L_0200bbd6
.L_0200bbbc:
	cmp r5, #0
	bge .L_0200bbd6
	ldr r0, .L_0200bcc0
	bl Func_0200412c
	negs r0, r5
	movs r1, #5
	bl Func_0200403c
	movs r0, #14
	movs r1, #0
	bl Func_02004144
.L_0200bbd6:
	bl Func_02004074
	b .L_0200bcb2
.L_0200bbdc:
	cmp r3, #20
	bne .L_0200bcb2
	movs r3, #166
	lsls r3, r3, #1
	adds r7, r2, r3
	bl Func_0200406c
	movs r0, #0
	bl Func_02004204
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #0
	ldrsb r3, [r7, r3]
	movs r5, #1
	negs r5, r5
	cmp r3, r5
	bne .L_0200bc12
	movs r0, #1
	bl Func_02001938
	b .L_0200bcae
.L_0200bc12:
	movs r1, #2
	negs r1, r1
	cmp r3, r1
	bne .L_0200bc2a
	ldr r0, .L_0200bcc4
	bl Func_0200412c
	movs r0, #18
	movs r1, #0
	bl Func_02004144
	b .L_0200bcae
.L_0200bc2a:
	ldr r0, .L_0200bcc8
	bl Func_0200412c
	movs r0, #18
	movs r1, #0
	bl Func_02004144
	movs r3, #0
	ldrsb r3, [r7, r3]
	cmp r3, r5
	beq .L_0200bc94
	adds r6, r7, #0
.L_0200bc42:
	cmp r6, r7
	bne .L_0200bc4e
	ldr r0, .L_0200bccc
	bl Func_0200412c
	b .L_0200bc54
.L_0200bc4e:
	ldr r0, .L_0200bcd0
	bl Func_0200412c
.L_0200bc54:
	movs r0, #0
	ldrsb r0, [r6, r0]
	bl Func_02001a60
	movs r1, #2
	adds r5, r0, #0
	bl Func_0200403c
	movs r0, #18
	movs r1, #0
	bl Func_02004144
	adds r0, r5, #0
	movs r1, #3
	bl Func_020041fc
	movs r1, #0
	adds r0, r5, #0
	bl PartyInventory_GiveItem
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	adds r6, #1
	bl Func_0200415c
	movs r3, #0
	ldrsb r3, [r6, r3]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_0200bc42
.L_0200bc94:
	ldr r3, .L_0200bcb4
	movs r1, #166
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #254
	ldr r0, .L_0200bcd4
	strb r2, [r3]
	bl Func_0200412c
	movs r0, #18
	movs r1, #0
	bl Func_02004144
.L_0200bcae:
	bl Func_02004074
.L_0200bcb2:
	pop {r5, r6, r7, pc}
.L_0200bcb4:
	.4byte gPartyState
.L_0200bcb8:
	.4byte gSceneState
.L_0200bcbc:
	.4byte 0x00001387
.L_0200bcc0:
	.4byte 0x00001388
.L_0200bcc4:
	.4byte 0x000013a2
.L_0200bcc8:
	.4byte 0x000013aa
.L_0200bccc:
	.4byte 0x000013ab
.L_0200bcd0:
	.4byte 0x000013ac
.L_0200bcd4:
	.4byte 0x000013ad
	.section .text.x0200bcd8,"ax",%progbits
	.global Func_02003cd8
	.thumb_func
Func_02003cd8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	ldr r2, [r7, #104]
	adds r6, r7, #0
	adds r6, #99
	mov r8, r2
	ldrb r2, [r6]
	movs r3, #1
	ands r3, r2
	sub sp, #24
	cmp r3, #0
	beq .L_0200bd12
	ldrb r0, [r6]
	movs r1, #6
	lsrs r0, r0, #1
	bl Engine_MathModulo
	adds r1, r0, #0
	lsls r1, r1, #24
	lsrs r1, r1, #24
	adds r0, r7, #0
	bl Animation_ApplyChildValues
.L_0200bd12:
	adds r3, r7, #0
	adds r3, #98
	ldrb r5, [r3]
	cmp r5, #0
	bne .L_0200bd50
	ldrb r2, [r6]
	movs r3, #15
	ands r3, r2
	cmp r3, #0
	bne .L_0200bd86
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #86
	bl Func_020042b4
	mov r1, r8
	adds r1, #166
	movs r2, #0
	ldrsh r3, [r1, r2]
	mov r2, r8
	lsls r3, r3, #1
	adds r3, #160
	strh r5, [r2, r3]
	ldr r2, .L_0200bd4c
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	b .L_0200bd86
	.2byte 0x0000
.L_0200bd4c:
	.4byte 0x00000001
.L_0200bd50:
	cmp r5, #1
	bne .L_0200bd86
	mov r3, r8
	adds r3, #160
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #31
	ble .L_0200bd86
	mov r3, r8
	adds r3, #162
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #31
	ble .L_0200bd86
	adds r0, r7, #0
	movs r1, #0
	bl Animation_ApplyChildValues
	mov r3, r8
	adds r3, #164
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_02003fac
	movs r3, #0
	str r3, [r7, #108]
	b .L_0200be40
.L_0200bd86:
	ldrb r3, [r6]
	movs r2, #1
	adds r3, #1
	strb r3, [r6]
	movs r3, #0
	str r3, [sp, #0]
	mov r6, r8
	mov r11, r2
	adds r6, #160
.L_0200bd98:
	movs r3, #0
	ldrsh r0, [r6, r3]
	lsls r0, r0, #10
	bl Math_Sine
	str r0, [sp, #4]
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #0
	blt .L_0200be2c
	cmp r3, #31
	bgt .L_0200be2c
	ldr r3, [r7, #8]
	add r5, sp, #12
	str r3, [r5]
	adds r0, r5, #0
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r7, #12]
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	bl Func_0200421c
	ldr r2, [r5]
	movs r3, #0
	str r2, [sp, #8]
	mov r10, r3
	ldr r5, [r5, #8]
	mov r9, r5
	ldr r5, [sp, #0]
	add r5, r8
.L_0200bddc:
	ldr r2, [sp, #8]
	mov r3, r9
	str r3, [r5, #16]
	str r2, [r5, #12]
	ldr r2, [sp, #4]
	mov r3, r10
	str r2, [r5, #20]
	str r2, [r5, #24]
	cmp r3, #0
	bne .L_0200bdfc
	adds r0, r7, #0
	bl Func_0200425c
	subs r0, #1
	strh r0, [r5, #30]
	b .L_0200be14
.L_0200bdfc:
	adds r0, r7, #0
	bl Func_0200425c
	ldr r3, [r5, #16]
	ldr r2, .L_0200be50
	adds r0, #1
	adds r3, r3, r2
	str r3, [r5, #16]
	ldr r3, [r5, #24]
	strh r0, [r5, #30]
	negs r3, r3
	str r3, [r5, #24]
.L_0200be14:
	adds r0, r5, #0
	bl Func_0200424c
	movs r3, #1
	add r10, r3
	mov r2, r10
	adds r5, #40
	cmp r2, #1
	ble .L_0200bddc
	ldrh r3, [r6]
	adds r3, #1
	strh r3, [r6]
.L_0200be2c:
	ldr r3, [sp, #0]
	movs r2, #1
	negs r2, r2
	adds r3, #80
	add r11, r2
	str r3, [sp, #0]
	mov r3, r11
	adds r6, #2
	cmp r3, #0
	bge .L_0200bd98
.L_0200be40:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200be50:
	.4byte 0xffff0000
	.section .text.x0200be54,"ax",%progbits
	.global Func_02003e54
	.thumb_func
Func_02003e54:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r7, r1, #0
	sub sp, #4
	bl Resource_FindFreeEntry
	movs r1, #164
	adds r1, r1, r7
	mov r8, r1
	ldr r2, .L_0200beb0
	mov r3, r8
	strh r0, [r3]
	movs r1, #128
	lsls r0, r0, #16
	mov r10, r2
	lsls r1, r1, #1
	ldr r2, .L_0200beb4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r3, r7, #0
	movs r2, #186
	movs r5, #0
	adds r3, #166
	lsls r2, r2, #2
	strh r5, [r3]
	adds r2, #255
	subs r3, #6
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	adds r3, #6
	str r6, [r3]
	adds r3, r6, #0
	mov r0, r10
	adds r3, #98
	strb r0, [r3]
	adds r3, #1
	b .L_0200beb8
	.2byte 0x0000
.L_0200beb0:
	.4byte 0x00000000
.L_0200beb4:
	.4byte Data_020042bc
.L_0200beb8:
	strb r0, [r3]
	ldr r3, .L_0200bf48
	mov r0, r8
	str r3, [r6, #108]
	movs r1, #0
	ldrsh r3, [r0, r1]
	ldr r2, .L_0200bf4c
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	str r7, [r6, #104]
	lsrs r3, r3, #5
	mov r11, r3
	mov r9, r5
	mov r10, r5
.L_0200bed6:
	movs r1, #1
	mov r2, r10
	mov r8, r1
	adds r5, r2, r7
.L_0200bede:
	mov r3, r11
	str r3, [sp, #0]
	adds r0, r5, #0
	movs r1, #16
	movs r2, #16
	ldr r3, .L_0200bf50
	bl Func_02004244
	ldrb r3, [r5, #5]
	movs r0, #33
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	movs r3, #15
	ands r3, r2
	strb r3, [r5, #9]
	adds r0, r6, #0
	bl Func_02004254
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r5, #9]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	movs r2, #1
	lsls r0, r0, #2
	negs r2, r2
	orrs r3, r0
	add r8, r2
	strb r3, [r5, #9]
	mov r3, r8
	adds r5, #40
	cmp r3, #0
	bge .L_0200bede
	movs r1, #1
	add r9, r1
	movs r0, #80
	mov r2, r9
	add r10, r0
	cmp r2, #1
	ble .L_0200bed6
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200bf48:
	.4byte Func_02003cd8
.L_0200bf4c:
	.4byte ResourceTableEntries
.L_0200bf50:
	.4byte 0x80004000
	.section .text.x0200bf54,"ax",%progbits
	.global Func_02003f54
	.thumb_func
Func_02003f54:
	adds r0, #98
	movs r3, #1
	strb r3, [r0]
	bx lr
	.section .rodata.x0200c2bc,"a",%progbits
	.global Data_020042bc
Data_020042bc:
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x70000000
	.4byte 0xf7700000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x77770000
	.4byte 0xffff7770
	.4byte 0xfffffff7
	.4byte 0x7777ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00007777
	.4byte 0x0777ffff
	.4byte 0x7fffffff
	.4byte 0xffff7777
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000007
	.4byte 0x0000077f
	.4byte 0xfff70000
	.4byte 0xffff7000
	.4byte 0x7ffff700
	.4byte 0x07ffff70
	.4byte 0x007fff70
	.4byte 0x007ffff7
	.4byte 0x0007fff7
	.4byte 0x0007fff7
	.4byte 0x000077ff
	.4byte 0x00000077
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xff770000
	.4byte 0x77000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00007fff
	.4byte 0x0007ffff
	.4byte 0x007ffff7
	.4byte 0x07ffff70
	.4byte 0x07fff700
	.4byte 0x7ffff700
	.4byte 0x7fff7000
	.4byte 0x7fff7000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x40000000
	.4byte 0xf4400000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x44440000
	.4byte 0xffff4440
	.4byte 0xfffffff4
	.4byte 0x4444ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004444
	.4byte 0x0444ffff
	.4byte 0x4fffffff
	.4byte 0xffff4444
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000044f
	.4byte 0xfff40000
	.4byte 0xffff4000
	.4byte 0x4ffff400
	.4byte 0x04ffff40
	.4byte 0x004fff40
	.4byte 0x004ffff4
	.4byte 0x0004fff4
	.4byte 0x0004fff4
	.4byte 0x000044ff
	.4byte 0x00000044
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xff440000
	.4byte 0x44000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004fff
	.4byte 0x0004ffff
	.4byte 0x004ffff4
	.4byte 0x04ffff40
	.4byte 0x04fff400
	.4byte 0x4ffff400
	.4byte 0x4fff4000
	.4byte 0x4fff4000
	.4byte 0xe9000000
	.4byte 0x000000e9
	.4byte 0xeb000000
	.4byte 0x000000eb
	.4byte 0xedeb0000
	.4byte 0x0000ebed
	.4byte 0xeeedebe9
	.4byte 0xe9ebedee
	.4byte 0xeeedebe9
	.4byte 0xe9ebedee
	.4byte 0xedeb0000
	.4byte 0x0000ebed
	.4byte 0xeb000000
	.4byte 0x000000eb
	.4byte 0xe9000000
	.4byte 0x000000e9
	.4byte 0x0000e900
	.4byte 0x00e90000
	.4byte 0x00ebe9e9
	.4byte 0xe9e9eb00
	.4byte 0xedebeb00
	.4byte 0x00ebebed
	.4byte 0xefed0000
	.4byte 0x0000edef
	.4byte 0xefed0000
	.4byte 0x0000edef
	.4byte 0xedebeb00
	.4byte 0x00ebebed
	.4byte 0x00ebe9e9
	.4byte 0xe9e9eb00
	.4byte 0x0000e900
	.4byte 0x00e90000
	.4byte 0xe9000000
	.4byte 0x000000e9
	.4byte 0xebe90000
	.4byte 0x0000e9eb
	.4byte 0xedebe900
	.4byte 0x00e9ebed
	.4byte 0xeeedebe9
	.4byte 0xe9ebedee
	.4byte 0xeeedebe9
	.4byte 0xe9ebedee
	.4byte 0xedebe900
	.4byte 0x00e9ebed
	.4byte 0xebe90000
	.4byte 0x0000e9eb
	.4byte 0xe9000000
	.4byte 0x000000e9
.L_0200c57c:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
.L_0200c5b8:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
.L_0200c5f4:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02004630
Data_02004630:
	.4byte 0x000000fa
	.4byte 0x000000fb
	.4byte 0x000000fc
	.4byte 0x00000100
	.4byte 0x00000101
	.4byte 0x00000102
	.4byte 0x00000106
	.4byte 0x00000107
	.4byte 0x00000108
	.4byte 0x000000b7
	.4byte 0x000000b6
	.4byte 0x000000b5
	.4byte 0x000000bd
	.4byte 0x000000ba
	.4byte 0x000000bc
	.global Data_0200466c
Data_0200466c:
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000007
	.4byte 0xffff8000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000018
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_0200469c
Data_0200469c:
	.4byte 0x005a0059
	.4byte 0x005c005b
	.4byte 0x005e005d
	.4byte 0xffff005f
	.global Data_020046ac
Data_020046ac:
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0xffff0016
	.global Data_020046d8
Data_020046d8:
	.4byte 0x001b001e
	.4byte 0x0016001f
	.4byte 0x001f0015
	.4byte 0x00120014
	.4byte 0x000d001f
	.4byte 0x001b000b
	.4byte 0x0008000b
	.4byte 0x00080018
	.4byte 0x00150005
	.4byte 0x001f001f
	.4byte 0xffff001f
	.global Data_02004704
Data_02004704:
	.4byte 0x0059005e
	.4byte 0x005b005a
	.4byte 0x005d005c
	.4byte 0x005e005d
	.4byte 0x005a0059
	.4byte 0x005c005b
	.4byte 0x005d005c
	.4byte 0x0059005e
	.4byte 0x005b005a
	.4byte 0x005c005b
	.4byte 0x005e005d
	.4byte 0x005a0059
	.4byte 0x005b005a
	.4byte 0x005d005c
	.4byte 0x0059005e
	.4byte 0x005a0059
	.4byte 0x005c005b
	.4byte 0x005e005d
	.global Data_0200474c
Data_0200474c:
	.4byte .L_0200c57c
	.4byte .L_0200c5b8
	.4byte .L_0200c5f4
.L_0200c758:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x00000004
	.4byte 0x01520000
	.4byte 0x00000000
	.4byte 0x034c0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x013e0000
	.4byte 0x00000000
	.4byte 0x034c0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01400000
	.4byte 0x00000000
	.4byte 0x03740000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x03740000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
.L_0200c7fc:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x00000004
	.4byte 0x013e0000
	.4byte 0x00000000
	.4byte 0x03740000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01520000
	.4byte 0x00000000
	.4byte 0x03740000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01520000
	.4byte 0x00000000
	.4byte 0x034c0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x013e0000
	.4byte 0x00000000
	.4byte 0x034c0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_020048a0
Data_020048a0:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00a40000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_020048f8
Data_020048f8:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00be0000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_0200493c
Data_0200493c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00be0000
	.4byte 0x00000000
	.4byte 0x01860000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02004980
Data_02004980:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00b60000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_020049c4
Data_020049c4:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x017e0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_02004a1c
Data_02004a1c:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffd800
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00000800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001400
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000a00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffe00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000a00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffe00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_02004ac0
Data_02004ac0:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x017a0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02004b18
Data_02004b18:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x01670000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
.L_0200cb54:
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000002e
	.4byte Func_020007a4
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_02004bc0
Data_02004bc0:
	.4byte 0xffff0000
	.4byte 0x0000010f
	.4byte 0x40000328
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004bf0
Data_02004bf0:
	.4byte 0x000000ec
	.4byte 0x00136002
	.4byte 0x002010ed
	.4byte 0x003020ed
	.4byte 0x0040a0ee
	.4byte 0x005070ed
	.4byte 0x006080ee
	.4byte 0x007090ee
	.4byte 0x008050ed
	.4byte 0x009060ed
	.4byte 0x00a030ed
	.4byte 0x00b040ed
	.4byte 0x00c150ef
	.4byte 0x000000ef
	.4byte 0x0150c0ec
	.4byte 0x01603134
	.4byte 0x01701134
	.4byte 0x01804134
	.4byte 0x000000ed
	.4byte 0x001020ec
	.4byte 0x002030ec
	.4byte 0x0030a0ec
	.4byte 0x0040b0ec
	.4byte 0x005080ec
	.4byte 0x006090ec
	.4byte 0x007050ec
	.4byte 0x015030eb
	.4byte 0x000000ee
	.4byte 0x008060ec
	.4byte 0x009070ec
	.4byte 0x00a040ec
	.4byte 0x000001ff
	.global Data_02004c70
Data_02004c70:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004c88
Data_02004c88:
	.4byte 0xffff0073
	.4byte 0x00000003
	.4byte 0x01540000
	.4byte 0x00000000
	.4byte 0x03a80000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x03ae0000
	.4byte 0x0000c000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0xffff006b
	.4byte 0x00000003
	.4byte 0x00740000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x0000c000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00013000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x015c0000
	.4byte 0x00000000
	.4byte 0x03240000
	.4byte 0x0001b000
	.4byte 0xffff0088
	.4byte .L_0200c758
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x0002c000
	.4byte 0xffff008a
	.4byte .L_0200c7fc
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00024000
	.4byte 0x003e00f3
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x006700f5
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x02c20000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004d90
Data_02004d90:
	.4byte 0xffff0073
	.4byte 0x00000003
	.4byte 0x034e0000
	.4byte 0x00000000
	.4byte 0x02940000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0001d000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x02ae0000
	.4byte 0x00000000
	.4byte 0x02f40000
	.4byte 0x00015000
	.4byte 0xffff004f
	.4byte .L_0200cb54
	.4byte 0x03060000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00018000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x024c0000
	.4byte 0x00010000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x03260000
	.4byte 0x00000000
	.4byte 0x02540000
	.4byte 0x00015000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x03400000
	.4byte 0x00000000
	.4byte 0x01360000
	.4byte 0x00005000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0001d000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x015a0000
	.4byte 0x0001d000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x01570000
	.4byte 0x00005000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x02d00000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff01a6
	.4byte 0x00000007
	.4byte 0x02d00000
	.4byte 0x00000000
	.4byte 0x025e0000
	.4byte 0x00024000
	.4byte 0xffff01a6
	.4byte 0x00000007
	.4byte 0x02f00000
	.4byte 0x00000000
	.4byte 0x025e0000
	.4byte 0x00024000
	.4byte 0xffff01a6
	.4byte 0x00000007
	.4byte 0x03100000
	.4byte 0x00000000
	.4byte 0x026e0000
	.4byte 0x00024000
	.4byte 0xffff01a7
	.4byte 0x00000007
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x024e0000
	.4byte 0x00024000
	.4byte 0xffff01a7
	.4byte 0x00000007
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x025e0000
	.4byte 0x00024000
	.4byte 0xffff01a7
	.4byte 0x00000007
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02660000
	.4byte 0x00024000
	.4byte 0xffff01a7
	.4byte 0x00000007
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x026e0000
	.4byte 0x00024000
	.4byte 0xffff01a7
	.4byte 0x00000007
	.4byte 0x033a0000
	.4byte 0x00000000
	.4byte 0x026e0000
	.4byte 0x00024000
	.4byte 0xffff02a8
	.4byte 0x00000001
	.4byte 0x02bc0000
	.4byte 0x00000000
	.4byte 0x025c0000
	.4byte 0x00024000
	.4byte 0xffff02a8
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0xffff02a8
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x029c0000
	.4byte 0x01024000
	.4byte 0xffff02a8
	.4byte 0x00000001
	.4byte 0x02f60000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00024000
	.4byte 0xffff02a8
	.4byte 0x00000001
	.4byte 0x035a0000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x01024000
	.4byte 0xffff02a8
	.4byte 0x00000001
	.4byte 0x030e0000
	.4byte 0x00000000
	.4byte 0x02a60000
	.4byte 0x00024000
	.4byte 0xffff02a8
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x029d0000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005018
Data_02005018:
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00005000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00003000
	.4byte 0xffff0088
	.4byte 0x00000002
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x00820000
	.4byte 0x00004000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x005e0000
	.4byte 0x0000b000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00005000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02dd0000
	.4byte 0x00000000
	.4byte 0x005b0000
	.4byte 0x00003000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02ae0000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x0001d000
	.4byte 0xffff0088
	.4byte 0x00000003
	.4byte 0x02c40000
	.4byte 0x00000000
	.4byte 0x009a0000
	.4byte 0x00004000
	.4byte 0xffff00a1
	.4byte 0x00000001
	.4byte 0x02f40000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x00024000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00003000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00005000
	.4byte 0xffff0086
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00005000
	.4byte 0xffff0087
	.4byte 0x00000003
	.4byte 0x00ba0000
	.4byte 0x00000000
	.4byte 0x027c0000
	.4byte 0x00000000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00013000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02500000
	.4byte 0x0001d000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x005e0000
	.4byte 0x00000000
	.4byte 0x023e0000
	.4byte 0x00015000
	.4byte 0xffff004f
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x0001d000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0xffff0071
	.4byte 0x00000003
	.4byte 0x01360000
	.4byte 0x00000000
	.4byte 0x02660000
	.4byte 0x00008000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x02b00000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00005000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005228
Data_02005228:
	.4byte 0xffff003b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00028000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00028000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00028000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00028000
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x017e0000
	.4byte 0x00020000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x016e0000
	.4byte 0x00020000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x01620000
	.4byte 0x00020000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x018e0000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005318
Data_02005318:
	.4byte 0xffff003b
	.4byte 0x00000001
	.4byte 0x009e0000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00034000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005390
Data_02005390:
	.4byte 0xffff00ec
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00003000
	.4byte 0xffff00ec
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00005000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x005c0000
	.4byte 0x00000000
	.4byte 0x03860000
	.4byte 0x0001d000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x0001b000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x005a0000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x0001d000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x038a0000
	.4byte 0x00024000
	.4byte 0xffff00ec
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00003000
	.4byte 0xffff00ec
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00005000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x038a0000
	.4byte 0x00024000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x01540000
	.4byte 0x00000000
	.4byte 0x038a0000
	.4byte 0x00024000
	.4byte 0xffff00e4
	.4byte 0x00000001
	.4byte Data_02000000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00003000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x01fc0000
	.4byte 0x00000000
	.4byte 0x038a0000
	.4byte 0x00024000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x02240000
	.4byte 0x00000000
	.4byte 0x038a0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020054e0
Data_020054e0:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020054ec
Data_020054ec:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000ce01
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000ce01
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x0000ce01
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x0000ce01
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x0000ce01
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x0000ce01
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x0000ce01
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x0000ce01
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000ce01
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x0000ce01
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00009c05
	.4byte 0xffff001e
	.4byte Func_02001250
	.4byte 0x50008905
	.4byte 0xffff0027
	.4byte Func_02001284
	.4byte 0x50008905
	.4byte 0xffff0028
	.4byte Func_020012a4
	.4byte 0x50008905
	.4byte 0xffff0029
	.4byte Func_020012c4
	.4byte 0x50008905
	.4byte 0xffff002a
	.4byte Func_02001354
	.4byte 0x50008905
	.4byte 0xffff002b
	.4byte Func_02001374
	.4byte 0x50008905
	.4byte 0xffff002c
	.4byte Func_02001394
	.4byte 0x50008905
	.4byte 0xffff002d
	.4byte Func_020013b4
	.4byte 0x50008905
	.4byte 0xffff002e
	.4byte Func_020013d4
	.4byte 0x50008905
	.4byte 0xffff002f
	.4byte Func_020013f4
	.4byte 0x50008905
	.4byte 0xffff0030
	.4byte Func_02001414
	.4byte 0x50008905
	.4byte 0xffff0031
	.4byte Func_02001434
	.4byte 0x50008905
	.4byte 0xffff0032
	.4byte Func_02001454
	.4byte 0x50008905
	.4byte 0xffff0033
	.4byte Func_02001474
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte Func_020014b4
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte Func_02001494
	.4byte 0x00008e15
	.4byte 0x09e70011
	.4byte Func_020015ac
	.4byte 0x00000000
	.4byte 0x19e70011
	.4byte Func_02000608
	.4byte 0x00000000
	.4byte 0x0a210008
	.4byte Func_020009ac
	.4byte 0x00000000
	.4byte 0x09b20008
	.4byte 0x00002b55
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002be4
	.4byte 0x00008d15
	.4byte 0x0a210008
	.4byte 0x000028dd
	.4byte 0x00008d15
	.4byte 0x09b20008
	.4byte 0x00002b59
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002bea
	.4byte 0x00000000
	.4byte 0x0a210009
	.4byte 0x000028d6
	.4byte 0x00000000
	.4byte 0x09b20009
	.4byte 0x00002b56
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002be5
	.4byte 0x00008d15
	.4byte 0x0a210009
	.4byte 0x000028de
	.4byte 0x00008d15
	.4byte 0x09b20009
	.4byte 0x00002b5a
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002beb
	.4byte 0x00000000
	.4byte 0x0a21000a
	.4byte 0x000028d7
	.4byte 0x00000000
	.4byte 0x09b2000a
	.4byte 0x00002b57
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002be6
	.4byte 0x00008d15
	.4byte 0x0a21000a
	.4byte 0x000028df
	.4byte 0x00008d15
	.4byte 0x09b2000a
	.4byte 0x00002b5b
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002bec
	.4byte 0x00000000
	.4byte 0x0a21000b
	.4byte 0x000028d8
	.4byte 0x00000000
	.4byte 0x09b2000b
	.4byte 0x00002b58
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002be7
	.4byte 0x00008d15
	.4byte 0x0a21000b
	.4byte 0x000028e0
	.4byte 0x00008d15
	.4byte 0x09b2000b
	.4byte 0x00002b5c
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002bed
	.4byte 0x00000000
	.4byte 0x0a21000c
	.4byte 0x000028d9
	.4byte 0x00000000
	.4byte 0x09b2000c
	.4byte 0x00002c59
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002c5d
	.4byte 0x00008d15
	.4byte 0x0a21000c
	.4byte 0x000028e1
	.4byte 0x00008d15
	.4byte 0x09b2000c
	.4byte 0x00002c5b
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002c5f
	.4byte 0x00000000
	.4byte 0x09b2000d
	.4byte 0x000028da
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002be8
	.4byte 0x00008d15
	.4byte 0x09b2000d
	.4byte 0x000028e2
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002bee
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_02001094
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte Func_02001094
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_0200118c
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte Func_0200118c
	.4byte 0x50008805
	.4byte 0xffff0020
	.4byte Func_020014d4
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte Func_020005f8
	.4byte 0x00000003
	.4byte 0xffff0018
	.4byte Func_0200092c
	.4byte 0x00000003
	.4byte 0xffff0019
	.4byte Func_0200094c
	.4byte 0x00000003
	.4byte 0xffff001a
	.4byte Func_0200096c
	.4byte 0x0000c403
	.4byte 0xffff0013
	.4byte Func_0200098c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005870
Data_02005870:
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x00000015
	.4byte 0x0000c602
	.4byte 0xffff000d
	.4byte Func_02001d08
	.4byte 0x0000ce01
	.4byte 0xffff000e
	.4byte 0x00000017
	.4byte 0x0000c602
	.4byte 0xffff000f
	.4byte Func_02001d38
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000290e
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002914
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000290f
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002915
	.4byte 0x00000000
	.4byte 0x09b2000a
	.4byte 0x00002910
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002c47
	.4byte 0x00008d15
	.4byte 0x09b2000a
	.4byte 0x00002916
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002c48
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002911
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002917
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002912
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002918
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002913
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002919
	.4byte 0x00000000
	.4byte 0x09b2000e
	.4byte Func_02001070
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002c49
	.4byte 0x00008d15
	.4byte 0x09b2000e
	.4byte 0x00002920
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002c4d
	.4byte 0x00000000
	.4byte 0x09b2000f
	.4byte 0x0000291d
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002c4a
	.4byte 0x00008d15
	.4byte 0x09b2000f
	.4byte 0x00002921
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002c4e
	.4byte 0x00000000
	.4byte 0x09b20010
	.4byte 0x0000291e
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002c4b
	.4byte 0x00008d15
	.4byte 0x09b20010
	.4byte 0x00002922
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002c4f
	.4byte 0x00000000
	.4byte 0x09b20011
	.4byte 0x0000291f
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002c4c
	.4byte 0x00008d15
	.4byte 0x09b20011
	.4byte 0x00002923
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002c50
	.4byte 0x00000003
	.4byte 0xffff0016
	.4byte Func_020008ec
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_0200090c
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_0200152c
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_0200156c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005a44
Data_02005a44:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000000
	.4byte 0x09b20008
	.4byte 0x000028e5
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002bf0
	.4byte 0x00008d15
	.4byte 0x09b20008
	.4byte 0x000028e8
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002bf3
	.4byte 0x00000000
	.4byte 0x09b20009
	.4byte 0x000028e6
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002bf1
	.4byte 0x00008d15
	.4byte 0x09b20009
	.4byte 0x000028e9
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002bf4
	.4byte 0x00000000
	.4byte 0x09b2000a
	.4byte 0x000028e7
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002bf2
	.4byte 0x00008d15
	.4byte 0x09b2000a
	.4byte 0x000028ea
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002bf5
	.4byte 0x00000000
	.4byte 0x09b2000b
	.4byte 0x000028eb
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002bf6
	.4byte 0x00008d15
	.4byte 0x09b2000b
	.4byte 0x000028ed
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002bf8
	.4byte 0x00000000
	.4byte 0x09b2000c
	.4byte 0x000028ec
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002bf7
	.4byte 0x00008d15
	.4byte 0x09b2000c
	.4byte 0x000028ee
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002bf9
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_02000cf4
	.4byte 0x00008d15
	.4byte 0x09b2000d
	.4byte 0x000028ff
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002c3c
	.4byte 0x00000000
	.4byte 0x09b2000e
	.4byte 0x000028ef
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002bfa
	.4byte 0x00008d15
	.4byte 0x09b2000e
	.4byte 0x000028f6
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002c01
	.4byte 0x00000000
	.4byte 0x09b2000f
	.4byte 0x000028f0
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002bfb
	.4byte 0x00008d15
	.4byte 0x09b2000f
	.4byte 0x000028f7
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002c02
	.4byte 0x00000000
	.4byte 0x09b20010
	.4byte 0x000028f1
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002bfc
	.4byte 0x00008d15
	.4byte 0x09b20010
	.4byte 0x000028f8
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002c03
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte Func_02000af4
	.4byte 0x00008d15
	.4byte 0x09b20011
	.4byte 0x000028f9
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002c04
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_02000d44
	.4byte 0x00008d15
	.4byte 0x09b20012
	.4byte 0x000028fb
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002c38
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_02000d94
	.4byte 0x00008d15
	.4byte 0x09b20013
	.4byte 0x000028fd
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002c3a
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte Func_02000de4
	.4byte 0x00008d15
	.4byte 0x0a210014
	.4byte 0x00002929
	.4byte 0x00008d15
	.4byte 0x09b20014
	.4byte 0x00002bb2
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002c41
	.4byte 0x00000000
	.4byte 0x0a210015
	.4byte 0x00002901
	.4byte 0x00000000
	.4byte 0x09b20015
	.4byte 0x00002baf
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00002c3e
	.4byte 0x00008d15
	.4byte 0x0a210015
	.4byte 0x0000292a
	.4byte 0x00008d15
	.4byte 0x09b20015
	.4byte 0x00002bb3
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002c42
	.4byte 0x00000000
	.4byte 0x0a210016
	.4byte 0x00002902
	.4byte 0x00000000
	.4byte 0x09b20016
	.4byte 0x00002bb0
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00002c3f
	.4byte 0x00008d15
	.4byte 0x0a210016
	.4byte 0x0000292b
	.4byte 0x00008d15
	.4byte 0x09b20016
	.4byte 0x00002bb4
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002c43
	.4byte 0x00000000
	.4byte 0x0a210017
	.4byte 0x00002903
	.4byte 0x00000000
	.4byte 0x09b20017
	.4byte 0x00002bb1
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00002c40
	.4byte 0x00008d15
	.4byte 0x0a210017
	.4byte 0x0000292c
	.4byte 0x00008d15
	.4byte 0x09b20017
	.4byte 0x00002bb5
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002c44
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00002904
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00002906
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00002905
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00002907
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00002908
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x0000290a
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x00002909
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x0000290b
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte Func_02000e50
	.4byte 0x00008d15
	.4byte 0x0a21001c
	.4byte 0x0000290d
	.4byte 0x00008d15
	.4byte 0x09b2001c
	.4byte 0x00002bb7
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x00002c46
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00403058
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x00403059
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x0040305a
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005e70
Data_02005e70:
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002c35
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002c36
	.4byte 0x00000002
	.4byte 0x09d10014
	.4byte Func_02002e18
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00403058
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x00403059
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x0040305a
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005ed0
Data_02005ed0:
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02001b84
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000013dd
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000013ce
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000013de
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_02000edc
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_02000eb8
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000013df
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000013db
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000013e0
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000013dc
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000013e1
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_02001abc
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000139a
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001385
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000139b
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte Func_02000f40
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte Func_02000f7c
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_02000fc8
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000013ca
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_02000fe0
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte Func_0200102c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
