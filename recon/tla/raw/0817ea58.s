.syntax unified
	.thumb
	.global Func_0817ea58
	.thumb_func
Func_0817ea58:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #268
	str r0, [sp, #148]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #96]
	adds r3, r5, #0
	str r0, [sp, #144]
	adds r3, #176
	ldr r2, [r5, #100]
	ldr r1, [r5, #92]
	str r2, [sp, #136]
	movs r0, #0
	ldr r3, [r3]
	mov r11, r1
	str r3, [sp, #132]
	ldr r7, .L_0817eb0c
	ldr r3, [r5, #36]
	str r3, [sp, #128]
	bl BattleFx_BeginCanvasLayer
	bl Func_0813ba50
	bl Func_08179e6c
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	ldr r3, [r5, #104]
	movs r2, #239
	lsls r2, r2, #7
	str r3, [sp, #176]
	add r2, r11
	movs r3, #0
	movs r1, #200
	str r3, [r2]
	ldr r0, .L_0817eb10
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	movs r4, #0
	str r4, [sp, #124]
	str r4, [sp, #112]
	str r7, [sp, #108]
	str r4, [sp, #104]
	ldr r3, .L_0817eb14
	movs r0, #80
	ldrh r3, [r3, #4]
	negs r0, r0
	str r3, [sp, #100]
	str r0, [sp, #96]
	str r4, [sp, #140]
.L_0817eace:
	ldr r1, [sp, #140]
	cmp r1, #27
	beq .L_0817ead6
	b .L_0817eec0
.L_0817ead6:
	ldr r2, [sp, #128]
	ldr r5, .L_0817eb08
	ldr r0, [r2, #84]
	bl Resource_ResetEntry
	ldr r2, .L_0817eb18
	movs r3, #240
	add r0, sp, #240
	str r3, [r2, #16]
	movs r3, #255
	strh r3, [r0]
	movs r1, #0
	bl BattleActor_SpawnObjectsForListFar
	movs r0, #1
	ldr r1, .L_0817eb1c
	movs r2, #0
	bl Func_08118040
	movs r4, #160
	lsls r4, r4, #19
	movs r3, #0
	adds r4, #192
	mov r8, r3
	b .L_0817eb20
.L_0817eb08:
	.4byte 0x0000001f
.L_0817eb0c:
	.4byte 0xffe30000
.L_0817eb10:
	.4byte Func_08143000
.L_0817eb14:
	.4byte Data_03001120
.L_0817eb18:
	.4byte gCameraSceneParameters
.L_0817eb1c:
	.4byte 0x00000045
.L_0817eb20:
	ldrh r3, [r4]
	movs r1, #31
	ands r1, r3
	lsls r3, r3, #16
	lsrs r2, r3, #21
	ands r2, r5
	lsrs r3, r3, #26
	ands r3, r5
	subs r0, r2, #4
	subs r3, #4
	subs r1, #4
	cmp r0, #0
	bge .L_0817eb3c
	movs r0, #0
.L_0817eb3c:
	cmp r1, #0
	bge .L_0817eb42
	movs r1, #0
.L_0817eb42:
	cmp r3, #0
	bge .L_0817eb48
	movs r3, #0
.L_0817eb48:
	cmp r0, #31
	ble .L_0817eb4e
	movs r0, #31
.L_0817eb4e:
	cmp r1, #31
	ble .L_0817eb54
	movs r1, #31
.L_0817eb54:
	cmp r3, #31
	ble .L_0817eb5a
	movs r3, #31
.L_0817eb5a:
	lsls r3, r3, #10
	lsls r2, r1, #5
	movs r7, #1
	orrs r3, r2
	add r8, r7
	orrs r3, r0
	mov r0, r8
	strh r3, [r4]
	adds r4, #2
	cmp r0, #128
	bne .L_0817eb20
	movs r3, #238
	lsls r3, r3, #7
	movs r2, #238
	adds r3, #144
	lsls r2, r2, #7
	movs r1, #0
	add r3, r11
	adds r2, #148
	str r1, [r3]
	add r2, r11
	movs r3, #5
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #152
	add r2, r11
	subs r3, #6
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #156
	add r3, r11
	str r1, [r3]
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0817ee84
	bl Scheduler_AddOrUpdateCallback
	ldr r1, [sp, #132]
	movs r3, #1
	str r3, [r1, #16]
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_0817ee88
	movs r2, #0
	add r1, r11
	movs r3, #0
	bl Func_08157cf4
	ldr r6, .L_0817ee8c
	movs r2, #0
	mov r10, r2
	movs r5, #6
	movs r4, #32
	movs r0, #0
.L_0817ebca:
	ldr r7, [sp, #136]
	movs r3, #0
	mov r8, r3
	lsls r3, r0, #1
	adds r2, r3, r7
	mov r3, r10
	lsls r1, r3, #2
.L_0817ebd8:
	ldrh r3, [r6, r5]
	movs r7, #224
	lsls r7, r7, #3
	add r3, r8
	adds r3, r3, r7
	mov r7, r11
	ldrb r3, [r7, r3]
	cmp r3, #0
	beq .L_0817ebf4
	subs r3, r3, r1
	subs r3, #40
	cmp r3, #0
	bgt .L_0817ebf4
	movs r3, #1
.L_0817ebf4:
	strb r3, [r2]
	movs r3, #1
	add r8, r3
	adds r2, #1
	cmp r8, r4
	bne .L_0817ebd8
	add r10, r3
	mov r7, r10
	adds r0, #16
	cmp r7, #10
	bne .L_0817ebca
	ldr r0, .L_0817ee90
	bl Resource_GetTableEntry
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #32
	ldr r3, .L_0817ee94
	ldr r0, .L_0817ee98
	mov lr, r3
	.2byte 0xf800
	movs r1, #224
	adds r5, #32
	lsls r1, r1, #3
	movs r6, #238
	adds r0, r5, #0
	add r1, r11
	lsls r6, r6, #7
	bl Func_0801587c
	adds r6, #220
	movs r4, #0
	mov r10, r4
	mov r9, r4
	add r6, r11
	movs r7, #0
.L_0817ec3c:
	mov r1, r10
	lsls r3, r1, #12
	movs r2, #224
	movs r0, #0
	add r3, r11
	lsls r2, r2, #3
	mov r8, r0
	adds r5, r3, r2
.L_0817ec4c:
	movs r2, #128
	movs r3, #240
	movs r1, #32
	lsls r2, r2, #24
	lsls r3, r3, #8
	movs r0, #32
	bl Func_0815b290
	mov r4, r8
	movs r1, #238
	adds r3, r7, r4
	lsls r1, r1, #7
	adds r1, #220
	lsls r3, r3, #2
	adds r3, r3, r1
	mov r2, r11
	str r0, [r2, r3]
	movs r4, #13
	ldrb r2, [r0, #9]
	negs r4, r4
	adds r3, r4, #0
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	ldrb r3, [r0, #16]
	strb r2, [r0, #9]
	ldr r0, .L_0817ee9c
	lsls r3, r3, #2
	adds r3, r3, r0
	ldrh r0, [r3, #2]
	ldr r1, .L_0817eea0
	movs r2, #128
	adds r0, r0, r1
	lsls r2, r2, #2
	adds r1, r5, #0
	ldr r3, .L_0817ee94
	mov lr, r3
	.2byte 0xf800
	movs r0, #1
	movs r4, #128
	add r8, r0
	lsls r4, r4, #2
	mov r1, r8
	adds r5, r5, r4
	cmp r1, #8
	bne .L_0817ec4c
	movs r2, #128
	movs r3, #240
	movs r1, #32
	lsls r2, r2, #24
	lsls r3, r3, #8
	movs r0, #32
	bl Func_0815b3b0
	ldr r1, [r6]
	movs r2, #24
	ldr r3, .L_0817ee94
	str r0, [r6, #32]
	mov lr, r3
	.2byte 0xf800
	mov r2, r9
	add r2, r10
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	movs r0, #238
	add r3, r11
	lsls r2, r2, #2
	lsls r0, r0, #7
	movs r4, #0
	adds r1, r3, #0
	add r2, r11
	adds r0, #220
	mov r8, r4
	adds r1, #24
	adds r2, r2, r0
.L_0817ece4:
	ldmia r2!, {r3}
	ldrh r3, [r3, #8]
	lsls r3, r3, #22
	lsrs r3, r3, #22
	str r3, [r1]
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r1, #28
	cmp r4, #8
	bne .L_0817ece4
	add r10, r3
	movs r0, #8
	mov r1, r10
	add r9, r0
	adds r6, #36
	adds r7, #9
	cmp r1, #2
	bne .L_0817ec3c
	ldr r0, .L_0817eea4
	bl Resource_GetTableEntry
	movs r2, #224
	adds r5, r0, #0
	adds r1, r5, #0
	lsls r2, r2, #1
	ldr r3, .L_0817ee94
	ldr r0, .L_0817eea8
	mov lr, r3
	.2byte 0xf800
	movs r4, #224
	lsls r4, r4, #1
	adds r5, r5, r4
	adds r0, r5, #0
	ldr r1, .L_0817eeac
	bl Func_0801587c
	movs r5, #240
	ldr r0, .L_0817ee9c
	ldr r1, .L_0817ee94
	lsls r5, r5, #7
	ldr r6, .L_0817eeac
	movs r7, #0
	adds r5, #36
	mov r8, r7
	mov r9, r0
	mov r10, r1
	add r5, r11
.L_0817ed44:
	movs r1, #32
	ldr r2, .L_0817eeb0
	movs r3, #0
	movs r0, #32
	bl Func_0815b290
	ldrb r3, [r0, #9]
	movs r7, #12
	orrs r3, r7
	strb r3, [r0, #9]
	ldrb r3, [r0, #16]
	stmia r5!, {r0}
	lsls r3, r3, #2
	add r3, r9
	ldrh r0, [r3, #2]
	ldr r2, .L_0817eea0
	adds r1, r6, #0
	adds r0, r0, r2
	movs r2, #128
	lsls r2, r2, #3
	mov lr, r10
	.2byte 0xf800
	movs r4, #1
	movs r3, #128
	add r8, r4
	lsls r3, r3, #3
	mov r0, r8
	adds r6, r6, r3
	cmp r0, #21
	bne .L_0817ed44
	movs r1, #64
	ldr r2, .L_0817eeb4
	movs r3, #0
	movs r0, #32
	bl Func_0815b290
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #120
	mov r1, r11
	str r0, [r1, r3]
	ldrb r3, [r0, #9]
	ldr r2, .L_0817ee9c
	orrs r3, r7
	strb r3, [r0, #9]
	ldrb r3, [r0, #16]
	ldr r7, .L_0817eeac
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r0, [r3, #2]
	ldr r3, .L_0817eea0
	mov r4, r8
	lsls r1, r4, #10
	movs r2, #128
	adds r1, r1, r7
	lsls r2, r2, #4
	adds r0, r0, r3
	ldr r3, .L_0817ee94
	mov lr, r3
	.2byte 0xf800
	ldr r0, .L_0817eeb8
	bl Resource_GetTableEntry
	adds r5, r0, #0
	ldr r4, .L_0817ee94
	adds r1, r5, #0
	movs r2, #32
	adds r5, #32
	ldr r0, .L_0817eebc
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	ldr r1, .L_0817eeac
	bl Func_0801587c
	movs r2, #128
	movs r3, #224
	movs r1, #8
	lsls r2, r2, #7
	lsls r3, r3, #8
	movs r0, #16
	bl Func_0815b290
	movs r7, #13
	ldrb r2, [r0, #9]
	negs r7, r7
	adds r3, r7, #0
	movs r5, #241
	ands r2, r3
	lsls r5, r5, #7
	movs r3, #4
	orrs r2, r3
	add r5, r11
	ldrb r3, [r0, #16]
	strb r2, [r0, #9]
	str r0, [r5]
	ldr r0, .L_0817ee9c
	lsls r3, r3, #2
	adds r3, r3, r0
	ldrh r0, [r3, #2]
	ldr r1, .L_0817eea0
	movs r2, #128
	adds r0, r0, r1
	lsls r2, r2, #1
	ldr r1, .L_0817eeac
	ldr r3, .L_0817ee94
	mov lr, r3
	.2byte 0xf800
	movs r6, #240
	ldr r7, .L_0817ee94
	movs r4, #0
	lsls r6, r6, #7
	mov r8, r4
	adds r6, #132
.L_0817ee28:
	movs r2, #128
	movs r3, #240
	lsls r3, r3, #8
	movs r1, #8
	lsls r2, r2, #7
	movs r0, #16
	bl Func_0815b3b0
	mov r1, r11
	str r0, [r6, r1]
	movs r2, #24
	ldr r1, [r5]
	mov lr, r7
	.2byte 0xf800
	mov r2, r11
	ldr r3, [r5]
	ldr r0, [r6, r2]
	ldrh r2, [r3, #8]
	ldr r1, .L_0817ee80
	ldrh r3, [r0, #8]
	lsls r2, r2, #22
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #8]
	mov r3, r8
	cmp r3, #6
	ble .L_0817ee70
	ldrb r3, [r0, #9]
	movs r4, #13
	negs r4, r4
	adds r2, r4, #0
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r0, #9]
.L_0817ee70:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r6, #4
	cmp r1, #15
	bne .L_0817ee28
	b .L_0817eec0
	.2byte 0x0000
.L_0817ee80:
	.4byte 0xfffffc00
.L_0817ee84:
	.4byte Func_0813baec
.L_0817ee88:
	.4byte 0x00000134
.L_0817ee8c:
	.4byte Data_08197410
.L_0817ee90:
	.4byte 0x000000a0
.L_0817ee94:
	.4byte IwramCopyWords
.L_0817ee98:
	.4byte 0x050003e0
.L_0817ee9c:
	.4byte ResourceTableEntries
.L_0817eea0:
	.4byte 0x06010000
.L_0817eea4:
	.4byte 0x000000a1
.L_0817eea8:
	.4byte 0x05000200
.L_0817eeac:
	.4byte gMapCellBuffer
.L_0817eeb0:
	.4byte 0x80002000
.L_0817eeb4:
	.4byte 0xc000a000
.L_0817eeb8:
	.4byte 0x000000a2
.L_0817eebc:
	.4byte 0x050003c0
.L_0817eec0:
	ldr r2, [sp, #140]
	cmp r2, #27
	bgt .L_0817eec8
	b .L_0817f020
.L_0817eec8:
	ldr r3, .L_0817ef7c
	add r6, sp, #224
	ldr r4, [r3, #4]
	ldr r3, [r3]
	ldr r7, .L_0817ef80
	str r3, [sp, #184]
	str r4, [sp, #188]
	movs r3, #0
	str r3, [r6, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r6, #4]
	ldr r3, [sp, #112]
	movs r4, #128
	lsls r4, r4, #8
	adds r4, r3, r4
	str r4, [sp, #116]
	str r4, [sp, #112]
	cmp r4, r7
	ble .L_0817ef16
	ldr r2, [sp, #104]
	adds r2, #7
	adds r3, r2, #0
	cmp r2, #0
	bge .L_0817eefe
	ldr r3, [sp, #104]
	adds r3, #14
.L_0817eefe:
	asrs r3, r3, #3
	str r3, [sp, #104]
	lsls r3, r3, #3
	subs r2, r2, r3
	ldr r0, [sp, #116]
	ldr r3, .L_0817ef84
	ldr r1, [sp, #112]
	adds r0, r0, r3
	adds r1, r1, r3
	str r2, [sp, #104]
	str r0, [sp, #116]
	str r1, [sp, #112]
.L_0817ef16:
	mov r3, sp
	movs r4, #238
	movs r0, #206
	adds r3, #184
	lsls r4, r4, #7
	lsls r0, r0, #15
	adds r4, #252
	str r3, [sp, #36]
	str r6, [sp, #92]
	str r0, [sp, #12]
	movs r2, #0
	add r4, r11
	mov r10, r2
	movs r7, #0
	mov r9, r4
.L_0817ef34:
	ldr r2, [sp, #12]
	ldr r5, [sp, #104]
	movs r0, #238
	lsls r3, r7, #2
	lsls r0, r0, #7
	str r2, [sp, #88]
	movs r1, #0
	add r3, r11
	adds r0, #220
	mov r8, r1
	adds r5, #64
	adds r4, r3, r0
.L_0817ef4c:
	ldr r2, [sp, #116]
	mov r1, r8
	ldr r0, [sp, #92]
	lsls r3, r1, #21
	adds r3, r3, r2
	str r3, [r0]
	ldr r1, [sp, #88]
	mov r2, r8
	str r1, [r0, #8]
	cmp r2, #8
	bne .L_0817ef88
	mov r3, r9
	ldr r0, [r3]
	ldr r3, [sp, #104]
	adds r3, #71
	adds r2, r3, #0
	cmp r3, #0
	bge .L_0817ef74
	ldr r2, [sp, #104]
	adds r2, #78
.L_0817ef74:
	asrs r2, r2, #3
	lsls r2, r2, #3
	subs r2, r3, r2
	b .L_0817efae
.L_0817ef7c:
	.4byte Data_08196ed0
.L_0817ef80:
	.4byte 0x001fffff
.L_0817ef84:
	.4byte 0xffe00000
.L_0817ef88:
	mov r0, r8
	movs r1, #238
	adds r3, r7, r0
	lsls r1, r1, #7
	lsls r3, r3, #2
	adds r1, #220
	adds r3, r3, r1
	mov r2, r11
	ldr r0, [r2, r3]
	ldr r3, [sp, #104]
	adds r2, r5, #0
	add r3, r8
	cmp r5, #0
	bge .L_0817efa8
	adds r2, r3, #0
	adds r2, #71
.L_0817efa8:
	asrs r2, r2, #3
	lsls r2, r2, #3
	subs r2, r5, r2
.L_0817efae:
	adds r2, r7, r2
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, #24
	mov r2, r11
	ldr r1, [r2, r3]
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r1, r3
	ldr r2, .L_0817efe0
	ldrh r3, [r0, #8]
	ands r3, r2
	orrs r3, r1
	strh r3, [r0, #8]
	ldr r3, [r6]
	ldr r0, .L_0817efe4
	cmp r3, r0
	ble .L_0817efec
	ldr r1, .L_0817efe8
	adds r3, r3, r1
	str r3, [r6]
	b .L_0817efec
	.2byte 0x0000
.L_0817efe0:
	.4byte 0xfffffc00
.L_0817efe4:
	.4byte 0x00ffffff
.L_0817efe8:
	.4byte 0xfee00000
.L_0817efec:
	ldmia r4!, {r0}
	ldr r2, [sp, #36]
	movs r3, #0
	adds r1, r6, #0
	str r4, [sp, #8]
	bl Render_ApplyProjectedPlacementFar
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r5, #1
	ldr r4, [sp, #8]
	cmp r3, #9
	bne .L_0817ef4c
	ldr r0, [sp, #12]
	movs r1, #128
	lsls r1, r1, #14
	add r10, r2
	movs r4, #36
	adds r0, r0, r1
	mov r2, r10
	adds r7, #9
	add r9, r4
	str r0, [sp, #12]
	cmp r2, #2
	bne .L_0817ef34
.L_0817f020:
	ldr r3, [sp, #96]
	movs r6, #128
	adds r3, #4
	lsls r6, r6, #19
	str r3, [sp, #96]
	adds r6, #40
	lsls r3, r3, #8
	str r3, [r6]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	add r3, r11
	movs r7, #1
	str r7, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #140]
	adds r4, #1
	str r4, [sp, #140]
	cmp r4, #52
	beq .L_0817f04e
	b .L_0817eace
.L_0817f04e:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r1, #240
	ldr r0, [sp, #144]
	ldr r5, .L_0817f0b0
	lsls r1, r1, #6
	mov lr, r5
	.2byte 0xf800
	movs r1, #128
	lsls r1, r1, #7
	ldr r0, .L_0817f0b4
	mov lr, r5
	.2byte 0xf800
	ldr r3, .L_0817f0a8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r3, .L_0817f0ac
	mov r0, sp
	adds r2, #20
	movs r5, #0
	adds r0, #176
	strh r3, [r2]
	str r5, [r6]
	str r0, [sp, #84]
	ldr r1, [sp, #84]
	movs r0, #0
	bl Func_08144aac
	movs r3, #238
	lsls r3, r3, #7
	movs r2, #238
	adds r3, #144
	lsls r2, r2, #7
	add r3, r11
	adds r2, #148
	str r5, [r3]
	add r2, r11
	movs r3, #5
	str r3, [r2]
	movs r2, #238
	b .L_0817f0b8
	.2byte 0x0000
.L_0817f0a8:
	.4byte 0x00000786
.L_0817f0ac:
	.4byte 0x00000080
.L_0817f0b0:
	.4byte IwramClearWords
.L_0817f0b4:
	.4byte 0x06004000
.L_0817f0b8:
	lsls r2, r2, #7
	adds r2, #152
	add r2, r11
	subs r3, #6
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #156
	add r3, r11
	str r5, [r3]
	ldr r1, [sp, #132]
	ldr r0, .L_0817f170
	str r7, [r1, #16]
	movs r1, #224
	lsls r1, r1, #3
	add r1, r11
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r1, #220
	lsls r1, r1, #4
	ldr r0, .L_0817f174
	add r1, r11
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r1, #247
	lsls r1, r1, #6
	ldr r0, .L_0817f178
	add r1, r11
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r1, .L_0817f17c
	movs r2, #0
	movs r3, #0
	ldr r0, .L_0817f180
	bl Func_08157cf4
	ldr r0, .L_0817f184
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0817f188
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r11
	movs r3, #50
	str r3, [r2]
	ldr r3, .L_0817f164
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0817f168
	subs r2, #2
	strh r3, [r2]
	ldr r3, .L_0817f18c
	movs r2, #0
	str r3, [sp, #80]
	mov r8, r2
	ldrh r3, [r3]
	ldr r2, .L_0817f16c
	lsrs r3, r3, #5
	ands r3, r2
	cmp r3, #31
	beq .L_0817f1ac
	ldr r0, [sp, #80]
	ldr r1, .L_0817f16c
	adds r2, r0, #0
	b .L_0817f190
	.2byte 0x0000
.L_0817f164:
	.4byte 0x00001010
.L_0817f168:
	.4byte 0x00003f44
.L_0817f16c:
	.4byte 0x0000001f
.L_0817f170:
	.4byte 0x000000ba
.L_0817f174:
	.4byte 0x0000013e
.L_0817f178:
	.4byte 0x000000bb
.L_0817f17c:
	.4byte gMapCellBuffer
.L_0817f180:
	.4byte 0x000000c2
.L_0817f184:
	.4byte 0x00000161
.L_0817f188:
	.4byte IwramCopyWords
.L_0817f18c:
	.4byte 0x05000200
.L_0817f190:
	movs r4, #1
	add r8, r4
	mov r7, r8
	cmp r7, #224
	beq .L_0817f1ac
	lsls r3, r7, #1
	adds r3, r3, r0
	str r3, [sp, #80]
	adds r2, #2
	ldrh r3, [r2]
	lsrs r3, r3, #5
	ands r3, r1
	cmp r3, #31
	bne .L_0817f190
.L_0817f1ac:
	ldr r3, .L_0817f240
	movs r0, #0
	movs r1, #1
	movs r2, #128
	mov r8, r0
	negs r1, r1
	lsls r2, r2, #2
.L_0817f1ba:
	movs r4, #1
	add r8, r4
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_0817f1ba
	movs r7, #0
	str r7, [sp, #140]
	ldr r1, .L_0817f244
	movs r2, #2
	ldr r3, [r1]
	ands r3, r2
	cmp r3, #0
	beq .L_0817f1da
	bl .L_081806f0
.L_0817f1da:
	ldr r3, [r1, #12]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0817f1e8
	bl .L_081806f0
.L_0817f1e8:
	mov r0, sp
	mov r1, sp
	adds r0, #208
	adds r1, #168
	str r0, [sp, #28]
	str r1, [sp, #40]
.L_0817f1f4:
	ldr r2, [sp, #140]
	cmp r2, #0
	beq .L_0817f1fc
	b .L_0817f390
.L_0817f1fc:
	ldr r3, .L_0817f238
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r4, [sp, #80]
	ldr r3, .L_0817f23c
	ldr r7, .L_0817f248
	strh r3, [r4]
	movs r0, #0
	movs r3, #240
	str r7, [sp, #108]
	str r0, [sp, #124]
	lsls r3, r3, #7
	adds r3, #120
	add r3, r11
	ldr r2, [r3]
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r2, #18]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #116
	add r3, r11
	add r1, sp, #140
	ldr r3, [r3]
	ldrh r1, [r1]
	movs r5, #224
	b .L_0817f24c
	.2byte 0x0000
.L_0817f238:
	.4byte 0x00000100
.L_0817f23c:
	.4byte 0x0000190b
.L_0817f240:
	.4byte Data_02014018
.L_0817f244:
	.4byte gInput
.L_0817f248:
	.4byte 0xffe30000
.L_0817f24c:
	strh r1, [r3, #18]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #88
	add r3, r11
	ldr r3, [r3]
	adds r2, r1, #0
	strh r2, [r3, #18]
	ldr r4, [sp, #140]
	movs r3, #140
	lsls r3, r3, #2
	add r3, r11
	str r4, [r3]
	movs r3, #141
	lsls r3, r3, #2
	add r3, r11
	str r4, [r3]
	movs r2, #143
	ldr r3, .L_0817f3cc
	lsls r2, r2, #2
	add r2, r11
	str r3, [r2]
	movs r3, #144
	lsls r3, r3, #2
	add r3, r11
	str r4, [r3]
	movs r3, #146
	lsls r3, r3, #2
	add r3, r11
	str r4, [r3]
	movs r3, #161
	lsls r3, r3, #2
	add r3, r11
	str r4, [r3]
	movs r3, #162
	lsls r3, r3, #2
	add r3, r11
	str r4, [r3]
	movs r2, #164
	ldr r3, .L_0817f3d0
	lsls r2, r2, #2
	add r2, r11
	str r3, [r2]
	movs r3, #165
	lsls r3, r3, #2
	add r3, r11
	str r4, [r3]
	movs r3, #167
	lsls r3, r3, #2
	add r3, r11
	str r4, [r3]
	movs r3, #147
	lsls r3, r3, #2
	add r3, r11
	str r4, [r3]
	movs r3, #148
	lsls r3, r3, #2
	add r3, r11
	str r4, [r3]
	movs r2, #150
	ldr r3, .L_0817f3d4
	lsls r2, r2, #2
	add r2, r11
	str r3, [r2]
	movs r3, #151
	lsls r3, r3, #2
	add r3, r11
	str r4, [r3]
	movs r3, #153
	lsls r3, r3, #2
	add r3, r11
	movs r2, #154
	str r4, [r3]
	lsls r2, r2, #2
	movs r3, #160
	lsls r3, r3, #16
	add r2, r11
	str r3, [r2]
	movs r3, #155
	lsls r3, r3, #2
	add r3, r11
	str r4, [r3]
	movs r3, #157
	lsls r3, r3, #2
	add r3, r11
	str r4, [r3]
	movs r3, #158
	lsls r3, r3, #2
	add r3, r11
	str r4, [r3]
	movs r3, #160
	lsls r3, r3, #2
	add r3, r11
	str r4, [r3]
	lsls r5, r5, #2
	mov r8, r0
	movs r6, #0
	add r5, r11
.L_0817f310:
	mov r0, r8
	movs r1, #3
	bl Math_Div
	movs r7, #176
	lsls r0, r0, #18
	lsls r7, r7, #16
	adds r0, r0, r7
	str r0, [r5]
	movs r1, #3
	mov r0, r8
	bl Math_Mod
	lsls r3, r0, #1
	adds r3, r3, r0
	movs r1, #1
	movs r0, #148
	lsls r3, r3, #17
	lsls r0, r0, #15
	add r8, r1
	adds r3, r3, r0
	mov r2, r8
	str r3, [r5, #4]
	str r6, [r5, #12]
	str r6, [r5, #16]
	str r6, [r5, #24]
	adds r5, #28
	cmp r2, #8
	bne .L_0817f310
	movs r5, #140
	movs r3, #0
	lsls r5, r5, #3
	mov r8, r3
	movs r6, #0
	add r5, r11
.L_0817f356:
	mov r0, r8
	movs r1, #3
	bl Math_Div
	movs r4, #224
	lsls r4, r4, #15
	lsls r0, r0, #18
	adds r0, r0, r4
	str r0, [r5]
	movs r1, #3
	mov r0, r8
	bl Math_Mod
	lsls r3, r0, #1
	adds r3, r3, r0
	movs r7, #164
	movs r0, #1
	lsls r3, r3, #17
	lsls r7, r7, #15
	add r8, r0
	adds r3, r3, r7
	mov r1, r8
	str r3, [r5, #4]
	str r6, [r5, #12]
	str r6, [r5, #16]
	str r6, [r5, #24]
	adds r5, #28
	cmp r1, #8
	bne .L_0817f356
.L_0817f390:
	ldr r4, [sp, #140]
	ldr r7, .L_0817f3d8
	ldr r2, [sp, #124]
	adds r3, r4, r7
	str r2, [sp, #120]
	cmp r3, #15
	bhi .L_0817f3e8
	ldr r0, [sp, #80]
	ldr r2, .L_0817f3c8
	ldrh r3, [r0]
	movs r0, #31
	ands r0, r3
	lsls r3, r3, #16
	lsrs r1, r3, #21
	lsrs r3, r3, #26
	ands r1, r2
	ands r3, r2
	cmp r0, #30
	bgt .L_0817f3b8
	adds r0, #1
.L_0817f3b8:
	cmp r1, #19
	bgt .L_0817f3be
	adds r1, #1
.L_0817f3be:
	cmp r3, #5
	bgt .L_0817f3dc
	adds r3, #1
	b .L_0817f3dc
	.2byte 0x0000
.L_0817f3c8:
	.4byte 0x0000001f
.L_0817f3cc:
	.4byte 0xfffa0000
.L_0817f3d0:
	.4byte 0xfff40000
.L_0817f3d4:
	.4byte 0xffff0000
.L_0817f3d8:
	.4byte 0xfffffde2
.L_0817f3dc:
	lsls r2, r1, #5
	lsls r3, r3, #10
	ldr r1, [sp, #80]
	orrs r3, r2
	orrs r3, r0
	strh r3, [r1]
.L_0817f3e8:
	movs r3, #151
	ldr r2, [sp, #140]
	lsls r3, r3, #1
	adds r3, #255
	cmp r2, r3
	ble .L_0817f42c
	ldr r4, [sp, #80]
	ldr r2, .L_0817f41c
	ldrh r3, [r4]
	movs r0, #31
	ands r0, r3
	lsls r3, r3, #16
	lsrs r1, r3, #21
	lsrs r3, r3, #26
	ands r1, r2
	ands r3, r2
	cmp r0, #20
	ble .L_0817f40e
	subs r0, #1
.L_0817f40e:
	cmp r1, #0
	ble .L_0817f414
	subs r1, #1
.L_0817f414:
	cmp r3, #0
	ble .L_0817f420
	subs r3, #1
	b .L_0817f420
.L_0817f41c:
	.4byte 0x0000001f
.L_0817f420:
	lsls r3, r3, #10
	lsls r2, r1, #5
	ldr r7, [sp, #80]
	orrs r3, r2
	orrs r3, r0
	strh r3, [r7]
.L_0817f42c:
	ldr r0, [sp, #140]
	movs r1, #140
	lsls r1, r1, #1
	cmp r0, r1
	bne .L_0817f454
	ldr r0, .L_0817f4a8
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0817f4ac
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r2, #146
	lsls r2, r2, #2
	add r2, r11
	movs r3, #1
	str r3, [r2]
.L_0817f454:
	ldr r2, [sp, #140]
	movs r3, #144
	lsls r3, r3, #1
	cmp r2, r3
	bne .L_0817f468
	movs r2, #167
	lsls r2, r2, #2
	add r2, r11
	movs r3, #1
	str r3, [r2]
.L_0817f468:
	ldr r4, [sp, #140]
	movs r7, #24
	adds r7, #255
	cmp r4, r7
	bne .L_0817f488
	ldr r1, .L_0817f4a0
	movs r0, #128
	movs r2, #128
	ldr r3, .L_0817f4a4
	lsls r0, r0, #19
	lsls r2, r2, #19
	adds r0, #32
	adds r2, #12
	strh r1, [r0]
	strh r3, [r2]
	strh r1, [r0]
.L_0817f488:
	ldr r0, [sp, #140]
	movs r1, #253
	lsls r1, r1, #1
	cmp r0, r1
	bne .L_0817f4b0
	movs r2, #153
	lsls r2, r2, #2
	add r2, r11
	movs r3, #1
	str r3, [r2]
	b .L_0817f4b0
	.2byte 0x0000
.L_0817f4a0:
	.4byte 0x00000080
.L_0817f4a4:
	.4byte 0x00000784
.L_0817f4a8:
	.4byte 0x00000161
.L_0817f4ac:
	.4byte IwramCopyWords
.L_0817f4b0:
	ldr r2, [sp, #140]
	movs r3, #234
	adds r3, #255
	cmp r2, r3
	bne .L_0817f4d6
	ldr r3, .L_0817f4f0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_0817f4f4
	subs r2, #20
	strh r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #152
	add r2, r11
	movs r3, #0
	str r3, [r2]
.L_0817f4d6:
	ldr r4, [sp, #140]
	movs r7, #132
	lsls r7, r7, #2
	cmp r4, r7
	bne .L_0817f500
	ldr r3, .L_0817f4f0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_0817f4f8
	b .L_0817f4fc
	.2byte 0x0000
.L_0817f4f0:
	.4byte 0x00000080
.L_0817f4f4:
	.4byte 0x00000784
.L_0817f4f8:
	.4byte 0x00000785
.L_0817f4fc:
	subs r2, #20
	strh r3, [r2]
.L_0817f500:
	ldr r0, [sp, #140]
	cmp r0, #0
	bne .L_0817f55a
	movs r1, #239
	movs r2, #238
	lsls r1, r1, #7
	lsls r2, r2, #7
	add r1, r11
	movs r3, #1
	adds r2, #132
	str r3, [r1]
	add r2, r11
	movs r3, #2
	str r0, [r2]
	str r3, [r1]
	movs r3, #50
	str r3, [r2]
	ldr r7, .L_0817f69c
	ldr r6, .L_0817f6a0
	ldr r5, .L_0817f6a4
	movs r1, #0
	mov r8, r1
.L_0817f52c:
	ldrb r3, [r6]
	lsls r3, r3, #16
	str r3, [r5]
	ldrb r3, [r6, #1]
	adds r6, #2
	lsls r3, r3, #16
	str r3, [r5, #4]
	ldr r3, [r7]
	str r3, [r5, #12]
	ldr r3, [r7, #4]
	adds r7, #8
	str r3, [r5, #16]
	bl Random16
	movs r3, #127
	movs r2, #1
	ands r3, r0
	add r8, r2
	str r3, [r5, #24]
	mov r3, r8
	adds r5, #28
	cmp r3, #10
	bne .L_0817f52c
.L_0817f55a:
	ldr r5, .L_0817f6a4
	movs r4, #0
	mov r8, r4
.L_0817f560:
	ldr r3, .L_0817f6a8
	mov r7, r8
	ldrb r3, [r3, r7]
	ldr r0, [sp, #140]
	cmp r0, r3
	blt .L_0817f5b6
	ldr r0, [r5, #24]
	ldr r6, .L_0817f6ac
	cmp r0, #0
	bge .L_0817f576
	adds r0, #3
.L_0817f576:
	movs r1, #12
	asrs r0, r0, #2
	bl Math_Mod
	ldrb r1, [r6, r0]
	movs r2, #247
	lsls r1, r1, #8
	movs r0, #16
	lsls r2, r2, #6
	add r1, r11
	adds r1, r1, r2
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r4, #6
	ldrsh r3, [r5, r4]
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #176]
	ldr r0, [sp, #144]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5]
	ldr r2, [r5, #12]
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #16]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_0817f5b6:
	movs r7, #1
	add r8, r7
	mov r0, r8
	adds r5, #28
	cmp r0, #10
	bne .L_0817f560
	ldr r1, [sp, #140]
	movs r2, #218
	adds r2, #255
	cmp r1, r2
	bgt .L_0817f5d6
	ldr r3, [sp, #112]
	movs r4, #128
	lsls r4, r4, #8
	adds r3, r3, r4
	str r3, [sp, #112]
.L_0817f5d6:
	ldr r7, [sp, #112]
	ldr r0, [sp, #124]
	movs r1, #128
	adds r7, r7, r0
	lsls r1, r1, #14
	str r7, [sp, #116]
	cmp r7, r1
	blt .L_0817f60c
	ldr r2, [sp, #104]
	adds r2, #7
	adds r3, r2, #0
	cmp r2, #0
	bge .L_0817f5f4
	ldr r3, [sp, #104]
	adds r3, #14
.L_0817f5f4:
	asrs r3, r3, #3
	str r3, [sp, #104]
	lsls r3, r3, #3
	subs r2, r2, r3
	ldr r4, [sp, #112]
	ldr r3, .L_0817f6b0
	str r2, [sp, #104]
	ldr r2, [sp, #116]
	adds r4, r4, r3
	adds r2, r2, r3
	str r2, [sp, #116]
	str r4, [sp, #112]
.L_0817f60c:
	ldr r7, [sp, #108]
	cmp r7, #0
	bge .L_0817f61c
	movs r1, #128
	adds r0, r7, #0
	lsls r1, r1, #6
	adds r0, r0, r1
	str r0, [sp, #108]
.L_0817f61c:
	ldr r3, .L_0817f6b4
	movs r2, #0
	ldr r4, [r3, #4]
	ldr r3, [r3]
	str r2, [sp, #68]
	str r3, [sp, #168]
	str r4, [sp, #172]
	movs r4, #141
	ldr r3, [sp, #140]
	lsls r4, r4, #1
	adds r4, #255
	str r2, [sp, #64]
	cmp r3, r4
	ble .L_0817f6cc
	ldr r5, .L_0817f6b8
	adds r0, r5, #0
	bl Trig_Cos
	lsls r0, r0, #2
	str r0, [sp, #76]
	adds r0, r5, #0
	bl Trig_Sin
	lsls r0, r0, #2
	str r0, [sp, #72]
	ldr r7, [sp, #140]
	ldr r0, .L_0817f6bc
	adds r3, r7, r0
	cmp r3, #15
	bhi .L_0817f674
	ldr r1, .L_0817f6c0
	lsls r5, r7, #11
	adds r5, r5, r1
	adds r0, r5, #0
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	adds r0, r5, #0
	str r3, [sp, #68]
	bl Trig_Sin
	lsls r0, r0, #1
	str r0, [sp, #64]
.L_0817f674:
	ldr r2, [sp, #140]
	ldr r4, .L_0817f6c4
	adds r3, r2, r4
	cmp r3, #15
	bhi .L_0817f6e4
	ldr r7, .L_0817f6c8
	lsls r5, r2, #11
	adds r5, r5, r7
	adds r0, r5, #0
	bl Trig_Sin
	lsls r0, r0, #1
	str r0, [sp, #68]
	adds r0, r5, #0
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	str r3, [sp, #64]
	b .L_0817f6e4
.L_0817f69c:
	.4byte Data_08199528
.L_0817f6a0:
	.4byte Data_08199514
.L_0817f6a4:
	.4byte Data_020176e8
.L_0817f6a8:
	.4byte Data_08199578
.L_0817f6ac:
	.4byte Data_08199508
.L_0817f6b0:
	.4byte 0xffe00000
.L_0817f6b4:
	.4byte Data_08196ed8
.L_0817f6b8:
	.4byte 0x00021a00
.L_0817f6bc:
	.4byte 0xfffffde2
.L_0817f6c0:
	.4byte 0xffef1000
.L_0817f6c4:
	.4byte 0xfffffdd0
.L_0817f6c8:
	.4byte 0xffee8000
.L_0817f6cc:
	ldr r0, [sp, #140]
	lsls r5, r0, #8
	adds r0, r5, #0
	bl Trig_Cos
	lsls r0, r0, #2
	str r0, [sp, #76]
	adds r0, r5, #0
	bl Trig_Sin
	lsls r0, r0, #2
	str r0, [sp, #72]
.L_0817f6e4:
	ldr r1, [sp, #72]
	cmp r1, #0
	ble .L_0817f6ee
	negs r1, r1
	str r1, [sp, #72]
.L_0817f6ee:
	ldr r2, [sp, #108]
	ldr r4, [sp, #72]
	ldr r7, [sp, #28]
	lsls r3, r2, #1
	subs r4, r4, r3
	movs r6, #238
	movs r3, #0
	str r4, [sp, #72]
	lsls r6, r6, #7
	str r3, [r7, #12]
	ldr r5, .L_0817fa8c
	movs r3, #255
	lsls r3, r3, #16
	movs r0, #0
	adds r6, #168
	str r3, [r7, #4]
	mov r8, r0
	add r6, r11
	movs r7, #23
.L_0817f714:
	ldrh r3, [r5]
	ldr r1, [sp, #140]
	adds r5, #2
	cmp r1, r3
	bne .L_0817f726
	str r7, [r6]
	movs r0, #144
	bl Audio_PlayCue
.L_0817f726:
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #5
	bne .L_0817f714
	ldr r4, [sp, #140]
	movs r7, #255
	lsls r7, r7, #1
	cmp r4, r7
	bne .L_0817f740
	movs r0, #186
	bl Audio_PlayCue
.L_0817f740:
	ldr r0, [sp, #140]
	movs r1, #142
	lsls r1, r1, #2
	cmp r0, r1
	bne .L_0817f75c
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r11
	movs r3, #10
	str r3, [r2]
	movs r0, #145
	bl Audio_PlayCue
.L_0817f75c:
	movs r3, #128
	ldr r2, [sp, #140]
	lsls r3, r3, #2
	adds r3, #66
	cmp r2, r3
	bne .L_0817f76e
	movs r0, #163
	bl Audio_PlayCue
.L_0817f76e:
	movs r5, #224
	movs r7, #146
	movs r4, #0
	lsls r5, r5, #2
	lsls r7, r7, #2
	mov r8, r4
	movs r6, #0
	add r5, r11
	add r7, r11
.L_0817f780:
	ldr r3, [r5, #24]
	cmp r3, #0
	beq .L_0817f7f0
	ldr r2, [r5]
	ldr r0, .L_0817fa90
	adds r3, #1
	str r3, [r5, #24]
	cmp r2, r0
	ble .L_0817f7b4
	ldr r3, [r5, #12]
	movs r1, #255
	adds r3, r2, r3
	str r3, [r5]
	ldr r2, [r5, #16]
	ldr r3, [r5, #4]
	lsls r1, r1, #8
	ldr r4, .L_0817fa94
	adds r3, r3, r2
	adds r1, #255
	str r3, [r5, #4]
	adds r3, r2, r1
	cmp r3, r4
	bhi .L_0817f7b4
	ldr r3, [r5, #20]
	adds r3, r2, r3
	str r3, [r5, #16]
.L_0817f7b4:
	ldr r3, [r5, #12]
	ldr r0, .L_0817fa98
	cmp r3, r0
	ble .L_0817f7c2
	ldr r1, .L_0817fa9c
	adds r3, r3, r1
	str r3, [r5, #12]
.L_0817f7c2:
	ldr r3, [r5, #24]
	mov r2, r8
	lsls r1, r2, #5
	movs r2, #31
	ands r3, r2
	adds r1, r1, r3
	lsls r2, r1, #3
	ldr r3, .L_0817faa0
	subs r2, r2, r1
	lsls r2, r2, #2
	adds r2, r2, r3
	str r6, [r2, #24]
	ldr r4, .L_0817faa4
	ldr r3, [r5]
	lsrs r1, r3, #31
	adds r3, r3, r1
	asrs r3, r3, #1
	str r3, [r2]
	ldr r3, [r5, #4]
	str r6, [r2, #12]
	adds r3, r3, r4
	str r3, [r2, #4]
	str r6, [r2, #16]
.L_0817f7f0:
	movs r3, #7
	mov r0, r8
	ands r3, r0
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, [r7]
	lsls r2, r2, #1
	adds r2, #10
	cmp r3, r2
	bne .L_0817f854
	movs r0, #138
	bl Audio_PlayCue
	movs r3, #1
	str r3, [r5, #24]
	ldr r1, [sp, #76]
	ldr r3, [r5]
	ldr r2, .L_0817faa8
	adds r3, r3, r1
	str r3, [r5]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	add r3, r11
	ldr r3, [r3]
	ldrsb r3, [r2, r3]
	ldr r2, [sp, #72]
	lsls r3, r3, #16
	subs r3, r2, r3
	ldr r2, [r5, #4]
	adds r2, r2, r3
	str r2, [r5, #4]
	bl Random16
	movs r3, #255
	ldr r4, .L_0817fa9c
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r0
	adds r3, r3, r4
	str r3, [r5, #16]
	bl Random16
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	ands r3, r0
	ldr r0, .L_0817faac
	adds r3, r3, r0
	str r3, [r5, #20]
.L_0817f854:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r5, #28
	cmp r2, #16
	bne .L_0817f780
	ldr r3, [sp, #140]
	movs r4, #234
	adds r4, #255
	cmp r3, r4
	bgt .L_0817f8aa
	movs r7, #8
	movs r6, #240
	mov r8, r7
	lsls r6, r6, #7
	movs r5, #140
	ldr r7, [sp, #28]
	adds r6, #156
	lsls r5, r5, #3
	add r6, r11
	add r5, r11
.L_0817f87e:
	ldr r3, [r5]
	ldr r0, [sp, #124]
	adds r3, r3, r0
	str r3, [r7]
	ldr r3, [r5, #4]
	str r3, [r7, #8]
	ldr r3, [r5, #24]
	cmp r3, #0
	beq .L_0817f89c
	ldr r0, [r6]
	ldr r1, [sp, #28]
	ldr r2, [sp, #40]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
.L_0817f89c:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r6, #4
	adds r5, #28
	cmp r2, #16
	bne .L_0817f87e
.L_0817f8aa:
	movs r4, #128
	ldr r3, [sp, #140]
	lsls r4, r4, #2
	adds r4, #66
	cmp r3, r4
	bne .L_0817f8ce
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #148
	add r2, r11
	movs r3, #1
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #144
	add r2, r11
	movs r3, #0
	str r3, [r2]
.L_0817f8ce:
	ldr r7, [sp, #140]
	ldr r0, .L_0817fab0
	adds r3, r7, r0
	cmp r3, #15
	bhi .L_0817f8ee
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #152
	add r2, r11
	cmp r3, #0
	bge .L_0817f8e8
	ldr r1, .L_0817fab4
	adds r3, r7, r1
.L_0817f8e8:
	asrs r3, r3, #3
	mvns r3, r3
	str r3, [r2]
.L_0817f8ee:
	ldr r4, [sp, #124]
	ldr r7, [sp, #76]
	ldr r0, [sp, #68]
	movs r6, #240
	subs r3, r4, r7
	movs r1, #240
	lsls r6, r6, #7
	ldr r5, [sp, #28]
	ldr r4, .L_0817faa8
	movs r2, #0
	adds r3, r3, r0
	lsls r1, r1, #15
	adds r6, #92
	mov r8, r2
	adds r7, r3, r1
	add r6, r11
.L_0817f90e:
	mov r2, r8
	lsls r3, r2, #5
	movs r2, #238
	str r7, [r5]
	lsls r2, r2, #7
	adds r2, #168
	add r2, r11
	ldr r2, [r2]
	ldr r0, [sp, #72]
	ldrsb r2, [r4, r2]
	ldr r1, [sp, #64]
	subs r3, r3, r2
	lsls r3, r3, #16
	adds r3, r3, r0
	movs r2, #200
	adds r3, r3, r1
	lsls r2, r2, #15
	adds r3, r3, r2
	str r3, [r5, #8]
	ldmia r6!, {r0}
	movs r3, #0
	adds r1, r5, #0
	ldr r2, [sp, #40]
	str r4, [sp, #8]
	bl Render_ApplyProjectedPlacementFar
	movs r3, #1
	add r8, r3
	mov r0, r8
	ldr r4, [sp, #8]
	cmp r0, #2
	bne .L_0817f90e
	ldr r1, [sp, #140]
	movs r2, #158
	lsls r2, r2, #1
	cmp r1, r2
	bgt .L_0817f9f8
	movs r0, #161
	lsls r0, r0, #2
	add r0, r11
	ldr r3, [r0, #24]
	cmp r3, #0
	beq .L_0817f9b2
	adds r3, #1
	ldr r1, [r0, #12]
	str r3, [r0, #24]
	ldr r3, [r0]
	ldr r2, [r0, #16]
	adds r3, r3, r1
	str r3, [r0]
	ldr r3, [r0, #4]
	adds r3, r3, r2
	str r3, [r0, #4]
	movs r3, #128
	lsls r3, r3, #6
	adds r2, r2, r3
	lsls r3, r1, #1
	adds r3, r3, r1
	lsls r3, r3, #4
	str r2, [r0, #16]
	cmp r3, #0
	bge .L_0817f98c
	adds r3, #63
.L_0817f98c:
	asrs r3, r3, #6
	str r3, [r0, #12]
	lsls r3, r2, #6
	subs r2, r3, r2
	cmp r2, #0
	bge .L_0817f99a
	adds r2, #63
.L_0817f99a:
	asrs r3, r2, #6
	str r3, [r0, #16]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #116
	add r3, r11
	ldr r2, [r3]
	movs r4, #128
	ldrh r3, [r2, #18]
	lsls r4, r4, #2
	adds r3, r3, r4
	strh r3, [r2, #18]
.L_0817f9b2:
	ldr r3, [r0]
	ldr r7, [sp, #76]
	ldr r1, [sp, #124]
	subs r3, r3, r7
	ldr r4, [sp, #28]
	movs r2, #240
	adds r3, r3, r1
	lsls r2, r2, #15
	adds r3, r3, r2
	str r3, [r4]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	add r3, r11
	ldr r3, [r3]
	ldr r2, .L_0817faa8
	ldr r7, [sp, #72]
	ldrsb r2, [r2, r3]
	movs r3, #99
	subs r3, r3, r2
	ldr r2, [r0, #4]
	lsls r3, r3, #16
	adds r3, r3, r7
	adds r3, r3, r2
	str r3, [r4, #8]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #116
	add r3, r11
	ldr r0, [r3]
	ldr r1, [sp, #28]
	ldr r2, [sp, #40]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
.L_0817f9f8:
	ldr r0, [sp, #124]
	ldr r1, [sp, #68]
	ldr r4, [sp, #28]
	movs r2, #142
	adds r6, r0, r1
	lsls r2, r2, #16
	adds r3, r6, r2
	movs r0, #238
	str r3, [r4]
	lsls r0, r0, #7
	ldr r7, .L_0817faa8
	adds r0, #168
	add r0, r11
	ldr r3, [r0]
	mov r10, r7
	mov r1, r10
	ldrsb r3, [r1, r3]
	ldr r2, [sp, #72]
	movs r5, #72
	ldr r4, [sp, #64]
	subs r3, r5, r3
	ldr r7, [sp, #28]
	lsls r3, r3, #16
	adds r3, r3, r2
	adds r3, r3, r4
	str r3, [r7, #8]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #36
	add r3, r11
	ldr r1, [sp, #28]
	ldr r2, [sp, #40]
	mov r8, r0
	ldr r0, [r3]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
	movs r0, #174
	lsls r0, r0, #16
	adds r3, r6, r0
	str r3, [r7]
	mov r1, r8
	ldr r3, [r1]
	mov r2, r10
	ldrsb r3, [r2, r3]
	ldr r4, [sp, #64]
	subs r5, r5, r3
	ldr r3, [sp, #72]
	lsls r5, r5, #16
	adds r5, r5, r3
	movs r3, #240
	lsls r3, r3, #7
	adds r5, r5, r4
	adds r3, #40
	str r5, [r7, #8]
	add r3, r11
	ldr r0, [r3]
	ldr r1, [sp, #28]
	ldr r2, [sp, #40]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
	movs r7, #128
	ldr r0, [sp, #28]
	lsls r7, r7, #16
	adds r6, r6, r7
	str r6, [r0]
	mov r1, r8
	ldr r3, [r1]
	mov r4, r10
	ldrsb r2, [r4, r3]
	ldr r7, [sp, #72]
	b .L_0817fab8
	.2byte 0x0000
.L_0817fa8c:
	.4byte Data_08199582
.L_0817fa90:
	.4byte 0xfff00000
.L_0817fa94:
	.4byte 0x0001fffe
.L_0817fa98:
	.4byte 0xfff80000
.L_0817fa9c:
	.4byte 0xffff8000
.L_0817faa0:
	.4byte Data_02014000
.L_0817faa4:
	.4byte 0xffec0000
.L_0817faa8:
	.4byte Data_081994f0
.L_0817faac:
	.4byte 0xffffe000
.L_0817fab0:
	.4byte 0xfffffdbe
.L_0817fab4:
	.4byte 0xfffffdc5
.L_0817fab8:
	movs r3, #104
	ldr r0, [sp, #64]
	subs r3, r3, r2
	ldr r1, [sp, #28]
	lsls r3, r3, #16
	adds r3, r3, r7
	adds r3, r3, r0
	str r3, [r1, #8]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #44
	add r3, r11
	ldr r0, [r3]
	ldr r2, [sp, #40]
	movs r3, #0
	ldr r1, [sp, #28]
	bl Render_ApplyProjectedPlacementFar
	movs r3, #128
	ldr r2, [sp, #140]
	movs r7, #154
	lsls r3, r3, #2
	lsls r7, r7, #2
	adds r3, #90
	add r7, r11
	cmp r2, r3
	ble .L_0817faf4
	ldr r0, .L_0817fd6c
	bl Func_0815f0a0
.L_0817faf4:
	movs r0, #128
	ldr r4, [sp, #140]
	lsls r0, r0, #2
	adds r0, #58
	cmp r4, r0
	bne .L_0817fba0
	movs r3, #152
	lsls r3, r3, #16
	str r3, [r7]
	mov r1, r8
	ldr r3, [r1]
	mov r4, r10
	ldrsb r2, [r4, r3]
	ldr r0, [sp, #72]
	movs r3, #92
	subs r3, r3, r2
	lsls r3, r3, #16
	adds r3, r3, r0
	str r3, [r7, #4]
	movs r3, #1
	str r3, [r7, #24]
	ldr r3, .L_0817fd70
	ldr r5, .L_0817fd74
	str r3, [r7, #12]
	ldr r3, .L_0817fd78
	movs r1, #0
	str r3, [r7, #16]
	mov r8, r1
	movs r6, #31
.L_0817fb2e:
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #56
	lsls r3, r3, #16
	str r3, [r5]
	bl Random16
	ands r0, r6
	adds r0, #64
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	ands r0, r6
	adds r0, #8
	negs r0, r0
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	adds r0, #8
	movs r2, #1
	movs r3, #0
	negs r0, r0
	add r8, r2
	str r3, [r5, #24]
	lsls r0, r0, #12
	mov r3, r8
	str r0, [r5, #16]
	adds r5, #28
	cmp r3, #16
	bne .L_0817fb2e
	ldr r3, .L_0817fd7c
	movs r4, #0
	mov r8, r4
	subs r2, #2
.L_0817fb7c:
	movs r0, #1
	add r8, r0
	mov r1, r8
	str r2, [r3]
	adds r3, #28
	cmp r1, #32
	bne .L_0817fb7c
	ldr r3, .L_0817fd80
	movs r2, #0
	mov r8, r2
	subs r2, #1
.L_0817fb92:
	movs r4, #1
	add r8, r4
	mov r0, r8
	str r2, [r3]
	adds r3, #28
	cmp r0, #32
	bne .L_0817fb92
.L_0817fba0:
	ldr r1, [r7, #24]
	cmp r1, #0
	bne .L_0817fba8
	b .L_0817fe98
.L_0817fba8:
	cmp r1, #9
	ble .L_0817fc1e
	movs r3, #31
	ands r3, r1
	lsls r5, r3, #3
	ldr r1, .L_0817fd84
	movs r6, #240
	subs r5, r5, r3
	lsls r6, r6, #7
	lsls r5, r5, #2
	adds r6, #120
	adds r5, r5, r1
	add r6, r11
	movs r3, #0
	str r3, [r5, #24]
	ldr r3, [r6]
	ldrh r0, [r3, #18]
	bl Trig_Sin
	lsls r0, r0, #1
	negs r0, r0
	ldr r3, [r6]
	str r0, [r5, #12]
	ldrh r0, [r3, #18]
	bl Trig_Cos
	lsls r0, r0, #1
	ldr r3, [r6]
	str r0, [r5, #16]
	ldrh r0, [r3, #18]
	bl Trig_Sin
	ldr r2, [r7]
	ldr r3, [sp, #124]
	adds r2, r2, r3
	lsrs r3, r2, #31
	adds r2, r2, r3
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #2
	subs r3, r3, r0
	asrs r3, r3, #1
	asrs r2, r2, #1
	subs r2, r2, r3
	str r2, [r5]
	ldr r3, [r6]
	ldrh r0, [r3, #18]
	bl Trig_Cos
	lsls r3, r0, #3
	ldr r2, [r7, #4]
	subs r3, r3, r0
	lsls r3, r3, #2
	ldr r4, .L_0817fd88
	subs r3, r3, r0
	adds r2, r2, r3
	adds r2, r2, r4
	str r2, [r5, #4]
	ldr r1, [r7, #24]
.L_0817fc1e:
	adds r3, r1, #0
	subs r3, #50
	cmp r3, #9
	bls .L_0817fc2c
	subs r3, #63
	cmp r3, #86
	bhi .L_0817fc70
.L_0817fc2c:
	movs r2, #3
	ands r2, r1
	cmp r2, #0
	bne .L_0817fc70
	movs r6, #31
	adds r3, r6, #0
	ands r3, r1
	lsls r5, r3, #3
	ldr r0, .L_0817fd8c
	subs r5, r5, r3
	lsls r5, r5, #2
	adds r5, r5, r0
	str r2, [r5, #24]
	bl Random16
	ands r0, r6
	adds r0, #16
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl Random16
	movs r3, #16
	negs r3, r3
	negs r0, r0
	orrs r0, r3
	movs r3, #176
	lsls r3, r3, #15
	str r3, [r5]
	movs r3, #128
	lsls r0, r0, #14
	lsls r3, r3, #16
	str r0, [r5, #16]
	str r3, [r5, #4]
	ldr r1, [r7, #24]
.L_0817fc70:
	ldr r2, [sp, #124]
	movs r3, #154
	str r2, [sp, #120]
	lsls r3, r3, #2
	add r3, r11
	ldr r2, [r3, #24]
	cmp r2, #229
	ble .L_0817fc8c
	ldr r3, [r3]
	subs r2, #230
	lsls r2, r2, #17
	ldr r4, [sp, #124]
	adds r3, r3, r2
	b .L_0817fc94
.L_0817fc8c:
	cmp r2, #0
	ble .L_0817fcb2
	ldr r3, [r3]
	ldr r4, [sp, #124]
.L_0817fc94:
	negs r3, r3
	movs r0, #152
	subs r2, r3, r4
	lsls r0, r0, #16
	adds r3, r2, r0
	cmp r3, #0
	bge .L_0817fcaa
	movs r4, #152
	lsls r4, r4, #16
	adds r4, #3
	adds r3, r2, r4
.L_0817fcaa:
	ldr r0, [sp, #124]
	asrs r3, r3, #2
	adds r0, r0, r3
	str r0, [sp, #124]
.L_0817fcb2:
	cmp r1, #1
	ble .L_0817fcc8
	ldr r3, [r7]
	ldr r2, [r7, #12]
	ldr r1, [r7, #24]
	adds r3, r3, r2
	str r3, [r7]
	ldr r2, [r7, #16]
	ldr r3, [r7, #4]
	adds r3, r3, r2
	str r3, [r7, #4]
.L_0817fcc8:
	cmp r1, #7
	ble .L_0817fcf6
	ldr r2, [r7, #12]
	lsls r3, r2, #6
	subs r3, r3, r2
	cmp r3, #0
	bge .L_0817fcd8
	adds r3, #63
.L_0817fcd8:
	ldr r2, [r7, #16]
	asrs r3, r3, #6
	str r3, [r7, #12]
	lsls r3, r2, #6
	subs r3, r3, r2
	cmp r3, #0
	bge .L_0817fce8
	adds r3, #63
.L_0817fce8:
	movs r1, #128
	lsls r1, r1, #4
	adds r1, #200
	asrs r3, r3, #6
	adds r3, r3, r1
	str r3, [r7, #16]
	ldr r1, [r7, #24]
.L_0817fcf6:
	cmp r1, #79
	bgt .L_0817fd06
	ldr r3, [r7, #16]
	movs r2, #128
	lsls r2, r2, #2
	adds r3, r3, r2
	str r3, [r7, #16]
	ldr r1, [r7, #24]
.L_0817fd06:
	cmp r1, #29
	ble .L_0817fd14
	ldr r3, [r7, #12]
	ldr r4, .L_0817fd90
	ldr r1, [r7, #24]
	adds r3, r3, r4
	str r3, [r7, #12]
.L_0817fd14:
	adds r3, r1, #0
	subs r3, #40
	cmp r3, #27
	bhi .L_0817fd26
	ldr r3, [r7, #16]
	ldr r0, .L_0817fd94
	ldr r1, [r7, #24]
	adds r3, r3, r0
	str r3, [r7, #16]
.L_0817fd26:
	adds r3, r1, #0
	subs r3, #50
	cmp r3, #9
	bhi .L_0817fd4a
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #120
	add r3, r11
	ldr r2, [r3]
	lsls r3, r1, #7
	ldr r1, .L_0817fd98
	adds r3, r3, r1
	strh r3, [r2, #18]
	ldr r3, [r7, #16]
	ldr r2, .L_0817fd94
	ldr r1, [r7, #24]
	adds r3, r3, r2
	str r3, [r7, #16]
.L_0817fd4a:
	adds r3, r1, #0
	subs r3, #108
	cmp r3, #31
	bhi .L_0817fdbe
	subs r3, #12
	cmp r3, #2
	bhi .L_0817fda0
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #120
	add r3, r11
	ldr r2, [r3]
	ldr r4, .L_0817fd9c
	ldrh r3, [r2, #18]
	adds r3, r3, r4
	b .L_0817fdb2
	.2byte 0x0000
.L_0817fd6c:
	.4byte 0x00000148
.L_0817fd70:
	.4byte 0xfffc0000
.L_0817fd74:
	.4byte Data_02016220
.L_0817fd78:
	.4byte 0xffffb000
.L_0817fd7c:
	.4byte Data_02016bd8
.L_0817fd80:
	.4byte Data_02016f58
.L_0817fd84:
	.4byte Data_02016bc0
.L_0817fd88:
	.4byte 0xfff40000
.L_0817fd8c:
	.4byte Data_02016f40
.L_0817fd90:
	.4byte 0xffffc000
.L_0817fd94:
	.4byte 0xfffff000
.L_0817fd98:
	.4byte 0xffffa700
.L_0817fd9c:
	.4byte 0xfffffe80
.L_0817fda0:
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #120
	add r3, r11
	ldr r2, [r3]
	movs r0, #128
	ldrh r3, [r2, #18]
	lsls r0, r0, #1
	adds r3, r3, r0
.L_0817fdb2:
	strh r3, [r2, #18]
	ldr r3, [r7, #16]
	ldr r1, .L_0817fed8
	adds r3, r3, r1
	str r3, [r7, #16]
	ldr r1, [r7, #24]
.L_0817fdbe:
	adds r3, r1, #0
	subs r3, #140
	cmp r3, #19
	bhi .L_0817fdd0
	ldr r3, [r7, #16]
	ldr r2, .L_0817fedc
	ldr r1, [r7, #24]
	adds r3, r3, r2
	str r3, [r7, #16]
.L_0817fdd0:
	cmp r1, #159
	ble .L_0817fddc
	ldr r3, [r7, #16]
	ldr r4, .L_0817fee0
	adds r3, r3, r4
	str r3, [r7, #16]
.L_0817fddc:
	ldr r5, .L_0817fee4
	movs r0, #0
	mov r8, r0
.L_0817fde2:
	ldr r3, [r5, #24]
	cmp r3, #5
	bhi .L_0817fe24
	lsls r1, r3, #3
	adds r1, r1, r3
	lsls r1, r1, #5
	movs r2, #224
	lsls r2, r2, #3
	movs r0, #12
	add r1, r11
	adds r1, r1, r2
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r4, #6
	ldrsh r3, [r5, r4]
	str r0, [sp, #0]
	movs r0, #24
	str r0, [sp, #4]
	ldr r0, [sp, #84]
	subs r3, #12
	ldr r4, [r0, #4]
	subs r2, #2
	ldr r0, [sp, #144]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #64
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_0817fe24:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r5, #28
	cmp r2, #32
	bne .L_0817fde2
	ldr r5, .L_0817fee8
	movs r3, #0
	mov r8, r3
.L_0817fe36:
	ldr r1, [r5, #24]
	cmp r1, #23
	bhi .L_0817fe86
	cmp r1, #0
	bge .L_0817fe42
	adds r1, #3
.L_0817fe42:
	asrs r1, r1, #2
	lsls r1, r1, #11
	movs r4, #220
	movs r0, #2
	ldrsh r2, [r5, r0]
	lsls r4, r4, #4
	movs r0, #32
	add r1, r11
	adds r1, r1, r4
	movs r4, #6
	ldrsh r3, [r5, r4]
	str r0, [sp, #0]
	movs r0, #64
	str r0, [sp, #4]
	ldr r0, [sp, #84]
	subs r2, #16
	ldr r4, [r0, #4]
	subs r3, #32
	ldr r0, [sp, #144]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5, #12]
	movs r1, #128
	lsls r1, r1, #5
	adds r3, r3, r1
	str r3, [r5, #12]
	adds r0, r5, #0
	movs r1, #64
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_0817fe86:
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r5, #28
	cmp r3, #32
	bne .L_0817fe36
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_0817fe98:
	ldr r3, .L_0817feec
	ldr r4, [sp, #40]
	str r3, [sp, #168]
	str r3, [r4, #4]
	ldr r3, [r7, #24]
	cmp r3, #0
	bne .L_0817fef4
	ldr r7, [sp, #124]
	ldr r0, [sp, #68]
	ldr r2, [sp, #28]
	movs r1, #152
	adds r3, r7, r0
	lsls r1, r1, #16
	adds r3, r3, r1
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	add r3, r11
	ldr r3, [r3]
	ldr r2, .L_0817fef0
	ldr r4, [sp, #72]
	ldrsb r2, [r2, r3]
	movs r3, #92
	ldr r7, [sp, #64]
	subs r3, r3, r2
	lsls r3, r3, #16
	ldr r0, [sp, #28]
	adds r3, r3, r4
	adds r3, r3, r7
	str r3, [r0, #8]
	b .L_0817ff02
.L_0817fed8:
	.4byte 0xfffff000
.L_0817fedc:
	.4byte 0xfffff800
.L_0817fee0:
	.4byte 0xffffe000
.L_0817fee4:
	.4byte Data_02016bc0
.L_0817fee8:
	.4byte Data_02016f40
.L_0817feec:
	.4byte 0x000103ff
.L_0817fef0:
	.4byte Data_081994f0
.L_0817fef4:
	ldr r3, [r7]
	ldr r1, [sp, #124]
	ldr r2, [sp, #28]
	adds r3, r3, r1
	str r3, [r2]
	ldr r3, [r7, #4]
	str r3, [r2, #8]
.L_0817ff02:
	movs r5, #240
	lsls r5, r5, #7
	adds r5, #120
	add r5, r11
	ldr r2, [sp, #40]
	movs r3, #0
	ldr r0, [r5]
	ldr r1, [sp, #28]
	bl Render_ApplyProjectedPlacementFar
	ldr r2, [r5]
	movs r3, #32
	movs r6, #0
	strb r6, [r2, #22]
	strb r3, [r2, #23]
	ldr r3, [sp, #140]
	movs r4, #142
	lsls r4, r4, #2
	cmp r3, r4
	blt .L_0817ffcc
	movs r0, #128
	lsls r0, r0, #2
	bl Runtime_BumpAllocateAlternatePool
	adds r7, r0, #0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_081802e4
	ldr r3, [sp, #160]
	movs r1, #7
	ands r3, r2
	ldr r2, .L_081802e8
	orrs r3, r1
	ands r3, r2
	movs r2, #224
	adds r5, r0, #0
	lsls r2, r2, #3
	ldr r0, .L_081802ec
	orrs r3, r2
	str r3, [sp, #160]
	add r3, sp, #160
	str r0, [r3, #4]
	str r3, [r5, #16]
	ldr r3, .L_081802f0
	str r6, [r5, #4]
	str r3, [r5, #8]
	str r1, [r5]
	str r7, [r5, #12]
	ldr r2, .L_081802f4
	ldr r1, [sp, #140]
	movs r3, #96
	adds r6, r1, r2
	lsls r2, r6, #4
	subs r3, r3, r2
	cmp r3, #0
	ble .L_0817ff76
	movs r3, #0
.L_0817ff76:
	movs r4, #63
	negs r4, r4
	cmp r3, r4
	blt .L_0817ffc0
	str r3, [r5, #20]
	bl Func_08014de4
	ldr r1, [sp, #124]
	movs r2, #128
	lsls r2, r2, #10
	adds r0, r1, r2
	movs r1, #128
	lsls r1, r1, #12
	movs r2, #0
	bl Func_08015160
	movs r0, #134
	lsls r0, r0, #7
	bl Func_080150e4
	ldr r0, .L_081802f8
	bl SceneTransform_ApplyPitch
	movs r3, #131
	lsls r3, r3, #7
	lsls r0, r6, #14
	adds r0, r0, r3
	bl Func_0801521c
	ldr r0, .L_081802fc
	adds r1, r7, #0
	movs r2, #4
	bl Func_08196958
	adds r0, r5, #0
	bl Func_08196a7c
.L_0817ffc0:
	adds r0, r5, #0
	bl Sys_Free
	adds r0, r7, #0
	bl Sys_Free
.L_0817ffcc:
	ldr r4, [sp, #140]
	movs r7, #142
	lsls r7, r7, #2
	cmp r4, r7
	bne .L_0817ffec
	movs r2, #210
	lsls r2, r2, #2
	add r2, r11
	movs r3, #0
	str r3, [r2]
	movs r2, #213
	lsls r2, r2, #2
	movs r3, #128
	add r2, r11
	lsls r3, r3, #12
	str r3, [r2]
.L_0817ffec:
	movs r1, #210
	lsls r1, r1, #2
	add r1, r11
	str r1, [sp, #60]
	movs r0, #0
	mov r10, r0
.L_0817fff8:
	ldr r2, .L_08180300
	mov r4, r10
	lsls r3, r4, #1
	ldrh r2, [r2, r3]
	ldr r7, [sp, #140]
	cmp r7, r2
	blt .L_081800a0
	adds r3, r2, #0
	adds r3, #26
	cmp r7, r3
	bge .L_081800a0
	ldr r0, [sp, #60]
	ldr r4, [sp, #140]
	ldr r1, [r0]
	subs r3, r4, r2
	movs r0, #213
	lsrs r2, r3, #31
	lsls r0, r0, #2
	adds r3, r3, r2
	add r0, r11
	asrs r4, r3, #1
	ldr r3, [r0]
	ldr r2, [sp, #60]
	asrs r7, r1, #16
	adds r1, r1, r3
	str r1, [r2]
	ldr r2, [r0]
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #4
	cmp r3, #0
	bge .L_0818003a
	adds r3, #63
.L_0818003a:
	asrs r3, r3, #6
	str r3, [r0]
	cmp r4, #5
	bgt .L_081800a0
	movs r3, #0
	mov r8, r3
	lsls r3, r4, #3
	adds r3, r3, r4
	lsls r3, r3, #5
	add r3, r11
	mov r9, r3
.L_08180050:
	mov r4, r8
	lsls r6, r4, #11
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, r7, #0
	muls r5, r0
	lsrs r3, r5, #31
	adds r5, r5, r3
	ldr r3, .L_08180304
	mov r0, r10
	ldrsb r3, [r3, r0]
	asrs r5, r5, #17
	adds r0, r6, #0
	adds r5, r5, r3
	bl Trig_Cos
	adds r3, r7, #0
	muls r3, r0
	movs r2, #12
	lsls r3, r3, #1
	movs r1, #224
	str r2, [sp, #0]
	asrs r3, r3, #16
	movs r2, #24
	subs r5, #6
	lsls r1, r1, #3
	str r2, [sp, #4]
	add r1, r9
	adds r2, r5, #0
	adds r3, #60
	ldr r4, [sp, #176]
	ldr r0, [sp, #144]
	mov lr, r4
	.2byte 0xf800
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #32
	bne .L_08180050
.L_081800a0:
	movs r3, #1
	add r10, r3
	mov r4, r10
	cmp r4, #1
	bne .L_0817fff8
	ldr r7, [sp, #40]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [sp, #168]
	str r3, [r7, #4]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #48
	add r3, r11
	ldr r0, [r3]
	movs r4, #13
	ldrb r2, [r0, #9]
	negs r4, r4
	adds r3, r4, #0
	ands r3, r2
	movs r2, #240
	lsls r2, r2, #7
	movs r1, #8
	adds r2, #52
	orrs r3, r1
	add r2, r11
	strb r3, [r0, #9]
	str r2, [sp, #56]
	adds r3, r4, #0
	ldr r1, [r2]
	movs r7, #8
	ldrb r2, [r1, #9]
	movs r6, #104
	ands r3, r2
	orrs r3, r7
	strb r3, [r1, #9]
	movs r1, #240
	lsls r1, r1, #7
	adds r1, #56
	add r1, r11
	str r1, [sp, #52]
	adds r3, r4, #0
	ldr r1, [r1]
	str r4, [sp, #8]
	ldrb r2, [r1, #9]
	movs r5, #136
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r1, #9]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #60
	add r3, r11
	str r3, [sp, #48]
	ldr r1, [r3]
	adds r3, r4, #0
	ldrb r2, [r1, #9]
	ands r3, r2
	orrs r3, r7
	strb r3, [r1, #9]
	ldr r2, [sp, #68]
	ldr r1, [sp, #120]
	ldr r3, [sp, #28]
	adds r1, r1, r2
	movs r7, #160
	mov r8, r1
	lsls r7, r7, #16
	add r7, r8
	movs r2, #238
	str r7, [r3]
	lsls r2, r2, #7
	adds r2, #168
	add r2, r11
	ldr r1, .L_08180308
	ldr r3, [r2]
	mov r9, r1
	ldrsb r3, [r1, r3]
	ldr r1, [sp, #72]
	subs r3, r6, r3
	mov r10, r2
	lsls r3, r3, #16
	ldr r2, [sp, #64]
	adds r3, r3, r1
	ldr r1, [sp, #28]
	adds r3, r3, r2
	str r3, [r1, #8]
	ldr r1, [sp, #28]
	ldr r2, [sp, #40]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
	movs r2, #128
	ldr r3, [sp, #28]
	lsls r2, r2, #16
	add r2, r8
	str r2, [sp, #44]
	str r2, [r3]
	mov r0, r10
	ldr r3, [r0]
	mov r1, r9
	ldrsb r3, [r1, r3]
	ldr r2, [sp, #72]
	ldr r0, [sp, #64]
	subs r3, r5, r3
	ldr r1, [sp, #28]
	lsls r3, r3, #16
	adds r3, r3, r2
	adds r3, r3, r0
	str r3, [r1, #8]
	ldr r2, [sp, #56]
	ldr r1, [sp, #28]
	ldr r0, [r2]
	movs r3, #0
	ldr r2, [sp, #40]
	bl Render_ApplyProjectedPlacementFar
	ldr r3, [sp, #28]
	mov r0, r9
	str r7, [r3]
	mov r7, r10
	ldr r3, [r7]
	ldr r1, [sp, #72]
	ldrsb r3, [r0, r3]
	ldr r2, [sp, #64]
	subs r3, r5, r3
	ldr r7, [sp, #28]
	lsls r3, r3, #16
	adds r3, r3, r1
	adds r3, r3, r2
	str r3, [r7, #8]
	ldr r1, [sp, #52]
	ldr r2, [sp, #40]
	ldr r0, [r1]
	movs r3, #0
	ldr r1, [sp, #28]
	bl Render_ApplyProjectedPlacementFar
	movs r2, #192
	lsls r2, r2, #16
	add r8, r2
	mov r3, r8
	str r3, [r7]
	mov r7, r10
	ldr r3, [r7]
	mov r0, r9
	ldrsb r3, [r0, r3]
	ldr r1, [sp, #72]
	ldr r2, [sp, #64]
	subs r5, r5, r3
	lsls r5, r5, #16
	ldr r3, [sp, #28]
	adds r5, r5, r1
	adds r5, r5, r2
	str r5, [r3, #8]
	ldr r7, [sp, #48]
	ldr r1, [sp, #28]
	ldr r0, [r7]
	ldr r2, [sp, #40]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #64
	add r3, r11
	ldr r0, [r3]
	ldr r4, [sp, #8]
	ldrb r3, [r0, #9]
	movs r1, #8
	ands r4, r3
	orrs r4, r1
	strb r4, [r0, #9]
	ldr r2, [sp, #44]
	ldr r3, [sp, #28]
	mov r4, r10
	str r2, [r3]
	mov r7, r9
	ldr r3, [r4]
	ldr r1, [sp, #72]
	ldrsb r3, [r7, r3]
	ldr r2, [sp, #64]
	subs r6, r6, r3
	lsls r6, r6, #16
	ldr r3, [sp, #28]
	adds r6, r6, r1
	adds r6, r6, r2
	str r6, [r3, #8]
	ldr r1, [sp, #28]
	ldr r2, [sp, #40]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
	movs r6, #240
	lsls r6, r6, #7
	ldr r5, [sp, #28]
	ldr r7, .L_0818030c
	movs r4, #0
	adds r6, #68
	mov r8, r4
	add r6, r11
	mov r4, r9
.L_08180234:
	ldrb r3, [r7]
	ldr r0, [sp, #76]
	ldr r1, [sp, #120]
	ldr r2, [sp, #68]
	lsls r3, r3, #16
	adds r3, r3, r0
	adds r3, r3, r1
	movs r0, #176
	adds r3, r3, r2
	lsls r0, r0, #16
	adds r3, r3, r0
	movs r2, #238
	str r3, [r5]
	lsls r2, r2, #7
	adds r2, #168
	add r2, r11
	ldr r2, [r2]
	ldrb r3, [r7, #1]
	ldrsb r2, [r4, r2]
	ldr r1, [sp, #72]
	subs r3, r3, r2
	ldr r2, [sp, #64]
	lsls r3, r3, #16
	adds r3, r3, r1
	movs r0, #168
	adds r3, r3, r2
	lsls r0, r0, #15
	adds r3, r3, r0
	str r3, [r5, #8]
	ldmia r6!, {r0}
	movs r1, #13
	ldrb r3, [r0, #9]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r0, #9]
	ldr r2, [sp, #40]
	movs r3, #0
	adds r1, r5, #0
	str r4, [sp, #8]
	bl Render_ApplyProjectedPlacementFar
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r7, #2
	ldr r4, [sp, #8]
	cmp r3, #5
	bne .L_08180234
	ldr r4, [sp, #140]
	movs r7, #234
	adds r7, #255
	cmp r4, r7
	bgt .L_08180310
	movs r6, #241
	movs r5, #224
	ldr r7, [sp, #28]
	movs r0, #0
	lsls r6, r6, #7
	lsls r5, r5, #2
	mov r8, r0
	add r6, r11
	add r5, r11
.L_081802b6:
	ldr r3, [r5]
	ldr r1, [sp, #124]
	adds r3, r3, r1
	str r3, [r7]
	ldr r3, [r5, #4]
	str r3, [r7, #8]
	ldr r3, [r5, #24]
	cmp r3, #0
	beq .L_081802d4
	ldr r0, [r6]
	ldr r1, [sp, #28]
	ldr r2, [sp, #40]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
.L_081802d4:
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r6, #4
	adds r5, #28
	cmp r3, #8
	bne .L_081802b6
	b .L_08180310
.L_081802e4:
	.4byte 0xffffff00
.L_081802e8:
	.4byte 0xffff00ff
.L_081802ec:
	.4byte gMapCellBuffer
.L_081802f0:
	.4byte Data_08199364
.L_081802f4:
	.4byte 0xfffffdc8
.L_081802f8:
	.4byte 0xfffff800
.L_081802fc:
	.4byte Data_08199210
.L_08180300:
	.4byte Data_0819958c
.L_08180304:
	.4byte Data_08199590
.L_08180308:
	.4byte Data_081994f0
.L_0818030c:
	.4byte Data_08199592
.L_08180310:
	ldr r4, [sp, #140]
	movs r7, #135
	lsls r7, r7, #2
	cmp r4, r7
	bge .L_081803d8
	movs r5, #147
	lsls r5, r5, #2
	add r5, r11
	ldr r3, [r5, #24]
	cmp r3, #1
	bne .L_0818035c
	ldr r1, [r5, #12]
	ldr r3, [r5]
	ldr r2, [r5, #16]
	adds r3, r3, r1
	str r3, [r5]
	ldr r3, [r5, #4]
	movs r0, #128
	adds r3, r3, r2
	str r3, [r5, #4]
	lsls r3, r1, #4
	lsls r0, r0, #6
	subs r3, r3, r1
	adds r2, r2, r0
	lsls r3, r3, #2
	str r2, [r5, #16]
	cmp r3, #0
	bge .L_0818034a
	adds r3, #63
.L_0818034a:
	asrs r3, r3, #6
	str r3, [r5, #12]
	lsls r3, r2, #6
	subs r2, r3, r2
	cmp r2, #0
	bge .L_08180358
	adds r2, #63
.L_08180358:
	asrs r3, r2, #6
	str r3, [r5, #16]
.L_0818035c:
	movs r7, #240
	lsls r7, r7, #7
	ldr r6, [sp, #28]
	ldr r4, .L_08180508
	movs r1, #0
	adds r7, #100
	mov r8, r1
	add r7, r11
.L_0818036c:
	mov r2, r8
	movs r3, #1
	ands r3, r2
	ldr r2, [r5]
	ldr r0, [sp, #124]
	lsls r3, r3, #21
	adds r3, r3, r2
	movs r1, #128
	adds r3, r3, r0
	lsls r1, r1, #16
	adds r3, r3, r1
	mov r2, r8
	str r3, [r6]
	lsrs r3, r2, #31
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r11
	ldr r2, [r2]
	add r3, r8
	ldrsb r2, [r4, r2]
	asrs r3, r3, #1
	ldr r0, [sp, #72]
	lsls r3, r3, #5
	subs r3, r3, r2
	ldr r2, [r5, #4]
	lsls r3, r3, #16
	adds r3, r3, r0
	movs r1, #208
	adds r3, r3, r2
	lsls r1, r1, #15
	adds r3, r3, r1
	str r3, [r6, #8]
	ldmia r7!, {r0}
	movs r1, #13
	ldrb r3, [r0, #9]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r0, #9]
	ldr r2, [sp, #40]
	movs r3, #0
	adds r1, r6, #0
	str r4, [sp, #8]
	bl Render_ApplyProjectedPlacementFar
	movs r2, #1
	add r8, r2
	mov r3, r8
	ldr r4, [sp, #8]
	cmp r3, #4
	bne .L_0818036c
.L_081803d8:
	ldr r4, [sp, #140]
	movs r7, #158
	lsls r7, r7, #1
	cmp r4, r7
	bgt .L_08180490
	movs r4, #140
	lsls r4, r4, #2
	add r4, r11
	ldr r3, [r4, #24]
	cmp r3, #0
	beq .L_0818043c
	adds r3, #1
	ldr r1, [r4, #12]
	str r3, [r4, #24]
	ldr r3, [r4]
	ldr r2, [r4, #16]
	adds r3, r3, r1
	str r3, [r4]
	ldr r3, [r4, #4]
	movs r0, #128
	adds r3, r3, r2
	str r3, [r4, #4]
	lsls r3, r1, #1
	lsls r0, r0, #6
	adds r3, r3, r1
	adds r2, r2, r0
	lsls r3, r3, #4
	str r2, [r4, #16]
	cmp r3, #0
	bge .L_08180416
	adds r3, #63
.L_08180416:
	asrs r3, r3, #6
	str r3, [r4, #12]
	lsls r3, r2, #6
	subs r2, r3, r2
	cmp r2, #0
	bge .L_08180424
	adds r2, #63
.L_08180424:
	asrs r3, r2, #6
	str r3, [r4, #16]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #88
	add r3, r11
	ldr r2, [r3]
	movs r1, #128
	ldrh r3, [r2, #18]
	lsls r1, r1, #1
	adds r3, r3, r1
	strh r3, [r2, #18]
.L_0818043c:
	ldr r2, [sp, #76]
	ldr r3, [r4]
	ldr r7, [sp, #124]
	ldr r1, [sp, #28]
	adds r3, r2, r3
	movs r0, #184
	lsls r0, r0, #16
	adds r3, r3, r7
	adds r3, r3, r0
	str r3, [r1]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	add r3, r11
	ldr r3, [r3]
	ldr r2, .L_08180508
	ldrsb r2, [r2, r3]
	movs r3, #92
	subs r3, r3, r2
	ldr r2, [sp, #72]
	lsls r3, r3, #16
	adds r3, r3, r2
	ldr r2, [r4, #4]
	adds r3, r3, r2
	str r3, [r1, #8]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #88
	add r3, r11
	ldr r0, [r3]
	movs r3, #13
	ldrb r2, [r0, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r0, #9]
	ldr r1, [sp, #28]
	ldr r2, [sp, #40]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
.L_08180490:
	ldr r4, [sp, #28]
	movs r3, #0
	mov r9, r4
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #252
	mov r10, r3
	movs r6, #0
	add r4, r11
.L_081804a2:
	mov r0, r10
	lsls r0, r0, #5
	ldr r5, [sp, #104]
	movs r1, #238
	lsls r3, r6, #2
	lsls r1, r1, #7
	str r0, [sp, #24]
	movs r7, #0
	add r3, r11
	adds r1, #220
	mov r8, r7
	adds r5, #64
	adds r7, r3, r1
.L_081804bc:
	ldr r0, [sp, #116]
	mov r2, r8
	lsls r3, r2, #21
	adds r3, r3, r0
	mov r1, r9
	str r3, [r1]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	add r3, r11
	ldr r2, .L_08180508
	ldr r3, [r3]
	ldr r0, [sp, #108]
	ldrsb r3, [r2, r3]
	ldr r2, [sp, #24]
	movs r1, #132
	subs r3, r2, r3
	lsls r3, r3, #16
	adds r3, r3, r0
	lsls r1, r1, #16
	adds r3, r3, r1
	mov r2, r9
	str r3, [r2, #8]
	mov r3, r8
	cmp r3, #8
	bne .L_0818050c
	ldr r3, [sp, #104]
	ldr r1, [r4]
	adds r3, #71
	adds r2, r3, #0
	cmp r3, #0
	bge .L_08180500
	ldr r2, [sp, #104]
	adds r2, #78
.L_08180500:
	asrs r2, r2, #3
	lsls r2, r2, #3
	subs r2, r3, r2
	b .L_08180532
.L_08180508:
	.4byte Data_081994f0
.L_0818050c:
	mov r1, r8
	movs r2, #238
	adds r3, r6, r1
	lsls r2, r2, #7
	adds r2, #220
	lsls r3, r3, #2
	adds r3, r3, r2
	mov r0, r11
	ldr r1, [r0, r3]
	ldr r3, [sp, #104]
	adds r2, r5, #0
	add r3, r8
	cmp r5, #0
	bge .L_0818052c
	adds r2, r3, #0
	adds r2, #71
.L_0818052c:
	asrs r2, r2, #3
	lsls r2, r2, #3
	subs r2, r5, r2
.L_08180532:
	adds r2, r6, r2
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, #24
	mov r0, r11
	ldr r2, [r0, r3]
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r2, r3
	ldr r0, .L_081807c0
	ldrh r3, [r1, #8]
	ands r3, r0
	orrs r3, r2
	strh r3, [r1, #8]
	ldr r1, [sp, #28]
	ldr r2, .L_081807c4
	ldr r3, [r1]
	cmp r3, r2
	ble .L_08180562
	ldr r0, .L_081807c8
	adds r3, r3, r0
	str r3, [r1]
.L_08180562:
	ldr r1, [sp, #28]
	ldr r2, [sp, #40]
	ldmia r7!, {r0}
	movs r3, #0
	str r4, [sp, #8]
	bl Render_ApplyProjectedPlacementFar
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r5, #1
	ldr r4, [sp, #8]
	cmp r2, #9
	bne .L_081804bc
	add r10, r1
	mov r3, r10
	adds r6, #9
	adds r4, #36
	cmp r3, #2
	bne .L_081804a2
	ldr r0, .L_081807cc
	movs r4, #0
	mov r10, r4
	movs r7, #0
	mov r9, r0
.L_08180594:
	ldr r3, .L_081807d0
	ldr r2, [sp, #140]
	ldr r4, .L_081807d4
	movs r1, #0
	adds r6, r2, r3
	lsls r3, r7, #1
	mov r8, r1
	adds r5, r3, r4
.L_081805a4:
	mov r0, r8
	movs r1, #245
	ldr r4, [sp, #140]
	lsls r2, r0, #1
	lsls r1, r1, #1
	adds r3, r2, r1
	cmp r4, r3
	blt .L_08180600
	movs r0, #251
	lsls r0, r0, #1
	adds r3, r2, r0
	cmp r4, r3
	bge .L_08180600
	ldr r4, [sp, #72]
	ldrb r3, [r5, #1]
	asrs r1, r4, #16
	adds r3, r3, r1
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #168
	add r1, r11
	ldr r1, [r1]
	mov r4, r9
	ldrsb r1, [r4, r1]
	lsrs r0, r6, #31
	adds r0, r6, r0
	asrs r0, r0, #1
	subs r3, r3, r1
	lsls r1, r0, #3
	adds r1, r1, r0
	lsls r1, r1, #5
	movs r0, #224
	ldrb r2, [r5]
	lsls r0, r0, #3
	add r1, r11
	adds r1, r1, r0
	movs r0, #12
	str r0, [sp, #0]
	movs r0, #24
	str r0, [sp, #4]
	subs r2, #6
	subs r3, #12
	ldr r4, [sp, #176]
	ldr r0, [sp, #144]
	mov lr, r4
	.2byte 0xf800
.L_08180600:
	movs r1, #1
	add r8, r1
	mov r2, r8
	subs r6, #2
	adds r5, #2
	cmp r2, #6
	bne .L_081805a4
	add r10, r1
	mov r3, r10
	adds r7, #6
	cmp r3, #4
	bne .L_08180594
	movs r4, #0
	mov r8, r4
	movs r6, #0
.L_0818061e:
	ldr r0, .L_081807d8
	movs r7, #0
	mov r10, r7
	adds r5, r6, r0
.L_08180626:
	ldr r3, [r5, #24]
	cmp r3, #5
	bhi .L_08180672
	lsls r1, r3, #3
	adds r1, r1, r3
	lsls r1, r1, #5
	movs r2, #224
	lsls r2, r2, #3
	add r1, r11
	adds r1, r1, r2
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r4, #6
	ldrsh r3, [r5, r4]
	movs r0, #12
	str r0, [sp, #0]
	movs r0, #24
	subs r2, #2
	subs r3, #12
	str r0, [sp, #4]
	ldr r4, [sp, #176]
	ldr r0, [sp, #144]
	mov lr, r4
	.2byte 0xf800
	ldr r2, [r5]
	ldr r3, [r5, #12]
	ldr r7, .L_081807dc
	adds r2, r2, r3
	str r2, [r5]
	ldr r3, [r5, #4]
	ldr r2, [r5, #16]
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r5, #24]
	adds r2, r2, r7
	adds r3, #1
	str r2, [r5, #16]
	str r3, [r5, #24]
.L_08180672:
	movs r0, #1
	add r10, r0
	mov r1, r10
	adds r5, #28
	cmp r1, #32
	bne .L_08180626
	movs r2, #224
	add r8, r0
	lsls r2, r2, #2
	mov r3, r8
	adds r6, r6, r2
	cmp r3, #16
	bne .L_0818061e
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #168
	add r4, r11
	ldr r3, [r4]
	ldr r2, .L_081807cc
	ldr r0, .L_081807e0
	ldrsb r1, [r2, r3]
	ldr r2, .L_081807e4
	adds r3, r1, #0
	adds r3, #32
	strh r3, [r0, #6]
	movs r3, #120
	subs r3, r3, r1
	str r3, [r2, #16]
	ldr r3, [r4]
	cmp r3, #0
	ble .L_081806b4
	subs r3, #1
	str r3, [r4]
.L_081806b4:
	bl Func_081434f8
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	add r3, r11
	movs r5, #1
	str r5, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #140]
	movs r7, #200
	adds r4, #1
	lsls r7, r7, #2
	str r4, [sp, #140]
	cmp r4, r7
	beq .L_081806f0
	ldr r1, .L_081807e8
	movs r2, #2
	ldr r3, [r1]
	ands r3, r2
	cmp r3, #0
	bne .L_081806f0
	ldr r3, [r1, #12]
	ands r3, r5
	cmp r3, #0
	bne .L_081806f0
	bl .L_0817f1f4
.L_081806f0:
	ldr r0, .L_081807ec
	bl Scheduler_RemoveCallback
	add r0, sp, #100
	ldr r3, .L_081807e0
	ldrh r0, [r0]
	movs r2, #0
	strh r0, [r3, #4]
	ldr r1, [sp, #132]
	movs r5, #238
	str r2, [r1, #16]
	lsls r5, r5, #7
	bl Func_0814cca8
	adds r5, #220
	movs r2, #0
	mov r8, r2
	add r5, r11
.L_08180714:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #57
	bne .L_08180714
	movs r0, #128
	bl Resource_LoadIntoFreeSlot
	ldr r7, [sp, #128]
	str r0, [r7, #84]
	bl Func_08014c4c
	movs r3, #128
	movs r1, #160
	movs r2, #128
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_081807f0
	adds r1, #160
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_081807f4
	movs r2, #160
	ldrh r3, [r3]
	lsls r2, r2, #19
	adds r2, #188
	strh r3, [r2]
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r11
	movs r3, #50
	str r3, [r2]
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #40
	movs r3, #0
	str r3, [r2]
	ldr r3, .L_081807ac
	adds r2, #40
	strh r3, [r2]
	ldr r3, .L_081807b0
	subs r2, #48
	strh r3, [r2]
	ldr r3, .L_081807b4
	subs r2, #20
	strh r3, [r2]
	ldr r3, .L_081807b8
	adds r2, #70
	strh r3, [r2]
	ldr r0, [sp, #148]
	ldr r2, .L_081807bc
	ldr r3, [r0, #20]
	adds r1, r0, #0
	lsls r3, r3, #1
	adds r3, #36
	strh r2, [r1, r3]
	adds r0, #36
	movs r1, #0
	bl BattleActor_SpawnObjectsForListFar
	movs r1, #204
	lsls r1, r1, #1
	movs r2, #1
	b .L_081807f8
.L_081807ac:
	.4byte 0x00003f44
.L_081807b0:
	.4byte 0x00000080
.L_081807b4:
	.4byte 0x00000784
.L_081807b8:
	.4byte 0x00001010
.L_081807bc:
	.4byte 0x000000ff
.L_081807c0:
	.4byte 0xfffffc00
.L_081807c4:
	.4byte 0x00ffffff
.L_081807c8:
	.4byte 0xfee00000
.L_081807cc:
	.4byte Data_081994f0
.L_081807d0:
	.4byte 0xfffffe16
.L_081807d4:
	.4byte Data_0819959c
.L_081807d8:
	.4byte Data_02014000
.L_081807dc:
	.4byte 0xffffc000
.L_081807e0:
	.4byte Data_03001120
.L_081807e4:
	.4byte gCameraSceneParameters
.L_081807e8:
	.4byte gInput
.L_081807ec:
	.4byte Func_0813baec
.L_081807f0:
	.4byte 0x05000200
.L_081807f4:
	.4byte 0x050001e8
.L_081807f8:
	adds r1, #255
	movs r0, #1
	bl Func_08152404
	movs r5, #238
	ldr r6, .L_08180a6c
	movs r2, #0
	lsls r5, r5, #7
	mov r8, r2
	adds r5, #224
.L_0818080c:
	movs r1, #16
	ldr r2, .L_08180a70
	movs r3, #0
	movs r0, #32
	bl Func_0815b3b0
	mov r3, r11
	str r0, [r5, r3]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #220
	add r3, r11
	ldr r1, [r3]
	movs r2, #24
	mov lr, r6
	.2byte 0xf800
	mov r4, r11
	ldr r2, [r5, r4]
	movs r7, #1
	movs r3, #32
	add r8, r7
	strb r3, [r2, #22]
	mov r0, r8
	movs r3, #8
	strb r3, [r2, #23]
	adds r5, #4
	cmp r0, #15
	bne .L_0818080c
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_08180a74
	add r1, r11
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r1, #220
	lsls r1, r1, #6
	add r1, r11
	movs r2, #0
	movs r3, #0
	ldr r0, .L_08180a78
	bl Func_08157cf4
	ldr r0, .L_08180a7c
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	movs r2, #128
	ldr r3, .L_08180a6c
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	mov r2, sp
	movs r1, #0
	adds r2, #192
	str r1, [sp, #140]
	str r2, [sp, #32]
.L_08180882:
	ldr r3, [sp, #140]
	cmp r3, #0
	bne .L_08180934
	movs r6, #238
	lsls r6, r6, #7
	movs r4, #0
	adds r6, #220
	movs r0, #0
	mov r8, r4
	movs r7, #0
	add r6, r11
	mov r10, r0
	mov r5, r11
.L_0818089c:
	str r7, [r5, #24]
	bl Random16
	str r7, [r5, #20]
	mov r2, r10
	ldr r3, [r5, #20]
	lsls r0, r3, #2
	adds r0, r0, r3
	ldr r3, .L_08180a80
	lsls r0, r0, #3
	subs r0, r2, r0
	adds r0, r0, r3
	str r0, [r5, #8]
	bl Trig_Cos
	negs r0, r0
	lsls r0, r0, #3
	str r0, [r5, #12]
	ldr r0, [r5, #8]
	bl Trig_Sin
	negs r0, r0
	lsls r0, r0, #3
	str r0, [r5, #16]
	bl Random16
	movs r1, #80
	bl Math_ModU
	ldr r2, [r5, #12]
	adds r0, #80
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #3
	lsls r0, r0, #16
	subs r0, r0, r3
	str r0, [r5]
	bl Random16
	ldr r2, [r5, #16]
	movs r1, #7
	ands r1, r0
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r1, #96
	lsls r3, r3, #3
	lsls r1, r1, #16
	subs r1, r1, r3
	ldmia r6!, {r2}
	ldr r3, [r5, #8]
	ldr r4, .L_08180a84
	movs r0, #1
	add r8, r0
	str r1, [r5, #4]
	strh r3, [r2, #18]
	mov r1, r8
	movs r3, #8
	strb r7, [r2, #22]
	strb r3, [r2, #23]
	add r10, r4
	adds r5, #28
	cmp r1, #16
	bne .L_0818089c
	movs r2, #0
	ldr r3, .L_08180a88
	mov r8, r2
	movs r1, #1
	movs r2, #128
	negs r1, r1
	lsls r2, r2, #2
.L_08180928:
	movs r4, #1
	add r8, r4
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_08180928
.L_08180934:
	ldr r3, .L_08180a8c
	ldr r7, [sp, #32]
	ldr r4, [r3, #4]
	ldr r3, [r3]
	movs r1, #238
	str r3, [sp, #152]
	str r4, [sp, #156]
	movs r3, #0
	str r3, [r7, #12]
	movs r3, #255
	movs r0, #0
	lsls r3, r3, #16
	lsls r1, r1, #7
	str r3, [r7, #4]
	adds r1, #220
	str r0, [sp, #20]
	str r0, [sp, #16]
	add r1, r11
	mov r8, r0
	mov r4, r11
	mov r9, r1
.L_0818095e:
	ldr r3, .L_08180a90
	ldr r2, [sp, #20]
	ldr r7, [sp, #140]
	ldrh r3, [r3, r2]
	cmp r7, r3
	bge .L_0818096c
	b .L_08180b38
.L_0818096c:
	ldr r3, [r4, #24]
	cmp r3, #0
	beq .L_08180974
	b .L_08180b38
.L_08180974:
	ldr r3, [r4]
	ldr r0, [sp, #32]
	ldr r1, .L_08180a94
	str r3, [r0]
	ldr r3, [r4, #4]
	str r3, [r0, #8]
	subs r3, #1
	cmp r3, r1
	bhi .L_08180998
	mov r2, r9
	ldr r0, [r2]
	ldr r1, [sp, #32]
	add r2, sp, #152
	movs r3, #0
	str r4, [sp, #8]
	bl Render_ApplyProjectedPlacementFar
	ldr r4, [sp, #8]
.L_08180998:
	ldr r3, [r4, #4]
	ldr r7, .L_08180a98
	cmp r3, r7
	bgt .L_081809a2
	b .L_08180aa0
.L_081809a2:
	movs r3, #1
	str r3, [r4, #24]
	ldr r1, [sp, #16]
	ldr r2, .L_08180a9c
	movs r0, #0
	mov r10, r0
	adds r7, r1, r2
.L_081809b0:
	str r4, [sp, #8]
	bl Random16
	movs r6, #31
	ands r6, r0
	bl Random16
	movs r5, #254
	ldr r4, [sp, #8]
	lsls r5, r5, #7
	adds r5, #255
	movs r3, #128
	ands r5, r0
	lsls r3, r3, #7
	adds r5, r5, r3
	ldr r3, [r4]
	adds r0, r5, #0
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7]
	adds r6, #32
	ldr r3, [r4, #4]
	str r3, [r7, #4]
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #4
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	movs r0, #1
	asrs r3, r3, #4
	add r10, r0
	str r3, [r7, #16]
	mov r1, r10
	movs r3, #0
	str r3, [r7, #24]
	ldr r4, [sp, #8]
	adds r7, #28
	cmp r1, #2
	bne .L_081809b0
	movs r0, #144
	bl Audio_PlayCue
	ldr r7, [sp, #148]
	movs r2, #0
	ldr r3, [r7, #20]
	mov r10, r2
	ldr r4, [sp, #8]
	cmp r3, #0
	beq .L_08180ae2
	movs r5, #36
.L_08180a22:
	ldr r1, [sp, #148]
	movs r3, #128
	lsls r3, r3, #10
	ldrsh r0, [r5, r1]
	str r3, [sp, #0]
	movs r3, #100
	str r3, [sp, #4]
	movs r1, #1
	movs r2, #0
	movs r3, #0
	str r4, [sp, #8]
	bl Func_0815f000
	ldr r3, [sp, #148]
	movs r1, #7
	ldrsh r0, [r5, r3]
	movs r3, #7
	str r3, [sp, #0]
	movs r2, #5
	mov r3, r10
	bl Func_0814cd48
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	movs r3, #4
	add r2, r11
	str r3, [r2]
	ldr r1, [sp, #148]
	movs r0, #1
	ldr r3, [r1, #20]
	add r10, r0
	adds r5, #2
	ldr r4, [sp, #8]
	cmp r10, r3
	bne .L_08180a22
	b .L_08180ae2
.L_08180a6c:
	.4byte IwramCopyWords
.L_08180a70:
	.4byte 0x80006000
.L_08180a74:
	.4byte 0x0000013e
.L_08180a78:
	.4byte 0x000000ba
.L_08180a7c:
	.4byte 0x00000148
.L_08180a80:
	.4byte 0xfffff800
.L_08180a84:
	.4byte 0xffffff00
.L_08180a88:
	.4byte Data_02014018
.L_08180a8c:
	.4byte Data_08196ee0
.L_08180a90:
	.4byte Data_081995cc
.L_08180a94:
	.4byte 0x007ffffe
.L_08180a98:
	.4byte 0x006fffff
.L_08180a9c:
	.4byte Data_02014000
.L_08180aa0:
	ldr r3, .L_08180b64
	ldr r2, [sp, #20]
	ldr r7, [sp, #140]
	ldrh r3, [r3, r2]
	subs r1, r7, r3
	adds r3, r1, #0
	cmp r1, #0
	bge .L_08180ab2
	adds r3, #15
.L_08180ab2:
	asrs r2, r3, #4
	lsls r3, r2, #4
	mov r0, r8
	subs r2, r1, r3
	lsls r3, r0, #4
	adds r3, r3, r2
	lsls r2, r3, #3
	subs r2, r2, r3
	ldr r1, .L_08180b68
	ldr r3, [r4]
	lsls r2, r2, #2
	adds r2, r2, r1
	lsrs r1, r3, #31
	adds r3, r3, r1
	asrs r3, r3, #1
	str r3, [r2]
	ldr r7, .L_08180b6c
	ldr r3, [r4, #4]
	adds r3, r3, r7
	str r3, [r2, #4]
	movs r3, #0
	str r3, [r2, #12]
	str r3, [r2, #16]
	str r3, [r2, #24]
.L_08180ae2:
	mov r0, r9
	ldr r3, [r4, #8]
	ldr r2, [r0]
	movs r1, #64
	adds r0, r4, #0
	strh r3, [r2, #18]
	movs r2, #0
	str r4, [sp, #8]
	bl BattleFxKernels_IntegrateVector3
	mov r1, r9
	ldr r3, [r1]
	mov r7, r11
	ldrh r0, [r3, #18]
	bl Trig_Cos
	ldr r4, [sp, #8]
	negs r0, r0
	mov r2, r9
	lsls r0, r0, #3
	ldr r3, [r2]
	str r0, [r4, #12]
	ldrh r0, [r3, #18]
	bl Trig_Sin
	movs r1, #238
	ldr r4, [sp, #8]
	negs r0, r0
	lsls r1, r1, #7
	lsls r0, r0, #3
	mov r3, r9
	adds r1, #220
	str r0, [r4, #16]
	ldr r0, [r3]
	ldr r3, [r7, r1]
	ldr r1, .L_08180b60
	ldrh r2, [r3, #8]
	ldrh r3, [r0, #8]
	lsls r2, r2, #22
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #8]
.L_08180b38:
	ldr r3, [sp, #20]
	ldr r7, [sp, #16]
	movs r0, #1
	add r8, r0
	movs r2, #4
	adds r3, #2
	adds r7, #224
	mov r1, r8
	add r9, r2
	str r3, [sp, #20]
	adds r4, #28
	str r7, [sp, #16]
	cmp r1, #16
	beq .L_08180b56
	b .L_0818095e
.L_08180b56:
	ldr r5, .L_08180b68
	movs r2, #0
	mov r8, r2
	b .L_08180b70
	.2byte 0x0000
.L_08180b60:
	.4byte 0xfffffc00
.L_08180b64:
	.4byte Data_081995cc
.L_08180b68:
	.4byte Data_02015c00
.L_08180b6c:
	.4byte 0xfff00000
.L_08180b70:
	ldr r3, [r5, #24]
	cmp r3, #7
	bhi .L_08180bae
	subs r2, r3, #2
	cmp r2, #0
	blt .L_08180baa
	lsls r1, r2, #3
	adds r1, r1, r2
	lsls r1, r1, #5
	movs r3, #220
	lsls r3, r3, #6
	movs r0, #12
	add r1, r11
	movs r4, #2
	ldrsh r2, [r5, r4]
	adds r1, r1, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	movs r0, #24
	str r0, [sp, #4]
	ldr r0, [sp, #84]
	subs r3, #12
	ldr r4, [r0, #4]
	subs r2, #6
	ldr r0, [sp, #144]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5, #24]
.L_08180baa:
	adds r3, #1
	str r3, [r5, #24]
.L_08180bae:
	movs r1, #1
	movs r2, #128
	add r8, r1
	lsls r2, r2, #1
	adds r5, #28
	cmp r8, r2
	bne .L_08180b70
	ldr r5, .L_08180c88
	movs r3, #0
	mov r8, r3
.L_08180bc2:
	ldr r0, [r5, #24]
	cmp r0, #17
	bhi .L_08180c10
	movs r1, #3
	bl Math_Div
	adds r1, r0, #0
	lsls r1, r1, #11
	movs r0, #224
	lsls r0, r0, #3
	add r1, r11
	adds r1, r1, r0
	mov r7, r8
	movs r0, #32
	movs r4, #1
	movs r3, #2
	ldrsh r2, [r5, r3]
	ands r4, r7
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	movs r0, #64
	str r0, [sp, #4]
	ldr r0, [sp, #84]
	lsls r4, r4, #2
	subs r3, #48
	ldr r4, [r4, r0]
	subs r2, #16
	ldr r0, [sp, #144]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #56
	ldr r2, .L_08180c8c
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_08180c10:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r5, #28
	cmp r2, #128
	bne .L_08180bc2
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	movs r3, #1
	add r2, r11
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #140]
	adds r3, #1
	str r3, [sp, #140]
	cmp r3, #109
	beq .L_08180c46
	b .L_08180882
.L_08180c46:
	movs r5, #238
	lsls r5, r5, #7
	movs r4, #0
	adds r5, #220
	mov r8, r4
	add r5, r11
.L_08180c52:
	movs r7, #1
	ldmia r5!, {r0}
	add r8, r7
	bl ResourceObject_ReleaseFar
	mov r0, r8
	cmp r0, #16
	bne .L_08180c52
	ldr r0, .L_08180c90
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #268
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08180c88:
	.4byte Data_02014000
.L_08180c8c:
	.4byte 0xffffe000
.L_08180c90:
	.4byte Func_08143000
