.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080b6,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009b50,"ax",%progbits
	.global Func_02001b50
	.thumb_func
Func_02001b50:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02009c80
	movs r0, #133
	lsls r0, r0, #2
	adds r0, r0, r3
	mov r9, r0
	ldr r0, [r0]
	bl Actor_GetById
	adds r6, r0, #0
	ldr r2, [r6, #8]
	ldr r1, [r6, #16]
	asrs r3, r2, #20
	asrs r0, r1, #20
	mov r10, r3
	ldr r3, [r6, #12]
	mov r8, r0
	movs r0, #128
	lsls r0, r0, #13
	movs r7, #0
	cmp r3, r0
	bgt .L_02009c76
	ldr r0, .L_02009c84
	adds r3, r2, r0
	movs r2, #240
	lsls r2, r2, #16
	cmp r3, r2
	bhi .L_02009c76
	ldr r0, .L_02009c88
	movs r2, #200
	adds r3, r1, r0
	lsls r2, r2, #15
	cmp r3, r2
	bhi .L_02009c76
	mov r3, r10
	cmp r3, #27
	bne .L_02009bda
	mov r0, r8
	cmp r0, #12
	bne .L_02009bda
	movs r0, #13
	bl Actor_GetById
	mov r2, r9
	adds r5, r0, #0
	ldr r0, [r2]
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #13
	movs r1, #4
	bl Actor_SetModeById
	adds r3, r6, #0
	adds r3, #85
	strb r7, [r3]
	movs r0, #130
	ldr r3, [r5, #12]
	lsls r0, r0, #2
	str r3, [r6, #12]
	bl GameFlag_SetBit
	movs r7, #1
.L_02009bda:
	mov r3, r10
	cmp r3, #37
	bne .L_02009c22
	mov r0, r8
	cmp r0, #14
	bne .L_02009c22
	movs r0, #14
	bl Actor_GetById
	ldr r3, .L_02009c80
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #14
	movs r1, #4
	bl Actor_SetModeById
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r0, #133
	ldr r3, [r5, #12]
	lsls r0, r0, #1
	str r3, [r6, #12]
	adds r0, #255
	bl GameFlag_SetBit
	movs r7, #1
.L_02009c22:
	cmp r7, #0
	beq .L_02009c30
	movs r0, #126
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02009c76
.L_02009c30:
	movs r0, #13
	movs r1, #0
	bl Actor_SetModeById
	movs r1, #0
	movs r0, #14
	bl Actor_SetModeById
	ldr r3, .L_02009c80
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Actor_GetById
	movs r1, #1
	bl Actor_SetSingleChildFlags
	adds r2, r6, #0
	adds r2, #85
	movs r3, #3
	movs r0, #130
	strb r3, [r2]
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #126
	adds r0, #255
	bl GameFlag_ClearBit
.L_02009c76:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02009c80:
	.4byte gMainRoutineWork
.L_02009c84:
	.4byte 0xfe700000
.L_02009c88:
	.4byte 0xff4c0000
	.section .text.x0200aa84,"ax",%progbits
	.balign 4
	.ifdef TLA_EDITION_JA
	.global Func_02002a84
	.thumb_func
Func_02002a84:
	push {r5, r6, lr}
	movs r0, #9
	sub sp, #8
	bl Actor_GetById
	adds r6, r0, #0
	movs r0, #9
	bl Func_020052c0
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r0, #9
	movs r1, #2
	bl Actor_SetModeById
	movs r0, #8
	bl Actor_GetById
	adds r6, r0, #0
	adds r3, r6, #0
	movs r0, #192
	movs r5, #0
	adds r3, #85
	lsls r0, r0, #2
	strb r5, [r3]
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200aad8
	movs r0, #8
	bl Actor_GetById
	movs r1, #156
	movs r2, #128
	movs r3, #236
	lsls r1, r1, #17
	lsls r2, r2, #14
	lsls r3, r3, #17
	bl Actor_SetPositionAndResetMotion
	movs r3, #19
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #3
	bl Func_02005178
	b .L_ja_0200ab10
.L_ja_0200aad8:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200ab10
	movs r0, #8
	bl Actor_GetById
	movs r1, #172
	movs r2, #128
	movs r3, #236
	lsls r1, r1, #17
	lsls r2, r2, #14
	lsls r3, r3, #17
	bl Actor_SetPositionAndResetMotion
	movs r3, #19
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #3
	movs r3, #3
	bl Func_02005178
.L_ja_0200ab10:
	ldr r0, .L_ja_0200abe0
	bl Func_020053b8
	movs r0, #10
	bl Actor_GetById
	movs r3, #179
	adds r6, r0, #0
	lsls r3, r3, #8
	adds r3, #51
	adds r2, r6, #0
	str r3, [r6, #24]
	str r3, [r6, #28]
	adds r2, #89
	movs r3, #0
	movs r0, #0
	strb r3, [r2]
	bl Func_02005338
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #45
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200ab76
	movs r3, #1
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #0
	movs r0, #41
	movs r1, #0
	movs r2, #20
	bl Func_02005170
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005230
.L_ja_0200ab76:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200abda
	ldr r2, .L_ja_0200abe4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #2
	bne .L_ja_0200aba6
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r2, r1
	ldr r0, [r3]
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #15
	str r3, [r0, #12]
	b .L_ja_0200abda
.L_ja_0200aba6:
	cmp r3, #3
	bne .L_ja_0200abda
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #45
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200abda
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	bl Func_02005028
.L_ja_0200abda:
	sub sp, #-8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_ja_0200abe0:
	.4byte gData_020054e0
.L_ja_0200abe4:
	.4byte gMainRoutineWork
	.else
	.global Func_02002a84
	.thumb_func
Func_02002a84:
	push {r5, r6, lr}
	movs r0, #9
	sub sp, #8
	bl Actor_GetById
	adds r6, r0, #0
	movs r0, #9
	bl Func_020052c0
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r0, #9
	movs r1, #2
	bl Actor_SetModeById
	movs r0, #8
	bl Actor_GetById
	adds r6, r0, #0
	adds r3, r6, #0
	movs r0, #192
	movs r5, #0
	adds r3, #85
	lsls r0, r0, #2
	strb r5, [r3]
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200aaf4
	movs r0, #8
	bl Actor_GetById
	movs r1, #156
	movs r2, #128
	movs r3, #236
	lsls r1, r1, #17
	lsls r2, r2, #14
	lsls r3, r3, #17
	bl Actor_SetPositionAndResetMotion
	movs r3, #19
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #3
	bl Func_02005178
	b .L_0200ab2c
.L_0200aaf4:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ab2c
	movs r0, #8
	bl Actor_GetById
	movs r1, #172
	movs r2, #128
	movs r3, #236
	lsls r1, r1, #17
	lsls r2, r2, #14
	lsls r3, r3, #17
	bl Actor_SetPositionAndResetMotion
	movs r3, #19
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #3
	movs r3, #3
	bl Func_02005178
.L_0200ab2c:
	ldr r0, .L_0200ac14
	bl Func_020053b8
	movs r0, #10
	bl Actor_GetById
	movs r3, #179
	adds r6, r0, #0
	lsls r3, r3, #8
	adds r3, #51
	adds r2, r6, #0
	str r3, [r6, #24]
	str r3, [r6, #28]
	adds r2, #89
	movs r3, #0
	movs r0, #0
	strb r3, [r2]
	bl Func_02005338
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #45
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ab92
	movs r3, #1
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #0
	movs r0, #41
	movs r1, #0
	movs r2, #20
	bl Func_02005170
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005230
.L_0200ab92:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200abf6
	ldr r2, .L_0200ac18
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #2
	bne .L_0200abc2
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r2, r1
	ldr r0, [r3]
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #15
	str r3, [r0, #12]
	b .L_0200abf6
.L_0200abc2:
	cmp r3, #3
	bne .L_0200abf6
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #45
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200abf6
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	bl Func_02005028
.L_0200abf6:
	ldr r3, .L_0200ac18
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #2
	bne .L_0200ac0e
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
.L_0200ac0e:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200ac14:
	.4byte gData_020054e0
.L_0200ac18:
	.4byte gMainRoutineWork
	.endif
	.section .text.x0200aec2,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200b06c,"ax",%progbits
	.global Func_0200306c
	.thumb_func
Func_0200306c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200b2b8
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Actor_GetById
	adds r6, r0, #0
	movs r0, #9
	bl Actor_GetById
	mov r8, r0
	bl Func_020052f0
	adds r7, r0, #0
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	bl Func_020052f0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	adds r0, #85
	strb r3, [r0]
	strb r3, [r2]
	movs r0, #1
	bl wait
	movs r0, #130
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #162
	bl Func_020053d8
	bl Func_02000d64
	movs r2, #0
	mov r10, r2
.L_0200b0f6:
	ldr r3, [r6, #12]
	movs r2, #144
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r6, #12]
	mov r1, r8
	ldr r3, [r1, #12]
	movs r5, #128
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r3, [r7, #12]
	movs r2, #230
	lsls r2, r2, #9
	adds r2, #204
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #128
	str r3, [r7, #12]
	movs r0, #11
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #8
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #0
	adds r2, r5, #0
	movs r0, #16
	bl Func_02000cd8
	mov r1, r10
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	movs r2, #240
	asrs r1, r3, #16
	lsls r2, r2, #12
	mov r10, r1
	cmp r3, r2
	bls .L_0200b0f6
	bl Func_02005158
	movs r0, #1
	bl wait
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #10
	bl FieldEvent_Wait
.L_0200b194:
	ldr r3, [r6, #12]
	ldr r2, .L_0200b2bc
	mov r1, r8
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r5, #192
	ldr r3, [r1, #12]
	lsls r5, r5, #5
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r3, [r7, #12]
	ldr r2, .L_0200b2c0
	movs r1, #192
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #11
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #144
	movs r0, #13
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	movs r0, #15
	lsls r1, r1, #8
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #0
	adds r2, r5, #0
	movs r0, #16
	bl Func_02000cf8
	movs r0, #1
	bl wait
	movs r0, #16
	bl Actor_GetById
	ldr r3, [r0, #28]
	cmp r3, r5
	bgt .L_0200b194
	ldr r2, .L_0200b2c4
	movs r3, #0
	movs r0, #131
	str r3, [r2]
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #195
	lsls r0, r0, #1
	bl Func_020053d8
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r2, #0
	movs r0, #16
	movs r1, #0
	bl Func_02005230
	movs r1, #1
	movs r0, #9
	bl Actor_SetModeById
	movs r0, #9
	bl Actor_GetById
	movs r1, #216
	movs r3, #216
	lsls r1, r1, #16
	movs r2, #0
	lsls r3, r3, #16
	bl Actor_SetPositionAndResetMotion
	mov r1, r8
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r0, #9
	movs r1, #3
	bl Actor_SetSpritePriority
	movs r2, #0
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	adds r0, r6, #0
	bl Actor_SetPositionAndResetMotion
	ldr r3, .L_0200b2b8
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Actor_GetById
	movs r1, #1
	bl Actor_SetSingleChildFlags
	movs r0, #1
	bl wait
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	bl Func_020051c8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b2b8:
	.4byte gMainRoutineWork
.L_0200b2bc:
	.4byte 0xfff94000
.L_0200b2c0:
	.4byte 0xfffd4ccd
.L_0200b2c4:
	.4byte gData_02006298
	.section .text.x0200b2c8,"ax",%progbits
	.global Func_020032c8
	.thumb_func
Func_020032c8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200b514
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Actor_GetById
	adds r6, r0, #0
	movs r0, #10
	bl Actor_GetById
	mov r8, r0
	bl Func_020052f0
	adds r7, r0, #0
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	bl Func_020052f0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	adds r0, #85
	strb r3, [r0]
	strb r3, [r2]
	movs r0, #1
	bl wait
	movs r0, #130
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_ClearBit
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #162
	bl Func_020053d8
	bl Func_02000e60
	movs r2, #0
	mov r10, r2
.L_0200b352:
	ldr r3, [r6, #12]
	movs r2, #144
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r6, #12]
	mov r1, r8
	ldr r3, [r1, #12]
	movs r5, #128
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r3, [r7, #12]
	movs r2, #230
	lsls r2, r2, #9
	adds r2, #204
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #128
	str r3, [r7, #12]
	movs r0, #12
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	movs r0, #18
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	movs r0, #19
	lsls r1, r1, #8
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #0
	adds r2, r5, #0
	movs r0, #20
	bl Func_02000cd8
	mov r1, r10
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	movs r2, #240
	asrs r1, r3, #16
	lsls r2, r2, #12
	mov r10, r1
	cmp r3, r2
	bls .L_0200b352
	bl Func_02005158
	movs r0, #1
	bl wait
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #10
	bl FieldEvent_Wait
.L_0200b3f0:
	ldr r3, [r6, #12]
	ldr r2, .L_0200b518
	mov r1, r8
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r5, #192
	ldr r3, [r1, #12]
	lsls r5, r5, #5
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r3, [r7, #12]
	ldr r2, .L_0200b51c
	movs r1, #192
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #12
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #144
	movs r0, #17
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	movs r0, #18
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	movs r0, #19
	lsls r1, r1, #8
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #0
	adds r2, r5, #0
	movs r0, #20
	bl Func_02000cf8
	movs r0, #1
	bl wait
	movs r0, #20
	bl Actor_GetById
	ldr r3, [r0, #28]
	cmp r3, r5
	bgt .L_0200b3f0
	ldr r2, .L_0200b520
	movs r0, #135
	movs r3, #0
	lsls r0, r0, #1
	str r3, [r2]
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #195
	lsls r0, r0, #1
	bl Func_020053d8
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r2, #0
	movs r0, #20
	movs r1, #0
	bl Func_02005230
	movs r1, #1
	movs r0, #10
	bl Actor_SetModeById
	movs r0, #10
	bl Actor_GetById
	movs r1, #188
	movs r3, #248
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #16
	bl Actor_SetPositionAndResetMotion
	mov r1, r8
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r0, #10
	movs r1, #3
	bl Actor_SetSpritePriority
	movs r2, #0
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	adds r0, r6, #0
	bl Actor_SetPositionAndResetMotion
	ldr r3, .L_0200b514
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Actor_GetById
	movs r1, #1
	bl Actor_SetSingleChildFlags
	movs r0, #1
	bl wait
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	bl Func_020051c8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200b514:
	.4byte gMainRoutineWork
.L_0200b518:
	.4byte 0xfff94000
.L_0200b51c:
	.4byte 0xfffd4ccd
.L_0200b520:
	.4byte gData_0200629c
	.section .text.x0200ba14,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_02003a14
	.thumb_func
Func_02003a14:
	push {lr}
	ldr r2, .L_ja_0200ba04
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	movs r3, #0
	str r3, [r2]
	subs r3, #13
	ldrb r2, [r1, #23]
	movs r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #23]
	bl Func_02005338
	pop {pc}
	.2byte 0x0000
.L_ja_0200ba04:
	.4byte gData_02006294
	.else
	.global Func_02003a14
	.thumb_func
Func_02003a14:
	push {lr}
	ldr r2, .L_0200ba54
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	movs r3, #0
	str r3, [r2]
	subs r3, #13
	ldrb r2, [r1, #23]
	movs r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #23]
	bl Func_02005338
	ldr r3, .L_0200ba58
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #1
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_0200ba52
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
.L_0200ba52:
	pop {pc}
.L_0200ba54:
	.4byte gData_02006294
.L_0200ba58:
	.4byte gMainRoutineWork
	.endif
	.section .bss,"aw",%nobits
	.global gData_02006294
gData_02006294:
	.space 0x00000004
	.global gData_02006298
gData_02006298:
	.space 0x00000004
	.global gData_0200629c
gData_0200629c:
	.space 0x00000004
	.global gData_020062a0
gData_020062a0:
	.space 0x00000004
	.global gData_020062a4
gData_020062a4:
	.space 0x00000004
	.global gData_020062a8
gData_020062a8:
	.space 0x00000002
	.global gData_020062aa
gData_020062aa:
	.space 0x00000002
	.global gData_020062ac
gData_020062ac:
