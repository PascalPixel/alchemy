.syntax unified
	.thumb
	.section .text.x0200810e,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200815e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020081ea,"ax",%progbits
	.2byte 0x0000
	.section .text.x020081ec,"ax",%progbits
	.global Func_020001ec
	.thumb_func
Func_020001ec:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	str r3, [sp, #0]
	ldr r3, .L_020083bc
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
	beq .L_02008234
	cmp r7, #0
	beq .L_02008234
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_0200823c
.L_02008234:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_0200823c:
	mov r3, r8
	bl Engine_ObjectCreate
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200824a
	b .L_020083ae
.L_0200824a:
	ldr r3, [r6, #80]
	mov r1, r10
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	mov r8, r3
	bl Func_020011b4
	ldr r2, .L_020083c0
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl Func_020011bc
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r3, .L_020083c4
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
	ldr r3, .L_020083c8
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_020083ae
	cmp r7, #0
	beq .L_020083ae
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_020082cc
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_020082cc:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02008304
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
.L_02008304:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r10
	ands r2, r3
	cmp r2, #0
	beq .L_02008318
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02008318:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200835e
	ldr r3, .L_020083c0
	mov r1, r11
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_02008346
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_02008358
.L_02008346:
	ldr r2, .L_020083c8
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_020083c8
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_02008358:
	bl __divsi3
	str r0, [r6, #52]
.L_0200835e:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200837a
	adds r0, r6, #0
	movs r1, #1
	bl Func_020011b4
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_020011bc
.L_0200837a:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200838c
	ldrh r3, [r7, #32]
	mov r1, r8
	strh r3, [r1, #18]
.L_0200838c:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200839e
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_0200839e:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_020083ae
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_020083ae:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020083bc:
	.4byte gPartyState
.L_020083c0:
	.4byte gData_020019b4
.L_020083c4:
	.4byte BattleEffect_UpdateObjectMotionScaleAndLinkedAngle
.L_020083c8:
	.4byte 0xffff0000
	.section .text.x020083f8,"ax",%progbits
	.global Func_020003f8
	.thumb_func
Func_020003f8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r1, #0
	adds r0, r6, #0
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r3, #76
	beq .L_02008416
	b .L_0200856c
.L_02008416:
	bl Func_0200120c
	movs r0, #0
	bl Func_020012e4
	movs r3, #74
	movs r2, #33
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #74
	movs r1, #31
	movs r2, #1
	bl Func_020011d4
	movs r0, #15
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #76
	beq .L_02008444
	b .L_02008568
.L_02008444:
	movs r1, #85
	adds r2, r7, #0
	adds r1, r1, r7
	movs r3, #0
	adds r2, #89
	strb r3, [r1]
	strb r3, [r2]
	mov r8, r1
	movs r5, #0
.L_02008456:
	movs r0, #1
	bl WaitFrames
	ldr r2, [r7, #80]
	ldr r1, .L_02008578
	ldrh r3, [r2, #18]
	adds r5, #1
	adds r3, r3, r1
	strh r3, [r2, #18]
	ldr r3, [r7, #80]
	ldrh r0, [r3, #18]
	bl Math_Cosine
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r3, [r7, #8]
	asrs r0, r0, #1
	subs r3, r3, r0
	str r3, [r7, #8]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	cmp r5, #17
	bls .L_02008456
	ldr r3, .L_0200857c
	movs r1, #192
	movs r2, #192
	str r3, [r7, #108]
	adds r0, r6, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #3
	lsls r2, r2, #2
	adds r0, r6, #0
	adds r1, #169
	adds r2, #18
	bl ObjectMotion_ResetAndSetPosition
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	mov r1, r8
	movs r3, #3
	str r2, [r7, #72]
	strb r3, [r1]
	adds r3, r7, #0
	adds r3, #34
	movs r5, #0
	strb r5, [r3]
	adds r0, r6, #0
	mov r10, r2
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	adds r0, r7, #0
	bl Func_020003dc
	movs r0, #188
	bl Func_0200131c
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl Func_020011f4
	movs r0, #141
	bl Func_0200131c
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_020011f4
	ldr r2, [r7, #80]
	movs r3, #128
	lsls r3, r3, #5
	strh r3, [r2, #18]
	movs r3, #138
	add r4, sp, #16
	lsls r3, r3, #1
	str r5, [r7, #108]
	strh r3, [r4, #24]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r4, #8]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r4, #16]
	ldr r3, .L_02008580
	mov r2, r10
	str r3, [r4, #20]
	movs r3, #224
	str r2, [r4, #12]
	lsls r3, r3, #13
	ldr r2, [r7, #16]
	ldr r1, [r7, #12]
	ldr r0, [r7, #8]
	str r3, [sp, #8]
	movs r3, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	str r4, [sp, #12]
	bl Func_020001ec
	movs r0, #154
	bl Func_0200131c
	adds r0, r6, #0
	movs r1, #3
	bl Object_SetModeById
	bl Func_020011fc
	movs r0, #15
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #132
	bl GameFlag_SetBit
.L_02008568:
	bl Func_02001214
.L_0200856c:
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008578:
	.4byte 0xffffff00
.L_0200857c:
	.4byte Func_020003cc
.L_02008580:
	.4byte 0x00013333
	.section .text.x02008660,"ax",%progbits
	.balign 4
	.global Func_02000660
	.thumb_func
Func_02000660:
	push {r5, lr}
	ldr r3, .L_020086ec
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	ldr r3, [r0, #8]
	mov r5, sp
	str r3, [r5]
	adds r2, r5, #0
	ldr r3, [r0, #12]
	str r3, [r5, #4]
	ldr r3, [r0, #16]
	str r3, [r5, #8]
	ldrh r1, [r0, #6]
	movs r0, #128
	lsls r0, r0, #13
	bl Vector_AddPolarOffsetFar
	ldr r3, [r5]
	asrs r3, r3, #20
	cmp r3, #119
	bne .L_020086ae
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #45
	bne .L_020086ae
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #173
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
	b .L_020086e8
.L_020086ae:
	bl Func_0200120c
	movs r0, #0
	bl Func_020012e4
	ldr r3, .L_020086ec
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #16
	bl Object_LinkPair
	ldr r0, .L_020086f0
	bl Func_0200128c
	movs r0, #16
	movs r1, #0
	bl Func_02001294
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	bl Func_02001214
.L_020086e8:
	add sp, #12
	pop {r5, pc}
.L_020086ec:
	.4byte gPartyState
.L_020086f0:
	.4byte MsgRrruffRrruff2
	.section .text.x02008738,"ax",%progbits
	.global Func_02000738
	.thumb_func
Func_02000738:
	push {lr}
	ldr r3, .L_02008760
	movs r0, #160
	ldr r3, [r3]
	ldr r4, .L_02008764
	lsls r0, r0, #19
	adds r0, #24
	lsrs r2, r3, #4
	movs r1, #3
.L_0200874a:
	movs r3, #3
	ands r3, r2
	lsls r3, r3, #1
	ldrh r3, [r4, r3]
	subs r1, #1
	strh r3, [r0]
	adds r2, #1
	adds r0, #2
	cmp r1, #0
	bge .L_0200874a
	pop {pc}
.L_02008760:
	.4byte gFrameCount
.L_02008764:
	.4byte gData_02001ae0
	.section .text.x02008be4,"ax",%progbits
	.global Func_02000be4
	.thumb_func
Func_02000be4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_02008d98
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	mov r9, r0
	movs r0, #158
	mov r4, r9
	lsls r0, r0, #1
	movs r3, #2
	ldrsh r2, [r4, r3]
	adds r3, r1, r0
	ldr r3, [r3]
	sub sp, #4
	str r3, [sp, #0]
	movs r4, #156
	lsls r4, r4, #1
	adds r3, r1, r4
	ldr r1, .L_02008d9c
	ldr r3, [r3]
	lsls r2, r2, #2
	mov r5, r9
	movs r0, #0
	adds r1, r1, r2
	adds r5, #4
	mov r11, r3
	mov r10, r0
	mov r8, r1
.L_02008c28:
	movs r2, #0
	ldrsh r0, [r5, r2]
	cmp r0, #0
	bne .L_02008c32
	b .L_02008d78
.L_02008c32:
	bl Object_GetById
	adds r7, r0, #0
	ldr r3, [r7, #8]
	cmp r3, #0
	bne .L_02008c46
	ldr r3, [r7, #16]
	cmp r3, #0
	bne .L_02008c46
	b .L_02008d78
.L_02008c46:
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r4, #8
	ldrsh r3, [r5, r4]
	cmp r2, r3
	bne .L_02008c5c
	movs r0, #206
	bl Func_0200131c
	movs r3, #4
	strh r3, [r5, #18]
.L_02008c5c:
	movs r0, #2
	ldrsh r2, [r5, r0]
	movs r1, #10
	ldrsh r3, [r5, r1]
	cmp r2, r3
	bne .L_02008c78
	movs r0, #140
	adds r0, #255
	bl Func_0200131c
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #248
	strh r3, [r5, #18]
.L_02008c78:
	movs r4, #18
	ldrsh r3, [r5, r4]
	ldrh r2, [r5, #18]
	cmp r3, #0
	beq .L_02008cb2
	ldrh r3, [r5, #4]
	movs r0, #0
	adds r3, r3, r2
	movs r4, #12
	ldrsh r2, [r5, r4]
	strh r3, [r5, #4]
	lsls r3, r3, #16
	asrs r3, r3, #16
	ldrh r1, [r5, #12]
	cmp r3, r2
	blt .L_02008c9e
	strh r1, [r5, #4]
	strh r0, [r5, #18]
	strh r0, [r5, #22]
.L_02008c9e:
	movs r1, #4
	ldrsh r2, [r5, r1]
	movs r4, #14
	ldrsh r3, [r5, r4]
	ldrh r1, [r5, #14]
	cmp r2, r3
	bgt .L_02008cb2
	strh r1, [r5, #4]
	strh r0, [r5, #18]
	strh r0, [r5, #22]
.L_02008cb2:
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008cd2
	ldrh r3, [r5, #2]
	movs r1, #6
	ldrsh r2, [r5, r1]
	adds r3, #1
	strh r3, [r5, #2]
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r3, r2
	blt .L_02008cd2
	strh r0, [r5, #2]
.L_02008cd2:
	ldr r3, [r7, #16]
	ldr r2, [r7, #8]
	asrs r3, r3, #20
	asrs r2, r2, #20
	lsls r3, r3, #7
	adds r1, r2, r3
	mov r2, r9
	ldrb r3, [r2, #2]
	ldr r4, [sp, #0]
	add r3, r10
	strb r3, [r4, r1]
	movs r0, #14
	ldrsh r2, [r7, r0]
	movs r4, #4
	ldrsh r3, [r5, r4]
	adds r2, r2, r3
	cmp r2, #0
	bge .L_02008cf8
	adds r2, #7
.L_02008cf8:
	asrs r3, r2, #3
	lsls r3, r3, #8
	mov r0, r8
	str r3, [r0]
	mov r2, r11
	lsls r3, r1, #2
	adds r1, r2, r3
	movs r4, #18
	ldrsh r3, [r5, r4]
	cmp r3, #0
	beq .L_02008d12
	movs r3, #0
	b .L_02008d18
.L_02008d12:
	ldrb r2, [r1, #3]
	movs r3, #128
	orrs r3, r2
.L_02008d18:
	strb r3, [r1, #3]
	movs r0, #4
	ldrsh r3, [r5, r0]
	cmp r3, #0
	beq .L_02008d28
	ldrb r2, [r1, #3]
	movs r3, #16
	orrs r3, r2
.L_02008d28:
	strb r3, [r1, #3]
	ldr r6, [r5, #24]
	cmp r6, #0
	beq .L_02008d78
	movs r1, #4
	ldrsh r3, [r5, r1]
	cmp r3, #0
	bne .L_02008d40
	ldr r3, [r7, #12]
	ldr r2, .L_02008da0
	adds r3, r3, r2
	b .L_02008d6e
.L_02008d40:
	ldrh r0, [r5, #22]
	movs r3, #128
	lsls r3, r3, #5
	adds r0, r0, r3
	strh r0, [r5, #22]
	lsls r0, r0, #16
	asrs r0, r0, #16
	bl Math_Sine
	adds r1, r0, #0
	movs r0, #128
	ldr r3, .L_02008da4
	lsls r0, r0, #10
	mov lr, r3
	.2byte 0xf800
	movs r1, #4
	ldrsh r2, [r5, r1]
	ldr r3, [r7, #12]
	ldr r4, .L_02008da8
	lsls r2, r2, #16
	adds r0, r0, r4
	adds r3, r3, r2
	adds r3, r3, r0
.L_02008d6e:
	str r3, [r6, #12]
	ldr r3, [r7, #8]
	str r3, [r6, #8]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
.L_02008d78:
	movs r3, #1
	add r10, r3
	movs r2, #4
	mov r4, r10
	add r8, r2
	adds r5, #28
	cmp r4, #15
	bgt .L_02008d8a
	b .L_02008c28
.L_02008d8a:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008d98:
	.4byte gData_020023c4 + 0x188
.L_02008d9c:
	.4byte gMapCollision
.L_02008da0:
	.4byte 0xfff00000
.L_02008da4:
	.4byte IwramMulQ16
.L_02008da8:
	.4byte 0xfff20000
	.section .text.x02008dac,"ax",%progbits
	.global Func_02000dac
	.thumb_func
Func_02000dac:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_02008fb0
	sub sp, #36
	adds r0, r2, #4
	str r0, [sp, #32]
	ldr r1, .L_02008fb4
	movs r0, #0
	ldrsh r3, [r2, r0]
	lsls r3, r3, #2
	adds r3, r3, r1
	ldrh r3, [r3, #2]
	movs r1, #226
	lsrs r3, r3, #5
	str r3, [sp, #24]
	lsls r1, r1, #1
	adds r2, r2, r1
	ldr r2, [r2]
	movs r3, #192
	mov r9, r2
	movs r2, #0
	str r2, [sp, #16]
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	adds r2, r1, #0
	adds r2, #228
	ldr r3, [r2]
	str r3, [sp, #12]
	ldr r0, [sp, #12]
	ldr r3, .L_02008fb8
	ands r0, r3
	str r0, [sp, #12]
	ldr r2, [r2, #4]
	ands r2, r3
	str r2, [sp, #8]
	ldr r3, [r1]
	movs r1, #15
	ldr r3, [r3, #4]
	str r1, [sp, #28]
	str r3, [sp, #4]
.L_02008e06:
	ldr r3, [sp, #32]
	movs r2, #0
	ldrsh r0, [r3, r2]
	cmp r0, #0
	bne .L_02008e12
	b .L_02008f90
.L_02008e12:
	movs r1, #4
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_02008e1c
	b .L_02008f90
.L_02008e1c:
	bl Object_GetById
	ldr r3, .L_02008fbc
	movs r1, #12
	mov r10, r0
	ldr r0, [r3]
	bl __umodsi3
	movs r1, #3
	bl __udivsi3
	lsls r0, r0, #3
	adds r0, #32
	str r0, [sp, #20]
	ldr r1, [sp, #32]
	movs r2, #0
	movs r0, #4
	ldrsh r3, [r1, r0]
	mov r11, r2
	cmp r11, r3
	bge .L_02008ee0
.L_02008e46:
	ldr r2, [sp, #16]
	cmp r2, #79
	bgt .L_02008ed2
	mov r0, r10
	ldr r3, [r0, #8]
	ldr r1, [sp, #12]
	subs r7, r3, r1
	mov r3, r11
	lsls r2, r3, #16
	ldr r3, [r0, #12]
	ldr r0, [sp, #4]
	adds r3, r3, r2
	mov r1, r10
	subs r0, r3, r0
	ldr r2, [sp, #8]
	ldr r3, [r1, #16]
	mov r8, r0
	ldr r0, [sp, #4]
	subs r3, r3, r2
	subs r5, r3, r0
	mov r1, r8
	subs r6, r5, r1
	asrs r3, r6, #16
	adds r6, r3, #0
	adds r3, r1, r5
	asrs r3, r3, #16
	asrs r2, r7, #16
	adds r1, r3, #0
	movs r3, #167
	adds r7, r2, #0
	lsls r3, r3, #1
	adds r2, #7
	subs r7, #8
	subs r6, #16
	adds r1, #58
	cmp r2, r3
	bhi .L_02008ed2
	movs r0, #16
	negs r0, r0
	cmp r6, r0
	ble .L_02008ed2
	cmp r6, #239
	bgt .L_02008ed2
	adds r3, #177
	ands r7, r3
	movs r3, #255
	mov r4, r9
	ands r6, r3
	movs r3, #0
	stmia r4!, {r3}
	lsls r3, r7, #16
	orrs r6, r3
	ldr r3, .L_02008fc0
	orrs r6, r3
	stmia r4!, {r6}
	ldr r2, [sp, #24]
	ldr r0, [sp, #20]
	adds r3, r2, r0
	movs r2, #128
	lsls r2, r2, #4
	orrs r3, r2
	mov r0, r9
	str r3, [r4]
	bl Func_0200119c
	ldr r2, [sp, #16]
	movs r1, #12
	adds r2, #1
	str r2, [sp, #16]
	add r9, r1
.L_02008ed2:
	ldr r1, [sp, #32]
	movs r3, #16
	add r11, r3
	movs r0, #4
	ldrsh r3, [r1, r0]
	cmp r11, r3
	blt .L_02008e46
.L_02008ee0:
	ldr r2, [sp, #16]
	cmp r2, #79
	bgt .L_02008f90
	mov r0, r10
	ldr r3, [r0, #8]
	ldr r1, [sp, #12]
	ldr r0, [sp, #32]
	subs r7, r3, r1
	movs r3, #4
	ldrsh r2, [r0, r3]
	mov r1, r10
	ldr r3, [r1, #12]
	lsls r2, r2, #16
	adds r3, r3, r2
	ldr r2, [sp, #4]
	ldr r0, [sp, #8]
	subs r2, r3, r2
	ldr r3, [r1, #16]
	ldr r1, [sp, #4]
	subs r3, r3, r0
	subs r5, r3, r1
	ldr r3, [sp, #32]
	subs r6, r5, r2
	mov r8, r2
	movs r2, #22
	ldrsh r0, [r3, r2]
	bl Math_Sine
	ldr r3, .L_02008fc4
	adds r1, r0, #0
	ldr r0, .L_02008fc8
	mov lr, r3
	.2byte 0xf800
	ldr r1, .L_02008fc8
	adds r0, r6, r0
	mov r2, r8
	asrs r7, r7, #16
	adds r0, r0, r1
	adds r3, r2, r5
	mov r10, r7
	asrs r0, r0, #16
	asrs r3, r3, #16
	adds r1, r3, #0
	subs r6, r0, #4
	mov r3, r10
	movs r0, #167
	adds r3, #7
	lsls r0, r0, #1
	subs r7, #8
	adds r1, #58
	cmp r3, r0
	bhi .L_02008f90
	movs r2, #16
	negs r2, r2
	cmp r6, r2
	ble .L_02008f90
	cmp r6, #239
	bgt .L_02008f90
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r7, r3
	movs r3, #255
	mov r4, r9
	ands r6, r3
	movs r3, #0
	stmia r4!, {r3}
	lsls r3, r7, #16
	orrs r6, r3
	ldr r3, .L_02008fc0
	orrs r6, r3
	stmia r4!, {r6}
	ldr r0, [sp, #24]
	ldr r2, [sp, #20]
	adds r3, r0, r2
	movs r2, #128
	lsls r2, r2, #4
	subs r3, #32
	orrs r3, r2
	mov r0, r9
	str r3, [r4]
	bl Func_0200119c
	ldr r0, [sp, #16]
	movs r3, #12
	adds r0, #1
	str r0, [sp, #16]
	add r9, r3
.L_02008f90:
	ldr r1, [sp, #28]
	ldr r2, [sp, #32]
	subs r1, #1
	adds r2, #28
	str r1, [sp, #28]
	str r2, [sp, #32]
	cmp r1, #0
	blt .L_02008fa2
	b .L_02008e06
.L_02008fa2:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008fb0:
	.4byte gData_020023c4 + 0x188
.L_02008fb4:
	.4byte gResourceTableEntries
.L_02008fb8:
	.4byte 0xffff0000
.L_02008fbc:
	.4byte gFrameCount
.L_02008fc0:
	.4byte 0x40002000
.L_02008fc4:
	.4byte IwramMulQ16
.L_02008fc8:
	.4byte 0xfffe0000
	.section .text.x02009068,"ax",%progbits
	.global Func_02001068
	.thumb_func
Func_02001068:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r9, r2
	mov r10, r3
	ldr r2, .L_020090fc
	lsls r3, r0, #3
	subs r3, r3, r0
	mov r8, r1
	lsls r3, r3, #2
	adds r3, r3, r2
	mov r0, r8
	ldr r7, [sp, #28]
	adds r5, r3, #4
	bl Object_GetById
	adds r6, r0, #0
	mov r0, r8
	bl Object_GetById
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009100
	cmp r7, r10
	bge .L_020090b2
	mov r12, r10
	mov r10, r7
	mov r7, r12
.L_020090b2:
	mov r3, r8
	strh r3, [r5]
	mov r3, r9
	strh r3, [r5, #2]
	movs r3, #180
	lsls r3, r3, #1
	strh r3, [r5, #6]
	movs r3, #60
	strh r3, [r5, #8]
	movs r3, #240
	strh r3, [r5, #10]
	mov r3, r10
	ldr r2, .L_020090f8
	strh r3, [r5, #14]
	movs r3, #1
	strh r3, [r5, #16]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	adds r2, r6, #0
	adds r2, #89
	movs r3, #8
	strb r3, [r2]
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strh r0, [r5, #4]
	strh r7, [r5, #12]
	strh r0, [r5, #18]
	strh r0, [r5, #22]
	strh r0, [r5, #20]
	strb r3, [r1]
	b .L_02009100
.L_020090f8:
	.4byte 0x00000000
.L_020090fc:
	.4byte gData_020023c4 + 0x188
.L_02009100:
	movs r0, #128
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	lsls r0, r0, #8
	bl Engine_ObjectCreate
	adds r1, r0, #0
	adds r2, r1, #0
	movs r3, #0
	adds r2, #89
	strb r3, [r2]
	subs r2, #4
	strb r3, [r2]
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
	str r1, [r5, #24]
	adds r0, r5, #0
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.section .bss,"aw",%nobits
	.global gData_02001ae0
gData_02001ae0:
