.syntax unified
	.thumb
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
