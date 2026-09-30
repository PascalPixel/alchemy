.syntax unified
	.thumb
	.global Func_0814ce30
	.thumb_func
Func_0814ce30:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #104
	str r0, [sp, #68]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	movs r1, #195
	str r0, [sp, #64]
	lsls r1, r1, #1
	ldr r3, [r3, #96]
	adds r1, #255
	movs r0, #8
	movs r2, #1
	str r3, [sp, #60]
	bl Func_08152404
	movs r1, #0
	ldr r2, .L_0814cf08
	mov r10, r1
	movs r1, #128
	movs r0, #127
	lsls r1, r1, #3
.L_0814ce68:
	mov r3, r10
	ands r3, r0
	strb r3, [r2]
	movs r3, #1
	add r10, r3
	adds r2, #1
	cmp r10, r1
	bne .L_0814ce68
	movs r4, #0
	movs r0, #127
	mov r10, r4
	mov r8, r0
	mov r9, r4
.L_0814ce82:
	movs r7, #0
	mov r6, r9
.L_0814ce86:
	bl Random16
	mov r1, r8
	adds r5, r0, #0
	ands r5, r1
	bl Random16
	ldr r3, .L_0814cf08
	mov r2, r8
	ands r0, r2
	adds r0, r6, r0
	adds r5, r6, r5
	adds r0, r0, r3
	adds r5, r5, r3
	ldrb r2, [r0]
	ldrb r3, [r5]
	adds r7, #1
	strb r3, [r0]
	strb r2, [r5]
	cmp r7, #128
	bne .L_0814ce86
	movs r0, #1
	add r10, r0
	movs r4, #128
	mov r1, r10
	add r9, r4
	cmp r1, #8
	bne .L_0814ce82
	movs r0, #0
	bl Func_081435e0
	ldr r3, .L_0814cf00
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_0814cf04
	adds r2, #48
	strh r3, [r2]
	ldr r2, [sp, #64]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0814cf0c
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r1, #35
	movs r0, #104
	bl Func_081963ec
	movs r5, #192
	lsls r5, r5, #18
	ldr r3, [r5, #104]
	movs r1, #39
	movs r0, #188
	str r3, [sp, #72]
	bl Func_081963ec
	b .L_0814cf10
.L_0814cf00:
	.4byte 0x00000100
.L_0814cf04:
	.4byte 0x00000000
.L_0814cf08:
	.4byte gMapCellBuffer
.L_0814cf0c:
	.4byte 0x00000176
.L_0814cf10:
	adds r5, #188
	ldr r3, [r5]
	mov r4, sp
	adds r4, #72
	str r4, [sp, #36]
	str r3, [r4, #4]
	ldr r0, [sp, #64]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r0, r3
	movs r1, #200
	movs r3, #0
	str r3, [r2]
	ldr r0, .L_0814d248
	lsls r1, r1, #4
	bl Func_080145a8
	movs r2, #128
	ldr r3, .L_0814d24c
	lsls r2, r2, #19
	adds r2, #40
	str r3, [r2]
	movs r0, #90
	ldr r5, [sp, #64]
	movs r4, #0
	negs r0, r0
	mov r10, r4
	movs r7, #7
	mov r8, r0
	movs r6, #0
.L_0814cf58:
	mov r1, r10
	cmp r1, #4
	bgt .L_0814cf6a
	str r6, [r5]
	bl Random16
	ands r0, r7
	adds r0, #104
	b .L_0814cf76
.L_0814cf6a:
	mov r2, r8
	str r2, [r5]
	bl Random16
	ands r0, r7
	adds r0, #108
.L_0814cf76:
	str r0, [r5, #4]
	bl Random16
	ands r0, r7
	adds r0, #4
	str r0, [r5, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	movs r4, #1
	adds r3, #16
	add r10, r4
	str r3, [r5, #24]
	mov r0, r10
	movs r3, #20
	add r8, r3
	adds r6, #20
	adds r5, #28
	cmp r0, #16
	bne .L_0814cf58
	ldr r2, [sp, #64]
	movs r3, #224
	movs r1, #0
	lsls r3, r3, #1
	mov r10, r1
	adds r5, r2, r3
.L_0814cfac:
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #32
	str r3, [r5]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #64
	str r3, [r5, #4]
	bl Random16
	movs r3, #7
	ands r3, r0
	movs r4, #1
	negs r3, r3
	add r10, r4
	subs r3, #8
	mov r0, r10
	str r3, [r5, #24]
	adds r5, #28
	cmp r0, #16
	bne .L_0814cfac
	ldr r2, [sp, #64]
	movs r3, #224
	movs r1, #0
	lsls r3, r3, #2
	mov r10, r1
	movs r6, #0
	adds r5, r2, r3
.L_0814cfec:
	movs r3, #128
	lsls r3, r3, #16
	str r3, [r5]
	movs r3, #128
	lsls r3, r3, #15
	str r3, [r5, #4]
	bl Random16
	movs r3, #255
	ands r3, r0
	adds r3, #200
	movs r4, #1
	negs r3, r3
	add r10, r4
	lsls r3, r3, #9
	mov r0, r10
	str r3, [r5, #12]
	str r6, [r5, #16]
	str r6, [r5, #24]
	adds r5, #28
	cmp r0, #16
	bne .L_0814cfec
	ldr r1, [sp, #64]
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #240
	adds r3, r1, r2
	ldr r0, [r3]
	bl Func_0814cc4c
	ldr r3, .L_0814d250
	movs r4, #0
	str r3, [sp, #52]
	str r4, [sp, #48]
	str r4, [sp, #56]
.L_0814d032:
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #48]
	ldr r3, .L_0814d254
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_0814d064
	ldr r0, [sp, #56]
	cmp r0, #190
	ble .L_0814d064
	movs r1, #30
	adds r1, #255
	cmp r0, r1
	bgt .L_0814d064
	movs r1, #128
	ldr r3, .L_0814d258
	ldr r0, [sp, #60]
	lsls r1, r1, #7
	mov lr, r3
	.2byte 0xf800
	movs r2, #143
	lsls r2, r2, #1
	str r2, [sp, #56]
.L_0814d064:
	ldr r3, [sp, #56]
	cmp r3, #224
	bne .L_0814d076
	ldr r4, [sp, #64]
	movs r0, #239
	lsls r0, r0, #7
	adds r2, r4, r0
	movs r3, #0
	str r3, [r2]
.L_0814d076:
	bl Func_08014de4
	adds r1, r5, #0
	adds r1, #12
	adds r0, r5, #0
	bl Func_080156e8
	ldr r1, [sp, #56]
	cmp r1, #31
	bne .L_0814d0c4
	ldr r2, [sp, #64]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #168
	adds r3, r2, r4
	movs r2, #8
	str r2, [r3]
	movs r0, #157
	bl Audio_PlayCue
	ldr r1, [sp, #68]
	movs r0, #0
	ldr r3, [r1, #20]
	mov r10, r0
	cmp r3, #0
	beq .L_0814d0c4
	movs r5, #36
.L_0814d0ac:
	ldr r2, [sp, #68]
	movs r1, #6
	ldrsh r0, [r5, r2]
	bl Func_08118088
	ldr r0, [sp, #68]
	movs r4, #1
	ldr r3, [r0, #20]
	add r10, r4
	adds r5, #2
	cmp r10, r3
	bne .L_0814d0ac
.L_0814d0c4:
	ldr r1, [sp, #56]
	cmp r1, #72
	bne .L_0814d0d0
	movs r0, #136
	bl Audio_PlayCue
.L_0814d0d0:
	ldr r2, [sp, #56]
	cmp r2, #140
	bne .L_0814d0dc
	movs r0, #156
	bl Audio_PlayCue
.L_0814d0dc:
	ldr r3, [sp, #48]
	ldr r0, [sp, #52]
	movs r4, #128
	lsls r4, r4, #7
	adds r3, r3, r4
	movs r1, #128
	adds r0, r0, r3
	lsls r1, r1, #15
	str r3, [sp, #48]
	str r0, [sp, #52]
	cmp r0, r1
	ble .L_0814d0f6
	str r1, [sp, #52]
.L_0814d0f6:
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #16
	ldr r2, [sp, #52]
	bl Func_0816442c
	ldr r0, [sp, #56]
	subs r0, #48
	cmp r0, #48
	bhi .L_0814d142
	movs r1, #24
	bl __divsi3
	movs r1, #3
	bl Math_Mod
	movs r4, #238
	ldr r2, [sp, #64]
	ldr r6, .L_0814d25c
	lsls r4, r4, #7
	adds r5, r0, #0
	adds r4, #232
	adds r3, r2, r4
	lsls r5, r5, #1
	ldrb r1, [r6, r5]
	ldr r0, [r3]
	bl Animation_ApplyChildArgumentFar
	movs r1, #238
	ldr r0, [sp, #64]
	lsls r1, r1, #7
	adds r1, #236
	adds r3, r0, r1
	adds r5, #1
	ldr r0, [r3]
	ldrb r1, [r6, r5]
	bl Animation_ApplyChildArgumentFar
.L_0814d142:
	ldr r3, [sp, #56]
	subs r3, #72
	cmp r3, #55
	bhi .L_0814d1c2
	ldr r3, [sp, #64]
	movs r4, #224
	movs r2, #0
	lsls r4, r4, #2
	mov r10, r2
	adds r6, r3, r4
.L_0814d156:
	ldr r0, [sp, #56]
	mov r3, r10
	adds r3, #72
	cmp r0, r3
	blt .L_0814d1b6
	ldr r5, [r6, #4]
	ldr r1, .L_0814d260
	cmp r5, r1
	bgt .L_0814d1b6
	add r0, r10
	cmp r0, #0
	bge .L_0814d170
	adds r0, #3
.L_0814d170:
	movs r1, #5
	asrs r0, r0, #2
	bl Math_Mod
	ldr r4, .L_0814d264
	lsls r1, r0, #1
	ldrh r1, [r4, r1]
	ldr r4, [sp, #64]
	movs r3, #2
	ldrsh r2, [r6, r3]
	adds r1, r4, r1
	movs r4, #224
	lsls r4, r4, #3
	adds r1, r1, r4
	ldr r4, .L_0814d268
	asrs r3, r5, #16
	ldrb r5, [r4, r0]
	lsrs r4, r5, #1
	subs r2, r2, r4
	ldr r4, .L_0814d26c
	ldrb r4, [r4, r0]
	str r5, [sp, #0]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #60]
	ldr r4, [sp, #72]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	adds r0, r6, #0
	movs r1, #64
	lsls r2, r2, #5
	bl BattleFxKernels_IntegrateVector2
.L_0814d1b6:
	movs r0, #1
	add r10, r0
	mov r1, r10
	adds r6, #28
	cmp r1, #16
	bne .L_0814d156
.L_0814d1c2:
	ldr r2, [sp, #56]
	cmp r2, #128
	bne .L_0814d22e
	ldr r4, [sp, #64]
	movs r0, #224
	movs r3, #0
	lsls r0, r0, #1
	mov r10, r3
	movs r6, #255
	adds r5, r4, r0
.L_0814d1d6:
	bl Random16
	movs r1, #96
	bl Math_ModU
	lsls r0, r0, #16
	str r0, [r5]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #88
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #128
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	negs r0, r0
	lsls r0, r0, #11
	str r0, [r5, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	movs r1, #1
	negs r3, r3
	add r10, r1
	subs r3, #16
	mov r2, r10
	str r3, [r5, #24]
	adds r5, #28
	cmp r2, #48
	bne .L_0814d1d6
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #40
	movs r3, #0
	str r3, [r2]
.L_0814d22e:
	ldr r3, [sp, #56]
	subs r3, #128
	str r3, [sp, #44]
	cmp r3, #96
	bls .L_0814d23a
	b .L_0814d3ba
.L_0814d23a:
	str r3, [sp, #40]
	cmp r3, #80
	ble .L_0814d270
	movs r4, #80
	str r4, [sp, #40]
	b .L_0814d27e
	.2byte 0x0000
.L_0814d248:
	.4byte Func_08143000
.L_0814d24c:
	.4byte 0xffffe000
.L_0814d250:
	.4byte 0xffc00000
.L_0814d254:
	.4byte gInput
.L_0814d258:
	.4byte IwramClearWords
.L_0814d25c:
	.4byte Data_081981fc
.L_0814d260:
	.4byte 0x0067ffff
.L_0814d264:
	.4byte Data_08198202
.L_0814d268:
	.4byte Data_0819820c
.L_0814d26c:
	.4byte Data_08198211
.L_0814d270:
	ldr r0, [sp, #64]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #168
	adds r2, r0, r1
	movs r3, #2
	str r3, [r2]
.L_0814d27e:
	ldr r7, [sp, #64]
	movs r2, #0
	mov r10, r2
.L_0814d284:
	ldr r3, [r7, #24]
	ldr r4, [sp, #40]
	cmp r4, r3
	bgt .L_0814d28e
	b .L_0814d3ac
.L_0814d28e:
	ldr r0, [sp, #64]
	movs r1, #224
	lsls r1, r1, #3
	adds r0, r0, r1
	mov r2, r10
	mov r9, r0
	cmp r2, #5
	ble .L_0814d2a8
	ldr r4, [sp, #64]
	movs r0, #220
	lsls r0, r0, #4
	adds r4, r4, r0
	mov r9, r4
.L_0814d2a8:
	ldr r1, [sp, #40]
	subs r2, r1, r3
	ldr r3, [r7, #16]
	adds r4, r3, #0
	muls r4, r2
	mov r11, r4
	mov r6, r11
	cmp r4, #184
	ble .L_0814d2c0
.L_0814d2ba:
	subs r6, #64
	cmp r6, #184
	bgt .L_0814d2ba
.L_0814d2c0:
	cmp r6, #119
	bgt .L_0814d2e8
	movs r1, #1
	mov r0, r10
	ands r0, r1
	movs r1, #24
	ldr r3, [r7, #4]
	ldr r2, [r7]
	str r1, [sp, #0]
	movs r1, #8
	str r1, [sp, #4]
	ldr r1, [sp, #36]
	lsls r0, r0, #2
	subs r3, r3, r6
	ldr r4, [r0, r1]
	subs r3, #8
	ldr r0, [sp, #60]
	mov r1, r9
	mov lr, r4
	.2byte 0xf800
.L_0814d2e8:
	mov r3, r10
	movs r4, #1
	ands r3, r4
	lsls r3, r3, #2
	str r3, [sp, #32]
	movs r2, #0
	mov r8, r2
.L_0814d2f6:
	ldr r1, [r7, #4]
	mov r0, r8
	lsls r3, r0, #6
	subs r2, r1, r6
	adds r5, r2, r3
	movs r3, #64
	negs r3, r3
	movs r2, #0
	movs r0, #64
	cmp r5, r3
	blt .L_0814d344
	cmp r5, #0
	bge .L_0814d31e
	negs r2, r5
	lsls r3, r2, #1
	adds r0, r5, #0
	adds r3, r3, r2
	lsls r2, r3, #3
	adds r0, #64
	movs r5, #0
.L_0814d31e:
	adds r3, r5, r0
	cmp r3, r1
	ble .L_0814d328
	subs r3, r3, r1
	subs r0, r0, r3
.L_0814d328:
	mov r4, r9
	movs r3, #24
	adds r1, r4, r2
	ldr r2, [r7]
	str r3, [sp, #0]
	str r0, [sp, #4]
	ldr r3, [sp, #36]
	ldr r0, [sp, #32]
	adds r1, #192
	ldr r4, [r0, r3]
	ldr r0, [sp, #60]
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
.L_0814d344:
	movs r4, #1
	add r8, r4
	mov r0, r8
	cmp r0, #3
	bne .L_0814d2f6
	mov r6, r10
	ands r6, r4
	cmp r6, #0
	beq .L_0814d3ac
	ldr r5, [r7, #4]
	mov r1, r11
	movs r2, #127
	subs r3, r5, r1
	ands r3, r2
	subs r3, #16
	movs r1, #3
	mov r0, r10
	mov r8, r3
	bl Math_Mod
	ldr r3, .L_0814d4c0
	adds r1, r0, #0
	ldrb r4, [r3, r1]
	mov r2, r8
	adds r3, r2, r4
	mov r12, r4
	cmp r3, r5
	ble .L_0814d380
	subs r3, r3, r5
	subs r4, r4, r3
.L_0814d380:
	cmp r4, #0
	ble .L_0814d3ac
	ldr r2, .L_0814d4c4
	lsls r3, r1, #1
	ldrh r1, [r2, r3]
	ldr r3, [sp, #64]
	movs r2, #224
	adds r1, r3, r1
	lsls r2, r2, #3
	mov r3, r12
	adds r1, r1, r2
	ldr r2, [r7]
	str r3, [sp, #0]
	str r4, [sp, #4]
	ldr r3, [sp, #36]
	lsls r0, r6, #2
	ldr r4, [r0, r3]
	adds r2, #8
	ldr r0, [sp, #60]
	mov r3, r8
	mov lr, r4
	.2byte 0xf800
.L_0814d3ac:
	movs r4, #1
	add r10, r4
	mov r0, r10
	adds r7, #28
	cmp r0, #10
	beq .L_0814d3ba
	b .L_0814d284
.L_0814d3ba:
	ldr r1, [sp, #44]
	cmp r1, #95
	bhi .L_0814d474
	ldr r3, [sp, #64]
	movs r4, #224
	movs r2, #0
	lsls r4, r4, #2
	mov r10, r2
	movs r6, #255
	adds r5, r3, r4
.L_0814d3ce:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_0814d464
	movs r1, #5
	mov r0, r10
	bl Math_Mod
	ldr r2, .L_0814d4c8
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #64]
	movs r3, #224
	adds r1, r2, r1
	lsls r3, r3, #3
	movs r4, #2
	ldrsh r2, [r5, r4]
	adds r1, r1, r3
	movs r4, #6
	ldrsh r3, [r5, r4]
	ldr r4, .L_0814d4cc
	ldrb r4, [r4, r0]
	str r4, [sp, #0]
	ldr r4, .L_0814d4d0
	ldrb r0, [r4, r0]
	ldr r4, [sp, #72]
	str r0, [sp, #4]
	ldr r0, [sp, #60]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5]
	ldr r2, [r5, #12]
	movs r0, #128
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #16]
	lsls r0, r0, #7
	movs r1, #240
	adds r2, r2, r3
	lsls r1, r1, #15
	adds r3, r3, r0
	str r2, [r5, #4]
	str r3, [r5, #16]
	cmp r2, r1
	bls .L_0814d462
	ldr r2, [sp, #56]
	cmp r2, #159
	bgt .L_0814d462
	bl Random16
	movs r1, #96
	bl Math_ModU
	lsls r0, r0, #16
	str r0, [r5]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #88
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #128
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	negs r0, r0
	lsls r0, r0, #11
	str r0, [r5, #16]
.L_0814d462:
	ldr r3, [r5, #24]
.L_0814d464:
	adds r3, #1
	str r3, [r5, #24]
	movs r3, #1
	add r10, r3
	mov r4, r10
	adds r5, #28
	cmp r4, #32
	bne .L_0814d3ce
.L_0814d474:
	ldr r3, [sp, #56]
	subs r3, #224
	cmp r3, #23
	bhi .L_0814d508
	ldr r0, [sp, #56]
	movs r3, #3
	ands r3, r0
	cmp r3, #0
	bne .L_0814d508
	ldr r2, .L_0814d4bc
	movs r4, #160
	movs r1, #0
	lsls r4, r4, #19
	mov r10, r1
	mov r8, r2
.L_0814d492:
	ldrh r3, [r4]
	movs r7, #31
	ands r7, r3
	lsls r3, r3, #16
	mov r0, r8
	lsrs r6, r3, #21
	lsrs r5, r3, #26
	ands r6, r0
	ands r5, r0
	adds r0, r7, r6
	adds r0, r0, r5
	movs r1, #3
	str r4, [sp, #8]
	bl __divsi3
	ldr r4, [sp, #8]
	cmp r7, r0
	ble .L_0814d4d4
	subs r7, #1
	b .L_0814d4d4
	.2byte 0x0000
.L_0814d4bc:
	.4byte 0x0000001f
.L_0814d4c0:
	.4byte Data_0819821c
.L_0814d4c4:
	.4byte Data_08198216
.L_0814d4c8:
	.4byte Data_08198220
.L_0814d4cc:
	.4byte Data_0819822a
.L_0814d4d0:
	.4byte Data_0819822f
.L_0814d4d4:
	cmp r7, r0
	bge .L_0814d4da
	adds r7, #1
.L_0814d4da:
	cmp r6, r0
	ble .L_0814d4e0
	subs r6, #1
.L_0814d4e0:
	cmp r6, r0
	bge .L_0814d4e6
	adds r6, #1
.L_0814d4e6:
	cmp r5, r0
	ble .L_0814d4ec
	subs r5, #1
.L_0814d4ec:
	cmp r5, r0
	bge .L_0814d4f2
	adds r5, #1
.L_0814d4f2:
	lsls r2, r6, #5
	lsls r3, r5, #10
	movs r1, #1
	orrs r3, r2
	add r10, r1
	orrs r3, r7
	mov r2, r10
	strh r3, [r4]
	adds r4, #2
	cmp r2, #64
	bne .L_0814d492
.L_0814d508:
	ldr r3, [sp, #44]
	cmp r3, #172
	bhi .L_0814d5e8
	ldr r0, [sp, #68]
	movs r4, #0
	ldr r2, [r0, #20]
	mov r10, r4
	cmp r2, #0
	beq .L_0814d5e8
	add r1, sp, #80
	movs r2, #36
	mov r8, r1
	add r6, sp, #92
	mov r9, r4
	mov r11, r2
.L_0814d526:
	ldr r1, [sp, #68]
	mov r3, r11
	ldrsh r0, [r3, r1]
	bl Func_08118088 + 0x10
	ldr r2, [r0]
	mov r1, r8
	ldr r3, [r2, #8]
	adds r0, r6, #0
	str r3, [r6]
	movs r7, #0
	ldr r3, [r2, #12]
	str r3, [r6, #4]
	ldr r3, [r2, #16]
	str r3, [r6, #8]
	bl Func_0815e1ec
	mov r2, r9
	lsls r3, r2, #3
	ldr r4, [sp, #64]
	subs r3, r3, r2
	lsls r3, r3, #2
	movs r0, #224
	adds r3, r3, r4
	lsls r0, r0, #1
	adds r5, r3, r0
.L_0814d55a:
	ldr r0, [r5, #24]
	cmp r0, #0
	bne .L_0814d582
	bl Random16
	ldr r3, [sp, #80]
	movs r1, #15
	ands r0, r1
	adds r3, r3, r0
	subs r3, #8
	str r3, [r5]
	bl Random16
	ldr r3, [sp, #84]
	movs r2, #15
	ands r0, r2
	adds r3, r3, r0
	subs r3, #40
	str r3, [r5, #4]
	ldr r0, [r5, #24]
.L_0814d582:
	cmp r0, #4
	bhi .L_0814d5b2
	ldr r2, .L_0814d724
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldr r3, [sp, #64]
	ldr r2, [r5]
	adds r1, r3, r1
	ldr r3, .L_0814d728
	movs r4, #224
	ldrb r0, [r3, r0]
	ldr r3, [r5, #4]
	lsls r4, r4, #3
	adds r1, r1, r4
	lsrs r4, r0, #1
	subs r2, r2, r4
	subs r3, r3, r4
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #72]
	ldr r0, [sp, #60]
	mov lr, r4
	.2byte 0xf800
	ldr r0, [r5, #24]
.L_0814d5b2:
	adds r3, r0, #1
	str r3, [r5, #24]
	ldr r0, [sp, #56]
	cmp r0, #199
	bgt .L_0814d5cc
	cmp r3, #5
	bne .L_0814d5cc
	bl Random16
	movs r3, #7
	ands r3, r0
	negs r3, r3
	str r3, [r5, #24]
.L_0814d5cc:
	adds r7, #1
	adds r5, #28
	cmp r7, #6
	bne .L_0814d55a
	ldr r4, [sp, #68]
	movs r2, #2
	add r11, r2
	ldr r2, [r4, #20]
	movs r3, #1
	movs r1, #6
	add r10, r3
	add r9, r1
	cmp r10, r2
	bne .L_0814d526
.L_0814d5e8:
	ldr r0, [sp, #56]
	cmp r0, #232
	ble .L_0814d6b6
	ldr r1, .L_0814d72c
	lsls r3, r0, #1
	adds r6, r3, r1
	movs r2, #0
	movs r3, #0
	mov r10, r2
	mov r12, r3
	adds r4, r6, #0
.L_0814d5fe:
	movs r7, #0
.L_0814d600:
	cmp r4, #127
	bhi .L_0814d642
	movs r5, #7
	adds r0, r4, #0
	ands r0, r5
	lsls r3, r0, #5
	ldr r1, .L_0814d730
	add r3, r10
	lsls r3, r3, #2
	adds r3, r3, r7
	adds r3, r3, r1
	ldrb r1, [r3]
	adds r3, r4, #0
	cmp r4, #0
	bge .L_0814d620
	adds r3, r4, #7
.L_0814d620:
	asrs r3, r3, #3
	adds r2, r1, #0
	cmp r1, #0
	bge .L_0814d62a
	adds r2, r1, #7
.L_0814d62a:
	asrs r2, r2, #3
	lsls r3, r3, #4
	adds r3, r3, r2
	lsls r3, r3, #3
	adds r3, r3, r0
	ldr r2, [sp, #60]
	ands r1, r5
	lsls r3, r3, #3
	adds r3, r3, r1
	adds r3, r2, r3
	mov r0, r12
	strb r0, [r3]
.L_0814d642:
	adds r7, #1
	cmp r7, #4
	bne .L_0814d600
	movs r1, #1
	add r10, r1
	mov r2, r10
	adds r4, #1
	cmp r2, #32
	bne .L_0814d5fe
	movs r4, #0
	movs r3, #0
	mov lr, r4
	mov r10, r3
	adds r4, r6, #1
.L_0814d65e:
	movs r7, #0
	mov r12, r4
.L_0814d662:
	cmp r4, #127
	bhi .L_0814d6a4
	movs r5, #7
	mov r0, r12
	ands r0, r5
	lsls r3, r0, #5
	ldr r1, .L_0814d730
	add r3, r10
	lsls r3, r3, #2
	adds r3, r3, r7
	adds r3, r3, r1
	ldrb r1, [r3]
	mov r3, r12
	cmp r3, #0
	bge .L_0814d682
	adds r3, #7
.L_0814d682:
	asrs r3, r3, #3
	adds r2, r1, #0
	cmp r1, #0
	bge .L_0814d68c
	adds r2, r1, #7
.L_0814d68c:
	asrs r2, r2, #3
	lsls r3, r3, #4
	adds r3, r3, r2
	lsls r3, r3, #3
	adds r3, r3, r0
	ldr r2, [sp, #60]
	ands r1, r5
	lsls r3, r3, #3
	adds r3, r3, r1
	adds r3, r2, r3
	mov r0, lr
	strb r0, [r3]
.L_0814d6a4:
	adds r7, #1
	cmp r7, #4
	bne .L_0814d662
	movs r1, #1
	add r10, r1
	mov r2, r10
	adds r4, #1
	cmp r2, #32
	bne .L_0814d65e
.L_0814d6b6:
	ldr r3, [sp, #56]
	subs r3, #161
	cmp r3, #62
	bhi .L_0814d734
	ldr r4, [sp, #68]
	movs r3, #0
	ldr r2, [r4, #20]
	mov r10, r3
	cmp r2, #0
	beq .L_0814d738
	movs r7, #36
.L_0814d6cc:
	mov r0, r10
	ldr r1, [sp, #56]
	lsls r3, r0, #3
	adds r3, #160
	cmp r1, r3
	ble .L_0814d718
	ldr r2, [sp, #68]
	ldrsh r0, [r7, r2]
	bl Func_08118088 + 0x10
	adds r6, r0, #0
	ldr r2, [r6]
	movs r4, #128
	ldr r3, [r2, #12]
	lsls r4, r4, #12
	movs r1, #128
	adds r3, r3, r4
	lsls r1, r1, #16
	str r3, [r2, #12]
	cmp r3, r1
	ble .L_0814d6f8
	str r1, [r2, #12]
.L_0814d6f8:
	movs r3, #0
	movs r5, #0
	str r3, [r2, #72]
	b .L_0814d708
.L_0814d700:
	movs r1, #5
	bl Animation_ApplyChildArgumentFar
	adds r5, #1
.L_0814d708:
	ldr r0, [r6]
	adds r1, r5, #0
	bl Func_08118088 + 0x50
	cmp r0, #0
	bne .L_0814d700
	ldr r0, [sp, #68]
	ldr r2, [r0, #20]
.L_0814d718:
	movs r1, #1
	add r10, r1
	adds r7, #2
	cmp r10, r2
	bne .L_0814d6cc
	b .L_0814d738
.L_0814d724:
	.4byte Data_08198234
.L_0814d728:
	.4byte Data_0819823e
.L_0814d72c:
	.4byte 0xfffffe10
.L_0814d730:
	.4byte gMapCellBuffer
.L_0814d734:
	ldr r3, [sp, #68]
	ldr r2, [r3, #20]
.L_0814d738:
	movs r4, #0
	mov r10, r4
	cmp r2, #0
	beq .L_0814d7a4
	movs r5, #143
	movs r7, #8
	movs r6, #36
	lsls r5, r5, #1
.L_0814d748:
	ldr r0, [sp, #56]
	cmp r0, r5
	bne .L_0814d766
	ldr r1, [sp, #68]
	ldrsh r0, [r6, r1]
	bl Func_08118088 + 0x10
	ldr r2, [r0]
	movs r3, #192
	lsls r3, r3, #15
	str r3, [r2, #12]
	movs r3, #171
	lsls r3, r3, #8
	adds r3, #133
	str r3, [r2, #72]
.L_0814d766:
	ldr r4, [sp, #56]
	adds r3, r5, #0
	adds r3, #16
	cmp r4, r3
	bne .L_0814d794
	ldr r1, [sp, #68]
	mov r3, r10
	ldrsh r0, [r6, r1]
	movs r2, #1
	movs r1, #7
	negs r2, r2
	str r7, [sp, #0]
	bl Func_0814cd48
	movs r0, #134
	bl Audio_PlayCue
	movs r0, #238
	ldr r4, [sp, #64]
	lsls r0, r0, #7
	adds r0, #168
	adds r3, r4, r0
	str r7, [r3]
.L_0814d794:
	ldr r3, [sp, #68]
	movs r1, #1
	ldr r2, [r3, #20]
	add r10, r1
	adds r6, #2
	adds r5, #5
	cmp r10, r2
	bne .L_0814d748
.L_0814d7a4:
	ldr r4, [sp, #56]
	movs r0, #151
	lsls r0, r0, #1
	cmp r4, r0
	beq .L_0814d7b0
	b .L_0814d8e2
.L_0814d7b0:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r2, [sp, #64]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0814d84c
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r4, [sp, #64]
	movs r2, #236
	lsls r2, r2, #5
	adds r1, r4, r2
	movs r3, #1
	movs r2, #1
	ldr r0, .L_0814d850
	bl Func_08157cf4
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r1, #23
	movs r0, #188
	str r3, [sp, #72]
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r4, [sp, #36]
	ldr r3, [r3]
	movs r2, #128
	str r3, [r4, #4]
	ldr r3, .L_0814d844
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_0814d848
	subs r2, #48
	strh r3, [r2]
	adds r2, #8
	movs r3, #0
	str r3, [r2]
	ldr r0, [sp, #64]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r0, r1
	movs r3, #2
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r0, r3
	movs r1, #200
	movs r3, #75
	str r3, [r2]
	ldr r0, .L_0814d854
	lsls r1, r1, #4
	bl Func_080145a8
	ldr r0, [sp, #68]
	movs r4, #0
	b .L_0814d858
	.2byte 0x0000
.L_0814d844:
	.4byte 0x00003f46
.L_0814d848:
	.4byte 0x00000080
.L_0814d84c:
	.4byte 0x0000015b
.L_0814d850:
	.4byte 0x00000184
.L_0814d854:
	.4byte Func_08143000
.L_0814d858:
	ldr r2, [r0, #20]
	mov r10, r4
	cmp r2, #0
	beq .L_0814d8e2
	movs r1, #36
	mov r9, r1
.L_0814d864:
	ldr r4, [sp, #68]
	mov r2, r9
	ldrsh r0, [r2, r4]
	bl Func_08118088 + 0x10
	ldr r0, [r0]
	ldr r1, [sp, #64]
	mov r8, r0
	mov r0, r10
	lsls r2, r0, #2
	add r2, r10
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #3
	movs r7, #0
	adds r6, r3, r1
.L_0814d884:
	mov r2, r8
	ldr r3, [r2, #8]
	str r3, [r6]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r6, #4]
	ldr r3, [r2, #16]
	str r3, [r6, #8]
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #52
	adds r5, r7, #0
	muls r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	lsls r0, r0, #2
	str r0, [r6, #12]
	bl Random16
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	movs r4, #128
	ands r3, r0
	lsls r4, r4, #9
	adds r3, r3, r4
	str r3, [r6, #16]
	adds r0, r5, #0
	bl Trig_Cos
	adds r7, #1
	lsls r0, r0, #2
	movs r3, #0
	str r0, [r6, #20]
	str r3, [r6, #24]
	adds r6, #28
	cmp r7, #10
	bne .L_0814d884
	ldr r3, [sp, #68]
	movs r1, #1
	ldr r2, [r3, #20]
	movs r0, #2
	add r10, r1
	add r9, r0
	cmp r10, r2
	bne .L_0814d864
.L_0814d8e2:
	ldr r4, [sp, #56]
	movs r0, #46
	adds r0, #255
	cmp r4, r0
	bgt .L_0814d8ee
	b .L_0814da4a
.L_0814d8ee:
	movs r1, #0
	mov r10, r1
	cmp r2, #0
	bne .L_0814d8f8
	b .L_0814da4a
.L_0814d8f8:
	mov r2, sp
	adds r2, #92
	mov r3, sp
	adds r3, #80
	str r2, [sp, #28]
	ldr r0, [sp, #56]
	ldr r1, .L_0814daf4
	movs r2, #151
	str r3, [sp, #24]
	movs r4, #36
	lsls r2, r2, #1
	movs r3, #0
	str r4, [sp, #20]
	str r2, [sp, #16]
	str r3, [sp, #12]
	adds r0, r0, r1
	mov r11, r0
.L_0814d91a:
	ldr r4, [sp, #56]
	ldr r0, [sp, #16]
	cmp r4, r0
	blt .L_0814d9a8
	ldr r1, [sp, #12]
	movs r2, #157
	lsls r2, r2, #1
	adds r3, r1, r2
	cmp r4, r3
	bge .L_0814d9a8
	ldr r4, [sp, #20]
	ldr r2, [sp, #68]
	mov r3, r11
	ldrsh r0, [r4, r2]
	lsrs r6, r3, #31
	bl Func_08118088 + 0x10
	ldr r2, [r0]
	ldr r4, [sp, #28]
	ldr r3, [r2, #8]
	add r6, r11
	str r3, [r4]
	movs r3, #0
	str r3, [r4, #4]
	asrs r6, r6, #1
	ldr r3, [r2, #16]
	lsls r5, r6, #4
	str r3, [r4, #8]
	ldr r0, [sp, #24]
	subs r5, r5, r6
	mov r8, r0
	mov r1, r8
	ldr r0, [sp, #28]
	bl Func_0815e1ec
	mov r1, r8
	ldr r2, [r1]
	lsls r5, r5, #5
	asrs r2, r2, #1
	str r2, [r1]
	ldr r3, [sp, #64]
	movs r4, #224
	adds r5, r3, r5
	ldr r3, [r1, #4]
	lsls r4, r4, #3
	adds r5, r5, r4
	movs r0, #20
	movs r1, #24
	subs r2, #20
	subs r3, #24
	str r0, [sp, #0]
	str r1, [sp, #4]
	ldr r4, [sp, #72]
	adds r1, r5, #0
	ldr r0, [sp, #60]
	mov lr, r4
	.2byte 0xf800
	mov r3, r8
	movs r4, #20
	movs r0, #24
	ldr r2, [r3]
	ldr r1, [sp, #36]
	ldr r3, [r3, #4]
	str r0, [sp, #4]
	str r4, [sp, #0]
	subs r3, #24
	ldr r4, [r1, #4]
	ldr r0, [sp, #60]
	adds r1, r5, #0
	mov lr, r4
	.2byte 0xf800
.L_0814d9a8:
	ldr r3, [sp, #16]
	ldr r2, [sp, #56]
	adds r3, #6
	cmp r2, r3
	blt .L_0814da24
	ldr r2, [sp, #12]
	add r3, sp, #80
	add r2, r10
	mov r8, r3
	ldr r4, [sp, #64]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #3
	movs r0, #12
	movs r7, #0
	mov r6, r8
	adds r5, r3, r4
	mov r9, r0
.L_0814d9cc:
	adds r0, r5, #0
	adds r1, r6, #0
	bl Func_0815e1ec
	ldr r3, [r6]
	asrs r2, r3, #1
	str r2, [r6]
	ldr r3, [r5, #24]
	cmp r3, #26
	bhi .L_0814da0a
	ldr r3, .L_0814daf8
	mov r4, r9
	ldrh r1, [r3, r4]
	ldr r0, [sp, #64]
	movs r3, #224
	adds r1, r0, r1
	lsls r3, r3, #3
	adds r1, r1, r3
	ldr r3, .L_0814dafc
	ldrh r4, [r3, r4]
	mov r3, r8
	ldr r3, [r3, #4]
	lsrs r0, r4, #1
	subs r2, r2, r0
	subs r3, r3, r0
	str r4, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #60]
	ldr r4, [sp, #72]
	mov lr, r4
	.2byte 0xf800
.L_0814da0a:
	movs r2, #128
	adds r0, r5, #0
	movs r1, #60
	lsls r2, r2, #5
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	adds r7, #1
	adds r3, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #5
	bne .L_0814d9cc
.L_0814da24:
	ldr r4, [sp, #20]
	ldr r1, [sp, #16]
	ldr r2, [sp, #12]
	adds r4, #2
	adds r1, #4
	adds r2, #4
	str r4, [sp, #20]
	str r1, [sp, #16]
	str r2, [sp, #12]
	ldr r4, [sp, #68]
	movs r3, #1
	add r10, r3
	ldr r3, [r4, #20]
	movs r0, #4
	negs r0, r0
	add r11, r0
	cmp r10, r3
	beq .L_0814da4a
	b .L_0814d91a
.L_0814da4a:
	ldr r0, [sp, #56]
	cmp r0, #127
	bgt .L_0814da5a
	movs r0, #4
	movs r1, #16
	bl Func_08158ce0
	b .L_0814da76
.L_0814da5a:
	ldr r1, [sp, #56]
	movs r2, #46
	adds r2, #255
	cmp r1, r2
	bgt .L_0814da6e
	movs r0, #4
	movs r1, #4
	bl Func_08158ce0
	b .L_0814da76
.L_0814da6e:
	movs r0, #4
	movs r1, #8
	bl Func_08158ce0
.L_0814da76:
	bl Func_081434f8
	movs r4, #240
	ldr r3, [sp, #64]
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #56]
	movs r1, #183
	adds r0, #1
	lsls r1, r1, #1
	str r0, [sp, #56]
	cmp r0, r1
	beq .L_0814daa0
	bl .L_0814d032
.L_0814daa0:
	ldr r0, .L_0814db00
	bl Func_08014644
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r0, #134
	bl Func_08118088 + 0x60
	movs r1, #128
	ldr r2, [sp, #52]
	movs r0, #2
	lsls r1, r1, #16
	bl Func_0816467c
	movs r4, #238
	ldr r3, [sp, #64]
	lsls r4, r4, #7
	movs r2, #0
	adds r4, #220
	mov r10, r2
	adds r5, r3, r4
.L_0814dad2:
	ldmia r5!, {r0}
	bl Func_08020040 + 0x8
	movs r0, #1
	add r10, r0
	mov r1, r10
	cmp r1, #8
	bne .L_0814dad2
	bl Func_08143bb8
	add sp, #104
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0814daf4:
	.4byte 0xfffffed2
.L_0814daf8:
	.4byte Data_08198244
.L_0814dafc:
	.4byte Data_08198252
.L_0814db00:
	.4byte Func_08143000
