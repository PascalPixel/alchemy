.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_020046cc
	.section .text.x02008040,"ax",%progbits
	.global Func_02000040
	.thumb_func
Func_02000040:
	movs r0, #0
	bx lr
	.section .text.x02008044,"ax",%progbits
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, .L_02008048
	bx lr
.L_02008048:
	.4byte Data_020046fc
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	ldr r3, .L_020080a8
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020080ac
	cmp r2, r3
	bne .L_02008064
	ldr r0, .L_020080b0
	b .L_020080a4
.L_02008064:
	ldr r3, .L_020080b4
	cmp r2, r3
	bne .L_0200806e
	ldr r0, .L_020080b8
	b .L_020080a4
.L_0200806e:
	ldr r3, .L_020080bc
	cmp r2, r3
	bne .L_02008078
	ldr r0, .L_020080c0
	b .L_020080a4
.L_02008078:
	ldr r3, .L_020080c4
	cmp r2, r3
	bne .L_02008082
	ldr r0, .L_020080c8
	b .L_020080a4
.L_02008082:
	ldr r3, .L_020080cc
	cmp r2, r3
	bne .L_020080a2
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #165
	bl Func_02003dcc
	cmp r0, #0
	beq .L_0200809e
	ldr r3, .L_020080d0
	movs r2, #1
	adds r3, #46
	strb r2, [r3]
.L_0200809e:
	ldr r0, .L_020080d0
	b .L_020080a4
.L_020080a2:
	ldr r0, .L_020080d4
.L_020080a4:
	pop {pc}
	.2byte 0x0000
.L_020080a8:
	.4byte gPartyState
.L_020080ac:
	.4byte 0x00000083
.L_020080b0:
	.4byte Data_0200476c
.L_020080b4:
	.4byte 0x00000084
.L_020080b8:
	.4byte Data_020047e4
.L_020080bc:
	.4byte 0x00000085
.L_020080c0:
	.4byte Data_0200494c
.L_020080c4:
	.4byte 0x00000086
.L_020080c8:
	.4byte Data_020049c4
.L_020080cc:
	.4byte 0x00000087
.L_020080d0:
	.4byte Data_02004a84
.L_020080d4:
	.4byte Data_02004754
	.section .text.x020080d8,"ax",%progbits
	.global Func_020000d8
	.thumb_func
Func_020000d8:
	push {r5, r6, lr}
	movs r0, #10
	sub sp, #32
	bl Object_GetById
	add r5, sp, #8
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_020037d8
	cmp r0, #0
	beq .L_02008192
	mov r2, sp
	add r3, sp, #24
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r2, [r5, #8]
	ldr r1, [r5, #4]
	ldr r3, [r5, #12]
	ldr r0, [r5]
	bl Func_02003a5c
	movs r0, #5
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r6, #72]
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r2, #8
	movs r1, #0
	movs r0, #10
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #22
	bl Battle_WaitMode0
	movs r0, #240
	bl Func_02003ffc
	movs r0, #10
	movs r1, #8
	bl Object_SetModeById
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
	ldr r3, .L_02008198
	str r1, [r6, #40]
	str r3, [r6, #12]
	str r3, [r6, #20]
	movs r3, #10
	movs r5, #9
	str r3, [sp, #4]
	movs r0, #9
	movs r1, #1
	movs r2, #4
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02003e24
	movs r0, #9
	movs r1, #8
	movs r2, #4
	movs r3, #1
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003e24
	movs r3, #74
	str r3, [sp, #4]
	movs r0, #9
	movs r1, #64
	movs r2, #4
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02003e24
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #44
	bl Func_02003dd4
.L_02008192:
	add sp, #32
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008198:
	.4byte 0xfff80000
	.section .text.x0200819c,"ax",%progbits
	.global Func_0200019c
	.thumb_func
Func_0200019c:
	push {lr}
	ldr r3, .L_020081e0
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020081e4
	cmp r2, r3
	bne .L_020081b4
	ldr r0, .L_020081e8
	b .L_020081de
.L_020081b4:
	ldr r3, .L_020081ec
	cmp r2, r3
	bne .L_020081be
	ldr r0, .L_020081f0
	b .L_020081de
.L_020081be:
	ldr r3, .L_020081f4
	cmp r2, r3
	bne .L_020081c8
	ldr r0, .L_020081f8
	b .L_020081de
.L_020081c8:
	ldr r3, .L_020081fc
	cmp r2, r3
	bne .L_020081d2
	ldr r0, .L_02008200
	b .L_020081de
.L_020081d2:
	ldr r3, .L_02008204
	cmp r2, r3
	bne .L_020081dc
	ldr r0, .L_02008208
	b .L_020081de
.L_020081dc:
	ldr r0, .L_0200820c
.L_020081de:
	pop {pc}
.L_020081e0:
	.4byte gPartyState
.L_020081e4:
	.4byte 0x00000083
.L_020081e8:
	.4byte Data_02004b20
.L_020081ec:
	.4byte 0x00000084
.L_020081f0:
	.4byte Data_02004bb0
.L_020081f4:
	.4byte 0x00000085
.L_020081f8:
	.4byte Data_02004ca0
.L_020081fc:
	.4byte 0x00000086
.L_02008200:
	.4byte Data_02004d84
.L_02008204:
	.4byte 0x00000087
.L_02008208:
	.4byte Data_02004e5c
.L_0200820c:
	.4byte Data_02004b14
	.section .text.x02008210,"ax",%progbits
	.global Func_02000210
	.thumb_func
Func_02000210:
	push {lr}
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #162
	bl Func_02003dcc
	cmp r0, #0
	beq .L_02008260
	movs r0, #128
	lsls r0, r0, #2
	bl Func_02003dcc
	cmp r0, #0
	beq .L_02008250
	ldr r3, .L_0200829c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r0, #9
	ldr r1, [r3]
	movs r2, #0
	bl Func_02003f14
	ldr r0, .L_020082a0
	bl Func_02003f24
	b .L_02008256
.L_02008250:
	ldr r0, .L_020082a4
	bl Func_02003f24
.L_02008256:
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	b .L_02008294
.L_02008260:
	movs r0, #9
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_0200829c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #9
	bl Func_02003f14
	ldr r0, .L_020082a8
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r0, #9
	movs r1, #19
	bl Object_LinkObjectAndSetCallback
.L_02008294:
	bl Func_02003e6c
	pop {pc}
	.2byte 0x0000
.L_0200829c:
	.4byte gPartyState
.L_020082a0:
	.4byte 0x000021bb
.L_020082a4:
	.4byte 0x000021b7
.L_020082a8:
	.4byte 0x000021b1
	.section .text.x020082ac,"ax",%progbits
	.global Func_020002ac
	.thumb_func
Func_020002ac:
	push {lr}
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #162
	bl Func_02003dcc
	cmp r0, #0
	beq .L_020082ea
	movs r0, #128
	lsls r0, r0, #2
	bl Func_02003dcc
	cmp r0, #0
	beq .L_020082da
	ldr r0, .L_02008300
	bl Func_02003f24
	b .L_020082e0
.L_020082da:
	ldr r0, .L_02008304
	bl Func_02003f24
.L_020082e0:
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	b .L_020082f8
.L_020082ea:
	ldr r0, .L_02008308
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
.L_020082f8:
	bl Func_02003e6c
	pop {pc}
	.2byte 0x0000
.L_02008300:
	.4byte 0x000021bd
.L_02008304:
	.4byte 0x000021b9
.L_02008308:
	.4byte 0x000021b3
	.section .text.x0200830c,"ax",%progbits
	.global Func_0200030c
	.thumb_func
Func_0200030c:
	push {lr}
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #162
	bl Func_02003dcc
	cmp r0, #0
	beq .L_0200835c
	movs r0, #128
	lsls r0, r0, #2
	bl Func_02003dcc
	cmp r0, #0
	beq .L_0200834c
	ldr r3, .L_02008370
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r0, #19
	ldr r1, [r3]
	movs r2, #0
	bl Func_02003f14
	ldr r0, .L_02008374
	bl Func_02003f24
	b .L_02008352
.L_0200834c:
	ldr r0, .L_02008378
	bl Func_02003f24
.L_02008352:
	movs r0, #19
	movs r1, #0
	bl Func_02003f2c
	b .L_0200836a
.L_0200835c:
	ldr r0, .L_0200837c
	bl Func_02003f24
	movs r0, #19
	movs r1, #0
	bl Func_02003f2c
.L_0200836a:
	bl Func_02003e6c
	pop {pc}
.L_02008370:
	.4byte gPartyState
.L_02008374:
	.4byte 0x000021bc
.L_02008378:
	.4byte 0x000021b7
.L_0200837c:
	.4byte 0x000021b2
	.section .text.x02008380,"ax",%progbits
	.global Func_02000380
	.thumb_func
Func_02000380:
	push {lr}
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #162
	bl Func_02003dcc
	cmp r0, #0
	beq .L_020083be
	movs r0, #128
	lsls r0, r0, #2
	bl Func_02003dcc
	cmp r0, #0
	beq .L_020083ae
	ldr r0, .L_020083d0
	bl Func_02003f24
	b .L_020083b4
.L_020083ae:
	ldr r0, .L_020083d4
	bl Func_02003f24
.L_020083b4:
	movs r0, #19
	movs r1, #0
	bl Func_02003f2c
	b .L_020083cc
.L_020083be:
	ldr r0, .L_020083d8
	bl Func_02003f24
	movs r0, #19
	movs r1, #0
	bl Func_02003f2c
.L_020083cc:
	pop {pc}
	.2byte 0x0000
.L_020083d0:
	.4byte 0x000021be
.L_020083d4:
	.4byte 0x000021ba
.L_020083d8:
	.4byte 0x000021b4
	.section .text.x020083dc,"ax",%progbits
	.global Func_020003dc
	.thumb_func
Func_020003dc:
	push {lr}
	ldr r3, .L_02008404
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
	ldr r2, .L_02008408
	lsls r3, r3, #16
	movs r0, #1
	cmp r3, r2
	bls .L_02008402
	movs r0, #0
.L_02008402:
	pop {pc}
.L_02008404:
	.4byte gPartyState
.L_02008408:
	.4byte 0x3ffe0000
	.section .text.x0200840c,"ax",%progbits
	.global Func_0200040c
	.thumb_func
Func_0200040c:
	push {lr}
	bl Func_020003dc
	cmp r0, #0
	beq .L_0200841e
	movs r0, #21
	bl Func_02003ff4
	b .L_0200843a
.L_0200841e:
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	ldr r0, .L_0200843c
	bl Func_02003f24
	movs r0, #21
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003e6c
.L_0200843a:
	pop {pc}
.L_0200843c:
	.4byte 0x000021f7
	.section .text.x02008440,"ax",%progbits
	.global Func_02000440
	.thumb_func
Func_02000440:
	push {lr}
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #133
	lsls r0, r0, #2
	bl Func_02003dcc
	cmp r0, #0
	bne .L_020084b6
	ldr r0, .L_020084e8
	bl Func_02003f24
	movs r0, #8
	movs r1, #0
	bl Func_02003f2c
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	bl Func_02003f5c
	movs r1, #128
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02003f34
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r0, #8
	movs r1, #0
	bl Func_02003f3c
	movs r2, #10
	movs r0, #9
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	bl Func_02003f5c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #133
	lsls r0, r0, #2
	bl Func_02003dd4
.L_020084b6:
	ldr r3, .L_020084ec
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #8
	bl Func_02003f14
	ldr r0, .L_020084f0
	bl Func_02003f24
	movs r0, #8
	movs r1, #0
	bl Func_02003f2c
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02003f34
	bl Func_02003e6c
	pop {pc}
.L_020084e8:
	.4byte 0x000021a4
.L_020084ec:
	.4byte gPartyState
.L_020084f0:
	.4byte 0x000021a7
	.section .text.x020084f4,"ax",%progbits
	.global Func_020004f4
	.thumb_func
Func_020004f4:
	push {lr}
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	ldr r0, .L_0200851c
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003e6c
	pop {pc}
.L_0200851c:
	.4byte 0x000021a9
	.section .text.x02008520,"ax",%progbits
	.global Func_02000520
	.thumb_func
Func_02000520:
	push {lr}
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #164
	bl Func_02003dcc
	cmp r0, #0
	beq .L_0200854a
	ldr r3, .L_02008560
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r0, #10
	bl Object_LinkObjectAndSetCallback
.L_0200854a:
	ldr r0, .L_02008564
	bl Func_02003f24
	movs r0, #10
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003e6c
	pop {pc}
	.2byte 0x0000
.L_02008560:
	.4byte gPartyState
.L_02008564:
	.4byte 0x000021bf
	.section .text.x02008568,"ax",%progbits
	.global Func_02000568
	.thumb_func
Func_02000568:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #226
	lsls r0, r0, #1
	bl PartyInventory_FindOwner
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_020085be
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #164
	bl Func_02003dcc
	cmp r0, #0
	beq .L_020085ae
	ldr r3, .L_020086e4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r0, #10
	ldr r1, [r3]
	bl Object_LinkObjectAndSetCallback
	ldr r0, .L_020086e8
	bl Func_02003f24
	b .L_020085b4
.L_020085ae:
	ldr r0, .L_020086ec
	bl Func_02003f24
.L_020085b4:
	movs r0, #10
	movs r1, #0
	bl Func_02003f2c
	b .L_020086da
.L_020085be:
	ldr r3, .L_020086e4
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	ldr r0, [r7]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #10
	bl Object_GetById
	ldr r1, [r7]
	adds r5, r0, #0
	movs r2, #0
	movs r0, #10
	bl Func_02003f14
	movs r2, #10
	movs r1, #4
	movs r0, #10
	bl ObjectMotion_Launch
	ldr r3, .L_020086f0
	mov r8, r3
	mov r0, r8
	bl Func_02003f24
	movs r0, #10
	movs r1, #0
	bl Func_02003f2c
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	ldr r3, [r5, #8]
	ldr r1, [r6, #8]
	ldr r2, [r5, #16]
	adds r1, r1, r3
	lsrs r3, r1, #31
	adds r1, r1, r3
	ldr r3, [r6, #16]
	asrs r1, r1, #1
	adds r3, r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	ldr r2, [r6, #12]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #20
	bl WaitFrames
	movs r1, #240
	movs r2, #128
	movs r3, #156
	lsls r2, r2, #16
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #15
	bl Object_SetPositionAndResetMotion
	movs r1, #5
	movs r0, #10
	bl Object_SetModeById
	movs r0, #10
	bl WaitFrames
	movs r1, #128
	ldr r0, [r7]
	lsls r1, r1, #7
	bl Func_02003f3c
	movs r0, #234
	movs r2, #152
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	adds r0, #255
	lsls r2, r2, #16
	bl Func_02003df4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200868c
	ldr r0, [r7]
	movs r1, #28
	bl Object_SetModeById
	movs r1, #198
	adds r0, r5, #0
	adds r1, #255
	bl Func_02003e3c
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_0200868c:
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02003fc4
	mov r0, r8
	movs r1, #1
	adds r0, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #226
	lsls r0, r0, #1
	bl PartyInventory_Remove
	movs r0, #198
	movs r1, #0
	adds r0, #255
	bl PartyInventory_GiveItem
	bl Func_02003fbc
	cmp r5, #0
	beq .L_020086c8
	adds r0, r5, #0
	bl Func_02003dfc
	ldr r0, [r7]
	movs r1, #1
	bl Object_SetModeById
.L_020086c8:
	movs r0, #10
	movs r1, #0
	bl Func_02003f3c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #164
	bl Func_02003dd4
.L_020086da:
	bl Func_02003e6c
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_020086e4:
	.4byte gPartyState
.L_020086e8:
	.4byte 0x000021c4
.L_020086ec:
	.4byte 0x000021c0
.L_020086f0:
	.4byte 0x000021c1
	.section .text.x020086f4,"ax",%progbits
	.global Func_020006f4
	.thumb_func
Func_020006f4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #198
	adds r0, #255
	bl PartyInventory_FindOwner
	movs r5, #1
	negs r5, r5
	cmp r0, r5
	bne .L_0200871a
	b .L_02008d5c
.L_0200871a:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02003dcc
	adds r7, r0, #0
	cmp r7, #0
	bne .L_0200872c
	b .L_02008978
.L_0200872c:
	adds r2, r5, #0
	movs r3, #0
	adds r0, r5, #0
	adds r1, r5, #0
	bl Func_02003f6c
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_02003f64
	movs r0, #140
	movs r2, #190
	lsls r0, r0, #16
	adds r1, r5, #0
	lsls r2, r2, #16
	movs r3, #1
	bl Func_02003f6c
	ldr r3, .L_0200896c
	movs r0, #133
	lsls r0, r0, #2
	adds r7, r3, r0
	ldr r1, [r7]
	movs r0, #10
	movs r2, #0
	bl Func_02003f14
	ldr r2, .L_02008970
	mov r10, r2
	mov r0, r10
	bl Func_02003f24
	movs r0, #10
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003f74
	bl Func_02003fc4
	ldr r0, [r7]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	mov r9, r0
	cmp r0, #0
	beq .L_02008794
	b .L_02008944
.L_02008794:
	ldr r0, [r7]
	bl Object_GetById
	ldr r1, [r7]
	mov r8, r0
	movs r0, #10
	bl Object_LinkObjectAndSetCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r7]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #168
	ldr r0, [r7]
	movs r1, #98
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r7]
	lsls r1, r1, #8
	bl Func_02003f3c
	ldr r0, [r7]
	movs r1, #4
	movs r2, #10
	bl ObjectMotion_Launch
	movs r0, #234
	movs r1, #208
	movs r2, #192
	movs r3, #160
	adds r0, #255
	lsls r1, r1, #15
	lsls r2, r2, #13
	lsls r3, r3, #16
	bl Func_02003df4
	adds r5, r0, #0
	movs r6, #0
	cmp r5, #0
	beq .L_0200880e
	movs r1, #198
	adds r1, #255
	bl Func_02003e3c
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r3, r5, #0
	adds r3, #85
	adds r2, r5, #0
	strb r6, [r3]
	adds r2, #92
	movs r3, #1
	strb r3, [r2]
.L_0200880e:
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02003fbc
	movs r0, #10
	bl ObjectMotion_EnableActionAndResetMotion
	ldr r0, [r7]
	movs r1, #0
	movs r2, #0
	bl Func_02003f34
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02003f54
	mov r0, r10
	adds r0, #1
	bl Func_02003f24
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #10
	movs r1, #0
	bl Func_02003f2c
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #164
	movs r0, #10
	movs r1, #128
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #10
	bl Func_02003f3c
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #204
	lsls r2, r2, #8
	ldr r1, .L_02008974
	adds r2, #204
	movs r0, #10
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	mov r10, r3
	ands r3, r2
	strb r3, [r0]
	movs r1, #108
	movs r2, #168
	movs r0, #10
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	mov r0, r8
	ldr r2, [r0, #12]
	movs r3, #192
	lsls r3, r3, #13
	ldr r1, [r0, #8]
	adds r2, r2, r3
	ldr r3, [r0, #16]
	movs r0, #234
	adds r0, #255
	bl Func_02003df4
	adds r6, r0, #0
	cmp r6, #0
	beq .L_020088f0
	ldr r0, [r7]
	movs r1, #28
	bl Object_SetModeById
	adds r3, r6, #0
	adds r3, #85
	mov r0, r9
	movs r1, #227
	strb r0, [r3]
	lsls r1, r1, #1
	adds r0, r6, #0
	bl Func_02003e3c
	movs r0, #20
	bl Battle_WaitMode0
.L_020088f0:
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r10
	ands r3, r2
	movs r1, #116
	strb r3, [r0]
	movs r2, #164
	movs r0, #10
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	bl Func_02003fc4
	movs r0, #198
	adds r0, #255
	bl PartyInventory_Remove
	movs r0, #227
	lsls r0, r0, #1
	movs r1, #0
	bl PartyInventory_GiveItem
	cmp r6, #0
	bne .L_0200893a
	b .L_02008cf4
.L_0200893a:
	adds r0, r6, #0
	bl Func_02003dfc
	ldr r0, [r7]
	b .L_02008cee
.L_02008944:
	bl Func_02003fbc
	movs r1, #10
	adds r1, #255
	movs r2, #20
	movs r0, #10
	bl Func_02003f54
	mov r0, r10
	adds r0, #4
	bl Func_02003f24
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #10
	movs r1, #0
	bl Func_02003f2c
	b .L_02008d80
	.2byte 0x0000
.L_0200896c:
	.4byte gPartyState
.L_02008970:
	.4byte 0x000021ca
.L_02008974:
	.4byte 0x00019999
.L_02008978:
	ldr r2, .L_02008c90
	mov r9, r2
	mov r0, r9
	bl Func_02003f24
	movs r0, #10
	movs r1, #0
	bl Func_02003f2c
	adds r2, r5, #0
	adds r0, r5, #0
	adds r1, r5, #0
	movs r3, #0
	bl Func_02003f6c
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_02003f64
	movs r0, #140
	movs r2, #190
	adds r1, r5, #0
	movs r3, #1
	lsls r0, r0, #16
	lsls r2, r2, #16
	bl Func_02003f6c
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02003f34
	ldr r3, .L_02008c94
	movs r0, #133
	lsls r0, r0, #2
	movs r1, #204
	movs r2, #204
	adds r6, r3, r0
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r6]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #190
	ldr r0, [r6]
	movs r1, #120
	bl ObjectMotion_SetPositionAndReset
	movs r1, #0
	ldr r0, [r6]
	bl Func_02003f3c
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	mov r10, r3
	ands r3, r2
	strb r3, [r0]
	movs r1, #132
	movs r2, #190
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	mov r8, r2
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r1, #140
	movs r0, #234
	movs r3, #190
	adds r0, #255
	lsls r1, r1, #16
	movs r2, #0
	lsls r3, r3, #16
	bl Func_02003df4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008a5a
	movs r1, #198
	adds r1, #255
	bl Func_02003e3c
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r3, r5, #0
	adds r3, #85
	adds r2, r5, #0
	strb r7, [r3]
	adds r2, #92
	movs r3, #1
	strb r3, [r2]
.L_02008a5a:
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r10
	ands r3, r2
	strb r3, [r0]
	movs r1, #120
	movs r2, #190
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	movs r1, #192
	strb r3, [r0]
	lsls r1, r1, #6
	movs r0, #10
	movs r2, #0
	bl Func_02003f34
	movs r2, #10
	movs r0, #10
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #10
	movs r1, #0
	bl Func_02003f2c
	ldr r1, [r6]
	movs r0, #10
	bl Object_LinkObjectAndSetCallback
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r10
	ands r3, r2
	strb r3, [r0]
	movs r1, #132
	movs r2, #190
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r1, #195
	movs r3, #230
	lsls r1, r1, #16
	movs r2, #0
	lsls r3, r3, #15
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r10
	ands r3, r2
	strb r3, [r0]
	movs r1, #120
	movs r2, #190
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, [r6]
	movs r1, #98
	movs r2, #168
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r6]
	movs r1, #4
	movs r2, #10
	bl ObjectMotion_Launch
	movs r1, #208
	movs r2, #192
	movs r3, #160
	lsls r2, r2, #13
	lsls r3, r3, #16
	lsls r1, r1, #15
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02003fc4
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #7
	bl Func_02003f3c
	mov r0, r9
	adds r0, #2
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	bl Func_02003fbc
	movs r2, #10
	movs r1, #6
	movs r0, #10
	bl ObjectMotion_Launch
	mov r0, r9
	adds r0, #3
	bl Func_02003f24
	movs r1, #0
	movs r0, #10
	bl Func_02003f2c
	movs r0, #10
	bl ObjectMotion_EnableActionAndResetMotion
	ldr r0, [r6]
	movs r1, #0
	bl Func_02003f3c
	bl Func_02003fc4
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	beq .L_02008bba
	b .L_02008d04
.L_02008bba:
	ldr r0, [r6]
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02003fbc
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02003f54
	mov r0, r9
	adds r0, #4
	bl Func_02003f24
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #10
	movs r1, #0
	bl Func_02003f2c
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #164
	movs r0, #10
	movs r1, #128
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #10
	bl Func_02003f3c
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #204
	lsls r2, r2, #8
	ldr r1, .L_02008c98
	adds r2, #204
	movs r0, #10
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r10
	ands r3, r2
	strb r3, [r0]
	movs r1, #108
	movs r2, #168
	movs r0, #10
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r3, #192
	ldr r2, [r5, #12]
	lsls r3, r3, #13
	movs r0, #234
	ldr r1, [r5, #8]
	adds r2, r2, r3
	adds r0, #255
	ldr r3, [r5, #16]
	bl Func_02003df4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008c9c
	ldr r0, [r6]
	movs r1, #28
	bl Object_SetModeById
	ldr r3, .L_02008c8c
	adds r2, r5, #0
	adds r2, #85
	movs r1, #227
	adds r0, r5, #0
	strb r3, [r2]
	lsls r1, r1, #1
	bl Func_02003e3c
	movs r0, #20
	bl Battle_WaitMode0
	b .L_02008c9c
.L_02008c8c:
	.4byte 0x00000000
.L_02008c90:
	.4byte 0x000021c7
.L_02008c94:
	.4byte gPartyState
.L_02008c98:
	.4byte 0x00019999
.L_02008c9c:
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r10
	ands r3, r2
	movs r1, #116
	strb r3, [r0]
	movs r2, #164
	movs r0, #10
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	bl Func_02003fc4
	movs r0, #198
	adds r0, #255
	bl PartyInventory_Remove
	movs r0, #227
	lsls r0, r0, #1
	movs r1, #0
	bl PartyInventory_GiveItem
	cmp r5, #0
	beq .L_02008cf4
	adds r0, r5, #0
	bl Func_02003dfc
	ldr r0, [r6]
.L_02008cee:
	movs r1, #1
	bl Object_SetModeById
.L_02008cf4:
	bl Func_02003fbc
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #163
	bl Func_02003dd4
	b .L_02008d80
.L_02008d04:
	bl Func_02003fbc
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #10
	bl Func_02003f54
	mov r0, r9
	adds r0, #7
	bl Func_02003f24
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #10
	movs r1, #0
	bl Func_02003f2c
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Func_02003f3c
	ldr r0, [r6]
	movs r1, #4
	movs r2, #10
	bl ObjectMotion_Launch
	cmp r5, #0
	beq .L_02008d46
	adds r0, r5, #0
	bl Func_02003dfc
.L_02008d46:
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #7
	bl Func_02003f3c
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02003dd4
	b .L_02008d80
.L_02008d5c:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #163
	bl Func_02003dcc
	cmp r0, #0
	beq .L_02008d72
	ldr r0, .L_02008d98
	bl Func_02003f24
	b .L_02008d78
.L_02008d72:
	ldr r0, .L_02008d9c
	bl Func_02003f24
.L_02008d78:
	movs r0, #10
	movs r1, #0
	bl Func_02003f2c
.L_02008d80:
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	bl Func_02003e6c
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008d98:
	.4byte 0x000021cd
.L_02008d9c:
	.4byte 0x000021c6
	.section .text.x02008da0,"ax",%progbits
	.global Func_02000da0
	.thumb_func
Func_02000da0:
	push {r5, r6, lr}
	ldr r2, [r0, #12]
	movs r3, #192
	lsls r3, r3, #13
	adds r2, r2, r3
	adds r6, r1, #0
	ldr r3, [r0, #16]
	ldr r1, [r0, #8]
	movs r0, #234
	adds r0, #255
	bl Func_02003df4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008dee
	ldr r3, .L_02008df4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #28
	bl Object_SetModeById
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r0, r5, #0
	adds r1, r6, #0
	bl Func_02003e3c
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r2, r5, #0
	adds r2, #92
	movs r3, #1
	strb r3, [r2]
.L_02008dee:
	adds r0, r5, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008df4:
	.4byte gPartyState
	.section .text.x02008df8,"ax",%progbits
	.global Func_02000df8
	.thumb_func
Func_02000df8:
	push {r5, lr}
	ldr r3, .L_02008e30
	movs r2, #3
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02008e2e
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	movs r0, #14
	bl Func_02003df4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008e2e
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #1
	bl Func_02003de4
	ldr r1, .L_02008e34
	adds r0, r5, #0
	bl Func_02003dec
.L_02008e2e:
	pop {r5, pc}
.L_02008e30:
	.4byte Data_0300122c
.L_02008e34:
	.4byte Data_020044f0
	.section .text.x02008e38,"ax",%progbits
	.global Func_02000e38
	.thumb_func
Func_02000e38:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #227
	lsls r0, r0, #1
	bl PartyInventory_FindOwner
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	bne .L_02008e64
	b .L_020091f2
.L_02008e64:
	ldr r3, .L_02009230
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #11
	bl Object_GetById
	movs r3, #0
	str r3, [sp, #0]
	mov r10, r0
	ldr r1, [r5]
	movs r0, #11
	movs r2, #0
	mov r9, r3
	bl Func_02003f14
	ldr r7, .L_02009234
	mov r1, r10
	str r7, [r1, #108]
	movs r0, #11
	movs r1, #4
	movs r2, #10
	bl ObjectMotion_Launch
	mov r2, r9
	mov r3, r10
	str r2, [r3, #108]
	ldr r0, .L_02009238
	bl Func_02003f24
	movs r1, #0
	movs r0, #11
	bl Func_02003f2c
	mov r1, r10
	ldr r2, [r6, #8]
	ldr r3, [r1, #8]
	cmp r2, r3
	ble .L_02008ebe
	movs r2, #1
	str r2, [sp, #0]
.L_02008ebe:
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_0200923c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #11
	ldr r1, .L_0200923c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r3, [sp, #0]
	cmp r3, #0
	beq .L_02008f16
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02003f34
	movs r1, #148
	movs r2, #214
	lsls r2, r2, #2
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Func_02003f3c
	movs r1, #227
	lsls r1, r1, #1
	adds r0, r6, #0
	bl Func_02000da0
	mov r1, r10
	str r7, [r1, #108]
	mov r8, r0
	ldr r1, .L_02009240
	b .L_02008f4a
.L_02008f16:
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02003f34
	movs r1, #132
	movs r2, #214
	lsls r2, r2, #2
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #0
	bl Func_02003f3c
	movs r1, #227
	lsls r1, r1, #1
	adds r0, r6, #0
	bl Func_02000da0
	mov r1, r10
	str r7, [r1, #108]
	mov r8, r0
	ldr r1, .L_02009244
.L_02008f4a:
	movs r0, #11
	bl Object_SetActionCallbackAndRefreshById
	mov r2, r9
	mov r3, r10
	str r2, [r3, #108]
	movs r0, #11
	movs r1, #0
	bl Func_02003f2c
	ldr r3, .L_02009230
	movs r1, #133
	lsls r1, r1, #2
	adds r7, r3, r1
	ldr r1, [r7]
	movs r0, #11
	bl Object_LinkObjectAndSetCallback
	mov r2, r8
	cmp r2, #0
	bne .L_02008f76
	b .L_02009142
.L_02008f76:
	adds r2, #89
	movs r3, #9
	strb r3, [r2]
	movs r1, #208
	movs r3, #210
	lsls r1, r1, #15
	lsls r3, r3, #18
	mov r0, r8
	movs r2, #0
	bl Object_SetPositionAndResetMotion
	ldr r3, .L_02009234
	ldr r1, [sp, #0]
	mov r11, r3
	cmp r1, #0
	beq .L_0200905a
	movs r1, #148
	movs r2, #210
	str r3, [r6, #108]
	ldr r0, [r7]
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r2, #0
	movs r1, #128
	str r2, [r6, #108]
	lsls r1, r1, #8
	ldr r0, [r7]
	bl Func_02003f3c
	mov r3, r11
	str r3, [r6, #108]
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #144
	movs r2, #210
	lsls r1, r1, #1
	lsls r2, r2, #2
	ldr r0, [r7]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #1
	mov r9, r1
	mov r2, r9
	orrs r3, r2
	strb r3, [r0]
	movs r1, #140
	movs r3, #210
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #18
	mov r0, r8
	bl Object_SetPositionAndResetMotion
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #148
	movs r2, #210
	lsls r1, r1, #1
	lsls r2, r2, #2
	ldr r0, [r7]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r1, r9
	orrs r3, r1
	strb r3, [r0]
	movs r2, #0
	str r2, [r6, #108]
	ldr r0, [r7]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	mov r3, r11
	mov r1, r10
	str r3, [r1, #108]
	movs r0, #11
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #146
	b .L_0200911c
.L_0200905a:
	mov r3, r11
	movs r1, #132
	movs r2, #210
	str r3, [r6, #108]
	lsls r2, r2, #2
	ldr r0, [r7]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	ldr r1, [sp, #0]
	ldr r0, [r7]
	str r1, [r6, #108]
	movs r1, #0
	bl Func_02003f3c
	mov r2, r11
	str r2, [r6, #108]
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #136
	movs r2, #210
	lsls r1, r1, #1
	lsls r2, r2, #2
	ldr r0, [r7]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #1
	mov r9, r1
	mov r2, r9
	orrs r3, r2
	strb r3, [r0]
	movs r1, #140
	movs r3, #210
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #18
	mov r0, r8
	bl Object_SetPositionAndResetMotion
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #132
	movs r2, #210
	lsls r1, r1, #1
	lsls r2, r2, #2
	ldr r0, [r7]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r1, r9
	orrs r3, r1
	strb r3, [r0]
	ldr r2, [sp, #0]
	ldr r0, [r7]
	str r2, [r6, #108]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	mov r3, r11
	mov r1, r10
	str r3, [r1, #108]
	movs r0, #11
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #134
.L_0200911c:
	ands r5, r3
	movs r2, #211
	lsls r2, r2, #2
	lsls r1, r1, #1
	strb r5, [r0]
	movs r0, #11
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #11
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r9
	orrs r3, r2
	strb r3, [r0]
.L_02009142:
	movs r7, #200
	adds r7, #255
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_02000da0
	mov r9, r0
	movs r0, #11
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #140
	movs r2, #214
	strb r3, [r0]
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #11
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #11
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r5, #0
	mov r3, r10
	str r5, [r3, #108]
	bl Func_02003fc4
	movs r1, #1
	ldr r0, .L_02009248
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #227
	lsls r0, r0, #1
	bl PartyInventory_Remove
	movs r1, #0
	adds r0, r7, #0
	bl PartyInventory_GiveItem
	bl Func_02003fbc
	ldr r3, .L_02009230
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r3, r1
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #7
	mov r2, r9
	strh r3, [r0, #6]
	cmp r2, #0
	beq .L_020091d2
	mov r0, r9
	bl Func_02003dfc
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
.L_020091d2:
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #8
	bl Func_02003f3c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #161
	bl Func_02003dd4
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #22
	bl Func_02003dd4
	b .L_02009262
.L_020091f2:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #161
	bl Func_02003dcc
	cmp r0, #0
	beq .L_02009254
	ldr r3, .L_02009230
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r0, #11
	ldr r1, [r3]
	bl Object_LinkObjectAndSetCallback
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #22
	bl Func_02003dcc
	cmp r0, #0
	beq .L_02009226
	ldr r0, .L_0200924c
	bl Func_02003f24
	b .L_0200925a
.L_02009226:
	ldr r0, .L_02009250
	bl Func_02003f24
	b .L_0200925a
	.2byte 0x0000
.L_02009230:
	.4byte gPartyState
.L_02009234:
	.4byte Func_02000df8
.L_02009238:
	.4byte 0x000021d2
.L_0200923c:
	.4byte 0x00019999
.L_02009240:
	.4byte Data_020043f8
.L_02009244:
	.4byte Data_02004300
.L_02009248:
	.4byte 0x000021d4
.L_0200924c:
	.4byte 0x000021d5
.L_02009250:
	.4byte 0x000021d6
.L_02009254:
	ldr r0, .L_02009274
	bl Func_02003f24
.L_0200925a:
	movs r0, #11
	movs r1, #0
	bl Func_02003f2c
.L_02009262:
	bl Func_02003e6c
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009274:
	.4byte 0x000021d1
	.section .text.x02009278,"ax",%progbits
	.global Func_02001278
	.thumb_func
Func_02001278:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r0, #9
	bl Object_GetById
	adds r5, r0, #0
	ldr r2, [r5, #12]
	movs r3, #128
	lsls r3, r3, #13
	adds r2, r2, r3
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	adds r0, r6, #0
	bl Object_SetPositionAndResetMotion
	ldrh r3, [r5, #6]
	strh r3, [r6, #6]
	pop {r5, r6, pc}
	.section .text.x0200929c,"ax",%progbits
	.global Func_0200129c
	.thumb_func
Func_0200129c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, .L_020094c4
	adds r5, r0, #0
	movs r0, #133
	lsls r0, r0, #2
	adds r6, r6, r0
	ldr r0, [r6]
	sub sp, #36
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #9
	bl Object_GetById
	ldr r3, .L_020094c8
	mov r8, sp
	mov r2, r8
	mov r11, r0
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Func_02003f6c
	bl Func_02003fbc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #9
	bl Func_02003f54
	ldr r1, .L_020094cc
	mov r2, r11
	str r1, [r2, #108]
	movs r0, #9
	mov r1, r8
	bl Object_SetActionCallbackAndRefreshById
	mov r4, r11
	movs r3, #0
	str r3, [r4, #108]
	ldr r0, .L_020094d0
	mov r8, r3
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02003f3c
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_020094d4
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r0, .L_020094cc
	mov r1, r11
	str r0, [r1, #108]
	movs r2, #226
	movs r1, #148
	movs r0, #9
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	mov r2, r8
	mov r3, r11
	movs r1, #160
	str r2, [r3, #108]
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02003f34
	ldr r1, .L_020094cc
	movs r0, #204
	ldr r4, .L_020094d4
	lsls r0, r0, #8
	adds r0, #204
	mov r10, r0
	str r0, [r5, #52]
	str r1, [r5, #108]
	adds r0, r5, #0
	movs r1, #2
	mov r9, r4
	str r4, [r5, #48]
	bl Func_02003de4
	movs r1, #139
	movs r3, #226
	lsls r3, r3, #17
	movs r2, #0
	adds r0, r5, #0
	lsls r1, r1, #17
	bl Func_02003e0c
	adds r0, r5, #0
	bl Func_02003e14
	adds r0, r5, #0
	movs r1, #1
	bl Func_02003de4
	movs r0, #200
	mov r2, r8
	adds r0, #255
	str r2, [r5, #108]
	bl PartyInventory_Remove
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #166
	lsls r1, r1, #2
	movs r0, #88
	bl Runtime_AllocateBlock
	movs r3, #203
	lsls r3, r3, #1
	adds r3, #255
	adds r0, r0, r3
	movs r3, #99
	strb r3, [r0]
	movs r1, #3
	ldr r0, [r6]
	bl Motion_SetModeAndWaitAnimation
	ldr r0, [r6]
	mov r1, r9
	mov r2, r10
	bl ObjectMotion_SetSpeedParameters
	ldr r4, .L_020094cc
	movs r1, #156
	movs r2, #223
	str r4, [r7, #108]
	ldr r0, [r6]
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	mov r0, r8
	str r0, [r7, #108]
	movs r1, #6
	ldr r0, [r6]
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #148
	movs r2, #226
	lsls r2, r2, #1
	ldr r0, [r6]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	ldr r3, .L_020094d8
	ldr r0, [r6]
	str r3, [r7, #108]
	bl Object_GetById
	movs r1, #15
	bl Func_02003f1c
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r2, #204
	lsls r2, r2, #7
	mov r1, r10
	movs r0, #9
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r3, #204
	ldr r2, .L_020094cc
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r5, #52]
	mov r3, r11
	str r2, [r3, #108]
	mov r1, r10
	movs r2, #128
	str r1, [r5, #48]
	lsls r2, r2, #2
	movs r1, #144
	adds r2, #26
	lsls r1, r1, #1
	movs r0, #9
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_02003fc4
	bl Func_020034ec
	ldr r4, .L_020094cc
	adds r0, r5, #0
	movs r1, #2
	str r4, [r5, #108]
	bl Func_02003de4
	movs r1, #144
	lsls r1, r1, #17
	adds r0, r5, #0
	movs r2, #0
	ldr r3, .L_020094dc
	bl Func_02003e0c
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	movs r2, #129
	lsls r0, r0, #1
	lsls r2, r2, #1
	adds r3, r3, r0
	adds r2, #255
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #3
	bl Func_02003f7c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #165
	bl Func_02003dd4
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020094c4:
	.4byte gPartyState
.L_020094c8:
	.4byte Data_02004004
.L_020094cc:
	.4byte Func_02000df8
.L_020094d0:
	.4byte 0x000021de
.L_020094d4:
	.4byte 0x00019999
.L_020094d8:
	.4byte Func_02001278
.L_020094dc:
	.4byte 0x021a0000
	.section .text.x020094e0,"ax",%progbits
	.global Func_020014e0
	.thumb_func
Func_020014e0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #200
	adds r0, #255
	bl PartyInventory_FindOwner
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_0200950a
	b .L_020098ac
.L_0200950a:
	ldr r3, .L_02009894
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	bl Object_GetById
	mov r8, r0
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02003dcc
	mov r10, r0
	cmp r0, #0
	bne .L_0200952c
	b .L_02009694
.L_0200952c:
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r6]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	ldr r7, .L_02009898
	mov r3, r8
	movs r1, #159
	movs r2, #212
	str r7, [r3, #108]
	ldr r0, [r6]
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r3, #0
	mov r2, r8
	movs r1, #128
	str r3, [r2, #108]
	ldr r0, [r6]
	movs r2, #0
	lsls r1, r1, #7
	bl Func_02003f34
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #9
	bl Func_02003f3c
	ldr r5, .L_0200989c
	adds r0, r5, #0
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003fc4
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	mov r10, r0
	cmp r0, #0
	bne .L_02009688
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Func_02003f3c
	movs r2, #204
	lsls r2, r2, #8
	ldr r1, .L_020098a0
	adds r2, #204
	ldr r0, [r6]
	bl ObjectMotion_SetSpeedParameters
	mov r3, r8
	str r7, [r3, #108]
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	mov r11, r3
	ands r3, r2
	strb r3, [r0]
	movs r1, #151
	movs r2, #212
	lsls r1, r1, #1
	lsls r2, r2, #1
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	mov r9, r2
	mov r2, r9
	orrs r3, r2
	strb r3, [r0]
	movs r1, #148
	movs r3, #212
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #17
	movs r0, #163
	bl Func_02003df4
	movs r1, #148
	movs r3, #212
	adds r7, r0, #0
	lsls r1, r1, #17
	movs r0, #14
	movs r2, #0
	lsls r3, r3, #17
	bl Func_02003df4
	adds r5, r0, #0
	cmp r7, #0
	beq .L_0200961e
	adds r3, r7, #0
	adds r3, #85
	mov r2, r10
	strb r2, [r3]
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
.L_0200961e:
	cmp r5, #0
	beq .L_02009640
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #28]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #1
	bl Func_02003de4
	ldr r1, .L_020098a4
	adds r0, r5, #0
	bl Func_02003dec
.L_02009640:
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r11
	ands r3, r2
	strb r3, [r0]
	movs r1, #159
	movs r2, #212
	lsls r1, r1, #1
	lsls r2, r2, #1
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r9
	orrs r3, r2
	strb r3, [r0]
	mov r2, r8
	mov r3, r10
	movs r1, #192
	str r3, [r2, #108]
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02003f34
	b .L_0200982c
.L_02009688:
	bl Func_02003fbc
	subs r0, r5, #1
	bl Func_02003f24
	b .L_02009a1c
.L_02009694:
	ldr r0, .L_020098a8
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #20
	ldr r0, [r6]
	bl Func_02003f54
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r6]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	ldr r3, .L_02009898
	mov r2, r8
	str r3, [r2, #108]
	movs r1, #159
	movs r2, #212
	lsls r2, r2, #1
	ldr r0, [r6]
	lsls r1, r1, #1
	mov r9, r3
	movs r7, #0
	bl ObjectMotion_SetPositionAndReset
	mov r3, r8
	movs r1, #128
	str r7, [r3, #108]
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Func_02003f3c
	movs r2, #204
	lsls r2, r2, #8
	ldr r1, .L_020098a0
	ldr r0, [r6]
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	mov r2, r9
	mov r3, r8
	str r2, [r3, #108]
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #151
	movs r2, #212
	lsls r1, r1, #1
	lsls r2, r2, #1
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	mov r11, r2
	mov r2, r11
	orrs r3, r2
	strb r3, [r0]
	movs r1, #148
	movs r3, #212
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #17
	movs r0, #163
	bl Func_02003df4
	movs r1, #148
	movs r3, #212
	adds r7, r0, #0
	lsls r1, r1, #17
	movs r0, #14
	movs r2, #0
	lsls r3, r3, #17
	bl Func_02003df4
	adds r5, r0, #0
	cmp r7, #0
	beq .L_0200976a
	adds r3, r7, #0
	adds r3, #85
	mov r2, r10
	strb r2, [r3]
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
.L_0200976a:
	cmp r5, #0
	beq .L_0200978c
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #28]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #1
	bl Func_02003de4
	ldr r1, .L_020098a4
	adds r0, r5, #0
	bl Func_02003dec
.L_0200978c:
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #159
	movs r2, #212
	lsls r1, r1, #1
	lsls r2, r2, #1
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r11
	orrs r3, r2
	strb r3, [r0]
	mov r2, r8
	mov r3, r10
	movs r1, #128
	str r3, [r2, #108]
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02003f34
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02003f54
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #9
	bl Func_02003f3c
	movs r0, #9
	bl Object_GetById
	mov r3, r9
	adds r5, r0, #0
	str r3, [r5, #108]
	movs r0, #9
	movs r1, #4
	movs r2, #20
	bl ObjectMotion_Launch
	mov r2, r10
	str r2, [r5, #108]
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02003f3c
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003fc4
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009834
.L_0200982c:
	adds r0, r7, #0
	bl Func_0200129c
	b .L_02009a3e
.L_02009834:
	bl Func_02003fbc
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #9
	bl Func_02003f54
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #160
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02003f34
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Func_02003f3c
	mov r3, r9
	mov r2, r8
	str r3, [r2, #108]
	movs r1, #151
	movs r2, #212
	lsls r2, r2, #1
	ldr r0, [r6]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	mov r3, r10
	mov r2, r8
	str r3, [r2, #108]
	cmp r7, #0
	beq .L_02009886
	adds r0, r7, #0
	bl Func_02003dfc
.L_02009886:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02003dd4
	b .L_02009a3e
	.2byte 0x0000
.L_02009894:
	.4byte gPartyState
.L_02009898:
	.4byte Func_02000df8
.L_0200989c:
	.4byte 0x000021dd
.L_020098a0:
	.4byte 0x00019999
.L_020098a4:
	.4byte Data_020044f0
.L_020098a8:
	.4byte 0x000021d9
.L_020098ac:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #165
	bl Func_02003dcc
	cmp r0, #0
	bne .L_020098bc
	b .L_02009a30
.L_020098bc:
	ldr r3, .L_02009a50
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	ldr r1, [r7]
	movs r2, #0
	movs r0, #9
	bl Func_02003f14
	ldr r0, .L_02009a54
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003fc4
	ldr r0, [r7]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	beq .L_020098ee
	b .L_02009a18
.L_020098ee:
	ldr r0, [r7]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02003fbc
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r5, #226
	lsls r5, r5, #1
	adds r2, r2, r5
	mov r8, r3
	ldrh r3, [r2]
	movs r0, #9
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_02003f2c
	movs r1, #166
	lsls r1, r1, #2
	movs r0, #88
	bl Runtime_AllocateBlock
	movs r2, #203
	lsls r2, r2, #1
	adds r2, #255
	movs r3, #99
	adds r0, r0, r2
	strb r3, [r0]
	movs r1, #3
	ldr r0, [r7]
	bl Motion_SetModeAndWaitAnimation
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02003f3c
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r7]
	ldr r1, .L_02009a58
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r7]
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #148
	adds r2, r5, #0
	lsls r1, r1, #1
	ldr r0, [r7]
	bl ObjectMotion_SetPositionAndCommit
	ldr r3, .L_02009a5c
	ldr r0, [r7]
	str r3, [r6, #108]
	bl Object_GetById
	movs r1, #15
	bl Func_02003f1c
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003fc4
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
	adds r1, #204
	adds r2, #102
	movs r0, #10
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	bl Object_GetById
	ldr r5, .L_02009a60
	movs r2, #128
	movs r1, #144
	lsls r2, r2, #2
	lsls r1, r1, #1
	adds r2, #26
	str r5, [r0, #108]
	movs r0, #9
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #40
	bl Battle_WaitMode0
	bl Func_02003fc4
	bl Func_020034ec
	movs r0, #10
	bl Object_GetById
	movs r2, #128
	movs r1, #144
	lsls r2, r2, #2
	lsls r1, r1, #1
	adds r2, #26
	str r5, [r0, #108]
	movs r0, #10
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #20
	bl Battle_WaitMode0
	mov r2, r8
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02003fbc
	movs r0, #3
	bl Func_02003f7c
	b .L_02009a3e
.L_02009a18:
	bl Func_02003fbc
.L_02009a1c:
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02003f3c
	b .L_02009a3e
.L_02009a30:
	ldr r0, .L_02009a64
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
.L_02009a3e:
	bl Func_02003e6c
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009a50:
	.4byte gPartyState
.L_02009a54:
	.4byte 0x000021eb
.L_02009a58:
	.4byte 0x00019999
.L_02009a5c:
	.4byte Func_02001278
.L_02009a60:
	.4byte Func_02000df8
.L_02009a64:
	.4byte 0x000021d8
	.section .text.x02009a68,"ax",%progbits
	.global Func_02001a68
	.thumb_func
Func_02001a68:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009bd8
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	mov r10, r0
	movs r0, #9
	bl Object_GetById
	mov r8, r0
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r2, #0
	ldr r1, [r5]
	movs r0, #9
	bl Func_02003f14
	movs r0, #10
	bl Battle_WaitMode0
	ldr r6, .L_02009bdc
	adds r0, r6, #0
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003fc4
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	adds r7, r0, #0
	cmp r7, #0
	bne .L_02009bae
	bl Func_02003fbc
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #166
	lsls r1, r1, #2
	movs r0, #88
	bl Runtime_AllocateBlock
	movs r3, #203
	lsls r3, r3, #1
	adds r3, #255
	adds r0, r0, r3
	movs r3, #99
	strb r3, [r0]
	movs r1, #6
	ldr r0, [r5]
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_02009be0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #192
	ldr r0, [r5]
	movs r1, #152
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndCommit
	ldr r3, .L_02009be4
	mov r2, r10
	str r3, [r2, #108]
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #15
	bl Func_02003f1c
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_02003fc4
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_02009be0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #10
	ldr r1, .L_02009be0
	bl ObjectMotion_SetSpeedParameters
	ldr r5, .L_02009be8
	mov r6, r8
	mov r3, r8
	adds r6, #99
	ldr r1, .L_02009bec
	strb r7, [r6]
	movs r0, #9
	str r5, [r3, #108]
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	ldr r1, .L_02009bf0
	str r5, [r0, #108]
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
.L_02009b74:
	movs r0, #1
	bl WaitFrames
	ldrb r3, [r6]
	cmp r3, #0
	beq .L_02009b74
	bl Func_020034ec
	movs r0, #1
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02003fbc
	movs r0, #4
	bl Func_02003f7c
	b .L_02009bca
.L_02009bae:
	bl Func_02003fbc
	adds r0, r6, #4
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02003f3c
.L_02009bca:
	bl Func_02003e6c
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009bd8:
	.4byte gPartyState
.L_02009bdc:
	.4byte 0x000021f2
.L_02009be0:
	.4byte 0x00019999
.L_02009be4:
	.4byte Func_02001278
.L_02009be8:
	.4byte Func_02000df8
.L_02009bec:
	.4byte Data_020045a4
.L_02009bf0:
	.4byte Data_020045f0
	.section .text.x02009bf4,"ax",%progbits
	.global Func_02001bf4
	.thumb_func
Func_02001bf4:
	push {lr}
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	ldr r3, .L_02009c30
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #9
	bl Func_02003f14
	ldr r0, .L_02009c34
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02003f3c
	bl Func_02003e6c
	pop {pc}
.L_02009c30:
	.4byte gPartyState
.L_02009c34:
	.4byte 0x0000218e
	.section .text.x02009c38,"ax",%progbits
	.global Func_02001c38
	.thumb_func
Func_02001c38:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #173
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
	bx lr
	.2byte 0x0000
	.section .text.x02009c4c,"ax",%progbits
	.global Func_02001c4c
	.thumb_func
Func_02001c4c:
	push {r5, lr}
	ldr r3, .L_02009ca8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r2, #160
	lsls r2, r2, #14
	cmp r3, r2
	bge .L_02009c84
	movs r0, #11
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #8
	orrs r3, r5
	strb r3, [r0]
	movs r0, #12
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	orrs r5, r3
	b .L_02009ca2
.L_02009c84:
	movs r0, #11
	bl Object_GetById
	adds r0, #89
	ldrb r2, [r0]
	movs r5, #247
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #12
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	ands r5, r3
.L_02009ca2:
	strb r5, [r0]
	pop {r5, pc}
	.2byte 0x0000
.L_02009ca8:
	.4byte gPartyState
	.section .text.x02009cac,"ax",%progbits
	.global Func_02001cac
	.thumb_func
Func_02001cac:
	ldr r3, .L_02009cf0
	ldrb r3, [r3]
	lsls r0, r3, #2
	adds r0, r0, r3
	ldr r3, .L_02009cf4
	lsls r0, r0, #6
	adds r0, r0, r3
	movs r2, #0
	ldrsh r3, [r0, r2]
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #20
	strh r3, [r1]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #176
	ldrh r4, [r3, #10]
	movs r2, #197
	lsls r2, r2, #8
	adds r2, #255
	ands r2, r4
	strh r2, [r3, #10]
	movs r2, #254
	ldrh r4, [r3, #10]
	lsls r2, r2, #7
	adds r2, #255
	ands r2, r4
	strh r2, [r3, #10]
	adds r0, #2
	ldrh r2, [r3, #10]
	ldr r2, .L_02009cf8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bx lr
.L_02009cf0:
	.4byte gOverlayArea + 0x4f04
.L_02009cf4:
	.4byte gOverlayArea + 0x4f10
.L_02009cf8:
	.4byte 0xa2600001
	.section .text.x02009cfc,"ax",%progbits
	.global Func_02001cfc
	.thumb_func
Func_02001cfc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #188
	lsls r1, r1, #1
	adds r1, r1, r3
	ldr r3, .L_02009d84
	sub sp, #8
	ldrb r2, [r3]
	movs r3, #1
	eors r3, r2
	lsls r2, r3, #2
	adds r2, r2, r3
	ldr r3, .L_02009d88
	lsls r2, r2, #6
	adds r2, r2, r3
	add r7, sp, #4
	movs r3, #0
	str r3, [r7]
	mov r10, r1
	mov r8, r2
.L_02009d2e:
	ldr r3, .L_02009d8c
	ldr r0, [r7]
	ldrb r3, [r3]
	mov r1, r10
	adds r0, r0, r3
	movs r2, #6
	ldrsh r3, [r1, r2]
	adds r0, r0, r3
	lsls r0, r0, #9
	bl Math_Sine
	str r0, [sp, #0]
	mov r3, r10
	movs r2, #2
	ldrsh r5, [r3, r2]
	ldr r6, [r7]
	asrs r0, r0, #15
	adds r5, r5, r0
	movs r1, #3
	adds r0, r6, #0
	bl Engine_MathRemainder
	adds r5, r5, r0
	mov r1, r8
	subs r5, #1
	movs r2, #2
	adds r6, #1
	strh r5, [r1]
	add r8, r2
	str r6, [r7]
	cmp r6, #160
	bne .L_02009d2e
	ldr r3, .L_02009d84
	movs r1, #1
	ldrb r2, [r3]
	add sp, #8
	eors r2, r1
	strb r2, [r3]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009d84:
	.4byte gOverlayArea + 0x4f04
.L_02009d88:
	.4byte gOverlayArea + 0x4f10
.L_02009d8c:
	.4byte Data_0300122c
	.section .text.x02009d90,"ax",%progbits
	.global Func_02001d90
	.thumb_func
Func_02001d90:
	push {lr}
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02009da8
	bl Func_02003da4
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_02009dac
	bl Func_02003da4
	pop {pc}
.L_02009da8:
	.4byte Func_02001cfc
.L_02009dac:
	.4byte Func_02001cac
	.section .text.x02009db0,"ax",%progbits
	.global Func_02001db0
	.thumb_func
Func_02001db0:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	movs r0, #19
	bl Object_GetById
	ldr r2, .L_02009f4c
	ldr r3, [r0, #8]
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	cmp r3, r2
	bls .L_02009dce
	b .L_02009f44
.L_02009dce:
	ldr r0, [r0, #16]
	movs r3, #164
	lsls r3, r3, #16
	cmp r0, r3
	bge .L_02009dda
	b .L_02009f44
.L_02009dda:
	movs r2, #172
	lsls r2, r2, #16
	cmp r0, r2
	ble .L_02009de4
	b .L_02009f44
.L_02009de4:
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #19
	movs r1, #1
	bl Object_SetModeById
	ldr r3, .L_02009f50
	movs r2, #133
	mov r8, r3
	lsls r2, r2, #2
	add r8, r2
	mov r3, r8
	ldr r0, [r3]
	movs r1, #216
	movs r2, #184
	bl ObjectMotion_SetPositionAndReset
	mov r2, r8
	ldr r0, [r2]
	movs r1, #9
	bl Func_02003fac
	ldr r0, .L_02009f54
	bl Func_02003f24
	movs r0, #9
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #4
	movs r2, #10
	bl ObjectMotion_Launch
	mov r3, r8
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl Func_02003f34
	movs r1, #128
	movs r2, #10
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02003f34
	movs r1, #0
	movs r0, #9
	bl Func_02003f2c
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r2, #0
	strb r3, [r0]
	movs r1, #222
	mov r10, r2
	movs r0, #9
	movs r2, #186
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #186
	ands r5, r3
	movs r1, #232
	strb r5, [r0]
	movs r0, #9
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #128
	orrs r6, r3
	strb r6, [r0]
	mov r3, r8
	ldr r0, [r3]
	lsls r1, r1, #7
	bl Func_02003f3c
	mov r2, r8
	ldr r0, [r2]
	movs r1, #28
	bl Object_SetModeById
	mov r3, r8
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #20
	bl Object_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #85
	mov r2, r10
	movs r1, #226
	strb r2, [r3]
	lsls r1, r1, #1
	bl Func_02003e3c
	ldr r2, [r6, #12]
	movs r3, #192
	lsls r3, r3, #13
	adds r2, r2, r3
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_02003fc4
	movs r0, #226
	movs r1, #0
	lsls r0, r0, #1
	bl PartyInventory_GiveItem
	bl Func_02003fbc
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02003edc
	mov r2, r8
	ldr r0, [r2]
	movs r1, #1
	bl Object_SetModeById
	bl Func_02003fb4
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02003f3c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #162
	bl Func_02003dd4
	bl Func_02003e6c
.L_02009f44:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_02009f4c:
	.4byte 0xff1c0000
.L_02009f50:
	.4byte gPartyState
.L_02009f54:
	.4byte 0x000021b5
	.section .text.x02009f58,"ax",%progbits
	.global Func_02001f58
	.thumb_func
Func_02001f58:
	push {lr}
	ldr r3, .L_02009f70
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02009f6c
	movs r0, #118
	bl Func_02003ffc
.L_02009f6c:
	pop {pc}
	.2byte 0x0000
.L_02009f70:
	.4byte Data_0300122c
	.section .text.x02009f74,"ax",%progbits
	.global Func_02001f74
	.thumb_func
Func_02001f74:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #10
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #10
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	mov r10, r2
	adds r7, r5, #0
	movs r2, #204
	adds r7, #85
	mov r3, r10
	lsls r2, r2, #8
	strb r3, [r7]
	movs r0, #10
	ldr r1, .L_0200a080
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #134
	movs r2, #128
	movs r3, #195
	lsls r1, r1, #17
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r2, r2, #15
	bl Object_SetPositionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r2, #128
	lsls r2, r2, #16
	str r2, [r5, #20]
	movs r0, #1
	mov r8, r2
	bl WaitFrames
	movs r3, #160
	lsls r3, r3, #16
	str r3, [r5, #12]
	movs r0, #1
	bl WaitFrames
	ldr r6, .L_0200a084
	movs r1, #144
	adds r0, r6, #0
	lsls r1, r1, #3
	bl Func_02003da4
	movs r1, #132
	movs r3, #160
	mov r2, r8
	lsls r3, r3, #17
	lsls r1, r1, #16
	adds r0, r5, #0
	bl Func_02003e0c
	adds r0, r5, #0
	bl Func_02003e14
	adds r0, r6, #0
	bl Scheduler_RemoveCallbackFar
	movs r0, #10
	movs r1, #5
	bl Object_SetModeById
	movs r3, #3
	strb r3, [r7]
	movs r0, #10
	movs r1, #2
	bl Func_02003f44
	ldr r3, .L_0200a088
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #10
	bl Func_02003f14
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #156
	lsls r2, r2, #1
	movs r0, #10
	movs r1, #120
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #10
	movs r1, #5
	bl Object_SetModeById
	mov r3, r10
	strb r3, [r7]
	movs r1, #0
	movs r0, #10
	bl Func_02003f3c
	movs r0, #139
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02003dd4
	movs r0, #1
	bl WaitFrames
	bl Func_02003e6c
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a080:
	.4byte 0x00019999
.L_0200a084:
	.4byte Func_02001f58
.L_0200a088:
	.4byte gPartyState
	.section .text.x0200a08c,"ax",%progbits
	.global Func_0200208c
	.thumb_func
Func_0200208c:
	push {lr}
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	ldr r3, .L_0200a0b0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #6
	movs r2, #1
	bl Func_02003f0c
	bl Func_02003e6c
	pop {pc}
.L_0200a0b0:
	.4byte gPartyState
	.section .text.x0200a0b4,"ax",%progbits
	.global Func_020020b4
	.thumb_func
Func_020020b4:
	push {lr}
	ldr r3, .L_0200a0cc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #2
	adds r0, #34
	strb r3, [r0]
	pop {pc}
.L_0200a0cc:
	.4byte gPartyState
	.section .text.x0200a0d0,"ax",%progbits
	.global Func_020020d0
	.thumb_func
Func_020020d0:
	push {lr}
	ldr r3, .L_0200a0e8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #34
	strb r3, [r0]
	pop {pc}
.L_0200a0e8:
	.4byte gPartyState
	.section .text.x0200a0ec,"ax",%progbits
	.global Func_020020ec
	.thumb_func
Func_020020ec:
	push {r5, r6, lr}
	adds r6, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	adds r5, r2, #0
	mov r12, r3
	movs r3, #156
	lsls r3, r3, #1
	add r3, r12
	ldr r4, [r3]
	movs r3, #212
	lsls r3, r3, #1
	add r3, r12
	ldr r2, [r3]
	cmp r0, #0
	bge .L_0200a112
	ldr r3, .L_0200a140
	adds r0, r0, r3
.L_0200a112:
	asrs r0, r0, #20
	cmp r1, #0
	bge .L_0200a11c
	ldr r3, .L_0200a140
	adds r1, r1, r3
.L_0200a11c:
	asrs r3, r1, #20
	lsls r3, r3, #7
	adds r0, r0, r3
	movs r3, #1
	ands r3, r5
	cmp r3, #0
	beq .L_0200a130
	lsls r3, r0, #2
	adds r4, r4, r3
	strb r6, [r4, #2]
.L_0200a130:
	movs r3, #2
	ands r3, r5
	cmp r3, #0
	beq .L_0200a13e
	lsls r3, r0, #2
	adds r2, r2, r3
	strb r6, [r2, #2]
.L_0200a13e:
	pop {r5, r6, pc}
.L_0200a140:
	.4byte 0x000fffff
	.section .text.x0200a144,"ax",%progbits
	.global Func_02002144
	.thumb_func
Func_02002144:
	push {r5, r6, lr}
	movs r0, #12
	bl Object_GetById
	ldr r5, .L_0200a1c0
	ldr r3, [r0, #8]
	ldr r6, .L_0200a1c4
	str r3, [r5]
	ldr r3, [r0, #16]
	movs r0, #13
	str r3, [r6]
	bl Object_GetById
	ldr r3, .L_0200a1c8
	ldr r4, [r0, #8]
	str r4, [r3]
	ldr r3, .L_0200a1cc
	ldr r2, [r0, #16]
	str r2, [r3]
	ldr r0, [r5]
	asrs r3, r0, #20
	cmp r3, #7
	bne .L_0200a18e
	ldr r1, [r6]
	asrs r3, r1, #20
	cmp r3, #29
	bne .L_0200a18e
	asrs r3, r4, #20
	cmp r3, #7
	bne .L_0200a18e
	asrs r3, r2, #20
	cmp r3, #28
	bne .L_0200a18e
	movs r2, #2
	movs r3, #0
	bl Func_020020ec
.L_0200a18e:
	ldr r3, .L_0200a1c0
	ldr r3, [r3]
	asrs r3, r3, #20
	cmp r3, #12
	bne .L_0200a1be
	ldr r3, .L_0200a1c4
	ldr r3, [r3]
	asrs r3, r3, #20
	cmp r3, #25
	bne .L_0200a1be
	ldr r3, .L_0200a1c8
	ldr r0, [r3]
	asrs r3, r0, #20
	cmp r3, #12
	bne .L_0200a1be
	ldr r3, .L_0200a1cc
	ldr r1, [r3]
	asrs r3, r1, #20
	cmp r3, #26
	bne .L_0200a1be
	movs r2, #2
	movs r3, #0
	bl Func_020020ec
.L_0200a1be:
	pop {r5, r6, pc}
.L_0200a1c0:
	.4byte gOverlayArea + 0x519c
.L_0200a1c4:
	.4byte gOverlayArea + 0x5194
.L_0200a1c8:
	.4byte gOverlayArea + 0x5198
.L_0200a1cc:
	.4byte gOverlayArea + 0x5190
	.section .text.x0200a1d0,"ax",%progbits
	.global Func_020021d0
	.thumb_func
Func_020021d0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #12
	sub sp, #16
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #13
	bl Object_GetById
	ldr r3, [r7, #8]
	ldr r5, .L_0200a420
	asrs r3, r3, #20
	mov r11, r3
	ldr r3, [r7, #16]
	mov r8, r0
	asrs r3, r3, #20
	mov r9, r3
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	str r3, [sp, #12]
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	str r3, [sp, #8]
	ldr r0, [r5]
	asrs r3, r0, #20
	cmp r3, #12
	bne .L_0200a232
	ldr r6, .L_0200a424
	ldr r1, [r6]
	asrs r3, r1, #20
	cmp r3, #24
	bne .L_0200a232
	movs r2, #1
	movs r3, #21
	bl Func_020020ec
	ldr r0, [r5]
	ldr r1, [r6]
	movs r2, #2
	movs r3, #0
	bl Func_020020ec
	b .L_0200a240
.L_0200a232:
	ldr r3, .L_0200a424
	ldr r0, [r5]
	ldr r1, [r3]
	movs r2, #3
	movs r3, #0
	bl Func_020020ec
.L_0200a240:
	ldr r5, .L_0200a428
	ldr r0, [r5]
	asrs r3, r0, #20
	cmp r3, #7
	bne .L_0200a26a
	ldr r6, .L_0200a42c
	ldr r1, [r6]
	asrs r3, r1, #20
	cmp r3, #27
	bne .L_0200a26a
	movs r2, #1
	movs r3, #21
	bl Func_020020ec
	ldr r0, [r5]
	ldr r1, [r6]
	movs r2, #2
	movs r3, #0
	bl Func_020020ec
	b .L_0200a278
.L_0200a26a:
	ldr r3, .L_0200a42c
	ldr r0, [r5]
	ldr r1, [r3]
	movs r2, #3
	movs r3, #0
	bl Func_020020ec
.L_0200a278:
	ldr r0, [r7, #8]
	ldr r1, [r7, #16]
	movs r2, #3
	movs r3, #255
	bl Func_020020ec
	mov r2, r8
	ldr r0, [r2, #8]
	ldr r1, [r2, #16]
	movs r3, #255
	movs r2, #3
	bl Func_020020ec
	movs r0, #132
	lsls r0, r0, #2
	bl Func_02003dcc
	cmp r0, #0
	bne .L_0200a352
	mov r3, r11
	cmp r3, #12
	bne .L_0200a352
	mov r2, r9
	cmp r2, #26
	bne .L_0200a352
	movs r3, #0
	mov r10, r3
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	ldr r2, [sp, #12]
	cmp r2, #12
	bne .L_0200a2dc
	ldr r3, [sp, #8]
	cmp r3, #26
	bne .L_0200a2dc
	mov r2, r11
	mov r3, r9
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r0, #7
	movs r1, #2
	movs r3, #1
	bl Func_02003e24
	movs r2, #1
	mov r10, r2
.L_0200a2dc:
	adds r6, r7, #0
	movs r3, #3
	adds r6, #85
	strb r3, [r6]
	movs r0, #10
	bl WaitFrames
	movs r0, #161
	bl Func_02003ffc
	movs r0, #10
	bl WaitFrames
	movs r5, #0
	movs r0, #132
	strb r5, [r6]
	lsls r0, r0, #2
	bl Func_02003dd4
	mov r3, r10
	cmp r3, #0
	beq .L_0200a34e
	ldr r0, [r7, #8]
	ldr r1, [r7, #16]
	movs r2, #3
	movs r3, #0
	bl Func_020020ec
	mov r2, r8
	ldr r0, [r2, #8]
	ldr r1, [r2, #16]
	movs r3, #0
	movs r2, #3
	bl Func_020020ec
	movs r3, #12
	movs r2, #26
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	movs r0, #7
	movs r1, #3
	bl Func_02003e24
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl Func_02003dd4
	adds r2, r7, #0
	adds r2, #98
	movs r3, #1
	strb r3, [r2]
	mov r2, r8
	adds r2, #98
	strb r3, [r2]
.L_0200a34e:
	bl Func_02003e6c
.L_0200a352:
	movs r0, #137
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02003dcc
	cmp r0, #0
	bne .L_0200a412
	ldr r3, [sp, #12]
	cmp r3, #7
	bne .L_0200a412
	ldr r2, [sp, #8]
	cmp r2, #29
	bne .L_0200a412
	movs r3, #0
	mov r10, r3
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	mov r2, r11
	cmp r2, #7
	bne .L_0200a39a
	mov r3, r9
	cmp r3, #29
	bne .L_0200a39a
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r0, #7
	movs r1, #2
	movs r3, #1
	bl Func_02003e24
	movs r2, #1
	mov r10, r2
.L_0200a39a:
	mov r6, r8
	movs r3, #3
	adds r6, #85
	strb r3, [r6]
	movs r0, #10
	bl WaitFrames
	movs r0, #161
	bl Func_02003ffc
	movs r0, #10
	bl WaitFrames
	movs r0, #137
	movs r5, #0
	lsls r0, r0, #1
	strb r5, [r6]
	adds r0, #255
	bl Func_02003dd4
	mov r3, r10
	cmp r3, #0
	beq .L_0200a40e
	ldr r0, [r7, #8]
	ldr r1, [r7, #16]
	movs r2, #3
	movs r3, #0
	bl Func_020020ec
	mov r2, r8
	ldr r0, [r2, #8]
	ldr r1, [r2, #16]
	movs r3, #0
	movs r2, #3
	bl Func_020020ec
	movs r3, #7
	movs r2, #29
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	movs r0, #7
	movs r1, #3
	bl Func_02003e24
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl Func_02003dd4
	adds r2, r7, #0
	adds r2, #98
	movs r3, #1
	strb r3, [r2]
	mov r2, r8
	adds r2, #98
	strb r3, [r2]
.L_0200a40e:
	bl Func_02003e6c
.L_0200a412:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a420:
	.4byte gOverlayArea + 0x519c
.L_0200a424:
	.4byte gOverlayArea + 0x5194
.L_0200a428:
	.4byte gOverlayArea + 0x5198
.L_0200a42c:
	.4byte gOverlayArea + 0x5190
	.section .text.x0200a430,"ax",%progbits
	.global Func_02002430
	.thumb_func
Func_02002430:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #12
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #13
	bl Object_GetById
	ldr r3, [r5, #8]
	ldr r7, .L_0200a4b8
	ldr r6, .L_0200a4bc
	str r3, [r7]
	ldr r2, .L_0200a4c0
	ldr r3, [r5, #16]
	mov r8, r2
	str r3, [r6]
	ldr r3, [r0, #8]
	str r3, [r2]
	ldr r3, .L_0200a4c4
	mov r10, r3
	ldr r3, [r0, #16]
	mov r2, r10
	str r3, [r2]
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	ldr r3, [r7]
	asrs r3, r3, #20
	cmp r3, #7
	bne .L_0200a4a4
	ldr r3, [r6]
	asrs r3, r3, #20
	cmp r3, #29
	bne .L_0200a4a4
	mov r2, r8
	ldr r3, [r2]
	asrs r3, r3, #20
	cmp r3, #7
	bne .L_0200a4a4
	mov r2, r10
	ldr r3, [r2]
	asrs r3, r3, #20
	cmp r3, #28
	bne .L_0200a4a4
	adds r2, r5, #0
	adds r2, #98
	movs r3, #0
	strb r3, [r2]
	ldr r0, [r7]
	ldr r1, [r6]
	movs r2, #2
	bl Func_020020ec
.L_0200a4a4:
	bl Func_02003fe4
	bl Func_020021d0
	bl Func_02003e6c
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a4b8:
	.4byte gOverlayArea + 0x519c
.L_0200a4bc:
	.4byte gOverlayArea + 0x5194
.L_0200a4c0:
	.4byte gOverlayArea + 0x5198
.L_0200a4c4:
	.4byte gOverlayArea + 0x5190
	.section .text.x0200a4c8,"ax",%progbits
	.global Func_020024c8
	.thumb_func
Func_020024c8:
	push {lr}
	sub sp, #8
	movs r3, #5
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #2
	movs r2, #1
	movs r3, #2
	movs r0, #5
	bl Func_02003e24
	movs r0, #138
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02003dd4
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x0200a4f0,"ax",%progbits
	.global Func_020024f0
	.thumb_func
Func_020024f0:
	push {r5, r6, lr}
	ldr r3, .L_0200a534
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200a532
	ldr r5, .L_0200a538
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	adds r6, #85
	movs r3, #0
	strb r3, [r6]
	movs r2, #140
	ldr r0, [r5]
	movs r1, #72
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r3, #3
	strb r3, [r6]
	bl Func_02003e6c
.L_0200a532:
	pop {r5, r6, pc}
.L_0200a534:
	.4byte gInput
.L_0200a538:
	.4byte gPartyState
	.section .text.x0200a53c,"ax",%progbits
	.global Func_0200253c
	.thumb_func
Func_0200253c:
	push {r5, r6, r7, lr}
	ldr r3, .L_0200a598
	movs r2, #128
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200a596
	ldr r3, .L_0200a59c
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	ldr r0, [r7]
	ldr r5, .L_0200a5a0
	bl Object_GetById
	ldrh r3, [r5]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #246
	adds r3, r3, r2
	movs r2, #200
	lsls r3, r3, #16
	lsls r2, r2, #16
	adds r6, r0, #0
	cmp r3, r2
	bls .L_0200a596
	bl Func_02003e64
	adds r5, r6, #0
	movs r0, #0
	bl Func_02003fa4
	adds r5, #85
	movs r3, #0
	strb r3, [r5]
	movs r2, #148
	ldr r0, [r7]
	movs r1, #72
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r3, #3
	strb r3, [r5]
	bl Func_02003e6c
.L_0200a596:
	pop {r5, r6, r7, pc}
.L_0200a598:
	.4byte gInput
.L_0200a59c:
	.4byte gPartyState
.L_0200a5a0:
	.4byte gSceneState
	.section .text.x0200a5a4,"ax",%progbits
	.global Func_020025a4
	.thumb_func
Func_020025a4:
	push {lr}
	movs r0, #134
	lsls r0, r0, #2
	bl Func_02003dd4
	pop {pc}
	.section .text.x0200a5b0,"ax",%progbits
	.global Func_020025b0
	.thumb_func
Func_020025b0:
	push {lr}
	movs r0, #134
	lsls r0, r0, #2
	bl Func_02003ddc
	pop {pc}
	.section .text.x0200a5bc,"ax",%progbits
	.global Func_020025bc
	.thumb_func
Func_020025bc:
	push {lr}
	sub sp, #12
	movs r3, #65
	movs r2, #10
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #65
	movs r1, #0
	movs r2, #4
	movs r3, #4
	bl Func_02003fec
	add sp, #12
	pop {pc}
	.section .text.x0200a5dc,"ax",%progbits
	.global Func_020025dc
	.thumb_func
Func_020025dc:
	push {lr}
	sub sp, #12
	movs r3, #71
	movs r2, #10
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #65
	movs r1, #0
	movs r2, #2
	movs r3, #2
	bl Func_02003fec
	add sp, #12
	pop {pc}
	.section .text.x0200a5fc,"ax",%progbits
	.global Func_020025fc
	.thumb_func
Func_020025fc:
	push {lr}
	sub sp, #12
	movs r3, #68
	movs r2, #15
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #65
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02003fec
	add sp, #12
	pop {pc}
	.section .text.x0200a61c,"ax",%progbits
	.global Func_0200261c
	.thumb_func
Func_0200261c:
	push {lr}
	sub sp, #12
	movs r3, #75
	movs r2, #20
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #65
	movs r1, #0
	movs r2, #2
	movs r3, #3
	bl Func_02003fec
	add sp, #12
	pop {pc}
	.section .text.x0200a63c,"ax",%progbits
	.global Func_0200263c
	.thumb_func
Func_0200263c:
	push {lr}
	sub sp, #12
	movs r3, #78
	movs r2, #22
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #65
	movs r1, #0
	movs r2, #4
	movs r3, #1
	bl Func_02003fec
	add sp, #12
	pop {pc}
	.section .text.x0200a65c,"ax",%progbits
	.global Func_0200265c
	.thumb_func
Func_0200265c:
	push {lr}
	sub sp, #12
	movs r3, #79
	movs r2, #23
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #65
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02003fec
	add sp, #12
	pop {pc}
	.section .text.x0200a67c,"ax",%progbits
	.global Func_0200267c
	.thumb_func
Func_0200267c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	ldr r3, .L_0200a740
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	ldr r0, [r7]
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r2, r2
	negs r0, r0
	negs r1, r1
	bl Func_02003f6c
	ldr r0, [r7]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #128
	movs r2, #128
	ldr r0, [r7]
	lsls r2, r2, #7
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5, #8]
	asrs r2, r0, #16
	adds r3, r2, #0
	cmp r2, #0
	bge .L_0200a6d2
	adds r3, #15
.L_0200a6d2:
	asrs r3, r3, #4
	lsls r3, r3, #4
	subs r3, r2, r3
	movs r1, #8
	subs r1, r1, r3
	lsls r1, r1, #16
	adds r1, r1, r0
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Func_02003e0c
	adds r0, r5, #0
	bl Func_02003e14
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r5, #6]
	movs r3, #170
	lsls r3, r3, #1
	adds r6, r6, r3
	movs r2, #0
	ldrsh r5, [r6, r2]
	movs r0, #158
	bl Func_02003ffc
	subs r5, #1
	ldr r0, .L_0200a744
	lsls r5, r5, #3
	adds r3, r5, #4
	ldrh r1, [r0, r3]
	adds r3, r3, r0
	ldrh r2, [r3, #2]
	ldr r0, [r0, r5]
	bl Func_02003e1c
	ldr r1, .L_0200a748
	ldr r0, [r7]
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #12
	bl WaitFrames
	movs r0, #123
	bl Func_02003ffc
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl Func_02003f7c
	pop {r5, r6, r7, pc}
.L_0200a740:
	.4byte gPartyState
.L_0200a744:
	.4byte Data_020046bc
.L_0200a748:
	.4byte Data_02004028
	.section .text.x0200a74c,"ax",%progbits
	.global Func_0200274c
	.thumb_func
Func_0200274c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	bl Func_020034ec
	movs r0, #123
	bl Func_02003ffc
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r3, #170
	lsls r3, r3, #1
	adds r5, r5, r3
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl Func_02003f7c
	pop {r5, pc}
	.section .text.x0200a780,"ax",%progbits
	.global Func_02002780
	.thumb_func
Func_02002780:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	ldr r3, .L_0200a828
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r2, r2
	negs r0, r0
	negs r1, r1
	bl Func_02003f6c
	ldr r0, [r6]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #128
	movs r2, #128
	ldr r0, [r6]
	lsls r2, r2, #7
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5, #8]
	asrs r2, r0, #16
	adds r3, r2, #0
	cmp r2, #0
	bge .L_0200a7d6
	adds r3, #15
.L_0200a7d6:
	asrs r3, r3, #4
	lsls r3, r3, #4
	subs r3, r2, r3
	movs r1, #8
	subs r1, r1, r3
	lsls r1, r1, #16
	ldr r2, [r5, #12]
	adds r1, r1, r0
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Func_02003e0c
	adds r0, r5, #0
	bl Func_02003e14
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r5, #6]
	ldr r1, .L_0200a82c
	ldr r0, [r6]
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #12
	bl WaitFrames
	movs r0, #123
	bl Func_02003ffc
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_02003f7c
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a828:
	.4byte gPartyState
.L_0200a82c:
	.4byte Data_02004028
	.section .text.x0200a830,"ax",%progbits
	.global Func_02002830
	.thumb_func
Func_02002830:
	push {r5, r6, lr}
	mov r6, r11
	mov r5, r10
	push {r5, r6}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	ldr r5, .L_0200a9ac
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r5, r1
	ldr r0, [r5]
	bl Object_GetById
	mov r10, r0
	movs r0, #9
	bl Object_GetById
	ldr r1, [r5]
	mov r8, r0
	movs r0, #9
	bl Func_02003eec
	movs r2, #130
	ldr r1, .L_0200a9b0
	lsls r2, r2, #18
	movs r0, #10
	bl Func_02003edc
	movs r0, #1
	bl WaitFrames
	movs r3, #192
	mov r2, r8
	lsls r3, r3, #8
	strh r3, [r2, #6]
	ldr r3, .L_0200a9b4
	mov r1, r10
	str r3, [r1, #108]
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #15
	bl Func_02003f1c
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #1
	bl WaitFrames
	movs r2, #192
	lsls r2, r2, #18
	movs r3, #214
	mov r9, r2
	lsls r3, r3, #1
	ldr r2, [r2, #108]
	mov r11, r3
	mov r1, r11
	adds r3, #85
	str r3, [r2, r1]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #153
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #153
	adds r2, #204
	movs r0, #10
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	bl Object_GetById
	ldr r3, .L_0200a9b8
	mov r2, r8
	str r3, [r0, #108]
	movs r1, #140
	str r3, [r2, #108]
	movs r2, #234
	movs r0, #10
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #148
	movs r2, #226
	lsls r2, r2, #1
	movs r0, #9
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #10
	movs r1, #1
	bl Object_SetModeById
	movs r6, #0
	mov r3, r10
	mov r1, r8
	str r6, [r3, #108]
	movs r0, #10
	str r6, [r1, #108]
	bl Object_GetById
	movs r1, #9
	str r6, [r0, #108]
	ldr r0, [r5]
	bl Func_02003fac
	ldr r0, .L_0200a9bc
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r0, #9
	movs r1, #1
	bl Object_SetModeById
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	ldr r0, [r5]
	ldr r1, .L_0200a9c0
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl Func_02003f1c
	ldr r0, [r5]
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #156
	movs r2, #220
	lsls r2, r2, #1
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #7
	bl Func_02003f3c
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003fb4
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02003f3c
	mov r3, r9
	ldr r2, [r3, #108]
	movs r3, #128
	lsls r3, r3, #1
	mov r1, r11
	str r3, [r2, r1]
	bl Func_020034d8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r3}
	mov r11, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a9ac:
	.4byte gPartyState
.L_0200a9b0:
	.4byte 0x01110000
.L_0200a9b4:
	.4byte Func_02001278
.L_0200a9b8:
	.4byte Func_02000df8
.L_0200a9bc:
	.4byte 0x000021f4
.L_0200a9c0:
	.4byte 0x00019999
	.section .text.x0200a9c4,"ax",%progbits
	.global Func_020029c4
	.thumb_func
Func_020029c4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, .L_0200aa40
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #9
	bl Object_GetById
	ldr r1, [r5]
	adds r7, r0, #0
	movs r0, #9
	bl Func_02003eec
	movs r1, #240
	movs r2, #135
	lsls r2, r2, #18
	lsls r1, r1, #16
	movs r0, #10
	bl Func_02003edc
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_0200aa3c
	mov r8, r3
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r7, #6]
	ldr r3, .L_0200aa44
	ldr r0, [r5]
	str r3, [r6, #108]
	bl Object_GetById
	movs r1, #15
	bl Func_02003f1c
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #1
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	bl Event_SetStatus1c6
	b .L_0200aa48
.L_0200aa3c:
	.4byte 0x00000000
.L_0200aa40:
	.4byte gPartyState
.L_0200aa44:
	.4byte Func_02001278
.L_0200aa48:
	bl Event_WaitValue1c8Frames
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
	adds r1, #204
	adds r2, #102
	movs r0, #10
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	bl Object_GetById
	ldr r3, .L_0200ab4c
	adds r5, r7, #0
	str r3, [r0, #108]
	adds r5, #99
	str r3, [r7, #108]
	mov r3, r8
	ldr r1, .L_0200ab50
	adds r0, r7, #0
	strb r3, [r5]
	bl Func_02003dec
	ldr r1, .L_0200ab54
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
.L_0200aa94:
	movs r0, #1
	bl WaitFrames
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_0200aa94
	movs r5, #0
	str r5, [r6, #108]
	movs r0, #10
	str r5, [r7, #108]
	bl Object_GetById
	movs r1, #192
	str r5, [r0, #108]
	lsls r1, r1, #8
	movs r0, #9
	bl Func_02003f3c
	ldr r5, .L_0200ab58
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	movs r1, #9
	bl Func_02003fac
	ldr r0, .L_0200ab5c
	bl Func_02003f24
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	movs r0, #9
	movs r1, #1
	bl Object_SetModeById
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	ldr r0, [r5]
	ldr r1, .L_0200ab60
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl Func_02003f1c
	ldr r0, [r5]
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #230
	ldr r0, [r5]
	movs r1, #152
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	bl Func_02003f3c
	movs r0, #9
	movs r1, #0
	bl Func_02003f2c
	bl Func_02003fb4
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #7
	bl Func_02003f3c
	bl Func_020034d8
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ab4c:
	.4byte Func_02000df8
.L_0200ab50:
	.4byte Data_02004508
.L_0200ab54:
	.4byte Data_02004554
.L_0200ab58:
	.4byte gPartyState
.L_0200ab5c:
	.4byte 0x000021ef
.L_0200ab60:
	.4byte 0x00019999
	.section .text.x0200ab64,"ax",%progbits
	.global Func_02002b64
	.thumb_func
Func_02002b64:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #128
	adds r3, r3, r1
	lsls r2, r2, #1
	str r2, [r3]
	ldr r3, .L_0200abc0
	adds r2, #224
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200abc4
	cmp r2, r3
	bne .L_0200ab8e
	bl Func_02002c38
	b .L_0200abbc
.L_0200ab8e:
	ldr r3, .L_0200abc8
	cmp r2, r3
	bne .L_0200ab9a
	bl Func_02002f1c
	b .L_0200abbc
.L_0200ab9a:
	ldr r3, .L_0200abcc
	cmp r2, r3
	bne .L_0200aba6
	bl Func_020030b4
	b .L_0200abbc
.L_0200aba6:
	ldr r3, .L_0200abd0
	cmp r2, r3
	bne .L_0200abb2
	bl Func_02003164
	b .L_0200abbc
.L_0200abb2:
	ldr r3, .L_0200abd4
	cmp r2, r3
	bne .L_0200abbc
	bl Func_0200329c
.L_0200abbc:
	movs r0, #0
	pop {pc}
.L_0200abc0:
	.4byte gPartyState
.L_0200abc4:
	.4byte 0x00000083
.L_0200abc8:
	.4byte 0x00000084
.L_0200abcc:
	.4byte 0x00000085
.L_0200abd0:
	.4byte 0x00000086
.L_0200abd4:
	.4byte 0x00000087
	.section .text.x0200abd8,"ax",%progbits
	.global Func_02002bd8
	.thumb_func
Func_02002bd8:
	push {r5, lr}
	ldr r3, .L_0200ac34
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #12
	cmp r3, r2
	ble .L_0200ac12
	movs r0, #9
	bl Object_GetById
	adds r0, #89
	ldrb r2, [r0]
	movs r5, #247
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #10
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	ands r5, r3
	b .L_0200ac2e
.L_0200ac12:
	movs r0, #9
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #8
	orrs r3, r5
	strb r3, [r0]
	movs r0, #10
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	orrs r5, r3
.L_0200ac2e:
	strb r5, [r0]
	pop {r5, pc}
	.2byte 0x0000
.L_0200ac34:
	.4byte gPartyState
	.section .text.x0200ac38,"ax",%progbits
	.global Func_02002c38
	.thumb_func
Func_02002c38:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #32]
	ldr r3, .L_0200ad04
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_0200ac58
	movs r0, #48
	adds r0, #255
	bl Func_02003ddc
.L_0200ac58:
	ldrb r2, [r5, #23]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r5, #23]
	movs r0, #11
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	movs r0, #10
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	bl Object_GetById
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #161
	bl Func_02003dcc
	cmp r0, #0
	beq .L_0200acee
	movs r0, #234
	movs r1, #140
	movs r3, #210
	adds r0, #255
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #18
	bl Func_02003df4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200ace0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	movs r1, #227
	strb r3, [r2]
	lsls r1, r1, #1
	bl Func_02003e3c
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r2, r5, #0
	adds r2, #89
	movs r3, #9
	strb r3, [r2]
	adds r2, #3
	movs r3, #1
	strb r3, [r2]
.L_0200ace0:
	movs r0, #11
	bl Object_GetById
	movs r3, #208
	lsls r3, r3, #8
	strh r3, [r0, #6]
	b .L_0200acf6
.L_0200acee:
	movs r0, #11
	movs r1, #5
	bl Object_SetModeById
.L_0200acf6:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200ad08
	bl Func_02003da4
	pop {r5, pc}
	.2byte 0x0000
.L_0200ad04:
	.4byte gPartyState
.L_0200ad08:
	.4byte Func_02002bd8
	.section .text.x0200ad0c,"ax",%progbits
	.global Func_02002d0c
	.thumb_func
Func_02002d0c:
	push {r5, lr}
	bl Object_GetById
	movs r1, #0
	adds r5, r0, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r2, r5, #0
	adds r2, #92
	movs r3, #3
	strb r3, [r2]
	adds r3, r5, #0
	movs r1, #0
	adds r3, #85
	strb r1, [r3]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #128
	orrs r3, r2
	strb r3, [r1]
	movs r3, #152
	lsls r3, r3, #6
	adds r3, #102
	str r3, [r5, #52]
	movs r3, #152
	lsls r3, r3, #7
	adds r3, #204
	str r3, [r5, #48]
	pop {r5, pc}
	.section .text.x0200ad48,"ax",%progbits
	.global Func_02002d48
	.thumb_func
Func_02002d48:
	push {r5, r6, lr}
	adds r6, r2, #0
	adds r5, r1, #0
	bl Object_GetById
	adds r3, r0, #0
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r5, r5, #5
	adds r5, r5, r3
	adds r3, r0, #0
	adds r3, #102
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r6, r6, #5
	adds r6, r6, r3
	lsls r5, r5, #16
	lsls r6, r6, #16
	ldr r2, [r0, #12]
	adds r1, r5, #0
	adds r3, r6, #0
	bl Func_02003e0c
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200ad7c,"ax",%progbits
	.global Func_02002d7c
	.thumb_func
Func_02002d7c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, .L_0200aec0
	ldr r6, [r3, #108]
	movs r2, #179
	lsls r2, r2, #1
	adds r1, r7, #2
	adds r3, r6, r2
	mov r8, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200adaa
	movs r0, #134
	lsls r0, r0, #2
	bl Func_02003dcc
	movs r5, #11
	cmp r0, #0
	beq .L_0200adce
.L_0200adaa:
	movs r5, #11
.L_0200adac:
	adds r0, r5, #0
	bl Object_GetById
	adds r5, #1
	adds r0, #91
	movs r3, #1
	strb r3, [r0]
	cmp r5, #18
	ble .L_0200adac
	b .L_0200af14
.L_0200adc0:
	adds r0, r5, #0
	bl Object_GetById
	movs r3, #0
	adds r0, #91
	strb r3, [r0]
	adds r5, #1
.L_0200adce:
	cmp r5, #18
	ble .L_0200adc0
	ldr r3, .L_0200aec4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r1, .L_0200aec8
	ldr r3, [r0, #12]
	cmp r3, r1
	bge .L_0200ae0e
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	movs r5, #11
.L_0200adf0:
	adds r0, r5, #0
	bl Object_GetById
	adds r5, #1
	adds r0, #91
	movs r3, #1
	strb r3, [r0]
	cmp r5, #18
	ble .L_0200adf0
	movs r3, #181
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #200
	strh r3, [r2]
	b .L_0200af14
.L_0200ae0e:
	mov r1, r8
	ldrh r3, [r1]
	movs r1, #171
	adds r2, r3, #1
	mov r3, r8
	strh r2, [r3]
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_0200ae28
	b .L_0200af14
.L_0200ae28:
	lsls r3, r2, #16
	cmp r3, #0
	beq .L_0200af14
.L_0200ae2e:
	ldrh r3, [r7]
	cmp r3, #100
	beq .L_0200aeae
	cmp r3, #100
	bgt .L_0200ae3e
	cmp r3, #0
	beq .L_0200ae48
	b .L_0200aee6
.L_0200ae3e:
	cmp r3, #130
	beq .L_0200aecc
	cmp r3, #220
	beq .L_0200aeae
	b .L_0200aee6
.L_0200ae48:
	movs r5, #11
.L_0200ae4a:
	adds r0, r5, #0
	movs r1, #2
	adds r5, #1
	bl Object_SetModeById
	cmp r5, #18
	ble .L_0200ae4a
	movs r5, #1
	negs r5, r5
	movs r0, #11
	movs r1, #0
	movs r2, #1
	bl Func_02002d48
	movs r0, #12
	movs r1, #0
	adds r2, r5, #0
	bl Func_02002d48
	movs r0, #13
	movs r1, #0
	movs r2, #1
	bl Func_02002d48
	movs r0, #14
	movs r1, #0
	movs r2, #1
	bl Func_02002d48
	movs r0, #15
	adds r1, r5, #0
	movs r2, #0
	bl Func_02002d48
	movs r0, #16
	movs r1, #0
	movs r2, #1
	bl Func_02002d48
	movs r0, #17
	movs r1, #1
	movs r2, #0
	bl Func_02002d48
	movs r0, #18
	movs r1, #0
	adds r2, r5, #0
	bl Func_02002d48
	b .L_0200aee6
.L_0200aeae:
	movs r5, #11
.L_0200aeb0:
	adds r0, r5, #0
	movs r1, #1
	adds r5, #1
	bl Object_SetModeById
	cmp r5, #18
	ble .L_0200aeb0
	b .L_0200aee6
.L_0200aec0:
	.4byte gSceneState
.L_0200aec4:
	.4byte gPartyState
.L_0200aec8:
	.4byte 0xfff40000
.L_0200aecc:
	movs r5, #11
.L_0200aece:
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetModeById
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	adds r5, #1
	bl Func_02002d48
	cmp r5, #18
	ble .L_0200aece
.L_0200aee6:
	ldrh r3, [r7]
	movs r2, #130
	adds r3, #1
	strh r3, [r7]
	lsls r2, r2, #17
	lsls r3, r3, #16
	cmp r3, r2
	bne .L_0200aefa
	ldr r3, .L_0200af10
	strh r3, [r7]
.L_0200aefa:
	mov r1, r8
	ldrh r3, [r1]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200ae2e
	b .L_0200af14
.L_0200af10:
	.4byte 0x00000000
.L_0200af14:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200af1c,"ax",%progbits
	.global Func_02002f1c
	.thumb_func
Func_02002f1c:
	push {r5, r6, lr}
	ldr r5, .L_0200b09c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	movs r2, #241
	lsls r2, r2, #1
	strb r3, [r0]
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_0200af4e
	movs r0, #48
	adds r0, #255
	bl Func_02003ddc
.L_0200af4e:
	movs r5, #11
.L_0200af50:
	adds r0, r5, #0
	adds r5, #1
	bl Func_02002d0c
	cmp r5, #18
	ble .L_0200af50
	ldr r5, .L_0200b0a0
	movs r0, #10
	adds r0, #255
	adds r6, r5, #2
	bl Func_02003dcc
	cmp r0, #0
	bne .L_0200af70
	strh r0, [r5]
	strh r0, [r6]
.L_0200af70:
	movs r1, #144
	ldr r0, .L_0200b0a4
	lsls r1, r1, #3
	bl Func_02003da4
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #44
	bl Func_02003dcc
	cmp r0, #0
	beq .L_0200afe8
	movs r0, #10
	bl Object_GetById
	movs r1, #176
	movs r2, #168
	adds r5, r0, #0
	lsls r2, r2, #16
	movs r0, #10
	lsls r1, r1, #16
	bl Func_02003edc
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	adds r0, r5, #0
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	adds r3, r5, #0
	movs r1, #0
	adds r3, #85
	strb r1, [r3]
	ldr r3, .L_0200b0a8
	movs r0, #9
	str r3, [r5, #12]
	str r3, [r5, #20]
	movs r3, #10
	movs r5, #9
	str r3, [sp, #4]
	movs r1, #1
	movs r2, #4
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02003e24
	movs r3, #74
	str r3, [sp, #4]
	movs r0, #9
	movs r1, #64
	movs r2, #4
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02003e24
	b .L_0200afee
.L_0200afe8:
	movs r0, #10
	bl Func_02003684
.L_0200afee:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #162
	bl Func_02003dcc
	cmp r0, #0
	bne .L_0200b01e
	movs r0, #9
	movs r1, #19
	bl Object_LinkObjectAndSetCallback
	movs r0, #19
	movs r1, #0
	bl Object_SetModeById
	movs r0, #19
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	b .L_0200b096
.L_0200b01e:
	movs r0, #10
	adds r0, #255
	bl Func_02003dcc
	cmp r0, #0
	bne .L_0200b096
	movs r0, #9
	bl Object_GetById
	movs r3, #160
	lsls r3, r3, #7
	strh r3, [r0, #6]
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #128
	orrs r3, r5
	strb r3, [r0]
	movs r1, #234
	movs r3, #192
	movs r2, #204
	lsls r3, r3, #6
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #19
	bl Func_02003ee4
	movs r0, #19
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r1, #128
	orrs r5, r3
	movs r2, #128
	strb r5, [r0]
	lsls r1, r1, #10
	movs r0, #9
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #19
	ldr r1, .L_0200b0ac
	bl ObjectMotion_SetSpeedParameters
	ldr r5, .L_0200b0b0
	movs r0, #9
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #19
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #128
	lsls r0, r0, #2
	bl Func_02003dd4
.L_0200b096:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b09c:
	.4byte gPartyState
.L_0200b0a0:
	.4byte gSceneState
.L_0200b0a4:
	.4byte Func_02002d7c
.L_0200b0a8:
	.4byte 0xfff80000
.L_0200b0ac:
	.4byte 0x00019999
.L_0200b0b0:
	.4byte Data_02004274
	.section .text.x0200b0b4,"ax",%progbits
	.global Func_020030b4
	.thumb_func
Func_020030b4:
	push {r5, lr}
	ldr r3, .L_0200b160
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_0200b0ce
	movs r0, #48
	adds r0, #255
	bl Func_02003ddc
.L_0200b0ce:
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #163
	bl Func_02003dcc
	cmp r0, #0
	beq .L_0200b138
	movs r0, #234
	movs r1, #208
	movs r2, #192
	movs r3, #160
	adds r0, #255
	lsls r1, r1, #15
	lsls r2, r2, #13
	lsls r3, r3, #16
	bl Func_02003df4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200b12a
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	movs r1, #198
	strb r3, [r2]
	adds r1, #255
	bl Func_02003e3c
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r2, r5, #0
	adds r2, #92
	movs r3, #1
	strb r3, [r2]
.L_0200b12a:
	movs r1, #208
	movs r2, #204
	movs r0, #10
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl Func_02003edc
.L_0200b138:
	movs r1, #1
	movs r0, #11
	bl Func_02003f44
	movs r0, #11
	bl Object_GetById
	movs r1, #15
	bl Func_02003f1c
	movs r0, #11
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	pop {r5, pc}
	.2byte 0x0000
.L_0200b160:
	.4byte gPartyState
	.section .text.x0200b164,"ax",%progbits
	.global Func_02003164
	.thumb_func
Func_02003164:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #10
	ldr r5, [r3, #32]
	sub sp, #8
	bl Object_GetById
	movs r2, #200
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r6, #0
	adds r2, #4
	str r6, [r3]
	adds r3, r5, r2
	str r6, [r3]
	ldr r3, .L_0200b298
	adds r2, #78
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r7, r0, #0
	cmp r3, #11
	bne .L_0200b19c
	movs r0, #48
	adds r0, #255
	bl Func_02003ddc
.L_0200b19c:
	movs r0, #10
	adds r0, #255
	bl Func_02003dcc
	cmp r0, #0
	bne .L_0200b1b6
	adds r3, r7, #0
	adds r3, #85
	strb r6, [r3]
	movs r0, #10
	movs r1, #1
	bl Func_02003f44
.L_0200b1b6:
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl Func_02003edc
	movs r0, #12
	bl Func_02003f4c
	movs r0, #13
	bl Func_02003f4c
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl Func_02003dcc
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200b216
	movs r0, #12
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #13
	bl Object_GetById
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	adds r3, r0, #0
	adds r3, #85
	strb r6, [r3]
	adds r0, #98
	movs r3, #1
	adds r5, #98
	strb r3, [r5]
	movs r2, #29
	strb r3, [r0]
	movs r3, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #7
	movs r1, #3
	movs r2, #1
	movs r3, #1
	bl Func_02003e24
	b .L_0200b24a
.L_0200b216:
	movs r0, #12
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #13
	bl Object_GetById
	adds r3, r5, #0
	adds r6, r0, #0
	adds r3, #85
	strb r7, [r3]
	adds r3, r6, #0
	adds r3, #85
	strb r7, [r3]
	movs r2, #3
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r3, #255
	bl Func_020020ec
	ldr r0, [r6, #8]
	ldr r1, [r6, #16]
	movs r2, #3
	movs r3, #255
	bl Func_020020ec
.L_0200b24a:
	movs r0, #14
	bl Func_02003f4c
	movs r0, #14
	bl Object_GetById
	movs r3, #1
	adds r1, r0, #0
	adds r0, #98
	strb r3, [r0]
	adds r3, r1, #0
	movs r2, #0
	adds r3, #85
	strb r2, [r3]
	movs r2, #1
	ldr r0, [r1, #8]
	movs r3, #255
	ldr r1, [r1, #16]
	bl Func_020020ec
	movs r0, #138
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02003dcc
	cmp r0, #0
	beq .L_0200b294
	movs r3, #5
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #5
	movs r1, #2
	movs r2, #1
	movs r3, #2
	bl Func_02003e24
.L_0200b294:
	add sp, #8
	pop {r5, r6, r7, pc}
.L_0200b298:
	.4byte gPartyState
	.section .text.x0200b29c,"ax",%progbits
	.global Func_0200329c
	.thumb_func
Func_0200329c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #32]
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	ldrb r2, [r5, #23]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r5, #23]
	ldr r3, .L_0200b400
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_0200b2d4
	movs r0, #48
	adds r0, #255
	bl Func_02003ddc
.L_0200b2d4:
	bl Func_02003410
	movs r1, #12
	movs r2, #5
	movs r3, #4
	movs r0, #3
	bl Func_0200346c
	movs r0, #4
	movs r1, #12
	movs r2, #5
	movs r3, #4
	bl Func_0200346c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #165
	bl Func_02003dcc
	cmp r0, #0
	beq .L_0200b332
	movs r0, #10
	adds r0, #255
	bl Func_02003dcc
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200b332
	movs r1, #140
	movs r2, #234
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #10
	bl Func_02003edc
	movs r0, #10
	bl Object_GetById
	movs r1, #148
	adds r0, #85
	movs r2, #226
	strb r5, [r0]
	lsls r1, r1, #17
	movs r0, #9
	lsls r2, r2, #17
	bl Func_02003edc
.L_0200b332:
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #11
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #144
	ldr r0, .L_0200b404
	lsls r1, r1, #3
	bl Func_02003da4
	ldr r3, .L_0200b400
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #4
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_0200b3b0
	movs r0, #140
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02003dd4
	movs r0, #10
	adds r0, #255
	bl Func_02003dcc
	cmp r0, #0
	bne .L_0200b3b0
	movs r3, #128
	movs r1, #152
	movs r2, #192
	lsls r3, r3, #7
	movs r0, #9
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02003ee4
	movs r1, #176
	movs r0, #10
	lsls r1, r1, #16
	ldr r2, .L_0200b408
	bl Func_02003edc
	movs r0, #1
	bl WaitFrames
.L_0200b3b0:
	movs r0, #10
	adds r0, #255
	bl Func_02003dcc
	cmp r0, #0
	bne .L_0200b3ee
	ldr r3, .L_0200b400
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_0200b3d2
	bl Func_02002830
	b .L_0200b3fa
.L_0200b3d2:
	cmp r3, #4
	bne .L_0200b3dc
	bl Func_020029c4
	b .L_0200b3fa
.L_0200b3dc:
	cmp r3, #5
	beq .L_0200b3fa
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_020034d8
	b .L_0200b3fa
.L_0200b3ee:
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_020034d8
.L_0200b3fa:
	bl Func_02003e6c
	pop {r5, pc}
.L_0200b400:
	.4byte gPartyState
.L_0200b404:
	.4byte Func_02001c4c
.L_0200b408:
	.4byte 0x03090000
	.section .text.x0200b40c,"ax",%progbits
	.global Func_0200340c
	.thumb_func
Func_0200340c:
	movs r0, #0
	bx lr
	.section .text.x0200b410,"ax",%progbits
	.global Func_02003410
	.thumb_func
Func_02003410:
	push {r5, lr}
	movs r0, #112
	movs r1, #180
	bl Runtime_AllocateBlock
	movs r5, #0
	adds r4, r0, #0
.L_0200b41e:
	movs r3, #0
	str r3, [r4]
	strh r3, [r4, #4]
	strh r3, [r4, #6]
	strh r3, [r4, #8]
	strh r3, [r4, #10]
	movs r2, #0
.L_0200b42c:
	lsls r3, r2, #16
	lsrs r3, r3, #16
	ldr r1, .L_0200b45c
	lsls r2, r3, #1
	adds r2, #12
	strh r1, [r4, r2]
	adds r3, #1
	movs r1, #128
	lsls r3, r3, #16
	lsls r1, r1, #13
	asrs r2, r3, #16
	cmp r3, r1
	bne .L_0200b42c
	movs r2, #128
	lsls r3, r5, #16
	lsls r2, r2, #9
	movs r1, #128
	adds r3, r3, r2
	lsls r1, r1, #11
	adds r4, #44
	asrs r5, r3, #16
	cmp r3, r1
	bne .L_0200b41e
	b .L_0200b460
.L_0200b45c:
	.4byte 0x00000000
.L_0200b460:
	adds r2, r0, #0
	adds r2, #176
	movs r3, #0
	strh r3, [r2]
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200b46c,"ax",%progbits
	.global Func_0200346c
	.thumb_func
Func_0200346c:
	push {r5, r6, r7, lr}
	lsls r2, r2, #16
	lsls r3, r3, #16
	asrs r7, r2, #16
	asrs r2, r3, #16
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #112]
	lsls r1, r1, #16
	adds r5, r4, #0
	adds r5, #176
	asrs r6, r1, #16
	ldrh r1, [r5]
	lsls r0, r0, #16
	asrs r0, r0, #16
	cmp r1, #3
	bls .L_0200b494
	movs r0, #1
	negs r0, r0
	b .L_0200b4d6
.L_0200b494:
	movs r3, #44
	muls r1, r3
	lsls r0, r0, #16
	lsls r3, r6, #16
	lsrs r3, r3, #16
	lsrs r0, r0, #12
	adds r0, r0, r3
	movs r3, #160
	lsls r3, r3, #19
	lsls r0, r0, #1
	adds r1, r4, r1
	adds r0, r0, r3
	movs r3, #0
	strh r3, [r1, #4]
	strh r3, [r1, #6]
	lsls r2, r2, #16
	movs r4, #128
	movs r3, #128
	lsrs r2, r2, #16
	lsls r4, r4, #24
	lsls r3, r3, #19
	strh r2, [r1, #10]
	str r0, [r1]
	strh r7, [r1, #8]
	adds r3, #212
	adds r1, #12
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldrh r3, [r5]
	movs r0, #0
	adds r3, #1
	strh r3, [r5]
.L_0200b4d6:
	pop {r5, r6, r7, pc}
	.section .text.x0200b4d8,"ax",%progbits
	.global Func_020034d8
	.thumb_func
Func_020034d8:
	push {lr}
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200b4e8
	bl Func_02003da4
	pop {pc}
	.2byte 0x0000
.L_0200b4e8:
	.4byte Func_020034fc
	.section .text.x0200b4ec,"ax",%progbits
	.global Func_020034ec
	.thumb_func
Func_020034ec:
	push {lr}
	ldr r0, .L_0200b4f8
	bl Scheduler_RemoveCallbackFar
	pop {pc}
	.2byte 0x0000
.L_0200b4f8:
	.4byte Func_020034fc
	.section .text.x0200b4fc,"ax",%progbits
	.global Func_020034fc
	.thumb_func
Func_020034fc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #192
	lsls r1, r1, #18
	ldr r2, [r1, #108]
	movs r0, #179
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	sub sp, #32
	cmp r3, #0
	bne .L_0200b610
	movs r0, #173
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	bne .L_0200b610
	movs r0, #175
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200b610
	ldr r1, [r1, #112]
	movs r0, #0
	mov r9, r1
	mov r3, r9
	adds r3, #176
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	lsls r1, r3, #16
	mov r8, r0
	mov r11, r1
	cmp r8, r3
	bcs .L_0200b610
.L_0200b556:
	movs r3, #44
	mov r2, r8
	muls r2, r3
	mov r0, r9
	adds r3, r2, #0
	adds r5, r0, r3
	ldrh r2, [r5, #6]
	adds r3, r2, #0
	cmp r3, #0
	bne .L_0200b5f4
	movs r1, #4
	ldrsh r6, [r5, r1]
	movs r3, #10
	ldrsh r2, [r5, r3]
	ldr r0, [r5]
	subs r3, r2, r6
	lsls r7, r2, #16
	lsls r3, r3, #24
	lsrs r1, r7, #16
	mov r10, r0
	adds r4, r5, #0
	lsrs r0, r3, #24
	mov r12, r1
	adds r4, #12
	cmp r0, r12
	bcs .L_0200b59e
.L_0200b58a:
	ldrh r3, [r4]
	lsls r2, r0, #1
	mov r1, sp
	strh r3, [r1, r2]
	adds r3, r0, #1
	lsls r3, r3, #24
	lsrs r0, r3, #24
	adds r4, #2
	cmp r0, r12
	bcc .L_0200b58a
.L_0200b59e:
	lsls r6, r6, #16
	lsrs r2, r7, #16
	mov lr, r6
	lsrs r6, r6, #16
	movs r0, #0
	subs r3, r2, r6
	mov r12, r2
	cmp r0, r3
	bge .L_0200b5c8
.L_0200b5b0:
	ldrh r3, [r4]
	mov r1, sp
	lsls r2, r0, #1
	strh r3, [r1, r2]
	adds r3, r0, #1
	lsls r3, r3, #24
	mov r1, r12
	lsrs r0, r3, #24
	subs r3, r1, r6
	adds r4, #2
	cmp r0, r3
	blt .L_0200b5b0
.L_0200b5c8:
	movs r2, #128
	movs r3, #128
	lsrs r4, r7, #16
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	mov r0, sp
	mov r1, r10
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #128
	lsls r3, r3, #9
	add r3, lr
	asrs r6, r3, #16
	lsrs r3, r3, #16
	cmp r3, r4
	bcc .L_0200b5ee
	movs r6, #0
.L_0200b5ee:
	ldrh r3, [r5, #8]
	strh r6, [r5, #4]
	b .L_0200b5fc
.L_0200b5f4:
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r2, r0
.L_0200b5fc:
	strh r3, [r5, #6]
	mov r3, r8
	adds r3, #1
	lsls r3, r3, #24
	lsrs r3, r3, #24
	mov r1, r11
	mov r8, r3
	lsrs r3, r1, #16
	cmp r8, r3
	bcc .L_0200b556
.L_0200b610:
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200b620,"ax",%progbits
	.global Func_02003620
	.thumb_func
Func_02003620:
	push {r5, lr}
	bl Func_02003e64
	movs r0, #0
	bl Func_02003fa4
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #38
	bl Func_02003ffc
	ldr r5, .L_0200b678
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #18
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #40
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #3
	bl Func_02003f7c
	pop {r5, pc}
.L_0200b678:
	.4byte gPartyState
	.section .text.x0200b67c,"ax",%progbits
	.global Func_0200367c
	.thumb_func
Func_0200367c:
	push {lr}
	bl Func_02003f84
	pop {pc}
	.section .text.x0200b684,"ax",%progbits
	.global Func_02003684
	.thumb_func
Func_02003684:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #8
	mov r8, r3
	bl Object_GetById
	ldr r3, [r0, #80]
	ldr r5, .L_0200b788
	ldr r3, [r3, #40]
	movs r1, #0
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r4, r3, #16
	ldrh r3, [r5, r1]
	lsrs r2, r4, #16
	cmp r2, r3
	beq .L_0200b6ca
.L_0200b6b0:
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	lsrs r2, r3, #16
	asrs r1, r3, #16
	cmp r2, #5
	bhi .L_0200b6ca
	lsls r3, r2, #1
	ldrh r3, [r5, r3]
	lsrs r2, r4, #16
	cmp r2, r3
	bne .L_0200b6b0
.L_0200b6ca:
	lsls r3, r1, #16
	lsrs r2, r3, #16
	cmp r2, #6
	bne .L_0200b6d6
	movs r0, #0
	b .L_0200b77e
.L_0200b6d6:
	ldr r6, .L_0200b78c
	lsls r2, r2, #2
	ldrsb r4, [r6, r2]
	adds r1, r4, #0
	cmp r4, #0
	bge .L_0200b6e4
	negs r1, r4
.L_0200b6e4:
	adds r3, r2, #2
	ldrsb r3, [r6, r3]
	cmp r3, #0
	bge .L_0200b6ee
	negs r3, r3
.L_0200b6ee:
	adds r3, r1, r3
	asrs r7, r3, #4
	adds r3, r2, #1
	ldrsb r1, [r6, r3]
	adds r5, r1, #0
	cmp r1, #0
	bge .L_0200b6fe
	negs r5, r1
.L_0200b6fe:
	adds r3, r2, #3
	ldrsb r2, [r6, r3]
	cmp r2, #0
	bge .L_0200b708
	negs r2, r2
.L_0200b708:
	adds r5, r5, r2
	mov r10, r5
	ldr r6, [r0, #8]
	mov r3, r10
	ldr r5, [r0, #16]
	asrs r3, r3, #4
	mov r10, r3
	lsls r3, r4, #16
	adds r6, r6, r3
	lsls r3, r1, #16
	adds r5, r5, r3
	movs r3, #164
	lsls r3, r3, #1
	add r3, r8
	ldr r3, [r3]
	asrs r6, r6, #20
	asrs r1, r3, #20
	movs r3, #166
	lsls r3, r3, #1
	add r3, r8
	ldr r3, [r3]
	lsls r2, r1, #16
	asrs r3, r3, #20
	lsls r3, r3, #16
	asrs r5, r5, #20
	lsrs r2, r2, #16
	lsrs r3, r3, #16
	adds r2, r6, r2
	adds r3, r5, r3
	str r2, [sp, #0]
	str r3, [sp, #4]
	adds r0, r6, #0
	adds r1, r5, #0
	adds r2, r7, #0
	mov r3, r10
	bl Func_02003e24
	movs r3, #255
	mov r2, r10
	str r2, [sp, #0]
	str r3, [sp, #4]
	mov r8, r3
	movs r0, #0
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r7, #0
	bl Func_02003790
	mov r2, r10
	mov r3, r8
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #2
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r7, #0
	bl Func_02003790
	movs r0, #1
.L_0200b77e:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200b788:
	.4byte Data_02004070
.L_0200b78c:
	.4byte Data_0200407c
	.section .text.x0200b790,"ax",%progbits
	.global Func_02003790
	.thumb_func
Func_02003790:
	push {r5, r6, lr}
	adds r5, r3, #0
	ldr r3, [sp, #12]
	lsls r2, r2, #7
	mov r12, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r0, r0, #1
	lsls r3, r3, #3
	adds r3, r3, r0
	ldr r0, [r4, r3]
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r0, r0, r1
	movs r1, #0
	ldr r6, [sp, #16]
	cmp r1, r12
	bcs .L_0200b7d6
.L_0200b7bc:
	lsls r3, r1, #9
	movs r2, #0
	adds r3, r0, r3
	cmp r2, r5
	bcs .L_0200b7d0
.L_0200b7c6:
	adds r2, #1
	strb r6, [r3, #2]
	adds r3, #4
	cmp r2, r5
	bcc .L_0200b7c6
.L_0200b7d0:
	adds r1, #1
	cmp r1, r12
	bcc .L_0200b7bc
.L_0200b7d6:
	pop {r5, r6, pc}
	.section .text.x0200b7d8,"ax",%progbits
	.global Func_020037d8
	.thumb_func
Func_020037d8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r5, r6, #0
	sub sp, #40
	adds r1, r6, #0
	adds r5, #12
	add r0, sp, #24
	adds r1, #16
	adds r2, r5, #0
	bl Func_02003940
	adds r4, r0, #0
	cmp r4, #0
	bne .L_0200b802
	b .L_0200b922
.L_0200b802:
	ldr r5, [r5]
	ldr r0, .L_0200b934
	str r5, [sp, #20]
	lsls r1, r5, #2
	ldrsb r2, [r0, r1]
	cmp r2, #0
	bge .L_0200b812
	negs r2, r2
.L_0200b812:
	adds r3, r1, #2
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_0200b81c
	negs r3, r3
.L_0200b81c:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #16]
	adds r3, r1, #1
	ldrsb r2, [r0, r3]
	cmp r2, #0
	bge .L_0200b82c
	negs r2, r2
.L_0200b82c:
	adds r3, r1, #3
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_0200b836
	negs r3, r3
.L_0200b836:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #12]
	ldr r3, [sp, #24]
	ldr r2, .L_0200b938
	ldr r1, .L_0200b93c
	lsls r3, r3, #2
	ldr r3, [r2, r3]
	mov r9, r1
	mov r2, r9
	ands r2, r3
	lsls r3, r3, #16
	mov r10, r3
	movs r3, #0
	str r3, [r6, #20]
	mov r11, r3
	adds r3, r4, #0
	adds r3, #34
	str r3, [sp, #8]
	ldr r1, [sp, #8]
	movs r3, #2
	strb r3, [r1]
	mov r9, r2
	ldr r3, [r4, #8]
	add r3, r9
	str r3, [r6]
	ldr r3, [r4, #16]
	add r3, r10
	str r3, [r6, #8]
	ldr r3, [r4, #12]
	str r3, [sp, #32]
.L_0200b874:
	ldr r3, [sp, #20]
	ldr r2, .L_0200b934
	lsls r3, r3, #2
	str r3, [sp, #4]
	adds r3, #1
	ldrsb r2, [r2, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #16
	adds r3, r3, r2
	ldr r2, [sp, #12]
	movs r1, #0
	mov r8, r1
	str r3, [sp, #36]
	cmp r8, r2
	bge .L_0200b8e2
.L_0200b892:
	ldr r3, .L_0200b934
	ldr r1, [sp, #4]
	add r5, sp, #28
	ldrsb r2, [r3, r1]
	ldr r3, [r6]
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [sp, #16]
	movs r7, #0
	cmp r7, r2
	bge .L_0200b8cc
.L_0200b8aa:
	adds r0, r4, #0
	add r1, sp, #28
	str r4, [sp, #0]
	bl Func_02003e2c
	ldr r4, [sp, #0]
	cmp r0, #2
	beq .L_0200b8f4
	ldr r3, [r5]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r5]
	ldr r2, [sp, #16]
	adds r7, #1
	cmp r7, r2
	blt .L_0200b8aa
.L_0200b8cc:
	add r2, sp, #28
	ldr r3, [r2, #8]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r2, #8]
	ldr r3, [sp, #12]
	movs r2, #1
	add r8, r2
	cmp r8, r3
	blt .L_0200b892
.L_0200b8e2:
	ldr r3, [r6]
	movs r1, #1
	add r3, r9
	str r3, [r6]
	ldr r3, [r6, #8]
	add r11, r1
	add r3, r10
	str r3, [r6, #8]
	b .L_0200b874
.L_0200b8f4:
	ldr r2, [sp, #8]
	movs r3, #0
	strb r3, [r2]
	mov r3, r11
	movs r0, #0
	cmp r3, #0
	beq .L_0200b924
	mov r1, r9
	ldr r3, [r4, #8]
	mov r2, r11
	muls r2, r1
	adds r3, r3, r2
	str r3, [r6]
	movs r0, #1
	ldr r3, [r4, #12]
	str r3, [r6, #4]
	mov r3, r10
	mov r2, r11
	muls r2, r3
	ldr r3, [r4, #16]
	adds r3, r3, r2
	str r3, [r6, #8]
	b .L_0200b924
.L_0200b922:
	movs r0, #0
.L_0200b924:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b934:
	.4byte Data_0200407c
.L_0200b938:
	.4byte Data_02004094
.L_0200b93c:
	.4byte 0xffff0000
	.section .text.x0200b940,"ax",%progbits
	.global Func_02003940
	.thumb_func
Func_02003940:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	str r0, [sp, #8]
	str r1, [sp, #4]
	str r2, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	ldr r3, .L_0200ba4c
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Object_GetById
	adds r7, r0, #0
	ldrh r3, [r7, #6]
	ldr r1, [sp, #8]
	lsrs r3, r3, #12
	str r3, [r1]
	movs r2, #8
	adds r5, #52
	mov r11, r2
	mov lr, r5
.L_0200b97c:
	mov r3, lr
	ldr r6, [r3]
	movs r5, #0
.L_0200b982:
	ldr r3, [r6, #80]
	ldr r2, .L_0200ba50
	ldr r3, [r3, #40]
	movs r0, #0
	ldrsh r1, [r3, r0]
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	cmp r1, r3
	bne .L_0200ba26
	ldr r0, [sp, #8]
	movs r2, #10
	ldrsh r1, [r7, r2]
	ldr r3, [r0]
	ldr r2, .L_0200ba54
	lsls r3, r3, #2
	ldr r3, [r2, r3]
	ldr r4, .L_0200ba58
	asrs r2, r3, #16
	adds r1, r1, r2
	asrs r1, r1, #4
	mov r9, r1
	movs r1, #18
	ldrsh r2, [r7, r1]
	lsls r3, r3, #16
	asrs r3, r3, #16
	adds r2, r2, r3
	asrs r2, r2, #4
	mov r8, r2
	movs r2, #10
	ldrsh r0, [r6, r2]
	lsls r2, r5, #2
	ldrsb r3, [r4, r2]
	adds r3, r0, r3
	asrs r3, r3, #4
	mov r10, r3
	movs r3, #18
	ldrsh r1, [r6, r3]
	adds r3, r2, #1
	ldrsb r3, [r4, r3]
	adds r3, r1, r3
	asrs r3, r3, #4
	mov r12, r3
	adds r3, r2, #2
	ldrsb r3, [r4, r3]
	adds r2, #3
	adds r0, r0, r3
	ldrsb r3, [r4, r2]
	asrs r0, r0, #4
	adds r1, r1, r3
	asrs r1, r1, #4
	cmp r10, r9
	bgt .L_0200ba26
	cmp r9, r0
	bge .L_0200ba26
	cmp r12, r8
	bgt .L_0200ba26
	cmp r8, r1
	bge .L_0200ba26
	ldr r0, [sp, #0]
	movs r3, #1
	ands r3, r5
	str r5, [r0]
	cmp r3, #0
	beq .L_0200ba14
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r10, r3
	beq .L_0200ba26
	ldr r2, [sp, #4]
	mov r1, r11
	str r1, [r2]
	adds r0, r6, #0
	b .L_0200ba3c
.L_0200ba14:
	ldr r3, [r7, #16]
	asrs r3, r3, #20
	cmp r12, r3
	beq .L_0200ba26
	ldr r0, [sp, #4]
	mov r3, r11
	str r3, [r0]
	adds r0, r6, #0
	b .L_0200ba3c
.L_0200ba26:
	adds r5, #1
	cmp r5, #5
	bls .L_0200b982
	movs r2, #1
	add r11, r2
	movs r1, #4
	mov r3, r11
	add lr, r1
	cmp r3, #63
	bls .L_0200b97c
	movs r0, #0
.L_0200ba3c:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ba4c:
	.4byte gPartyState
.L_0200ba50:
	.4byte Data_02004070
.L_0200ba54:
	.4byte Data_02004094
.L_0200ba58:
	.4byte Data_0200407c
	.section .text.x0200ba5c,"ax",%progbits
	.global Func_02003a5c
	.thumb_func
Func_02003a5c:
	sub sp, #16
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #56
	str r0, [sp, #88]
	str r1, [sp, #92]
	str r2, [sp, #96]
	str r3, [sp, #100]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r0, #133
	str r3, [sp, #28]
	ldr r3, .L_0200bcfc
	lsls r0, r0, #2
	adds r0, r0, r3
	mov r10, r0
	ldr r0, [r0]
	bl Object_GetById
	mov r8, r0
	ldr r0, [sp, #104]
	bl Object_GetById
	mov r3, r8
	ldr r3, [r3, #48]
	mov r4, r8
	str r3, [sp, #20]
	adds r6, r0, #0
	ldr r4, [r4, #52]
	mov r0, sp
	adds r0, #32
	str r0, [sp, #12]
	str r4, [sp, #16]
	ldr r2, [sp, #100]
	ldr r3, [r6, #8]
	movs r1, #0
	str r3, [r0]
	mov r9, r1
	ldr r3, [r6, #16]
	mov r1, sp
	adds r1, #44
	str r3, [r0, #8]
	ldr r5, .L_0200bd00
	str r1, [sp, #8]
	lsls r7, r2, #2
	ldrsb r1, [r5, r7]
	ldr r3, [r6, #8]
	lsls r2, r1, #16
	adds r3, r3, r2
	ldr r2, [sp, #8]
	asrs r3, r3, #20
	str r3, [r2]
	mov lr, r3
	adds r3, r7, #1
	ldrsb r4, [r5, r3]
	ldr r3, [r6, #16]
	ldr r0, [sp, #8]
	lsls r2, r4, #16
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [r0, #8]
	adds r0, r1, #0
	mov r12, r3
	cmp r0, #0
	bge .L_0200baec
	negs r0, r0
.L_0200baec:
	adds r3, r7, #2
	ldrsb r1, [r5, r3]
	cmp r1, #0
	bge .L_0200baf6
	negs r1, r1
.L_0200baf6:
	adds r3, r0, r1
	asrs r3, r3, #4
	adds r1, r4, #0
	str r3, [sp, #24]
	cmp r1, #0
	bge .L_0200bb04
	negs r1, r1
.L_0200bb04:
	adds r3, r7, #3
	ldrsb r2, [r5, r3]
	cmp r2, #0
	bge .L_0200bb0e
	negs r2, r2
.L_0200bb0e:
	adds r3, r1, r2
	asrs r3, r3, #4
	str r3, [sp, #0]
	mov r11, r3
	movs r3, #0
	str r3, [sp, #4]
	mov r1, lr
	mov r2, r12
	ldr r3, [sp, #24]
	movs r0, #0
	bl Func_02003790
	mov r1, r10
	movs r2, #200
	ldr r0, [r1]
	lsls r2, r2, #5
	movs r1, #128
	lsls r1, r1, #8
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	mov r2, r10
	ldr r0, [r2]
	movs r1, #8
	bl Object_SetModeById
	movs r0, #15
	bl WaitFrames
	ldr r4, [sp, #12]
	ldr r1, [sp, #88]
	ldr r3, [r4]
	ldr r2, [sp, #96]
	subs r1, r1, r3
	ldr r3, [r4, #8]
	asrs r1, r1, #17
	subs r2, r2, r3
	mov r3, r10
	asrs r2, r2, #17
	ldr r0, [r3]
	bl ObjectMotion_OffsetPositionAndResetMotion
	mov r4, r10
	ldr r0, [r4]
	bl Object_GetById
	ldr r3, .L_0200bd04
	str r3, [r0, #108]
	movs r0, #4
	bl WaitFrames
	movs r1, #2
	adds r0, r6, #0
	bl Func_02003de4
	movs r0, #239
	bl Func_02003ffc
	movs r2, #200
	movs r1, #128
	lsls r2, r2, #5
	ldr r0, [sp, #104]
	lsls r1, r1, #8
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	adds r0, r6, #0
	ldr r1, [sp, #88]
	ldr r2, [sp, #92]
	ldr r3, [sp, #96]
	bl Func_02003e0c
	ldr r3, .L_0200bcfc
	movs r0, #133
	lsls r0, r0, #2
	adds r5, r3, r0
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #152
	movs r2, #200
	lsls r1, r1, #7
	lsls r2, r2, #5
	ldr r0, [r5]
	adds r1, #204
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	ldr r2, .L_0200bd08
	mov r1, r9
	lsls r3, r1, #2
	ldr r2, [r2, r3]
	ldr r0, [r5]
	lsls r2, r2, #16
	asrs r1, r2, #31
	asrs r2, r2, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	ldr r3, [sp, #108]
	cmp r3, #0
	beq .L_0200bbe4
	mov lr, r3
	.2byte 0xf800
.L_0200bbe4:
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #1
	ldr r0, [r5]
	bl Object_SetModeById
	mov r3, r8
	movs r2, #0
	str r2, [r3, #108]
	ldr r4, [sp, #20]
	movs r5, #255
	str r4, [r3, #48]
	ldr r0, [sp, #16]
	str r0, [r3, #52]
	adds r0, r6, #0
	bl Func_02003e14
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02003ffc
	movs r0, #213
	bl Func_02003ffc
	ldr r2, [r6, #12]
	ldr r1, [sp, #88]
	ldr r3, [sp, #96]
	adds r0, r6, #0
	bl Object_SetPositionAndResetMotion
	adds r0, r6, #0
	movs r1, #1
	bl Func_02003de4
	ldr r1, .L_0200bd00
	ldr r0, [sp, #88]
	ldrsb r3, [r1, r7]
	adds r2, r7, #1
	lsls r3, r3, #16
	adds r0, r0, r3
	ldrsb r3, [r1, r2]
	mov r10, r1
	ldr r1, [sp, #96]
	lsls r3, r3, #16
	adds r1, r1, r3
	ldr r4, [sp, #28]
	asrs r0, r0, #20
	asrs r1, r1, #20
	str r0, [sp, #88]
	str r1, [sp, #96]
	mov r9, r2
	movs r2, #164
	lsls r2, r2, #1
	adds r3, r4, r2
	ldr r3, [r3]
	adds r2, #4
	asrs r3, r3, #20
	mov r8, r3
	adds r3, r4, r2
	ldr r6, [r3]
	mov r4, r8
	asrs r6, r6, #20
	adds r3, r4, r0
	adds r2, r6, r1
	str r3, [sp, #0]
	str r2, [sp, #4]
	mov r3, r11
	ldr r2, [sp, #24]
	bl Func_02003e24
	mov r0, r11
	ldr r1, [sp, #88]
	ldr r2, [sp, #96]
	str r0, [sp, #0]
	ldr r3, [sp, #24]
	movs r0, #0
	str r5, [sp, #4]
	bl Func_02003790
	mov r3, r11
	ldr r1, [sp, #88]
	ldr r2, [sp, #96]
	str r3, [sp, #0]
	movs r0, #2
	ldr r3, [sp, #24]
	str r5, [sp, #4]
	bl Func_02003790
	ldr r0, [sp, #12]
	mov r4, r10
	ldrsb r3, [r4, r7]
	ldr r1, [r0]
	ldr r2, [sp, #8]
	lsls r3, r3, #16
	adds r1, r1, r3
	asrs r1, r1, #20
	str r1, [r2]
	mov r3, r9
	ldrsb r2, [r4, r3]
	ldr r3, [r0, #8]
	ldr r4, [sp, #8]
	lsls r2, r2, #16
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [r4, #8]
	add r8, r1
	adds r6, r6, r3
	str r1, [sp, #0]
	str r3, [sp, #4]
	ldr r2, [sp, #24]
	mov r0, r8
	adds r1, r6, #0
	mov r3, r11
	bl Func_02003e24
	ldr r0, [sp, #8]
	mov r3, r11
	ldr r1, [r0]
	ldr r2, [r0, #8]
	movs r4, #0
	str r3, [sp, #0]
	movs r0, #2
	ldr r3, [sp, #24]
	str r4, [sp, #4]
	bl Func_02003790
	bl Func_02003fd4
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r3}
	add sp, #16
	bx r3
	.2byte 0x0000
.L_0200bcfc:
	.4byte gPartyState
.L_0200bd00:
	.4byte Data_0200407c
.L_0200bd04:
	.4byte Func_02003d0c
.L_0200bd08:
	.4byte Data_02004094
	.section .text.x0200bd0c,"ax",%progbits
	.global Func_02003d0c
	.thumb_func
Func_02003d0c:
	push {r5, r6, lr}
	adds r5, r0, #0
	ldrh r3, [r5, #6]
	movs r2, #12
	lsrs r1, r3, #12
	adds r3, r1, #2
	ands r3, r2
	lsls r1, r3, #12
	ldr r3, [r5, #8]
	sub sp, #12
	mov r6, sp
	str r3, [r6]
	ldr r3, [r5, #12]
	movs r0, #128
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	lsls r0, r0, #13
	adds r2, r6, #0
	str r3, [r6, #8]
	bl Vector_AddPolarOffsetFar
	adds r0, r6, #0
	movs r1, #1
	bl Func_02003fdc
	cmp r0, #0
	beq .L_0200bd68
	movs r4, #0
.L_0200bd44:
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	movs r2, #0
	ldrsh r1, [r3, r2]
	ldr r2, .L_0200bd90
	lsls r3, r4, #1
	ldrh r3, [r2, r3]
	cmp r1, r3
	beq .L_0200bd8c
	adds r4, #1
	cmp r4, #5
	bls .L_0200bd44
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
.L_0200bd68:
	ldr r3, [r5, #8]
	adds r0, r5, #0
	str r3, [r6]
	ldr r3, [r5, #12]
	adds r1, r6, #0
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	str r3, [r6, #8]
	bl Func_02003e2c
	cmp r0, #0
	ble .L_0200bd8c
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
.L_0200bd8c:
	add sp, #12
	pop {r5, r6, pc}
.L_0200bd90:
	.4byte Data_02004070
	.section .rodata.x0200c004,"a",%progbits
	.global Data_02004004
Data_02004004:
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_02004028
Data_02004028:
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000007
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000018
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_02004070
Data_02004070:
	.4byte 0x01030102
	.4byte 0x01260125
	.4byte 0x014b014c
	.global Data_0200407c
Data_0200407c:
	.4byte Sound_Wave29 + 0x134c
	.4byte 0x2008e0f8
	.4byte 0x0020f0e0
	.4byte 0x2008e0f8
	.4byte Sound_Wave29 + 0x134c
	.4byte 0x2008e0f8
	.global Data_02004094
Data_02004094:
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
.L_0200c0d4:
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00006666
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
.L_0200c198:
	.4byte 0x00000027
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00400000
	.4byte 0x80010000
	.4byte 0x00000004
	.4byte 0x01600000
	.4byte 0x00400000
	.4byte 0x01500000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01700000
	.4byte 0x00400000
	.4byte 0x01500000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00400000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00400000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01700000
	.4byte 0x00400000
	.4byte 0x01300000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01600000
	.4byte 0x00400000
	.4byte 0x01300000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00400000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00400000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.global Data_02004274
Data_02004274:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00ea0000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01140000
	.4byte 0x00000000
	.4byte 0x00de0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01340000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_02004300
Data_02004300:
	.4byte 0x00000027
	.4byte 0x00000003
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xfff00000
	.4byte 0x00000001
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000005
	.4byte 0xffe00000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000005
	.4byte 0x00200000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x03580000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020043f8
Data_020043f8:
	.4byte 0x00000027
	.4byte 0x00000003
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00100000
	.4byte 0x00000001
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000005
	.4byte 0x00200000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x00000001
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000005
	.4byte 0xffe00000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000029
	.4byte 0x00000098
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x03580000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020044f0
Data_020044f0:
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_02004508
Data_02004508:
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02ae0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte gHeapSlots
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte gHeapSlots
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02004554
Data_02004554:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02ae0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte gHeapSlots
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x03090000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020045a4
Data_020045a4:
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte gHeapSlots
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02ae0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020045f0
Data_020045f0:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte gHeapSlots
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02ae0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
.L_0200c640:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x03a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000050
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x03a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
.L_0200c6a4:
	.4byte 0x000a005c
	.4byte 0x00020001
	.4byte 0x005c0006
	.4byte 0x00010008
	.4byte 0x00060002
	.2byte 0xffff
.L_0200c6ba:
	.2byte 0xffff
	.global Data_020046bc
Data_020046bc:
	.4byte .L_0200c6ba
	.4byte 0x00400040
	.4byte .L_0200c6a4
	.4byte 0x000a0052
	.global Data_020046cc
Data_020046cc:
	.4byte 0xffff0000
	.4byte 0x000000d0
	.4byte 0x40000320
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020046fc
Data_020046fc:
	.4byte 0x00000083
	.4byte 0x0011c002
	.4byte 0x002090b7
	.4byte 0x00000084
	.4byte 0x0011d002
	.4byte 0x00205039
	.4byte 0x00301084
	.4byte 0x00000085
	.4byte 0x0011e002
	.4byte 0x00250002
	.4byte 0x0030700b
	.4byte 0x00451002
	.4byte 0x00000086
	.4byte 0x00a1f002
	.4byte 0x00b080b7
	.4byte 0x00000087
	.4byte 0x00120002
	.4byte 0x0020609c
	.4byte 0x00304087
	.4byte 0x00403087
	.4byte 0x00501088
	.4byte 0x000001ff
	.global Data_02004754
Data_02004754:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200476c
Data_0200476c:
	.4byte 0xffff0049
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00018000
	.4byte 0xffff004d
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x0002a000
	.4byte 0xffff004d
	.4byte .L_0200c640
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x03a80000
	.4byte 0x00010000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x03580000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020047e4
Data_020047e4:
	.4byte 0xffff0047
	.4byte .L_0200c0d4
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00004000
	.4byte 0xffff018f
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0102
	.4byte 0x00000007
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x008c0000
	.4byte 0x00024000
	.4byte 0xffff0192
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0192
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0xffff0192
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0xffff0192
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0xffff0192
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff0192
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff0192
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff0192
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff0190
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00003000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200494c
Data_0200494c:
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00004000
	.4byte 0xffff0049
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00012000
	.4byte 0xffff0050
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00018000
	.4byte 0xffff012f
	.4byte 0x00000007
	.4byte 0x01460000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020049c4
Data_020049c4:
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0001c000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00014000
	.4byte 0xffff0195
	.4byte .L_0200c198
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0xffff0120
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff0120
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004a84
Data_02004a84:
	.4byte 0xffff004b
	.4byte 0x00000002
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff00a2
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00004000
	.4byte 0xffff00a3
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00ca0000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x010a0000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004b14
Data_02004b14:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004b20
Data_02004b20:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_02002780
	.4byte 0x00000602
	.4byte 0xffff0010
	.4byte Func_0200367c
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000218c
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_02001bf4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000218f
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000021d0
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002190
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002192
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002193
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte Func_02000e38
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004bb0
Data_02004bb0:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_0200267c
	.4byte 0x00004602
	.4byte 0xffff000a
	.4byte Func_0200208c
	.4byte 0x00004602
	.4byte 0xffff000b
	.4byte Func_0200367c
	.4byte 0x00004602
	.4byte 0x0a2c001e
	.4byte Func_020000d8
	.4byte 0x00000002
	.4byte 0x0aa20014
	.4byte Func_02001db0
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte Func_020024f0
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte Func_0200253c
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002196
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_02000210
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_0200030c
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte Func_02003620
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002199
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte Func_020002ac
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte Func_02000380
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte Func_0200040c
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000021f8
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_020025a4
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_020025b0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004ca0
Data_02004ca0:
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
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000219a
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000219b
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000021c5
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000219f
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000021a0
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte Func_020006f4
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte Func_020020b4
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte Func_020020d0
	.4byte 0x50008905
	.4byte 0xffff000a
	.4byte Func_020025bc
	.4byte 0x50008905
	.4byte 0xffff000b
	.4byte Func_020025dc
	.4byte 0x50008905
	.4byte 0xffff000c
	.4byte Func_020025fc
	.4byte 0x50008905
	.4byte 0xffff000d
	.4byte Func_0200261c
	.4byte 0x50008905
	.4byte 0xffff000e
	.4byte Func_0200263c
	.4byte 0x50008905
	.4byte 0xffff000f
	.4byte Func_0200265c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004d84
Data_02004d84:
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x0000ce01
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00004602
	.4byte 0x02120015
	.4byte Func_02002430
	.4byte 0x00000008
	.4byte 0x02120000
	.4byte Func_02002144
	.4byte 0x10008c15
	.4byte 0x0212000c
	.4byte Func_02002144
	.4byte 0x10008c15
	.4byte 0x0212000d
	.4byte Func_02002144
	.4byte 0x00000009
	.4byte 0x02120000
	.4byte Func_020021d0
	.4byte 0x00008c15
	.4byte 0x0212000c
	.4byte Func_020021d0
	.4byte 0x00008c15
	.4byte 0x0212000d
	.4byte Func_020021d0
	.4byte 0x00001815
	.4byte 0x0213000e
	.4byte Func_020024c8
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000440
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_020004f4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_02000520
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000021ab
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000021ac
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte Func_02000568
	.4byte 0x00000002
	.4byte 0x02150014
	.4byte Func_02001f74
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004e5c
Data_02004e5c:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte Func_0200274c
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte Func_0200274c
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000021ad
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000021af
	.4byte 0x00000000
	.4byte 0x0aa50009
	.4byte 0x000021d7
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000021ea
	.4byte 0x00008d15
	.4byte 0x02170009
	.4byte Func_020014e0
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte Func_02001a68
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_02001c38
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_02001c38
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte Func_020020b4
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte Func_020020d0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
