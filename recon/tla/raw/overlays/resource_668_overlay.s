.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_02004d14
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
	.4byte Data_02004da4
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl Func_020049fc
	cmp r0, #0
	beq .L_02008060
	ldr r0, .L_02008064
	b .L_02008062
.L_02008060:
	ldr r0, .L_02008068
.L_02008062:
	pop {pc}
.L_02008064:
	.4byte Data_02004fe4
.L_02008068:
	.4byte Data_02004dbc
	.section .text.x0200806c,"ax",%progbits
	.global Func_0200006c
	.thumb_func
Func_0200006c:
	push {r5, r6, lr}
	adds r6, r1, #0
	adds r5, r0, #0
	cmp r6, #0
	bne .L_0200807a
	bl Func_02004004
.L_0200807a:
	cmp r5, #1
	bne .L_0200808c
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl Func_02004a04
	bl Func_02000fa4
.L_0200808c:
	cmp r5, #2
	bne .L_020080a2
	movs r0, #249
	movs r1, #128
	movs r2, #206
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #17
	movs r3, #2
	bl Func_02003f48
.L_020080a2:
	cmp r5, #3
	bne .L_020080b8
	movs r0, #249
	movs r1, #128
	movs r2, #206
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #17
	movs r3, #30
	bl Func_02003f48
.L_020080b8:
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	cmp r6, r3
	bne .L_020080c6
	bl Func_020040c0
.L_020080c6:
	pop {r5, r6, pc}
	.section .text.x020080c8,"ax",%progbits
	.global Func_020000c8
	.thumb_func
Func_020000c8:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r2, r1, #0
	ldr r0, .L_0200818c
	adds r1, r5, #0
	bl Func_02004868
	cmp r5, #1
	bne .L_02008188
	movs r0, #19
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #21
	bl Object_GetById
	movs r2, #128
	movs r3, #128
	adds r5, r0, #0
	lsls r2, r2, #11
	lsls r3, r3, #8
	str r2, [r6, #48]
	str r3, [r6, #52]
	str r3, [r5, #52]
	str r2, [r5, #48]
	movs r1, #244
	movs r3, #156
	ldr r2, [r6, #12]
	adds r0, r6, #0
	lsls r1, r1, #17
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	movs r1, #220
	movs r3, #156
	adds r0, r6, #0
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #17
	bl Func_02004a34
	movs r1, #244
	movs r3, #156
	ldr r2, [r5, #12]
	adds r0, r5, #0
	lsls r1, r1, #17
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	movs r1, #220
	movs r3, #188
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #17
	adds r0, r5, #0
	bl Func_02004a34
	movs r0, #1
	bl Func_02004c8c
	movs r0, #2
	bl Func_02004c8c
	movs r7, #0
.L_02008148:
	movs r1, #18
	movs r2, #19
	movs r0, #1
	bl Func_02004c24
	movs r1, #20
	movs r2, #21
	movs r0, #2
	bl Func_02004c24
	movs r0, #1
	bl WaitFrames
	movs r0, #1
	bl Func_02004c8c
	movs r0, #2
	bl Func_02004c8c
	adds r0, r6, #0
	bl Func_02004a7c
	cmp r0, #0
	beq .L_02008182
	adds r0, r5, #0
	bl Func_02004a7c
	cmp r0, #0
	bne .L_02008188
.L_02008182:
	adds r7, #1
	cmp r7, #59
	ble .L_02008148
.L_02008188:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200818c:
	.4byte Data_02004d0c
	.section .text.x02008190,"ax",%progbits
	.global Func_02000190
	.thumb_func
Func_02000190:
	push {r5, r6, r7, lr}
	ldr r3, .L_020081fc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	adds r5, r1, #0
	bl Object_GetById
	adds r6, r0, #0
	adds r0, r5, #0
	bl Object_GetById
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	cmp r5, #10
	bne .L_020081fa
	ldr r3, [r6, #8]
	asrs r7, r3, #20
	ldr r3, [r0, #8]
	asrs r6, r3, #20
	ldr r3, [r0, #16]
	asrs r5, r3, #20
	cmp r6, #34
	bne .L_020081de
	cmp r5, #32
	bne .L_020081de
	cmp r7, #33
	bne .L_020081de
	movs r0, #132
	movs r1, #128
	lsls r0, r0, #18
	lsls r1, r1, #18
	movs r2, #2
	movs r3, #255
	bl Func_02004c7c
.L_020081de:
	cmp r6, #33
	bne .L_020081fa
	cmp r5, #32
	bne .L_020081fa
	cmp r7, #32
	bne .L_020081fa
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #18
	lsls r1, r1, #18
	movs r2, #2
	movs r3, #255
	bl Func_02004c7c
.L_020081fa:
	pop {r5, r6, r7, pc}
.L_020081fc:
	.4byte gPartyState
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
	bl Object_GetById
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
	bl Func_02004a04
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
	bl Func_02004a04
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
	bl Func_02004a04
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
	bl Func_02004a04
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
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #5
	bl WaitFrames
	ldr r3, [r7, #12]
	movs r6, #0
	cmp r3, #0
	ble .L_020082ec
.L_020082da:
	movs r0, #1
	adds r6, #1
	bl WaitFrames
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
	bl WaitFrames
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
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #154
	bl Func_02004c9c
	adds r6, #1
	movs r0, #5
	bl WaitFrames
	cmp r6, #2
	ble .L_0200831e
	bl Func_02000fa4
	ldr r3, .L_02008390
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #30
	bl WaitFrames
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
	.4byte gPartyState
	.section .text.x02008394,"ax",%progbits
	.global Func_02000394
	.thumb_func
Func_02000394:
	push {lr}
	bl Func_02000190
	movs r0, #160
	movs r1, #232
	lsls r1, r1, #17
	movs r2, #2
	movs r3, #255
	lsls r0, r0, #17
	bl Func_02004c7c
	movs r0, #160
	movs r1, #240
	lsls r1, r1, #17
	movs r2, #2
	movs r3, #255
	lsls r0, r0, #17
	bl Func_02004c7c
	movs r0, #160
	movs r1, #248
	lsls r1, r1, #17
	movs r2, #2
	movs r3, #255
	lsls r0, r0, #17
	bl Func_02004c7c
	movs r0, #160
	movs r1, #128
	lsls r1, r1, #18
	movs r2, #2
	movs r3, #255
	lsls r0, r0, #17
	bl Func_02004c7c
	movs r0, #176
	movs r1, #232
	lsls r1, r1, #17
	movs r2, #2
	movs r3, #255
	lsls r0, r0, #17
	bl Func_02004c7c
	movs r0, #176
	movs r1, #240
	lsls r1, r1, #17
	movs r2, #2
	movs r3, #255
	lsls r0, r0, #17
	bl Func_02004c7c
	movs r0, #176
	movs r1, #248
	lsls r1, r1, #17
	movs r2, #2
	movs r3, #255
	lsls r0, r0, #17
	bl Func_02004c7c
	movs r0, #176
	movs r1, #128
	lsls r1, r1, #18
	movs r2, #2
	movs r3, #255
	lsls r0, r0, #17
	bl Func_02004c7c
	movs r0, #168
	movs r1, #132
	lsls r1, r1, #18
	movs r2, #2
	movs r3, #0
	lsls r0, r0, #17
	bl Func_02004c7c
	pop {pc}
	.section .text.x0200842c,"ax",%progbits
	.global Func_0200042c
	.thumb_func
Func_0200042c:
	push {r5, r6, r7, lr}
	adds r6, r1, #0
	adds r7, r0, #0
	adds r0, r6, #0
	bl Object_GetById
	ldr r2, [r0, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	movs r5, #0
	cmp r2, #21
	bne .L_0200844c
	cmp r3, #33
	bne .L_0200844c
	movs r5, #1
.L_0200844c:
	movs r0, #160
	movs r1, #232
	lsls r1, r1, #17
	movs r2, #2
	movs r3, #0
	lsls r0, r0, #17
	bl Func_02004c7c
	movs r0, #160
	movs r1, #240
	lsls r1, r1, #17
	movs r2, #2
	movs r3, #0
	lsls r0, r0, #17
	bl Func_02004c7c
	movs r0, #160
	movs r1, #248
	lsls r1, r1, #17
	movs r2, #2
	movs r3, #0
	lsls r0, r0, #17
	bl Func_02004c7c
	movs r0, #160
	movs r1, #128
	lsls r1, r1, #18
	movs r2, #2
	movs r3, #0
	lsls r0, r0, #17
	bl Func_02004c7c
	movs r0, #176
	movs r1, #232
	lsls r1, r1, #17
	movs r2, #2
	movs r3, #0
	lsls r0, r0, #17
	bl Func_02004c7c
	movs r0, #176
	movs r1, #240
	lsls r1, r1, #17
	movs r2, #2
	movs r3, #0
	lsls r0, r0, #17
	bl Func_02004c7c
	movs r0, #176
	movs r1, #248
	lsls r1, r1, #17
	movs r2, #2
	movs r3, #0
	lsls r0, r0, #17
	bl Func_02004c7c
	movs r0, #176
	movs r1, #128
	lsls r1, r1, #18
	movs r2, #2
	movs r3, #0
	lsls r0, r0, #17
	bl Func_02004c7c
	movs r0, #168
	movs r1, #132
	lsls r0, r0, #17
	lsls r1, r1, #18
	movs r2, #2
	movs r3, #255
	bl Func_02004c7c
	cmp r5, #0
	beq .L_020084f2
	movs r0, #168
	movs r1, #1
	movs r2, #140
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #1
	bl Func_02004b8c
.L_020084f2:
	adds r0, r7, #0
	adds r1, r6, #0
	bl Func_02000200
	cmp r5, #0
	beq .L_0200850e
	ldr r3, .L_02008510
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Func_02004b7c
.L_0200850e:
	pop {r5, r6, r7, pc}
.L_02008510:
	.4byte gPartyState
	.section .text.x02008514,"ax",%progbits
	.global Func_02000514
	.thumb_func
Func_02000514:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #15
	sub sp, #32
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #16
	bl Object_GetById
	ldr r3, .L_020086e0
	mov r10, r0
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Object_GetById
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
	bl Func_020049fc
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
	bl Func_020049fc
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
	bl Func_02004a04
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
	bl Func_02004b8c
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
	bl Object_SetModeById
	movs r2, #16
	movs r0, #15
	movs r1, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #20
	bl Battle_WaitMode0
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
	bl WaitFrames
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
	bl Object_SetModeById
	movs r0, #60
	bl Battle_WaitMode0
	ldr r3, .L_020086e0
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	movs r1, #1
	bl Func_02004b7c
	bl Func_02004a9c
.L_020086b2:
	mov r1, r10
	cmp r1, #0
	beq .L_020086d4
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #164
	bl Func_020049fc
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
	.4byte gPartyState
.L_020086e4:
	.4byte 0xffe80000
.L_020086e8:
	.4byte 0xfff80000
	.section .text.x020086ec,"ax",%progbits
	.global Func_020006ec
	.thumb_func
Func_020006ec:
	push {r5, lr}
	ldr r3, .L_0200872c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Func_02004bb4
	cmp r0, #16
	bne .L_02008720
	adds r0, r5, #0
	movs r1, #16
	bl Func_02000394
	bl Func_02004c44
	cmp r0, #0
	beq .L_02008716
	bl Func_02004c4c
.L_02008716:
	adds r0, r5, #0
	movs r1, #16
	bl Func_0200042c
	b .L_02008728
.L_02008720:
	cmp r0, #15
	bne .L_02008728
	bl Func_02000514
.L_02008728:
	pop {r5, pc}
	.2byte 0x0000
.L_0200872c:
	.4byte gPartyState
	.section .text.x02008730,"ax",%progbits
	.global Func_02000730
	.thumb_func
Func_02000730:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #168
	bl Func_02004a04
	bl Func_02004a94
	movs r0, #0
	bl Func_02004be4
	ldr r0, .L_02008830
	bl Func_02004b3c
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #22
	bl Func_02004b6c
	ldr r3, .L_02008834
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #22
	bl Func_02004b34
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #22
	movs r1, #0
	bl Func_02004b4c
	movs r1, #2
	movs r0, #22
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #22
	bl Func_02004b44
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020087f0
	movs r5, #192
	movs r0, #20
	lsls r5, r5, #18
	bl Battle_WaitMode0
	ldr r3, [r5, #108]
	movs r2, #226
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r2, [r3]
	movs r1, #0
	adds r2, #1
	strh r2, [r3]
	movs r0, #22
	bl Func_02004b44
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020087f0
	movs r0, #20
	bl Battle_WaitMode0
	ldr r3, [r5, #108]
	movs r2, #226
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r2, [r3]
	movs r1, #0
	adds r2, #1
	strh r2, [r3]
	movs r0, #22
	bl Func_02004b44
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	beq .L_02008802
.L_020087f0:
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02004b4c
	b .L_02008820
.L_02008802:
	movs r0, #20
	bl Battle_WaitMode0
	ldr r2, [r5, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #22
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #0
	bl Func_02004b4c
.L_02008820:
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02004b54
	bl Func_02004a9c
	pop {r5, pc}
.L_02008830:
	.4byte 0x00001b58
.L_02008834:
	.4byte gPartyState
	.section .text.x02008838,"ax",%progbits
	.global Func_02000838
	.thumb_func
Func_02000838:
	push {lr}
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl Func_020049fc
	cmp r0, #0
	beq .L_02008866
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #4
	bl Func_020049fc
	cmp r0, #0
	bne .L_020088c8
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #4
	bl Func_02004a04
	bl Func_02001cb0
	b .L_020088c8
.L_02008866:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl Func_020049fc
	cmp r0, #0
	beq .L_020088ac
	movs r0, #138
	lsls r0, r0, #4
	bl Func_020049fc
	cmp r0, #0
	beq .L_020088c8
	movs r0, #151
	bl Func_02004a84
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_020088c8
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #173
	bl Func_020049fc
	cmp r0, #0
	bne .L_020088c8
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #173
	bl Func_02004a04
	bl Func_020017ec
	b .L_020088c8
.L_020088ac:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #169
	bl Func_020049fc
	cmp r0, #0
	bne .L_020088c8
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #169
	bl Func_02004a04
	bl Func_0200112c
.L_020088c8:
	pop {pc}
	.2byte 0x0000
	.section .text.x020088cc,"ax",%progbits
	.global Func_020008cc
	.thumb_func
Func_020008cc:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	bl Func_02004a94
	movs r0, #0
	bl Func_02004be4
	movs r0, #158
	bl Func_02004c9c
	ldrh r1, [r5, #4]
	ldrh r2, [r5, #6]
	ldr r0, [r5]
	bl Func_02004a44
	ldr r5, .L_0200893c
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #2
	adds r0, #85
	strb r3, [r0]
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r2, r2, #7
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	movs r2, #8
	movs r1, #2
	negs r2, r2
	ldr r0, [r5]
	bl ObjectMotion_SnapHeadingAndOffset
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r6, #0
	bl Func_02004bac
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02004a9c
	pop {r5, r6, pc}
.L_0200893c:
	.4byte gPartyState
	.section .text.x02008940,"ax",%progbits
	.global Func_02000940
	.thumb_func
Func_02000940:
	push {lr}
	adds r1, r0, #0
	movs r2, #0
	ldr r0, .L_02008950
	bl Func_020008cc
	pop {pc}
	.2byte 0x0000
.L_02008950:
	.4byte Data_02005290
	.global Data_02000954
Data_02000954:
	.4byte 0x00004770
	.section .text.x02008958,"ax",%progbits
	.global Func_02000958
	.thumb_func
Func_02000958:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #190
	bl Func_02004a04
	pop {pc}
	.2byte 0x0000
	.section .text.x02008968,"ax",%progbits
	.global Func_02000968
	.thumb_func
Func_02000968:
	push {lr}
	b .L_02008a08
.L_0200896c:
	movs r0, #151
	bl Func_02004a84
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_02008a32
	movs r0, #248
	lsls r0, r0, #3
	adds r0, #255
	bl Func_02004a04
	bl Func_02004a94
	movs r0, #0
	bl Func_02004be4
	movs r1, #244
	movs r2, #220
	movs r0, #4
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #16
	movs r3, #0
	movs r2, #0
	movs r0, #5
	negs r1, r1
	bl Func_02004bec
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #4
	bl Func_02004b5c
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, .L_02008a34
	bl Func_02004b3c
	movs r2, #5
	movs r0, #5
	movs r1, #0
	bl Func_02004b4c
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_02008a38
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_020089f2
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_020089f2:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	bl Func_02004a9c
	b .L_02008a32
.L_02008a08:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl Func_020049fc
	cmp r0, #0
	beq .L_02008a32
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl Func_020049fc
	cmp r0, #0
	bne .L_02008a32
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #190
	bl Func_020049fc
	cmp r0, #0
	bne .L_0200896c
.L_02008a32:
	pop {pc}
.L_02008a34:
	.4byte 0x00001cef
.L_02008a38:
	.4byte gPartyState
	.section .text.x02008a3c,"ax",%progbits
	.global Func_02000a3c
	.thumb_func
Func_02000a3c:
	ldr r0, .L_02008a40
	bx lr
.L_02008a40:
	.4byte Data_02005298
	.section .text.x02008a44,"ax",%progbits
	.global Func_02000a44
	.thumb_func
Func_02000a44:
	ldr r3, .L_02008a8c
	movs r1, #128
	ldrb r2, [r3]
	movs r3, #1
	eors r3, r2
	lsls r0, r3, #2
	adds r0, r0, r3
	ldr r3, .L_02008a90
	lsls r0, r0, #6
	adds r0, r0, r3
	ldrh r3, [r0]
	lsls r1, r1, #19
	adds r1, #22
	strh r3, [r1]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #176
	ldrh r4, [r3, #10]
	movs r2, #197
	lsls r2, r2, #8
	adds r2, #255
	ands r2, r4
	strh r2, [r3, #10]
	movs r2, #254
	ldrh r4, [r3, #10]
	lsls r2, r2, #7
	adds r2, #255
	ands r2, r4
	strh r2, [r3, #10]
	adds r0, #2
	ldrh r2, [r3, #10]
	ldr r2, .L_02008a94
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bx lr
	.2byte 0x0000
.L_02008a8c:
	.4byte Data_020054f0
.L_02008a90:
	.4byte gOverlayArea + 0x56e0
.L_02008a94:
	.4byte 0xa2600001
	.section .text.x02008a98,"ax",%progbits
	.global Func_02000a98
	.thumb_func
Func_02000a98:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #188
	lsls r2, r2, #1
	adds r7, r3, r2
	ldr r3, .L_02008aec
	movs r6, #0
	ldrb r2, [r3]
	movs r3, #1
	eors r3, r2
	lsls r2, r3, #2
	adds r2, r2, r3
	ldr r3, .L_02008af0
	lsls r2, r2, #6
	adds r5, r2, r3
.L_02008aba:
	ldr r3, .L_02008af4
	ldrb r0, [r3]
	movs r2, #6
	ldrsh r3, [r7, r2]
	subs r0, r0, r6
	subs r0, r0, r3
	adds r0, #160
	lsls r0, r0, #9
	bl Math_Sine
	movs r2, #6
	ldrsh r3, [r7, r2]
	asrs r0, r0, #15
	adds r3, r3, r0
	adds r6, #1
	strh r3, [r5]
	adds r5, #2
	cmp r6, #160
	bne .L_02008aba
	ldr r3, .L_02008aec
	movs r1, #1
	ldrb r2, [r3]
	eors r2, r1
	strb r2, [r3]
	pop {r5, r6, r7, pc}
.L_02008aec:
	.4byte Data_020054f0
.L_02008af0:
	.4byte gOverlayArea + 0x56e0
.L_02008af4:
	.4byte Data_0300122c
	.section .text.x02008af8,"ax",%progbits
	.global Func_02000af8
	.thumb_func
Func_02000af8:
	push {lr}
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008b10
	bl Func_0200499c
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_02008b14
	bl Func_0200499c
	pop {pc}
.L_02008b10:
	.4byte Func_02000a98
.L_02008b14:
	.4byte Func_02000a44
	.section .text.x02008b18,"ax",%progbits
	.global Func_02000b18
	.thumb_func
Func_02000b18:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	lsls r0, r0, #1
	movs r2, #128
	adds r3, r3, r0
	lsls r2, r2, #1
	str r2, [r3]
	sub sp, #8
	bl Func_02000af8
	ldr r3, .L_02008ec4
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
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl Func_020049fc
	cmp r0, #0
	bne .L_02008b5a
	b .L_02008cd6
.L_02008b5a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #5
	bl Func_020049fc
	cmp r0, #0
	beq .L_02008c5c
	movs r0, #129
	lsls r0, r0, #4
	adds r0, #255
	bl Func_020049fc
	cmp r0, #0
	bne .L_02008c5c
	movs r1, #184
	movs r2, #154
	movs r0, #30
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02004b04
	movs r1, #136
	movs r2, #150
	movs r0, #31
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02004b04
	movs r1, #168
	movs r2, #146
	movs r0, #28
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02004b04
	movs r1, #128
	movs r0, #28
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004b54
	movs r0, #30
	movs r1, #0
	movs r2, #13
	bl Func_02004c5c
	movs r1, #128
	movs r0, #31
	lsls r1, r1, #8
	movs r2, #13
	bl Func_02004c5c
	movs r0, #28
	movs r1, #0
	movs r2, #13
	bl Func_02004c5c
	movs r1, #224
	movs r2, #216
	movs r0, #25
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Func_02004b04
	movs r1, #176
	movs r2, #204
	movs r0, #27
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_02004b04
	movs r1, #152
	movs r2, #188
	movs r0, #29
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_02004b04
	movs r1, #128
	movs r2, #0
	movs r0, #29
	lsls r1, r1, #7
	bl Func_02004b54
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #25
	bl Func_02004c54
	movs r0, #25
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #27
	bl Func_02004c54
	movs r0, #27
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #29
	bl Func_02004c54
	movs r0, #29
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #25
	movs r1, #13
	bl Object_SetModeById
	movs r0, #27
	movs r1, #13
	bl Object_SetModeById
	movs r0, #29
	movs r1, #13
	bl Object_SetModeById
.L_02008c5c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #5
	bl Func_020049fc
	cmp r0, #0
	beq .L_02008cd6
	movs r3, #6
	str r3, [sp, #4]
	movs r5, #11
	movs r0, #40
	movs r1, #0
	movs r2, #24
	movs r3, #31
	str r5, [sp, #0]
	bl Func_02004a5c
	movs r3, #75
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #84
	movs r1, #15
	movs r2, #10
	movs r3, #10
	bl Func_02004a5c
	movs r3, #24
	str r3, [sp, #4]
	movs r0, #10
	movs r3, #1
	movs r1, #24
	movs r2, #1
	str r5, [sp, #0]
	bl Func_02004a54
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
.L_02008cd6:
	ldr r2, .L_02008ec4
	movs r3, #241
	lsls r3, r3, #1
	adds r1, r2, r3
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #99
	beq .L_02008ce8
	b .L_02008e2e
.L_02008ce8:
	movs r3, #245
	lsls r3, r3, #1
	adds r2, r2, r3
	movs r3, #1
	strh r3, [r2]
	strh r3, [r1]
	movs r2, #138
	movs r1, #136
	movs r0, #4
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02004b04
	movs r1, #240
	movs r2, #138
	movs r0, #26
	lsls r1, r1, #15
	lsls r2, r2, #18
	bl Func_02004b04
	movs r1, #184
	movs r2, #154
	movs r0, #30
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02004b04
	movs r1, #136
	movs r2, #150
	movs r0, #31
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02004b04
	movs r1, #160
	movs r2, #146
	lsls r2, r2, #18
	movs r0, #28
	lsls r1, r1, #16
	bl Func_02004b04
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #30
	bl Func_02004c54
	movs r0, #30
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #31
	bl Func_02004c54
	movs r0, #31
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #28
	bl Func_02004c54
	movs r0, #28
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #27
	movs r1, #2
	bl Func_02004b64
	movs r0, #25
	movs r1, #2
	bl Func_02004b64
	movs r0, #29
	movs r1, #2
	bl Func_02004b64
	movs r1, #160
	movs r2, #196
	movs r0, #25
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_02004b04
	movs r1, #176
	movs r2, #204
	movs r0, #27
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_02004b04
	movs r1, #152
	movs r2, #188
	lsls r2, r2, #17
	movs r0, #29
	lsls r1, r1, #16
	bl Func_02004b04
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #27
	bl Func_02004c54
	movs r0, #27
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #29
	bl Func_02004c54
	movs r0, #29
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #208
	movs r0, #25
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004b54
	movs r1, #128
	movs r2, #0
	movs r0, #29
	lsls r1, r1, #7
	bl Func_02004b54
	movs r0, #27
	movs r1, #13
	bl Object_SetModeById
	movs r0, #29
	movs r1, #13
	bl Object_SetModeById
	movs r0, #30
	movs r1, #13
	bl Object_SetModeById
	movs r0, #31
	movs r1, #13
	bl Object_SetModeById
	movs r0, #28
	movs r1, #13
	bl Object_SetModeById
	bl Func_02003274
.L_02008e2e:
	bl Func_02000fa4
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #166
	bl Func_020049fc
	cmp r0, #0
	bne .L_02008e46
	movs r0, #15
	bl Func_020040ec
.L_02008e46:
	bl Func_02004c14
	movs r1, #128
	lsls r1, r1, #4
	adds r1, #163
	movs r2, #8
	movs r3, #9
	movs r0, #0
	bl Func_02004c2c
	movs r0, #8
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #32
	orrs r3, r5
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	bl Func_02004c1c
	movs r2, #13
	movs r1, #12
	movs r0, #0
	bl Func_02004c24
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #13
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, .L_02008ec8
	bl Func_020047fc
	movs r0, #14
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #162
	bl Func_020049fc
	adds r7, r0, #0
	cmp r7, #0
	bne .L_02008f28
	b .L_02008ecc
	.2byte 0x0000
.L_02008ec4:
	.4byte gPartyState
.L_02008ec8:
	.4byte Data_02004d0c
.L_02008ecc:
	movs r0, #18
	bl Object_GetById
	adds r2, r0, #0
	adds r2, #89
	ldrb r3, [r2]
	movs r6, #8
	orrs r3, r6
	strb r3, [r2]
	movs r0, #19
	bl Object_GetById
	movs r5, #128
	adds r3, r0, #0
	adds r3, #85
	lsls r5, r5, #13
	strb r7, [r3]
	movs r1, #18
	movs r2, #19
	str r5, [r0, #12]
	movs r0, #1
	bl Func_02004c24
	movs r0, #1
	bl Func_02004c94
	movs r0, #20
	bl Object_GetById
	adds r2, r0, #0
	adds r2, #89
	ldrb r3, [r2]
	movs r0, #21
	orrs r3, r6
	strb r3, [r2]
	bl Object_GetById
	adds r3, r0, #0
	adds r3, #85
	strb r7, [r3]
	movs r1, #20
	str r5, [r0, #12]
	movs r2, #21
	movs r0, #2
	bl Func_02004c24
.L_02008f28:
	movs r0, #16
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #17
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #10
	movs r1, #2
	bl Func_02004b64
	movs r0, #23
	bl Object_GetById
	movs r3, #1
	adds r0, #98
	strb r3, [r0]
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl Func_020049fc
	cmp r0, #0
	beq .L_02008f98
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl Func_020049fc
	cmp r0, #0
	bne .L_02008f98
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	movs r0, #24
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
.L_02008f98:
	movs r0, #0
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008fa0,"ax",%progbits
	.global Func_02000fa0
	.thumb_func
Func_02000fa0:
	movs r0, #0
	bx lr
	.section .text.x02008fa4,"ax",%progbits
	.global Func_02000fa4
	.thumb_func
Func_02000fa4:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #166
	sub sp, #8
	bl Func_020049fc
	cmp r0, #0
	beq .L_02009022
	movs r0, #15
	bl Object_GetById
	movs r1, #168
	movs r2, #134
	adds r5, r0, #0
	lsls r2, r2, #18
	movs r0, #15
	lsls r1, r1, #17
	bl Func_02004b04
	movs r0, #15
	movs r1, #4
	bl Object_SetModeById
	adds r0, r5, #0
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	adds r2, r5, #0
	strb r3, [r0]
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
	ldr r3, .L_02009128
	movs r1, #0
	str r1, [r5, #40]
	movs r0, #168
	movs r1, #132
	str r3, [r5, #12]
	str r3, [r5, #20]
	lsls r1, r1, #18
	movs r2, #2
	movs r3, #0
	lsls r0, r0, #17
	bl Func_02004c7c
	movs r0, #168
	movs r1, #132
	lsls r1, r1, #18
	movs r2, #0
	movs r3, #0
	lsls r0, r0, #17
	bl Func_02004c7c
	movs r0, #176
	movs r1, #132
	lsls r0, r0, #17
	lsls r1, r1, #18
	movs r2, #0
	movs r3, #0
	bl Func_02004c7c
.L_02009022:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #164
	bl Func_020049fc
	cmp r0, #0
	beq .L_0200903a
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
.L_0200903a:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #165
	bl Func_020049fc
	cmp r0, #0
	beq .L_02009052
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
.L_02009052:
	movs r0, #138
	lsls r0, r0, #4
	bl Func_020049fc
	cmp r0, #0
	beq .L_020090a8
	movs r0, #10
	bl Object_GetById
	movs r1, #252
	movs r2, #130
	adds r5, r0, #0
	lsls r2, r2, #18
	movs r0, #10
	lsls r1, r1, #17
	bl Func_02004b04
	movs r0, #10
	movs r1, #2
	bl Object_SetModeById
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
	movs r2, #0
	ldr r1, [r5, #16]
	movs r3, #0
	ldr r0, [r5, #8]
	bl Func_02004c7c
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r2, #0
	movs r3, #128
	bl Func_02004c84
.L_020090a8:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #161
	bl Func_020049fc
	cmp r0, #0
	beq .L_02009100
	movs r0, #11
	bl Object_GetById
	movs r1, #204
	movs r2, #134
	adds r5, r0, #0
	lsls r2, r2, #18
	movs r0, #11
	lsls r1, r1, #17
	bl Func_02004b04
	movs r0, #11
	movs r1, #2
	bl Object_SetModeById
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
	movs r2, #0
	ldr r1, [r5, #16]
	movs r3, #0
	ldr r0, [r5, #8]
	bl Func_02004c7c
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r2, #0
	movs r3, #128
	bl Func_02004c84
.L_02009100:
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl Func_020049fc
	cmp r0, #0
	beq .L_02009122
	movs r3, #30
	movs r2, #24
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #32
	movs r1, #24
	movs r2, #2
	movs r3, #2
	bl Func_02004a4c
.L_02009122:
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
.L_02009128:
	.4byte 0xfffc0000
	.section .text.x0200912c,"ax",%progbits
	.global Func_0200112c
	.thumb_func
Func_0200112c:
	push {r5, lr}
	bl Func_02004a94
	movs r0, #0
	bl Func_02004be4
	ldr r0, .L_0200952c
	bl Func_02004b3c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl Func_02004b84
	movs r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r2, #196
	movs r0, #4
	movs r1, #232
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #16
	movs r3, #128
	movs r0, #26
	movs r1, #0
	negs r2, r2
	lsls r3, r3, #6
	bl Func_02004bec
	movs r3, #128
	movs r0, #5
	movs r1, #0
	movs r2, #0
	lsls r3, r3, #6
	bl Func_02004bec
	movs r3, #128
	lsls r3, r3, #6
	movs r0, #6
	movs r1, #0
	movs r2, #16
	bl Func_02004bec
	movs r0, #4
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #4
	bl Func_02004b54
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #22
	bl Func_02004b9c
	bl Func_02004b94
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #22
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #22
	movs r1, #0
	bl Func_02004b4c
	movs r1, #1
	movs r0, #23
	bl Func_02004b9c
	bl Func_02004b94
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #23
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #23
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02004b54
	movs r1, #128
	movs r0, #24
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02004b54
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #25
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #11
	bl Func_02004b9c
	bl Func_02004b94
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #23
	movs r1, #0
	bl Func_02004b4c
	movs r1, #1
	movs r0, #24
	bl Func_02004b9c
	bl Func_02004b94
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #24
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #24
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #25
	bl Func_02004b9c
	bl Func_02004b94
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #25
	bl Func_02004b54
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02004b54
	movs r1, #208
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #23
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #14
	bl Func_02004b9c
	bl Func_02004b94
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl Func_02004b4c
	movs r1, #1
	movs r0, #22
	bl Func_02004b9c
	bl Func_02004b94
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #40
	movs r0, #22
	bl Func_02004b6c
	movs r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #244
	movs r2, #208
	movs r3, #1
	movs r1, #0
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Func_02004b8c
	bl Func_02004b94
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #23
	bl Func_02004b6c
	movs r1, #0
	movs r2, #0
	movs r0, #23
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #24
	movs r1, #0
	movs r2, #0
	bl Func_02004b54
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #25
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #25
	bl Func_02004b74
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #196
	movs r1, #128
	movs r2, #208
	movs r3, #1
	lsls r2, r2, #17
	lsls r1, r1, #13
	lsls r0, r0, #17
	bl Func_02004b8c
	bl Func_02004b94
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #24
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #23
	bl Func_02004b54
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #22
	bl Func_02004b6c
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #23
	bl Func_02004b6c
	movs r1, #6
	movs r2, #50
	adds r1, #255
	movs r0, #25
	bl Func_02004b6c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #25
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #25
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #20
	movs r0, #25
	movs r1, #0
	bl Func_02004b4c
	movs r0, #22
	movs r1, #3
	bl Object_SetModeById
	movs r0, #23
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #24
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02004b54
	movs r1, #160
	movs r0, #23
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004b54
	movs r1, #192
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004b54
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #25
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	ldr r3, .L_02009530
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	movs r1, #1
	ldr r0, [r5]
	bl Func_02004b9c
	bl Func_02004b94
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_02009534
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_020094c6
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_020094c6:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_02009534
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009504
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_02009504:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #26
	ldr r1, .L_02009534
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #26
	movs r1, #2
	bl Object_SetModeById
	b .L_02009538
.L_0200952c:
	.4byte 0x00001b4c
.L_02009530:
	.4byte gPartyState
.L_02009534:
	.4byte 0x00013333
.L_02009538:
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009550
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #26
	bl ObjectMotion_ResetAndSetPosition
.L_02009550:
	movs r0, #26
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #0
	movs r0, #26
	movs r1, #0
	bl Func_02004b04
	ldr r0, [r5]
	movs r1, #1
	bl Func_02004b7c
	bl Func_02004b94
	bl Func_02004a9c
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02009574,"ax",%progbits
	.global Func_02001574
	.thumb_func
Func_02001574:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl Func_020049fc
	cmp r0, #0
	bne .L_02009586
	b .L_020097dc
.L_02009586:
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl Func_020049fc
	cmp r0, #0
	beq .L_02009596
	b .L_020097dc
.L_02009596:
	bl Func_02004a94
	movs r0, #0
	bl Func_02004be4
	ldr r0, .L_020097e0
	bl Func_02004b3c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #172
	bl Func_02004a04
	movs r1, #132
	movs r2, #224
	movs r0, #4
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #16
	movs r3, #128
	movs r0, #26
	movs r1, #0
	negs r2, r2
	lsls r3, r3, #7
	bl Func_02004bec
	movs r3, #128
	movs r0, #5
	movs r1, #16
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02004bec
	movs r2, #16
	movs r3, #192
	movs r0, #6
	movs r1, #16
	negs r2, r2
	lsls r3, r3, #7
	bl Func_02004bec
	movs r0, #7
	bl Func_020049fc
	cmp r0, #0
	beq .L_02009604
	movs r3, #192
	movs r0, #7
	movs r1, #8
	movs r2, #16
	lsls r3, r3, #8
	bl Func_02004bec
.L_02009604:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #0
	movs r1, #0
	movs r0, #4
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #26
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #26
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_02004b6c
	movs r0, #6
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl Func_02004b54
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl Func_02004b54
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r2, #0
	movs r1, #0
	movs r0, #4
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #26
	bl Func_02004b6c
	movs r0, #26
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #7
	bl Func_020049fc
	cmp r0, #0
	beq .L_02009716
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #7
	ldr r1, .L_020097e4
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_020097e8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009706
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_02009706:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
.L_02009716:
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #5
	ldr r1, .L_020097e4
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_020097e8
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200974c
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_0200974c:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_020097e4
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200978a
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_0200978a:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #26
	ldr r1, .L_020097e4
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #26
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_020097c8
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #26
	bl ObjectMotion_ResetAndSetPosition
.L_020097c8:
	movs r0, #26
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	bl Func_02004a9c
.L_020097dc:
	pop {r5, pc}
	.2byte 0x0000
.L_020097e0:
	.4byte 0x00001cdc
.L_020097e4:
	.4byte 0x00013333
.L_020097e8:
	.4byte gPartyState
	.section .text.x020097ec,"ax",%progbits
	.global Func_020017ec
	.thumb_func
Func_020017ec:
	push {r5, lr}
	bl Func_02004a94
	movs r0, #0
	bl Func_02004be4
	ldr r0, .L_02009b8c
	bl Func_02004b3c
	movs r2, #196
	movs r0, #4
	movs r1, #232
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #16
	movs r3, #128
	movs r0, #26
	movs r1, #0
	negs r2, r2
	lsls r3, r3, #7
	bl Func_02004bec
	movs r3, #128
	movs r0, #5
	movs r1, #16
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02004bec
	movs r2, #16
	movs r3, #192
	movs r0, #6
	movs r1, #16
	negs r2, r2
	lsls r3, r3, #7
	bl Func_02004bec
	movs r0, #7
	bl Func_020049fc
	cmp r0, #0
	beq .L_02009850
	movs r3, #192
	movs r0, #7
	movs r1, #8
	movs r2, #16
	lsls r3, r3, #8
	bl Func_02004bec
.L_02009850:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #4
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #26
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #26
	bl Func_02004b44
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020098ea
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_02004b6c
	movs r0, #6
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004b54
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #5
	bl Func_02004b54
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl Func_02004b4c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	b .L_02009938
.L_020098ea:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #40
	adds r3, #2
	strh r3, [r2]
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #6
	movs r1, #0
	bl Func_02004b4c
	movs r0, #5
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
.L_02009938:
	movs r0, #7
	bl Func_020049fc
	cmp r0, #0
	beq .L_0200996c
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004b54
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004b54
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	bl Func_02004b5c
	movs r0, #10
	bl Battle_WaitMode0
	b .L_02009a2e
.L_0200996c:
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #26
	bl Func_02004b6c
	movs r0, #26
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004b54
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004b54
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #6
	bl Func_02004b54
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #26
	bl Func_02004b6c
	movs r0, #26
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #26
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #26
	bl Func_02004b6c
	movs r0, #26
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004b54
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #6
	bl Func_02004b54
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004b54
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #6
	bl Func_02004b54
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #26
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #26
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
.L_02009a2e:
	ldr r0, .L_02009b90
	bl Func_02004b3c
	movs r1, #3
	movs r0, #26
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #26
	movs r1, #0
	bl Func_02004b4c
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #4
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	bl Func_020049fc
	cmp r0, #0
	beq .L_02009ac2
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #7
	ldr r1, .L_02009b94
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_02009b98
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009ab2
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_02009ab2:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
.L_02009ac2:
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #5
	ldr r1, .L_02009b94
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_02009b98
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009af8
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_02009af8:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_02009b94
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009b36
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_02009b36:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #26
	ldr r1, .L_02009b94
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #26
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009b74
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #26
	bl ObjectMotion_ResetAndSetPosition
.L_02009b74:
	movs r0, #26
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	bl Func_02004a9c
	pop {r5, pc}
	.2byte 0x0000
.L_02009b8c:
	.4byte 0x00001ce1
.L_02009b90:
	.4byte 0x00001cea
.L_02009b94:
	.4byte 0x00013333
.L_02009b98:
	.4byte gPartyState
	.section .text.x02009b9c,"ax",%progbits
	.global Func_02001b9c
	.thumb_func
Func_02001b9c:
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
	bl Func_02004a1c
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02009be8
	movs r1, #1
	ldr r5, [r6, #80]
	bl Func_02004a0c
	ldr r1, .L_02009bf0
	adds r0, r6, #0
	bl Func_02004a14
	movs r1, #1
	adds r0, r6, #0
	bl Func_02004a74
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r3, [sp, #16]
	ldr r1, .L_02009bec
	adds r2, #9
	strh r3, [r2]
	strb r1, [r5, #26]
	strh r7, [r5, #18]
.L_02009be8:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009bec:
	.4byte 0x00000000
.L_02009bf0:
	.4byte Data_020056d4
	.section .text.x02009bf4,"ax",%progbits
	.global Func_02001bf4
	.thumb_func
Func_02001bf4:
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
	bl Func_02001b9c
	movs r0, #151
	bl Func_02004c9c
	movs r0, #15
	bl Battle_WaitMode0
	ldr r0, [r5, #8]
	ldr r3, .L_02009cac
	ldr r1, [r5, #12]
	adds r0, r0, r3
	movs r3, #240
	ldr r2, [r5, #16]
	add r1, r8
	lsls r3, r3, #8
	str r6, [sp, #0]
	mov r9, r3
	bl Func_02001b9c
	movs r0, #151
	bl Func_02004c9c
	movs r0, #15
	bl Battle_WaitMode0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	ldr r2, [r5, #16]
	add r1, r8
	mov r3, r11
	add r0, r10
	str r6, [sp, #0]
	bl Func_02001b9c
	movs r0, #151
	bl Func_02004c9c
	movs r0, #15
	bl Battle_WaitMode0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	ldr r3, .L_02009cac
	ldr r2, [r5, #16]
	add r1, r8
	adds r0, r0, r3
	mov r3, r9
	str r6, [sp, #0]
	bl Func_02001b9c
	movs r0, #151
	bl Func_02004c9c
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
.L_02009cac:
	.4byte 0xfff80000
	.section .text.x02009cb0,"ax",%progbits
	.global Func_02001cb0
	.thumb_func
Func_02001cb0:
	push {lr}
	bl Func_02004a94
	movs r0, #0
	bl Func_02004be4
	ldr r0, .L_0200a044
	bl Func_02004b3c
	movs r1, #179
	movs r2, #179
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #28
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #179
	movs r2, #179
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #29
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #23
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #24
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #164
	movs r2, #154
	movs r0, #28
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02004b04
	movs r1, #224
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004b54
	movs r1, #148
	movs r2, #154
	movs r0, #29
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02004b04
	movs r1, #224
	movs r0, #29
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004b54
	movs r0, #28
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004b54
	movs r0, #164
	movs r1, #1
	movs r2, #158
	movs r3, #1
	lsls r2, r2, #18
	negs r1, r1
	lsls r0, r0, #17
	bl Func_02004b8c
	bl Func_02004b94
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #28
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #28
	bl Func_02004b74
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #29
	bl Func_02004b6c
	movs r0, #28
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #28
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #28
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #28
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r2, #0
	movs r1, #0
	movs r0, #29
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #29
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #28
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #29
	bl Func_02004b54
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #28
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #160
	movs r0, #29
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	movs r1, #0
	movs r2, #0
	movs r0, #29
	bl Func_02004b04
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #132
	movs r2, #192
	movs r0, #23
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02004b04
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl Func_02004b54
	movs r1, #140
	movs r2, #196
	movs r0, #24
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02004b04
	movs r0, #24
	movs r1, #0
	movs r2, #0
	bl Func_02004b54
	movs r1, #232
	movs r2, #192
	movs r0, #4
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_02004b04
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl Func_02004b54
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #23
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #23
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #132
	movs r1, #1
	movs r2, #196
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Func_02004b8c
	bl Func_02004b94
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #16
	negs r1, r1
	movs r2, #0
	movs r0, #23
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #23
	bl Func_02004b44
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009f18
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #23
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #23
	movs r1, #0
	bl Func_02004b4c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009f48
.L_02009f18:
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #4
	movs r0, #23
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #23
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
.L_02009f48:
	movs r1, #0
	movs r2, #0
	movs r0, #23
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #24
	bl Func_02004b54
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #45
	movs r0, #24
	bl Func_02004b6c
	movs r0, #24
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r2, #23
	movs r1, #6
	movs r0, #24
	bl ObjectMotion_Launch
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #23
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #23
	movs r1, #0
	bl Func_02004b4c
	movs r1, #3
	movs r0, #24
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #24
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #8
	movs r0, #23
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #4
	movs r1, #23
	bl Object_LinkObjectAndSetCallback
	movs r1, #96
	movs r0, #24
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #96
	movs r0, #23
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #23
	movs r1, #0
	movs r2, #80
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #32
	movs r0, #24
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r2, #80
	movs r0, #24
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #4
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #24
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	movs r2, #0
	movs r0, #23
	movs r1, #0
	bl Func_02004b04
	movs r0, #4
	movs r1, #1
	bl Func_02004b7c
	bl Func_02004b94
	bl Func_02004a9c
	pop {pc}
.L_0200a044:
	.4byte 0x000024c1
	.section .text.x0200a048,"ax",%progbits
	.global Func_02002048
	.thumb_func
Func_02002048:
	push {r5, lr}
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl Func_020049fc
	cmp r0, #0
	bne .L_0200a05c
	bl .L_0200aa00
.L_0200a05c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #5
	bl Func_02004a04
	bl Func_02004a94
	movs r0, #0
	bl Func_02004be4
	ldr r0, .L_0200a46c
	bl Func_02004b3c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl Func_02004b84
	movs r1, #224
	movs r2, #158
	movs r0, #25
	lsls r1, r1, #14
	lsls r2, r2, #18
	bl Func_02004b04
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl Func_02004b54
	movs r1, #224
	movs r2, #162
	movs r0, #27
	lsls r1, r1, #14
	lsls r2, r2, #18
	bl Func_02004b04
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_02004b54
	movs r1, #192
	movs r2, #154
	movs r0, #28
	lsls r1, r1, #14
	lsls r2, r2, #18
	bl Func_02004b04
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_02004b54
	movs r1, #128
	movs r2, #152
	movs r0, #29
	lsls r1, r1, #14
	lsls r2, r2, #18
	bl Func_02004b04
	movs r1, #0
	movs r2, #0
	movs r0, #29
	bl Func_02004b54
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #144
	movs r1, #1
	movs r2, #166
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #18
	lsls r0, r0, #16
	bl Func_02004b8c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #28
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #29
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
	movs r0, #27
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #25
	movs r1, #32
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #27
	movs r1, #32
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #28
	movs r1, #24
	movs r2, #24
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #24
	movs r2, #24
	movs r0, #29
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #28
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #29
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_02004b54
	movs r1, #0
	movs r2, #0
	movs r0, #29
	bl Func_02004b54
	bl Func_02004b94
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #0
	movs r0, #28
	bl Func_02004b6c
	movs r1, #8
	adds r1, #255
	movs r2, #40
	movs r0, #28
	bl Func_02004b6c
	movs r1, #192
	movs r2, #192
	movs r0, #28
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #192
	movs r2, #192
	lsls r2, r2, #9
	movs r0, #29
	lsls r1, r1, #10
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_0200a470
	movs r0, #28
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a474
	movs r0, #29
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #28
	bl Object_RefreshSelectorById
	movs r0, #29
	bl Object_RefreshSelectorById
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #28
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #28
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #29
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #29
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #29
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #28
	ldr r1, .L_0200a478
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #29
	ldr r1, .L_0200a478
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #25
	ldr r1, .L_0200a478
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #27
	ldr r1, .L_0200a478
	bl ObjectMotion_SetSpeedParameters
	movs r0, #25
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #25
	lsls r1, r1, #1
	bl Func_02004b74
	movs r0, #27
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #27
	bl Func_02004b74
	movs r0, #40
	bl Battle_WaitMode0
	ldr r1, .L_0200a47c
	movs r0, #28
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a480
	movs r0, #29
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #1
	bl Func_02004b7c
	movs r0, #25
	movs r1, #64
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #27
	movs r1, #64
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #154
	movs r0, #25
	movs r1, #168
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r2, #154
	lsls r2, r2, #2
	movs r1, #184
	movs r0, #27
	bl ObjectMotion_SetPositionAndReset
	movs r0, #25
	bl Object_RefreshSelectorById
	movs r0, #25
	movs r1, #1
	bl Object_SetModeById
	movs r1, #224
	movs r0, #25
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004b54
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #27
	bl Func_02004b54
	movs r0, #28
	bl Object_RefreshSelectorById
	movs r0, #29
	bl Object_RefreshSelectorById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #25
	lsls r1, r1, #1
	bl Func_02004b74
	movs r0, #27
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #27
	bl Func_02004b74
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl Func_02004b84
	movs r0, #192
	movs r1, #1
	movs r2, #240
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Func_02004b8c
	bl Func_02004b94
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl Func_02004b4c
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl Func_02004b84
	movs r0, #240
	movs r1, #1
	movs r2, #172
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Func_02004b8c
	bl Func_02004b94
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl Func_02004b4c
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl Func_02004b84
	movs r0, #244
	movs r1, #1
	movs r2, #228
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Func_02004b8c
	bl Func_02004b94
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl Func_02004b4c
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl Func_02004b84
	movs r0, #192
	movs r1, #1
	movs r2, #154
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #18
	lsls r0, r0, #16
	bl Func_02004b8c
	bl Func_02004b94
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #25
	bl Func_02004b6c
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #25
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #192
	movs r0, #28
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02004b54
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #29
	bl Func_02004b54
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #28
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #29
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #27
	bl Func_02004b54
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_02004b6c
	movs r0, #27
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	b .L_0200a484
.L_0200a46c:
	.4byte 0x000024ce
.L_0200a470:
	.4byte Data_020054f4
.L_0200a474:
	.4byte Data_0200556c
.L_0200a478:
	.4byte 0x00019999
.L_0200a47c:
	.4byte Data_0200d5d0
.L_0200a480:
	.4byte Data_02005620
.L_0200a484:
	movs r2, #0
	movs r1, #0
	movs r0, #25
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl Func_02004b4c
	movs r0, #28
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #29
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #208
	movs r0, #25
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004b54
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #27
	bl Func_02004b54
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #25
	bl Func_02004b6c
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl Func_02004b4c
	movs r0, #28
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #28
	lsls r1, r1, #1
	bl Func_02004b74
	movs r0, #29
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #29
	bl Func_02004b74
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #176
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004b54
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #29
	bl Func_02004b54
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #28
	bl Func_02004b6c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #29
	bl Func_02004b6c
	movs r1, #176
	movs r0, #25
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004b54
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #27
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #144
	movs r1, #1
	movs r2, #136
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #18
	lsls r0, r0, #16
	bl Func_02004b8c
	bl Func_02004b94
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_02004b6c
	movs r1, #192
	movs r2, #192
	movs r0, #25
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #192
	movs r2, #192
	movs r0, #27
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #32
	movs r2, #32
	movs r0, #25
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #32
	movs r2, #32
	movs r0, #27
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #24
	movs r0, #25
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #24
	movs r1, #0
	negs r2, r2
	movs r0, #27
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #25
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #208
	movs r0, #25
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004b54
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #27
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #25
	bl Func_02004b6c
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r2, #130
	movs r0, #4
	movs r1, #136
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r3, #160
	lsls r3, r3, #7
	movs r0, #26
	movs r1, #16
	movs r2, #0
	bl Func_02004bec
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #4
	bl Func_02004b54
	movs r0, #26
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #26
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #26
	movs r1, #0
	bl Func_02004b4c
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl Func_02004b4c
	movs r0, #26
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #26
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #25
	bl Func_02004b6c
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #26
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #26
	movs r1, #0
	bl Func_02004b4c
	movs r1, #2
	movs r0, #27
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #26
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004b54
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #27
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #27
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #25
	bl Func_02004b6c
	movs r1, #0
	movs r2, #0
	movs r0, #25
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #192
	movs r0, #27
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02004b54
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #25
	bl Func_02004b54
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
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl Func_02004b4c
	movs r1, #2
	movs r0, #28
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #28
	movs r1, #0
	bl Func_02004b4c
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #29
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #29
	movs r1, #0
	bl Func_02004b4c
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #0
	movs r2, #20
	bl Func_02004b4c
	movs r0, #28
	movs r1, #0
	movs r2, #64
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #0
	movs r2, #64
	movs r0, #29
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_02004b04
	movs r1, #176
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004b54
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #25
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl Func_02004b4c
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #25
	bl Func_02004b44
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	beq .L_0200a88a
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #26
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #26
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #26
	movs r1, #0
	bl Func_02004b4c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a8d0
.L_0200a88a:
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #26
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #26
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #6
	strh r3, [r2]
	adds r0, #26
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
.L_0200a8d0:
	movs r2, #0
	movs r1, #0
	movs r0, #4
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004b54
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #26
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02004b54
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #27
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #4
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #26
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #25
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #204
	adds r2, #102
	movs r0, #27
	bl ObjectMotion_SetSpeedParameters
	movs r0, #4
	bl Object_GetById
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r0, #26
	bl Object_GetById
	adds r0, #85
	strb r5, [r0]
	movs r0, #25
	bl Object_GetById
	adds r0, #85
	strb r5, [r0]
	movs r0, #27
	bl Object_GetById
	adds r0, #85
	strb r5, [r0]
	movs r1, #0
	movs r0, #4
	movs r2, #120
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #26
	movs r1, #0
	movs r2, #120
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #27
	movs r1, #0
	movs r2, #80
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #25
	movs r1, #0
	movs r2, #80
	bl ObjectMotion_CommitPositionAndActivate
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r3, #218
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #60
	str r3, [r2]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #172
	str r3, [r2]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r0, .L_0200aa04
	movs r1, #81
	bl Func_02004ba4
	bl Func_02004a9c
.L_0200aa00:
	pop {r5, pc}
	.2byte 0x0000
.L_0200aa04:
	.4byte 0x00000061
	.section .text.x0200aa08,"ax",%progbits
	.global Func_02002a08
	.thumb_func
Func_02002a08:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	sub sp, #8
	bl Func_020049fc
	cmp r0, #0
	bne .L_0200aa22
	bl .L_0200b260
.L_0200aa22:
	movs r0, #129
	lsls r0, r0, #4
	adds r0, #255
	bl Func_02004a04
	movs r0, #136
	lsls r0, r0, #4
	adds r0, #255
	bl Func_02004a04
	bl Func_02004a94
	movs r0, #0
	bl Func_02004be4
	ldr r0, .L_0200ae3c
	bl Func_02004b3c
	movs r0, #29
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r2, #138
	movs r0, #4
	movs r1, #136
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #16
	movs r3, #160
	lsls r3, r3, #8
	movs r0, #26
	negs r1, r1
	movs r2, #0
	bl Func_02004bec
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl Func_02004b54
	movs r0, #26
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #26
	bl Func_02004b6c
	movs r0, #176
	movs r1, #1
	movs r2, #196
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #16
	bl Func_02004b8c
	bl Func_02004b94
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #25
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl Func_02004b4c
	movs r1, #2
	movs r0, #27
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #27
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #224
	movs r2, #128
	movs r0, #22
	lsls r1, r1, #16
	lsls r2, r2, #12
	movs r5, #192
	bl Func_02004b04
	lsls r5, r5, #18
	movs r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
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
	movs r0, #128
	adds r3, #8
	movs r1, #128
	str r2, [r3]
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl Func_02004b84
	movs r0, #208
	movs r1, #1
	movs r2, #168
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #16
	bl Func_02004b8c
	movs r0, #50
	bl Battle_WaitMode0
	movs r0, #78
	bl Func_02004c9c
	bl Event_ClearStatus1c6
	movs r0, #16
	bl Func_02004bc4
	bl Event_WaitValue1c8Frames
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Func_02004b8c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #202
	movs r1, #1
	movs r2, #166
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #0
	bl Func_02004b8c
	ldr r3, [r5, #108]
	movs r1, #230
	lsls r1, r1, #1
	adds r3, r3, r1
	ldr r0, [r3]
	movs r5, #15
	ldr r3, [r0, #16]
	ldr r2, [r0, #12]
	ldr r1, [r0, #8]
	bl Object_SetPositionAndResetMotion
	movs r0, #50
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #1
	bl Func_02004b64
	movs r0, #23
	movs r1, #1
	bl Func_02004b64
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #23
	ldr r1, .L_0200ae40
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #22
	ldr r1, .L_0200ae40
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #198
	movs r2, #214
	movs r0, #22
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02004b04
	movs r1, #206
	movs r2, #214
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02004b04
	movs r1, #186
	movs r2, #254
	movs r0, #26
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02004b04
	movs r1, #186
	movs r2, #254
	lsls r2, r2, #18
	lsls r1, r1, #18
	movs r0, #25
	bl Func_02004b04
	movs r0, #35
	bl Func_02004c9c
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02004bbc
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl Func_02004b84
	movs r0, #202
	movs r1, #1
	movs r2, #212
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #18
	lsls r0, r0, #18
	bl Func_02004b8c
	bl Func_02004b94
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #22
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #22
	movs r1, #0
	bl Func_02004b4c
	movs r1, #3
	movs r0, #23
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02004b54
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #23
	bl Func_02004b54
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #23
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #22
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004b54
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #23
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #26
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #23
	bl Func_02004b6c
	movs r1, #128
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #22
	bl Func_02004b6c
	movs r0, #22
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #22
	bl Func_02004b74
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #16
	movs r0, #22
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #22
	bl Func_02004b6c
	movs r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #23
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #23
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #23
	bl Func_02004b6c
	movs r0, #23
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #23
	bl Func_02004b54
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #23
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #0
	movs r2, #0
	movs r0, #22
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #45
	movs r0, #22
	bl Func_02004b6c
	movs r1, #0
	movs r2, #32
	movs r0, #22
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #45
	movs r0, #22
	bl Func_02004b6c
	movs r2, #32
	movs r0, #22
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #0
	movs r1, #0
	movs r0, #22
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #23
	bl Func_02004b54
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #23
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #23
	bl Func_02004b6c
	movs r0, #23
	movs r1, #0
	b .L_0200ae44
.L_0200ae3c:
	.4byte 0x0000253b
.L_0200ae40:
	.4byte 0x00013333
.L_0200ae44:
	movs r2, #10
	bl Func_02004b4c
	movs r1, #2
	adds r1, #255
	movs r2, #50
	movs r0, #22
	bl Func_02004b6c
	movs r1, #0
	movs r2, #32
	movs r0, #22
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #22
	bl Func_02004b6c
	movs r2, #32
	movs r0, #22
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #0
	movs r1, #0
	movs r0, #22
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #23
	bl Func_02004b6c
	movs r0, #23
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #22
	bl Func_02004b54
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #22
	bl Func_02001bf4
	movs r0, #103
	bl Func_02004c9c
	movs r1, #6
	adds r1, #255
	movs r2, #50
	movs r0, #22
	bl Func_02004b6c
	movs r2, #0
	movs r1, #0
	movs r0, #22
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #23
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #22
	bl Func_02004b54
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #22
	bl Func_02001bf4
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #22
	bl Func_02004b6c
	movs r2, #0
	movs r1, #0
	movs r0, #22
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #23
	bl Func_02004b6c
	movs r2, #10
	movs r0, #23
	movs r1, #0
	bl Func_02004b4c
	movs r1, #2
	movs r0, #22
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #22
	lsls r1, r1, #7
	bl Func_02004b5c
	movs r1, #7
	movs r0, #22
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #6
	movs r0, #22
	bl Object_SetModeById
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #23
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #23
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #23
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #129
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #22
	bl Func_02004b6c
	movs r1, #7
	movs r0, #22
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #1
	bl Object_SetModeById
	movs r1, #0
	movs r2, #0
	movs r0, #22
	bl Func_02004b54
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #23
	bl Func_02004b6c
	movs r0, #23
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #22
	bl Func_02004b6c
	movs r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #22
	movs r1, #0
	movs r2, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #22
	movs r1, #10
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r0, #23
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004b54
	movs r0, #22
	movs r1, #6
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #22
	bl Func_02004b54
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #22
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #26
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #22
	bl Func_02004b6c
	movs r0, #128
	lsls r0, r0, #7
	movs r2, #20
	adds r0, #22
	movs r1, #0
	bl Func_02004b4c
	movs r1, #8
	movs r0, #22
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #22
	movs r1, #7
	movs r2, #0
	bl ObjectMotion_Launch
.L_0200b0e0:
	movs r0, #22
	bl Object_GetById
	ldr r2, .L_0200b268
	ldrh r3, [r0, #6]
	subs r5, #1
	adds r3, r3, r2
	strh r3, [r0, #6]
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200b0e0
	movs r1, #5
	movs r0, #22
	bl Object_SetModeById
	movs r0, #7
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #0
	movs r2, #20
	bl Func_02004b4c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #23
	bl Func_02004b6c
	movs r2, #36
	movs r1, #0
	movs r0, #23
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #22
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #23
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #7
	movs r2, #15
	adds r0, #23
	movs r1, #0
	bl Func_02004b4c
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #9
	movs r0, #22
	bl Object_SetModeById
	movs r0, #70
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #22
	ldr r7, [r3, #32]
	bl Object_GetById
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r0, #23
	bl Object_GetById
	movs r3, #64
	adds r0, #85
	strb r5, [r0]
	mov r8, r3
	str r3, [sp, #4]
	movs r5, #42
	movs r0, #42
	movs r1, #60
	movs r2, #17
	movs r3, #4
	str r5, [sp, #0]
	bl Func_02004a5c
	movs r6, #68
	movs r0, #42
	movs r1, #60
	movs r2, #17
	movs r3, #4
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004a5c
	mov r1, r8
	movs r5, #106
	str r1, [sp, #4]
	movs r0, #106
	movs r1, #60
	movs r2, #17
	movs r3, #4
	str r5, [sp, #0]
	bl Func_02004a5c
	movs r0, #106
	movs r1, #60
	movs r2, #17
	movs r3, #4
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004a5c
	movs r5, #239
.L_0200b1f2:
	movs r3, #138
	lsls r3, r3, #1
	adds r2, r7, r3
	ldr r3, [r2]
	movs r1, #128
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r2]
	movs r3, #166
	lsls r3, r3, #1
	adds r2, r7, r3
	ldr r3, [r2]
	movs r0, #22
	adds r3, r3, r1
	str r3, [r2]
	bl Object_GetById
	ldr r1, .L_0200b26c
	ldr r3, [r0, #16]
	subs r5, #1
	adds r3, r3, r1
	str r3, [r0, #16]
	movs r0, #23
	bl Object_GetById
	ldr r2, .L_0200b26c
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200b1f2
	movs r0, #78
	bl Func_02004c9c
	movs r0, #30
	bl Battle_WaitMode0
	bl Func_02004a9c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Func_02004b8c
	ldr r0, .L_0200b270
	movs r1, #99
	bl Func_02004ba4
.L_0200b260:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200b268:
	.4byte 0xfffff000
.L_0200b26c:
	.4byte 0xffff0000
.L_0200b270:
	.4byte 0x00000062
	.section .text.x0200b274,"ax",%progbits
	.global Func_02003274
	.thumb_func
Func_02003274:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	bl Func_02004a94
	movs r0, #0
	bl Func_02004be4
	ldr r0, .L_0200b688
	bl Func_02004b3c
	movs r0, #152
	movs r1, #1
	movs r2, #136
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #0
	bl Func_02004b8c
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #230
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r0, [r3]
	movs r5, #254
	ldr r3, [r0, #16]
	ldr r2, [r0, #12]
	ldr r1, [r0, #8]
	bl Object_SetPositionAndResetMotion
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02004bbc
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #152
	movs r1, #1
	movs r2, #196
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #16
	bl Func_02004b8c
	bl Func_02004b94
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #25
	bl Func_02004b6c
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #25
	bl Func_02004b54
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #25
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r1, #6
	movs r2, #23
	movs r0, #25
	bl ObjectMotion_Launch
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #25
	bl Func_02004b6c
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #25
	bl Func_02004b54
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #25
	bl Func_02004b6c
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #51
	adds r2, #153
	movs r0, #25
	bl ObjectMotion_SetSpeedParameters
	movs r0, #25
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r2, #8
	movs r1, #0
	movs r0, #25
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #25
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	strb r3, [r0]
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #25
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	ldr r1, .L_0200b68c
	ldr r2, .L_0200b690
	movs r0, #25
	bl ObjectMotion_SetSpeedParameters
	movs r0, #25
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r2, #16
	strb r3, [r0]
	movs r1, #0
	negs r2, r2
	movs r0, #25
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #25
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r7, #0
	orrs r3, r6
	strb r3, [r0]
	movs r0, #25
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r2, #8
	movs r0, #25
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #133
	bl Func_02004c9c
	ldr r1, .L_0200b68c
	ldr r2, .L_0200b690
	movs r0, #29
	bl ObjectMotion_SetSpeedParameters
	movs r0, #29
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r2, #4
	movs r1, #0
	strb r3, [r0]
	negs r2, r2
	movs r0, #29
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #29
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	movs r0, #29
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #8
	mov r11, r3
	mov r2, r11
	strh r2, [r0, #6]
	movs r0, #29
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r2, #4
	movs r1, #0
	strb r3, [r0]
	negs r2, r2
	movs r0, #29
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #29
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	movs r0, #29
	bl Object_GetById
	movs r3, #176
	lsls r3, r3, #8
	mov r8, r3
	mov r2, r8
	strh r2, [r0, #6]
	movs r0, #29
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r2, #4
	movs r1, #0
	strb r3, [r0]
	negs r2, r2
	movs r0, #29
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #29
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	movs r0, #29
	bl Object_GetById
	movs r3, #208
	lsls r3, r3, #8
	mov r9, r3
	mov r2, r9
	strh r2, [r0, #6]
	movs r0, #29
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r2, #4
	movs r1, #0
	negs r2, r2
	strb r3, [r0]
	movs r0, #29
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #29
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	movs r0, #29
	bl Object_GetById
	strh r7, [r0, #6]
	movs r0, #29
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r2, #4
	movs r1, #0
	strb r3, [r0]
	negs r2, r2
	movs r0, #29
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #29
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	movs r0, #29
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #6
	mov r10, r3
	mov r2, r10
	strh r2, [r0, #6]
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #25
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #0
	orrs r3, r6
	strb r3, [r0]
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #25
	movs r2, #10
	bl Func_02004b4c
	movs r1, #168
	movs r2, #184
	lsls r2, r2, #17
	movs r0, #29
	lsls r1, r1, #16
	bl Func_02004b04
	movs r1, #0
	movs r0, #29
	bl Func_02004c54
	movs r0, #29
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #160
	movs r0, #29
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004b54
	movs r0, #29
	movs r1, #6
	movs r2, #40
	bl ObjectMotion_Launch
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #29
	bl Func_02004b6c
	mov r1, r10
	movs r2, #0
	movs r0, #25
	bl Func_02004b54
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #51
	adds r2, #153
	movs r0, #25
	bl ObjectMotion_SetSpeedParameters
	movs r0, #25
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r2, #8
	strb r3, [r0]
	movs r1, #0
	negs r2, r2
	movs r0, #25
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #25
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	movs r0, #20
	bl Battle_WaitMode0
	ldr r1, .L_0200b68c
	ldr r2, .L_0200b690
	movs r0, #25
	bl ObjectMotion_SetSpeedParameters
	movs r0, #25
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r2, #16
	movs r0, #25
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #25
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	movs r0, #133
	bl Func_02004c9c
	ldr r1, .L_0200b68c
	ldr r2, .L_0200b690
	movs r0, #29
	b .L_0200b694
	.2byte 0x0000
.L_0200b688:
	.4byte 0x00002559
.L_0200b68c:
	.4byte 0x00026666
.L_0200b690:
	.4byte 0x00013333
.L_0200b694:
	bl ObjectMotion_SetSpeedParameters
	movs r0, #27
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r1, #0
	movs r2, #3
	strb r3, [r0]
	movs r0, #27
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #27
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	movs r0, #27
	bl Object_GetById
	strh r7, [r0, #6]
	movs r0, #27
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r1, #0
	movs r2, #3
	strb r3, [r0]
	movs r0, #27
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #27
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	movs r0, #27
	bl Object_GetById
	mov r3, r9
	strh r3, [r0, #6]
	movs r0, #27
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r1, #0
	strb r3, [r0]
	movs r2, #3
	movs r0, #27
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #27
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	movs r0, #27
	bl Object_GetById
	mov r2, r8
	strh r2, [r0, #6]
	movs r0, #27
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r1, #0
	movs r2, #3
	strb r3, [r0]
	movs r0, #27
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #27
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	movs r0, #27
	bl Object_GetById
	mov r3, r11
	strh r3, [r0, #6]
	movs r0, #27
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #0
	ands r5, r3
	movs r2, #3
	strb r5, [r0]
	movs r0, #27
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #27
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r6, r3
	strb r6, [r0]
	movs r0, #27
	bl Object_GetById
	movs r6, #160
	lsls r6, r6, #7
	strh r6, [r0, #6]
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #168
	movs r2, #212
	lsls r2, r2, #17
	movs r0, #27
	lsls r1, r1, #16
	bl Func_02004b04
	movs r1, #0
	movs r0, #27
	bl Func_02004c54
	movs r0, #27
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #27
	bl Object_GetById
	mov r2, r8
	strh r2, [r0, #6]
	movs r1, #6
	movs r0, #27
	movs r2, #40
	bl ObjectMotion_Launch
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #27
	bl Func_02004b6c
	mov r1, r9
	movs r2, #0
	movs r0, #25
	bl Func_02004b54
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #25
	bl Func_02004b6c
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #10
	adds r1, #255
	movs r2, #50
	movs r0, #29
	bl Func_02004b6c
	mov r1, r10
	movs r2, #0
	movs r0, #25
	bl Func_02004b54
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #25
	bl Func_02004b6c
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r1, #129
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #27
	bl Func_02004b6c
	movs r1, #5
	movs r0, #25
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #29
	mov r1, r10
	movs r2, #0
	bl Func_02004b54
	movs r2, #0
	mov r1, r9
	movs r0, #27
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #29
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #29
	lsls r1, r1, #1
	bl Func_02004b74
	movs r0, #27
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #27
	bl Func_02004b74
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #29
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r0, #29
	movs r1, #16
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #29
	movs r1, #0
	movs r2, #40
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #0
	mov r1, r8
	movs r0, #29
	bl Func_02004b54
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #25
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #29
	mov r1, r8
	movs r2, #0
	bl Func_02004b54
	movs r0, #27
	mov r1, r8
	movs r2, #0
	bl Func_02004b54
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #25
	movs r1, #0
	bl Func_02004b4c
	movs r0, #27
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #29
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #25
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #27
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #29
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #28
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #30
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	movs r0, #31
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #25
	movs r1, #1
	bl Func_02004b7c
	movs r1, #16
	movs r0, #25
	negs r1, r1
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #25
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #252
	movs r0, #25
	movs r1, #152
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #16
	movs r0, #29
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #16
	movs r0, #27
	negs r1, r1
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #16
	movs r0, #29
	negs r1, r1
	movs r2, #16
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #27
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #244
	movs r0, #27
	movs r1, #152
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r2, #236
	lsls r2, r2, #1
	movs r0, #29
	movs r1, #152
	bl ObjectMotion_SetPositionAndReset
	movs r0, #25
	movs r1, #1
	bl Object_SetModeById
	movs r0, #27
	movs r1, #1
	bl Object_SetModeById
	movs r0, #29
	adds r1, r6, #0
	movs r2, #0
	bl Func_02004b54
	movs r0, #27
	adds r1, r6, #0
	movs r2, #0
	bl Func_02004b54
	movs r2, #0
	adds r1, r6, #0
	movs r0, #25
	bl Func_02004b54
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #192
	lsls r0, r0, #7
	movs r2, #10
	adds r0, #25
	movs r1, #0
	bl Func_02004b4c
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #4
	lsls r1, r1, #1
	bl Func_02004b74
	movs r0, #26
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #26
	bl Func_02004b74
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #27
	movs r1, #0
	movs r2, #64
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #29
	movs r1, #0
	movs r2, #64
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #25
	movs r1, #0
	movs r2, #64
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #25
	bl Func_02004b6c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #26
	mov r1, r10
	movs r2, #0
	bl Func_02004b54
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #4
	bl Func_02004b54
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r2, #16
	movs r1, #0
	movs r0, #25
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #25
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl Func_02004b4c
	movs r0, #30
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r0, #31
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #2
	movs r0, #28
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #176
	movs r2, #148
	lsls r2, r2, #18
	movs r0, #28
	lsls r1, r1, #16
	bl Func_02004b04
	movs r1, #0
	movs r0, #28
	bl Func_02004c54
	movs r0, #28
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #28
	movs r1, #6
	movs r2, #20
	bl ObjectMotion_Launch
	movs r0, #28
	mov r1, r11
	movs r2, #0
	bl Func_02004b54
	movs r1, #128
	movs r2, #152
	lsls r2, r2, #18
	movs r0, #31
	lsls r1, r1, #16
	bl Func_02004b04
	movs r1, #0
	movs r0, #31
	bl Func_02004c54
	movs r0, #31
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #31
	movs r1, #6
	movs r2, #20
	bl ObjectMotion_Launch
	movs r0, #31
	mov r1, r9
	movs r2, #0
	bl Func_02004b54
	movs r1, #192
	movs r2, #154
	lsls r2, r2, #18
	movs r0, #30
	lsls r1, r1, #16
	bl Func_02004b04
	movs r1, #0
	movs r0, #30
	bl Func_02004c54
	movs r0, #30
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #30
	movs r1, #6
	movs r2, #20
	bl ObjectMotion_Launch
	movs r2, #0
	mov r1, r8
	movs r0, #30
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl Func_02004b4c
	movs r0, #30
	movs r1, #3
	bl Object_SetModeById
	movs r0, #31
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #28
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #25
	bl Object_LinkObjectAndSetCallback
	movs r0, #26
	movs r1, #25
	bl Object_LinkObjectAndSetCallback
	movs r0, #30
	movs r1, #25
	bl Object_LinkObjectAndSetCallback
	movs r0, #31
	movs r1, #25
	bl Object_LinkObjectAndSetCallback
	movs r0, #28
	movs r1, #25
	bl Object_LinkObjectAndSetCallback
	ldr r5, .L_0200beb8
	movs r0, #25
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #27
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #29
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #70
	bl Battle_WaitMode0
	movs r1, #16
	movs r2, #0
	movs r0, #28
	negs r1, r1
	bl ObjectMotion_CommitPositionAndActivate
	adds r1, r5, #0
	movs r0, #28
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #33
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #31
	movs r1, #16
	bl ObjectMotion_CommitPositionAndActivate
	adds r1, r5, #0
	movs r0, #31
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #15
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #30
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #25
	bl Object_RefreshSelectorById
	movs r2, #0
	adds r1, r6, #0
	movs r0, #26
	bl Func_02004b54
	movs r0, #30
	bl Object_RefreshSelectorById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #4
	bl Func_02004b7c
	bl Func_02004b94
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #6
	adds r1, #255
	movs r2, #50
	movs r0, #26
	bl Func_02004b6c
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Func_02004b54
	mov r1, r11
	movs r2, #0
	movs r0, #4
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #26
	bl Func_02004b44
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200bd3e
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #26
	movs r1, #0
	bl Func_02004b4c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200bd60
.L_0200bd3e:
	movs r0, #40
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #26
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
.L_0200bd60:
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #26
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #16
	movs r0, #26
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004b54
	movs r1, #208
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #26
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #26
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #26
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
	movs r0, #26
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004b54
	movs r1, #0
	movs r2, #0
	movs r0, #26
	bl Func_02004b54
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #26
	bl Func_02004b44
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	beq .L_0200be2a
	movs r0, #40
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #26
	movs r1, #0
	bl Func_02004b4c
	movs r1, #3
	movs r0, #26
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #26
	movs r1, #0
	bl Func_02004b4c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	b .L_0200be4c
.L_0200be2a:
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #26
	adds r3, #2
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_02004b4c
.L_0200be4c:
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #26
	ldr r1, .L_0200bebc
	bl ObjectMotion_SetSpeedParameters
	movs r0, #26
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_0200bec0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200be90
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #26
	bl ObjectMotion_ResetAndSetPosition
.L_0200be90:
	movs r0, #26
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #26
	bl Func_02004b04
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02004a9c
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200beb8:
	.4byte Data_02005670
.L_0200bebc:
	.4byte 0x00013333
.L_0200bec0:
	.4byte gPartyState
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
	.section .text.x0200bf48,"ax",%progbits
	.global Func_02003f48
	.thumb_func
Func_02003f48:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	str r2, [sp, #0]
	str r0, [sp, #8]
	str r1, [sp, #4]
	adds r2, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	mov r11, r3
	cmp r2, #0
	ble .L_0200bff4
	adds r7, r2, #0
.L_0200bf70:
	bl Random16Far
	movs r1, #176
	lsls r1, r1, #5
	add r1, r11
	movs r2, #0
	ldrsh r3, [r1, r2]
	mov r10, r1
	lsls r6, r3, #3
	subs r6, r6, r3
	lsls r6, r6, #2
	movs r3, #160
	add r6, r11
	lsls r3, r3, #3
	adds r5, r6, r3
	movs r1, #0
	str r1, [r5, #24]
	ldr r2, [sp, #8]
	mov r8, r1
	str r2, [r5]
	ldr r3, [sp, #4]
	mov r9, r0
	str r3, [r5, #4]
	ldr r1, [sp, #0]
	subs r7, #1
	str r1, [r5, #8]
	bl Random16Far
	movs r2, #128
	lsls r2, r2, #12
	lsls r0, r0, #3
	adds r0, r0, r2
	mov r1, r9
	adds r2, r5, #0
	bl Vector_AddPolarOffsetFar
	mov r3, r8
	str r3, [r5, #12]
	movs r3, #160
	lsls r3, r3, #11
	mov r1, r8
	str r3, [r5, #16]
	str r1, [r5, #20]
	bl Random16Far
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #12
	movs r2, #128
	adds r6, r6, r3
	lsls r2, r2, #10
	lsls r0, r0, #1
	adds r0, r0, r2
	mov r1, r9
	adds r2, r6, #0
	bl Vector_AddPolarOffsetFar
	mov r1, r10
	ldrh r3, [r1]
	movs r2, #63
	adds r3, #1
	ands r3, r2
	mov r2, r10
	strh r3, [r2]
	cmp r7, #0
	bne .L_0200bf70
.L_0200bff4:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200c004,"ax",%progbits
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
	bl Runtime_AllocateHeapBlockFar
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
	bl VramBlock_LoadCached
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
	bl Func_0200499c
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c0b8:
	.4byte 0x000001f0
.L_0200c0bc:
	.4byte Func_02003ec4
	.section .text.x0200c0c0,"ax",%progbits
	.global Func_020040c0
	.thumb_func
Func_020040c0:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r0, .L_0200c0e8
	ldr r5, [r3]
	bl Scheduler_RemoveCallbackFar
	movs r3, #176
	lsls r3, r3, #5
	adds r3, #4
	adds r5, r5, r3
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl Func_020049dc
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	pop {r5, pc}
.L_0200c0e8:
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
	bl Object_GetById
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
	bl Func_020041f8
	mov r2, r10
	mov r3, r8
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #2
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r7, #0
	bl Func_020041f8
	movs r0, #1
.L_0200c1e6:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c1f0:
	.4byte Data_02004ca4
.L_0200c1f4:
	.4byte Data_02004cb0
	.section .text.x0200c1f8,"ax",%progbits
	.global Func_020041f8
	.thumb_func
Func_020041f8:
	push {r5, r6, lr}
	adds r5, r3, #0
	ldr r3, [sp, #12]
	lsls r2, r2, #7
	mov r12, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r0, r0, #1
	lsls r3, r3, #3
	adds r3, r3, r0
	ldr r0, [r4, r3]
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r0, r0, r1
	movs r1, #0
	ldr r6, [sp, #16]
	cmp r1, r12
	bcs .L_0200c23e
.L_0200c224:
	lsls r3, r1, #9
	movs r2, #0
	adds r3, r0, r3
	cmp r2, r5
	bcs .L_0200c238
.L_0200c22e:
	adds r2, #1
	strb r6, [r3, #2]
	adds r3, #4
	cmp r2, r5
	bcc .L_0200c22e
.L_0200c238:
	adds r1, #1
	cmp r1, r12
	bcc .L_0200c224
.L_0200c23e:
	pop {r5, r6, pc}
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
	.4byte Data_02004cb0
.L_0200c3a0:
	.4byte Data_02004cc8
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
	.4byte gPartyState
.L_0200c4b8:
	.4byte Data_02004ca4
.L_0200c4bc:
	.4byte Data_02004cc8
.L_0200c4c0:
	.4byte Data_02004cb0
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
	bl Object_GetById
	mov r8, r0
	ldr r0, [sp, #104]
	bl Object_GetById
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
	bl Func_020041f8
	mov r1, r10
	movs r2, #200
	ldr r0, [r1]
	lsls r2, r2, #5
	movs r1, #128
	lsls r1, r1, #8
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	mov r2, r10
	ldr r0, [r2]
	movs r1, #8
	bl Object_SetModeById
	movs r0, #15
	bl WaitFrames
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
	bl ObjectMotion_OffsetPositionAndResetMotion
	mov r4, r10
	ldr r0, [r4]
	bl Object_GetById
	ldr r3, .L_0200c76c
	str r3, [r0, #108]
	movs r0, #4
	bl WaitFrames
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
	bl ObjectMotion_SetSpeedParameters
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
	ldr r2, .L_0200c770
	mov r1, r9
	lsls r3, r1, #2
	ldr r2, [r2, r3]
	ldr r0, [r5]
	lsls r2, r2, #16
	asrs r1, r2, #31
	asrs r2, r2, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	ldr r3, [sp, #108]
	cmp r3, #0
	beq .L_0200c64c
	mov lr, r3
	.2byte 0xf800
.L_0200c64c:
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #1
	ldr r0, [r5]
	bl Object_SetModeById
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
	bl Object_SetPositionAndResetMotion
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
	bl Func_020041f8
	mov r3, r11
	ldr r1, [sp, #88]
	ldr r2, [sp, #96]
	str r3, [sp, #0]
	movs r0, #2
	ldr r3, [sp, #24]
	str r5, [sp, #4]
	bl Func_020041f8
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
	bl Func_020041f8
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
	.4byte gPartyState
.L_0200c768:
	.4byte Data_02004cb0
.L_0200c76c:
	.4byte Func_02004774
.L_0200c770:
	.4byte Data_02004cc8
	.section .text.x0200c774,"ax",%progbits
	.global Func_02004774
	.thumb_func
Func_02004774:
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
	bl Func_02004c3c
	cmp r0, #0
	beq .L_0200c7d0
	movs r4, #0
.L_0200c7ac:
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	movs r2, #0
	ldrsh r1, [r3, r2]
	ldr r2, .L_0200c7f8
	lsls r3, r4, #1
	ldrh r3, [r2, r3]
	cmp r1, r3
	beq .L_0200c7f4
	adds r4, #1
	cmp r4, #5
	bls .L_0200c7ac
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
.L_0200c7d0:
	ldr r3, [r5, #8]
	adds r0, r5, #0
	str r3, [r6]
	ldr r3, [r5, #12]
	adds r1, r6, #0
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	str r3, [r6, #8]
	bl Func_02004a64
	cmp r0, #0
	ble .L_0200c7f4
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
.L_0200c7f4:
	add sp, #12
	pop {r5, r6, pc}
.L_0200c7f8:
	.4byte Data_02004ca4
	.section .text.x0200c7fc,"ax",%progbits
	.global Func_020047fc
	.thumb_func
Func_020047fc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	sub sp, #4
	cmp r3, r2
	beq .L_0200c860
	adds r7, r0, #0
.L_0200c812:
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
	bl ObjectDispatch_SetSingleChildField26
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
	bl Func_020048ec
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200c812
.L_0200c860:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
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
	bl Object_GetById
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
	bl Func_02004a04
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
	.section .text.x0200c8ec,"ax",%progbits
	.global Func_020048ec
	.thumb_func
Func_020048ec:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r3, #0
	mov r8, r2
	adds r6, r1, #0
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	lsls r3, r5, #3
	subs r3, r3, r5
	movs r1, #156
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r5, [r2, r3]
	adds r7, r0, #0
	bl Func_02004c34
	lsls r0, r0, #2
	adds r5, r5, r0
	mov r0, r8
	bl Func_020049fc
	cmp r0, #0
	beq .L_0200c960
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #0
	strb r3, [r5, #2]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	cmp r6, #1
	beq .L_0200c94c
	cmp r6, #1
	bcc .L_0200c942
	cmp r6, #2
	beq .L_0200c956
	b .L_0200c98e
.L_0200c942:
	adds r0, r7, #0
	movs r1, #2
	bl Func_02004a0c
	b .L_0200c98e
.L_0200c94c:
	adds r0, r7, #0
	movs r1, #4
	bl Func_02004a0c
	b .L_0200c98e
.L_0200c956:
	adds r0, r7, #0
	movs r1, #6
	bl Func_02004a0c
	b .L_0200c98e
.L_0200c960:
	movs r3, #255
	strb r3, [r5, #2]
	cmp r6, #1
	beq .L_0200c97c
	cmp r6, #1
	bcc .L_0200c972
	cmp r6, #2
	beq .L_0200c986
	b .L_0200c98e
.L_0200c972:
	adds r0, r7, #0
	movs r1, #1
	bl Func_02004a0c
	b .L_0200c98e
.L_0200c97c:
	adds r0, r7, #0
	movs r1, #3
	bl Func_02004a0c
	b .L_0200c98e
.L_0200c986:
	adds r0, r7, #0
	movs r1, #5
	bl Func_02004a0c
.L_0200c98e:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .rodata.x0200cca4,"a",%progbits
	.global Data_02004ca4
Data_02004ca4:
	.4byte 0x01030102
	.4byte 0x01260125
	.4byte 0x014b014c
	.global Data_02004cb0
Data_02004cb0:
	.4byte Sound_Wave29 + 0x134c
	.4byte 0x2008e0f8
	.4byte 0x0020f0e0
	.4byte 0x2008e0f8
	.4byte Sound_Wave29 + 0x134c
	.4byte 0x2008e0f8
	.global Data_02004cc8
Data_02004cc8:
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.4byte 0xffff000a
	.global Data_02004d0c
Data_02004d0c:
	.4byte 0x0001000e
	.4byte 0xffff08a2
	.global Data_02004d14
Data_02004d14:
	.4byte 0xffff0000
	.4byte 0x000001e8
	.4byte 0xc00001a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000208
	.4byte 0xc0000208
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x00000108
	.4byte 0x000001a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x000001b8
	.4byte 0x40000158
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0063
	.4byte 0x00000088
	.4byte 0xc0000238
	.4byte 0x00200000
	.4byte 0x02600000
	.4byte 0x000002c0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004da4
Data_02004da4:
	.4byte 0x00000062
	.4byte 0x1010b05f
	.4byte 0xffffffff
	.4byte 0x10201063
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02004dbc
Data_02004dbc:
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x01024000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff0102
	.4byte 0x00000007
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff01d3
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00024000
	.4byte 0xffff01d3
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01bc0000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01e20000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01bc0000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01ec0000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00020000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00015000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x0001c000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0001e000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004fe4
Data_02004fe4:
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x01024000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff0102
	.4byte 0x00000007
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff01d3
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00024000
	.4byte 0xffff01d3
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01bc0000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01e20000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01bc0000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01ec0000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff0010
	.4byte 0x0000000a
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0025
	.4byte 0x0000000a
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0026
	.4byte 0x0000000a
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00018000
	.4byte 0xffff0018
	.4byte 0x0000000a
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0070
	.4byte 0x0000000a
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff00c3
	.4byte 0x0000000a
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff00bb
	.4byte 0x0000000a
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00020000
	.4byte 0xffff0073
	.4byte 0x0000000a
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0072
	.4byte 0x0000000a
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
.L_0200d284:
	.4byte 0x00000000
	.4byte 0x00020001
	.4byte 0xffff0006
	.global Data_02005290
Data_02005290:
	.4byte .L_0200d284
	.4byte 0x00110010
	.global Data_02005298
Data_02005298:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_02000940
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_02000730
	.4byte 0x00008d15
	.4byte Resource_Data276 + 0x2162
	.4byte Func_02000730
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001b60
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001b61
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00001b62
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001b63
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001b64
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001b65
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00001b66
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x00002535
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x00002536
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00002537
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x00002538
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte 0x00002539
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x0000253a
	.4byte 0x50009705
	.4byte Monster_PurpleBeastSprites + 0xd2
	.4byte Func_0200006c
	.4byte 0x00008515
	.4byte Monster_SirenSprites + 0x2070
	.4byte 0x00000000
	.4byte 0x50008615
	.4byte Monster_OrcLordSprites + 0x3422
	.4byte Func_020000c8
	.4byte 0x10008c15
	.4byte Monster_SpearWarriorSprites + 0xcd6
	.4byte Func_02000190
	.4byte 0x00008c15
	.4byte Monster_SpearWarriorSprites + 0xcd6
	.4byte Func_02000200
	.4byte 0x10008c15
	.4byte Monster_ShieldKnightSprites + 0x2ff
	.4byte Func_02000190
	.4byte 0x00008c15
	.4byte Monster_ShieldKnightSprites + 0x2ff
	.4byte Func_02000200
	.4byte 0x10008c15
	.4byte Monster_WingedDragonSprites + 0xf48
	.4byte Func_02000394
	.4byte 0x00008c15
	.4byte Monster_WingedDragonSprites + 0xf48
	.4byte Func_0200042c
	.4byte 0x10008c15
	.4byte Monster_MimicSprites + 0x1bd
	.4byte Func_02000190
	.4byte 0x00008c15
	.4byte Monster_MimicSprites + 0x1bd
	.4byte Func_02000200
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000190
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000200
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte Func_020006ec
	.4byte 0x00004602
	.4byte 0xffff000c
	.4byte Func_020006ec
	.4byte 0x0000c602
	.4byte 0xffff000d
	.4byte Func_020006ec
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte Func_02000514
	.4byte 0x00004602
	.4byte 0xffff000b
	.4byte Func_02000514
	.4byte 0x0000a602
	.4byte 0xffff000c
	.4byte Data_02000954 + 0x1
	.4byte 0x0000b602
	.4byte 0xffff000c
	.4byte Data_02000954 + 0x1
	.4byte 0x00002602
	.4byte 0xffff000c
	.4byte Data_02000954 + 0x1
	.4byte 0x00003602
	.4byte 0xffff000c
	.4byte Data_02000954 + 0x1
	.4byte 0x0000a602
	.4byte 0xffff000b
	.4byte Data_02000954 + 0x1
	.4byte 0x0000b602
	.4byte 0xffff000b
	.4byte Data_02000954 + 0x1
	.4byte 0x00002602
	.4byte 0xffff000b
	.4byte Data_02000954 + 0x1
	.4byte 0x00003602
	.4byte 0xffff000b
	.4byte Data_02000954 + 0x1
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte Func_02000838
	.4byte 0x00000002
	.4byte 0x09050015
	.4byte Func_02002048
	.4byte 0x00000002
	.4byte 0x090f0016
	.4byte Func_02002a08
	.4byte 0x00000002
	.4byte Tileset_Set07TilesB + 0x2ab
	.4byte Func_02001574
	.4byte 0x00000002
	.4byte Tileset_Set44TilesB + 0xef8
	.4byte Func_02000968
	.4byte 0x00000002
	.4byte Tileset_Set42TilesA + 0x15a1
	.4byte Func_02000958
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020054f0
Data_020054f0:
	.4byte 0x00000000
	.global Data_020054f4
Data_020054f4:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00100000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0xfff00000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xfff00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_0200556c
Data_0200556c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00200000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0xfff00000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00020000
	.4byte 0x00000000
	.4byte 0xfffa0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_0200d5d0
Data_0200d5d0:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005620
Data_02005620:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005670
Data_02005670:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02800000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0xffb00000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0xffe00000
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0xffe80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020056d4
Data_020056d4:
	.4byte 0x00000026
