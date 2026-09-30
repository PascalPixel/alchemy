.syntax unified
	.thumb
	.global Func_0816d724
	.thumb_func
Func_0816d724:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #72
	str r0, [sp, #36]
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #96]
	ldr r0, [r3, #92]
	str r2, [sp, #32]
	mov r11, r0
	ldr r3, [r3, #100]
	movs r0, #1
	str r3, [sp, #28]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0816d78c
	movs r2, #128
	lsls r2, r2, #19
	movs r1, #224
	adds r2, #82
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_0816d790
	add r1, r11
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r1, #182
	lsls r1, r1, #4
	ldr r0, .L_0816d794
	add r1, r11
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r2, #0
	ldr r1, [sp, #28]
	movs r3, #0
	ldr r0, .L_0816d798
	bl Func_08157cf4
	ldr r0, .L_0816d79c
	bl Resource_GetTableEntry
	b .L_0816d7a0
	.2byte 0x0000
.L_0816d78c:
	.4byte 0x00000c10
.L_0816d790:
	.4byte 0x00000113
.L_0816d794:
	.4byte 0x00000184
.L_0816d798:
	.4byte 0x00000134
.L_0816d79c:
	.4byte 0x00000148
.L_0816d7a0:
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0816da0c
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
	movs r3, #75
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0816da10
	bl Scheduler_AddOrUpdateCallback
	ldr r4, [sp, #36]
	movs r3, #36
	ldrsh r0, [r4, r3]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	ldr r3, [sp, #36]
	str r0, [sp, #24]
	mov r4, sp
	adds r4, #60
	movs r2, #36
	ldrsh r0, [r3, r2]
	adds r1, r4, #0
	str r4, [sp, #20]
	bl Func_0815e21c
	ldr r0, [sp, #20]
	ldr r3, [r0]
	lsrs r2, r3, #31
	adds r3, r3, r2
	movs r2, #238
	lsls r2, r2, #7
	asrs r3, r3, #1
	adds r2, #168
	str r3, [r0]
	add r2, r11
	movs r3, #8
	str r3, [r2]
	mov r3, sp
	adds r3, #40
	str r3, [sp, #12]
	movs r2, #0
	mov r9, r2
.L_0816d810:
	mov r4, r9
	cmp r4, #0
	bne .L_0816d8f6
	ldr r0, [sp, #24]
	movs r2, #0
	str r4, [r0, #12]
	mov r8, r2
	mov r7, r11
.L_0816d820:
	bl Random16
	movs r6, #255
	movs r3, #128
	lsls r3, r3, #1
	ands r6, r0
	adds r6, r6, r3
	bl Random16
	movs r5, #254
	lsls r5, r5, #7
	adds r5, #255
	ands r5, r0
	ldr r0, [sp, #20]
	movs r4, #128
	ldr r3, [r0]
	lsls r4, r4, #7
	lsls r3, r3, #16
	str r3, [r7]
	adds r5, r5, r4
	bl Random16
	movs r2, #31
	ands r2, r0
	movs r3, #96
	subs r3, r3, r2
	lsls r3, r3, #16
	str r3, [r7, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #9
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	movs r2, #1
	ands r3, r0
	add r8, r2
	str r3, [r7, #24]
	mov r3, r8
	adds r7, #28
	cmp r3, #16
	bne .L_0816d820
	movs r7, #224
	movs r4, #0
	lsls r7, r7, #2
	mov r8, r4
	add r7, r11
.L_0816d894:
	bl Random16
	movs r5, #127
	ands r5, r0
	bl Random16
	movs r3, #254
	adds r6, r0, #0
	ldr r0, [sp, #24]
	lsls r3, r3, #7
	adds r3, #255
	ands r6, r3
	ldr r3, [r0, #8]
	adds r5, #128
	str r3, [r7]
	movs r3, #208
	lsls r3, r3, #14
	str r3, [r7, #4]
	ldr r3, [r0, #16]
	str r3, [r7, #8]
	bl Random16
	movs r3, #255
	ands r3, r0
	subs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #16]
	adds r0, r6, #0
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	movs r2, #1
	asrs r3, r3, #7
	str r3, [r7, #20]
	add r8, r2
	movs r3, #0
	str r3, [r7, #24]
	mov r3, r8
	adds r7, #28
	cmp r3, #32
	bne .L_0816d894
.L_0816d8f6:
	mov r4, r9
	cmp r4, #4
	bne .L_0816d902
	movs r0, #212
	bl Audio_PlayCue
.L_0816d902:
	mov r0, r9
	cmp r0, #10
	bne .L_0816d93a
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r11
	movs r3, #8
	str r3, [r2]
	movs r0, #144
	bl Audio_PlayCue
	ldr r3, [sp, #36]
	movs r1, #7
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #80
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	ldr r4, [sp, #24]
	movs r3, #0
	str r3, [r4, #72]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r4, #12]
.L_0816d93a:
	mov r0, r9
	cmp r0, #80
	bne .L_0816d94a
	movs r3, #171
	ldr r2, [sp, #24]
	lsls r3, r3, #8
	adds r3, #133
	str r3, [r2, #72]
.L_0816d94a:
	mov r3, r9
	cmp r3, #24
	bne .L_0816d964
	ldr r4, [sp, #36]
	movs r3, #0
	movs r1, #1
	ldr r0, [r4, #8]
	negs r1, r1
	str r3, [sp, #0]
	movs r2, #3
	subs r3, #1
	bl Func_0814cd48
.L_0816d964:
	mov r0, r9
	cmp r0, #40
	bne .L_0816d970
	movs r0, #142
	bl Audio_PlayCue
.L_0816d970:
	mov r2, r9
	cmp r2, #32
	bne .L_0816d97c
	movs r0, #134
	bl Func_081180e8
.L_0816d97c:
	mov r3, r9
	cmp r3, #64
	bne .L_0816d996
	ldr r4, [sp, #36]
	movs r3, #0
	movs r2, #1
	ldr r0, [r4, #8]
	movs r1, #7
	str r3, [sp, #0]
	negs r2, r2
	subs r3, #1
	bl Func_0814cd48
.L_0816d996:
	mov r0, r9
	cmp r0, #88
	bne .L_0816d9b0
	ldr r2, [sp, #36]
	movs r3, #0
	ldr r0, [r2, #8]
	movs r2, #1
	str r3, [sp, #0]
	movs r1, #0
	negs r2, r2
	subs r3, #1
	bl Func_0814cd48
.L_0816d9b0:
	mov r3, r9
	cmp r3, #7
	ble .L_0816da56
	ldr r4, [sp, #20]
	ldr r1, [r4]
	cmp r3, #79
	bgt .L_0816d9ca
	lsls r3, r3, #1
	add r3, r9
	lsls r3, r3, #2
	adds r5, r3, #0
	subs r5, #96
	b .L_0816d9d8
.L_0816d9ca:
	mov r0, r9
	lsls r3, r0, #1
	add r3, r9
	movs r2, #140
	lsls r3, r3, #1
	lsls r2, r2, #2
	subs r5, r2, r3
.L_0816d9d8:
	cmp r5, #0
	ble .L_0816da56
	cmp r5, #80
	ble .L_0816d9e6
	movs r5, #80
	movs r7, #1
	b .L_0816d9e8
.L_0816d9e6:
	movs r7, #0
.L_0816d9e8:
	movs r2, #0
	adds r6, r1, #0
	movs r3, #104
	mov r8, r2
	subs r6, #14
	mov r10, r3
.L_0816d9f4:
	mov r4, r8
	cmp r4, #0
	bne .L_0816da14
	movs r0, #104
	movs r1, #7
	movs r2, #7
	movs r3, #3
	str r7, [sp, #0]
	bl Func_08196404
	b .L_0816da22
	.2byte 0x0000
.L_0816da0c:
	.4byte IwramCopyWords
.L_0816da10:
	.4byte Func_08143000
.L_0816da14:
	movs r0, #104
	movs r1, #7
	movs r2, #7
	movs r3, #7
	str r7, [sp, #0]
	bl Func_08196404
.L_0816da22:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r1, #224
	mov r12, r3
	str r3, [sp, #40]
	mov r4, r10
	movs r3, #14
	lsls r1, r1, #3
	adds r2, r6, #0
	str r3, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #32]
	add r1, r11
	subs r3, r4, r5
	mov lr, r12
	.2byte 0xf800
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r0, #1
	add r8, r0
	mov r2, r8
	adds r6, #14
	cmp r2, #2
	bne .L_0816d9f4
.L_0816da56:
	ldr r3, [sp, #36]
	ldr r1, [sp, #12]
	ldr r0, [r3, #4]
	bl Func_08144aac
	movs r4, #0
	mov r8, r4
	mov r5, r11
.L_0816da66:
	mov r0, r8
	lsrs r3, r0, #31
	add r3, r8
	asrs r3, r3, #1
	adds r3, #8
	cmp r9, r3
	blt .L_0816dad6
	ldr r0, [r5, #24]
	cmp r0, #28
	bgt .L_0816dad6
	movs r2, #3
	movs r1, #3
	str r2, [sp, #8]
	bl Math_Div
	movs r3, #2
	ldrsh r6, [r5, r3]
	movs r4, #6
	ldrsh r7, [r5, r4]
	ldr r2, [sp, #8]
	cmp r0, #6
	ble .L_0816da94
	movs r0, #6
.L_0816da94:
	mov r3, r8
	ands r3, r2
	movs r4, #4
	cmp r3, #0
	beq .L_0816daa0
	movs r4, #0
.L_0816daa0:
	ldr r3, .L_0816dc3c
	lsls r2, r0, #1
	ldrh r1, [r3, r2]
	ldr r3, .L_0816dc40
	movs r0, #182
	lsls r0, r0, #4
	add r1, r11
	adds r1, r1, r0
	ldrh r0, [r3, r2]
	lsrs r3, r0, #1
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r0, [sp, #12]
	subs r2, r6, r3
	ldr r4, [r4, r0]
	subs r3, r7, r3
	ldr r0, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5, #24]
	adds r0, r5, #0
	adds r3, #1
	str r3, [r5, #24]
	movs r1, #62
	ldr r2, .L_0816dc44
	bl BattleFxKernels_IntegrateVector2
.L_0816dad6:
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r5, #28
	cmp r3, #16
	bne .L_0816da66
	mov r4, r9
	cmp r4, #31
	ble .L_0816dbde
	ldr r2, [sp, #36]
	movs r6, #224
	ldr r0, [r2, #8]
	bl GetBattleObjectSlotFar
	ldr r3, [sp, #36]
	ldr r7, [r0]
	ldr r0, [r3, #8]
	bl Func_08118070
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	str r0, [sp, #16]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #48]
	bl Func_08014de4
	adds r1, r5, #0
	adds r0, r5, #0
	adds r1, #12
	bl Func_080156e8
	movs r0, #48
	movs r4, #0
	add r0, sp
	lsls r6, r6, #2
	mov r8, r4
	mov r10, r0
	add r6, r11
.L_0816db26:
	ldr r3, [r6, #24]
	cmp r3, #0
	blt .L_0816dbd2
	mov r2, r8
	mov r1, r10
	movs r5, #1
	adds r0, r6, #0
	ands r5, r2
	bl Func_0815e1ec
	adds r5, #9
	ldr r2, .L_0816dc48
	lsls r0, r5, #1
	subs r3, r0, #2
	mov r4, r10
	ldrh r1, [r2, r3]
	ldr r2, [r4]
	ldr r3, [sp, #28]
	asrs r2, r2, #1
	adds r1, r3, r1
	lsrs r3, r5, #1
	subs r2, r2, r3
	ldr r3, [r4, #4]
	str r0, [sp, #4]
	subs r3, r3, r5
	str r5, [sp, #0]
	ldr r4, [sp, #40]
	ldr r0, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	adds r0, r6, #0
	movs r1, #62
	movs r2, #0
	bl BattleFxKernels_IntegrateVector3
	mov r3, r8
	adds r3, #32
	cmp r9, r3
	ble .L_0816dbd2
	ldr r3, [sp, #16]
	ldr r2, [r7, #12]
	ldr r1, [r7, #16]
	adds r2, r2, r3
	ldr r3, [r6, #4]
	ldr r0, [r7, #8]
	subs r2, r2, r3
	ldr r3, [r6, #8]
	ldr r4, [r6]
	subs r1, r1, r3
	ldr r3, [r6, #12]
	subs r0, r0, r4
	asrs r0, r0, #7
	adds r3, r3, r0
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	asrs r2, r2, #7
	adds r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #20]
	asrs r1, r1, #7
	adds r3, r3, r1
	adds r1, r4, #0
	str r3, [r6, #20]
	cmp r1, #0
	bge .L_0816dbaa
	negs r1, r1
.L_0816dbaa:
	ldr r3, [r7, #8]
	adds r2, r3, #0
	cmp r3, #0
	bge .L_0816dbb4
	negs r2, r3
.L_0816dbb4:
	cmp r1, r2
	blt .L_0816dbd2
	cmp r3, #0
	bge .L_0816dbc4
	lsrs r3, r4, #31
	cmp r3, #0
	bne .L_0816dbcc
	b .L_0816dbd2
.L_0816dbc4:
	mvns r3, r4
	lsrs r3, r3, #31
	cmp r3, #0
	beq .L_0816dbd2
.L_0816dbcc:
	movs r4, #1
	negs r4, r4
	str r4, [r6, #24]
.L_0816dbd2:
	movs r0, #1
	add r8, r0
	mov r2, r8
	adds r6, #28
	cmp r2, #32
	bne .L_0816db26
.L_0816dbde:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	mov r3, r9
	cmp r3, #7
	bgt .L_0816dbfa
	movs r0, #4
	movs r1, #4
	bl Func_08158ce0
	b .L_0816dc02
.L_0816dbfa:
	movs r0, #16
	movs r1, #16
	bl Func_08158ce0
.L_0816dc02:
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r11
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	movs r4, #1
	add r9, r4
	mov r0, r9
	cmp r0, #106
	beq .L_0816dc24
	b .L_0816d810
.L_0816dc24:
	ldr r0, .L_0816dc4c
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #72
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0816dc3c:
	.4byte Data_08198b5e
.L_0816dc40:
	.4byte Data_08198b6c
.L_0816dc44:
	.4byte 0xffffe000
.L_0816dc48:
	.4byte Data_08197410
.L_0816dc4c:
	.4byte Func_08143000
