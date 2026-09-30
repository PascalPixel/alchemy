.syntax unified
	.thumb
	.global Func_0814b9e8
	.thumb_func
Func_0814b9e8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #88
	str r1, [sp, #36]
	str r0, [sp, #40]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	str r0, [sp, #32]
	movs r0, #0
	ldr r3, [r3, #96]
	str r3, [sp, #28]
	bl BattleFx_BeginCanvasLayer
	ldr r1, [sp, #36]
	cmp r1, #0
	bne .L_0814ba70
	ldr r2, [sp, #40]
	movs r3, #1
	ldr r1, [r2, #4]
	adds r0, r2, #0
	eors r1, r3
	lsls r1, r1, #4
	movs r3, #33
	orrs r1, r3
	mov r3, sp
	adds r3, #76
	str r3, [sp, #16]
	ldr r2, [sp, #16]
	add r3, sp, #64
	bl Func_0815585c
	movs r2, #184
	ldr r4, [sp, #32]
	lsls r2, r2, #6
	adds r2, #16
	adds r1, r4, r2
	ldr r0, .L_0814bb40
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r4, #216
	ldr r3, [sp, #32]
	lsls r4, r4, #7
	adds r4, #192
	adds r1, r3, r4
	movs r2, #1
	movs r3, #0
	ldr r0, .L_0814bb44
	bl Func_08157cf4
	ldr r0, .L_0814bb48
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0814bb4c
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	b .L_0814baea
.L_0814ba70:
	ldr r0, [sp, #36]
	cmp r0, #1
	bne .L_0814babc
	ldr r2, [sp, #40]
	movs r3, #2
	ldr r1, [r2, #4]
	eors r1, r0
	lsls r1, r1, #4
	orrs r1, r3
	mov r3, sp
	adds r3, #76
	str r3, [sp, #16]
	adds r0, r2, #0
	add r3, sp, #64
	ldr r2, [sp, #16]
	bl Func_0815585c
	movs r2, #216
	ldr r4, [sp, #32]
	lsls r2, r2, #7
	adds r2, #192
	adds r1, r4, r2
	ldr r0, .L_0814bb50
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r4, #184
	ldr r3, [sp, #32]
	lsls r4, r4, #6
	adds r4, #16
	adds r1, r3, r4
	ldr r0, .L_0814bb54
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	b .L_0814baea
.L_0814babc:
	ldr r2, [sp, #40]
	add r5, sp, #52
	movs r1, #36
	ldrsh r0, [r2, r1]
	adds r1, r5, #0
	bl Func_0815e21c
	mov r3, sp
	adds r3, #76
	str r3, [sp, #16]
	ldr r4, [sp, #16]
	ldr r3, [r5]
	ldr r0, .L_0814bb54
	str r3, [r4]
	ldr r2, [sp, #32]
	movs r3, #184
	lsls r3, r3, #6
	adds r3, #16
	adds r1, r2, r3
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
.L_0814baea:
	ldr r4, [sp, #32]
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
	lsls r1, r1, #4
	ldr r0, .L_0814bb58
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_0814bb3c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, [sp, #16]
	movs r1, #128
	ldr r2, [r3]
	movs r3, #64
	lsls r1, r1, #19
	subs r3, r3, r2
	lsls r3, r3, #8
	adds r1, #40
	str r3, [r1]
	ldr r4, [sp, #40]
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_0814bb5c
	movs r0, #104
	movs r1, #7
	bl Func_081963ec
	b .L_0814bb64
	.2byte 0x0000
.L_0814bb3c:
	.4byte 0x00000100
.L_0814bb40:
	.4byte 0x000000d0
.L_0814bb44:
	.4byte 0x00000154
.L_0814bb48:
	.4byte 0x00000130
.L_0814bb4c:
	.4byte IwramCopyWords
.L_0814bb50:
	.4byte 0x00000150
.L_0814bb54:
	.4byte 0x000000d1
.L_0814bb58:
	.4byte Func_08143000
.L_0814bb5c:
	movs r0, #104
	movs r1, #3
	bl Func_081963ec
.L_0814bb64:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r0, #0
	str r3, [sp, #20]
	mov r9, r0
.L_0814bb70:
	mov r1, r9
	cmp r1, #8
	bne .L_0814bb7c
	movs r0, #246
	bl Audio_PlayCue
.L_0814bb7c:
	mov r2, r9
	cmp r2, #0
	bne .L_0814bbf6
	movs r6, #255
	ldr r7, .L_0814beb4
	ldr r5, [sp, #32]
	movs r3, #0
	lsls r6, r6, #8
	mov r8, r3
	adds r6, #255
.L_0814bb90:
	bl Random16
	ands r0, r6
	str r0, [r5]
	bl Random16
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #8]
	ldr r4, [sp, #36]
	cmp r4, #2
	bne .L_0814bbb0
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r5, #4]
	b .L_0814bbb2
.L_0814bbb0:
	str r7, [r5, #4]
.L_0814bbb2:
	bl Random16
	movs r1, #128
	ands r0, r6
	lsls r1, r1, #9
	lsls r0, r0, #1
	adds r0, r0, r1
	str r0, [r5, #16]
	bl Random16
	ands r0, r6
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	str r0, [r5, #20]
	bl Random16
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	movs r2, #128
	ands r3, r0
	lsls r2, r2, #6
	adds r3, r3, r2
	str r3, [r5, #24]
	movs r4, #1
	ldr r3, .L_0814beb8
	add r8, r4
	mov r0, r8
	adds r7, r7, r3
	adds r5, #28
	cmp r0, #32
	bne .L_0814bb90
.L_0814bbf6:
	mov r1, r9
	cmp r1, #56
	bne .L_0814bc3a
	movs r0, #212
	bl Audio_PlayCue
	movs r0, #1
	negs r0, r0
	bl Func_081180e8
	ldr r4, [sp, #40]
	movs r2, #0
	ldr r3, [r4, #20]
	mov r8, r2
	cmp r3, #0
	beq .L_0814bc3a
	movs r5, #36
	movs r6, #8
.L_0814bc1a:
	ldr r1, [sp, #40]
	mov r3, r8
	ldrsh r0, [r5, r1]
	movs r2, #1
	movs r1, #7
	negs r2, r2
	str r6, [sp, #0]
	bl Func_0814cd48
	ldr r4, [sp, #40]
	movs r3, #1
	add r8, r3
	ldr r3, [r4, #20]
	adds r5, #2
	cmp r8, r3
	bne .L_0814bc1a
.L_0814bc3a:
	ldr r0, [sp, #40]
	ldr r3, [r0, #28]
	cmp r3, #1
	bne .L_0814bd32
	mov r1, r9
	lsls r5, r1, #11
	adds r0, r5, #0
	bl Trig_Sin
	negs r0, r0
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	asrs r3, r3, #16
	adds r3, #44
	adds r0, r5, #0
	mov r8, r3
	bl Trig_Cos
	ldr r2, [sp, #16]
	lsls r0, r0, #2
	ldr r3, [r2, #4]
	asrs r0, r0, #16
	adds r0, r0, r3
	adds r5, r0, #0
	mov r3, r9
	subs r5, #24
	cmp r3, #32
	ble .L_0814bc7c
	lsls r3, r3, #2
	subs r3, r5, r3
	adds r5, r3, #0
	adds r5, #128
.L_0814bc7c:
	ldr r0, [sp, #32]
	movs r1, #216
	lsls r1, r1, #7
	adds r1, #192
	adds r4, r0, r1
	movs r6, #40
	adds r1, r4, #0
	mov r2, r8
	str r4, [sp, #8]
	str r6, [sp, #0]
	str r6, [sp, #4]
	ldr r7, [sp, #20]
	ldr r0, [sp, #28]
	adds r3, r5, #0
	mov lr, r7
	.2byte 0xf800
	mov r2, r9
	ldr r4, [sp, #8]
	cmp r2, #3
	bgt .L_0814bcb4
	str r6, [sp, #0]
	str r6, [sp, #4]
	ldr r0, [sp, #28]
	adds r1, r4, #0
	mov r2, r8
	adds r3, r5, #0
	mov lr, r7
	.2byte 0xf800
.L_0814bcb4:
	mov r3, r9
	cmp r3, #79
	bgt .L_0814bd32
	lsrs r3, r3, #31
	add r3, r9
	asrs r3, r3, #1
	lsls r1, r3, #8
	adds r1, r1, r3
	ldr r2, .L_0814bebc
	lsls r3, r1, #16
	adds r1, r1, r3
	ldr r4, [sp, #32]
	subs r2, r2, r1
	movs r1, #224
	lsls r1, r1, #3
	ldr r3, .L_0814bec0
	adds r0, r4, r1
	movs r1, #128
	mov lr, r3
	.2byte 0xf800
	movs r2, #0
	mov r3, r9
	mov r8, r2
	lsls r5, r3, #9
.L_0814bce4:
	adds r0, r5, #0
	bl Trig_Sin
	mov r3, r9
	muls r3, r0
	adds r0, r5, #0
	asrs r3, r3, #16
	adds r6, r3, #0
	bl Trig_Cos
	mov r3, r9
	muls r3, r0
	adds r6, #64
	cmp r3, #0
	bge .L_0814bd04
	adds r3, #7
.L_0814bd04:
	movs r2, #1
	ldr r4, [sp, #32]
	str r2, [sp, #0]
	movs r2, #128
	str r2, [sp, #4]
	movs r2, #224
	lsls r2, r2, #3
	asrs r3, r3, #19
	adds r1, r4, r2
	subs r3, #16
	adds r2, r6, #0
	ldr r0, [sp, #28]
	ldr r4, [sp, #20]
	mov lr, r4
	.2byte 0xf800
	movs r1, #1
	movs r0, #128
	add r8, r1
	lsls r0, r0, #5
	mov r2, r8
	adds r5, r5, r0
	cmp r2, #16
	bne .L_0814bce4
.L_0814bd32:
	bl Func_08014de4
	ldr r2, .L_0814bec4
	movs r3, #104
	str r3, [r2, #16]
	mov r3, r9
	cmp r3, #63
	ble .L_0814bd44
	b .L_0814be68
.L_0814bd44:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r11, r0
	movs r0, #1
	bl Func_081969f8
	mov r4, r9
	mov r10, r0
	movs r3, #0
	cmp r4, #55
	ble .L_0814bd62
	movs r3, #56
	subs r3, r3, r4
	lsls r3, r3, #3
.L_0814bd62:
	mov r0, r10
	str r3, [r0, #20]
	ldr r2, .L_0814bec8
	ldr r3, [sp, #44]
	movs r1, #6
	ands r3, r2
	ldr r2, .L_0814becc
	orrs r3, r1
	ands r3, r2
	movs r2, #192
	lsls r2, r2, #3
	orrs r3, r2
	movs r4, #184
	ldr r2, [sp, #32]
	lsls r4, r4, #6
	adds r4, #16
	str r3, [sp, #44]
	adds r3, r2, r4
	add r2, sp, #44
	str r3, [r2, #4]
	ldr r3, .L_0814bed0
	str r1, [r0]
	mov r1, r11
	str r2, [r0, #16]
	str r3, [r0, #8]
	str r1, [r0, #12]
	movs r2, #0
	str r2, [sp, #12]
	ldr r6, [sp, #32]
	mov r8, r2
.L_0814bd9e:
	ldr r3, [sp, #12]
	ldr r7, [r6, #24]
	cmp r9, r3
	blt .L_0814be4a
	bl Func_08014de4
	movs r0, #176
	lsls r0, r0, #4
	adds r0, #184
	bl SceneTransform_ApplyPitch
	ldr r4, [sp, #36]
	cmp r4, #2
	bne .L_0814bdc2
	ldr r3, [r6, #4]
	ldr r2, [r6, #16]
	subs r3, r3, r2
	b .L_0814bdc8
.L_0814bdc2:
	ldr r3, [r6, #4]
	ldr r2, [r6, #16]
	adds r3, r3, r2
.L_0814bdc8:
	str r3, [r6, #4]
	ldr r3, [r6, #16]
	ldr r0, .L_0814bed4
	cmp r3, r0
	bge .L_0814bddc
	ldr r3, [r6, #8]
	movs r1, #128
	lsls r1, r1, #9
	adds r3, r3, r1
	b .L_0814bde4
.L_0814bddc:
	ldr r3, [r6, #8]
	movs r2, #192
	lsls r2, r2, #8
	adds r3, r3, r2
.L_0814bde4:
	str r3, [r6, #8]
	ldr r0, [r6]
	bl Trig_Sin
	movs r4, #10
	ldrsh r3, [r6, r4]
	adds r5, r3, #0
	muls r5, r0
	ldr r0, [r6]
	bl Trig_Cos
	movs r1, #10
	ldrsh r3, [r6, r1]
	movs r4, #128
	adds r2, r3, #0
	muls r2, r0
	ldr r3, [r6]
	lsls r4, r4, #2
	adds r3, r3, r4
	str r3, [r6]
	ldr r1, [r6, #4]
	adds r0, r5, #0
	bl Func_08015160
	adds r1, r7, #0
	adds r2, r7, #0
	adds r0, r7, #0
	bl Func_080151e4
	ldr r0, [r6, #12]
	bl SceneTransform_ApplyPitch
	ldr r0, [r6, #20]
	bl Func_080150e4
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #3
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #20]
	ldr r0, .L_0814bed8
	adds r3, r3, r2
	str r3, [r6, #20]
	mov r1, r11
	movs r2, #4
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_0814be4a:
	ldr r0, [sp, #12]
	movs r1, #1
	add r8, r1
	adds r0, #3
	mov r2, r8
	str r0, [sp, #12]
	adds r6, #28
	cmp r2, #16
	bne .L_0814bd9e
	mov r0, r10
	bl Sys_Free
	mov r0, r11
	bl Sys_Free
.L_0814be68:
	ldr r2, .L_0814bec4
	movs r3, #120
	str r3, [r2, #12]
	str r3, [r2, #16]
	bl Func_081434f8
	movs r4, #240
	ldr r3, [sp, #32]
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	movs r0, #1
	add r9, r0
	mov r1, r9
	cmp r1, #64
	beq .L_0814be94
	b .L_0814bb70
.L_0814be94:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_0814bedc
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #88
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0814beb4:
	.4byte 0xffe00000
.L_0814beb8:
	.4byte 0xfffc0000
.L_0814bebc:
	.4byte 0x28282828
.L_0814bec0:
	.4byte IwramFillWords
.L_0814bec4:
	.4byte gCameraSceneParameters
.L_0814bec8:
	.4byte 0xffffff00
.L_0814becc:
	.4byte 0xffff00ff
.L_0814bed0:
	.4byte Data_08199340
.L_0814bed4:
	.4byte 0xfffe0000
.L_0814bed8:
	.4byte Data_08199210
.L_0814bedc:
	.4byte Func_08143000
