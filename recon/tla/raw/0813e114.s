.syntax unified
	.thumb
	.global Func_0813e114
	.thumb_func
Func_0813e114:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #36
	str r0, [sp, #12]
	movs r5, #192
	lsls r5, r5, #18
	ldr r2, [r5, #92]
	ldr r3, [r5, #96]
	movs r0, #1
	str r3, [sp, #8]
	mov r9, r2
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0813e174
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_0813e178
	adds r2, #50
	strh r3, [r2]
	movs r1, #35
	movs r0, #104
	bl Func_081963ec
	ldr r3, [r5, #104]
	movs r1, #39
	movs r0, #188
	str r3, [sp, #16]
	bl Func_081963ec
	adds r5, #188
	ldr r3, [r5]
	movs r4, #16
	movs r1, #224
	add r4, sp
	lsls r1, r1, #3
	str r3, [r4, #4]
	ldr r0, .L_0813e17c
	add r1, r9
	movs r2, #1
	movs r3, #1
	b .L_0813e180
.L_0813e174:
	.4byte 0x00000100
.L_0813e178:
	.4byte 0x00001000
.L_0813e17c:
	.4byte 0x00000139
.L_0813e180:
	mov r11, r4
	bl Resource_LoadAndDecompress
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	movs r3, #1
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	movs r3, #0
	add r2, r9
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0813e21c
	bl Scheduler_AddOrUpdateCallback
	ldr r2, [sp, #12]
	add r5, sp, #24
	adds r1, r5, #0
	movs r6, #36
	ldrsh r0, [r2, r6]
	bl Func_0815e20c
	ldr r2, [r5]
	movs r1, #128
	movs r3, #64
	lsls r1, r1, #19
	subs r3, r3, r2
	lsls r3, r3, #8
	adds r1, #40
	str r3, [r1]
	movs r3, #0
	mov r8, r3
	mov r5, r9
.L_0813e1ca:
	bl Random16
	movs r1, #96
	bl Math_ModU
	mov r2, r8
	adds r0, #16
	str r0, [r5]
	cmp r2, #0
	bge .L_0813e1e0
	adds r2, #3
.L_0813e1e0:
	asrs r2, r2, #2
	movs r3, #24
	subs r3, r3, r2
	lsls r3, r3, #16
	str r3, [r5, #4]
	cmp r0, #43
	bgt .L_0813e1f2
	movs r3, #3
	b .L_0813e224
.L_0813e1f2:
	cmp r0, #51
	bgt .L_0813e1fa
	movs r3, #2
	b .L_0813e224
.L_0813e1fa:
	cmp r0, #59
	bgt .L_0813e202
	movs r3, #1
	b .L_0813e224
.L_0813e202:
	cmp r0, #67
	bgt .L_0813e20a
	movs r3, #0
	b .L_0813e224
.L_0813e20a:
	cmp r0, #75
	bgt .L_0813e212
	movs r3, #1
	b .L_0813e222
.L_0813e212:
	cmp r0, #83
	bgt .L_0813e220
	movs r3, #2
	b .L_0813e222
	.2byte 0x0000
.L_0813e21c:
	.4byte Func_08143000
.L_0813e220:
	movs r3, #3
.L_0813e222:
	negs r3, r3
.L_0813e224:
	str r3, [r5, #12]
	ldr r3, [r5, #12]
	movs r4, #1
	lsls r3, r3, #17
	str r3, [r5, #12]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r5, #16]
	ldr r3, [r5]
	add r8, r4
	lsls r3, r3, #16
	mov r6, r8
	str r3, [r5]
	adds r5, #28
	cmp r6, #64
	bne .L_0813e1ca
	movs r0, #212
	bl Audio_PlayCue
	movs r7, #0
.L_0813e24c:
	cmp r7, #16
	bgt .L_0813e268
	ldr r2, .L_0813e280
	adds r3, r7, #0
	orrs r3, r2
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	cmp r7, #16
	bne .L_0813e268
	ldr r3, .L_0813e284
	subs r2, #2
	strh r3, [r2]
.L_0813e268:
	cmp r7, #103
	ble .L_0813e29a
	ldr r3, .L_0813e288
	ldr r2, .L_0813e280
	movs r4, #128
	subs r3, r3, r7
	lsls r4, r4, #19
	orrs r3, r2
	adds r4, #82
	strh r3, [r4]
	b .L_0813e28c
	.2byte 0x0000
.L_0813e280:
	.4byte 0x00001000
.L_0813e284:
	.4byte 0x00000000
.L_0813e288:
	.4byte 0x00000078
.L_0813e28c:
	cmp r7, #104
	bne .L_0813e29a
	movs r2, #128
	ldr r3, .L_0813e2b4
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
.L_0813e29a:
	movs r5, #210
	movs r6, #15
	movs r2, #32
	lsls r5, r5, #1
	mov r8, r6
	mov r10, r2
	add r5, r9
.L_0813e2a8:
	ldr r0, [r5, #12]
	adds r1, r0, #0
	cmp r0, #0
	bge .L_0813e2b8
	negs r1, r0
	b .L_0813e2b8
.L_0813e2b4:
	.4byte 0x00003f44
.L_0813e2b8:
	mov r3, r8
	lsls r6, r3, #2
	adds r3, r6, #0
	adds r3, #25
	asrs r1, r1, #17
	cmp r7, r3
	bge .L_0813e30e
	lsls r1, r1, #10
	movs r4, #224
	lsls r4, r4, #3
	add r1, r9
	movs r3, #2
	ldrsh r2, [r5, r3]
	adds r1, r1, r4
	movs r4, #6
	ldrsh r3, [r5, r4]
	lsrs r0, r0, #31
	subs r3, #16
	mov r12, r3
	mov r3, r10
	str r3, [sp, #0]
	str r3, [sp, #4]
	lsls r0, r0, #2
	mov r3, r11
	ldr r4, [r0, r3]
	subs r2, #16
	mov r3, r12
	ldr r0, [sp, #8]
	mov lr, r4
	.2byte 0xf800
	adds r3, r6, #0
	adds r3, #16
	cmp r7, r3
	blt .L_0813e338
	ldr r3, [r5]
	ldr r2, [r5, #12]
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #16]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
	b .L_0813e338
.L_0813e30e:
	lsls r1, r1, #10
	movs r4, #224
	movs r6, #2
	ldrsh r2, [r5, r6]
	lsls r4, r4, #3
	mov r6, r10
	add r1, r9
	adds r1, r1, r4
	movs r4, #6
	ldrsh r3, [r5, r4]
	str r6, [sp, #0]
	str r6, [sp, #4]
	lsrs r0, r0, #31
	lsls r0, r0, #2
	mov r6, r11
	ldr r4, [r0, r6]
	subs r2, #16
	subs r3, #16
	ldr r0, [sp, #8]
	mov lr, r4
	.2byte 0xf800
.L_0813e338:
	movs r2, #1
	negs r2, r2
	add r8, r2
	subs r5, #28
	cmp r8, r2
	bne .L_0813e2a8
	adds r3, r7, #0
	subs r3, #23
	cmp r3, #64
	bhi .L_0813e382
	movs r3, #3
	ands r3, r7
	cmp r3, #0
	bne .L_0813e382
	ldr r4, [sp, #12]
	movs r2, #5
	movs r3, #36
	ldrsh r0, [r4, r3]
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r3, #0
	bl Func_0814cd48
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	movs r3, #1
	add r2, r9
	str r3, [r2]
	movs r3, #7
	ands r3, r7
	cmp r3, #0
	bne .L_0813e382
	movs r0, #133
	bl Audio_PlayCue
.L_0813e382:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r9
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #120
	beq .L_0813e3a8
	b .L_0813e24c
.L_0813e3a8:
	ldr r0, .L_0813e3cc
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0813e3cc:
	.4byte Func_08143000
