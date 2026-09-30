.syntax unified
	.thumb
	.global Func_08145df8
	.thumb_func
Func_08145df8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #160
	str r0, [sp, #76]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #92]
	movs r0, #1
	str r1, [sp, #72]
	ldr r2, [r3, #96]
	str r2, [sp, #68]
	ldr r5, [r3, #100]
	ldr r3, [r3, #48]
	str r3, [sp, #56]
	bl BattleFx_BeginCanvasLayer
	ldr r1, [sp, #76]
	ldr r3, [r1, #28]
	cmp r3, #1
	bne .L_08145e3c
	ldr r1, [r1, #4]
	movs r3, #3
	lsls r1, r1, #4
	orrs r1, r3
	add r2, sp, #148
	add r3, sp, #136
	ldr r0, [sp, #76]
	bl Func_0815585c
.L_08145e3c:
	ldr r3, .L_08145e7c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r2, [sp, #72]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_08145e80
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	ldr r0, .L_08145e84
	adds r1, r5, #0
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r2, [sp, #72]
	movs r3, #184
	lsls r3, r3, #5
	adds r1, r2, r3
	ldr r0, .L_08145e88
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r2, [sp, #72]
	b .L_08145e8c
	.2byte 0x0000
.L_08145e7c:
	.4byte 0x00000100
.L_08145e80:
	.4byte 0x0000013a
.L_08145e84:
	.4byte 0x00000134
.L_08145e88:
	.4byte 0x00000137
.L_08145e8c:
	movs r3, #156
	lsls r3, r3, #6
	adds r1, r2, r3
	ldr r0, .L_08146250
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r1, [sp, #72]
	movs r3, #239
	lsls r3, r3, #7
	adds r2, r1, r3
	movs r3, #3
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r1, r3
	ldr r3, .L_08146254
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08146258
	bl Scheduler_AddOrUpdateCallback
	ldr r2, [sp, #76]
	mov r3, sp
	adds r3, #124
	movs r1, #36
	ldrsh r0, [r2, r1]
	adds r1, r3, #0
	str r3, [sp, #48]
	bl Func_0815e20c
	ldr r1, [sp, #48]
	movs r3, #64
	ldr r2, [r1]
	movs r0, #142
	subs r3, r3, r2
	movs r2, #128
	lsls r2, r2, #19
	str r3, [sp, #52]
	adds r2, #40
	lsls r3, r3, #8
	str r3, [r2]
	bl Audio_PlayCue
	movs r2, #0
	ldr r3, [sp, #76]
	str r2, [sp, #64]
	movs r1, #72
	ldr r2, [r3, #20]
	negs r1, r1
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #2
	cmp r3, r1
	bne .L_08145f02
	b .L_08146244
.L_08145f02:
	mov r2, sp
	adds r2, #80
	str r2, [sp, #24]
.L_08145f08:
	ldr r3, [sp, #64]
	cmp r3, #64
	bne .L_08145f14
	movs r0, #0
	bl Func_081180e8
.L_08145f14:
	movs r1, #170
	movs r2, #170
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #171
	movs r3, #0
	ldr r0, [sp, #64]
	adds r2, #85
	bl Func_081496c8
	ldr r1, [sp, #76]
	ldr r3, [r1, #28]
	cmp r3, #1
	bne .L_08145fbc
	ldr r2, [sp, #64]
	lsls r6, r2, #11
	adds r0, r6, #0
	bl Trig_Sin
	add r5, sp, #148
	lsls r3, r0, #2
	ldr r2, [r5]
	adds r3, r3, r0
	ldr r1, [sp, #52]
	lsls r3, r3, #2
	asrs r3, r3, #16
	adds r3, r3, r2
	adds r3, r3, r1
	subs r3, #20
	adds r0, r6, #0
	mov r8, r3
	bl Trig_Cos
	ldr r3, [r5, #4]
	ldr r2, [sp, #76]
	lsls r0, r0, #2
	asrs r0, r0, #16
	adds r0, r0, r3
	adds r6, r0, #0
	ldr r1, [sp, #24]
	ldr r0, [r2, #4]
	bl Func_08144aac
	ldr r3, [sp, #64]
	subs r6, #24
	cmp r3, #32
	ble .L_08145f7a
	lsls r3, r3, #1
	subs r3, r6, r3
	adds r6, r3, #0
	adds r6, #64
.L_08145f7a:
	ldr r1, [sp, #72]
	movs r2, #156
	lsls r2, r2, #6
	adds r7, r1, r2
	movs r5, #40
	adds r3, r6, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	ldr r4, [sp, #80]
	ldr r0, [sp, #68]
	adds r1, r7, #0
	mov r2, r8
	mov lr, r4
	.2byte 0xf800
	ldr r3, [sp, #64]
	cmp r3, #3
	bgt .L_08145fb0
	ldr r1, [sp, #24]
	str r5, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #68]
	ldr r4, [r1, #4]
	mov r2, r8
	adds r1, r7, #0
	adds r3, r6, #0
	mov lr, r4
	.2byte 0xf800
.L_08145fb0:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
.L_08145fbc:
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r0, #188
	movs r1, #23
	str r3, [sp, #80]
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r3, [r3]
	ldr r2, [sp, #24]
	str r3, [r2, #4]
	ldr r3, [sp, #64]
	cmp r3, #16
	ble .L_08146002
	ldr r1, [sp, #64]
	movs r3, #15
	ands r3, r1
	cmp r3, #0
	bne .L_08146002
	ldr r3, [sp, #72]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #132
	adds r2, r3, r1
	ldr r3, [r2]
	ldr r1, .L_0814625c
	adds r3, r3, r1
	str r3, [r2]
.L_08146002:
	movs r2, #0
	ldr r3, [sp, #72]
	str r2, [sp, #60]
	str r2, [sp, #16]
	ldr r2, [sp, #64]
	movs r1, #224
	lsls r1, r1, #3
	adds r1, r3, r1
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #9
	str r3, [sp, #12]
	movs r3, #36
	str r1, [sp, #28]
	str r3, [sp, #8]
	mov r11, r2
.L_08146022:
	ldr r1, [sp, #8]
	ldr r3, [sp, #76]
	ldrsh r0, [r1, r3]
	bl GetBattleObjectSlotFar
	mov r1, r11
	ldr r6, [r0]
	cmp r1, #95
	bls .L_08146036
	b .L_081461e4
.L_08146036:
	bl Func_08014de4
	ldr r0, [sp, #56]
	adds r1, r0, #0
	adds r1, #12
	bl Func_080156e8
	ldr r3, [r6, #8]
	add r5, sp, #112
	str r3, [r5]
	movs r2, #100
	ldr r3, [r6, #12]
	add r2, sp
	str r3, [r5, #4]
	mov r9, r2
	ldr r3, [r6, #16]
	mov r1, r9
	str r3, [r5, #8]
	adds r0, r5, #0
	bl Func_0815e1ec
	ldr r1, [sp, #48]
	ldr r2, [sp, #52]
	ldr r3, [r1]
	mov r1, r9
	adds r3, r3, r2
	str r3, [r1]
	ldr r3, [r1, #4]
	mov r2, r11
	subs r3, #24
	str r3, [r1, #4]
	cmp r2, #67
	ble .L_0814607a
	b .L_081461a8
.L_0814607a:
	movs r1, #0
	str r1, [sp, #20]
	ldr r1, [sp, #16]
	movs r3, #168
	lsls r2, r1, #3
	subs r2, r2, r1
	ldr r1, [sp, #72]
	lsls r2, r2, #2
	adds r5, r2, r1
	ldr r2, [sp, #12]
	lsls r3, r3, #10
	subs r2, r3, r2
	mov r1, r11
	movs r3, #64
	subs r3, r3, r1
	lsls r3, r3, #9
	movs r7, #0
	add r6, sp, #88
	mov r8, r2
	mov r10, r3
.L_081460a2:
	bl Func_08014de4
	mov r2, r11
	cmp r2, #63
	bgt .L_081460c6
	mov r3, r8
	str r3, [r6]
	str r3, [r6, #4]
	str r3, [r6, #8]
	adds r0, r6, #0
	bl Func_080151ac
	mov r0, r10
	bl Func_080150e4
	mov r0, r10
	bl Func_08015068
.L_081460c6:
	ldr r0, [sp, #20]
	bl Func_080150e4
	add r2, sp, #112
	adds r1, r2, #0
	ldr r0, .L_08146260
	bl Func_0815e1ec
	mov r1, r9
	ldr r3, [r1]
	ldr r2, [sp, #112]
	adds r7, #1
	adds r2, r2, r3
	str r2, [r5, #12]
	ldr r3, [sp, #116]
	ldr r2, [r1, #4]
	adds r3, r3, r2
	adds r3, #16
	str r3, [r5, #16]
	ldr r2, [sp, #20]
	movs r3, #170
	lsls r3, r3, #7
	adds r3, #85
	adds r2, r2, r3
	str r2, [sp, #20]
	adds r5, #28
	cmp r7, #3
	bne .L_081460a2
	ldr r1, [sp, #16]
	movs r7, #0
	str r1, [sp, #40]
.L_08146104:
	ldr r3, [sp, #40]
	ldr r1, [sp, #72]
	adds r2, r7, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r7, #1
	lsls r3, r3, #2
	adds r3, r1, r3
	adds r0, r7, #0
	movs r1, #3
	str r3, [sp, #44]
	str r7, [sp, #32]
	bl __modsi3
	ldr r2, [sp, #40]
	ldr r1, [sp, #72]
	adds r0, r0, r2
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #2
	adds r3, r1, r3
	mov r2, r11
	str r3, [sp, #36]
	cmp r2, #0
	bge .L_08146138
	adds r2, #15
.L_08146138:
	asrs r2, r2, #4
	movs r3, #5
	subs r3, r3, r2
	movs r2, #0
	mov r8, r3
	mov r10, r2
	lsls r7, r3, #1
.L_08146146:
	ldr r1, [sp, #36]
	ldr r2, [sp, #44]
	ldr r3, [r1, #12]
	ldr r6, [r2, #12]
	movs r1, #24
	subs r3, r3, r6
	mov r0, r10
	muls r0, r3
	bl Math_Div
	ldr r1, [sp, #36]
	ldr r2, [sp, #44]
	ldr r3, [r1, #16]
	ldr r5, [r2, #16]
	adds r6, r6, r0
	subs r3, r3, r5
	mov r0, r10
	muls r0, r3
	movs r1, #24
	bl Math_Div
	ldr r2, .L_08146264
	subs r3, r7, #2
	ldrh r1, [r2, r3]
	ldr r3, [sp, #72]
	adds r5, r5, r0
	adds r1, r3, r1
	movs r2, #184
	mov r3, r8
	lsls r2, r2, #5
	subs r6, r6, r3
	subs r5, r5, r3
	adds r1, r1, r2
	str r7, [sp, #0]
	adds r2, r6, #0
	str r7, [sp, #4]
	ldr r4, [sp, #80]
	ldr r0, [sp, #68]
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
	movs r1, #1
	add r10, r1
	mov r2, r10
	cmp r2, #24
	bne .L_08146146
	ldr r7, [sp, #32]
	cmp r7, #3
	bne .L_08146104
.L_081461a8:
	mov r3, r11
	cmp r3, #63
	ble .L_081461e4
	mov r1, r9
	ldr r2, [r1]
	ldr r3, [r1, #4]
	movs r5, #48
	movs r1, #24
	subs r2, #24
	subs r3, #24
	str r1, [sp, #0]
	ldr r4, [sp, #80]
	ldr r1, [sp, #28]
	str r5, [sp, #4]
	ldr r0, [sp, #68]
	mov lr, r4
	.2byte 0xf800
	mov r3, r9
	movs r1, #24
	ldr r2, [r3]
	ldr r3, [r3, #4]
	str r1, [sp, #0]
	str r5, [sp, #4]
	ldr r1, [sp, #24]
	subs r3, #24
	ldr r4, [r1, #4]
	ldr r0, [sp, #68]
	ldr r1, [sp, #28]
	mov lr, r4
	.2byte 0xf800
.L_081461e4:
	ldr r3, [sp, #12]
	ldr r1, .L_08146268
	ldr r2, [sp, #16]
	adds r3, r3, r1
	str r3, [sp, #12]
	ldr r1, [sp, #60]
	ldr r3, [sp, #8]
	adds r2, #32
	str r2, [sp, #16]
	movs r2, #8
	negs r2, r2
	adds r3, #2
	adds r1, #1
	add r11, r2
	str r3, [sp, #8]
	str r1, [sp, #60]
	cmp r1, #1
	beq .L_0814620a
	b .L_08146022
.L_0814620a:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r1, #240
	ldr r2, [sp, #72]
	lsls r1, r1, #7
	adds r1, #232
	adds r3, r2, r1
	ldr r2, [sp, #60]
	movs r0, #1
	str r2, [r3]
	bl WaitFrames
	ldr r3, [sp, #64]
	ldr r1, [sp, #76]
	adds r3, #1
	str r3, [sp, #64]
	ldr r2, [r1, #20]
	lsls r3, r2, #2
	adds r3, r3, r2
	ldr r2, [sp, #64]
	lsls r3, r3, #2
	adds r3, #72
	cmp r2, r3
	beq .L_08146244
	b .L_08145f08
.L_08146244:
	ldr r0, .L_08146258
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	b .L_0814626c
.L_08146250:
	.4byte 0x00000152
.L_08146254:
	.4byte 0x04040404
.L_08146258:
	.4byte Func_08143000
.L_0814625c:
	.4byte 0x01010101
.L_08146260:
	.4byte Data_0819790c
.L_08146264:
	.4byte Data_08197424
.L_08146268:
	.4byte 0xffffd000
.L_0814626c:
	add sp, #160
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
