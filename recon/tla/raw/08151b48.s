.syntax unified
	.thumb
	.global Func_08151b48
	.thumb_func
Func_08151b48:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #64
	str r0, [sp, #40]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #92]
	str r1, [sp, #36]
	ldr r2, [r3, #96]
	str r2, [sp, #32]
	ldr r3, [r3, #100]
	str r3, [sp, #24]
	movs r3, #0
	str r3, [sp, #20]
	ldr r3, [r0, #4]
	cmp r3, #0
	bne .L_08151b80
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #1
	bl BattleFx_BeginTiledCanvasFilled
	b .L_08151b8a
.L_08151b80:
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #1
	bl BattleFx_BeginTiledCanvas
.L_08151b8a:
	ldr r1, [sp, #40]
	ldr r3, [r1, #24]
	cmp r3, #2
	bne .L_08151b9c
	movs r2, #128
	ldr r3, .L_08151b98
	b .L_08151ba0
.L_08151b98:
	.4byte 0x00000080
.L_08151b9c:
	movs r2, #128
	ldr r3, .L_08151bdc
.L_08151ba0:
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	movs r5, #192
	lsls r5, r5, #18
	ldr r3, [r5, #104]
	movs r1, #7
	movs r0, #188
	str r3, [sp, #44]
	bl Func_081963ec
	adds r5, #188
	ldr r3, [r5]
	mov r2, sp
	adds r2, #44
	str r2, [sp, #12]
	str r3, [r2, #4]
	ldr r3, [sp, #36]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #14
	adds r1, r3, r2
	ldr r0, .L_08151be0
	movs r2, #1
	movs r3, #1
	b .L_08151be4
.L_08151bdc:
	.4byte 0x00000100
.L_08151be0:
	.4byte 0x00000188
.L_08151be4:
	bl Resource_LoadAndDecompress
	movs r3, #0
	ldr r1, [sp, #24]
	ldr r0, .L_08151ec8
	movs r2, #0
	bl Resource_LoadAndDecompress
	ldr r1, [sp, #40]
	ldr r3, [r1, #24]
	cmp r3, #2
	bne .L_08151c24
	ldr r2, [sp, #36]
	movs r1, #240
	lsls r1, r1, #7
	adds r1, #240
	adds r3, r2, r1
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_08151c18
	movs r2, #128
	ldr r3, .L_08151ecc
	lsls r2, r2, #19
	adds r2, #40
	b .L_08151c56
.L_08151c18:
	movs r2, #128
	lsls r2, r2, #19
	movs r3, #128
	adds r2, #40
	lsls r3, r3, #5
	b .L_08151c56
.L_08151c24:
	add r5, sp, #52
	adds r0, r5, #0
	bl Func_0815e22c
	ldr r2, [r5]
	adds r3, r2, #0
	subs r3, #64
	str r3, [r5]
	cmp r3, #0
	bge .L_08151c3e
	str r3, [sp, #20]
	movs r3, #0
	b .L_08151c48
.L_08151c3e:
	cmp r3, #112
	ble .L_08151c4a
	subs r2, #176
	movs r3, #112
	str r2, [sp, #20]
.L_08151c48:
	str r3, [r5]
.L_08151c4a:
	ldr r3, [r5]
	movs r2, #128
	lsls r2, r2, #19
	negs r3, r3
	adds r2, #40
	lsls r3, r3, #8
.L_08151c56:
	str r3, [r2]
	ldr r7, .L_08151ed0
	movs r2, #0
	mov r8, r2
.L_08151c5e:
	bl Random16
	movs r6, #192
	lsls r6, r6, #2
	adds r6, #255
	movs r3, #128
	lsls r3, r3, #1
	ands r6, r0
	adds r6, r6, r3
	bl Random16
	movs r5, #254
	ldr r1, .L_08151ed4
	lsls r5, r5, #7
	movs r3, #128
	lsls r3, r3, #7
	adds r5, #255
	str r3, [r7]
	ands r5, r0
	movs r3, #224
	adds r5, r5, r1
	lsls r3, r3, #7
	str r3, [r7, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #16
	str r3, [r7, #8]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #16
	str r3, [r7, #20]
	movs r3, #0
	str r3, [r7, #24]
	movs r2, #1
	movs r3, #128
	add r8, r2
	lsls r3, r3, #3
	adds r7, #28
	cmp r8, r3
	bne .L_08151c5e
	ldr r1, [sp, #36]
	movs r3, #239
	lsls r3, r3, #7
	adds r2, r1, r3
	movs r3, #2
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r1, r3
	movs r3, #75
	movs r1, #200
	lsls r1, r1, #4
	str r3, [r2]
	ldr r0, .L_08151ed8
	bl Scheduler_AddOrUpdateCallback
	movs r0, #138
	bl Audio_PlayCue
	movs r1, #0
	str r1, [sp, #28]
.L_08151cea:
	ldr r2, [sp, #28]
	cmp r2, #20
	bne .L_08151cf6
	movs r0, #133
	bl Func_081180e8
.L_08151cf6:
	ldr r3, [sp, #28]
	cmp r3, #15
	ble .L_08151cfe
	b .L_08151e96
.L_08151cfe:
	adds r0, r3, #0
	movs r1, #5
	bl Math_Mod
	cmp r0, #2
	bne .L_08151d18
	movs r1, #128
	ldr r3, .L_08151edc
	ldr r0, [sp, #32]
	lsls r1, r1, #7
	ldr r2, .L_08151ee0
	mov lr, r3
	.2byte 0xf800
.L_08151d18:
	ldr r2, [sp, #28]
	movs r1, #0
	lsls r2, r2, #11
	str r2, [sp, #8]
	mov r11, r1
.L_08151d22:
	ldr r1, [sp, #8]
	movs r2, #128
	lsls r2, r2, #7
	movs r3, #0
	str r3, [sp, #16]
	adds r3, r1, r2
	mov r5, r11
	muls r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	ldr r1, [sp, #28]
	movs r3, #32
	subs r3, r3, r1
	muls r3, r0
	asrs r3, r3, #16
	adds r3, #64
	adds r0, r5, #0
	mov r10, r3
	bl Trig_Cos
	ldr r2, [sp, #40]
	lsls r0, r0, #3
	asrs r0, r0, #16
	ldr r3, [r2, #24]
	negs r0, r0
	adds r6, r0, #0
	subs r6, #8
	cmp r3, #0
	bne .L_08151da2
	bl Random16
	movs r3, #3
	ands r0, r3
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r5, r3, #4
	subs r5, r5, r3
	ldr r3, [sp, #36]
	movs r1, #208
	lsls r5, r5, #6
	lsls r1, r1, #4
	adds r5, r3, r5
	adds r1, #14
	adds r5, r5, r1
	bl Random16
	ldr r2, [sp, #20]
	movs r3, #7
	ands r0, r3
	add r2, r10
	movs r3, #24
	adds r2, r2, r0
	str r3, [sp, #0]
	movs r3, #120
	str r3, [sp, #4]
	subs r2, #16
	ldr r4, [sp, #44]
	ldr r0, [sp, #32]
	adds r1, r5, #0
	adds r3, r6, #0
	mov lr, r4
	.2byte 0xf800
	b .L_08151dee
.L_08151da2:
	bl Random16
	movs r3, #3
	ands r0, r3
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r5, r3, #4
	ldr r1, [sp, #36]
	subs r5, r5, r3
	movs r2, #208
	lsls r5, r5, #6
	lsls r2, r2, #4
	adds r5, r1, r5
	adds r2, #14
	adds r5, r5, r2
	bl Random16
	movs r3, #7
	ands r0, r3
	movs r1, #1
	mov r3, r11
	ands r1, r3
	ldr r2, [sp, #20]
	movs r3, #24
	str r3, [sp, #0]
	movs r3, #120
	str r3, [sp, #4]
	ldr r3, [sp, #12]
	add r2, r10
	adds r2, r2, r0
	lsls r1, r1, #2
	ldr r4, [r1, r3]
	subs r2, #16
	ldr r0, [sp, #32]
	adds r1, r5, #0
	adds r3, r6, #0
	mov lr, r4
	.2byte 0xf800
.L_08151dee:
	adds r3, r6, #0
	adds r3, #112
	ldr r7, .L_08151ed0
	movs r1, #0
	lsls r3, r3, #16
	mov r8, r1
	mov r9, r3
.L_08151dfc:
	ldr r3, [r7, #24]
	cmp r3, #0
	bne .L_08151e6e
	bl Random16
	movs r6, #128
	lsls r6, r6, #1
	adds r6, #255
	ands r6, r0
	bl Random16
	movs r5, #254
	ldr r2, .L_08151ed4
	lsls r5, r5, #7
	adds r5, #255
	mov r1, r10
	ands r5, r0
	lsls r3, r1, #16
	adds r5, r5, r2
	mov r2, r9
	str r2, [r7, #4]
	str r3, [r7]
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #128
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #9
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #7
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	ands r0, r3
	adds r0, #32
	str r0, [r7, #24]
	ldr r1, [sp, #16]
	ldr r2, [sp, #40]
	adds r1, #1
	str r1, [sp, #16]
	ldr r1, .L_08151ee4
	ldr r3, [r2, #24]
	ldr r2, [sp, #16]
	lsls r3, r3, #1
	adds r3, #1
	ldrb r3, [r1, r3]
	cmp r2, r3
	beq .L_08151e7c
.L_08151e6e:
	movs r3, #1
	movs r1, #128
	add r8, r3
	lsls r1, r1, #3
	adds r7, #28
	cmp r8, r1
	bne .L_08151dfc
.L_08151e7c:
	movs r2, #1
	add r11, r2
	mov r3, r11
	cmp r3, #4
	beq .L_08151e88
	b .L_08151d22
.L_08151e88:
	ldr r1, [sp, #36]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
.L_08151e96:
	ldr r5, .L_08151ed0
	movs r1, #0
	mov r8, r1
.L_08151e9c:
	ldr r3, [r5, #24]
	cmp r3, #0
	ble .L_08151f2e
	subs r3, #1
	str r3, [r5, #24]
	ldr r2, .L_08151ee8
	adds r0, r5, #0
	movs r1, #60
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #4]
	movs r2, #240
	lsls r2, r2, #15
	cmp r3, r2
	ble .L_08151eec
	ldr r3, [r5, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5, #16]
	b .L_08151f2e
.L_08151ec8:
	.4byte 0x00000134
.L_08151ecc:
	.4byte 0xfffff000
.L_08151ed0:
	.4byte gMapCellBuffer
.L_08151ed4:
	.4byte 0xffffc000
.L_08151ed8:
	.4byte Func_08143000
.L_08151edc:
	.4byte IwramFillWords
.L_08151ee0:
	.4byte 0x10101010
.L_08151ee4:
	.4byte Data_0819839c
.L_08151ee8:
	.4byte 0xfffff800
.L_08151eec:
	ldr r2, [r5]
	ldr r1, .L_08151fcc
	cmp r2, r1
	bhi .L_08151f2e
	cmp r3, #0
	blt .L_08151f2e
	ldr r0, [r5, #24]
	asrs r6, r2, #16
	asrs r7, r3, #16
	cmp r0, #0
	bge .L_08151f04
	adds r0, #7
.L_08151f04:
	asrs r0, r0, #3
	adds r0, #1
	ldr r2, .L_08151fd0
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #24]
	ldr r3, [sp, #20]
	adds r1, r2, r1
	adds r2, r3, r6
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	str r0, [sp, #0]
	subs r3, r7, r0
	str r4, [sp, #4]
	ldr r0, [sp, #32]
	ldr r4, [sp, #44]
	mov lr, r4
	.2byte 0xf800
.L_08151f2e:
	movs r1, #1
	movs r2, #128
	add r8, r1
	lsls r2, r2, #3
	adds r5, #28
	cmp r8, r2
	bne .L_08151e9c
	ldr r3, [sp, #28]
	subs r3, #4
	cmp r3, #91
	bhi .L_08151f7c
	ldr r1, [sp, #40]
	movs r3, #0
	mov r8, r3
	ldr r3, [r1, #20]
	cmp r3, #0
	beq .L_08151f7c
	movs r6, #36
	movs r5, #4
.L_08151f54:
	ldr r2, [sp, #28]
	cmp r2, r5
	bne .L_08151f70
	ldr r3, [sp, #40]
	movs r2, #5
	ldrsh r0, [r6, r3]
	movs r3, #10
	str r3, [sp, #0]
	movs r1, #7
	mov r3, r8
	bl Func_0814cd48
	ldr r2, [sp, #40]
	ldr r3, [r2, #20]
.L_08151f70:
	movs r1, #1
	add r8, r1
	adds r6, #2
	adds r5, #4
	cmp r8, r3
	bne .L_08151f54
.L_08151f7c:
	movs r0, #2
	movs r1, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r1, #240
	ldr r3, [sp, #36]
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r3, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #28]
	adds r2, #1
	str r2, [sp, #28]
	cmp r2, #64
	beq .L_08151fa8
	b .L_08151cea
.L_08151fa8:
	ldr r0, .L_08151fd4
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #64
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08151fcc:
	.4byte 0x007effff
.L_08151fd0:
	.4byte Data_08197410
.L_08151fd4:
	.4byte Func_08143000
