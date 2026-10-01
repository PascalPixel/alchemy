.syntax unified
	.thumb
	.global Func_08157638
	.thumb_func
Func_08157638:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #96]
	sub sp, #40
	mov r11, r0
	ldr r0, [r3, #92]
	str r1, [sp, #28]
	mov r9, r0
	ldr r3, [r3, #100]
	movs r0, #0
	str r3, [sp, #20]
	bl BattleFx_BeginCanvasLayer
	mov r2, sp
	adds r2, #32
	adds r1, r2, #0
	movs r0, #0
	str r2, [sp, #16]
	bl Func_08144aac
	ldr r0, .L_081579b0
	ldr r1, [sp, #20]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r5, .L_081579b4
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_081579b8
	add r1, r9
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	adds r1, r5, #0
	ldr r0, .L_081579bc
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r1, #144
	lsls r1, r1, #4
	adds r1, #168
	adds r0, r5, #0
	add r1, r9
	movs r2, #17
	movs r3, #104
	bl Graphics_TransposeCopy
	movs r3, #221
	movs r1, #128
	lsls r3, r3, #3
	lsls r1, r1, #5
	adds r5, r5, r3
	adds r1, #144
	add r1, r9
	adds r0, r5, #0
	movs r2, #34
	movs r3, #65
	bl Graphics_TransposeCopy
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
	ldr r0, .L_081579c0
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	movs r7, #0
	movs r2, #1
	mov r3, r9
	mov r10, r7
	negs r2, r2
	adds r3, #24
.L_081576ee:
	movs r0, #1
	add r10, r0
	mov r1, r10
	str r2, [r3]
	adds r3, #28
	cmp r1, #8
	bne .L_081576ee
	movs r2, #0
	ldr r3, .L_081579c4
	mov r10, r2
	movs r1, #1
	movs r2, #128
	negs r1, r1
	lsls r2, r2, #2
.L_0815770a:
	movs r7, #1
	add r10, r7
	str r1, [r3]
	adds r3, #28
	cmp r10, r2
	bne .L_0815770a
	movs r0, #162
	bl Audio_PlayCue
	movs r0, #0
	str r0, [sp, #24]
.L_08157720:
	ldr r1, [sp, #24]
	cmp r1, #56
	bne .L_0815772c
	movs r0, #133
	bl Func_081180e8
.L_0815772c:
	movs r2, #0
	str r2, [sp, #12]
	mov r10, r2
	mov r8, r9
.L_08157734:
	mov r7, r8
	ldr r3, [r7, #24]
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	beq .L_081577ec
	movs r1, #65
	ldr r2, [r7]
	ldr r3, [r7, #4]
	str r1, [sp, #0]
	movs r1, #34
	str r1, [sp, #4]
	movs r1, #128
	lsls r1, r1, #5
	adds r1, #144
	subs r3, #17
	subs r2, #16
	ldr r4, [sp, #32]
	ldr r0, [sp, #28]
	add r1, r9
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r7]
	subs r3, #12
	str r3, [r7]
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
	cmp r3, #5
	bne .L_081577ec
	movs r0, #133
	bl Audio_PlayCue
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r9
	movs r3, #4
	str r3, [r2]
	ldr r1, [sp, #12]
	ldr r2, .L_081579b4
	movs r4, #0
	adds r7, r1, r2
.L_0815778a:
	str r4, [sp, #8]
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r6, r0, #0
	ands r6, r3
	bl Random16
	movs r5, #128
	lsls r5, r5, #1
	adds r5, #255
	movs r3, #128
	ands r5, r0
	lsls r3, r3, #1
	mov r0, r8
	adds r5, r5, r3
	ldr r3, [r0]
	lsls r3, r3, #16
	str r3, [r7]
	ldr r3, [r0, #4]
	adds r0, r6, #0
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #8
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #16]
	bl Random16
	ldr r4, [sp, #8]
	movs r3, #15
	ands r3, r0
	adds r3, #32
	adds r4, #1
	str r3, [r7, #24]
	adds r7, #28
	cmp r4, #32
	bne .L_0815778a
.L_081577ec:
	ldr r2, [sp, #12]
	movs r3, #224
	movs r7, #1
	lsls r3, r3, #2
	add r10, r7
	movs r1, #28
	adds r2, r2, r3
	mov r0, r10
	add r8, r1
	str r2, [sp, #12]
	cmp r0, #5
	bne .L_08157734
	ldr r1, [sp, #24]
	cmp r1, #95
	bgt .L_081578fa
	lsls r6, r1, #11
	adds r0, r6, #0
	bl Trig_Sin
	ldr r2, [sp, #24]
	movs r5, #64
	lsls r3, r2, #1
	subs r5, r5, r3
	adds r3, r5, #0
	muls r3, r0
	asrs r7, r3, #17
	movs r3, #96
	adds r3, r3, r7
	adds r0, r6, #0
	mov r8, r3
	bl Trig_Cos
	movs r1, #20
	str r1, [sp, #0]
	adds r3, r5, #0
	muls r3, r0
	movs r1, #34
	str r1, [sp, #4]
	ldr r0, [sp, #16]
	movs r1, #224
	asrs r3, r3, #16
	adds r2, r7, #0
	lsls r1, r1, #3
	adds r6, r3, #0
	adds r2, #86
	adds r3, #43
	ldr r4, [r0, #4]
	add r1, r9
	ldr r0, [sp, #28]
	mov lr, r4
	.2byte 0xf800
	movs r1, #0
	mov r10, r1
	ldr r1, .L_081579c8
	mov r2, r10
	ldrb r3, [r1, r2]
	ldr r0, [sp, #24]
	adds r6, #60
	cmp r0, r3
	bne .L_0815787c
	mov r2, r9
	ldr r3, [r2, #24]
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	bne .L_0815787c
	adds r3, r7, #0
	adds r3, #88
	str r3, [r2]
	mov r1, r8
	mov r3, r10
	b .L_081578f4
.L_0815787c:
	mov r7, r10
	ldrb r3, [r1, r7]
	ldr r0, [sp, #24]
	adds r3, #6
	cmp r0, r3
	bne .L_081578be
	mov r1, r11
	ldr r3, [r1, #20]
	movs r4, #0
	cmp r3, #0
	beq .L_081578be
	movs r5, #36
.L_08157894:
	mov r2, r11
	ldrsh r0, [r5, r2]
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #7
	adds r3, r4, #0
	movs r2, #5
	mov r7, r11
	str r4, [sp, #8]
	bl Func_0814cd48
	ldrsh r0, [r5, r7]
	movs r1, #6
	bl Func_08118088
	ldr r4, [sp, #8]
	ldr r3, [r7, #20]
	adds r4, #1
	adds r5, #2
	cmp r4, r3
	bne .L_08157894
.L_081578be:
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #5
	beq .L_081578fa
	ldr r3, .L_081579c8
	mov r7, r10
	adds r1, r3, #0
	ldrb r3, [r1, r7]
	ldr r0, [sp, #24]
	cmp r0, r3
	bne .L_0815787c
	lsls r3, r7, #3
	subs r3, r3, r7
	lsls r3, r3, #2
	mov r7, r9
	adds r2, r7, r3
	ldr r3, [r2, #24]
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	bne .L_0815787c
	mov r3, r8
	subs r3, #8
	str r3, [r2]
	mov r1, r8
	movs r3, #0
.L_081578f4:
	str r1, [r2, #12]
	str r6, [r2, #4]
	str r3, [r2, #24]
.L_081578fa:
	ldr r5, .L_081579b4
	ldr r6, .L_081579cc
	movs r2, #0
	mov r10, r2
.L_08157902:
	ldr r0, [r5, #24]
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_08157952
	cmp r0, #0
	bge .L_08157912
	adds r0, #15
.L_08157912:
	asrs r0, r0, #4
	adds r0, #2
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r7, [sp, #20]
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
	ldr r0, [sp, #16]
	ldr r4, [r0, #4]
	ldr r0, [sp, #28]
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
.L_08157952:
	movs r1, #1
	movs r2, #128
	add r10, r1
	lsls r2, r2, #1
	adds r5, #28
	cmp r10, r2
	bne .L_08157902
	movs r0, #4
	movs r1, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	movs r3, #1
	add r2, r9
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #24]
	adds r3, #1
	str r3, [sp, #24]
	cmp r3, #96
	beq .L_0815798a
	b .L_08157720
.L_0815798a:
	ldr r0, .L_081579c0
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081579b0:
	.4byte 0x00000134
.L_081579b4:
	.4byte gMapCellBuffer
.L_081579b8:
	.4byte 0x00000155
.L_081579bc:
	.4byte 0x00000130
.L_081579c0:
	.4byte Func_08143000
.L_081579c4:
	.4byte Data_02010018
.L_081579c8:
	.4byte Data_0819850e
.L_081579cc:
	.4byte Data_08197410
