.syntax unified
	.thumb
	.global Func_08180c94
	.thumb_func
Func_08180c94:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #68
	str r0, [sp, #32]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #96]
	str r0, [sp, #28]
	ldr r2, [r5, #92]
	mov r10, r2
	bl Func_0813ba50
	ldr r3, .L_08180cf0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #68
	strh r3, [r2]
	ldr r3, .L_08180cf4
	adds r2, #4
	strh r3, [r2]
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_08180cf8
	movs r2, #128
	lsls r2, r2, #19
	movs r1, #128
	adds r2, #82
	lsls r1, r1, #2
	strh r3, [r2]
	adds r1, #150
	movs r2, #3
	movs r0, #1
	bl Func_08152404
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #240
	add r3, r10
	ldr r0, [r3]
	b .L_08180cfc
.L_08180cf0:
	.4byte 0x00001080
.L_08180cf4:
	.4byte 0x00002737
.L_08180cf8:
	.4byte 0x00001010
.L_08180cfc:
	bl Func_0814cc4c
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	ldr r5, [r5, #104]
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_08180fa0
	add r1, r10
	movs r2, #1
	movs r3, #0
	str r5, [sp, #20]
	bl Resource_LoadAndDecompress
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r10
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	ldr r0, .L_08180fa4
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #180
	add r2, r10
	movs r3, #24
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #184
	movs r3, #0
	add r2, r10
	str r3, [r2]
	mov r11, r3
	ldr r3, .L_08180fa8
	str r3, [sp, #8]
.L_08180d5a:
	mov r4, r11
	cmp r4, #0
	bne .L_08180d8a
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r10
	movs r3, #50
	str r3, [r2]
	ldr r0, .L_08180fac
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08180fb0
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_08180d8a:
	mov r0, r11
	cmp r0, #56
	bne .L_08180d96
	movs r0, #140
	bl Audio_PlayCue
.L_08180d96:
	movs r6, #0
.L_08180d98:
	ldr r3, .L_08180fb4
	ldrb r3, [r3, r6]
	cmp r11, r3
	bne .L_08180dfa
	ldr r4, [sp, #32]
	movs r2, #0
	ldr r3, [r4, #20]
	mov r8, r2
	cmp r3, #0
	beq .L_08180de8
	movs r7, #128
	lsls r7, r7, #10
	movs r5, #36
.L_08180db2:
	ldr r2, [sp, #32]
	movs r1, #1
	ldrsh r0, [r5, r2]
	movs r3, #120
	str r3, [sp, #4]
	movs r3, #128
	adds r2, r7, #0
	lsls r3, r3, #12
	str r7, [sp, #0]
	bl Func_0815f000
	ldr r4, [sp, #32]
	movs r3, #7
	ldrsh r0, [r5, r4]
	movs r1, #7
	str r3, [sp, #0]
	movs r2, #5
	mov r3, r8
	bl Func_0814cd48
	ldr r4, [sp, #32]
	movs r3, #1
	add r8, r3
	ldr r3, [r4, #20]
	adds r5, #2
	cmp r8, r3
	bne .L_08180db2
.L_08180de8:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r10
	movs r3, #4
	str r3, [r2]
	movs r0, #145
	bl Audio_PlayCue
.L_08180dfa:
	adds r6, #1
	cmp r6, #7
	bne .L_08180d98
	mov r0, r11
	cmp r0, #79
	bgt .L_08180e08
	b .L_08180f20
.L_08180e08:
	cmp r0, #80
	bne .L_08180e38
	ldr r3, .L_08180fb8
	movs r2, #1
	movs r6, #0
	negs r2, r2
.L_08180e14:
	adds r6, #1
	str r2, [r3]
	adds r3, #28
	cmp r6, #64
	bne .L_08180e14
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	add r3, r10
	movs r2, #8
	str r2, [r3]
	movs r1, #240
	ldr r3, .L_08180fbc
	ldr r0, [sp, #28]
	lsls r1, r1, #6
	ldr r2, .L_08180fc0
	mov lr, r3
	.2byte 0xf800
.L_08180e38:
	mov r3, r11
	subs r3, #80
	cmp r3, #7
	bhi .L_08180ed0
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #28
	lsls r3, r3, #16
	str r3, [sp, #16]
	bl Random16
	movs r3, #31
	ands r3, r0
	ldr r4, [sp, #8]
	adds r3, #80
	lsls r3, r3, #16
	str r3, [sp, #12]
	movs r3, #15
	mov r9, r3
	ldr r0, .L_08180fc4
	lsls r3, r4, #3
	subs r3, r3, r4
	movs r2, #0
	lsls r3, r3, #2
	mov r8, r2
	adds r7, r3, r0
.L_08180e70:
	bl Random16
	movs r5, #31
	ands r5, r0
	bl Random16
	adds r6, r0, #0
	bl Random16
	mov r2, r9
	ldr r3, [sp, #16]
	ands r0, r2
	subs r0, #8
	lsls r0, r0, #16
	adds r0, r3, r0
	str r0, [r7]
	bl Random16
	mov r4, r9
	ands r0, r4
	ldr r2, [sp, #12]
	subs r0, #8
	lsls r0, r0, #16
	adds r0, r2, r0
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
	asrs r3, r3, #3
	str r3, [r7, #16]
	movs r3, #0
	str r3, [r7, #24]
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r7, #28
	cmp r4, #8
	bne .L_08180e70
.L_08180ed0:
	ldr r5, .L_08180fc4
	movs r6, #0
.L_08180ed4:
	ldr r0, [r5, #24]
	cmp r0, #17
	bhi .L_08180f18
	movs r1, #3
	bl Math_Div
	adds r1, r0, #0
	lsls r1, r1, #11
	movs r0, #224
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsls r0, r0, #3
	movs r4, #6
	ldrsh r3, [r5, r4]
	add r1, r10
	adds r1, r1, r0
	movs r0, #32
	str r0, [sp, #0]
	movs r0, #64
	subs r3, #48
	str r0, [sp, #4]
	subs r2, #16
	ldr r0, [sp, #28]
	ldr r4, [sp, #20]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #62
	ldr r2, .L_08180fc8
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_08180f18:
	adds r6, #1
	adds r5, #28
	cmp r6, #64
	bne .L_08180ed4
.L_08180f20:
	mov r0, r11
	cmp r0, #91
	bgt .L_08180ff4
	ldr r3, .L_08180fcc
	movs r1, #0
	ldr r4, [r3, #4]
	ldr r3, [r3]
	str r3, [sp, #44]
	str r4, [sp, #48]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #220
	add r3, r10
	ldr r2, [r3]
	movs r3, #32
	strb r3, [r2, #23]
	movs r3, #136
	lsls r3, r3, #8
	strh r3, [r2, #18]
	movs r3, #255
	add r4, sp, #52
	lsls r3, r3, #16
	strb r1, [r2, #22]
	str r1, [r4, #12]
	str r3, [r4, #4]
	cmp r0, #63
	bgt .L_08180f72
	mov r3, r11
	cmp r0, #0
	bge .L_08180f5e
	adds r3, #3
.L_08180f5e:
	asrs r3, r3, #2
	movs r2, #152
	subs r2, r2, r3
	lsls r2, r2, #16
	ldr r0, .L_08180fd0
	str r2, [r4]
	mov r2, r11
	lsls r3, r2, #17
	adds r3, r3, r0
	b .L_08180fde
.L_08180f72:
	mov r1, r11
	subs r1, #64
	cmp r1, #63
	bhi .L_08180fd4
	adds r2, r1, #0
	cmp r1, #0
	bge .L_08180f84
	mov r2, r11
	subs r2, #49
.L_08180f84:
	asrs r2, r2, #4
	movs r3, #136
	subs r3, r3, r2
	lsls r3, r3, #16
	str r3, [r4]
	lsrs r3, r1, #31
	adds r3, r1, r3
	asrs r3, r3, #1
	movs r2, #128
	lsls r3, r3, #16
	lsls r2, r2, #15
	adds r3, r3, r2
	b .L_08180fde
	.2byte 0x0000
.L_08180fa0:
	.4byte 0x0000013e
.L_08180fa4:
	.4byte Func_08143000
.L_08180fa8:
	.4byte 0xfffffd80
.L_08180fac:
	.4byte 0x00000148
.L_08180fb0:
	.4byte IwramCopyWords
.L_08180fb4:
	.4byte Data_081995ec
.L_08180fb8:
	.4byte Data_02014018
.L_08180fbc:
	.4byte IwramFillWords
.L_08180fc0:
	.4byte 0x3f3f3f3f
.L_08180fc4:
	.4byte Data_02014000
.L_08180fc8:
	.4byte 0xfffff800
.L_08180fcc:
	.4byte Data_08196ee8
.L_08180fd0:
	.4byte 0xffc00000
.L_08180fd4:
	movs r3, #136
	lsls r3, r3, #16
	str r3, [r4]
	movs r3, #128
	lsls r3, r3, #15
.L_08180fde:
	str r3, [r4, #8]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #220
	add r3, r10
	ldr r0, [r3]
	adds r1, r4, #0
	add r2, sp, #44
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
.L_08180ff4:
	mov r3, r11
	subs r3, #56
	cmp r3, #35
	bls .L_08180ffe
	b .L_08181158
.L_08180ffe:
	mov r4, r11
	lsls r3, r4, #1
	movs r6, #128
	subs r3, #104
	lsls r6, r6, #15
	movs r5, #128
	cmp r3, #64
	bgt .L_0818101c
	lsls r5, r3, #1
	movs r2, #128
	ldr r0, .L_081811b4
	adds r1, r5, #0
	lsls r2, r2, #9
	bl Func_0815b434
.L_0818101c:
	movs r3, #160
	lsls r3, r3, #15
	asrs r2, r3, #16
	ldr r0, .L_081811b4
	asrs r1, r6, #16
	adds r3, r5, #0
	bl Func_0818caa8
	mov r4, r11
	cmp r4, #56
	bne .L_08181080
	movs r6, #0
	movs r7, #0
	mov r5, r10
.L_08181038:
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
	beq .L_08181064
	ldr r3, [r5, #4]
	negs r3, r3
	str r3, [r5, #4]
.L_08181064:
	adds r6, #1
	str r7, [r5, #24]
	subs r7, #4
	adds r5, #28
	cmp r6, #16
	bne .L_08181038
	movs r1, #142
	lsls r1, r1, #7
	ldr r0, .L_081811b8
	add r1, r10
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
.L_08181080:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r9, r0
	movs r0, #1
	bl Func_081969f8
	movs r3, #0
	adds r7, r0, #0
	ldr r2, .L_081811bc
	str r3, [r7, #20]
	ldr r3, [sp, #36]
	mov r0, r9
	ands r3, r2
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_081811c0
	str r0, [r7, #12]
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #2
	orrs r3, r2
	str r3, [sp, #36]
	movs r3, #142
	lsls r3, r3, #7
	add r2, sp, #36
	add r3, r10
	str r3, [r2, #4]
	movs r3, #7
	str r3, [r7]
	ldr r3, .L_081811c4
	str r2, [r7, #16]
	str r3, [r7, #8]
	movs r2, #128
	lsls r2, r2, #9
	movs r6, #0
	mov r8, r2
	mov r5, r10
.L_081810cc:
	ldr r2, [r5, #4]
	ldr r1, [r5, #24]
	ldr r3, [r5]
	lsls r2, r2, #1
	adds r1, #2
	adds r3, r3, r2
	str r1, [r5, #24]
	str r3, [r5]
	cmp r1, #47
	bhi .L_08181144
	bl Func_08014de4
	movs r1, #3
	ands r1, r6
	ldr r4, .L_081811c8
	lsls r0, r1, #17
	movs r3, #132
	adds r1, #2
	movs r2, #160
	lsls r3, r3, #15
	lsls r2, r2, #15
	lsls r1, r1, #16
	adds r0, r0, r3
	subs r1, r2, r1
	adds r0, r0, r4
	adds r1, r1, r4
	movs r2, #0
	bl Func_08015160
	movs r0, #128
	mov r1, r8
	mov r2, r8
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
	add r0, r8
	lsls r1, r1, #10
	mov r2, r8
	bl Func_080151e4
	ldr r0, .L_081811cc
	mov r1, r9
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_08181144:
	adds r6, #1
	adds r5, #28
	cmp r6, #6
	bne .L_081810cc
	adds r0, r7, #0
	bl Sys_Free
	mov r0, r9
	bl Sys_Free
.L_08181158:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	movs r3, #1
	add r2, r10
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #8]
	movs r4, #1
	add r11, r4
	adds r3, #8
	mov r0, r11
	str r3, [sp, #8]
	cmp r0, #108
	beq .L_08181188
	b .L_08180d5a
.L_08181188:
	ldr r0, .L_081811d0
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #220
	add r3, r10
	ldr r0, [r3]
	bl ResourceObject_ReleaseFar
	bl Func_08143bb8
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_081811b4:
	.4byte gMapCellBuffer
.L_081811b8:
	.4byte 0x000000c9
.L_081811bc:
	.4byte 0xffffff00
.L_081811c0:
	.4byte 0xffff00ff
.L_081811c4:
	.4byte Data_08199220
.L_081811c8:
	.4byte 0xffc00000
.L_081811cc:
	.4byte Data_081991b0
.L_081811d0:
	.4byte Func_08143000
