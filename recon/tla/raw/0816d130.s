.syntax unified
	.thumb
	.global Func_0816d130
	.thumb_func
Func_0816d130:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #64
	str r0, [sp, #36]
	movs r6, #192
	lsls r6, r6, #18
	ldr r0, [r6, #96]
	ldr r7, [r6, #92]
	str r0, [sp, #32]
	movs r0, #1
	ldr r2, [r6, #48]
	str r2, [sp, #24]
	ldr r3, [r6, #100]
	str r3, [sp, #20]
	bl Func_081435e0
	movs r2, #0
	ldr r1, [sp, #20]
	movs r3, #0
	ldr r0, .L_0816d3c4
	bl Func_08157cf4
	ldr r0, .L_0816d3c8
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0816d3cc
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r2, [sp, #36]
	movs r4, #36
	ldrsh r0, [r2, r4]
	bl GetBattleObjectSlotFar
	ldr r4, [sp, #36]
	ldr r5, [r0]
	movs r3, #36
	ldrsh r0, [r4, r3]
	bl Func_08118070
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r3, [r5, #8]
	asrs r0, r0, #1
	str r3, [r7]
	movs r1, #19
	ldr r3, [r5, #12]
	adds r3, r3, r0
	str r3, [r7, #4]
	movs r0, #104
	ldr r3, [r5, #16]
	str r3, [r7, #8]
	movs r3, #0
	str r3, [r7, #12]
	str r3, [r7, #16]
	str r3, [r7, #20]
	str r3, [r7, #24]
	bl Func_081963ec
	ldr r6, [r6, #104]
	movs r0, #239
	lsls r0, r0, #7
	adds r2, r7, r0
	movs r3, #2
	str r6, [sp, #28]
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r7, r3
	movs r1, #200
	movs r3, #50
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0816d3d0
	bl Func_080145a8
	movs r0, #142
	bl Audio_PlayCue
	ldr r0, [sp, #24]
	movs r4, #0
	adds r0, #12
	str r0, [sp, #12]
	mov r9, r4
.L_0816d1ea:
	ldr r2, [sp, #36]
	ldr r0, [r2, #8]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	ldr r3, [sp, #36]
	mov r11, r0
	ldr r0, [r3, #8]
	bl Func_08118070
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	mov r4, r9
	str r0, [sp, #16]
	cmp r4, #64
	bne .L_0816d212
	movs r0, #134
	bl Func_081180e8
.L_0816d212:
	bl Func_08014de4
	ldr r0, [sp, #24]
	ldr r1, [sp, #12]
	bl Func_080156e8
	mov r0, r9
	cmp r0, #72
	bne .L_0816d238
	ldr r2, [sp, #36]
	movs r3, #0
	ldr r0, [r2, #8]
	movs r2, #1
	str r3, [sp, #0]
	movs r1, #7
	negs r2, r2
	subs r3, #1
	bl Func_0814cd48
.L_0816d238:
	mov r4, r9
	cmp r4, #79
	bne .L_0816d252
	ldr r2, [sp, #36]
	movs r3, #0
	ldr r0, [r2, #8]
	movs r2, #1
	str r3, [sp, #0]
	movs r1, #0
	negs r2, r2
	subs r3, #1
	bl Func_0814cd48
.L_0816d252:
	mov r4, r9
	cmp r4, #0
	bne .L_0816d26c
	ldr r3, [sp, #36]
	movs r1, #7
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #42
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
.L_0816d26c:
	ldr r3, [r7, #24]
	cmp r3, #0
	beq .L_0816d274
	b .L_0816d382
.L_0816d274:
	mov r2, r9
	lsls r2, r2, #10
	mov r0, r9
	str r2, [sp, #8]
	movs r4, #0
	lsls r0, r0, #9
	mov r8, r4
	mov r10, r0
	add r6, sp, #52
	add r5, sp, #40
.L_0816d288:
	bl Func_08014e38
	ldr r0, [r7]
	ldr r1, [r7, #4]
	ldr r2, [r7, #8]
	bl Func_08015160
	mov r3, r8
	cmp r3, #11
	bgt .L_0816d2a4
	mov r0, r10
	bl Func_08015024
	b .L_0816d2ae
.L_0816d2a4:
	movs r0, #128
	lsls r0, r0, #7
	add r0, r10
	bl Func_08015024
.L_0816d2ae:
	movs r3, #168
	lsls r3, r3, #5
	adds r3, #85
	ldr r4, [sp, #8]
	mov r0, r8
	muls r0, r3
	adds r0, r0, r4
	bl Func_080150e4
	movs r3, #154
	adds r3, #255
	mov r0, r9
	muls r0, r3
	bl Trig_Sin
	adds r1, r0, #0
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #0
	bl Func_08015160
	movs r0, #0
	adds r1, r5, #0
	str r0, [r6]
	str r0, [r6, #4]
	str r0, [r6, #8]
	adds r0, r6, #0
	bl Func_0815e1ec
	ldr r3, [r5]
	asrs r3, r3, #1
	str r3, [r5]
	bl Func_08014ea8
	ldr r2, .L_0816d3d4
	movs r3, #12
	subs r3, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #20]
	movs r3, #3
	adds r1, r2, r1
	ldr r2, [r5]
	movs r4, #6
	subs r2, r2, r3
	ldr r3, [r5, #4]
	movs r0, #12
	str r4, [sp, #0]
	str r0, [sp, #4]
	subs r3, #6
	ldr r0, [sp, #32]
	ldr r4, [sp, #28]
	mov lr, r4
	.2byte 0xf800
	movs r0, #1
	add r8, r0
	mov r2, r8
	cmp r2, #24
	bne .L_0816d288
	adds r0, r7, #0
	movs r1, #62
	movs r2, #0
	bl BattleFxKernels_IntegrateVector3
	mov r3, r9
	cmp r3, #0
	ble .L_0816d382
	mov r3, r11
	ldr r2, [r3, #12]
	ldr r3, [sp, #16]
	mov r4, r11
	adds r2, r2, r3
	ldr r3, [r7, #4]
	ldr r0, [r4, #8]
	subs r2, r2, r3
	mov r3, r11
	ldr r1, [r3, #16]
	ldr r3, [r7, #8]
	ldr r4, [r7]
	subs r1, r1, r3
	ldr r3, [r7, #12]
	subs r0, r0, r4
	asrs r0, r0, #10
	adds r3, r3, r0
	str r3, [r7, #12]
	ldr r3, [r7, #16]
	asrs r2, r2, #10
	adds r3, r3, r2
	str r3, [r7, #16]
	ldr r3, [r7, #20]
	asrs r1, r1, #10
	adds r3, r3, r1
	str r3, [r7, #20]
	mov r2, r11
	ldr r0, [r2, #8]
	cmp r0, #0
	bge .L_0816d370
	negs r0, r0
.L_0816d370:
	adds r3, r4, #0
	cmp r3, #0
	bge .L_0816d378
	negs r3, r3
.L_0816d378:
	cmp r0, r3
	bge .L_0816d382
	movs r3, #1
	negs r3, r3
	str r3, [r7, #24]
.L_0816d382:
	bl Func_081434f8
	movs r4, #240
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r7, r4
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r0, #1
	add r9, r0
	mov r2, r9
	cmp r2, #80
	beq .L_0816d3a4
	b .L_0816d1ea
.L_0816d3a4:
	ldr r0, .L_0816d3d0
	bl Func_08014644
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
	.2byte 0x0000
.L_0816d3c4:
	.4byte 0x00000134
.L_0816d3c8:
	.4byte 0x0000017f
.L_0816d3cc:
	.4byte IwramCopyWords
.L_0816d3d0:
	.4byte Func_08143000
.L_0816d3d4:
	.4byte Data_08197410
