.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080ce,"ax",%progbits
	.2byte 0x0000
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
	bl Func_02003cc4
	ldr r2, .L_02008330
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl Func_02003ccc
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
	bl Func_02003cc4
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02003ccc
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
	.4byte Data_020044f8
.L_02008334:
	.4byte BattleFx_UpdateObjectMotionScaleAndLinkedAngle
.L_02008338:
	.4byte 0xffff0000
	.section .text.x0200842c,"ax",%progbits
	.global Func_0200042c
	.thumb_func
Func_0200042c:
	push {r5, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	sub sp, #8
	bl Object_GetById
	adds r2, r0, #0
	cmp r5, #10
	bne .L_02008486
	ldr r3, [r2, #16]
	asrs r5, r3, #20
	cmp r5, #12
	bne .L_020084ac
	adds r1, r2, #0
	adds r1, #85
	movs r3, #0
	strb r3, [r1]
	ldr r1, .L_020084b0
	ldr r3, [r2, #12]
	movs r0, #144
	adds r3, r3, r1
	lsls r0, r0, #4
	str r3, [r2, #12]
	adds r0, #90
	bl GameFlag_SetBit
	movs r3, #9
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #12
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02003d1c
	movs r3, #13
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #12
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02003d1c
	b .L_020084ac
.L_02008486:
	cmp r5, #11
	bne .L_020084ac
	ldr r3, [r2, #8]
	asrs r3, r3, #20
	cmp r3, #51
	bne .L_020084ac
	adds r1, r2, #0
	adds r1, #85
	movs r3, #0
	strb r3, [r1]
	ldr r1, .L_020084b0
	ldr r3, [r2, #12]
	movs r0, #144
	adds r3, r3, r1
	lsls r0, r0, #4
	str r3, [r2, #12]
	adds r0, #91
	bl GameFlag_SetBit
.L_020084ac:
	add sp, #8
	pop {r5, pc}
.L_020084b0:
	.4byte 0xffff0000
	.section .text.x020084bc,"ax",%progbits
	.global Func_020004bc
	.thumb_func
Func_020004bc:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	adds r5, r1, #0
	movs r2, #160
	lsls r2, r2, #1
	adds r0, r5, #0
	sub sp, #8
	adds r6, r3, r2
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	adds r0, r5, #0
	bl Func_02001e78
	movs r1, #3
	adds r0, r5, #0
	bl Object_SetModeById
	ldr r2, .L_02008570
	ldr r3, [r6, #12]
	movs r5, #0
	adds r3, r3, r2
	str r3, [r6, #12]
	bl Func_02003d54
	bl Func_02003cdc
	movs r0, #12
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	adds r7, r3, #0
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	movs r0, #139
	lsls r0, r0, #2
	bl Func_02003ef4
	subs r7, #42
.L_0200851a:
	lsls r0, r5, #16
	bl Func_02003460
	movs r1, #0
	adds r0, r7, #0
	bl Func_0200349c
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r0, #4
	adds r5, #1
	bl WaitFrames
	cmp r5, #31
	ble .L_0200851a
	movs r0, #148
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200855e
	movs r3, #43
	movs r2, #44
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #41
	movs r1, #42
	movs r2, #1
	movs r3, #1
	bl Func_02003d1c
.L_0200855e:
	bl Func_02003d84
	movs r0, #140
	lsls r0, r0, #2
	bl GameFlag_SetBit
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008570:
	.4byte 0xffe00000
	.section .text.x02008574,"ax",%progbits
	.global Func_02000574
	.thumb_func
Func_02000574:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	adds r5, r1, #0
	movs r2, #160
	lsls r2, r2, #1
	adds r0, r5, #0
	adds r6, r3, r2
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	adds r0, r5, #0
	bl Func_02001e78
	adds r0, r5, #0
	movs r1, #3
	bl Object_SetModeById
	ldr r2, .L_020085f0
	ldr r3, [r6, #12]
	movs r5, #0
	adds r3, r3, r2
	str r3, [r6, #12]
	bl Func_02003d54
	bl Func_02003cdc
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	movs r0, #139
	lsls r0, r0, #2
	bl Func_02003ef4
.L_020085c2:
	lsls r0, r5, #16
	bl Func_02003460
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r0, #4
	adds r5, #1
	bl WaitFrames
	cmp r5, #31
	ble .L_020085c2
	bl Func_02003d84
	movs r0, #153
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020085f0:
	.4byte 0xffe00000
	.global Data_020005f4
Data_020005f4:
	.4byte 0x00004770
	.section .text.x0200861c,"ax",%progbits
	.global Func_0200061c
	.thumb_func
Func_0200061c:
	.ifdef TLA_EDITION_JA
	.else
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	adds r7, r6, #0
	adds r7, #85
	movs r3, #3
	strb r3, [r7]
.L_02008628:
	movs r0, #1
	bl WaitFrames
	ldr r5, [r6, #40]
	cmp r5, #0
	bne .L_02008628
	movs r0, #188
	bl Func_02003ef4
	movs r0, #10
	bl WaitFrames
	strb r5, [r7]
	pop {r5, r6, r7, pc}
	.endif
	.section .text.x02008644,"ax",%progbits
	.global Func_02000644
	.thumb_func
Func_02000644:
	.ifdef TLA_EDITION_JA
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r1, #0
	adds r0, r7, #0
	sub sp, #8
	bl Object_GetById
	cmp r7, #9
	bne .L_ja_02008668
	ldr r3, [r0, #8]
	asrs r6, r3, #20
	cmp r6, #39
	bne .L_ja_02008668
	adds r5, r0, #0
	movs r3, #0
	adds r5, #85
	mov r8, r3
	movs r3, #3
	strb r3, [r5, #0]
	movs r0, #15
	bl WaitFrames
	movs r0, #144
	mov r3, r8
	lsls r0, r0, #4
	strb r3, [r5, #0]
	adds r0, #92
	bl GameFlag_SetBit
	movs r0, #41
	movs r1, #9
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r7, [sp, #4]
	bl Func_02003d1c
.L_ja_02008668:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.else
	push {r5, r6, r7, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	sub sp, #8
	bl Object_GetById
	adds r6, r0, #0
	cmp r5, #9
	bne .L_0200868c
	ldr r3, [r6, #8]
	asrs r7, r3, #20
	cmp r7, #39
	bne .L_0200868c
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	adds r0, r6, #0
	bl Func_0200061c
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #92
	bl GameFlag_SetBit
	movs r0, #41
	movs r1, #9
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003d1c
	bl Func_02003d84
.L_0200868c:
	add sp, #8
	pop {r5, r6, r7, pc}
	.endif
	.section .text.x0200871c,"ax",%progbits
	.global Func_0200071c
	.thumb_func
Func_0200071c:
	push {lr}
	sub sp, #8
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	ldr r3, .L_020087a0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #128
	ldr r0, [r3]
	movs r2, #0
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #151
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02003ef4
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_020087a4
	bl Scheduler_AddOrUpdateCallback
	movs r0, #160
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl Func_02003e44
	movs r0, #194
	movs r1, #1
	movs r2, #162
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02003e54
	movs r0, #30
	bl Battle_WaitMode0
	bl Func_02003d84
	movs r0, #144
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r3, #46
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #59
	movs r1, #16
	movs r2, #5
	movs r3, #32
	bl Func_02003d1c
	add sp, #8
	pop {pc}
	.2byte 0x0000
.L_020087a0:
	.4byte gPartyState
.L_020087a4:
	.4byte Func_020006d0
	.section .text.x020087d4,"ax",%progbits
	.global Func_020007d4
	.thumb_func
Func_020007d4:
	push {r5, r6, lr}
	sub sp, #8
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	ldr r3, .L_0200894c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #192
	ldr r0, [r3]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	bl Func_020007a8
	movs r0, #78
	bl Func_02003ef4
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #162
	bl Func_02003ef4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Func_02003d44
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Func_02003d44
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #10
	lsls r0, r0, #9
	bl Func_02003d44
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008950
	bl Scheduler_AddOrUpdateCallback
	movs r0, #160
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl Func_02003e44
	movs r0, #156
	movs r1, #1
	movs r2, #252
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #17
	bl Motion_CamBounds
	movs r0, #164
	bl Func_02003ef4
	movs r3, #80
	movs r2, #27
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #75
	movs r1, #96
	movs r2, #7
	movs r3, #13
	bl Func_02003d24
	movs r6, #31
	movs r5, #16
	movs r0, #91
	movs r1, #96
	movs r2, #7
	movs r3, #10
	str r6, [sp, #4]
	str r5, [sp, #0]
	bl Func_02003d24
	movs r3, #91
	str r3, [sp, #4]
	movs r0, #99
	movs r1, #96
	movs r2, #7
	movs r3, #14
	str r5, [sp, #0]
	bl Func_02003d24
	movs r3, #17
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #0
	movs r2, #5
	movs r3, #9
	str r6, [sp, #4]
	bl Func_02003d1c
	movs r6, #0
.L_020088ca:
	movs r5, #0
.L_020088cc:
	movs r3, #1
	ldr r2, .L_02008954
	bics r3, r5
	adds r3, r6, r3
	lsls r3, r3, #1
	ldrb r0, [r2, r3]
	adds r3, #1
	ldrb r1, [r2, r3]
	movs r3, #80
	movs r2, #27
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #13
	movs r2, #7
	bl Func_02003d24
	adds r5, #1
	movs r0, #1
	bl WaitFrames
	cmp r5, #6
	ble .L_020088cc
	adds r6, #1
	cmp r6, #14
	ble .L_020088ca
	bl Func_02003e54
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02003d44
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02003ef4
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #80
	bl Func_02003ef4
	bl AudioCommand_WaitForCompletion
	bl Func_02003d84
	bl Func_02003ebc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #93
	bl GameFlag_SetBit
	movs r0, #68
	adds r0, #255
	bl GameFlag_SetBit
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200894c:
	.4byte gPartyState
.L_02008950:
	.4byte Func_020006d0
.L_02008954:
	.4byte Data_02003fb8
	.section .text.x02008958,"ax",%progbits
	.global Func_02000958
	.thumb_func
Func_02000958:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #8
	mov r8, r0
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	movs r0, #188
	bl Func_02003ef4
	movs r5, #1
	movs r6, #2
	movs r1, #96
	movs r2, #19
	movs r3, #29
	movs r0, #107
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003cf4
	movs r0, #8
	bl Battle_WaitMode0
	movs r1, #96
	movs r2, #19
	movs r3, #29
	movs r0, #108
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003cf4
	movs r0, #8
	bl Battle_WaitMode0
	movs r1, #98
	movs r2, #19
	movs r3, #29
	movs r0, #107
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003cf4
	movs r0, #8
	bl Battle_WaitMode0
	movs r1, #98
	movs r2, #19
	movs r3, #29
	movs r0, #108
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003cf4
	movs r0, #15
	bl Battle_WaitMode0
	ldr r5, .L_02008a20
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #52]
	str r3, [r6, #48]
	movs r0, #123
	bl Func_02003ef4
	movs r1, #156
	movs r2, #248
	lsls r1, r1, #1
	lsls r2, r2, #1
	ldr r0, [r5]
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #5
	bl Battle_WaitMode0
	mov r0, r8
	bl Func_02003e5c
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_02008a20:
	.4byte gPartyState
	.section .text.x02008a98,"ax",%progbits
	.global Func_02000a98
	.thumb_func
Func_02000a98:
	push {r5, lr}
	bl Func_02003628
	movs r1, #4
	movs r2, #16
	movs r3, #2
	movs r0, #16
	bl Func_02003688
	movs r0, #22
	movs r1, #4
	movs r2, #16
	movs r3, #0
	bl Func_02003688
	movs r5, #5
.L_02008ab8:
	movs r1, #55
	movs r0, #33
	bl Func_020036b8
	subs r5, #1
	movs r0, #15
	bl WaitFrames
	cmp r5, #0
	bge .L_02008ab8
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #102
	bl GameFlag_SetBit
	pop {r5, pc}
	.section .text.x02008bd4,"ax",%progbits
	.balign 4
	.global Func_02000bd4
	.thumb_func
Func_02000bd4:
	push {r5, r6, lr}
	movs r0, #165
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008c24
	movs r1, #38
	movs r0, #12
	bl Func_02000b24
	bl Func_02003628
	movs r0, #12
	movs r1, #2
	movs r2, #45
	movs r3, #1
	bl Func_02003688
	movs r5, #3
.L_02008bfe:
	movs r0, #33
	movs r1, #55
	bl Func_020036b8
	subs r5, #1
	adds r6, r0, #0
	cmp r5, #0
	bge .L_02008bfe
	movs r0, #146
	lsls r0, r0, #2
	bl GameFlag_SetBit
	cmp r6, #1
	bne .L_02008c24
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #100
	bl GameFlag_SetBit
.L_02008c24:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008cc8,"ax",%progbits
	.global Func_02000cc8
	.thumb_func
Func_02000cc8:
	push {r5, r6, lr}
	ldr r3, .L_02008dcc
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008dd0
	sub sp, #12
	adds r6, r0, #0
	movs r5, #1
	cmp r2, r3
	bne .L_02008d00
	movs r0, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r6, #77
	bne .L_02008cf6
	cmp r3, #17
	bne .L_02008cf6
	movs r5, #0
.L_02008cf6:
	cmp r6, #78
	bne .L_02008d00
	cmp r3, #19
	bne .L_02008d00
	movs r5, #0
.L_02008d00:
	ldr r3, .L_02008dcc
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008dd4
	cmp r2, r3
	bne .L_02008d3a
	cmp r6, #77
	bne .L_02008d26
	movs r0, #10
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #9
	bne .L_02008d26
	movs r5, #0
.L_02008d26:
	cmp r6, #78
	bne .L_02008d3a
	movs r0, #11
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #13
	bne .L_02008d3a
	movs r5, #0
.L_02008d3a:
	cmp r5, #0
	beq .L_02008dc6
	ldr r3, .L_02008dcc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	b .L_02008da8
.L_02008d5a:
	cmp r0, #0
	bge .L_02008d6c
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	adds r0, r6, #0
	bl Func_02000ca8
	b .L_02008dc6
.L_02008d6c:
	ldr r3, [r5, #16]
	movs r1, #128
	lsls r1, r1, #10
	adds r3, r3, r1
	str r3, [r5, #16]
	ldr r2, [r5, #8]
	ldr r3, .L_02008dd8
	ldr r1, .L_02008ddc
	ands r3, r2
	adds r3, r3, r1
	movs r1, #128
	lsls r1, r1, #9
	cmp r3, r1
	ble .L_02008d8c
	movs r3, #128
	lsls r3, r3, #9
.L_02008d8c:
	ldr r1, .L_02008de0
	cmp r3, r1
	bge .L_02008d94
	ldr r3, .L_02008de0
.L_02008d94:
	subs r3, r2, r3
	str r3, [r5, #8]
	ldrh r3, [r5, #6]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r5, #6]
	movs r0, #1
	bl WaitFrames
.L_02008da8:
	ldr r3, [r5, #8]
	mov r1, sp
	str r3, [r1]
	movs r2, #128
	ldr r3, [r5, #12]
	lsls r2, r2, #12
	str r3, [r1, #4]
	adds r0, r5, #0
	ldr r3, [r5, #16]
	adds r3, r3, r2
	str r3, [r1, #8]
	bl Func_02003d2c
	cmp r0, #0
	ble .L_02008d5a
.L_02008dc6:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008dcc:
	.4byte gPartyState
.L_02008dd0:
	.4byte 0x000000a4
.L_02008dd4:
	.4byte 0x000000a5
.L_02008dd8:
	.4byte 0x000fffff
.L_02008ddc:
	.4byte 0xfff80000
.L_02008de0:
	.4byte 0xffff0000
	.section .text.x02008e18,"ax",%progbits
	.global Func_02000e18
	.thumb_func
Func_02000e18:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, .L_02008ec8
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	mov r8, r0
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	ldr r0, [r5]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	adds r3, r6, #0
	adds r3, #85
	movs r7, #0
	strb r7, [r3]
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #32
	orrs r3, r2
	strb r3, [r1]
	adds r0, r6, #0
	movs r1, #18
	bl Func_02003cc4
	movs r0, #215
	bl Func_02003ef4
	movs r0, #13
	bl WaitFrames
	adds r0, r6, #0
	movs r1, #9
	bl Animation_ApplyChildValues
	adds r0, r6, #0
	movs r1, #28
	bl Func_02003cc4
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #54
	bl Func_02003ef4
	movs r5, #0
.L_02008e86:
	cmp r5, #30
	bne .L_02008e8e
	bl Event_FinishScreenTransition
.L_02008e8e:
	movs r3, #128
	lsls r3, r3, #7
	adds r7, r7, r3
	ldr r3, [r6, #12]
	adds r3, r3, r7
	str r3, [r6, #12]
	movs r3, #7
	ands r3, r5
	cmp r3, #0
	bne .L_02008eac
	movs r0, #15
	bl Object_GetById
	bl Func_02002bd4
.L_02008eac:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	ble .L_02008e86
	bl Func_02003d84
	mov r0, r8
	bl Func_02003e5c
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008ec8:
	.4byte gPartyState
	.section .text.x02008ecc,"ax",%progbits
	.global Func_02000ecc
	.thumb_func
Func_02000ecc:
	push {r5, r6, lr}
	adds r0, r1, #0
	sub sp, #8
	bl Object_GetById
	adds r2, r0, #0
	ldr r3, [r2, #8]
	asrs r3, r3, #20
	cmp r3, #49
	beq .L_02008ee2
	b .L_02009002
.L_02008ee2:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #160
	lsls r1, r1, #1
	adds r6, r3, r1
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	ldr r3, .L_02009008
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #128
	ldr r0, [r3]
	movs r2, #0
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #151
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02003ef4
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
	movs r3, #113
	movs r2, #39
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #114
	movs r1, #39
	movs r2, #1
	movs r3, #2
	bl Func_02003d24
	movs r5, #31
.L_02008f34:
	ldr r3, [r6, #12]
	ldr r1, .L_0200900c
	movs r0, #4
	adds r3, r3, r1
	str r3, [r6, #12]
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_02008f34
	movs r5, #13
.L_02008f4a:
	adds r0, r5, #0
	bl Object_GetById
	adds r2, r0, #0
	adds r3, r2, #0
	adds r3, #100
	movs r1, #0
	ldrsh r3, [r3, r1]
	adds r0, r5, #0
	lsls r3, r3, #16
	str r3, [r2, #8]
	adds r3, r2, #0
	adds r3, #102
	movs r1, #0
	ldrsh r3, [r3, r1]
	adds r1, r2, #0
	lsls r3, r3, #16
	str r3, [r2, #16]
	adds r1, #85
	movs r3, #0
	strb r3, [r1]
	ldr r3, .L_02009010
	movs r1, #3
	str r3, [r2, #20]
	str r3, [r2, #12]
	adds r5, #1
	bl ObjectMotion_SetActionVariant
	cmp r5, #15
	ble .L_02008f4a
	bl Func_02003d04
	movs r5, #12
.L_02008f8c:
	ldr r1, .L_02009014
	ldr r0, .L_02009018
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r3, [r1]
	cmp r3, #31
	bgt .L_02008fc0
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	lsls r2, r2, #2
	strh r3, [r1]
	movs r3, #128
	adds r2, r2, r1
	lsls r3, r3, #5
	adds r2, #4
	orrs r3, r5
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2]
.L_02008fc0:
	strh r4, [r0]
	movs r0, #4
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_02008f8c
	movs r5, #41
	movs r0, #105
	movs r1, #41
	movs r2, #12
	movs r3, #13
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003d1c
	movs r3, #105
	str r3, [sp, #0]
	movs r0, #105
	movs r1, #105
	movs r2, #12
	movs r3, #13
	str r5, [sp, #4]
	bl Func_02003d24
	bl Func_02003cfc
	bl Func_02003d84
	movs r0, #142
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_02009002:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009008:
	.4byte gPartyState
.L_0200900c:
	.4byte 0xffff0000
.L_02009010:
	.4byte 0xffe00000
.L_02009014:
	.4byte gIoWriteQueue
.L_02009018:
	.4byte 0x04000208
	.section .text.x0200907c,"ax",%progbits
	.global Func_0200107c
	.thumb_func
Func_0200107c:
	push {r5, r6, lr}
	subs r1, #10
	sub sp, #8
	cmp r1, #1
	bhi .L_020090ca
	movs r0, #10
	bl Object_GetById
	adds r3, r0, #0
	adds r3, #85
	movs r5, #0
	strb r5, [r3]
	movs r1, #33
	ldr r3, [r0, #8]
	movs r2, #1
	asrs r3, r3, #20
	str r3, [sp, #0]
	movs r6, #34
	movs r3, #1
	movs r0, #8
	str r6, [sp, #4]
	bl Func_02003d1c
	movs r0, #11
	bl Object_GetById
	adds r3, r0, #0
	adds r3, #85
	strb r5, [r3]
	movs r1, #33
	ldr r3, [r0, #8]
	movs r2, #1
	asrs r3, r3, #20
	str r3, [sp, #0]
	movs r0, #8
	movs r3, #1
	str r6, [sp, #4]
	bl Func_02003d1c
.L_020090ca:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02009170,"ax",%progbits
	.global Func_02001170
	.thumb_func
Func_02001170:
	push {r5, r6, lr}
	movs r0, #166
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020091d2
	movs r0, #12
	movs r1, #38
	bl Func_02000b24
	movs r0, #13
	movs r1, #39
	bl Func_02000b24
	movs r1, #37
	movs r0, #15
	bl Func_02000b24
	bl Func_02003628
	movs r0, #38
	movs r1, #2
	movs r2, #19
	movs r3, #1
	bl Func_02003688
	movs r5, #8
.L_020091aa:
	movs r0, #33
	movs r1, #55
	bl Func_020036b8
	subs r5, #1
	adds r6, r0, #0
	cmp r5, #0
	bge .L_020091aa
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #74
	bl GameFlag_SetBit
	cmp r6, #1
	bne .L_020091d2
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #101
	bl GameFlag_SetBit
.L_020091d2:
	pop {r5, r6, pc}
	.section .text.x020091d4,"ax",%progbits
	.global Func_020011d4
	.thumb_func
Func_020011d4:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	adds r5, r1, #0
	movs r2, #160
	lsls r2, r2, #1
	adds r0, r5, #0
	adds r6, r3, r2
	bl Object_GetById
	movs r1, #5
	bl Object_SetPartAttribute
	adds r0, r5, #0
	bl Func_02001e78
	adds r0, r5, #0
	movs r1, #3
	bl Object_SetModeById
	ldr r2, .L_02009248
	ldr r3, [r6, #12]
	movs r5, #31
	adds r3, r3, r2
	str r3, [r6, #12]
	bl Func_02003d54
	bl Func_02003cdc
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	movs r0, #139
	lsls r0, r0, #2
	bl Func_02003ef4
.L_02009222:
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r0, #4
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_02009222
	bl Func_02003d84
	movs r0, #166
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	pop {r5, r6, pc}
.L_02009248:
	.4byte 0xffe00000
	.section .text.x0200924c,"ax",%progbits
	.global Func_0200124c
	.thumb_func
Func_0200124c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #160
	lsls r2, r2, #1
	adds r2, r2, r3
	sub sp, #8
	mov r8, r2
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	mov r2, r8
	ldr r3, [r2, #12]
	ldr r2, .L_02009324
	movs r7, #14
	adds r3, r3, r2
	mov r2, r8
	str r3, [r2, #12]
	bl Func_02003cdc
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_02003d44
.L_0200928e:
	movs r1, #8
	movs r2, #7
	movs r3, #6
	movs r5, #77
	movs r6, #8
	movs r0, #70
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003d24
	movs r0, #1
	bl WaitFrames
	movs r0, #50
	movs r1, #8
	movs r2, #7
	movs r3, #6
	str r5, [sp, #0]
	str r6, [sp, #4]
	subs r7, #1
	bl Func_02003d24
	movs r0, #1
	bl WaitFrames
	cmp r7, #0
	bge .L_0200928e
	movs r0, #10
	bl WaitFrames
	movs r7, #127
.L_020092cc:
	mov r2, r8
	ldr r3, [r2, #12]
	movs r2, #128
	lsls r2, r2, #7
	adds r3, r3, r2
	mov r2, r8
	str r3, [r2, #12]
	movs r0, #1
	subs r7, #1
	bl WaitFrames
	cmp r7, #0
	bge .L_020092cc
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02003d44
	bl Func_02003d84
	movs r3, #13
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #9
	movs r2, #7
	movs r3, #6
	movs r0, #38
	bl Func_02003d1c
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #73
	bl GameFlag_SetBit
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009324:
	.4byte 0xffe00000
	.section .text.x020093bc,"ax",%progbits
	.global Func_020013bc
	.thumb_func
Func_020013bc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r0, #0
	movs r2, #102
	adds r2, r2, r6
	adds r5, r6, #0
	mov r10, r2
	ldrh r2, [r2]
	adds r5, #100
	ldrh r3, [r5]
	lsls r2, r2, #16
	ldr r7, [r6, #104]
	asrs r2, r2, #17
	adds r3, r3, r2
	strh r3, [r5]
	ldr r3, [r7, #8]
	movs r2, #128
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	movs r2, #8
	str r3, [r6, #16]
	adds r2, r2, r6
	movs r3, #0
	ldrsh r1, [r5, r3]
	ldr r0, [r6, #76]
	mov r8, r2
	bl Vector_AddPolarOffsetFar
	adds r2, r6, #0
	adds r2, #98
	ldrb r3, [r2]
	movs r0, #0
	adds r3, #255
	strb r3, [r2]
	lsls r3, r3, #24
	cmp r3, #0
	beq .L_0200946c
	ldr r3, [r6, #76]
	movs r2, #128
	lsls r2, r2, #10
	movs r0, #1
	cmp r3, r2
	beq .L_0200946c
	adds r0, r6, #0
	bl Func_02001364
	ldr r2, .L_02009478
	ldr r3, [r6, #76]
	mov r9, r2
	add r3, r9
	str r3, [r6, #76]
	mov r3, r10
	ldrh r2, [r3]
	ldrh r3, [r5]
	lsls r2, r2, #16
	asrs r2, r2, #17
	adds r3, r3, r2
	strh r3, [r5]
	ldr r3, [r7, #8]
	mov r2, r8
	str r3, [r2]
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	ldr r0, [r6, #76]
	str r3, [r6, #16]
	mov r2, r8
	movs r3, #0
	ldrsh r1, [r5, r3]
	bl Vector_AddPolarOffsetFar
	adds r0, r6, #0
	bl Func_02001364
	ldr r3, [r6, #76]
	movs r0, #1
	add r3, r9
	str r3, [r6, #76]
.L_0200946c:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009478:
	.4byte 0xfffc0000
	.section .text.x0200947c,"ax",%progbits
	.global Func_0200147c
	.thumb_func
Func_0200147c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r0, #102
	adds r0, r0, r7
	mov r8, r0
	adds r5, r7, #0
	adds r5, #100
	mov r1, r8
	ldrh r3, [r1]
	ldrh r0, [r5]
	adds r0, r0, r3
	strh r0, [r5]
	lsls r0, r0, #16
	asrs r0, r0, #16
	bl Math_Cosine
	ldr r1, [r7, #76]
	ldr r6, .L_02009518
	mov lr, r6
	.2byte 0xf800
	str r0, [r7, #8]
	movs r2, #0
	ldrsh r0, [r5, r2]
	bl Math_Sine
	ldr r1, [r7, #76]
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r7, #8]
	ldr r2, [r7, #68]
	asrs r0, r0, #1
	adds r3, r3, r2
	str r3, [r7, #8]
	ldr r3, [r7, #72]
	subs r5, #1
	adds r0, r0, r3
	str r0, [r7, #16]
	ldrb r3, [r5]
	cmp r3, #141
	beq .L_020094f4
	ldr r3, .L_0200951c
	movs r2, #1
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_020094f4
	adds r3, r7, #0
	adds r3, #98
	ldrb r0, [r3]
	lsls r0, r0, #10
	bl Math_Sine
	ldrb r3, [r5]
	muls r3, r0
	str r3, [r7, #76]
	ldrb r3, [r5]
	adds r3, #10
	strb r3, [r5]
.L_020094f4:
	mov r3, r8
	ldrh r2, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	movs r0, #0
	cmp r3, #0
	beq .L_02009512
	ldrb r3, [r5]
	cmp r3, #141
	bne .L_02009510
	adds r3, r2, #0
	subs r3, #128
	mov r1, r8
	strh r3, [r1]
.L_02009510:
	movs r0, #1
.L_02009512:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02009518:
	.4byte IwramMulQ16
.L_0200951c:
	.4byte gFrameCount
	.section .text.x02009520,"ax",%progbits
	.global Func_02001520
	.thumb_func
Func_02001520:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r5, r0, #0
	ldr r6, [r5, #104]
	ldr r3, [r5, #8]
	ldr r0, [r6, #8]
	movs r1, #10
	subs r0, r0, r3
	movs r3, #10
	mov r8, r3
	bl __divsi3
	str r0, [r5, #68]
	ldr r3, [r5, #12]
	ldr r0, [r6, #12]
	movs r1, #10
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #13
	adds r0, r0, r3
	bl __divsi3
	str r0, [r5, #76]
	ldr r3, [r5, #16]
	ldr r0, [r6, #16]
	movs r1, #10
	subs r0, r0, r3
	bl __divsi3
	mov r3, r8
	str r0, [r5, #72]
	adds r5, #98
	strb r3, [r5]
	movs r0, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.section .text.x0200956c,"ax",%progbits
	.global Func_0200156c
	.thumb_func
Func_0200156c:
	ldr r3, [r0, #8]
	ldr r2, [r0, #68]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, [r0, #76]
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r2, [r0, #72]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
	adds r0, #98
	ldrb r3, [r0]
	adds r3, #255
	strb r3, [r0]
	lsls r3, r3, #24
	lsrs r3, r3, #24
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
	bx lr
	.section .text.x02009598,"ax",%progbits
	.global Func_02001598
	.thumb_func
Func_02001598:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200961c
	sub sp, #68
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	mov r10, r0
	cmp r7, #0
	bne .L_02009612
	add r2, sp, #28
	str r3, [r2, #4]
	movs r3, #209
	lsls r3, r3, #1
	adds r3, #255
	strh r3, [r2, #24]
	movs r3, #1
	str r3, [r2]
	mov r8, r2
	bl Random16Far
	movs r6, #31
	mov r2, r10
	ldr r3, [r2, #8]
	ands r0, r6
	subs r0, #16
	lsls r0, r0, #16
	add r5, sp, #16
	adds r3, r3, r0
	str r3, [r5]
	bl Random16Far
	mov r3, r10
	ldr r1, [r3, #12]
	ands r0, r6
	lsls r0, r0, #16
	movs r2, #128
	adds r1, r1, r0
	lsls r2, r2, #12
	adds r1, r1, r2
	str r1, [r5, #4]
	ldr r0, [r5]
	ldr r2, [r3, #16]
	movs r3, #128
	lsls r3, r3, #11
	str r2, [r5, #8]
	adds r2, r2, r3
	movs r3, #128
	lsls r3, r3, #8
	str r3, [sp, #0]
	movs r3, #152
	lsls r3, r3, #13
	str r3, [sp, #8]
	mov r3, r8
	str r3, [sp, #12]
	movs r3, #0
	str r7, [sp, #4]
	bl Func_0200015c
.L_02009612:
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200961c:
	.4byte gFrameCount
	.section .text.x02009620,"ax",%progbits
	.global Func_02001620
	.thumb_func
Func_02001620:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	mov r11, r1
	mov r9, r0
	bl Object_GetById
	adds r7, r0, #0
	mov r0, r11
	bl Object_GetById
	mov r10, r0
	movs r0, #78
	bl Func_02003ef4
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl Func_02003e44
	movs r2, #10
	ldrsh r0, [r7, r2]
	movs r3, #14
	ldrsh r1, [r7, r3]
	movs r3, #18
	ldrsh r2, [r7, r3]
	lsls r1, r1, #16
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #16
	bl Motion_CamBounds
	movs r0, #141
	bl Func_02003ef4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl Func_02003d44
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02003e7c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #3
	bl Func_02003e74
	movs r0, #60
	bl Func_02003e84
	adds r2, r7, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	str r3, [r7, #52]
	str r3, [r7, #48]
	ldr r1, .L_020096f4
	mov r0, r9
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #194
	bl Func_02003ef4
	movs r0, #45
	bl Battle_WaitMode0
	movs r1, #128
	mov r0, r9
	lsls r1, r1, #1
	bl Func_02003e04
	ldr r3, .L_020096ec
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_020096f0
	subs r2, #2
	strh r3, [r2]
	movs r0, #0
	mov r8, r0
	b .L_020096f8
	.2byte 0x0000
.L_020096ec:
	.4byte 0x00001008
.L_020096f0:
	.4byte 0x00003f10
.L_020096f4:
	.4byte Data_02004034
.L_020096f8:
	movs r0, #168
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	lsls r0, r0, #2
	bl Engine_ObjectCreate
	adds r5, r0, #0
	movs r0, #246
	bl Func_02003ef4
	bl Random16Far
	movs r3, #31
	ldr r2, [r5, #8]
	ands r3, r0
	subs r3, #16
	lsls r3, r3, #16
	adds r2, r2, r3
	str r2, [r5, #8]
	bl Random16Far
	movs r3, #15
	ldr r2, [r5, #12]
	ands r3, r0
	subs r3, #8
	lsls r3, r3, #16
	adds r2, r2, r3
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r5, #48]
	movs r3, #204
	lsls r3, r3, #6
	str r2, [r5, #12]
	adds r3, #51
	adds r2, r5, #0
	adds r2, #85
	str r3, [r5, #52]
	movs r3, #0
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetSpritePriority
	adds r0, r5, #0
	movs r1, #7
	bl Func_02003cc4
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	ldr r1, .L_020097b8
	adds r0, r5, #0
	bl Func_02003ccc
	movs r6, #128
	ldr r3, .L_020097ac
	ldr r5, .L_020097b0
	lsls r6, r6, #19
	adds r6, #82
	strh r3, [r6]
	movs r0, #2
	bl WaitFrames
	movs r0, #2
	strh r5, [r6]
	bl WaitFrames
	ldr r3, .L_020097b4
	movs r0, #2
	strh r3, [r6]
	bl WaitFrames
	strh r5, [r6]
	movs r0, #2
	bl WaitFrames
	movs r2, #1
	add r8, r2
	mov r3, r8
	b .L_020097bc
.L_020097ac:
	.4byte 0x00001004
.L_020097b0:
	.4byte 0x0000100a
.L_020097b4:
	.4byte 0x00001010
.L_020097b8:
	.4byte Data_02003fe8
.L_020097bc:
	cmp r3, #16
	bne .L_020096f8
	ldr r3, .L_020097fc
	movs r0, #30
	strh r3, [r6]
	bl WaitFrames
	movs r0, #0
	mov r8, r0
.L_020097ce:
	mov r0, r10
	ldr r3, [r0, #16]
	mov r2, r10
	movs r0, #168
	ldr r1, [r2, #8]
	lsls r0, r0, #2
	ldr r2, [r2, #12]
	bl Engine_ObjectCreate
	adds r5, r0, #0
	movs r0, #195
	bl Func_02003ef4
	bl Random16Far
	ldr r3, .L_02009800
	movs r2, #128
	ands r0, r3
	lsls r2, r2, #8
	adds r6, r5, #0
	adds r0, r0, r2
	adds r6, #100
	b .L_02009804
.L_020097fc:
	.4byte 0x00001008
.L_02009800:
	.4byte 0x00007fff
.L_02009804:
	strh r0, [r6]
	bl Random16Far
	mov r3, r8
	movs r2, #1
	ands r2, r3
	movs r3, #3
	ands r3, r0
	lsls r2, r2, #1
	adds r3, #9
	subs r2, #1
	lsls r2, r3
	ldr r7, .L_02009858
	adds r3, r5, #0
	adds r3, #102
	strh r2, [r3]
	subs r3, #17
	strb r7, [r3]
	movs r3, #244
	lsls r3, r3, #15
	str r3, [r5, #76]
	mov r2, r8
	movs r3, #16
	subs r3, r3, r2
	lsls r3, r3, #3
	adds r2, r5, #0
	adds r3, #15
	adds r2, #98
	mov r0, r10
	str r0, [r5, #104]
	movs r1, #2
	strb r3, [r2]
	adds r0, r5, #0
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetSpritePriority
	adds r0, r5, #0
	movs r1, #7
	b .L_0200985c
.L_02009858:
	.4byte 0x00000000
.L_0200985c:
	bl Func_02003cc4
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	adds r2, r5, #0
	movs r3, #0
	ldrsh r1, [r6, r3]
	ldr r0, [r5, #76]
	adds r2, #8
	bl Vector_AddPolarOffsetFar
	adds r0, r5, #0
	ldr r1, .L_02009954
	bl Func_02003ccc
	mov r0, r8
	cmp r0, #3
	bne .L_020098b0
	movs r1, #128
	mov r0, r11
	lsls r1, r1, #1
	bl Func_02003e04
	mov r3, r10
	adds r3, #85
	strb r7, [r3]
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	mov r2, r10
	str r3, [r2, #52]
	str r3, [r2, #48]
	mov r0, r11
	ldr r1, .L_02009958
	bl ObjectMotion_EnableActionAndSetCallback
.L_020098b0:
	movs r0, #8
	bl WaitFrames
	movs r3, #1
	add r8, r3
	mov r0, r8
	cmp r0, #16
	beq .L_020098c2
	b .L_020097ce
.L_020098c2:
	movs r0, #220
	bl Func_02003ef4
	movs r0, #16
	bl WaitFrames
	movs r2, #2
	mov r8, r2
.L_020098d2:
	mov r3, r10
	ldr r1, [r3, #8]
	ldr r3, [r3, #12]
	mov r0, r8
	lsls r2, r0, #16
	adds r2, r2, r3
	ldr r3, .L_0200995c
	mov r0, r10
	adds r2, r2, r3
	ldr r3, [r0, #16]
	movs r0, #168
	lsls r0, r0, #2
	bl Engine_ObjectCreate
	adds r5, r0, #0
	bl Random16Far
	adds r3, r5, #0
	adds r3, #100
	strh r0, [r3]
	adds r2, r5, #0
	movs r3, #128
	adds r2, #102
	lsls r3, r3, #4
	strh r3, [r2]
	adds r3, r5, #0
	mov r2, r8
	adds r3, #98
	strb r2, [r3]
	movs r2, #1
	adds r3, #1
	strb r2, [r3]
	ldr r1, .L_02009950
	ldr r3, [r5, #8]
	movs r6, #0
	str r3, [r5, #68]
	ldr r3, [r5, #16]
	str r6, [r5, #76]
	str r3, [r5, #72]
	mov r3, r10
	str r3, [r5, #104]
	adds r3, r5, #0
	adds r3, #85
	strb r1, [r3]
	subs r3, #50
	strb r2, [r3]
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #7
	bl Func_02003cc4
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	b .L_02009960
.L_02009950:
	.4byte 0x00000000
.L_02009954:
	.4byte Data_02004004
.L_02009958:
	.4byte Data_02004058
.L_0200995c:
	.4byte 0xfff80000
.L_02009960:
	adds r0, r5, #0
	ldr r1, .L_02009a0c
	bl Func_02003ccc
	movs r0, #2
	add r8, r0
	mov r2, r8
	cmp r2, #32
	bne .L_020098d2
	movs r0, #220
	bl Func_02003ef4
	movs r0, #50
	bl WaitFrames
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02003e74
	movs r0, #8
	bl Func_02003e84
	movs r0, #16
	bl WaitFrames
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl Func_02003d44
	mov r0, r9
	movs r1, #0
	bl Func_02003e04
	mov r0, r11
	movs r1, #0
	bl Func_02003e04
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #62
	bl Func_02003ef4
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02003e74
	movs r0, #80
	bl Func_02003e84
	mov r0, r9
	ldr r1, .L_02009a10
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009a14
	mov r0, r11
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r3, .L_02009a18
	mov r0, r10
	str r3, [r0, #108]
	movs r0, #120
	bl WaitFrames
	mov r2, r10
	movs r0, #195
	str r6, [r2, #108]
	lsls r0, r0, #1
	bl Func_02003ef4
	bl Func_02003ebc
	movs r0, #1
	bl WaitFrames
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009a0c:
	.4byte Data_02004014
.L_02009a10:
	.4byte Data_0200407c
.L_02009a14:
	.4byte Data_020040ac
.L_02009a18:
	.4byte Func_02001598
	.section .text.x02009a1c,"ax",%progbits
	.global Func_02001a1c
	.thumb_func
Func_02001a1c:
	push {r5, r6, lr}
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	ldr r6, .L_02009c58
	movs r1, #1
	adds r0, r6, #0
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #174
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009a46
	bl Func_02003d84
	b .L_02009c56
.L_02009a46:
	adds r0, r6, #0
	bl Func_02003e14
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	ldr r5, .L_02009c5c
	adds r3, #1
	strh r3, [r2]
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r1, [r5]
	movs r0, #7
	bl Func_02003de4
	movs r0, #1
	bl Battle_WaitMode0
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #184
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #137
	lsls r1, r1, #1
	movs r2, #184
	movs r0, #7
	bl ObjectMotion_ResetAndSetPositionInMode2
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	bl Func_02003e1c
	movs r1, #3
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #248
	movs r2, #184
	bl ObjectMotion_SetPositionAndReset
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	ldr r0, [r5]
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #7
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #7
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #132
	movs r0, #7
	lsls r1, r1, #1
	movs r2, #168
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r6, #2
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #14
	movs r1, #7
	bl Func_02001620
	movs r1, #138
	movs r0, #7
	bl UiText_DrawQuantityPairWithCue
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #174
	bl GameFlag_SetBit
	movs r0, #107
	bl Func_02003ef4
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r5]
	bl Func_02003e34
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_02003e34
	bl Func_0200124c
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02003ef4
	movs r0, #30
	bl Battle_WaitMode0
	ldr r0, .L_02009c60
	bl Func_02003e14
	movs r1, #6
	adds r1, #255
	movs r2, #20
	movs r0, #7
	bl Func_02003e34
	movs r2, #0
	movs r1, #7
	ldr r0, [r5]
	bl Object_LinkPair
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	bl Func_02003e1c
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #7
	bl Func_02003e34
	movs r1, #0
	movs r0, #7
	bl Func_02003e1c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_02009c64
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009c42
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_02009c42:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02003ddc
	bl Func_02003d84
.L_02009c56:
	pop {r5, r6, pc}
.L_02009c58:
	.4byte MsgCheckedTheTabletIt
.L_02009c5c:
	.4byte gPartyState
.L_02009c60:
	.4byte MsgWhatWentWrong
.L_02009c64:
	.4byte 0x00013333
	.section .text.x02009ea0,"ax",%progbits
	.balign 4
	.global Func_02001ea0
	.thumb_func
Func_02001ea0:
	push {r5, r6, r7, lr}
	ldr r3, .L_02009efc
	ldr r7, [r3]
	movs r3, #15
	ands r7, r3
	cmp r7, #0
	bne .L_02009efa
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	movs r0, #14
	adds r0, #255
	bl Engine_ObjectCreate
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009efa
	ldr r1, .L_02009f00
	ldr r6, [r5, #80]
	bl Func_02003ccc
	adds r3, r5, #0
	adds r3, #85
	adds r2, r5, #0
	strb r7, [r3]
	adds r2, #34
	movs r3, #1
	strb r3, [r2]
	cmp r6, #0
	beq .L_02009efa
	adds r0, r6, #0
	movs r1, #2
	bl Animation_ApplyChildArgument
	movs r3, #128
	ldrb r2, [r6, #9]
	lsls r3, r3, #7
	strh r3, [r6, #18]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r7, [r6, #26]
	strb r3, [r6, #9]
.L_02009efa:
	pop {r5, r6, r7, pc}
.L_02009efc:
	.4byte gFrameCount
.L_02009f00:
	.4byte Data_020040dc
	.section .text.x02009f04,"ax",%progbits
	.global Func_02001f04
	.thumb_func
Func_02001f04:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009fc4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #85
	adds r6, r0, #0
	adds r3, r3, r6
	ldrb r2, [r3]
	mov r8, r3
	mov r10, r2
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	bl Event_StartScreenTransition
	adds r0, r6, #0
	movs r1, #9
	bl Animation_ApplyChildValues
	movs r3, #0
	mov r2, r8
	strb r3, [r2]
	movs r3, #160
	lsls r3, r3, #15
	str r3, [r6, #12]
	movs r7, #192
	lsls r7, r7, #10
	movs r5, #31
.L_02009f4e:
	ldr r3, [r6, #12]
	movs r2, #128
	subs r3, r3, r7
	str r3, [r6, #12]
	ldrh r3, [r6, #6]
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r6, #6]
	ldr r3, .L_02009fc8
	adds r0, r6, #0
	movs r1, #0
	adds r7, r7, r3
	subs r5, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_02009f4e
	adds r0, r6, #0
	bl Func_02002bd4
	adds r0, r6, #0
	movs r1, #0
	bl Animation_ApplyChildValues
	ldr r5, .L_02009fc4
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	mov r2, r8
	mov r3, r10
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r6, #40]
	movs r0, #215
	bl Func_02003ef4
	ldr r0, [r5]
	movs r1, #168
	movs r2, #200
	bl ObjectMotion_SetPositionAndCommit
	bl Func_02003d84
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009fc4:
	.4byte gPartyState
.L_02009fc8:
	.4byte 0xfffff334
	.section .text.x02009fcc,"ax",%progbits
	.global Func_02001fcc
	.thumb_func
Func_02001fcc:
	push {lr}
	bl Func_02003bcc
	cmp r0, #0
	beq .L_02009fde
	ldr r3, .L_0200a018
	cmp r0, r3
	beq .L_02009ff6
	b .L_0200a00e
.L_02009fde:
	movs r1, #1
	movs r0, #0
	bl Func_02003bb8
	movs r0, #1
	movs r1, #1
	bl Func_02003bb8
	movs r0, #170
	bl Func_02003ecc
	b .L_0200a00e
.L_02009ff6:
	movs r1, #0
	movs r0, #0
	bl Func_02003bb8
	movs r0, #1
	movs r1, #0
	bl Func_02003bb8
	movs r0, #1
	negs r0, r0
	bl Func_02003ecc
.L_0200a00e:
	ldr r0, .L_0200a01c
	bl Func_02003bd8
	pop {pc}
	.2byte 0x0000
.L_0200a018:
	.4byte 0x00034bbf
.L_0200a01c:
	.4byte 0x00034bc0
	.section .text.x0200a020,"ax",%progbits
	.global Func_02002020
	.thumb_func
Func_02002020:
	push {r5, r6, lr}
	ldr r2, .L_0200a304
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r0, #0
	ldrsh r1, [r3, r0]
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r2, #0
	ldrsh r6, [r3, r2]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	subs r0, #54
	movs r2, #129
	adds r3, r3, r0
	lsls r2, r2, #2
	str r2, [r3]
	ldr r3, .L_0200a308
	sub sp, #8
	cmp r1, r3
	bne .L_0200a0a2
	adds r0, #132
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a06a
	movs r0, #8
	bl Func_02001e78
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	b .L_0200a076
.L_0200a06a:
	movs r0, #8
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
.L_0200a076:
	movs r0, #153
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a094
	movs r0, #9
	bl Func_02001e78
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	b .L_0200a2fe
.L_0200a094:
	movs r0, #9
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	b .L_0200a2fe
.L_0200a0a2:
	ldr r3, .L_0200a30c
	cmp r1, r3
	bne .L_0200a140
	movs r0, #144
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a0ea
	movs r3, #110
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #118
	movs r1, #15
	movs r2, #5
	movs r3, #32
	bl Func_02003d24
	movs r3, #46
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #16
	movs r0, #59
	movs r2, #5
	movs r3, #32
	bl Func_02003d1c
	movs r0, #8
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	b .L_0200a0f8
.L_0200a0ea:
	movs r0, #8
	bl Func_02001e78
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
.L_0200a0f8:
	movs r0, #9
	bl Object_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	movs r0, #144
	adds r3, #85
	movs r6, #0
	lsls r0, r0, #4
	strb r6, [r3]
	adds r0, #92
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a118
	b .L_0200a2fe
.L_0200a118:
	movs r1, #158
	movs r2, #152
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02003ddc
	movs r3, #39
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #41
	movs r1, #9
	movs r2, #1
	movs r3, #1
	bl Func_02003d1c
	str r6, [r5, #12]
	str r6, [r5, #20]
	b .L_0200a2fe
.L_0200a140:
	ldr r3, .L_0200a310
	cmp r1, r3
	bne .L_0200a152
	cmp r6, #7
	beq .L_0200a14c
	b .L_0200a2fe
.L_0200a14c:
	bl Func_02002ffc
	b .L_0200a2fe
.L_0200a152:
	ldr r3, .L_0200a314
	cmp r1, r3
	bne .L_0200a1e8
	movs r0, #10
	bl Func_02001e78
	movs r1, #3
	movs r0, #10
	bl Object_SetModeById
	movs r0, #11
	bl Func_02001e78
	movs r0, #11
	movs r1, #3
	bl Object_SetModeById
	subs r3, r6, #6
	cmp r3, #1
	bhi .L_0200a1b4
	movs r0, #165
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a1a8
	movs r0, #9
	bl Object_GetById
	movs r1, #5
	bl Object_SetPartAttribute
	movs r0, #9
	bl Func_02001e78
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	bl Func_02000b5c
	b .L_0200a1b4
.L_0200a1a8:
	movs r0, #9
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
.L_0200a1b4:
	movs r0, #13
	bl Object_GetById
	movs r5, #1
	adds r0, #98
	strb r5, [r0]
	movs r0, #14
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #64
	bl Object_GetById
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200a1d8
	b .L_0200a2fe
.L_0200a1d8:
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	ldr r3, .L_0200a318
	str r3, [r5, #20]
	str r3, [r5, #12]
	b .L_0200a2fe
.L_0200a1e8:
	ldr r3, .L_0200a31c
	cmp r1, r3
	bne .L_0200a208
	subs r3, r6, #5
	cmp r3, #1
	bls .L_0200a1f6
	b .L_0200a2fe
.L_0200a1f6:
	ldr r0, .L_0200a320
	ldr r1, .L_0200a324
	ldr r2, .L_0200a328
	bl Func_02003ab0
	movs r0, #170
	bl Func_02003ecc
	b .L_0200a2fe
.L_0200a208:
	ldr r3, .L_0200a32c
	cmp r1, r3
	bne .L_0200a284
	cmp r6, #5
	beq .L_0200a216
	cmp r6, #7
	bne .L_0200a22e
.L_0200a216:
	ldr r1, .L_0200a330
	ldr r2, .L_0200a328
	ldr r0, .L_0200a334
	bl Func_02003ab0
	movs r0, #170
	bl Func_02003ecc
	movs r0, #0
	movs r1, #10
	bl Func_0200107c
.L_0200a22e:
	movs r0, #142
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a248
	movs r0, #9
	bl Func_02001e78
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
.L_0200a248:
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	adds r5, r0, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r2, r5, #0
	adds r2, #89
	movs r3, #0
	strb r3, [r2]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	ldr r3, .L_0200a338
	movs r0, #10
	str r3, [r5, #108]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a2fe
	cmp r6, #13
	bne .L_0200a2fe
	bl Func_02001f04
	b .L_0200a2fe
.L_0200a284:
	ldr r3, .L_0200a33c
	cmp r1, r3
	bne .L_0200a2fe
	movs r0, #166
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a2b4
	movs r0, #9
	bl Object_GetById
	movs r1, #5
	bl Object_SetPartAttribute
	movs r0, #9
	bl Func_02001e78
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	b .L_0200a2c0
.L_0200a2b4:
	movs r0, #9
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
.L_0200a2c0:
	movs r0, #10
	bl Object_GetById
	movs r5, #1
	adds r0, #98
	strb r5, [r0]
	movs r0, #11
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #14
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #16
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #17
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #18
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
.L_0200a2fe:
	movs r0, #0
	add sp, #8
	pop {r5, r6, pc}
.L_0200a304:
	.4byte gPartyState
.L_0200a308:
	.4byte 0x000000a0
.L_0200a30c:
	.4byte 0x000000a1
.L_0200a310:
	.4byte 0x000000a2
.L_0200a314:
	.4byte 0x000000a3
.L_0200a318:
	.4byte 0xffe00000
.L_0200a31c:
	.4byte 0x000000a4
.L_0200a320:
	.4byte Data_020040f4
.L_0200a324:
	.4byte Data_02004102
.L_0200a328:
	.4byte Func_02001fcc
.L_0200a32c:
	.4byte 0x000000a5
.L_0200a330:
	.4byte Data_02004114
.L_0200a334:
	.4byte Data_02004106
.L_0200a338:
	.4byte Func_02001ea0
.L_0200a33c:
	.4byte 0x000000a8
	.section .text.x0200a340,"ax",%progbits
	.global Func_02002340
	.thumb_func
Func_02002340:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r6, .L_0200a644
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r2, #0
	ldrsh r5, [r3, r2]
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r2, #0
	ldrsh r7, [r3, r2]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	mov r8, r3
	ldrb r2, [r1, #23]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #23]
	sub sp, #12
	bl Func_02003278
	ldr r3, .L_0200a648
	cmp r5, r3
	beq .L_0200a380
	b .L_0200a52a
.L_0200a380:
	subs r3, r7, #3
	cmp r3, #1
	bls .L_0200a38e
	cmp r7, #6
	beq .L_0200a38e
	cmp r7, #8
	bne .L_0200a39c
.L_0200a38e:
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
	b .L_0200a4c8
.L_0200a39c:
	cmp r7, #5
	beq .L_0200a3a4
	cmp r7, #7
	bne .L_0200a462
.L_0200a3a4:
	movs r0, #12
	bl Object_GetById
	adds r6, r0, #0
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	movs r1, #0
	str r5, [r6, #20]
	str r5, [r6, #12]
	bl Func_02003cc4
	movs r0, #13
	bl Object_GetById
	adds r6, r0, #0
	adds r3, r6, #0
	adds r3, #85
	movs r0, #140
	strb r5, [r3]
	lsls r0, r0, #2
	str r5, [r6, #20]
	str r5, [r6, #12]
	bl GameFlag_Test
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200a422
	movs r3, #104
	movs r2, #40
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #104
	movs r1, #80
	movs r2, #16
	movs r3, #10
	bl Func_02003d24
	movs r3, #39
	movs r2, #105
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #110
	movs r1, #116
	movs r2, #18
	movs r3, #12
	bl Func_02003d24
	movs r3, #32
	movs r0, #64
	movs r1, #32
	movs r2, #64
	str r5, [sp, #0]
	str r3, [sp, #4]
	bl Func_02003d1c
	movs r0, #33
	movs r1, #0
	movs r2, #0
	bl Func_020032d4
	b .L_0200a440
.L_0200a422:
	movs r1, #4
	movs r0, #33
	movs r2, #0
	bl Func_020032d4
	movs r0, #12
	bl Object_GetById
	adds r6, r0, #0
	ldr r0, [r6, #8]
	movs r1, #0
	asrs r0, r0, #20
	subs r0, #42
	bl Func_0200349c
.L_0200a440:
	movs r0, #148
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a4c8
	movs r3, #43
	movs r2, #44
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #41
	movs r1, #42
	movs r2, #1
	movs r3, #1
	bl Func_02003d1c
	b .L_0200a4c8
.L_0200a462:
	adds r3, r7, #0
	subs r3, #9
	cmp r3, #1
	bhi .L_0200a4c8
	movs r0, #153
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200a4be
	movs r3, #77
	str r3, [sp, #0]
	movs r5, #32
	movs r0, #77
	movs r1, #80
	movs r2, #23
	movs r3, #20
	str r5, [sp, #4]
	bl Func_02003d24
	movs r3, #12
	movs r2, #98
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #112
	movs r1, #96
	movs r2, #16
	movs r3, #20
	bl Func_02003d24
	movs r0, #64
	movs r1, #32
	movs r2, #64
	movs r3, #32
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003d1c
	movs r0, #33
	movs r1, #0
	movs r2, #0
	bl Func_020032d4
	b .L_0200a4c8
.L_0200a4be:
	movs r0, #33
	movs r1, #4
	movs r2, #0
	bl Func_020032d4
.L_0200a4c8:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #90
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a50a
	movs r1, #184
	movs r2, #200
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02003ddc
	movs r3, #9
	movs r5, #12
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #12
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02003d1c
	movs r3, #13
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #12
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02003d1c
.L_0200a50a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #91
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a51a
	b .L_0200abc4
.L_0200a51a:
	movs r1, #206
	movs r2, #216
	movs r0, #11
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02003ddc
	b .L_0200abc4
.L_0200a52a:
	ldr r3, .L_0200a64c
	cmp r5, r3
	bne .L_0200a558
	subs r3, r7, #1
	cmp r3, #1
	bhi .L_0200a542
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
.L_0200a542:
	movs r0, #8
	bl Object_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	str r3, [r6, #20]
	str r3, [r6, #12]
	b .L_0200abc4
.L_0200a558:
	ldr r3, .L_0200a650
	cmp r5, r3
	bne .L_0200a654
	movs r0, #0
	bl Func_02003ea4
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #8
	bl Func_020032d4
	movs r0, #40
	movs r1, #8
	movs r2, #8
	bl Func_020032d4
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #102
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a5cc
	bl Func_02003628
	movs r1, #4
	movs r2, #16
	movs r3, #2
	movs r0, #16
	bl Func_02003688
	movs r0, #22
	movs r1, #4
	movs r2, #16
	movs r3, #0
	bl Func_02003688
	movs r5, #5
.L_0200a5be:
	movs r0, #33
	movs r1, #55
	subs r5, #1
	bl Func_020036b8
	cmp r5, #0
	bge .L_0200a5be
.L_0200a5cc:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #93
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a62c
	movs r3, #80
	movs r2, #27
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #75
	movs r1, #96
	movs r2, #7
	movs r3, #13
	bl Func_02003d24
	movs r5, #16
	movs r6, #31
	movs r0, #91
	movs r1, #96
	movs r2, #7
	movs r3, #10
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003d24
	movs r3, #91
	str r3, [sp, #4]
	movs r0, #99
	movs r1, #96
	movs r2, #7
	movs r3, #14
	str r5, [sp, #0]
	bl Func_02003d24
	movs r3, #17
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #0
	movs r2, #5
	movs r3, #9
	str r6, [sp, #4]
	bl Func_02003d1c
	bl Func_020007a8
	b .L_0200abc4
.L_0200a62c:
	movs r3, #17
	movs r2, #31
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #17
	movs r1, #32
	movs r2, #5
	movs r3, #1
	bl Func_02003d1c
	b .L_0200abc4
	.2byte 0x0000
.L_0200a644:
	.4byte gPartyState
.L_0200a648:
	.4byte 0x000000a0
.L_0200a64c:
	.4byte 0x000000a1
.L_0200a650:
	.4byte 0x000000a2
.L_0200a654:
	ldr r3, .L_0200a720
	cmp r5, r3
	beq .L_0200a65c
	b .L_0200a7cc
.L_0200a65c:
	movs r0, #0
	bl Func_02003ea4
	subs r3, r7, #6
	cmp r3, #1
	bls .L_0200a66a
	b .L_0200a7aa
.L_0200a66a:
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #5
	movs r2, #4
	movs r0, #12
	bl Func_02003420
	movs r1, #3
	movs r2, #4
	movs r0, #13
	bl Func_02003420
	movs r1, #4
	movs r2, #4
	movs r0, #14
	bl Func_02003420
	movs r0, #40
	movs r1, #4
	movs r2, #4
	bl Func_020032d4
	movs r0, #165
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a724
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #100
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a724
	mov r3, r8
	ldr r6, [r3, #32]
	movs r3, #71
	str r3, [sp, #0]
	movs r5, #39
	movs r0, #64
	movs r1, #64
	movs r2, #16
	movs r3, #13
	str r5, [sp, #4]
	bl Func_02003d24
	movs r3, #7
	str r3, [sp, #0]
	movs r0, #71
	movs r1, #39
	movs r2, #16
	movs r3, #15
	str r5, [sp, #4]
	bl Func_02003d1c
	bl Func_02000b84
	movs r3, #131
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #1
	strb r3, [r2]
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #10
	ldrh r2, [r0]
	movs r3, #255
	lsls r3, r3, #8
	mov r1, sp
	adds r3, #252
	adds r1, #10
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_0200a71c
	orrs r3, r2
	strh r3, [r0]
	b .L_0200a78c
	.2byte 0x0000
.L_0200a71c:
	.4byte 0x00000001
.L_0200a720:
	.4byte 0x000000a3
.L_0200a724:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #100
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a73e
	movs r0, #146
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a78c
.L_0200a73e:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #100
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a75a
	movs r1, #140
	movs r2, #178
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02003ddc
.L_0200a75a:
	movs r1, #38
	movs r0, #12
	bl Func_02000b24
	bl Func_02003628
	movs r0, #12
	movs r1, #2
	movs r2, #45
	movs r3, #1
	bl Func_02003688
	movs r5, #7
.L_0200a774:
	movs r0, #33
	movs r1, #55
	subs r5, #1
	bl Func_020036b8
	cmp r5, #0
	bge .L_0200a774
	movs r0, #165
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_0200a78c:
	movs r5, #12
.L_0200a78e:
	adds r0, r5, #0
	bl Object_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	adds r5, #1
	strb r3, [r2]
	str r3, [r6, #20]
	str r3, [r6, #12]
	cmp r5, #14
	ble .L_0200a78e
	b .L_0200a7b6
.L_0200a7aa:
	movs r2, #4
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
.L_0200a7b6:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #106
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a7c6
	b .L_0200abc4
.L_0200a7c6:
	bl Func_02000ad8
	b .L_0200abc4
.L_0200a7cc:
	ldr r3, .L_0200aa50
	cmp r5, r3
	bne .L_0200a82c
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
	movs r0, #40
	movs r1, #0
	movs r2, #0
	bl Func_020032d4
	movs r0, #0
	bl Func_02003ea4
	subs r3, r7, #1
	cmp r3, #3
	bls .L_0200a7fa
	cmp r7, #7
	beq .L_0200a7fa
	b .L_0200abc4
.L_0200a7fa:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #103
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a80a
	b .L_0200abc4
.L_0200a80a:
	bl Func_02003628
	movs r0, #44
	movs r1, #0
	movs r2, #34
	movs r3, #2
	bl Func_02003688
	movs r5, #0
.L_0200a81c:
	movs r0, #33
	movs r1, #55
	adds r5, #1
	bl Func_020036b8
	cmp r5, #4
	ble .L_0200a81c
	b .L_0200abc4
.L_0200a82c:
	ldr r3, .L_0200aa54
	cmp r5, r3
	bne .L_0200a900
	movs r0, #0
	bl Func_02003ea4
	cmp r7, #1
	beq .L_0200a840
	cmp r7, #13
	bne .L_0200a84e
.L_0200a840:
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
	b .L_0200a866
.L_0200a84e:
	cmp r7, #9
	beq .L_0200a85a
	cmp r7, #11
	beq .L_0200a85a
	cmp r7, #12
	bne .L_0200a866
.L_0200a85a:
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
.L_0200a866:
	movs r0, #142
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a890
	movs r5, #13
.L_0200a874:
	adds r0, r5, #0
	bl Object_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	adds r5, #1
	strb r3, [r2]
	str r3, [r6, #8]
	str r3, [r6, #16]
	cmp r5, #15
	ble .L_0200a874
	b .L_0200abc4
.L_0200a890:
	movs r3, #113
	movs r2, #39
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #114
	movs r1, #39
	movs r2, #1
	movs r3, #2
	bl Func_02003d24
	movs r5, #41
	movs r0, #105
	movs r1, #41
	movs r2, #12
	movs r3, #13
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003d1c
	movs r3, #105
	str r3, [sp, #0]
	movs r0, #105
	movs r1, #105
	movs r2, #12
	movs r3, #13
	str r5, [sp, #4]
	bl Func_02003d24
	movs r5, #13
.L_0200a8ca:
	adds r0, r5, #0
	bl Object_GetById
	movs r2, #145
	lsls r2, r2, #2
	adds r6, r0, #0
	adds r0, r5, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a8f8
	ldr r2, [r6, #8]
	ldr r3, [r6, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #47
	movs r1, #45
	movs r2, #1
	movs r3, #1
	bl Func_02003d1c
.L_0200a8f8:
	adds r5, #1
	cmp r5, #15
	ble .L_0200a8ca
	b .L_0200abc4
.L_0200a900:
	ldr r3, .L_0200aa58
	cmp r5, r3
	bne .L_0200a914
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
	b .L_0200abc4
.L_0200a914:
	ldr r3, .L_0200aa5c
	cmp r5, r3
	bne .L_0200a93c
	movs r0, #0
	bl Func_02003ea4
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
	bl Func_02003edc
	movs r0, #0
	movs r1, #8
	movs r2, #9
	bl Func_02003ee4
	b .L_0200abc4
.L_0200a93c:
	ldr r3, .L_0200aa60
	cmp r5, r3
	beq .L_0200a944
	b .L_0200ab46
.L_0200a944:
	subs r3, r7, #3
	cmp r3, #1
	bls .L_0200a94c
	b .L_0200aafe
.L_0200a94c:
	ldr r3, .L_0200aa64
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #6
	movs r2, #4
	movs r0, #10
	bl Func_02003420
	movs r1, #3
	movs r2, #4
	movs r0, #11
	bl Func_02003420
	movs r1, #5
	movs r2, #4
	movs r0, #12
	bl Func_02003420
	movs r1, #6
	movs r2, #4
	movs r0, #13
	bl Func_02003420
	movs r1, #5
	movs r2, #4
	movs r0, #14
	bl Func_02003420
	movs r1, #4
	movs r2, #4
	movs r0, #15
	bl Func_02003420
	movs r1, #4
	movs r2, #4
	movs r0, #16
	bl Func_02003420
	movs r1, #3
	movs r2, #4
	movs r0, #17
	bl Func_02003420
	movs r1, #6
	movs r2, #4
	movs r0, #18
	bl Func_02003420
	movs r0, #40
	movs r1, #4
	movs r2, #4
	bl Func_020032d4
	movs r0, #166
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200aa68
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #101
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200aa68
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #32]
	movs r3, #100
	str r3, [sp, #0]
	movs r5, #13
	movs r0, #64
	movs r1, #64
	movs r2, #19
	movs r3, #17
	str r5, [sp, #4]
	bl Func_02003d24
	movs r3, #35
	movs r2, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #35
	movs r1, #105
	movs r2, #23
	movs r3, #24
	bl Func_02003d24
	movs r3, #36
	str r3, [sp, #0]
	movs r0, #100
	movs r1, #13
	movs r2, #19
	movs r3, #19
	str r5, [sp, #4]
	bl Func_02003d1c
	movs r3, #131
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #1
	strb r3, [r2]
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #10
	ldrh r2, [r0]
	movs r3, #255
	lsls r3, r3, #8
	mov r1, sp
	adds r3, #252
	adds r1, #10
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_0200aa4c
	orrs r3, r2
	strh r3, [r0]
	b .L_0200aafe
.L_0200aa4c:
	.4byte 0x00000001
.L_0200aa50:
	.4byte 0x000000a4
.L_0200aa54:
	.4byte 0x000000a5
.L_0200aa58:
	.4byte 0x000000a6
.L_0200aa5c:
	.4byte 0x000000a7
.L_0200aa60:
	.4byte 0x000000a8
.L_0200aa64:
	.4byte gPartyState
.L_0200aa68:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #101
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200aa84
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #74
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200aafe
.L_0200aa84:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #101
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200aabc
	movs r1, #202
	movs r2, #148
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02003ddc
	movs r1, #170
	movs r2, #180
	movs r0, #13
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02003ddc
	movs r1, #186
	movs r2, #212
	movs r0, #15
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02003ddc
.L_0200aabc:
	movs r0, #12
	movs r1, #38
	bl Func_02000b24
	movs r0, #13
	movs r1, #39
	bl Func_02000b24
	movs r1, #37
	movs r0, #15
	bl Func_02000b24
	bl Func_02003628
	movs r0, #38
	movs r1, #2
	movs r2, #19
	movs r3, #1
	bl Func_02003688
	movs r5, #7
.L_0200aae6:
	movs r0, #33
	movs r1, #55
	subs r5, #1
	bl Func_020036b8
	cmp r5, #0
	bge .L_0200aae6
	movs r0, #166
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_0200aafe:
	movs r5, #10
.L_0200ab00:
	adds r0, r5, #0
	bl Object_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	adds r5, #1
	strb r3, [r2]
	str r3, [r6, #20]
	str r3, [r6, #12]
	cmp r5, #20
	ble .L_0200ab00
	movs r0, #19
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	movs r2, #39
	asrs r3, r3, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #23
	movs r1, #39
	movs r2, #1
	bl Func_02003d1c
	subs r3, r7, #1
	cmp r3, #1
	bhi .L_0200abc4
	movs r0, #0
	bl Func_02003ea4
	b .L_0200abc4
.L_0200ab46:
	ldr r3, .L_0200abd0
	cmp r5, r3
	bne .L_0200abc4
	movs r0, #0
	bl Func_02003ea4
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #73
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ab8a
	movs r3, #13
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #9
	movs r2, #7
	movs r3, #6
	bl Func_02003d1c
	b .L_0200abc4
.L_0200ab8a:
	movs r3, #50
	str r3, [sp, #0]
	movs r5, #8
	movs r0, #77
	movs r1, #8
	movs r2, #7
	movs r3, #6
	str r5, [sp, #4]
	bl Func_02003d24
	movs r3, #77
	str r3, [sp, #0]
	movs r0, #70
	movs r1, #8
	movs r2, #7
	movs r3, #6
	str r5, [sp, #4]
	bl Func_02003d24
	movs r3, #13
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #17
	movs r2, #7
	movs r3, #6
	bl Func_02003d1c
.L_0200abc4:
	movs r0, #0
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200abd0:
	.4byte 0x000000a9
	.section .text.x0200abd4,"ax",%progbits
	.global Func_02002bd4
	.thumb_func
Func_02002bd4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	movs r0, #128
	lsls r0, r0, #11
	adds r2, r2, r0
	adds r3, r3, r0
	ldr r1, [r6, #8]
	movs r0, #14
	bl Engine_ObjectCreate
	ldr r2, [r6, #80]
	adds r5, r0, #0
	mov r8, r2
	cmp r5, #0
	beq .L_0200ac30
	ldr r3, [r6, #20]
	ldr r7, [r5, #80]
	str r3, [r5, #20]
	ldr r1, .L_0200ac38
	bl Func_02003ccc
	adds r3, r5, #0
	adds r3, #85
	movs r5, #0
	strb r5, [r3]
	cmp r7, #0
	beq .L_0200ac30
	movs r1, #1
	adds r0, r7, #0
	bl Animation_ApplyChildArgument
	strb r5, [r7, #26]
	mov r2, r8
	ldrb r3, [r2, #9]
	ldrb r1, [r7, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r7, #9]
.L_0200ac30:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ac38:
	.4byte Data_0200411c
	.section .text.x0200aca4,"ax",%progbits
	.global Func_02002ca4
	.thumb_func
Func_02002ca4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r6, .L_0200adf0
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #133
	ldr r2, [r3, #108]
	lsls r0, r0, #2
	adds r3, r6, r0
	movs r1, #230
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r2, r2, r1
	ldr r2, [r2]
	mov r8, r3
	mov r0, r8
	mov r10, r2
	sub sp, #12
	bl Object_GetById
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r6, r2
	adds r5, r0, #0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_0200adf4
	cmp r2, r3
	bne .L_0200ace8
	ldr r3, [r5, #20]
	cmp r3, #0
	beq .L_0200ade4
.L_0200ace8:
	ldr r3, [r5, #8]
	mov r7, sp
	str r3, [r7]
	movs r1, #128
	ldr r3, [r5, #12]
	lsls r1, r1, #10
	str r3, [r7, #4]
	adds r0, r5, #0
	ldr r3, [r5, #16]
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r1, r7, #0
	bl Func_02003d2c
	ldr r3, .L_0200adf8
	movs r2, #4
	ldr r3, [r3]
	adds r6, r0, #0
	ands r3, r2
	cmp r3, #0
	bne .L_0200ad18
	adds r0, r5, #0
	bl Func_02002bd4
.L_0200ad18:
	cmp r6, #0
	bge .L_0200ad6a
	movs r1, #129
	mov r0, r8
	lsls r1, r1, #1
	bl Func_02003e3c
	ldr r3, [r5, #16]
	movs r0, #128
	lsls r0, r0, #12
	adds r3, r3, r0
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	adds r0, r5, #0
	bl Func_02003ce4
	adds r0, r5, #0
	movs r1, #49
	bl Func_02003cc4
	adds r0, r5, #0
	bl Func_02003cec
.L_0200ad46:
	movs r0, #1
	bl WaitFrames
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	cmp r2, r3
	bne .L_0200ad46
	adds r0, r5, #0
	bl Func_02002bd4
	adds r0, r5, #0
	movs r1, #49
	bl Func_02003cc4
	movs r0, #3
	bl WaitFrames
	b .L_0200ade4
.L_0200ad6a:
	ldr r3, [r5, #8]
	movs r1, #128
	str r3, [r7]
	lsls r1, r1, #12
	ldr r3, [r5, #12]
	adds r0, r5, #0
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r1, r7, #0
	bl Func_02003d2c
	adds r6, r0, #0
	cmp r6, #0
	bgt .L_0200ade4
	ldr r3, [r5, #8]
	ldr r2, .L_0200adfc
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r7]
	adds r1, r7, #0
	ldr r3, [r5, #12]
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	adds r3, r3, r2
	str r3, [r7, #8]
	bl Func_02003d2c
	adds r6, r0, #0
	cmp r6, #0
	bgt .L_0200ade4
	ldr r3, [r5, #8]
	ldr r2, .L_0200ae00
	ldr r0, .L_0200adfc
	adds r3, r3, r2
	str r3, [r7]
	adds r1, r7, #0
	ldr r3, [r5, #12]
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	adds r3, r3, r0
	str r3, [r7, #8]
	adds r0, r5, #0
	bl Func_02003d2c
	adds r6, r0, #0
	cmp r6, #0
	bgt .L_0200ade4
	mov r1, r10
	ldr r3, [r1, #16]
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r1, #16]
	ldr r3, [r5, #16]
	adds r3, r3, r2
	str r3, [r5, #16]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
.L_0200ade4:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200adf0:
	.4byte gPartyState
.L_0200adf4:
	.4byte 0x0000009e
.L_0200adf8:
	.4byte gFrameCount
.L_0200adfc:
	.4byte 0x0005b333
.L_0200ae00:
	.4byte 0xfffa4ccd
	.section .text.x0200ae34,"ax",%progbits
	.balign 4
	.global Func_02002e34
	.thumb_func
Func_02002e34:
	push {r5, r6, r7, lr}
	ldr r5, .L_0200aecc
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	adds r7, r0, #0
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	ldr r0, [r5]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #32
	orrs r3, r2
	strb r3, [r1]
	movs r0, #215
	bl Func_02003ef4
	adds r0, r6, #0
	movs r1, #18
	bl Func_02003cc4
	movs r0, #153
	lsls r0, r0, #2
	bl Func_02003ef4
	movs r5, #0
.L_0200ae86:
	cmp r5, #30
	bne .L_0200ae8e
	bl Event_FinishScreenTransition
.L_0200ae8e:
	ldr r3, [r6, #12]
	ldr r2, .L_0200aed0
	adds r3, r3, r2
	str r3, [r6, #12]
	ldrh r3, [r6, #6]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r6, #6]
	movs r3, #7
	ands r3, r5
	cmp r3, #0
	bne .L_0200aeb2
	movs r0, #15
	bl Object_GetById
	bl Func_02002bd4
.L_0200aeb2:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	ble .L_0200ae86
	bl Func_02003d84
	adds r0, r7, #0
	bl Func_02003e5c
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200aecc:
	.4byte gPartyState
.L_0200aed0:
	.4byte 0xffffc000
	.section .text.x0200aed4,"ax",%progbits
	.global Func_02002ed4
	.thumb_func
Func_02002ed4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200af5c
	sub sp, #56
	ldr r2, [r3]
	mov r8, r3
	movs r3, #1
	ands r3, r2
	adds r7, r0, #0
	cmp r3, #0
	beq .L_0200af50
	movs r3, #7
	add r6, sp, #16
	str r3, [r6, #4]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	bne .L_0200aefe
	movs r3, #5
	str r3, [r6, #4]
.L_0200aefe:
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	movs r5, #0
	str r3, [r6, #8]
	str r3, [r6, #12]
	str r5, [r6]
	bl Random16Far
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r4, r0, #1
	adds r4, r4, r0
	lsls r3, r4, #4
	adds r4, r4, r3
	lsls r3, r4, #8
	adds r4, r4, r3
	mov r3, r8
	ldr r2, [r3]
	movs r3, #15
	ldr r0, [r7, #8]
	ands r2, r3
	movs r3, #8
	subs r3, r3, r2
	ldr r1, [r7, #12]
	lsls r3, r3, #16
	adds r0, r0, r3
	movs r3, #208
	lsls r3, r3, #13
	adds r1, r1, r3
	movs r3, #176
	lsls r3, r3, #12
	ldr r2, [r7, #16]
	negs r4, r4
	str r3, [sp, #8]
	movs r3, #0
	str r4, [sp, #0]
	str r5, [sp, #4]
	str r6, [sp, #12]
	bl Func_0200015c
.L_0200af50:
	movs r0, #0
	add sp, #56
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200af5c:
	.4byte gFrameCount
	.section .text.x0200af60,"ax",%progbits
	.global Func_02002f60
	.thumb_func
Func_02002f60:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r5, .L_0200aff4
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	mov r10, r0
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	movs r0, #228
	bl Func_02003ef4
	ldr r3, .L_0200aff8
	movs r2, #0
	str r3, [r6, #108]
	mov r8, r2
	adds r3, r6, #0
	mov r2, r8
	adds r3, #85
	strb r2, [r3]
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	str r3, [r6, #48]
	movs r1, #2
	ldr r0, [r5]
	bl Object_SetModeById
	movs r2, #8
	negs r2, r2
	movs r1, #0
	ldr r0, [r5]
	bl ObjectMotion_OffsetPositionAndResetMotion
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #9
	bl Object_SetPartAttribute
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	mov r3, r8
	str r3, [r6, #108]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	mov r0, r10
	bl Func_02003e5c
	bl Func_02003d84
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200aff4:
	.4byte gPartyState
.L_0200aff8:
	.4byte Func_02002ed4
	.section .text.x0200affc,"ax",%progbits
	.global Func_02002ffc
	.thumb_func
Func_02002ffc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200b0e8
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	adds r7, r0, #0
	cmp r7, #0
	bne .L_0200b0e2
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r0, r0
	negs r1, r1
	movs r3, #0
	bl Motion_CamBounds
	movs r3, #85
	adds r3, r3, r6
	strb r7, [r3]
	mov r8, r3
	movs r2, #10
	ldrsh r1, [r6, r2]
	movs r3, #18
	ldrsh r2, [r6, r3]
	ldr r3, .L_0200b0ec
	lsls r2, r2, #16
	adds r2, r2, r3
	lsls r1, r1, #16
	ldr r0, [r5]
	bl Func_02003ddc
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #9
	bl Object_SetPartAttribute
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	bl Event_StartScreenTransition
	movs r0, #228
	bl Func_02003ef4
	ldr r3, .L_0200b0f0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	str r3, [r6, #108]
	ldr r0, [r5]
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r2, #8
	movs r1, #0
	ldr r0, [r5]
	bl ObjectMotion_CommitPositionAndActivate
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	ldr r1, [r6, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	movs r2, #10
	ldr r0, [r5]
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r3, #3
	mov r2, r8
	strb r3, [r2]
	str r7, [r6, #108]
	bl Func_02003ed4
	bl Event_WaitTransitionFrames
	bl Func_02003d84
.L_0200b0e2:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200b0e8:
	.4byte gPartyState
.L_0200b0ec:
	.4byte 0xfff00000
.L_0200b0f0:
	.4byte Func_02002ed4
	.section .text.x0200b0f4,"ax",%progbits
	.global Func_020030f4
	.thumb_func
Func_020030f4:
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
	sub sp, #16
	adds r2, r1, #0
	adds r2, #228
	ldr r3, [r2]
	ldr r0, .L_0200b268
	str r3, [sp, #12]
	ldr r4, [sp, #12]
	ldr r3, .L_0200b26c
	adds r5, r0, #4
	ands r4, r3
	str r4, [sp, #12]
	ldr r2, [r2, #4]
	ands r2, r3
	ldr r3, [r1]
	mov r11, r2
	ldr r3, [r3, #4]
	ldr r2, .L_0200b270
	mov r10, r3
	ldrh r3, [r0]
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsrs r3, r3, #5
	str r3, [sp, #0]
	movs r3, #31
	str r3, [sp, #4]
.L_0200b13c:
	ldrh r0, [r5, #18]
	adds r3, r0, #0
	cmp r3, #0
	bne .L_0200b146
	b .L_0200b24c
.L_0200b146:
	ldr r4, [r5, #12]
	ldr r1, [r5, #20]
	ldr r2, [r5, #4]
	mov r12, r4
	cmp r1, #0
	beq .L_0200b170
	ldr r3, [r1]
	ldr r4, [sp, #12]
	subs r6, r3, r4
	ldr r3, [r1, #4]
	mov r4, r11
	adds r3, r3, r2
	mov r2, r10
	subs r2, r3, r2
	ldr r3, [r1, #8]
	mov r1, r10
	subs r3, r3, r4
	subs r7, r3, r1
	mov r8, r2
	subs r4, r7, r2
	b .L_0200b188
.L_0200b170:
	ldr r3, [r5]
	ldr r4, [sp, #12]
	mov r1, r10
	subs r6, r3, r4
	ldr r3, [r5, #8]
	subs r1, r2, r1
	mov r2, r11
	mov r4, r10
	subs r3, r3, r2
	subs r7, r3, r4
	mov r8, r1
	subs r4, r7, r1
.L_0200b188:
	ldr r3, [r5, #4]
	cmp r3, r12
	bne .L_0200b19a
	adds r3, r0, #0
	movs r0, #192
	lsls r0, r0, #4
	mov r9, r0
	cmp r3, #2
	bne .L_0200b1a0
.L_0200b19a:
	movs r1, #128
	lsls r1, r1, #4
	mov r9, r1
.L_0200b1a0:
	mov r0, r8
	adds r3, r0, r7
	asrs r3, r3, #16
	adds r3, #50
	asrs r2, r6, #16
	asrs r1, r4, #16
	str r3, [sp, #8]
	movs r3, #135
	adds r6, r2, #0
	adds r4, r1, #0
	adds r2, #7
	lsls r3, r3, #1
	subs r6, #8
	subs r4, #8
	cmp r2, r3
	bhi .L_0200b24c
	adds r3, r1, #0
	adds r3, #39
	cmp r3, #238
	bhi .L_0200b1fa
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r6, r3
	movs r3, #255
	ands r4, r3
	ldrh r3, [r5, #16]
	mov r0, r9
	lsls r1, r3, #3
	movs r3, #0
	str r3, [r5, #24]
	lsls r3, r6, #16
	orrs r4, r3
	ldr r3, .L_0200b274
	orrs r4, r3
	str r4, [r5, #28]
	ldr r4, [sp, #0]
	adds r3, r4, r1
	orrs r3, r0
	str r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #24
	ldr r1, [sp, #8]
	bl Func_02003c94
.L_0200b1fa:
	mov r1, r8
	subs r4, r7, r1
	asrs r3, r4, #16
	adds r4, r3, #0
	adds r3, #55
	adds r4, #8
	cmp r3, #238
	bhi .L_0200b24c
	ldr r3, [r5, #4]
	ldr r2, [r5, #12]
	subs r3, r3, r2
	asrs r1, r3, #16
	cmp r1, #0
	beq .L_0200b24c
	movs r3, #255
	adds r0, r5, #0
	subs r1, #1
	ands r4, r3
	adds r0, #36
	cmp r1, #7
	bls .L_0200b226
	movs r1, #7
.L_0200b226:
	lsls r3, r1, #2
	adds r1, r3, #0
	movs r3, #0
	str r3, [r0]
	lsls r3, r6, #16
	orrs r4, r3
	movs r3, #192
	lsls r3, r3, #7
	orrs r4, r3
	str r4, [r5, #40]
	ldr r2, [sp, #0]
	adds r1, #64
	adds r3, r2, r1
	mov r4, r9
	orrs r4, r3
	str r4, [r5, #44]
	ldr r1, [sp, #8]
	bl Func_02003c94
.L_0200b24c:
	ldr r0, [sp, #4]
	adds r5, #48
	subs r0, #1
	str r0, [sp, #4]
	cmp r0, #0
	blt .L_0200b25a
	b .L_0200b13c
.L_0200b25a:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200b268:
	.4byte Data_0200536c
.L_0200b26c:
	.4byte 0xffff0000
.L_0200b270:
	.4byte ResourceTableEntries
.L_0200b274:
	.4byte 0x40002000
	.section .text.x0200b278,"ax",%progbits
	.global Func_02003278
	.thumb_func
Func_02003278:
	push {r5, r6, lr}
	ldr r6, .L_0200b2c4
	movs r1, #192
	lsls r1, r1, #3
	ldr r3, .L_0200b2c8
	adds r1, #4
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	movs r0, #200
	lsls r0, r0, #4
	bl MemAlloc
	adds r5, r0, #0
	adds r1, r5, #0
	ldr r0, .L_0200b2cc
	bl Func_02003c7c
	bl Resource_FindFreeEntry
	strh r0, [r6]
	movs r1, #200
	lsls r1, r1, #4
	adds r2, r5, #0
	ldrh r0, [r6]
	bl VramBlock_LoadCached
	adds r0, r5, #0
	bl MemFree
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #28
	ldr r0, .L_0200b2d0
	bl Scheduler_AddOrUpdateCallback
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b2c4:
	.4byte Data_0200536c
.L_0200b2c8:
	.4byte IwramClearWords
.L_0200b2cc:
	.4byte Data_0200414c
.L_0200b2d0:
	.4byte Func_020030f4
	.section .text.x0200b2d4,"ax",%progbits
	.global Func_020032d4
	.thumb_func
Func_020032d4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #28
	str r1, [sp, #24]
	str r2, [sp, #20]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	mov r10, r0
	mov r12, r3
	movs r3, #132
	lsls r3, r3, #1
	add r3, r12
	ldr r1, [r3, #48]
	mov r2, r12
	adds r2, #236
	ldr r0, [r2]
	mov r8, r1
	ldr r1, [r3, #8]
	adds r2, #4
	adds r1, r1, r0
	str r1, [sp, #16]
	ldr r1, [r2]
	ldr r3, [r3, #12]
	ldr r2, .L_0200b418
	adds r3, r3, r1
	str r3, [sp, #12]
	mov r3, r12
	adds r3, #244
	ldr r3, [r3]
	mov r9, r2
	subs r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #8]
	mov r3, r12
	adds r3, #248
	ldr r3, [r3]
	asrs r0, r0, #20
	subs r3, r3, r1
	asrs r3, r3, #20
	str r3, [sp, #4]
	asrs r1, r1, #20
	ldrh r2, [r2, #2]
	lsls r1, r1, #7
	lsls r3, r2, #1
	adds r1, r1, r0
	str r1, [sp, #0]
	adds r3, r3, r2
	ldr r1, [sp, #24]
	ldr r2, .L_0200b41c
	lsls r3, r3, #4
	add r3, r9
	adds r5, r3, #4
	lsls r3, r1, #8
	str r3, [r2]
	ldr r2, [sp, #0]
	ldr r1, [sp, #4]
	lsls r3, r2, #2
	add r8, r3
	movs r3, #0
	mov lr, r3
	cmp lr, r1
	bge .L_0200b408
.L_0200b35c:
	mov r2, lr
	lsls r2, r2, #16
	lsrs r3, r2, #7
	mov r11, r2
	ldr r2, [sp, #8]
	mov r1, r8
	movs r7, #0
	adds r6, r1, r3
	cmp r7, r2
	bge .L_0200b3f2
.L_0200b370:
	ldrb r4, [r6, #2]
	cmp r4, #0
	beq .L_0200b3de
	cmp r4, r10
	bcc .L_0200b3de
	mov r3, r10
	adds r3, #8
	cmp r4, r3
	bcs .L_0200b3de
	ldr r2, [sp, #16]
	lsls r1, r7, #16
	lsrs r1, r1, #16
	lsls r3, r1, #20
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [sp, #12]
	mov r3, r11
	lsrs r0, r3, #16
	lsls r3, r0, #20
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5, #8]
	ldr r2, [sp, #20]
	lsls r0, r0, #7
	lsls r3, r2, #19
	str r3, [r5, #12]
	ldr r2, [sp, #24]
	adds r1, r1, r0
	lsls r3, r2, #19
	mov r2, r10
	str r3, [r5, #4]
	subs r3, r4, r2
	strh r3, [r5, #16]
	movs r2, #0
	movs r3, #1
	str r2, [r5, #20]
	strh r3, [r5, #18]
	mov r2, r9
	ldrh r3, [r2, #2]
	adds r5, #48
	adds r3, #1
	strh r3, [r2, #2]
	movs r3, #158
	lsls r3, r3, #1
	add r3, r12
	ldr r2, [r3]
	ldr r3, [sp, #0]
	adds r2, r2, r3
	movs r3, #120
	strb r3, [r2, r1]
.L_0200b3de:
	movs r1, #128
	lsls r3, r7, #16
	lsls r1, r1, #9
	ldr r2, [sp, #8]
	adds r3, r3, r1
	asrs r7, r3, #16
	lsrs r3, r3, #16
	adds r6, #4
	cmp r3, r2
	blt .L_0200b370
.L_0200b3f2:
	mov r1, lr
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	ldr r2, [sp, #4]
	asrs r1, r3, #16
	lsrs r3, r3, #16
	mov lr, r1
	cmp r3, r2
	blt .L_0200b35c
.L_0200b408:
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b418:
	.4byte Data_0200536c
.L_0200b41c:
	.4byte Data_0202c001 + 0x1df
	.section .text.x0200b49c,"ax",%progbits
	.global Func_0200349c
	.thumb_func
Func_0200349c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r4, r1, #0
	ldr r2, .L_0200b4f0
	ldr r1, [r3, #32]
	cmp r0, #31
	bhi .L_0200b4ec
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	adds r3, r3, r2
	adds r0, r3, #4
	ldr r3, [r0]
	cmp r3, #0
	beq .L_0200b4ec
	adds r3, r4, #0
	cmp r4, #0
	bge .L_0200b4c6
	ldr r2, .L_0200b4f4
	adds r3, r4, r2
.L_0200b4c6:
	movs r2, #255
	asrs r3, r3, #19
	ands r3, r2
	ldr r2, .L_0200b4f8
	lsls r3, r3, #8
	str r3, [r2]
	movs r2, #158
	lsls r2, r2, #1
	adds r3, r1, r2
	ldr r1, [r3]
	ldr r3, [r0, #8]
	ldr r2, [r0]
	asrs r3, r3, #20
	lsls r3, r3, #7
	asrs r2, r2, #20
	adds r2, r2, r3
	movs r3, #121
	strb r3, [r1, r2]
	str r4, [r0, #4]
.L_0200b4ec:
	pop {pc}
	.2byte 0x0000
.L_0200b4f0:
	.4byte Data_0200536c
.L_0200b4f4:
	.4byte 0x0007ffff
.L_0200b4f8:
	.4byte Data_0202c001 + 0x1e3
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
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	sub sp, #16
	adds r2, r1, #0
	adds r2, #228
	ldr r3, [r2]
	ldr r0, .L_0200b618
	str r3, [sp, #12]
	ldr r4, [sp, #12]
	ldr r3, .L_0200b61c
	ands r4, r3
	str r4, [sp, #12]
	ldr r2, [r2, #4]
	ands r2, r3
	ldr r3, [r1]
	mov r11, r2
	ldr r3, [r3, #4]
	ldr r2, .L_0200b620
	str r3, [sp, #8]
	ldrh r3, [r0]
	adds r0, #36
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	mov r9, r0
	lsrs r3, r3, #5
	str r3, [sp, #0]
	movs r3, #31
	str r3, [sp, #4]
.L_0200b546:
	mov r4, r9
	ldrh r3, [r4, #12]
	cmp r3, #0
	beq .L_0200b5fa
	ldr r3, [r4]
	ldr r2, [sp, #12]
	ldr r5, [r4, #4]
	ldr r4, [r4, #8]
	subs r2, r3, r2
	ldr r3, [sp, #8]
	mov r8, r4
	ldr r4, [sp, #8]
	mov r10, r2
	subs r5, r5, r3
	mov r2, r8
	mov r3, r11
	subs r2, r2, r3
	subs r2, r2, r4
	subs r7, r2, r5
	mov r8, r2
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	mov r2, r10
	lsls r3, r3, #1
	asrs r6, r2, #16
	lsrs r3, r3, #16
	adds r6, r6, r3
	movs r3, #10
	negs r3, r3
	adds r3, r3, r6
	mov r10, r3
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	asrs r2, r7, #16
	add r5, r8
	lsrs r3, r3, #16
	adds r2, r2, r3
	asrs r5, r5, #16
	movs r4, #135
	adds r7, r2, #0
	adds r5, #50
	adds r6, #5
	lsls r4, r4, #1
	subs r7, #10
	mov r8, r5
	cmp r6, r4
	bhi .L_0200b5fa
	adds r3, r2, #0
	adds r3, #37
	cmp r3, #238
	bhi .L_0200b5fa
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	mov r2, r10
	ands r2, r3
	movs r3, #255
	mov r4, r9
	ands r7, r3
	movs r3, #0
	str r3, [r4, #16]
	lsls r3, r2, #16
	orrs r7, r3
	ldr r3, .L_0200b624
	mov r5, r9
	orrs r7, r3
	str r7, [r4, #20]
	bl Random16Far
	lsls r3, r0, #1
	adds r3, r3, r0
	ldr r2, [sp, #0]
	lsrs r3, r3, #16
	lsls r3, r3, #3
	movs r4, #128
	adds r3, r2, r3
	lsls r4, r4, #4
	adds r5, #24
	orrs r3, r4
	mov r0, r9
	str r3, [r5]
	adds r0, #16
	mov r1, r8
	bl Func_02003c94
.L_0200b5fa:
	ldr r2, [sp, #4]
	movs r3, #28
	subs r2, #1
	str r2, [sp, #4]
	add r9, r3
	cmp r2, #0
	bge .L_0200b546
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b618:
	.4byte Data_02005970
.L_0200b61c:
	.4byte 0xffff0000
.L_0200b620:
	.4byte ResourceTableEntries
.L_0200b624:
	.4byte 0x40002000
	.section .text.x0200b628,"ax",%progbits
	.global Func_02003628
	.thumb_func
Func_02003628:
	push {r5, r6, lr}
	ldr r6, .L_0200b678
	movs r1, #233
	ldr r3, .L_0200b67c
	lsls r1, r1, #2
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	movs r0, #128
	lsls r0, r0, #3
	bl MemAlloc
	adds r5, r0, #0
	adds r1, r5, #0
	ldr r0, .L_0200b680
	bl Func_02003c7c
	bl Resource_FindFreeEntry
	strh r0, [r6]
	movs r1, #128
	adds r2, r5, #0
	lsls r1, r1, #3
	ldrh r0, [r6]
	bl VramBlock_LoadCached
	adds r0, r5, #0
	bl MemFree
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #28
	ldr r0, .L_0200b684
	bl Scheduler_AddOrUpdateCallback
	movs r0, #220
	bl Func_02003ef4
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b678:
	.4byte Data_02005970
.L_0200b67c:
	.4byte IwramClearWords
.L_0200b680:
	.4byte Data_02004351
.L_0200b684:
	.4byte Func_020034fc
	.section .text.x0200b6b8,"ax",%progbits
	.global Func_020036b8
	.thumb_func
Func_020036b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	str r1, [sp, #12]
	movs r1, #0
	str r1, [sp, #8]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	ldr r4, .L_0200b83c
	str r3, [sp, #4]
	movs r2, #1
	ldrh r6, [r4, #2]
	mov r11, r0
	mov r10, r1
	adds r7, r4, #4
	mov r8, r2
.L_0200b6e4:
	ldr r3, [r7]
	cmp r3, #0
	beq .L_0200b7ac
	ldrh r2, [r4, #2]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, r4, r3
	adds r5, r3, #0
	adds r5, #36
.L_0200b6f8:
	ldr r1, [sp, #4]
	movs r2, #156
	lsls r2, r2, #1
	adds r3, r1, r2
	ldr r1, [r3]
	ldr r3, [r7, #8]
	ldr r0, [r7]
	asrs r3, r3, #20
	asrs r2, r0, #20
	lsls r3, r3, #7
	adds r2, r2, r3
	lsls r2, r2, #2
	adds r1, r1, r2
	ldrb r1, [r1, #2]
	cmp r1, #255
	bne .L_0200b738
	str r0, [r5]
	ldr r3, [r7, #8]
	movs r1, #1
	str r3, [r5, #8]
	ldr r3, [r7, #4]
	mov r10, r1
	str r3, [r5, #4]
	movs r3, #1
	strh r3, [r5, #12]
	adds r5, #28
	ldrh r3, [r4, #2]
	adds r3, #1
	strh r3, [r4, #2]
	movs r3, #0
	mov r9, r3
	b .L_0200b782
.L_0200b738:
	cmp r1, r11
	blt .L_0200b75e
	mov r3, r11
	adds r3, #7
	cmp r1, r3
	bgt .L_0200b75e
	ldrh r3, [r7, #14]
	ldr r2, .L_0200b840
	lsls r3, r3, #3
	adds r3, r3, r1
	mov r1, r11
	subs r3, r3, r1
	ldrsb r3, [r2, r3]
	mov r9, r3
	cmp r3, #8
	beq .L_0200b75a
	ldrh r6, [r4, #2]
.L_0200b75a:
	movs r2, #0
	b .L_0200b780
.L_0200b75e:
	ldr r3, [sp, #12]
	cmp r1, r3
	bne .L_0200b776
	ldr r2, [sp, #8]
	movs r1, #8
	adds r2, #1
	movs r3, #0
	ldrh r6, [r4, #2]
	mov r9, r1
	str r2, [sp, #8]
	mov r10, r3
	b .L_0200b782
.L_0200b776:
	cmp r1, #0
	bne .L_0200b782
	movs r1, #8
	movs r2, #0
	mov r9, r1
.L_0200b780:
	mov r10, r2
.L_0200b782:
	mov r3, r9
	cmp r3, #8
	beq .L_0200b7a4
	ldrh r3, [r7, #14]
	movs r2, #3
	add r3, r9
	ands r3, r2
	strh r3, [r7, #14]
	movs r0, #128
	ldrh r1, [r7, #14]
	lsls r0, r0, #13
	lsls r1, r1, #14
	adds r2, r7, #0
	str r4, [sp, #0]
	bl Vector_AddPolarOffsetFar
	ldr r4, [sp, #0]
.L_0200b7a4:
	mov r1, r10
	cmp r1, #0
	bne .L_0200b6f8
	strh r6, [r4, #2]
.L_0200b7ac:
	movs r2, #1
	negs r2, r2
	add r8, r2
	mov r3, r8
	adds r7, #16
	cmp r3, #0
	bge .L_0200b6e4
	lsls r3, r6, #3
	subs r3, r3, r6
	lsls r3, r3, #2
	adds r3, r4, r3
	adds r5, r3, #0
	adds r5, #36
	cmp r6, #31
	bgt .L_0200b7e8
	movs r3, #32
	subs r3, r3, r6
	mov r8, r3
.L_0200b7d0:
	ldrh r3, [r5, #12]
	cmp r3, #0
	beq .L_0200b7da
	movs r3, #0
	strh r3, [r5, #12]
.L_0200b7da:
	movs r1, #1
	negs r1, r1
	add r8, r1
	mov r2, r8
	adds r5, #28
	cmp r2, #0
	bne .L_0200b7d0
.L_0200b7e8:
	adds r5, r4, #0
	adds r5, #36
	cmp r6, #0
	beq .L_0200b82c
	mov r8, r6
.L_0200b7f2:
	ldrh r3, [r5, #12]
	cmp r3, #0
	beq .L_0200b81e
	ldr r1, [sp, #4]
	movs r2, #156
	lsls r2, r2, #1
	adds r3, r1, r2
	ldr r1, [r3]
	ldr r3, [r5, #8]
	ldr r2, [r5]
	asrs r3, r3, #20
	lsls r3, r3, #7
	asrs r2, r2, #20
	adds r2, r2, r3
	lsls r2, r2, #2
	adds r1, r1, r2
	ldrb r2, [r1, #3]
	movs r3, #0
	strb r3, [r1, #2]
	movs r3, #128
	orrs r3, r2
	strb r3, [r1, #3]
.L_0200b81e:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r1, r8
	adds r5, #28
	cmp r1, #0
	bne .L_0200b7f2
.L_0200b82c:
	ldr r0, [sp, #8]
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200b83c:
	.4byte Data_02005970
.L_0200b840:
	.4byte Data_0200534c
	.section .text.x0200b89c,"ax",%progbits
	.global Func_0200389c
	.thumb_func
Func_0200389c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r2, .L_0200b98c
	ldr r3, .L_0200b990
	mov r8, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	bl Object_GetById
	ldr r5, [r6, #68]
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r4, r0, #0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #16]
	str r4, [sp, #0]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bge .L_0200b8ea
	adds r3, #15
.L_0200b8ea:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_0200b936
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02003844
	cmp r0, #0
	beq .L_0200b936
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	movs r3, #0
	str r3, [r6, #76]
.L_0200b936:
	movs r3, #164
	lsls r3, r3, #1
	mov r2, r8
	ldrh r0, [r2, r3]
	movs r7, #0
	cmp r0, #0
	beq .L_0200b982
	adds r5, r2, r3
.L_0200b946:
	bl Object_GetById
	adds r4, r0, #0
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_0200b974
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02003844
	cmp r0, #0
	beq .L_0200b974
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	movs r3, #0
	str r3, [r6, #76]
.L_0200b974:
	adds r7, #1
	cmp r7, #3
	bgt .L_0200b982
	adds r5, #2
	ldrh r0, [r5]
	cmp r0, #0
	bne .L_0200b946
.L_0200b982:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b98c:
	.4byte Data_020023c4 + 0x188
.L_0200b990:
	.4byte gPartyState
	.section .text.x0200b994,"ax",%progbits
	.global Func_02003994
	.thumb_func
Func_02003994:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r2, r5, #0
	adds r3, #4
	strb r6, [r3]
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r3, #3
	ldr r0, [r5, #80]
	ands r1, r3
	ldrb r2, [r0, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	lsls r1, r1, #2
	orrs r3, r1
	strb r3, [r0, #9]
	movs r1, #0
	adds r0, r5, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #2
	bl Func_02003cc4
	adds r0, r5, #0
	ldr r1, .L_0200b9ec
	bl Func_02003ccc
	adds r0, r5, #0
	movs r1, #10
	bl Object_SetPartAttribute
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	str r6, [r5, #48]
	str r6, [r5, #52]
	pop {r5, r6, pc}
.L_0200b9ec:
	.4byte Data_020044c8
	.section .text.x0200ba70,"ax",%progbits
	.global Func_02003a70
	.thumb_func
Func_02003a70:
	push {r5, r6, lr}
	ldr r3, .L_0200baac
	movs r6, #15
	ldr r0, [r3, #4]
	adds r5, r3, #0
	mov lr, r0
	.2byte 0xf800
	adds r5, #8
.L_0200ba80:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #0
	beq .L_0200baa2
	movs r1, #2
	ldrsh r3, [r5, r1]
	ldrh r2, [r5, #2]
	cmp r3, #0
	bgt .L_0200ba9e
	adds r0, r5, #4
	ldr r1, [r5, #16]
	bl Func_020039f0
	movs r3, #9
	b .L_0200baa0
.L_0200ba9e:
	subs r3, r2, #1
.L_0200baa0:
	strh r3, [r5, #2]
.L_0200baa2:
	subs r6, #1
	adds r5, #20
	cmp r6, #0
	bge .L_0200ba80
	pop {r5, r6, pc}
.L_0200baac:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200bab0,"ax",%progbits
	.global Func_02003ab0
	.thumb_func
Func_02003ab0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r0, #0
	ldr r0, .L_0200bb98
	mov r9, r2
	mov r10, r0
	movs r2, #8
	movs r0, #10
	add r2, r10
	adds r0, #255
	sub sp, #4
	adds r7, r1, #0
	mov r8, r2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200bb58
	movs r1, #168
	lsls r1, r1, #1
	ldr r3, .L_0200bb9c
	mov r0, r10
	mov lr, r3
	.2byte 0xf800
	ldrh r1, [r6]
	movs r4, #0
	adds r6, #2
	cmp r1, #0
	ble .L_0200bb30
.L_0200baee:
	ldrh r2, [r6]
	mov r0, r8
	movs r3, #0
	ldrh r5, [r6, #2]
	strh r3, [r0]
	movs r3, #128
	lsls r3, r3, #13
	lsls r1, r1, #16
	lsls r2, r2, #16
	str r2, [r0, #12]
	str r1, [r0, #4]
	adds r2, r2, r3
	movs r0, #0
	str r4, [sp, #0]
	bl Map_GetTerrainHeight
	ldr r4, [sp, #0]
	mov r2, r8
	mov r3, r8
	lsls r5, r5, #16
	str r0, [r2, #8]
	str r5, [r2, #16]
	movs r0, #20
	strh r4, [r3, #2]
	adds r4, #1
	adds r6, #4
	add r8, r0
	cmp r4, #15
	bgt .L_0200bb30
	ldrh r1, [r6]
	adds r6, #2
	cmp r1, #0
	bgt .L_0200baee
.L_0200bb30:
	cmp r7, #0
	beq .L_0200bb58
	ldrh r1, [r7]
	movs r4, #0
	adds r7, #2
	cmp r1, #0
	ble .L_0200bb58
.L_0200bb3e:
	movs r2, #164
	lsls r3, r4, #1
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r0, r10
	adds r4, #1
	strh r1, [r0, r3]
	cmp r4, #3
	bgt .L_0200bb58
	ldrh r1, [r7]
	adds r7, #2
	cmp r1, #0
	bgt .L_0200bb3e
.L_0200bb58:
	movs r1, #128
	lsls r1, r1, #19
	mov r3, r10
	mov r2, r9
	adds r1, #80
	str r2, [r3, #4]
	ldrh r3, [r1]
	cmp r3, #0
	bne .L_0200bb80
	movs r3, #192
	movs r2, #128
	lsls r3, r3, #4
	lsls r2, r2, #19
	adds r3, #8
	adds r2, #82
	strh r3, [r2]
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #16
	strh r3, [r1]
.L_0200bb80:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200bba0
	bl Scheduler_AddOrUpdateCallback
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200bb98:
	.4byte Data_020023c4 + 0x188
.L_0200bb9c:
	.4byte IwramClearWords
.L_0200bba0:
	.4byte Func_02003a70
	.section .text.x0200bba4,"ax",%progbits
	.global Func_02003ba4
	.thumb_func
Func_02003ba4:
	ldr r2, .L_0200bbb4
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r3, r3, r2
	movs r2, #8
	ldrsh r0, [r3, r2]
	bx lr
.L_0200bbb4:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200bbb8,"ax",%progbits
	.global Func_02003bb8
	.thumb_func
Func_02003bb8:
	ldr r2, .L_0200bbc8
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r3, r3, r2
	strh r1, [r3, #8]
	bx lr
	.2byte 0x0000
.L_0200bbc8:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200bc00,"ax",%progbits
	.global Func_02003c00
	.thumb_func
Func_02003c00:
	ldr r2, .L_0200bc10
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r3, r3, r2
	str r1, [r3, #24]
	bx lr
	.2byte 0x0000
.L_0200bc10:
	.4byte Data_020023c4 + 0x188
	.section .bss,"aw",%nobits
	.global Data_0200536c
Data_0200536c:
	.space 0x00000604
	.global Data_02005970
Data_02005970:
