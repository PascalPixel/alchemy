.syntax unified
	.thumb
	.section .text.x02008200,"ax",%progbits
	.global Func_02000200
	.thumb_func
Func_02000200:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r10, r1
	mov r0, r10
	sub sp, #16
	bl Actor_GetById
	adds r7, r0, #0
	ldr r3, [r7, #8]
	movs r2, #0
	asrs r6, r3, #20
	ldr r3, [r7, #16]
	mov r8, r2
	asrs r5, r3, #20
	mov r3, r10
	cmp r3, #10
	bne .L_02008246
	movs r0, #132
	movs r1, #128
	lsls r1, r1, #18
	movs r2, #2
	movs r3, #0
	lsls r0, r0, #18
	bl Func_02004c7c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #18
	lsls r1, r1, #18
	movs r2, #2
	movs r3, #0
	bl Func_02004c7c
.L_02008246:
	cmp r6, #31
	bne .L_0200825a
	cmp r5, #32
	bne .L_0200825a
	movs r0, #138
	lsls r0, r0, #4
	bl GameFlag_SetBit
	movs r2, #1
	mov r8, r2
.L_0200825a:
	cmp r6, #25
	bne .L_02008270
	cmp r5, #33
	bne .L_02008270
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #161
	bl GameFlag_SetBit
	movs r3, #1
	mov r8, r3
.L_02008270:
	cmp r6, #21
	bne .L_02008286
	cmp r5, #33
	bne .L_02008286
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #164
	bl GameFlag_SetBit
	movs r2, #1
	mov r8, r2
.L_02008286:
	cmp r6, #27
	bne .L_0200829c
	cmp r5, #33
	bne .L_0200829c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #165
	bl GameFlag_SetBit
	movs r3, #1
	mov r8, r3
.L_0200829c:
	mov r2, r8
	cmp r2, #0
	beq .L_02008386
	mov r3, r10
	cmp r3, #10
	beq .L_020082b0
	movs r0, #155
	lsls r0, r0, #1
	bl Func_02004c9c
.L_020082b0:
	movs r3, #184
	lsls r3, r3, #5
	adds r3, #10
	adds r2, r7, #0
	adds r2, #85
	str r3, [r7, #72]
	movs r3, #3
	strb r3, [r2]
	mov r0, r10
	bl Actor_GetById
	movs r1, #1
	bl Actor_SetSingleChildFlags
	movs r0, #5
	bl wait
	ldr r3, [r7, #12]
	movs r6, #0
	cmp r3, #0
	ble .L_020082ec
.L_020082da:
	movs r0, #1
	adds r6, #1
	bl wait
	cmp r6, #59
	bgt .L_020082ec
	ldr r3, [r7, #12]
	cmp r3, #0
	bgt .L_020082da
.L_020082ec:
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02004c9c
	movs r0, #1
	bl wait
	movs r0, #240
	bl Func_02004c9c
	adds r2, r7, #0
	adds r2, #89
	movs r3, #0
	strb r3, [r2]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	adds r0, r7, #0
	movs r1, #4
	bl Func_02004a0c
	movs r6, #0
.L_0200831e:
	movs r0, #138
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	lsls r0, r0, #1
	bl Func_02004a1c
	mov r8, sp
	mov r2, r8
	adds r5, r0, #0
	lsls r3, r6, #2
	str r5, [r2, r3]
	movs r3, #166
	lsls r3, r3, #8
	adds r3, #102
	str r3, [r5, #24]
	movs r1, #1
	bl Func_02004a0c
	movs r1, #0
	adds r0, r5, #0
	bl Actor_SetSingleChildFlags
	movs r0, #154
	bl Func_02004c9c
	adds r6, #1
	movs r0, #5
	bl wait
	cmp r6, #2
	ble .L_0200831e
	bl Func_02000fa4
	ldr r3, .L_02008390
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_Stop
	movs r0, #30
	bl wait
	mov r5, r8
	movs r6, #2
.L_0200837a:
	ldmia r5!, {r0}
	subs r6, #1
	bl Func_02004a24
	cmp r6, #0
	bge .L_0200837a
.L_02008386:
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008390:
	.4byte gSequenceParentWork
	.section .text.x02008514,"ax",%progbits
	.balign 4
	.global Func_02000514
	.thumb_func
Func_02000514:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #15
	sub sp, #32
	bl Actor_GetById
	adds r7, r0, #0
	movs r0, #16
	bl Actor_GetById
	ldr r3, .L_020086e0
	mov r10, r0
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Actor_GetById
	ldrh r3, [r0, #6]
	movs r1, #128
	lsls r1, r1, #6
	adds r6, r3, r1
	movs r3, #192
	lsls r3, r3, #8
	mov r2, r10
	ands r6, r3
	cmp r2, #0
	beq .L_0200856c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #164
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200856c
	mov r3, r10
	ldr r0, [r3, #8]
	ldr r1, [r3, #16]
	movs r2, #2
	movs r3, #255
	bl Func_02004c7c
.L_0200856c:
	add r5, sp, #8
	adds r0, r5, #0
	bl Func_02004240
	cmp r0, #0
	bne .L_0200857a
	b .L_020086b2
.L_0200857a:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #164
	bl GameFlag_Test
	add r1, sp, #24
	mov r8, r1
	cmp r0, #0
	beq .L_02008594
	movs r2, #192
	lsls r2, r2, #8
	cmp r6, r2
	bne .L_020085aa
.L_02008594:
	mov r2, sp
	mov r3, r8
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [r5]
	ldr r1, [r5, #4]
	ldr r2, [r5, #8]
	ldr r3, [r5, #12]
	bl Func_020044c4
	b .L_020086b2
.L_020085aa:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #166
	bl GameFlag_SetBit
	bl Func_02004a94
	movs r0, #0
	bl Func_02004be4
	movs r0, #168
	movs r1, #1
	movs r2, #140
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #1
	bl Camera_MoveWithinMapBounds
	mov r3, r8
	mov r2, sp
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [r5]
	ldr r1, [r5, #4]
	ldr r2, [r5, #8]
	ldr r3, [r5, #12]
	bl Func_020044c4
	ldr r5, .L_020086e4
	ldr r0, [r7, #8]
	ldr r1, [r7, #16]
	adds r0, r0, r5
	movs r2, #0
	movs r3, #0
	bl Func_02004c7c
	ldr r0, [r7, #8]
	movs r6, #128
	lsls r6, r6, #12
	ldr r1, [r7, #16]
	movs r2, #0
	movs r3, #0
	adds r0, r0, r6
	bl Func_02004c7c
	ldr r0, [r7, #8]
	ldr r1, [r7, #16]
	adds r0, r0, r5
	movs r2, #2
	movs r3, #0
	bl Func_02004c7c
	ldr r0, [r7, #8]
	ldr r2, .L_020086e8
	ldr r1, [r7, #16]
	adds r0, r0, r2
	movs r3, #0
	movs r2, #2
	bl Func_02004c7c
	ldr r0, [r7, #8]
	ldr r1, [r7, #16]
	movs r2, #2
	movs r3, #0
	adds r0, r0, r6
	bl Func_02004c7c
	ldr r0, [r7, #8]
	movs r3, #192
	lsls r3, r3, #13
	movs r2, #2
	adds r0, r0, r3
	ldr r1, [r7, #16]
	movs r3, #0
	bl Func_02004c7c
	movs r0, #15
	movs r1, #3
	bl Actor_SetModeById
	movs r2, #16
	movs r0, #15
	movs r1, #0
	bl Actor_OffsetPositionAndResetMotion
	movs r0, #20
	bl FieldEvent_Wait
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #204
	adds r2, r7, #0
	str r3, [r7, #72]
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	ldr r3, [r7, #12]
	movs r5, #0
	cmp r3, #0
	ble .L_02008686
.L_02008674:
	movs r0, #1
	adds r5, #1
	bl wait
	cmp r5, #59
	bgt .L_02008686
	ldr r3, [r7, #12]
	cmp r3, #0
	bgt .L_02008674
.L_02008686:
	bl Func_02000fa4
	movs r0, #240
	bl Func_02004c9c
	movs r1, #8
	movs r0, #15
	bl Actor_SetModeById
	movs r0, #60
	bl FieldEvent_Wait
	ldr r3, .L_020086e0
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	movs r1, #1
	bl Camera_FollowActor
	bl Func_02004a9c
.L_020086b2:
	mov r1, r10
	cmp r1, #0
	beq .L_020086d4
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #164
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020086d4
	mov r2, r10
	ldr r0, [r2, #8]
	ldr r1, [r2, #16]
	movs r3, #0
	movs r2, #2
	bl Func_02004c7c
.L_020086d4:
	add sp, #32
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020086e0:
	.4byte gSequenceParentWork
.L_020086e4:
	.4byte 0xffe80000
.L_020086e8:
	.4byte 0xfff80000
	.section .text.x0200bec4,"ax",%progbits
	.global Func_02003ec4
	.thumb_func
Func_02003ec4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	movs r2, #160
	lsls r2, r2, #3
	movs r3, #192
	adds r5, r7, r2
	lsls r3, r3, #4
	movs r2, #63
	adds r6, r7, r3
	mov r8, r2
.L_0200bee2:
	ldr r3, [r5, #24]
	cmp r3, #19
	bhi .L_0200bf30
	movs r2, #176
	lsls r2, r2, #5
	adds r2, #2
	adds r1, r7, r2
	ldrh r1, [r1]
	movs r2, #7
	asrs r3, r3, #2
	ands r3, r2
	lsls r3, r3, #3
	adds r1, r1, r3
	ldr r3, .L_0200bf24
	ldr r2, .L_0200bf28
	ands r1, r3
	ldrh r3, [r6, #8]
	adds r0, r6, #0
	ands r3, r2
	orrs r3, r1
	strh r3, [r6, #8]
	adds r1, r5, #0
	bl Func_02004c6c
	adds r0, r5, #0
	movs r1, #63
	ldr r2, .L_0200bf2c
	bl Func_02004c74
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	b .L_0200bf30
.L_0200bf24:
	.4byte 0x000003ff
.L_0200bf28:
	.4byte 0xfffffc00
.L_0200bf2c:
	.4byte 0xffff8000
.L_0200bf30:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r2, r8
	adds r6, #40
	adds r5, #28
	cmp r2, #0
	bge .L_0200bee2
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200c004,"ax",%progbits
	.balign 4
	.global Func_02004004
	.thumb_func
Func_02004004:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r1, #176
	lsls r1, r1, #5
	adds r1, #8
	movs r0, #220
	sub sp, #4
	bl HeapSlot_AllocFar
	adds r6, r0, #0
	ldr r0, .L_0200c0b8
	bl Resource_GetTableEntry
	adds r1, r6, #0
	bl Func_020049d4
	bl Resource_FindFreeEntry
	movs r1, #160
	lsls r1, r1, #3
	adds r2, r6, #0
	adds r5, r0, #0
	bl VramCache_Load
	movs r1, #176
	lsls r1, r1, #5
	adds r1, #2
	mov r10, r0
	adds r3, r6, r1
	mov r2, r10
	adds r1, #2
	strh r2, [r3]
	adds r3, r6, r1
	strh r5, [r3]
	movs r2, #160
	movs r3, #192
	lsls r2, r2, #3
	lsls r3, r3, #4
	movs r1, #63
	adds r7, r6, r2
	adds r5, r6, r3
	mov r8, r1
.L_0200c05c:
	mov r2, r10
	movs r3, #128
	str r2, [sp, #0]
	adds r0, r5, #0
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	bl Func_02004c64
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r5, #9]
	movs r1, #13
	strb r3, [r5, #5]
	negs r1, r1
	movs r3, #15
	ands r3, r2
	adds r2, r1, #0
	ands r3, r2
	strb r3, [r5, #9]
	movs r3, #240
	strh r3, [r5, #30]
	subs r3, #241
	add r8, r3
	mov r2, r8
	str r3, [r7, #24]
	adds r5, #40
	adds r7, #28
	cmp r2, #0
	bge .L_0200c05c
	movs r1, #176
	lsls r1, r1, #5
	adds r2, r6, r1
	movs r3, #0
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_0200c0bc
	bl Task_Add
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c0b8:
	.4byte 0x000001f0
.L_0200c0bc:
	.4byte Func_02003ec4
	.section .text.x0200c0ec,"ax",%progbits
	.global Func_020040ec
	.thumb_func
Func_020040ec:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #8
	mov r8, r3
	bl Actor_GetById
	ldr r3, [r0, #80]
	ldr r5, .L_0200c1f0
	ldr r3, [r3, #40]
	movs r1, #0
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r4, r3, #16
	ldrh r3, [r5, r1]
	lsrs r2, r4, #16
	cmp r2, r3
	beq .L_0200c132
.L_0200c118:
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	lsrs r2, r3, #16
	asrs r1, r3, #16
	cmp r2, #5
	bhi .L_0200c132
	lsls r3, r2, #1
	ldrh r3, [r5, r3]
	lsrs r2, r4, #16
	cmp r2, r3
	bne .L_0200c118
.L_0200c132:
	lsls r3, r1, #16
	lsrs r2, r3, #16
	cmp r2, #6
	bne .L_0200c13e
	movs r0, #0
	b .L_0200c1e6
.L_0200c13e:
	ldr r6, .L_0200c1f4
	lsls r2, r2, #2
	ldrsb r4, [r6, r2]
	adds r1, r4, #0
	cmp r4, #0
	bge .L_0200c14c
	negs r1, r4
.L_0200c14c:
	adds r3, r2, #2
	ldrsb r3, [r6, r3]
	cmp r3, #0
	bge .L_0200c156
	negs r3, r3
.L_0200c156:
	adds r3, r1, r3
	asrs r7, r3, #4
	adds r3, r2, #1
	ldrsb r1, [r6, r3]
	adds r5, r1, #0
	cmp r1, #0
	bge .L_0200c166
	negs r5, r1
.L_0200c166:
	adds r3, r2, #3
	ldrsb r2, [r6, r3]
	cmp r2, #0
	bge .L_0200c170
	negs r2, r2
.L_0200c170:
	adds r5, r5, r2
	mov r10, r5
	ldr r6, [r0, #8]
	mov r3, r10
	ldr r5, [r0, #16]
	asrs r3, r3, #4
	mov r10, r3
	lsls r3, r4, #16
	adds r6, r6, r3
	lsls r3, r1, #16
	adds r5, r5, r3
	movs r3, #164
	lsls r3, r3, #1
	add r3, r8
	ldr r3, [r3]
	asrs r6, r6, #20
	asrs r1, r3, #20
	movs r3, #166
	lsls r3, r3, #1
	add r3, r8
	ldr r3, [r3]
	lsls r2, r1, #16
	asrs r3, r3, #20
	lsls r3, r3, #16
	asrs r5, r5, #20
	lsrs r2, r2, #16
	lsrs r3, r3, #16
	adds r2, r6, r2
	adds r3, r5, r3
	str r2, [sp, #0]
	str r3, [sp, #4]
	adds r0, r6, #0
	adds r1, r5, #0
	adds r2, r7, #0
	mov r3, r10
	bl Func_02004a54
	movs r3, #255
	mov r2, r10
	str r2, [sp, #0]
	str r3, [sp, #4]
	mov r8, r3
	movs r0, #0
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r7, #0
	bl Map_FillCellRectByte2
	mov r2, r10
	mov r3, r8
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #2
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r7, #0
	bl Map_FillCellRectByte2
	movs r0, #1
.L_0200c1e6:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c1f0:
	.4byte gData_02004ca4
.L_0200c1f4:
	.4byte gData_02004cb0
	.section .text.x0200c240,"ax",%progbits
	.global Func_02004240
	.thumb_func
Func_02004240:
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
	bl Func_020043a8
	adds r4, r0, #0
	cmp r4, #0
	bne .L_0200c26a
	b .L_0200c38a
.L_0200c26a:
	ldr r5, [r5]
	ldr r0, .L_0200c39c
	str r5, [sp, #20]
	lsls r1, r5, #2
	ldrsb r2, [r0, r1]
	cmp r2, #0
	bge .L_0200c27a
	negs r2, r2
.L_0200c27a:
	adds r3, r1, #2
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_0200c284
	negs r3, r3
.L_0200c284:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #16]
	adds r3, r1, #1
	ldrsb r2, [r0, r3]
	cmp r2, #0
	bge .L_0200c294
	negs r2, r2
.L_0200c294:
	adds r3, r1, #3
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_0200c29e
	negs r3, r3
.L_0200c29e:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #12]
	ldr r3, [sp, #24]
	ldr r2, .L_0200c3a0
	ldr r1, .L_0200c3a4
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
.L_0200c2dc:
	ldr r3, [sp, #20]
	ldr r2, .L_0200c39c
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
	bge .L_0200c34a
.L_0200c2fa:
	ldr r3, .L_0200c39c
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
	bge .L_0200c334
.L_0200c312:
	adds r0, r4, #0
	add r1, sp, #28
	str r4, [sp, #0]
	bl Func_02004a64
	ldr r4, [sp, #0]
	cmp r0, #2
	beq .L_0200c35c
	ldr r3, [r5]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r5]
	ldr r2, [sp, #16]
	adds r7, #1
	cmp r7, r2
	blt .L_0200c312
.L_0200c334:
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
	blt .L_0200c2fa
.L_0200c34a:
	ldr r3, [r6]
	movs r1, #1
	add r3, r9
	str r3, [r6]
	ldr r3, [r6, #8]
	add r11, r1
	add r3, r10
	str r3, [r6, #8]
	b .L_0200c2dc
.L_0200c35c:
	ldr r2, [sp, #8]
	movs r3, #0
	strb r3, [r2]
	mov r3, r11
	movs r0, #0
	cmp r3, #0
	beq .L_0200c38c
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
	b .L_0200c38c
.L_0200c38a:
	movs r0, #0
.L_0200c38c:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c39c:
	.4byte gData_02004cb0
.L_0200c3a0:
	.4byte gData_02004cc8
.L_0200c3a4:
	.4byte 0xffff0000
	.section .text.x0200c3a8,"ax",%progbits
	.global Func_020043a8
	.thumb_func
Func_020043a8:
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
	ldr r3, .L_0200c4b4
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Actor_GetById
	adds r7, r0, #0
	ldrh r3, [r7, #6]
	ldr r1, [sp, #8]
	lsrs r3, r3, #12
	str r3, [r1]
	movs r2, #8
	adds r5, #52
	mov r11, r2
	mov lr, r5
.L_0200c3e4:
	mov r3, lr
	ldr r6, [r3]
	movs r5, #0
.L_0200c3ea:
	ldr r3, [r6, #80]
	ldr r2, .L_0200c4b8
	ldr r3, [r3, #40]
	movs r0, #0
	ldrsh r1, [r3, r0]
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	cmp r1, r3
	bne .L_0200c48e
	ldr r0, [sp, #8]
	movs r2, #10
	ldrsh r1, [r7, r2]
	ldr r3, [r0]
	ldr r2, .L_0200c4bc
	lsls r3, r3, #2
	ldr r3, [r2, r3]
	ldr r4, .L_0200c4c0
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
	bgt .L_0200c48e
	cmp r9, r0
	bge .L_0200c48e
	cmp r12, r8
	bgt .L_0200c48e
	cmp r8, r1
	bge .L_0200c48e
	ldr r0, [sp, #0]
	movs r3, #1
	ands r3, r5
	str r5, [r0]
	cmp r3, #0
	beq .L_0200c47c
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r10, r3
	beq .L_0200c48e
	ldr r2, [sp, #4]
	mov r1, r11
	str r1, [r2]
	adds r0, r6, #0
	b .L_0200c4a4
.L_0200c47c:
	ldr r3, [r7, #16]
	asrs r3, r3, #20
	cmp r12, r3
	beq .L_0200c48e
	ldr r0, [sp, #4]
	mov r3, r11
	str r3, [r0]
	adds r0, r6, #0
	b .L_0200c4a4
.L_0200c48e:
	adds r5, #1
	cmp r5, #5
	bls .L_0200c3ea
	movs r2, #1
	add r11, r2
	movs r1, #4
	mov r3, r11
	add lr, r1
	cmp r3, #63
	bls .L_0200c3e4
	movs r0, #0
.L_0200c4a4:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c4b4:
	.4byte gSequenceParentWork
.L_0200c4b8:
	.4byte gData_02004ca4
.L_0200c4bc:
	.4byte gData_02004cc8
.L_0200c4c0:
	.4byte gData_02004cb0
	.section .text.x0200c4c4,"ax",%progbits
	.global Func_020044c4
	.thumb_func
Func_020044c4:
	sub sp, #16
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #56
	str r0, [sp, #88]
	str r1, [sp, #92]
	str r2, [sp, #96]
	str r3, [sp, #100]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r0, #133
	str r3, [sp, #28]
	ldr r3, .L_0200c764
	lsls r0, r0, #2
	adds r0, r0, r3
	mov r10, r0
	ldr r0, [r0]
	bl Actor_GetById
	mov r8, r0
	ldr r0, [sp, #104]
	bl Actor_GetById
	mov r3, r8
	ldr r3, [r3, #48]
	mov r4, r8
	str r3, [sp, #20]
	adds r6, r0, #0
	ldr r4, [r4, #52]
	mov r0, sp
	adds r0, #32
	str r0, [sp, #12]
	str r4, [sp, #16]
	ldr r2, [sp, #100]
	ldr r3, [r6, #8]
	movs r1, #0
	str r3, [r0]
	mov r9, r1
	ldr r3, [r6, #16]
	mov r1, sp
	adds r1, #44
	str r3, [r0, #8]
	ldr r5, .L_0200c768
	str r1, [sp, #8]
	lsls r7, r2, #2
	ldrsb r1, [r5, r7]
	ldr r3, [r6, #8]
	lsls r2, r1, #16
	adds r3, r3, r2
	ldr r2, [sp, #8]
	asrs r3, r3, #20
	str r3, [r2]
	mov lr, r3
	adds r3, r7, #1
	ldrsb r4, [r5, r3]
	ldr r3, [r6, #16]
	ldr r0, [sp, #8]
	lsls r2, r4, #16
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [r0, #8]
	adds r0, r1, #0
	mov r12, r3
	cmp r0, #0
	bge .L_0200c554
	negs r0, r0
.L_0200c554:
	adds r3, r7, #2
	ldrsb r1, [r5, r3]
	cmp r1, #0
	bge .L_0200c55e
	negs r1, r1
.L_0200c55e:
	adds r3, r0, r1
	asrs r3, r3, #4
	adds r1, r4, #0
	str r3, [sp, #24]
	cmp r1, #0
	bge .L_0200c56c
	negs r1, r1
.L_0200c56c:
	adds r3, r7, #3
	ldrsb r2, [r5, r3]
	cmp r2, #0
	bge .L_0200c576
	negs r2, r2
.L_0200c576:
	adds r3, r1, r2
	asrs r3, r3, #4
	str r3, [sp, #0]
	mov r11, r3
	movs r3, #0
	str r3, [sp, #4]
	mov r1, lr
	mov r2, r12
	ldr r3, [sp, #24]
	movs r0, #0
	bl Map_FillCellRectByte2
	mov r1, r10
	movs r2, #200
	ldr r0, [r1]
	lsls r2, r2, #5
	movs r1, #128
	lsls r1, r1, #8
	adds r2, #153
	bl Actor_SetSpeedParameters
	mov r2, r10
	ldr r0, [r2]
	movs r1, #8
	bl Actor_SetModeById
	movs r0, #15
	bl wait
	ldr r4, [sp, #12]
	ldr r1, [sp, #88]
	ldr r3, [r4]
	ldr r2, [sp, #96]
	subs r1, r1, r3
	ldr r3, [r4, #8]
	asrs r1, r1, #17
	subs r2, r2, r3
	mov r3, r10
	asrs r2, r2, #17
	ldr r0, [r3]
	bl Actor_OffsetPositionAndResetMotion
	mov r4, r10
	ldr r0, [r4]
	bl Actor_GetById
	ldr r3, .L_0200c76c
	str r3, [r0, #108]
	movs r0, #4
	bl wait
	movs r1, #2
	adds r0, r6, #0
	bl Func_02004a0c
	movs r0, #239
	bl Func_02004c9c
	movs r2, #200
	movs r1, #128
	lsls r2, r2, #5
	ldr r0, [sp, #104]
	lsls r1, r1, #8
	adds r2, #153
	bl Actor_SetSpeedParameters
	adds r0, r6, #0
	ldr r1, [sp, #88]
	ldr r2, [sp, #92]
	ldr r3, [sp, #96]
	bl Func_02004a34
	ldr r3, .L_0200c764
	movs r0, #133
	lsls r0, r0, #2
	adds r5, r3, r0
	ldr r0, [r5]
	bl Actor_WaitForMoveById
	ldr r0, [r5]
	movs r1, #2
	bl Actor_SetModeById
	movs r1, #152
	movs r2, #200
	lsls r1, r1, #7
	lsls r2, r2, #5
	ldr r0, [r5]
	adds r1, #204
	adds r2, #153
	bl Actor_SetSpeedParameters
	ldr r2, .L_0200c770
	mov r1, r9
	lsls r3, r1, #2
	ldr r2, [r2, r3]
	ldr r0, [r5]
	lsls r2, r2, #16
	asrs r1, r2, #31
	asrs r2, r2, #17
	bl Actor_OffsetPositionAndResetMotion
	ldr r3, [sp, #108]
	cmp r3, #0
	beq .L_0200c64c
	mov lr, r3
	.2byte 0xf800
.L_0200c64c:
	ldr r0, [r5]
	bl Actor_WaitForMoveById
	movs r1, #1
	ldr r0, [r5]
	bl Actor_SetModeById
	mov r3, r8
	movs r2, #0
	str r2, [r3, #108]
	ldr r4, [sp, #20]
	movs r5, #255
	str r4, [r3, #48]
	ldr r0, [sp, #16]
	str r0, [r3, #52]
	adds r0, r6, #0
	bl Func_02004a3c
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02004c9c
	movs r0, #213
	bl Func_02004c9c
	ldr r2, [r6, #12]
	ldr r1, [sp, #88]
	ldr r3, [sp, #96]
	adds r0, r6, #0
	bl Actor_SetPositionAndResetMotion
	adds r0, r6, #0
	movs r1, #1
	bl Func_02004a0c
	ldr r1, .L_0200c768
	ldr r0, [sp, #88]
	ldrsb r3, [r1, r7]
	adds r2, r7, #1
	lsls r3, r3, #16
	adds r0, r0, r3
	ldrsb r3, [r1, r2]
	mov r10, r1
	ldr r1, [sp, #96]
	lsls r3, r3, #16
	adds r1, r1, r3
	ldr r4, [sp, #28]
	asrs r0, r0, #20
	asrs r1, r1, #20
	str r0, [sp, #88]
	str r1, [sp, #96]
	mov r9, r2
	movs r2, #164
	lsls r2, r2, #1
	adds r3, r4, r2
	ldr r3, [r3]
	adds r2, #4
	asrs r3, r3, #20
	mov r8, r3
	adds r3, r4, r2
	ldr r6, [r3]
	mov r4, r8
	asrs r6, r6, #20
	adds r3, r4, r0
	adds r2, r6, r1
	str r3, [sp, #0]
	str r2, [sp, #4]
	mov r3, r11
	ldr r2, [sp, #24]
	bl Func_02004a54
	mov r0, r11
	ldr r1, [sp, #88]
	ldr r2, [sp, #96]
	str r0, [sp, #0]
	ldr r3, [sp, #24]
	movs r0, #0
	str r5, [sp, #4]
	bl Map_FillCellRectByte2
	mov r3, r11
	ldr r1, [sp, #88]
	ldr r2, [sp, #96]
	str r3, [sp, #0]
	movs r0, #2
	ldr r3, [sp, #24]
	str r5, [sp, #4]
	bl Map_FillCellRectByte2
	ldr r0, [sp, #12]
	mov r4, r10
	ldrsb r3, [r4, r7]
	ldr r1, [r0]
	ldr r2, [sp, #8]
	lsls r3, r3, #16
	adds r1, r1, r3
	asrs r1, r1, #20
	str r1, [r2]
	mov r3, r9
	ldrsb r2, [r4, r3]
	ldr r3, [r0, #8]
	ldr r4, [sp, #8]
	lsls r2, r2, #16
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [r4, #8]
	add r8, r1
	adds r6, r6, r3
	str r1, [sp, #0]
	str r3, [sp, #4]
	ldr r2, [sp, #24]
	mov r0, r8
	adds r1, r6, #0
	mov r3, r11
	bl Func_02004a54
	ldr r0, [sp, #8]
	mov r3, r11
	ldr r1, [r0]
	ldr r2, [r0, #8]
	movs r4, #0
	str r3, [sp, #0]
	movs r0, #2
	ldr r3, [sp, #24]
	str r4, [sp, #4]
	bl Map_FillCellRectByte2
	bl Func_02004c0c
	add sp, #56
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
.L_0200c764:
	.4byte gSequenceParentWork
.L_0200c768:
	.4byte gData_02004cb0
.L_0200c76c:
	.4byte Func_02004774
.L_0200c770:
	.4byte gData_02004cc8
	.section .text.x0200c868,"ax",%progbits
	.global Func_02004868
	.thumb_func
Func_02004868:
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
	b .L_0200c8d4
.L_0200c884:
	ldrh r3, [r5]
	movs r1, #26
	ldrsh r7, [r2, r1]
	cmp r7, r3
	bne .L_0200c8d0
	adds r0, r7, #0
	bl Actor_GetById
	adds r5, #2
	ldrh r2, [r5]
	mov r3, r10
	adds r6, r0, #0
	mov r8, r2
	ldrh r5, [r5, #2]
	cmp r3, #7
	bgt .L_0200c8ac
	ldr r3, [r6, #28]
	ldr r1, .L_0200c8e8
	adds r3, r3, r1
	str r3, [r6, #28]
.L_0200c8ac:
	mov r2, r9
	cmp r2, #1
	bne .L_0200c8de
	adds r0, r5, #0
	bl GameFlag_SetBit
	adds r3, r6, #0
	adds r3, #34
	ldrb r3, [r3]
	adds r0, r7, #0
	mov r1, r8
	adds r2, r5, #0
	bl Func_020048ec
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #28]
	b .L_0200c8de
.L_0200c8d0:
	adds r5, #6
	movs r1, #255
.L_0200c8d4:
	ldrh r3, [r5]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	bne .L_0200c884
.L_0200c8de:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200c8e8:
	.4byte 0xffffe100
	.section .bss,"aw",%nobits
	.space 0x00000008
	.global gData_020056e0
gData_020056e0:
