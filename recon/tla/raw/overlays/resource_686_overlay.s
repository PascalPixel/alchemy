	.set ANKOHL_RUINS_ES_IT, 0
	.ifdef TLA_EDITION_ES
	.set ANKOHL_RUINS_ES_IT, 1
	.endif
	.ifdef TLA_EDITION_IT
	.set ANKOHL_RUINS_ES_IT, 1
	.endif
.syntax unified
	.thumb
	.section .text.x0200815a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008416,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.else
	.2byte 0x0000
	.endif
	.section .text.x02008418,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.ifdef TLA_EDITION_JA
	.else
	.global Func_02000418
	.thumb_func
Func_02000418:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_020085ac
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Actor_GetById
	adds r7, r0, #0
	bl Func_02002e44
	movs r0, #0
	bl Func_02002edc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	bl Func_02002dec
	movs r0, #1
	bl wait
	movs r3, #130
	lsls r3, r3, #16
	str r3, [r7, #12]
	movs r3, #128
	lsls r3, r3, #8
	adds r5, r7, #0
	str r3, [r7, #72]
	adds r5, #85
	movs r3, #0
	str r3, [r7, #68]
	strb r3, [r5]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r4, #214
	lsls r4, r4, #1
	movs r2, #128
	adds r3, r3, r4
	lsls r2, r2, #1
	str r2, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #204
	bl Func_02002eec
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl FieldEvent_Wait
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_020085b0
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_020084b2:
	mov r4, r10
	lsls r5, r4, #12
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
	ldr r3, .L_020085b4
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_020085b8
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r5, r5, r3
	adds r5, r5, r4
	ldr r4, [r6, #4]
	str r5, [r6, #8]
	ldr r2, [r7, #16]
	ldr r3, [r6]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	str r4, [sp, #0]
	ldr r4, .L_020085bc
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_0200015c
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_020084b2
	movs r0, #188
	bl Func_02002eec
	ldr r5, .L_020085ac
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02002e9c
	ldr r0, [r5]
	movs r1, #49
	bl Actor_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02002e24
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02002e24
	bl Func_02002e2c
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02002e9c
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #68]
	movs r0, #10
	bl FieldEvent_Wait
	ldr r0, [r5]
	movs r1, #1
	bl Actor_SetModeById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	bl Func_02002e4c
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_020085ac:
	.4byte gMainRoutineWork
.L_020085b0:
	.4byte Effect_AdvanceMotion
.L_020085b4:
	.4byte 0xffffa000
.L_020085b8:
	.4byte 0xffffd000
.L_020085bc:
	.4byte 0x01090001
	.endif
	.endif
	.section .text.x020092c4,"ax",%progbits
	.global Func_020012c4
	.thumb_func
Func_020012c4:
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
	sub sp, #4
	mov r8, r3
	mov r2, r8
	adds r2, #228
	ldr r7, [r2]
	ldr r3, .L_02009408
	ldr r6, [r2, #4]
	mov r1, r8
	ands r7, r3
	ands r6, r3
	ldr r3, [r1]
	ldr r0, .L_0200940c
	ldr r3, [r3, #4]
	ldr r2, .L_02009410
	str r3, [sp, #0]
	mov r10, r0
	movs r1, #2
	ldrsh r3, [r0, r1]
	movs r0, #131
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsls r0, r0, #1
	lsrs r3, r3, #5
	mov r5, r10
	adds r0, #255
	mov r11, r3
	adds r5, #16
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020093e8
	mov r2, r10
	ldr r3, [r2, #4]
	movs r0, #128
	lsls r0, r0, #14
	cmp r3, r0
	bge .L_020093e8
	movs r1, #0
	mov r9, r1
.L_02009328:
	ldr r3, [r5, #12]
	ldr r1, [r5]
	ldr r0, [r5, #8]
	ldr r4, [r5, #4]
	cmp r3, #4
	bhi .L_02009388
	ldr r2, .L_02009414
	lsls r3, r3, #2
	ldr r2, [r3, r2]
	mov r3, r10
	mov r12, r2
	ldr r2, [r3, #4]
	mov pc, r12
	.2byte 0x0000
.L_02009344:
	.4byte .L_02009358
	.4byte .L_02009366
	.4byte .L_0200936c
	.4byte .L_02009372
	.4byte .L_0200937a
.L_02009358:
	movs r3, #128
	lsls r3, r3, #13
	subs r1, r1, r2
.L_0200935e:
	adds r0, r0, r2
	cmp r2, r3
	ble .L_02009388
	b .L_02009386
.L_02009366:
	subs r1, r1, r2
	adds r0, r0, r2
	b .L_02009388
.L_0200936c:
	adds r1, r1, r2
	adds r0, r0, r2
	b .L_02009388
.L_02009372:
	movs r3, #128
	lsls r3, r3, #13
	adds r1, r1, r2
	b .L_0200935e
.L_0200937a:
	movs r3, #128
	lsls r3, r3, #13
	cmp r2, r3
	bge .L_02009386
	adds r0, r0, r2
	b .L_02009388
.L_02009386:
	ldr r1, .L_02009418
.L_02009388:
	subs r1, r1, r7
	subs r3, r4, r6
	subs r2, r3, r0
	asrs r3, r1, #16
	movs r0, #167
	adds r1, r3, #0
	asrs r2, r2, #16
	adds r3, #7
	lsls r0, r0, #1
	subs r1, #8
	subs r2, #8
	cmp r3, r0
	bhi .L_020093dc
	movs r3, #15
	negs r3, r3
	cmp r2, r3
	blt .L_020093dc
	cmp r2, #239
	bgt .L_020093dc
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r1, r3
	movs r3, #255
	ands r2, r3
	movs r3, #0
	str r3, [r5, #16]
	lsls r3, r1, #16
	orrs r2, r3
	ldr r3, .L_0200941c
	mov r0, r11
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #3
	orrs r3, r0
	adds r0, r5, #0
	str r2, [r5, #20]
	str r3, [r5, #24]
	adds r0, #16
	movs r1, #255
	bl Func_02002dac
.L_020093dc:
	movs r1, #1
	add r9, r1
	mov r2, r9
	adds r5, #28
	cmp r2, #7
	ble .L_02009328
.L_020093e8:
	mov r0, r10
	ldr r2, [r0, #12]
	ldr r1, [r0, #4]
	movs r3, #160
	lsls r3, r3, #1
	add r3, r8
	adds r2, r2, r1
	str r2, [r3, #12]
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009408:
	.4byte 0xffff0000
.L_0200940c:
	.4byte Data_02004030
.L_02009410:
	.4byte gResourceTableEntries
.L_02009414:
	.4byte .L_02009344
.L_02009418:
	.4byte 0xfff00000
.L_0200941c:
	.4byte 0x40000800
	.section .bss,"aw",%nobits
	.balign 16
	.global Data_02004030
Data_02004030:
