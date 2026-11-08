.syntax unified
	.thumb
	.section .text.x0200ccc8,"ax",%progbits
	.balign 4
	.ifdef TLA_EDITION_JA
	.global Func_02004cc8
	.thumb_func
Func_02004cc8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_ja_0200ce88
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Actor_GetById
	adds r7, r0, #0
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	movs r0, #216
	movs r2, #136
	movs r3, #143
	lsls r3, r3, #1
	ldr r1, .L_ja_0200ce8c
	lsls r2, r2, #18
	lsls r0, r0, #16
	bl OverlayObject_PrepareObject
	movs r2, #85
	adds r2, r2, r7
	movs r5, #0
	strb r5, [r2]
	mov r11, r0
	movs r1, #0
	adds r0, r7, #0
	mov r10, r2
	bl Actor_SetSingleChildFlags
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r7, #48]
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	ldr r3, .L_ja_0200ce90
	movs r0, #140
	adds r2, r2, r3
	ldr r1, [r7, #8]
	ldr r3, [r7, #16]
	lsls r0, r0, #1
	bl Import_ActorCreate
	movs r1, #2
	adds r6, r0, #0
	bl Func_02005d5c
	adds r0, r6, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r3, r6, #0
	adds r3, #85
	strb r5, [r3]
	movs r3, #179
	lsls r3, r3, #9
	adds r3, #102
	str r3, [r6, #48]
	str r3, [r6, #52]
	ldr r2, [r7, #12]
	movs r3, #160
	lsls r3, r3, #15
	ldr r1, [r7, #8]
	adds r2, r2, r3
	adds r0, r7, #0
	ldr r3, [r7, #16]
	bl Func_02005d8c
	ldr r2, [r6, #12]
	movs r3, #144
	lsls r3, r3, #15
	adds r2, r2, r3
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	adds r0, r6, #0
	bl Func_02005d8c
	movs r0, #137
	bl Func_02005f94
	bl Event_StartScreenTransition
	b .L_ja_0200cda0
.L_ja_0200cd80:
	ldr r3, .L_ja_0200ce94
	mov r1, r8
	ldr r2, [r3]
	ldrb r3, [r3]
	movs r0, #1
	lsls r3, r3, #12
	strh r3, [r7, #6]
	movs r3, #1
	ands r2, r3
	movs r3, #2
	lsls r3, r2
	ldrb r2, [r1]
	eors r3, r2
	strb r3, [r1]
	bl wait
.L_ja_0200cda0:
	adds r0, r7, #0
	bl Func_02005dec
	movs r2, #35
	adds r2, r2, r7
	mov r8, r2
	cmp r0, #0
	beq .L_ja_0200cd80
	movs r0, #144
	lsls r0, r0, #1
	bl Func_02005f94
	movs r0, #10
	bl FieldEvent_Wait
	ldr r5, .L_ja_0200ce88
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	ldr r3, .L_ja_0200ce98
	ldr r2, [r6, #16]
	mov r9, r3
	add r2, r9
	ldr r1, [r6, #8]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r1, #6
	str r0, [r7, #12]
	str r0, [r7, #20]
	adds r0, r7, #0
	bl Func_02005d5c
	movs r0, #6
	bl wait
	movs r0, #152
	bl Func_02005f94
	adds r0, r7, #0
	movs r1, #7
	bl Func_02005d5c
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r7, #40]
	mov r1, r10
	movs r3, #2
	strb r3, [r1]
	mov r2, r8
	movs r3, #1
	strb r3, [r2]
	ldr r3, [r7, #16]
	ldr r2, [r7, #12]
	add r3, r9
	ldr r1, [r7, #8]
	adds r0, r7, #0
	bl Func_02005d8c
	adds r0, r7, #0
	bl OverlayObject_WaitUntilIdle
	adds r0, r7, #0
	bl Func_02005d94
	ldr r0, [r5]
	movs r1, #1
	bl Actor_SetModeById
	movs r1, #5
	adds r0, r6, #0
	bl Func_02005d5c
	movs r0, #12
	bl wait
	adds r0, r6, #0
	bl Func_02005d7c
	mov r1, r10
	movs r3, #3
	strb r3, [r1]
	ldr r3, [r7, #12]
	movs r1, #1
	str r3, [r7, #20]
	adds r0, r7, #0
	bl Actor_SetSingleChildFlags
	mov r0, r11
	bl Func_02005d7c
	movs r0, #1
	bl wait
	bl Event_WaitTransitionFrames
	bl Func_02005e1c
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_ja_0200ce88:
	.4byte gMainRoutineWork
.L_ja_0200ce8c:
	.4byte 0xffe00000
.L_ja_0200ce90:
	.4byte 0xfff80000
.L_ja_0200ce94:
	.4byte gFrameCount
.L_ja_0200ce98:
	.4byte 0xfff00000
	.endif
	.section .text.x0200d1f8,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_020051f8
	.thumb_func
Func_020051f8:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #129
	adds r3, r3, r1
	ldr r5, .L_ja_0200d2b4
	lsls r2, r2, #2
	str r2, [r3]
	adds r2, #16
	adds r6, r5, r2
	ldr r0, [r6]
	sub sp, #8
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #244
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_ja_0200d2b8
	cmp r2, r3
	beq .L_ja_0200d234
	b .L_ja_0200d3fc
.L_ja_0200d234:
	bl Func_02002770
	movs r2, #241
	lsls r2, r2, #1
	adds r5, r5, r2
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #16
	bne .L_ja_0200d262
	ldr r0, [r6]
	bl Actor_GetById
	movs r3, #1
	adds r0, #34
	strb r3, [r0]
	movs r0, #8
	bl wait
	bl Func_02005d84
	movs r0, #1
	bl wait
.L_ja_0200d262:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #11
	ble .L_ja_0200d26e
	cmp r3, #16
	bne .L_ja_0200d28c
.L_ja_0200d26e:
	movs r1, #144
	ldr r0, .L_ja_0200d2bc
	lsls r1, r1, #3
	bl Task_Add
	ldr r3, .L_ja_0200d2ac
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_ja_0200d2b0
	subs r2, #2
	strh r3, [r2]
	bl Func_02004eb0
.L_ja_0200d28c:
	ldr r3, .L_ja_0200d2b4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #13
	ble .L_ja_0200d2d4
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #142
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d2ce
	b .L_ja_0200d2c0
.L_ja_0200d2ac:
	.4byte 0x00001008
.L_ja_0200d2b0:
	.4byte 0x00003f10
.L_ja_0200d2b4:
	.4byte gMainRoutineWork
.L_ja_0200d2b8:
	.4byte 0x00000055
.L_ja_0200d2bc:
	.4byte Func_02004f3c
.L_ja_0200d2c0:
	movs r1, #206
	movs r2, #164
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02005e6c
.L_ja_0200d2ce:
	ldr r0, .L_ja_0200d3f4
	bl Func_02005f6c
.L_ja_0200d2d4:
	movs r0, #10
	bl Actor_GetById
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r0, #10
	bl Actor_GetById
	str r5, [r0, #12]
	movs r0, #10
	bl Actor_GetById
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	movs r1, #3
	movs r0, #10
	bl Actor_SetSpritePriority
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #154
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200d318
	movs r0, #10
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	b .L_ja_0200d346
.L_ja_0200d318:
	movs r0, #10
	bl Actor_GetById
	movs r2, #0
	movs r1, #8
	bl Func_02001084
	movs r0, #11
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r3, #3
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #78
	movs r2, #18
	movs r3, #105
	bl Func_02005d9c
.L_ja_0200d346:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #153
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200d37a
	movs r0, #8
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r0, #9
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r0, #12
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	b .L_ja_0200d3a4
.L_ja_0200d37a:
	movs r0, #8
	bl Actor_GetById
	movs r1, #180
	movs r2, #0
	bl Func_02001084
	movs r0, #9
	bl Actor_GetById
	movs r1, #60
	movs r2, #0
	bl Func_02001084
	movs r0, #12
	bl Actor_GetById
	movs r1, #60
	movs r2, #0
	bl Func_02001420
.L_ja_0200d3a4:
	movs r0, #8
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #9
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #12
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r3, .L_ja_0200d3f8
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #11
	beq .L_ja_0200d3dc
	.2byte 0xf000, 0xfc50 @ bl 0x0200dc7c
.L_ja_0200d3dc:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d3ec
	.2byte 0xf000, 0xfc48 @ bl 0x0200dc7c
.L_ja_0200d3ec:
	bl Func_0200505c
	.2byte 0xf000, 0xfc44 @ bl 0x0200dc7c
.L_ja_0200d3f4:
	.4byte gData_02006050
.L_ja_0200d3f8:
	.4byte gMainRoutineWork
.L_ja_0200d3fc:
	ldr r3, .L_ja_0200d43c
	cmp r2, r3
	bne .L_ja_0200d4e8
	ldr r3, .L_ja_0200d434
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_ja_0200d438
	subs r2, #2
	strh r3, [r2]
	bl Func_02002770
	movs r0, #243
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d440
	movs r1, #164
	movs r2, #196
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02005e6c
	b .L_ja_0200d440
.L_ja_0200d434:
	.4byte 0x00001008
.L_ja_0200d438:
	.4byte 0x00003f10
.L_ja_0200d43c:
	.4byte 0x00000056
.L_ja_0200d440:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #152
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d45c
	movs r1, #208
	movs r2, #196
	movs r0, #11
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Func_02005e6c
.L_ja_0200d45c:
	ldr r0, .L_ja_0200d4e0
	bl Func_02005f6c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #153
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200d47e
	movs r0, #8
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	b .L_ja_0200d4ac
.L_ja_0200d47e:
	movs r0, #8
	bl Actor_GetById
	movs r2, #0
	movs r1, #60
	bl Func_02001084
	movs r0, #10
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r3, #3
	movs r2, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #68
	movs r1, #84
	movs r2, #12
	movs r3, #77
	bl Func_02005d9c
.L_ja_0200d4ac:
	movs r0, #8
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r3, .L_ja_0200d4e4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	beq .L_ja_0200d4ca
	b .L_ja_0200dc7c
.L_ja_0200d4ca:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d4d8
	b .L_ja_0200dc7c
.L_ja_0200d4d8:
	bl Func_02004cc8
	b .L_ja_0200dc7c
	.2byte 0x0000
.L_ja_0200d4e0:
	.4byte gData_02006054
.L_ja_0200d4e4:
	.4byte gMainRoutineWork
.L_ja_0200d4e8:
	ldr r3, .L_ja_0200d53c
	cmp r2, r3
	beq .L_ja_0200d4f0
	b .L_ja_0200d61c
.L_ja_0200d4f0:
	bl Func_02002770
	movs r0, #245
	bl Func_02005f4c
	ldr r3, .L_ja_0200d534
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_ja_0200d538
	subs r2, #2
	movs r1, #144
	strh r3, [r2]
	ldr r0, .L_ja_0200d540
	lsls r1, r1, #3
	bl Task_Add
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #155
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d544
	movs r1, #132
	movs r2, #158
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02005e6c
	b .L_ja_0200d544
	.2byte 0x0000
.L_ja_0200d534:
	.4byte 0x00000c08
.L_ja_0200d538:
	.4byte 0x00003f10
.L_ja_0200d53c:
	.4byte 0x00000057
.L_ja_0200d540:
	.4byte Func_02001690
.L_ja_0200d544:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #156
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d560
	movs r1, #154
	movs r2, #142
	movs r0, #11
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02005e6c
.L_ja_0200d560:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #157
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d57c
	movs r1, #236
	movs r2, #130
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02005e6c
.L_ja_0200d57c:
	ldr r0, .L_ja_0200d8a4
	bl Func_02005f6c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #153
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200d5b6
	movs r0, #9
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r0, #10
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r0, #13
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	b .L_ja_0200d5e0
.L_ja_0200d5b6:
	movs r0, #9
	bl Actor_GetById
	movs r1, #60
	movs r2, #0
	bl Func_02001084
	movs r0, #10
	bl Actor_GetById
	movs r1, #180
	movs r2, #0
	bl Func_02001084
	movs r0, #13
	bl Actor_GetById
	movs r1, #200
	movs r2, #0
	bl Func_02001084
.L_ja_0200d5e0:
	movs r0, #9
	bl Actor_GetById
	movs r5, #0
	adds r0, #90
	strb r5, [r0]
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	strb r5, [r0]
	movs r0, #13
	bl Actor_GetById
	adds r0, #90
	strb r5, [r0]
	movs r0, #9
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #10
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #13
	b .L_ja_0200db9e
.L_ja_0200d61c:
	ldr r3, .L_ja_0200d8a8
	cmp r2, r3
	beq .L_ja_0200d624
	b .L_ja_0200d794
.L_ja_0200d624:
	bl Func_02005f54
	movs r1, #11
	movs r2, #12
	movs r0, #0
	bl Func_02005f5c
	movs r1, #13
	movs r2, #14
	movs r0, #1
	bl Func_02005f5c
	movs r1, #15
	movs r2, #16
	movs r0, #2
	bl Func_02005f5c
	movs r1, #17
	movs r2, #18
	movs r0, #3
	bl Func_02005f5c
	movs r1, #19
	movs r2, #20
	movs r0, #4
	bl Func_02005f5c
	movs r0, #18
	bl Actor_GetById
	movs r3, #1
	adds r0, #98
	strb r3, [r0]
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d6c6
	movs r3, #5
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #69
	movs r1, #64
	movs r2, #29
	movs r3, #85
	bl Func_02005d9c
	movs r3, #29
	movs r2, #21
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #20
	movs r2, #5
	movs r3, #5
	bl Func_02005dac
	movs r3, #31
	movs r2, #89
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #32
	movs r1, #89
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	movs r3, #33
	movs r2, #87
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #50
	movs r1, #86
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	b .L_ja_0200d716
.L_ja_0200d6c6:
	movs r3, #5
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #29
	movs r3, #85
	bl Func_02005d9c
	movs r3, #29
	movs r2, #21
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #52
	movs r1, #20
	movs r2, #5
	movs r3, #5
	bl Func_02005dac
	movs r3, #33
	movs r2, #87
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #33
	movs r1, #88
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	movs r3, #31
	movs r2, #89
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #48
	movs r1, #88
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
.L_ja_0200d716:
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d770
	movs r3, #4
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #90
	movs r1, #25
	movs r2, #86
	movs r3, #25
	bl Func_02005d9c
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #21
	movs r1, #88
	movs r2, #24
	movs r3, #89
	bl Func_02005d9c
	movs r3, #89
	movs r5, #23
	str r3, [sp, #4]
	movs r0, #23
	movs r1, #92
	movs r2, #2
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005dac
	movs r3, #25
	str r3, [sp, #4]
	movs r0, #18
	movs r1, #33
	movs r2, #2
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005dac
.L_ja_0200d770:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #158
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d78c
	movs r1, #162
	movs r2, #148
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02005e6c
.L_ja_0200d78c:
	ldr r0, .L_ja_0200d8ac
	bl Func_02005f6c
	b .L_ja_0200dc7c
.L_ja_0200d794:
	ldr r3, .L_ja_0200d8b0
	cmp r2, r3
	bne .L_ja_0200d816
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	bl Func_02002770
	movs r6, #0
.L_ja_0200d7a8:
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #2
	adds r0, r6, r3
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d7e0
	lsls r3, r6, #1
	movs r5, #24
	subs r5, r5, r3
	movs r2, #2
	movs r3, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #65
	movs r1, #1
	movs r2, #80
	adds r3, r5, #0
	bl Func_02005d9c
	movs r3, #255
	movs r0, #0
	movs r1, #16
	adds r2, r5, #0
	lsls r3, r3, #8
	bl Func_0200062c
.L_ja_0200d7e0:
	adds r6, #1
	cmp r6, #6
	bne .L_ja_0200d7a8
	movs r0, #8
	bl Actor_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #8
	bl Actor_GetById
	adds r5, r0, #0
	movs r0, #8
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r0, #20]
	str r3, [r5, #12]
	movs r0, #8
	bl Actor_GetById
	movs r1, #9
	bl Actor_SetActionCallback
	b .L_ja_0200dc7c
.L_ja_0200d816:
	ldr r3, .L_ja_0200d8b4
	cmp r2, r3
	beq .L_ja_0200d81e
	b .L_ja_0200d976
.L_ja_0200d81e:
	bl Func_02002770
	bl Func_02005f54
	movs r1, #11
	movs r2, #12
	movs r0, #0
	bl Func_02005f5c
	movs r1, #13
	movs r2, #14
	movs r0, #1
	bl Func_02005f5c
	movs r0, #2
	movs r1, #15
	movs r2, #16
	bl Func_02005f5c
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200d8b8
	movs r3, #5
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #69
	movs r1, #64
	movs r2, #32
	movs r3, #79
	bl Func_02005d9c
	movs r3, #32
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #15
	movs r2, #5
	movs r3, #5
	bl Func_02005dac
	movs r3, #34
	movs r2, #82
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #33
	movs r1, #82
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	movs r3, #36
	movs r2, #81
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #52
	movs r1, #81
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	b .L_ja_0200d908
.L_ja_0200d8a4:
	.4byte gData_0200605a
.L_ja_0200d8a8:
	.4byte 0x00000058
.L_ja_0200d8ac:
	.4byte gData_02006064
.L_ja_0200d8b0:
	.4byte 0x00000059
.L_ja_0200d8b4:
	.4byte 0x0000005a
.L_ja_0200d8b8:
	movs r3, #5
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #32
	movs r3, #79
	bl Func_02005d9c
	movs r3, #32
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #21
	movs r2, #5
	movs r3, #5
	bl Func_02005dac
	movs r3, #36
	movs r2, #81
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #35
	movs r1, #81
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	movs r3, #34
	movs r2, #82
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #50
	movs r1, #82
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
.L_ja_0200d908:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d924
	movs r1, #130
	movs r2, #148
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02005e6c
.L_ja_0200d924:
	movs r0, #244
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200d934
	b .L_ja_0200dc7c
.L_ja_0200d934:
	movs r1, #156
	movs r2, #148
	lsls r2, r2, #17
	movs r0, #8
	lsls r1, r1, #17
	bl Func_02005e6c
	movs r1, #2
	movs r0, #8
	bl Actor_SetSpritePriority
	movs r0, #8
	bl Actor_GetById
	movs r3, #2
	adds r0, #85
	strb r3, [r0]
	movs r0, #8
	bl Actor_GetById
	ldr r3, .L_ja_0200da80
	movs r2, #18
	str r3, [r0, #20]
	movs r3, #19
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #19
	movs r1, #16
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	b .L_ja_0200dc7c
.L_ja_0200d976:
	ldr r3, .L_ja_0200da84
	cmp r2, r3
	beq .L_ja_0200d97e
	b .L_ja_0200da90
.L_ja_0200d97e:
	bl Func_02002770
	movs r0, #12
	bl Actor_GetById
	movs r6, #1
	adds r0, #98
	strb r6, [r0]
	movs r0, #13
	bl Actor_GetById
	adds r0, #98
	strb r6, [r0]
	movs r0, #14
	bl Actor_GetById
	adds r0, #98
	strb r6, [r0]
	movs r0, #15
	bl Actor_GetById
	adds r0, #98
	strb r6, [r0]
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200d9cc
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
.L_ja_0200d9cc:
	ldr r0, .L_ja_0200da88
	bl Func_02000908
	movs r0, #141
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d9ec
	movs r1, #172
	movs r2, #174
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02005e6c
.L_ja_0200d9ec:
	ldr r0, .L_ja_0200da8c
	bl Func_02005f6c
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200da36
	movs r3, #3
	str r3, [sp, #4]
	movs r0, #39
	movs r1, #82
	movs r2, #38
	movs r3, #82
	str r6, [sp, #0]
	bl Func_02005d9c
	movs r3, #82
	movs r5, #38
	str r3, [sp, #4]
	movs r0, #39
	movs r1, #82
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005dac
	movs r3, #18
	str r3, [sp, #4]
	movs r0, #38
	movs r1, #19
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005dac
.L_ja_0200da36:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200da46
	b .L_ja_0200dc7c
.L_ja_0200da46:
	movs r3, #3
	str r3, [sp, #4]
	movs r0, #39
	movs r1, #82
	movs r2, #38
	movs r3, #99
	str r6, [sp, #0]
	bl Func_02005d9c
	movs r3, #99
	movs r5, #38
	str r3, [sp, #4]
	movs r0, #38
	movs r1, #98
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005dac
	movs r3, #37
	str r3, [sp, #4]
	movs r0, #38
	movs r1, #35
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005dac
	b .L_ja_0200dc7c
.L_ja_0200da80:
	.4byte 0xffe00000
.L_ja_0200da84:
	.4byte 0x0000005b
.L_ja_0200da88:
	.4byte gData_0200621c
.L_ja_0200da8c:
	.4byte gData_02006068
.L_ja_0200da90:
	ldr r3, .L_ja_0200dae0
	cmp r2, r3
	beq .L_ja_0200da98
	b .L_ja_0200dbaa
.L_ja_0200da98:
	movs r0, #245
	bl Func_02005f4c
	ldr r3, .L_ja_0200dad8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_ja_0200dadc
	subs r2, #2
	movs r1, #144
	strh r3, [r2]
	ldr r0, .L_ja_0200dae4
	lsls r1, r1, #3
	bl Task_Add
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #209
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200dae8
	movs r1, #140
	movs r2, #236
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02005e6c
	b .L_ja_0200dae8
	.2byte 0x0000
.L_ja_0200dad8:
	.4byte 0x00000c08
.L_ja_0200dadc:
	.4byte 0x00003f10
.L_ja_0200dae0:
	.4byte 0x0000005c
.L_ja_0200dae4:
	.4byte Func_02001690
.L_ja_0200dae8:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #210
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200db04
	movs r1, #180
	movs r2, #210
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02005e6c
.L_ja_0200db04:
	movs r0, #153
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200db28
	movs r1, #184
	movs r2, #154
	movs r0, #12
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02005e6c
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_ja_0200db28:
	movs r1, #2
	movs r0, #9
	bl Actor_SetSpritePriority
	ldr r0, .L_ja_0200dc84
	bl Func_02005f6c
	movs r0, #8
	bl Actor_GetById
	ldr r5, [r0, #8]
	movs r0, #8
	bl Actor_GetById
	ldr r2, [r0, #16]
	asrs r5, r5, #20
	adds r1, r5, #0
	asrs r2, r2, #20
	movs r3, #0
	movs r0, #2
	bl Func_0200062c
	movs r0, #9
	bl Actor_GetById
	ldr r5, [r0, #8]
	movs r0, #9
	bl Actor_GetById
	ldr r2, [r0, #16]
	asrs r5, r5, #20
	asrs r2, r2, #20
	movs r0, #2
	adds r1, r5, #0
	movs r3, #0
	bl Func_0200062c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #153
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200db8e
	movs r0, #11
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	b .L_ja_0200db9c
.L_ja_0200db8e:
	movs r0, #11
	bl Actor_GetById
	movs r1, #8
	movs r2, #0
	bl Func_02001084
.L_ja_0200db9c:
	movs r0, #11
.L_ja_0200db9e:
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	b .L_ja_0200dc7c
.L_ja_0200dbaa:
	ldr r3, .L_ja_0200dc88
	cmp r2, r3
	bne .L_ja_0200dc7c
	bl Func_02002770
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #211
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200dbd0
	movs r1, #180
	movs r2, #132
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02005e6c
.L_ja_0200dbd0:
	ldr r0, .L_ja_0200dc8c
	bl Func_02005f6c
	movs r0, #10
	bl Actor_GetById
	ldr r5, [r0, #8]
	movs r0, #10
	bl Actor_GetById
	ldr r2, [r0, #16]
	asrs r5, r5, #20
	movs r3, #0
	asrs r2, r2, #20
	adds r1, r5, #0
	movs r0, #2
	bl Func_0200062c
	movs r0, #10
	bl Actor_GetById
	movs r3, #0
	adds r0, #90
	strb r3, [r0]
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #153
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200dc1c
	movs r0, #10
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	b .L_ja_0200dc2a
.L_ja_0200dc1c:
	movs r0, #10
	bl Actor_GetById
	movs r1, #60
	movs r2, #0
	bl Func_02001084
.L_ja_0200dc2a:
	movs r0, #10
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200dc7c
	movs r3, #2
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #15
	movs r1, #80
	movs r2, #15
	movs r3, #77
	bl Func_02005d9c
	movs r3, #79
	movs r5, #15
	str r3, [sp, #4]
	movs r0, #15
	movs r1, #80
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005dac
	movs r0, #17
	movs r1, #15
	movs r2, #2
	movs r3, #1
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005dac
.L_ja_0200dc7c:
	movs r0, #0
	sub sp, #-8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_ja_0200dc84:
	.4byte gData_02006072
.L_ja_0200dc88:
	.4byte 0x0000005d
.L_ja_0200dc8c:
	.4byte gData_0200607e
	.endif
	.section .bss,"aw",%nobits
	.global gData_020073dc
gData_020073dc:
	.space 0x00000004
	.global gData_020073e0
gData_020073e0:
	.space 0x00000004
	.global gData_020073e4
gData_020073e4:
	.space 0x00000004
	.global gData_020073e8
gData_020073e8:
