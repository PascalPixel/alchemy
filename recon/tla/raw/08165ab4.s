.syntax unified
	.thumb
	.global Func_08165ab4
	.thumb_func
Func_08165ab4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #96
	str r0, [sp, #60]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #96]
	movs r6, #240
	str r0, [sp, #56]
	movs r0, #0
	ldr r1, [r3, #92]
	ldr r3, [r3, #100]
	mov r11, r1
	str r3, [sp, #48]
	bl BattleFx_BeginCanvasLayer
	bl Func_0813ba50
	ldr r2, .L_08165b20
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #0
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08165b24
	bl Scheduler_AddOrUpdateCallback
	movs r1, #0
	movs r0, #1
	bl Func_08163c2c
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #240
	add r3, r11
	ldr r0, [r3]
	bl Func_0814cc4c
	movs r1, #162
	movs r2, #2
	lsls r1, r1, #2
	b .L_08165b28
.L_08165b20:
	.4byte 0x00000000
.L_08165b24:
	.4byte Func_08143000
.L_08165b28:
	movs r0, #9
	bl Func_08152404
	movs r2, #13
	negs r2, r2
	movs r7, #0
	lsls r6, r6, #7
	mov r8, r2
.L_08165b38:
	movs r0, #199
	lsls r0, r0, #1
	adds r0, #255
	bl GetBattleEffectObject
	mov r3, r11
	adds r5, r0, #0
	str r5, [r6, r3]
	cmp r5, #0
	beq .L_08165b70
	movs r3, #0
	strb r3, [r5, #26]
	movs r1, #3
	adds r0, r7, #0
	bl __modsi3
	adds r1, r0, #0
	adds r0, r5, #0
	bl Animation_ApplyChildArgumentFar
	mov r0, r11
	ldr r1, [r6, r0]
	mov r2, r8
	ldrb r3, [r1, #9]
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
.L_08165b70:
	adds r7, #1
	adds r6, #4
	cmp r7, #6
	bne .L_08165b38
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	movs r5, #192
	lsls r5, r5, #18
	ldr r3, [r5, #104]
	movs r1, #3
	movs r0, #188
	str r3, [sp, #72]
	bl Func_081963ec
	adds r5, #188
	ldr r3, [r5]
	mov r6, sp
	adds r6, #72
	str r6, [sp, #28]
	movs r2, #128
	str r3, [r6, #4]
	ldr r3, .L_08165bd8
	lsls r2, r2, #19
	adds r2, #72
	strh r3, [r2]
	ldr r3, .L_08165bdc
	subs r2, #8
	strh r3, [r2]
	ldr r3, .L_08165be0
	adds r2, #6
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldr r1, .L_08165be4
	movs r0, #1
	bl Func_08118040
	movs r0, #1
	movs r1, #1
	bl Func_08163c2c
	ldr r0, .L_08165be8
	ldr r1, [sp, #48]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	b .L_08165bec
.L_08165bd8:
	.4byte 0x00002737
.L_08165bdc:
	.4byte 0x000000f0
.L_08165be0:
	.4byte 0x00001088
.L_08165be4:
	.4byte 0x00000073
.L_08165be8:
	.4byte 0x00000134
.L_08165bec:
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_08165c44
	add r1, r11
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	ldr r3, .L_08165c34
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_08165c38
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_08165c3c
	adds r2, #50
	strh r3, [r2]
	ldr r3, .L_08165c40
	subs r2, #2
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
	movs r0, #188
	movs r3, #184
	movs r1, #160
	b .L_08165c48
.L_08165c34:
	.4byte 0x00007741
.L_08165c38:
	.4byte 0x00000080
.L_08165c3c:
	.4byte 0x00001010
.L_08165c40:
	.4byte 0x00003f44
.L_08165c44:
	.4byte 0x00000184
.L_08165c48:
	lsls r3, r3, #15
	lsls r0, r0, #16
	lsls r1, r1, #16
	str r0, [sp, #40]
	str r3, [sp, #44]
	str r1, [sp, #32]
	str r3, [sp, #36]
	movs r7, #0
	movs r6, #0
	movs r2, #0
	mov r5, r11
.L_08165c5e:
	str r2, [sp, #12]
	bl Random16
	movs r3, #127
	ands r3, r0
	lsls r3, r3, #16
	str r3, [r5]
	ldr r2, [sp, #12]
	ldr r3, .L_08165f78
	adds r7, #1
	str r2, [r5, #4]
	str r6, [r5, #12]
	str r6, [r5, #16]
	str r6, [r5, #24]
	adds r2, r2, r3
	adds r5, #28
	cmp r7, #6
	bne .L_08165c5e
	mov r3, r11
	movs r7, #0
	movs r2, #24
	adds r3, #192
.L_08165c8a:
	adds r7, #1
	str r2, [r3]
	adds r3, #28
	cmp r7, #58
	bne .L_08165c8a
	ldr r3, .L_08165f7c
	movs r1, #1
	movs r2, #128
	movs r7, #0
	negs r1, r1
	lsls r2, r2, #3
.L_08165ca0:
	adds r7, #1
	str r1, [r3]
	adds r3, #28
	cmp r7, r2
	bne .L_08165ca0
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #180
	add r2, r11
	movs r3, #24
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #184
	movs r3, #0
	add r2, r11
	str r3, [r2]
	str r3, [sp, #52]
	ldr r3, .L_08165f80
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_08165cd2
	b .L_0816617c
.L_08165cd2:
	mov r6, sp
	mov r0, sp
	adds r6, #80
	adds r0, #64
	str r6, [sp, #20]
	str r0, [sp, #24]
.L_08165cde:
	ldr r1, [sp, #52]
	cmp r1, #94
	bne .L_08165cea
	movs r0, #156
	bl Audio_PlayCue
.L_08165cea:
	ldr r2, [sp, #52]
	cmp r2, #136
	bne .L_08165cf6
	movs r0, #156
	bl Audio_PlayCue
.L_08165cf6:
	ldr r3, [sp, #52]
	cmp r3, #178
	bne .L_08165d02
	movs r0, #156
	bl Audio_PlayCue
.L_08165d02:
	ldr r6, [sp, #52]
	movs r0, #130
	lsls r0, r0, #1
	cmp r6, r0
	bne .L_08165d12
	movs r0, #145
	bl Audio_PlayCue
.L_08165d12:
	ldr r3, .L_08165f84
	ldr r4, [r3, #4]
	ldr r3, [r3]
	str r3, [sp, #64]
	str r4, [sp, #68]
	ldr r3, [sp, #52]
	subs r3, #96
	cmp r3, #155
	bls .L_08165d2e
	ldr r1, [sp, #52]
	ldr r2, .L_08165f88
	adds r3, r1, r2
	cmp r3, #3
	bhi .L_08165d3a
.L_08165d2e:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r11
	movs r3, #1
	str r3, [r2]
.L_08165d3a:
	movs r3, #0
	movs r6, #238
	lsls r6, r6, #7
	str r3, [sp, #92]
	str r3, [sp, #84]
	ldr r5, [sp, #20]
	ldr r4, .L_08165f8c
	adds r6, #220
	movs r7, #0
	add r6, r11
.L_08165d4e:
	ldr r3, .L_08165f90
	ldr r0, [sp, #40]
	ldrb r3, [r3, r7]
	ldr r1, [sp, #44]
	lsls r3, r3, #16
	adds r3, r3, r0
	adds r3, r3, r4
	str r3, [r5]
	ldr r3, .L_08165f94
	ldmia r6!, {r0}
	ldrb r3, [r3, r7]
	str r4, [sp, #8]
	lsls r3, r3, #16
	adds r3, r3, r1
	adds r3, r3, r4
	str r3, [r5, #8]
	adds r1, r5, #0
	ldr r2, [sp, #24]
	movs r3, #0
	bl Func_08020010
	adds r7, #1
	ldr r4, [sp, #8]
	cmp r7, #7
	bne .L_08165d4e
	ldr r2, [sp, #52]
	cmp r2, #90
	bgt .L_08165da8
	lsls r5, r2, #9
	adds r0, r5, #0
	bl Trig_Sin
	movs r3, #156
	lsls r0, r0, #4
	lsls r3, r3, #16
	adds r3, r0, r3
	adds r0, r5, #0
	str r3, [sp, #32]
	bl Trig_Cos
	movs r6, #184
	lsls r0, r0, #4
	lsls r6, r6, #15
	adds r6, r0, r6
	str r6, [sp, #36]
.L_08165da8:
	ldr r0, [sp, #52]
	cmp r0, #196
	bgt .L_08165e3e
	movs r7, #0
	movs r6, #91
	mov r8, r11
.L_08165db4:
	ldr r1, [sp, #52]
	cmp r1, r6
	blt .L_08165dca
	adds r3, r6, #4
	cmp r1, r3
	bge .L_08165dca
	ldr r2, [sp, #36]
	movs r3, #128
	lsls r3, r3, #12
	adds r3, r2, r3
	str r3, [sp, #36]
.L_08165dca:
	ldr r0, [sp, #52]
	adds r3, r6, #3
	cmp r0, r3
	bne .L_08165e1a
	movs r2, #255
	mov r5, r8
	movs r1, #0
	mov r10, r2
	adds r5, #168
.L_08165ddc:
	movs r3, #128
	lsls r3, r3, #15
	str r3, [r5]
	movs r3, #192
	lsls r3, r3, #15
	str r3, [r5, #4]
	str r1, [sp, #16]
	bl Random16
	mov r3, r10
	ands r0, r3
	subs r0, #127
	lsls r0, r0, #10
	str r0, [r5, #12]
	bl Random16
	mov r2, r10
	ands r0, r2
	subs r0, #127
	lsls r0, r0, #10
	str r0, [r5, #16]
	bl Random16
	ldr r1, [sp, #16]
	movs r3, #15
	ands r3, r0
	adds r1, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r1, #4
	bne .L_08165ddc
.L_08165e1a:
	ldr r0, [sp, #52]
	adds r3, r6, #0
	adds r3, #20
	cmp r0, r3
	blt .L_08165e32
	adds r3, #16
	cmp r0, r3
	bge .L_08165e32
	ldr r1, [sp, #36]
	ldr r2, .L_08165f98
	adds r2, r1, r2
	str r2, [sp, #36]
.L_08165e32:
	movs r3, #224
	adds r7, #1
	adds r6, #40
	add r8, r3
	cmp r7, #3
	bne .L_08165db4
.L_08165e3e:
	ldr r3, [sp, #52]
	subs r3, #244
	cmp r3, #7
	bhi .L_08165e4e
	ldr r6, [sp, #32]
	ldr r0, .L_08165f9c
	adds r0, r6, r0
	str r0, [sp, #32]
.L_08165e4e:
	ldr r3, [sp, #52]
	subs r3, #252
	cmp r3, #23
	bhi .L_08165e62
	ldr r3, [sp, #52]
	ldr r1, [sp, #32]
	subs r3, #250
	lsls r3, r3, #16
	subs r3, r1, r3
	str r3, [sp, #32]
.L_08165e62:
	ldr r2, [sp, #52]
	movs r3, #4
	adds r3, #255
	cmp r2, r3
	bgt .L_08165eb2
	ldr r0, [sp, #36]
	movs r3, #255
	lsls r3, r3, #24
	str r3, [sp, #84]
	adds r3, r0, r3
	str r3, [sp, #88]
	movs r3, #238
	lsls r3, r3, #7
	ldr r6, [sp, #32]
	adds r3, #248
	add r2, sp, #80
	add r3, r11
	ldr r0, [r3]
	adds r1, r2, #0
	movs r3, #0
	ldr r2, [sp, #24]
	str r6, [sp, #80]
	bl Func_08020010
	ldr r6, [sp, #32]
	movs r0, #128
	lsls r0, r0, #14
	adds r3, r6, r0
	str r3, [sp, #80]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #252
	add r3, r11
	add r2, sp, #80
	ldr r0, [r3]
	adds r1, r2, #0
	movs r3, #0
	ldr r2, [sp, #24]
	bl Func_08020010
.L_08165eb2:
	ldr r6, [sp, #20]
	movs r3, #0
	str r3, [r6, #4]
	movs r7, #0
	mov r9, r6
	mov r5, r11
	mov r10, r11
.L_08165ec0:
	ldr r3, [r5, #24]
	cmp r3, #2
	beq .L_08165fae
	ldr r3, [r5]
	mov r0, r9
	str r3, [r0]
	movs r1, #240
	ldr r3, [r5, #4]
	lsls r1, r1, #7
	str r3, [r0, #8]
	lsls r3, r7, #2
	adds r3, r3, r1
	mov r2, r11
	ldr r0, [r2, r3]
	mov r1, r9
	ldr r2, [sp, #24]
	movs r3, #0
	bl Func_08020010
	ldr r3, [r5]
	ldr r2, [r5, #12]
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #16]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [sp, #52]
	cmp r3, #96
	ble .L_08165f04
	movs r6, #128
	lsls r6, r6, #7
	adds r3, r2, r6
	str r3, [r5, #16]
.L_08165f04:
	ldr r3, [r5, #4]
	movs r0, #240
	lsls r0, r0, #15
	cmp r3, r0
	ble .L_08165fae
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	cmp r3, #1
	bne .L_08165fa0
	ldr r3, [r5, #16]
	movs r6, #210
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5, #16]
	movs r2, #255
	lsls r6, r6, #2
	movs r1, #0
	mov r8, r2
	add r6, r10
.L_08165f30:
	ldr r3, [r5]
	ldr r0, .L_08165f8c
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6]
	str r1, [sp, #16]
	ldr r3, [r5, #4]
	adds r3, r3, r0
	str r3, [r6, #4]
	bl Random16
	mov r2, r8
	ands r0, r2
	subs r0, #127
	lsls r0, r0, #10
	str r0, [r6, #12]
	bl Random16
	mov r3, r8
	ands r0, r3
	subs r0, #127
	lsls r0, r0, #10
	str r0, [r6, #16]
	bl Random16
	ldr r1, [sp, #16]
	movs r3, #15
	ands r3, r0
	adds r1, #1
	str r3, [r6, #24]
	adds r6, #28
	cmp r1, #2
	bne .L_08165f30
	b .L_08165fae
	.2byte 0x0000
.L_08165f78:
	.4byte 0xfff00000
.L_08165f7c:
	.4byte Data_02010018
.L_08165f80:
	.4byte gInput
.L_08165f84:
	.4byte Data_08196e6c
.L_08165f88:
	.4byte 0xfffffefc
.L_08165f8c:
	.4byte 0xffe00000
.L_08165f90:
	.4byte Data_08198a64
.L_08165f94:
	.4byte Data_08198a6d
.L_08165f98:
	.4byte 0xfffe0000
.L_08165f9c:
	.4byte 0xffff0000
.L_08165fa0:
	ldr r6, [sp, #52]
	cmp r6, #199
	bgt .L_08165fae
	movs r3, #0
	str r3, [r5, #4]
	str r3, [r5, #16]
	str r3, [r5, #24]
.L_08165fae:
	movs r0, #56
	adds r7, #1
	adds r5, #28
	add r10, r0
	cmp r7, #6
	beq .L_08165fbc
	b .L_08165ec0
.L_08165fbc:
	mov r5, r11
	movs r7, #0
	adds r5, #168
.L_08165fc2:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_08166010
	cmp r0, #23
	bhi .L_08166000
	movs r1, #6
	bl Math_Div
	ldr r3, .L_081661c0
	adds r0, #3
	lsls r0, r0, #1
	ldrh r1, [r3, r0]
	movs r2, #224
	lsls r2, r2, #3
	add r1, r11
	adds r1, r1, r2
	movs r3, #2
	ldrsh r2, [r5, r3]
	ldr r3, .L_081661c4
	ldrh r4, [r3, r0]
	movs r6, #6
	ldrsh r3, [r5, r6]
	lsrs r0, r4, #1
	subs r2, r2, r0
	subs r3, r3, r0
	str r4, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #56]
	ldr r4, [sp, #72]
	mov lr, r4
	.2byte 0xf800
.L_08166000:
	adds r0, r5, #0
	movs r1, #60
	ldr r2, .L_081661c8
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_08166010:
	adds r7, #1
	adds r5, #28
	cmp r7, #56
	bne .L_08165fc2
	ldr r0, [sp, #52]
	movs r1, #130
	lsls r1, r1, #1
	cmp r0, r1
	bne .L_081660dc
	ldr r2, [sp, #60]
	movs r7, #0
	ldr r3, [r2, #20]
	cmp r3, #0
	beq .L_08166058
	movs r5, #36
.L_0816602e:
	ldr r3, [sp, #60]
	movs r1, #4
	ldrsh r0, [r5, r3]
	bl Func_08118088
	ldr r1, [sp, #60]
	movs r3, #8
	ldrsh r0, [r5, r1]
	movs r2, #1
	str r3, [sp, #0]
	movs r1, #7
	adds r3, r7, #0
	negs r2, r2
	bl Func_0814cd48
	ldr r6, [sp, #60]
	adds r7, #1
	ldr r3, [r6, #20]
	adds r5, #2
	cmp r7, r3
	bne .L_0816602e
.L_08166058:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r11
	movs r3, #8
	str r3, [r2]
	ldr r0, [sp, #52]
	movs r1, #130
	lsls r1, r1, #1
	cmp r0, r1
	bne .L_081660dc
	ldr r2, .L_081661cc
	movs r7, #0
.L_08166072:
	str r2, [sp, #12]
	bl Random16
	movs r5, #192
	lsls r5, r5, #2
	adds r5, #255
	ands r5, r0
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	ldr r2, [sp, #12]
	adds r6, r0, #0
	adds r3, #255
	ands r6, r3
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r2]
	movs r3, #184
	lsls r3, r3, #15
	str r3, [r2, #4]
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #32
	adds r3, r5, #0
	muls r3, r0
	ldr r2, [sp, #12]
	asrs r3, r3, #7
	str r3, [r2, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	ldr r2, [sp, #12]
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #7
	str r3, [r2, #16]
	bl Random16
	movs r3, #15
	ldr r2, [sp, #12]
	ands r3, r0
	adds r3, #32
	str r3, [r2, #24]
	movs r3, #128
	adds r7, #1
	lsls r3, r3, #2
	adds r2, #28
	cmp r7, r3
	bne .L_08166072
.L_081660dc:
	ldr r0, .L_081661d0
	ldr r6, .L_081661cc
	movs r7, #0
	mov r8, r0
.L_081660e4:
	ldr r0, [r6, #24]
	cmp r0, #0
	blt .L_08166136
	asrs r0, r0, #3
	adds r0, #1
	lsls r5, r0, #1
	subs r3, r5, #2
	mov r2, r8
	ldrh r1, [r2, r3]
	ldr r3, [sp, #48]
	movs r4, #1
	adds r1, r3, r1
	movs r3, #2
	ldrsh r2, [r6, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	mov lr, r2
	movs r3, #6
	ldrsh r2, [r6, r3]
	str r0, [sp, #0]
	subs r3, r2, r0
	str r5, [sp, #4]
	ldr r0, [sp, #28]
	ands r4, r7
	lsls r4, r4, #2
	ldr r4, [r4, r0]
	mov r2, lr
	ldr r0, [sp, #56]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	adds r0, r6, #0
	movs r1, #62
	lsls r2, r2, #5
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r6, #24]
	subs r3, #1
	str r3, [r6, #24]
.L_08166136:
	movs r1, #128
	adds r7, #1
	lsls r1, r1, #2
	adds r6, #28
	cmp r7, r1
	bne .L_081660e4
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r11
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #52]
	movs r3, #160
	adds r2, #1
	lsls r3, r3, #1
	str r2, [sp, #52]
	cmp r2, r3
	beq .L_0816617c
	ldr r3, .L_081661d4
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	bne .L_0816617c
	b .L_08165cde
.L_0816617c:
	movs r0, #134
	bl Func_081180e8
	bl Func_0814cca8
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #220
	movs r7, #0
	add r5, r11
.L_08166190:
	ldmia r5!, {r0}
	adds r7, #1
	bl ResourceObject_ReleaseFar
	cmp r7, #15
	bne .L_08166190
	ldr r0, .L_081661d8
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #96
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_081661c0:
	.4byte Data_08198a76
.L_081661c4:
	.4byte Data_08198a84
.L_081661c8:
	.4byte 0xffffc000
.L_081661cc:
	.4byte gMapCellBuffer
.L_081661d0:
	.4byte Data_08197410
.L_081661d4:
	.4byte gInput
.L_081661d8:
	.4byte Func_08143000
