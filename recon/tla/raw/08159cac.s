.syntax unified
	.thumb
	.global Func_08159cac
	.thumb_func
Func_08159cac:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #60
	str r0, [sp, #32]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #92]
	str r0, [sp, #28]
	movs r0, #1
	ldr r1, [r5, #96]
	str r1, [sp, #24]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_08159d0c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_08159d10
	adds r2, #48
	strh r3, [r2]
	ldr r2, [sp, #28]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_08159d14
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r1, #35
	movs r0, #104
	bl Func_081963ec
	ldr r5, [r5, #104]
	ldr r6, [sp, #32]
	str r5, [sp, #20]
	movs r4, #36
	ldrsh r0, [r6, r4]
	add r6, sp, #48
	adds r1, r6, #0
	b .L_08159d18
	.2byte 0x0000
.L_08159d0c:
	.4byte 0x00000100
.L_08159d10:
	.4byte 0x00000000
.L_08159d14:
	.4byte 0x0000014d
.L_08159d18:
	bl Func_0815e20c
	ldr r0, [sp, #32]
	add r5, sp, #36
	ldr r3, [r0, #20]
	lsls r3, r3, #1
	adds r3, #34
	ldrsh r0, [r0, r3]
	adds r1, r5, #0
	bl Func_0815e20c
	ldr r1, [r6]
	ldr r3, [r5]
	movs r4, #239
	subs r3, r3, r1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r1, r1, r3
	movs r2, #128
	movs r3, #64
	subs r3, r3, r1
	lsls r2, r2, #19
	adds r2, #40
	lsls r3, r3, #8
	str r1, [r6]
	str r3, [r2]
	ldr r3, [sp, #28]
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	ldr r6, [sp, #28]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #132
	adds r2, r6, r0
	movs r3, #0
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0815a01c
	bl Scheduler_AddOrUpdateCallback
	ldr r1, [sp, #32]
	ldr r2, .L_0815a020
	ldr r3, [r1, #24]
	ldrb r2, [r2, r3]
	str r2, [sp, #16]
	ldr r3, [sp, #16]
	movs r2, #0
	mov r10, r2
	cmp r3, #0
	beq .L_08159e12
	ldr r1, .L_0815a024
	ldr r3, [sp, #28]
.L_08159d88:
	str r1, [r3, #4]
	str r2, [r3, #16]
	ldr r6, [sp, #16]
	movs r4, #1
	add r10, r4
	adds r3, #28
	cmp r10, r6
	bne .L_08159d88
	movs r0, #0
	mov r10, r0
	cmp r6, #0
	beq .L_08159e12
	ldr r1, .L_0815a028
	movs r7, #0
	mov r9, r1
.L_08159da6:
	movs r3, #140
	mov r4, r10
	muls r4, r3
	ldr r0, .L_0815a02c
	adds r3, r4, #0
	movs r2, #0
	adds r3, r7, r3
	mov r8, r2
	mov r6, r9
	adds r5, r3, r0
.L_08159dba:
	ldr r2, .L_0815a030
	mov r1, r10
	ldrsb r2, [r2, r1]
	ldrb r3, [r6]
	adds r3, r3, r2
	lsls r3, r3, #16
	str r3, [r5]
	ldrb r3, [r6, #1]
	adds r6, #2
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	movs r1, #96
	bl Math_ModU
	subs r0, #48
	lsls r0, r0, #10
	str r0, [r5, #12]
	bl Random16
	movs r3, #127
	ands r3, r0
	adds r3, #32
	negs r3, r3
	lsls r3, r3, #11
	str r3, [r5, #16]
	movs r2, #1
	movs r3, #32
	str r3, [r5, #8]
	add r8, r2
	movs r3, #0
	str r3, [r5, #24]
	mov r3, r8
	adds r5, #28
	cmp r3, #21
	bne .L_08159dba
	ldr r6, [sp, #16]
	movs r4, #224
	lsls r4, r4, #1
	add r10, r2
	adds r7, r7, r4
	cmp r10, r6
	bne .L_08159da6
.L_08159e12:
	ldr r2, [sp, #16]
	ldr r1, .L_0815a034
	subs r2, #1
	str r2, [sp, #12]
	movs r4, #65
	ldrb r3, [r1, r2]
	movs r0, #0
	negs r4, r4
	mov r11, r0
	cmp r3, r4
	bne .L_08159e2a
	b .L_08159ffc
.L_08159e2a:
	ldrb r3, [r1, r2]
	adds r3, #48
	cmp r11, r3
	bne .L_08159e38
	movs r0, #133
	bl Func_081180e8
.L_08159e38:
	ldr r0, [sp, #16]
	movs r6, #0
	mov r10, r6
	cmp r0, #0
	bne .L_08159e44
	b .L_08159fca
.L_08159e44:
	ldr r1, [sp, #28]
	mov r9, r1
.L_08159e48:
	ldr r5, .L_0815a034
	mov r2, r10
	ldrb r3, [r5, r2]
	adds r3, #18
	cmp r11, r3
	bne .L_08159e76
	cmp r2, #0
	bne .L_08159e60
	movs r0, #134
	bl Func_081180e8
	b .L_08159e66
.L_08159e60:
	movs r0, #134
	bl Audio_PlayCue
.L_08159e66:
	ldr r3, [sp, #28]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #168
	adds r2, r3, r4
	movs r3, #4
	str r3, [r2]
	ldr r5, .L_0815a034
.L_08159e76:
	mov r6, r10
	ldrb r3, [r5, r6]
	adds r3, #18
	cmp r11, r3
	blt .L_08159f2a
	lsls r6, r6, #2
	str r6, [sp, #8]
	movs r0, #0
	mov r8, r0
.L_08159e88:
	ldr r3, [sp, #8]
	ldr r1, .L_0815a02c
	add r3, r10
	lsls r3, r3, #2
	add r3, r10
	add r3, r8
	lsls r2, r3, #3
	subs r2, r2, r3
	lsls r2, r2, #2
	adds r7, r2, r1
	mov r0, r8
	movs r1, #5
	bl Math_Mod
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r1, #96
	ldr r0, [r7, #24]
	bl Math_Div
	movs r1, #3
	bl Math_Mod
	ldr r2, .L_0815a038
	adds r5, r5, r0
	lsls r3, r5, #1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #28]
	movs r3, #240
	lsls r3, r3, #4
	adds r1, r2, r1
	adds r3, #60
	adds r1, r1, r3
	ldr r3, .L_0815a03c
	movs r4, #2
	ldrsh r2, [r7, r4]
	ldrb r6, [r3, r5]
	lsrs r3, r6, #1
	subs r2, r2, r3
	movs r0, #6
	ldrsh r3, [r7, r0]
	ldr r0, .L_0815a040
	ldrb r4, [r0, r5]
	str r6, [sp, #0]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #24]
	ldr r4, [sp, #20]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	lsls r2, r2, #7
	adds r0, r7, #0
	movs r1, #64
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r7, #24]
	ldr r2, [r7, #8]
	adds r3, r3, r2
	str r3, [r7, #24]
	cmp r2, #1
	ble .L_08159f14
	movs r3, #1
	mov r6, r11
	ands r3, r6
	cmp r3, #0
	beq .L_08159f14
	subs r3, r2, #1
	str r3, [r7, #8]
.L_08159f14:
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #21
	bne .L_08159e88
	ldr r5, .L_0815a034
	mov r2, r10
	ldrb r3, [r5, r2]
	adds r3, #18
	cmp r11, r3
	bge .L_08159f82
.L_08159f2a:
	mov r4, r10
	ldrb r3, [r5, r4]
	cmp r11, r3
	blt .L_08159f56
	ldr r3, .L_0815a030
	mov r0, r9
	ldrsb r2, [r3, r4]
	ldr r4, [sp, #28]
	movs r6, #6
	ldrsh r3, [r0, r6]
	movs r1, #34
	movs r6, #224
	str r1, [sp, #0]
	lsls r6, r6, #3
	movs r1, #62
	str r1, [sp, #4]
	adds r2, #47
	adds r1, r4, r6
	ldr r0, [sp, #24]
	ldr r4, [sp, #20]
	mov lr, r4
	.2byte 0xf800
.L_08159f56:
	mov r6, r9
	ldr r3, [r6, #4]
	ldr r2, [r6, #16]
	mov r0, r10
	adds r3, r3, r2
	str r3, [r6, #4]
	ldrb r3, [r5, r0]
	cmp r11, r3
	ble .L_08159f70
	movs r1, #128
	lsls r1, r1, #9
	adds r3, r2, r1
	str r3, [r6, #16]
.L_08159f70:
	mov r2, r9
	ldr r3, [r2, #4]
	movs r2, #200
	lsls r2, r2, #14
	cmp r3, r2
	ble .L_08159f80
	mov r3, r9
	str r2, [r3, #4]
.L_08159f80:
	ldr r5, .L_0815a034
.L_08159f82:
	mov r4, r10
	ldrb r3, [r5, r4]
	adds r3, #18
	cmp r11, r3
	bne .L_08159fba
	ldr r0, [sp, #32]
	movs r6, #0
	ldr r3, [r0, #20]
	mov r8, r6
	cmp r3, #0
	beq .L_08159fba
	movs r6, #8
	movs r5, #36
.L_08159f9c:
	ldr r1, [sp, #32]
	mov r3, r8
	ldrsh r0, [r5, r1]
	movs r1, #7
	movs r2, #5
	str r6, [sp, #0]
	bl Func_0814cd48
	ldr r4, [sp, #32]
	movs r3, #1
	add r8, r3
	ldr r3, [r4, #20]
	adds r5, #2
	cmp r8, r3
	bne .L_08159f9c
.L_08159fba:
	ldr r1, [sp, #16]
	movs r0, #1
	movs r6, #28
	add r10, r0
	add r9, r6
	cmp r10, r1
	beq .L_08159fca
	b .L_08159e48
.L_08159fca:
	movs r1, #4
	movs r0, #2
	bl Func_08158ce0
	bl Func_081434f8
	movs r4, #240
	ldr r3, [sp, #28]
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_0815a034
	ldr r2, [sp, #12]
	movs r6, #1
	ldrb r3, [r1, r2]
	add r11, r6
	adds r3, #65
	cmp r11, r3
	beq .L_08159ffc
	b .L_08159e2a
.L_08159ffc:
	ldr r0, .L_0815a01c
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #60
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0815a01c:
	.4byte Func_08143000
.L_0815a020:
	.4byte Data_081985b0
.L_0815a024:
	.4byte 0xffc00000
.L_0815a028:
	.4byte Data_08198566
.L_0815a02c:
	.4byte gMapCellBuffer
.L_0815a030:
	.4byte Data_081985a6
.L_0815a034:
	.4byte Data_081985ab
.L_0815a038:
	.4byte Data_081974bc
.L_0815a03c:
	.4byte Data_0819749e
.L_0815a040:
	.4byte Data_081974ad
