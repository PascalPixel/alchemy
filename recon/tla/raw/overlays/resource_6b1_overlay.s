.syntax unified
	.thumb
	.section .text.x0200815a,"ax",%progbits
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
	bl Engine_ObjectCreate
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
	bl ObjectDispatch_SetSingleChildFlags
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
	.4byte gData_02001964
.L_02008334:
	.4byte BattleFx_UpdateObjectMotionScaleAndLinkedAngle
.L_02008338:
	.4byte 0xffff0000
	.section .text.x02008588,"ax",%progbits
	.balign 4
	.global Func_02000588
	.thumb_func
Func_02000588:
	.ifdef TLA_EDITION_JA
	push {r5, r6, r7, lr}
	ldr r3, .L_ja_020086e0
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6, #0]
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #16]
	movs r7, #152
	asrs r3, r3, #20
	lsls r7, r7, #17
	cmp r3, #9
	bgt .L_ja_020085a8
	negs r7, r7
.L_ja_020085a8:
	movs r0, #0
	bl Func_02000d44
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_020085bc
	b .L_ja_020086de
.L_ja_020085bc:
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3, #0]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Func_02000c8c
	cmp r0, #77
	beq .L_ja_020085d0
	b .L_ja_020086de
.L_ja_020085d0:
	ldr r0, [r6, #0]
	movs r1, #1
	bl Object_SetModeById
	ldr r0, [r6, #0]
	cmp r7, #0
	ble .L_ja_020085ea
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	b .L_ja_020085f4
.L_ja_020085ea:
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
.L_ja_020085f4:
	movs r0, #10
	bl WaitFrames
	movs r6, #0
.L_ja_020085fc:
	ldr r3, .L_ja_020086e4
	ldrsb r0, [r3, r6]
	bl Object_GetById
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2, #0]
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
	bl ObjectDispatch_SetSingleChildFlags
	ldr r1, [r5, #8]
	ldr r2, .L_ja_020086e8
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Func_02000c74
	ldr r3, .L_ja_020086ec
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
	ble .L_ja_020085fc
	movs r0, #40
	bl WaitFrames
	movs r0, #198
	bl Func_02000d64
	movs r6, #9
.L_ja_02008662:
	adds r0, r6, #0
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #16]
	ldr r1, [r5, #8]
	subs r3, r3, r7
	ldr r2, .L_ja_020086e8
	adds r6, #1
	bl Func_02000c74
	cmp r6, #14
	ble .L_ja_02008662
	movs r0, #9
	bl Object_GetById
	bl Func_02000c7c
	movs r0, #40
	bl WaitFrames
	movs r6, #9
.L_ja_0200868e:
	adds r0, r6, #0
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #16]
	ldr r2, .L_ja_020086f0
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
	strb r3, [r1, #0]
	str r2, [r5, #108]
	cmp r6, #14
	ble .L_ja_0200868e
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
	bne .L_ja_020086de
	cmp r7, #0
	ble .L_ja_020086de
	bl Func_02000388
.L_ja_020086de:
	pop {r5, r6, r7, pc}
.L_ja_020086e0:
	.4byte gPartyState
.L_ja_020086e4:
	.4byte gData_02000eec
.L_ja_020086e8:
	.4byte 0xfff80000
.L_ja_020086ec:
	.4byte Func_020004b4
.L_ja_020086f0:
	.4byte 0xfff00000
	bx lr
	movs r0, r0
	.else
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
	bl ObjectDispatch_SetSingleChildFlags
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
	.4byte gData_02000eec
.L_02008708:
	.4byte 0xfff80000
.L_0200870c:
	.4byte Func_020004b4
	.4byte 0x00004770
	.endif
	.section .text.x02008858,"ax",%progbits
	.balign 4
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
	.4byte gData_02001cdc
.L_02008a70:
	.4byte gResourceTableEntries
.L_02008a74:
	.4byte gData_02001d20
.L_02008a78:
	.4byte gData_02001de4
.L_02008a7c:
	.4byte gFrameCount
.L_02008a80:
	.4byte gData_02001cde
.L_02008a84:
	.4byte gData_02001ce0
.L_02008a88:
	.4byte gData_02001de0
.L_02008a8c:
	.4byte 0x40002000
.L_02008a90:
	.4byte gData_02001de2
.L_02008a94:
	.4byte 0xc000a000
	.section .bss,"aw",%nobits
	.global gData_02001cdc
gData_02001cdc:
	.space 0x00000002
	.global gData_02001cde
gData_02001cde:
	.space 0x00000002
	.global gData_02001ce0
gData_02001ce0:
	.space 0x00000040
	.global gData_02001d20
gData_02001d20:
	.space 0x000000c0
	.global gData_02001de0
gData_02001de0:
	.space 0x00000002
	.global gData_02001de2
gData_02001de2:
	.space 0x00000002
	.global gData_02001de4
gData_02001de4:
