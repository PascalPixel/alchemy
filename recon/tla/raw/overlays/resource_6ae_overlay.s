.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {r5, r6, lr}
	adds r6, r0, #0
	adds r5, r6, #0
	adds r5, #98
	ldrb r2, [r5]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0200804e
	adds r3, #255
	strb r3, [r5]
	b .L_02008064
.L_0200804e:
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	adds r3, #60
	strb r3, [r5]
	bl Random16Far
	strh r0, [r6, #6]
.L_02008064:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008068,"ax",%progbits
	.global Func_02000068
	.thumb_func
Func_02000068:
	push {lr}
	ldr r3, .L_02008088
	movs r2, #1
	ldr r3, [r3]
	lsrs r3, r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02008080
	movs r1, #15
	bl Object_SetPartAttribute
	b .L_02008086
.L_02008080:
	movs r1, #0
	bl Object_SetPartAttribute
.L_02008086:
	pop {pc}
.L_02008088:
	.4byte Data_0300122c
	.section .text.x0200808c,"ax",%progbits
	.global Func_0200008c
	.thumb_func
Func_0200008c:
	push {r5, r6, lr}
	adds r5, r0, #0
	ldr r3, [r5, #24]
	movs r2, #128
	lsls r2, r2, #9
	cmp r3, r2
	ble .L_020080a8
	ldr r2, .L_020080dc
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r2, .L_020080e0
	ldr r3, [r5, #28]
	adds r3, r3, r2
	str r3, [r5, #28]
.L_020080a8:
	adds r6, r5, #0
	adds r6, #98
	ldrb r2, [r6]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_020080ba
	adds r3, #255
	strb r3, [r6]
	b .L_020080da
.L_020080ba:
	bl Random16Far
	lsls r3, r0, #2
	ldrb r2, [r6]
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	adds r2, r2, r3
	movs r3, #144
	lsls r3, r3, #9
	adds r2, #40
	strb r2, [r6]
	str r3, [r5, #24]
	movs r3, #136
	lsls r3, r3, #9
	str r3, [r5, #28]
.L_020080da:
	pop {r5, r6, pc}
.L_020080dc:
	.4byte 0xfffff800
.L_020080e0:
	.4byte 0xfffffc00
	.section .text.x020080e4,"ax",%progbits
	.global Func_020000e4
	.thumb_func
Func_020000e4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r7, r0, #0
	movs r0, #8
	bl Object_GetById
	adds r5, r7, #0
	adds r5, #100
	ldrh r6, [r5]
	mov r8, r0
	adds r0, r6, #0
	bl Math_Cosine
	movs r1, #98
	adds r1, r1, r7
	ldrb r2, [r1]
	ldr r3, [r7, #48]
	mov r10, r1
	adds r3, r3, r2
	mov r1, r8
	adds r3, #6
	adds r2, r3, #0
	muls r2, r0
	ldr r3, [r1, #8]
	adds r0, r6, #0
	adds r3, r3, r2
	str r3, [r7, #8]
	bl Math_Sine
	mov r2, r10
	ldrb r3, [r2]
	mov r1, r8
	adds r3, #4
	adds r2, r3, #0
	muls r2, r0
	ldr r3, [r1, #16]
	adds r3, r3, r2
	ldr r2, [r7, #8]
	str r3, [r7, #16]
	str r2, [r7, #56]
	str r3, [r7, #64]
	ldr r2, .L_0200814c
	ldrh r3, [r5]
	adds r3, r3, r2
	strh r3, [r5]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200814c:
	.4byte 0xfffff800
	.section .text.x02008164,"ax",%progbits
	.global Func_02000164
	.thumb_func
Func_02000164:
	push {lr}
	ldr r3, .L_02008190
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #2
	bne .L_0200817c
	ldr r0, .L_02008194
	b .L_0200818e
.L_0200817c:
	subs r3, r2, #3
	movs r2, #128
	lsls r3, r3, #16
	lsls r2, r2, #10
	cmp r3, r2
	bhi .L_0200818c
	ldr r0, .L_02008198
	b .L_0200818e
.L_0200818c:
	ldr r0, .L_0200819c
.L_0200818e:
	pop {pc}
.L_02008190:
	.4byte gPartyState
.L_02008194:
	.4byte Data_0200644c
.L_02008198:
	.4byte Data_020066bc
.L_0200819c:
	.4byte Data_0200626c
	.section .text.x020081a0,"ax",%progbits
	.global Func_020001a0
	.thumb_func
Func_020001a0:
	push {lr}
	bl Func_02004fac
	bl Func_020057cc
	ldr r3, .L_020081cc
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	movs r0, #1
	bl Func_02005744
	movs r0, #40
	bl Battle_WaitMode0
	bl Func_02005010
	pop {pc}
	.2byte 0x0000
.L_020081cc:
	.4byte gPartyState
	.section .text.x020081d8,"ax",%progbits
	.global Func_020001d8
	.thumb_func
Func_020001d8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r2, .L_020082f4
	ldr r0, .L_020082f8
	ldr r1, .L_020082fc
	ldr r3, .L_02008300
	bl Func_02004ef0
	ldr r3, .L_02008304
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r5, #10
.L_02008206:
	adds r0, r5, #0
	bl Object_GetById
	adds r7, r0, #0
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #128
	orrs r2, r3
	strb r2, [r0]
	movs r1, #5
	adds r0, r7, #0
	adds r5, #1
	bl Animation_ApplyChildValues
	cmp r5, #16
	ble .L_02008206
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #60
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020082ec
	movs r1, #128
	movs r2, #132
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200567c
	movs r1, #128
	ldr r2, .L_02008308
	lsls r1, r1, #18
	movs r0, #9
	bl Func_0200567c
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	movs r0, #8
	bl ObjectMotion_SetActionVariant
	movs r0, #8
	bl Object_GetById
	adds r7, r0, #0
	adds r3, r7, #0
	movs r5, #224
	adds r3, #85
	movs r6, #4
	lsls r5, r5, #15
	movs r2, #0
	strb r6, [r3]
	movs r1, #5
	str r5, [r7, #12]
	mov r10, r2
	bl Animation_ApplyChildValues
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r2, r7, #0
	adds r2, #35
	ldrb r3, [r2]
	movs r1, #32
	mov r8, r1
	mov r1, r8
	orrs r3, r1
	strb r3, [r2]
	movs r1, #2
	movs r0, #9
	bl ObjectMotion_SetActionVariant
	movs r0, #9
	bl Object_GetById
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #85
	strb r6, [r3]
	movs r3, #166
	lsls r3, r3, #9
	adds r3, #204
	str r3, [r7, #24]
	str r3, [r7, #28]
	movs r1, #5
	str r5, [r7, #12]
	bl Animation_ApplyChildValues
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r1, r7, #0
	adds r1, #35
	ldrb r3, [r1]
	mov r2, r8
	orrs r3, r2
	movs r2, #2
	orrs r3, r2
	strb r3, [r1]
	adds r0, r7, #0
	movs r1, #1
	bl Animation_SetStateFlags
	ldr r3, .L_0200830c
	mov r1, r10
	str r1, [r3]
	movs r1, #144
	ldr r0, .L_02008310
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
.L_020082ec:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_020082f4:
	.4byte Data_02005ae0
.L_020082f8:
	.4byte Data_02005aa4
.L_020082fc:
	.4byte Data_02005ab4
.L_02008300:
	.4byte Data_02005b0c
.L_02008304:
	.4byte gPartyState
.L_02008308:
	.4byte 0x010f0000
.L_0200830c:
	.4byte gOverlayArea + 0x6b60
.L_02008310:
	.4byte Func_020004a4
	.section .text.x02008314,"ax",%progbits
	.global Func_02000314
	.thumb_func
Func_02000314:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	bl Func_020001d8
	movs r0, #137
	lsls r0, r0, #1
	bl GameFlag_SetBit
	ldr r5, .L_02008360
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #5
	bne .L_02008364
	ldr r3, .L_02008358
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200835c
	subs r2, #2
	strh r3, [r2]
	bl Func_02003ff4
	b .L_02008446
.L_02008358:
	.4byte 0x00000c08
.L_0200835c:
	.4byte 0x00003f10
.L_02008360:
	.4byte gPartyState
.L_02008364:
	cmp r3, #4
	bne .L_0200840e
	bl Func_0200560c
	movs r0, #0
	bl Func_02005784
	movs r0, #20
	movs r1, #8
	bl Object_SetModeById
	movs r0, #21
	movs r1, #8
	bl Object_SetModeById
	movs r0, #22
	movs r1, #7
	bl Object_SetModeById
	movs r1, #249
	movs r2, #160
	movs r0, #19
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200567c
	movs r2, #160
	movs r0, #1
	ldr r1, .L_0200844c
	lsls r2, r2, #17
	bl Func_0200567c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	movs r1, #253
	movs r2, #140
	ldr r0, [r3]
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200567c
	movs r0, #1
	bl WaitFrames
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_0200573c
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #192
	movs r0, #128
	movs r2, #156
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_0200558c
	movs r0, #1
	bl WaitFrames
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #40
	bl Battle_WaitMode0
	bl Func_02003658
	bl Func_02005614
	b .L_02008446
.L_0200840e:
	cmp r3, #3
	bne .L_0200843e
	movs r0, #20
	movs r1, #8
	bl Object_SetModeById
	movs r0, #21
	movs r1, #8
	bl Object_SetModeById
	movs r0, #22
	movs r1, #7
	bl Object_SetModeById
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #61
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008446
	bl Func_020029ac
	b .L_02008446
.L_0200843e:
	cmp r3, #2
	bne .L_02008446
	bl Func_02001f10
.L_02008446:
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
.L_0200844c:
	.4byte 0x021a0000
	.section .text.x02008450,"ax",%progbits
	.global Func_02000450
	.thumb_func
Func_02000450:
	push {lr}
	ldr r3, .L_02008498
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_02008476
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #61
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008476
	movs r0, #1
	bl Func_020057d4
.L_02008476:
	ldr r3, .L_02008498
	movs r2, #253
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldr r2, .L_0200849c
	ldr r3, .L_020084a0
	movs r1, #160
	subs r3, r3, r2
	adds r0, r0, r3
	lsls r1, r1, #19
	bl Func_020055dc
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_02008498:
	.4byte gPartyState
.L_0200849c:
	.4byte 0x0000010e
.L_020084a0:
	.4byte 0x00000121
	.section .text.x020084a4,"ax",%progbits
	.global Func_020004a4
	.thumb_func
Func_020004a4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #8
	bl Object_GetById
	mov r8, r0
	bl Random16Far
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	mov r0, r8
	lsls r6, r3, #16
	cmp r0, #0
	beq .L_0200858a
	ldr r3, .L_02008574
	ldr r5, [r3]
	movs r3, #15
	ands r5, r3
	cmp r5, #0
	bne .L_0200858a
	ldr r2, [r0, #12]
	ldr r1, [r0, #8]
	movs r3, #128
	lsls r3, r3, #12
	adds r2, r2, r6
	adds r1, r1, r3
	adds r2, r2, r3
	ldr r3, [r0, #16]
	movs r0, #168
	lsls r0, r0, #2
	bl Func_02005574
	movs r1, #192
	adds r7, r0, #0
	lsls r1, r1, #11
	adds r0, r6, #0
	bl Engine_MathDivide
	lsls r6, r0, #16
	cmp r7, #0
	beq .L_0200858a
	ldr r1, [r7, #80]
	adds r0, r7, #0
	mov r10, r1
	ldr r1, .L_02008578
	bl Func_0200556c
	movs r1, #10
	adds r0, r7, #0
	bl Object_SetPartAttribute
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	bl Random16Far
	ldr r3, .L_0200857c
	adds r2, r7, #0
	ands r3, r0
	adds r2, #100
	strh r3, [r2]
	adds r3, r7, #0
	adds r3, #102
	strh r5, [r3]
	ldr r3, .L_02008580
	ldr r0, .L_02008570
	str r3, [r7, #108]
	ldr r3, .L_02008584
	mov r1, r8
	ands r6, r3
	mov r9, r0
	str r1, [r7, #104]
	asrs r0, r6, #4
	bl Math_Sine
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	asrs r3, r3, #16
	str r3, [r7, #48]
	mov r2, r9
	mov r3, r10
	strb r2, [r3, #26]
	mov r0, r8
	ldr r3, [r0, #80]
	movs r2, #12
	ldrb r3, [r3, #9]
	mov r0, r10
	ands r2, r3
	mov r3, r10
	ldrb r1, [r3, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	b .L_02008588
	.2byte 0x0000
.L_02008570:
	.4byte 0x00000000
.L_02008574:
	.4byte gOverlayArea + 0x6b60
.L_02008578:
	.4byte Data_02006ad0
.L_0200857c:
	.4byte 0x0ffff000
.L_02008580:
	.4byte Func_020005a0
.L_02008584:
	.4byte 0x000fffff
.L_02008588:
	strb r3, [r0, #9]
.L_0200858a:
	ldr r2, .L_0200859c
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200859c:
	.4byte gOverlayArea + 0x6b60
	.section .text.x020085a0,"ax",%progbits
	.global Func_020005a0
	.thumb_func
Func_020005a0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #100
	ldrh r6, [r7]
	ldr r1, [r5, #104]
	adds r0, r6, #0
	mov r8, r1
	bl Math_Cosine
	ldr r3, [r5, #48]
	mov r1, r8
	adds r3, #28
	adds r2, r3, #0
	muls r2, r0
	ldr r3, [r1, #8]
	adds r0, r6, #0
	adds r3, r3, r2
	str r3, [r5, #8]
	bl Math_Sine
	movs r2, #132
	ldr r3, [r5, #8]
	lsls r0, r0, #4
	lsls r2, r2, #17
	adds r0, r0, r2
	str r0, [r5, #16]
	str r3, [r5, #56]
	str r0, [r5, #64]
	ldr r1, [r5, #80]
	cmp r0, r2
	bge .L_020085f0
	ldrb r3, [r1, #9]
	movs r2, #13
	negs r2, r2
	ands r2, r3
	movs r3, #8
	b .L_020085fa
.L_020085f0:
	ldrb r3, [r1, #9]
	movs r2, #13
	negs r2, r2
	ands r2, r3
	movs r3, #4
.L_020085fa:
	orrs r2, r3
	strb r2, [r1, #9]
	ldrh r3, [r7]
	ldr r1, .L_0200860c
	adds r3, r3, r1
	strh r3, [r7]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200860c:
	.4byte 0xfffffe00
	.section .text.x02008610,"ax",%progbits
	.global Func_02000610
	.thumb_func
Func_02000610:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl Object_GetById
	adds r6, r0, #0
	adds r0, r5, #0
	bl ObjectMotion_EnableActionAndResetMotion
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	adds r2, r6, #0
	lsrs r3, r3, #16
	adds r3, #20
	adds r2, #98
	strb r3, [r2]
	ldr r3, .L_0200863c
	str r3, [r6, #108]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200863c:
	.4byte Func_02000038
	.section .text.x02008640,"ax",%progbits
	.global Func_02000640
	.thumb_func
Func_02000640:
	push {r5, lr}
	adds r5, r0, #0
	bl Object_GetById
	movs r3, #0
	movs r1, #192
	str r3, [r0, #108]
	lsls r1, r1, #8
	adds r0, r5, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200865c,"ax",%progbits
	.global Func_0200065c
	.thumb_func
Func_0200065c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r5, .L_02008a60
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	ldr r3, .L_02008a64
	adds r7, r0, #0
	mov r8, r3
	bl Func_0200560c
	movs r0, #0
	bl Func_02005784
	movs r0, #18
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r1, #128
	movs r2, #138
	lsls r2, r2, #17
	lsls r1, r1, #18
	movs r0, #18
	bl Func_0200567c
	movs r0, #1
	bl WaitFrames
	movs r0, #6
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #5
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #19
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #2
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r1, #1
	movs r0, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #0
	bl Func_020057dc
	ldr r0, .L_02008a68
	bl Func_020056dc
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	ldr r0, [r5]
	bl Func_02005714
	bl Func_0200573c
	movs r6, #0
	adds r0, #85
	strb r6, [r0]
	movs r1, #200
	movs r0, #204
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_02005724
	movs r0, #128
	movs r1, #1
	movs r2, #152
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_02008a6c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #252
	movs r2, #160
	ldr r0, [r5]
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	ldr r0, [r5]
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	ldr r1, [r5]
	movs r0, #0
	bl Func_0200568c
	movs r0, #1
	bl WaitFrames
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #0
	ldr r1, .L_02008a6c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #130
	movs r2, #160
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #1
	bl Func_0200568c
	movs r0, #1
	bl WaitFrames
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #1
	ldr r1, .L_02008a6c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #134
	movs r2, #160
	movs r0, #1
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #1
	movs r1, #0
	bl Func_020056f4
	movs r1, #1
	movs r0, #3
	bl Func_0200568c
	movs r0, #1
	bl WaitFrames
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #3
	ldr r1, .L_02008a6c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #136
	movs r2, #164
	lsls r2, r2, #1
	movs r0, #3
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #3
	movs r1, #4
	bl Object_SetModeById
	movs r1, #0
	movs r0, #3
	bl Func_020056f4
	ldr r0, [r5]
	bl ObjectMotion_EnableActionAndResetMotion
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r2, r7, #0
	lsrs r3, r3, #16
	adds r2, #98
	adds r3, #20
	strb r3, [r2]
	ldr r3, .L_02008a70
	movs r0, #1
	str r3, [r7, #108]
	bl Func_02000610
	movs r0, #3
	bl Func_02000610
	ldr r1, [r5]
	movs r0, #5
	bl Func_0200568c
	ldr r1, [r5]
	movs r0, #6
	bl Func_0200568c
	ldr r1, [r5]
	movs r0, #7
	bl Func_0200568c
	ldr r1, [r5]
	movs r0, #2
	bl Func_0200568c
	ldr r1, [r5]
	movs r0, #19
	bl Func_0200568c
	movs r0, #1
	bl WaitFrames
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_02008a6c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_02008a6c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_02008a6c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #2
	ldr r1, .L_02008a6c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #19
	ldr r1, .L_02008a6c
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_02008a74
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02008a78
	movs r0, #6
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02008a7c
	movs r0, #7
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02008a80
	movs r0, #19
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02008a84
	movs r0, #2
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #16
	bl Func_0200563c
	movs r0, #15
	bl Func_0200563c
	movs r0, #14
	bl Func_0200563c
	movs r0, #13
	bl Func_0200563c
	movs r0, #12
	bl Func_0200563c
	movs r0, #11
	bl Func_0200563c
	movs r0, #10
	bl Func_0200563c
	movs r0, #5
	bl Func_02000610
	movs r0, #6
	bl Func_02000610
	movs r0, #7
	bl Func_02000610
	movs r0, #2
	bl Func_02000610
	movs r0, #19
	bl Func_02000610
	movs r0, #80
	bl Battle_WaitMode0
	movs r0, #6
	bl Object_GetById
	movs r1, #0
	str r6, [r0, #108]
	movs r0, #6
	bl Func_02005704
	movs r1, #0
	movs r0, #6
	bl Func_020056f4
	movs r0, #5
	bl Object_GetById
	movs r1, #128
	str r6, [r0, #108]
	lsls r1, r1, #8
	movs r0, #5
	bl Func_02005704
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #7
	bl Object_GetById
	movs r1, #0
	str r6, [r0, #108]
	movs r0, #7
	bl Func_02005704
	movs r2, #10
	movs r0, #7
	movs r1, #4
	bl ObjectMotion_Launch
	movs r1, #0
	movs r0, #7
	bl Func_020056f4
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #128
	str r6, [r0, #108]
	lsls r1, r1, #8
	ldr r0, [r5]
	bl Func_02005704
	movs r1, #3
	ldr r0, [r5]
	bl Motion_SetModeAndWaitAnimation
	movs r0, #0
	bl Func_02000640
	movs r0, #1
	bl Func_02000640
	movs r0, #19
	bl Func_02000640
	movs r0, #2
	bl Func_02000640
	movs r0, #3
	bl Func_02000640
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02005764
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #3
	bl Func_0200575c
	movs r0, #40
	bl Func_0200576c
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #252
	movs r2, #146
	lsls r2, r2, #1
	lsls r1, r1, #1
	ldr r0, [r5]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #162
	bl Func_020057dc
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #129
	lsls r1, r1, #1
	ldr r0, [r5]
	bl Func_0200571c
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #204
	movs r2, #200
	lsls r1, r1, #6
	lsls r2, r2, #5
	ldr r0, [r5]
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #252
	movs r2, #154
	lsls r2, r2, #1
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #3
	ldr r0, [r5]
	bl Motion_SetVarCbAndRefresh
	movs r0, #195
	lsls r0, r0, #1
	bl Func_020057dc
	movs r1, #129
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_0200571c
	movs r1, #252
	movs r2, #160
	lsls r1, r1, #1
	b .L_02008a88
.L_02008a60:
	.4byte gPartyState
.L_02008a64:
	.4byte 0x0500021e
.L_02008a68:
	.4byte 0x00002d8c
.L_02008a6c:
	.4byte 0x00019999
.L_02008a70:
	.4byte Func_02000038
.L_02008a74:
	.4byte Data_02005ba8
.L_02008a78:
	.4byte Data_02005bd0
.L_02008a7c:
	.4byte Data_02005bf8
.L_02008a80:
	.4byte Data_02005c48
.L_02008a84:
	.4byte Data_02005c20
.L_02008a88:
	lsls r2, r2, #1
	ldr r0, [r5]
	bl ObjectMotion_SetPositionAndCommit
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	ldr r3, .L_02008ae0
	mov r2, r8
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	ldr r0, [r5]
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	b .L_02008ae4
	.2byte 0x0000
.L_02008ae0:
	.4byte 0x00007fff
.L_02008ae4:
	bl Func_02005704
	movs r1, #130
	movs r2, #154
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	movs r2, #20
	lsls r0, r0, #8
	movs r1, #0
	bl Func_020056ec
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #0
	bl Func_02005714
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #1
	bl Func_02005714
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #40
	movs r0, #1
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #2
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	bl Func_02005704
	movs r0, #0
	movs r1, #4
	bl Object_SetModeById
	movs r1, #4
	movs r0, #1
	bl Motion_SetModeAndWaitAnimation
	movs r0, #36
	bl Func_020057dc
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r1, #192
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #1
	bl Func_02005704
	ldr r0, [r5]
	bl Func_02000610
	movs r0, #5
	bl Func_02000610
	movs r0, #6
	bl Func_02000610
	movs r0, #7
	bl Func_02000610
	movs r0, #0
	bl Func_02000610
	movs r0, #1
	bl Func_02000610
	movs r0, #3
	bl Func_02000610
	movs r0, #2
	bl Func_02000610
	movs r0, #19
	bl Func_02000610
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #19
	bl Func_02000640
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #19
	bl Func_02005714
	movs r0, #19
	movs r1, #4
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #19
	movs r1, #6
	bl ObjectMotion_Launch
	movs r1, #0
	movs r0, #19
	bl Func_020056f4
	ldr r0, [r5]
	bl Func_02000640
	movs r0, #5
	bl Func_02000640
	movs r0, #6
	bl Func_02000640
	movs r0, #7
	bl Func_02000640
	movs r0, #0
	bl Func_02000640
	movs r0, #1
	bl Func_02000640
	movs r0, #3
	bl Func_02000640
	movs r0, #2
	bl Func_02000640
	movs r0, #18
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #18
	bl Object_GetById
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #85
	strb r6, [r3]
	movs r0, #18
	bl Object_GetById
	movs r1, #128
	movs r2, #240
	movs r3, #208
	lsls r2, r2, #15
	lsls r3, r3, #16
	lsls r1, r1, #18
	bl Object_SetPositionAndResetMotion
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_02009080
	adds r1, #153
	bl Func_02005724
	movs r0, #128
	movs r1, #1
	movs r2, #216
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02005734
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl Func_020056ec
	movs r0, #128
	movs r1, #1
	movs r2, #152
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02005734
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl Func_0200571c
	movs r1, #129
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_0200571c
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl Func_0200571c
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	bl Func_0200571c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #19
	bl Func_0200571c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #0
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #5
	movs r1, #0
	bl Func_020056f4
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #0
	bl Func_02005714
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	bl Func_020056f4
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #2
	bl Func_02005714
	movs r0, #2
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	bl Func_02005704
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #1
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	bl Func_02005704
	movs r0, #160
	lsls r0, r0, #7
	movs r1, #0
	bl Func_020056f4
	movs r1, #6
	movs r2, #0
	adds r1, #255
	movs r0, #3
	bl Func_02005714
	movs r0, #3
	movs r1, #0
	bl Func_020056f4
	movs r1, #4
	movs r2, #0
	adds r1, #255
	movs r0, #1
	bl Func_02005714
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	bl Func_02005704
	movs r0, #160
	lsls r0, r0, #7
	adds r0, #1
	movs r1, #0
	bl Func_020056f4
	movs r0, #7
	movs r1, #0
	bl Func_02005704
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #7
	movs r1, #0
	bl Func_020056f4
	movs r0, #204
	movs r1, #192
	lsls r0, r0, #7
	lsls r1, r1, #4
	adds r0, #102
	adds r1, #204
	bl Func_02005724
	movs r0, #128
	movs r1, #1
	movs r2, #148
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #18
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #192
	movs r3, #138
	lsls r3, r3, #17
	lsls r2, r2, #15
	lsls r1, r1, #18
	adds r0, r7, #0
	bl Func_02005594
	movs r0, #18
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #0
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r1, #130
	movs r2, #160
	lsls r2, r2, #1
	movs r0, #0
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	bl Func_02005704
	movs r1, #9
	movs r0, #18
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r0, #5
	movs r1, #0
	bl Func_020056f4
	movs r1, #1
	movs r0, #18
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r1, #129
	movs r0, #6
	lsls r1, r1, #1
	bl Func_0200571c
	movs r0, #6
	movs r1, #0
	bl Func_020056f4
	movs r1, #9
	movs r0, #18
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r0, #0
	movs r1, #0
	bl Func_020056f4
	movs r0, #3
	movs r1, #0
	bl Func_020056f4
	movs r0, #7
	movs r1, #0
	bl Func_020056f4
	movs r2, #10
	movs r0, #6
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #6
	movs r1, #0
	bl Func_020056f4
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #5
	movs r1, #0
	bl Func_020056f4
	movs r1, #11
	movs r0, #18
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	bl Func_020056f4
	movs r1, #12
	movs r0, #18
	bl Object_SetModeById
	movs r0, #18
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #19
	bl Func_02005714
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r1, #10
	movs r0, #18
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #18
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #10
	movs r0, #18
	bl Object_SetModeById
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #18
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #6
	movs r2, #80
	adds r1, #255
	movs r0, #18
	bl Func_02005714
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #5
	movs r1, #0
	bl Func_020056f4
	movs r1, #9
	movs r0, #18
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r1, #4
	movs r2, #20
	adds r1, #255
	movs r0, #19
	bl Func_02005714
	b .L_02009084
.L_02009080:
	.4byte 0x0004cccc
.L_02009084:
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r1, #10
	movs r0, #18
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r1, #9
	movs r0, #18
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #18
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #6
	movs r0, #18
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	ldr r1, .L_02009460
	adds r0, r7, #0
	bl Func_020053e4
	movs r0, #80
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #18
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #19
	bl Func_02005714
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r1, #3
	movs r0, #18
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	ldr r0, [r5]
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #6
	movs r0, #18
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #142
	bl Func_020057dc
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #5
	bl Func_0200575c
	movs r0, #40
	bl Func_0200576c
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	movs r1, #1
	bl Func_0200575c
	movs r0, #128
	movs r1, #2
	lsls r0, r0, #9
	bl Func_0200575c
	movs r0, #40
	bl Func_0200576c
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #18
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #5
	movs r1, #0
	bl Func_020056f4
	movs r1, #4
	movs r0, #18
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #19
	bl Func_02005714
	movs r1, #0
	movs r0, #19
	bl Func_020056f4
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #13
	movs r0, #18
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #20
	movs r0, #18
	movs r1, #0
	bl Func_020056ec
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r1, #8
	movs r0, #18
	bl Object_SetModeById
	movs r0, #180
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r1, #8
	movs r2, #0
	adds r1, #255
	movs r0, #2
	bl Func_02005714
	movs r0, #2
	movs r1, #0
	bl Func_020056f4
	movs r2, #0
	movs r0, #19
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #1
	movs r0, #18
	bl Object_SetModeById
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #9
	bl Object_SetModeById
	movs r1, #6
	adds r1, #255
	movs r2, #40
	movs r0, #18
	bl Func_02005714
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #19
	bl Func_02005714
	movs r1, #0
	movs r0, #19
	bl Func_020056f4
	adds r0, r7, #0
	bl Func_020054e4
	movs r1, #1
	movs r0, #18
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r1, #4
	adds r1, #255
	movs r2, #0
	movs r0, #7
	bl Func_02005714
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #10
	movs r0, #7
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #7
	movs r1, #0
	bl Func_020056f4
	movs r1, #10
	movs r0, #18
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #11
	movs r0, #18
	bl Object_SetModeById
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #18
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #8
	movs r2, #0
	adds r1, #255
	movs r0, #1
	bl Func_02005714
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	bl Func_02005704
	movs r0, #160
	lsls r0, r0, #7
	adds r0, #1
	movs r1, #0
	bl Func_020056f4
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #3
	bl Func_02005714
	movs r0, #3
	movs r1, #4
	bl Object_SetModeById
	movs r0, #3
	movs r1, #0
	bl Func_020056f4
	movs r0, #0
	movs r1, #4
	bl Object_SetModeById
	movs r0, #0
	movs r1, #0
	bl Func_020056f4
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #0
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #7
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009464
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r2, #10
	movs r0, #6
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #6
	movs r1, #0
	bl Func_020056f4
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009490
	.2byte 0x0000
.L_02009460:
	.4byte gOverlayArea + 0x6c20
.L_02009464:
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r0, #6
	movs r1, #4
	movs r2, #10
	bl ObjectMotion_Launch
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #6
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_020056f4
.L_02009490:
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r3, .L_02009620
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	movs r1, #192
	movs r2, #0
	ldr r0, [r5]
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #11
	movs r0, #18
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r0, #6
	movs r1, #0
	bl Func_02005704
	movs r1, #10
	adds r1, #255
	movs r2, #20
	movs r0, #6
	bl Func_02005714
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #5
	bl Func_02005714
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #5
	movs r1, #0
	bl Func_020056f4
	movs r0, #18
	movs r1, #8
	bl Object_SetModeById
	movs r1, #128
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	ldr r0, [r5]
	bl ObjectMotion_ArmCallback
	movs r0, #18
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #18
	movs r1, #1
	bl Object_SetModeById
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #2
	bl Func_02005714
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #2
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #1
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #7
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009624
	ldr r0, [r5]
	bl ObjectMotion_WaitForAnimationChange
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009638
	.2byte 0x0000
.L_02009620:
	.4byte gPartyState
.L_02009624:
	movs r0, #1
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #1
	movs r1, #0
	bl Func_020056f4
.L_02009638:
	ldr r2, .L_02009a34
	movs r5, #133
	mov r8, r2
	lsls r5, r5, #2
	add r5, r8
	ldr r0, [r5]
	movs r1, #0
	bl Func_02005704
	ldr r0, [r5]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Func_02005704
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	ldr r0, [r5]
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #252
	movs r2, #154
	lsls r2, r2, #1
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #201
	bl Func_020057dc
	movs r1, #14
	movs r0, #18
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #18
	lsls r1, r1, #1
	bl Func_020056cc
	ldr r0, [r5]
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_02009a38
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #252
	movs r2, #160
	lsls r1, r1, #1
	lsls r2, r2, #1
	ldr r0, [r5]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r1, #129
	strb r3, [r0]
	lsls r1, r1, #1
	movs r0, #7
	bl Func_0200571c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	bl Func_020056f4
	movs r1, #14
	movs r0, #18
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #10
	movs r0, #18
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r1, #9
	movs r0, #18
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #40
	ldr r0, [r5]
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #7
	bl Func_020056f4
	movs r0, #143
	lsls r0, r0, #2
	bl Func_020057dc
	movs r1, #0
	movs r0, #18
	bl Func_020056cc
	movs r0, #18
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #20
	ldr r0, [r5]
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #18
	movs r1, #0
	bl Func_020056f4
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #1
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #2
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #78
	bl Func_020057dc
	ldr r3, .L_02009a3c
	ldr r1, .L_02009a40
	str r3, [r7, #108]
	movs r0, #18
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #1
	bl Func_0200575c
	movs r0, #20
	bl Func_0200576c
	movs r0, #20
	bl WaitFrames
	movs r1, #0
	movs r0, #0
	bl Func_0200575c
	movs r0, #40
	bl Func_0200576c
	movs r0, #40
	bl WaitFrames
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_0200567c
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl Func_0200567c
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_0200567c
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl Func_0200567c
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200567c
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200567c
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_0200567c
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl Func_0200567c
	movs r1, #0
	movs r2, #0
	movs r0, #3
	bl Func_0200567c
	movs r0, #1
	bl WaitFrames
	movs r0, #17
	bl Object_GetById
	movs r3, #128
	adds r7, r0, #0
	lsls r3, r3, #5
	movs r1, #128
	movs r2, #140
	str r3, [r7, #24]
	str r3, [r7, #28]
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r0, #17
	bl Func_0200567c
	movs r0, #1
	bl WaitFrames
	movs r3, #85
	adds r3, r3, r7
	movs r6, #0
	mov r10, r3
	strb r6, [r3]
	movs r1, #128
	movs r2, #192
	movs r3, #140
	lsls r3, r3, #17
	adds r0, r7, #0
	lsls r1, r1, #18
	lsls r2, r2, #14
	bl Object_SetPositionAndResetMotion
	movs r0, #1
	bl WaitFrames
	mov r2, r10
	movs r0, #128
	strb r6, [r2]
	movs r1, #2
	lsls r0, r0, #9
	bl Func_0200575c
	movs r0, #20
	bl Func_0200576c
	movs r6, #128
	movs r0, #39
	bl Func_020057dc
	lsls r6, r6, #19
	ldr r1, .L_02009a44
	movs r0, #17
	bl Object_SetActionCallbackAndRefreshById
	ldrh r2, [r6]
	movs r3, #249
	lsls r3, r3, #8
	adds r3, #255
	movs r0, #128
	ands r3, r2
	lsls r0, r0, #9
	strh r3, [r6]
	movs r1, #1
	adds r0, #3
	bl Func_0200575c
	movs r0, #40
	bl Func_0200576c
	movs r2, #204
	lsls r2, r2, #6
	adds r2, #51
	ldr r1, .L_02009a48
	movs r0, #17
	bl ObjectMotion_SetSpeedParameters
	movs r0, #138
	bl Func_020057dc
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_0200575c
	movs r0, #1
	bl Func_0200576c
	movs r0, #1
	bl WaitFrames
	movs r1, #136
	movs r2, #176
	movs r3, #140
	lsls r2, r2, #15
	lsls r3, r3, #17
	b .L_02009a4c
	.2byte 0x0000
.L_02009a34:
	.4byte gPartyState
.L_02009a38:
	.4byte 0x00019999
.L_02009a3c:
	.4byte Func_02000068
.L_02009a40:
	.4byte Data_02005c70
.L_02009a44:
	.4byte Data_02005cac
.L_02009a48:
	.4byte 0x00033333
.L_02009a4c:
	adds r0, r7, #0
	lsls r1, r1, #18
	bl Func_02005594
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	bl Func_0200575c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #3
	bl Func_0200575c
	movs r0, #10
	bl Func_0200576c
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #138
	bl Func_020057dc
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_0200575c
	movs r0, #1
	bl Func_0200576c
	movs r0, #1
	bl WaitFrames
	movs r1, #232
	movs r2, #176
	movs r3, #140
	lsls r2, r2, #15
	lsls r3, r3, #17
	adds r0, r7, #0
	lsls r1, r1, #17
	bl Func_02005594
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	bl Func_0200575c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #3
	bl Func_0200575c
	movs r0, #10
	bl Func_0200576c
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #138
	bl Func_020057dc
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_0200575c
	movs r0, #1
	bl Func_0200576c
	movs r0, #1
	bl WaitFrames
	movs r1, #144
	movs r2, #176
	movs r3, #140
	lsls r3, r3, #17
	lsls r2, r2, #15
	adds r0, r7, #0
	lsls r1, r1, #18
	bl Func_02005594
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	bl Func_0200575c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #3
	bl Func_0200575c
	movs r0, #10
	bl Func_0200576c
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r2, .L_02009bd8
	ldr r1, .L_02009bdc
	movs r0, #17
	bl ObjectMotion_SetSpeedParameters
	movs r0, #138
	bl Func_020057dc
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_0200575c
	movs r0, #1
	bl Func_0200576c
	movs r0, #1
	bl WaitFrames
	movs r1, #254
	movs r2, #208
	movs r3, #144
	lsls r2, r2, #16
	lsls r3, r3, #17
	adds r0, r7, #0
	lsls r1, r1, #17
	bl Func_02005594
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	bl Func_0200575c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #3
	bl Func_0200575c
	movs r0, #10
	bl Func_0200576c
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #138
	bl Func_020057dc
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_0200575c
	movs r0, #1
	bl Func_0200576c
	movs r0, #1
	bl WaitFrames
	ldrh r3, [r6]
	ldr r2, .L_02009bd4
	movs r1, #244
	orrs r3, r2
	movs r2, #160
	strh r3, [r6]
	movs r0, #7
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200567c
	movs r1, #252
	movs r2, #160
	ldr r0, [r5]
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200567c
	movs r1, #130
	movs r2, #160
	movs r0, #0
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200567c
	movs r1, #134
	movs r2, #160
	movs r0, #1
	lsls r1, r1, #18
	lsls r2, r2, #17
	b .L_02009be0
	.2byte 0x0000
.L_02009bd4:
	.4byte 0x00000600
.L_02009bd8:
	.4byte 0x00033333
.L_02009bdc:
	.4byte 0x00066666
.L_02009be0:
	bl Func_0200567c
	movs r1, #240
	movs r2, #164
	movs r0, #6
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200567c
	movs r1, #248
	movs r2, #164
	movs r0, #5
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200567c
	movs r1, #128
	movs r2, #164
	movs r0, #19
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200567c
	movs r1, #132
	movs r2, #164
	movs r0, #2
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200567c
	movs r1, #136
	movs r2, #164
	lsls r2, r2, #17
	lsls r1, r1, #18
	movs r0, #3
	bl Func_0200567c
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	movs r1, #1
	bl Func_0200575c
	movs r0, #128
	movs r1, #2
	lsls r0, r0, #9
	bl Func_0200575c
	movs r0, #10
	bl Func_0200576c
	movs r0, #20
	bl WaitFrames
	movs r0, #138
	bl Func_020057dc
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_0200575c
	movs r0, #1
	bl Func_0200576c
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	movs r1, #1
	bl Func_0200575c
	movs r0, #128
	movs r1, #2
	lsls r0, r0, #9
	bl Func_0200575c
	movs r0, #40
	bl Func_0200576c
	movs r0, #40
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #7
	mov r2, r10
	str r3, [r7, #72]
	movs r3, #3
	strb r3, [r2]
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #208
	bl Func_020057dc
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_020055c4
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r1, r1
	negs r0, r0
	bl Func_020055c4
	bl Func_020055cc
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #17
	bl ObjectMotion_SetVariantCallback
	movs r0, #148
	bl Func_020057dc
	movs r0, #80
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	bl Func_020056f4
	movs r1, #4
	movs r2, #0
	adds r1, #255
	movs r0, #7
	bl Func_02005714
	movs r0, #7
	movs r1, #0
	bl Func_020056f4
	movs r1, #4
	movs r2, #0
	adds r1, #255
	movs r0, #1
	bl Func_02005714
	movs r0, #1
	movs r1, #0
	bl Func_020056f4
	movs r0, #3
	movs r1, #3
	bl Object_SetModeById
	movs r0, #3
	movs r1, #0
	bl Func_020056f4
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #2
	movs r1, #0
	bl Func_020056f4
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_02009e88
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_02009e88
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #0
	ldr r1, .L_02009e88
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #1
	ldr r1, .L_02009e88
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_02009e88
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_02009e88
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #2
	ldr r1, .L_02009e88
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #3
	ldr r1, .L_02009e88
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_02009e8c
	movs r0, #7
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r0, [r5]
	ldr r1, .L_02009e90
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009e94
	movs r0, #0
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009e98
	movs r0, #6
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009e9c
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009ea0
	movs r0, #2
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009ea4
	movs r0, #3
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009ea8
	movs r0, #1
	bl Object_SetActionCallbackAndRefreshById
	movs r1, #2
	movs r2, #40
	adds r1, #255
	movs r0, #19
	bl Func_02005714
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r1, #129
	movs r0, #19
	lsls r1, r1, #1
	bl Func_0200571c
	movs r1, #160
	movs r0, #19
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	bl Func_02005704
	movs r2, #10
	movs r0, #1
	movs r1, #6
	bl ObjectMotion_Launch
	movs r0, #1
	movs r1, #0
	bl Func_020056f4
	movs r0, #19
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #19
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r3, #166
	lsls r3, r3, #1
	adds r3, #255
	add r8, r3
	mov r2, r8
	movs r3, #2
	strb r3, [r2]
	ldr r0, .L_02009eac
	movs r1, #2
	bl Party_SetFields1eeAnd1f0
	movs r0, #102
	movs r1, #1
	bl Func_0200574c
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02009e88:
	.4byte 0x00019999
.L_02009e8c:
	.4byte Data_02005df8
.L_02009e90:
	.4byte Data_02005ce8
.L_02009e94:
	.4byte Data_02005d2c
.L_02009e98:
	.4byte Data_02005db4
.L_02009e9c:
	.4byte Data_02005d70
.L_02009ea0:
	.4byte Data_02005e3c
.L_02009ea4:
	.4byte Data_02005e80
.L_02009ea8:
	.4byte Data_02005ec4
.L_02009eac:
	.4byte 0x00000129
	.section .text.x02009eb0,"ax",%progbits
	.global Func_02001eb0
	.thumb_func
Func_02001eb0:
	push {lr}
	ldr r3, .L_02009edc
	ldr r2, .L_02009ee0
	ldr r3, [r3]
	ldr r2, [r2]
	lsrs r3, r2
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02009ecc
	movs r1, #15
	bl Object_SetPartAttribute
	b .L_02009ed2
.L_02009ecc:
	movs r1, #0
	bl Object_SetPartAttribute
.L_02009ed2:
	ldr r2, .L_02009edc
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	pop {pc}
.L_02009edc:
	.4byte gOverlayArea + 0x6b64
.L_02009ee0:
	.4byte gOverlayArea + 0x6b6c
	.section .text.x02009ee4,"ax",%progbits
	.global Func_02001ee4
	.thumb_func
Func_02001ee4:
	push {lr}
	ldr r3, .L_02009f08
	ldr r2, .L_02009f0c
	ldr r3, [r3]
	ldr r2, [r2]
	lsrs r3, r2
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02009f00
	movs r1, #0
	bl Object_SetPartAttribute
	b .L_02009f06
.L_02009f00:
	movs r1, #15
	bl Object_SetPartAttribute
.L_02009f06:
	pop {pc}
.L_02009f08:
	.4byte gOverlayArea + 0x6b64
.L_02009f0c:
	.4byte gOverlayArea + 0x6b6c
	.section .text.x02009f10,"ax",%progbits
	.global Func_02001f10
	.thumb_func
Func_02001f10:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #17
	sub sp, #8
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #30
	bl Object_GetById
	mov r9, r0
	movs r0, #31
	bl Object_GetById
	mov r11, r0
	movs r0, #32
	bl Object_GetById
	str r0, [sp, #4]
	bl Func_0200560c
	movs r0, #0
	bl Func_02005784
	movs r0, #17
	movs r1, #2
	bl Object_SetModeById
	movs r2, #0
	mov r8, r2
	adds r3, r7, #0
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	ldr r3, [r7, #8]
	ldr r2, .L_0200a35c
	movs r0, #28
	adds r3, r3, r2
	str r3, [r7, #8]
	ldr r3, [r7, #16]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r7, #16]
	movs r1, #2
	bl Object_SetModeById
	movs r0, #29
	movs r1, #3
	bl Object_SetModeById
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r1, r1
	negs r0, r0
	bl Motion_CamBounds
	movs r0, #1
	bl WaitFrames
	movs r0, #30
	movs r1, #7
	bl Object_SetModeById
	movs r0, #31
	movs r1, #7
	bl Object_SetModeById
	movs r0, #32
	movs r1, #6
	bl Object_SetModeById
	ldr r3, .L_0200a360
	movs r2, #133
	mov r10, r3
	lsls r2, r2, #2
	add r10, r2
	mov r3, r10
	ldr r0, [r3]
	movs r1, #252
	movs r3, #192
	movs r2, #160
	lsls r3, r3, #8
	lsls r2, r2, #17
	lsls r1, r1, #17
	bl Func_02005684
	movs r0, #1
	bl WaitFrames
	movs r0, #0
	movs r1, #0
	bl Func_02005764
	movs r1, #0
	movs r0, #0
	bl Func_0200575c
	movs r0, #1
	bl Func_0200576c
	movs r0, #30
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r0, #31
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r0, #32
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r0, #1
	bl WaitFrames
	movs r0, #21
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #20
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #19
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #26
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #25
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #88
	str r3, [r2]
	subs r3, #80
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #195
	lsls r0, r0, #1
	bl Func_020057dc
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	movs r1, #1
	bl Func_0200575c
	movs r0, #128
	movs r1, #2
	lsls r0, r0, #9
	bl Func_0200575c
	movs r0, #16
	bl Func_0200576c
	movs r0, #16
	bl WaitFrames
	movs r0, #80
	bl Battle_WaitMode0
	ldr r3, .L_0200a364
	ldr r6, .L_0200a368
	mov r2, r8
	str r2, [r3]
	movs r3, #4
	str r3, [r6]
	movs r0, #17
	bl Object_GetById
	ldr r5, .L_0200a36c
	str r5, [r0, #108]
	movs r0, #27
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #28
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #29
	bl Object_GetById
	ldr r3, .L_0200a370
	mov r2, r9
	str r5, [r0, #108]
	str r3, [r2, #108]
	mov r2, r11
	str r3, [r2, #108]
	ldr r2, [sp, #4]
	movs r0, #170
	lsls r0, r0, #1
	str r3, [r2, #108]
	adds r0, #255
	bl Func_020057dc
	movs r0, #10
	bl WaitFrames
	movs r0, #170
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020057dc
	movs r0, #10
	bl WaitFrames
	movs r0, #170
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020057dc
	movs r0, #10
	bl WaitFrames
	movs r0, #170
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020057dc
	movs r0, #10
	bl WaitFrames
	movs r0, #170
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020057dc
	movs r0, #40
	bl WaitFrames
	movs r3, #3
	str r3, [r6]
	movs r0, #30
	bl WaitFrames
	movs r3, #2
	str r3, [r6]
	movs r0, #20
	bl WaitFrames
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_0200567c
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_0200567c
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_0200567c
	movs r1, #0
	movs r2, #0
	movs r0, #29
	bl Func_0200567c
	movs r0, #17
	bl Object_GetById
	mov r3, r8
	str r3, [r0, #108]
	movs r0, #27
	bl Object_GetById
	mov r2, r8
	str r2, [r0, #108]
	movs r0, #28
	bl Object_GetById
	mov r3, r8
	str r3, [r0, #108]
	movs r0, #29
	bl Object_GetById
	mov r2, r8
	mov r3, r9
	str r2, [r0, #108]
	str r2, [r3, #108]
	mov r3, r11
	str r2, [r3, #108]
	ldr r3, [sp, #4]
	movs r0, #1
	str r2, [r3, #108]
	bl WaitFrames
	movs r0, #103
	bl Func_020057dc
	movs r0, #30
	bl Object_GetById
	movs r1, #9
	bl Object_SetPartAttribute
	movs r0, #31
	bl Object_GetById
	movs r1, #9
	bl Object_SetPartAttribute
	movs r0, #32
	bl Object_GetById
	movs r1, #9
	bl Object_SetPartAttribute
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #30
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #31
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #32
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #20
	bl Battle_WaitMode0
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	mov r2, r9
	adds r3, #40
	adds r2, #98
	strb r3, [r2]
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	mov r2, r11
	adds r3, #40
	adds r2, #98
	strb r3, [r2]
	bl Random16Far
	lsls r3, r0, #2
	ldr r2, [sp, #4]
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	adds r3, #40
	adds r2, #98
	strb r3, [r2]
	ldr r3, .L_0200a374
	mov r2, r9
	str r3, [r2, #108]
	mov r2, r11
	str r3, [r2, #108]
	ldr r2, [sp, #4]
	movs r0, #20
	str r3, [r2, #108]
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl Func_0200575c
	movs r0, #40
	bl Func_0200576c
	movs r0, #80
	bl WaitFrames
	ldr r0, .L_0200a378
	bl Func_020056dc
	movs r1, #128
	movs r0, #26
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #26
	movs r1, #0
	bl Func_020056f4
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	mov r3, r10
	movs r1, #128
	ldr r0, [r3]
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #23
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #24
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #26
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #25
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #204
	lsls r1, r1, #6
	adds r1, #51
	ldr r0, .L_0200a37c
	bl Func_02005724
	bl Func_0200573c
	mov r2, r8
	adds r0, #85
	strb r2, [r0]
	movs r1, #160
	movs r0, #128
	movs r2, #170
	movs r3, #1
	lsls r0, r0, #18
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Motion_CamBounds
	bl Func_02005734
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #19
	bl Func_02005714
	movs r1, #0
	movs r0, #19
	bl Func_020056f4
	movs r0, #8
	bl Func_020057dc
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #24
	bl Func_0200571c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #24
	movs r1, #0
	bl Func_020056f4
	movs r0, #25
	movs r1, #0
	bl Func_020056f4
	movs r0, #19
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r1, #129
	movs r0, #26
	lsls r1, r1, #1
	bl Func_0200571c
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	b .L_0200a380
	.2byte 0x0000
.L_0200a35c:
	.4byte 0xfffc0000
.L_0200a360:
	.4byte gPartyState
.L_0200a364:
	.4byte gOverlayArea + 0x6b64
.L_0200a368:
	.4byte gOverlayArea + 0x6b6c
.L_0200a36c:
	.4byte Func_02001eb0
.L_0200a370:
	.4byte Func_02001ee4
.L_0200a374:
	.4byte Func_0200008c
.L_0200a378:
	.4byte 0x00002ddd
.L_0200a37c:
	.4byte 0x00019999
.L_0200a380:
	bl Func_020056f4
	movs r0, #19
	movs r1, #4
	bl Object_SetModeById
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r1, #4
	adds r1, #255
	movs r2, #0
	movs r0, #23
	bl Func_02005714
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #23
	ldr r1, .L_0200a7a0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #164
	lsls r2, r2, #1
	movs r0, #23
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #23
	lsls r1, r1, #6
	bl Func_02005704
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #23
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #19
	bl Func_02005714
	movs r1, #2
	movs r0, #21
	bl ObjectMotion_SetActionVariant
	movs r0, #20
	bl Object_GetById
	mov r3, r8
	adds r0, #98
	strb r3, [r0]
	movs r0, #21
	bl Object_GetById
	mov r2, r8
	adds r0, #98
	strb r2, [r0]
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #21
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	movs r0, #20
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_0200a7a4
	movs r0, #21
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a7a8
	movs r0, #20
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #19
	ldr r1, .L_0200a7ac
	ldr r2, .L_0200a7b0
	bl ObjectMotion_SetSpeedParameters
	movs r1, #135
	movs r2, #170
	movs r0, #19
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #10
	movs r0, #19
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r0, #21
	bl Object_GetById
	adds r7, r0, #0
.L_0200a47a:
	movs r0, #1
	bl WaitFrames
	adds r3, r7, #0
	adds r3, #98
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0200a47a
	movs r1, #128
	movs r2, #0
	movs r0, #20
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #6
	bl Func_02005704
	movs r0, #21
	movs r1, #0
	bl Func_020056f4
	movs r0, #20
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #22
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #20
	bl Func_02005714
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #20
	ldr r1, .L_0200a7a0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #245
	movs r2, #148
	movs r0, #20
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #224
	movs r0, #21
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #204
	movs r2, #200
	lsls r1, r1, #6
	lsls r2, r2, #5
	adds r1, #51
	adds r2, #153
	movs r0, #20
	bl ObjectMotion_SetSpeedParameters
	movs r0, #20
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	mov r8, r3
	ands r3, r2
	movs r2, #0
	mov r10, r2
	movs r1, #244
	movs r2, #150
	strb r3, [r0]
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #20
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #20
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	mov r11, r2
	mov r2, r11
	orrs r3, r2
	strb r3, [r0]
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #20
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r8
	ands r3, r2
	movs r1, #243
	movs r2, #154
	strb r3, [r0]
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #20
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #20
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r11
	orrs r3, r2
	strb r3, [r0]
	movs r1, #200
	movs r0, #204
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_02005724
	movs r0, #128
	movs r1, #192
	movs r2, #160
	movs r3, #1
	lsls r0, r0, #18
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Motion_CamBounds
	bl Func_02005734
	movs r0, #21
	movs r1, #0
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r2, #20
	movs r0, #21
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #21
	movs r1, #0
	bl Func_02005704
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #21
	movs r1, #0
	bl Func_020056f4
	movs r1, #192
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r6, .L_0200a7b4
	movs r3, #133
	lsls r3, r3, #2
	adds r6, r6, r3
	movs r1, #160
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #23
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #0
	movs r0, #25
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #20
	bl Func_020056f4
	movs r0, #32
	bl Object_GetById
	movs r5, #128
	mov r2, r10
	adds r7, r0, #0
	lsls r5, r5, #9
	str r2, [r7, #108]
	movs r0, #1
	bl WaitFrames
	str r5, [r7, #24]
	str r5, [r7, #28]
	movs r0, #32
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #32
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #23
	bl Func_02005714
	movs r1, #224
	movs r0, #23
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #25
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #25
	bl Func_02005714
	movs r0, #25
	movs r1, #0
	bl Func_020056f4
	movs r2, #40
	movs r0, #23
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #23
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #32
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r0, #32
	movs r1, #0
	bl Func_020056f4
	movs r1, #192
	movs r2, #40
	movs r0, #24
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #24
	lsls r1, r1, #7
	bl Func_02005704
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #24
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #6
	bl Func_02005704
	movs r1, #129
	movs r0, #21
	lsls r1, r1, #1
	bl Func_0200571c
	movs r0, #21
	movs r1, #0
	bl Func_020056f4
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Object_GetById
	mov r3, r10
	adds r7, r0, #0
	str r3, [r7, #108]
	movs r0, #1
	bl WaitFrames
	str r5, [r7, #24]
	str r5, [r7, #28]
	movs r0, #31
	bl Object_GetById
	mov r2, r10
	adds r7, r0, #0
	str r2, [r7, #108]
	movs r0, #1
	bl WaitFrames
	str r5, [r7, #24]
	str r5, [r7, #28]
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #31
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r0, #249
	lsls r0, r0, #5
	adds r0, #255
	movs r1, #0
	bl Func_020056f4
	movs r0, #30
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #30
	movs r1, #0
	bl Func_020056f4
	movs r0, #20
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #20
	movs r1, #4
	b .L_0200a7b8
	.2byte 0x0000
.L_0200a7a0:
	.4byte 0x00019999
.L_0200a7a4:
	.4byte Data_02005f08
.L_0200a7a8:
	.4byte Data_02005f50
.L_0200a7ac:
	.4byte 0x00026666
.L_0200a7b0:
	.4byte 0x00013333
.L_0200a7b4:
	.4byte gPartyState
.L_0200a7b8:
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_020056f4
	movs r0, #20
	ldr r1, .L_0200a964
	ldr r2, .L_0200a968
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r6]
	ldr r1, .L_0200a964
	ldr r2, .L_0200a968
	bl ObjectMotion_SetSpeedParameters
	movs r0, #23
	ldr r1, .L_0200a964
	ldr r2, .L_0200a968
	bl ObjectMotion_SetSpeedParameters
	ldr r2, .L_0200a968
	movs r0, #24
	ldr r1, .L_0200a964
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_0200a96c
	movs r0, #20
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r0, [r6]
	ldr r1, .L_0200a970
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a974
	movs r0, #23
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a978
	movs r0, #24
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #22
	ldr r1, .L_0200a97c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #132
	movs r2, #160
	movs r0, #22
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #0
	movs r0, #22
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #25
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #160
	lsls r0, r0, #7
	adds r0, #22
	movs r1, #0
	bl Func_020056f4
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #25
	ldr r1, .L_0200a97c
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_0200a980
	movs r0, #25
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #243
	movs r2, #154
	lsls r2, r2, #1
	movs r0, #22
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r0, #22
	lsls r1, r1, #8
	bl Func_02005704
	ldr r1, .L_0200a964
	ldr r2, .L_0200a968
	movs r0, #21
	bl ObjectMotion_SetSpeedParameters
	movs r0, #21
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	ands r2, r3
	strb r2, [r0]
	mov r8, r2
	movs r1, #237
	movs r2, #158
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #21
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #21
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r11
	orrs r2, r3
	movs r1, #224
	strb r2, [r0]
	lsls r1, r1, #8
	movs r0, #21
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #231
	movs r2, #140
	lsls r2, r2, #1
	movs r0, #22
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #22
	movs r1, #0
	bl Func_02005704
	movs r1, #1
	movs r0, #22
	bl ObjectMotion_SetActionVariant
	movs r0, #22
	bl Object_GetById
	ldr r1, .L_0200a984
	bl Func_020053e4
	movs r0, #25
	bl Object_GetById
	ldr r1, .L_0200a988
	bl Func_020053e4
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #19
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r0, #19
	bl Func_020056f4
	movs r0, #78
	bl Func_020057dc
	movs r1, #0
	movs r0, #0
	bl Func_0200575c
	movs r0, #120
	bl Func_0200576c
	movs r0, #120
	bl WaitFrames
	movs r0, #22
	bl Object_GetById
	bl Func_020054e4
	movs r0, #25
	bl Object_GetById
	bl Func_020054e4
	movs r0, #80
	bl Battle_WaitMode0
	movs r0, #2
	bl Func_02005744
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a964:
	.4byte 0x00026666
.L_0200a968:
	.4byte 0x00013333
.L_0200a96c:
	.4byte Data_02005f98
.L_0200a970:
	.4byte Data_02005fdc
.L_0200a974:
	.4byte Data_02006004
.L_0200a978:
	.4byte Data_0200602c
.L_0200a97c:
	.4byte 0x00019999
.L_0200a980:
	.4byte Data_02006070
.L_0200a984:
	.4byte gOverlayArea + 0x6c20
.L_0200a988:
	.4byte gOverlayArea + 0x6b70
	.section .text.x0200a98c,"ax",%progbits
	.global Func_0200298c
	.thumb_func
Func_0200298c:
	push {r5, lr}
	adds r5, r0, #0
	bl Object_GetById
	adds r2, r0, #0
	movs r3, #0
	adds r2, #89
	adds r0, #85
	strb r3, [r2]
	movs r1, #1
	strb r3, [r0]
	adds r0, r5, #0
	bl ObjectMotion_SetActionVariant
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200a9ac,"ax",%progbits
	.global Func_020029ac
	.thumb_func
Func_020029ac:
	push {r5, r6, lr}
	sub sp, #8
	bl Func_0200560c
	movs r0, #0
	bl Func_02005784
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	movs r0, #1
	bl WaitFrames
	ldr r6, .L_0200adcc
	movs r3, #133
	lsls r3, r3, #2
	adds r6, r6, r3
	movs r1, #253
	movs r2, #140
	lsls r2, r2, #17
	lsls r1, r1, #17
	ldr r0, [r6]
	bl Func_0200567c
	movs r0, #1
	bl WaitFrames
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r1, #1
	movs r0, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #7
	bl Object_GetById
	ldr r1, .L_0200add0
	bl Func_020053e4
	movs r0, #3
	bl Object_GetById
	ldr r1, .L_0200add4
	bl Func_020053e4
	movs r0, #120
	bl Battle_WaitMode0
	movs r0, #7
	bl Object_GetById
	bl Func_020054e4
	movs r0, #3
	bl Object_GetById
	bl Func_020054e4
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #6
	bl Func_02005704
	ldr r0, .L_0200add8
	bl Func_020056dc
	movs r0, #6
	movs r1, #0
	bl Func_020056f4
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #2
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #6
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #3
	bl Func_02005714
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #3
	movs r1, #0
	bl Func_020056f4
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r2, #0
	ldr r0, [r6]
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	bl Func_02005704
	movs r1, #8
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_02005714
	movs r0, #5
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #5
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #5
	movs r1, #0
	bl Func_020056f4
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #2
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #5
	movs r1, #0
	bl Func_020056f4
	movs r0, #7
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_020056f4
	movs r1, #10
	adds r1, #255
	movs r2, #20
	movs r0, #5
	bl Func_02005714
	movs r0, #128
	lsls r0, r0, #8
	movs r2, #40
	adds r0, #5
	movs r1, #0
	bl Func_020056ec
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #6
	movs r1, #0
	bl Func_020056f4
	movs r0, #5
	movs r1, #0
	bl Func_02005704
	movs r0, #5
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #5
	movs r1, #0
	bl Func_020056f4
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #5
	ldr r1, .L_0200addc
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #5
	bl Object_SetModeById
	movs r1, #230
	movs r2, #144
	lsls r2, r2, #1
	movs r0, #5
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #5
	movs r1, #22
	bl Object_SetModeById
	movs r0, #5
	movs r1, #0
	bl Func_020056f4
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #19
	bl Func_02005714
	movs r0, #78
	bl Func_020057dc
	movs r0, #204
	movs r1, #192
	lsls r0, r0, #6
	lsls r1, r1, #3
	adds r0, #51
	adds r1, #102
	bl Func_02005724
	movs r0, #128
	movs r1, #1
	movs r2, #160
	movs r3, #1
	lsls r2, r2, #17
	lsls r0, r0, #18
	negs r1, r1
	bl Motion_CamBounds
	bl Func_02005734
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #2
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #7
	bl Func_02005704
	movs r0, #19
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #20
	adds r0, #19
	movs r1, #0
	bl Func_020056ec
	movs r1, #1
	movs r0, #5
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	bl Func_02005704
	movs r0, #5
	movs r1, #0
	bl Func_020056f4
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #20
	adds r0, #19
	movs r1, #0
	bl Func_020056ec
	movs r0, #5
	movs r1, #0
	bl Func_020056f4
	movs r0, #19
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #0
	movs r1, #0
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #204
	adds r2, #102
	movs r0, #1
	bl ObjectMotion_SetSpeedParameters
	movs r0, #1
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	movs r1, #128
	ands r3, r2
	lsls r1, r1, #2
	movs r2, #164
	strb r3, [r0]
	adds r1, #30
	lsls r2, r2, #1
	movs r0, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #1
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #200
	movs r0, #204
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r1, #153
	adds r0, #204
	bl Func_02005724
	bl Func_0200573c
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r1, #152
	movs r0, #135
	movs r2, #170
	movs r3, #1
	lsls r2, r2, #17
	lsls r0, r0, #18
	lsls r1, r1, #15
	bl Motion_CamBounds
	bl Func_02005734
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	bl Func_02005704
	movs r1, #128
	lsls r1, r1, #2
	movs r2, #170
	movs r0, #1
	adds r1, #30
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #1
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #160
	lsls r0, r0, #7
	adds r0, #1
	movs r1, #0
	bl Func_020056f4
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r2, #10
	movs r0, #1
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #160
	lsls r0, r0, #7
	adds r0, #1
	movs r1, #0
	bl Func_020056f4
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200ade0
	adds r1, #153
	b .L_0200ade4
.L_0200adcc:
	.4byte gPartyState
.L_0200add0:
	.4byte gOverlayArea + 0x6c20
.L_0200add4:
	.4byte gOverlayArea + 0x6b70
.L_0200add8:
	.4byte 0x00002df5
.L_0200addc:
	.4byte 0x00019999
.L_0200ade0:
	.4byte 0x0004cccc
.L_0200ade4:
	bl Func_02005724
	movs r0, #239
	movs r1, #192
	movs r2, #156
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Motion_CamBounds
	bl Func_02005734
	movs r1, #128
	movs r2, #40
	movs r0, #6
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	bl Func_02005704
	movs r1, #4
	movs r2, #20
	adds r1, #255
	movs r0, #6
	bl Func_02005714
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #7
	bl Func_02005704
	movs r2, #10
	movs r0, #6
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #6
	movs r1, #0
	bl Func_020056f4
	movs r0, #240
	movs r1, #1
	movs r2, #138
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	bl Func_02005734
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #20
	movs r0, #7
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #7
	movs r1, #0
	bl Func_020056f4
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #7
	movs r1, #0
	bl Func_020056f4
	movs r0, #160
	lsls r0, r0, #7
	movs r1, #0
	bl Func_020056f4
	movs r0, #128
	movs r1, #1
	movs r2, #156
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #40
	movs r0, #1
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	movs r2, #20
	bl Func_020056ec
	movs r1, #6
	movs r2, #40
	adds r1, #255
	ldr r0, [r6]
	bl Func_02005714
	movs r0, #0
	movs r1, #0
	bl Func_02005704
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #0
	bl Func_02005714
	movs r0, #144
	lsls r0, r0, #8
	movs r1, #0
	bl Func_020056f4
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #8
	bl UiText_OpenMessageAtObject
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200af84
	movs r0, #5
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #5
	movs r1, #0
	bl Func_020056f4
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200afa6
.L_0200af84:
	movs r0, #5
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #5
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_020056f4
.L_0200afa6:
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #1
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #137
	movs r2, #160
	lsls r2, r2, #1
	movs r0, #1
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #1
	movs r1, #3
	bl Object_SetModeById
	movs r0, #1
	movs r1, #0
	bl Func_020056f4
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #10
	movs r2, #40
	adds r1, #255
	movs r0, #6
	bl Func_02005714
	movs r0, #6
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #6
	movs r2, #40
	adds r1, #255
	movs r0, #2
	bl Func_02005714
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #2
	movs r1, #0
	bl Func_020056f4
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #0
	bl Func_020056f4
	movs r0, #3
	movs r1, #4
	bl Object_SetModeById
	movs r0, #3
	movs r1, #0
	bl Func_020056f4
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #19
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #249
	movs r2, #160
	lsls r2, r2, #1
	movs r0, #19
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #19
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #19
	movs r1, #0
	bl Func_020056f4
	ldr r5, .L_0200b208
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	bl Func_02005704
	movs r1, #3
	ldr r0, [r5]
	bl Motion_SetModeAndWaitAnimation
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #61
	bl GameFlag_SetBit
	movs r0, #28
	bl Func_020057dc
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #0
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #135
	movs r2, #143
	lsls r2, r2, #1
	movs r0, #0
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #0
	bl Func_02005704
	movs r0, #7
	bl Func_0200298c
	movs r0, #5
	bl Func_0200298c
	movs r0, #6
	bl Func_0200298c
	movs r0, #19
	bl Func_0200298c
	movs r0, #0
	bl Func_0200298c
	movs r0, #3
	bl Func_0200298c
	movs r0, #2
	bl Func_0200298c
	movs r0, #1
	bl Func_0200298c
	ldr r0, [r5]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	movs r3, #23
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #40
	movs r2, #18
	movs r3, #16
	movs r0, #39
	bl Func_020055ac
	movs r0, #1
	bl Func_020055fc
	movs r0, #7
	bl Party_RemoveOwnerRestored
	movs r0, #5
	bl Party_RemoveOwnerRestored
	movs r0, #6
	bl Party_RemoveOwnerRestored
	movs r0, #0
	bl Party_RemoveOwnerRestored
	movs r0, #3
	bl Party_RemoveOwnerRestored
	movs r0, #2
	bl Party_RemoveOwnerRestored
	movs r0, #1
	bl Party_RemoveOwnerRestored
	movs r0, #20
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #8
	orrs r3, r5
	strb r3, [r0]
	movs r0, #21
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r6, #0
	orrs r5, r3
	strb r5, [r0]
	movs r0, #28
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r1, #142
	movs r2, #137
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r0, #28
	bl Func_0200567c
	movs r0, #17
	bl Object_GetById
	adds r0, #85
	strb r6, [r0]
	movs r0, #17
	bl Object_GetById
	movs r1, #246
	movs r2, #192
	movs r3, #142
	lsls r1, r1, #17
	lsls r2, r2, #15
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	movs r0, #29
	bl Object_GetById
	adds r0, #85
	strb r6, [r0]
	movs r0, #29
	bl Object_GetById
	movs r1, #255
	movs r2, #192
	lsls r1, r1, #17
	lsls r2, r2, #15
	ldr r3, .L_0200b20c
	bl Object_SetPositionAndResetMotion
	movs r0, #30
	bl Object_GetById
	adds r0, #85
	strb r6, [r0]
	movs r0, #30
	bl Object_GetById
	movs r1, #135
	movs r2, #192
	movs r3, #150
	lsls r1, r1, #18
	lsls r2, r2, #15
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r0, #190
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	bl Func_02005614
	add sp, #8
	pop {r5, r6, pc}
.L_0200b208:
	.4byte gPartyState
.L_0200b20c:
	.4byte 0x01270000
	.section .text.x0200b210,"ax",%progbits
	.global Func_02003210
	.thumb_func
Func_02003210:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	adds r5, r0, #0
	movs r0, #23
	bl Object_GetById
	adds r6, r5, #0
	adds r6, #100
	ldrh r1, [r6]
	mov r10, r0
	mov r8, r1
	mov r0, r8
	bl Math_Cosine
	ldr r3, [r5, #48]
	mov r1, r10
	adds r3, #3
	adds r2, r3, #0
	muls r2, r0
	ldr r3, [r1, #8]
	mov r0, r8
	adds r3, r3, r2
	str r3, [r5, #8]
	bl Math_Sine
	mov r2, r10
	ldr r3, [r2, #16]
	ldr r2, [r5, #8]
	lsls r0, r0, #1
	adds r3, r3, r0
	str r3, [r5, #16]
	str r2, [r5, #56]
	str r3, [r5, #64]
	ldr r1, .L_0200b268
	ldrh r3, [r6]
	adds r3, r3, r1
	strh r3, [r6]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b268:
	.4byte 0xfffff800
	.section .text.x0200b26c,"ax",%progbits
	.global Func_0200326c
	.thumb_func
Func_0200326c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b28c
	ldr r3, .L_0200b2ac
	movs r1, #3
	ldr r0, [r3]
	bl Engine_MathModulo
	cmp r0, #0
	bne .L_0200b350
.L_0200b28c:
	movs r0, #23
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b2b0
	bl Random16Far
	adds r2, r0, #0
	ldr r3, [r5, #12]
	lsls r2, r2, #8
	b .L_0200b2ba
.L_0200b2ac:
	.4byte Data_0300122c
.L_0200b2b0:
	bl Random16Far
	adds r2, r0, #0
	ldr r3, [r5, #12]
	lsls r2, r2, #6
.L_0200b2ba:
	lsrs r2, r2, #16
	lsls r2, r2, #16
	adds r2, r2, r3
	ldr r3, .L_0200b340
	movs r0, #168
	adds r2, r2, r3
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	lsls r0, r0, #2
	bl Func_02005574
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200b350
	ldr r1, .L_0200b344
	adds r0, r7, #0
	ldr r6, [r7, #80]
	bl Func_0200556c
	movs r1, #1
	adds r0, r7, #0
	bl Object_SetPartAttribute
	adds r3, r7, #0
	adds r3, #85
	movs r5, #0
	strb r5, [r3]
	bl Random16Far
	ldr r3, .L_0200b348
	adds r2, r7, #0
	adds r2, #100
	ands r3, r0
	strh r3, [r2]
	adds r3, r7, #0
	adds r3, #102
	strh r5, [r3]
	ldr r3, .L_0200b34c
	ldr r1, .L_0200b33c
	str r3, [r7, #108]
	mov r8, r1
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #16
	subs r0, r0, r3
	lsrs r0, r0, #20
	bl Math_Sine
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	asrs r3, r3, #16
	str r3, [r7, #48]
	mov r3, r8
	ldrb r2, [r6, #9]
	strb r3, [r6, #26]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r6, #9]
	b .L_0200b350
	.2byte 0x0000
.L_0200b33c:
	.4byte 0x00000000
.L_0200b340:
	.4byte 0xffe40000
.L_0200b344:
	.4byte Data_02006afc
.L_0200b348:
	.4byte 0x0ffff000
.L_0200b34c:
	.4byte Func_02003210
.L_0200b350:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200b370,"ax",%progbits
	.global Func_02003370
	.thumb_func
Func_02003370:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #8
	bl Object_GetById
	ldr r5, .L_0200b4c8
	adds r6, r0, #0
	ldr r3, [r5]
	movs r7, #0
	cmp r3, #26
	bls .L_0200b38c
	b .L_0200b506
.L_0200b38c:
	ldr r2, .L_0200b4cc
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0200b394:
	.4byte .L_0200b400
	.4byte .L_0200b506
	.4byte .L_0200b506
	.4byte .L_0200b506
	.4byte .L_0200b506
	.4byte .L_0200b506
	.4byte .L_0200b506
	.4byte .L_0200b506
	.4byte .L_0200b41a
	.4byte .L_0200b506
	.4byte .L_0200b506
	.4byte .L_0200b506
	.4byte .L_0200b506
	.4byte .L_0200b506
	.4byte .L_0200b506
	.4byte .L_0200b506
	.4byte .L_0200b42c
	.4byte .L_0200b506
	.4byte .L_0200b506
	.4byte .L_0200b506
	.4byte .L_0200b506
	.4byte .L_0200b506
	.4byte .L_0200b506
	.4byte .L_0200b506
	.4byte .L_0200b43e
	.4byte .L_0200b466
	.4byte .L_0200b4a8
.L_0200b400:
	movs r0, #220
	bl Func_020057dc
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_020055c4
	ldr r0, .L_0200b4d0
	b .L_0200b41e
.L_0200b41a:
	movs r0, #128
	lsls r0, r0, #9
.L_0200b41e:
	movs r1, #1
	bl Func_0200575c
	movs r0, #8
	bl Func_0200576c
	b .L_0200b506
.L_0200b42c:
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_020055c4
	b .L_0200b506
.L_0200b43e:
	movs r3, #128
	lsls r3, r3, #18
	str r3, [r6, #8]
	ldr r3, .L_0200b4d4
	adds r0, r6, #0
	str r3, [r6, #12]
	movs r3, #204
	lsls r3, r3, #16
	str r3, [r6, #16]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #24]
	str r3, [r6, #28]
	bl SceneActor_ParkRecord
	ldr r1, .L_0200b4d8
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	b .L_0200b506
.L_0200b466:
	ldr r3, [r5]
	subs r3, #1
	str r3, [r5]
	ldr r3, [r6, #12]
	cmp r3, #0
	ble .L_0200b488
	ldr r0, .L_0200b4dc
	movs r1, #0
	bl Func_0200575c
	movs r0, #16
	bl Func_0200576c
	ldr r3, [r5]
	adds r3, #1
	str r3, [r5]
	b .L_0200b506
.L_0200b488:
	ldr r3, .L_0200b4e0
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200b49a
	movs r0, #246
	bl Func_020057dc
.L_0200b49a:
	ldr r3, [r6, #12]
	movs r1, #144
	lsls r1, r1, #10
	adds r3, r3, r1
	movs r7, #1
	str r3, [r6, #12]
	b .L_0200b506
.L_0200b4a8:
	ldr r3, [r5]
	movs r2, #160
	subs r3, #1
	str r3, [r5]
	lsls r2, r2, #13
	ldr r3, [r6, #12]
	cmp r3, r2
	ble .L_0200b4e8
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	str r3, [r5]
	ldr r0, .L_0200b4e4
	bl Scheduler_RemoveCallbackFar
	b .L_0200b506
.L_0200b4c8:
	.4byte gOverlayArea + 0x6ccc
.L_0200b4cc:
	.4byte .L_0200b394
.L_0200b4d0:
	.4byte 0x002063ff
.L_0200b4d4:
	.4byte 0xfe980000
.L_0200b4d8:
	.4byte Data_02005b54
.L_0200b4dc:
	.4byte 0x00203210
.L_0200b4e0:
	.4byte Data_0300122c
.L_0200b4e4:
	.4byte Func_02003370
.L_0200b4e8:
	ldr r3, .L_0200b59c
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200b4fa
	movs r0, #246
	bl Func_020057dc
.L_0200b4fa:
	ldr r3, [r6, #12]
	movs r1, #144
	lsls r1, r1, #10
	adds r3, r3, r1
	str r3, [r6, #12]
	movs r7, #1
.L_0200b506:
	cmp r7, #0
	beq .L_0200b5c0
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	ldr r2, [r6, #12]
	lsls r3, r3, #4
	lsrs r3, r3, #16
	lsls r3, r3, #16
	subs r2, r2, r3
	ldr r3, .L_0200b5a0
	movs r0, #168
	adds r2, r2, r3
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	lsls r0, r0, #2
	bl Func_02005574
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200b5c0
	ldr r1, [r7, #80]
	movs r5, #0
	mov r10, r1
	ldr r1, .L_0200b5a4
	bl Func_0200556c
	movs r1, #1
	adds r0, r7, #0
	bl Object_SetPartAttribute
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	bl Random16Far
	ldr r3, .L_0200b5a8
	adds r2, r7, #0
	adds r2, #100
	ands r3, r0
	strh r3, [r2]
	ldr r1, .L_0200b598
	adds r3, r7, #0
	adds r3, #102
	strh r5, [r3]
	mov r8, r1
	bl Random16Far
	adds r3, r7, #0
	lsrs r0, r0, #13
	adds r3, #98
	strb r0, [r3]
	ldr r3, .L_0200b5ac
	str r3, [r7, #108]
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #16
	subs r0, r0, r3
	lsrs r0, r0, #20
	bl Math_Sine
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	str r3, [r7, #48]
	mov r1, r10
	movs r2, #50
	ldrsh r3, [r6, r2]
	str r3, [r7, #48]
	mov r3, r8
	b .L_0200b5b0
.L_0200b598:
	.4byte 0x00000000
.L_0200b59c:
	.4byte Data_0300122c
.L_0200b5a0:
	.4byte 0xfff80000
.L_0200b5a4:
	.4byte Data_02006180
.L_0200b5a8:
	.4byte 0x0ffff000
.L_0200b5ac:
	.4byte Func_020000e4
.L_0200b5b0:
	ldrb r2, [r1, #9]
	strb r3, [r1, #26]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
.L_0200b5c0:
	ldr r2, .L_0200b5e0
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	ldr r2, .L_0200b5e4
	ldr r2, [r2]
	cmp r3, r2
	bls .L_0200b5d6
	ldr r0, .L_0200b5e8
	bl Scheduler_RemoveCallbackFar
.L_0200b5d6:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b5e0:
	.4byte gOverlayArea + 0x6ccc
.L_0200b5e4:
	.4byte gOverlayArea + 0x6b68
.L_0200b5e8:
	.4byte Func_02003370
	.section .text.x0200b5ec,"ax",%progbits
	.global Func_020035ec
	.thumb_func
Func_020035ec:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl Object_GetById
	movs r1, #1
	adds r5, r0, #0
	adds r0, r6, #0
	bl ObjectMotion_SetActionVariant
	ldr r1, [r5, #8]
	ldr r0, .L_0200b618
	ldr r3, [r5, #16]
	adds r1, r1, r0
	movs r0, #128
	lsls r0, r0, #14
	adds r3, r3, r0
	ldr r2, [r5, #12]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b618:
	.4byte 0xfff00000
	.section .text.x0200b61c,"ax",%progbits
	.global Func_0200361c
	.thumb_func
Func_0200361c:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl Object_GetById
	movs r1, #1
	adds r5, r0, #0
	adds r0, r6, #0
	bl ObjectMotion_SetActionVariant
	ldr r1, [r5, #8]
	movs r0, #128
	lsls r0, r0, #13
	ldr r3, [r5, #16]
	adds r1, r1, r0
	movs r0, #128
	lsls r0, r0, #14
	adds r3, r3, r0
	ldr r2, [r5, #12]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
	pop {r5, r6, pc}
	.section .text.x0200b648,"ax",%progbits
	.global Func_02003648
	.thumb_func
Func_02003648:
	push {lr}
	bl Object_GetById
	movs r3, #3
	adds r0, #85
	strb r3, [r0]
	pop {pc}
	.2byte 0x0000
	.section .text.x0200b658,"ax",%progbits
	.global Func_02003658
	.thumb_func
Func_02003658:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	sub sp, #8
	bl Motion_CamBounds
	movs r0, #1
	bl WaitFrames
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_0200567c
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_0200567c
	movs r2, #0
	movs r0, #30
	movs r1, #0
	bl Func_0200567c
	movs r1, #204
	lsls r1, r1, #6
	adds r1, #51
	ldr r0, .L_0200b718
	bl Func_02005724
	bl Func_0200573c
	movs r1, #0
	mov r10, r1
	adds r0, #85
	mov r2, r10
	strb r2, [r0]
	movs r1, #192
	movs r0, #128
	movs r2, #156
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02005734
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_0200b710
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200b714
	subs r2, #2
	strh r3, [r2]
	movs r1, #10
	movs r3, #23
	str r3, [sp, #0]
	str r1, [sp, #4]
	movs r2, #18
	movs r1, #40
	movs r0, #18
	mov r11, r3
	movs r3, #16
	bl Func_020055ac
	movs r0, #7
	bl Func_02003648
	movs r0, #5
	bl Func_02003648
	movs r0, #6
	bl Func_02003648
	movs r0, #19
	b .L_0200b71c
.L_0200b710:
	.4byte 0x00000c08
.L_0200b714:
	.4byte 0x00003f10
.L_0200b718:
	.4byte 0x00019999
.L_0200b71c:
	bl Func_02003648
	movs r0, #0
	bl Func_02003648
	movs r0, #3
	bl Func_02003648
	movs r0, #2
	bl Func_02003648
	movs r0, #1
	bl Func_02003648
	movs r0, #0
	movs r1, #0
	bl Event_PrepareObjectAndApplyValue
	ldr r3, .L_0200bb38
	movs r2, #133
	lsls r2, r2, #2
	adds r2, r2, r3
	mov r9, r2
	ldr r0, [r2]
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	mov r3, r9
	movs r1, #128
	movs r2, #140
	ldr r0, [r3]
	lsls r2, r2, #1
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	mov r1, r9
	ldr r0, [r1]
	movs r1, #192
	lsls r1, r1, #8
	bl Func_02005704
	mov r2, r9
	ldr r0, [r2]
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #222
	bl PartyInventory_Remove
	movs r0, #4
	bl Object_GetById
	ldr r2, [r0, #12]
	movs r3, #128
	lsls r3, r3, #12
	ldr r1, [r0, #8]
	adds r2, r2, r3
	ldr r3, [r0, #16]
	movs r0, #234
	adds r0, #255
	bl Func_02005574
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200b8ac
	ldr r6, [r7, #80]
	movs r3, #33
	ldrb r2, [r6, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	mov r1, r10
	ands r3, r2
	movs r2, #13
	strb r1, [r6, #27]
	negs r2, r2
	adds r1, r7, #0
	adds r1, #35
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	ldrb r2, [r1]
	strb r3, [r6, #9]
	movs r3, #254
	ands r3, r2
	movs r2, #85
	strb r3, [r1]
	adds r2, r2, r7
	mov r3, r10
	strb r3, [r2]
	mov r8, r2
	adds r2, r7, #0
	adds r2, #92
	movs r3, #1
	strb r3, [r2]
	movs r2, #2
	ldrb r3, [r1]
	orrs r3, r2
	strb r3, [r1]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r7, #48]
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #204
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #8
	movs r1, #193
	str r3, [r7, #72]
	lsls r1, r1, #3
	movs r0, #68
	bl Runtime_AllocateHeapBlockFar
	adds r5, r0, #0
	movs r0, #247
	bl Func_020055ec
	movs r1, #128
	lsls r1, r1, #3
	adds r5, r5, r1
	adds r2, r5, #0
	movs r1, #128
	ldrb r0, [r6, #16]
	bl VramBlock_LoadCached
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
	mov r2, r10
	mov r3, r8
	strb r2, [r3]
	movs r1, #128
	movs r2, #128
	movs r3, #140
	lsls r2, r2, #16
	lsls r1, r1, #18
	lsls r3, r3, #17
	adds r0, r7, #0
	bl Func_02005594
	adds r0, r7, #0
	bl Func_0200559c
	movs r3, #4
	mov r1, r8
	strb r3, [r1]
	movs r0, #60
	bl Battle_WaitMode0
	mov r2, r10
	mov r3, r8
	strb r2, [r3]
	movs r1, #128
	movs r2, #128
	movs r3, #132
	lsls r1, r1, #18
	lsls r2, r2, #16
	lsls r3, r3, #17
	adds r0, r7, #0
	bl Func_02005594
	adds r0, r7, #0
	bl Func_0200559c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #160
	movs r3, #252
	lsls r1, r1, #18
	lsls r2, r2, #15
	lsls r3, r3, #16
	adds r0, r7, #0
	bl Func_02005594
	adds r0, r7, #0
	bl Func_0200559c
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r7, #0
	bl Func_0200557c
.L_0200b8ac:
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #24
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #25
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #26
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r1, #2
	movs r0, #27
	bl ObjectMotion_SetActionVariant
	movs r0, #141
	bl Func_020057dc
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #9
	lsls r0, r0, #9
	bl Func_020055c4
	movs r0, #23
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #23
	bl Object_GetById
	ldr r6, .L_0200bb3c
	movs r5, #200
	adds r3, r0, #0
	lsls r5, r5, #5
	adds r5, #153
	adds r3, #85
	mov r1, r10
	str r5, [r0, #24]
	str r6, [r0, #28]
	movs r2, #128
	strb r1, [r3]
	movs r1, #128
	movs r3, #204
	lsls r2, r2, #15
	lsls r3, r3, #16
	lsls r1, r1, #18
	bl Object_SetPositionAndResetMotion
	movs r0, #24
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #24
	bl Object_GetById
	adds r3, r0, #0
	adds r3, #85
	mov r2, r10
	str r5, [r0, #24]
	str r6, [r0, #28]
	movs r1, #128
	strb r2, [r3]
	movs r2, #192
	movs r3, #204
	lsls r2, r2, #15
	lsls r1, r1, #18
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	ldr r3, .L_0200bb40
	movs r1, #144
	mov r8, r3
	lsls r1, r1, #3
	mov r0, r8
	bl Scheduler_AddOrUpdateCallback
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #80
	movs r0, #1
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	ldr r5, .L_0200bb44
	movs r0, #23
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #24
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #145
	bl Func_020057dc
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl Func_020055c4
	movs r1, #0
	ldr r0, .L_0200bb48
	bl Func_0200575c
	movs r0, #16
	bl Func_0200576c
	movs r0, #20
	bl WaitFrames
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_0200575c
	movs r0, #24
	bl Func_0200576c
	movs r0, #60
	bl WaitFrames
	movs r0, #141
	bl Func_020057dc
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #23
	bl Object_GetById
	ldr r3, .L_0200bb4c
	str r3, [r0, #12]
	movs r0, #24
	bl Object_GetById
	ldr r3, .L_0200bb50
	str r3, [r0, #12]
	movs r0, #25
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #25
	bl Object_GetById
	adds r5, r0, #0
	str r6, [r5, #28]
	movs r0, #23
	bl Object_GetById
	ldr r3, [r0, #24]
	mov r1, r10
	str r3, [r5, #24]
	adds r3, r5, #0
	adds r3, #85
	strb r1, [r3]
	movs r2, #128
	movs r1, #128
	movs r3, #204
	lsls r2, r2, #12
	lsls r3, r3, #16
	adds r0, r5, #0
	lsls r1, r1, #18
	bl Object_SetPositionAndResetMotion
	movs r0, #26
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #26
	bl Object_GetById
	adds r5, r0, #0
	str r6, [r5, #28]
	movs r0, #23
	bl Object_GetById
	ldr r3, [r0, #24]
	mov r2, r10
	str r3, [r5, #24]
	adds r3, r5, #0
	adds r3, #85
	strb r2, [r3]
	movs r1, #128
	movs r2, #160
	movs r3, #204
	lsls r2, r2, #14
	lsls r3, r3, #16
	adds r0, r5, #0
	lsls r1, r1, #18
	bl Object_SetPositionAndResetMotion
	movs r0, #27
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #27
	bl Object_GetById
	adds r5, r0, #0
	str r6, [r5, #28]
	movs r0, #23
	bl Object_GetById
	ldr r3, [r0, #24]
	mov r1, r10
	str r3, [r5, #24]
	adds r3, r5, #0
	adds r3, #85
	strb r1, [r3]
	movs r2, #144
	movs r1, #128
	movs r3, #204
	adds r0, r5, #0
	lsls r1, r1, #18
	lsls r2, r2, #15
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	mov r2, r11
	str r2, [sp, #4]
	movs r5, #17
	movs r0, #106
	movs r1, #36
	movs r2, #87
	movs r3, #7
	str r5, [sp, #0]
	bl Func_020055a4
	mov r3, r11
	str r3, [sp, #4]
	movs r0, #42
	movs r1, #98
	movs r2, #23
	movs r3, #69
	str r5, [sp, #0]
	bl Func_020055a4
	mov r1, r11
	movs r2, #10
	str r1, [sp, #0]
	str r2, [sp, #4]
	movs r1, #40
	movs r2, #18
	movs r3, #20
	movs r0, #0
	bl Func_020055ac
	movs r0, #7
	bl Func_020035ec
	movs r0, #5
	bl Func_020035ec
	movs r0, #6
	bl Func_020035ec
	movs r0, #20
	b .L_0200bb54
	.2byte 0x0000
.L_0200bb38:
	.4byte gPartyState
.L_0200bb3c:
	.4byte 0xffff0000
.L_0200bb40:
	.4byte Func_0200326c
.L_0200bb44:
	.4byte Data_020060b4
.L_0200bb48:
	.4byte 0x004063ff
.L_0200bb4c:
	.4byte 0xffc80000
.L_0200bb50:
	.4byte 0xffe80000
.L_0200bb54:
	bl Func_020035ec
	movs r0, #21
	bl Func_020035ec
	movs r0, #19
	bl Func_020035ec
	movs r0, #2
	bl Func_0200361c
	movs r0, #1
	bl Func_0200361c
	movs r0, #22
	bl Func_0200361c
	movs r0, #0
	bl Func_0200361c
	movs r0, #3
	bl Func_0200361c
	mov r3, r9
	ldr r0, [r3]
	movs r1, #242
	movs r3, #224
	movs r2, #156
	lsls r3, r3, #8
	lsls r2, r2, #17
	lsls r1, r1, #17
	bl Func_02005684
	mov r1, r9
	ldr r0, [r1]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl Func_020055c4
	movs r1, #0
	ldr r0, .L_0200bfac
	bl Func_0200575c
	movs r0, #120
	bl Func_0200576c
	ldr r5, .L_0200bfb0
	movs r0, #23
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #24
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #25
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #26
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #27
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #120
	bl WaitFrames
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl Func_020055c4
	movs r1, #0
	ldr r0, .L_0200bfb4
	bl Func_0200575c
	movs r0, #120
	bl Func_0200576c
	movs r0, #120
	bl WaitFrames
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl Func_020055c4
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_0200575c
	movs r0, #120
	bl Func_0200576c
	movs r0, #120
	bl WaitFrames
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #9
	lsls r0, r0, #9
	bl Func_020055c4
	movs r0, #78
	bl Func_020057dc
	movs r0, #23
	bl Object_GetById
	ldr r5, .L_0200bfb8
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #30
	str r3, [r0, #28]
	adds r1, r5, #0
	movs r0, #24
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #25
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #26
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #27
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #195
	lsls r0, r0, #1
	bl Func_020057dc
	movs r0, #23
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r0, #20
	bl Battle_WaitMode0
	ldr r1, .L_0200bfbc
	movs r0, #23
	bl Object_SetActionCallbackAndRefreshById
	mov r0, r8
	bl Scheduler_RemoveCallbackFar
	movs r0, #8
	bl Object_GetById
	adds r3, r0, #0
	adds r3, #85
	mov r2, r10
	strb r2, [r3]
	movs r1, #1
	bl Animation_SetStateFlags
	movs r0, #8
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #8
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	ldr r3, .L_0200bfc0
	ldr r2, .L_0200bfc4
	mov r1, r10
	str r1, [r3]
	movs r3, #240
	movs r1, #144
	str r3, [r2]
	ldr r0, .L_0200bfc8
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
.L_0200bcea:
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_0200bfc0
	ldr r3, [r3]
	cmp r3, #100
	bls .L_0200bcea
	movs r0, #72
	bl Func_020057dc
	movs r0, #120
	bl Battle_WaitMode0
	movs r1, #10
	movs r2, #40
	adds r1, #255
	movs r0, #3
	bl Func_02005714
	ldr r6, .L_0200bfcc
	adds r0, r6, #0
	bl Func_020056dc
	movs r0, #3
	movs r1, #0
	bl Func_020056f4
	movs r1, #6
	movs r2, #40
	adds r1, #255
	movs r0, #2
	bl Func_02005714
	movs r0, #2
	movs r1, #0
	bl Func_020056f4
	movs r0, #6
	movs r1, #0
	bl Func_020056f4
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #7
	bl Func_02005714
	movs r0, #7
	movs r1, #0
	bl Func_020056f4
	movs r1, #4
	movs r2, #40
	adds r1, #255
	movs r0, #1
	bl Func_02005714
	movs r0, #1
	movs r1, #0
	bl Func_020056f4
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r5, .L_0200bfd0
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #19
	movs r1, #3
	bl Object_SetModeById
	movs r0, #3
	movs r1, #3
	bl Object_SetModeById
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #1
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #2
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #0
	movs r1, #0
	bl Func_02005704
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_020056ec
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_020055c4
	bl Func_020055cc
	movs r0, #23
	movs r1, #24
	bl Func_0200578c
	adds r0, r6, #6
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02005794
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #24
	bl Func_0200578c
	adds r0, r6, #7
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02005794
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	mov r8, r1
	add r2, r8
	mov r10, r3
	ldrh r3, [r2]
	movs r1, #0
	adds r3, #2
	strh r3, [r2]
	mov r9, r1
	ldr r0, [r5]
	bl Func_02000610
	movs r0, #7
	bl Func_02000610
	movs r0, #5
	bl Func_02000610
	movs r0, #6
	bl Func_02000610
	movs r0, #19
	bl Func_02000610
	movs r0, #1
	bl Func_02000610
	movs r0, #2
	bl Func_02000610
	movs r0, #3
	bl Func_02000610
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #40
	movs r0, #0
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	bl Func_02005704
	movs r0, #192
	movs r2, #10
	lsls r0, r0, #7
	movs r1, #0
	bl Func_020056ec
	adds r6, #9
	movs r0, #23
	movs r1, #24
	bl Func_0200578c
	adds r0, r6, #0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02005794
	movs r0, #3
	bl Func_02000640
	movs r0, #192
	lsls r0, r0, #7
	movs r1, #128
	lsls r1, r1, #7
	adds r0, #3
	bl Func_02005704
	movs r0, #3
	bl Object_GetById
	mov r2, r9
	str r2, [r0, #108]
	mov r3, r10
	ldr r2, [r3, #108]
	movs r1, #128
	add r2, r8
	ldrh r3, [r2]
	lsls r1, r1, #1
	adds r3, #1
	strh r3, [r2]
	movs r0, #3
	movs r2, #20
	bl Func_02005714
	movs r2, #20
	movs r1, #0
	movs r0, #3
	bl Func_020056ec
	movs r0, #201
	bl Func_020057dc
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_0200575c
	movs r0, #20
	b .L_0200bfd4
.L_0200bfac:
	.4byte 0x004063ff
.L_0200bfb0:
	.4byte Data_020060fc
.L_0200bfb4:
	.4byte 0x00203210
.L_0200bfb8:
	.4byte Data_02006120
.L_0200bfbc:
	.4byte Data_02006150
.L_0200bfc0:
	.4byte gOverlayArea + 0x6ccc
.L_0200bfc4:
	.4byte gOverlayArea + 0x6b68
.L_0200bfc8:
	.4byte Func_02003370
.L_0200bfcc:
	.4byte 0x00002e24
.L_0200bfd0:
	.4byte gPartyState
.L_0200bfd4:
	bl Func_0200576c
	movs r0, #20
	bl WaitFrames
	movs r0, #3
	bl Func_02005744
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200bff4,"ax",%progbits
	.global Func_02003ff4
	.thumb_func
Func_02003ff4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	bl Func_0200560c
	movs r0, #0
	bl Func_02005784
	ldr r6, .L_0200c058
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r6, r2
	ldr r0, [r6]
	bl Object_GetById
	movs r5, #128
	ldr r3, .L_0200c054
	lsls r5, r5, #7
	strh r5, [r0, #6]
	movs r0, #7
	mov r10, r3
	bl Object_GetById
	strh r5, [r0, #6]
	movs r0, #5
	bl Object_GetById
	strh r5, [r0, #6]
	movs r0, #6
	bl Object_GetById
	strh r5, [r0, #6]
	movs r0, #19
	bl Object_GetById
	movs r2, #192
	lsls r2, r2, #6
	mov r8, r2
	mov r3, r8
	strh r3, [r0, #6]
	movs r0, #0
	bl Object_GetById
	strh r5, [r0, #6]
	movs r0, #1
	b .L_0200c05c
	.2byte 0x0000
.L_0200c054:
	.4byte 0x00000000
.L_0200c058:
	.4byte gPartyState
.L_0200c05c:
	bl Object_GetById
	strh r5, [r0, #6]
	movs r0, #2
	bl Object_GetById
	strh r5, [r0, #6]
	movs r0, #1
	bl WaitFrames
	movs r0, #20
	movs r1, #8
	bl Object_SetModeById
	movs r0, #21
	movs r1, #8
	bl Object_SetModeById
	movs r0, #22
	movs r1, #7
	bl Object_SetModeById
	movs r1, #249
	movs r2, #160
	movs r0, #19
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200567c
	movs r2, #160
	movs r0, #1
	ldr r1, .L_0200c498
	lsls r2, r2, #17
	bl Func_0200567c
	movs r1, #253
	movs r2, #140
	ldr r0, [r6]
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200567c
	movs r0, #1
	bl WaitFrames
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_0200573c
	mov r2, r10
	adds r0, #85
	strb r2, [r0]
	movs r1, #192
	movs r0, #128
	movs r2, #144
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #0
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_0200558c
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
	bl Event_WaitValue1c8Frames
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r0, [r6]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #40
	movs r0, #2
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #6
	bl Func_02005704
	ldr r0, .L_0200c49c
	bl Func_020056dc
	movs r0, #6
	movs r1, #0
	bl Func_020056f4
	movs r1, #192
	movs r2, #20
	movs r0, #3
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #3
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #10
	movs r0, #19
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #7
	adds r1, r5, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r0, [r6]
	adds r1, r5, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #2
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_02005724
	movs r0, #128
	movs r1, #1
	movs r2, #160
	movs r3, #1
	lsls r2, r2, #17
	lsls r0, r0, #18
	negs r1, r1
	bl Motion_CamBounds
	bl Func_02005734
	movs r1, #208
	movs r0, #19
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #19
	movs r1, #3
	bl Object_SetModeById
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	ldr r0, [r6]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #3
	movs r1, #3
	bl Object_SetModeById
	movs r0, #2
	movs r1, #3
	bl Object_SetModeById
	movs r0, #1
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #19
	bl Func_0200571c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r0, #7
	movs r1, #0
	bl Func_020056f4
	movs r1, #176
	movs r0, #19
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #19
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #4
	movs r2, #20
	adds r1, #255
	movs r0, #1
	bl Func_02005714
	movs r1, #0
	movs r0, #1
	bl Func_020056f4
	movs r0, #28
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r1, #128
	movs r2, #180
	lsls r2, r2, #17
	lsls r1, r1, #18
	movs r0, #28
	bl Func_0200567c
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02005764
	movs r0, #23
	movs r1, #24
	bl Func_0200578c
	movs r0, #28
	movs r1, #0
	bl Func_020056f4
	bl Func_02005794
	movs r0, #2
	adds r1, r5, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #10
	movs r0, #2
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #2
	movs r1, #0
	bl Func_020056f4
	ldr r0, [r6]
	adds r1, r5, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #7
	adds r1, r5, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	adds r1, r5, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	adds r1, r5, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #19
	mov r1, r8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #1
	adds r1, r5, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r0, #3
	adds r1, r5, #0
	bl ObjectMotion_ArmCallback
	movs r0, #0
	adds r1, r5, #0
	bl Func_02005704
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #0
	bl Func_02005714
	movs r0, #0
	movs r1, #0
	bl Func_020056f4
	movs r0, #23
	movs r1, #24
	bl Func_0200578c
	movs r0, #28
	movs r1, #0
	bl Func_020056f4
	bl Func_02005794
	ldr r0, [r6]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	ldr r0, [r6]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #0
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #0
	ldr r0, [r6]
	adds r1, r5, #0
	bl ObjectMotion_ArmCallback
	movs r0, #0
	adds r1, r5, #0
	bl Func_02005704
	movs r0, #23
	movs r1, #24
	bl Func_0200578c
	movs r1, #0
	movs r0, #28
	bl Func_020056f4
	bl Func_02005794
	movs r0, #5
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #7
	adds r7, r0, #0
	strh r3, [r7, #6]
	movs r1, #22
	movs r0, #5
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r0, #5
	movs r1, #0
	bl Func_020056f4
	movs r0, #23
	movs r1, #24
	bl Func_0200578c
	movs r0, #28
	movs r1, #0
	bl Func_020056f4
	bl Func_02005794
	strh r5, [r7, #6]
	movs r0, #5
	movs r1, #1
	bl Object_SetModeById
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #5
	bl Func_02005714
	movs r0, #5
	movs r1, #0
	bl Func_020056f4
	movs r0, #23
	movs r1, #24
	bl Func_0200578c
	movs r0, #28
	movs r1, #0
	bl Func_020056f4
	bl Func_02005794
	movs r0, #1
	movs r1, #0
	bl Func_020056f4
	movs r0, #23
	movs r1, #24
	bl Func_0200578c
	movs r0, #28
	movs r1, #0
	bl Func_020056f4
	bl Func_02005794
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #1
	bl Func_02005714
	movs r0, #1
	movs r1, #0
	bl Func_020056f4
	movs r0, #23
	movs r1, #24
	bl Func_0200578c
	movs r0, #28
	movs r1, #0
	bl Func_020056f4
	bl Func_02005794
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #19
	bl Func_02005714
	movs r0, #19
	b .L_0200c4a0
	.2byte 0x0000
.L_0200c498:
	.4byte 0x021a0000
.L_0200c49c:
	.4byte 0x00002e44
.L_0200c4a0:
	movs r1, #0
	bl Func_020056f4
	movs r0, #23
	movs r1, #24
	bl Func_0200578c
	movs r1, #0
	movs r0, #28
	bl Func_020056f4
	bl Func_02005794
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #1
	movs r2, #148
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r1, #160
	movs r0, #19
	lsls r1, r1, #7
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r2, #40
	movs r0, #19
	mov r1, r8
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #19
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	bl Func_02005704
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #5
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	bl Func_020056f4
	movs r0, #7
	movs r1, #0
	bl Func_02005704
	movs r0, #7
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	bl Func_02005704
	movs r1, #10
	movs r2, #40
	adds r1, #255
	movs r0, #2
	bl Func_02005714
	movs r1, #0
	movs r0, #2
	bl UiText_OpenMessageAtObject
	ldr r0, [r6]
	movs r1, #0
	bl Func_02005704
	movs r1, #0
	ldr r0, [r6]
	bl Inventory_PromptAndSetObjectMode
	ldr r0, [r6]
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #4
	bl Func_0200575c
	movs r0, #40
	bl Func_0200576c
	movs r0, #78
	bl Func_020057dc
	movs r0, #141
	bl Func_020057dc
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #9
	lsls r0, r0, #9
	bl Func_020055c4
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Func_02000610
	movs r0, #5
	bl Func_02000610
	movs r0, #6
	bl Func_02000610
	movs r0, #7
	bl Func_02000610
	movs r0, #19
	bl Func_02000610
	movs r0, #0
	bl Func_02000610
	movs r0, #1
	bl Func_02000610
	movs r0, #2
	bl Func_02000610
	movs r0, #3
	bl Func_02000610
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #204
	lsls r1, r1, #6
	ldr r0, .L_0200c9d0
	adds r1, #51
	bl Func_02005724
	movs r0, #128
	movs r1, #1
	movs r2, #144
	lsls r2, r2, #17
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02005734
	ldr r0, .L_0200c9d4
	bl Scheduler_RemoveCallbackFar
	movs r0, #140
	bl Func_020057dc
	movs r0, #8
	bl Object_GetById
	movs r1, #12
	bl Object_SetPartAttribute
	movs r0, #9
	bl Object_GetById
	movs r1, #12
	bl Object_SetPartAttribute
	movs r1, #2
	movs r0, #23
	bl ObjectMotion_SetActionVariant
	movs r0, #23
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #23
	bl Object_GetById
	ldr r3, .L_0200c9d8
	adds r7, r0, #0
	str r3, [r7, #28]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #24]
	adds r3, r7, #0
	adds r3, #85
	mov r2, r10
	strb r2, [r3]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r2, #128
	movs r1, #128
	movs r3, #204
	lsls r1, r1, #18
	lsls r2, r2, #15
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	movs r6, #0
.L_0200c66c:
	cmp r6, #20
	bne .L_0200c72a
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_020055c4
	ldr r5, .L_0200c9dc
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r5]
	bl Func_02005714
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_02005714
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_02005714
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_02005714
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #19
	bl Func_02005714
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #0
	bl Func_02005714
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #1
	bl Func_02005714
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #2
	bl Func_02005714
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #3
	bl Func_02005714
	ldr r0, [r5]
	bl Func_02000640
	movs r0, #5
	bl Func_02000640
	movs r0, #6
	bl Func_02000640
	movs r0, #7
	bl Func_02000640
	movs r0, #19
	bl Func_02000640
	movs r0, #0
	bl Func_02000640
	movs r0, #1
	bl Func_02000640
	movs r0, #2
	bl Func_02000640
	movs r0, #3
	bl Func_02000640
.L_0200c72a:
	ldr r3, [r7, #24]
	movs r2, #128
	lsls r2, r2, #3
	adds r3, r3, r2
	str r3, [r7, #24]
	movs r0, #1
	adds r6, #1
	bl WaitFrames
	cmp r6, #39
	bls .L_0200c66c
	movs r0, #63
	bl Func_020057dc
	ldr r1, .L_0200c9e0
	movs r0, #23
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #80
	bl Battle_WaitMode0
	movs r0, #1
	movs r1, #4
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #1
	movs r1, #6
	bl ObjectMotion_Launch
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #1
	movs r1, #0
	bl Func_020056f4
	movs r0, #7
	movs r1, #0
	bl Func_020056f4
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #6
	movs r1, #0
	bl Func_020056f4
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	ldr r5, .L_0200c9dc
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #20
	movs r0, #3
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #3
	movs r1, #0
	bl Func_020056f4
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	movs r0, #3
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #3
	movs r1, #3
	bl Object_SetModeById
	movs r0, #2
	movs r1, #3
	bl Object_SetModeById
	movs r0, #1
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	movs r1, #1
	movs r2, #156
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl Func_0200567c
	movs r0, #9
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r1, #1
	movs r0, #9
	bl ObjectMotion_SetActionVariant
	movs r0, #9
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #253
	ands r3, r2
	strb r3, [r0]
	movs r0, #140
	bl Func_020057dc
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl Func_020055c4
	movs r1, #0
	ldr r0, .L_0200c9e4
	bl Func_0200575c
	movs r0, #40
	bl Func_0200576c
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	b .L_0200c9e8
.L_0200c9d0:
	.4byte 0x00019999
.L_0200c9d4:
	.4byte Func_020004a4
.L_0200c9d8:
	.4byte 0xffff0000
.L_0200c9dc:
	.4byte gPartyState
.L_0200c9e0:
	.4byte Data_020061d4
.L_0200c9e4:
	.4byte 0x00203210
.L_0200c9e8:
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #80
	bl ObjectMotion_ArmCallback
	movs r0, #1
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	ldr r1, .L_0200cbbc
	ldr r2, .L_0200cbc0
	movs r0, #1
	bl ObjectMotion_SetSpeedParameters
	movs r0, #1
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	movs r1, #128
	ands r3, r2
	lsls r1, r1, #2
	movs r2, #164
	strb r3, [r0]
	adds r1, #30
	lsls r2, r2, #1
	movs r0, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #1
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r0, #1
	bl Func_020056f4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #4
	movs r2, #0
	adds r1, #255
	movs r0, #0
	bl Func_02005714
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	bl Func_02005704
	movs r2, #10
	movs r0, #0
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #5
	movs r1, #0
	bl Func_020056f4
	movs r0, #1
	movs r1, #4
	bl Object_SetModeById
	movs r0, #1
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #3
	bl Func_02005714
	movs r0, #3
	movs r1, #0
	bl Func_020056f4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #7
	bl Func_0200571c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	bl Func_020056f4
	movs r2, #10
	movs r0, #5
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #5
	movs r1, #0
	bl Func_020056f4
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #2
	movs r1, #0
	bl Func_020056f4
	movs r1, #208
	movs r0, #19
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #19
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #19
	movs r1, #4
	bl ObjectMotion_Launch
	movs r1, #0
	movs r0, #19
	bl Func_020056f4
	movs r0, #149
	lsls r0, r0, #1
	bl Func_020057dc
	movs r0, #78
	bl Func_020057dc
	movs r0, #1
	bl WaitFrames
	movs r0, #140
	bl Func_020057dc
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_0200575c
	movs r0, #20
	bl Func_0200576c
	movs r0, #240
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #19
	movs r6, #0
	strh r6, [r3]
	movs r1, #0
	movs r0, #0
	bl Func_0200575c
	movs r0, #8
	bl Func_0200576c
	movs r0, #8
	bl WaitFrames
	movs r0, #4
	bl Func_02005744
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200cbbc:
	.4byte 0x00033333
.L_0200cbc0:
	.4byte 0x00019999
	.section .text.x0200cbc4,"ax",%progbits
	.global Func_02004bc4
	.thumb_func
Func_02004bc4:
	push {r5, lr}
	bl Func_0200560c
	movs r0, #0
	bl Func_02005784
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #19
	bl Func_02005704
	ldr r0, .L_0200cc24
	bl Func_020056dc
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #19
	movs r1, #0
	bl Func_020056f4
	ldr r5, .L_0200cc28
	movs r3, #133
	lsls r3, r3, #2
	movs r1, #204
	movs r2, #204
	adds r5, r5, r3
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #240
	movs r2, #174
	ldr r0, [r5]
	lsls r2, r2, #1
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #176
	movs r0, #19
	lsls r1, r1, #8
	bl Func_02005704
	bl Func_02005614
	pop {r5, pc}
.L_0200cc24:
	.4byte 0x00002e22
.L_0200cc28:
	.4byte gPartyState
	.section .text.x0200cc2c,"ax",%progbits
	.global Func_02004c2c
	.thumb_func
Func_02004c2c:
	push {r5, lr}
	bl Func_0200560c
	movs r0, #0
	bl Func_02005784
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #19
	bl Func_02005704
	ldr r0, .L_0200cc8c
	bl Func_020056dc
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #19
	movs r1, #0
	bl Func_020056f4
	ldr r5, .L_0200cc90
	movs r3, #133
	lsls r3, r3, #2
	movs r1, #204
	movs r2, #204
	adds r5, r5, r3
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #136
	movs r2, #174
	ldr r0, [r5]
	lsls r2, r2, #1
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #176
	movs r0, #19
	lsls r1, r1, #8
	bl Func_02005704
	bl Func_02005614
	pop {r5, pc}
.L_0200cc8c:
	.4byte 0x00002e22
.L_0200cc90:
	.4byte gPartyState
	.section .text.x0200cc94,"ax",%progbits
	.global Func_02004c94
	.thumb_func
Func_02004c94:
	push {lr}
	bl Func_0200560c
	movs r0, #0
	bl Func_02005784
	movs r1, #128
	movs r2, #232
	lsls r2, r2, #16
	lsls r1, r1, #18
	movs r0, #18
	bl Func_0200567c
	movs r0, #1
	bl WaitFrames
	ldr r0, .L_0200cd0c
	bl Func_020056dc
	movs r1, #0
	movs r0, #18
	bl UiText_OpenMessageAtObject
	ldr r3, .L_0200cd10
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200ccea
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_0200567c
	movs r0, #1
	bl WaitFrames
	bl Func_02003658
.L_0200ccea:
	movs r1, #0
	movs r2, #0
	movs r0, #18
	bl Func_0200567c
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	bl Func_02005614
	pop {pc}
	.2byte 0x0000
.L_0200cd0c:
	.4byte 0x00002e23
.L_0200cd10:
	.4byte gPartyState
	.section .text.x0200cd14,"ax",%progbits
	.global Func_02004d14
	.thumb_func
Func_02004d14:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_ClearBit
	pop {pc}
	.2byte 0x0000
	.section .text.x0200cd24,"ax",%progbits
	.global Func_02004d24
	.thumb_func
Func_02004d24:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #60]
	movs r1, #128
	ldr r3, .L_0200cd4c
	lsls r1, r1, #2
	adds r1, #106
	adds r3, r3, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, r0
	bne .L_0200cd4a
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #138
	adds r2, r2, r3
	movs r3, #1
	strb r3, [r2]
.L_0200cd4a:
	pop {pc}
.L_0200cd4c:
	.4byte gPartyState
	.section .text.x0200cd50,"ax",%progbits
	.global Func_02004d50
	.thumb_func
Func_02004d50:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #60]
	movs r4, #128
	ldr r3, .L_0200cd98
	lsls r4, r4, #2
	adds r4, #106
	adds r2, r3, r4
	movs r3, #0
	ldrsh r6, [r2, r3]
	ldrh r4, [r2]
	cmp r6, r0
	bne .L_0200cd9c
	movs r0, #152
	lsls r0, r0, #5
	adds r0, #138
	adds r3, r1, r0
	movs r5, #0
	strb r5, [r3]
	ldr r3, .L_0200cd94
	adds r0, r6, #0
	orrs r3, r4
	strh r3, [r2]
	bl Object_GetById
	str r5, [r0, #108]
	adds r0, r6, #0
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	b .L_0200cd9c
.L_0200cd94:
	.4byte 0x0000ffff
.L_0200cd98:
	.4byte gPartyState
.L_0200cd9c:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200cda0,"ax",%progbits
	.global Func_02004da0
	.thumb_func
Func_02004da0:
	push {lr}
	bl Func_0200560c
	movs r0, #0
	bl Func_02005784
	movs r0, #2
	bl Func_02004d24
	ldr r3, .L_0200cde8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #2
	bl ObjectMotion_SetAngleToward
	ldr r0, .L_0200cdec
	bl Func_020056dc
	movs r0, #2
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #2
	bl Func_02005704
	movs r0, #2
	bl Func_02004d50
	bl Func_02005614
	pop {pc}
.L_0200cde8:
	.4byte gPartyState
.L_0200cdec:
	.4byte 0x00002e19
	.section .text.x0200cdf0,"ax",%progbits
	.global Func_02004df0
	.thumb_func
Func_02004df0:
	push {lr}
	bl Func_0200560c
	movs r0, #0
	bl Func_02005784
	movs r0, #20
	bl Func_02004d24
	ldr r0, .L_0200ce1c
	bl Func_020056dc
	movs r1, #0
	movs r0, #20
	bl Func_020056f4
	movs r0, #20
	bl Func_02004d50
	bl Func_02005614
	pop {pc}
.L_0200ce1c:
	.4byte 0x00002e20
	.section .text.x0200ce20,"ax",%progbits
	.global Func_02004e20
	.thumb_func
Func_02004e20:
	push {lr}
	bl Func_0200560c
	movs r0, #0
	bl Func_02005784
	movs r0, #22
	bl Func_02004d24
	ldr r0, .L_0200ce4c
	bl Func_020056dc
	movs r1, #0
	movs r0, #22
	bl Func_020056f4
	movs r0, #22
	bl Func_02004d50
	bl Func_02005614
	pop {pc}
.L_0200ce4c:
	.4byte 0x00002e21
	.section .text.x0200ce50,"ax",%progbits
	.global Func_02004e50
	.thumb_func
Func_02004e50:
	push {lr}
	bl Func_0200560c
	movs r0, #0
	bl Func_02005784
	movs r0, #21
	bl Func_02004d24
	ldr r0, .L_0200ce7c
	bl Func_020056dc
	movs r1, #0
	movs r0, #21
	bl Func_020056f4
	movs r0, #21
	bl Func_02004d50
	bl Func_02005614
	pop {pc}
.L_0200ce7c:
	.4byte 0x00002e1f
	.section .text.x0200ce80,"ax",%progbits
	.global Func_02004e80
	.thumb_func
Func_02004e80:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r6, r1, #0
	bl Object_GetById
	ldr r3, .L_0200cebc
	movs r1, #181
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	movs r5, #0
	strb r5, [r2]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #106
	adds r3, r3, r2
	mov r8, r0
	strh r6, [r3]
	adds r0, r6, #0
	bl Object_GetById
	mov r1, r8
	ldr r3, [r1, #108]
	str r3, [r0, #108]
	str r5, [r1, #108]
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200cebc:
	.4byte gPartyState
	.section .text.x0200cec0,"ax",%progbits
	.global Func_02004ec0
	.thumb_func
Func_02004ec0:
	push {lr}
	movs r0, #28
	movs r1, #2
	bl Func_02004e80
	pop {pc}
	.section .text.x0200cecc,"ax",%progbits
	.global Func_02004ecc
	.thumb_func
Func_02004ecc:
	push {lr}
	movs r0, #17
	movs r1, #20
	bl Func_02004e80
	pop {pc}
	.section .text.x0200ced8,"ax",%progbits
	.global Func_02004ed8
	.thumb_func
Func_02004ed8:
	push {lr}
	movs r0, #30
	movs r1, #22
	bl Func_02004e80
	pop {pc}
	.section .text.x0200cee4,"ax",%progbits
	.global Func_02004ee4
	.thumb_func
Func_02004ee4:
	push {lr}
	movs r0, #29
	movs r1, #21
	bl Func_02004e80
	pop {pc}
	.section .text.x0200cef0,"ax",%progbits
	.global Func_02004ef0
	.thumb_func
Func_02004ef0:
	push {r5, r6, lr}
	adds r4, r1, #0
	movs r1, #192
	lsls r1, r1, #18
	adds r1, #128
	ldr r6, [r1]
	ldr r1, .L_0200cf94
	adds r5, r0, #0
	str r5, [r1]
	ldr r1, .L_0200cf98
	str r4, [r1]
	ldr r1, .L_0200cf9c
	str r2, [r1]
	ldr r2, .L_0200cfa0
	str r3, [r2]
	movs r2, #255
	ldrh r3, [r5]
	b .L_0200cf3a
.L_0200cf14:
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
.L_0200cf3a:
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_0200cf48
	ldrh r3, [r4]
	cmp r3, r2
	bne .L_0200cf14
.L_0200cf48:
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
	ldr r0, .L_0200cfa4
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_0200575c
	ldr r3, .L_0200cfa8
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #2
	bne .L_0200cf92
	bl Func_020051e0
.L_0200cf92:
	pop {r5, r6, pc}
.L_0200cf94:
	.4byte gOverlayArea + 0x6ce0
.L_0200cf98:
	.4byte gOverlayArea + 0x6ce4
.L_0200cf9c:
	.4byte gOverlayArea + 0x6ce8
.L_0200cfa0:
	.4byte gOverlayArea + 0x6cd4
.L_0200cfa4:
	.4byte 0x05000200
.L_0200cfa8:
	.4byte gPartyState
	.section .text.x0200cfac,"ax",%progbits
	.global Func_02004fac
	.thumb_func
Func_02004fac:
	push {r5, lr}
	ldr r2, .L_0200cff0
	ldr r3, .L_0200cfe0
	ldr r5, .L_0200cff4
	strh r3, [r2]
	ldr r3, .L_0200cff8
	ldr r0, [r3]
	bl Func_0200508c
	ldr r2, .L_0200cffc
	ldr r3, .L_0200cfe4
	strh r0, [r5]
	strh r3, [r2]
	ldr r2, .L_0200d000
	ldr r3, .L_0200cfe8
	movs r1, #144
	strh r3, [r2]
	ldr r2, .L_0200d004
	ldr r3, .L_0200cfec
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_0200d008
	bl Scheduler_AddOrUpdateCallback
	b .L_0200d00c
	.2byte 0x0000
.L_0200cfe0:
	.4byte 0x00000000
.L_0200cfe4:
	.4byte 0x0000000f
.L_0200cfe8:
	.4byte 0x00000010
.L_0200cfec:
	.4byte 0x00000001
.L_0200cff0:
	.4byte gOverlayArea + 0x6cf0
.L_0200cff4:
	.4byte gOverlayArea + 0x6cec
.L_0200cff8:
	.4byte gOverlayArea + 0x6ce0
.L_0200cffc:
	.4byte gOverlayArea + 0x6cdc
.L_0200d000:
	.4byte gOverlayArea + 0x6cd8
.L_0200d004:
	.4byte gOverlayArea + 0x6cd0
.L_0200d008:
	.4byte Func_020050b0
.L_0200d00c:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200d010,"ax",%progbits
	.global Func_02005010
	.thumb_func
Func_02005010:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	ldr r0, .L_0200d02c
	bl Scheduler_RemoveCallbackFar
	pop {pc}
	.2byte 0x0000
.L_0200d02c:
	.4byte Func_020050b0
	.section .text.x0200d030,"ax",%progbits
	.global Func_02005030
	.thumb_func
Func_02005030:
	push {r5, lr}
	ldr r2, .L_0200d06c
	ldr r3, .L_0200d060
	ldr r5, .L_0200d070
	strh r3, [r2]
	ldr r3, .L_0200d074
	ldr r0, [r3]
	bl Func_0200508c
	ldr r2, .L_0200d064
	ldr r3, .L_0200d078
	strh r0, [r5]
	strh r2, [r3]
	ldr r3, .L_0200d07c
	movs r1, #144
	strh r2, [r3]
	ldr r2, .L_0200d080
	ldr r3, .L_0200d068
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_0200d084
	bl Scheduler_AddOrUpdateCallback
	b .L_0200d088
.L_0200d060:
	.4byte 0x00000000
.L_0200d064:
	.4byte 0x00000002
.L_0200d068:
	.4byte 0x00000001
.L_0200d06c:
	.4byte gOverlayArea + 0x6cf0
.L_0200d070:
	.4byte gOverlayArea + 0x6cec
.L_0200d074:
	.4byte gOverlayArea + 0x6ce0
.L_0200d078:
	.4byte gOverlayArea + 0x6cdc
.L_0200d07c:
	.4byte gOverlayArea + 0x6cd8
.L_0200d080:
	.4byte gOverlayArea + 0x6cd0
.L_0200d084:
	.4byte Func_020050b0
.L_0200d088:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200d08c,"ax",%progbits
	.global Func_0200508c
	.thumb_func
Func_0200508c:
	push {lr}
	ldr r1, .L_0200d0a4
	ldrh r3, [r0]
	movs r2, #0
	cmp r3, r1
	beq .L_0200d0a8
.L_0200d098:
	adds r0, #2
	ldrh r3, [r0]
	adds r2, #1
	cmp r3, r1
	bne .L_0200d098
	b .L_0200d0a8
.L_0200d0a4:
	.4byte 0x0000ffff
.L_0200d0a8:
	subs r2, #1
	adds r0, r2, #0
	pop {pc}
	.2byte 0x0000
	.section .text.x0200d0b0,"ax",%progbits
	.global Func_020050b0
	.thumb_func
Func_020050b0:
	push {r5, r6, r7, lr}
	ldr r1, .L_0200d160
	movs r4, #0
	ldrh r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0200d0ea
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r2, r0
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200d0ea
	ldr r0, .L_0200d164
	movs r4, #1
	ldrh r2, [r0]
	strh r2, [r1]
	movs r1, #128
	lsls r3, r2, #16
	lsls r1, r1, #10
	cmp r3, r1
	bls .L_0200d0ea
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
	strh r3, [r0]
.L_0200d0ea:
	cmp r4, #0
	bne .L_0200d0f0
	b .L_0200d1dc
.L_0200d0f0:
	ldr r3, .L_0200d168
	ldr r6, .L_0200d16c
	ldr r1, [r3]
	ldrh r3, [r6]
	movs r5, #0
	cmp r5, r3
	bcs .L_0200d13e
	ldr r3, .L_0200d170
	ldr r2, .L_0200d174
	ldr r7, [r3]
	mov lr, r2
	mov r12, r6
.L_0200d108:
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
	bcc .L_0200d108
.L_0200d13e:
	ldr r3, .L_0200d16c
	movs r0, #160
	ldrh r1, [r3]
	ldr r3, .L_0200d178
	lsls r2, r1, #1
	ldr r3, [r3]
	lsls r0, r0, #19
	ldrh r3, [r2, r3]
	adds r2, r2, r1
	lsls r3, r3, #1
	adds r4, r3, r0
	ldr r3, .L_0200d17c
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_0200d184
	ldr r3, .L_0200d180
	b .L_0200d186
.L_0200d160:
	.4byte gOverlayArea + 0x6cd8
.L_0200d164:
	.4byte gOverlayArea + 0x6cdc
.L_0200d168:
	.4byte gOverlayArea + 0x6ce8
.L_0200d16c:
	.4byte gOverlayArea + 0x6cec
.L_0200d170:
	.4byte gOverlayArea + 0x6cd4
.L_0200d174:
	.4byte gOverlayArea + 0x6cf0
.L_0200d178:
	.4byte gOverlayArea + 0x6ce0
.L_0200d17c:
	.4byte gOverlayArea + 0x6cd0
.L_0200d180:
	.4byte gOverlayArea + 0x6ce4
.L_0200d184:
	ldr r3, .L_0200d1cc
.L_0200d186:
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
	ldr r1, .L_0200d1d0
	ldr r2, .L_0200d1c4
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	ldr r1, .L_0200d1d4
	ldr r2, .L_0200d1d8
	ldrh r3, [r1]
	adds r3, #1
	strh r3, [r1]
	lsls r3, r3, #16
	ldrh r2, [r2]
	lsrs r3, r3, #16
	cmp r3, r2
	bne .L_0200d1dc
	ldr r3, .L_0200d1c8
	strh r3, [r1]
	b .L_0200d1dc
	.2byte 0x0000
.L_0200d1c4:
	.4byte 0x00000001
.L_0200d1c8:
	.4byte 0x00000000
.L_0200d1cc:
	.4byte gOverlayArea + 0x6ce8
.L_0200d1d0:
	.4byte gOverlayArea + 0x6cd0
.L_0200d1d4:
	.4byte gOverlayArea + 0x6cf0
.L_0200d1d8:
	.4byte gOverlayArea + 0x6cec
.L_0200d1dc:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200d1e0,"ax",%progbits
	.global Func_020051e0
	.thumb_func
Func_020051e0:
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
	ldr r0, .L_0200d260
	movs r1, #1
	mov r10, r2
	bl Func_0200575c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	bl Func_0200575c
	ldr r2, [r6, #108]
	movs r3, #128
	lsls r3, r3, #1
	mov r1, r8
	str r3, [r2, r1]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02005030
	bl Func_020057c4
	movs r0, #40
	bl WaitFrames
	bl Func_02005010
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl Func_0200575c
	movs r0, #16
	bl Func_0200576c
	movs r0, #16
	bl WaitFrames
	ldr r3, .L_0200d264
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
.L_0200d260:
	.4byte 0x00202108
.L_0200d264:
	.4byte gPartyState
	.section .text.x0200d268,"ax",%progbits
	.global Func_02005268
	.thumb_func
Func_02005268:
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
	beq .L_0200d2a2
	ldrb r0, [r6]
	movs r1, #6
	lsrs r0, r0, #1
	bl Engine_MathModulo
	adds r1, r0, #0
	lsls r1, r1, #24
	lsrs r1, r1, #24
	adds r0, r7, #0
	bl Animation_ApplyChildValues
.L_0200d2a2:
	adds r3, r7, #0
	adds r3, #98
	ldrb r5, [r3]
	cmp r5, #0
	bne .L_0200d2e0
	ldrb r2, [r6]
	movs r3, #15
	ands r3, r2
	cmp r3, #0
	bne .L_0200d316
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #86
	bl Func_020057dc
	mov r1, r8
	adds r1, #166
	movs r2, #0
	ldrsh r3, [r1, r2]
	mov r2, r8
	lsls r3, r3, #1
	adds r3, #160
	strh r5, [r2, r3]
	ldr r2, .L_0200d2dc
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	b .L_0200d316
	.2byte 0x0000
.L_0200d2dc:
	.4byte 0x00000001
.L_0200d2e0:
	cmp r5, #1
	bne .L_0200d316
	mov r3, r8
	adds r3, #160
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #31
	ble .L_0200d316
	mov r3, r8
	adds r3, #162
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #31
	ble .L_0200d316
	adds r0, r7, #0
	movs r1, #0
	bl Animation_ApplyChildValues
	mov r3, r8
	adds r3, #164
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Resource_ResetEntry
	movs r3, #0
	str r3, [r7, #108]
	b .L_0200d3d0
.L_0200d316:
	ldrb r3, [r6]
	movs r2, #1
	adds r3, #1
	strb r3, [r6]
	movs r3, #0
	str r3, [sp, #0]
	mov r6, r8
	mov r11, r2
	adds r6, #160
.L_0200d328:
	movs r3, #0
	ldrsh r0, [r6, r3]
	lsls r0, r0, #10
	bl Math_Sine
	str r0, [sp, #4]
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #0
	blt .L_0200d3bc
	cmp r3, #31
	bgt .L_0200d3bc
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
	bl Func_0200579c
	ldr r2, [r5]
	movs r3, #0
	str r2, [sp, #8]
	mov r10, r3
	ldr r5, [r5, #8]
	mov r9, r5
	ldr r5, [sp, #0]
	add r5, r8
.L_0200d36c:
	ldr r2, [sp, #8]
	mov r3, r9
	str r3, [r5, #16]
	str r2, [r5, #12]
	ldr r2, [sp, #4]
	mov r3, r10
	str r2, [r5, #20]
	str r2, [r5, #24]
	cmp r3, #0
	bne .L_0200d38c
	adds r0, r7, #0
	bl Func_020057bc
	subs r0, #1
	strh r0, [r5, #30]
	b .L_0200d3a4
.L_0200d38c:
	adds r0, r7, #0
	bl Func_020057bc
	ldr r3, [r5, #16]
	ldr r2, .L_0200d3e0
	adds r0, #1
	adds r3, r3, r2
	str r3, [r5, #16]
	ldr r3, [r5, #24]
	strh r0, [r5, #30]
	negs r3, r3
	str r3, [r5, #24]
.L_0200d3a4:
	adds r0, r5, #0
	bl Func_020057ac
	movs r3, #1
	add r10, r3
	mov r2, r10
	adds r5, #40
	cmp r2, #1
	ble .L_0200d36c
	ldrh r3, [r6]
	adds r3, #1
	strh r3, [r6]
.L_0200d3bc:
	ldr r3, [sp, #0]
	movs r2, #1
	negs r2, r2
	adds r3, #80
	add r11, r2
	str r3, [sp, #0]
	mov r3, r11
	adds r6, #2
	cmp r3, #0
	bge .L_0200d328
.L_0200d3d0:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200d3e0:
	.4byte 0xffff0000
	.section .text.x0200d3e4,"ax",%progbits
	.global Func_020053e4
	.thumb_func
Func_020053e4:
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
	ldr r2, .L_0200d440
	mov r3, r8
	strh r0, [r3]
	movs r1, #128
	lsls r0, r0, #16
	mov r10, r2
	lsls r1, r1, #1
	ldr r2, .L_0200d444
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
	b .L_0200d448
	.2byte 0x0000
.L_0200d440:
	.4byte 0x00000000
.L_0200d444:
	.4byte Data_020057e4
.L_0200d448:
	strb r0, [r3]
	ldr r3, .L_0200d4d8
	mov r0, r8
	str r3, [r6, #108]
	movs r1, #0
	ldrsh r3, [r0, r1]
	ldr r2, .L_0200d4dc
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	str r7, [r6, #104]
	lsrs r3, r3, #5
	mov r11, r3
	mov r9, r5
	mov r10, r5
.L_0200d466:
	movs r1, #1
	mov r2, r10
	mov r8, r1
	adds r5, r2, r7
.L_0200d46e:
	mov r3, r11
	str r3, [sp, #0]
	adds r0, r5, #0
	movs r1, #16
	movs r2, #16
	ldr r3, .L_0200d4e0
	bl Func_020057a4
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
	bl Func_020057b4
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
	bge .L_0200d46e
	movs r1, #1
	add r9, r1
	movs r0, #80
	mov r2, r9
	add r10, r0
	cmp r2, #1
	ble .L_0200d466
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200d4d8:
	.4byte Func_02005268
.L_0200d4dc:
	.4byte ResourceTableEntries
.L_0200d4e0:
	.4byte 0x80004000
	.section .text.x0200d4e4,"ax",%progbits
	.global Func_020054e4
	.thumb_func
Func_020054e4:
	adds r0, #98
	movs r3, #1
	strb r3, [r0]
	bx lr
	.section .rodata.x0200d7e4,"a",%progbits
	.global Data_020057e4
Data_020057e4:
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
	.global Data_02005aa4
Data_02005aa4:
	.4byte 0x008a0089
	.4byte 0x008c008b
	.4byte 0x008e008d
	.4byte 0xffff008f
	.global Data_02005ab4
Data_02005ab4:
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
	.global Data_02005ae0
Data_02005ae0:
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
	.global Data_02005b0c
Data_02005b0c:
	.4byte 0x0089008e
	.4byte 0x008b008a
	.4byte 0x008d008c
	.4byte 0x008e008d
	.4byte 0x008a0089
	.4byte 0x008c008b
	.4byte 0x008d008c
	.4byte 0x0089008e
	.4byte 0x008b008a
	.4byte 0x008c008b
	.4byte 0x008e008d
	.4byte 0x008a0089
	.4byte 0x008b008a
	.4byte 0x008d008c
	.4byte 0x0089008e
	.4byte 0x008a0089
	.4byte 0x008c008b
	.4byte 0x008e008d
	.global Data_02005b54
Data_02005b54:
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffa00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000600
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0020000
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_02005ba8
Data_02005ba8:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01f00000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005bd0
Data_02005bd0:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01e00000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005bf8
Data_02005bf8:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005c20
Data_02005c20:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02100000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005c48
Data_02005c48:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte Data_02000000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005c70
Data_02005c70:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000040
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005cac
Data_02005cac:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000200
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00000200
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000080
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_02005ce8
Data_02005ce8:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01f40000
	.4byte 0x00000000
	.4byte 0x01320000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02005d2c
Data_02005d2c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x020c0000
	.4byte 0x00000000
	.4byte 0x01320000
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
	.4byte 0x00000011
	.global Data_02005d70
Data_02005d70:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02005db4
Data_02005db4:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02005df8
Data_02005df8:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x01180000
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
	.global Data_02005e3c
Data_02005e3c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x021a0000
	.4byte 0x00000000
	.4byte 0x01400000
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
	.4byte 0x00000011
	.global Data_02005e80
Data_02005e80:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01300000
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
	.4byte 0x00000011
	.global Data_02005ec4
Data_02005ec4:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x01180000
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
	.global Data_02005f08
Data_02005f08:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005f50
Data_02005f50:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01dc0000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005f98
Data_02005f98:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01e60000
	.4byte 0x00000000
	.4byte 0x011e0000
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
	.4byte 0x00000011
	.global Data_02005fdc
Data_02005fdc:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02006004
Data_02006004:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02100000
	.4byte 0x00000000
	.4byte 0x01240000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_0200602c
Data_0200602c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x01180000
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
	.global Data_02006070
Data_02006070:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02240000
	.4byte 0x00000000
	.4byte 0x01240000
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
	.4byte 0x00000011
	.global Data_020060b4
Data_020060b4:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc80
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000008c
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_020060fc
Data_020060fc:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc80
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x000000a0
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_02006120
Data_02006120:
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02006150
Data_02006150:
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00020000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x000000a0
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02006180
Data_02006180:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000010
	.4byte 0xc0010000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_020061d4
Data_020061d4:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000008
	.4byte 0xc0010000
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000c0
	.4byte 0x800000c0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x00000129
	.4byte 0x0010111f
	.4byte 0x00203129
	.4byte 0x00302135
	.4byte 0x00414110
	.4byte 0x000001ff
	.global Data_0200626c
Data_0200626c:
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02480000
	.4byte 0x00100000
	.4byte 0x01b80000
	.4byte 0x00024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02680000
	.4byte 0x00100000
	.4byte 0x01b80000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01b80000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff00b6
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0137
	.4byte 0x00000007
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
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0007
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
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0002
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
	.global Data_0200644c
Data_0200644c:
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00b6
	.4byte 0x00000007
	.4byte Data_02000000
	.4byte 0x00000000
	.4byte 0x00ec0000
	.4byte 0x00024000
	.4byte 0xffff0137
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x02200000
	.4byte 0x00000000
	.4byte 0x01620000
	.4byte 0x0002b000
	.4byte 0xffff0030
	.4byte 0x00000001
	.4byte 0x01f00000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0xffff0031
	.4byte 0x00000001
	.4byte 0x01e00000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0xffff0032
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x0002c000
	.4byte 0xffff0033
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x0002c000
	.4byte 0xffff0034
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x0002c000
	.4byte 0xffff0036
	.4byte 0x00000001
	.4byte 0x02200000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0xffff0035
	.4byte 0x00000001
	.4byte 0x02100000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0xffff00b7
	.4byte 0x00000007
	.4byte 0x01de0000
	.4byte 0x00000000
	.4byte 0x010a0000
	.4byte 0x00024000
	.4byte 0xffff00b7
	.4byte 0x00000007
	.4byte 0x01fa0000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00b7
	.4byte 0x00000007
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01140000
	.4byte 0x00024000
	.4byte 0xffff003d
	.4byte 0x00000007
	.4byte 0x01e00000
	.4byte 0x00000000
	.4byte 0x011a0000
	.4byte 0x00024000
	.4byte 0xffff003c
	.4byte 0x00000007
	.4byte 0x01fa0000
	.4byte 0x00000000
	.4byte 0x01240000
	.4byte 0x00024000
	.4byte 0xffff003e
	.4byte 0x00000007
	.4byte 0x021c0000
	.4byte 0x00000000
	.4byte 0x011c0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020066bc
Data_020066bc:
	.4byte 0xffff012f
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff012f
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02480000
	.4byte 0x00100000
	.4byte 0x01b80000
	.4byte 0x00024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02680000
	.4byte 0x00100000
	.4byte 0x01b80000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01b80000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002b000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x01ca0000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00010000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x01dc0000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0001e000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x01e20000
	.4byte 0x00000000
	.4byte 0x01140000
	.4byte 0x00014000
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x02110000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00010000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x021a0000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0001e000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01120000
	.4byte 0x00014000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x02360000
	.4byte 0x00000000
	.4byte 0x011e0000
	.4byte 0x00018000
	.4byte 0xffff003d
	.4byte 0x00000007
	.4byte 0x01e00000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00024000
	.4byte 0xffff003c
	.4byte 0x00000007
	.4byte 0x01f60000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff003e
	.4byte 0x00000007
	.4byte 0x02240000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00024000
	.4byte 0xffff0126
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0126
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0126
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0126
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0126
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff003b
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
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
	.global gSceneEvents
gSceneEvents:
	.4byte 0x00009c05
	.4byte 0xffff0001
	.4byte Func_020001a0
	.4byte 0x00000002
	.4byte 0x0a3d000a
	.4byte Func_0200065c
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00002e17
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00002e18
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x00002e1a
	.4byte 0x00000000
	.4byte 0xffff0005
	.4byte 0x00002e1b
	.4byte 0x00000000
	.4byte 0xffff0006
	.4byte 0x00002e1c
	.4byte 0x00000000
	.4byte 0xffff0007
	.4byte 0x00002e1d
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00002e1e
	.4byte 0x00004400
	.4byte 0xffff001c
	.4byte Func_02004da0
	.4byte 0x00004400
	.4byte 0xffff0011
	.4byte Func_02004df0
	.4byte 0x00006400
	.4byte 0xffff0011
	.4byte Func_02004df0
	.4byte 0x0000e400
	.4byte 0xffff001e
	.4byte Func_02004e20
	.4byte 0x00000400
	.4byte 0xffff001e
	.4byte Func_02004e20
	.4byte 0x00002400
	.4byte 0xffff001e
	.4byte Func_02004e20
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte Func_02004e50
	.4byte 0x00009115
	.4byte 0xffff001c
	.4byte Func_02004ec0
	.4byte 0x00009115
	.4byte 0xffff0011
	.4byte Func_02004ecc
	.4byte 0x00009115
	.4byte 0xffff001e
	.4byte Func_02004ed8
	.4byte 0x00009115
	.4byte 0xffff001d
	.4byte Func_02004ee4
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte Func_02004bc4
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte Func_02004c2c
	.4byte 0x00000002
	.4byte Data_02020004 + 0x12
	.4byte Func_02004c94
	.4byte 0x00000002
	.4byte 0x12020017
	.4byte Func_02004d14
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006ad0
Data_02006ad0:
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00016666
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00016666
	.4byte 0x00000027
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000026
	.global Data_02006afc
Data_02006afc:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000010
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
