.syntax unified
	.thumb
	.section .text.x02008be8,"ax",%progbits
	.balign 4
	.global Func_02000be8
	.thumb_func
Func_02000be8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #132
	movs r2, #188
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r1, r1, r3
	adds r2, r2, r3
	mov r10, r1
	mov r8, r2
	bl Func_02001da0
	movs r0, #0
	bl Func_02001ee8
	bl Func_02001d60
	movs r7, #1
	movs r6, #0
.L_02008c18:
	adds r3, r6, #0
	subs r3, #17
	cmp r3, #62
	bhi .L_02008c3e
	adds r1, r7, #1
	adds r3, r1, #0
	cmp r1, #0
	bge .L_02008c2a
	adds r3, r7, #4
.L_02008c2a:
	asrs r0, r3, #2
	movs r3, #16
	subs r3, r3, r0
	lsls r2, r0, #8
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	adds r7, r1, #0
.L_02008c3e:
	mov r1, r10
	ldr r3, [r1, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r1, #12]
	mov r1, r8
	ldr r3, [r1, #12]
	adds r6, #1
	adds r3, r3, r2
	movs r2, #176
	lsls r2, r2, #1
	str r3, [r1, #12]
	cmp r6, r2
	blt .L_02008c18
	bl Func_02001d40
	movs r0, #1
	bl wait
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	ldr r5, .L_02008d34
	movs r3, #133
	lsls r3, r3, #2
	movs r1, #204
	movs r2, #204
	adds r5, r5, r3
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl Actor_SetSpeedParameters
	movs r1, #164
	lsls r1, r1, #1
	movs r2, #184
	ldr r0, [r5]
	bl Actor_SetPositionAndReset
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #154
	lsls r0, r0, #1
	bl Func_02001f10
	ldr r0, [r5]
	movs r1, #2
	bl Actor_SetSpritePriority
	cmp r6, #0
	beq .L_02008cf2
.L_02008cae:
	adds r3, r6, #0
	subs r3, #33
	cmp r3, #62
	bhi .L_02008cd4
	adds r3, r7, #1
	adds r2, r3, #0
	cmp r3, #0
	bge .L_02008cc0
	adds r2, r7, #4
.L_02008cc0:
	asrs r1, r2, #2
	movs r3, #16
	subs r3, r3, r1
	lsls r2, r1, #8
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	subs r7, #1
.L_02008cd4:
	mov r1, r10
	ldr r3, [r1, #12]
	ldr r2, .L_02008d38
	movs r0, #1
	adds r3, r3, r2
	str r3, [r1, #12]
	mov r1, r8
	ldr r3, [r1, #12]
	subs r6, #1
	adds r3, r3, r2
	str r3, [r1, #12]
	bl wait
	cmp r6, #0
	bne .L_02008cae
.L_02008cf2:
	ldr r3, .L_02008d34
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Actor_SetSpritePriority
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02001f10
	movs r0, #1
	bl wait
	movs r0, #125
	bl Func_02001f10
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_SetBit
	bl Func_02001da8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008d34:
	.4byte gMainRoutineWork
.L_02008d38:
	.4byte 0xffff0000
	.section .text.x02008d3c,"ax",%progbits
	.global Func_02000d3c
	.thumb_func
Func_02000d3c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #188
	adds r1, r1, r3
	lsls r2, r2, #1
	adds r7, r3, r2
	mov r8, r1
	bl Func_02001da0
	movs r0, #0
	bl Func_02001ee8
	bl Func_02001d60
	ldr r5, .L_02008e4c
	movs r3, #133
	lsls r3, r3, #2
	movs r1, #204
	movs r2, #204
	adds r5, r5, r3
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl Actor_SetSpeedParameters
	movs r2, #8
	negs r2, r2
	ldr r0, [r5]
	movs r1, #0
	bl Actor_OffsetPositionAndResetMotion
	ldr r0, [r5]
	bl Actor_WaitForMoveById
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	bl Func_02001e70
	movs r0, #154
	lsls r0, r0, #1
	bl Func_02001f10
	ldr r0, [r5]
	movs r1, #2
	bl Actor_SetSpritePriority
	movs r6, #1
	movs r5, #0
.L_02008dae:
	adds r3, r5, #0
	subs r3, #17
	cmp r3, #62
	bhi .L_02008dd4
	adds r1, r6, #1
	adds r3, r1, #0
	cmp r1, #0
	bge .L_02008dc0
	adds r3, r6, #4
.L_02008dc0:
	asrs r0, r3, #2
	movs r3, #16
	subs r3, r3, r0
	lsls r2, r0, #8
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	adds r6, r1, #0
.L_02008dd4:
	mov r1, r8
	ldr r3, [r1, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r3, [r7, #12]
	movs r0, #1
	adds r3, r3, r2
	str r3, [r7, #12]
	bl wait
	movs r2, #96
	adds r5, #1
	adds r2, #255
	cmp r5, r2
	ble .L_02008dae
	ldr r5, .L_02008e4c
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #1
	ldr r0, [r5]
	bl Actor_SetSpritePriority
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02001f10
	movs r0, #1
	bl wait
	movs r0, #125
	bl Func_02001f10
	movs r0, #20
	bl FieldEvent_Wait
	ldr r0, [r5]
	bl Actor_GetById
	movs r3, #1
	adds r0, #34
	strb r3, [r0]
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_SetBit
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_02001da8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008e4c:
	.4byte gMainRoutineWork
	.section .text.x02009ba8,"ax",%progbits
	.global Func_02001ba8
	.thumb_func
Func_02001ba8:
	push {r5, r6, r7, lr}
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009c28
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009c3c
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #132
	lsls r2, r2, #1
	adds r7, r3, r2
	adds r2, #112
	adds r6, r3, r2
	movs r5, #1
	bl Func_02001d60
	movs r4, #0
.L_02009bdc:
	adds r3, r4, #0
	subs r3, #17
	cmp r3, #62
	bhi .L_02009c02
	adds r1, r5, #1
	adds r3, r1, #0
	cmp r1, #0
	bge .L_02009bee
	adds r3, r5, #4
.L_02009bee:
	asrs r0, r3, #2
	movs r3, #16
	subs r3, r3, r0
	lsls r2, r0, #8
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	adds r5, r1, #0
.L_02009c02:
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r7, #12]
	adds r4, #1
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r3, #176
	lsls r3, r3, #1
	cmp r4, r3
	blt .L_02009bdc
	bl Func_02001d40
	movs r0, #1
	bl wait
	b .L_02009c3c
.L_02009c28:
	ldr r3, .L_02009c40
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_02009c3c
	bl Func_02000be8
.L_02009c3c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009c40:
	.4byte gMainRoutineWork
