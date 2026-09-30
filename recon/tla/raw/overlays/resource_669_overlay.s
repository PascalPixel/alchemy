.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_020033fc
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
	.4byte Data_0200342c
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, .L_02008050
	bx lr
.L_02008050:
	.4byte Data_0200345c
	.global Data_02000054
Data_02000054:
	.4byte 0x00004770
	.section .text.x02008058,"ax",%progbits
	.global Func_02000058
	.thumb_func
Func_02000058:
	push {r5, r6, lr}
	adds r0, r1, #0
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r6, r3, #20
	ldr r3, [r0, #16]
	asrs r5, r3, #20
	cmp r6, #24
	bne .L_02008078
	cmp r5, #40
	bne .L_02008078
	movs r0, #144
	lsls r0, r0, #4
	bl GameFlag_SetBit
.L_02008078:
	cmp r6, #23
	bne .L_02008088
	cmp r5, #40
	bne .L_02008088
	movs r0, #144
	lsls r0, r0, #4
	bl GameFlag_ClearBit
.L_02008088:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200808c,"ax",%progbits
	.global Func_0200008c
	.thumb_func
Func_0200008c:
	push {r5, r6, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #1
	bl GameFlag_SetBit
	ldr r5, .L_02008124
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r6, #192
	lsls r6, r6, #11
	str r6, [r0, #40]
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r2, #244
	ldr r0, [r5]
	movs r1, #136
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #236
	movs r0, #15
	movs r1, #120
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPosition
	movs r2, #220
	adds r2, #255
	movs r1, #136
	movs r0, #14
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #15
	bl Object_GetById
	str r6, [r0, #40]
	movs r0, #156
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020032a8
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	ldr r0, [r5]
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #15
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #159
	bl Func_020032a8
	pop {r5, r6, pc}
.L_02008124:
	.4byte gPartyState
	.section .text.x02008128,"ax",%progbits
	.global Func_02000128
	.thumb_func
Func_02000128:
	push {lr}
	movs r2, #144
	movs r1, #202
	lsls r2, r2, #4
	adds r1, #255
	adds r2, #2
	bl Func_02003148
	pop {pc}
	.2byte 0x0000
	.section .text.x0200813c,"ax",%progbits
	.global Func_0200013c
	.thumb_func
Func_0200013c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	movs r0, #12
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02003130
	movs r0, #0
	bl Func_02003240
	ldr r0, .L_02008244
	bl Func_020031c8
	movs r0, #11
	movs r1, #0
	bl Func_020031e0
	movs r1, #144
	lsls r1, r1, #5
	adds r1, #16
	movs r0, #8
	bl Func_02003268
	bl Func_02003278
	mov r0, r8
	bl Object_GetById
	movs r1, #2
	bl Func_02003290
	movs r0, #201
	bl Func_020032a8
	adds r7, r6, #0
	movs r0, #30
	bl Battle_WaitMode0
	adds r7, #85
	movs r3, #4
	strb r3, [r7]
	movs r5, #39
.L_02008196:
	ldr r3, [r6, #12]
	movs r2, #204
	lsls r2, r2, #6
	adds r2, #51
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r0, #1
	subs r5, #1
	bl Battle_WaitMode0
	cmp r5, #0
	bge .L_02008196
	movs r0, #70
	bl Battle_WaitMode0
	mov r0, r8
	bl Object_GetById
	movs r1, #0
	bl Func_02003290
	bl Func_02003288
	bl Func_02003280
	movs r0, #8
	bl Field_BeginPaletteTransition
	movs r3, #3
	strb r3, [r7]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r6, #72]
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #132
	bl Func_020032a8
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #174
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200823a
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #174
	bl GameFlag_SetBit
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #175
	lsls r1, r1, #8
	movs r0, #10
	adds r1, #255
	movs r2, #0
	bl ObjectMotion_ArmCallback
.L_0200823a:
	bl Func_02003138
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008244:
	.4byte 0x00001c10
	.section .text.x02008248,"ax",%progbits
	.global Func_02000248
	.thumb_func
Func_02000248:
	ldr r0, .L_0200824c
	bx lr
.L_0200824c:
	.4byte Data_02003654
	.section .text.x02008250,"ax",%progbits
	.global Func_02000250
	.thumb_func
Func_02000250:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r5, .L_020083dc
	adds r2, #93
	str r2, [r3]
	movs r3, #241
	lsls r3, r3, #1
	adds r6, r5, r3
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #99
	bne .L_02008296
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #170
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_SetBit
	movs r3, #245
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #3
	strh r3, [r2]
	strh r3, [r6]
	bl Func_02000c5c
.L_02008296:
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020082c6
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02003198
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02003198
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02003198
	movs r0, #9
	b .L_02008302
.L_020082c6:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200830c
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02003198
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02003198
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #189
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008320
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02003198
	movs r0, #11
.L_02008302:
	movs r1, #0
	movs r2, #0
	bl Func_02003198
	b .L_02008320
.L_0200830c:
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02003198
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02003198
.L_02008320:
	movs r0, #144
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200833a
	movs r1, #196
	movs r2, #162
	movs r0, #13
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02003198
.L_0200833a:
	movs r0, #13
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	movs r1, #202
	strb r3, [r0]
	adds r1, #255
	movs r0, #15
	bl Func_02003298
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020083a2
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020083d6
	movs r0, #14
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #128
	orrs r3, r2
	strb r3, [r0]
	movs r0, #15
	bl Object_GetById
	adds r1, r0, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #64
	orrs r3, r2
	strb r3, [r1]
	movs r2, #128
	ldr r3, [r0, #12]
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r3, [r0, #20]
	adds r3, r3, r2
	str r3, [r0, #20]
	b .L_020083d6
.L_020083a2:
	movs r1, #136
	movs r0, #14
	lsls r1, r1, #16
	ldr r2, .L_020083e0
	bl Func_02003198
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020083cc
	movs r1, #240
	movs r2, #236
	movs r0, #15
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Func_02003198
	b .L_020083d6
.L_020083cc:
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02003198
.L_020083d6:
	movs r0, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020083dc:
	.4byte gPartyState
.L_020083e0:
	.4byte 0x01db0000
	.section .text.x020083e4,"ax",%progbits
	.global Func_020003e4
	.thumb_func
Func_020003e4:
	movs r0, #0
	bx lr
	.section .text.x020083e8,"ax",%progbits
	.global Func_020003e8
	.thumb_func
Func_020003e8:
	push {r5, r6, r7, lr}
	adds r4, r0, #0
	adds r6, r2, #0
	adds r5, r1, #0
	lsls r3, r3, #16
	movs r0, #244
	asrs r7, r3, #16
	lsls r0, r0, #1
	adds r3, r6, #0
	adds r1, r4, #0
	adds r2, r5, #0
	bl Func_02003110
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200842c
	movs r1, #1
	ldr r5, [r6, #80]
	bl Func_02003100
	ldr r1, .L_02008434
	adds r0, r6, #0
	bl Func_02003108
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r3, [sp, #16]
	ldr r1, .L_02008430
	adds r2, #9
	strh r3, [r2]
	strb r1, [r5, #26]
	strh r7, [r5, #18]
.L_0200842c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008430:
	.4byte 0x00000000
.L_02008434:
	.4byte Data_02003720
	.section .text.x02008438,"ax",%progbits
	.global Func_02000438
	.thumb_func
Func_02000438:
	push {r5, r6, lr}
	mov r6, r11
	mov r5, r10
	push {r5, r6}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	sub sp, #4
	bl Object_GetById
	movs r3, #128
	adds r5, r0, #0
	lsls r3, r3, #12
	ldr r0, [r5, #8]
	mov r10, r3
	ldr r1, [r5, #12]
	movs r3, #224
	lsls r3, r3, #13
	mov r8, r3
	movs r3, #128
	ldr r2, [r5, #16]
	add r1, r8
	lsls r3, r3, #5
	movs r6, #15
	add r0, r10
	str r6, [sp, #0]
	mov r11, r3
	bl Func_020003e8
	movs r0, #151
	bl Func_020032a8
	movs r0, #15
	bl Battle_WaitMode0
	ldr r0, [r5, #8]
	ldr r3, .L_020084f0
	ldr r1, [r5, #12]
	adds r0, r0, r3
	movs r3, #240
	ldr r2, [r5, #16]
	add r1, r8
	lsls r3, r3, #8
	str r6, [sp, #0]
	mov r9, r3
	bl Func_020003e8
	movs r0, #151
	bl Func_020032a8
	movs r0, #15
	bl Battle_WaitMode0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	ldr r2, [r5, #16]
	add r1, r8
	mov r3, r11
	add r0, r10
	str r6, [sp, #0]
	bl Func_020003e8
	movs r0, #151
	bl Func_020032a8
	movs r0, #15
	bl Battle_WaitMode0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	ldr r3, .L_020084f0
	ldr r2, [r5, #16]
	add r1, r8
	adds r0, r0, r3
	mov r3, r9
	str r6, [sp, #0]
	bl Func_020003e8
	movs r0, #151
	bl Func_020032a8
	movs r0, #15
	bl Battle_WaitMode0
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r3}
	mov r11, r3
	pop {r5, r6, pc}
.L_020084f0:
	.4byte 0xfff80000
	.section .text.x020084f4,"ax",%progbits
	.global Func_020004f4
	.thumb_func
Func_020004f4:
	push {r5, lr}
	bl Func_02003130
	movs r0, #0
	bl Func_02003240
	ldr r0, .L_020088fc
	bl Func_020031c8
	movs r5, #192
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	lsls r5, r5, #18
	bl Motion_CamBounds
	ldr r0, [r5, #32]
	movs r1, #0
	adds r3, r0, #0
	adds r3, #236
	movs r2, #128
	str r1, [r3]
	lsls r2, r2, #19
	adds r3, #8
	str r2, [r3]
	subs r3, #4
	str r1, [r3]
	adds r3, #8
	str r2, [r3]
	movs r0, #8
	movs r2, #10
	bl Func_020031d8
	movs r1, #228
	movs r2, #204
	movs r0, #4
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #16
	movs r3, #128
	movs r0, #25
	movs r1, #0
	negs r2, r2
	lsls r3, r3, #7
	bl Func_02003248
	movs r3, #128
	movs r0, #5
	movs r1, #16
	movs r2, #0
	lsls r3, r3, #7
	bl Func_02003248
	movs r2, #16
	movs r3, #128
	lsls r3, r3, #7
	movs r1, #16
	negs r2, r2
	movs r0, #6
	bl Func_02003248
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #188
	movs r1, #1
	movs r2, #208
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02003218
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r0, #9
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #8
	bl Func_020031f8
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #8
	bl Func_020031f8
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #9
	bl Func_020031f8
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #9
	bl Func_020031f8
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #2
	adds r1, #255
	movs r2, #20
	movs r0, #9
	bl Func_020031f8
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #8
	bl Func_020031f8
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl Func_020031d8
	movs r0, #9
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02003200
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #9
	bl Func_02000438
	movs r0, #151
	bl Func_020032a8
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #9
	bl Func_020031f8
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_020031f8
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #5
	bl Func_020031f8
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	movs r0, #25
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r1, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #25
	lsls r1, r1, #1
	bl Func_02003200
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #4
	bl Func_02003200
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #8
	bl Func_020031f8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	b .L_02008900
.L_020088fc:
	.4byte 0x00001b67
.L_02008900:
	movs r0, #9
	bl Func_020031f8
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #192
	movs r2, #192
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #192
	movs r2, #192
	lsls r2, r2, #8
	movs r0, #9
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_02008c4c
	movs r0, #9
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #5
	bl Battle_WaitMode0
	ldr r1, .L_02008c50
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #8
	bl Object_RefreshSelectorById
	movs r1, #128
	movs r0, #25
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #224
	movs r1, #1
	movs r2, #208
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02003218
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_020031f8
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #8
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #45
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r1, #0
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #25
	bl Func_02003200
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #25
	bl Func_020031f8
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	movs r0, #25
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #25
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #4
	adds r1, #255
	movs r2, #50
	movs r0, #8
	bl Func_020031f8
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #50
	movs r0, #25
	bl Func_020031f8
	movs r0, #128
	lsls r0, r0, #5
	movs r1, #0
	adds r0, #25
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008b8e
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl Func_020031d8
	ldr r2, [r5, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008bc2
.L_02008b8e:
	bl Func_020032a0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	ldr r2, [r5, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #5
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
.L_02008bc2:
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #9
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r2, #16
	movs r0, #8
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	negs r2, r2
	movs r1, #0
	movs r0, #9
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, .L_02008c54
	movs r1, #99
	bl Party_SetFields1eeAnd1f0
	ldr r3, .L_02008c58
	movs r2, #166
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	movs r0, #10
	movs r1, #1
	bl Func_02003220
	bl Func_02003138
	pop {r5, pc}
	.2byte 0x0000
.L_02008c4c:
	.4byte Data_0200333c
.L_02008c50:
	.4byte Data_020032b0
.L_02008c54:
	.4byte 0x00000063
.L_02008c58:
	.4byte gPartyState
	.section .text.x02008c5c,"ax",%progbits
	.global Func_02000c5c
	.thumb_func
Func_02000c5c:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	sub sp, #28
	bl Func_02003130
	movs r0, #0
	bl Func_02003240
	ldr r0, .L_02009008
	bl Func_020031c8
	movs r1, #218
	movs r2, #208
	lsls r2, r2, #17
	movs r0, #8
	lsls r1, r1, #17
	bl Func_02003198
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #8
	bl Func_02003260
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #228
	movs r2, #216
	lsls r2, r2, #17
	movs r0, #9
	lsls r1, r1, #17
	bl Func_02003198
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #9
	bl Func_02003260
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #9
	bl Object_GetById
	movs r5, #128
	lsls r5, r5, #7
	movs r1, #228
	movs r2, #230
	lsls r2, r2, #17
	strh r5, [r0, #6]
	lsls r1, r1, #17
	movs r0, #22
	bl Func_02003198
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #22
	bl Func_02003260
	movs r0, #22
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #210
	movs r2, #212
	lsls r2, r2, #17
	movs r0, #23
	lsls r1, r1, #17
	bl Func_02003198
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #23
	bl Func_02003260
	movs r0, #23
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #194
	movs r2, #220
	lsls r2, r2, #17
	movs r0, #24
	lsls r1, r1, #17
	bl Func_02003198
	adds r1, r5, #0
	movs r0, #24
	bl Func_02003260
	movs r0, #24
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #228
	movs r2, #188
	movs r0, #25
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02003198
	movs r1, #236
	movs r2, #196
	movs r0, #5
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02003198
	movs r1, #236
	movs r2, #188
	movs r0, #6
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02003198
	movs r1, #228
	movs r2, #138
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02003198
	movs r1, #236
	movs r2, #140
	movs r0, #21
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02003198
	movs r1, #220
	movs r2, #140
	movs r0, #20
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02003198
	movs r1, #228
	movs r2, #148
	movs r0, #19
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02003198
	movs r1, #236
	movs r2, #148
	movs r0, #17
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02003198
	movs r1, #220
	movs r2, #148
	movs r0, #18
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02003198
	movs r1, #228
	movs r2, #152
	movs r0, #16
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02003198
	movs r1, #128
	movs r2, #128
	movs r0, #21
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #20
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #19
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #17
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #18
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #16
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #25
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #6
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #10
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #22
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #23
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #24
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #232
	movs r1, #1
	movs r2, #204
	movs r3, #0
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #6
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #25
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #191
	lsls r1, r1, #8
	movs r0, #4
	adds r1, #255
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	adds r1, #1
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #50
	bl Battle_WaitMode0
	movs r0, #4
	adds r1, r5, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	adds r1, r5, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #8
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #8
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200900c
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #10
	movs r0, #6
	bl Func_020031d8
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200903c
	.2byte 0x0000
.L_02009008:
	.4byte 0x00001b8b
.L_0200900c:
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
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
	movs r2, #10
	bl Func_020031d8
.L_0200903c:
	movs r1, #2
	movs r0, #25
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #25
	bl Func_020031f8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_020031f8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_020031f8
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #4
	bl Func_020031f8
	movs r0, #10
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02003218
	movs r1, #228
	movs r2, #252
	movs r0, #10
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #236
	movs r2, #212
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #10
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #232
	movs r1, #1
	movs r2, #204
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02003218
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #25
	bl Func_020031f8
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #25
	movs r1, #0
	bl Func_020031d8
	movs r0, #10
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02003200
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_020031f8
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_020031f8
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_020031d8
	movs r0, #25
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #6
	movs r1, #0
	bl Func_020031d8
	movs r0, #10
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	movs r1, #0
	movs r2, #15
	bl Func_020031d8
	movs r1, #191
	lsls r1, r1, #8
	movs r0, #4
	adds r1, #255
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	adds r1, #1
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #8
	movs r1, #0
	bl Func_020031d8
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #25
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #8
	adds r1, #255
	movs r2, #0
	movs r0, #8
	bl Func_020031f8
	movs r1, #0
	mov r9, r1
	str r1, [r0, #108]
	ldr r2, [r0, #12]
	ldr r1, [r0, #8]
	movs r6, #176
	movs r5, #128
	lsls r6, r6, #13
	lsls r5, r5, #12
	ldr r3, [r0, #16]
	adds r1, r1, r6
	adds r2, r2, r5
	bl Object_SetPositionAndResetMotion
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_020031f8
	movs r0, #6
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #0
	movs r2, #0
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #208
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #5
	bl Func_020031f8
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #25
	bl Func_020031f8
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_020031f8
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_020031d8
	movs r0, #8
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #8
	movs r1, #0
	movs r2, #20
	bl Func_020031d8
	movs r0, #16
	movs r1, #0
	movs r2, #8
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #17
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #8
	bl Func_020031f8
	mov r2, r9
	str r2, [r0, #108]
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	adds r1, r1, r6
	adds r2, r2, r5
	bl Object_SetPositionAndResetMotion
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #10
	bl Func_020031f8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_020031f8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_020031f8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_020031f8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #25
	bl Func_020031f8
	movs r1, #192
	movs r0, #4
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
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #232
	movs r1, #1
	movs r2, #152
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02003218
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #16
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #16
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #16
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #18
	bl Func_020031f8
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #16
	bl Func_020031f8
	movs r0, #16
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #17
	bl Func_020031f8
	movs r0, #17
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #19
	bl Func_020031f8
	movs r0, #19
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #16
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #16
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #19
	bl Func_020031f8
	movs r2, #10
	movs r0, #19
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #16
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #16
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #19
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #16
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #16
	movs r1, #0
	bl Func_020031d8
	movs r0, #19
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #2
	movs r0, #18
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #16
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #16
	bl Func_020031f8
	movs r0, #16
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #18
	bl Func_020031f8
	movs r1, #2
	adds r1, #255
	movs r2, #60
	movs r0, #19
	bl Func_020031f8
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #17
	bl Func_020031f8
	movs r0, #17
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #16
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #16
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #18
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #16
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #16
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #19
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #19
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #16
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #16
	bl Func_020031f8
	movs r2, #10
	movs r0, #16
	movs r1, #0
	bl Func_020031d8
	movs r0, #18
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #18
	bl Func_02003200
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #16
	bl Func_020031f8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #17
	bl Func_020031f8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #18
	bl Func_020031f8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #19
	bl Func_020031f8
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #18
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #19
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #232
	movs r1, #1
	movs r2, #172
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02003218
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #25
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #0
	movs r2, #16
	movs r0, #17
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r0, #16
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #18
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #1
	movs r0, #16
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #11
	mov r8, r1
	movs r3, #17
	movs r1, #6
	str r3, [sp, #8]
	str r1, [sp, #20]
	movs r2, #22
	mov r10, r1
	mov r3, r8
	mov r1, r9
	str r3, [sp, #12]
	str r2, [sp, #16]
	str r1, [sp, #24]
	movs r3, #13
	movs r2, #1
	movs r5, #4
	movs r6, #16
	movs r0, #18
	movs r1, #13
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_020031f0
	movs r0, #16
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #2
	movs r0, #18
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #18
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #16
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #17
	bl Func_020031f8
	movs r1, #10
	adds r1, #255
	movs r2, #60
	movs r0, #16
	bl Func_020031f8
	movs r1, #132
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #18
	bl Func_020031f8
	movs r0, #16
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #16
	bl Func_020031f8
	movs r1, #8
	adds r1, #255
	movs r2, #70
	movs r0, #18
	bl Func_020031f8
	movs r1, #129
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #16
	bl Func_020031f8
	movs r1, #3
	movs r0, #16
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #16
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #16
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #18
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #19
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #19
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #160
	movs r0, #25
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #252
	lsls r1, r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #252
	lsls r1, r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #25
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #6
	movs r1, #0
	bl Func_020031d8
	movs r0, #16
	movs r1, #3
	bl Object_SetModeById
	movs r0, #17
	movs r1, #3
	bl Object_SetModeById
	movs r0, #18
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #19
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #25
	movs r1, #0
	bl Func_020031d8
	movs r0, #16
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #2
	movs r0, #17
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #9
	movs r2, #14
	str r3, [sp, #8]
	str r2, [sp, #16]
	mov r3, r8
	mov r1, r10
	mov r2, r9
	str r3, [sp, #12]
	str r1, [sp, #20]
	str r2, [sp, #24]
	movs r3, #25
	movs r2, #1
	movs r0, #17
	movs r1, #12
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_020031f0
	movs r0, #17
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #17
	lsls r1, r1, #1
	bl Func_02003200
	movs r0, #16
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #16
	bl Func_02003200
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #224
	movs r0, #16
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #17
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #16
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #16
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #17
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #17
	bl Func_020031f8
	movs r0, #17
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r0, #232
	movs r1, #1
	movs r2, #216
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02003218
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #10
	bl Func_020031f8
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #8
	bl Func_020031f8
	movs r3, #0
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	str r3, [r0, #108]
	movs r5, #128
	movs r3, #176
	lsls r5, r5, #12
	lsls r3, r3, #13
	adds r1, r1, r3
	adds r2, r2, r5
	ldr r3, [r0, #16]
	bl Object_SetPositionAndResetMotion
	movs r0, #50
	bl Battle_WaitMode0
	movs r0, #232
	movs r1, #1
	movs r2, #172
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02003218
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #16
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #16
	movs r1, #0
	bl Func_020031d8
	movs r0, #16
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #16
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r0, #18
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #18
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r0, #232
	movs r1, #1
	movs r2, #212
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02003218
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	movs r0, #25
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	movs r1, #0
	adds r0, #8
	bl Func_020031d8
	movs r0, #8
	bl Object_GetById
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r3, #160
	lsls r3, r3, #13
	lsls r1, r1, #16
	lsls r2, r2, #16
	adds r1, r1, r3
	adds r2, r2, r5
	movs r0, #8
	bl Func_02003198
	movs r1, #0
	movs r0, #8
	bl Func_02003260
	movs r0, #8
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #40
	movs r0, #8
	movs r1, #6
	bl ObjectMotion_Launch
	movs r0, #8
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #228
	movs r2, #220
	lsls r2, r2, #17
	movs r0, #9
	lsls r1, r1, #17
	bl Func_02003198
	movs r1, #0
	movs r0, #9
	bl Func_02003260
	movs r0, #9
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #228
	movs r2, #228
	lsls r2, r2, #17
	movs r0, #22
	lsls r1, r1, #17
	bl Func_02003198
	movs r1, #0
	movs r0, #22
	bl Func_02003260
	movs r0, #22
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #212
	movs r2, #212
	lsls r2, r2, #17
	movs r0, #23
	lsls r1, r1, #17
	bl Func_02003198
	movs r1, #0
	movs r0, #23
	bl Func_02003260
	movs r0, #23
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #204
	movs r2, #220
	lsls r2, r2, #17
	movs r0, #24
	lsls r1, r1, #17
	bl Func_02003198
	movs r1, #0
	movs r0, #24
	bl Func_02003260
	movs r0, #24
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #23
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #22
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #23
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #24
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #40
	movs r0, #9
	movs r1, #6
	bl ObjectMotion_Launch
	movs r0, #22
	movs r1, #4
	bl Object_SetModeById
	movs r0, #23
	movs r1, #4
	bl Object_SetModeById
	movs r0, #24
	movs r1, #4
	bl Object_SetModeById
	movs r1, #4
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #232
	movs r1, #1
	movs r2, #184
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02003218
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #17
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #17
	movs r1, #0
	bl Func_020031d8
	movs r0, #8
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #8
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #18
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r0, #232
	movs r1, #1
	movs r2, #200
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02003218
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #6
	movs r2, #40
	adds r1, #255
	movs r0, #8
	bl Func_020031f8
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #8
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #130
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #8
	bl Func_020031f8
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #8
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #24
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #16
	movs r0, #8
	movs r1, #16
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #176
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #32
	movs r0, #9
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #32
	movs r0, #22
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #23
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #24
	movs r1, #16
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #16
	movs r2, #32
	movs r0, #5
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #16
	movs r2, #32
	negs r2, r2
	negs r1, r1
	movs r0, #6
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #1
	bl Object_SetModeById
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #9
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #22
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #23
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #24
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #8
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #232
	movs r1, #1
	movs r2, #192
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02003218
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #23
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl Func_020031d8
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #4
	bl Func_02003200
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #16
	movs r0, #9
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #22
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #23
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #24
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #8
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #16
	movs r0, #9
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #22
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #23
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #24
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #8
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #16
	movs r0, #9
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #22
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #23
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #24
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #8
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #232
	movs r1, #1
	movs r2, #168
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	bl Func_02003218
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
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
	lsls r1, r1, #8
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #17
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #17
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #22
	bl Func_020031f8
	movs r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_020031f8
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #22
	bl Func_020031f8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #23
	bl Func_020031f8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #24
	bl Func_020031f8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #9
	bl Func_020031f8
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #232
	movs r1, #1
	movs r2, #208
	lsls r2, r2, #17
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02003218
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02003200
	movs r0, #50
	bl Battle_WaitMode0
	movs r0, #232
	movs r1, #1
	movs r2, #168
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02003218
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #17
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #228
	movs r2, #156
	strb r3, [r0]
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #17
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #17
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #16
	movs r0, #9
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #22
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #23
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #24
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #8
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #0
	movs r1, #0
	movs r0, #16
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #16
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #16
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #16
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #16
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #0
	movs r1, #0
	movs r0, #16
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #32
	movs r0, #9
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #32
	movs r0, #22
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #32
	movs r0, #23
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #32
	movs r0, #24
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #32
	movs r0, #8
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #232
	movs r1, #1
	movs r2, #148
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02003218
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #19
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #19
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_020031f8
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #18
	movs r1, #0
	bl Func_020031d8
	movs r0, #19
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #19
	bl Func_02003200
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #19
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #19
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #18
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #19
	bl Func_020031f8
	movs r0, #19
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #17
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #17
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #17
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #19
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #19
	movs r1, #0
	bl Func_020031d8
	movs r0, #17
	movs r1, #3
	bl Object_SetModeById
	movs r0, #18
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #19
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #32
	movs r0, #19
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #16
	movs r0, #19
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #19
	bl Func_02003198
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	movs r1, #16
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #0
	movs r1, #0
	movs r0, #20
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #20
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #20
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	ldr r5, .L_0200aa3c
	movs r0, #21
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #9
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #22
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #23
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #24
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #75
	bl Battle_WaitMode0
	movs r0, #20
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #32
	movs r0, #20
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #32
	movs r0, #20
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #20
	bl Func_02003198
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #18
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #17
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #16
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #16
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #16
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #16
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #16
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #32
	movs r0, #16
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #18
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #18
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #18
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #16
	bl Func_020031f8
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #16
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #16
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r0, #232
	movs r1, #1
	movs r2, #160
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02003218
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #25
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #18
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #4
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #25
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #32
	movs r0, #4
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #32
	movs r0, #25
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #16
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #16
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #16
	bl Func_020031f8
	movs r0, #16
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #2
	movs r2, #50
	adds r1, #255
	movs r0, #25
	bl Func_020031f8
	movs r1, #3
	movs r0, #16
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #16
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #16
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #16
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #0
	movs r1, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #32
	movs r0, #16
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #32
	movs r0, #18
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #32
	movs r0, #16
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #16
	movs r0, #18
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #18
	bl Func_02003198
	movs r0, #16
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #16
	bl Func_02003198
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #17
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #17
	movs r1, #0
	bl Func_020031d8
	movs r0, #25
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #17
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	adds r1, #1
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #32
	movs r2, #8
	negs r1, r1
	negs r2, r2
	movs r0, #17
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #17
	b .L_0200aa40
.L_0200aa3c:
	.4byte Data_020033c8
.L_0200aa40:
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #16
	movs r1, #24
	negs r2, r2
	movs r0, #17
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #16
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #17
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #17
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #17
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #25
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #25
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200ab7a
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r2, #10
	movs r0, #17
	bl Func_020031d8
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200abc0
.L_0200ab7a:
	movs r0, #40
	bl Battle_WaitMode0
	bl Func_020032a0
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #17
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
.L_0200abc0:
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #17
	bl Func_020031f8
	movs r1, #0
	movs r2, #10
	movs r0, #17
	bl Func_020031d8
	movs r0, #17
	bl Func_02000438
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #17
	bl Func_020031f8
	movs r2, #10
	movs r0, #17
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #17
	bl Func_020031f8
	movs r2, #10
	movs r0, #17
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #40
	movs r0, #17
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #24
	movs r0, #17
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #17
	bl Func_02003198
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #232
	movs r1, #1
	movs r2, #200
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02003218
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_020031f8
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	movs r0, #25
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #10
	bl Func_020031f8
	movs r0, #10
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02003200
	movs r0, #40
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_020031d8
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_020031d8
	movs r0, #25
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #236
	movs r2, #188
	lsls r2, r2, #1
	movs r0, #10
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #16
	movs r2, #32
	movs r0, #25
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #16
	movs r2, #32
	negs r2, r2
	movs r0, #4
	negs r1, r1
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #25
	movs r1, #1
	bl Object_SetModeById
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #236
	movs r2, #160
	movs r0, #10
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #32
	movs r0, #5
	movs r1, #16
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #32
	movs r0, #6
	movs r1, #16
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #236
	movs r2, #132
	movs r0, #10
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Motion_CamBounds
	movs r1, #176
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #220
	movs r2, #132
	movs r0, #10
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl Func_02003198
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #6
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl Func_020031d8
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r1, #0
	movs r2, #10
	adds r0, #25
	bl Func_020031d8
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #25
	bl Func_020031f8
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_020031d8
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #6
	bl UiText_OpenMessageAtObject
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200afbe
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #10
	movs r0, #5
	bl Func_020031d8
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200afe8
.L_0200afbe:
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #4
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
	movs r2, #10
	bl Func_020031d8
.L_0200afe8:
	movs r0, #25
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_0200b0e0
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_0200b0e4
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r3, r1
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200b03c
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_0200b03c:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02003198
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_0200b0e0
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200b07a
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_0200b07a:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02003198
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #25
	ldr r1, .L_0200b0e0
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #25
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200b0b8
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #25
	bl ObjectMotion_ResetAndSetPosition
.L_0200b0b8:
	movs r0, #25
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #0
	movs r0, #25
	movs r1, #0
	bl Func_02003198
	ldr r0, [r5]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02003138
	add sp, #28
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_0200b0e0:
	.4byte 0x00013333
.L_0200b0e4:
	.4byte gPartyState
	.section .rodata.x0200b2b0,"a",%progbits
	.global Data_020032b0
Data_020032b0:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00200000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x001c0000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x000c0000
	.4byte 0x00000000
	.4byte 0x000c0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_0200333c
Data_0200333c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00200000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x000c0000
	.4byte 0x00000000
	.4byte 0xfff40000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00340000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x000c0000
	.4byte 0x00000000
	.4byte 0x000c0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020033c8
Data_020033c8:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_020033fc
Data_020033fc:
	.4byte 0xffff0000
	.4byte 0x000001c8
	.4byte 0x40000188
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200342c
Data_0200342c:
	.4byte 0x00000063
	.4byte 0x10102062
	.4byte 0xffffffff
	.4byte 0x10203063
	.4byte 0xffffffff
	.4byte 0x10302063
	.4byte 0xffffffff
	.4byte 0x10405063
	.4byte 0xffffffff
	.4byte 0x10504063
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_0200345c
Data_0200345c:
	.4byte 0xffff0010
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x018c0000
	.4byte 0x00014000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0001c000
	.4byte 0xffff0025
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x0001a000
	.4byte 0xffff0026
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00028000
	.4byte 0xffff01d2
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00014000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00024000
	.4byte 0xffff0120
	.4byte 0x00000001
	.4byte 0x00950000
	.4byte 0x00000000
	.4byte 0x01db0000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00950000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00024000
	.4byte 0xffff0018
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0013
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0058
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003654
Data_02003654:
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
	.4byte 0x00000002
	.4byte Field_Map006 + 0x48
	.4byte Func_020004f4
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_0200013c
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001c12
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001c13
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001c14
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_02000128
	.4byte 0x10008c15
	.4byte 0xffff000d
	.4byte Data_02000054 + 0x1
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte Func_02000058
	.4byte 0x00000008
	.4byte 0xffff001e
	.4byte Data_02000054 + 0x1
	.4byte 0x00000009
	.4byte 0xffff001e
	.4byte Func_02000058
	.4byte 0x00008715
	.4byte 0x0901000e
	.4byte Func_0200008c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003720
Data_02003720:
	.4byte 0x00000026
