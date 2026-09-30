.syntax unified
	.thumb
	.global Func_08157d30
	.thumb_func
Func_08157d30:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r5, #192
	lsls r5, r5, #18
	ldr r1, [r5, #96]
	sub sp, #44
	mov r11, r0
	ldr r0, [r5, #92]
	str r1, [sp, #40]
	mov r9, r0
	ldr r2, [r5, #100]
	movs r0, #0
	str r2, [sp, #24]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_08157d98
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	movs r1, #27
	movs r0, #104
	bl Func_081963ec
	movs r1, #3
	movs r0, #188
	bl Func_081963ec
	ldr r3, [r5, #104]
	adds r5, #188
	ldr r5, [r5]
	str r3, [sp, #28]
	ldr r1, [sp, #24]
	ldr r0, .L_08157d9c
	movs r2, #0
	movs r3, #0
	str r5, [sp, #32]
	bl Func_08157cf4
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_08157da0
	add r1, r9
	movs r2, #1
	movs r3, #1
	b .L_08157da4
	.2byte 0x0000
.L_08157d98:
	.4byte 0x00001010
.L_08157d9c:
	.4byte 0x00000134
.L_08157da0:
	.4byte 0x00000157
.L_08157da4:
	bl Func_08157cf4
	movs r1, #144
	lsls r1, r1, #4
	adds r1, #248
	ldr r0, .L_08158054
	add r1, r9
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r9
	movs r3, #75
	movs r1, #200
	str r3, [r2]
	ldr r0, .L_08158058
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	movs r4, #0
	mov r10, r4
	movs r7, #63
	mov r5, r9
	movs r6, #104
.L_08157de4:
	bl Random16
	ands r0, r7
	str r0, [r5]
	movs r0, #1
	add r10, r0
	mov r1, r10
	str r6, [r5, #4]
	adds r5, #28
	cmp r1, #32
	bne .L_08157de4
	movs r2, #0
	ldr r3, .L_0815805c
	mov r10, r2
	movs r1, #1
	movs r2, #128
	negs r1, r1
	lsls r2, r2, #2
.L_08157e08:
	movs r4, #1
	add r10, r4
	str r1, [r3]
	adds r3, #28
	cmp r10, r2
	bne .L_08157e08
	movs r0, #141
	bl Audio_PlayCue
	movs r0, #128
	movs r7, #0
	lsls r0, r0, #8
	str r7, [sp, #36]
	str r0, [sp, #16]
.L_08157e24:
	ldr r1, [sp, #36]
	cmp r1, #79
	bgt .L_08157e66
	ldr r0, [sp, #16]
	bl Trig_Sin
	lsls r5, r0, #1
	adds r5, r5, r0
	ldr r0, [sp, #16]
	bl Trig_Cos
	ldr r3, [sp, #36]
	lsls r5, r5, #3
	lsls r2, r3, #1
	movs r3, #64
	subs r3, r3, r2
	muls r3, r0
	movs r2, #20
	asrs r5, r5, #16
	movs r1, #224
	asrs r3, r3, #16
	adds r5, #22
	str r2, [sp, #0]
	lsls r1, r1, #3
	movs r2, #38
	str r2, [sp, #4]
	adds r3, #29
	ldr r0, [sp, #40]
	add r1, r9
	adds r2, r5, #0
	ldr r4, [sp, #32]
	mov lr, r4
	.2byte 0xf800
.L_08157e66:
	ldr r7, [sp, #36]
	cmp r7, #56
	bne .L_08157e72
	movs r0, #133
	bl Func_081180e8
.L_08157e72:
	movs r0, #0
	movs r1, #16
	str r1, [sp, #20]
	str r0, [sp, #12]
	mov r10, r0
	mov r8, r9
.L_08157e7e:
	ldr r2, [sp, #36]
	ldr r3, [sp, #20]
	cmp r2, r3
	blt .L_08157f7a
	mov r4, r8
	movs r1, #34
	ldr r2, [r4]
	ldr r3, [r4, #4]
	str r1, [sp, #0]
	movs r1, #65
	str r1, [sp, #4]
	movs r1, #135
	lsls r1, r1, #5
	add r1, r9
	subs r2, #17
	subs r3, #32
	ldr r0, [sp, #40]
	ldr r7, [sp, #28]
	mov lr, r7
	.2byte 0xf800
	ldr r0, [sp, #36]
	ldr r1, [sp, #20]
	cmp r0, r1
	bne .L_08157f72
	ldr r2, [sp, #12]
	ldr r3, .L_08158060
	movs r4, #0
	adds r7, r2, r3
.L_08157eb6:
	str r4, [sp, #8]
	bl Random16
	movs r6, #254
	lsls r6, r6, #7
	adds r6, #255
	ands r6, r0
	movs r0, #128
	lsls r0, r0, #7
	adds r6, r6, r0
	bl Random16
	mov r2, r8
	ldr r3, [r2]
	movs r5, #128
	lsls r3, r3, #16
	str r3, [r7]
	lsls r5, r5, #1
	ldr r3, [r2, #4]
	adds r5, #255
	adds r3, #16
	lsls r3, r3, #16
	movs r1, #128
	lsls r1, r1, #1
	str r3, [r7, #4]
	ands r5, r0
	adds r0, r6, #0
	adds r5, r5, r1
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl Random16
	ldr r4, [sp, #8]
	movs r3, #15
	ands r3, r0
	adds r3, #32
	adds r4, #1
	str r3, [r7, #24]
	adds r7, #28
	cmp r4, #16
	bne .L_08157eb6
	movs r3, #1
	mov r4, r10
	ands r3, r4
	cmp r3, #0
	beq .L_08157f2e
	movs r0, #133
	bl Audio_PlayCue
.L_08157f2e:
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	add r3, r9
	movs r2, #4
	str r2, [r3]
	mov r7, r11
	ldr r3, [r7, #20]
	movs r4, #0
	cmp r3, #0
	beq .L_08157f72
	movs r5, #36
.L_08157f46:
	mov r1, r11
	movs r3, #6
	ldrsh r0, [r5, r1]
	str r3, [sp, #0]
	movs r1, #7
	adds r3, r4, #0
	movs r2, #5
	str r4, [sp, #8]
	bl Func_0814cd48
	mov r3, r11
	ldrsh r0, [r5, r3]
	movs r1, #6
	bl Func_08118088
	mov r0, r11
	ldr r4, [sp, #8]
	ldr r3, [r0, #20]
	adds r4, #1
	adds r5, #2
	cmp r4, r3
	bne .L_08157f46
.L_08157f72:
	mov r1, r8
	ldr r3, [r1, #4]
	subs r3, #12
	str r3, [r1, #4]
.L_08157f7a:
	ldr r2, [sp, #20]
	ldr r4, [sp, #12]
	movs r7, #224
	movs r0, #1
	lsls r7, r7, #2
	add r10, r0
	adds r2, #4
	movs r3, #28
	adds r4, r4, r7
	mov r1, r10
	str r2, [sp, #20]
	add r8, r3
	str r4, [sp, #12]
	cmp r1, #10
	beq .L_08157f9a
	b .L_08157e7e
.L_08157f9a:
	ldr r5, .L_08158060
	ldr r6, .L_08158064
	movs r2, #0
	mov r10, r2
.L_08157fa2:
	ldr r0, [r5, #24]
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_08157ff0
	cmp r0, #0
	bge .L_08157fb2
	adds r0, #15
.L_08157fb2:
	asrs r0, r0, #4
	adds r0, #2
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r7, [sp, #24]
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	adds r1, r7, r1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #40]
	ldr r4, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	adds r0, r5, #0
	movs r1, #62
	lsls r2, r2, #6
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_08157ff0:
	movs r7, #1
	movs r0, #128
	add r10, r7
	lsls r0, r0, #2
	adds r5, #28
	cmp r10, r0
	bne .L_08157fa2
	movs r1, #4
	movs r0, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r9
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, .L_08158068
	ldr r1, [sp, #16]
	ldr r3, [sp, #36]
	adds r1, r1, r2
	adds r3, #1
	str r1, [sp, #16]
	str r3, [sp, #36]
	cmp r3, #96
	beq .L_08158030
	b .L_08157e24
.L_08158030:
	ldr r0, .L_08158058
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08158054:
	.4byte 0x00000130
.L_08158058:
	.4byte Func_08143000
.L_0815805c:
	.4byte Data_02010018
.L_08158060:
	.4byte gMapCellBuffer
.L_08158064:
	.4byte Data_08197410
.L_08158068:
	.4byte 0xfffff800
