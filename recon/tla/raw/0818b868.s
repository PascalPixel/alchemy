.syntax unified
	.thumb
	.global Func_0818b868
	.thumb_func
Func_0818b868:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #84
	str r0, [sp, #60]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #92]
	str r0, [sp, #56]
	movs r0, #1
	ldr r1, [r5, #96]
	str r1, [sp, #52]
	ldr r2, [r5, #100]
	str r2, [sp, #36]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0818b8cc
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, [sp, #56]
	movs r0, #239
	lsls r0, r0, #7
	adds r2, r3, r0
	movs r3, #2
	str r3, [r2]
	ldr r1, [sp, #56]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r1, r3
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0818b8d0
	bl Scheduler_AddOrUpdateCallback
	ldr r1, [sp, #60]
	movs r3, #80
	ldr r0, [r1, #8]
	movs r2, #36
	ldrsh r1, [r1, r2]
	movs r2, #8
	b .L_0818b8d4
.L_0818b8cc:
	.4byte 0x00001010
.L_0818b8d0:
	.4byte Func_08143000
.L_0818b8d4:
	bl Func_08157530
	movs r0, #8
	bl WaitFrames
	movs r1, #23
	movs r0, #104
	bl Func_081963ec
	ldr r5, [r5, #104]
	ldr r0, .L_0818bc20
	ldr r1, .L_0818bc24
	movs r2, #0
	movs r3, #0
	str r5, [sp, #40]
	bl Func_08157cf4
	movs r2, #0
	ldr r1, [sp, #36]
	movs r3, #0
	ldr r0, .L_0818bc28
	bl Func_08157cf4
	ldr r0, .L_0818bc2c
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0818bc30
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	mov r0, sp
	mov r1, sp
	movs r3, #0
	adds r0, #72
	adds r1, #64
	str r3, [sp, #48]
	str r0, [sp, #20]
	str r1, [sp, #24]
.L_0818b926:
	ldr r2, [sp, #48]
	cmp r2, #0
	bne .L_0818b946
	movs r3, #0
	mov r9, r3
	ldr r3, .L_0818bc34
	movs r1, #1
	movs r2, #128
	negs r1, r1
	lsls r2, r2, #2
.L_0818b93a:
	movs r0, #1
	add r9, r0
	str r1, [r3]
	adds r3, #28
	cmp r9, r2
	bne .L_0818b93a
.L_0818b946:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #32]
	movs r0, #1
	bl Func_081969f8
	ldr r2, [sp, #60]
	str r0, [sp, #28]
	movs r1, #36
	ldrsh r0, [r2, r1]
	ldr r1, [sp, #20]
	bl Func_0815e21c
	ldr r2, .L_0818bc38
	ldr r3, [sp, #64]
	movs r0, #7
	ands r3, r2
	ldr r2, .L_0818bc3c
	orrs r3, r0
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	ldr r1, .L_0818bc24
	ldr r2, [sp, #24]
	str r3, [sp, #64]
	str r1, [r2, #4]
	ldr r0, [sp, #28]
	ldr r3, .L_0818bc40
	movs r1, #7
	str r3, [r0, #8]
	str r1, [r0]
	str r2, [r0, #16]
	ldr r2, [sp, #32]
	movs r3, #0
	str r2, [r0, #12]
	ldr r0, [sp, #56]
	movs r1, #120
	str r1, [sp, #16]
	str r3, [sp, #12]
	mov r9, r3
	mov r10, r0
.L_0818b99c:
	ldr r2, .L_0818bc44
	mov r0, r9
	ldrb r3, [r2, r0]
	ldr r1, [sp, #48]
	cmp r1, r3
	bne .L_0818ba8c
	ldr r3, [sp, #20]
	ldr r0, [sp, #12]
	ldr r1, .L_0818bc48
	movs r2, #0
	mov r11, r2
	mov r8, r3
	adds r7, r0, r1
.L_0818b9b6:
	mov r2, r8
	ldr r3, [r2]
	mov r0, r8
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r7]
	movs r5, #255
	ldr r3, [r0, #4]
	subs r3, #24
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Random16
	adds r6, r0, #0
	bl Random16
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #32
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #8
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #16]
	ldr r1, [sp, #60]
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_0818ba0a
	ldr r3, [r7, #12]
	ldr r2, .L_0818bc4c
	adds r3, r3, r2
	b .L_0818ba12
.L_0818ba0a:
	ldr r3, [r7, #12]
	movs r0, #128
	lsls r0, r0, #8
	adds r3, r3, r0
.L_0818ba12:
	str r3, [r7, #12]
	mov r1, r8
	ldr r3, [r1]
	mov r2, r10
	str r3, [r2]
	ldr r3, [r1, #4]
	subs r3, #24
	str r3, [r2, #4]
	bl Random16
	movs r3, #7
	ands r0, r3
	adds r0, #16
	str r0, [r7, #24]
	movs r0, #1
	add r11, r0
	mov r1, r11
	adds r7, #28
	cmp r1, #32
	bne .L_0818b9b6
	mov r2, r9
	cmp r2, #2
	bne .L_0818ba48
	movs r0, #134
	bl Func_081180e8
	b .L_0818ba4e
.L_0818ba48:
	movs r0, #134
	bl Audio_PlayCue
.L_0818ba4e:
	ldr r1, [sp, #60]
	ldr r2, [sp, #16]
	movs r3, #36
	ldrsh r0, [r1, r3]
	movs r3, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #12
	movs r1, #1
	lsls r2, r2, #10
	bl Func_0815f000
	ldr r1, [sp, #60]
	movs r5, #8
	movs r3, #36
	ldrsh r0, [r1, r3]
	movs r2, #5
	movs r3, #0
	movs r1, #7
	str r5, [sp, #0]
	bl Func_0814cd48
	movs r0, #238
	ldr r2, [sp, #56]
	lsls r0, r0, #7
	adds r0, #168
	adds r3, r2, r0
	str r5, [r3]
	ldr r2, .L_0818bc44
.L_0818ba8c:
	mov r1, r9
	ldrb r2, [r2, r1]
	ldr r3, [sp, #48]
	cmp r3, r2
	blt .L_0818bb18
	ldr r0, [sp, #48]
	adds r3, r2, #0
	adds r3, #16
	cmp r0, r3
	bge .L_0818bb18
	subs r1, r0, r2
	lsls r2, r1, #3
	movs r3, #64
	subs r2, r3, r2
	movs r3, #156
	lsls r3, r3, #6
	adds r3, #208
	muls r3, r1
	movs r1, #128
	lsls r1, r1, #7
	adds r5, r3, r1
	cmp r2, #0
	ble .L_0818babc
	movs r2, #0
.L_0818babc:
	movs r3, #64
	negs r3, r3
	cmp r2, r3
	ble .L_0818bb18
	ldr r0, [sp, #28]
	str r2, [r0, #20]
	bl Func_08014de4
	movs r2, #128
	lsls r2, r2, #8
	movs r1, #128
	adds r0, r2, #0
	lsls r1, r1, #9
	bl Func_080151e4
	mov r1, r10
	ldr r0, [r1]
	ldr r1, [r1, #4]
	subs r0, #128
	subs r1, #64
	lsls r1, r1, #16
	lsls r0, r0, #16
	movs r2, #0
	bl Func_08015160
	mov r2, r9
	movs r3, #128
	lsls r0, r2, #14
	lsls r3, r3, #6
	adds r0, r0, r3
	bl Func_080150e4
	ldr r0, .L_0818bc50
	bl SceneTransform_ApplyPitch
	adds r0, r5, #0
	bl Func_0801521c
	ldr r0, .L_0818bc54
	ldr r1, [sp, #32]
	movs r2, #4
	bl Func_08196958
	ldr r0, [sp, #28]
	bl Func_08196a7c
.L_0818bb18:
	ldr r1, [sp, #16]
	movs r0, #28
	ldr r2, [sp, #12]
	add r10, r0
	movs r3, #224
	movs r0, #1
	adds r1, #20
	lsls r3, r3, #4
	add r9, r0
	str r1, [sp, #16]
	adds r2, r2, r3
	mov r1, r9
	str r2, [sp, #12]
	cmp r1, #3
	beq .L_0818bb38
	b .L_0818b99c
.L_0818bb38:
	ldr r3, .L_0818bc58
	ldr r0, [sp, #40]
	ldr r7, .L_0818bc48
	movs r2, #0
	mov r9, r2
	mov r11, r3
	mov r10, r0
.L_0818bb46:
	ldr r6, [r7, #24]
	cmp r6, #0
	blt .L_0818bbbc
	asrs r6, r6, #2
	adds r6, #3
	lsls r1, r6, #1
	mov r8, r1
	mov r4, r8
	subs r4, #2
	mov r2, r11
	ldrh r1, [r2, r4]
	ldr r3, [sp, #36]
	movs r0, #2
	ldrsh r2, [r7, r0]
	adds r1, r3, r1
	lsrs r5, r6, #31
	movs r0, #6
	ldrsh r3, [r7, r0]
	adds r5, r6, r5
	asrs r5, r5, #1
	mov r0, r8
	str r0, [sp, #4]
	str r4, [sp, #8]
	str r6, [sp, #0]
	ldr r0, [sp, #52]
	subs r3, r3, r6
	subs r2, r2, r5
	mov lr, r10
	.2byte 0xf800
	adds r0, r7, #0
	movs r1, #62
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r4, [sp, #8]
	mov r2, r11
	ldrh r1, [r2, r4]
	ldr r3, [sp, #36]
	movs r0, #2
	ldrsh r2, [r7, r0]
	adds r1, r3, r1
	movs r0, #6
	ldrsh r3, [r7, r0]
	mov r0, r8
	subs r3, r3, r6
	str r0, [sp, #4]
	subs r2, r2, r5
	str r6, [sp, #0]
	ldr r0, [sp, #52]
	mov lr, r10
	.2byte 0xf800
	adds r0, r7, #0
	movs r1, #62
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r7, #24]
	subs r3, #1
	str r3, [r7, #24]
.L_0818bbbc:
	movs r1, #1
	movs r2, #128
	add r9, r1
	lsls r2, r2, #2
	adds r7, #28
	cmp r9, r2
	bne .L_0818bb46
	ldr r0, [sp, #28]
	bl Sys_Free
	ldr r0, [sp, #32]
	bl Sys_Free
	movs r1, #8
	movs r0, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r0, #240
	ldr r3, [sp, #56]
	lsls r0, r0, #7
	adds r0, #232
	adds r2, r3, r0
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #48]
	adds r1, #1
	str r1, [sp, #48]
	cmp r1, #50
	beq .L_0818bc02
	b .L_0818b926
.L_0818bc02:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_0818bc5c
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #84
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0818bc20:
	.4byte 0x000000c2
.L_0818bc24:
	.4byte gMapCellBuffer
.L_0818bc28:
	.4byte 0x00000134
.L_0818bc2c:
	.4byte 0x00000130
.L_0818bc30:
	.4byte IwramCopyWords
.L_0818bc34:
	.4byte Data_02014018
.L_0818bc38:
	.4byte 0xffffff00
.L_0818bc3c:
	.4byte 0xffff00ff
.L_0818bc40:
	.4byte Data_08199364
.L_0818bc44:
	.4byte Data_08199d94
.L_0818bc48:
	.4byte Data_02014000
.L_0818bc4c:
	.4byte 0xffff8000
.L_0818bc50:
	.4byte 0xfffff000
.L_0818bc54:
	.4byte Data_08199210
.L_0818bc58:
	.4byte Data_08197410
.L_0818bc5c:
	.4byte Func_08143000
