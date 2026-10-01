.syntax unified
	.thumb
	.global Func_08142944
	.thumb_func
Func_08142944:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #80
	str r1, [sp, #52]
	movs r1, #246
	lsls r1, r1, #7
	str r0, [sp, #56]
	adds r1, #124
	movs r0, #92
	bl Runtime_AllocateHeapBlock
	movs r1, #128
	str r0, [sp, #48]
	lsls r1, r1, #7
	movs r0, #96
	bl Runtime_AllocateHeapBlock
	movs r1, #240
	str r0, [sp, #44]
	ldr r0, [sp, #48]
	lsls r1, r1, #7
	adds r1, #228
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_081429c4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	ldr r2, [sp, #48]
	str r3, [sp, #36]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_081429c8
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r4, [sp, #48]
	movs r2, #220
	lsls r2, r2, #6
	adds r1, r4, r2
	ldr r0, .L_081429cc
	movs r2, #0
	movs r3, #0
	b .L_081429d0
	.2byte 0x0000
.L_081429c4:
	.4byte 0x00001010
.L_081429c8:
	.4byte 0x0000013e
.L_081429cc:
	.4byte 0x000000c9
.L_081429d0:
	bl Resource_LoadAndDecompress
	ldr r0, .L_08142b5c
	ldr r1, .L_08142b60
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r3, [sp, #48]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
	ldr r0, [sp, #48]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #132
	adds r2, r0, r1
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08142b64
	bl Scheduler_AddOrUpdateCallback
	movs r4, #238
	ldr r3, [sp, #48]
	lsls r4, r4, #7
	adds r4, #180
	adds r2, r3, r4
	movs r3, #24
	str r3, [r2]
	ldr r0, [sp, #48]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #184
	adds r2, r0, r1
	movs r3, #0
	str r3, [r2]
	ldr r3, [sp, #52]
	movs r2, #100
	str r2, [sp, #32]
	cmp r3, #80
	beq .L_08142a2e
	movs r4, #76
	str r4, [sp, #32]
.L_08142a2e:
	ldr r1, [sp, #32]
	movs r0, #0
	mov r9, r0
	cmp r1, #0
	bne .L_08142a3a
	b .L_08142f86
.L_08142a3a:
	movs r3, #96
	negs r3, r3
	movs r2, #68
	str r3, [sp, #8]
	add r2, sp
	mov r11, r2
.L_08142a46:
	ldr r4, [sp, #56]
	mov r1, r11
	ldr r0, [r4, #8]
	bl Func_0815e21c
	ldr r0, [sp, #52]
	cmp r0, #78
	bne .L_08142a66
	mov r1, r11
	ldr r3, [r1]
	adds r3, #40
	str r3, [r1]
	ldr r3, [r1, #4]
	subs r3, #24
	str r3, [r1, #4]
	b .L_08142a84
.L_08142a66:
	ldr r2, [sp, #52]
	cmp r2, #79
	bne .L_08142a7c
	mov r4, r11
	ldr r3, [r4]
	subs r3, #16
	str r3, [r4]
	ldr r3, [r4, #4]
	subs r3, #24
	str r3, [r4, #4]
	b .L_08142a84
.L_08142a7c:
	mov r0, r11
	ldr r3, [r0, #4]
	subs r3, #48
	str r3, [r0, #4]
.L_08142a84:
	mov r1, r9
	cmp r1, #0
	bne .L_08142ae0
	ldr r3, [sp, #48]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
	ldr r0, [sp, #48]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #132
	adds r2, r0, r1
	movs r3, #50
	str r3, [r2]
	ldr r2, [sp, #52]
	cmp r2, #79
	bne .L_08142ac0
	ldr r0, .L_08142b68
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08142b6c
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	b .L_08142ad4
.L_08142ac0:
	ldr r0, .L_08142b70
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08142b6c
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_08142ad4:
	mov r3, r9
	cmp r3, #0
	bne .L_08142ae0
	movs r0, #140
	bl Audio_PlayCue
.L_08142ae0:
	mov r4, r9
	cmp r4, #48
	bne .L_08142aec
	movs r0, #144
	bl Audio_PlayCue
.L_08142aec:
	mov r0, r9
	cmp r0, #56
	bne .L_08142af8
	movs r0, #148
	bl Audio_PlayCue
.L_08142af8:
	mov r1, r9
	cmp r1, #32
	bne .L_08142b10
	ldr r2, [sp, #56]
	movs r3, #1
	negs r3, r3
	ldr r0, [r2, #8]
	movs r1, #7
	adds r2, r3, #0
	str r3, [sp, #0]
	bl Func_0814cd48
.L_08142b10:
	mov r3, r9
	cmp r3, #68
	bne .L_08142b28
	ldr r4, [sp, #56]
	movs r3, #1
	negs r3, r3
	ldr r0, [r4, #8]
	movs r1, #0
	adds r2, r3, #0
	str r3, [sp, #0]
	bl Func_0814cd48
.L_08142b28:
	ldr r0, [sp, #52]
	cmp r0, #80
	bne .L_08142b78
	mov r1, r9
	cmp r1, #61
	ble .L_08142b84
	ldr r3, [sp, #48]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	ldr r0, [sp, #48]
	movs r1, #238
	lsls r1, r1, #7
	ldr r3, .L_08142b74
	adds r1, #132
	adds r2, r0, r1
	str r3, [r2]
	movs r0, #2
	movs r1, #2
	movs r2, #2
	bl Func_08164a4c
	b .L_08142b84
	.2byte 0x0000
.L_08142b5c:
	.4byte 0x000000da
.L_08142b60:
	.4byte Data_02012000
.L_08142b64:
	.4byte Func_08143000
.L_08142b68:
	.4byte 0x00000148
.L_08142b6c:
	.4byte IwramCopyWords
.L_08142b70:
	.4byte 0x0000014a
.L_08142b74:
	.4byte 0x10101010
.L_08142b78:
	mov r2, r9
	cmp r2, #62
	bne .L_08142b84
	ldr r0, [sp, #52]
	bl Func_08118150
.L_08142b84:
	mov r3, r9
	cmp r3, #47
	bgt .L_08142b8c
	b .L_08142cb4
.L_08142b8c:
	cmp r3, #48
	bne .L_08142bbe
	ldr r3, .L_08142ee0
	movs r2, #1
	movs r7, #0
	negs r2, r2
.L_08142b98:
	adds r7, #1
	str r2, [r3]
	adds r3, #28
	cmp r7, #64
	bne .L_08142b98
	ldr r4, [sp, #48]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #168
	adds r3, r4, r0
	movs r2, #8
	str r2, [r3]
	movs r1, #240
	ldr r3, .L_08142ee4
	ldr r0, [sp, #44]
	lsls r1, r1, #6
	ldr r2, .L_08142ee8
	mov lr, r3
	.2byte 0xf800
.L_08142bbe:
	mov r3, r9
	subs r3, #48
	cmp r3, #7
	bhi .L_08142c62
	bl Random16
	ldr r3, [sp, #68]
	movs r2, #63
	lsrs r1, r3, #31
	adds r3, r3, r1
	asrs r3, r3, #1
	ands r2, r0
	adds r2, r2, r3
	subs r2, #32
	lsls r2, r2, #16
	str r2, [sp, #28]
	bl Random16
	ldr r2, [sp, #72]
	movs r3, #31
	ands r3, r0
	ldr r4, [sp, #8]
	adds r3, r3, r2
	lsls r3, r3, #16
	str r3, [sp, #24]
	ldr r0, .L_08142eec
	lsls r3, r4, #3
	subs r3, r3, r4
	movs r1, #0
	movs r2, #15
	lsls r3, r3, #2
	mov r8, r1
	mov r10, r2
	adds r7, r3, r0
.L_08142c02:
	bl Random16
	movs r5, #31
	ands r5, r0
	bl Random16
	adds r6, r0, #0
	bl Random16
	mov r1, r10
	ands r0, r1
	ldr r2, [sp, #28]
	subs r0, #8
	lsls r0, r0, #16
	adds r0, r2, r0
	str r0, [r7]
	bl Random16
	mov r3, r10
	ldr r4, [sp, #24]
	ands r0, r3
	subs r0, #8
	lsls r0, r0, #16
	adds r0, r4, r0
	str r0, [r7, #4]
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #32
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #3
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	movs r0, #1
	asrs r3, r3, #3
	add r8, r0
	str r3, [r7, #16]
	mov r1, r8
	movs r3, #0
	str r3, [r7, #24]
	adds r7, #28
	cmp r1, #2
	bne .L_08142c02
.L_08142c62:
	ldr r5, .L_08142eec
	movs r7, #0
.L_08142c66:
	ldr r0, [r5, #24]
	cmp r0, #17
	bhi .L_08142cac
	movs r1, #3
	bl Math_Div
	ldr r2, [sp, #48]
	adds r1, r0, #0
	lsls r1, r1, #11
	movs r3, #224
	adds r1, r2, r1
	lsls r3, r3, #3
	movs r4, #2
	ldrsh r2, [r5, r4]
	adds r1, r1, r3
	movs r0, #6
	ldrsh r3, [r5, r0]
	movs r0, #32
	str r0, [sp, #0]
	movs r0, #64
	subs r3, #48
	str r0, [sp, #4]
	subs r2, #16
	ldr r0, [sp, #44]
	ldr r4, [sp, #36]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #62
	ldr r2, .L_08142ef0
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_08142cac:
	adds r7, #1
	adds r5, #28
	cmp r7, #16
	bne .L_08142c66
.L_08142cb4:
	mov r0, r9
	cmp r0, #0
	bge .L_08142cbc
	b .L_08142f54
.L_08142cbc:
	ldr r3, [sp, #68]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [sp, #20]
	ldr r3, [sp, #72]
	lsls r3, r3, #16
	str r3, [sp, #16]
	cmp r0, #63
	bgt .L_08142cfa
	mov r3, r9
	adds r3, #8
	movs r5, #128
	cmp r3, #64
	bgt .L_08142cea
	lsls r5, r3, #1
	movs r2, #128
	ldr r0, .L_08142ef4
	adds r1, r5, #0
	lsls r2, r2, #9
	bl Func_0815b434
.L_08142cea:
	ldr r2, [sp, #20]
	ldr r3, [sp, #16]
	asrs r1, r2, #16
	ldr r0, .L_08142ef4
	asrs r2, r3, #16
	adds r3, r5, #0
	bl Func_0818caa8
.L_08142cfa:
	mov r4, r9
	cmp r4, #0
	bne .L_08142d3e
	ldr r5, [sp, #48]
	movs r7, #0
	movs r6, #0
.L_08142d06:
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r0
	str r3, [r5]
	bl Random16
	movs r3, #255
	ands r3, r0
	adds r3, #128
	str r3, [r5, #4]
	bl Random16
	movs r3, #1
	ands r0, r3
	cmp r0, #0
	beq .L_08142d32
	ldr r3, [r5, #4]
	negs r3, r3
	str r3, [r5, #4]
.L_08142d32:
	adds r7, #1
	str r6, [r5, #24]
	subs r6, #4
	adds r5, #28
	cmp r7, #16
	bne .L_08142d06
.L_08142d3e:
	movs r5, #128
	lsls r5, r5, #2
	adds r0, r5, #0
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #12]
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_08142ef8
	ldr r3, [sp, #60]
	mov r8, r0
	ands r3, r2
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_08142efc
	mov r1, r8
	movs r0, #0
	str r0, [r1, #20]
	ands r3, r2
	ldr r2, [sp, #48]
	movs r4, #220
	orrs r3, r5
	lsls r4, r4, #6
	str r3, [sp, #60]
	add r6, sp, #60
	adds r3, r2, r4
	str r3, [r6, #4]
	movs r3, #7
	str r3, [r1]
	ldr r3, .L_08142f00
	str r6, [r1, #16]
	str r3, [r1, #8]
	ldr r0, [sp, #12]
	movs r7, #0
	str r0, [r1, #12]
	ldr r5, [sp, #48]
	movs r1, #128
	lsls r1, r1, #9
	mov r10, r1
.L_08142d8e:
	ldr r2, [r5, #4]
	ldr r1, [r5, #24]
	ldr r3, [r5]
	lsls r2, r2, #1
	adds r1, #2
	adds r3, r3, r2
	str r1, [r5, #24]
	str r3, [r5]
	cmp r1, #47
	bhi .L_08142e08
	bl Func_08014de4
	movs r1, #3
	ands r1, r7
	movs r2, #128
	lsls r0, r1, #17
	lsls r2, r2, #10
	ldr r3, [sp, #20]
	adds r0, r0, r2
	ldr r2, [sp, #16]
	ldr r4, .L_08142f04
	adds r1, #2
	lsls r1, r1, #16
	adds r0, r3, r0
	subs r1, r2, r1
	adds r0, r0, r4
	adds r1, r1, r4
	movs r2, #0
	bl Func_08015160
	movs r0, #128
	mov r1, r10
	mov r2, r10
	lsls r0, r0, #8
	bl Func_080151e4
	ldr r0, [r5]
	bl Func_080150e4
	ldr r2, [r5, #24]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #170
	adds r0, r2, #0
	muls r0, r3
	bl Trig_Sin
	movs r1, #128
	add r0, r10
	lsls r1, r1, #10
	mov r2, r10
	bl Func_080151e4
	ldr r0, .L_08142f08
	ldr r1, [sp, #12]
	movs r2, #4
	bl Func_08196958
	mov r0, r8
	bl Func_08196a7c
.L_08142e08:
	adds r7, #1
	adds r5, #28
	cmp r7, #16
	bne .L_08142d8e
	mov r3, r8
	str r6, [r3, #16]
	ldr r4, [sp, #12]
	movs r2, #6
	str r4, [r3, #12]
	add r3, sp, #60
	strb r2, [r3]
	ldr r3, .L_08142f0c
	mov r0, r8
	str r3, [r6, #4]
	movs r3, #7
	str r3, [r0]
	ldr r3, .L_08142f10
	mov r1, r9
	strb r2, [r6, #1]
	str r3, [r0, #8]
	lsls r1, r1, #3
	movs r7, #0
	mov r10, r1
.L_08142e36:
	ldr r3, .L_08142f14
	movs r2, #127
	ldrb r3, [r3, r7]
	adds r1, r3, #0
	mov r3, r10
	ands r3, r2
	adds r1, #48
	mov r2, r8
	strb r3, [r2, #25]
	cmp r9, r1
	ble .L_08142f3c
	mov r4, r9
	subs r3, r1, r4
	lsls r3, r3, #3
	adds r0, r3, #0
	movs r2, #16
	adds r0, #56
	negs r2, r2
	cmp r0, r2
	ble .L_08142e62
	movs r0, #16
	negs r0, r0
.L_08142e62:
	movs r3, #64
	negs r3, r3
	cmp r0, r3
	ble .L_08142f3c
	ldr r3, .L_08142f18
	mov r4, r9
	ldrb r3, [r3, r7]
	subs r2, r4, r1
	muls r2, r3
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r2
	movs r1, #128
	mov r2, r8
	lsls r6, r3, #4
	lsls r1, r1, #7
	str r0, [r2, #20]
	adds r5, r6, r1
	bl Func_08014de4
	mov r3, r11
	ldr r0, [r3]
	movs r1, #128
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #60
	lsls r0, r0, #16
	lsls r1, r1, #14
	movs r2, #0
	bl Func_08015160
	ldr r3, .L_08142f1c
	movs r4, #128
	ldrb r3, [r3, r7]
	lsls r4, r4, #11
	muls r3, r5
	lsls r0, r5, #1
	adds r1, r3, r4
	cmp r5, #0
	bge .L_08142ebe
	movs r2, #128
	lsls r2, r2, #7
	adds r2, #3
	adds r5, r6, r2
.L_08142ebe:
	asrs r2, r5, #2
	bl Func_080151e4
	movs r0, #250
	lsls r0, r0, #2
	bl SceneTransform_ApplyPitch
	movs r3, #1
	ands r3, r7
	cmp r3, #0
	beq .L_08142f20
	lsls r0, r7, #3
	add r0, r9
	lsls r0, r0, #10
	bl Func_08015068
	b .L_08142f2c
.L_08142ee0:
	.4byte Data_02014018
.L_08142ee4:
	.4byte IwramFillWords
.L_08142ee8:
	.4byte 0x3f3f3f3f
.L_08142eec:
	.4byte Data_02014000
.L_08142ef0:
	.4byte 0xfffff800
.L_08142ef4:
	.4byte gMapCellBuffer
.L_08142ef8:
	.4byte 0xffffff00
.L_08142efc:
	.4byte 0xffff00ff
.L_08142f00:
	.4byte Data_08199220
.L_08142f04:
	.4byte 0xffc00000
.L_08142f08:
	.4byte Data_081991b0
.L_08142f0c:
	.4byte Data_02012000
.L_08142f10:
	.4byte Data_081990d0
.L_08142f14:
	.4byte Data_081977ec
.L_08142f18:
	.4byte Data_081977f0
.L_08142f1c:
	.4byte Data_081977f4
.L_08142f20:
	lsls r0, r7, #3
	mov r3, r9
	subs r0, r0, r3
	lsls r0, r0, #10
	bl Func_08015068
.L_08142f2c:
	ldr r0, .L_08142fec
	ldr r1, [sp, #12]
	movs r2, #32
	bl Func_08196958
	mov r0, r8
	bl Func_08196a7c
.L_08142f3c:
	movs r4, #32
	adds r7, #1
	add r10, r4
	cmp r7, #4
	beq .L_08142f48
	b .L_08142e36
.L_08142f48:
	mov r0, r8
	bl Sys_Free
	ldr r0, [sp, #12]
	bl Sys_Free
.L_08142f54:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r1, #240
	ldr r0, [sp, #48]
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #8]
	ldr r4, [sp, #32]
	movs r3, #1
	adds r2, #2
	add r9, r3
	str r2, [sp, #8]
	cmp r9, r4
	beq .L_08142f86
	b .L_08142a46
.L_08142f86:
	ldr r0, .L_08142ff0
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, [sp, #52]
	cmp r0, #80
	bne .L_08142fcc
	movs r0, #195
	lsls r0, r0, #1
	bl Audio_PlayCue
	movs r4, #238
	ldr r1, [sp, #48]
	lsls r4, r4, #7
	adds r4, #160
	adds r3, r1, r4
	ldr r3, [r3]
	ldr r2, .L_08142ff4
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #164
	strh r3, [r2, #4]
	adds r3, r1, r0
	ldr r3, [r3]
	ldr r0, .L_08142ff8
	strh r3, [r2, #6]
	ldr r2, .L_08142ffc
	movs r3, #120
	str r3, [r2, #12]
	str r3, [r2, #16]
	bl Scheduler_RemoveCallback
	b .L_08142fd0
.L_08142fcc:
	bl Func_08143bb8
.L_08142fd0:
	movs r0, #96
	bl Runtime_ReleaseHeapBlock
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	add sp, #80
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08142fec:
	.4byte Data_08199090
.L_08142ff0:
	.4byte Func_08143000
.L_08142ff4:
	.4byte Data_03001120
.L_08142ff8:
	.4byte Func_08143488
.L_08142ffc:
	.4byte gCameraSceneParameters
