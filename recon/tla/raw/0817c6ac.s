.syntax unified
	.thumb
	.global Func_0817c6ac
	.thumb_func
Func_0817c6ac:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #116
	str r0, [sp, #80]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #92]
	str r0, [sp, #76]
	movs r0, #1
	ldr r1, [r5, #96]
	str r1, [sp, #72]
	bl BattleFx_BeginCanvasLayer
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	ldr r3, .L_0817c718
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, [sp, #76]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
	ldr r0, [sp, #76]
	movs r1, #238
	lsls r1, r1, #7
	ldr r5, [r5, #104]
	adds r1, #132
	adds r2, r0, r1
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0817c71c
	str r5, [sp, #64]
	bl Scheduler_AddOrUpdateCallback
	movs r2, #0
	str r2, [sp, #52]
	str r2, [sp, #48]
	ldr r3, [sp, #80]
	mov r4, sp
	b .L_0817c720
	.2byte 0x0000
.L_0817c718:
	.4byte 0x00001010
.L_0817c71c:
	.4byte Func_08143000
.L_0817c720:
	adds r4, #104
	adds r1, r4, #0
	ldr r0, [r3, #8]
	str r4, [sp, #36]
	bl Func_0815e21c
	ldr r2, [sp, #80]
	mov r3, sp
	adds r3, #92
	movs r1, #36
	ldrsh r0, [r2, r1]
	adds r1, r3, #0
	str r3, [sp, #32]
	bl Func_0815e21c
	ldr r4, [sp, #76]
	movs r2, #224
	lsls r2, r2, #3
	adds r1, r4, r2
	ldr r0, .L_0817c7a8
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r3, [sp, #76]
	movs r4, #240
	lsls r4, r4, #4
	adds r1, r3, r4
	ldr r0, .L_0817c7ac
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r2, [sp, #76]
	movs r3, #142
	lsls r3, r3, #7
	adds r1, r2, r3
	ldr r0, .L_0817c7b0
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r3, .L_0817c7a4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r0, .L_0817c7b4
	movs r1, #4
	movs r2, #32
	movs r3, #32
	bl Func_08178680
	movs r4, #0
	str r4, [sp, #56]
.L_0817c78e:
	ldr r0, [sp, #56]
	cmp r0, #0
	bne .L_0817c7c6
	movs r1, #0
	ldr r3, [sp, #76]
	str r1, [sp, #40]
	str r1, [sp, #44]
	mov r10, r1
	movs r2, #24
	adds r3, #24
	b .L_0817c7b8
.L_0817c7a4:
	.4byte 0x00000080
.L_0817c7a8:
	.4byte 0x000000f4
.L_0817c7ac:
	.4byte 0x0000013e
.L_0817c7b0:
	.4byte 0x000000e8
.L_0817c7b4:
	.4byte Data_02012000
.L_0817c7b8:
	movs r4, #1
	add r10, r4
	mov r0, r10
	str r2, [r3]
	adds r3, #28
	cmp r0, #64
	bne .L_0817c7b8
.L_0817c7c6:
	ldr r1, [sp, #56]
	cmp r1, #22
	bne .L_0817c894
	movs r1, #128
	ldr r0, [sp, #72]
	lsls r1, r1, #7
	ldr r3, .L_0817cb60
	ldr r2, .L_0817cb64
	mov lr, r3
	.2byte 0xf800
	movs r0, #144
	bl Func_081180e8
	movs r4, #238
	ldr r3, [sp, #76]
	lsls r4, r4, #7
	adds r4, #168
	adds r2, r3, r4
	movs r3, #8
	str r3, [r2]
	ldr r2, [sp, #80]
	movs r3, #120
	movs r1, #36
	ldrsh r0, [r2, r1]
	str r3, [sp, #4]
	movs r2, #128
	movs r3, #128
	lsls r2, r2, #10
	lsls r3, r3, #11
	movs r1, #1
	str r2, [sp, #0]
	bl Func_0815f000
	ldr r4, [sp, #80]
	movs r1, #7
	movs r3, #36
	ldrsh r0, [r4, r3]
	movs r3, #20
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	ldr r1, .L_0817cb68
	ldr r7, [sp, #32]
	ldr r6, [sp, #76]
	movs r0, #0
	mov r10, r0
	mov r8, r1
.L_0817c828:
	mov r2, r10
	negs r3, r2
	cmp r3, #0
	bge .L_0817c832
	adds r3, #3
.L_0817c832:
	asrs r3, r3, #2
	adds r3, #2
	str r3, [r6, #24]
	ldr r3, [r7]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r6]
	bl Random16
	ldr r3, [r7, #4]
	movs r2, #15
	ands r2, r0
	adds r3, r3, r2
	subs r3, #32
	lsls r3, r3, #16
	str r3, [r6, #4]
	bl Random16
	ldr r3, [sp, #80]
	mov r4, r8
	ldr r2, [r3, #24]
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, #6
	ldrh r5, [r4, r3]
	adds r1, r5, #0
	bl __umodsi3
	lsrs r5, r5, #1
	subs r0, r0, r5
	lsls r0, r0, #12
	str r0, [r6, #12]
	bl Random16
	movs r2, #255
	ands r2, r0
	movs r3, #192
	movs r0, #1
	subs r3, r3, r2
	add r10, r0
	lsls r3, r3, #10
	mov r1, r10
	str r3, [r6, #16]
	adds r6, #28
	cmp r1, #32
	bne .L_0817c828
.L_0817c894:
	ldr r3, [sp, #80]
	movs r2, #0
	mov r10, r2
	ldr r2, [r3, #24]
	ldr r0, .L_0817cb68
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, #8
	ldrh r3, [r0, r3]
	cmp r3, #0
	beq .L_0817c926
	ldr r5, [sp, #76]
	mov r8, r0
.L_0817c8b0:
	ldr r3, [r5, #24]
	cmp r3, #23
	bhi .L_0817c908
	cmp r3, #1
	ble .L_0817c8f2
	adds r1, r3, #0
	cmp r3, #0
	bge .L_0817c8c2
	adds r1, r3, #3
.L_0817c8c2:
	ldr r4, [sp, #76]
	asrs r1, r1, #2
	lsls r1, r1, #11
	movs r0, #240
	movs r3, #2
	ldrsh r2, [r5, r3]
	adds r1, r4, r1
	lsls r0, r0, #4
	movs r4, #6
	ldrsh r3, [r5, r4]
	adds r1, r1, r0
	movs r0, #32
	str r0, [sp, #0]
	movs r0, #64
	subs r2, #16
	str r0, [sp, #4]
	subs r3, #32
	ldr r0, [sp, #72]
	ldr r4, [sp, #64]
	mov lr, r4
	.2byte 0xf800
	ldr r1, [sp, #80]
	ldr r0, .L_0817cb68
	ldr r2, [r1, #24]
.L_0817c8f2:
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, #10
	ldrh r2, [r0, r3]
	adds r0, r5, #0
	negs r2, r2
	movs r1, #60
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
.L_0817c908:
	adds r3, #1
	str r3, [r5, #24]
	ldr r3, [sp, #80]
	movs r2, #1
	add r10, r2
	ldr r2, [r3, #24]
	mov r0, r8
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, #8
	ldrh r3, [r0, r3]
	adds r5, #28
	cmp r10, r3
	bne .L_0817c8b0
.L_0817c926:
	ldr r4, [sp, #56]
	cmp r4, #0
	bge .L_0817c92e
	b .L_0817cbd4
.L_0817c92e:
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	ldrh r1, [r0, r3]
	str r1, [sp, #28]
	movs r1, #44
	adds r2, r4, #0
	muls r2, r1
	str r2, [sp, #40]
	ldr r4, [sp, #40]
	movs r2, #247
	lsls r2, r2, #2
	cmp r4, r2
	ble .L_0817c94c
	str r2, [sp, #40]
.L_0817c94c:
	adds r3, #2
	ldrh r3, [r0, r3]
	ldr r0, [sp, #56]
	cmp r0, r3
	blt .L_0817c964
	subs r3, r0, r3
	adds r4, r3, #0
	muls r4, r1
	str r4, [sp, #44]
	cmp r4, r2
	ble .L_0817c964
	str r2, [sp, #44]
.L_0817c964:
	ldr r1, [sp, #40]
	ldr r2, [sp, #44]
	movs r0, #0
	subs r1, r1, r2
	str r0, [sp, #60]
	str r1, [sp, #24]
	str r0, [sp, #20]
	str r0, [sp, #16]
.L_0817c974:
	ldr r4, [sp, #36]
	ldr r3, [r4]
	cmp r3, #0
	bge .L_0817c97e
	adds r3, #7
.L_0817c97e:
	ldr r0, [sp, #32]
	asrs r2, r3, #3
	ldr r3, [r0]
	cmp r3, #0
	bge .L_0817c98a
	adds r3, #7
.L_0817c98a:
	ldr r1, [sp, #44]
	asrs r3, r3, #3
	subs r4, r2, r3
	ldr r2, [sp, #20]
	lsls r3, r1, #5
	adds r3, r3, r1
	adds r2, r2, r3
	ldr r1, [sp, #60]
	ldr r3, [sp, #56]
	mov r8, r2
	adds r0, r3, r1
	lsls r0, r0, #12
	str r4, [sp, #8]
	bl Trig_Sin
	ldr r2, [sp, #60]
	lsls r0, r0, #2
	asrs r0, r0, #16
	adds r3, r2, #0
	muls r3, r0
	ldr r4, [sp, #8]
	cmp r3, #0
	bge .L_0817c9ba
	adds r3, #31
.L_0817c9ba:
	asrs r3, r3, #5
	mov r9, r3
	cmp r4, #0
	bge .L_0817c9c4
	negs r4, r4
.L_0817c9c4:
	ldr r3, [sp, #80]
	ldr r1, .L_0817cb68
	ldr r2, [r3, #24]
	ldr r0, [sp, #60]
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, #4
	ldrh r1, [r1, r3]
	lsls r6, r0, #2
	mov r11, r1
	cmp r6, #24
	ble .L_0817c9e0
	movs r6, #24
.L_0817c9e0:
	ldr r1, [sp, #28]
	adds r3, r1, #0
	muls r3, r6
	cmp r3, #0
	bge .L_0817c9f2
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
.L_0817c9f2:
	asrs r6, r3, #16
	lsrs r3, r3, #31
	adds r3, r6, r3
	asrs r3, r3, #1
	movs r0, #0
	negs r3, r3
	mov r1, r11
	adds r2, r1, r3
	mov r10, r0
	adds r3, r4, r3
	mov r0, r9
	ldr r1, [sp, #16]
	adds r7, r3, r0
	ldr r0, .L_0817cb6c
	lsls r3, r1, #2
	add r2, r9
	adds r5, r3, r0
.L_0817ca14:
	mov r0, r8
	str r2, [sp, #12]
	str r4, [sp, #8]
	bl Trig_Cos
	adds r3, r7, #0
	muls r3, r0
	ldr r4, [sp, #8]
	asrs r3, r3, #16
	subs r3, r3, r4
	strb r3, [r5]
	mov r0, r8
	bl Trig_Sin
	ldr r2, [sp, #12]
	movs r1, #0
	adds r3, r2, #0
	muls r3, r0
	asrs r3, r3, #16
	negs r3, r3
	strb r3, [r5, #1]
	movs r3, #1
	add r10, r3
	mov r0, r10
	strb r1, [r5, #2]
	adds r2, r2, r6
	adds r7, r7, r6
	adds r5, #4
	ldr r4, [sp, #8]
	cmp r0, #2
	bne .L_0817ca14
	ldr r1, [sp, #60]
	cmp r1, #32
	bne .L_0817ca7e
	mov r0, r8
	bl Trig_Cos
	ldr r4, [sp, #8]
	mov r2, r9
	adds r3, r4, r2
	muls r3, r0
	asrs r3, r3, #16
	subs r3, r3, r4
	mov r0, r8
	str r3, [sp, #52]
	bl Trig_Sin
	mov r3, r11
	add r3, r9
	muls r3, r0
	asrs r3, r3, #16
	negs r3, r3
	str r3, [sp, #48]
.L_0817ca7e:
	ldr r3, [sp, #20]
	ldr r0, [sp, #16]
	ldr r1, [sp, #60]
	ldr r4, [sp, #24]
	adds r0, #2
	adds r3, r3, r4
	adds r1, #1
	str r3, [sp, #20]
	str r0, [sp, #16]
	str r1, [sp, #60]
	cmp r1, #33
	beq .L_0817ca98
	b .L_0817c974
.L_0817ca98:
	ldr r2, [sp, #56]
	movs r5, #0
	cmp r2, #33
	ble .L_0817caa6
	movs r3, #34
	subs r3, r3, r2
	lsls r5, r3, #3
.L_0817caa6:
	movs r3, #64
	negs r3, r3
	cmp r5, r3
	bgt .L_0817cab0
	b .L_0817cbd4
.L_0817cab0:
	movs r0, #1
	bl Func_081969f8
	movs r4, #6
	adds r6, r0, #0
	mov r8, r4
	str r4, [r6]
	movs r0, #0
	ldr r2, .L_0817cb70
	ldr r3, [sp, #40]
	ldr r4, [sp, #44]
	mov r10, r0
	add r7, sp, #84
	mov r1, r10
	str r7, [r6, #16]
	str r5, [r6, #20]
	strb r1, [r6, #25]
	str r2, [r6, #12]
	cmp r3, r4
	ble .L_0817cbce
	mov r0, r8
	movs r3, #5
	strb r0, [r7]
	strb r3, [r7, #1]
	ldr r1, [sp, #76]
	movs r2, #224
	lsls r2, r2, #3
	adds r3, r1, r2
	str r3, [r7, #4]
	ldr r3, .L_0817cb74
	movs r5, #128
	str r3, [r6, #8]
	ldr r4, [sp, #56]
	movs r3, #127
	lsls r2, r4, #3
	bics r3, r2
	strb r3, [r6, #24]
	bl Func_08014de4
	ldr r1, [sp, #36]
	ldr r2, .L_0817cb78
	ldr r0, [r1]
	ldr r1, [r1, #4]
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	lsls r1, r1, #16
	adds r1, r1, r2
	lsls r0, r0, #16
	movs r2, #0
	bl Func_08015160
	lsls r5, r5, #8
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_080151e4
	ldr r4, [sp, #80]
	ldr r3, [r4, #4]
	cmp r3, #1
	bne .L_0817cb36
	adds r0, r5, #0
	bl Func_08015068
.L_0817cb36:
	movs r0, #128
	lsls r0, r0, #11
	bl Func_0801521c
	ldr r0, .L_0817cb6c
	ldr r1, .L_0817cb70
	movs r2, #66
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
	ldr r0, [sp, #56]
	cmp r0, #22
	bgt .L_0817cbce
	ldr r3, .L_0817cb7c
	mov r1, r8
	mov r2, r10
	str r3, [r6, #8]
	strb r1, [r7]
	b .L_0817cb80
.L_0817cb60:
	.4byte IwramFillWords
.L_0817cb64:
	.4byte 0x3f3f3f3f
.L_0817cb68:
	.4byte Data_08199484
.L_0817cb6c:
	.4byte gMapCellBuffer
.L_0817cb70:
	.4byte Data_02011000
.L_0817cb74:
	.4byte Data_02012000
.L_0817cb78:
	.4byte 0xffc00000
.L_0817cb7c:
	.4byte Data_08199340
.L_0817cb80:
	strb r1, [r7, #1]
	strb r2, [r6, #24]
	ldr r4, [sp, #76]
	movs r0, #142
	lsls r0, r0, #7
	adds r3, r4, r0
	str r3, [r7, #4]
	ldr r1, [sp, #52]
	ldr r2, [sp, #48]
	lsls r0, r1, #16
	lsls r1, r2, #16
	movs r2, #0
	bl Func_08015160
	ldr r4, [sp, #40]
	movs r0, #128
	lsls r3, r4, #5
	lsls r0, r0, #7
	subs r0, r0, r3
	bl Func_080150e4
	ldr r0, [sp, #28]
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r0, r3, #2
	cmp r0, #0
	bge .L_0817cbb8
	adds r0, #31
.L_0817cbb8:
	asrs r0, r0, #5
	bl Func_0801521c
	ldr r0, .L_0817cc24
	ldr r1, .L_0817cc28
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_0817cbce:
	adds r0, r6, #0
	bl Sys_Free
.L_0817cbd4:
	bl Func_081434f8
	ldr r2, [sp, #80]
	ldr r1, [r2, #24]
	lsls r1, r1, #1
	adds r1, #2
	adds r0, r1, #0
	bl Func_08158ce0
	movs r4, #240
	ldr r3, [sp, #76]
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #56]
	adds r0, #1
	str r0, [sp, #56]
	cmp r0, #50
	beq .L_0817cc06
	b .L_0817c78e
.L_0817cc06:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_0817cc2c
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #116
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0817cc24:
	.4byte Data_081991e0
.L_0817cc28:
	.4byte Data_02011000
.L_0817cc2c:
	.4byte Func_08143000
