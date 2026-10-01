.syntax unified
	.thumb
	.global Func_0816bc70
	.thumb_func
Func_0816bc70:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #76
	str r0, [sp, #40]
	movs r6, #192
	lsls r6, r6, #18
	ldr r0, [r6, #92]
	str r0, [sp, #36]
	movs r0, #0
	ldr r1, [r6, #96]
	str r1, [sp, #32]
	ldr r2, [r6, #100]
	mov r8, r2
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0816bcd8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r4, [sp, #40]
	add r5, sp, #64
	movs r3, #36
	ldrsh r0, [r4, r3]
	adds r1, r5, #0
	bl Func_0815e21c
	ldr r3, [r5]
	movs r1, #19
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	movs r0, #104
	str r3, [sp, #20]
	bl Func_081963ec
	ldr r2, [sp, #36]
	ldr r6, [r6, #104]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0816bcdc
	movs r2, #1
	movs r3, #0
	str r6, [sp, #24]
	b .L_0816bce0
	.2byte 0x0000
.L_0816bcd8:
	.4byte 0x00001010
.L_0816bcdc:
	.4byte 0x00000193
.L_0816bce0:
	bl Resource_LoadAndDecompress
	ldr r4, .L_0816be28
	movs r2, #156
	lsls r2, r2, #7
	adds r2, #32
	adds r1, r4, r2
	ldr r0, .L_0816be2c
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r0, .L_0816be30
	ldr r1, .L_0816be28
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r3, .L_0816be28
	movs r4, #128
	lsls r4, r4, #5
	adds r1, r3, r4
	ldr r0, .L_0816be34
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_0816be38
	mov r1, r8
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r2, [sp, #36]
	movs r3, #222
	lsls r3, r3, #6
	adds r1, r2, r3
	ldr r0, .L_0816be3c
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r4, [sp, #36]
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	lsls r1, r1, #7
	adds r2, r4, r0
	movs r3, #2
	adds r1, #132
	str r3, [r2]
	adds r2, r4, r1
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	ldr r0, .L_0816be40
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	movs r2, #128
	negs r2, r2
	str r2, [sp, #16]
	movs r3, #0
	mov r9, r3
.L_0816bd60:
	mov r4, r9
	cmp r4, #0
	bne .L_0816bd86
	movs r0, #104
	negs r0, r0
	ldr r3, [sp, #36]
	str r0, [sp, #16]
	movs r1, #0
	movs r2, #1
	mov r8, r1
	negs r2, r2
	adds r3, #24
.L_0816bd78:
	movs r4, #1
	add r8, r4
	mov r0, r8
	str r2, [r3]
	adds r3, #28
	cmp r0, #64
	bne .L_0816bd78
.L_0816bd86:
	mov r1, r9
	cmp r1, #63
	bgt .L_0816bd98
	ldr r3, [sp, #40]
	add r1, sp, #52
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl Func_0815e21c
.L_0816bd98:
	mov r4, r9
	cmp r4, #20
	bne .L_0816bdae
	ldr r1, [sp, #40]
	movs r3, #60
	ldr r0, [r1, #8]
	movs r2, #36
	ldrsh r1, [r1, r2]
	movs r2, #8
	bl Func_08157530
.L_0816bdae:
	mov r3, r9
	cmp r3, #2
	bne .L_0816bdba
	movs r0, #212
	bl Audio_PlayCue
.L_0816bdba:
	mov r4, r9
	cmp r4, #127
	ble .L_0816bdc2
	b .L_0816bf12
.L_0816bdc2:
	movs r0, #104
	str r0, [sp, #12]
	cmp r4, #8
	bgt .L_0816bdd2
	ldr r1, [sp, #16]
	adds r1, #16
	str r1, [sp, #16]
	b .L_0816bee4
.L_0816bdd2:
	mov r6, r9
	subs r6, #24
	cmp r6, #64
	bls .L_0816bddc
	b .L_0816bee4
.L_0816bddc:
	adds r3, r6, #0
	cmp r6, #0
	bge .L_0816bde6
	mov r3, r9
	subs r3, #17
.L_0816bde6:
	asrs r3, r3, #3
	lsls r3, r3, #3
	subs r5, r6, r3
	cmp r5, #4
	bne .L_0816bdf6
	movs r0, #212
	bl Audio_PlayCue
.L_0816bdf6:
	cmp r5, #7
	bne .L_0816bee4
	ldr r3, [sp, #36]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #168
	adds r2, r3, r4
	mov r0, r9
	movs r3, #4
	str r3, [r2]
	cmp r0, #79
	bgt .L_0816be44
	movs r0, #144
	bl Audio_PlayCue
	ldr r1, [sp, #40]
	movs r3, #70
	ldr r0, [r1, #8]
	movs r2, #36
	ldrsh r1, [r1, r2]
	movs r2, #16
	bl Func_08157530
	b .L_0816be64
	.2byte 0x0000
.L_0816be28:
	.4byte gMapCellBuffer
.L_0816be2c:
	.4byte 0x000000ea
.L_0816be30:
	.4byte 0x000000c1
.L_0816be34:
	.4byte 0x000000da
.L_0816be38:
	.4byte 0x00000134
.L_0816be3c:
	.4byte 0x000000c2
.L_0816be40:
	.4byte Func_08143000
.L_0816be44:
	ldr r4, [sp, #40]
	movs r1, #4
	movs r3, #36
	ldrsh r0, [r4, r3]
	bl Func_08118088
	movs r0, #144
	bl Func_081180e8
	movs r1, #128
	ldr r3, .L_0816c1dc
	ldr r0, [sp, #32]
	lsls r1, r1, #7
	ldr r2, .L_0816c1e0
	mov lr, r3
	.2byte 0xf800
.L_0816be64:
	ldr r2, [sp, #40]
	movs r3, #6
	movs r1, #36
	ldrsh r0, [r2, r1]
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	ldr r3, [sp, #16]
	ldr r1, [sp, #20]
	adds r3, #8
	str r3, [sp, #16]
	movs r4, #0
	subs r0, r6, #7
	movs r2, #127
	mov r8, r4
	mov r11, r0
	lsls r7, r1, #16
	mov r10, r2
.L_0816be8e:
	mov r3, r11
	cmp r3, #0
	bge .L_0816be96
	adds r3, r6, #0
.L_0816be96:
	asrs r3, r3, #3
	lsls r3, r3, #3
	add r3, r8
	movs r2, #63
	adds r3, #32
	ands r3, r2
	lsls r5, r3, #3
	subs r5, r5, r3
	ldr r3, [sp, #36]
	lsls r5, r5, #2
	adds r5, r3, r5
	movs r3, #192
	lsls r3, r3, #15
	str r3, [r5, #4]
	str r7, [r5]
	bl Random16
	mov r4, r10
	ands r0, r4
	subs r0, #64
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	mov r1, r10
	movs r3, #32
	ands r0, r1
	adds r0, #64
	str r3, [r5, #8]
	movs r3, #1
	negs r0, r0
	add r8, r3
	lsls r0, r0, #11
	movs r2, #0
	mov r4, r8
	str r0, [r5, #16]
	str r2, [r5, #24]
	cmp r4, #8
	bne .L_0816be8e
.L_0816bee4:
	ldr r3, [sp, #16]
	adds r3, #104
	cmp r3, #108
	ble .L_0816bef6
	ldr r0, [sp, #12]
	ldr r1, [sp, #16]
	subs r3, r0, r1
	adds r3, #4
	str r3, [sp, #12]
.L_0816bef6:
	ldr r2, [sp, #12]
	cmp r2, #0
	ble .L_0816bf12
	str r2, [sp, #4]
	ldr r2, [sp, #20]
	movs r3, #48
	str r3, [sp, #0]
	ldr r0, [sp, #32]
	ldr r1, .L_0816c1e4
	subs r2, #24
	ldr r3, [sp, #16]
	ldr r4, [sp, #24]
	mov lr, r4
	.2byte 0xf800
.L_0816bf12:
	mov r0, r9
	cmp r0, #7
	bne .L_0816bf6e
	ldr r1, [sp, #36]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	adds r2, r1, r3
	movs r3, #8
	movs r0, #145
	str r3, [r2]
	bl Audio_PlayCue
	ldr r0, [sp, #20]
	ldr r5, [sp, #36]
	movs r4, #0
	mov r8, r4
	lsls r7, r0, #16
	movs r6, #127
.L_0816bf38:
	movs r3, #192
	lsls r3, r3, #15
	str r3, [r5, #4]
	str r7, [r5]
	bl Random16
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	adds r0, #64
	movs r2, #1
	movs r3, #32
	negs r0, r0
	add r8, r2
	str r3, [r5, #8]
	lsls r0, r0, #12
	movs r1, #0
	mov r3, r8
	str r0, [r5, #16]
	str r1, [r5, #24]
	adds r5, #28
	cmp r3, #32
	bne .L_0816bf38
.L_0816bf6e:
	mov r4, r9
	cmp r4, #6
	ble .L_0816c01c
	ldr r7, [sp, #36]
	movs r0, #0
	mov r8, r0
.L_0816bf7a:
	movs r1, #192
	ldr r6, [r7, #24]
	lsls r1, r1, #2
	adds r1, #255
	cmp r6, r1
	bhi .L_0816c010
	movs r1, #5
	mov r0, r8
	bl __modsi3
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r1, #96
	adds r0, r6, #0
	bl Math_Div
	movs r1, #3
	bl __modsi3
	ldr r2, .L_0816c1e8
	adds r5, r5, r0
	lsls r3, r5, #2
	ldr r1, [r2, r3]
	ldr r2, [sp, #36]
	movs r3, #240
	adds r1, r2, r1
	lsls r3, r3, #4
	adds r1, r1, r3
	ldr r3, .L_0816c1ec
	movs r4, #2
	ldrsh r2, [r7, r4]
	ldrb r6, [r3, r5]
	lsrs r3, r6, #1
	subs r2, r2, r3
	movs r0, #6
	ldrsh r3, [r7, r0]
	ldr r0, .L_0816c1f0
	ldrb r4, [r0, r5]
	str r6, [sp, #0]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #32]
	ldr r4, [sp, #24]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	adds r0, r7, #0
	movs r1, #64
	lsls r2, r2, #7
	bl BattleFxKernels_IntegrateVector2
	movs r0, #6
	ldrsh r3, [r7, r0]
	cmp r3, #104
	ble .L_0816bff6
	ldr r3, [r7, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7, #16]
.L_0816bff6:
	ldr r3, [r7, #24]
	ldr r2, [r7, #8]
	adds r3, r3, r2
	str r3, [r7, #24]
	cmp r2, #1
	ble .L_0816c010
	movs r3, #1
	mov r1, r9
	ands r3, r1
	cmp r3, #0
	beq .L_0816c010
	subs r3, r2, #1
	str r3, [r7, #8]
.L_0816c010:
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r7, #28
	cmp r3, #64
	bne .L_0816bf7a
.L_0816c01c:
	mov r4, r9
	cmp r4, #5
	bgt .L_0816c024
	b .L_0816c224
.L_0816c024:
	movs r0, #128
	lsls r0, r0, #3
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #8]
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_0816c1f4
	ldr r3, [sp, #44]
	adds r6, r0, #0
	ands r3, r2
	ldr r2, .L_0816c1f8
	movs r0, #7
	orrs r3, r0
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #44]
	movs r3, #0
	str r3, [r6, #4]
	ldr r2, .L_0816c1fc
	ldr r3, .L_0816c200
	add r1, sp, #44
	str r2, [r1, #4]
	str r0, [r6]
	str r1, [r6, #16]
	str r3, [r6, #8]
	ldr r4, [sp, #8]
	movs r0, #0
	str r4, [r6, #12]
	strb r0, [r6, #24]
	strb r0, [r6, #25]
	ldr r2, [sp, #20]
	mov r10, r1
	subs r2, #64
	movs r1, #0
	mov r8, r1
	mov r7, r9
	mov r11, r2
.L_0816c076:
	ldr r3, .L_0816c204
	mov r4, r8
	ldrb r3, [r3, r4]
	adds r1, r3, #6
	cmp r9, r1
	ble .L_0816c0f2
	ldr r3, .L_0816c208
	mov r0, r9
	ldrb r3, [r3, r4]
	subs r2, r0, r1
	muls r2, r3
	movs r3, #175
	lsls r3, r3, #3
	muls r3, r2
	movs r2, #131
	lsls r2, r2, #7
	adds r5, r3, r2
	subs r3, r1, r0
	lsls r3, r3, #3
	adds r3, #56
	cmp r3, #0
	ble .L_0816c0a4
	movs r3, #0
.L_0816c0a4:
	movs r4, #64
	negs r4, r4
	cmp r3, r4
	ble .L_0816c0f2
	str r3, [r6, #20]
	bl Func_08014de4
	ldr r3, .L_0816c20c
	mov r0, r8
	ldrsb r1, [r3, r0]
	mov r2, r11
	lsls r0, r2, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	adds r1, r5, #0
	adds r2, r5, #0
	adds r0, r5, #0
	bl Func_080151e4
	movs r4, #7
	adds r3, r7, #0
	ands r3, r4
	movs r0, #176
	lsls r3, r3, #4
	lsls r0, r0, #4
	strb r3, [r6, #24]
	adds r0, #184
	bl SceneTransform_ApplyPitch
	ldr r0, .L_0816c210
	ldr r1, [sp, #8]
	movs r2, #32
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_0816c0f2:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r7, #5
	cmp r1, #4
	bne .L_0816c076
	movs r2, #7
	mov r3, r10
	strb r2, [r3]
	add r3, sp, #44
	strb r2, [r3, #1]
	ldr r4, [sp, #36]
	movs r0, #222
	lsls r0, r0, #6
	adds r2, r4, r0
	str r2, [r3, #4]
	ldr r3, .L_0816c214
	movs r1, #0
	str r3, [r6, #8]
	strb r1, [r6, #24]
	movs r3, #52
	strb r1, [r6, #25]
	movs r2, #0
	add r3, sp
	mov r7, r9
	mov r8, r2
	mov r10, r3
	subs r7, #31
.L_0816c12a:
	cmp r7, #0
	blt .L_0816c1c2
	mov r4, r8
	movs r0, #128
	lsls r3, r4, #11
	lsls r0, r0, #6
	adds r3, r3, r0
	muls r3, r7
	movs r1, #128
	lsls r1, r1, #8
	adds r1, r1, r3
	mov r11, r1
	movs r3, #0
	cmp r7, #3
	ble .L_0816c14e
	movs r3, #4
	subs r3, r3, r7
	lsls r3, r3, #4
.L_0816c14e:
	movs r2, #64
	negs r2, r2
	str r3, [r6, #20]
	cmp r3, r2
	ble .L_0816c1c2
	bl Func_08014de4
	mov r3, r10
	ldr r0, [r3]
	mov r4, r10
	lsrs r3, r0, #31
	ldr r1, [r4, #4]
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	subs r1, #80
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Func_080151e4
	ldr r1, [sp, #40]
	movs r2, #128
	ldr r0, [r1, #4]
	lsls r2, r2, #8
	lsls r0, r0, #15
	adds r0, r0, r2
	bl Func_08015068
	ldr r3, .L_0816c218
	mov r4, r8
	lsls r5, r4, #1
	ldrsh r0, [r3, r5]
	bl Func_080150e4
	ldr r3, .L_0816c21c
	ldrsh r0, [r3, r5]
	bl SceneTransform_ApplyPitch
	mov r0, r11
	bl Func_0801521c
	ldr r0, .L_0816c220
	ldr r1, [sp, #8]
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_0816c1c2:
	movs r3, #1
	add r8, r3
	mov r4, r8
	subs r7, #8
	cmp r4, #8
	bne .L_0816c12a
	adds r0, r6, #0
	bl Sys_Free
	ldr r0, [sp, #8]
	bl Sys_Free
	b .L_0816c224
.L_0816c1dc:
	.4byte IwramFillWords
.L_0816c1e0:
	.4byte 0x3f3f3f3f
.L_0816c1e4:
	.4byte Data_02014e20
.L_0816c1e8:
	.4byte Data_08197834
.L_0816c1ec:
	.4byte Data_0819781a
.L_0816c1f0:
	.4byte Data_08197826
.L_0816c1f4:
	.4byte 0xffffff00
.L_0816c1f8:
	.4byte 0xffff00ff
.L_0816c1fc:
	.4byte gMapCellBuffer
.L_0816c200:
	.4byte Data_08198ec4
.L_0816c204:
	.4byte Data_08198b26
.L_0816c208:
	.4byte Data_08198b2a
.L_0816c20c:
	.4byte Data_08198b2e
.L_0816c210:
	.4byte Data_08198cac
.L_0816c214:
	.4byte Data_08199364
.L_0816c218:
	.4byte Data_08198b32
.L_0816c21c:
	.4byte Data_08198b42
.L_0816c220:
	.4byte Data_08199210
.L_0816c224:
	movs r0, #4
	movs r1, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r1, #240
	ldr r0, [sp, #36]
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	add r9, r2
	mov r3, r9
	cmp r3, #148
	beq .L_0816c250
	b .L_0816bd60
.L_0816c250:
	ldr r0, .L_0816c270
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #76
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0816c270:
	.4byte Func_08143000
