.syntax unified
	.thumb
	.global Func_0814db04
	.thumb_func
Func_0814db04:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #336
	str r0, [sp, #52]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #96]
	movs r3, #1
	str r0, [sp, #48]
	movs r0, #0
	ldr r1, [r5, #92]
	movs r7, #239
	str r1, [sp, #44]
	lsls r7, r7, #7
	ldr r2, [r5, #100]
	str r3, [sp, #24]
	str r2, [sp, #28]
	bl BattleFx_BeginCanvasLayer
	bl Func_0813ba50
	ldr r2, .L_0814db74
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	ldr r4, [sp, #44]
	movs r3, #0
	adds r6, r4, r7
	movs r1, #200
	str r3, [r6]
	lsls r1, r1, #4
	ldr r0, .L_0814db80
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_0814db78
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #72
	strh r3, [r2]
	movs r0, #1
	movs r1, #0
	bl Func_08163c2c
	ldr r3, .L_0814db7c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #64
	b .L_0814db84
	.2byte 0x0000
.L_0814db74:
	.4byte 0x00000000
.L_0814db78:
	.4byte 0x00002137
.L_0814db7c:
	.4byte 0x0000f0f0
.L_0814db80:
	.4byte Func_08143000
.L_0814db84:
	strh r3, [r2]
	ldr r2, [sp, #44]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0814dc34
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r2, #0
	ldr r1, [sp, #28]
	movs r3, #0
	ldr r0, .L_0814dc38
	bl Func_08157cf4
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	ldr r4, [r5, #104]
	movs r1, #35
	movs r0, #188
	str r4, [sp, #36]
	bl Func_081963ec
	adds r5, #188
	ldr r2, .L_0814dc3c
	ldr r5, [r5]
	movs r3, #240
	str r5, [sp, #32]
	str r3, [r2, #16]
	ldr r5, [sp, #44]
	movs r7, #240
	lsls r7, r7, #7
	adds r7, #240
	adds r3, r5, r7
	ldr r0, [r3]
	bl Func_0814cc4c
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_0814dc40
	movs r0, #1
	movs r2, #0
	bl Func_08118040
	movs r1, #194
	lsls r1, r1, #1
	adds r1, #255
	movs r0, #9
	movs r2, #1
	bl Func_08152404
	ldr r3, .L_0814dc24
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_0814dc28
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_0814dc2c
	adds r2, #50
	strh r3, [r2]
	ldr r3, .L_0814dc30
	movs r0, #238
	subs r2, #2
	lsls r0, r0, #7
	strh r3, [r2]
	adds r0, #132
	movs r3, #2
	str r3, [r6]
	adds r2, r5, r0
	movs r3, #50
	str r3, [r2]
	ldr r5, .L_0814dc44
	movs r1, #0
	mov r8, r1
	b .L_0814dc48
.L_0814dc24:
	.4byte 0x00007741
.L_0814dc28:
	.4byte 0x00000080
.L_0814dc2c:
	.4byte 0x00001010
.L_0814dc30:
	.4byte 0x00003f44
.L_0814dc34:
	.4byte 0x0000017d
.L_0814dc38:
	.4byte 0x0000017e
.L_0814dc3c:
	.4byte gCameraSceneParameters
.L_0814dc40:
	.4byte 0x00000045
.L_0814dc44:
	.4byte gMapCellBuffer
.L_0814dc48:
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #32
	str r3, [r5]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #120
	str r3, [r5, #4]
	movs r2, #1
	movs r3, #1
	negs r3, r3
	add r8, r2
	str r3, [r5, #24]
	mov r3, r8
	adds r5, #28
	cmp r3, #64
	bne .L_0814dc48
	ldr r1, .L_0814dcfc
	movs r4, #0
	mov r8, r4
	add r2, sp, #80
.L_0814dc7a:
	ldrb r3, [r1]
	movs r5, #1
	str r3, [r2]
	ldrb r3, [r1, #1]
	add r8, r5
	mov r6, r8
	str r3, [r2, #4]
	adds r1, #2
	adds r2, #8
	cmp r6, #32
	bne .L_0814dc7a
	movs r0, #141
	bl Audio_PlayCue
	movs r7, #0
	str r7, [sp, #40]
	ldr r3, .L_0814dd00
	mov r1, sp
	adds r1, #56
	ldr r3, [r3, #12]
	str r1, [sp, #16]
	movs r0, #16
	mov r10, r0
.L_0814dca8:
	ldr r2, [sp, #40]
	cmp r2, #15
	bhi .L_0814dd10
	ldr r7, .L_0814dd04
	cmp r2, #1
	bne .L_0814dcdc
	ldr r5, .L_0814dd08
	movs r3, #63
	adds r6, r5, #0
	adds r6, #128
.L_0814dcbc:
	str r3, [sp, #12]
	bl Random16
	ldr r3, [sp, #12]
	ands r0, r3
	strb r0, [r5]
	adds r5, #1
	cmp r5, r6
	bne .L_0814dcbc
	ldr r3, .L_0814dcf8
	movs r1, #200
	strh r3, [r7]
	ldr r0, .L_0814dd0c
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
.L_0814dcdc:
	ldr r4, [sp, #24]
	ldrh r3, [r7]
	adds r3, r3, r4
	strh r3, [r7]
	ldr r5, [sp, #40]
	adds r4, #3
	str r4, [sp, #24]
	cmp r5, #15
	bne .L_0814dd10
	ldr r0, .L_0814dd0c
	bl Scheduler_RemoveCallback
	b .L_0814dd10
	.2byte 0x0000
.L_0814dcf8:
	.4byte 0x00000000
.L_0814dcfc:
	.4byte Data_08198260
.L_0814dd00:
	.4byte gInput
.L_0814dd04:
	.4byte gMapCellBuffer
.L_0814dd08:
	.4byte Data_02010002
.L_0814dd0c:
	.4byte Func_0814cbcc
.L_0814dd10:
	ldr r6, [sp, #40]
	cmp r6, #103
	ble .L_0814dd1c
	movs r7, #0
	mov r10, r7
	b .L_0814dd32
.L_0814dd1c:
	ldr r0, [sp, #40]
	cmp r0, #63
	ble .L_0814dd28
	movs r1, #6
	mov r10, r1
	b .L_0814dd32
.L_0814dd28:
	ldr r2, [sp, #40]
	cmp r2, #31
	ble .L_0814dd32
	movs r3, #10
	mov r10, r3
.L_0814dd32:
	ldr r4, [sp, #40]
	cmp r4, #167
	bgt .L_0814dd58
	bl Random16
	movs r5, #3
	ands r0, r5
	subs r0, #1
	mov r9, r0
	bl Random16
	ldr r3, .L_0814e07c
	ands r0, r5
	subs r7, r0, #1
	mov r5, r9
	adds r0, #31
	strh r5, [r3, #4]
	strh r0, [r3, #6]
	b .L_0814dd66
.L_0814dd58:
	ldr r2, .L_0814e07c
	movs r7, #0
	movs r3, #32
	strh r7, [r2, #4]
	strh r3, [r2, #6]
	movs r6, #0
	mov r9, r6
.L_0814dd66:
	ldr r2, [sp, #40]
	subs r2, #176
	cmp r2, #3
	bhi .L_0814dd82
	ldr r3, .L_0814e080
	ldrsb r3, [r3, r2]
	ldr r2, .L_0814e07c
	negs r7, r3
	mov r9, r7
	mov r0, r9
	adds r7, r3, #0
	adds r3, #32
	strh r0, [r2, #4]
	strh r3, [r2, #6]
.L_0814dd82:
	movs r1, #0
	mov r2, r10
	mov r8, r1
	cmp r2, #0
	beq .L_0814ddd4
	ldr r3, .L_0814e084
	ldr r6, .L_0814e088
	mov r11, r3
.L_0814dd92:
	mov r0, r8
	movs r1, #3
	bl Math_Mod
	mov r4, r11
	lsls r3, r0, #1
	ldrh r1, [r4, r3]
	ldr r5, [sp, #44]
	ldr r4, .L_0814e08c
	movs r2, #224
	adds r1, r5, r1
	lsls r2, r2, #3
	adds r1, r1, r2
	ldrb r2, [r6]
	ldrb r5, [r4, r0]
	ldr r4, .L_0814e090
	mov r3, r9
	subs r2, r2, r3
	ldrb r3, [r6, #1]
	ldrb r0, [r4, r0]
	subs r3, r3, r5
	str r5, [sp, #4]
	movs r5, #1
	str r0, [sp, #0]
	subs r3, r3, r7
	ldr r0, [sp, #48]
	ldr r4, [sp, #36]
	add r8, r5
	mov lr, r4
	.2byte 0xf800
	adds r6, #2
	cmp r8, r10
	bne .L_0814dd92
.L_0814ddd4:
	ldr r3, .L_0814e094
	ldr r6, [sp, #40]
	ldr r4, [r3, #4]
	ldr r3, [r3]
	str r3, [sp, #56]
	str r4, [sp, #60]
	cmp r6, #174
	bne .L_0814de0a
	ldr r1, [sp, #44]
	movs r2, #238
	lsls r2, r2, #7
	movs r0, #0
	movs r4, #13
	adds r2, #220
	mov r8, r0
	negs r4, r4
	adds r0, r1, r2
.L_0814ddf6:
	ldmia r0!, {r1}
	adds r3, r4, #0
	ldrb r2, [r1, #9]
	ands r3, r2
	strb r3, [r1, #9]
	movs r3, #1
	add r8, r3
	mov r5, r8
	cmp r5, #9
	bne .L_0814ddf6
.L_0814de0a:
	ldr r6, [sp, #40]
	cmp r6, #208
	ble .L_0814de32
	ldr r0, [sp, #44]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #228
	adds r3, r0, r1
	ldr r0, [r3]
	ldr r1, .L_0814e098
	adds r3, r6, #0
	cmp r6, #0
	bge .L_0814de26
	adds r3, #3
.L_0814de26:
	asrs r3, r3, #2
	movs r2, #3
	ands r3, r2
	ldrb r1, [r1, r3]
	bl Animation_ApplyChildArgumentFar
.L_0814de32:
	ldr r2, [sp, #16]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [sp, #56]
	str r3, [r2, #4]
	add r2, sp, #64
	movs r3, #0
	str r3, [r2, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r2, #4]
	movs r3, #0
	str r3, [sp, #20]
	mov r11, r3
	movs r5, #152
	lsls r3, r7, #16
	negs r3, r3
	mov r6, r9
	lsls r5, r5, #15
	adds r4, r3, r5
	lsls r3, r6, #16
	negs r3, r3
	mov r10, r2
	mov r9, r3
.L_0814de62:
	ldr r1, [sp, #44]
	mov r0, r11
	movs r2, #238
	movs r5, #144
	lsls r3, r0, #2
	lsls r2, r2, #7
	lsls r5, r5, #16
	adds r3, r3, r1
	adds r2, #220
	movs r7, #0
	mov r8, r4
	add r5, r9
	adds r6, r3, r2
.L_0814de7c:
	mov r3, r10
	mov r0, r8
	str r5, [r3]
	str r0, [r3, #8]
	mov r1, r10
	ldmia r6!, {r0}
	ldr r2, [sp, #16]
	movs r3, #0
	str r4, [sp, #8]
	bl Render_ApplyProjectedPlacementFar
	movs r1, #128
	lsls r1, r1, #14
	adds r7, #1
	adds r5, r5, r1
	ldr r4, [sp, #8]
	cmp r7, #3
	bne .L_0814de7c
	ldr r3, [sp, #20]
	movs r2, #3
	adds r3, #1
	add r11, r2
	adds r4, r4, r1
	str r3, [sp, #20]
	cmp r3, #3
	bne .L_0814de62
	ldr r1, [sp, #40]
	subs r1, #160
	cmp r1, #157
	bhi .L_0814df02
	ldr r3, [sp, #40]
	ldr r5, [sp, #40]
	subs r3, #208
	movs r2, #80
	movs r4, #8
	cmp r5, #175
	bgt .L_0814ded2
	movs r3, #96
	subs r2, r3, r1
	lsls r3, r1, #2
	adds r4, r3, #0
	subs r4, #56
	b .L_0814dee6
.L_0814ded2:
	ldr r6, [sp, #40]
	cmp r6, #208
	ble .L_0814dee6
	cmp r3, #0
	bge .L_0814dee0
	adds r3, r6, #0
	subs r3, #205
.L_0814dee0:
	asrs r3, r3, #2
	adds r4, r3, #0
	adds r4, #8
.L_0814dee6:
	movs r3, #24
	str r3, [sp, #0]
	movs r3, #48
	str r3, [sp, #4]
	ldr r7, [sp, #44]
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #70
	adds r1, r7, r3
	ldr r0, [sp, #48]
	adds r3, r4, #0
	ldr r4, [sp, #32]
	mov lr, r4
	.2byte 0xf800
.L_0814df02:
	ldr r5, [sp, #40]
	cmp r5, #32
	bne .L_0814df0e
	movs r0, #134
	bl Audio_PlayCue
.L_0814df0e:
	ldr r6, [sp, #40]
	cmp r6, #64
	bne .L_0814df1a
	movs r0, #134
	bl Audio_PlayCue
.L_0814df1a:
	ldr r7, [sp, #40]
	cmp r7, #104
	bne .L_0814df26
	movs r0, #134
	bl Audio_PlayCue
.L_0814df26:
	ldr r0, [sp, #40]
	cmp r0, #176
	bne .L_0814df32
	movs r0, #134
	bl Audio_PlayCue
.L_0814df32:
	ldr r1, [sp, #40]
	cmp r1, #226
	bne .L_0814df3e
	movs r0, #145
	bl Audio_PlayCue
.L_0814df3e:
	bl Func_08014de4
	ldr r2, [sp, #40]
	cmp r2, #32
	bne .L_0814df9c
	ldr r5, .L_0814e09c
	movs r3, #0
	mov r8, r3
	movs r7, #31
	movs r6, #127
.L_0814df52:
	bl Random16
	ands r0, r7
	adds r0, #68
	lsls r0, r0, #16
	str r0, [r5]
	bl Random16
	ands r0, r7
	adds r0, #8
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #63
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	negs r0, r0
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #11
	str r0, [r5, #16]
	bl Random16
	movs r3, #15
	movs r4, #1
	ands r3, r0
	add r8, r4
	adds r3, #32
	mov r0, r8
	str r3, [r5, #24]
	adds r5, #28
	cmp r0, #32
	bne .L_0814df52
.L_0814df9c:
	ldr r1, [sp, #40]
	cmp r1, #64
	bne .L_0814dffa
	ldr r5, .L_0814e0a0
	movs r2, #0
	mov r8, r2
	movs r6, #31
.L_0814dfaa:
	bl Random16
	movs r1, #48
	bl Math_ModU
	adds r0, #60
	lsls r0, r0, #16
	str r0, [r5]
	bl Random16
	ands r0, r6
	adds r0, #52
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	movs r3, #127
	ands r3, r0
	subs r3, #63
	lsls r3, r3, #12
	str r3, [r5, #12]
	bl Random16
	negs r0, r0
	ands r0, r6
	subs r0, #32
	lsls r0, r0, #13
	str r0, [r5, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #32
	str r3, [r5, #24]
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r5, #28
	cmp r4, #32
	bne .L_0814dfaa
.L_0814dffa:
	ldr r5, [sp, #40]
	cmp r5, #104
	bne .L_0814e056
	ldr r5, .L_0814e09c
	movs r6, #0
	mov r8, r6
	movs r6, #31
.L_0814e008:
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #52
	lsls r3, r3, #16
	str r3, [r5]
	bl Random16
	ands r0, r6
	adds r0, #72
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	movs r3, #127
	ands r3, r0
	subs r3, #63
	lsls r3, r3, #11
	str r3, [r5, #12]
	bl Random16
	negs r0, r0
	ands r0, r6
	subs r0, #32
	lsls r0, r0, #13
	str r0, [r5, #16]
	bl Random16
	movs r7, #1
	movs r3, #15
	ands r3, r0
	add r8, r7
	adds r3, #32
	mov r0, r8
	str r3, [r5, #24]
	adds r5, #28
	cmp r0, #32
	bne .L_0814e008
.L_0814e056:
	ldr r3, [sp, #40]
	subs r3, #32
	cmp r3, #175
	bhi .L_0814e0fa
	ldr r5, .L_0814e09c
	movs r1, #0
	mov r8, r1
.L_0814e064:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_0814e0ee
	ldr r2, [sp, #40]
	cmp r2, #191
	ble .L_0814e0a4
	mov r0, r8
	movs r1, #7
	bl Math_Mod
	adds r4, r0, #4
	b .L_0814e0aa
.L_0814e07c:
	.4byte Data_03001120
.L_0814e080:
	.4byte Data_08198280
.L_0814e084:
	.4byte Data_08198284
.L_0814e088:
	.4byte Data_08198260
.L_0814e08c:
	.4byte Data_0819828d
.L_0814e090:
	.4byte Data_0819828a
.L_0814e094:
	.4byte Data_08196e14
.L_0814e098:
	.4byte Data_08198290
.L_0814e09c:
	.4byte gMapCellBuffer
.L_0814e0a0:
	.4byte Data_02010380
.L_0814e0a4:
	movs r4, #3
	mov r3, r8
	ands r4, r3
.L_0814e0aa:
	ldr r2, .L_0814e30c
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	movs r0, #2
	ldrsh r2, [r5, r0]
	ldr r0, .L_0814e310
	ldr r6, [sp, #44]
	ldrb r0, [r0, r4]
	adds r1, r6, r1
	movs r6, #6
	ldrsh r3, [r5, r6]
	str r0, [sp, #0]
	ldr r0, .L_0814e314
	movs r7, #224
	ldrb r0, [r0, r4]
	lsls r7, r7, #3
	adds r1, r1, r7
	str r0, [sp, #4]
	ldr r7, [sp, #32]
	ldr r0, [sp, #48]
	mov lr, r7
	.2byte 0xf800
	ldr r2, [r5]
	ldr r3, [r5, #12]
	movs r0, #128
	adds r2, r2, r3
	str r2, [r5]
	ldr r3, [r5, #4]
	ldr r2, [r5, #16]
	lsls r0, r0, #6
	adds r3, r3, r2
	adds r2, r2, r0
	str r3, [r5, #4]
	str r2, [r5, #16]
.L_0814e0ee:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r5, #28
	cmp r2, #64
	bne .L_0814e064
.L_0814e0fa:
	ldr r3, [sp, #40]
	cmp r3, #223
	bgt .L_0814e102
	b .L_0814e270
.L_0814e102:
	cmp r3, #224
	bne .L_0814e146
	ldr r5, .L_0814e318
	movs r4, #0
	mov r8, r4
	movs r6, #127
.L_0814e10e:
	movs r3, #144
	lsls r3, r3, #15
	str r3, [r5]
	movs r3, #224
	lsls r3, r3, #14
	str r3, [r5, #4]
	bl Random16
	ands r0, r6
	negs r0, r0
	subs r0, #64
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	adds r0, #16
	lsls r0, r0, #11
	str r0, [r5, #16]
	bl Random16
	movs r7, #1
	add r8, r7
	str r0, [r5, #24]
	mov r0, r8
	adds r5, #28
	cmp r0, #128
	bne .L_0814e10e
.L_0814e146:
	ldr r5, .L_0814e318
	movs r1, #0
	mov r8, r1
.L_0814e14c:
	mov r3, r8
	cmp r3, #0
	bge .L_0814e154
	adds r3, #3
.L_0814e154:
	ldr r2, [sp, #40]
	asrs r3, r3, #2
	adds r3, #224
	cmp r2, r3
	blt .L_0814e1d0
	mov r0, r8
	movs r1, #3
	bl Math_Mod
	mov r6, r8
	movs r3, #1
	ands r3, r6
	adds r4, r0, #0
	cmp r3, #0
	bne .L_0814e19e
	ldr r2, .L_0814e31c
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r7, [sp, #44]
	movs r0, #224
	adds r1, r7, r1
	lsls r0, r0, #3
	adds r1, r1, r0
	ldr r0, .L_0814e320
	movs r3, #2
	ldrsh r2, [r5, r3]
	ldrb r0, [r0, r4]
	movs r6, #6
	ldrsh r3, [r5, r6]
	str r0, [sp, #0]
	ldr r0, .L_0814e324
	ldr r7, [sp, #36]
	ldrb r0, [r0, r4]
	str r0, [sp, #4]
	ldr r0, [sp, #48]
	mov lr, r7
	.2byte 0xf800
.L_0814e19e:
	ldr r2, [r5]
	ldr r3, [r5, #12]
	ldr r1, [r5, #4]
	adds r2, r2, r3
	ldr r3, [r5, #16]
	movs r0, #16
	str r2, [r5]
	adds r1, r1, r3
	asrs r2, r2, #16
	negs r0, r0
	str r1, [r5, #4]
	cmp r2, r0
	blt .L_0814e1be
	asrs r3, r1, #16
	cmp r3, #120
	ble .L_0814e1ca
.L_0814e1be:
	movs r3, #144
	lsls r3, r3, #15
	str r3, [r5]
	movs r3, #224
	lsls r3, r3, #14
	str r3, [r5, #4]
.L_0814e1ca:
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_0814e1d0:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r5, #28
	cmp r2, #128
	bne .L_0814e14c
	ldr r3, [sp, #40]
	cmp r3, #228
	bne .L_0814e204
	ldr r2, .L_0814e328
	ldr r1, .L_0814e318
	movs r4, #0
	mov r8, r4
	movs r0, #0
.L_0814e1ec:
	ldr r3, [r1]
	movs r5, #1
	str r3, [r2]
	add r8, r5
	ldr r3, [r1, #4]
	mov r6, r8
	str r3, [r2, #4]
	str r0, [r2, #24]
	adds r1, #28
	adds r2, #28
	cmp r6, #128
	bne .L_0814e1ec
.L_0814e204:
	ldr r5, .L_0814e328
	ldr r6, .L_0814e318
	movs r7, #0
	mov r8, r7
.L_0814e20c:
	ldr r0, [sp, #40]
	mov r3, r8
	adds r3, #228
	cmp r0, r3
	blt .L_0814e262
	ldr r0, [r5, #24]
	movs r1, #9
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	bl Math_Mod
	ldr r3, .L_0814e32c
	ldr r2, [sp, #28]
	ldrb r4, [r3, r0]
	ldr r3, .L_0814e330
	lsls r0, r0, #1
	ldrh r1, [r3, r0]
	lsrs r0, r4, #1
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r7, #6
	ldrsh r3, [r5, r7]
	subs r2, r2, r0
	subs r3, r3, r0
	str r4, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #48]
	ldr r4, [sp, #36]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	cmp r3, #18
	bne .L_0814e262
	ldr r3, [r6]
	str r3, [r5]
	ldr r3, [r6, #4]
	str r3, [r5, #4]
	movs r3, #0
	str r3, [r5, #24]
.L_0814e262:
	movs r7, #1
	add r8, r7
	mov r0, r8
	adds r5, #28
	adds r6, #28
	cmp r0, #128
	bne .L_0814e20c
.L_0814e270:
	ldr r1, [sp, #44]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #40]
	movs r5, #144
	adds r4, #1
	lsls r5, r5, #1
	str r4, [sp, #40]
	cmp r4, r5
	beq .L_0814e2aa
	ldr r3, .L_0814e334
	movs r2, #3
	ldr r3, [r3, #12]
	movs r6, #16
	ands r3, r2
	mov r10, r6
	cmp r3, #0
	bne .L_0814e2a4
	b .L_0814dca8
.L_0814e2a4:
	cmp r4, #16
	bgt .L_0814e2aa
	b .L_0814dca8
.L_0814e2aa:
	ldr r0, [sp, #44]
	movs r1, #238
	lsls r1, r1, #7
	movs r7, #0
	adds r1, #220
	mov r8, r7
	adds r5, r0, r1
.L_0814e2b8:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #9
	bne .L_0814e2b8
	bl Func_0814cca8
	ldr r2, .L_0814e308
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	movs r0, #195
	strh r2, [r3]
	lsls r0, r0, #1
	bl Audio_PlayCue
	ldr r5, [sp, #44]
	movs r4, #0
	mov r8, r4
	movs r7, #127
	movs r6, #63
.L_0814e2e8:
	bl Random16
	ldr r3, .L_0814e338
	ands r0, r7
	adds r0, #64
	lsls r0, r0, #16
	str r3, [r5, #8]
	str r0, [r5]
	bl Random16
	ands r0, r7
	negs r0, r0
	subs r0, #64
	lsls r0, r0, #16
	str r0, [r5, #4]
	b .L_0814e33c
.L_0814e308:
	.4byte 0x00001010
.L_0814e30c:
	.4byte Data_081982aa
.L_0814e310:
	.4byte Data_08198294
.L_0814e314:
	.4byte Data_0819829f
.L_0814e318:
	.4byte gMapCellBuffer
.L_0814e31c:
	.4byte Data_081982c6
.L_0814e320:
	.4byte Data_081982c0
.L_0814e324:
	.4byte Data_081982c3
.L_0814e328:
	.4byte Data_02010e00
.L_0814e32c:
	.4byte Data_0819745e
.L_0814e330:
	.4byte Data_0819744c
.L_0814e334:
	.4byte gInput
.L_0814e338:
	.4byte 0xfff00000
.L_0814e33c:
	bl Random16
	ands r0, r6
	negs r0, r0
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	adds r0, #127
	lsls r0, r0, #12
	str r0, [r5, #16]
	movs r0, #1
	add r8, r0
	movs r3, #0
	mov r1, r8
	str r3, [r5, #24]
	adds r5, #28
	cmp r1, #64
	bne .L_0814e2e8
	ldr r5, .L_0814e6c0
	mov r8, r3
.L_0814e36a:
	bl Random16
	movs r3, #127
	ands r3, r0
	str r3, [r5]
	bl Random16
	mov r4, r8
	lsrs r3, r4, #31
	add r3, r8
	movs r2, #63
	asrs r3, r3, #1
	ands r2, r0
	adds r2, r2, r3
	negs r3, r4
	str r2, [r5, #4]
	movs r6, #1
	lsrs r2, r3, #31
	adds r3, r3, r2
	add r8, r6
	asrs r3, r3, #1
	mov r7, r8
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #64
	bne .L_0814e36a
	movs r0, #0
	ldr r1, .L_0814e6c4
	str r0, [sp, #40]
	mov r9, r1
.L_0814e3a6:
	ldr r3, [sp, #40]
	movs r2, #0
	mov r10, r2
	cmp r3, #96
	bne .L_0814e3b6
	movs r0, #134
	bl Func_081180e8
.L_0814e3b6:
	ldr r4, [sp, #40]
	cmp r4, #16
	bne .L_0814e3ca
	ldr r5, [sp, #44]
	movs r6, #238
	lsls r6, r6, #7
	adds r6, #168
	adds r2, r5, r6
	movs r3, #32
	str r3, [r2]
.L_0814e3ca:
	ldr r7, [sp, #40]
	cmp r7, #16
	ble .L_0814e3e4
	adds r3, r7, #0
	subs r3, #16
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	mov r10, r3
	cmp r3, #16
	ble .L_0814e3e4
	movs r0, #16
	mov r10, r0
.L_0814e3e4:
	ldr r3, [sp, #40]
	subs r3, #9
	cmp r3, #62
	bhi .L_0814e3fc
	ldr r1, [sp, #40]
	movs r3, #3
	ands r3, r1
	cmp r3, #0
	bne .L_0814e3fc
	movs r0, #132
	bl Audio_PlayCue
.L_0814e3fc:
	ldr r2, [sp, #40]
	cmp r2, #72
	bne .L_0814e408
	movs r0, #145
	bl Audio_PlayCue
.L_0814e408:
	ldr r3, [sp, #40]
	cmp r3, #64
	ble .L_0814e436
	ldr r4, [sp, #40]
	movs r3, #64
	subs r3, r3, r4
	lsls r2, r3, #3
	ldr r5, [sp, #44]
	movs r6, #216
	subs r2, r2, r3
	lsls r6, r6, #5
	movs r3, #40
	str r3, [sp, #0]
	adds r6, #249
	movs r3, #80
	str r3, [sp, #4]
	adds r2, #88
	ldr r0, [sp, #48]
	adds r1, r5, r6
	mov r3, r9
	ldr r7, [sp, #36]
	mov lr, r7
	.2byte 0xf800
.L_0814e436:
	ldr r0, [sp, #40]
	cmp r0, #71
	bgt .L_0814e486
	movs r1, #0
	mov r2, r10
	mov r8, r1
	cmp r2, #0
	beq .L_0814e486
	ldr r6, .L_0814e6c8
	ldr r7, .L_0814e6cc
.L_0814e44a:
	mov r0, r8
	movs r1, #3
	bl Math_Mod
	lsls r3, r0, #1
	ldrh r1, [r7, r3]
	ldr r3, [sp, #44]
	movs r4, #224
	adds r1, r3, r1
	lsls r4, r4, #3
	adds r1, r1, r4
	ldr r4, .L_0814e6d0
	ldrb r2, [r6]
	ldrb r5, [r4, r0]
	ldr r4, .L_0814e6d4
	ldrb r3, [r6, #1]
	ldrb r0, [r4, r0]
	subs r3, r3, r5
	str r0, [sp, #0]
	str r5, [sp, #4]
	subs r2, #56
	ldr r0, [sp, #48]
	ldr r5, [sp, #36]
	mov lr, r5
	.2byte 0xf800
	movs r0, #1
	add r8, r0
	adds r6, #2
	cmp r8, r10
	bne .L_0814e44a
.L_0814e486:
	ldr r1, [sp, #40]
	cmp r1, #72
	bne .L_0814e4ea
	ldr r6, .L_0814e6c8
	ldr r5, [sp, #44]
	movs r2, #0
	mov r8, r2
.L_0814e494:
	mov r3, r8
	movs r2, #15
	ands r2, r3
	lsls r2, r2, #1
	ldrb r3, [r6, r2]
	adds r2, #1
	subs r3, #56
	lsls r3, r3, #16
	str r3, [r5]
	ldrb r3, [r6, r2]
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	movs r3, #127
	ands r3, r0
	subs r3, #63
	lsls r3, r3, #13
	str r3, [r5, #12]
	bl Random16
	movs r3, #31
	ands r3, r0
	negs r3, r3
	movs r4, #1
	subs r3, #16
	add r8, r4
	lsls r3, r3, #14
	mov r7, r8
	str r3, [r5, #16]
	adds r5, #28
	cmp r7, #64
	bne .L_0814e494
	ldr r0, [sp, #40]
	cmp r0, #72
	bne .L_0814e4ea
	ldr r1, [sp, #44]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	adds r2, r1, r3
	movs r3, #4
	str r3, [r2]
.L_0814e4ea:
	ldr r4, [sp, #40]
	cmp r4, #71
	ble .L_0814e590
	movs r5, #32
	mov r10, r5
	cmp r4, #72
	beq .L_0814e4fc
	movs r6, #64
	mov r10, r6
.L_0814e4fc:
	movs r7, #0
	mov r0, r10
	mov r8, r7
	cmp r0, #0
	beq .L_0814e58a
	ldr r6, [sp, #44]
.L_0814e508:
	movs r1, #6
	ldrsh r7, [r6, r1]
	cmp r7, #135
	bgt .L_0814e580
	movs r1, #3
	mov r0, r8
	bl Math_Mod
	ldr r2, .L_0814e6d8
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #44]
	movs r3, #224
	adds r1, r2, r1
	lsls r3, r3, #3
	movs r4, #2
	ldrsh r2, [r6, r4]
	adds r1, r1, r3
	ldr r4, .L_0814e6dc
	ldr r3, .L_0814e6e0
	ldrb r5, [r3, r0]
	ldrb r0, [r4, r0]
	subs r3, r7, r5
	str r0, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #48]
	ldr r5, [sp, #36]
	mov lr, r5
	.2byte 0xf800
	movs r2, #128
	adds r0, r6, #0
	movs r1, #64
	lsls r2, r2, #9
	bl BattleFxKernels_IntegrateVector2
	movs r7, #6
	ldrsh r3, [r6, r7]
	cmp r3, #120
	ble .L_0814e580
	ldr r3, [r6, #16]
	movs r0, #128
	lsls r0, r0, #12
	cmp r3, r0
	ble .L_0814e580
	negs r3, r3
	cmp r3, #0
	bge .L_0814e568
	adds r3, #3
.L_0814e568:
	asrs r3, r3, #2
	str r3, [r6, #16]
	movs r3, #240
	lsls r3, r3, #15
	str r3, [r6, #4]
	ldr r1, [sp, #44]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
.L_0814e580:
	movs r4, #1
	add r8, r4
	adds r6, #28
	cmp r8, r10
	bne .L_0814e508
.L_0814e58a:
	ldr r5, [sp, #40]
	cmp r5, #71
	bgt .L_0814e5f4
.L_0814e590:
	ldr r5, [sp, #44]
	movs r6, #0
	mov r8, r6
.L_0814e596:
	movs r7, #2
	ldrsh r2, [r5, r7]
	movs r0, #6
	ldrsh r3, [r5, r0]
	ldr r4, [sp, #44]
	movs r1, #24
	movs r6, #224
	str r1, [sp, #0]
	lsls r6, r6, #3
	movs r1, #48
	subs r2, #12
	subs r3, #24
	str r1, [sp, #4]
	ldr r0, [sp, #48]
	adds r1, r4, r6
	ldr r7, [sp, #36]
	mov lr, r7
	.2byte 0xf800
	ldr r3, [r5]
	ldr r2, [r5, #12]
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #16]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
	asrs r3, r3, #16
	cmp r3, #120
	ble .L_0814e5e8
	ldr r0, [sp, #40]
	cmp r0, #47
	bgt .L_0814e5e8
	bl Random16
	movs r3, #127
	ands r3, r0
	adds r3, #64
	lsls r3, r3, #16
	str r3, [r5]
	ldr r3, .L_0814e6e4
	str r3, [r5, #4]
.L_0814e5e8:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r5, #28
	cmp r2, #64
	bne .L_0814e596
.L_0814e5f4:
	ldr r4, [sp, #52]
	movs r3, #0
	mov r8, r3
	ldr r3, [r4, #20]
	cmp r3, #0
	beq .L_0814e62c
	movs r6, #36
	movs r5, #32
.L_0814e604:
	ldr r7, [sp, #40]
	cmp r7, r5
	bne .L_0814e620
	ldr r1, [sp, #52]
	movs r3, #0
	ldrsh r0, [r6, r1]
	str r3, [sp, #0]
	movs r1, #9
	subs r3, #1
	movs r2, #5
	bl Func_0814cd48
	ldr r4, [sp, #52]
	ldr r3, [r4, #20]
.L_0814e620:
	movs r7, #1
	add r8, r7
	adds r6, #2
	adds r5, #8
	cmp r8, r3
	bne .L_0814e604
.L_0814e62c:
	ldr r0, [sp, #40]
	cmp r0, #72
	ble .L_0814e6ae
	ldr r5, .L_0814e6c0
	movs r1, #0
	adds r6, r0, #0
	mov r8, r1
	subs r6, #54
.L_0814e63c:
	ldr r3, [r5, #24]
	cmp r3, #17
	bhi .L_0814e674
	lsrs r0, r3, #31
	adds r0, r3, r0
	movs r1, #9
	asrs r0, r0, #1
	bl Math_Mod
	ldr r3, .L_0814e6e8
	ldr r2, [sp, #28]
	ldrb r4, [r3, r0]
	ldr r3, .L_0814e6ec
	lsls r0, r0, #1
	ldrh r1, [r3, r0]
	ldr r3, [r5, #4]
	adds r1, r2, r1
	ldr r2, [r5]
	lsrs r0, r4, #1
	subs r3, r3, r0
	subs r2, r2, r0
	str r4, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #48]
	ldr r4, [sp, #36]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5, #24]
.L_0814e674:
	adds r3, #1
	str r3, [r5, #24]
	cmp r3, #18
	bne .L_0814e6a2
	ldr r7, [sp, #40]
	cmp r7, #127
	bgt .L_0814e6a2
	bl Random16
	movs r3, #127
	ands r3, r0
	str r3, [r5]
	bl Random16
	lsrs r3, r6, #31
	movs r2, #63
	adds r3, r6, r3
	asrs r3, r3, #1
	ands r2, r0
	adds r2, r2, r3
	movs r3, #0
	str r2, [r5, #4]
	str r3, [r5, #24]
.L_0814e6a2:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #28
	cmp r1, #64
	bne .L_0814e63c
.L_0814e6ae:
	ldr r3, [sp, #40]
	subs r3, #72
	cmp r3, #7
	bhi .L_0814e6f0
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	b .L_0814e6f8
.L_0814e6c0:
	.4byte gMapCellBuffer
.L_0814e6c4:
	.4byte 0xfffffc20
.L_0814e6c8:
	.4byte Data_08198260
.L_0814e6cc:
	.4byte Data_081982cc
.L_0814e6d0:
	.4byte Data_081982d5
.L_0814e6d4:
	.4byte Data_081982d2
.L_0814e6d8:
	.4byte Data_081982de
.L_0814e6dc:
	.4byte Data_081982d8
.L_0814e6e0:
	.4byte Data_081982db
.L_0814e6e4:
	.4byte 0xfff00000
.L_0814e6e8:
	.4byte Data_0819745e
.L_0814e6ec:
	.4byte Data_0819744c
.L_0814e6f0:
	movs r0, #2
	movs r1, #2
	bl Func_08158ce0
.L_0814e6f8:
	ldr r3, [sp, #44]
	movs r4, #240
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r6, [sp, #40]
	movs r5, #14
	adds r6, #1
	add r9, r5
	str r6, [sp, #40]
	cmp r6, #146
	beq .L_0814e71c
	b .L_0814e3a6
.L_0814e71c:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_0814e740
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #336
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0814e740:
	.4byte Func_08143000
