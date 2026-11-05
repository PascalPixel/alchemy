.syntax unified
	.thumb
	.section .text.x02008e3c,"ax",%progbits
	.global Func_02000e3c
	.thumb_func
Func_02000e3c:
	push {r5, r6, r7, lr}
	ldr r5, .L_02008ecc
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r5, r1
	adds r7, r0, #0
	ldr r0, [r5]
	bl Actor_GetById
	adds r6, r0, #0
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	ldr r0, [r5]
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r3, #0
	movs r1, #128
	str r3, [r6, #108]
	movs r2, #0
	lsls r1, r1, #1
	ldr r0, [r5]
	bl Func_02004d00
	adds r0, r6, #0
	movs r1, #27
	bl Func_02004bc0
	movs r1, #48
	adds r0, r6, #0
	bl Actor_SetAnimationSpeed
	movs r0, #30
	bl wait
	movs r3, #128
	ldr r2, .L_02008ec8
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	ldr r2, .L_02008ed0
	ldr r3, [r6, #8]
	asrs r4, r3, #20
	ldr r3, [r6, #16]
	asrs r0, r3, #20
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	cmp r1, #0
	beq .L_02008ed4
	adds r3, r6, #0
	adds r3, #34
	ldrb r3, [r3]
	movs r2, #3
	ands r2, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [r1, r3]
	b .L_02008ed4
.L_02008ec8:
	.4byte 0x00000000
.L_02008ecc:
	.4byte gMainRoutineWork
.L_02008ed0:
	.4byte gMapCellBuffer
.L_02008ed4:
	lsls r3, r0, #7
	adds r3, r4, r3
	lsls r3, r3, #2
	movs r1, #128
	adds r3, r2, r3
	lsls r1, r1, #2
	adds r2, r3, r1
	ldrb r2, [r2, #3]
	movs r3, #64
	ands r3, r2
	cmp r3, #0
	bne .L_02008f0a
	movs r2, #226
	movs r3, #128
	lsls r2, r2, #4
	lsls r3, r3, #19
	adds r2, #255
	adds r3, #74
	strh r2, [r3]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_02008f38
	orrs r3, r2
	strh r3, [r1]
	bl Func_02000df0
.L_02008f0a:
	ldr r3, .L_02008f3c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #28
	bl Actor_SetModeById
	movs r0, #16
	bl FieldEvent_Wait
	movs r5, #0
.L_02008f22:
	cmp r5, #5
	bne .L_02008f2c
	movs r0, #204
	bl Func_02004de0
.L_02008f2c:
	ldr r3, [r6, #24]
	ldr r1, .L_02008f40
	ldr r2, .L_02008f44
	adds r3, r3, r1
	str r3, [r6, #24]
	b .L_02008f48
.L_02008f38:
	.4byte 0x00008000
.L_02008f3c:
	.4byte gMainRoutineWork
.L_02008f40:
	.4byte 0xfffffc00
.L_02008f44:
	.4byte 0xfffffd00
.L_02008f48:
	ldr r3, [r6, #28]
	ldr r1, .L_02008f9c
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r5, #1
	bl wait
	cmp r5, #39
	ble .L_02008f22
	ldr r3, .L_02008fa0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	movs r3, #0
	adds r0, #84
	strb r3, [r0]
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
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	adds r0, r7, #0
	bl Func_02004d30
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008f9c:
	.4byte 0xffff6667
.L_02008fa0:
	.4byte gMainRoutineWork
	.section .text.x02009254,"ax",%progbits
	.balign 4
	.global Func_02001254
	.thumb_func
Func_02001254:
	push {r5, lr}
	ldr r3, .L_0200927c
	ldr r5, .L_02009280
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #59
	bgt .L_02009284
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	bne .L_02009298
	movs r0, #0
	movs r1, #3
	bl Func_0200246c
	ldr r3, .L_02009278
	b .L_02009296
	.2byte 0x0000
.L_02009278:
	.4byte 0x00000001
.L_0200927c:
	.4byte gData_020068a0
.L_02009280:
	.4byte gData_02006868
.L_02009284:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #1
	bne .L_02009298
	movs r0, #0
	movs r1, #67
	bl Func_0200246c
	ldr r3, .L_020092b0
.L_02009296:
	strh r3, [r5]
.L_02009298:
	ldr r2, .L_020092b4
	movs r1, #150
	ldrh r3, [r2]
	lsls r1, r1, #17
	adds r3, #1
	strh r3, [r2]
	lsls r3, r3, #16
	cmp r3, r1
	ble .L_020092b8
	ldr r3, .L_020092b0
	strh r3, [r2]
	b .L_020092b8
.L_020092b0:
	.4byte 0x00000000
.L_020092b4:
	.4byte gData_020068a0
.L_020092b8:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020092e4,"ax",%progbits
	.balign 4
	.global Func_020012e4
	.thumb_func
Func_020012e4:
	push {lr}
	ldr r2, [r0, #80]
	movs r1, #128
	ldrh r3, [r2, #18]
	lsls r1, r1, #4
	adds r3, r3, r1
	adds r1, r0, #0
	adds r1, #100
	strh r3, [r2, #18]
	ldrh r3, [r1]
	movs r2, #128
	adds r3, #1
	strh r3, [r1]
	lsls r2, r2, #12
	lsls r3, r3, #16
	movs r4, #0
	cmp r3, r2
	bne .L_0200930e
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r0, #40]
.L_0200930e:
	movs r2, #0
	ldrsh r3, [r1, r2]
	cmp r3, #9
	ble .L_0200932e
	ldr r3, [r0, #8]
	movs r2, #128
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, .L_02009340
	ldr r3, [r0, #24]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
.L_0200932e:
	movs r2, #0
	ldrsh r3, [r1, r2]
	cmp r3, #32
	bne .L_0200933c
	str r4, [r0, #8]
	str r4, [r0, #16]
	str r4, [r0, #108]
.L_0200933c:
	pop {pc}
	.2byte 0x0000
.L_02009340:
	.4byte 0xfffff800
	.section .text.x02009ae0,"ax",%progbits
	.balign 4
	.global Func_02001ae0
	.thumb_func
Func_02001ae0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02009c3c
	ldr r2, .L_02009c40
	mov r10, r3
	movs r3, #133
	lsls r3, r3, #2
	add r3, r10
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	mov r8, r2
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r5, [r6, #68]
	mov r9, r3
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
	bge .L_02009b3c
	adds r3, #15
.L_02009b3c:
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
	bne .L_02009bce
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001a1c
	cmp r0, #0
	beq .L_02009bce
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r3, r10
	ldrb r2, [r3]
	cmp r2, #0
	bne .L_02009bce
	ldr r1, [r6, #76]
	cmp r1, #0
	beq .L_02009ba2
	mov r3, r8
	adds r3, #104
	strh r2, [r3]
	mov r2, r8
	adds r2, #106
	cmp r1, #0
	ble .L_02009b9a
	movs r3, #1
	b .L_02009ba0
.L_02009b9a:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
.L_02009ba0:
	strh r3, [r2]
.L_02009ba2:
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r1, #0
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	mov r2, r8
	movs r3, #1
	strh r3, [r2, #4]
	ldrh r2, [r2, #10]
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	strh r2, [r3]
.L_02009bce:
	movs r3, #84
	mov r2, r8
	ldrh r0, [r2, r3]
	movs r7, #0
	cmp r0, #0
	beq .L_02009c2e
	mov r5, r8
	adds r5, #84
.L_02009bde:
	bl Actor_GetById
	adds r4, r0, #0
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_02009c20
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001a80
	cmp r0, #0
	beq .L_02009c20
	ldrh r1, [r5, #2]
	cmp r1, #0
	bne .L_02009c1a
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	b .L_02009c20
.L_02009c1a:
	movs r3, #1
	mov r2, r8
	strh r3, [r2, #6]
.L_02009c20:
	adds r7, #1
	adds r5, #4
	cmp r7, #3
	bgt .L_02009c2e
	ldrh r0, [r5]
	cmp r0, #0
	bne .L_02009bde
.L_02009c2e:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009c3c:
	.4byte gMainRoutineWork
.L_02009c40:
	.4byte gData_020023c4 + 0x188
	.section .text.x02009cbc,"ax",%progbits
	.balign 4
	.global Func_02001cbc
	.thumb_func
Func_02001cbc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02009e14
	ldr r2, .L_02009e18
	mov r10, r3
	movs r3, #133
	lsls r3, r3, #2
	add r3, r10
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	mov r8, r2
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r5, [r6, #68]
	mov r9, r3
	ldr r3, [r6, #16]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #16]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #8]
	adds r4, r0, #0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #8]
	str r4, [sp, #0]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bge .L_02009d18
	adds r3, #15
.L_02009d18:
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
	bne .L_02009da8
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001a1c
	cmp r0, #0
	beq .L_02009da8
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r3, r10
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_02009da8
	ldr r2, [r6, #76]
	cmp r2, #0
	beq .L_02009d7c
	mov r1, r8
	adds r1, #106
	strh r3, [r1]
	subs r1, #2
	cmp r2, #0
	ble .L_02009d74
	movs r3, #1
	b .L_02009d7a
.L_02009d74:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
.L_02009d7a:
	strh r3, [r1]
.L_02009d7c:
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r1, #0
	ldr r3, [r6, #16]
	ldr r2, [r6, #68]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	mov r2, r8
	movs r3, #1
	strh r3, [r2, #4]
	ldrh r2, [r2, #10]
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	strh r2, [r3]
.L_02009da8:
	movs r3, #84
	mov r2, r8
	ldrh r0, [r2, r3]
	movs r7, #0
	cmp r0, #0
	beq .L_02009e08
	mov r5, r8
	adds r5, #84
.L_02009db8:
	bl Actor_GetById
	adds r4, r0, #0
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_02009dfa
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001a80
	cmp r0, #0
	beq .L_02009dfa
	ldrh r1, [r5, #2]
	cmp r1, #0
	bne .L_02009df4
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	b .L_02009dfa
.L_02009df4:
	movs r3, #1
	mov r2, r8
	strh r3, [r2, #6]
.L_02009dfa:
	adds r7, #1
	adds r5, #4
	cmp r7, #3
	bgt .L_02009e08
	ldrh r0, [r5]
	cmp r0, #0
	bne .L_02009db8
.L_02009e08:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02009e14:
	.4byte gMainRoutineWork
.L_02009e18:
	.4byte gData_020023c4 + 0x188
	.section .text.x0200a2e4,"ax",%progbits
	.balign 4
	.global Func_020022e4
	.thumb_func
Func_020022e4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r8, r1
	ldr r1, .L_0200a410
	sub sp, #16
	adds r6, r0, #0
	movs r0, #10
	str r1, [sp, #4]
	adds r0, #255
	adds r1, #20
	str r2, [sp, #12]
	str r3, [sp, #8]
	mov r9, r1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a3c8
	ldrh r3, [r6]
	movs r2, #0
	mov r11, r2
	mov r10, r3
	adds r6, #2
	cmp r3, #0
	ble .L_0200a386
.L_0200a31e:
	ldrh r7, [r6]
	movs r1, #15
	ands r1, r7
	movs r3, #240
	mov r0, r10
	str r1, [sp, #0]
	ands r7, r3
	bl Actor_GetById
	adds r5, r0, #0
	adds r6, #2
	cmp r5, #0
	beq .L_0200a372
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r3, r5, #0
	movs r2, #128
	adds r3, #98
	movs r1, #1
	ands r2, r7
	strb r1, [r3]
	cmp r2, #0
	bne .L_0200a352
	subs r3, #9
	strb r2, [r3]
.L_0200a352:
	mov r2, r9
	mov r3, r9
	strh r1, [r2]
	mov r0, r10
	strh r7, [r3, #4]
	bl Actor_GetById
	mov r1, r9
	str r0, [r1, #8]
	ldr r2, [sp, #0]
	lsls r3, r2, #16
	str r3, [r1, #12]
	mov r3, r11
	strh r3, [r1, #2]
	movs r2, #16
	add r9, r2
.L_0200a372:
	movs r3, #1
	add r11, r3
	mov r1, r11
	cmp r1, #3
	bgt .L_0200a386
	ldrh r2, [r6]
	adds r6, #2
	mov r10, r2
	cmp r2, #0
	bgt .L_0200a31e
.L_0200a386:
	mov r3, r8
	cmp r3, #0
	beq .L_0200a3c8
	movs r1, #0
	ldrh r2, [r3]
	mov r11, r1
	ldr r1, [sp, #4]
	movs r3, #2
	add r8, r3
	movs r3, #84
	strh r2, [r1, r3]
	cmp r2, #0
	ble .L_0200a3c8
	adds r2, r1, #0
	adds r2, #84
.L_0200a3a4:
	mov r1, r8
	ldrh r3, [r1]
	movs r1, #1
	strh r3, [r2, #2]
	add r11, r1
	movs r3, #2
	add r8, r3
	mov r3, r11
	adds r2, #4
	cmp r3, #3
	bgt .L_0200a3c8
	mov r1, r8
	ldrh r3, [r1]
	movs r1, #2
	add r8, r1
	strh r3, [r2]
	cmp r3, #0
	bgt .L_0200a3a4
.L_0200a3c8:
	ldr r2, [sp, #12]
	ldr r3, [sp, #4]
	add r1, sp, #8
	str r2, [r3, #16]
	ldrh r1, [r1]
	ldr r2, [sp, #4]
	strh r1, [r2, #10]
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #80
	ldrh r3, [r1]
	cmp r3, #0
	bne .L_0200a3f8
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
.L_0200a3f8:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a414
	bl Task_Add
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a410:
	.4byte gData_020023c4 + 0x188
.L_0200a414:
	.4byte Func_020020ec
	.section .text.x0200a4c0,"ax",%progbits
	.balign 4
	.global Func_020024c0
	.thumb_func
Func_020024c0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #16
	ldr r6, [r3, #108]
	bl Func_02004dc0
	mov r8, r0
	bl Actor_GetById
	bl Party_CountActiveOwners
	movs r5, #0
	adds r7, r0, #0
	cmp r5, r7
	bge .L_0200a502
.L_0200a4e6:
	ldr r2, .L_0200a5a8
	movs r1, #134
	lsls r1, r1, #2
	adds r3, r5, r1
	ldrb r0, [r2, r3]
	bl Unit_Get
	ldrh r3, [r0, #56]
	lsls r2, r5, #1
	mov r1, sp
	adds r5, #1
	strh r3, [r1, r2]
	cmp r5, r7
	blt .L_0200a4e6
.L_0200a502:
	movs r0, #10
	negs r0, r0
	movs r1, #0
	bl Func_02004da8
	movs r2, #182
	lsls r2, r2, #1
	movs r4, #183
	adds r3, r6, r2
	lsls r4, r4, #1
	movs r2, #0
	strh r2, [r3]
	movs r1, #129
	adds r3, r6, r4
	strh r2, [r3]
	mov r0, r8
	lsls r1, r1, #1
	movs r5, #0
	bl Func_02004d08
	cmp r5, r7
	bge .L_0200a59c
.L_0200a52e:
	ldr r1, .L_0200a5a8
	movs r2, #134
	lsls r2, r2, #2
	adds r2, r2, r5
	ldrb r0, [r1, r2]
	mov r10, r1
	mov r8, r2
	bl Unit_Get
	movs r4, #56
	ldrsh r3, [r0, r4]
	cmp r3, #0
	ble .L_0200a556
	movs r1, #183
	lsls r1, r1, #1
	adds r2, r6, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a596
.L_0200a556:
	mov r3, sp
	lsls r2, r5, #1
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a596
	movs r2, #182
	lsls r2, r2, #1
	adds r1, r6, r2
	ldrh r3, [r1]
	movs r4, #184
	adds r2, r3, #1
	lsls r3, r3, #16
	lsls r4, r4, #1
	asrs r3, r3, #15
	strh r2, [r1]
	adds r3, r3, r4
	mov r1, r10
	mov r4, r8
	ldrb r2, [r1, r4]
	movs r1, #181
	strh r2, [r6, r3]
	movs r3, #255
	lsls r1, r1, #1
	lsls r3, r3, #8
	adds r2, r6, r1
	adds r3, #255
	strh r3, [r2]
	movs r3, #50
	adds r3, #255
	adds r2, r0, r3
	movs r3, #0
	strb r3, [r2]
.L_0200a596:
	adds r5, #1
	cmp r5, r7
	blt .L_0200a52e
.L_0200a59c:
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a5a8:
	.4byte gMainRoutineWork
	.section .text.x0200a63c,"ax",%progbits
	.balign 4
	.global Func_0200263c
	.thumb_func
Func_0200263c:
	.ifdef TLA_EDITION_JA
	push {r5, r6, r7, lr}
	mov r7, sl
	mov r6, r8
	push {r6, r7}
	ldr r5, .L_ja_0200a7d0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3, #0]
	sub sp, #12
	cmp r3, #0
	beq .L_ja_0200a660
	b .L_ja_0200a7c0
.L_ja_0200a660:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_020024c0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3, #0]
	bl Actor_GetById
	ldr r2, .L_ja_0200a7d4
	adds r5, r0, #0
	mov sl, r2
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	ldr r3, .L_ja_0200a7d8
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	movs r1, #49
	bl Func_02004bc0
.L_ja_0200a698:
	adds r3, r5, #0
	adds r3, #34
	ldr r1, [r5, #8]
	ldrb r0, [r3, #0]
	ldr r2, [r5, #16]
	bl Func_02004c18
	mov r6, sl
	mov r7, sl
	adds r6, #104
	adds r7, #106
	mov r1, sp
	cmp r0, #7
	bne .L_ja_0200a710
	movs r0, #0
	ldrsh r3, [r6, r0]
	movs r2, #0
	lsls r3, r3, #17
	str r3, [r5, #36]
	mov r8, r2
	movs r1, #0
	ldrsh r3, [r7, r1]
	lsls r3, r3, #17
	str r3, [r5, #44]
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r5, #52]
.L_ja_0200a6ce:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	mov r1, sp
	adds r3, r3, r2
	str r3, [r1, #0]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_02004c38
	cmp r0, #0
	beq .L_ja_0200a6fe
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	b .L_ja_0200a7b4
.L_ja_0200a6fe:
	movs r0, #1
	bl wait
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #9
	ble .L_ja_0200a6ce
	b .L_ja_0200a7b4
.L_ja_0200a710:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #0]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_02004c38
	cmp r0, #0
	bgt .L_ja_0200a7b4
	cmp r0, #0
	bge .L_ja_0200a756
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	ldr r3, .L_ja_0200a7d0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3, #0]
	movs r1, #6
	negs r2, r2
	bl Func_02004ce8
	b .L_ja_0200a7b4
.L_ja_0200a756:
	movs r2, #0
	ldrsh r3, [r6, r2]
	ldr r2, [r5, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	ldrsh r3, [r7, r0]
	ldr r7, .L_ja_0200a7dc
	lsls r3, r3, #17
	ldr r0, .L_ja_0200a7e0
	adds r6, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r6, [r5, #16]
	cmp r2, r4
	ble .L_ja_0200a782
	adds r2, r4, #0
.L_ja_0200a782:
	ldr r0, .L_ja_0200a7e4
	cmp r2, r0
	bge .L_ja_0200a78a
	adds r2, r0, #0
.L_ja_0200a78a:
	subs r3, r1, r2
	ldr r1, .L_ja_0200a7e0
	str r3, [r5, #8]
	adds r3, r6, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_ja_0200a79c
	adds r2, r4, #0
.L_ja_0200a79c:
	cmp r2, r0
	bge .L_ja_0200a7a2
	adds r2, r0, #0
.L_ja_0200a7a2:
	subs r3, r6, r2
	str r3, [r5, #16]
	mov r2, sl
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl wait
	b .L_ja_0200a698
.L_ja_0200a7b4:
	movs r3, #0
	str r3, [r5, #108]
	adds r0, r5, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_ja_0200a7c0:
	mov r0, sl
	movs r3, #0
	strh r3, [r0, #4]
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov sl, r5
	pop {r5, r6, r7, pc}
.L_ja_0200a7d0:
	.4byte gMainRoutineWork
.L_ja_0200a7d4:
	.4byte 0x0200254c
.L_ja_0200a7d8:
	.4byte Func_02002614
.L_ja_0200a7dc:
	.4byte 0x000fffff
.L_ja_0200a7e0:
	.4byte 0xfff80000
.L_ja_0200a7e4:
	.4byte 0xffff0000
	.else
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_0200a830
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3]
	sub sp, #16
	cmp r3, #0
	beq .L_0200a65e
	b .L_0200a81c
.L_0200a65e:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_020024c0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Actor_GetById
	ldr r2, .L_0200a834
	adds r6, r0, #0
	str r2, [sp, #0]
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	ldr r3, .L_0200a838
	adds r0, r6, #0
	str r3, [r6, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	movs r1, #49
	bl Func_02004bc0
.L_0200a696:
	adds r3, r6, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Func_02004c18
	ldr r3, [sp, #0]
	ldr r1, [sp, #0]
	adds r3, #104
	adds r1, #106
	mov r9, r1
	mov r10, r3
	add r1, sp, #4
	cmp r0, #7
	bne .L_0200a712
	movs r2, #0
	ldrsh r3, [r3, r2]
	mov r1, r9
	lsls r3, r3, #17
	str r3, [r6, #36]
	movs r5, #0
	movs r0, #0
	ldrsh r3, [r1, r0]
	lsls r3, r3, #17
	str r3, [r6, #44]
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r6, #52]
.L_0200a6d0:
	mov r0, r10
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #19
	add r1, sp, #4
	adds r3, r3, r2
	str r3, [r1]
	mov r0, r9
	ldr r3, [r6, #12]
	str r3, [r1, #4]
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r6, #0
	bl Func_02004c38
	cmp r0, #0
	beq .L_0200a704
	movs r3, #0
	str r3, [r6, #36]
	str r3, [r6, #44]
	b .L_0200a810
.L_0200a704:
	movs r0, #1
	adds r5, #1
	bl wait
	cmp r5, #9
	ble .L_0200a6d0
	b .L_0200a810
.L_0200a712:
	mov r0, r10
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1]
	mov r0, r9
	ldr r3, [r6, #12]
	str r3, [r1, #4]
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r6, #0
	bl Func_02004c38
	cmp r0, #0
	bgt .L_0200a810
	cmp r0, #0
	bge .L_0200a75c
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	ldr r3, .L_0200a830
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3]
	movs r1, #6
	negs r2, r2
	bl Func_02004ce8
	b .L_0200a810
.L_0200a75c:
	ldrh r3, [r6, #32]
	movs r2, #0
	subs r3, #2
	mov r11, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	mov r8, r2
	adds r7, r5, #0
	adds r7, #89
.L_0200a770:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200a7a0
	ldrb r2, [r7]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200a7a0
	cmp r5, r6
	beq .L_0200a7a0
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	mov r1, r11
	add r2, sp, #4
	bl Func_02004c78
	cmp r0, #0
	blt .L_0200a7a0
	movs r0, #1
	bl wait
	b .L_0200a810
.L_0200a7a0:
	movs r3, #1
	add r8, r3
	mov r0, r8
	adds r7, #128
	adds r5, #128
	cmp r0, #63
	ble .L_0200a770
	mov r2, r10
	movs r1, #0
	ldrsh r3, [r2, r1]
	ldr r2, [r6, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r6, #8]
	mov r2, r9
	movs r0, #0
	ldrsh r3, [r2, r0]
	ldr r2, [r6, #16]
	ldr r7, .L_0200a83c
	lsls r3, r3, #17
	ldr r0, .L_0200a840
	adds r5, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r5, [r6, #16]
	cmp r2, r4
	ble .L_0200a7de
	adds r2, r4, #0
.L_0200a7de:
	ldr r0, .L_0200a844
	cmp r2, r0
	bge .L_0200a7e6
	adds r2, r0, #0
.L_0200a7e6:
	subs r3, r1, r2
	ldr r1, .L_0200a840
	str r3, [r6, #8]
	adds r3, r5, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_0200a7f8
	adds r2, r4, #0
.L_0200a7f8:
	cmp r2, r0
	bge .L_0200a7fe
	adds r2, r0, #0
.L_0200a7fe:
	subs r3, r5, r2
	str r3, [r6, #16]
	ldr r2, [sp, #0]
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl wait
	b .L_0200a696
.L_0200a810:
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_0200a81c:
	ldr r0, [sp, #0]
	movs r3, #0
	strh r3, [r0, #4]
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a830:
	.4byte gMainRoutineWork
.L_0200a834:
	.4byte gData_020023c4 + 0x188
.L_0200a838:
	.4byte Func_02002614
.L_0200a83c:
	.4byte 0x000fffff
.L_0200a840:
	.4byte 0xfff80000
.L_0200a844:
	.4byte 0xffff0000
	.endif
	.section .text.x0200a9c8,"ax",%progbits
	.balign 4
	.global Func_020029c8
	.thumb_func
Func_020029c8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_0200ab30
	sub sp, #4
	ldr r3, [r1, #112]
	mov r11, r0
	cmp r3, #0
	bne .L_0200a9e4
	b .L_0200ab3c
.L_0200a9e4:
	movs r2, #0
	str r2, [sp, #0]
.L_0200a9e8:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r3, r11
	ldr r3, [r3, #8]
	lsls r5, r5, #4
	mov r8, r3
	add r8, r5
	lsls r0, r0, #4
	mov r1, r8
	subs r1, r1, r0
	mov r8, r1
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r2, r11
	ldr r3, [r2, #16]
	ldr r2, [r2, #12]
	lsls r5, r5, #4
	lsls r6, r6, #3
	movs r1, #128
	lsls r0, r0, #4
	adds r6, r6, r2
	lsls r1, r1, #11
	adds r3, r3, r5
	subs r3, r3, r0
	adds r6, r6, r1
	movs r0, #234
	adds r0, #255
	mov r1, r8
	adds r2, r6, #0
	bl Func_02004bd8
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200ab1c
	bl Random16Far
	mov r10, r0
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	movs r2, #128
	lsls r2, r2, #8
	adds r5, r5, r0
	ldr r1, .L_0200ab34
	adds r0, r7, #0
	mov r9, r2
	bl Func_02004bd0
	adds r0, r7, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	mov r1, r10
	movs r2, #128
	lsls r2, r2, #10
	lsls r3, r1, #2
	adds r3, r3, r2
	str r3, [r7, #40]
	mov r0, r10
	bl Math_Cosine
	ldr r3, .L_0200ab38
	lsls r6, r6, #3
	mov r8, r3
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	str r0, [r7, #44]
	mov r0, r10
	bl Math_Sine
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	ldr r2, .L_0200ab30
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	str r3, [r7, #72]
	lsrs r5, r5, #2
	ldr r3, [r2, #112]
	add r5, r9
	movs r6, #0
	mov r1, r9
	str r5, [r7, #24]
	str r5, [r7, #28]
	str r1, [r7, #68]
	ldr r5, [r7, #80]
	str r0, [r7, #36]
	str r6, [r7, #52]
	ldr r3, [r3, #80]
	ldrb r0, [r5, #16]
	mov r8, r3
	bl Resource_ResetEntry
	ldrb r3, [r5, #17]
	ldr r1, .L_0200ab30
	movs r2, #1
	orrs r3, r2
	strb r3, [r5, #17]
	ldrh r3, [r1, #12]
	ldr r0, [r5, #40]
	strb r3, [r5, #16]
	bl ResourceMetadata_ClearRecord
	str r6, [r5, #40]
	strb r6, [r5, #27]
	mov r2, r8
	ldrb r3, [r2, #20]
	ldrb r0, [r5, #5]
	strb r3, [r5, #20]
	ldrb r3, [r2, #21]
	strb r3, [r5, #21]
	ldrb r1, [r2, #5]
	movs r2, #63
	adds r3, r2, #0
	lsrs r1, r1, #6
	lsls r1, r1, #6
	ands r3, r0
	orrs r3, r1
	strb r3, [r5, #5]
	mov r1, r8
	ldrb r3, [r1, #7]
	ldrb r1, [r5, #7]
	lsrs r3, r3, #6
	lsls r3, r3, #6
	ands r2, r1
	orrs r2, r3
	strb r2, [r5, #7]
	mov r3, r8
	ldrh r2, [r3, #8]
	ldr r1, .L_0200ab2c
	ldrh r3, [r5, #8]
	lsls r2, r2, #22
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
.L_0200ab1c:
	ldr r1, [sp, #0]
	subs r1, #1
	str r1, [sp, #0]
	cmp r1, #0
	blt .L_0200ab28
	b .L_0200a9e8
.L_0200ab28:
	b .L_0200ab3c
	.2byte 0x0000
.L_0200ab2c:
	.4byte 0xfffffc00
.L_0200ab30:
	.4byte gData_020023c4 + 0x188
.L_0200ab34:
	.4byte gData_020056d0
.L_0200ab38:
	.4byte IwramMulQ16
.L_0200ab3c:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200afc4,"ax",%progbits
	.balign 4
	.global Func_02002fc4
	.thumb_func
Func_02002fc4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r0
	mov r3, r11
	adds r3, #98
	ldrb r0, [r3]
	sub sp, #12
	bl Actor_GetById
	mov r3, r11
	adds r7, r0, #0
	adds r3, #102
	movs r2, #0
	ldrsh r6, [r3, r2]
	ldr r3, [r7, #80]
	mov r2, r11
	mov r9, r3
	ldr r3, [r2, #8]
	mov r5, sp
	str r3, [r5]
	movs r0, #128
	ldr r3, [r2, #12]
	ldr r2, .L_0200b054
	adds r1, r6, #0
	adds r3, r3, r2
	str r3, [r5, #4]
	mov r2, r11
	ldr r3, [r2, #16]
	lsls r0, r0, #14
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Vector_AddPolarOffsetFar
	ldr r3, .L_0200b050
	movs r2, #0
	mov r10, r3
	adds r3, r7, #0
	mov r8, r2
	adds r3, #85
	mov r2, r10
	strh r6, [r7, #6]
	strb r2, [r3]
	adds r0, r7, #0
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	bl Actor_SetPositionAndResetMotion
	ldr r3, .L_0200b058
	mov r2, r10
	str r3, [r7, #108]
	adds r3, r7, #0
	adds r3, #90
	strb r2, [r3]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r7, #52]
	ldr r3, .L_0200b05c
	mov r2, r9
	adds r6, r6, r3
	b .L_0200b060
	.2byte 0x0000
.L_0200b050:
	.4byte 0x00000000
.L_0200b054:
	.4byte 0xfff40000
.L_0200b058:
	.4byte Func_02002c40
.L_0200b05c:
	.4byte 0xffffc000
.L_0200b060:
	mov r3, r8
	strh r6, [r2, #18]
	str r3, [r7, #24]
	str r3, [r7, #28]
	adds r3, r7, #0
	mov r2, r8
	adds r3, #100
	strh r2, [r3]
	mov r2, r11
	ldr r3, [r2, #104]
	movs r0, #104
	str r3, [r7, #104]
	bl Func_02004de0
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200b564,"ax",%progbits
	.balign 4
	.global Func_02003564
	.thumb_func
Func_02003564:
	.ifdef TLA_EDITION_JA
	push {r5, r6, r7, lr}
	mov r7, fp
	mov r6, sl
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_ja_0200b564
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3, #0]
	sub sp, #40
	bl ObjectTable_Get
	ldr r2, .L_ja_0200b568
	ldr r3, .L_ja_0200b560
	adds r7, r0, #0
	strh r3, [r2, #0]
.L_ja_0200b52a:
	bl Func_02003284
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_ja_0200b56c
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov sl, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, sl
	str r3, [sp, #12]
	str r3, [r0, #0]
	b .L_ja_0200b570
.L_ja_0200b560:
	.4byte 0x00000000
.L_ja_0200b564:
	.4byte gMainRoutineWork
.L_ja_0200b568:
	.4byte gData_020068b0
.L_ja_0200b56c:
	.4byte 0xfff00000
.L_ja_0200b570:
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3, #0]
	ldr r2, [sp, #16]
	bl Func_02004c10
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_ja_0200b5dc
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_ja_0200b5ec
	ldr r3, .L_ja_0200b5e0
	movs r2, #15
	ldr r3, [r3, #0]
	ldr r1, .L_ja_0200b5e4
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_ja_0200b5e8
	cmp r3, r2
	bne .L_ja_0200b618
	b .L_ja_0200b7e0
.L_ja_0200b5dc:
	.4byte IwramMulQ16
.L_ja_0200b5e0:
	.4byte gInput
.L_ja_0200b5e4:
	.4byte gData_02005798
.L_ja_0200b5e8:
	.4byte 0xffff0000
.L_ja_0200b5ec:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_ja_0200b614
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_ja_0200b618
.L_ja_0200b614:
	.4byte 0xffffc000
.L_ja_0200b618:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, sl
	ldr r1, [r2, #0]
	ldrb r0, [r3, #0]
	ldr r2, [r2, #8]
	bl Func_02004c10
	mov fp, r0
	cmp r0, #255
	beq .L_ja_0200b69a
	ldr r3, [sp, #4]
	mov r2, sl
	ldrb r0, [r3, #0]
	ldr r1, [r2, #0]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_ja_0200b69a
	ldr r0, [sp, #12]
	mov r2, sl
	str r0, [r2, #0]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2, #0]
	ldr r1, [sp, #12]
	ldr r3, [sp, #8]
	ldr r2, [r7, #12]
	bl Func_02004bf8
	adds r0, r7, #0
	movs r1, #2
	bl Func_02004bc0
	adds r0, r7, #0
	movs r1, #48
	bl Actor_SetAnimationSpeed
	adds r0, r7, #0
	bl Func_02004c00
	ldr r3, .L_ja_0200b7f0
	str r3, [r7, #108]
	b .L_ja_0200b776
.L_ja_0200b69a:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_ja_0200b7c2
.L_ja_0200b6ae:
	ldr r2, [sp, #4]
	ldr r1, [r6, #0]
	ldrb r0, [r2, #0]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_ja_0200b796
	ldr r3, .L_ja_0200b7f4
	ldrh r3, [r3, #0]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_ja_0200b796
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov sl, r0
	mov r8, r2
.L_ja_0200b6e6:
	ldr r3, [r5, #0]
	cmp r3, #0
	beq .L_ja_0200b710
	mov r3, r8
	ldrb r2, [r3, #0]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_ja_0200b710
	cmp r5, r7
	beq .L_ja_0200b710
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02004c78
	cmp r0, #0
	bge .L_ja_0200b796
.L_ja_0200b710:
	movs r0, #1
	add sl, r0
	movs r2, #128
	mov r3, sl
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_ja_0200b6e6
	ldr r0, [r6, #0]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	movs r5, #0
	ldr r1, [r6, #0]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Func_02004bf8
	b .L_ja_0200b74e
.L_ja_0200b746:
	movs r0, #1
	bl wait
	adds r5, #1
.L_ja_0200b74e:
	cmp r5, #179
	bgt .L_ja_0200b766
	adds r0, r7, #0
	bl Func_02004c70
	cmp r0, #0
	bne .L_ja_0200b766
	ldr r3, .L_ja_0200b7f4
	ldrh r3, [r3, #0]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_ja_0200b746
.L_ja_0200b766:
	ldr r3, .L_ja_0200b7f4
	ldrh r3, [r3, #0]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_ja_0200b7c2
	ldr r3, [sp, #24]
	cmp fp, r3
	bne .L_ja_0200b7bc
.L_ja_0200b776:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2, #0]
	ldr r1, [r6, #0]
	ldr r2, [r6, #8]
	bl Func_02004c10
	mov fp, r0
	cmp r0, #255
	bne .L_ja_0200b6ae
.L_ja_0200b796:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_02004bf8
	adds r0, r7, #0
	bl Func_02004c00
	movs r0, #2
	bl wait
	b .L_ja_0200b52a
.L_ja_0200b7bc:
	movs r0, #10
	bl wait
.L_ja_0200b7c2:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1, #0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1, #0]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_02004bc0
.L_ja_0200b7e0:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov sl, r6
	mov fp, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_0200b7f0:
	.4byte Func_020031f8
.L_ja_0200b7f4:
	.4byte gData_020068b0
	.else
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200b5c4
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	ldr r2, .L_0200b5c8
	ldr r3, .L_0200b5c0
	adds r7, r0, #0
	strh r3, [r2]
.L_0200b58a:
	bl Func_02003284
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_0200b5cc
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, r10
	str r3, [sp, #12]
	str r3, [r0]
	b .L_0200b5d0
.L_0200b5c0:
	.4byte 0x00000000
.L_0200b5c4:
	.4byte gMainRoutineWork
.L_0200b5c8:
	.4byte gData_020068b0
.L_0200b5cc:
	.4byte 0xfff00000
.L_0200b5d0:
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3]
	ldr r2, [sp, #16]
	bl Func_02004c10
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_0200b63c
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_0200b64c
	ldr r3, .L_0200b640
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_0200b644
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_0200b648
	cmp r3, r2
	bne .L_0200b678
	b .L_0200b842
.L_0200b63c:
	.4byte IwramMulQ16
.L_0200b640:
	.4byte gInput
.L_0200b644:
	.4byte gData_02005798
.L_0200b648:
	.4byte 0xffff0000
.L_0200b64c:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_0200b674
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_0200b678
.L_0200b674:
	.4byte 0xffffc000
.L_0200b678:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, r10
	ldr r1, [r2]
	ldrb r0, [r3]
	ldr r2, [r2, #8]
	bl Func_02004c10
	mov r11, r0
	cmp r0, #255
	beq .L_0200b6f2
	ldr r3, [sp, #4]
	mov r2, r10
	ldrb r0, [r3]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200b6f2
	ldr r0, [sp, #12]
	mov r2, r10
	str r0, [r2]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	ldr r1, [sp, #12]
	ldr r2, [r7, #12]
	ldr r3, [sp, #8]
	bl Func_02004bf8
	adds r0, r7, #0
	movs r1, #2
	bl Func_02004bc0
	adds r0, r7, #0
	movs r1, #48
	bl Actor_SetAnimationSpeed
	movs r5, #0
	b .L_0200b71a
.L_0200b6f2:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_0200b842
.L_0200b706:
	ldr r3, .L_0200b870
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_0200b712
	b .L_0200b842
.L_0200b712:
	movs r0, #1
	bl wait
	adds r5, #1
.L_0200b71a:
	cmp r5, #179
	bgt .L_0200b728
	adds r0, r7, #0
	bl Func_02004c70
	cmp r0, #0
	beq .L_0200b706
.L_0200b728:
	ldr r3, .L_0200b874
	str r3, [r7, #108]
	b .L_0200b7f6
.L_0200b72e:
	ldr r2, [sp, #4]
	ldr r1, [r6]
	ldrb r0, [r2]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200b816
	ldr r3, .L_0200b870
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200b842
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov r10, r0
	mov r8, r2
.L_0200b766:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200b790
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200b790
	cmp r5, r7
	beq .L_0200b790
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02004c78
	cmp r0, #0
	bge .L_0200b816
.L_0200b790:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_0200b766
	ldr r0, [r6]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	movs r5, #0
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Func_02004bf8
	b .L_0200b7ce
.L_0200b7c6:
	movs r0, #1
	bl wait
	adds r5, #1
.L_0200b7ce:
	cmp r5, #179
	bgt .L_0200b7e6
	adds r0, r7, #0
	bl Func_02004c70
	cmp r0, #0
	bne .L_0200b7e6
	ldr r3, .L_0200b870
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_0200b7c6
.L_0200b7e6:
	ldr r3, .L_0200b870
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200b842
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_0200b83c
.L_0200b7f6:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_02004c10
	mov r11, r0
	cmp r0, #255
	bne .L_0200b72e
.L_0200b816:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_02004bf8
	adds r0, r7, #0
	bl Func_02004c00
	movs r0, #2
	bl wait
	b .L_0200b58a
.L_0200b83c:
	movs r0, #10
	bl wait
.L_0200b842:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_02004bc0
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b870:
	.4byte gData_020068b0
.L_0200b874:
	.4byte Func_020031f8
	.endif
	.section .text.x0200c490,"ax",%progbits
	.global Func_02004490
	.thumb_func
Func_02004490:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200c538
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Actor_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_0200c53c
	asrs r3, r3, #20
	mov r8, r3
	ldr r3, [r0, #16]
	adds r5, r6, #0
	asrs r3, r3, #20
	mov r10, r3
	movs r1, #10
	ldrsh r3, [r6, r1]
	movs r7, #0
	adds r5, #32
	ldrh r2, [r6, #10]
	cmp r7, r3
	bge .L_0200c52c
.L_0200c4c4:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_0200c520
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_0200c520
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c4f4
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_020040f0
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_0200c52c
.L_0200c4f4:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_0200c52c
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_02004160
	movs r2, #2
	ldrsh r0, [r6, r2]
	mov r1, r8
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r6, r3]
	mov r1, r10
	adds r0, #8
	bl GameFlag_SetByte
	movs r0, #1
	b .L_0200c52e
.L_0200c520:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_0200c4c4
.L_0200c52c:
	movs r0, #0
.L_0200c52e:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c538:
	.4byte gMainRoutineWork
.L_0200c53c:
	.4byte Data_020068b4
	.section .text.x0200c5f8,"ax",%progbits
	.global Func_020045f8
	.thumb_func
Func_020045f8:
	push {r5, r6, lr}
	adds r4, r1, #0
	movs r1, #192
	lsls r1, r1, #18
	adds r1, #128
	ldr r6, [r1]
	ldr r1, .L_0200c69c
	adds r5, r0, #0
	str r5, [r1]
	ldr r1, .L_0200c6a0
	str r4, [r1]
	ldr r1, .L_0200c6a4
	str r2, [r1]
	ldr r2, .L_0200c6a8
	str r3, [r2]
	movs r2, #255
	ldrh r3, [r5]
	b .L_0200c642
.L_0200c61c:
	ldrh r0, [r4]
	adds r4, #2
	ldrh r2, [r4]
	adds r4, #2
	ldrh r1, [r5]
	ldrh r3, [r4]
	adds r5, #2
	adds r4, #2
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	movs r2, #160
	lsls r2, r2, #19
	lsls r1, r1, #1
	orrs r3, r0
	adds r1, r1, r2
	strh r3, [r1]
	ldrh r3, [r5]
	movs r2, #255
.L_0200c642:
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_0200c650
	ldrh r3, [r4]
	cmp r3, r2
	bne .L_0200c61c
.L_0200c650:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r0, #160
	lsls r2, r2, #24
	adds r3, #212
	lsls r0, r0, #19
	adds r1, r6, #0
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #224
	lsls r2, r2, #1
	adds r1, r6, r2
	movs r2, #132
	lsls r2, r2, #24
	ldr r0, .L_0200c6ac
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02004d58
	ldr r3, .L_0200c6b0
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #2
	bne .L_0200c69a
	bl Func_020048e8
.L_0200c69a:
	pop {r5, r6, pc}
.L_0200c69c:
	.4byte gData_0200e88c
.L_0200c6a0:
	.4byte gData_02006890
.L_0200c6a4:
	.4byte Data_02006894
.L_0200c6a8:
	.4byte Data_02006880
.L_0200c6ac:
	.4byte 0x05000200
.L_0200c6b0:
	.4byte gMainRoutineWork
	.section .text.x0200c7b8,"ax",%progbits
	.balign 4
	.global Func_020047b8
	.thumb_func
Func_020047b8:
	push {r5, r6, r7, lr}
	ldr r1, .L_0200c868
	movs r4, #0
	ldrh r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0200c7f2
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r2, r0
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200c7f2
	ldr r0, .L_0200c86c
	movs r4, #1
	ldrh r2, [r0]
	strh r2, [r1]
	movs r1, #128
	lsls r3, r2, #16
	lsls r1, r1, #10
	cmp r3, r1
	bls .L_0200c7f2
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
	strh r3, [r0]
.L_0200c7f2:
	cmp r4, #0
	bne .L_0200c7f8
	b .L_0200c8e4
.L_0200c7f8:
	ldr r3, .L_0200c870
	ldr r6, .L_0200c874
	ldr r1, [r3]
	ldrh r3, [r6]
	movs r5, #0
	cmp r5, r3
	bcs .L_0200c846
	ldr r3, .L_0200c878
	ldr r2, .L_0200c87c
	ldr r7, [r3]
	mov lr, r2
	mov r12, r6
.L_0200c810:
	mov r3, lr
	ldrh r2, [r3]
	ldrh r3, [r6]
	movs r0, #160
	muls r3, r2
	adds r3, r3, r5
	lsls r3, r3, #1
	ldrh r3, [r3, r7]
	lsls r0, r0, #19
	lsls r3, r3, #1
	adds r4, r3, r0
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	adds r1, #2
	ldrh r3, [r1]
	adds r1, #2
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	adds r5, #1
	mov r2, r12
	ldrh r3, [r2]
	cmp r5, r3
	bcc .L_0200c810
.L_0200c846:
	ldr r3, .L_0200c874
	movs r0, #160
	ldrh r1, [r3]
	ldr r3, .L_0200c880
	lsls r2, r1, #1
	ldr r3, [r3]
	lsls r0, r0, #19
	ldrh r3, [r2, r3]
	adds r2, r2, r1
	lsls r3, r3, #1
	adds r4, r3, r0
	ldr r3, .L_0200c884
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_0200c88c
	ldr r3, .L_0200c888
	b .L_0200c88e
.L_0200c868:
	.4byte gData_02006884
.L_0200c86c:
	.4byte gData_0200e888
.L_0200c870:
	.4byte Data_02006894
.L_0200c874:
	.4byte gData_02006898
.L_0200c878:
	.4byte Data_02006880
.L_0200c87c:
	.4byte gData_0200689c
.L_0200c880:
	.4byte gData_0200e88c
.L_0200c884:
	.4byte gData_0200687c
.L_0200c888:
	.4byte gData_02006890
.L_0200c88c:
	ldr r3, .L_0200c8d4
.L_0200c88e:
	lsls r2, r2, #1
	ldr r3, [r3]
	adds r1, r3, r2
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	ldrh r3, [r1, #2]
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	ldr r1, .L_0200c8d8
	ldr r2, .L_0200c8cc
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	ldr r1, .L_0200c8dc
	ldr r2, .L_0200c8e0
	ldrh r3, [r1]
	adds r3, #1
	strh r3, [r1]
	lsls r3, r3, #16
	ldrh r2, [r2]
	lsrs r3, r3, #16
	cmp r3, r2
	bne .L_0200c8e4
	ldr r3, .L_0200c8d0
	strh r3, [r1]
	b .L_0200c8e4
	.2byte 0x0000
.L_0200c8cc:
	.4byte 0x00000001
.L_0200c8d0:
	.4byte 0x00000000
.L_0200c8d4:
	.4byte Data_02006894
.L_0200c8d8:
	.4byte gData_0200687c
.L_0200c8dc:
	.4byte gData_0200689c
.L_0200c8e0:
	.4byte gData_02006898
.L_0200c8e4:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .bss,"aw",%nobits
	.global Data_02006878
Data_02006878:
	.space 0x00000004
	.global gData_0200687c
gData_0200687c:
	.space 0x00000004
	.global Data_02006880
Data_02006880:
	.space 0x00000004
	.global gData_02006884
gData_02006884:
	.space 0x00000004
	.global gData_0200e888
gData_0200e888:
	.space 0x00000004
	.global gData_0200e88c
gData_0200e88c:
	.space 0x00000004
	.global gData_02006890
gData_02006890:
	.space 0x00000004
	.global Data_02006894
Data_02006894:
	.space 0x00000004
	.global gData_02006898
gData_02006898:
	.space 0x00000004
	.global gData_0200689c
gData_0200689c:
	.space 0x00000004
	.global gData_020068a0
gData_020068a0:
	.space 0x00000002
	.global gData_020068a2
gData_020068a2:
	.space 0x00000002
	.global Data_020068a4
Data_020068a4:
	.space 0x0000000c
	.global gData_020068b0
gData_020068b0:
	.space 0x00000004
	.global Data_020068b4
Data_020068b4:
