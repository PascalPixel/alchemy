.syntax unified
	.thumb
	.global Func_0802a124
	.thumb_func
Func_0802a124:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	adds r1, r7, #0
	sub sp, #100
	adds r1, #102
	movs r0, #0
	str r0, [sp, #16]
	str r1, [sp, #4]
	add r2, sp, #16
	ldrh r2, [r2]
	adds r3, r1, #0
	strh r2, [r3]
	adds r2, r7, #0
	movs r3, #1
	adds r2, #100
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	ldr r5, .L_0802a460
	movs r6, #128
	ldr r3, [r5, #12]
	lsls r6, r6, #2
	ands r3, r6
	cmp r3, #0
	beq .L_0802a170
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r7, #48]
.L_0802a170:
	ldr r3, [r5]
	ldr r1, .L_0802a464
	lsrs r3, r3, #4
	movs r2, #15
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r4, [r1, r3]
	movs r1, #255
	lsls r3, r4, #16
	lsrs r3, r3, #16
	lsls r1, r1, #8
	mov r8, r3
	adds r1, #255
	str r4, [sp, #12]
	cmp r8, r1
	bne .L_0802a19e
	movs r2, #4
	str r2, [sp, #16]
	b .L_0802a310
.L_0802a196:
	mov r3, r9
	asrs r3, r3, #16
	str r3, [sp, #12]
	b .L_0802a310
.L_0802a19e:
	mov r4, sp
	adds r4, #88
	str r4, [sp, #0]
	ldr r3, [r7, #8]
	movs r0, #128
	str r3, [r4]
	ldr r3, [r7, #12]
	lsls r0, r0, #12
	str r3, [r4, #4]
	ldr r3, [r7, #16]
	mov r1, r8
	str r3, [r4, #8]
	ldr r2, [sp, #0]
	bl Vector_AddPolarOffset
	ldr r3, .L_0802a468
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0802a1ce
	ldr r3, [r5]
	ands r3, r6
	cmp r3, #0
	beq .L_0802a1ce
	b .L_0802a310
.L_0802a1ce:
	ldr r3, [r7, #8]
	add r0, sp, #76
	str r3, [r0]
	ldr r3, [r7, #12]
	movs r1, #168
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	lsls r1, r1, #5
	mov r11, r0
	str r3, [r0, #8]
	adds r1, #85
	movs r0, #128
	add r1, r8
	lsls r0, r0, #12
	mov r2, r11
	bl Vector_AddPolarOffset
	ldr r3, [r7, #8]
	add r1, sp, #64
	str r3, [r1]
	ldr r3, [r7, #12]
	mov r9, r1
	str r3, [r1, #4]
	ldr r3, [r7, #16]
	movs r0, #128
	str r3, [r1, #8]
	ldr r1, .L_0802a46c
	mov r2, r9
	add r1, r8
	lsls r0, r0, #12
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	ldr r1, [sp, #0]
	bl Func_0802db64
	mov r1, r11
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_0802db64
	mov r1, r9
	adds r6, r0, #0
	adds r0, r7, #0
	bl Func_0802db64
	orrs r5, r6
	orrs r5, r0
	cmp r5, #0
	beq .L_0802a310
	movs r2, #20
	ldr r0, [sp, #16]
	add r2, sp
	movs r3, #128
	mov r10, r2
	lsls r3, r3, #5
	add r3, r8
	mov r4, r10
	strh r3, [r4, r0]
	ldr r3, .L_0802a470
	mov r1, r10
	add r3, r8
	strh r3, [r1, #2]
	movs r3, #128
	lsls r3, r3, #6
	add r3, r8
	strh r3, [r2, #4]
	ldr r3, .L_0802a474
	mov r0, r10
	add r3, r8
	strh r3, [r4, #6]
	movs r3, #192
	lsls r3, r3, #6
	add r3, r8
	strh r3, [r0, #8]
	ldr r3, .L_0802a478
	movs r2, #0
	add r3, r8
	strh r3, [r1, #10]
	str r2, [sp, #8]
	mov r8, r9
.L_0802a270:
	ldr r4, [sp, #8]
	mov r0, r10
	lsls r3, r4, #1
	ldrsh r2, [r0, r3]
	ldr r3, [r7, #8]
	lsls r2, r2, #16
	str r3, [sp, #88]
	ldr r3, [r7, #12]
	lsrs r5, r2, #16
	str r3, [sp, #92]
	ldr r3, [r7, #16]
	movs r0, #128
	str r3, [sp, #96]
	add r3, sp, #88
	adds r1, r5, #0
	lsls r0, r0, #12
	mov r9, r2
	adds r2, r3, #0
	bl Vector_AddPolarOffset
	ldr r3, [r7, #8]
	mov r4, r11
	str r3, [r4]
	ldr r3, [r7, #12]
	movs r0, #168
	str r3, [r4, #4]
	ldr r3, [r7, #16]
	lsls r0, r0, #5
	adds r0, #85
	adds r1, r5, r0
	movs r0, #128
	str r3, [r4, #8]
	lsls r0, r0, #12
	mov r2, r11
	bl Vector_AddPolarOffset
	ldr r3, [r7, #8]
	mov r1, r8
	str r3, [r1]
	ldr r3, [r7, #12]
	ldr r2, .L_0802a46c
	str r3, [r1, #4]
	ldr r3, [r7, #16]
	adds r5, r5, r2
	movs r0, #128
	mov r2, r8
	str r3, [r1, #8]
	lsls r0, r0, #12
	adds r1, r5, #0
	bl Vector_AddPolarOffset
	add r3, sp, #88
	adds r1, r3, #0
	adds r0, r7, #0
	bl Func_0802db64
	mov r1, r11
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_0802db64
	mov r1, r8
	adds r6, r0, #0
	adds r0, r7, #0
	bl Func_0802db64
	orrs r5, r6
	orrs r5, r0
	cmp r5, #0
	bne .L_0802a2fe
	b .L_0802a196
.L_0802a2fe:
	ldr r4, [sp, #8]
	adds r4, #1
	str r4, [sp, #8]
	cmp r4, #6
	blt .L_0802a270
	ldr r0, [sp, #16]
	movs r3, #1
	orrs r0, r3
	str r0, [sp, #16]
.L_0802a310:
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	cmp r1, #0
	beq .L_0802a346
	ldr r3, [sp, #16]
	movs r2, #3
	ands r2, r3
	cmp r2, #0
	beq .L_0802a332
	movs r4, #194
	lsls r4, r4, #1
	adds r2, r1, r4
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0802a33a
.L_0802a332:
	movs r0, #194
	lsls r0, r0, #1
	adds r3, r1, r0
	strh r2, [r3]
.L_0802a33a:
	ldr r3, .L_0802a460
	movs r4, #195
	ldr r3, [r3]
	lsls r4, r4, #1
	adds r2, r1, r4
	strh r3, [r2]
.L_0802a346:
	adds r0, r7, #0
	movs r1, #1
	bl ObjectDispatch_ApplyArgumentToChildren
	ldr r0, [sp, #16]
	cmp r0, #0
	beq .L_0802a3a2
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #60]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	movs r3, #3
	ands r3, r0
	cmp r3, #0
	beq .L_0802a390
	ldr r1, [sp, #12]
	ldrh r2, [r7, #6]
	lsls r3, r1, #16
	lsrs r3, r3, #16
	subs r3, r3, r2
	lsls r3, r3, #16
	asrs r1, r3, #16
	movs r3, #128
	lsls r3, r3, #5
	cmp r1, r3
	ble .L_0802a384
	adds r1, r3, #0
.L_0802a384:
	ldr r3, .L_0802a470
	cmp r1, r3
	bge .L_0802a38c
	adds r1, r3, #0
.L_0802a38c:
	adds r3, r2, r1
	strh r3, [r7, #6]
.L_0802a390:
	movs r2, #98
	adds r2, r2, r7
	movs r3, #0
	strb r3, [r2]
	ldr r4, [sp, #4]
	movs r3, #2
	mov r8, r2
	strh r3, [r4]
	b .L_0802a3f4
.L_0802a3a2:
	add r3, sp, #88
	ldr r2, [r3, #4]
	ldr r1, [r3]
	adds r0, r7, #0
	ldr r3, [r3, #8]
	bl Object_SetMoveTarget
	ldr r1, [r7, #36]
	ldr r6, .L_0802a47c
	adds r0, r1, #0
	mov lr, r6
	.2byte 0xf800
	ldr r1, [r7, #44]
	adds r5, r0, #0
	adds r0, r1, #0
	mov lr, r6
	.2byte 0xf800
	adds r5, r5, r0
	adds r0, r5, #0
	bl Func_080149e0
	ldr r1, [sp, #16]
	str r1, [r7, #36]
	str r1, [r7, #44]
	ldr r2, [sp, #12]
	lsls r1, r2, #16
	adds r2, r7, #0
	adds r2, #36
	lsrs r1, r1, #16
	bl Vector_AddPolarOffset
	movs r3, #98
	adds r3, r3, r7
	ldrb r2, [r3]
	mov r8, r3
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0802a3f4
	adds r3, #255
	mov r4, r8
	strb r3, [r4]
.L_0802a3f4:
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	ldr r3, .L_0802a460
	ldr r1, .L_0802a480
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	movs r2, #143
	lsls r3, r3, #2
	lsls r2, r2, #1
	ldr r4, [r1, r3]
	adds r1, r0, r2
	ldrh r0, [r1]
	subs r3, r4, r0
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r3, #0
	bge .L_0802a41e
	adds r3, #7
.L_0802a41e:
	asrs r2, r3, #3
	movs r3, #128
	lsls r3, r3, #2
	cmp r2, r3
	ble .L_0802a42a
	adds r2, r3, #0
.L_0802a42a:
	ldr r3, .L_0802a484
	cmp r2, r3
	bge .L_0802a432
	adds r2, r3, #0
.L_0802a432:
	adds r3, r2, #0
	adds r3, #15
	cmp r3, #30
	bhi .L_0802a43e
	ldrh r3, [r1]
	subs r2, r4, r3
.L_0802a43e:
	adds r3, r0, r2
	strh r3, [r1]
	adds r3, r7, #0
	adds r3, #84
	ldrb r6, [r3]
	cmp r6, #1
	bne .L_0802a48e
	adds r0, r7, #0
	adds r0, #8
	ldr r5, [r7, #80]
	bl GetWorldMapCollision
	cmp r0, #9
	bne .L_0802a488
	ldr r3, [r5, #44]
	strb r6, [r3, #6]
	b .L_0802a48e
.L_0802a460:
	.4byte gInput
.L_0802a464:
	.4byte Data_0802ec5c
.L_0802a468:
	.4byte Data_03001238
.L_0802a46c:
	.4byte 0xffffeaab
.L_0802a470:
	.4byte 0xfffff000
.L_0802a474:
	.4byte 0xffffe000
.L_0802a478:
	.4byte 0xffffd000
.L_0802a47c:
	.4byte IwramMulQ16
.L_0802a480:
	.4byte Data_0802eca0
.L_0802a484:
	.4byte 0xfffffe00
.L_0802a488:
	ldr r2, [r5, #44]
	movs r3, #9
	strb r3, [r2, #6]
.L_0802a48e:
	mov r4, r8
	ldrb r3, [r4]
	cmp r3, #0
	bne .L_0802a506
	ldr r0, [sp, #16]
	cmp r0, #0
	bne .L_0802a506
	ldr r3, [r7, #16]
	ldr r4, .L_0802a524
	movs r0, #178
	lsls r0, r0, #1
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	adds r3, r3, r4
	bl Func_08023220
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0802a506
	ldr r3, [r7, #20]
	ldr r1, .L_0802a528
	str r3, [r5, #20]
	ldr r6, [r5, #80]
	bl ObjectDispatch_Initialize
	adds r2, r5, #0
	adds r2, #35
	movs r3, #8
	add r0, sp, #16
	strb r3, [r2]
	ldrb r0, [r0]
	movs r3, #100
	adds r2, #64
	strb r3, [r2]
	adds r3, r5, #0
	adds r3, #85
	strb r0, [r3]
	cmp r6, #0
	beq .L_0802a500
	movs r1, #1
	adds r0, r6, #0
	bl Animation_ApplyChildArgument
	add r1, sp, #16
	ldrb r1, [r1]
	ldrb r2, [r6, #9]
	strb r1, [r6, #26]
	movs r1, #13
	negs r1, r1
	adds r3, r1, #0
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	ands r3, r1
	movs r2, #4
	orrs r3, r2
	strb r3, [r6, #9]
.L_0802a500:
	movs r3, #5
	mov r2, r8
	strb r3, [r2]
.L_0802a506:
	adds r0, r7, #0
	bl Func_08029838
	ldrh r3, [r7, #4]
	movs r0, #1
	adds r3, #1
	strh r3, [r7, #4]
	add sp, #100
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0802a524:
	.4byte 0xfffe0000
.L_0802a528:
	.4byte Data_0802ece0
