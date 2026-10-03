.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008080,"ax",%progbits
	.global Func_02000080
	.thumb_func
Func_02000080:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl Func_02000c6c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020080ca
	movs r1, #0
	bl Object_SetSpritePriority
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
	movs r3, #8
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #14
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	adds r0, r5, #0
	b .L_020080cc
.L_020080ca:
	movs r0, #0
.L_020080cc:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020080d0,"ax",%progbits
	.global Func_020000d0
	.thumb_func
Func_020000d0:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl Func_02000c6c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200811e
	movs r1, #1
	bl Object_SetSpritePriority
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
	movs r3, #8
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #15
	bl Object_SetPartAttribute
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #34
	orrs r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	b .L_02008120
.L_0200811e:
	movs r0, #0
.L_02008120:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008124,"ax",%progbits
	.global Func_02000124
	.thumb_func
Func_02000124:
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
	.section .text.x0200815c,"ax",%progbits
	.global Func_0200015c
	.thumb_func
Func_0200015c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	str r3, [sp, #0]
	ldr r3, .L_0200832c
	mov r8, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r1, #0
	ldr r1, [sp, #44]
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	mov r10, r1
	ldr r7, [sp, #48]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	mov r1, r10
	ands r3, r1
	mov r9, r0
	cmp r3, #0
	beq .L_020081a4
	cmp r7, #0
	beq .L_020081a4
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_020081ac
.L_020081a4:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_020081ac:
	mov r3, r8
	bl Func_02000c6c
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020081ba
	b .L_0200831e
.L_020081ba:
	ldr r3, [r6, #80]
	mov r1, r10
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	mov r8, r3
	bl Func_02000c5c
	ldr r2, .L_02008330
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl Func_02000c64
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_02008334
	mov r1, r9
	str r3, [r6, #108]
	ldr r3, [sp, #0]
	adds r0, r6, #0
	str r3, [r6, #68]
	ldr r3, [sp, #36]
	str r3, [r6, #72]
	ldr r3, [sp, #40]
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl Object_SetSpritePriority
	movs r2, #100
	adds r2, r2, r6
	mov r9, r2
	mov r3, r9
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r3]
	ldr r3, .L_02008338
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200831e
	cmp r7, #0
	beq .L_0200831e
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200823c
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_0200823c:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02008274
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r3, #3
	ldrb r2, [r7]
	adds r0, r6, #0
	ands r2, r3
	mov r3, r8
	ldrb r1, [r3, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r1
	lsls r2, r2, #2
	mov r1, r8
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r1, [r7]
	bl Object_SetSpritePriority
.L_02008274:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r10
	ands r2, r3
	cmp r2, #0
	beq .L_02008288
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02008288:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_020082ce
	ldr r3, .L_02008330
	mov r1, r11
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_020082b6
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_020082c8
.L_020082b6:
	ldr r2, .L_02008338
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_02008338
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_020082c8:
	bl __divsi3
	str r0, [r6, #52]
.L_020082ce:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_020082ea
	adds r0, r6, #0
	movs r1, #1
	bl Func_02000c5c
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02000c64
.L_020082ea:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_020082fc
	ldrh r3, [r7, #32]
	mov r1, r8
	strh r3, [r1, #18]
.L_020082fc:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200830e
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_0200830e:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200831e
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200831e:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200832c:
	.4byte gPartyState
.L_02008330:
	.4byte Data_02001964
.L_02008334:
	.4byte Func_02000124
.L_02008338:
	.4byte 0xffff0000
	.section .text.x0200833c,"ax",%progbits
	.global Func_0200033c
	.thumb_func
Func_0200033c:
	push {lr}
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #0
	pop {pc}
	.section .text.x0200835c,"ax",%progbits
	.global Func_0200035c
	.thumb_func
Func_0200035c:
	push {lr}
	ldr r3, .L_02008378
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200837c
	cmp r2, r3
	bne .L_02008374
	ldr r0, .L_02008380
	b .L_02008376
.L_02008374:
	ldr r0, .L_02008384
.L_02008376:
	pop {pc}
.L_02008378:
	.4byte gPartyState
.L_0200837c:
	.4byte 0x00000134
.L_02008380:
	.4byte Data_020019f4
.L_02008384:
	.4byte Data_020019dc
	.section .text.x02008388,"ax",%progbits
	.global Func_02000388
	.thumb_func
Func_02000388:
	push {lr}
	bl Func_02000cb4
	movs r0, #0
	bl Func_02000d44
	movs r1, #2
	movs r0, #8
	bl ObjectMotion_SetVariantCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02000d24
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #3
	bl Func_02000d1c
	movs r0, #60
	bl Func_02000d2c
	movs r0, #60
	bl Battle_WaitMode0
	ldr r0, .L_02008400
	movs r1, #0
	movs r2, #6
	bl Func_02000ca4
	ldr r2, .L_02008404
	movs r3, #149
	lsls r3, r3, #2
	adds r1, r2, r3
	movs r3, #205
	lsls r3, r3, #5
	adds r3, #255
	strh r3, [r1]
	movs r3, #166
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r2, r3
	movs r3, #2
	strb r3, [r2]
	movs r0, #103
	movs r1, #3
	bl Func_02000d04
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	pop {pc}
	.2byte 0x0000
.L_02008400:
	.4byte 0x000030ac
.L_02008404:
	.4byte gPartyState
	.section .text.x02008408,"ax",%progbits
	.global Func_02000408
	.thumb_func
Func_02000408:
	push {lr}
	movs r0, #15
	bl Object_GetById
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r0, #24]
	str r3, [r0, #28]
	bl Func_02000cb4
	movs r0, #0
	bl Func_02000d44
	movs r0, #151
	bl Func_02000d64
	movs r0, #8
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #15
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #190
	bl Func_02000d64
	movs r1, #8
	movs r0, #15
	bl Func_02000cdc
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_02008488
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200848c
	movs r0, #15
	bl Object_SetActionCallbackAndRefreshById
	ldr r1, .L_02008490
	movs r0, #15
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02000cbc
	pop {pc}
	.2byte 0x0000
.L_02008488:
	.4byte Data_02000ea4
.L_0200848c:
	.4byte Data_02000e20
.L_02008490:
	.4byte Data_02000e5c
	.section .text.x02008494,"ax",%progbits
	.global Func_02000494
	.thumb_func
Func_02000494:
	push {r5, lr}
	movs r5, #9
.L_02008498:
	adds r0, r5, #0
	bl Object_GetById
	adds r1, r0, #0
	adds r1, #98
	movs r2, #0
	movs r3, #1
	adds r5, #1
	strb r3, [r1]
	str r2, [r0, #108]
	cmp r5, #14
	ble .L_02008498
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020084b4,"ax",%progbits
	.global Func_020004b4
	.thumb_func
Func_020004b4:
	push {r5, lr}
	adds r5, r0, #0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	ldr r3, .L_020084e0
	str r0, [r5, #20]
	cmp r0, r3
	blt .L_020084d4
	adds r0, r5, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	b .L_020084dc
.L_020084d4:
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
.L_020084dc:
	pop {r5, pc}
	.2byte 0x0000
.L_020084e0:
	.4byte 0xffe00000
	.section .text.x020084e4,"ax",%progbits
	.global Func_020004e4
	.thumb_func
Func_020004e4:
	push {r5, r6, r7, lr}
	ldr r6, .L_02008530
	movs r5, #9
	adds r7, r6, #1
.L_020084ec:
	adds r0, r5, #0
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r2, #0
	ldrsb r2, [r6, r2]
	asrs r3, r3, #20
	cmp r3, r2
	bne .L_0200852c
	ldr r3, [r0, #16]
	movs r2, #0
	ldrsb r2, [r7, r2]
	asrs r3, r3, #20
	adds r7, #2
	adds r6, #2
	cmp r3, r2
	bne .L_0200852c
	adds r5, #1
	cmp r5, #14
	ble .L_020084ec
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #201
	bl Func_02000d64
	movs r0, #1
	bl Func_02000be0
	bl Func_02000494
.L_0200852c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008530:
	.4byte Data_02000ee0
	.section .text.x02008534,"ax",%progbits
	.global Func_02000534
	.thumb_func
Func_02000534:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_02000d4c
	ldr r3, .L_02008580
	cmp r0, #0
	beq .L_02008564
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Func_02000d0c
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_0200857c
	bl Func_02000d54
	movs r0, #0
	adds r1, r5, #0
	bl Func_020004e4
	b .L_0200857c
.L_02008564:
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r2, .L_02008584
	ldr r3, [r0, #12]
	cmp r3, r2
	ble .L_0200857c
	bl Func_02000d14
.L_0200857c:
	pop {r5, pc}
	.2byte 0x0000
.L_02008580:
	.4byte gPartyState
.L_02008584:
	.4byte 0xffe40000
	.section .text.x02008588,"ax",%progbits
	.global Func_02000588
	.thumb_func
Func_02000588:
	push {r5, r6, r7, lr}
	ldr r3, .L_020086fc
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #16]
	movs r7, #152
	asrs r3, r3, #20
	lsls r7, r7, #17
	cmp r3, #9
	bgt .L_020085a8
	negs r7, r7
.L_020085a8:
	movs r0, #0
	bl Func_02000d44
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020085bc
	b .L_020086f8
.L_020085bc:
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Func_02000c8c
	cmp r0, #77
	beq .L_020085d0
	b .L_020086f8
.L_020085d0:
	ldr r0, [r6]
	movs r1, #1
	bl Object_SetModeById
	ldr r2, .L_02008700
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	movs r0, #128
	lsls r0, r0, #12
	ands r1, r2
	ands r3, r2
	adds r1, r1, r0
	adds r3, r3, r0
	ldr r2, [r5, #12]
	adds r0, r5, #0
	bl Func_02000c74
	ldr r0, [r6]
	cmp r7, #0
	ble .L_02008604
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	b .L_0200860e
.L_02008604:
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
.L_0200860e:
	movs r0, #10
	bl WaitFrames
	movs r6, #0
.L_02008616:
	ldr r3, .L_02008704
	ldrsb r0, [r3, r6]
	bl Object_GetById
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r2, #128
	ldr r3, [r5, #12]
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r5, #12]
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, [r5, #8]
	ldr r2, .L_02008708
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Func_02000c74
	ldr r3, .L_0200870c
	movs r0, #152
	str r3, [r5, #108]
	bl Func_02000d64
	bl Random16Far
	lsls r0, r0, #2
	lsrs r0, r0, #16
	adds r0, #3
	adds r6, #1
	bl WaitFrames
	cmp r6, #6
	ble .L_02008616
	movs r0, #40
	bl WaitFrames
	movs r0, #198
	bl Func_02000d64
	movs r6, #9
.L_0200867c:
	adds r0, r6, #0
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #16]
	ldr r1, [r5, #8]
	subs r3, r3, r7
	ldr r2, .L_02008708
	adds r6, #1
	bl Func_02000c74
	cmp r6, #14
	ble .L_0200867c
	movs r0, #9
	bl Object_GetById
	bl Func_02000c7c
	movs r0, #40
	bl WaitFrames
	movs r6, #9
.L_020086a8:
	adds r0, r6, #0
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #16]
	ldr r2, .L_02008700
	adds r1, r5, #0
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5, #16]
	adds r1, #85
	movs r2, #0
	movs r3, #3
	adds r6, #1
	strb r3, [r1]
	str r2, [r5, #108]
	cmp r6, #14
	ble .L_020086a8
	movs r0, #3
	bl WaitFrames
	movs r0, #188
	bl Func_02000d64
	movs r0, #10
	bl WaitFrames
	movs r0, #154
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020086f8
	cmp r7, #0
	ble .L_020086f8
	bl Func_02000388
.L_020086f8:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020086fc:
	.4byte gPartyState
.L_02008700:
	.4byte 0xfff00000
.L_02008704:
	.4byte Data_02000eec
.L_02008708:
	.4byte 0xfff80000
.L_0200870c:
	.4byte Func_020004b4
	.4byte 0x00004770
	.section .text.x02008714,"ax",%progbits
	.global Func_02000714
	.thumb_func
Func_02000714:
	push {lr}
	ldr r3, .L_02008730
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008734
	cmp r2, r3
	bne .L_0200872c
	ldr r0, .L_02008738
	b .L_0200872e
.L_0200872c:
	ldr r0, .L_0200873c
.L_0200872e:
	pop {pc}
.L_02008730:
	.4byte gPartyState
.L_02008734:
	.4byte 0x00000134
.L_02008738:
	.4byte Data_02001c28
.L_0200873c:
	.4byte Data_02001c1c
	.section .text.x02008740,"ax",%progbits
	.global Func_02000740
	.thumb_func
Func_02000740:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	ldr r5, .L_0200884c
	str r2, [r3]
	subs r2, #36
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008850
	cmp r2, r3
	bne .L_02008822
	bl Func_02000b18
	movs r0, #2
	bl Func_02000bd4
	movs r1, #0
	movs r0, #9
	bl Func_02000ba4
	movs r1, #1
	movs r0, #10
	bl Func_02000ba4
	movs r1, #2
	movs r0, #11
	bl Func_02000ba4
	movs r1, #3
	movs r0, #12
	bl Func_02000ba4
	movs r1, #128
	movs r0, #13
	bl Func_02000ba4
	movs r1, #129
	movs r0, #14
	bl Func_02000ba4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #192
	orrs r3, r2
	strb r3, [r0]
	movs r0, #10
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #128
	orrs r3, r5
	strb r3, [r0]
	movs r0, #11
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #12
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #13
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #14
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008822
	movs r0, #1
	bl Func_02000be0
	bl Func_02000494
.L_02008822:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008846
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02000408
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
.L_02008846:
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
.L_0200884c:
	.4byte gPartyState
.L_02008850:
	.4byte 0x00000134
	.section .text.x02008858,"ax",%progbits
	.global Func_02000858
	.thumb_func
Func_02000858:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r3, .L_02008a68
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	mov r10, r0
	mov r8, r2
	mov r4, r10
	mov r0, r8
	ands r4, r3
	ands r0, r3
	ldr r3, .L_02008a6c
	mov r10, r4
	movs r4, #0
	ldrsh r3, [r3, r4]
	ldr r2, .L_02008a70
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #12
	lsrs r3, r3, #5
	str r3, [sp, #4]
	mov r8, r0
	ldr r3, [r1]
	movs r1, #5
	ldr r3, [r3, #4]
	ldr r7, .L_02008a74
	str r3, [sp, #0]
	ldr r3, .L_02008a78
	movs r0, #0
	ldrsh r5, [r3, r0]
	ldr r3, .L_02008a7c
	ldr r0, [r3]
	lsrs r0, r0, #2
	bl __umodsi3
	lsls r0, r0, #5
	adds r0, #32
	adds r1, r0, #0
	muls r1, r5
	ldr r3, .L_02008a80
	str r1, [sp, #8]
	movs r2, #0
	movs r4, #0
	ldrsh r3, [r3, r4]
	mov r9, r2
	cmp r9, r3
	blt .L_020088ce
	b .L_02008a5a
.L_020088ce:
	ldr r2, .L_02008a84
	mov r0, r9
	lsls r3, r0, #2
	ldr r6, [r2, r3]
	cmp r6, #0
	bne .L_020088dc
	b .L_02008a4a
.L_020088dc:
	ldr r3, [r6, #8]
	cmp r3, #0
	bne .L_020088e4
	b .L_02008a4a
.L_020088e4:
	mov r1, r10
	subs r5, r3, r1
	ldr r2, [sp, #0]
	ldr r3, [r6, #12]
	ldr r1, [sp, #0]
	subs r3, r3, r2
	ldr r2, [r6, #16]
	mov r0, r8
	movs r4, #128
	lsls r4, r4, #12
	subs r2, r2, r0
	subs r2, r2, r1
	adds r3, r3, r4
	subs r4, r2, r3
	adds r3, r3, r2
	asrs r3, r3, #16
	adds r3, #58
	mov r11, r3
	ldr r3, .L_02008a88
	asrs r1, r5, #16
	movs r0, #0
	ldrsh r2, [r3, r0]
	adds r3, r6, #0
	mov r12, r2
	asrs r2, r4, #16
	mov r4, r12
	adds r3, #100
	cmp r4, #0
	bne .L_020089b2
	movs r4, #0
	ldrsh r0, [r3, r4]
	adds r5, r1, #0
	adds r3, r1, #7
	movs r1, #167
	adds r4, r2, #0
	lsls r1, r1, #1
	subs r5, #8
	subs r4, #16
	cmp r3, r1
	bls .L_02008936
	b .L_02008a4a
.L_02008936:
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	bgt .L_02008940
	b .L_02008a4a
.L_02008940:
	cmp r4, #239
	ble .L_02008946
	b .L_02008a4a
.L_02008946:
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r5, r3
	movs r3, #255
	adds r1, r7, #0
	ands r4, r3
	mov r3, r12
	stmia r1!, {r3}
	lsls r3, r5, #16
	orrs r4, r3
	movs r3, #128
	ands r3, r0
	lsls r3, r3, #21
	orrs r4, r3
	ldr r3, .L_02008a8c
	orrs r4, r3
	stmia r1!, {r4}
	ldr r4, [sp, #4]
	ldr r3, [sp, #8]
	adds r2, r4, r3
	movs r3, #7
	ands r0, r3
	lsls r3, r0, #3
	adds r2, r2, r3
	movs r3, #128
	lsls r3, r3, #4
	orrs r2, r3
	ldr r3, .L_02008a90
	str r2, [r1]
	ldrh r2, [r3]
	movs r4, #0
	ldrsh r3, [r3, r4]
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	bne .L_020089a4
	adds r0, r6, #0
	bl Func_02000d5c
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r7, #9]
	negs r1, r1
	adds r2, r1, #0
	b .L_02008a22
.L_020089a4:
	movs r3, #3
	ands r3, r2
	movs r4, #13
	ldrb r2, [r7, #9]
	negs r4, r4
	adds r1, r4, #0
	b .L_02008a38
.L_020089b2:
	movs r4, #0
	ldrsh r0, [r3, r4]
	adds r5, r1, #0
	adds r3, r1, #0
	movs r1, #175
	adds r4, r2, #0
	adds r3, #23
	lsls r1, r1, #1
	subs r5, #8
	subs r4, #64
	cmp r3, r1
	bhi .L_02008a4a
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_02008a4a
	cmp r4, #175
	bgt .L_02008a4a
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r5, r3
	movs r3, #255
	adds r1, r7, #0
	ands r4, r3
	movs r3, #0
	stmia r1!, {r3}
	lsls r3, r5, #16
	orrs r4, r3
	ldr r3, .L_02008a94
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r4, [sp, #4]
	lsls r3, r0, #3
	lsls r2, r2, #4
	adds r3, r4, r3
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_02008a90
	movs r1, #1
	ldrh r2, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	negs r1, r1
	cmp r3, r1
	bne .L_02008a2c
	adds r0, r6, #0
	bl Func_02000d5c
	movs r3, #3
	ands r0, r3
	movs r4, #13
	ldrb r3, [r7, #9]
	negs r4, r4
	adds r2, r4, #0
.L_02008a22:
	lsls r0, r0, #2
	ands r3, r2
	orrs r3, r0
	strb r3, [r7, #9]
	b .L_02008a40
.L_02008a2c:
	movs r3, #3
	ands r3, r2
	movs r0, #13
	ldrb r2, [r7, #9]
	negs r0, r0
	adds r1, r0, #0
.L_02008a38:
	lsls r3, r3, #2
	ands r2, r1
	orrs r2, r3
	strb r2, [r7, #9]
.L_02008a40:
	adds r0, r7, #0
	mov r1, r11
	bl Func_02000c3c
	adds r7, #12
.L_02008a4a:
	ldr r3, .L_02008a80
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_02008a5a
	b .L_020088ce
.L_02008a5a:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008a68:
	.4byte 0xffff0000
.L_02008a6c:
	.4byte Data_02001cdc
.L_02008a70:
	.4byte ResourceTableEntries
.L_02008a74:
	.4byte Data_02001d20
.L_02008a78:
	.4byte Data_02001de4
.L_02008a7c:
	.4byte gFrameCount
.L_02008a80:
	.4byte Data_02001cde
.L_02008a84:
	.4byte Data_02001ce0
.L_02008a88:
	.4byte Data_02001de0
.L_02008a8c:
	.4byte 0x40002000
.L_02008a90:
	.4byte Data_02001de2
.L_02008a94:
	.4byte 0xc000a000
	.section .text.x02008a98,"ax",%progbits
	.global Func_02000a98
	.thumb_func
Func_02000a98:
	push {r5, r6, lr}
	movs r0, #192
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_02008af8
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_02008afc
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_02008b00
	bl Func_02000c24
	ldr r5, .L_02008b04
	bl Resource_FindFreeEntry
	movs r1, #192
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008b08
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_02008b0c
	ldr r2, .L_02008af0
	strh r2, [r3]
	ldr r3, .L_02008b10
	strh r2, [r3]
	ldr r2, .L_02008b14
	ldr r3, .L_02008af4
	strh r3, [r2]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008af0:
	.4byte 0x00000000
.L_02008af4:
	.4byte 0xffffffff
.L_02008af8:
	.4byte IwramClearWords
.L_02008afc:
	.4byte Data_02001ce0
.L_02008b00:
	.4byte Data_02000ef2
.L_02008b04:
	.4byte Data_02001cdc
.L_02008b08:
	.4byte Func_02000858
.L_02008b0c:
	.4byte Data_02001cde
.L_02008b10:
	.4byte Data_02001de0
.L_02008b14:
	.4byte Data_02001de2
	.section .text.x02008b18,"ax",%progbits
	.global Func_02000b18
	.thumb_func
Func_02000b18:
	push {r5, r6, lr}
	movs r0, #192
	lsls r0, r0, #5
	bl Runtime_BumpAllocate
	ldr r3, .L_02008b7c
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_02008b80
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_02008b84
	bl Func_02000c24
	ldr r5, .L_02008b88
	bl Resource_FindFreeEntry
	movs r1, #192
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #5
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008b8c
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_02008b74
	ldr r3, .L_02008b90
	ldr r1, .L_02008b94
	strh r2, [r3]
	ldr r3, .L_02008b98
	strh r2, [r3]
	ldr r3, .L_02008b78
	strh r3, [r1]
	ldr r3, .L_02008b9c
	strh r2, [r3]
	b .L_02008ba0
	.2byte 0x0000
.L_02008b74:
	.4byte 0x00000000
.L_02008b78:
	.4byte 0xffffffff
.L_02008b7c:
	.4byte IwramClearWords
.L_02008b80:
	.4byte Data_02001ce0
.L_02008b84:
	.4byte Data_02001054 + 0x1
.L_02008b88:
	.4byte Data_02001cdc
.L_02008b8c:
	.4byte Func_02000858
.L_02008b90:
	.4byte Data_02001cde
.L_02008b94:
	.4byte Data_02001de2
.L_02008b98:
	.4byte Data_02001de0
.L_02008b9c:
	.4byte Data_02001de4
.L_02008ba0:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008ba4,"ax",%progbits
	.global Func_02000ba4
	.thumb_func
Func_02000ba4:
	push {r5, lr}
	adds r5, r1, #0
	bl Object_GetById
	adds r4, r0, #0
	cmp r4, #0
	beq .L_02008bca
	adds r3, r4, #0
	adds r3, #100
	strh r5, [r3]
	ldr r1, .L_02008bcc
	ldr r0, .L_02008bd0
	ldrh r2, [r1]
	movs r5, #0
	ldrsh r3, [r1, r5]
	adds r2, #1
	lsls r3, r3, #2
	str r4, [r0, r3]
	strh r2, [r1]
.L_02008bca:
	pop {r5, pc}
.L_02008bcc:
	.4byte Data_02001cde
.L_02008bd0:
	.4byte Data_02001ce0
	.section .text.x02008bd4,"ax",%progbits
	.global Func_02000bd4
	.thumb_func
Func_02000bd4:
	ldr r3, .L_02008bdc
	strh r0, [r3]
	bx lr
	.2byte 0x0000
.L_02008bdc:
	.4byte Data_02001de2
	.section .text.x02008be0,"ax",%progbits
	.global Func_02000be0
	.thumb_func
Func_02000be0:
	ldr r3, .L_02008be8
	strh r0, [r3]
	bx lr
	.2byte 0x0000
.L_02008be8:
	.4byte Data_02001de4
	.section .rodata.x02008d6c,"a",%progbits
.L_02008d6c:
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
.L_02008da8:
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
.L_02008de4:
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
	.global Data_02000e20
Data_02000e20:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000400
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00000400
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xffffc000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000028
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_02000e5c
Data_02000e5c:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffff800
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000018
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02000ea4
Data_02000ea4:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000028
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02000ee0
Data_02000ee0:
	.4byte 0x1c0b1b0b
	.4byte 0x1c0c1b0c
	.4byte 0x1c0d1b0d
	.global Data_02000eec
Data_02000eec:
	.4byte 0x0b090e0c
	.2byte 0x0a0d
	.global Data_02000ef2
Data_02000ef2:
	.2byte 0x5d01
	.4byte 0x3b010634
	.4byte 0x00260800
	.4byte 0x70002f01
	.4byte 0x09065f1f
	.4byte 0x0901667b
	.4byte 0x78000104
	.4byte 0x3b3f0e28
	.4byte 0x080e5605
	.4byte 0x08005f6f
	.4byte 0x66010810
	.4byte 0x0800a107
	.4byte 0x1768001d
	.4byte 0x15bb0807
	.4byte 0x00166600
	.4byte 0x70000e02
	.4byte 0x080d6617
	.4byte 0x10df3b02
	.4byte 0x80100200
	.4byte 0x7a016612
	.4byte 0x79100103
	.4byte 0x013d0427
	.4byte 0x7800fb44
	.4byte 0x04140200
	.4byte 0x27002059
	.4byte 0x20665705
	.4byte 0x00165000
	.4byte 0x2a0020ff
	.4byte 0x002e0016
	.4byte 0x00200016
	.4byte 0x2000162a
	.4byte 0x00404e00
	.4byte 0x023b7f32
	.4byte 0x000604aa
	.4byte 0x92070250
	.4byte 0xe103e716
	.4byte 0xff0d0010
	.4byte 0xe7090301
	.4byte 0xdd030c01
	.4byte 0x61185906
	.4byte 0x0030210a
	.4byte 0xa425fd34
	.4byte 0x06300403
	.4byte 0x01005f29
	.4byte 0x3b580249
	.4byte 0x04aff806
	.4byte 0xb4063b57
	.4byte 0x200e163b
	.4byte 0xa00f0800
	.4byte 0x20ff283e
	.4byte 0x00100708
	.4byte 0x07f82e05
	.4byte 0x04880578
	.4byte 0x0ba80a98
	.4byte 0x0030dd66
	.4byte 0x5f220708
	.4byte 0x17063820
	.4byte 0x3b583348
	.4byte 0xfe780060
	.4byte 0x6918783f
	.4byte 0x79000807
	.4byte 0x27001f01
	.4byte 0x5f110a78
	.4byte 0x07a82aff
	.4byte 0x3000701f
	.4byte 0x5704ef27
	.4byte 0x20370050
	.4byte 0x00404c00
	.4byte 0x002af700
	.4byte 0x206e0040
	.4byte 0x00461100
	.4byte 0x0000403b
	.4byte 0x402c0020
	.4byte 0x19f72a00
	.4byte 0x40002448
	.4byte 0x08032d00
	.4byte 0x0722043b
	.4byte 0xb7050583
	.4byte 0x003b1817
	.4byte 0x59200028
	.4byte 0x22606601
	.4byte 0x041a0007
	.4byte 0x01ff6805
	.4byte 0x04583f1b
	.4byte 0x2a2c0615
	.4byte 0x09004018
	.4byte 0x03028f00
	.4byte 0xcd0ce013
	.4byte 0x00020030
	.global Data_02001054
Data_02001054:
	.4byte 0x7bbd0000
	.4byte 0xb781756f
	.4byte 0x0b053ef1
	.4byte 0xc54d76f6
	.4byte 0xf35db98a
	.4byte 0x829ea0bb
	.4byte 0x43dab61f
	.4byte 0xd36b7bf3
	.4byte 0x0b49b90e
	.4byte 0x915a6efe
	.4byte 0x8632a3d2
	.4byte 0xee04b405
	.4byte 0xa2cd00f9
	.4byte 0x2c181dde
	.4byte 0x7a2247b2
	.4byte 0x85c5343a
	.4byte 0xa6feb7a8
	.4byte 0x97845538
	.4byte 0xbf4c20a6
	.4byte 0x2a69eb7f
	.4byte 0xab110138
	.4byte 0x1ad2c1e1
	.4byte 0xbcc27573
	.4byte 0xc8a180de
	.4byte 0xc4cdcb79
	.4byte 0x73028041
	.4byte 0x3e1c5bf2
	.4byte 0x370f46e0
	.4byte 0x34d2908e
	.4byte 0x84802037
	.4byte 0xcae2783e
	.4byte 0xcd87c0f4
	.4byte 0x136bf0f4
	.4byte 0x28745e14
	.4byte 0x69120f90
	.4byte 0x3507ebc1
	.4byte 0x60018af2
	.4byte 0xf2f86f38
	.4byte 0x261f86f3
	.4byte 0x6f319c82
	.4byte 0x78ce027c
	.4byte 0xf9a973dd
	.4byte 0xd04e1b62
	.4byte 0x1713dc7a
	.4byte 0xb6118672
	.4byte 0x3398b23c
	.4byte 0xe3a90b8e
	.4byte 0x7834f2f8
	.4byte 0x07c2b8e7
	.4byte 0x00f8f240
	.4byte 0x67053e3c
	.4byte 0x01c3dcfc
	.4byte 0x041132b0
	.4byte 0x03c18382
	.4byte 0xd6dd8114
	.4byte 0xe8b11a79
	.4byte 0xd8fc7843
	.4byte 0xc8bc29f6
	.4byte 0xc781e5e6
	.4byte 0xc620bd7d
	.4byte 0xe0eb2c02
	.4byte 0x67705e9a
	.4byte 0x7d8be17c
	.4byte 0x23144ca0
	.4byte 0x2c816521
	.4byte 0xb87f0230
	.4byte 0x23101ec6
	.4byte 0x80f2f5a1
	.4byte 0x8479c501
	.4byte 0xb97a3801
	.4byte 0xe050cf88
	.4byte 0x20888241
	.4byte 0x56364a19
	.4byte 0x9285c8bb
	.4byte 0x01003d97
	.4byte 0x0e7c89f3
	.4byte 0xbd2c440e
	.4byte 0x445e9de4
	.4byte 0x84922162
	.4byte 0x42830810
	.4byte 0x02113ea7
	.4byte 0x141a701f
	.4byte 0x33a6720d
	.4byte 0xc05c60a4
	.4byte 0x1a837220
	.4byte 0x801c02e3
	.4byte 0x4450340b
	.4byte 0x667bc078
	.4byte 0xc0f13e2c
	.4byte 0x789f298c
	.4byte 0x6088e069
	.4byte 0x48233544
	.4byte 0x401c126f
	.4byte 0x29244f10
	.4byte 0x64586b42
	.4byte 0x10354e06
	.4byte 0x98f01501
	.4byte 0x1c3ab40a
	.4byte 0xe9343b78
	.4byte 0x4e3d8c79
	.4byte 0x1d006008
	.4byte 0x021869c8
	.4byte 0x422dc83c
	.4byte 0xdbd11484
	.4byte 0x109c64d8
	.4byte 0x30ae3c12
	.4byte 0x6f4279b1
	.4byte 0x382701fc
	.4byte 0xf2a94875
	.4byte 0x1613c780
	.4byte 0xc22e55c9
	.4byte 0x2f4af259
	.4byte 0x05413e26
	.4byte 0xa211c80c
	.4byte 0x9f5e4d93
	.4byte 0x15c711e1
	.4byte 0x213ce09c
	.4byte 0x042f98e2
	.4byte 0x1ac4e100
	.4byte 0x8d458061
	.4byte 0xc03a6090
	.4byte 0x2dd12809
	.4byte 0xe06fa262
	.4byte 0xe07df1a3
	.4byte 0x1af7ceba
	.4byte 0xb81343db
	.4byte 0xbabdf3b1
	.4byte 0xbe327d81
	.4byte 0x4ae532b7
	.4byte 0xea7a7be7
	.4byte 0x170f7d7a
	.4byte 0x4e1efaf0
	.4byte 0xaeaf7ce0
	.4byte 0xeaf7c84f
	.4byte 0xce4c407a
	.4byte 0x3df013e0
	.4byte 0xf9b0209c
	.4byte 0x87a06e1e
	.4byte 0xbe70471b
	.4byte 0xdf382787
	.4byte 0xbe3c15c3
	.4byte 0xf103e6c7
	.4byte 0xf985793d
	.4byte 0xb9c2bc9e
	.4byte 0x8f83799a
	.4byte 0x0670f7c5
	.4byte 0xd4b87baf
	.4byte 0x9c36c17c
	.4byte 0xdccf7cc0
	.4byte 0x67205c43
	.4byte 0x3393df30
	.4byte 0x19c9ef98
	.4byte 0x1d48171c
	.4byte 0x87be107c
	.4byte 0x93df380f
	.4byte 0xf9ef980f
	.4byte 0xa7c19c0c
	.4byte 0xbd2b7bec
	.4byte 0x4bd297be
	.4byte 0xef9cfa73
	.4byte 0x1ef8f5d5
	.4byte 0x4e18e0f4
	.4byte 0x4f7ce292
	.4byte 0xa1ef9f8f
	.4byte 0x48611e38
	.4byte 0x3e3df5e9
	.4byte 0x2909ef8f
	.4byte 0x2121ef9c
	.4byte 0x1113df1e
	.4byte 0xe3d38839
	.4byte 0x08507bef
	.4byte 0x80e0e3e4
	.4byte 0xbe720408
	.4byte 0x92162447
	.4byte 0x1844a5ef
	.4byte 0xfa7be428
	.4byte 0x00f84504
	.4byte 0x419141bf
	.4byte 0xc4f7c78e
	.4byte 0xef9d2b04
	.4byte 0xb7be71a9
	.4byte 0x757be3d2
	.4byte 0x3df229c5
	.4byte 0x9ef980b1
	.4byte 0x21880068
	.4byte 0x2380d2f2
	.4byte 0x9f2a2302
	.4byte 0x0be1ef80
	.4byte 0xa652f7ce
	.4byte 0x256586b4
	.4byte 0x06a9c1ae
	.4byte 0x3995104e
	.4byte 0x70ead015
	.4byte 0x8605dbc0
	.4byte 0xe443dc31
	.4byte 0xc0303df5
	.4byte 0x80c87be3
	.4byte 0x11064526
	.4byte 0x8e26c529
	.4byte 0x1e048413
	.4byte 0x3cf66057
	.4byte 0x7b019284
	.4byte 0x1d4e7be4
	.4byte 0xf2a68370
	.4byte 0x58278f00
	.4byte 0x11715724
	.4byte 0x4f7ce2ce
	.4byte 0xf7c34f9f
	.4byte 0x0ae30190
	.4byte 0x8279c09c
	.4byte 0x417cc8f0
	.4byte 0xb09c21a6
	.4byte 0xe2c2ce46
	.4byte 0x0f3df393
	.4byte 0xd87d4160
	.4byte 0x5383e623
	.4byte 0x1c63df30
	.4byte 0x7be2e7c5
	.4byte 0x7ac60760
	.4byte 0xe3d623dc
	.4byte 0x84b08f7c
	.4byte 0x04e0f7c7
	.4byte 0x8711ef9c
	.4byte 0x9ef8f9f1
	.4byte 0xc6207ec0
	.4byte 0x707be047
	.4byte 0xf7cd8d02
	.4byte 0x38f406e0
	.4byte 0xc7be631e
	.4byte 0x0f7c8d78
	.4byte 0x7be3c0ae
	.4byte 0xbe207cd8
	.4byte 0x7cc15e47
	.4byte 0x731f318f
	.4byte 0x7c0de635
	.4byte 0x6707be2c
	.4byte 0x2e0f75e0
	.4byte 0x1b605f35
	.4byte 0x8f7cc04e
	.4byte 0x170f07b9
	.4byte 0x6723df32
	.4byte 0xce47be60
	.4byte 0x870171c0
	.4byte 0xbe0c7c76
	.4byte 0x7ce01f07
	.4byte 0xf9807c8f
	.4byte 0x33819f1e
	.4byte 0x1efb29f0
	.4byte 0xf7c78f1c
	.4byte 0xdf31f398
	.4byte 0x1ef9cfa3
	.4byte 0x3df38f5c
	.4byte 0x06a3d71e
	.4byte 0x47be6277
	.4byte 0x41ef9f8f
	.4byte 0xc1847871
	.4byte 0xe1efaf4a
	.4byte 0x211ef8f3
	.4byte 0x907be705
	.4byte 0x23df1e10
	.4byte 0x8e20e222
	.4byte 0x63df7f1e
	.4byte 0x870bc67e
	.4byte 0xf7cc6804
	.4byte 0xc8858910
	.4byte 0x08e608f7
	.4byte 0x13ea70fa
	.4byte 0x1807c114
	.4byte 0x10069167
	.4byte 0x1ef8f0a8
	.4byte 0x90bc0131
	.4byte 0x1b2f1ef9
	.4byte 0x7c780382
	.4byte 0x0a71604f
	.4byte 0x00b11ef9
	.4byte 0x01a23df3
	.4byte 0x1fa10c40
	.4byte 0x44700d0f
	.4byte 0x4f8a88c0
	.4byte 0x0be0f7c0
	.4byte 0xc341ef9c
	.4byte 0x9682c358
	.4byte 0xee635e31
	.4byte 0xa98dbcc6
	.4byte 0x070ead00
	.4byte 0x72c6bb78
	.4byte 0x7910f639
	.4byte 0xc0180f7d
	.4byte 0x6ce47be3
	.4byte 0x0c884a24
	.4byte 0x71b01822
	.4byte 0xc048409c
	.4byte 0x3d980ae3
	.4byte 0x8064a10f
	.4byte 0x4e3df23d
	.4byte 0xc90fc81d
	.4byte 0x813c7803
	.4byte 0x2e157245
	.4byte 0xdf3859c2
	.4byte 0xe1a7cfa3
	.4byte 0x02139c7b
	.4byte 0xcc5b8031
	.4byte 0x988f0813
	.4byte 0xc848702f
	.4byte 0x2c167235
	.4byte 0xe3df393e
	.4byte 0x0fa82c01
	.4byte 0x707cc47b
	.4byte 0x8c7be60a
	.4byte 0x7c5cf8a3
	.4byte 0x58c0ec0f
	.4byte 0x7ac47b8f
	.4byte 0x9611ef9c
	.4byte 0xb11ef8f0
	.4byte 0xe23df32b
	.4byte 0xdf1f3e30
	.4byte 0xc40fd8d3
	.4byte 0x0f7c08f8
	.4byte 0xf9a5204e
	.4byte 0x1e80dc1e
	.4byte 0xf7cc63c7
	.4byte 0x4f087d38
	.4byte 0x7c7815c0
	.4byte 0x1f39998f
	.4byte 0x8231ef87
	.4byte 0xe63e65e7
	.4byte 0xf81bcc6a
	.4byte 0xce0f7c58
	.4byte 0x5c1eebc0
	.4byte 0x36c0be6a
	.4byte 0x1ef9809c
	.4byte 0x2e1e0f73
	.4byte 0x69c7be64
	.4byte 0x9c80983d
	.4byte 0x0e02e381
	.4byte 0x7c18f8ed
	.4byte 0xf997e08f
	.4byte 0x1e59f31e
	.4byte 0x0cefe137
	.4byte 0x07beca7c
	.4byte 0x3df1e3c7
	.4byte 0xf7cc7ce6
	.4byte 0x07be73e8
	.4byte 0x8f7ce3d7
	.4byte 0xc1a8f5c7
	.4byte 0xd1ef989d
	.4byte 0x0c7be7e3
	.4byte 0xe3d739c8
	.4byte 0x1e7c3df5
	.4byte 0xe0a423df
	.4byte 0xc2120f7c
	.4byte 0x44447be3
	.4byte 0xe3d1c41c
	.4byte 0x2b023df5
	.4byte 0x70bc67e6
	.4byte 0x7cc68048
	.4byte 0xf33a498f
	.4byte 0x8239823d
	.4byte 0x271e1e3e
	.4byte 0x38dbf3fa
	.4byte 0x4080348b
	.4byte 0x88f7c785
	.4byte 0xcc85e009
	.4byte 0x10d978f7
	.4byte 0x7be3c01c
	.4byte 0xc8538b1a
	.4byte 0x980588f7
	.4byte 0x000d11ef
	.4byte 0x78fd0862
	.4byte 0x02238068
	.4byte 0x027c5446
	.4byte 0xe05f07be
	.4byte 0xc61a0f7c
	.4byte 0x8cb4161a
	.4byte 0x37731af1
	.4byte 0x054c6de6
	.4byte 0xc0387568
	.4byte 0xba3189db
	.4byte 0x139ea8c7
	.4byte 0x8f006002
	.4byte 0x91b391ef
	.4byte 0x88322128
	.4byte 0x71c6c060
	.4byte 0x2d048702
	.4byte 0xb23cf662
	.4byte 0xbe479e86
	.4byte 0xf903a9c7
	.4byte 0x8f007921
	.4byte 0xae48b027
	.4byte 0x0b3845c2
	.4byte 0xf9f47be7
	.4byte 0x738f7c34
	.4byte 0x70062042
	.4byte 0xe102798b
	.4byte 0x0e05f311
	.4byte 0xce46b909
	.4byte 0xe727c582
	.4byte 0x05803c7b
	.4byte 0x988f61f5
	.4byte 0x7cc14e0f
	.4byte 0x3eb4138f
	.4byte 0x3b03df16
	.4byte 0x1ee3d630
	.4byte 0x7be71eb1
	.4byte 0xdf3afc4c
	.4byte 0xbe657623
	.4byte 0x9f5c09c7
	.4byte 0xe949ef8e
	.4byte 0x047c6207
	.4byte 0xd38a47be
	.4byte 0x24788f7c
	.4byte 0xe631e38f
	.4byte 0x843e9c7b
	.4byte 0x3ace8827
	.4byte 0xce6663df
	.4byte 0x8c7be1c7
	.4byte 0x8f9979e0
	.4byte 0x06f31ab9
	.4byte 0x83df163e
	.4byte 0x07baf033
	.4byte 0xb02f9a97
	.4byte 0xbe60270d
	.4byte 0x8783dcc7
	.4byte 0x71ef990b
	.4byte 0x20260f5a
	.4byte 0x80b8e067
	.4byte 0x063e3b43
	.4byte 0x65f823df
	.4byte 0x967cc7be
	.4byte 0x3bf84dc7
	.4byte 0xefb29f03
	.4byte 0x7c78f1c1
	.4byte 0xf31f398f
	.4byte 0xef9cfa3d
	.4byte 0xdf38f5c1
	.4byte 0x6a3d71e3
	.4byte 0x7be62770
	.4byte 0x7d7b6ccc
	.4byte 0xe739018f
	.4byte 0x87bebc7a
	.4byte 0x847be3cf
	.4byte 0x41ef9c14
	.4byte 0x8f7c7842
	.4byte 0x38838888
	.4byte 0x9f7dfc7a
	.4byte 0x1c2f19f9
	.4byte 0xdf31a012
	.4byte 0x7cce9263
	.4byte 0xa08e608f
	.4byte 0x89c7878f
	.4byte 0xce36fcfe
	.4byte 0x50200d22
	.4byte 0x623df1e1
	.4byte 0xf3217802
	.4byte 0x04365e3d
	.4byte 0x9ef8f007
	.4byte 0xf214e294
	.4byte 0xe601623d
	.4byte 0x8003447b
	.4byte 0x1e3f4218
	.4byte 0x8088e01a
	.4byte 0x809f1511
	.4byte 0x2c9e31ef
	.4byte 0xb1868149
	.4byte 0x632d0586
	.4byte 0x8ddcc6bc
	.4byte 0x18e6b9a3
	.4byte 0x070d52c4
	.4byte 0x46313b78
	.4byte 0x73d518f7
	.4byte 0xe00c0042
	.4byte 0x36723df1
	.4byte 0x06442512
	.4byte 0x38d80c11
	.4byte 0xa090e04e
	.4byte 0xdeb7f245
	.4byte 0x1eae3cd0
	.4byte 0x0ea71ef9
	.4byte 0x01e487e4
	.4byte 0x22c09e3c
	.4byte 0xe1170ab9
	.4byte 0xd1ef9c2c
	.4byte 0x3df0d3e7
	.4byte 0x188109ce
	.4byte 0x09e62dc0
	.4byte 0x17cc4784
	.4byte 0x1ae42438
	.4byte 0x9f160b39
	.4byte 0x00f1ef9c
	.4byte 0x3d87d416
	.4byte 0x05383e62
	.4byte 0xd04e3df3
	.4byte 0x0f7c58fa
	.4byte 0x8f58c0ec
	.4byte 0x9bfa0c7b
	.4byte 0xebf131ef
	.4byte 0x95d88f7c
	.4byte 0x70271ef9
	.4byte 0x67be3a7d
	.4byte 0xf1881fa5
	.4byte 0x291ef811
	.4byte 0xe23df34e
	.4byte 0xc78e3c91
	.4byte 0xfa71ef98
	.4byte 0x3a209e10
	.4byte 0x878f7ceb
	.4byte 0x7c30fbd7
	.4byte 0x2f3c118f
	.4byte 0xe35731f3
	.4byte 0xc54fbe08
	.4byte 0xbc0ce0f7
	.4byte 0xe6a5c1ee
	.4byte 0xdfb03c0b
	.4byte 0x07b98f7c
	.4byte 0xdf32170f
	.4byte 0x4c1eb4e3
	.4byte 0x71c0ce40
	.4byte 0x7c768701
	.4byte 0xf047be0c
	.4byte 0xf98f7ccb
	.4byte 0xf09b8f2c
	.4byte 0x653e0677
	.4byte 0xdfae63df
	.4byte 0x1f398f7c
	.4byte 0x9cfa3df3
	.4byte 0x38f5c1ef
	.4byte 0x3d71e3df
	.4byte 0xef9df1e6
	.4byte 0xf5edb331
	.4byte 0x9ce4063d
	.4byte 0x1efaf1eb
	.4byte 0x11ef8f3e
	.4byte 0xc7be7052
	.4byte 0x70e77bf9
	.4byte 0x8f5f103d
	.4byte 0x3f31efbf
	.4byte 0x024385e3
	.4byte 0x4c7be634
	.4byte 0x11ef99d2
	.4byte 0xf1f411cc
	.4byte 0x9fd138f0
	.4byte 0xcc59c6df
	.4byte 0x3c2a03c3
	.4byte 0xbb74c7be
	.4byte 0x365e3df3
	.4byte 0xf8f00704
	.4byte 0x14e2959e
	.4byte 0x01623df2
	.4byte 0x03447be6
	.4byte 0x3f421880
	.4byte 0x88e01a1e
	.4byte 0x9f151180
	.4byte 0x9e31ef80
	.4byte 0x8681492c
	.4byte 0xae4586b1
	.4byte 0xdcc6bc5e
	.4byte 0xe6b9a38d
	.4byte 0x0d52c418
	.4byte 0x313b7807
	.4byte 0xd518f746
	.4byte 0x0c004273
	.4byte 0x723df1e0
	.4byte 0x44251236
	.4byte 0xd80c1106
	.4byte 0x90e04e38
	.4byte 0xb7f245a0
	.4byte 0x7cb9d0de
	.4byte 0x809c1cb9
	.4byte 0x43f20753
	.4byte 0x4f1e00f2
	.4byte 0x855c9160
	.4byte 0xce16708b
	.4byte 0x69f3e8f7
	.4byte 0x84e71ef8
	.4byte 0x16e00c40
	.4byte 0x23c204f3
	.4byte 0x121c0be6
	.4byte 0x059c8d72
	.4byte 0xf7c4cf8b
	.4byte 0x000000f8
	.global Data_02001964
Data_02001964:
	.4byte .L_02008d6c
	.4byte .L_02008da8
	.4byte .L_02008de4
.L_02009970:
	.4byte 0x0000002e
	.4byte Func_0200033c
	.4byte 0x00000011
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
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
	.4byte 0x00000134
	.4byte 0x101170ef
	.4byte 0xffffffff
	.4byte 0x1020112f
	.4byte 0xffffffff
	.4byte 0x103160ef
	.4byte 0xffffffff
	.4byte 0x104180ef
	.4byte 0xffffffff
	.4byte 0x10505132
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_020019dc
Data_020019dc:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020019f4
Data_020019f4:
	.4byte 0x0a9f00b4
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00010000
	.4byte 0xffff01ae
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00010000
	.4byte 0xffff01ae
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01010000
	.4byte 0xffff01ae
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x01010000
	.4byte 0xffff01ae
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01010000
	.4byte 0xffff01ae
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01010000
	.4byte 0xffff01ae
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x01010000
	.4byte 0xffff012f
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01ad
	.4byte .L_02009970
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00018000
	.4byte 0xffff01ad
	.4byte .L_02009970
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01018000
	.4byte 0xffff01ad
	.4byte .L_02009970
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01010000
	.4byte 0xffff01ad
	.4byte .L_02009970
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01010000
	.4byte 0xffff01ad
	.4byte .L_02009970
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00018000
	.4byte 0xffff01ad
	.4byte .L_02009970
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00010000
	.4byte 0xffff01ad
	.4byte .L_02009970
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00018000
	.4byte 0xffff01ad
	.4byte .L_02009970
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00018000
	.4byte 0xffff01ad
	.4byte .L_02009970
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00010000
	.4byte 0xffff01ad
	.4byte .L_02009970
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00010000
	.4byte 0xffff01ad
	.4byte .L_02009970
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00018000
	.4byte 0xffff01ad
	.4byte .L_02009970
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x03000000
	.4byte 0x00018000
	.4byte 0xffff01ad
	.4byte .L_02009970
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00010000
	.4byte 0xffff01ad
	.4byte .L_02009970
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x03000000
	.4byte 0x00010000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001c1c
Data_02001c1c:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001c28
Data_02001c28:
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
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000202
	.4byte 0xffff0021
	.4byte Func_02000534
	.4byte 0x00000202
	.4byte 0xffff004d
	.4byte Func_02000534
	.4byte 0x00008c15
	.4byte 0x02000009
	.4byte Func_020004e4
	.4byte 0x00008c15
	.4byte 0x0200000a
	.4byte Func_020004e4
	.4byte 0x00008c15
	.4byte 0x0200000b
	.4byte Func_020004e4
	.4byte 0x00008c15
	.4byte 0x0200000c
	.4byte Func_020004e4
	.4byte 0x00008c15
	.4byte 0x0200000d
	.4byte Func_020004e4
	.4byte 0x00008c15
	.4byte 0x0200000e
	.4byte Func_020004e4
	.4byte 0x00009985
	.4byte 0x1200004d
	.4byte Func_02000588
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.section .bss,"aw",%nobits
	.global Data_02001cdc
Data_02001cdc:
	.space 0x00000002
	.global Data_02001cde
Data_02001cde:
	.space 0x00000002
	.global Data_02001ce0
Data_02001ce0:
	.space 0x00000040
	.global Data_02001d20
Data_02001d20:
	.space 0x000000c0
	.global Data_02001de0
Data_02001de0:
	.space 0x00000002
	.global Data_02001de2
Data_02001de2:
	.space 0x00000002
	.global Data_02001de4
Data_02001de4:
