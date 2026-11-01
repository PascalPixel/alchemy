.syntax unified
	.thumb
	.section .text.x0200af32,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200b308,"ax",%progbits
	.balign 4
	.global Func_02003308
	.thumb_func
Func_02003308:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	movs r2, #99
	adds r2, r2, r6
	ldrb r3, [r2]
	sub sp, #12
	movs r1, #0
	mov r8, r2
	cmp r3, #0
	beq .L_0200b342
	adds r3, r6, #0
	adds r2, r6, #0
	adds r3, #100
	adds r2, #102
	movs r1, #0
	ldrsh r2, [r2, r1]
	movs r4, #0
	ldrsh r3, [r3, r4]
	adds r3, r3, r2
	lsls r0, r3, #3
	adds r0, r0, r3
	lsls r0, r0, #9
	bl Math_Sine
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r1, r3, #1
.L_0200b342:
	ldr r3, [r6, #56]
	mov r5, sp
	str r3, [r5]
	adds r7, r6, #0
	ldr r3, [r6, #60]
	adds r7, #100
	str r3, [r5, #4]
	ldr r3, [r6, #64]
	str r3, [r5, #8]
	adds r3, r6, #0
	ldr r0, [r6, #76]
	adds r3, #102
	movs r4, #0
	ldrsh r2, [r3, r4]
	movs r4, #0
	ldrsh r3, [r7, r4]
	lsls r0, r0, #16
	adds r0, r0, r1
	lsls r1, r3, #1
	adds r1, r1, r3
	lsls r1, r1, #8
	adds r1, r1, r2
	adds r2, r5, #0
	bl Vector_AddPolarOffsetFar
	ldr r3, [r5]
	str r3, [r6, #8]
	ldr r3, [r5, #4]
	str r3, [r6, #12]
	ldr r3, [r5, #8]
	adds r5, r6, #0
	str r3, [r6, #16]
	adds r5, #98
	ldrb r3, [r5]
	cmp r3, #7
	bls .L_0200b38c
	b .L_0200b4f2
.L_0200b38c:
	ldr r2, .L_0200b4fc
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0200b394:
	.4byte .L_0200b3b4
	.4byte .L_0200b3f8
	.4byte .L_0200b42c
	.4byte .L_0200b44a
	.4byte .L_0200b468
	.4byte .L_0200b482
	.4byte .L_0200b4b0
	.4byte .L_0200b4e4
.L_0200b3b4:
	ldrh r3, [r7]
	movs r0, #1
	adds r3, #4
	strh r3, [r7]
	str r0, [r6, #76]
	ldr r2, [r6, #80]
	movs r1, #128
	ldrh r3, [r2, #18]
	lsls r1, r1, #5
	adds r3, r3, r1
	strh r3, [r2, #18]
	movs r1, #192
	ldr r3, [r6, #28]
	lsls r1, r1, #3
	ldr r2, [r6, #24]
	adds r1, #102
	adds r3, r3, r1
	str r3, [r6, #28]
	movs r3, #192
	adds r2, r2, r1
	lsls r3, r3, #9
	str r2, [r6, #24]
	cmp r2, r3
	bgt .L_0200b3e6
	b .L_0200b4f2
.L_0200b3e6:
	ldr r3, [r6, #80]
	ldrh r3, [r3, #18]
	cmp r3, #0
	beq .L_0200b3f0
	b .L_0200b4f2
.L_0200b3f0:
	mov r4, r8
	strb r0, [r5]
	strb r0, [r4]
	b .L_0200b4f2
.L_0200b3f8:
	ldrh r3, [r7]
	movs r1, #128
	adds r3, #4
	strh r3, [r7]
	lsls r1, r1, #5
	ldr r2, [r6, #80]
	ldrh r3, [r2, #18]
	adds r3, r3, r1
	strh r3, [r2, #18]
	ldr r3, [r6, #80]
	ldrh r3, [r3, #18]
	cmp r3, #0
	bne .L_0200b4f2
	mov r2, r8
	ldrb r3, [r2]
	adds r3, #255
	strb r3, [r2]
	lsls r3, r3, #24
	cmp r3, #0
	bne .L_0200b4f2
	movs r0, #220
	bl Func_02005c9c
	movs r3, #2
	strb r3, [r5]
	b .L_0200b4f2
.L_0200b42c:
	ldrh r3, [r7]
	movs r4, #128
	adds r3, #1
	strh r3, [r7]
	lsls r4, r4, #11
	ldr r3, [r6, #60]
	movs r1, #200
	adds r3, r3, r4
	lsls r1, r1, #15
	str r3, [r6, #60]
	cmp r3, r1
	ble .L_0200b4f2
	movs r3, #3
	strb r3, [r5]
	b .L_0200b4f2
.L_0200b44a:
	ldrh r3, [r7]
	mov r2, r8
	adds r3, #1
	strh r3, [r7]
	ldr r3, [r6, #76]
	adds r3, #2
	str r3, [r6, #76]
	movs r3, #10
	strb r3, [r2]
	ldr r3, [r6, #76]
	cmp r3, #40
	ble .L_0200b4f2
	movs r3, #4
	strb r3, [r5]
	b .L_0200b4f2
.L_0200b468:
	ldrh r3, [r7]
	mov r4, r8
	adds r3, #1
	strh r3, [r7]
	ldrb r3, [r4]
	adds r3, #255
	strb r3, [r4]
	lsls r3, r3, #24
	cmp r3, #0
	bne .L_0200b4f2
	movs r3, #5
	strb r3, [r5]
	b .L_0200b4f2
.L_0200b482:
	ldrh r3, [r7]
	movs r1, #136
	adds r3, #1
	strh r3, [r7]
	lsls r1, r1, #16
	ldr r3, [r6, #76]
	subs r3, #2
	str r3, [r6, #76]
	ldr r3, [r6, #60]
	cmp r3, r1
	bge .L_0200b4a0
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r6, #60]
.L_0200b4a0:
	ldr r3, [r6, #76]
	cmp r3, #1
	bgt .L_0200b4f2
	movs r3, #0
	str r3, [r6, #76]
	movs r3, #6
	strb r3, [r5]
	b .L_0200b4f2
.L_0200b4b0:
	ldrh r3, [r7]
	movs r4, #128
	adds r3, #4
	strh r3, [r7]
	lsls r4, r4, #5
	ldr r2, [r6, #80]
	ldr r1, .L_0200b500
	ldrh r3, [r2, #18]
	adds r3, r3, r4
	strh r3, [r2, #18]
	ldr r2, [r6, #24]
	ldr r3, [r6, #28]
	adds r2, r2, r1
	adds r3, r3, r1
	movs r1, #144
	lsls r1, r1, #5
	str r2, [r6, #24]
	str r3, [r6, #28]
	cmp r2, r1
	bge .L_0200b4f2
	movs r3, #100
	str r3, [r6, #56]
	str r3, [r6, #64]
	movs r3, #7
	strb r3, [r5]
	b .L_0200b4f2
.L_0200b4e4:
	ldr r3, [r6, #80]
	ldrb r0, [r3, #16]
	bl Resource_ResetEntry
	adds r0, r6, #0
	bl Func_02005a3c
.L_0200b4f2:
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b4fc:
	.4byte .L_0200b394
.L_0200b500:
	.4byte 0xfffff000
	.section .text.x0200b504,"ax",%progbits
	.global Func_02003504
	.thumb_func
Func_02003504:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #17
	bl Actor_GetById
	mov r8, r0
	movs r0, #0
	mov r11, r0
	movs r0, #154
	bl Func_02005c9c
	movs r2, #0
	mov r10, r2
.L_0200b528:
	movs r0, #17
	bl Actor_GetById
	movs r1, #7
	bl Actor_SetPartAttribute
	movs r0, #2
	bl wait
	movs r0, #17
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetPartAttribute
	movs r0, #2
	bl wait
	movs r3, #1
	add r10, r3
	mov r0, r10
	cmp r0, #7
	bls .L_0200b528
	movs r0, #209
	bl Func_02005c9c
	movs r2, #0
	mov r10, r2
	mov r9, r2
.L_0200b562:
	mov r3, r8
	ldr r2, [r3, #12]
	movs r0, #192
	lsls r0, r0, #13
	adds r2, r2, r0
	movs r0, #209
	lsls r0, r0, #1
	ldr r1, [r3, #8]
	adds r0, #255
	ldr r3, [r3, #16]
	bl Func_02005a34
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200b5f4
	mov r1, r11
	ldr r0, [r7, #80]
	bl Func_02005c44
	adds r3, r7, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	ldr r1, [r7, #80]
	mov r11, r0
	ldrb r3, [r1, #9]
	movs r0, #13
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	adds r0, r7, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r0, r7, #0
	movs r1, #1
	bl Func_02005a24
	adds r3, r7, #0
	adds r3, #100
	movs r1, #180
	strh r5, [r3]
	lsls r1, r1, #1
	str r5, [r7, #76]
	mov r0, r9
	bl __udivsi3
	ldr r6, .L_0200b5ec
	adds r3, r7, #0
	adds r3, #102
	strh r0, [r3]
	subs r3, #4
	strb r6, [r3]
	mov r2, r8
	ldr r3, [r2, #8]
	movs r0, #192
	str r3, [r7, #56]
	lsls r0, r0, #13
	ldr r3, [r2, #12]
	adds r3, r3, r0
	str r3, [r7, #60]
	ldr r3, [r2, #16]
	str r3, [r7, #64]
	ldr r3, .L_0200b5f0
	str r3, [r7, #108]
	b .L_0200b5f4
.L_0200b5ec:
	.4byte 0x00000000
.L_0200b5f0:
	.4byte Func_02003308
.L_0200b5f4:
	movs r3, #1
	movs r2, #176
	add r10, r3
	lsls r2, r2, #13
	mov r0, r10
	add r9, r2
	cmp r0, #15
	bls .L_0200b562
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x0200c524,"ax",%progbits
	.global Func_02004524
	.thumb_func
Func_02004524:
	push {r5, lr}
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r1, r1
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	bl Func_02005bbc
	movs r3, #0
	adds r0, #85
	ldr r5, .L_0200c5c4
	strb r3, [r0]
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r5, r1
	ldr r0, [r5]
	bl Actor_GetById
	movs r1, #15
	bl Actor_SetPartAttribute
	ldr r0, [r5]
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200c5c8
	bl Task_Add
	movs r0, #1
	movs r1, #1
	bl Func_02003640
	ldr r5, .L_0200c5cc
	ldr r2, [r5]
	cmp r2, #0
	beq .L_0200c59e
	ldr r3, [r2, #12]
	movs r1, #128
	lsls r1, r1, #14
	adds r3, r3, r1
	str r3, [r2, #12]
	movs r0, #1
	bl wait
	ldr r3, [r5]
	movs r2, #4
	adds r3, #85
	strb r2, [r3]
.L_0200c59e:
	movs r0, #1
	bl wait
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #40
	bl FieldEvent_Wait
	pop {r5, pc}
.L_0200c5c4:
	.4byte gMainRoutineWork
.L_0200c5c8:
	.4byte Func_02002ccc
.L_0200c5cc:
	.4byte gData_02007950
	.section .bss,"aw",%nobits
	.space 0x00000004
	.global gData_02007948
gData_02007948:
	.space 0x00000008
	.global gData_02007950
gData_02007950:
