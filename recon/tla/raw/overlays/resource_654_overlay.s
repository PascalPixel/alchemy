.syntax unified
	.thumb
	.section .text.x020080b8,"ax",%progbits
	.balign 4
	.global Func_020000b8
	.thumb_func
Func_020000b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_02008270
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
	beq .L_02008100
	cmp r7, #0
	beq .L_02008100
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_02008108
.L_02008100:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_02008108:
	mov r3, r10
	bl Func_02003978
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02008116
	b .L_02008262
.L_02008116:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_02003968
	ldr r2, .L_02008274
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02003970
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r3, .L_02008278
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
	bl Object_SetSpritePriority
	movs r2, #100
	adds r2, r2, r6
	mov r9, r2
	mov r3, r9
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r3]
	ldr r3, .L_0200827c
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02008262
	cmp r7, #0
	beq .L_02008262
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_02008198
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_02008198:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020081b8
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_020081b8:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_020081cc
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_020081cc:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02008212
	ldr r3, .L_02008274
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_020081fa
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200820c
.L_020081fa:
	ldr r2, .L_0200827c
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_0200827c
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200820c:
	bl __divsi3
	str r0, [r6, #52]
.L_02008212:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200822e
	adds r0, r6, #0
	movs r1, #1
	bl Func_02003968
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02003970
.L_0200822e:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008240
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_02008240:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008252
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_02008252:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008262
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_02008262:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008270:
	.4byte gPartyState
.L_02008274:
	.4byte gData_02003c04
.L_02008278:
	.4byte BattleFx_UpdateObjectMotionScaleAndLinkedAngle
.L_0200827c:
	.4byte 0xffff0000
	.section .text.x02008568,"ax",%progbits
	.balign 4
	.global Func_02000568
	.thumb_func
Func_02000568:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #160
	adds r5, r3, r1
	lsls r2, r2, #1
	adds r1, #112
	adds r7, r3, r1
	adds r2, r2, r3
	ldr r3, .L_02008610
	mov r8, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02003ad0
	mov r10, r0
	movs r0, #8
	bl Object_GetById
	mov r9, r0
	movs r0, #9
	bl Object_GetById
	mov r11, r0
	movs r0, #10
	bl Object_GetById
	ldr r3, [r5, #12]
	ldr r1, .L_02008614
	mov r2, r8
	adds r3, r3, r1
	str r3, [r5, #12]
	mov r1, r8
	ldr r3, [r2, #12]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r3, [r7, #12]
	mov r1, r10
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02008614
	ldr r3, [r6, #16]
	adds r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r1, #12]
	adds r3, r3, r2
	str r3, [r1, #12]
	mov r2, r9
	ldr r3, [r2, #16]
	ldr r1, .L_02008614
	adds r3, r3, r1
	str r3, [r2, #16]
	mov r2, r11
	ldr r3, [r2, #16]
	adds r3, r3, r1
	str r3, [r2, #16]
	ldr r3, [r0, #16]
	adds r3, r3, r1
	str r3, [r0, #16]
	movs r0, #4
	bl WaitFrames
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008610:
	.4byte gPartyState
.L_02008614:
	.4byte 0xffff8000
	.section .text.x0200863c,"ax",%progbits
	.balign 4
	.global Func_0200063c
	.thumb_func
Func_0200063c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #132
	lsls r2, r2, #1
	adds r7, r3, r2
	adds r2, #56
	sub sp, #8
	adds r6, r3, r2
	movs r3, #18
	movs r2, #6
	str r3, [sp, #0]
	movs r3, #56
	str r2, [sp, #4]
	bl Func_02003998
	movs r5, #0
.L_02008660:
	ldr r3, [r7, #12]
	ldr r2, .L_02008684
	movs r0, #1
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	bl WaitFrames
	adds r3, r5, #1
	lsls r3, r3, #24
	lsrs r5, r3, #24
	cmp r5, #15
	bls .L_02008660
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008684:
	.4byte 0xffffe000
	.section .text.x02008688,"ax",%progbits
	.global Func_02000688
	.thumb_func
Func_02000688:
	push {r5, r6, lr}
	sub sp, #8
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	bl Func_020039a0
	ldr r6, .L_02008878
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r6, r2
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #248
	movs r1, #1
	movs r2, #168
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #16
	bl Camera_MoveWithinMapBounds
	bl Func_02003ac8
	movs r0, #244
	bl Func_02003b48
	movs r3, #128
	movs r5, #128
	lsls r3, r3, #5
	lsls r5, r5, #19
	adds r3, #1
	adds r5, #82
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #2
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #3
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #4
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_020039b8
	movs r2, #3
	str r2, [sp, #4]
	movs r3, #12
	movs r2, #9
	movs r1, #124
	movs r0, #59
	str r3, [sp, #0]
	bl Func_02003998
	ldr r0, [r6]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #1
	ldr r0, [r6]
	bl ObjectMotion_SetActionVariant
	movs r0, #8
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #9
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #10
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r3, #18
	movs r2, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #56
	movs r1, #51
	movs r2, #6
	movs r0, #46
	bl Func_02003998
	movs r0, #10
	bl WaitFrames
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #9
	lsls r1, r1, #9
	bl Func_020039b8
	movs r0, #65
	movs r1, #51
	bl Func_0200063c
	movs r0, #65
	movs r1, #58
	bl Func_0200063c
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #5
	strh r3, [r5]
	movs r0, #65
	movs r1, #65
	bl Func_0200063c
	movs r0, #65
	movs r1, #72
	bl Func_0200063c
	movs r3, #224
	lsls r3, r3, #4
	adds r3, #6
	strh r3, [r5]
	movs r0, #65
	movs r1, #79
	bl Func_0200063c
	movs r0, #65
	movs r1, #86
	bl Func_0200063c
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #7
	strh r3, [r5]
	movs r0, #65
	movs r1, #93
	bl Func_0200063c
	movs r0, #65
	movs r1, #100
	bl Func_0200063c
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #8
	strh r3, [r5]
	movs r0, #84
	movs r1, #51
	bl Func_0200063c
	movs r0, #84
	movs r1, #58
	bl Func_0200063c
	movs r3, #176
	lsls r3, r3, #4
	adds r3, #9
	strh r3, [r5]
	movs r0, #84
	movs r1, #65
	bl Func_0200063c
	movs r0, #84
	movs r1, #72
	bl Func_0200063c
	movs r3, #160
	lsls r3, r3, #4
	adds r3, #10
	strh r3, [r5]
	movs r0, #84
	movs r1, #79
	bl Func_0200063c
	movs r0, #84
	movs r1, #86
	bl Func_0200063c
	movs r3, #144
	lsls r3, r3, #4
	adds r3, #11
	strh r3, [r5]
	movs r0, #84
	movs r1, #93
	bl Func_0200063c
	movs r0, #84
	movs r1, #100
	bl Func_0200063c
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #12
	strh r3, [r5]
	movs r5, #0
.L_02008840:
	adds r5, #1
	bl Func_02000618
	cmp r5, #23
	bls .L_02008840
	movs r5, #0
.L_0200884c:
	adds r5, #1
	bl Func_02000568
	cmp r5, #23
	bls .L_0200884c
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #3
	bl Func_02003ad8
	add sp, #8
	pop {r5, r6, pc}
.L_02008878:
	.4byte gPartyState
	.section .text.x0200887c,"ax",%progbits
	.global Func_0200087c
	.thumb_func
Func_0200087c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #160
	adds r5, r3, r1
	lsls r2, r2, #1
	adds r1, #112
	adds r7, r3, r1
	adds r2, r2, r3
	ldr r3, .L_02008924
	mov r8, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02003ad0
	mov r10, r0
	movs r0, #8
	bl Object_GetById
	mov r9, r0
	movs r0, #9
	bl Object_GetById
	mov r11, r0
	movs r0, #10
	bl Object_GetById
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r5, #12]
	mov r1, r8
	ldr r3, [r1, #12]
	ldr r1, .L_02008928
	adds r3, r3, r1
	mov r1, r8
	str r3, [r1, #12]
	ldr r3, [r7, #12]
	ldr r1, .L_02008928
	adds r3, r3, r1
	str r3, [r7, #12]
	mov r1, r10
	ldr r3, [r6, #16]
	adds r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r1, #12]
	adds r3, r3, r2
	str r3, [r1, #12]
	mov r1, r9
	ldr r3, [r1, #16]
	adds r3, r3, r2
	str r3, [r1, #16]
	mov r1, r11
	ldr r3, [r1, #16]
	adds r3, r3, r2
	str r3, [r1, #16]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
	movs r0, #4
	bl WaitFrames
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008924:
	.4byte gPartyState
.L_02008928:
	.4byte 0xffff8000
	.section .text.x02008994,"ax",%progbits
	.global Func_02000994
	.thumb_func
Func_02000994:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #32]
	movs r2, #132
	lsls r2, r2, #1
	adds r6, r3, r2
	adds r2, #56
	adds r2, r2, r3
	mov r8, r2
	movs r2, #188
	lsls r2, r2, #1
	sub sp, #16
	adds r2, r3, r2
	ldr r5, .L_02008cd8
	str r2, [sp, #12]
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02003ad0
	mov r10, r0
	movs r0, #8
	bl Object_GetById
	mov r9, r0
	movs r0, #9
	bl Object_GetById
	mov r11, r0
	movs r0, #10
	bl Object_GetById
	str r0, [sp, #8]
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r1, r1
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #1
	ldr r0, [r5]
	bl ObjectMotion_SetActionVariant
	movs r0, #8
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #9
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r1, #1
	movs r0, #10
	bl ObjectMotion_SetActionVariant
	bl Func_020039a0
	movs r0, #1
	bl WaitFrames
	movs r3, #25
	str r3, [sp, #4]
	movs r5, #18
	movs r0, #46
	movs r1, #51
	movs r2, #6
	movs r3, #56
	str r5, [sp, #0]
	bl Func_02003998
	movs r3, #6
	str r3, [sp, #4]
	movs r0, #84
	movs r1, #100
	movs r2, #6
	movs r3, #56
	str r5, [sp, #0]
	bl Func_02003998
	movs r2, #3
	movs r3, #12
	str r2, [sp, #4]
	movs r0, #59
	movs r1, #124
	movs r2, #9
	str r3, [sp, #0]
	bl Func_02003998
	movs r2, #128
	movs r3, #128
	lsls r2, r2, #4
	lsls r3, r3, #19
	adds r2, #12
	adds r3, #82
	strh r2, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6, #12]
	ldr r2, .L_02008cdc
	movs r5, #0
	adds r3, r3, r2
	str r3, [r6, #12]
	mov r2, r8
	ldr r3, [r2, #12]
	ldr r2, .L_02008ce0
	adds r3, r3, r2
	mov r2, r8
	str r3, [r2, #12]
	ldr r2, [sp, #12]
	ldr r3, [r2, #12]
	movs r2, #192
	lsls r2, r2, #12
	adds r3, r3, r2
	ldr r2, [sp, #12]
	str r3, [r2, #12]
	ldr r3, [r7, #16]
	ldr r2, .L_02008ce4
	adds r3, r3, r2
	str r3, [r7, #16]
	mov r2, r10
	ldr r3, [r2, #12]
	ldr r2, .L_02008ce4
	adds r3, r3, r2
	mov r2, r10
	str r3, [r2, #12]
	mov r2, r9
	ldr r3, [r2, #16]
	ldr r2, .L_02008ce4
	adds r3, r3, r2
	mov r2, r9
	str r3, [r2, #16]
	mov r2, r11
	ldr r3, [r2, #16]
	ldr r2, .L_02008ce4
	adds r3, r3, r2
	mov r2, r11
	str r3, [r2, #16]
	ldr r2, [sp, #8]
	ldr r3, [r2, #16]
	ldr r2, .L_02008ce4
	adds r3, r3, r2
	ldr r2, [sp, #8]
	str r3, [r2, #16]
	bl Func_02003988
	movs r0, #4
	bl WaitFrames
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_020039b8
	movs r0, #245
	bl Func_02003b48
.L_02008b1e:
	adds r5, #1
	bl Func_0200087c
	cmp r5, #24
	bls .L_02008b1e
	movs r5, #0
.L_02008b2a:
	adds r5, #1
	bl Func_0200092c
	cmp r5, #24
	bls .L_02008b2a
	movs r0, #84
	movs r1, #93
	bl Func_0200094c
	movs r0, #84
	movs r1, #86
	bl Func_0200094c
	movs r3, #144
	movs r5, #128
	lsls r3, r3, #4
	lsls r5, r5, #19
	adds r3, #11
	adds r5, #82
	strh r3, [r5]
	movs r0, #84
	movs r1, #79
	bl Func_0200094c
	movs r0, #84
	movs r1, #72
	bl Func_0200094c
	movs r3, #160
	lsls r3, r3, #4
	adds r3, #10
	strh r3, [r5]
	movs r0, #84
	movs r1, #65
	bl Func_0200094c
	movs r0, #84
	movs r1, #58
	bl Func_0200094c
	movs r3, #176
	lsls r3, r3, #4
	adds r3, #9
	strh r3, [r5]
	movs r0, #84
	movs r1, #51
	bl Func_0200094c
	movs r0, #65
	movs r1, #100
	bl Func_0200094c
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #8
	strh r3, [r5]
	movs r0, #65
	movs r1, #93
	bl Func_0200094c
	movs r0, #65
	movs r1, #86
	bl Func_0200094c
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #7
	strh r3, [r5]
	movs r0, #65
	movs r1, #79
	bl Func_0200094c
	movs r0, #65
	movs r1, #72
	bl Func_0200094c
	movs r3, #224
	lsls r3, r3, #4
	adds r3, #6
	strh r3, [r5]
	movs r0, #65
	movs r1, #65
	bl Func_0200094c
	movs r0, #65
	movs r1, #58
	bl Func_0200094c
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #5
	strh r3, [r5]
	movs r0, #65
	movs r1, #51
	bl Func_0200094c
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_020039b8
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #4
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02003b48
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_020039b8
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #3
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #2
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #1
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #5
	strh r3, [r5]
	movs r3, #18
	movs r2, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #103
	movs r1, #51
	movs r2, #6
	movs r3, #56
	bl Func_02003998
	movs r5, #3
	movs r1, #108
	movs r3, #12
	movs r2, #9
	movs r0, #46
	str r3, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003998
	movs r0, #10
	bl WaitFrames
	bl Func_020039c0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	movs r0, #8
	bl Func_02003854
	movs r0, #9
	bl Func_02003854
	movs r0, #10
	bl Func_02003854
	ldr r3, .L_02008cd8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	adds r3, r7, #0
	movs r1, #0
	adds r3, #85
	strb r5, [r3]
	str r1, [r7, #12]
	str r1, [r7, #20]
	bl Func_020039e0
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008cd8:
	.4byte gPartyState
.L_02008cdc:
	.4byte 0xffbc0000
.L_02008ce0:
	.4byte 0xffec0000
.L_02008ce4:
	.4byte 0xfff40000
	.section .text.x02008ce8,"ax",%progbits
	.global Func_02000ce8
	.thumb_func
Func_02000ce8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #32]
	mov r8, r2
	movs r2, #160
	lsls r2, r2, #1
	ldr r5, .L_02008e30
	adds r6, r3, r2
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	sub sp, #8
	bl Object_GetById
	adds r7, r0, #0
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	movs r0, #244
	bl Func_02003b48
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Camera_MoveWithinMapBounds
	movs r0, #172
	movs r1, #1
	movs r2, #248
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #0
	bl Camera_MoveWithinMapBounds
	movs r3, #19
	movs r2, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #18
	movs r2, #62
	movs r1, #77
	movs r0, #62
	bl Func_02003998
	movs r0, #1
	bl WaitFrames
	movs r1, #2
	ldr r0, [r5]
	bl ObjectMotion_SetActionVariant
	adds r2, r7, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r2, .L_02008e34
	movs r5, #0
	str r2, [r7, #12]
	str r2, [r7, #20]
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	bl Func_02003988
	movs r0, #1
	bl WaitFrames
	mov r2, r8
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #87
	str r2, [r3]
	bl Event_StartScreenTransition
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_020039b8
.L_02008da6:
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r3, [r7, #20]
	movs r0, #2
	adds r3, r3, r2
	str r3, [r7, #20]
	adds r5, #1
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	bl WaitFrames
	cmp r5, #99
	bls .L_02008da6
	movs r0, #144
	lsls r0, r0, #1
	bl Func_02003b48
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02003b48
	movs r1, #0
	movs r0, #0
	movs r2, #0
	bl Func_020039b8
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	ldr r3, .L_02008e30
	adds r2, #11
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r2, #6
	movs r3, #19
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #69
	movs r2, #62
	movs r3, #18
	movs r0, #62
	bl Func_02003998
	movs r0, #1
	bl WaitFrames
	bl Func_020039e0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008e30:
	.4byte gPartyState
.L_02008e34:
	.4byte 0xffce0000
	.section .text.x02008e38,"ax",%progbits
	.global Func_02000e38
	.thumb_func
Func_02000e38:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #160
	lsls r2, r2, #1
	ldr r5, .L_02008f10
	adds r7, r3, r2
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	sub sp, #8
	bl Object_GetById
	adds r6, r0, #0
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Camera_MoveWithinMapBounds
	movs r0, #172
	movs r1, #1
	movs r2, #248
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl Camera_MoveWithinMapBounds
	movs r3, #19
	movs r2, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #18
	movs r0, #62
	movs r1, #77
	movs r2, #62
	bl Func_02003998
	movs r1, #128
	ldr r0, [r5]
	movs r2, #20
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r0, #245
	bl Func_02003b48
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_020039b8
	movs r5, #0
.L_02008ece:
	ldr r3, [r6, #12]
	ldr r2, .L_02008f14
	movs r0, #2
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #20]
	adds r3, r3, r2
	str r3, [r6, #20]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	bl WaitFrames
	cmp r5, #90
	bne .L_02008f00
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #87
	str r2, [r3]
	bl Event_FinishScreenTransition
.L_02008f00:
	adds r5, #1
	cmp r5, #99
	bls .L_02008ece
	movs r0, #1
	bl Func_02003ad8
	add sp, #8
	pop {r5, r6, r7, pc}
.L_02008f10:
	.4byte gPartyState
.L_02008f14:
	.4byte 0xffff8000
	.section .text.x02009340,"ax",%progbits
	.balign 4
	.global Func_02001340
	.thumb_func
Func_02001340:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009420
	mov r8, r0
	ldr r7, [r0, #80]
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	adds r6, r1, #0
	movs r1, #129
	ldr r0, [r3]
	lsls r1, r1, #1
	bl Func_02003ab0
	movs r0, #210
	bl Func_02003b48
	mov r2, r8
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	mov r10, r3
.L_02009370:
	ldr r3, [r6, #12]
	ldr r2, .L_02009424
	movs r5, #0
	adds r3, r3, r2
	str r3, [r6, #12]
.L_0200937a:
	ldrh r3, [r7, #18]
	movs r0, #1
	adds r3, #128
	strh r3, [r7, #18]
	adds r5, #1
	bl WaitFrames
	cmp r5, #3
	bls .L_0200937a
	ldr r3, [r6, #12]
	movs r0, #128
	lsls r0, r0, #9
	adds r3, r3, r0
	str r3, [r6, #12]
	movs r5, #0
.L_02009398:
	ldrh r3, [r7, #18]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #128
	adds r3, r3, r2
	strh r3, [r7, #18]
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #7
	bls .L_02009398
	ldr r3, [r6, #12]
	ldr r0, .L_02009424
	movs r5, #0
	adds r3, r3, r0
	str r3, [r6, #12]
.L_020093ba:
	ldrh r3, [r7, #18]
	movs r0, #1
	adds r3, #128
	strh r3, [r7, #18]
	adds r5, #1
	bl WaitFrames
	cmp r5, #3
	bls .L_020093ba
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #1
	bls .L_02009370
	ldr r3, [r6, #12]
	movs r0, #128
	lsls r0, r0, #9
	adds r3, r3, r0
	str r3, [r6, #12]
	movs r0, #20
	bl WaitFrames
	mov r0, r8
	ldr r2, [r0, #8]
	ldr r3, [r6, #8]
	ldr r1, .L_02009428
	subs r3, r3, r2
	str r3, [r1]
	ldr r2, [r0, #12]
	ldr r3, [r6, #12]
	subs r3, r3, r2
	str r3, [r1, #4]
	ldr r2, [r0, #16]
	ldr r3, [r6, #16]
	subs r3, r3, r2
	str r3, [r1, #8]
	ldr r3, .L_02009420
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009420:
	.4byte gPartyState
.L_02009424:
	.4byte 0xffff0000
.L_02009428:
	.4byte gData_02004a18
	.section .text.x02009474,"ax",%progbits
	.balign 4
	.global Func_02001474
	.thumb_func
Func_02001474:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_0200963c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	sub sp, #68
	bl Object_GetById
	mov r10, r0
	movs r0, #14
	bl Object_GetById
	ldr r3, [r0, #80]
	mov r9, r0
	mov r11, r3
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	movs r1, #204
	movs r2, #164
	lsls r2, r2, #1
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	mov r0, r10
	mov r1, r9
	bl Func_02001340
	movs r3, #24
	movs r2, #70
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #50
	movs r2, #3
	movs r3, #1
	movs r0, #0
	bl Func_020039a8
	movs r0, #140
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #1
	bl WaitFrames
	mov r2, r10
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	movs r0, #14
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	mov r1, r9
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r1, #144
	ldr r0, .L_02009640
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
.L_02009508:
	movs r0, #1
	bl WaitFrames
	mov r4, r10
	ldr r3, [r4, #40]
	cmp r3, #0
	bne .L_02009508
	movs r0, #227
	bl Func_02003b48
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #9
	lsls r1, r1, #10
	bl Func_020039b8
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_02009644
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r7, #0
.L_02009544:
	lsls r5, r7, #12
	adds r0, r5, #0
	bl Math_Cosine
	add r6, sp, #16
	movs r3, #0
	str r0, [r6]
	adds r0, r5, #0
	str r3, [r6, #4]
	bl Math_Sine
	ldr r3, [r6]
	str r0, [r6, #8]
	asrs r2, r3, #1
	adds r3, r3, r2
	str r3, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r6]
	adds r3, r3, r0
	lsls r3, r3, #14
	lsrs r3, r3, #16
	adds r2, r2, r3
	ldr r3, .L_02009648
	adds r7, #1
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200964c
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r5, r5, r3
	adds r5, r5, r4
	str r5, [r6, #8]
	mov r2, r10
	ldr r1, [r2, #12]
	ldr r4, [r6, #4]
	ldr r3, .L_02009650
	ldr r0, [r2, #8]
	adds r1, r1, r3
	ldr r2, [r2, #16]
	ldr r3, [r6]
	str r4, [sp, #0]
	ldr r4, .L_02009654
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_020000b8
	cmp r7, #16
	bls .L_02009544
	movs r3, #128
	lsls r3, r3, #11
	mov r2, r10
	str r3, [r2, #40]
	movs r0, #2
	bl WaitFrames
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_020039b8
	ldr r0, .L_02009640
	bl Scheduler_RemoveCallbackFar
	movs r7, #0
.L_020095de:
	mov r4, r11
	ldrh r3, [r4, #18]
	movs r2, #128
	lsls r2, r2, #2
	adds r3, r3, r2
	strh r3, [r4, #18]
	mov r2, r9
	ldr r3, [r2, #16]
	movs r4, #128
	lsls r4, r4, #9
	adds r3, r3, r4
	str r3, [r2, #16]
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #7
	bls .L_020095de
	bl Func_020039c0
	ldr r3, .L_0200963c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	mov r2, r10
	adds r2, #34
	movs r3, #0
	strb r3, [r2]
	movs r0, #10
	bl WaitFrames
	bl Func_020039e0
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200963c:
	.4byte gPartyState
.L_02009640:
	.4byte Func_02001464
.L_02009644:
	.4byte Effect_AdvanceMotion
.L_02009648:
	.4byte 0xffffa000
.L_0200964c:
	.4byte 0xffffd000
.L_02009650:
	.4byte 0xfffc0000
.L_02009654:
	.4byte 0x01090001
	.section .text.x02009d24,"ax",%progbits
	.balign 4
	.global Func_02001d24
	.thumb_func
Func_02001d24:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #115
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02009d40
	b .L_0200a372
.L_02009d40:
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	bl Func_02003ad0
	adds r0, #85
	strb r6, [r0]
	movs r1, #1
	movs r0, #170
	movs r2, #176
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl Camera_MoveWithinMapBounds
	ldr r1, .L_0200a160
	movs r2, #133
	lsls r2, r2, #2
	adds r1, r1, r2
	mov r9, r1
	ldr r0, [r1]
	movs r2, #204
	movs r1, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #14
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	mov r3, r9
	movs r1, #172
	ldr r0, [r3]
	lsls r1, r1, #1
	movs r2, #184
	bl ObjectMotion_SetPositionAndReset
	mov r1, r9
	ldr r0, [r1]
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #172
	movs r2, #184
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #14
	bl Func_02003a30
	movs r0, #1
	bl WaitFrames
	movs r1, #178
	movs r2, #196
	movs r0, #14
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #14
	bl Func_02003a90
	ldr r0, .L_0200a164
	bl Func_02003a70
	movs r0, #14
	movs r1, #0
	bl Func_02003a80
	mov r2, r9
	ldr r0, [r2]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r1, #184
	movs r2, #168
	movs r0, #13
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02003a30
	mov r3, r9
	movs r1, #128
	ldr r0, [r3]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200a168
	adds r1, #153
	bl Func_02003ab8
	movs r0, #232
	movs r1, #128
	movs r2, #176
	movs r3, #1
	lsls r0, r0, #16
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl Camera_MoveWithinMapBounds
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #13
	ldr r1, .L_0200a16c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #168
	movs r1, #232
	movs r0, #13
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r1, #152
	lsls r1, r1, #6
	ldr r0, .L_0200a170
	adds r1, #102
	bl Func_02003ab8
	movs r0, #174
	movs r1, #1
	movs r2, #216
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #17
	bl Camera_MoveWithinMapBounds
	movs r0, #13
	bl Object_GetById
	adds r5, r0, #0
	ldr r1, [r5, #80]
	movs r0, #13
	mov r8, r1
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #13
	ldr r1, .L_0200a16c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #13
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #168
	movs r0, #13
	movs r1, #248
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #13
	movs r1, #6
	bl Object_SetModeById
	movs r2, #208
	ldr r3, .L_0200a174
	lsls r2, r2, #8
	mov r10, r2
	adds r7, r5, #0
	mov r2, r8
	mov r1, r10
	adds r7, #85
	str r3, [r5, #24]
	strh r1, [r2, #18]
	strb r3, [r7]
	movs r1, #192
	ldr r3, [r5, #12]
	lsls r1, r1, #11
	adds r3, r3, r1
	str r3, [r5, #12]
	movs r0, #13
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #13
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #164
	movs r2, #152
	movs r3, #168
	adds r0, r5, #0
	lsls r1, r1, #17
	lsls r2, r2, #14
	lsls r3, r3, #16
	bl Func_02003990
	movs r0, #13
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r3, #3
	strb r3, [r7]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r5, #20]
	movs r3, #128
	lsls r3, r3, #9
	mov r2, r8
	str r3, [r5, #24]
	movs r1, #1
	strh r6, [r2, #18]
	movs r0, #13
	bl Object_SetModeById
	movs r0, #13
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #13
	ldr r1, .L_0200a16c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #13
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #164
	movs r0, #13
	lsls r1, r1, #1
	movs r2, #184
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #13
	movs r1, #0
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #13
	bl Func_02003aa8
	movs r0, #13
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #14
	movs r1, #0
	bl Func_02003a80
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #13
	bl Func_02003aa8
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r1, #176
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #60
	adds r0, #14
	movs r1, #0
	bl Func_02003a78
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r0, #13
	movs r1, #3
	bl Object_SetModeById
	movs r1, #197
	movs r2, #143
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #12
	bl Func_02003a30
	movs r0, #1
	bl WaitFrames
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200a16c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #197
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #248
	bl ObjectMotion_SetPositionAndReset
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #20
	movs r0, #12
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #13
	bl Func_02003aa8
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	mov r3, r9
	movs r1, #128
	ldr r0, [r3]
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200a168
	adds r1, #153
	bl Func_02003ab8
	movs r0, #188
	movs r1, #1
	movs r2, #132
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	bl Camera_MoveWithinMapBounds
	bl Func_02003ac8
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r1, #152
	lsls r1, r1, #7
	ldr r0, .L_0200a178
	adds r1, #204
	bl Func_02003ab8
	movs r0, #174
	movs r1, #1
	movs r2, #216
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #17
	negs r1, r1
	bl Camera_MoveWithinMapBounds
	mov r1, r9
	ldr r0, [r1]
	movs r1, #12
	bl Object_LinkObjectAndSetCallback
	movs r0, #14
	movs r1, #12
	bl Object_LinkObjectAndSetCallback
	movs r0, #13
	movs r1, #12
	bl Object_LinkObjectAndSetCallback
	movs r1, #197
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #220
	bl ObjectMotion_SetPositionAndReset
	movs r1, #185
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #198
	bl ObjectMotion_SetPositionAndReset
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #14
	ldr r1, .L_0200a16c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #184
	movs r2, #180
	movs r0, #14
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #14
	movs r1, #12
	bl Object_LinkObjectAndSetCallback
	movs r1, #168
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #198
	bl ObjectMotion_SetPositionAndReset
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #13
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r2, #0
	movs r0, #12
	mov r1, r10
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	bl Func_02003a90
	movs r0, #12
	movs r1, #4
	movs r2, #20
	bl ObjectMotion_Launch
	movs r0, #12
	movs r1, #0
	movs r2, #20
	b .L_0200a17c
.L_0200a160:
	.4byte gPartyState
.L_0200a164:
	.4byte MsgFieldPraiseRopeWork
.L_0200a168:
	.4byte 0x0004cccc
.L_0200a16c:
	.4byte 0x00019999
.L_0200a170:
	.4byte 0x00013333
.L_0200a174:
	.4byte 0xffff0000
.L_0200a178:
	.4byte 0x00026666
.L_0200a17c:
	bl Func_02003a78
	movs r1, #6
	adds r1, #255
	movs r2, #40
	movs r0, #13
	bl Func_02003aa8
	movs r1, #2
	movs r2, #60
	adds r1, #255
	movs r0, #12
	bl Func_02003aa8
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r0, #13
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #12
	bl Func_02003aa8
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r1, #128
	movs r2, #20
	movs r0, #13
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r1, #128
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r1, #10
	movs r2, #40
	adds r1, #255
	movs r0, #12
	bl Func_02003aa8
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #12
	bl Func_02003aa8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #13
	bl Func_02003aa8
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r2, #40
	movs r0, #13
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r0, #13
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #13
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200a37c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #13
	ldr r1, .L_0200a37c
	bl ObjectMotion_SetSpeedParameters
	ldr r5, .L_0200a380
	movs r0, #12
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl FieldEvent_WaitFrames
	adds r1, r5, #0
	movs r0, #13
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #40
	bl FieldEvent_WaitFrames
	mov r2, r9
	ldr r0, [r2]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #14
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r0, #14
	movs r1, #0
	movs r2, #40
	bl Func_02003a78
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02003a30
	movs r2, #0
	movs r0, #12
	movs r1, #0
	bl Func_02003a30
	movs r0, #14
	movs r1, #0
	bl Func_02003a80
	mov r3, r9
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	movs r0, #14
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #14
	movs r1, #0
	bl Func_02003a80
	movs r0, #14
	movs r1, #2
	bl Object_SetModeById
	mov r1, r9
	ldr r0, [r1]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a354
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #14
	bl ObjectMotion_ResetAndSetPosition
.L_0200a354:
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02003a30
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #115
	bl GameFlag_SetBit
	bl Func_020039e0
.L_0200a372:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200a37c:
	.4byte 0x00013333
.L_0200a380:
	.4byte gData_02003c58
	.section .text.x0200ad50,"ax",%progbits
	.balign 4
	.global Func_02002d50
	.thumb_func
Func_02002d50:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #124
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ad62
	b .L_0200ae6a
.L_0200ad62:
	movs r0, #115
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ae6a
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	movs r0, #14
	bl Object_GetById
	movs r2, #170
	lsls r2, r2, #1
	adds r5, r5, r2
	movs r2, #0
	ldrsh r3, [r5, r2]
	subs r3, #13
	cmp r3, #7
	bhi .L_0200ae6a
	ldr r2, .L_0200ae6c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	adds r0, #98
	mov pc, r3
	.2byte 0x0000
.L_0200ad94:
	.4byte .L_0200adb4
	.4byte .L_0200adca
	.4byte .L_0200add6
	.4byte .L_0200adfa
	.4byte .L_0200ae10
	.4byte .L_0200ae26
	.4byte .L_0200ae4a
	.4byte .L_0200ae56
.L_0200adb4:
	ldrb r3, [r0]
	cmp r3, #0
	bne .L_0200adbe
	bl Func_020027f8
.L_0200adbe:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_ClearBit
	b .L_0200ae6a
.L_0200adca:
	ldrb r3, [r0]
	cmp r3, #0
	bne .L_0200ae6a
	bl Func_020029a4
	b .L_0200ae6a
.L_0200add6:
	ldrb r0, [r0]
	cmp r0, #1
	bne .L_0200ade2
	bl Func_02002830
	b .L_0200ae6a
.L_0200ade2:
	cmp r0, #4
	bne .L_0200ae6a
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ae6a
	bl Func_02002b50
	b .L_0200ae6a
.L_0200adfa:
	ldrb r0, [r0]
	cmp r0, #1
	bne .L_0200ae06
	bl Func_02002b18
	b .L_0200ae6a
.L_0200ae06:
	cmp r0, #5
	bne .L_0200ae6a
	bl Func_02002c50
	b .L_0200ae6a
.L_0200ae10:
	ldrb r0, [r0]
	cmp r0, #2
	bne .L_0200ae1c
	bl Func_0200291c
	b .L_0200ae6a
.L_0200ae1c:
	cmp r0, #4
	bne .L_0200ae6a
	bl Func_02002c18
	b .L_0200ae6a
.L_0200ae26:
	ldrb r0, [r0]
	cmp r0, #2
	bne .L_0200ae32
	bl Func_02002a2c
	b .L_0200ae6a
.L_0200ae32:
	cmp r0, #5
	bne .L_0200ae6a
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ae6a
	bl Func_02002c88
	b .L_0200ae6a
.L_0200ae4a:
	ldrb r3, [r0]
	cmp r3, #3
	bne .L_0200ae6a
	bl Func_0200296c
	b .L_0200ae6a
.L_0200ae56:
	ldrb r3, [r0]
	cmp r3, #3
	bne .L_0200ae60
	bl Func_020029dc
.L_0200ae60:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_ClearBit
.L_0200ae6a:
	pop {r5, pc}
.L_0200ae6c:
	.4byte .L_0200ad94
	.section .text.x0200b598,"ax",%progbits
	.balign 4
	.global Func_02003598
	.thumb_func
Func_02003598:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	movs r3, #13
	ldrb r2, [r1, #23]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #23]
	movs r0, #8
	sub sp, #8
	bl Func_02002f88
	movs r0, #9
	bl Func_02002f88
	movs r0, #10
	bl Func_02002f88
	movs r0, #11
	bl Func_02002f88
	movs r0, #12
	bl Func_02002f88
	movs r0, #13
	bl Func_02002f88
	movs r0, #14
	bl Func_02002f88
	movs r0, #15
	bl Func_02002f88
	movs r0, #16
	bl Func_02002f88
	movs r0, #13
	movs r1, #0
	bl Object_SetModeById
	movs r0, #14
	movs r1, #0
	bl Object_SetModeById
	movs r0, #15
	movs r1, #2
	bl Object_SetModeById
	movs r0, #16
	movs r1, #2
	bl Object_SetModeById
	movs r0, #153
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b658
	movs r3, #13
	str r3, [sp, #4]
	movs r5, #19
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl Func_020039a8
	movs r3, #33
	str r3, [sp, #4]
	movs r1, #0
	movs r2, #2
	movs r3, #1
	movs r0, #40
	str r5, [sp, #0]
	bl Func_020039a8
	movs r0, #15
	bl Object_GetById
	movs r1, #160
	lsls r1, r1, #17
	ldr r2, .L_0200b738
	ldr r3, .L_0200b73c
	bl Object_SetPositionAndResetMotion
	movs r0, #15
	movs r1, #3
	bl ObjectMotion_SetActionVariant
.L_0200b658:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #50
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b6a6
	movs r3, #22
	str r3, [sp, #4]
	movs r5, #24
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl Func_020039a8
	movs r1, #0
	movs r2, #2
	movs r3, #1
	movs r0, #40
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_020039a8
	movs r0, #16
	bl Object_GetById
	movs r1, #200
	movs r3, #195
	lsls r1, r1, #17
	ldr r2, .L_0200b738
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	movs r0, #16
	movs r1, #3
	bl ObjectMotion_SetActionVariant
.L_0200b6a6:
	bl Func_02003b18
	movs r1, #130
	lsls r1, r1, #1
	movs r0, #0
	adds r1, #255
	movs r2, #17
	movs r3, #18
	bl Func_02003b20
	movs r1, #129
	lsls r1, r1, #2
	movs r2, #19
	movs r3, #20
	movs r0, #1
	bl Func_02003b20
	movs r0, #21
	bl Func_02003aa0
	movs r0, #21
	bl Object_GetById
	movs r3, #0
	mov r8, r3
	mov r3, r8
	adds r0, #85
	strb r3, [r0]
	movs r0, #22
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #22
	bl Func_02003aa0
	adds r2, r5, #0
	adds r2, #35
	ldrb r3, [r2]
	movs r6, #2
	orrs r3, r6
	strb r3, [r2]
	adds r5, #85
	mov r3, r8
	strb r3, [r5]
	movs r0, #23
	bl Func_02003aa0
	movs r0, #23
	bl Object_GetById
	mov r3, r8
	adds r0, #85
	strb r3, [r0]
	movs r0, #24
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #24
	bl Func_02003aa0
	adds r2, r5, #0
	adds r2, #35
	ldrb r3, [r2]
	adds r5, #85
	orrs r6, r3
	mov r3, r8
	strb r6, [r2]
	add sp, #8
	strb r3, [r5]
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b738:
	.4byte 0xffeb0000
.L_0200b73c:
	.4byte 0x02160000
	.section .bss,"aw",%nobits
	.global gData_02004a18
gData_02004a18:
