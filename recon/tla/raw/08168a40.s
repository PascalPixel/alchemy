.syntax unified
	.thumb
	.global Func_08168a40
	.thumb_func
Func_08168a40:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #180
	str r0, [sp, #84]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #96]
	str r0, [sp, #80]
	movs r0, #0
	ldr r1, [r5, #92]
	str r1, [sp, #76]
	ldr r2, [r5, #100]
	adds r5, #176
	str r2, [sp, #68]
	bl BattleFx_BeginCanvasLayer
	bl Func_0813ba50
	ldr r3, .L_08168aa8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r3, .L_08168aac
	movs r2, #160
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_08168ab0
	adds r2, #2
	strh r3, [r2]
	ldr r3, [sp, #76]
	movs r4, #239
	lsls r4, r4, #7
	adds r6, r3, r4
	movs r1, #200
	movs r3, #0
	str r3, [r6]
	lsls r1, r1, #4
	ldr r0, .L_08168ab4
	bl Scheduler_AddOrUpdateCallback
	movs r0, #1
	movs r1, #0
	bl Func_08163c2c
	movs r1, #161
	b .L_08168ab8
.L_08168aa8:
	.4byte 0x00000784
.L_08168aac:
	.4byte 0x00000000
.L_08168ab0:
	.4byte 0x00000018
.L_08168ab4:
	.4byte Func_08143000
.L_08168ab8:
	lsls r1, r1, #2
	movs r0, #9
	movs r2, #1
	bl Func_08152404
	ldr r2, .L_08168b1c
	movs r3, #240
	str r3, [r2, #16]
	ldr r7, [sp, #76]
	movs r0, #240
	lsls r0, r0, #7
	adds r0, #240
	adds r3, r7, r0
	ldr r0, [r3]
	bl Func_0814cc4c
	ldr r3, .L_08168b14
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #72
	strh r3, [r2]
	ldr r3, .L_08168b18
	subs r2, #8
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldr r1, .L_08168b20
	movs r0, #1
	bl Func_08118040
	movs r0, #1
	movs r1, #1
	bl Func_08163c2c
	ldr r0, .L_08168b24
	ldr r1, [sp, #68]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r2, #224
	lsls r2, r2, #3
	b .L_08168b28
	.2byte 0x0000
.L_08168b14:
	.4byte 0x00002737
.L_08168b18:
	.4byte 0x000000ca
.L_08168b1c:
	.4byte gCameraSceneParameters
.L_08168b20:
	.4byte 0x00000044
.L_08168b24:
	.4byte 0x00000134
.L_08168b28:
	adds r1, r7, r2
	ldr r0, .L_08168b7c
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r3, .L_08168b6c
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_08168b70
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_08168b74
	adds r2, #50
	strh r3, [r2]
	ldr r3, .L_08168b78
	subs r2, #2
	strh r3, [r2]
	movs r3, #0
	str r3, [sp, #64]
	str r3, [sp, #60]
	ldr r3, .L_08168b80
	movs r4, #0
	ldrh r3, [r3, #4]
	movs r2, #1
	str r3, [sp, #56]
	movs r0, #238
	ldr r5, [r5]
	str r4, [sp, #48]
	str r5, [sp, #52]
	str r2, [r6]
	b .L_08168b84
	.2byte 0x0000
.L_08168b6c:
	.4byte 0x00007741
.L_08168b70:
	.4byte 0x00000080
.L_08168b74:
	.4byte 0x0000100e
.L_08168b78:
	.4byte 0x00003f44
.L_08168b7c:
	.4byte 0x00000158
.L_08168b80:
	.4byte Data_03001120
.L_08168b84:
	ldr r1, [sp, #64]
	lsls r0, r0, #7
	adds r0, #132
	adds r3, r7, r0
	str r1, [r3]
	ldr r3, [sp, #52]
	mov r8, r4
	str r2, [r3, #16]
	ldr r5, [sp, #76]
	movs r6, #31
.L_08168b98:
	bl Random16
	ands r0, r6
	adds r0, #16
	str r0, [r5]
	bl Random16
	ands r0, r6
	adds r0, #48
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #16
	lsls r0, r0, #16
	str r0, [r5, #16]
	bl Random16
	movs r1, #48
	bl __umodsi3
	movs r4, #1
	add r8, r4
	adds r0, #2
	mov r7, r8
	str r0, [r5, #24]
	adds r5, #28
	cmp r7, #64
	bne .L_08168b98
	movs r0, #104
	movs r1, #3
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r2, #128
	str r3, [sp, #72]
	ldr r3, .L_08168bf4
	lsls r2, r2, #19
	movs r0, #0
	adds r2, #12
	mov r11, r0
	strh r3, [r2]
	b .L_08168f3a
.L_08168bf4:
	.4byte 0x00000786
.L_08168bf8:
	mov r3, r11
	subs r3, #24
	cmp r3, #31
	bhi .L_08168c06
	ldr r1, [sp, #48]
	adds r1, #1
	str r1, [sp, #48]
.L_08168c06:
	ldr r2, [sp, #48]
	cmp r2, #24
	ble .L_08168c10
	movs r3, #24
	str r3, [sp, #48]
.L_08168c10:
	mov r4, r11
	cmp r4, #135
	bgt .L_08168c28
	ldr r7, .L_08168e00
	ldr r0, [sp, #48]
	ldrh r3, [r7, #4]
	adds r1, r7, #0
	subs r3, r3, r0
	strh r3, [r1, #4]
	ldr r2, [sp, #60]
	adds r2, r2, r0
	str r2, [sp, #60]
.L_08168c28:
	mov r3, r11
	cmp r3, #149
	bgt .L_08168d08
	ldr r3, .L_08168e04
	mov r7, r11
	ldr r4, [r3, #4]
	ldr r3, [r3]
	str r3, [sp, #96]
	str r4, [sp, #100]
	movs r4, #0
	mov r10, r4
	cmp r7, #103
	ble .L_08168c4a
	ldr r0, .L_08168e08
	lsls r3, r7, #4
	adds r0, r0, r3
	mov r10, r0
.L_08168c4a:
	mov r3, r11
	subs r3, #8
	cmp r3, #23
	bhi .L_08168c5c
	ldr r1, [sp, #64]
	ldr r2, [sp, #48]
	adds r3, r1, r2
	subs r3, #8
	str r3, [sp, #64]
.L_08168c5c:
	mov r3, r11
	cmp r3, #7
	ble .L_08168caa
	movs r5, #96
	cmp r3, #104
	bgt .L_08168c6a
	movs r5, #32
.L_08168c6a:
	ldr r7, .L_08168e0c
	mov r4, r11
	lsls r3, r4, #10
	adds r0, r3, r7
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	movs r1, #128
	ands r0, r3
	lsls r1, r1, #8
	cmp r0, r1
	ble .L_08168c86
	ldr r2, .L_08168e10
	adds r0, r0, r2
.L_08168c86:
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #16
	str r3, [sp, #44]
	mov r4, r11
	movs r3, #31
	ands r3, r4
	cmp r3, #8
	bne .L_08168caa
	ldr r7, [sp, #76]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #168
	adds r2, r7, r0
	movs r3, #4
	str r3, [r2]
.L_08168caa:
	add r3, sp, #164
	movs r2, #0
	str r2, [r3, #12]
	movs r2, #255
	lsls r2, r2, #16
	str r2, [r3, #4]
	adds r6, r3, #0
	ldr r2, [sp, #76]
	movs r3, #238
	lsls r3, r3, #7
	movs r1, #0
	adds r3, #220
	mov r8, r1
	add r7, sp, #96
	adds r5, r2, r3
.L_08168cc8:
	ldr r3, .L_08168e14
	mov r4, r8
	ldr r0, [sp, #64]
	ldrb r3, [r3, r4]
	mov r1, r10
	adds r3, r0, r3
	subs r3, r3, r1
	movs r2, #224
	lsls r2, r2, #16
	lsls r3, r3, #16
	adds r3, r3, r2
	str r3, [r6]
	ldr r3, .L_08168e18
	movs r0, #144
	ldrb r3, [r3, r4]
	ldr r4, [sp, #44]
	lsls r0, r0, #15
	subs r3, r3, r4
	lsls r3, r3, #16
	adds r3, r3, r0
	str r3, [r6, #8]
	adds r1, r6, #0
	adds r2, r7, #0
	ldmia r5!, {r0}
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #9
	bne .L_08168cc8
.L_08168d08:
	mov r3, r11
	cmp r3, #26
	bgt .L_08168da0
	ldr r7, [sp, #60]
	lsls r3, r3, #3
	adds r7, #4
	mov r10, r3
	cmp r7, #10
	ble .L_08168d1c
	movs r7, #10
.L_08168d1c:
	mov r4, r10
	cmp r4, #64
	ble .L_08168d26
	movs r0, #64
	mov r10, r0
.L_08168d26:
	movs r1, #0
	mov r2, r10
	mov r8, r1
	cmp r2, #0
	beq .L_08168da0
	ldr r3, [sp, #60]
	ldr r4, [sp, #60]
	lsls r3, r3, #1
	str r3, [sp, #40]
	adds r3, r3, r4
	lsls r3, r3, #2
	adds r3, #48
	str r3, [sp, #36]
	lsrs r3, r7, #31
	adds r3, r7, r3
	asrs r3, r3, #1
	str r3, [sp, #32]
	lsls r0, r7, #1
	mov r9, r0
.L_08168d4c:
	mov r1, r8
	lsls r6, r1, #10
	adds r0, r6, #0
	bl Trig_Sin
	ldr r3, [sp, #40]
	ldr r2, [sp, #60]
	adds r3, #8
	adds r5, r3, #0
	muls r5, r0
	adds r0, r6, #0
	asrs r5, r5, #16
	adds r5, r5, r2
	bl Trig_Cos
	ldr r4, [sp, #36]
	mov r2, r9
	adds r3, r4, #0
	muls r3, r0
	ldr r0, .L_08168e1c
	ldr r4, [sp, #32]
	subs r2, #2
	ldrh r1, [r0, r2]
	ldr r2, [sp, #68]
	adds r5, #96
	asrs r3, r3, #16
	subs r5, r5, r4
	mov r0, r9
	adds r3, #64
	adds r1, r2, r1
	str r0, [sp, #4]
	subs r3, r3, r7
	str r7, [sp, #0]
	ldr r0, [sp, #80]
	adds r2, r5, #0
	ldr r4, [sp, #72]
	mov lr, r4
	.2byte 0xf800
	movs r0, #1
	add r8, r0
	cmp r8, r10
	bne .L_08168d4c
.L_08168da0:
	mov r1, r11
	cmp r1, #24
	bne .L_08168dc0
	ldr r3, [sp, #76]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
	ldr r7, [sp, #76]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #132
	adds r2, r7, r0
	movs r3, #50
	str r3, [r2]
.L_08168dc0:
	mov r1, r11
	cmp r1, #28
	bne .L_08168dd0
	movs r2, #128
	ldr r3, .L_08168dfc
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
.L_08168dd0:
	mov r2, r11
	cmp r2, #17
	ble .L_08168e9a
	ldr r5, [sp, #76]
	movs r3, #0
	mov r8, r3
	movs r6, #31
.L_08168dde:
	ldr r3, [r5, #24]
	cmp r3, #0
	bne .L_08168e54
	movs r1, #3
	mov r0, r8
	bl __modsi3
	ldr r2, .L_08168e1c
	adds r0, #1
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r7, [sp, #68]
	ldr r2, [r5]
	b .L_08168e20
.L_08168dfc:
	.4byte 0x00000784
.L_08168e00:
	.4byte Data_03001120
.L_08168e04:
	.4byte Data_08196e7c
.L_08168e08:
	.4byte 0xfffff980
.L_08168e0c:
	.4byte 0xffffe000
.L_08168e10:
	.4byte 0xffff8000
.L_08168e14:
	.4byte Data_08198ada
.L_08168e18:
	.4byte Data_08198ae3
.L_08168e1c:
	.4byte Data_08197410
.L_08168e20:
	adds r1, r7, r1
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #80]
	ldr r4, [sp, #72]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5]
	ldr r2, [r5, #16]
	adds r3, #2
	str r3, [r5]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #4
	cmp r3, #0
	bge .L_08168e4e
	adds r3, #63
.L_08168e4e:
	asrs r3, r3, #6
	str r3, [r5, #16]
	b .L_08168e58
.L_08168e54:
	subs r3, #1
	str r3, [r5, #24]
.L_08168e58:
	ldr r3, [r5]
	cmp r3, #128
	bgt .L_08168e64
	ldr r3, [r5, #24]
	cmp r3, #1
	bne .L_08168e8e
.L_08168e64:
	bl Random16
	ldr r7, [sp, #64]
	ands r0, r6
	adds r0, r0, r7
	adds r0, #172
	str r0, [r5]
	bl Random16
	ldr r1, [sp, #44]
	ands r0, r6
	subs r0, r0, r1
	adds r0, #56
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #16
	lsls r0, r0, #15
	str r0, [r5, #16]
.L_08168e8e:
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r5, #28
	cmp r3, #48
	bne .L_08168dde
.L_08168e9a:
	mov r4, r11
	cmp r4, #31
	ble .L_08168ef0
	mov r3, r11
	subs r3, #32
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r5, r3, #1
	cmp r5, #40
	ble .L_08168eb0
	movs r5, #40
.L_08168eb0:
	movs r7, #0
	mov r8, r7
	movs r6, #0
	movs r7, #120
.L_08168eb8:
	bl Random16
	movs r3, #3
	ands r0, r3
	lsls r1, r0, #1
	adds r1, r1, r0
	ldr r0, [sp, #76]
	movs r3, #48
	lsls r1, r1, #9
	movs r2, #224
	adds r1, r0, r1
	lsls r2, r2, #3
	str r3, [sp, #0]
	movs r3, #32
	adds r1, r1, r2
	str r3, [sp, #4]
	ldr r0, [sp, #80]
	adds r3, r6, #0
	subs r2, r7, r5
	ldr r4, [sp, #72]
	mov lr, r4
	.2byte 0xf800
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r6, #18
	cmp r1, #6
	bne .L_08168eb8
.L_08168ef0:
	ldr r3, [sp, #76]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #168
	adds r2, r3, r4
	ldr r3, [r2]
	cmp r3, #0
	ble .L_08168f1c
	subs r3, #1
	str r3, [r2]
	bl Random16
	ldr r3, .L_08168f14
	ldr r7, .L_08168f18
	ands r0, r3
	adds r0, #28
	strh r0, [r7, #6]
	b .L_08168f22
.L_08168f14:
	.4byte 0x00000007
.L_08168f18:
	.4byte Data_03001120
.L_08168f1c:
	ldr r0, .L_08168fc8
	movs r3, #32
	strh r3, [r0, #6]
.L_08168f22:
	ldr r1, [sp, #76]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r4, #1
	add r11, r4
.L_08168f3a:
	mov r7, r11
	cmp r7, #120
	beq .L_08168f94
	movs r0, #0
	str r0, [sp, #44]
	cmp r7, #0
	bne .L_08168f4e
	movs r0, #136
	bl Audio_PlayCue
.L_08168f4e:
	mov r1, r11
	cmp r1, #26
	bne .L_08168f5a
	movs r0, #141
	bl Audio_PlayCue
.L_08168f5a:
	mov r2, r11
	cmp r2, #40
	bne .L_08168f66
	movs r0, #154
	bl Audio_PlayCue
.L_08168f66:
	mov r3, r11
	cmp r3, #72
	bne .L_08168f72
	movs r0, #154
	bl Audio_PlayCue
.L_08168f72:
	mov r4, r11
	cmp r4, #104
	bne .L_08168f7e
	movs r0, #154
	bl Audio_PlayCue
.L_08168f7e:
	ldr r3, .L_08168fcc
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	bne .L_08168f8c
	b .L_08168bf8
.L_08168f8c:
	mov r7, r11
	cmp r7, #16
	bgt .L_08168f94
	b .L_08168bf8
.L_08168f94:
	add r0, sp, #56
	ldr r3, .L_08168fc8
	ldrh r0, [r0]
	movs r2, #0
	strh r0, [r3, #4]
	ldr r1, [sp, #52]
	str r2, [r1, #16]
	bl Func_0814cca8
	ldr r3, .L_08168fc4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #64
	strh r3, [r2]
	ldr r3, [sp, #76]
	movs r4, #238
	lsls r4, r4, #7
	movs r2, #0
	adds r4, #220
	mov r8, r2
	movs r0, #12
	adds r1, r3, r4
	b .L_08168fd0
	.2byte 0x0000
.L_08168fc4:
	.4byte 0x000000f0
.L_08168fc8:
	.4byte Data_03001120
.L_08168fcc:
	.4byte gInput
.L_08168fd0:
	ldmia r1!, {r2}
	movs r7, #1
	ldrb r3, [r2, #9]
	add r8, r7
	orrs r3, r0
	strb r3, [r2, #9]
	mov r2, r8
	cmp r2, #9
	bne .L_08168fd0
	mov r4, sp
	adds r4, #120
	str r4, [sp, #24]
	movs r3, #224
	mov r2, sp
	str r3, [sp, #28]
	movs r1, #0
	adds r3, r4, #0
	adds r2, #134
.L_08168ff4:
	strb r1, [r3]
	adds r3, #1
	cmp r3, r2
	bne .L_08168ff4
	mov r7, sp
	adds r7, #136
	str r7, [sp, #20]
	ldr r5, [sp, #20]
	movs r7, #31
	add r6, sp, #152
.L_08169008:
	bl Random16
	ands r0, r7
	strb r0, [r5]
	adds r5, #1
	cmp r5, r6
	bne .L_08169008
	ldr r3, .L_08169080
	movs r0, #0
	movs r2, #160
	mov r8, r0
	movs r1, #0
	lsls r2, r2, #1
.L_08169022:
	movs r4, #1
	add r8, r4
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_08169022
	ldr r7, [sp, #76]
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	lsls r1, r1, #7
	adds r2, r7, r0
	movs r3, #2
	adds r1, #132
	str r3, [r2]
	adds r2, r7, r1
	movs r3, #75
	str r3, [r2]
	ldr r3, .L_08169078
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r3, .L_0816907c
	adds r2, #70
	strh r3, [r2]
	ldr r3, .L_08169084
	movs r2, #0
	str r3, [sp, #16]
	mov r11, r2
.L_0816905e:
	mov r4, r11
	cmp r4, #23
	bgt .L_08169120
	ldr r3, .L_08169088
	ldr r7, [sp, #28]
	ldr r4, [r3, #4]
	ldr r3, [r3]
	subs r7, #16
	mov r0, r11
	str r3, [sp, #88]
	str r4, [sp, #92]
	str r7, [sp, #28]
	b .L_0816908c
.L_08169078:
	.4byte 0x00000784
.L_0816907c:
	.4byte 0x00001010
.L_08169080:
	.4byte Data_02010018
.L_08169084:
	.4byte 0xfffffe20
.L_08169088:
	.4byte Data_08196e84
.L_0816908c:
	cmp r0, #8
	bgt .L_081690ac
	movs r1, #128
	lsls r3, r0, #11
	lsls r1, r1, #7
	movs r2, #128
	adds r0, r3, r1
	lsls r2, r2, #8
	cmp r0, r2
	ble .L_081690a4
	ldr r4, .L_08169398
	adds r0, r3, r4
.L_081690a4:
	bl Trig_Sin
	lsls r0, r0, #6
	b .L_081690c8
.L_081690ac:
	mov r7, r11
	movs r1, #128
	lsls r3, r7, #11
	lsls r1, r1, #7
	movs r2, #128
	adds r0, r3, r1
	lsls r2, r2, #8
	cmp r0, r2
	ble .L_081690c2
	ldr r4, .L_08169398
	adds r0, r3, r4
.L_081690c2:
	bl Trig_Sin
	lsls r0, r0, #5
.L_081690c8:
	asrs r4, r0, #16
	add r3, sp, #104
	movs r2, #0
	str r2, [r3, #12]
	movs r2, #255
	lsls r2, r2, #16
	str r2, [r3, #4]
	ldr r0, [sp, #76]
	movs r1, #238
	lsls r1, r1, #7
	movs r7, #0
	adds r1, #220
	mov r8, r7
	adds r6, r3, #0
	add r7, sp, #88
	adds r5, r0, r1
.L_081690e8:
	ldr r3, .L_0816939c
	mov r2, r8
	ldr r0, [sp, #28]
	ldrb r3, [r3, r2]
	movs r1, #144
	adds r3, r0, r3
	lsls r3, r3, #16
	str r3, [r6]
	ldr r3, .L_081693a0
	lsls r1, r1, #15
	ldrb r3, [r3, r2]
	ldmia r5!, {r0}
	subs r3, r3, r4
	lsls r3, r3, #16
	adds r3, r3, r1
	str r3, [r6, #8]
	adds r2, r7, #0
	movs r3, #0
	adds r1, r6, #0
	str r4, [sp, #8]
	bl Render_ApplyProjectedPlacementFar
	movs r2, #1
	add r8, r2
	mov r3, r8
	ldr r4, [sp, #8]
	cmp r3, #9
	bne .L_081690e8
.L_08169120:
	mov r4, r11
	cmp r4, #8
	bne .L_08169138
	ldr r7, [sp, #76]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #168
	adds r3, r7, r0
	str r4, [r3]
	movs r0, #145
	bl Audio_PlayCue
.L_08169138:
	mov r1, r11
	cmp r1, #11
	bne .L_08169144
	movs r0, #145
	bl Audio_PlayCue
.L_08169144:
	mov r2, r11
	cmp r2, #46
	bne .L_08169150
	movs r0, #137
	bl Audio_PlayCue
.L_08169150:
	ldr r4, [sp, #84]
	movs r3, #0
	ldr r2, [r4, #20]
	mov r8, r3
	cmp r2, #0
	beq .L_08169216
	str r3, [sp, #12]
	movs r7, #36
	mov r9, r7
.L_08169162:
	ldr r0, [sp, #24]
	mov r1, r8
	ldrb r3, [r0, r1]
	cmp r3, #0
	bne .L_08169200
	ldr r4, [sp, #84]
	add r5, sp, #152
	mov r2, r9
	ldrsh r0, [r2, r4]
	adds r1, r5, #0
	bl Func_0815e21c
	ldr r3, [r5]
	ldr r7, [sp, #28]
	cmp r3, r7
	ble .L_081691fc
	ldr r0, [sp, #24]
	movs r3, #1
	mov r1, r8
	strb r3, [r0, r1]
	ldr r2, [sp, #12]
	ldr r3, .L_081693a4
	mov r10, r5
	movs r6, #0
	movs r7, #255
	adds r5, r2, r3
.L_08169196:
	mov r4, r10
	ldr r3, [r4]
	adds r6, #1
	lsls r3, r3, #15
	str r3, [r5]
	ldr r3, [r4, #4]
	subs r3, #16
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	ands r0, r7
	subs r0, #128
	lsls r0, r0, #10
	str r0, [r5, #12]
	bl Random16
	ands r0, r7
	subs r0, #192
	lsls r3, r0, #11
	ldr r2, [r5, #12]
	str r3, [r5, #16]
	ldr r3, [r5]
	lsls r2, r2, #2
	adds r3, r3, r2
	str r3, [r5]
	ldr r3, [r5, #4]
	lsls r0, r0, #13
	adds r3, r3, r0
	str r3, [r5, #4]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #8
	str r3, [r5, #24]
	adds r5, #28
	cmp r6, #32
	bne .L_08169196
	ldr r2, [sp, #84]
	mov r7, r9
	ldrsh r0, [r7, r2]
	movs r1, #1
	bl Func_08118088
	movs r0, #134
	bl Audio_PlayCue
	ldr r3, [sp, #84]
	ldr r2, [r3, #20]
	b .L_08169200
.L_081691fc:
	ldr r4, [sp, #84]
	ldr r2, [r4, #20]
.L_08169200:
	ldr r0, [sp, #12]
	movs r1, #224
	lsls r1, r1, #2
	movs r3, #1
	movs r7, #2
	adds r0, r0, r1
	add r8, r3
	add r9, r7
	str r0, [sp, #12]
	cmp r8, r2
	bne .L_08169162
.L_08169216:
	ldr r5, .L_081693a4
	movs r4, #0
	mov r8, r4
	movs r7, #3
	movs r6, #6
.L_08169220:
	ldr r3, [r5, #24]
	cmp r3, #0
	ble .L_0816925c
	ldr r3, .L_081693a8
	ldr r0, [sp, #68]
	ldrh r1, [r3, #4]
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r4, #6
	ldrsh r3, [r5, r4]
	adds r1, r0, r1
	subs r2, #1
	subs r3, #3
	str r7, [sp, #0]
	str r6, [sp, #4]
	ldr r0, [sp, #80]
	ldr r4, [sp, #72]
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
	subs r3, #1
	str r3, [r5, #24]
.L_0816925c:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #28
	cmp r1, #192
	bne .L_08169220
	mov r2, r11
	cmp r2, #48
	bne .L_08169274
	movs r0, #136
	bl Audio_PlayCue
.L_08169274:
	mov r3, r11
	cmp r3, #40
	ble .L_081692dc
	ldr r4, [sp, #76]
	movs r7, #239
	movs r0, #238
	lsls r7, r7, #7
	lsls r0, r0, #7
	adds r2, r4, r7
	movs r3, #0
	adds r0, #132
	ldr r6, [sp, #16]
	str r3, [r2]
	adds r2, r4, r0
	movs r3, #75
	str r3, [r2]
	movs r1, #0
	movs r5, #8
	mov r8, r1
	negs r5, r5
.L_0816929c:
	bl Random16
	movs r3, #3
	ands r0, r3
	ldr r2, [sp, #76]
	ldr r4, [sp, #20]
	lsls r1, r0, #1
	adds r1, r1, r0
	mov r7, r8
	lsls r1, r1, #9
	adds r1, r2, r1
	movs r3, #224
	ldrb r2, [r4, r7]
	lsls r3, r3, #3
	adds r1, r1, r3
	movs r3, #48
	str r3, [sp, #0]
	subs r2, r2, r6
	movs r3, #32
	movs r7, #1
	str r3, [sp, #4]
	adds r2, #120
	adds r3, r5, #0
	ldr r0, [sp, #80]
	ldr r4, [sp, #72]
	add r8, r7
	mov lr, r4
	.2byte 0xf800
	mov r0, r8
	adds r5, #8
	cmp r0, #16
	bne .L_0816929c
.L_081692dc:
	mov r1, r11
	cmp r1, #64
	ble .L_081692ee
	ldr r3, [sp, #76]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
.L_081692ee:
	mov r7, r11
	cmp r7, #58
	bne .L_08169324
	ldr r1, [sp, #84]
	movs r0, #0
	ldr r3, [r1, #20]
	mov r8, r0
	cmp r3, #0
	beq .L_08169324
	movs r5, #36
	movs r6, #0
.L_08169304:
	ldr r2, [sp, #84]
	movs r1, #14
	ldrsh r0, [r5, r2]
	movs r3, #1
	negs r3, r3
	movs r2, #5
	str r6, [sp, #0]
	bl Func_0814cd48
	ldr r7, [sp, #84]
	movs r4, #1
	ldr r3, [r7, #20]
	add r8, r4
	adds r5, #2
	cmp r8, r3
	bne .L_08169304
.L_08169324:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r1, #240
	ldr r0, [sp, #76]
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #16]
	movs r3, #1
	add r11, r3
	adds r2, #12
	mov r4, r11
	str r2, [sp, #16]
	cmp r4, #96
	beq .L_08169356
	b .L_0816905e
.L_08169356:
	movs r0, #134
	bl Func_081180e8
	movs r1, #238
	ldr r0, [sp, #76]
	lsls r1, r1, #7
	movs r7, #0
	adds r1, #220
	mov r8, r7
	adds r6, r0, r1
.L_0816936a:
	ldmia r6!, {r0}
	bl ResourceObject_ReleaseFar
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #9
	bne .L_0816936a
	ldr r0, .L_081693ac
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #180
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08169398:
	.4byte 0xffffc000
.L_0816939c:
	.4byte Data_08198ada
.L_081693a0:
	.4byte Data_08198ae3
.L_081693a4:
	.4byte gMapCellBuffer
.L_081693a8:
	.4byte Data_08197410
.L_081693ac:
	.4byte Func_08143000
