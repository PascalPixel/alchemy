.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #32]
	sub sp, #8
	bl Object_GetById
	ldr r3, [r0, #80]
	ldr r5, .L_02008184
	ldr r3, [r3, #40]
	movs r1, #0
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r4, r3, #16
	ldrh r3, [r5, r1]
	lsrs r2, r4, #16
	cmp r2, r3
	beq .L_02008082
.L_02008068:
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	lsrs r2, r3, #16
	asrs r1, r3, #16
	cmp r2, #5
	bhi .L_02008082
	lsls r3, r2, #1
	ldrh r3, [r5, r3]
	lsrs r2, r4, #16
	cmp r2, r3
	bne .L_02008068
.L_02008082:
	lsls r3, r1, #16
	lsrs r2, r3, #16
	cmp r2, #6
	bne .L_0200808e
	movs r0, #0
	b .L_02008174
.L_0200808e:
	ldr r6, .L_02008188
	lsls r2, r2, #2
	ldrsb r4, [r6, r2]
	adds r1, r4, #0
	cmp r4, #0
	bge .L_0200809c
	negs r1, r4
.L_0200809c:
	adds r3, r2, #2
	ldrsb r3, [r6, r3]
	cmp r3, #0
	bge .L_020080a6
	negs r3, r3
.L_020080a6:
	adds r3, r1, r3
	asrs r3, r3, #4
	mov r11, r3
	adds r3, r2, #1
	ldrsb r1, [r6, r3]
	adds r5, r1, #0
	cmp r1, #0
	bge .L_020080b8
	negs r5, r1
.L_020080b8:
	adds r3, r2, #3
	ldrsb r2, [r6, r3]
	cmp r2, #0
	bge .L_020080c2
	negs r2, r2
.L_020080c2:
	adds r5, r5, r2
	mov r9, r5
	ldr r2, [r0, #8]
	mov r3, r9
	asrs r3, r3, #4
	mov r9, r3
	mov r10, r2
	lsls r3, r4, #16
	ldr r0, [r0, #16]
	add r10, r3
	mov r3, r10
	asrs r3, r3, #20
	mov r10, r3
	mov r8, r0
	lsls r3, r1, #16
	movs r2, #164
	add r8, r3
	lsls r2, r2, #1
	mov r0, r8
	adds r3, r7, r2
	ldr r3, [r3]
	asrs r0, r0, #20
	mov r8, r0
	movs r0, #166
	lsls r0, r0, #1
	asrs r1, r3, #20
	adds r3, r7, r0
	ldr r3, [r3]
	adds r0, #52
	adds r2, r7, r0
	adds r0, #4
	ldr r6, [r2]
	asrs r3, r3, #20
	adds r2, r7, r0
	ldr r5, [r2]
	lsls r3, r3, #16
	lsls r2, r1, #16
	lsrs r2, r2, #16
	lsrs r3, r3, #16
	add r2, r10
	add r3, r8
	str r2, [sp, #0]
	str r3, [sp, #4]
	mov r0, r10
	mov r1, r8
	mov r2, r11
	mov r3, r9
	bl Func_02001848
	asrs r6, r6, #20
	asrs r5, r5, #20
	lsls r6, r6, #16
	lsls r5, r5, #16
	lsrs r6, r6, #16
	lsrs r5, r5, #16
	mov r3, r10
	mov r2, r8
	add r5, r8
	adds r3, #64
	adds r2, #64
	add r6, r10
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r1, r5, #0
	adds r0, r6, #0
	mov r2, r11
	mov r3, r9
	bl Func_02001848
	mov r2, r9
	str r2, [sp, #0]
	movs r5, #255
	movs r0, #0
	mov r1, r10
	mov r2, r8
	mov r3, r11
	str r5, [sp, #4]
	bl Map_FillCellRectByte2
	mov r3, r9
	str r3, [sp, #0]
	movs r0, #2
	mov r1, r10
	mov r2, r8
	mov r3, r11
	str r5, [sp, #4]
	bl Map_FillCellRectByte2
	movs r0, #1
.L_02008174:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008184:
	.4byte Data_02001968
.L_02008188:
	.4byte Data_02001974
	.section .text.x020081d4,"ax",%progbits
	.balign 4
	.global Func_020001d4
	.thumb_func
Func_020001d4:
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
	bl Func_0200033c
	adds r4, r0, #0
	cmp r4, #0
	bne .L_020081fe
	b .L_0200831e
.L_020081fe:
	ldr r5, [r5]
	ldr r0, .L_02008330
	str r5, [sp, #20]
	lsls r1, r5, #2
	ldrsb r2, [r0, r1]
	cmp r2, #0
	bge .L_0200820e
	negs r2, r2
.L_0200820e:
	adds r3, r1, #2
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_02008218
	negs r3, r3
.L_02008218:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #16]
	adds r3, r1, #1
	ldrsb r2, [r0, r3]
	cmp r2, #0
	bge .L_02008228
	negs r2, r2
.L_02008228:
	adds r3, r1, #3
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_02008232
	negs r3, r3
.L_02008232:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #12]
	ldr r3, [sp, #24]
	ldr r2, .L_02008334
	ldr r1, .L_02008338
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
.L_02008270:
	ldr r3, [sp, #20]
	ldr r2, .L_02008330
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
	bge .L_020082de
.L_0200828e:
	ldr r3, .L_02008330
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
	bge .L_020082c8
.L_020082a6:
	adds r0, r4, #0
	add r1, sp, #28
	str r4, [sp, #0]
	bl Func_02001850
	ldr r4, [sp, #0]
	cmp r0, #2
	beq .L_020082f0
	ldr r3, [r5]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r5]
	ldr r2, [sp, #16]
	adds r7, #1
	cmp r7, r2
	blt .L_020082a6
.L_020082c8:
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
	blt .L_0200828e
.L_020082de:
	ldr r3, [r6]
	movs r1, #1
	add r3, r9
	str r3, [r6]
	ldr r3, [r6, #8]
	add r11, r1
	add r3, r10
	str r3, [r6, #8]
	b .L_02008270
.L_020082f0:
	ldr r2, [sp, #8]
	movs r3, #0
	strb r3, [r2]
	mov r3, r11
	movs r0, #0
	cmp r3, #0
	beq .L_02008320
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
	b .L_02008320
.L_0200831e:
	movs r0, #0
.L_02008320:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008330:
	.4byte Data_02001974
.L_02008334:
	.4byte Data_0200198c
.L_02008338:
	.4byte 0xffff0000
	.section .text.x0200833c,"ax",%progbits
	.global Func_0200033c
	.thumb_func
Func_0200033c:
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
	ldr r3, .L_02008448
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
.L_02008378:
	mov r3, lr
	ldr r6, [r3]
	movs r5, #0
.L_0200837e:
	ldr r3, [r6, #80]
	ldr r2, .L_0200844c
	ldr r3, [r3, #40]
	movs r0, #0
	ldrsh r1, [r3, r0]
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	cmp r1, r3
	bne .L_02008422
	ldr r0, [sp, #8]
	movs r2, #10
	ldrsh r1, [r7, r2]
	ldr r3, [r0]
	ldr r2, .L_02008450
	lsls r3, r3, #2
	ldr r3, [r2, r3]
	ldr r4, .L_02008454
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
	bgt .L_02008422
	cmp r9, r0
	bge .L_02008422
	cmp r12, r8
	bgt .L_02008422
	cmp r8, r1
	bge .L_02008422
	ldr r0, [sp, #0]
	movs r3, #1
	ands r3, r5
	str r5, [r0]
	cmp r3, #0
	beq .L_02008410
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r10, r3
	beq .L_02008422
	ldr r2, [sp, #4]
	mov r1, r11
	str r1, [r2]
	adds r0, r6, #0
	b .L_02008438
.L_02008410:
	ldr r3, [r7, #16]
	asrs r3, r3, #20
	cmp r12, r3
	beq .L_02008422
	ldr r0, [sp, #4]
	mov r3, r11
	str r3, [r0]
	adds r0, r6, #0
	b .L_02008438
.L_02008422:
	adds r5, #1
	cmp r5, #5
	bls .L_0200837e
	movs r2, #1
	add r11, r2
	movs r1, #4
	mov r3, r11
	add lr, r1
	cmp r3, #63
	bls .L_02008378
	movs r0, #0
.L_02008438:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008448:
	.4byte gPartyState
.L_0200844c:
	.4byte Data_02001968
.L_02008450:
	.4byte Data_0200198c
.L_02008454:
	.4byte Data_02001974
	.section .text.x02008458,"ax",%progbits
	.global Func_02000458
	.thumb_func
Func_02000458:
	sub sp, #16
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #64
	str r0, [sp, #96]
	str r1, [sp, #100]
	str r2, [sp, #104]
	str r3, [sp, #108]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r0, #133
	str r3, [sp, #36]
	ldr r3, .L_02008744
	lsls r0, r0, #2
	adds r0, r0, r3
	mov r10, r0
	ldr r0, [r0]
	bl Object_GetById
	mov r8, r0
	ldr r0, [sp, #112]
	bl Object_GetById
	mov r3, r8
	ldr r3, [r3, #48]
	mov r4, r8
	str r3, [sp, #24]
	adds r6, r0, #0
	ldr r4, [r4, #52]
	mov r0, sp
	adds r0, #40
	str r0, [sp, #16]
	str r4, [sp, #20]
	ldr r2, [sp, #108]
	ldr r3, [r6, #8]
	movs r1, #0
	str r3, [r0]
	mov r9, r1
	ldr r3, [r6, #16]
	mov r1, sp
	adds r1, #52
	str r3, [r0, #8]
	ldr r5, .L_02008748
	str r1, [sp, #12]
	lsls r2, r2, #2
	ldrsb r1, [r5, r2]
	ldr r3, [r6, #8]
	mov r11, r2
	lsls r2, r1, #16
	adds r3, r3, r2
	ldr r2, [sp, #12]
	asrs r3, r3, #20
	str r3, [r2]
	mov r12, r3
	mov r3, r11
	adds r3, #1
	ldrsb r4, [r5, r3]
	ldr r3, [r6, #16]
	lsls r2, r4, #16
	adds r3, r3, r2
	asrs r7, r3, #20
	ldr r3, [sp, #12]
	adds r0, r1, #0
	str r7, [r3, #8]
	cmp r0, #0
	bge .L_020084ea
	negs r0, r0
.L_020084ea:
	mov r3, r11
	adds r3, #2
	ldrsb r1, [r5, r3]
	cmp r1, #0
	bge .L_020084f6
	negs r1, r1
.L_020084f6:
	adds r3, r0, r1
	asrs r3, r3, #4
	adds r1, r4, #0
	str r3, [sp, #32]
	cmp r1, #0
	bge .L_02008504
	negs r1, r1
.L_02008504:
	mov r3, r11
	adds r3, #3
	ldrsb r2, [r5, r3]
	cmp r2, #0
	bge .L_02008510
	negs r2, r2
.L_02008510:
	adds r3, r1, r2
	asrs r3, r3, #4
	str r3, [sp, #28]
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	mov r1, r12
	ldr r3, [sp, #32]
	movs r0, #0
	adds r2, r7, #0
	bl Map_FillCellRectByte2
	movs r2, #200
	mov r4, r10
	movs r1, #128
	lsls r2, r2, #5
	ldr r0, [r4]
	adds r2, #153
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	mov r1, r10
	ldr r0, [r1]
	movs r1, #8
	bl Object_SetModeById
	movs r0, #15
	bl WaitFrames
	ldr r2, [sp, #16]
	ldr r4, [sp, #16]
	ldr r1, [sp, #96]
	ldr r3, [r2]
	ldr r2, [sp, #104]
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
	ldr r3, .L_0200874c
	str r3, [r0, #108]
	movs r0, #4
	bl WaitFrames
	movs r1, #2
	adds r0, r6, #0
	bl Func_02001828
	movs r0, #239
	bl Func_02001960
	movs r2, #200
	movs r1, #128
	lsls r2, r2, #5
	ldr r0, [sp, #112]
	lsls r1, r1, #8
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	adds r0, r6, #0
	ldr r1, [sp, #96]
	ldr r2, [sp, #100]
	ldr r3, [sp, #104]
	bl Func_02001838
	ldr r3, .L_02008744
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
	ldr r2, .L_02008750
	mov r1, r9
	lsls r3, r1, #2
	ldr r2, [r2, r3]
	ldr r0, [r5]
	lsls r2, r2, #16
	asrs r1, r2, #31
	asrs r2, r2, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	ldr r3, [sp, #116]
	cmp r3, #0
	beq .L_020085e8
	mov lr, r3
	.2byte 0xf800
.L_020085e8:
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #1
	ldr r0, [r5]
	bl Object_SetModeById
	mov r2, r8
	movs r3, #0
	str r3, [r2, #108]
	ldr r3, [sp, #24]
	adds r0, r6, #0
	str r3, [r2, #48]
	ldr r4, [sp, #20]
	movs r5, #255
	str r4, [r2, #52]
	bl Func_02001840
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02001960
	movs r0, #213
	bl Func_02001960
	ldr r2, [r6, #12]
	ldr r1, [sp, #96]
	ldr r3, [sp, #104]
	adds r0, r6, #0
	bl Object_SetPositionAndResetMotion
	adds r0, r6, #0
	movs r1, #1
	bl Func_02001828
	ldr r7, .L_02008748
	mov r0, r11
	mov r1, r11
	ldrsb r3, [r7, r0]
	adds r1, #1
	str r1, [sp, #8]
	ldr r0, [sp, #96]
	lsls r3, r3, #16
	adds r0, r0, r3
	ldrsb r3, [r7, r1]
	ldr r1, [sp, #104]
	lsls r3, r3, #16
	adds r1, r1, r3
	ldr r2, [sp, #36]
	asrs r0, r0, #20
	asrs r1, r1, #20
	str r0, [sp, #96]
	str r1, [sp, #104]
	movs r4, #164
	lsls r4, r4, #1
	adds r3, r2, r4
	ldr r3, [r3]
	ldr r4, [sp, #36]
	mov r8, r3
	mov r2, r8
	asrs r2, r2, #20
	mov r8, r2
	movs r2, #166
	lsls r2, r2, #1
	adds r3, r4, r2
	adds r2, #52
	ldr r6, [r3]
	adds r3, r4, r2
	ldr r3, [r3]
	adds r2, #4
	asrs r3, r3, #20
	mov r9, r3
	adds r3, r4, r2
	ldr r3, [r3]
	mov r4, r8
	asrs r6, r6, #20
	asrs r3, r3, #20
	adds r2, r6, r1
	mov r10, r3
	adds r3, r4, r0
	str r3, [sp, #0]
	str r2, [sp, #4]
	ldr r3, [sp, #28]
	ldr r2, [sp, #32]
	bl Func_02001848
	ldr r2, [sp, #96]
	ldr r3, [sp, #104]
	mov r4, r10
	mov r1, r9
	adds r0, r2, r1
	adds r1, r3, r4
	adds r2, #64
	adds r3, #64
	str r2, [sp, #0]
	str r3, [sp, #4]
	ldr r2, [sp, #32]
	ldr r3, [sp, #28]
	bl Func_02001848
	ldr r0, [sp, #28]
	ldr r1, [sp, #96]
	ldr r2, [sp, #104]
	str r0, [sp, #0]
	ldr r3, [sp, #32]
	movs r0, #0
	str r5, [sp, #4]
	bl Map_FillCellRectByte2
	ldr r3, [sp, #28]
	ldr r1, [sp, #96]
	ldr r2, [sp, #104]
	str r3, [sp, #0]
	movs r0, #2
	ldr r3, [sp, #32]
	str r5, [sp, #4]
	bl Map_FillCellRectByte2
	ldr r0, [sp, #16]
	mov r4, r11
	ldrsb r3, [r7, r4]
	ldr r1, [r0]
	ldr r2, [sp, #12]
	lsls r3, r3, #16
	adds r1, r1, r3
	asrs r1, r1, #20
	str r1, [r2]
	ldr r3, [sp, #8]
	ldr r4, [sp, #12]
	ldrsb r2, [r7, r3]
	ldr r3, [r0, #8]
	lsls r2, r2, #16
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [r4, #8]
	add r8, r1
	adds r6, r6, r3
	str r1, [sp, #0]
	str r3, [sp, #4]
	ldr r2, [sp, #32]
	ldr r3, [sp, #28]
	mov r0, r8
	adds r1, r6, #0
	bl Func_02001848
	ldr r0, [sp, #12]
	ldr r1, [sp, #12]
	ldr r2, [r0]
	ldr r3, [r1, #8]
	adds r0, r2, #0
	adds r1, r3, #0
	add r2, r9
	add r3, r10
	str r2, [sp, #0]
	str r3, [sp, #4]
	ldr r2, [sp, #32]
	ldr r3, [sp, #28]
	adds r0, #64
	adds r1, #64
	bl Func_02001848
	bl Func_02001938
	add sp, #64
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
.L_02008744:
	.4byte gPartyState
.L_02008748:
	.4byte Data_02001974
.L_0200874c:
	.4byte Func_02000754
.L_02008750:
	.4byte Data_0200198c
	.section .text.x02008754,"ax",%progbits
	.global Func_02000754
	.thumb_func
Func_02000754:
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
	bl Func_02001948
	cmp r0, #0
	beq .L_020087b0
	movs r4, #0
.L_0200878c:
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	movs r2, #0
	ldrsh r1, [r3, r2]
	ldr r2, .L_020087d8
	lsls r3, r4, #1
	ldrh r3, [r2, r3]
	cmp r1, r3
	beq .L_020087d4
	adds r4, #1
	cmp r4, #5
	bls .L_0200878c
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
.L_020087b0:
	ldr r3, [r5, #8]
	adds r0, r5, #0
	str r3, [r6]
	ldr r3, [r5, #12]
	adds r1, r6, #0
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	str r3, [r6, #8]
	bl Func_02001850
	cmp r0, #0
	ble .L_020087d4
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
.L_020087d4:
	add sp, #12
	pop {r5, r6, pc}
.L_020087d8:
	.4byte Data_02001968
	.section .text.x020087dc,"ax",%progbits
	.global Func_020007dc
	.thumb_func
Func_020007dc:
	push {r5, r6, lr}
	adds r4, r1, #0
	movs r1, #192
	lsls r1, r1, #18
	adds r1, #128
	ldr r6, [r1]
	ldr r1, .L_02008880
	adds r5, r0, #0
	str r5, [r1]
	ldr r1, .L_02008884
	str r4, [r1]
	ldr r1, .L_02008888
	str r2, [r1]
	ldr r2, .L_0200888c
	str r3, [r2]
	movs r2, #255
	ldrh r3, [r5]
	b .L_02008826
.L_02008800:
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
.L_02008826:
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_02008834
	ldrh r3, [r4]
	cmp r3, r2
	bne .L_02008800
.L_02008834:
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
	ldr r0, .L_02008890
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_020018f0
	ldr r3, .L_02008894
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #2
	bne .L_0200887e
	bl Func_02000acc
.L_0200887e:
	pop {r5, r6, pc}
.L_02008880:
	.4byte Data_02001fd8
.L_02008884:
	.4byte Data_02001fdc
.L_02008888:
	.4byte Data_02001fe0
.L_0200888c:
	.4byte Data_02001fcc
.L_02008890:
	.4byte 0x05000200
.L_02008894:
	.4byte gPartyState
	.section .text.x02008978,"ax",%progbits
	.balign 4
	.global Func_02000978
	.thumb_func
Func_02000978:
	push {lr}
	ldr r1, .L_02008990
	ldrh r3, [r0]
	movs r2, #0
	cmp r3, r1
	beq .L_02008994
.L_02008984:
	adds r0, #2
	ldrh r3, [r0]
	adds r2, #1
	cmp r3, r1
	bne .L_02008984
	b .L_02008994
.L_02008990:
	.4byte 0x0000ffff
.L_02008994:
	subs r2, #1
	adds r0, r2, #0
	pop {pc}
	.2byte 0x0000
	.section .text.x0200899c,"ax",%progbits
	.global Func_0200099c
	.thumb_func
Func_0200099c:
	push {r5, r6, r7, lr}
	ldr r1, .L_02008a4c
	movs r4, #0
	ldrh r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_020089d6
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r2, r0
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_020089d6
	ldr r0, .L_02008a50
	movs r4, #1
	ldrh r2, [r0]
	strh r2, [r1]
	movs r1, #128
	lsls r3, r2, #16
	lsls r1, r1, #10
	cmp r3, r1
	bls .L_020089d6
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
	strh r3, [r0]
.L_020089d6:
	cmp r4, #0
	bne .L_020089dc
	b .L_02008ac8
.L_020089dc:
	ldr r3, .L_02008a54
	ldr r6, .L_02008a58
	ldr r1, [r3]
	ldrh r3, [r6]
	movs r5, #0
	cmp r5, r3
	bcs .L_02008a2a
	ldr r3, .L_02008a5c
	ldr r2, .L_02008a60
	ldr r7, [r3]
	mov lr, r2
	mov r12, r6
.L_020089f4:
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
	bcc .L_020089f4
.L_02008a2a:
	ldr r3, .L_02008a58
	movs r0, #160
	ldrh r1, [r3]
	ldr r3, .L_02008a64
	lsls r2, r1, #1
	ldr r3, [r3]
	lsls r0, r0, #19
	ldrh r3, [r2, r3]
	adds r2, r2, r1
	lsls r3, r3, #1
	adds r4, r3, r0
	ldr r3, .L_02008a68
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_02008a70
	ldr r3, .L_02008a6c
	b .L_02008a72
.L_02008a4c:
	.4byte Data_02001fd0
.L_02008a50:
	.4byte Data_02001fd4
.L_02008a54:
	.4byte Data_02001fe0
.L_02008a58:
	.4byte Data_02001fe4
.L_02008a5c:
	.4byte Data_02001fcc
.L_02008a60:
	.4byte Data_02001fe8
.L_02008a64:
	.4byte Data_02001fd8
.L_02008a68:
	.4byte Data_02001fc8
.L_02008a6c:
	.4byte Data_02001fdc
.L_02008a70:
	ldr r3, .L_02008ab8
.L_02008a72:
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
	ldr r1, .L_02008abc
	ldr r2, .L_02008ab0
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	ldr r1, .L_02008ac0
	ldr r2, .L_02008ac4
	ldrh r3, [r1]
	adds r3, #1
	strh r3, [r1]
	lsls r3, r3, #16
	ldrh r2, [r2]
	lsrs r3, r3, #16
	cmp r3, r2
	bne .L_02008ac8
	ldr r3, .L_02008ab4
	strh r3, [r1]
	b .L_02008ac8
	.2byte 0x0000
.L_02008ab0:
	.4byte 0x00000001
.L_02008ab4:
	.4byte 0x00000000
.L_02008ab8:
	.4byte Data_02001fe0
.L_02008abc:
	.4byte Data_02001fc8
.L_02008ac0:
	.4byte Data_02001fe8
.L_02008ac4:
	.4byte Data_02001fe4
.L_02008ac8:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008b54,"ax",%progbits
	.global Func_02000b54
	.thumb_func
Func_02000b54:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	sub sp, #4
	cmp r3, r2
	beq .L_02008bb8
	adds r7, r0, #0
.L_02008b6a:
	ldrh r3, [r7]
	mov r8, r3
	mov r0, r8
	bl Object_GetById
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #89
	movs r2, #2
	ldrsh r6, [r7, r2]
	ldrb r2, [r1]
	movs r3, #4
	ldrsh r4, [r7, r3]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r1, #0
	str r4, [sp, #0]
	bl ObjectDispatch_SetSingleChildFlags
	ldr r4, [sp, #0]
	lsls r6, r6, #16
	lsls r4, r4, #16
	lsrs r4, r4, #16
	lsrs r6, r6, #16
	adds r5, #34
	ldrb r3, [r5]
	adds r2, r4, #0
	mov r0, r8
	adds r1, r6, #0
	adds r7, #6
	bl Func_02000c44
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_02008b6a
.L_02008bb8:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x02008bc0,"ax",%progbits
	.global Func_02000bc0
	.thumb_func
Func_02000bc0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	adds r5, r0, #0
	mov r9, r1
	mov r10, r2
	movs r1, #255
	ldr r2, [r3]
	b .L_02008c2c
.L_02008bdc:
	ldrh r3, [r5]
	movs r1, #26
	ldrsh r7, [r2, r1]
	cmp r7, r3
	bne .L_02008c28
	adds r0, r7, #0
	bl Object_GetById
	adds r5, #2
	ldrh r2, [r5]
	mov r3, r10
	adds r6, r0, #0
	mov r8, r2
	ldrh r5, [r5, #2]
	cmp r3, #7
	bgt .L_02008c04
	ldr r3, [r6, #28]
	ldr r1, .L_02008c40
	adds r3, r3, r1
	str r3, [r6, #28]
.L_02008c04:
	mov r2, r9
	cmp r2, #1
	bne .L_02008c36
	adds r0, r5, #0
	bl GameFlag_SetBit
	adds r3, r6, #0
	adds r3, #34
	ldrb r3, [r3]
	adds r0, r7, #0
	mov r1, r8
	adds r2, r5, #0
	bl Func_02000c44
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #28]
	b .L_02008c36
.L_02008c28:
	adds r5, #6
	movs r1, #255
.L_02008c2c:
	ldrh r3, [r5]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	bne .L_02008bdc
.L_02008c36:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008c40:
	.4byte 0xffffe100
	.section .text.x02008e68,"ax",%progbits
	.balign 4
	.global Func_02000e68
	.thumb_func
Func_02000e68:
	push {r5, r6, r7, lr}
	ldr r3, .L_02008f04
	movs r2, #240
	lsls r2, r2, #1
	adds r6, r3, r2
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, .L_02008f08
	sub sp, #8
	adds r7, r1, #0
	cmp r2, r3
	bne .L_02008ea6
	movs r3, #97
	movs r5, #12
	str r3, [sp, #0]
	movs r0, #98
	movs r1, #3
	movs r2, #3
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02001848
	movs r3, #34
	str r3, [sp, #0]
	movs r0, #33
	movs r1, #12
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02001848
.L_02008ea6:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, .L_02008f0c
	cmp r2, r3
	bne .L_02008efe
	cmp r7, #12
	bne .L_02008edc
	movs r3, #103
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #102
	movs r1, #15
	movs r2, #1
	movs r3, #1
	bl Func_02001848
	movs r3, #39
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #43
	movs r1, #14
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_02008edc:
	cmp r7, #13
	bne .L_02008efe
	movs r0, #13
	bl Object_GetById
	ldr r2, [r0, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #7
	movs r1, #10
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_02008efe:
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008f04:
	.4byte gPartyState
.L_02008f08:
	.4byte 0x00000050
.L_02008f0c:
	.4byte 0x00000051
	.section .text.x02008f10,"ax",%progbits
	.global Func_02000f10
	.thumb_func
Func_02000f10:
	push {r5, r6, r7, lr}
	adds r7, r1, #0
	adds r0, r7, #0
	sub sp, #8
	bl Object_GetById
	ldr r3, .L_02009034
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009038
	adds r6, r0, #0
	cmp r2, r3
	bne .L_02008f98
	ldr r3, [r6, #16]
	asrs r5, r3, #20
	cmp r5, #13
	bne .L_02008f72
	adds r2, r6, #0
	movs r0, #129
	adds r2, #85
	movs r3, #0
	lsls r0, r0, #2
	strb r3, [r2]
	adds r0, #255
	bl GameFlag_SetBit
	movs r3, #34
	str r3, [sp, #0]
	movs r0, #32
	movs r1, #13
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02001848
	movs r3, #97
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #95
	movs r1, #3
	movs r2, #3
	movs r3, #3
	bl Func_02001848
	b .L_02008f98
.L_02008f72:
	movs r3, #97
	movs r5, #12
	str r3, [sp, #0]
	movs r0, #92
	movs r1, #3
	movs r2, #3
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02001848
	movs r3, #34
	str r3, [sp, #0]
	movs r0, #32
	movs r1, #13
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02001848
.L_02008f98:
	ldr r3, .L_02009034
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200903c
	cmp r2, r3
	bne .L_02009030
	cmp r7, #12
	bne .L_02008ffc
	movs r3, #103
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #103
	movs r1, #16
	movs r2, #1
	bl Func_02001848
	ldr r3, [r6, #8]
	asrs r5, r3, #20
	cmp r5, #38
	bne .L_02008fe8
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_SetBit
	movs r3, #16
	str r3, [sp, #4]
	movs r0, #38
	movs r1, #15
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02001848
	b .L_02008ffc
.L_02008fe8:
	movs r3, #39
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #15
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_02008ffc:
	cmp r7, #13
	bne .L_02009030
	ldr r3, [r6, #8]
	asrs r3, r3, #20
	cmp r3, #10
	bne .L_02009018
	ldr r3, [r6, #16]
	asrs r3, r3, #20
	cmp r3, #11
	bne .L_02009018
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_02009018:
	ldr r2, [r6, #8]
	ldr r3, [r6, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #12
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_02009030:
	add sp, #8
	pop {r5, r6, r7, pc}
.L_02009034:
	.4byte gPartyState
.L_02009038:
	.4byte 0x00000050
.L_0200903c:
	.4byte 0x00000051
	.section .text.x02009040,"ax",%progbits
	.global Func_02001040
	.thumb_func
Func_02001040:
	push {r5, lr}
	ldr r5, .L_02009114
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	movs r3, #2
	adds r0, #34
	strb r3, [r0]
	movs r3, #240
	lsls r3, r3, #1
	adds r5, r5, r3
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldr r3, .L_02009118
	cmp r2, r3
	bne .L_02009090
	movs r3, #43
	movs r2, #70
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #55
	movs r1, #71
	movs r2, #4
	movs r3, #1
	bl Func_02001848
	movs r3, #55
	movs r2, #71
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #43
	movs r1, #71
	movs r2, #4
	movs r3, #1
	bl Func_02001848
.L_02009090:
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldr r3, .L_0200911c
	cmp r2, r3
	bne .L_0200910e
	movs r3, #50
	str r3, [sp, #4]
	movs r5, #90
	movs r0, #110
	movs r1, #45
	movs r2, #5
	movs r3, #2
	str r5, [sp, #0]
	bl Func_02001848
	movs r3, #52
	str r3, [sp, #4]
	movs r0, #113
	movs r1, #47
	movs r2, #2
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02001848
	movs r3, #53
	str r3, [sp, #4]
	movs r0, #107
	movs r1, #48
	movs r2, #3
	movs r3, #2
	str r5, [sp, #0]
	bl Func_02001848
	movs r3, #110
	movs r2, #45
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #97
	movs r1, #58
	movs r2, #5
	movs r3, #2
	bl Func_02001848
	movs r3, #113
	movs r2, #47
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #97
	movs r1, #58
	movs r2, #2
	movs r3, #1
	bl Func_02001848
	movs r3, #107
	movs r2, #48
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #97
	movs r1, #58
	movs r2, #3
	movs r3, #2
	bl Func_02001848
.L_0200910e:
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
.L_02009114:
	.4byte gPartyState
.L_02009118:
	.4byte 0x0000004f
.L_0200911c:
	.4byte 0x00000051
	.section .text.x02009120,"ax",%progbits
	.global Func_02001120
	.thumb_func
Func_02001120:
	push {r5, lr}
	ldr r5, .L_020091a8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	movs r3, #0
	adds r0, #34
	strb r3, [r0]
	movs r3, #240
	lsls r3, r3, #1
	adds r5, r5, r3
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldr r3, .L_020091ac
	cmp r2, r3
	bne .L_0200915c
	movs r3, #55
	movs r2, #71
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #43
	movs r1, #70
	movs r2, #4
	movs r3, #1
	bl Func_02001848
.L_0200915c:
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldr r3, .L_020091b0
	cmp r2, r3
	bne .L_020091a2
	movs r3, #110
	movs r2, #45
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #90
	movs r1, #50
	movs r2, #5
	movs r3, #2
	bl Func_02001848
	movs r3, #113
	movs r2, #47
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #90
	movs r1, #52
	movs r2, #2
	movs r3, #1
	bl Func_02001848
	movs r3, #107
	movs r2, #48
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #90
	movs r1, #53
	movs r2, #3
	movs r3, #2
	bl Func_02001848
.L_020091a2:
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
.L_020091a8:
	.4byte gPartyState
.L_020091ac:
	.4byte 0x0000004f
.L_020091b0:
	.4byte 0x00000051
	.section .text.x020091e0,"ax",%progbits
	.balign 4
	.global Func_020011e0
	.thumb_func
Func_020011e0:
	push {r5, r6, r7, lr}
	movs r0, #158
	lsls r0, r0, #2
	bl GameFlag_GetByte
	adds r5, r0, #0
	adds r3, r5, #0
	cmp r5, #0
	bge .L_020091f4
	adds r3, #31
.L_020091f4:
	ldr r2, .L_02009234
	asrs r3, r3, #5
	lsls r3, r3, #2
	ldrsh r7, [r2, r3]
	movs r0, #16
	adds r3, #2
	ldrsh r6, [r2, r3]
	bl Object_GetById
	adds r0, #91
	ldrb r3, [r0]
	cmp r3, #0
	bne .L_02009230
	movs r0, #16
	lsls r1, r7, #19
	lsls r2, r6, #19
	bl Func_020018a8
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009226
	adds r5, #1
.L_02009226:
	movs r0, #158
	lsls r0, r0, #2
	adds r1, r5, #0
	bl GameFlag_SetByte
.L_02009230:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009234:
	.4byte Data_020019cc
	.section .text.x02009238,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_02001238
	.thumb_func
Func_02001238:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	movs r0, #158
	lsls r0, r0, #2
	bl GameFlag_GetByte
	cmp r0, #0
	bge .L_ja_02009250
	adds r0, #31
.L_ja_02009250:
	ldr r2, .L_ja_020092fc
	asrs r3, r0, #5
	lsls r3, r3, #2
	ldrsh r7, [r2, r3]
	adds r3, #2
	ldrsh r6, [r2, r3]
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #200
	ldr r3, [r3, #92]
	lsls r1, r1, #5
	adds r1, #48
	adds r3, r3, r1
	mov r10, r2
	ldr r2, [r3]
	lsls r0, r7, #3
	lsls r5, r6, #3
	movs r1, #0
	movs r4, #0
	cmp r2, #0
	beq .L_ja_02009284
	ldr r3, [r2, #8]
	asrs r1, r3, #20
	ldr r3, [r2, #16]
	asrs r4, r3, #20
.L_ja_02009284:
	adds r3, r0, #0
	cmp r3, #0
	bge .L_ja_0200928c
	adds r3, #15
.L_ja_0200928c:
	asrs r3, r3, #4
	cmp r3, r1
	bne .L_ja_020092f4
	adds r3, r5, #0
	cmp r3, #0
	bge .L_ja_0200929a
	adds r3, #15
.L_ja_0200929a:
	asrs r3, r3, #4
	cmp r3, r4
	bne .L_ja_020092f4
	mov r2, r8
	cmp r2, #1
	bne .L_ja_020092b6
	ldr r0, .L_ja_02009300
	bl Scheduler_RemoveCallbackFar
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_020018a8
.L_ja_020092b6:
	mov r3, r8
	cmp r3, #2
	bne .L_ja_020092f4
	movs r0, #17
	bl Object_GetById
	lsls r1, r7, #19
	adds r5, r0, #0
	lsls r2, r6, #19
	movs r0, #17
	bl Func_020018a8
	movs r3, #128
	lsls r3, r3, #14
	adds r2, r5, #0
	str r3, [r5, #12]
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	movs r3, #184
	lsls r3, r3, #5
	adds r3, #10
	str r3, [r5, #72]
	movs r0, #50
	bl WaitFrames
	movs r2, #176
	lsls r2, r2, #1
	add r2, r10
	movs r3, #17
	strh r3, [r2]
.L_ja_020092f4:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_ja_020092fc:
	.4byte Data_020019cc
.L_ja_02009300:
	.4byte Func_020011e0
	.else
	.global Func_02001238
	.thumb_func
Func_02001238:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	movs r0, #158
	lsls r0, r0, #2
	bl GameFlag_GetByte
	cmp r0, #0
	bge .L_02009250
	adds r0, #31
.L_02009250:
	ldr r2, .L_02009304
	asrs r3, r0, #5
	lsls r3, r3, #2
	ldrsh r7, [r2, r3]
	adds r3, #2
	ldrsh r6, [r2, r3]
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #200
	ldr r3, [r3, #92]
	lsls r1, r1, #5
	adds r1, #48
	adds r3, r3, r1
	mov r10, r2
	ldr r2, [r3]
	lsls r0, r7, #3
	lsls r5, r6, #3
	movs r1, #0
	movs r4, #0
	cmp r2, #0
	beq .L_02009284
	ldr r3, [r2, #8]
	asrs r1, r3, #20
	ldr r3, [r2, #16]
	asrs r4, r3, #20
.L_02009284:
	adds r3, r0, #0
	cmp r3, #0
	bge .L_0200928c
	adds r3, #15
.L_0200928c:
	asrs r3, r3, #4
	cmp r3, r1
	bne .L_020092fc
	adds r3, r5, #0
	cmp r3, #0
	bge .L_0200929a
	adds r3, #15
.L_0200929a:
	asrs r3, r3, #4
	cmp r3, r4
	bne .L_020092fc
	mov r2, r8
	cmp r2, #1
	bne .L_020092b6
	ldr r0, .L_02009308
	bl Scheduler_RemoveCallbackFar
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_020018a8
.L_020092b6:
	mov r3, r8
	cmp r3, #2
	bne .L_020092fc
	movs r0, #17
	bl Object_GetById
	lsls r1, r7, #19
	adds r5, r0, #0
	lsls r2, r6, #19
	movs r0, #17
	bl Func_020018a8
	movs r3, #128
	lsls r3, r3, #14
	adds r2, r5, #0
	str r3, [r5, #12]
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	movs r3, #184
	lsls r3, r3, #5
	adds r3, #10
	str r3, [r5, #72]
	movs r0, #50
	bl WaitFrames
	movs r2, #176
	lsls r2, r2, #1
	add r2, r10
	movs r3, #17
	movs r0, #156
	strh r3, [r2]
	lsls r0, r0, #2
	bl GameFlag_ClearBit
.L_020092fc:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02009304:
	.4byte Data_020019cc
.L_02009308:
	.4byte Func_020011e0
	.endif
	.section .text.x0200930c,"ax",%progbits
	.global Func_0200130c
	.thumb_func
Func_0200130c:
	push {r5, lr}
	bl Func_02001870
	movs r0, #0
	bl Func_02001930
	movs r1, #2
	movs r0, #9
	bl ObjectMotion_SetVariantCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_020018f8
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #3
	bl Func_020018f0
	movs r0, #60
	bl Func_02001900
	movs r0, #60
	bl Battle_WaitMode0
	movs r2, #6
	ldr r0, .L_020093bc
	movs r1, #0
	bl Func_02001860
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_020018f0
	movs r0, #60
	bl Func_02001900
	movs r0, #60
	bl Battle_WaitMode0
	ldr r5, .L_020093c0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009388
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #9
	bl ObjectMotion_ResetAndSetPosition
.L_02009388:
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #149
	movs r3, #200
	lsls r1, r1, #2
	lsls r3, r3, #5
	adds r2, r5, r1
	adds r3, #205
	strh r3, [r2]
	movs r3, #166
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r5, r3
	movs r3, #2
	strb r3, [r2]
	ldr r3, .L_020093c4
	movs r0, #103
	subs r1, #94
	adds r2, r5, r1
	strh r3, [r2]
	movs r1, #1
	bl Func_020018e0
	pop {r5, pc}
	.2byte 0x0000
.L_020093bc:
	.4byte MsgIAmThe
.L_020093c0:
	.4byte gPartyState
.L_020093c4:
	.4byte 0x00000058
	.section .text.x02009418,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.balign 4
	.global Func_02001418
	.thumb_func
Func_02001418:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	lsls r0, r0, #1
	movs r2, #129
	adds r3, r3, r0
	lsls r2, r2, #2
	str r2, [r3]
	movs r0, #0
	sub sp, #8
	bl Func_02001928
	ldr r5, .L_ja_02009558
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_ja_0200955c
	cmp r2, r3
	bne .L_ja_02009500
	ldr r1, .L_ja_02009560
	ldr r2, .L_ja_02009564
	ldr r3, .L_ja_02009568
	ldr r0, .L_ja_0200956c
	bl Func_020007dc
	movs r0, #8
	bl Func_020018c8
	movs r0, #9
	bl Func_020018c8
	movs r0, #10
	bl Func_020018c8
	movs r0, #8
	bl Func_02000038
	movs r0, #9
	bl Func_02000038
	movs r0, #10
	bl Func_02000038
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02009498
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_020018a8
	movs r3, #34
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #32
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_ja_02009498:
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r6, [r3, r2]
	cmp r6, #1
	bne .L_ja_02009500
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #41
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_020094ca
	ldr r3, .L_ja_02009570
	movs r0, #152
	movs r1, #128
	lsls r0, r0, #2
	lsls r1, r1, #2
	adds r2, r5, r0
	adds r1, #98
	strh r3, [r2]
	adds r3, r5, r1
	strh r6, [r3]
	b .L_ja_02009500
.L_ja_020094ca:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #42
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_020094dc
	ldr r2, .L_ja_02009574
	b .L_ja_020094ec
.L_ja_020094dc:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #43
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02009500
	ldr r2, .L_ja_02009578
.L_ja_020094ec:
	movs r0, #152
	movs r1, #128
	lsls r0, r0, #2
	lsls r1, r1, #2
	adds r3, r5, r0
	adds r1, #98
	strh r2, [r3]
	adds r2, r5, r1
	movs r3, #2
	strh r3, [r2]
.L_ja_02009500:
	ldr r3, .L_ja_02009558
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_ja_0200957c
	cmp r2, r3
	bne .L_ja_020095aa
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02009580
	movs r1, #138
	movs r2, #216
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_020018a8
	movs r3, #34
	movs r5, #13
	str r3, [sp, #0]
	movs r0, #32
	movs r1, #13
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02001848
	movs r3, #98
	str r3, [sp, #0]
	movs r0, #95
	movs r1, #3
	movs r2, #3
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02001848
	b .L_ja_02009594
	.2byte 0x0000
.L_ja_02009558:
	.4byte gPartyState
.L_ja_0200955c:
	.4byte 0x0000004f
.L_ja_02009560:
	.4byte Data_020019fc
.L_ja_02009564:
	.4byte Data_02001a28
.L_ja_02009568:
	.4byte Data_02001a54
.L_ja_0200956c:
	.4byte Data_020019ec
.L_ja_02009570:
	.4byte 0x00000043
.L_ja_02009574:
	.4byte 0x0000004a
.L_ja_02009578:
	.4byte 0x0000004b
.L_ja_0200957c:
	.4byte 0x00000050
.L_ja_02009580:
	movs r3, #34
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #32
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_ja_02009594:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_020095aa
	movs r0, #64
	movs r1, #1
	bl Object_SetWideSprite
.L_ja_020095aa:
	ldr r1, .L_ja_0200961c
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_ja_02009620
	cmp r2, r3
	beq .L_ja_020095be
	b .L_ja_020097b0
.L_ja_020095be:
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	movs r0, #128
	subs r3, #3
	lsls r3, r3, #16
	lsls r0, r0, #10
	cmp r3, r0
	bhi .L_ja_02009624
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #10
	movs r1, #255
	ldrh r2, [r0]
	lsls r1, r1, #8
	adds r1, #252
	adds r3, r1, #0
	ands r3, r2
	strh r3, [r0]
	ldr r2, .L_ja_02009614
	ldrh r3, [r0]
	orrs r3, r2
	strh r3, [r0]
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	ldrh r0, [r2]
	adds r3, r1, #0
	ands r3, r0
	strh r3, [r2]
	ldr r0, .L_ja_02009618
	ldrh r3, [r2]
	orrs r3, r0
	strh r3, [r2]
	adds r2, #2
	ldrh r3, [r2]
	ands r1, r3
	strh r1, [r2]
	ldrh r3, [r2]
	orrs r3, r0
	strh r3, [r2]
	b .L_ja_02009624
.L_ja_02009614:
	.4byte 0x00000001
.L_ja_02009618:
	.4byte 0x00000003
.L_ja_0200961c:
	.4byte gPartyState
.L_ja_02009620:
	.4byte 0x00000051
.L_ja_02009624:
	movs r0, #8
	bl Func_020018c8
	movs r0, #9
	bl Func_020018c8
	movs r0, #10
	bl Func_020018c8
	movs r0, #11
	bl Func_020018c8
	movs r0, #8
	bl Func_02000038
	movs r0, #9
	bl Func_02000038
	movs r0, #10
	bl Func_02000038
	movs r0, #11
	bl Func_02000038
	ldr r0, .L_ja_020097bc
	bl Func_02000b54
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200967c
	movs r3, #46
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #48
	movs r1, #12
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_ja_0200967c:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02009694
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_020018a8
.L_ja_02009694:
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_020096c4
	movs r1, #168
	movs r2, #184
	movs r0, #13
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020018a8
	movs r3, #10
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #12
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl Func_02001848
	b .L_ja_020096d8
.L_ja_020096c4:
	movs r3, #8
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #12
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_ja_020096d8:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200970a
	movs r1, #154
	movs r2, #132
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020018a8
	movs r3, #38
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #15
	movs r2, #1
	movs r3, #1
	bl Func_02001848
	b .L_ja_0200971e
.L_ja_0200970a:
	movs r3, #39
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #15
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_ja_0200971e:
	movs r0, #65
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_020097b0
	movs r0, #145
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_020097b0
	movs r0, #158
	lsls r0, r0, #2
	bl GameFlag_GetByte
	adds r5, r0, #0
	movs r0, #185
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	beq .L_ja_02009776
	adds r3, r5, #0
	cmp r5, #0
	bge .L_ja_02009754
	adds r3, #31
.L_ja_02009754:
	movs r0, #16
	movs r2, #0
	movs r1, #0
	asrs r5, r3, #5
	bl Func_020018a8
	ldr r3, .L_ja_020097c0
	lsls r5, r5, #2
	ldrsh r1, [r3, r5]
	adds r5, #2
	ldrsh r2, [r3, r5]
	lsls r1, r1, #19
	lsls r2, r2, #19
	movs r0, #17
	bl Func_020018a8
	b .L_ja_020097b0
.L_ja_02009776:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200978c
	movs r0, #158
	lsls r0, r0, #2
	movs r1, #0
	bl GameFlag_SetByte
.L_ja_0200978c:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_ja_020097c4
	bl Scheduler_AddOrUpdateCallback
	movs r0, #156
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #16
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #16
	bl Object_GetById
	adds r0, #89
	strb r6, [r0]
.L_ja_020097b0:
	movs r0, #0
	bl Func_02001928
	movs r0, #0
	sub sp, #-8
	pop {r5, r6, pc}
.L_ja_020097bc:
	.4byte Data_02001d20
.L_ja_020097c0:
	.4byte Data_020019cc
.L_ja_020097c4:
	.4byte Func_020011e0
	.else
	.balign 4
	.global Func_02001418
	.thumb_func
Func_02001418:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	lsls r0, r0, #1
	movs r2, #129
	adds r3, r3, r0
	lsls r2, r2, #2
	str r2, [r3]
	movs r0, #0
	sub sp, #8
	bl Func_02001928
	ldr r5, .L_02009560
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009564
	cmp r2, r3
	bne .L_02009508
	ldr r1, .L_02009568
	ldr r2, .L_0200956c
	ldr r3, .L_02009570
	ldr r0, .L_02009574
	bl Func_020007dc
	movs r0, #8
	bl Func_020018c8
	movs r0, #9
	bl Func_020018c8
	movs r0, #10
	bl Func_020018c8
	movs r0, #8
	bl Func_02000038
	movs r0, #9
	bl Func_02000038
	movs r0, #10
	bl Func_02000038
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020094a0
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_020018a8
	movs r3, #34
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #32
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_020094a0:
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r6, [r3, r2]
	cmp r6, #1
	bne .L_02009508
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #41
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020094d2
	ldr r3, .L_02009578
	movs r0, #152
	movs r1, #128
	lsls r0, r0, #2
	lsls r1, r1, #2
	adds r2, r5, r0
	adds r1, #98
	strh r3, [r2]
	adds r3, r5, r1
	strh r6, [r3]
	b .L_02009508
.L_020094d2:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #42
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020094e4
	ldr r2, .L_0200957c
	b .L_020094f4
.L_020094e4:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #43
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009508
	ldr r2, .L_02009580
.L_020094f4:
	movs r0, #152
	movs r1, #128
	lsls r0, r0, #2
	lsls r1, r1, #2
	adds r3, r5, r0
	adds r1, #98
	strh r2, [r3]
	adds r2, r5, r1
	movs r3, #2
	strh r3, [r2]
.L_02009508:
	ldr r3, .L_02009560
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009584
	cmp r2, r3
	bne .L_020095b2
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009588
	movs r1, #138
	movs r2, #216
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_020018a8
	movs r3, #34
	movs r5, #13
	str r3, [sp, #0]
	movs r0, #32
	movs r1, #13
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02001848
	movs r3, #98
	str r3, [sp, #0]
	movs r0, #95
	movs r1, #3
	movs r2, #3
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02001848
	b .L_0200959c
	.2byte 0x0000
.L_02009560:
	.4byte gPartyState
.L_02009564:
	.4byte 0x0000004f
.L_02009568:
	.4byte Data_020019fc
.L_0200956c:
	.4byte Data_02001a28
.L_02009570:
	.4byte Data_02001a54
.L_02009574:
	.4byte Data_020019ec
.L_02009578:
	.4byte 0x00000043
.L_0200957c:
	.4byte 0x0000004a
.L_02009580:
	.4byte 0x0000004b
.L_02009584:
	.4byte 0x00000050
.L_02009588:
	movs r3, #34
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #32
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_0200959c:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020095b2
	movs r0, #64
	movs r1, #1
	bl Object_SetWideSprite
.L_020095b2:
	ldr r1, .L_02009624
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009628
	cmp r2, r3
	beq .L_020095c6
	b .L_020097c2
.L_020095c6:
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	movs r0, #128
	subs r3, #3
	lsls r3, r3, #16
	lsls r0, r0, #10
	cmp r3, r0
	bhi .L_0200962c
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #10
	movs r1, #255
	ldrh r2, [r0]
	lsls r1, r1, #8
	adds r1, #252
	adds r3, r1, #0
	ands r3, r2
	strh r3, [r0]
	ldr r2, .L_0200961c
	ldrh r3, [r0]
	orrs r3, r2
	strh r3, [r0]
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	ldrh r0, [r2]
	adds r3, r1, #0
	ands r3, r0
	strh r3, [r2]
	ldr r0, .L_02009620
	ldrh r3, [r2]
	orrs r3, r0
	strh r3, [r2]
	adds r2, #2
	ldrh r3, [r2]
	ands r1, r3
	strh r1, [r2]
	ldrh r3, [r2]
	orrs r3, r0
	strh r3, [r2]
	b .L_0200962c
.L_0200961c:
	.4byte 0x00000001
.L_02009620:
	.4byte 0x00000003
.L_02009624:
	.4byte gPartyState
.L_02009628:
	.4byte 0x00000051
.L_0200962c:
	movs r0, #8
	bl Func_020018c8
	movs r0, #9
	bl Func_020018c8
	movs r0, #10
	bl Func_020018c8
	movs r0, #11
	bl Func_020018c8
	movs r0, #8
	bl Func_02000038
	movs r0, #9
	bl Func_02000038
	movs r0, #10
	bl Func_02000038
	movs r0, #11
	bl Func_02000038
	ldr r0, .L_020097d0
	bl Func_02000b54
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009684
	movs r3, #46
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #48
	movs r1, #12
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_02009684:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200969c
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_020018a8
.L_0200969c:
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020096cc
	movs r1, #168
	movs r2, #184
	movs r0, #13
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020018a8
	movs r3, #10
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #12
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl Func_02001848
	b .L_020096ea
.L_020096cc:
	movs r0, #13
	bl Object_GetById
	ldr r2, [r0, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #12
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_020096ea:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200971c
	movs r1, #154
	movs r2, #132
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020018a8
	movs r3, #38
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #15
	movs r2, #1
	movs r3, #1
	bl Func_02001848
	b .L_02009730
.L_0200971c:
	movs r3, #39
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #15
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_02009730:
	movs r0, #65
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020097c2
	movs r0, #145
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020097c2
	movs r0, #158
	lsls r0, r0, #2
	bl GameFlag_GetByte
	adds r5, r0, #0
	movs r0, #185
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02009788
	adds r3, r5, #0
	cmp r5, #0
	bge .L_02009766
	adds r3, #31
.L_02009766:
	movs r0, #16
	movs r2, #0
	movs r1, #0
	asrs r5, r3, #5
	bl Func_020018a8
	ldr r3, .L_020097d4
	lsls r5, r5, #2
	ldrsh r1, [r3, r5]
	adds r5, #2
	ldrsh r2, [r3, r5]
	lsls r1, r1, #19
	lsls r2, r2, #19
	movs r0, #17
	bl Func_020018a8
	b .L_020097c2
.L_02009788:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200979e
	movs r0, #158
	lsls r0, r0, #2
	movs r1, #0
	bl GameFlag_SetByte
.L_0200979e:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_020097d8
	bl Scheduler_AddOrUpdateCallback
	movs r0, #156
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #16
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #16
	bl Object_GetById
	adds r0, #89
	strb r6, [r0]
.L_020097c2:
	movs r0, #0
	bl Func_02001928
	movs r0, #0
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020097d0:
	.4byte Data_02001d20
.L_020097d4:
	.4byte Data_020019cc
.L_020097d8:
	.4byte Func_020011e0
	.endif
	.section .bss,"aw",%nobits
	.global Data_02001fc8
Data_02001fc8:
	.space 0x00000004
	.global Data_02001fcc
Data_02001fcc:
	.space 0x00000004
	.global Data_02001fd0
Data_02001fd0:
	.space 0x00000004
	.global Data_02001fd4
Data_02001fd4:
	.space 0x00000004
	.global Data_02001fd8
Data_02001fd8:
	.space 0x00000004
	.global Data_02001fdc
Data_02001fdc:
	.space 0x00000004
	.global Data_02001fe0
Data_02001fe0:
	.space 0x00000004
	.global Data_02001fe4
Data_02001fe4:
	.space 0x00000004
	.global Data_02001fe8
Data_02001fe8:
