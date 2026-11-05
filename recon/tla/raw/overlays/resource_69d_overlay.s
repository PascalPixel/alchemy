.syntax unified
	.thumb
	.section .text.x02008300,"ax",%progbits
	.global Func_02000300
	.thumb_func
Func_02000300:
	push {r5, r6, r7, lr}
	movs r7, #0
	movs r6, #0
.L_02008306:
	movs r0, #168
	movs r1, #0
	movs r2, #0
	movs r3, #0
	lsls r0, r0, #2
	bl Func_02001dc0
	adds r5, r0, #0
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	subs r2, #50
	strb r3, [r2]
	bl Random16Far
	movs r3, #192
	lsls r3, r3, #11
	lsls r0, r0, #2
	adds r0, r0, r3
	str r0, [r5, #48]
	bl Random16Far
	movs r3, #128
	lsls r0, r0, #11
	lsls r3, r3, #4
	lsrs r0, r0, #16
	adds r0, r0, r3
	ldr r3, .L_02008380
	str r0, [r5, #52]
	str r3, [r5, #108]
	bl Random16Far
	adds r3, r5, #0
	adds r3, #100
	strh r0, [r3]
	adds r3, #2
	strh r6, [r3]
	adds r0, r5, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #2
	bl Actor_SetPartAttribute
	ldr r1, [r5, #80]
	movs r0, #13
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	adds r7, #1
	strb r3, [r1, #9]
	adds r6, #5
	cmp r7, #7
	bne .L_02008306
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008380:
	.4byte Func_02000244
	.section .text.x02008384,"ax",%progbits
	.global Func_02000384
	.thumb_func
Func_02000384:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	str r2, [r3]
	bl Func_020000c0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083b4
	bl Func_0200017c
.L_020083b4:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083c6
	bl Func_020001ac
.L_020083c6:
	bl Func_02000300
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	mov r10, r0
	cmp r0, #0
	bne .L_020084c2
	ldr r2, .L_020084f4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #11
	bne .L_020084c2
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r2, r3
	ldr r0, [r5]
	bl Actor_GetById
	adds r6, r0, #0
	adds r7, r6, #0
	adds r7, #35
	ldrb r2, [r7]
	movs r3, #191
	ands r3, r2
	strb r3, [r7]
	bl Func_02001e48
	movs r0, #0
	bl Func_02001ed0
	ldr r0, [r5]
	movs r1, #1
	bl Actor_SetSpritePriority
	ldr r0, [r5]
	movs r1, #2
	bl Actor_SetModeById
	adds r0, r6, #0
	movs r1, #64
	bl Actor_SetAnimationSpeed
	ldr r3, .L_020084f8
	movs r1, #85
	adds r1, r1, r6
	mov r2, r10
	strb r2, [r1]
	str r3, [r6, #20]
	str r3, [r6, #12]
	mov r8, r1
	bl Event_StartScreenTransition
	movs r0, #10
	bl FieldEvent_Wait
	ldr r3, [r6, #12]
	movs r5, #0
	cmp r3, #0
	bge .L_0200846c
.L_02008446:
	movs r1, #192
	lsls r1, r1, #10
	adds r3, r3, r1
	str r3, [r6, #12]
	str r3, [r6, #20]
	ldrh r3, [r6, #6]
	movs r2, #192
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r6, #6]
	movs r0, #2
	adds r5, #1
	bl wait
	cmp r5, #89
	bgt .L_0200846c
	ldr r3, [r6, #12]
	cmp r3, #0
	blt .L_02008446
.L_0200846c:
	movs r3, #0
	str r3, [r6, #12]
	str r3, [r6, #20]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	mov r1, r8
	movs r3, #3
	strb r3, [r1]
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r6, #40]
	movs r3, #128
	lsls r3, r3, #8
	ldr r5, .L_020084f4
	str r3, [r6, #72]
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #1
	ldr r0, [r5]
	bl Actor_SetModeById
	movs r0, #40
	bl FieldEvent_Wait
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #72]
	ldrb r2, [r7]
	movs r3, #64
	orrs r3, r2
	strb r3, [r7]
	ldr r0, [r5]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	bl Func_02001e50
.L_020084c2:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084ea
	ldr r2, .L_020084f4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #12
	bne .L_020084ea
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r2, r1
	ldr r0, [r3]
	bl Func_02001f00
.L_020084ea:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_020084f4:
	.4byte gMainRoutineWork
.L_020084f8:
	.4byte 0xff880000
	.section .text.x020085d6,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200921e,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009256,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009a1e,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009c68,"ax",%progbits
	.global Func_02001c68
	.thumb_func
Func_02001c68:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r3, .L_02009d18
	str r2, [sp, #0]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r9, r0
	ldr r0, [r3]
	mov r11, r1
	bl Actor_GetById
	movs r3, #192
	ldr r5, .L_02009d1c
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r6, r0, #0
	movs r2, #2
	ldrsh r0, [r5, r2]
	mov r10, r3
	bl GameFlag_GetByte
	adds r7, r0, #0
	movs r3, #2
	ldrsh r0, [r5, r3]
	adds r0, #8
	bl GameFlag_GetByte
	mov r8, r0
	cmp r7, #0
	bne .L_02009cb6
	cmp r0, #0
	beq .L_02009d0a
.L_02009cb6:
	movs r2, #2
	ldrsh r0, [r5, r2]
	movs r1, #0
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r5, r3]
	movs r1, #0
	adds r0, #8
	bl GameFlag_SetByte
	mov r3, r9
	adds r2, r7, r3
	mov r3, r8
	movs r1, #128
	add r3, r11
	lsls r1, r1, #12
	lsls r3, r3, #20
	adds r3, r3, r1
	str r3, [r6, #16]
	movs r3, #230
	lsls r3, r3, #1
	add r3, r10
	lsls r2, r2, #20
	adds r2, r2, r1
	ldr r1, [r3]
	str r2, [r6, #8]
	str r2, [r1, #8]
	ldr r3, [r6, #16]
	str r3, [r1, #16]
	bl Func_02001dd0
	bl Func_02001a20
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #0]
	str r2, [r3]
.L_02009d0a:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009d18:
	.4byte gMainRoutineWork
.L_02009d1c:
	.4byte gData_02002864
	.section .bss,"aw",%nobits
	.global gData_02002718
gData_02002718:
	.space 0x00000004
	.global gData_0200271c
gData_0200271c:
	.space 0x00000020
	.global gData_0200273c
gData_0200273c:
	.space 0x00000004
	.global gData_02002740
gData_02002740:
	.space 0x0000001c
	.global gData_0200275c
gData_0200275c:
	.space 0x00000002
	.global gData_0200275e
gData_0200275e:
	.space 0x00000002
	.global gData_02002760
gData_02002760:
	.space 0x00000040
	.global gData_020027a0
gData_020027a0:
	.space 0x000000c0
	.global gData_02002860
gData_02002860:
	.space 0x00000002
	.global gData_02002862
gData_02002862:
	.space 0x00000002
	.global gData_02002864
gData_02002864:
