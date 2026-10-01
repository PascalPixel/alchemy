.syntax unified
	.thumb
	.global Func_08191d60
	.thumb_func
Func_08191d60:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #60
	str r0, [sp, #40]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #96]
	str r0, [sp, #36]
	movs r0, #0
	ldr r1, [r3, #92]
	ldr r3, [r3, #100]
	mov r11, r1
	str r3, [sp, #32]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_08191dc8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	mov r2, sp
	adds r2, #52
	adds r1, r2, #0
	movs r0, #0
	str r2, [sp, #28]
	bl Func_08144aac
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_08191dcc
	add r1, r11
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r1, #220
	lsls r1, r1, #6
	ldr r0, .L_08191dd0
	add r1, r11
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_08191dd4
	ldr r1, [sp, #32]
	b .L_08191dd8
	.2byte 0x0000
.L_08191dc8:
	.4byte 0x00001010
.L_08191dcc:
	.4byte 0x0000013e
.L_08191dd0:
	.4byte 0x000000c1
.L_08191dd4:
	.4byte 0x00000134
.L_08191dd8:
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	bl Func_0815b410
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
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_081920f4
	bl Scheduler_AddOrUpdateCallback
	movs r3, #128
	lsls r3, r3, #16
	str r3, [sp, #20]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #240
	add r3, r11
	movs r4, #128
	lsls r4, r4, #15
	ldr r0, [r3]
	str r4, [sp, #24]
	bl Func_0814cc4c
	movs r0, #1
	bl WaitFrames
	movs r1, #128
	lsls r1, r1, #2
	movs r0, #12
	adds r1, #142
	movs r2, #2
	bl Func_08152404
	movs r0, #0
	mov r9, r0
.L_08191e36:
	ldr r3, .L_081920f8
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_08191e56
	mov r1, r9
	cmp r1, #231
	bgt .L_08191e56
	cmp r1, #15
	ble .L_08191e56
	movs r0, #144
	bl Audio_PlayCue
	movs r2, #232
	mov r9, r2
.L_08191e56:
	mov r3, r9
	cmp r3, #0
	bne .L_08191f3c
	movs r4, #0
	movs r2, #1
	mov r3, r11
	mov r10, r4
	negs r2, r2
	adds r3, #24
.L_08191e68:
	movs r0, #1
	add r10, r0
	mov r1, r10
	str r2, [r3]
	adds r3, #28
	cmp r1, #64
	bne .L_08191e68
	ldr r5, .L_081920fc
	movs r2, #0
	mov r10, r2
	movs r6, #0
.L_08191e7e:
	bl Random16
	mov r3, r10
	str r3, [r5, #24]
	bl Random16
	movs r3, #255
	ands r3, r0
	subs r3, #64
	movs r4, #1
	movs r0, #128
	lsls r3, r3, #16
	add r10, r4
	lsls r0, r0, #1
	str r3, [r5]
	str r6, [r5, #4]
	str r6, [r5, #12]
	str r6, [r5, #16]
	adds r5, #28
	cmp r10, r0
	bne .L_08191e7e
	movs r1, #0
	mov r10, r1
	movs r7, #0
	movs r6, #0
	mov r5, r11
.L_08191eb2:
	mov r2, r10
	adds r3, r6, r2
	adds r3, #44
	lsls r3, r3, #16
	str r3, [r5]
	movs r3, #144
	lsls r3, r3, #15
	str r3, [r5, #4]
	str r7, [r5, #12]
	bl Random16
	movs r3, #3
	ands r3, r0
	adds r3, #2
	negs r3, r3
	lsls r3, r3, #16
	str r3, [r5, #16]
	str r7, [r5, #20]
	bl Random16
	mov r3, r10
	negs r2, r3
	movs r4, #1
	movs r3, #15
	ands r3, r0
	add r10, r4
	subs r2, r2, r3
	mov r0, r10
	str r2, [r5, #24]
	adds r6, #3
	adds r5, #28
	cmp r0, #16
	bne .L_08191eb2
	movs r5, #168
	movs r1, #0
	lsls r5, r5, #2
	mov r10, r1
	movs r7, #3
	movs r6, #0
	add r5, r11
.L_08191f02:
	movs r3, #216
	lsls r3, r3, #14
	str r3, [r5]
	movs r3, #224
	lsls r3, r3, #14
	str r3, [r5, #4]
	str r6, [r5, #24]
	bl Random16
	movs r2, #128
	ands r0, r7
	lsls r2, r2, #10
	lsls r0, r0, #16
	adds r0, r0, r2
	negs r0, r0
	str r0, [r5, #12]
	bl Random16
	movs r3, #1
	ands r0, r7
	negs r0, r0
	add r10, r3
	lsls r0, r0, #15
	mov r4, r10
	str r0, [r5, #16]
	subs r6, #2
	adds r5, #28
	cmp r4, #16
	bne .L_08191f02
.L_08191f3c:
	movs r0, #16
	adds r0, #255
	cmp r9, r0
	ble .L_08191f46
	b .L_08192118
.L_08191f46:
	mov r2, r9
	mov r3, r9
	subs r2, #91
	subs r3, #217
	str r2, [sp, #12]
	str r3, [sp, #16]
	ldr r7, .L_081920fc
	movs r1, #0
	mov r10, r1
.L_08191f58:
	ldr r3, [r7, #24]
	cmp r3, #0
	beq .L_08191f60
	b .L_081920c4
.L_08191f60:
	ldr r2, .L_08192100
	movs r4, #7
	mov r3, r10
	ands r3, r4
	ldrb r6, [r2, r3]
	ldr r2, .L_08192104
	lsls r0, r6, #1
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #32]
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r7, r3]
	asrs r3, r6, #1
	subs r2, r2, r3
	movs r4, #6
	ldrsh r3, [r7, r4]
	str r0, [sp, #4]
	subs r3, r3, r6
	str r6, [sp, #0]
	ldr r4, [sp, #52]
	ldr r0, [sp, #36]
	mov lr, r4
	.2byte 0xf800
	ldr r1, [sp, #12]
	lsls r0, r6, #14
	mov r8, r0
	cmp r1, #28
	bhi .L_08191fde
	ldr r3, [r7, #12]
	cmp r3, #0
	bne .L_08191fde
	movs r4, #6
	ldrsh r3, [r7, r4]
	cmp r3, #113
	bgt .L_08191fda
	bl Random16
	movs r5, #63
	ands r5, r0
	ldr r3, [r7]
	movs r0, #224
	lsls r0, r0, #14
	lsls r5, r5, #16
	adds r5, r5, r0
	str r5, [r7, #12]
	subs r5, r5, r3
	ldr r3, [r7, #4]
	movs r0, #228
	lsls r0, r0, #15
	subs r0, r0, r3
	mov r1, r8
	bl Math_Div
	adds r1, r0, #0
	adds r0, r5, #0
	bl Math_Div
	lsls r6, r6, #14
	str r0, [r7, #12]
	b .L_08191fde
.L_08191fda:
	lsls r6, r6, #14
	mov r8, r6
.L_08191fde:
	mov r3, r9
	subs r3, #142
	cmp r3, #19
	bhi .L_08192046
	ldr r3, [r7]
	ldr r1, .L_08192108
	adds r5, r3, r1
	cmp r5, #0
	bge .L_08191ff2
	negs r5, r5
.L_08191ff2:
	bl Random16
	ldr r2, [r7, #4]
	lsrs r3, r2, #31
	adds r2, r2, r3
	movs r3, #31
	asrs r2, r2, #1
	ands r3, r0
	lsls r3, r3, #16
	subs r2, r2, r5
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #15
	adds r1, r2, r3
	cmp r1, #0
	bge .L_0819201a
	movs r4, #192
	lsls r4, r4, #15
	adds r4, #31
	adds r1, r2, r4
.L_0819201a:
	ldr r3, [r7]
	ldr r0, .L_0819210c
	asrs r2, r1, #5
	cmp r3, r0
	bgt .L_0819202a
	cmp r2, #0
	ble .L_0819202a
	negs r2, r2
.L_0819202a:
	movs r1, #192
	lsls r1, r1, #15
	cmp r3, r1
	ble .L_08192038
	cmp r2, #0
	bge .L_08192038
	negs r2, r2
.L_08192038:
	cmp r2, #0
	bge .L_0819203e
	adds r2, #7
.L_0819203e:
	ldr r3, [r7, #12]
	asrs r2, r2, #3
	adds r3, r3, r2
	str r3, [r7, #12]
.L_08192046:
	ldr r0, [r7, #4]
	asrs r0, r0, #5
	bl Trig_Sin
	movs r1, #5
	bl Math_Div
	ldr r2, [r7, #12]
	ldr r3, [r7]
	adds r2, r2, r0
	adds r3, r3, r2
	ldr r2, [r7, #16]
	str r3, [r7]
	ldr r3, [r7, #4]
	add r2, r8
	adds r3, r3, r2
	movs r2, #228
	lsls r2, r2, #15
	str r3, [r7, #4]
	cmp r3, r2
	ble .L_081920c8
	mov r3, r9
	cmp r3, #215
	ble .L_081920a2
	movs r3, #240
	movs r4, #0
	lsls r3, r3, #15
	str r4, [r7, #24]
	str r3, [r7]
	bl Random16
	movs r3, #127
	ands r3, r0
	subs r3, #32
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Random16
	movs r3, #3
	ands r3, r0
	subs r3, #8
	lsls r3, r3, #16
	movs r0, #0
	str r3, [r7, #12]
	str r0, [r7, #16]
	b .L_081920c8
.L_081920a2:
	bl Random16
	movs r5, #255
	ands r0, r5
	str r0, [r7, #24]
	bl Random16
	ldr r3, .L_08192110
	ands r0, r5
	subs r0, #64
	movs r1, #0
	lsls r0, r0, #16
	str r0, [r7]
	str r3, [r7, #4]
	str r1, [r7, #12]
	str r1, [r7, #16]
	b .L_081920c8
.L_081920c4:
	subs r3, #1
	str r3, [r7, #24]
.L_081920c8:
	ldr r2, [sp, #16]
	cmp r2, #28
	bhi .L_081920e0
	ldr r3, [r7, #12]
	ldr r4, .L_08192114
	movs r0, #128
	adds r3, r3, r4
	str r3, [r7, #12]
	ldr r3, [r7, #16]
	lsls r0, r0, #6
	adds r3, r3, r0
	str r3, [r7, #16]
.L_081920e0:
	movs r1, #1
	movs r2, #128
	add r10, r1
	lsls r2, r2, #1
	adds r7, #28
	cmp r10, r2
	beq .L_081920f0
	b .L_08191f58
.L_081920f0:
	b .L_0819211e
	.2byte 0x0000
.L_081920f4:
	.4byte Func_08143000
.L_081920f8:
	.4byte gInput
.L_081920fc:
	.4byte gMapCellBuffer
.L_08192100:
	.4byte Data_08196f28
.L_08192104:
	.4byte Data_08197410
.L_08192108:
	.4byte 0xffa00000
.L_0819210c:
	.4byte 0x005fffff
.L_08192110:
	.4byte 0xfff80000
.L_08192114:
	.4byte 0xffffe000
.L_08192118:
	mov r3, r9
	subs r3, #91
	str r3, [sp, #12]
.L_0819211e:
	ldr r4, [sp, #12]
	cmp r4, #164
	bhi .L_081921e4
	movs r0, #0
	mov r10, r0
	movs r7, #0
	mov r5, r11
	movs r6, #0
.L_0819212e:
	ldr r4, [r5, #20]
	cmp r4, #2
	beq .L_081921d6
	ldr r1, [r5, #24]
	cmp r1, #23
	bhi .L_08192174
	asrs r1, r1, #2
	lsls r1, r1, #11
	movs r2, #224
	lsls r2, r2, #3
	add r1, r11
	adds r1, r1, r2
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r0, #6
	ldrsh r3, [r5, r0]
	movs r0, #32
	str r0, [sp, #0]
	movs r0, #64
	str r0, [sp, #4]
	ldr r0, [sp, #28]
	lsls r4, r4, #2
	ldr r4, [r4, r0]
	ldr r0, [sp, #36]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5]
	ldr r2, [r5, #12]
	ldr r1, [r5, #24]
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #16]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
.L_08192174:
	adds r3, r1, #1
	str r3, [r5, #24]
	cmp r3, #24
	bne .L_081921b6
	mov r1, r10
	adds r3, r6, r1
	adds r3, #44
	lsls r3, r3, #16
	str r3, [r5]
	movs r3, #144
	lsls r3, r3, #15
	str r3, [r5, #4]
	str r7, [r5, #24]
	bl Random16
	movs r3, #3
	ands r3, r0
	adds r3, #2
	negs r3, r3
	lsls r3, r3, #16
	str r3, [r5, #16]
	mov r3, r9
	subs r3, #97
	str r7, [r5, #20]
	cmp r3, #42
	bhi .L_081921ac
	movs r3, #1
	b .L_081921b4
.L_081921ac:
	mov r2, r9
	cmp r2, #140
	ble .L_081921b6
	movs r3, #2
.L_081921b4:
	str r3, [r5, #20]
.L_081921b6:
	mov r3, r9
	cmp r3, #136
	bne .L_081921d6
	ldr r3, [r5]
	ldr r4, .L_08192508
	cmp r3, r4
	bgt .L_081921cc
	ldr r3, [r5, #12]
	ldr r0, .L_0819250c
	adds r3, r3, r0
	b .L_081921d4
.L_081921cc:
	ldr r3, [r5, #12]
	movs r1, #128
	lsls r1, r1, #11
	adds r3, r3, r1
.L_081921d4:
	str r3, [r5, #12]
.L_081921d6:
	movs r2, #1
	add r10, r2
	mov r3, r10
	adds r5, #28
	adds r6, #3
	cmp r3, #12
	bne .L_0819212e
.L_081921e4:
	mov r3, r9
	subs r3, #200
	cmp r3, #71
	bhi .L_08192270
	mov r4, r9
	cmp r4, #200
	ble .L_08192270
	movs r5, #168
	movs r0, #0
	lsls r5, r5, #2
	mov r10, r0
	add r5, r11
.L_081921fc:
	ldr r1, [r5, #24]
	cmp r1, #23
	bhi .L_08192238
	asrs r1, r1, #2
	lsls r1, r1, #11
	movs r2, #224
	lsls r2, r2, #3
	movs r0, #32
	add r1, r11
	adds r1, r1, r2
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r4, #6
	ldrsh r3, [r5, r4]
	str r0, [sp, #0]
	movs r0, #64
	str r0, [sp, #4]
	ldr r4, [sp, #52]
	ldr r0, [sp, #36]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5]
	ldr r2, [r5, #12]
	ldr r1, [r5, #24]
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #16]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
.L_08192238:
	adds r3, r1, #1
	str r3, [r5, #24]
	cmp r3, #24
	bne .L_08192264
	movs r3, #216
	lsls r3, r3, #14
	str r3, [r5]
	movs r3, #224
	lsls r3, r3, #14
	str r3, [r5, #4]
	bl Random16
	movs r3, #3
	ands r3, r0
	movs r0, #128
	lsls r3, r3, #16
	lsls r0, r0, #10
	adds r3, r3, r0
	negs r3, r3
	str r3, [r5, #12]
	movs r3, #0
	str r3, [r5, #24]
.L_08192264:
	movs r1, #1
	add r10, r1
	mov r2, r10
	adds r5, #28
	cmp r2, #16
	bne .L_081921fc
.L_08192270:
	mov r3, r9
	cmp r3, #135
	ble .L_0819235c
	movs r0, #128
	lsls r0, r0, #3
	bl Runtime_BumpAllocateAlternatePool
	mov r8, r0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_08192510
	ldr r3, [sp, #44]
	movs r4, #7
	ands r3, r2
	ldr r2, .L_08192514
	orrs r3, r4
	ands r3, r2
	movs r2, #160
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #44]
	movs r3, #220
	lsls r3, r3, #6
	add r3, r11
	add r2, sp, #44
	str r3, [r2, #4]
	ldr r3, .L_08192518
	adds r6, r0, #0
	str r3, [r6, #8]
	mov r0, r8
	movs r3, #0
	str r4, [r6]
	str r2, [r6, #16]
	str r0, [r6, #12]
	strb r3, [r6, #24]
	strb r3, [r6, #25]
	mov r10, r3
	mov r7, r9
.L_081922be:
	ldr r3, .L_0819251c
	mov r1, r10
	ldrb r3, [r3, r1]
	adds r1, r3, #0
	adds r1, #136
	cmp r9, r1
	ble .L_08192344
	mov r3, r9
	subs r2, r3, r1
	ldr r3, .L_08192520
	mov r4, r10
	ldrb r3, [r3, r4]
	movs r0, #128
	muls r2, r3
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r2
	lsls r3, r3, #4
	lsls r0, r0, #8
	mov r2, r9
	adds r5, r3, r0
	subs r3, r1, r2
	lsls r3, r3, #3
	adds r3, #56
	cmp r3, #0
	ble .L_081922f6
	movs r3, #0
.L_081922f6:
	movs r4, #64
	negs r4, r4
	cmp r3, r4
	ble .L_08192344
	str r3, [r6, #20]
	bl Func_08014de4
	ldr r3, .L_08192524
	mov r0, r10
	ldrsb r1, [r3, r0]
	movs r0, #128
	lsls r1, r1, #16
	lsls r0, r0, #13
	movs r2, #0
	bl Func_08015160
	adds r1, r5, #0
	adds r2, r5, #0
	adds r0, r5, #0
	bl Func_080151e4
	movs r0, #176
	lsls r0, r0, #4
	adds r0, #184
	bl SceneTransform_ApplyPitch
	movs r1, #7
	adds r3, r7, #0
	ands r3, r1
	lsls r3, r3, #4
	ldr r0, .L_08192528
	strb r3, [r6, #24]
	mov r1, r8
	movs r2, #32
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_08192344:
	movs r2, #1
	add r10, r2
	mov r3, r10
	adds r7, #5
	cmp r3, #4
	bne .L_081922be
	adds r0, r6, #0
	bl Sys_Free
	mov r0, r8
	bl Sys_Free
.L_0819235c:
	mov r4, r9
	cmp r4, #139
	ble .L_0819236c
	movs r0, #5
	ldr r1, [sp, #20]
	ldr r2, [sp, #24]
	bl Func_0816442c
.L_0819236c:
	movs r0, #0
	mov r10, r0
.L_08192370:
	mov r1, r10
	lsls r3, r1, #3
	adds r3, #212
	cmp r9, r3
	bne .L_081923da
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #240
	mov r2, r11
	ldr r3, [r2, r3]
	movs r7, #0
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_081923da
	movs r6, #240
	lsls r6, r6, #7
	lsls r1, r1, #4
	adds r6, #240
	movs r4, #128
	mov r8, r1
	add r6, r11
	lsls r4, r4, #9
	movs r5, #36
.L_0819239e:
	ldr r3, [r6]
	movs r2, #5
	ldrsh r0, [r3, r5]
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #7
	adds r3, r7, #0
	str r4, [sp, #8]
	bl Func_0814cd48
	ldr r2, [sp, #40]
	ldr r4, [sp, #8]
	ldrsh r0, [r5, r2]
	mov r3, r8
	adds r3, #100
	str r3, [sp, #4]
	movs r3, #128
	adds r2, r4, #0
	lsls r3, r3, #11
	movs r1, #1
	str r4, [sp, #0]
	bl Func_0815f000
	ldr r3, [r6]
	adds r7, #1
	ldr r3, [r3, #20]
	adds r5, #2
	ldr r4, [sp, #8]
	cmp r7, r3
	bne .L_0819239e
.L_081923da:
	movs r4, #1
	add r10, r4
	mov r0, r10
	cmp r0, #8
	bne .L_08192370
	mov r3, r9
	subs r3, #201
	cmp r3, #54
	bhi .L_081923f8
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r11
	movs r3, #2
	str r3, [r2]
.L_081923f8:
	mov r1, r9
	cmp r1, #32
	bne .L_08192404
	movs r0, #246
	bl Audio_PlayCue
.L_08192404:
	mov r2, r9
	cmp r2, #90
	bne .L_08192410
	movs r0, #225
	bl Audio_PlayCue
.L_08192410:
	mov r3, r9
	cmp r3, #140
	bne .L_0819241c
	movs r0, #154
	bl Audio_PlayCue
.L_0819241c:
	mov r4, r9
	cmp r4, #200
	bne .L_08192428
	movs r0, #208
	bl Audio_PlayCue
.L_08192428:
	mov r0, r9
	cmp r0, #232
	bne .L_08192434
	movs r0, #144
	bl Audio_PlayCue
.L_08192434:
	movs r1, #136
	lsls r1, r1, #1
	cmp r9, r1
	bne .L_0819245e
	movs r0, #134
	bl Func_081180e8
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #3
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	ldr r3, .L_0819252c
	adds r2, #132
	add r2, r11
	str r3, [r2]
	bl Func_0815b410
	b .L_08192492
.L_0819245e:
	ldr r3, .L_08192510
	add r3, r9
	cmp r3, #15
	bhi .L_08192492
	movs r3, #239
	lsls r3, r3, #7
	add r3, r11
	movs r2, #1
	str r2, [r3]
	movs r2, #238
	ldr r3, .L_08192530
	lsls r2, r2, #7
	adds r2, #132
	add r2, r11
	str r3, [r2]
	movs r3, #14
	adds r3, #255
	cmp r9, r3
	ble .L_08192488
	ldr r3, .L_08192534
	str r3, [r2]
.L_08192488:
	movs r0, #2
	movs r1, #2
	movs r2, #2
	bl Func_08164a4c
.L_08192492:
	movs r0, #4
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r11
	movs r3, #1
	movs r4, #1
	movs r0, #1
	str r3, [r2]
	add r9, r4
	bl WaitFrames
	movs r0, #148
	lsls r0, r0, #1
	cmp r9, r0
	beq .L_081924be
	b .L_08191e36
.L_081924be:
	ldr r0, .L_08192538
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r5, #238
	ldr r1, [sp, #20]
	movs r0, #5
	ldr r2, [sp, #24]
	lsls r5, r5, #7
	bl Func_0816467c
	adds r5, #220
	movs r1, #0
	mov r10, r1
	add r5, r11
.L_081924e6:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #12
	bne .L_081924e6
	bl Func_08143bb8
	add sp, #60
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08192508:
	.4byte 0x004fffff
.L_0819250c:
	.4byte 0xfffc0000
.L_08192510:
	.4byte 0xffffff00
.L_08192514:
	.4byte 0xffff00ff
.L_08192518:
	.4byte Data_08198ec4
.L_0819251c:
	.4byte Data_08199f34
.L_08192520:
	.4byte Data_08199f38
.L_08192524:
	.4byte Data_08199f3c
.L_08192528:
	.4byte Data_08198cac
.L_0819252c:
	.4byte Data_02020202
.L_08192530:
	.4byte 0x10101010
.L_08192534:
	.4byte 0x3f3f3f3f
.L_08192538:
	.4byte Func_08143000
